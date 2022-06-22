#include "advisetosave.h"
#include "ui_advisetosave.h"

AdviseToSave::AdviseToSave(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdviseToSave)
{
    ui->setupUi(this);
}

AdviseToSave::~AdviseToSave()
{
    delete ui;
}
