#include "prompttosave.h"
#include "ui_prompttosave.h"

PromptToSave::PromptToSave(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PromptToSave)
{
    ui->setupUi(this);

    connect(ui->SaveFile, SIGNAL(clicked(bool)), SIGNAL(save_file()));
}

PromptToSave::~PromptToSave()
{
    delete ui;
}

void PromptToSave::on_Cancel_clicked()
{
    this->close();
    emit show_mainwindow();
}

void PromptToSave::on_DontSave_clicked()
{
    this->close();
}
