#ifndef MEMORYMONITOR_H
#define MEMORYMONITOR_H

#include <QTableWidget>

class MemoryMonitor : public QTableWidget
{
    Q_OBJECT

public:
    explicit MemoryMonitor(QWidget *parent = nullptr);
    void add_data(int, int, uchar);
    void add_empty_data(QString, int);
    void clear_data();
};

#endif // MEMORYMONITOR_H
