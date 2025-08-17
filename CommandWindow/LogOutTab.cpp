#include "LogOutTab.h"

#include <KactusAPI/KactusColors.h>

#include <QMenu>
#include <QTimer>
#include <QDateTime>
#include <QFontDatabase>
#include <QTextEdit>
#include <QVBoxLayout>

LogOutTab::LogOutTab(QWidget *parent) : QWidget{parent}
{
    textEdit = new QTextEdit(this);
    textEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // 윈도우의 폭과 높이를 모두 사용

    textEdit->setReadOnly(true);
    textEdit->setStyleSheet("background-color: black; color: white;");
    textEdit->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(textEdit);
    layout->setContentsMargins(0, 0, 0, 0); // Set all margins to 0 pixels
    layout->setSpacing(1); // Reduce the space between widgets to 2 pixels

    setLayout(layout);

    LogPrint(">start console\n");
}

//-----------------------------------------------------------------------------
// Function: LogOutTab::onErrorMessage()
//-----------------------------------------------------------------------------
void LogOutTab::onErrorMessage(QString const& message)
{
//    if (message.isEmpty())
//    {
//        return;
//    }

//    moveCursor(QTextCursor::End);

//    // errors are printed in red color
//    setTextColor(KactusColors::ERROR);
//    appendPlainText(message);
}

//-----------------------------------------------------------------------------
// Function: LogOutTab::onNoticeMessage()
//-----------------------------------------------------------------------------
void LogOutTab::onNoticeMessage(QString const& message)
{
//    if (message.isEmpty())
//    {
//        return;
//    }

//    moveCursor(QTextCursor::End);

//    // notices are printed in blue
//    setTextColor(KactusColors::REGULAR_MESSAGE);
//    appendPlainText(message);

//    viewport()->update();
}

//-----------------------------------------------------------------------------
// Function: LogOutTab::contextMenuEvent()
//-----------------------------------------------------------------------------
void LogOutTab::contextMenuEvent(QContextMenuEvent* event)
{
//    QMenu menu;
//    menu.addAction(&copyAction_);
//    menu.addAction(&selectAllAction_);
//    menu.addAction(&clearAction_);

//    menu.exec(event->globalPos());
}

//-----------------------------------------------------------------------------
// Function: LogOutTab::setTextColor()
//-----------------------------------------------------------------------------
void LogOutTab::setTextColor(QColor const& color)
{
//    QTextCharFormat format = currentCharFormat();
//    format.setForeground(QBrush(color));
//    setCurrentCharFormat(format);
}

void LogOutTab::LogPrint(const QString &log, bool isErr)
{
    // Convert newline characters to HTML line breaks
    QString htmlLog = log.toHtmlEscaped().replace("\n", "<br>");

    QTextCursor cursor = textEdit->textCursor();
    cursor.movePosition(QTextCursor::End);

    if (isErr) {
        cursor.insertHtml("<font color='red'>" + htmlLog + "</font><br>");
    } else {
        cursor.insertHtml("<font color='white'>" + htmlLog + "</font><br>");
    }

    // Get number of lines
    int numLines = textEdit->document()->blockCount();

    // Remove old lines if number of lines exceeds 20000
    if (numLines > 2000000) {
        QTextCursor removeCursor(textEdit->document());
        removeCursor.setPosition(0);
        removeCursor.select(QTextCursor::BlockUnderCursor);
        removeCursor.removeSelectedText();
        removeCursor.deletePreviousChar();  // delete previous '\n'
    }

    // Ensure scrollbar is at the bottom
    cursor.movePosition(QTextCursor::End);
    textEdit->setTextCursor(cursor);
}

// ...
