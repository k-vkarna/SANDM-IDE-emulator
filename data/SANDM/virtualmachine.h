#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

#include <QObject>

typedef unsigned char uchar;
typedef unsigned short int ushort;
typedef unsigned int uint;
class VirtualMachine;
typedef void(VirtualMachine::*action)(void);

class VirtualMachine :  public QObject
{
    Q_OBJECT
public:
    VirtualMachine();
    ~VirtualMachine();

    int executeFile(const char *filename);
    int execute(ushort linecount, const char *byte_code);
    uchar *m_memory_map[256];
    bool onButtonExit;
private:
    void init();
    void init_page(uchar page);
    int execute_one();

    void com_add();
    void com_set();
    void com_div();
    void com_jmp();
    void com_jnz();
    void com_jgz();
    void com_load();
    void com_mod();
    void com_mul();
    void com_nop();
    void com_page();
    void com_read();
    void com_save();
    void com_sub();
    void com_write();

    const char *m_code;
    ushort m_code_size;
    uchar m_page;
    uint m_reg;
    ushort m_ip;
    uint m_regmask;
    uint m_arg;
    action *commands;
    char m_type;

    QString charachter;
    QString result;
    QString cmd;
    QVector<uint> stop_line;
    const int timeout_seconds = 300;
    bool debug;

private slots:
    void onCommand(QString);
    void onContinue();
    void nextStep();
    void debugStarted(QString);
    void exitButton();

signals:
    void print(QString);
    void finished(int res);
    void waitForInput();
    void waitForContinue(QString, QString);
    void onChange();
    void continueDebug();
    void step();
};

#endif // VIRTUALMACHINE_H
