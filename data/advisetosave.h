#ifndef ADVISETOSAVE_H
#define ADVISETOSAVE_H

#include <QWidget>

namespace Ui {
class AdviseToSave;
}

class AdviseToSave : public QWidget
{
    Q_OBJECT

public:
    explicit AdviseToSave(QWidget *parent = nullptr);
    ~AdviseToSave();

signals:
    void show_mainwindow();

private:
    Ui::AdviseToSave *ui;
};

#endif // ADVISETOSAVE_H
