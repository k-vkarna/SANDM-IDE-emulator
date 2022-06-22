#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "conio.h"

#include "SANDM/tokens.h"

#include "mainwindow.h"

#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QtCore/QCoreApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->Stop->setEnabled(false);
    ui->Continue->setEnabled(false);
    ui->Step->setEnabled(false);
    ui->Stop_Debug->setEnabled(false);

    workSpace = new WorkSpace;
    console = new Console;
    m_highLight = new HighLight(workSpace->document());
    prompt_to_save_window = new PromptToSave();
    memoryMonitor = new MemoryMonitor();

    thread = new QThread(this);
    compiler = new Compiler();
    vm = new VirtualMachine();

    vm->moveToThread(thread);

    workSpace->setFocus();
    workSpace->setWordWrapMode(QTextOption::NoWrap);

    QFont font;
    font.setPointSize(14);
    memoryMonitor->setFont(font);
    workSpace->setFont(font);
    console->setFont(font);

    horizontal = new QSplitter(Qt::Horizontal, this);
    horizontal->addWidget(workSpace);
    horizontal->addWidget(memoryMonitor);
    horizontal->setSizes({400, 200});
    horizontal->widget(1)->hide();

    vertical = new QSplitter(Qt::Vertical, this);
    vertical->addWidget(horizontal);
    vertical->addWidget(console);
    vertical->setSizes({250, 100});

    setCentralWidget(vertical);

    connect(this, SIGNAL(destroyed()), thread, SLOT(quit()));

    connect(prompt_to_save_window, &PromptToSave::show_mainwindow, this, &MainWindow::show);
    connect(prompt_to_save_window, SIGNAL(save_file()), this, SLOT(save_file_from_promt()));

    connect(workSpace, SIGNAL(textChanged()), this, SLOT(isModified()));

    connect(vm, SIGNAL(print(QString)), this, SLOT(output(QString)));
    connect(vm, SIGNAL(finished(int)), this, SLOT(check_finish(int)));
    connect(vm, &VirtualMachine::waitForInput, this, &MainWindow::waiting_for_input);
    connect(vm, &VirtualMachine::waitForContinue, this, &MainWindow::show_stopLine);

    connect(this, &MainWindow::startVirtualMachine, vm, &VirtualMachine::execute);
    connect(this, SIGNAL(startDebug(QString)), vm, SLOT(debugStarted(QString)));
    connect(this, SIGNAL(exitProgramm()), vm, SLOT(exitButton()));
    connect(ui->Step, SIGNAL(triggered()), vm, SLOT(nextStep()));
    connect(ui->Continue, SIGNAL(triggered()), vm, SLOT(onContinue()));
    connect(console, SIGNAL(onCommand(QString)), vm, SLOT(onCommand(QString)));

    on_Create_triggered();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::saved_File_or_not(){

    if (!documentModified)
        return true;
    else
        return false;
}

void MainWindow::on_Create_triggered()
{
    filepath = "";
    defaultFilename = "New project";
    currentFile = 1;
    QFile file(defaultFilename + ".asm");
    if (file.exists()) {
        while (true){
            currentFile = currentFile + 1;
            QFile filecheck(defaultFilename + QString::number(currentFile) + ".asm");
            if (!filecheck.exists())
                break;
        }
        defaultFilename = defaultFilename + QString::number(currentFile) + ".asm";
    }
    else
        defaultFilename = defaultFilename + ".asm";

    if (!saved_File_or_not()){
        MainWindow *one_more_window = new MainWindow();
        one_more_window->show();
    }
    else{
        if (!workSpace->lines.isEmpty()) {
            workSpace->lines.clear();
            workSpace->paint();
        }
        workSpace->clear();
        this->setWindowTitle(defaultFilename + windowName);
        documentModified = false;
    }
}

void MainWindow::on_Open_triggered()
{
    QString buff = filepath;
    filepath = QFileDialog::getOpenFileName(this, "Открыть", filepath, filter);
    QFile file(filepath);
    if (!file.exists()){
        ui->statusbar->showMessage("Файл не открыт", timeout);
        return;
    }
    file.close();

    if (!saved_File_or_not()) {
        MainWindow *one_more_window = new MainWindow();
        one_more_window->add_text_to_workspace(filepath);
        one_more_window->documentModified = false;
        one_more_window->show();
        one_more_window->filepath = filepath;
        filepath = buff;
    }
    else {
        if (!workSpace->lines.isEmpty()) {
            workSpace->lines.clear();
            workSpace->paint();
        }
        add_text_to_workspace(filepath);
        documentModified = false;
    }
}

void MainWindow::on_Save_as_triggered()
{
    filepath = QFileDialog::getSaveFileName(this, "Сохранить как", filepath + defaultFilename, filter);
    QFile file(filepath);
    if (file.exists())
        file.remove();
    if (!file.open(QFile::ReadWrite)){
        ui->statusbar->showMessage("Файл не сохранен", timeout);
        return;
    }
    QTextStream text(&file);
    text << workSpace->toPlainText();
    ui->statusbar->showMessage("Файл сохранен в " + filepath, timeout);
    file.close();
    this->setWindowTitle(filepath + windowName);
    documentModified = false;
}

void MainWindow::on_Exit_triggered()
{
    vm->onButtonExit = true;
    emit exitProgramm();
    reload();
    thread->quit();
    thread->wait();
    if (saved_File_or_not()){
        emit close();
    }
    else {
        prompt_to_save_window->show();
        this->hide();
    }
}

void MainWindow::closeEvent(QCloseEvent *event){
    on_Exit_triggered();
    event->accept();
}

void MainWindow::isModified()
{
    if(!documentModified) {
        documentModified = true;
        updateTitle();
    }
}

void MainWindow::updateTitle() {
    QString windowTitle;
    if (filepath == "")
        windowTitle = defaultFilename + windowName;
    else
        windowTitle = filepath + windowName;
    if(documentModified)
        this->setWindowTitle(QString("%1%2").arg("*").arg(windowTitle));
}

void MainWindow::on_Save_triggered()
{
    QFile file(filepath);
    if(file.open(QFile::ReadWrite)){
        QTextStream text(&file);
        file.close();
        file.remove();

        if (!file.open(QFile::ReadWrite)){
            ui->statusbar->showMessage("Файл не сохранен", timeout);
            return;
        }

        text << workSpace->toPlainText();
        file.close();
        ui->statusbar->showMessage("Файл сохранен в " + filepath, timeout);
        this->setWindowTitle(filepath + windowName);
        documentModified = false;
    }
    else
        on_Save_as_triggered();
}

void MainWindow::save_file_from_promt()
{
    on_Save_triggered();
    prompt_to_save_window->close();
}

void MainWindow::add_text_to_workspace(QString fileLocation)
{
    QFile file(fileLocation);
    file.open(QFile::ReadOnly);
    ui->statusbar->showMessage("Файл открыт", timeout);
    QTextStream text(&file);
    QString buff = text.readAll();
    file.flush();
    file.close();
    workSpace->setPlainText(buff);
    this->setWindowTitle(fileLocation + windowName);
}

void MainWindow::on_Start_triggered()
{
    if (documentModified)
        on_Save_triggered();

    if (workSpace->toPlainText() == "") {
        error(PS_FILE_ERROR, 0);
        return;
    }

    if (!filepath.contains(".asm")){
        error(PS_FILE_ERROR, 0);
        on_Stop_triggered();
        return;
    }

    QString buff;
    buff = filepath;
    if (!QDir::setCurrent(buff.remove(buff.lastIndexOf("/"), buff.size()))) {
        ui->statusbar->showMessage("Не удалось запустить выполнение программы", timeout);
        return;
    }

    if (debugging){
        reIndexPoints(filepath);
    }

    QString filename;
    buff = filepath;
    filename = buff.remove(0, buff.lastIndexOf("/") + 1);

    thread->start();

    if (debugging) {
        ui->Start->setEnabled(false);
        ui->Debug->setEnabled(false);
        ui->Continue->setEnabled(true);
        ui->Stop_Debug->setEnabled(true);
        ui->Step->setEnabled(true);
        ui->Stop->setEnabled(false);
    }
    else {
        ui->Start->setEnabled(false);
        ui->Debug->setEnabled(false);
        ui->Continue->setEnabled(false);
        ui->Stop_Debug->setEnabled(false);
        ui->Step->setEnabled(false);
        ui->Stop->setEnabled(true);
    }

    ui->Create->setEnabled(false);
    ui->Open->setEnabled(false);
    ui->Save->setEnabled(false);
    ui->Save_as->setEnabled(false);
    ui->Close->setEnabled(false);

    start(filename);

    if (!QDir::setCurrent(currentDir)) {
        ui->statusbar->showMessage("Не удалось вернуться", timeout);
        return;
    }
}

void MainWindow::output(QString result) {
    console->output(result);
}

//обработка ошибок
void MainWindow::error(const int err, const int numLine)
{
    reload();
    //версия программы
    const QString Version = "0.0.4";

    switch (err) {
    case PS_FILE_ERROR:
        console->output("ERROR: File I/O");
        break;
    case PS_ERROR:
        console->output("ERROR: Wrong syntax at line: " + QString::number(numLine));
        break;
    case PS_INPUT_TIMEOUT:
        console->output("ERROR: Input timeout");
        break;
    case PS_PROGRAMM_EXIT:
        console->output("Programm stopped");
        break;
    default:
        console->output("SANDM version " + Version);
        //        console->output("Usage: sandm -(p|b) <file>");
        //        console->output("\t--plain or -p for read plain text");
        //        console->output("\t--byte or -b for read byte-code");
    }
}

//точка входа в программу
void MainWindow::start(QString filename)
{
    std::string fname = filename.toStdString();
    char* buff = new char [fname.size()+1];
    strcpy(buff, fname.c_str());
    try {
        int res = compiler->Open(buff);
        if (res) {
            error(res, compiler->NumLine());
            return;
        }
        if (compiler->CountCmd() == 0)
            error(PS_FILE_ERROR, 0);
        emit startVirtualMachine(compiler->CountCmd(), compiler->GetByteCode());
    } catch (QString err) {
        qDebug() << err;
    }
}

void MainWindow::reIndexPoints(QString fileLocation)
{    
    QFile file(fileLocation);
    file.open(QFile::ReadOnly);

    QTextStream text(&file);

    QString buff;
    int count = 0;

    wholetext_index.clear();
    row_index.clear();

    for (int i = 0; i < workSpace->blockCount(); i++) {
        buff = text.readLine();
        if (buff.contains(";")){
            buff.truncate(buff.indexOf(";") - 1);

        }
        if (!buff.isEmpty()){
            count++;
            row_index.push_back(i);
        }
    }
    file.close();

    QVector<int> temporaryPoints;

    if (workSpace->lines.size() != 0)
        for (int j = 0; j < workSpace->lines.size(); j++)
            temporaryPoints.push_back(workSpace->lines.at(j).cursor.blockNumber());
    else
        temporaryPoints.push_back(0);

    file.open(QFile::ReadOnly);
    QTextStream data(&file);


    for (int i = 1; i <= count; i++) {
        buff = data.readLine();
        if (buff.isEmpty()) {
            for (int j = 0; j < temporaryPoints.size(); j++) {
                if (temporaryPoints[j] >= i) {
                    temporaryPoints[j]--;
                }
            }
            i--;
        }
        if (buff.contains(";")){
            buff.truncate(buff.indexOf(";") - 1);
            if (buff.isEmpty()) {
                for (int j = 0; j < temporaryPoints.size(); j++) {
                    if (temporaryPoints[j] >= i) {
                        temporaryPoints[j]--;
                    }
                }
                i--;
            }
        }
        if (!buff.isEmpty()) {
            wholetext_index.push_back(i-1);
        }
    }

    file.close();

    QString points;
    points.append(" " + QString::number(temporaryPoints.at(0)) + " ");
    for (int i = 1; i < temporaryPoints.size(); i++) {
        if (temporaryPoints[i] != temporaryPoints[i - 1]) {
            points.append(QString::number(temporaryPoints.at(i)));
            points.append(" ");
        }
    }
    emit startDebug(points);
}

void MainWindow::reload()
{
    ui->Start->setEnabled(true);
    ui->Debug->setEnabled(true);
    compiler->cntCmd = 0;
    compiler->byte_code.resize(0);
    compiler->numLine = 0;
    if (debugging) {
        ui->Continue->setEnabled(false);
        ui->Step->setEnabled(false);
        ui->Stop_Debug->setEnabled(false);
        horizontal->widget(1)->hide();
        ui->statusbar->showMessage("Отладка программы завершена");
        memoryMonitor->clear_data();
        debugging = false;
        workSpace->paint();
    }
    else {
        ui->statusbar->showMessage("Выполнение программы завершено");
        ui->Stop->setEnabled(false);
    }

    ui->Create->setEnabled(true);
    ui->Open->setEnabled(true);
    ui->Save->setEnabled(true);
    ui->Save_as->setEnabled(true);
    ui->Close->setEnabled(true);
}

void MainWindow::on_Stop_triggered()
{
    vm->onButtonExit = true;
    emit exitProgramm();
    ui->Start->setEnabled(true);
    ui->Debug->setEnabled(true);
    ui->Stop->setEnabled(false);
    thread->quit();
    thread->wait();
}

void MainWindow::check_finish(int res)
{
    if(res) error(res, 0);
    else reload();
    thread->quit();
    thread->wait();
}

void MainWindow::waiting_for_input()
{
    console->isLocked = false;
    console->setFocus();
    ui->statusbar->showMessage("Программа ожидает введения данных");
}

void MainWindow::on_Debug_triggered()
{
    debugging = true;
    step_forward = true;
    horizontal->widget(1)->show();

    on_Start_triggered();
}

void MainWindow::on_Stop_Debug_triggered()
{
    vm->onButtonExit = true;
    emit exitProgramm();
    ui->Start->setEnabled(true);
    ui->Debug->setEnabled(true);
    ui->Continue->setEnabled(false);
    ui->Step->setEnabled(false);
    ui->Stop_Debug->setEnabled(false);
    memoryMonitor->clear_data();
    horizontal->widget(1)->hide();
    memoryMonitor->clear_data();
    thread->quit();
    thread->wait();
}

void MainWindow::show_stopLine(QString line, QString reg)
{
    int registr = reg.toInt();

    memoryMonitor->clear_data();

    for (int i = 0; i < 256; i++ ){
        if (vm->m_memory_map[i] == nullptr)
            memoryMonitor->add_empty_data("null", i);
        else
            for (int j = 0; j < 256; j++){
                memoryMonitor->add_data(registr, i, vm->m_memory_map[i][j]);
            }
    }

    int id = row_index[wholetext_index.indexOf(line.toInt())];

    ui->statusbar->showMessage("Выполнение программы приостановлено на " + QString::number(id+1) + " строке");
    workSpace->light_line(id);

    step_forward = false;
}

void MainWindow::on_Continue_triggered()
{
    step_forward = true;
}

void MainWindow::on_Close_triggered()
{
    if (documentModified)
        on_Save_triggered();

    if (!documentModified)
        on_Create_triggered();
}
