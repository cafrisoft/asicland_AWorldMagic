//-----------------------------------------------------------------------------
// File: ComponentValidator.h
//-----------------------------------------------------------------------------
// Project: Kactus 2
// Author: Mikko Teuho
// Date: 14.12.2015
//
// Description:
// Validator for the ipxact:component.
//-----------------------------------------------------------------------------

#ifndef COMPONENTVALIDATOR_H
#define COMPONENTVALIDATOR_H

#include <IPXACTmodels/ipxactmodels_global.h>

#include <QSharedPointer>
#include <QString>
#include <QVector>

class Component;
class ResetType;

class BusInterfaceValidator;
class IndirectInterfaceValidator;
class ChannelValidator;
class RemapStateValidator;
class AddressSpaceValidator;
class MemoryMapValidator;
class ViewValidator;
class InstantiationsValidator;
class PortValidator;
class ComponentGeneratorValidator;
class ChoiceValidator;
class FileSetValidator;
class CPUValidator;
class OtherClockDriverValidator;
class ParameterValidator;
class AssertionValidator;

class ExpressionParser;
class LibraryInterface;

//-----------------------------------------------------------------------------
//! Validator for the ipxact:component.
//-----------------------------------------------------------------------------
//class IPXACTMODELS_EXPORT ComponentValidator
class ComponentValidator
{
public:

    /*!
     *  The constructor.
     *
     *      @param [in] parser      The used expression parser.
     *      @param [in] library     The used library interface.
     */
    ComponentValidator(QSharedPointer<ExpressionParser> parser, LibraryInterface* library);

	//! The destructor.
	~ComponentValidator() = default;
    
    /*!
     *  Validates the given component.
     *
     *      @param [in] component   The selected component.
     *
     *      @return True, if the component is valid IP-XACT, otherwise false.
     */
    bool validate(QSharedPointer<Component> component);

    /*!
     *  Check if the component vlnv is valid.
     *
     *      @param [in] component   The selected component.
     *
     *      @return True, if the vlnv is valid, otherwise false.
     */
    bool hasValidVLNV(QSharedPointer<Component> component) const;

    /*!
     *  Check if the contained bus interfaces are valid.
     *
     *      @param [in] component   The selected component.
     *
     *      @return True, if the bus interfaces are valid, otherwise false.
     */
    bool hasValidBusInterfaces(QSharedPointer<Component> component);
   
    /*!
     *  Check if the contained indirect interfaces are valid.
     *
     *      @param [in] component   The selected component.
     *
     *      @return True, if the indirect interfaces are valid, otherwise false.
     */     
    bool hasValidIndirectInterfaces(QSharedPointer<Component> component);

    /*!
     *  Check if the contained channels are valid.
     *
     *      @param [in] component   The selected component.
     *
     *      @return True, if the channels are valid, otherwise false.
     */
    bool hasValidChannels(QSharedPointer<Component> component);

    /*!
     *  Check if the contained remap states are valid.
     *
     *      @param [in] component   The selected component.
     *
     *      @return True, if the remap states are valid, otherwise false.
     */
    bool hasValidRemapStates(QSharedPointer<Component> component);

    /*!
     *  Check if the contained address spaces are valid.
     *
     *      @param [in] component   The selected component.
     *
     *      @return True, if the address spaces are valid, otherwise false.
     */
    bool hasValidAddressSpaces(QSharedPointer<Component> component);

    /*!
     *  Check if the contained memory maps are valid.
     *
     *      @param [in] component   The selected component.
     *
     *      @return True, if the memory maps are valid, otherwise false.
     */
    bool hasValidMemoryMaps(QSharedPointer<Component> component);

    /*!
     *  Check if the contained views are valid.
     *
     *      @param [in] component   The selected component.
     *
     *      @return True, if the views are valid, otherwise false.
     */
    bool hasValidViews(QSharedPointer<Component> component);

    /*!
     *  Check if the contained component instantiations are valid.
     *
     *      @param [in] component   The selected component.
     *
     *      @return True, if the component instantiations are valid, otherwise false.
     */
    bool hasValidComponentInstantiations(QSharedPointer<Component> component);

    /*!
     *  Check if the contained design instantiations are valid.
     *
     *      @param [in] component   The selected component.
     *
     *      @return True, if the design instantiations are valid, otherwise false.
     */
    bool hasValidDesignInstantiations(QSharedPointer<Component> component);

    /*!
     *  Check if the contained design configuration instantiations are valid.
     *
     *      @param [in] component   The selected component.
     *
     *      @return True, if the design configuration instantiations are valid, otherwise false.
     */
    bool hasValidDesignConfigurationInstantiations(QSharedPointer<Component> component);

    /*!
     *  Check if the contained ports are valid.
     *
     *      @param [in] component   The selected component.
     *
     *      @return True, if the ports are valid, otherwise false.
     */
    bool hasValidPorts(QSharedPointer<Component> component);

    /*!
     *  Check if the contained component generators are valid.
     *
     *      @param [in] component   The selected component.
     *
     *      @return True, if the component generators are valid, otherwise false.
     */
    bool hasValidComponentGenerators(QSharedPointer<Component> component);

    /*!
     *  Check if the contained choices are valid.
     *
     *      @param [in] component   The selected component.
     *
     *      @return True, if the choices are valid, otherwise false.
     */
    bool hasValidChoices(QSharedPointer<Component> component);

    /*!
     *  Check if the contained file sets are valid.
     *
     *      @param [in] component   The selected component.
     *
     *      @return True, if the file sets are valid, otherwise false.
     */
    bool hasValidFileSets(QSharedPointer<Component> component);

    /*!
     *  Check if the contained CPUs are valid.
     *
     *      @param [in] component   The selected component.
     *
     *      @return True, if the CPUs are valid, otherwise false.
     */
    bool hasValidCPUs(QSharedPointer<Component> component);

    /*!
     *  Check if the contained other clock drivers are valid.
     *
     *      @param [in] component   The selected component.
     *
     *      @return True, if the other clock drivers are valid, otherwise false.
     */
    bool hasValidOtherClockDrivers(QSharedPointer<Component> component);

    /*!
     *  Check if the reset types of the selected component are valid.
     *
     *      @param [in] component   The selected component.
     *
     *      @return True, if the reset types are valid, otherwise false.
     */
    bool hasValidResetTypes(QSharedPointer<Component> component);

    /*!
     *  Check if a single reset type is valid.
     *
     *      @param [in] resetType   The selected reset type.
     *      @param [in] resetTypes  The reset types of the containing component.
     *
     *      @return True, if the reset type is valid, otherwise false.
     */
    bool singleResetTypeIsValid(QSharedPointer<ResetType> resetType,
        QSharedPointer<QList<QSharedPointer<ResetType> > > resetTypes) const;

    /*!
     *  Check if the contained parameters are valid.
     *
     *      @param [in] component   The selected component.
     *
     *      @return True, if the parameters are valid, otherwise false.
     */
    bool hasValidParameters(QSharedPointer<Component> component);

    /*!
     *  Check if the contained assertions are valid.
     *
     *      @param [in] component   The selected component.
     *
     *      @return True, if the assertions are valid, otherwise false.
     */
    bool hasValidAssertions(QSharedPointer<Component> component);

    /*!
     *  Locate errors within a component.
     *
     *      @param [in] errors      List of found errors.
     *      @param [in] component   The selected component.
     */
    void findErrorsIn(QVector<QString>& errors, QSharedPointer<Component> component);

private:

	// Disable copying.
	ComponentValidator(ComponentValidator const& rhs);
	ComponentValidator& operator=(ComponentValidator const& rhs);

    /*!
     *  Find errors in component VLNV.
     *
     *      @param [in] errors      List of found errors.
     *      @param [in] component   The selected component.
     */
    void findErrorsInVLNV(QVector<QString>& errors, QSharedPointer<Component> component) const;

    /*!
     *  Find errors in bus interfaces.
     *
     *      @param [in] errors      List of found errors.
     *      @param [in] component   The selected component.
     *      @param [in] context     Context to help locate the error.
     */
    void findErrorsInBusInterface(QVector<QString>& errors, QSharedPointer<Component> component,
        QString const& context) const;

    /*!
     *  Find errors in indirect interfaces.
     *
     *      @param [in] errors      List of found errors.
     *      @param [in] component   The selected component.
     *      @param [in] context     Context to help locate the error.
     */
    void findErrorsInIndirectInterfaces(QVector<QString>& errors, QSharedPointer<Component> component, QString const& context);

    /*!
     *  Find errors in channels.
     *
     *      @param [in] errors      List of found errors.
     *      @param [in] component   The selected component.
     *      @param [in] context     Context to help locate the error.
     */
    void findErrorsInChannels(QVector<QString>& errors, QSharedPointer<Component> component,
        QString const& context) const;

    /*!
     *  Find errors in remap states.
     *
     *      @param [in] errors      List of found errors.
     *      @param [in] component   The selected component.
     *      @param [in] context     Context to help locate the error.
     */
    void findErrorsInRemapStates(QVector<QString>& errors, QSharedPointer<Component> component,
        QString const& context) const;

    /*!
     *  Find errors in address spaces.
     *
     *      @param [in] errors      List of found errors.
     *      @param [in] component   The selected component.
     *      @param [in] context     Context to help locate the error.
     */
    void findErrorsInAddressSpaces(QVector<QString>& errors, QSharedPointer<Component> component,
        QString const& context) const;

    /*!
     *  Find errors in memory maps.
     *
     *      @param [in] errors      List of found errors.
     *      @param [in] component   The selected component.
     *      @param [in] context     Context to help locate the error.
     */
    void findErrorsInMemoryMaps(QVector<QString>& errors, QSharedPointer<Component> component,
        QString const& context) const;

    /*!
     *  Find errors in views.
     *
     *      @param [in] errors      List of found errors.
     *      @param [in] component   The selected component.
     *      @param [in] context     Context to help locate the error.
     */
    void findErrorsInViews(QVector<QString>& errors, QSharedPointer<Component> component, QString const& context)
        const;

    /*!
     *  Find errors in component instantiations.
     *
     *      @param [in] errors      List of found errors.
     *      @param [in] component   The selected component.
     *      @param [in] context     Context to help locate the error.
     */
    void findErrorsInComponentInstantiations(QVector<QString>& errors, QSharedPointer<Component> component,
        QString const& context) const;

    /*!
     *  Find errors in design instantiations.
     *
     *      @param [in] errors      List of found errors.
     *      @param [in] component   The selected component.
     *      @param [in] context     Context to help locate the error.
     */
    void findErrorsInDesignInstantiations(QVector<QString>& errors, QSharedPointer<Component> component,
        QString const& context) const;

    /*!
     *  Find errors in design configuration instantiations.
     *
     *      @param [in] errors      List of found errors.
     *      @param [in] component   The selected component.
     *      @param [in] context     Context to help locate the error.
     */
    void findErrorsInDesignConfigurationInstantiations(QVector<QString>& errors,
        QSharedPointer<Component> component, QString const& context) const;

    /*!
     *  Find errors in ports.
     *
     *      @param [in] errors      List of found errors.
     *      @param [in] component   The selected component.
     *      @param [in] context     Context to help locate the error.
     */
    void findErrorsInPorts(QVector<QString>& errors, QSharedPointer<Component> component, QString const& context)
        const;

    /*!
     *  Find errors in component generators.
     *
     *      @param [in] errors      List of found errors.
     *      @param [in] component   The selected component.
     *      @param [in] context     Context to help locate the error.
     */
    void findErrorsInComponentGenerators(QVector<QString>& errors, QSharedPointer<Component> component,
        QString const& context) const;

    /*!
     *  Find errors in choices.
     *
     *      @param [in] errors      List of found errors.
     *      @param [in] component   The selected component.
     *      @param [in] context     Context to help locate the error.
     */
    void findErrorsInChoices(QVector<QString>& errors, QSharedPointer<Component> component, QString const& context)
        const;

    /*!
     *  Find errors in file sets.
     *
     *      @param [in] errors      List of found errors.
     *      @param [in] component   The selected component.
     *      @param [in] context     Context to help locate the error.
     */
    void findErrorsInFileSets(QVector<QString>& errors, QSharedPointer<Component> component,
        QString const& context) const;

    /*!
     *  Find errors in CPUs.
     *
     *      @param [in] errors      List of found errors.
     *      @param [in] component   The selected component.
     *      @param [in] context     Context to help locate the error.
     */
    void findErrorsInCPUs(QVector<QString>& errors, QSharedPointer<Component> component, QString const& context)
        const;

    /*!
     *  Find errors in other clock drivers.
     *
     *      @param [in] errors      List of found errors.
     *      @param [in] component   The selected component.
     *      @param [in] context     Context to help locate the error.
     */
    void findErrorsInOtherClockDrivers(QVector<QString>& errors, QSharedPointer<Component> component,
        QString const& context) const;

    /*!
     *  Find errors in reset types.
     *
     *      @param [in] errors      List of found errors.
     *      @param [in] component   The selected component.
     *      @param [in] context     Context to help locate the error.
     */
    void findErrorsInResetTypes(QVector<QString>& errors, QSharedPointer<Component> component,
        QString const& context) const;

    /*!
     *  Find errors in parameters.
     *
     *      @param [in] errors      List of found errors.
     *      @param [in] component   The selected component.
     *      @param [in] context     Context to help locate the error.
     */
    void findErrorsInParameters(QVector<QString>& errors, QSharedPointer<Component> component,
        QString const& context) const;

    /*!
     *  Find errors in assertions.
     *
     *      @param [in] errors      List of found errors.
     *      @param [in] component   The selected component.
     *      @param [in] context     Context to help locate the error.
     */
    void findErrorsInAssertions(QVector<QString>& errors, QSharedPointer<Component> component,
        QString const& context) const;

    /*!
     *  Change the used component.
     *
     *      @param [in] newComponent    The new component.
     */
    void changeComponent(QSharedPointer<Component> newComponent);
    
    //-----------------------------------------------------------------------------
    // Data.
    //-----------------------------------------------------------------------------

    //! The last validated component.
    QSharedPointer<Component> component_;

    //! The used bus interface validator.
    QSharedPointer<BusInterfaceValidator> busInterfaceValidator_;

    //! The used indirect interface validator.
    QSharedPointer<IndirectInterfaceValidator> indirectInterfaceValidator_;

    //! The used channel validator.
    QSharedPointer<ChannelValidator> channelValidator_;

    //! The used remap state validator.
    QSharedPointer<RemapStateValidator> remapStateValidator_;

    //! The used address space validator.
    QSharedPointer<AddressSpaceValidator> addressSpaceValidator_;
    
    //! The used memory map validator.
    QSharedPointer<MemoryMapValidator> memoryMapValidator_;

    //! The used view validator.
    QSharedPointer<ViewValidator> viewValidator_;

    //! The used instantiations validator.
    QSharedPointer<InstantiationsValidator> instantiationsValidator_;

    //! The used port validator.
    QSharedPointer<PortValidator> portValidator_;

    //! The used component generator validator.
    QSharedPointer<ComponentGeneratorValidator> generatorValidator_;

    //! The used choice validator.
    QSharedPointer<ChoiceValidator> choiceValidator_;

    //! The used file set validator.
    QSharedPointer<FileSetValidator> fileSetValidator_;

    //! The used CPU validator.
    QSharedPointer<CPUValidator> cpuValidator_;

    //! The used other clock driver validator.
    QSharedPointer<OtherClockDriverValidator> otherClockDriverValidator_;

    //! The used parameter validator.
    QSharedPointer<ParameterValidator> parameterValidator_;

    //! The used assertion validator.
    QSharedPointer<AssertionValidator> assertionValidator_;
};

#endif // COMPONENTVALIDATOR_H
