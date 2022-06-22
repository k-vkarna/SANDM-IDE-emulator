#include "highlight.h"
#include <QTextCharFormat>
#include <QColor>

HighLight::HighLight(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighLightRule rule;

    instuctionFormat.setForeground(Qt::blue);
    instuctionFormat.setFontWeight(QFont::Bold);

    QStringList keywordInstructions;
    keywordInstructions << "\\badd\\b" << "\\bdiv\\b" << "\\bjgz\\b" << "\\bjmp\\b"
                        << "\\bjnz\\b" << "\\bload\\b" << "\\bmod\\b" << "\\bmul\\b"
                        << "\\bnop\\b" << "\\bpage\\b" << "\\bread\\b" << "\\bsave\\b"
                        << "\\bset\\b" << "\\bsub\\b" << "\\bwrite\\b";

    for(const QString &pattern : keywordInstructions){
        rule.pattern = QRegExp(pattern);
        rule.format = instuctionFormat;
        instructionRule.append(rule);
    }

    startComment = QRegExp(";");
    commentFormat.setForeground(Qt::darkGreen);

    modPointer = QRegExp("&");
    modPointerFormat.setForeground(QColor("#e8b716"));

    modTypeFormat.setForeground(QColor("#16e1e8"));

    QStringList keywordModTypes;
    keywordModTypes << "\\bc\\b" << "\\bb\\b" << "\\bw\\b" << "\\bd\\b";

    for(const QString &pattern : keywordModTypes){
        rule.pattern = QRegExp(pattern);
        rule.format = modTypeFormat;
        modTypeRule.append(rule);
    }
}

void HighLight::highlightBlock(const QString &text)
{
    setFormat(0, text.length(), defaultformat);

    int indexComment = startComment.indexIn(text);
    while (indexComment >= 0){
        int lenght = text.length() - startComment.matchedLength() + 1;
        setFormat(indexComment, lenght, commentFormat);
        indexComment = startComment.indexIn(text, indexComment + lenght);
    }

    for (const HighLightRule &rule : instructionRule){
        QRegExp expression (rule.pattern);
        int indexInstruction = expression.indexIn(text);
        while (indexInstruction >= 0){
            int lenght = expression.matchedLength();
            setFormat(indexInstruction, lenght, rule.format);
            indexInstruction = expression.indexIn(text, indexInstruction + lenght);
        }
    }

    int indexModPointer = modPointer.indexIn(text);
    while (indexModPointer >= 0){
        int lenght = modPointer.matchedLength();
        setFormat(indexModPointer, lenght, modPointerFormat);
        indexModPointer = modPointer.indexIn(text, indexModPointer + lenght);
    }

    for (const HighLightRule &rule : modTypeRule){
        QRegExp expression (rule.pattern);
        int indexModType = expression.indexIn(text);
        while (indexModType >= 0){
            int lenght = expression.matchedLength();
            setFormat(indexModType, lenght, rule.format);
            indexModType = expression.indexIn(text, indexModType + lenght);
        }
    }
}
