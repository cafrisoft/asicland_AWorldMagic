//-----------------------------------------------------------------------------
// FileSet: FileSetValidator.cpp
//-----------------------------------------------------------------------------
// Project: Kactus 2
// Author: Janne Virtanen
// Date: 20.11.2015
//
// Description:
// Validator for ipxact:FileSet.
//-----------------------------------------------------------------------------

#include "FileSetValidator.h"
#include "FileValidator.h"

#include <KactusAPI/ExpressionParser.h>

#include <IPXACTmodels/Component/Choice.h>
#include <IPXACTmodels/common/Enumeration.h>
#include <IPXACTmodels/common/FileBuilder.h>

#include <QRegularExpression>
#include <QStringList>

//-----------------------------------------------------------------------------
// Function: SystemVerilogValidator::SystemVerilogValidator()
//-----------------------------------------------------------------------------
FileSetValidator::FileSetValidator(QSharedPointer<FileValidator> fileValidator,
    QSharedPointer<ExpressionParser> expressionParser):
fileValidator_(fileValidator),
expressionParser_(expressionParser)
{

}

//-----------------------------------------------------------------------------
// Function: FileSetValidator::validate()
//-----------------------------------------------------------------------------
bool FileSetValidator::validate(QSharedPointer<FileSet> fileSet) const
{
	if (!hasValidName( fileSet->name()))
	{
		return false;
	}

	for (QSharedPointer<FileBuilder> const& currentFileBuilder : *fileSet->getDefaultFileBuilders())
	{
		if (!hasValidName(currentFileBuilder->getFileType()) ||
            !fileBuilderHasValidReplaceDefaultFlags(currentFileBuilder))
		{
			return false;
		}
	}

	for (QSharedPointer<File> const& file : *fileSet->getFiles())
	{
		if (!fileValidator_->validate(file))
		{
			return false;
		}
	}

	return true;
}

//-----------------------------------------------------------------------------
// Function: FileSetValidator::fileBuilderHasValidReplaceDefaultFlags()
//-----------------------------------------------------------------------------
bool FileSetValidator::fileBuilderHasValidReplaceDefaultFlags(QSharedPointer<FileBuilder> builder) const
{
    if (!builder->getReplaceDefaultFlags().isEmpty())
    {
        bool canConvert = true;
        int replaceFlags =
            expressionParser_->parseExpression(builder->getReplaceDefaultFlags()).toInt(&canConvert);

        if (!canConvert || replaceFlags < 0 || replaceFlags > 1)
        {
            return false;
        }
    }

    return true;
}

//-----------------------------------------------------------------------------
// Function: FileSetValidator::findErrorsIn()
//-----------------------------------------------------------------------------
void FileSetValidator::findErrorsIn(QVector<QString>& errors, QSharedPointer<FileSet> fileSet,
    QString const& context) const
{
	if (!hasValidName(fileSet->name()))
	{
        errors.append(QObject::tr("Invalid name '%1' set for file set within %2")
            .arg(fileSet->name()).arg(context));
	}

	for (QSharedPointer<FileBuilder> const& currentFileBuilder : *fileSet->getDefaultFileBuilders())
	{
		if (!hasValidName(currentFileBuilder->getFileType()))
		{
			errors.append(QObject::tr("The type of default file builder is empty within file set %1.").arg(
                fileSet->name()));
		}

        if (!fileBuilderHasValidReplaceDefaultFlags(currentFileBuilder))
		{
            errors.append(QObject::tr("Invalid \"replace default flags\" value set for file set %1. Value must "
                "evaluate to 0 or 1.").
                arg(fileSet->name()));
		}
	}

    QString filesetContext = QObject::tr("file set %1").arg(fileSet->name());
	for (QSharedPointer<File> const& file : *fileSet->getFiles())
	{
		fileValidator_->findErrorsIn(errors, file, filesetContext);
	}
}

//-----------------------------------------------------------------------------
// Function: EnumeratedValueValidator::hasValidName()
//-----------------------------------------------------------------------------
bool FileSetValidator::hasValidName(QString const& name) const
{
	QRegularExpression whiteSpaceExpression(QStringLiteral("^\\s*$"));

	if (name.isEmpty() || whiteSpaceExpression.match(name).hasMatch())
	{
		return false;
	}

	return true;
}
