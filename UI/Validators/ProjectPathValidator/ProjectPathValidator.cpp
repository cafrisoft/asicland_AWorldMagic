/*
 *  	Created on: 11.1.2024
 *     Author: P.H.G
 * 	Filename: ProjectPathValidator.cpp
 *		Project: AWM
 */

#include "ProjectPathValidator.h"

#include <QRegularExpressionValidator>

//-----------------------------------------------------------------------------
// Function: ProjectPathValidator()
//-----------------------------------------------------------------------------
ProjectPathValidator::ProjectPathValidator(QObject *parent): QRegularExpressionValidator(parent)
{

}

//-----------------------------------------------------------------------------
// Function: validate()
//-----------------------------------------------------------------------------
QValidator::State ProjectPathValidator::validate(QString& input, int& pos) const
{
    State state = QRegularExpressionValidator::validate(input,pos);

    // Partial library path is considered invalid.
    if (state == QValidator::Intermediate)
    {
        return QValidator::Invalid;
    }

    return state;
}

//-----------------------------------------------------------------------------
// Function: ProjectPathValidator::setUnmodifiablePath()
//-----------------------------------------------------------------------------
void ProjectPathValidator::setUnmodifiablePath( const QString& path )
{
    QRegularExpression regExp(path + QString(".*"), 
        QRegularExpression::CaseInsensitiveOption | QRegularExpression::DotMatchesEverythingOption);
	setRegularExpression(regExp);
}
