/*
 *      Created on: 18.1.2024
 *      Author: P.H.G
 *      Filename: LogOut.h
 *      Project: AWM
 *      Description :
 *          This widget is used to print event information to the user.
 */

#include "LogOut.h"

#include <QMenu>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QIcon>
#include <QSize>
#include <QFile>
#include <QDateTime>
#include <QTextStream>
#include <QFileDialog>
#include <QColor>
#include <QRegExp>
#include <algorithm>

#include <KactusAPI/KactusColors.h>



LogOut::LogOut(QWidget *parent): copyAction_(tr("Copy"), this), selectAllAction_(tr("Select all"), this), clearAction_(tr("Clear"), this)
{
    QVBoxLayout *vBoxLayout = new QVBoxLayout(this);
    vBoxLayout->SetMaximumSize;
    vBoxLayout->setSpacing(1);

    QHBoxLayout *hBoxLayout = new QHBoxLayout(this);
    hBoxLayout->SetMinimumSize;
    hBoxLayout->setSpacing(1);

    btnClear = new QPushButton(this);
    btnClear->setFixedSize(30, 30);
    btnClear->setIcon(QIcon(":/resources/Res/clear.png"));
    btnClear->setIconSize(QSize(30, 30));

    btnTcl = new QPushButton(this);
    btnTcl->setFixedSize(30, 30);
    btnTcl->setIcon(QIcon(":/resources/Res/tcl.png"));
    btnTcl->setIconSize(QSize(30, 30));

    textEdit = new QTextEdit(this);
    QSizePolicy textEditPolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    textEdit->setSizePolicy(textEditPolicy);
    textEdit->setReadOnly(true);
    textEdit->setUndoRedoEnabled(false);
    textEdit->setStyleSheet("background-color: white; color: black;");
    textEdit->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));

    hBoxLayout->addStretch();
    hBoxLayout->addWidget(btnClear);
    hBoxLayout->addWidget(btnTcl);

    vBoxLayout->addLayout(hBoxLayout);
    vBoxLayout->addWidget(textEdit);

    setLayout(vBoxLayout);

	copyAction_.setDisabled(true);

	connect(&copyAction_, SIGNAL(triggered()), this, SLOT(copy()), Qt::UniqueConnection);
	connect(&selectAllAction_, SIGNAL(triggered()), this, SLOT(selectAll()), Qt::UniqueConnection);
	connect(&clearAction_, SIGNAL(triggered()),	this, SLOT(clear()), Qt::UniqueConnection);
	connect(this, SIGNAL(copyAvailable(bool)), &copyAction_, SLOT(setEnabled(bool)), Qt::UniqueConnection);
    connect(btnClear, SIGNAL(clicked()), this, SLOT(onClearLog()));
    connect(btnTcl, SIGNAL(clicked()), this, SLOT(onCreateTcl()));
}



//-----------------------------------------------------------------------------
// Function: LogOut::onInputMessage()
//-----------------------------------------------------------------------------
void LogOut::onInputMessage(QString const& message)
{
    if (message.isEmpty())
    {
        return;
    }

    appendInput(message);
}



//-----------------------------------------------------------------------------
// Function: LogOut::onNoticeMessage()
//-----------------------------------------------------------------------------
void LogOut::onNoticeMessage(QString const& message)
{
    if (message.isEmpty())
    {
        return;
    }

    appendLog(message);
}



//-----------------------------------------------------------------------------
// Function: LogOut::onErrorMessage()
//-----------------------------------------------------------------------------
void LogOut::onErrorMessage(QString const& message)
{
	if (message.isEmpty())
    {
		return;
    }

    appendLog(message);
}



//-----------------------------------------------------------------------------
// Function: LogOut::onUndefinedMessages()
//-----------------------------------------------------------------------------
void LogOut::onUndefinedMessage(QString const& message)
{
    if (message.isEmpty())
    {
        return;
    }

    checkUndefinedMessage(message);
}



//-----------------------------------------------------------------------------
// Function: MessageConsole::checkUndefinedMessage()
// NOTE : Need to be updated. New rule needed.
//-----------------------------------------------------------------------------
void LogOut::checkUndefinedMessage(QString const& message)
{
    if (message.isEmpty())
    {
        return;
    }

    if (checkSyntax(message, syntaxSystem))
    {
        appendLog(message);
    }

    else if (checkSyntax(message, syntaxInfo))
    {
        appendInfoLog(message);
    }

    else if (checkSyntax(message, syntaxWarning))
    {
        appendWarningLog(message);
    }

    else if (checkSyntax(message, syntaxError))
    {
        appendErrorLog(message);
    }

    else
    {
        if (message.startsWith("["))
        {
            appendLog(message);
        }

        else
        {
            QString editedMessage = "[ SYSTEM ] | " + message;
            appendLog(editedMessage);
        }
    }
}



//-----------------------------------------------------------------------------
// Function: LogOut::appendInput()
//-----------------------------------------------------------------------------
void LogOut::appendInput(QString inputText)
{
    textEdit->setTextColor(QColor(50, 50, 255));
    textEdit->append(inputText);
    workHistory.append(inputText);
}



//-----------------------------------------------------------------------------
// Function: LogOut::appendLog()
//-----------------------------------------------------------------------------
void LogOut::appendLog(QString inputText)
{
    textEdit->setTextColor(QColor(100, 100, 100));
    textEdit->append(inputText);
}



//-----------------------------------------------------------------------------
// Function: LogOut::appendInfoLog()
//-----------------------------------------------------------------------------
void LogOut::appendInfoLog(QString inputText)
{
    for (int i = 0; i < syntaxInfo.length(); i++)
    {
        if (inputText.startsWith(syntaxInfo[i]))
        {
            inputText.replace(QRegExp("^" + QRegExp::escape(syntaxInfo[i])), "[ INFO ] | ");
            textEdit->setTextColor(QColor(100, 100, 100));
            textEdit->append(inputText);
            break;
        }
    }
}



//-----------------------------------------------------------------------------
// Function: LogOut::appendWarningLog()
//-----------------------------------------------------------------------------
void LogOut::appendWarningLog(QString inputText)
{
    for (int i = 0; i < syntaxWarning.length(); i++)
    {
        if (inputText.startsWith(syntaxWarning[i]))
        {
            inputText.replace(QRegExp("^" + QRegExp::escape(syntaxWarning[i])), "[ WARNING ] | ");
            textEdit->setTextColor(QColor(100, 100, 100));
            textEdit->append(inputText);
            break;
        }
    }
}



//-----------------------------------------------------------------------------
// Function: LogOut::appendErrorLog()
//-----------------------------------------------------------------------------
void LogOut::appendErrorLog(QString inputText)
{
    for (int i = 0; i < syntaxError.length(); i++)
    {
        if (inputText.startsWith(syntaxError[i]))
        {
            inputText.replace(QRegExp("^" + QRegExp::escape(syntaxError[i])), "[ ERROR ] | ");
            textEdit->setTextColor(QColor(100, 100, 100));
            textEdit->append(inputText);
            break;
        }
    }
}



//-----------------------------------------------------------------------------
// Function: MessageConsole::onClearReport()
//-----------------------------------------------------------------------------
void LogOut::onCreateTcl()
{
    QStringList commandLines = workHistory;

    if (commandLines.isEmpty())
    {
        return;
    }

    else
    {
        QString fileDir = QFileDialog::getExistingDirectory(this, "Choose Save Location", QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

        QFile fileTCL(fileDir + "/AWM_Tcl_" + dateTimeNow.currentDateTime().toString("yyyyMMddhhmmss") + ".tcl");

        if(!fileTCL.open(QFile::WriteOnly | QFile::Text))
        {
            return;
        }

        QTextStream tclCommand(&fileTCL);

        for (int i=0; i < commandLines.length(); i++)
        {
            tclCommand << commandLines[i] << "\n";
        }

        fileTCL.flush();
        fileTCL.close();
    }
}




//-----------------------------------------------------------------------------
// Function: LogOut::onClearLog()
//-----------------------------------------------------------------------------
void LogOut::onClearLog()
{
    textEdit->clear();
    workHistory.clear();
}



//-----------------------------------------------------------------------------
// Function: LogOut::contextMenuEvent()
//-----------------------------------------------------------------------------
void LogOut::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu menu;
    menu.addAction(&copyAction_);
    menu.addAction(&selectAllAction_);
    menu.addAction(&clearAction_);

    menu.exec(event->globalPos());
}



//-----------------------------------------------------------------------------
// Function: checkSyntax()
//-----------------------------------------------------------------------------
bool LogOut::checkSyntax(const QString &inputText, const QStringList &syntaxList) {
    return std::any_of(syntaxList.begin(), syntaxList.end(), [&](const QString &syntax) {
        return inputText.startsWith(syntax);
    });
}
