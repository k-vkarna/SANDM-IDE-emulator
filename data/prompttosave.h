#ifndef PROMPTTOSAVE_H
#define PROMPTTOSAVE_H

#include <QWidget>
#include <QAbstractButton>

namespace Ui {
class PromptToSave;
}

class PromptToSave : public QWidget
{
    Q_OBJECT

public:
    explicit PromptToSave(QWidget *parent = nullptr);
    ~PromptToSave();

signals:
    void show_mainwindow();
    void save_file();

private slots:
    void on_Cancel_clicked();
    void on_DontSave_clicked();

private:
    Ui::PromptToSave *ui;
};

#endif // PROMPTTOSAVE_H
