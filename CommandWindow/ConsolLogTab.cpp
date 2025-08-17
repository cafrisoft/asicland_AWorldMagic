#include "ConsolLogTab.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTimer>
#include <QDateTime>
#include <QShortcut>
#include <algorithm>

#include <CommandWindow/ConsoleManager.h>



ConsolLogTab::ConsolLogTab(QWidget *parent) : QWidget{parent}
{
    // Setup UI
    QVBoxLayout *vBoxLayout = new QVBoxLayout(this);
    vBoxLayout->SetMaximumSize;
    vBoxLayout->setSpacing(1);

    textEdit = new QTextEdit(this);
    QSizePolicy textEditPolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    textEdit->setSizePolicy(textEditPolicy);
    textEdit->setReadOnly(true);
    textEdit->setStyleSheet("background-color: black; color: white;");
    textEdit->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));

    lineEdit = new QLineEdit(this);
    lineEdit->setBaseSize(0, 19);
    QSizePolicy lineEditPolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    lineEdit->setSizePolicy(lineEditPolicy);
    lineEdit->setStyleSheet("background-color: black; color: white;");
    lineEdit->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));

    vBoxLayout->addWidget(textEdit);
    vBoxLayout->addWidget(lineEdit);

    QHBoxLayout *hBoxLayout = new QHBoxLayout(this);
    hBoxLayout->SetMaximumSize;
    hBoxLayout->setSpacing(1);
    hBoxLayout->addLayout(vBoxLayout);

    setLayout(hBoxLayout);

    // Setup ConsoleTcl Instance and connect to pointer
    consoleTcl = new ConsoleTcl(this);

    // Setup Key Events on Console Tab
    QShortcut* commandHistoryKeyUp = new QShortcut(QKeySequence(Qt::Key_Up), lineEdit);
    QShortcut* commandHistoryKeyDown = new QShortcut(QKeySequence(Qt::Key_Down), lineEdit);

    connect(commandHistoryKeyUp, &QShortcut::activated, this, &ConsolLogTab::commandHistoryLoadUp);
    connect(commandHistoryKeyDown, &QShortcut::activated, this, &ConsolLogTab::commandHistoryLoadDown);
    connect(lineEdit, &QLineEdit::returnPressed, this, &ConsolLogTab::inputCommand);

    // Initialize Command History
    commandHistoryNow = commandHistory.length();

    // Connect Signal from Console
    ConsoleManager& conMan = ConsoleManager::getInstance();

    connect(&conMan, SIGNAL(reloadLibrary()), this, SIGNAL(reloadLibrary()));
    connect(&conMan, SIGNAL(reloadProject()), this, SIGNAL(reloadProject()));
    connect(&conMan, SIGNAL(reloadProjectLibrary()), this, SIGNAL(reloadProjectLibrary()));
    connect(&conMan, SIGNAL(openSchematic(VLNV const&)), this, SIGNAL(openSchematic(VLNV const&)));
    connect(&conMan, SIGNAL(refreshSchematic(VLNV const&)), this, SIGNAL(refreshSchematic(VLNV const&)));
    connect(&conMan, SIGNAL(closeSchematic(VLNV const&)), this, SIGNAL(closeSchematic(VLNV const&)));
    connect(&conMan, SIGNAL(closeAllSchematic()), this, SIGNAL(closeAllSchematic()));
    connect(&conMan, SIGNAL(openFile(QString)), this, SIGNAL(openFile(QString)));
    connect(&conMan, SIGNAL(requestSave()), this, SIGNAL(requestSave()));
    connect(&conMan, SIGNAL(requestSaveAll()), this, SIGNAL(requestSaveAll()));
    connect(&conMan, SIGNAL(requestImportProject(QString const&, QString const&, QString const&)), this, SIGNAL(requestImportProject(QString const&, QString const&, QString const&)));
    connect(&conMan, SIGNAL(requestExportProject(QString const&, QString const&, QString const&)), this, SIGNAL(requestExportProject(QString const&, QString const&, QString const&)));
    connect(&conMan, SIGNAL(inputMessage(QString const&)), this, SIGNAL(inputMessage(QString const&)));
    connect(&conMan, SIGNAL(noticeMessage(QString const&)), this, SIGNAL(noticeMessage(QString const&)));
    connect(&conMan, SIGNAL(errorMessage(QString const&)), this, SIGNAL(errorMessage(QString const&)));
    connect(&conMan, SIGNAL(undefinedMessage(QString const&)), this, SIGNAL(undefinedMessage(QString const&)));
}



//-----------------------------------------------------------------------------
// Function: commandHistoryLoadUp()
//-----------------------------------------------------------------------------
void ConsolLogTab::commandHistoryLoadUp()
{
    if (!commandHistory.isEmpty())
    {
        if (commandHistoryNow > 0)
        {
            commandHistoryNow -= 1;
            lineEdit->setText(commandHistory[commandHistoryNow]);
        }

        else if (commandHistoryNow == 0)
        {
            lineEdit->setText(commandHistory[0]);
        }

        else
        {
            return;
        }
    }

    else
    {
        return;
    }
}



//-----------------------------------------------------------------------------
// Function: commandHistoryLoadDown()
//-----------------------------------------------------------------------------
void ConsolLogTab::commandHistoryLoadDown()
{
    if (!commandHistory.isEmpty())
    {
        if (commandHistoryNow == commandHistory.length() - 1)
        {
            lineEdit->setText(commandHistory[commandHistory.length() - 1]);
        }

        else if (commandHistoryNow < commandHistory.length() - 1)
        {
            commandHistoryNow += 1;
            lineEdit->setText(commandHistory[commandHistoryNow]);
        }

        else
        {
            return;
        }
    }

    else
    {
        return;
    }
}



//-----------------------------------------------------------------------------
// Function: inputCommand()
//-----------------------------------------------------------------------------
void ConsolLogTab::inputCommand()
{
    QString inputText;
    inputText.append(lineEdit->text());
    lineEdit->clear();

    if (inputText != "")
    {
        commandHistory.append(inputText);
        commandHistoryNow = commandHistory.length();
    }

    else
    {
        return;
    }

    outputCommand(inputText, ConsoleManager::OutputLevel::INPUT);

    consoleTcl->tclInterpreter(inputText);
    QStringList outputText = consoleTcl->tclResultSender();

    for (int i=0; i < outputText.length(); i++)
    {

        if (checkSyntax(outputText[i], syntaxSystem))
        {
            outputCommand(outputText[i], ConsoleManager::OutputLevel::SYSTEM);
        }

        else if (checkSyntax(outputText[i], syntaxInfo))
        {
            outputCommand(outputText[i], ConsoleManager::OutputLevel::INFO);
        }

        else if (checkSyntax(outputText[i], syntaxWarning))
        {
            outputCommand(outputText[i], ConsoleManager::OutputLevel::WARNING);
        }

        else if (checkSyntax(outputText[i], syntaxError))
        {
            outputCommand(outputText[i], ConsoleManager::OutputLevel::ERROR);
        }
    }
}



//-----------------------------------------------------------------------------
// Function: outputCommand()
//-----------------------------------------------------------------------------
void ConsolLogTab::outputCommand(QString inputText, ConsoleManager::OutputLevel outputLevel)
{
    QString showText = dateTimeNow.currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + " | " + inputText;

    if (outputLevel == ConsoleManager::OutputLevel::INFO)
    {
        textEdit->setTextColor(QColor(100, 100, 100));
    }

    else if (outputLevel == ConsoleManager::OutputLevel::WARNING)
    {
        textEdit->setTextColor(QColor(255, 150, 0));
    }

    else if (outputLevel == ConsoleManager::OutputLevel::ERROR)
    {
        textEdit->setTextColor(QColor(255, 0, 0));
    }

    textEdit->append(showText);
    textEdit->setTextColor(QColor(255, 255, 255));
}



//-----------------------------------------------------------------------------
// Function: checkSyntax()
//-----------------------------------------------------------------------------
bool ConsolLogTab::checkSyntax(const QString &inputText, const QStringList &syntaxList) {
    return std::any_of(syntaxList.begin(), syntaxList.end(), [&](const QString &syntax) {
        return inputText.startsWith(syntax);
    });
}
