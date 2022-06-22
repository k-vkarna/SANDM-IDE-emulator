#ifndef HIGHTLIGHT_H
#define HIGHTLIGHT_H

#include <QSyntaxHighlighter>

QT_BEGIN_NAMESPACE
class QTextDocument;
class QTextCharFormat;
QT_END_NAMESPACE

class HighLight : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    explicit HighLight(QTextDocument *parent = nullptr);

protected:
    void highlightBlock(const QString &text) Q_DECL_OVERRIDE;

private:
    struct HighLightRule {
        QRegExp pattern;
        QTextCharFormat format;
    };

    QTextCharFormat defaultformat;             // Формат по умолчанию

    QVector<HighLightRule> instructionRule;    // Правила форматирования для инструкций
    QVector<HighLightRule> modTypeRule;        // Правила форматирования для модификаторов типа

    QTextCharFormat instuctionFormat;          // Форматирование иструкций

    QRegExp startComment;                      // Символ начала комментария = ";"
    QTextCharFormat commentFormat;             // Форматирование комментариев

    QRegExp modPointer;                        // Символ модификатора указателя = "&"
    QTextCharFormat modPointerFormat;          // Форматирование модификатора указателя

    QTextCharFormat modTypeFormat;             // Форматирование модификаторов типа
};

#endif // HIGHTLIGHT_H
