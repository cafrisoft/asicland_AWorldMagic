//-----------------------------------------------------------------------------
// File: ConsoleMediator.cpp
//-----------------------------------------------------------------------------
// Project: Kactus 2
// Author: Esko Pekkarinen
// Date: 13.02.2018
//
// Description:
// Console output for user messages.
//-----------------------------------------------------------------------------

#include <KactusAPI/ConsoleMediator.h>

#include <iostream>

#include <QTextStream>

//-----------------------------------------------------------------------------
// Function: ConsoleMediator::ConsoleMediator()
//-----------------------------------------------------------------------------
ConsoleMediator::ConsoleMediator()
{

}

//-----------------------------------------------------------------------------
// Function: ConsoleMediator::~ConsoleMediator()
//-----------------------------------------------------------------------------
ConsoleMediator::~ConsoleMediator()
{

}

//-----------------------------------------------------------------------------
// Function: ConsoleMediator::showMessage()
//-----------------------------------------------------------------------------
void ConsoleMediator::showMessage(QString const& message) const
{
    static QTextStream out(stdout); 
//    out << message << Qt::endl;
    out << message << endl;    //. not surported Qt.5.14 부터 지원
}

//-----------------------------------------------------------------------------
// Function: ConsoleMediator::showError()
//-----------------------------------------------------------------------------
void ConsoleMediator::showError(QString const& error) const
{
    static QTextStream err(stderr);
//    err << error << Qt::endl;
    err << error << endl;    //. not surported Qt.5.14 부터 지원
}

//-----------------------------------------------------------------------------
// Function: ConsoleMediator::showFailure()
//-----------------------------------------------------------------------------
void ConsoleMediator::showFailure(QString const& error) const
{
    showError(error);
}

//-----------------------------------------------------------------------------
// Function: ConsoleMediator::showStatusMessage()
//-----------------------------------------------------------------------------
void ConsoleMediator::showStatusMessage(QString const& status) const
{
    showMessage(status);
}
