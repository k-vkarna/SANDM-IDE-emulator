#include "memorymonitor.h"
#include <QHeaderView>

MemoryMonitor::MemoryMonitor(QWidget *parent) : QTableWidget(parent)
{
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->setEditTriggers(nullptr);
    this->setColumnCount(3);
    this->setHorizontalHeaderLabels({"Page data", "Memory page", "Register"});
}

void MemoryMonitor::add_data(int reg, int page, uchar data)
{
    this->insertRow(this->rowCount());
    QTableWidgetItem *registr = new QTableWidgetItem(tr("%1").arg(reg));
    QTableWidgetItem *mem_page = new QTableWidgetItem(tr("%1").arg(page));
    QTableWidgetItem *mem_data = new QTableWidgetItem(tr("%1").arg(data));

    this->setItem(0, 1, mem_page);
    this->setItem(0, 2, registr);

    for(int i = this->rowCount() - 1; i < this->rowCount(); i++) {

        QTableWidgetItem *id = new QTableWidgetItem(tr("%1").arg(i));
        this->setVerticalHeaderItem(i, id);

        for(int j = 0; j < this->columnCount(); j++) {

            if (j == 0)
                this->setItem(i, j, mem_data);
        }
    }
}

void MemoryMonitor::add_empty_data(QString message, int number)
{
    this->insertRow(this->rowCount());
    int page_id = number;
    QTableWidgetItem *mess = new QTableWidgetItem(tr("%1").arg(message));
    QTableWidgetItem *page = new QTableWidgetItem(tr("%1").arg(number));
    QTableWidgetItem *id = new QTableWidgetItem(tr("%1").arg(page_id));

    for(int i = this->rowCount() - 1; i < this->rowCount(); i++) {

        this->setVerticalHeaderItem(i, id);

        for(int j = 0; j < this->columnCount(); j++) {
            if (j == 1)
                this->setItem(i, 1, page);
            if (j == 0)
                this->setItem(i, 0, mess);
        }
    }
}

void MemoryMonitor::clear_data()
{
    this->clearContents();
    this->setRowCount(0);
}
