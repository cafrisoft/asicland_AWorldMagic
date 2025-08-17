//-----------------------------------------------------------------------------
// File: messageconsole.cpp
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Antti Kamppi
// Date: 22.02.2011
//
// Description:
// This widget is used to print event information to the user.
//-----------------------------------------------------------------------------

#include "MessageConsole.h"

#include <QMenu>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QIcon>
#include <QSize>
#include <algorithm>

#include <KactusAPI/KactusColors.h>
#include <CommandWindow/ConsoleManager.h>



MessageConsole::MessageConsole(QWidget *parent): copyAction_(tr("Copy"), this), selectAllAction_(tr("Select all"), this), clearAction_(tr("Clear"), this)
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

    btnInfo = new QPushButton(this);
    btnInfo->setCheckable(true);
    btnInfo->setChecked(true);
    btnInfo->setFixedSize(30, 30);
    btnInfo->setIcon(QIcon(":/resources/Res/info.png"));
    btnInfo->setIconSize(QSize(30, 30));

    btnWarning = new QPushButton(this);
    btnWarning->setCheckable(true);
    btnWarning->setChecked(true);
    btnWarning->setFixedSize(30, 30);
    btnWarning->setIcon(QIcon(":/resources/Res/warning.png"));
    btnWarning->setIconSize(QSize(30, 30));

    btnError = new QPushButton(this);
    btnError->setCheckable(true);
    btnError->setChecked(true);
    btnError->setFixedSize(30, 30);
    btnError->setIcon(QIcon(":/resources/Res/error.png"));
    btnError->setIconSize(QSize(30, 30));

    textEdit = new QTextEdit(this);
    QSizePolicy textEditPolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    textEdit->setSizePolicy(textEditPolicy);
    textEdit->setReadOnly(true);
    textEdit->setUndoRedoEnabled(false);
    textEdit->setStyleSheet("background-color: white; color: black;");
    textEdit->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));

    hBoxLayout->addStretch();
    hBoxLayout->addWidget(btnClear);
    hBoxLayout->addWidget(btnInfo);
    hBoxLayout->addWidget(btnWarning);
    hBoxLayout->addWidget(btnError);

    vBoxLayout->addLayout(hBoxLayout);
    vBoxLayout->addWidget(textEdit);

    setLayout(vBoxLayout);

	copyAction_.setDisabled(true);

	connect(&copyAction_, SIGNAL(triggered()), this, SLOT(copy()), Qt::UniqueConnection);
	connect(&selectAllAction_, SIGNAL(triggered()), this, SLOT(selectAll()), Qt::UniqueConnection);
	connect(&clearAction_, SIGNAL(triggered()),	this, SLOT(clear()), Qt::UniqueConnection);
	connect(this, SIGNAL(copyAvailable(bool)), &copyAction_, SLOT(setEnabled(bool)), Qt::UniqueConnection);
    connect(btnClear, SIGNAL(clicked()), this, SLOT(onClearReport()));
    connect(btnInfo, SIGNAL(clicked()), this, SLOT(onReloadReport()));
    connect(btnWarning, SIGNAL(clicked()), this, SLOT(onReloadReport()));
    connect(btnError, SIGNAL(clicked()), this, SLOT(onReloadReport()));

    reportConsole.clear();
}



//-----------------------------------------------------------------------------
// Function: LogOut::onInputMessage()
//-----------------------------------------------------------------------------
void MessageConsole::onInputMessage(QString const& message)
{
    return;
}



//-----------------------------------------------------------------------------
// Function: LogOut::onNoticeMessage()
//-----------------------------------------------------------------------------
void MessageConsole::onNoticeMessage(QString const& message)
{
    if (message.isEmpty())
    {
        return;
    }

    checkUndefinedMessage(message);
}



//-----------------------------------------------------------------------------
// Function: LogOut::onErrorMessage()
//-----------------------------------------------------------------------------
void MessageConsole::onErrorMessage(QString const& message)
{
    if (message.isEmpty())
    {
        return;
    }

    checkErrorMessage(message);
}



//-----------------------------------------------------------------------------
// Function: LogOut::onUndefinedMessages()
//-----------------------------------------------------------------------------
void MessageConsole::onUndefinedMessage(QString const& message)
{
    if (message.isEmpty())
    {
        return;
    }

    checkUndefinedMessage(message);
}



//-----------------------------------------------------------------------------
// Function: MessageConsole::checkErrorMessage()
//-----------------------------------------------------------------------------
void MessageConsole::checkErrorMessage(QString const& message)
{
    QString message_ = message;

    if (message_.isEmpty())
    {
        return;
    }

    if (checkSyntax(message_, syntaxInfo))
    {
        appendReport(message_, ConsoleManager::OutputLevel::INFO);
    }

    else if (checkSyntax(message_, syntaxWarning))
    {
        appendReport(message_, ConsoleManager::OutputLevel::WARNING);
    }

    else if (checkSyntax(message_, syntaxError))
    {
        appendReport(message_, ConsoleManager::OutputLevel::ERROR);
    }

    else
    {
        return;
    }
}



//-----------------------------------------------------------------------------
// Function: MessageConsole::checkUndefinedMessage()
// NOTE : Need to be updated. New rule needed.
//-----------------------------------------------------------------------------
void MessageConsole::checkUndefinedMessage(QString const& message)
{
    QString message_ = message;

    if (message_.isEmpty())
    {
        return;
    }

    if (checkSyntax(message_, syntaxInfo))
    {
        appendReport(message_, ConsoleManager::OutputLevel::INFO);
    }

    else if (checkSyntax(message_, syntaxWarning))
    {
        appendReport(message_, ConsoleManager::OutputLevel::WARNING);
    }

    else if (checkSyntax(message_, syntaxError))
    {
        appendReport(message_, ConsoleManager::OutputLevel::ERROR);
    }

    else
    {
        return;
    }
}



//-----------------------------------------------------------------------------
// Function: MessageConsole::contextMenuEvent()
//-----------------------------------------------------------------------------
void MessageConsole::contextMenuEvent(QContextMenuEvent* event)
{
	QMenu menu;
	menu.addAction(&copyAction_);
	menu.addAction(&selectAllAction_);
	menu.addAction(&clearAction_);

	menu.exec(event->globalPos());
}



//-----------------------------------------------------------------------------
// Function: MessageConsole::appendReport()
//-----------------------------------------------------------------------------
void MessageConsole::appendReport(QString inputText, ConsoleManager::OutputLevel outputLevel)
{
    reportConsole.append(inputText);

    if (outputLevel == ConsoleManager::OutputLevel::INFO)
    {
        if (btnInfo->isChecked())
        {
            textEdit->setTextColor(QColor(0, 0, 0));
            textEdit->append(inputText);
            textEdit->setTextColor(QColor(0, 0, 0));
        }

        else
        {
            return;
        }
    }

    else if (outputLevel == ConsoleManager::OutputLevel::WARNING)
    {
        if (btnWarning->isChecked())
        {
            textEdit->setTextColor(QColor(255, 150, 0));
            textEdit->append(inputText);
            textEdit->setTextColor(QColor(0, 0, 0));
        }

        else
        {
            return;
        }
    }

    else if (outputLevel == ConsoleManager::OutputLevel::ERROR)
    {
        if (btnError->isChecked())
        {
            textEdit->setTextColor(QColor(255, 0, 0));
            textEdit->append(inputText);
            textEdit->setTextColor(QColor(0, 0, 0));
        }

        else
        {
            return;
        }
    }
}



//-----------------------------------------------------------------------------
// Function: MessageConsole::onReloadReport()
//-----------------------------------------------------------------------------
void MessageConsole::onReloadReport()
{
    textEdit->clear();

    QStringList reportHistory = reportConsole;

    reportConsole.clear();

    for (int i=0; i < reportHistory.length(); i++)
    {
        if (checkSyntax(reportHistory[i], syntaxSystem))
        {
            appendReport(reportHistory[i], ConsoleManager::OutputLevel::SYSTEM);
        }

        else if (checkSyntax(reportHistory[i], syntaxInfo))
        {
            appendReport(reportHistory[i], ConsoleManager::OutputLevel::INFO);
        }

        else if (checkSyntax(reportHistory[i], syntaxWarning))
        {
            appendReport(reportHistory[i], ConsoleManager::OutputLevel::WARNING);
        }

        else if (checkSyntax(reportHistory[i], syntaxError))
        {
            appendReport(reportHistory[i], ConsoleManager::OutputLevel::ERROR);
        }

        else
        {
            return;
        }
    }
}



//-----------------------------------------------------------------------------
// Function: MessageConsole::onClearReport()
//-----------------------------------------------------------------------------
void MessageConsole::onClearReport()
{
    textEdit->clear();
    reportConsole.clear();
}



//-----------------------------------------------------------------------------
// Function: checkSyntax()
//-----------------------------------------------------------------------------
bool MessageConsole::checkSyntax(const QString &inputText, const QStringList &syntaxList) {
    return std::any_of(syntaxList.begin(), syntaxList.end(), [&](const QString &syntax) {
        return inputText.startsWith(syntax);
    });
}
