#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QEventLoop>
#include <QTimer>

#include "virtualmachine.h"
#include "byte_code.h"
#include "tokens.h"

//количество команд S&M
const int ComCount = 15;
VirtualMachine::VirtualMachine()
{
    action coms[ComCount] = {
        &VirtualMachine::com_set,
        &VirtualMachine::com_save,
        &VirtualMachine::com_load,
        &VirtualMachine::com_page,
        &VirtualMachine::com_read,
        &VirtualMachine::com_write,
        &VirtualMachine::com_add,
        &VirtualMachine::com_sub,
        &VirtualMachine::com_mul,
        &VirtualMachine::com_div,
        &VirtualMachine::com_mod,
        &VirtualMachine::com_jmp,
        &VirtualMachine::com_jnz,
        &VirtualMachine::com_jgz,
        &VirtualMachine::com_nop
    };
    commands = new action [ComCount];
    for (int i=0; i<ComCount; i++) {
        commands[i] = coms[i];
    }
    for (int i=0; i<256; i++) {
        m_memory_map[i] = 0;
    }
}

VirtualMachine::~VirtualMachine()
{

}

void VirtualMachine::com_add()
{
    m_reg += m_arg;
    m_reg &= m_regmask;
    m_ip++;
}

void VirtualMachine::com_set()
{
    m_reg = m_arg;
    m_reg &= m_regmask;
    m_ip++;
}

void VirtualMachine::com_div()
{
    m_reg &= m_regmask;
    m_arg &= m_regmask;
    m_reg = int(m_reg) / int(m_arg);
    m_ip++;
}

void VirtualMachine::com_jmp()
{
    m_ip = m_arg & 0xFFFF;
}

void VirtualMachine::com_jnz()
{
    m_reg &= m_regmask;
    if (m_reg)
        m_ip = m_arg & 0xFFFF;
    else
        m_ip++;
}

void VirtualMachine::com_jgz()
{
    m_reg &= m_regmask;
    if (0 < int(m_reg))
        m_ip = m_arg & 0xFFFF;
    else
        m_ip++;
}

void VirtualMachine::com_load()
{
    m_reg &= 0xFF;
    m_reg = *(uint *)(m_memory_map[m_page] + m_reg);
    m_reg &= m_regmask;
    m_ip++;
}

void VirtualMachine::com_mod()
{
    m_reg &= m_regmask;
    m_arg &= m_regmask;
    m_reg = int(m_reg) % int(m_arg);
    m_ip++;
}

void VirtualMachine::com_mul()
{
    m_reg *= m_arg;
    m_reg &= m_regmask;
    m_ip++;
}

void VirtualMachine::com_nop()
{
    m_ip++;
}

void VirtualMachine::com_page()
{
    m_page = m_arg & m_regmask;
    if (!m_memory_map[m_page])
    {
        init_page(m_page);
    }
    m_ip++;
}

void VirtualMachine::com_read()
{
    if (m_type == TYPE_CHAR)
    {
        QString buff;
        buff.append(QString::number(cmd.at(0).unicode(), 10));
        buff.append(" ");
        cmd.remove(0, 1);
        m_reg = buff.toUInt();
    }
    else
    {
        QString buff = cmd;
        m_reg = buff.toUInt();
        cmd.clear();
    }
    m_reg &= m_regmask;
    m_ip++;
}

void VirtualMachine::com_save()
{
    m_arg &= 0xFF;
    m_reg &= m_regmask;
    uint tmp = *(uint *)(m_memory_map[m_page] + m_arg);
    tmp &= ~m_regmask;
    tmp |= m_reg;
    *(uint *)(m_memory_map[m_page] + m_arg) = tmp;
    m_ip++;
}

void VirtualMachine::com_sub()
{
    m_reg -= int(m_arg);
    m_reg &= m_regmask;
    m_ip++;
}

void VirtualMachine::com_write()
{
    m_reg &= m_regmask;
    if (m_type == TYPE_CHAR)
    {
        charachter = QString::number(m_reg, 16);
        charachter = QByteArray::fromHex(charachter.remove(" ").toLocal8Bit());
    }
    else
    {
        charachter = QString::number(m_reg);
    }
    m_ip++;
}

void VirtualMachine::init_page(uchar page)
{
    m_memory_map[page] = new uchar [259];
    for (int i=0; i<259; i++)
    {
        m_memory_map[page][i] = 0;
    }
}

void VirtualMachine::init()
{
    onButtonExit = false;
    result.clear();
    m_arg = 0;
    m_ip = 0;
    m_page = 0;
    m_reg = 0;
    m_regmask = 0;
    m_type = 0;
    for (int i=0; i<256; i++)
    {
        if(m_memory_map[i])
        {
            free(m_memory_map[i]);
        }
        m_memory_map[i] = 0;
    }
    init_page(0);
}

void VirtualMachine::onCommand(QString s)
{
    cmd = s;
    emit onChange();
}

void VirtualMachine::onContinue()
{
    debug = false;
    emit continueDebug();
}

void VirtualMachine::nextStep()
{
    emit step();
}

void VirtualMachine::debugStarted(QString points)
{
    stop_line.clear();
    QString buff;
    for (int i = 0; i < points.size(); i++) {
        if (points.at(i) != " ") {
            buff.append(points.at(i));
            if (points.at(i + 1) == " ") {
                stop_line.push_back(buff.toUInt());
                buff.clear();
            }
        }
    }
}

void VirtualMachine::exitButton()
{
    emit continueDebug();
    emit onChange();
}

int VirtualMachine::execute_one()
{
    char line[5];
    memcpy(&line, m_code + m_ip*5, 5);
    int command_code = (line[0] & COMMAND_MASK) >> 4;
    if (command_code >= ComCount)
    {
        return PS_ERROR;
    }
    action command = commands[command_code];

    m_arg = *(uint *)(line+1);

    char pointer_mode = line[0] & POINTERMODE_MASK;
    if (pointer_mode == POINTERMODE_ON)
    {
        m_arg &= 0xFF;
        m_arg = *(uint *)(m_memory_map[m_page] + m_arg);
    }

    m_type = line[0] & TYPE_MASK;
    switch (m_type)
    {
    case TYPE_BYTE:
        m_regmask = 0x000000FF;
        break;
    case TYPE_WORD:
        m_regmask = 0x0000FFFF;
        break;
    default:
        m_regmask = 0xFFFFFFFF;
    }

    if (!stop_line.isEmpty()){
        if (stop_line.contains(m_ip)) {
            debug = true;
            QString line = QString::number(m_ip);
            QString reg = QString::number(m_reg);
            emit waitForContinue(line, reg);
            QTimer timer;
            timer.setSingleShot(true);
            QEventLoop loop;
            connect(this, &VirtualMachine::continueDebug, &loop, &QEventLoop::quit);
            connect(this, &VirtualMachine::step, &loop, &QEventLoop::quit);
            connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
            timer.start(timeout_seconds * 1000);
            loop.exec();
        }
        if (onButtonExit) {
            onButtonExit = false;
            return PS_PROGRAMM_EXIT;
        }
    }

    if (debug && !stop_line.contains(m_ip)){
        QString line = QString::number(m_ip);
        QString reg = QString::number(m_reg);
        emit waitForContinue(line, reg);
        QTimer timer;
        timer.setSingleShot(true);
        QEventLoop loop;
        connect(this, &VirtualMachine::continueDebug, &loop, &QEventLoop::quit);
        connect(this, &VirtualMachine::step, &loop, &QEventLoop::quit);
        connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
        timer.start(timeout_seconds * 1000);
        loop.exec();
        if (onButtonExit) {
            onButtonExit = false;
            return PS_PROGRAMM_EXIT;
        }
    }

    if (command_code == 4) {
        if (cmd.isEmpty()) {
            emit waitForInput();
            QTimer timer;
            timer.setSingleShot(true);
            QEventLoop loop;
            connect(this, &VirtualMachine::onChange, &loop, &QEventLoop::quit);
            connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
            timer.start(timeout_seconds * 1000);
            loop.exec();

            if (onButtonExit) {
                onButtonExit = false;
                return PS_PROGRAMM_EXIT;
            }

            if (cmd.isEmpty())
                return PS_INPUT_TIMEOUT;
        }
    }

    (this->*command)();

    if (command_code == 5) {
        result.append(charachter);
    }
    return 0;
}

// цикл выполнения программы
int VirtualMachine::execute(ushort linecount, const char *byte_code)
{
    m_code = byte_code;
    m_code_size = linecount;
    init();
    debug = false;
    while (m_ip < m_code_size)
    {
        int res = execute_one();
        if (res){
            if (!result.isEmpty()) {
                emit print(result);
                result.clear();
            }
            emit finished(res);
            stop_line.clear();
            debug = false;
            return res;
        }
    }
    if (!result.isEmpty()) {
        emit print(result);
        result.clear();
    }
    emit finished(0);
    stop_line.clear();
    debug = false;
    return 0;
}

int VirtualMachine::executeFile(const char *filename)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp) return PS_FILE_ERROR;
    ushort size = 0;
    int tmp;
    while ((tmp = fgetc(fp))!=EOF) size++;
    fflush(fp);
    char *code = new char [size];
    fread(code, sizeof(char), size, fp);
    return execute(size/5, code);
}
