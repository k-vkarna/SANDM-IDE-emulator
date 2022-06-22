#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QSplitter>
#include <QThread>
#include "highlight.h"
#include "prompttosave.h"
#include "ui_mainwindow.h"
#include "workspace.h"
#include "console.h"
#include "memorymonitor.h"
#include "SANDM/Compiler.h"
#include "SANDM/virtualmachine.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    const QString windowName = " - SANDM эмулятор";
    const QString filter = "Все файлы (*) ;; Исходный код ассемблера (*.asm)";

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_Open_triggered();
    void on_Save_as_triggered();
    void on_Exit_triggered();
    void on_Create_triggered();
    void on_Save_triggered();
    void save_file_from_promt();
    void add_text_to_workspace(QString fileLocation);
    void isModified();
    void updateTitle();

    bool saved_File_or_not();

    void on_Start_triggered();
    void on_Stop_triggered();
    void output(QString s);

    void check_finish(int);
    void waiting_for_input();

    void on_Debug_triggered();

    void on_Stop_Debug_triggered();
    void show_stopLine(QString, QString);

    void on_Continue_triggered();

    void on_Close_triggered();

private:
    Ui::MainWindow *ui;
    HighLight *m_highLight;
    QString defaultFilename;
    QString filepath;
    QString dirName = "Projects";
    const QString currentDir = QDir::currentPath();
    int timeout = 2500;
    bool documentModified;
    bool debugging;
    bool step_forward;
    int currentFile;
    PromptToSave *prompt_to_save_window;
    WorkSpace *workSpace;
    Console *console;
    Compiler *compiler;
    MemoryMonitor *memoryMonitor;
    VirtualMachine *vm;
    QSplitter *horizontal;
    QSplitter *vertical;
    QThread *thread;
    QVector<int> wholetext_index;
    QVector<int> row_index;

    void error(const int, const int);
    void start(QString);
    void reIndexPoints(QString);
    void reload();

signals:
    void startVirtualMachine(ushort linecount, const char *byte_code);
    void startDebug(QString);
    void exitProgramm();

};
#endif // MAINWINDOW_H
