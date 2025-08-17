//-----------------------------------------------------------------------------
// File: RegisterFileValidator.cpp
//-----------------------------------------------------------------------------
// Project: Kactus 2
// Author: Dan Chianucci
// Date: 04.06.2018
//
// Description:
// Validator for ipxact:RegisterFile.
//-----------------------------------------------------------------------------

#include "RegisterFileValidator.h"
#include <KactusAPI/ExpressionParser.h>
#include <IPXACTmodels/Component/validators/RegisterValidator.h>
#include <IPXACTmodels/common/validators/ParameterValidator.h>

#include <IPXACTmodels/Component/RegisterBase.h>
#include <IPXACTmodels/Component/RegisterFile.h>
#include <IPXACTmodels/Component/Register.h>
#include <QRegularExpression>

//-----------------------------------------------------------------------------
// Function: RegisterFileValidator::RegisterFileValidator()
//-----------------------------------------------------------------------------
RegisterFileValidator::RegisterFileValidator(QSharedPointer<ExpressionParser> expressionParser,
    QSharedPointer<RegisterValidator> registerValidator,
    QSharedPointer<ParameterValidator> parameterValidator) :
    RegisterBaseValidator(expressionParser, parameterValidator),
    registerValidator_(registerValidator)
{

}

//-----------------------------------------------------------------------------
// Function: RegisterFileValidator::getRegisterValidator()
//-----------------------------------------------------------------------------
QSharedPointer<RegisterValidator> RegisterFileValidator::getRegisterValidator() const
{
    return registerValidator_;
}

//-----------------------------------------------------------------------------
// Function: RegisterFileValidator::validate()
//-----------------------------------------------------------------------------
bool RegisterFileValidator::validate(QSharedPointer<RegisterFile> selectedRegisterFile) const
{
    return RegisterBaseValidator::validate(selectedRegisterFile) &&
        hasValidRange(selectedRegisterFile) &&
        hasValidRegisterData(selectedRegisterFile);
}

//-----------------------------------------------------------------------------
// Function: RegisterFileValidator::hasValidSize()
//-----------------------------------------------------------------------------
bool RegisterFileValidator::hasValidRange(QSharedPointer<RegisterFile> selectedRegisterFile) const
{
    bool toIntOk = true;
    quint64 range = expressionParser_->parseExpression(selectedRegisterFile->getRange()).toULongLong(&toIntOk);

    return toIntOk && range > 0;
}

//-----------------------------------------------------------------------------
// Function: RegisterFileValidator::hasValidRegisterData()
//-----------------------------------------------------------------------------
bool RegisterFileValidator::hasValidRegisterData(QSharedPointer<RegisterFile> selectedRegisterFile) const
{
    //TODO
    return true;
}

//-----------------------------------------------------------------------------
// Function: RegisterFileValidator::findErrorsIn()
//-----------------------------------------------------------------------------
void RegisterFileValidator::findErrorsIn(QVector<QString>& errors, QSharedPointer<RegisterFile> selectedRegisterFile,
    QString const& context) const
{
    QString registerContext = QStringLiteral("register file") + selectedRegisterFile->name();

    findErrorsInName(errors, selectedRegisterFile, context);
    findErrorsInIsPresent(errors, selectedRegisterFile, context);
    findErrorsInDimension(errors, selectedRegisterFile, context);
    findErrorsInAddressOffset(errors, selectedRegisterFile, context);
    findErrorsInParameters(errors, selectedRegisterFile, registerContext);

    findErrorsInRange(errors, selectedRegisterFile, context);
    findErrorsInRegisterData(errors, selectedRegisterFile, registerContext);
}

//-----------------------------------------------------------------------------
// Function: RegisterFileValidator::findErrorsInSize()
//-----------------------------------------------------------------------------
void RegisterFileValidator::findErrorsInRange(QVector<QString>& errors,
    QSharedPointer<RegisterFile> selectedRegisterFile,
    QString const& context) const
{
    if (!hasValidRange(selectedRegisterFile))
    {
        errors.append(QObject::tr("Invalid range specified for register file %1 within %2").
            arg(selectedRegisterFile->name()).arg(context));
    }
}

//-----------------------------------------------------------------------------
// Function: RegisterFileValidator::findErrorsInRegisterData()
//-----------------------------------------------------------------------------
void RegisterFileValidator::findErrorsInRegisterData(QVector<QString>& errors,
    QSharedPointer<RegisterFile> selectedRegisterFile,
    QString const& context) const
{
    //TODO
}
