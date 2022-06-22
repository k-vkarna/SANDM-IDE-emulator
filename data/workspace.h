#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <QPlainTextEdit>
#include <QGridLayout>
#include <QColor>
#include <QShortcut>

class WorkSpace : public QPlainTextEdit{

    Q_OBJECT

public:
    explicit WorkSpace(QWidget *parent = nullptr);
    int lineNumberAreaWidth();
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    QWidget* getlineNumberArea();
    QVector<QTextEdit::ExtraSelection> lines;

protected:
    void resizeEvent(QResizeEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);

public slots:
    void light_line(int);
    void paint();

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(const QRect &, int);
    void add_stop_point();
    void delete_stop_point();
    void stop_point();

private:
    QColor color = "#8bff79";
    QColor stopColor = "#ffd6d6";
    QColor currentLine = "#edff21";
    void createActions();
    QTextEdit::ExtraSelection selection;
    QVector<int> stopLine;
    QWidget *lineNumberArea;
    QAction *addStopPoint;
    QAction *deleteStopPoint;
    QShortcut *keyF9;
};

#endif // WORKSPACE_H
