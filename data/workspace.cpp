#include <QPainter>
#include "workspace.h"
#include "linenumberarea.h"

WorkSpace::WorkSpace(QWidget *parent) : QPlainTextEdit(parent){

    lineNumberArea = new LineNumberArea(this);
    keyF9 = new QShortcut(this);
    keyF9->setKey(Qt::Key_F9);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(keyF9, SIGNAL(activated()), this, SLOT(stop_point()));
    updateLineNumberAreaWidth(0);

    createActions();
}

void WorkSpace::createActions() {
    addStopPoint = new QAction(tr("Добавить точку останова"), this);
    deleteStopPoint = new QAction(tr("Удалить точку останова"), this);

#if QT_CONFIG(shortcut)
    addStopPoint->setShortcut(QCoreApplication::translate("MainWindow", "F9", nullptr));
#endif

#if QT_CONFIG(shortcut)
    deleteStopPoint->setShortcut(QCoreApplication::translate("MainWindow", "F9", nullptr));
#endif

    connect(addStopPoint, SIGNAL(triggered()), SLOT(add_stop_point()));
    connect(deleteStopPoint, SIGNAL(triggered()), SLOT(delete_stop_point()));
}

void WorkSpace::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = createStandardContextMenu();
    stopLine.clear();
    for (int i = 0; i < lines.size(); i++)
        stopLine.push_back(lines.at(i).cursor.blockNumber());
    if (!stopLine.contains(textCursor().blockNumber()))
        menu->addAction(addStopPoint);
    if (stopLine.contains(textCursor().blockNumber()))
        menu->addAction(deleteStopPoint);

    menu->exec(event->globalPos());
    delete menu;
}

int WorkSpace::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }
    int space = 7 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
    return space;
}

void WorkSpace::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), color);
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + blockBoundingRect(block).height();
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignCenter, number);
        }
        block = block.next();
        top = bottom;
        bottom = top + blockBoundingRect(block).height();
        ++blockNumber;
    }
}

void WorkSpace::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);
    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void WorkSpace::updateLineNumberAreaWidth(int)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void WorkSpace::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void WorkSpace::add_stop_point() {
    QList<QTextEdit::ExtraSelection> extraSelections;

    selection.format.setBackground(stopColor);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = textCursor();

    lines.push_back(selection);
    for(int i = 0; i < lines.size(); i++){
        extraSelections.append(lines.at(i));
        setExtraSelections(extraSelections);
    }
}

void WorkSpace::delete_stop_point() {
    QList<QTextEdit::ExtraSelection> extraSelections;

    selection.format.setBackground(stopColor);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = textCursor();

    if(stopLine.contains(textCursor().blockNumber())) {
        if (lines.size() == 1) {
            QColor lineColor = QColor(QColor(stopColor)).lighter(255);
            selection.format.setBackground(lineColor);
            selection.format.setProperty(QTextFormat::FullWidthSelection, true);
            selection.cursor = textCursor();
            extraSelections.append(selection);
            setExtraSelections(extraSelections);
        }

        lines.remove(stopLine.indexOf(textCursor().blockNumber()));
        stopLine.remove(stopLine.indexOf(textCursor().blockNumber()));

        if (!lines.isEmpty())
            for(int i = 0; i < lines.size(); i++) {
                extraSelections.append(lines.at(i));
                setExtraSelections(extraSelections);
            }
    }
}

void WorkSpace::stop_point()
{
    stopLine.clear();
    for (int i = 0; i < lines.size(); i++)
        stopLine.push_back(lines.at(i).cursor.blockNumber());
    if (!stopLine.contains(textCursor().blockNumber()))
        add_stop_point();
    if (stopLine.contains(textCursor().blockNumber()))
        delete_stop_point();
}

void WorkSpace::paint()
{
    QList<QTextEdit::ExtraSelection> extraSelections;
    for(int i = 0; i < lines.size(); i++){
        extraSelections.append(lines.at(i));
        setExtraSelections(extraSelections);
    }
    if (lines.isEmpty()) {
        QColor lineColor = QColor(QColor(stopColor)).lighter(255);
        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        extraSelections.append(selection);
        setExtraSelections(extraSelections);
    }
}

void WorkSpace::light_line(int line)
{
    QList<QTextEdit::ExtraSelection> extraSelections;
    QTextEdit::ExtraSelection select_ion;
    QTextCursor cursor = textCursor();
    select_ion.format.setBackground(currentLine);
    select_ion.format.setProperty(QTextFormat::FullWidthSelection, true);
    if (textCursor().blockNumber() < line){
        int down = line - textCursor().blockNumber();
        cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, down);
    }
    if (textCursor().blockNumber() > line){
        int up = textCursor().blockNumber() - line;
        cursor.movePosition(QTextCursor::Up, QTextCursor::MoveAnchor, up);
    }
    this->setTextCursor(cursor);
    select_ion.cursor = textCursor();
    extraSelections.append(select_ion);
    setExtraSelections(extraSelections);
}

QWidget* WorkSpace::getlineNumberArea()
{
    return lineNumberArea;
}
