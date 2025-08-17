//-----------------------------------------------------------------------------
// File: FieldValidator.h
//-----------------------------------------------------------------------------
// Project: Kactus 2
// Author: Mikko Teuho
// Date: 17.11.2015
//
// Description:
// Validator for ipxact:Field.
//-----------------------------------------------------------------------------

#ifndef FIELDVALIDATOR_H
#define FIELDVALIDATOR_H

#include <IPXACTmodels/ipxactmodels_global.h>

#include <IPXACTmodels/Component/Field.h>

#include <QSharedPointer>
#include <QString>

class ExpressionParser;
class Field;
class FieldReset;
class ResetType;
class ParameterValidator;
class EnumeratedValueValidator;
class Component;

//-----------------------------------------------------------------------------
//! Validator for ipxact:Field.
//-----------------------------------------------------------------------------
//class IPXACTMODELS_EXPORT FieldValidator
class FieldValidator
{
public:

    /*!
     *  The constructor.
     *
     *      @param [in] expressionParser            The parser used to solve expressions.
     *      @param [in] enumeratedValueValidator    Validator used for enumerated values.
     *      @param [in] parameterValidator          Validator used for parameters.
     */
    FieldValidator(QSharedPointer<ExpressionParser> expressionParser,
        QSharedPointer<EnumeratedValueValidator> enumeratedValueValidator,
        QSharedPointer<ParameterValidator> parameterValidator);

	/*!
     *  The destructor.
     */
	~FieldValidator() = default;

    /*!
     *  Change the containing component.
     *
     *      @param [in] newComponent    The selected component.
     */
    void componentChange(QSharedPointer<Component> newComponent);

    /*!
     *  Get the validator used for enumerated values.
     *
     *      @return The validator used for enumerated values.
     */
    QSharedPointer<EnumeratedValueValidator> getEnumeratedValueValidator() const;

    /*!
     *  Validates the given field.
     *
     *      @param [in] field   The field to validate.
     *
     *      @return True, if the field is valid IP-XACT, otherwise false.
     */
    bool validate(QSharedPointer<Field> field) const;

    /*!
     *  Check if the field contains a valid name.
     *
     *      @param [in] field   The selected field.
     *
     *      @return True, if the name is valid, otherwise false.
     */
    bool hasValidName(QSharedPointer<Field> field) const;

    /*!
     *  Check if the field has a valid presence value.
     *
     *      @param [in] field   The selected field.
     *
     *      @return True, if the isPresent is valid, otherwise false.
     */
    bool hasValidIsPresent(QSharedPointer<Field> field) const;

    /*!
     *  Check if the field has a valid bit offset.
     *
     *      @param [in] field   The selected field.
     *
     *      @return True, if the bit offset is valid, otherwise false.
     */
    bool hasValidBitOffset(QSharedPointer<Field> field) const;

    /*!
     *  Check if the resets within the field are valid.
     *
     *     @param [in] field  The field whose resets to check
     *
     *     @return True, if all resets are valid, otherwise false.
     */
    bool hasValidResets(QSharedPointer<Field> field) const;

    /*!
    *  Check if the reset type references within a field are valid.
    *
    *     @param [in] field     The selected field.
    *
    *     @return True, if all reset type references are valid, otherwise false.
    */
    bool hasValidResetsTypeReferences(QSharedPointer<Field> field) const;

    /*!
     *  Check if the selected reset type reference is valid.
     *
     *     @param [in] fieldReset   Reset containing the selected reset type reference.
     *
     *     @return True, if the reset type reference is valid.
     */
    bool hasValidResetTypeReference(QSharedPointer<FieldReset> fieldReset) const;

    /*!
     *  Check if the field has a valid reset value.
     *
     *      @param [in] fieldReset   The selected field reset.
     *
     *      @return True, if the reset value is valid, otherwise false.
     */
    bool hasValidResetValue(QSharedPointer<FieldReset> fieldReset) const;

    /*!
     *  Check if the field has a valid reset mask.
     *
     *      @param [in] field   The selected field reset.
     *
     *      @return True, if the reset mask is valid, otherwise false.
     */
    bool hasValidResetMask(QSharedPointer<FieldReset> fieldReset) const;

    /*!
     *  Check if the field has a valid write value constraint.
     *
     *      @param [in] field   The selected field.
     *
     *      @return True, if the write value constraint is valid, otherwise false.
     */
    bool hasValidWriteValueConstraint(QSharedPointer<Field> field) const;

    /*!
     *  Check if the field has a valid reserved value.
     *
     *      @param [in] field   The selected field.
     *
     *      @return True, if the reserved is valid, otherwise false.
     */
    bool hasValidReserved(QSharedPointer<Field> field) const;

    /*!
     *  Check if the field has a valid bit width.
     *
     *      @param [in] field   The selected field.
     *
     *      @return True, if the bit width is valid, otherwise false.
     */
    bool hasValidBitWidth(QSharedPointer<Field> field) const;

    /*!
     *  Check if the field has valid enumerated values.
     *
     *      @param [in] field   The selected field.
     *
     *      @return True, if the enumerated values are valid, otherwise false.
     */
    bool hasValidEnumeratedValues(QSharedPointer<Field> field) const;

    /*!
     *  Check if the field has valid parameters.
     *
     *      @param [in] field   The selected field.
     *
     *      @return True, if the parameters are valid, otherwise false.
     */
    bool hasValidParameters(QSharedPointer<Field> field) const;

    /*!
     *  Check if the field has a valid access value.
     *
     *      @param [in] field   The selected field.
     *
     *      @return True, if the access is valid, otherwise false.
     */
    bool hasValidAccess(QSharedPointer<Field> field) const;

    /*!
     *  Locate errors within a field.
     *
     *      @param [in] errors      List of found errors.
     *      @param [in] field       The selected field.
     *      @param [in] context     Context to help locate the error.
     */
    void findErrorsIn(QVector<QString>& errors, QSharedPointer<Field> field, QString const& context) const;

private:

	// Disable copying.
	FieldValidator(FieldValidator const& rhs);
	FieldValidator& operator=(FieldValidator const& rhs);

    /*!
     *  Find errors within field name.
     *
     *      @param [in] errors      List of found errors.
     *      @param [in] field       The selected field.
     *      @param [in] context     Context to help locate the error.
     */
    void findErrorsInName(QVector<QString>& errors, QSharedPointer<Field> field, QString const& context) const;

    /*!
     *  Find errors within field is present.
     *
     *      @param [in] errors      List of found errors.
     *      @param [in] field       The selected field.
     *      @param [in] context     Context to help locate the error.
     */
    void findErrorsInIsPresent(QVector<QString>& errors, QSharedPointer<Field> field, QString const& context) const;

    /*!
     *  Find errors within field bit offset.
     *
     *      @param [in] errors      List of found errors.
     *      @param [in] field       The selected field.
     *      @param [in] context     Context to help locate the error.
     */
    void findErrorsInBitOffset(QVector<QString>& errors, QSharedPointer<Field> field, QString const& context) const;

    /*!
     *  Find errors within resets.
     *
     *      @param [in] errors      List of found errors.
     *      @param [in] field       The selected field.
     *      @param [in] context     Context to help locate the errors.
     */
    void findErrorsInResets(QVector<QString>& errors, QSharedPointer<Field> field, QString const& context) const;

    /*!
     *  Find errors within reset type reference.
     *
     *      @param [in] errors      List of found errors.
     *      @param [in] fieldReset  The selected reset.
     *      @param [in] context     Context to help locate the errors.
     */
    void findErrorsInResetTypeReference(QVector<QString>& errors, QSharedPointer<FieldReset> fieldReset,
        QString const& context) const;

    /*!
     *  Find errors within field reset value.
     *
     *      @param [in] errors      List of found errors.
     *      @param [in] fieldReset  The selected field reset.
     *      @param [in] context     Context to help locate the error.
     */
    void findErrorsInResetValue(QVector<QString>& errors, QSharedPointer<FieldReset> fieldReset,
        QString const& context) const;

    /*!
     *  Find errors within field reset mask.
     *
     *      @param [in] errors      List of found errors.
     *      @param [in] fieldReset  The selected field reset.
     *      @param [in] context     Context to help locate the error.
     */
    void findErrorsInResetMask(QVector<QString>& errors, QSharedPointer<FieldReset> fieldReset,
        QString const& context) const;

    /*!
     *  Find errors within field write value constraint.
     *
     *      @param [in] errors      List of found errors.
     *      @param [in] field       The selected field.
     *      @param [in] context     Context to help locate the error.
     */
    void findErrorsInWriteValueConstraint(QVector<QString>& errors, QSharedPointer<Field> field,
        QString const& context) const;

    /*!
     *  Find errors within field reserved value.
     *
     *      @param [in] errors      List of found errors.
     *      @param [in] field       The selected field.
     *      @param [in] context     Context to help locate the error.
     */
    void findErrorsInReserved(QVector<QString>& errors, QSharedPointer<Field> field, QString const& context) const;

    /*!
     *  Find errors within field bit width.
     *
     *      @param [in] errors      List of found errors.
     *      @param [in] field       The selected field.
     *      @param [in] context     Context to help locate the error.
     */
    void findErrorsInBitWidth(QVector<QString>& errors, QSharedPointer<Field> field, QString const& context) const;

    /*!
     *  Find errors within field enumerated values.
     *
     *      @param [in] errors      List of found errors.
     *      @param [in] field       The selected field.
     */
    void findErrorsInEnumeratedValues(QVector<QString>& errors, QSharedPointer<Field> field) const;

    /*!
     *  Find errors within field parameters.
     *
     *      @param [in] errors      List of found errors.
     *      @param [in] field       The selected field.
     */
    void findErrorsInParameters(QVector<QString>& errors, QSharedPointer<Field> field) const;

    /*!
     *  Find errors within field access.
     *
     *      @param [in] errors      List of found errors.
     *      @param [in] field       The selected field.
     *      @param [in] context     Context to help locate the error.
     */
    void findErrorsInAccess(QVector<QString>& errors, QSharedPointer<Field> field, QString const& context) const;

    /*!
     *  Check if the contained bit expression is valid.
     *
     *      @param [in] expression  The expression to check.
     *
     *      @return True, if the expression is a valid bit expression, otherwise false.
     */
    bool isBitExpressionValid(QString const& expression) const;

    //-----------------------------------------------------------------------------
    // Data.
    //-----------------------------------------------------------------------------

    //! The expression parser to use.
    QSharedPointer<ExpressionParser> expressionParser_;

    //! The used enumerated value validator.
    QSharedPointer<EnumeratedValueValidator> enumeratedValueValidator_;

    //! The used parameter validator.
    QSharedPointer<ParameterValidator> parameterValidator_;

    //! The reset types of the containing component.
    QSharedPointer<QList<QSharedPointer<ResetType> > > availableResetTypes_;
};

#endif // FIELDVALIDATOR_H
