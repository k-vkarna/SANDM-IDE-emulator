#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H
#include <QWidget>
#include "mainwindow.h"

class LineNumberArea : public QWidget {
public:
    LineNumberArea(WorkSpace *workSpace);
    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    WorkSpace *workSpace;
};

#endif // LINENUMBERAREA_H
