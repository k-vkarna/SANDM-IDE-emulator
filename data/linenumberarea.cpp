#include "linenumberarea.h"

LineNumberArea::LineNumberArea(WorkSpace *workPlace) : QWidget(workPlace){
    workSpace = workPlace;
}

QSize LineNumberArea::sizeHint() const{
    return QSize(workSpace->lineNumberAreaWidth(), 0);
}

void LineNumberArea::paintEvent(QPaintEvent *event){
    workSpace->lineNumberAreaPaintEvent(event);
}
