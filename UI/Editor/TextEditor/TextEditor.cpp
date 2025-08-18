#include "TextEditor.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QScrollBar>
#include <QFontDatabase>
#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QColor>
#include <QTextDocument>
#include <QChar>
#include <QRegularExpression>

#include <UI/Editor/TextEditor/TextEditorManager.h>



TextEditor::TextEditor(QWidget *parent) : TabDocument(parent, DOC_PROTECTION_SUPPORT)
{
    supportedWindows_ |= TabDocument::TEXTEDITORWINDOW;

    whatWindowIsThis_ = TabDocument::TEXTEDITOR;

    // Setup UI
    QHBoxLayout *hBoxLayout = new QHBoxLayout(this);
    hBoxLayout->SetMaximumSize;
    hBoxLayout->setSpacing(1);

    editorLine = new QTextEdit(this);
    editorLine->setFixedWidth(50);
    QSizePolicy editorLinePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
    editorLine->setSizePolicy(editorLinePolicy);
    editorLine->setReadOnly(true);
    editorLine->setTextInteractionFlags(Qt::NoTextInteraction);
    editorLine->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    editorLine->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    editorLine->setStyleSheet("background-color: black; color: white;");
    editorLine->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));

    editorText = new QTextEdit(this);
    QSizePolicy editorTextPolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    editorText->setSizePolicy(editorTextPolicy);
    editorText->setStyleSheet("background-color: black; color: white;");
    editorText->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));

    hBoxLayout->addWidget(editorLine);
    hBoxLayout->addWidget(editorText);

    setLayout(hBoxLayout);

    connect(editorLine->verticalScrollBar(), &QScrollBar::valueChanged, editorText->verticalScrollBar(), &QScrollBar::setValue);
    connect(editorText->verticalScrollBar(), &QScrollBar::valueChanged, editorLine->verticalScrollBar(), &QScrollBar::setValue);
    connect(editorText, &QTextEdit::textChanged, this, &TextEditor::loadFileLine);
    connect(editorText, &QTextEdit::textChanged, this, &TextEditor::syntaxHighlighter);

    fileExtension.clear();
}



TextEditor::~TextEditor()
{

}



void TextEditor::setProtection( bool locked )
{
    TabDocument::setProtection(locked);
}



VLNV TextEditor::getIdentifyingVLNV() const
{
    return getDocumentVLNV();
}



void TextEditor::refresh()
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    bool locked = isProtected();

    setModified(false);

    TabDocument::refresh();

    setProtection(locked);

    QApplication::restoreOverrideCursor();
}



bool TextEditor::save()
{
    QString filePath = getDocumentName();
    saveFile(filePath);

    setModified(false);

    return false;
}



bool TextEditor::saveAs()
{
    setModified(false);

    return true;
}



void TextEditor::loadFile(QString filePath)
{
    QFile file(filePath);
    file.open(QFile::ReadOnly | QFile::Text);

    QTextStream fileText(&file);

    QString fileTexts = fileText.readAll();

    if (fileTexts.endsWith("\n"))
    {
        fileTexts.chop(1);
    }

    editorText->setText(fileTexts);
    loadFileLine();

    file.close();

    QFileInfo fileInfo(file);
    fileExtension = fileInfo.suffix();

    syntaxHighlighter();

    setObjectName(QString(filePath));
    setDocumentName(QString(filePath));
}



void TextEditor::saveFile(QString filePath)
{
    QFile file(filePath);
    file.open(QFile::WriteOnly | QFile::Text);

    QTextStream fileOut(&file);

    QString fileTexts = editorText->toPlainText();
    fileOut << fileTexts;

    file.close();
}



void TextEditor::loadFileLine()
{
    QString lines = "";

    int lineNumber = editorText->document()->blockCount();

    for (int i = 1; i < lineNumber + 1; i++)
    {
        if (i == 1)
        {
            lines += QString::number(i);
        }

        else
        {
            lines += "\n" + QString::number(i);
        }
    }

    editorLine->setText(lines);
}



void TextEditor::syntaxHighlighter()
{
    editorText->blockSignals(true);

    QTextCursor textCursor = editorText->textCursor();
    QTextCharFormat textFormat;
    QList<QStringList> syntaxInfos = TextEditorManager::getInstance().syntaxChecker(fileExtension);
    QTextDocument *textDocu = editorText->document();

    textCursor.select(QTextCursor::Document);

    textFormat.setForeground(QColor("#FFFFFF"));
    textCursor.setCharFormat(textFormat);

    textCursor = editorText->textCursor();

    textCursor.movePosition(QTextCursor::Start);

    for (int syntaxNum = 0; syntaxNum < syntaxInfos.length(); syntaxNum++)
    {
        while (!(textCursor = textDocu->find(QString(syntaxInfos[syntaxNum][0]), textCursor)).isNull())
        {
            int cursorStart = textCursor.selectionStart();
            int cursorEnd = textCursor.selectionEnd();

            QChar textBefore = (cursorStart > 0) ? textDocu->characterAt(cursorStart - 1) : QChar();
            QChar textAfter = (cursorEnd < textDocu->characterCount()) ? textDocu->characterAt(cursorEnd) : QChar();

            bool isBoundaryBefore = textBefore.isNull() || textBefore.isSpace() || textBefore.isPunct();
            bool isBoundaryAfter = textAfter.isNull() || textAfter.isSpace() || textAfter.isPunct();

#ifdef  _WIN32
            if (textBefore == QChar('_') || textAfter == QChar('_'))
#else
            if (textBefore == "_" || textAfter == "_"))
#endif
            {
                isBoundaryBefore = false;
                isBoundaryAfter = false;
            }

            if (textCursor.selectedText() == QString(syntaxInfos[syntaxNum][0]) && isBoundaryBefore && isBoundaryAfter)
            {
                if (QString(syntaxInfos[syntaxNum][1]) == "")
                {
                        textFormat.setForeground(QColor(QString(syntaxInfos[syntaxNum][2])));
                        textCursor.setCharFormat(textFormat);
                }
            }
        }
    }

    areaHighlighter();

    editorText->blockSignals(false);
}



void TextEditor::areaHighlighter()
{
    QString fileTexts = editorText->toPlainText();
    QTextCursor textCursor = editorText->textCursor();
    QTextCharFormat textFormat;

    QList<QStringList> syntaxInfos = TextEditorManager::getInstance().syntaxChecker(fileExtension);

    QRegularExpressionMatchIterator regExpMatchIter;
    QRegularExpressionMatch regExpMatch;

    textCursor.movePosition(QTextCursor::Start);

    for (int syntaxNum = 0; syntaxNum < syntaxInfos.length(); syntaxNum++)
    {
        QString syntax_0 = syntaxInfos[syntaxNum][0];
        QString syntax_1 = syntaxInfos[syntaxNum][1];
        QString syntax_2 = syntaxInfos[syntaxNum][2];

        if (syntax_1 != "")
        {
            if (syntax_1 == "\n")
            {
                QRegularExpression regExp(syntax_0 + ".*");
                regExpMatchIter = regExp.globalMatch(fileTexts);
            }

            else
            {
                QRegularExpression regExp(syntax_0 + ".*?" + syntax_1);
                regExpMatchIter = regExp.globalMatch(fileTexts);
            }

            while (regExpMatchIter.hasNext())
            {
                regExpMatch = regExpMatchIter.next();

                if (regExpMatch.hasMatch())
                {
                    int startMatch = regExpMatch.capturedStart(0);
                    int lengthMatch = regExpMatch.capturedLength(0);

                    textCursor.setPosition(startMatch);
                    textCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, lengthMatch);

                    textFormat.setForeground(QColor(syntax_2));
                    textCursor.setCharFormat(textFormat);
                }
            }
        }
    }
}
