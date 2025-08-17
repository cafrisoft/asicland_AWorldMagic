//-----------------------------------------------------------------------------
// File: ComponentInstanceEditor.h
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Antti Kamppi
// Date: 11.08.2011
//
// Description:
// Editor to edit the details of a component instance within a design.
//-----------------------------------------------------------------------------

#ifndef COMPONENTINSTANCEEDITOR_H
#define COMPONENTINSTANCEEDITOR_H

#include <QWidget>
#include <QComboBox>
#include <QGroupBox>
#include <QSharedPointer>

#include <KactusAPI/ComponentParameterFinder.h>

#include <UI/Common/IEditProvider.h>

#include <UI/Widgets/vlnvDisplayer/vlnvdisplayer.h>
#include <UI/Widgets/nameGroupBox/namegroupbox.h>

#include <UI/ComponentEditor/common/ConfigurableElementFinder.h>
#include <UI/ComponentEditor/software/PropertyValueEditor.h>

#include <UI/Common/ComponentInstanceEditor/ComponentInstanceConfigurableElementsEditor.h>


class ComponentItem;
class ParameterFinder;
class TopComponentParameterFinder;
class Design;
class DesignCompletionModel;
class View;
class ListParameterFinder;

//-----------------------------------------------------------------------------
//! Editor to edit the details of a component instance within a design.
//-----------------------------------------------------------------------------
class ComponentInstanceEditor : public QWidget
{
	Q_OBJECT

public:

	/*!
     *  The constructor
	 *
	 *      @param [in] parent Pointer to the owner of this widget.
	 */
	ComponentInstanceEditor(QWidget *parent);
	
	//! The destructor.
	virtual ~ComponentInstanceEditor() = default;

	/*!
     *  Set the component to be edited.
	 *
	 *      @param [in] component       Pointer to the component instance being edited.
     *      @param [in] editProvider    Interface for providing editing capabilities (undo & redo).
     *      @param [in] design          Design containing the component instance.
	 */
    void setComponentInstance(ComponentItem* component, QSharedPointer<IEditProvider> editProvider,
        QSharedPointer<Design> design, QSharedPointer<DesignConfiguration> designConfiguration, QString const& activeViewName);

    /*!
     *  Sets the top component for the instances.
     *
     *      @param [in] topComponent            The top component.
     *      @param [in] editProvider            The generic edit provider.
     */
    void setContext(QSharedPointer<Component> topComponent, QSharedPointer<IEditProvider> editProvider);

    /*!
     *  Sets the protection state for the instance editor.
     *
     *      @param [in] locked   Locks/unlock the editor.
     */
    void setProtection(bool locked);

public slots:
        
    /*!
     *  Sets the active view to use for the top component.
     *
     *      @param [in] activeView   The name of the active view.
     */
    void setTopComponentActiveView(QSharedPointer<View> activeView);

	/*!
     *  Clear the editor so no instance details are shown
	 */
	void clear();

signals:

	/*!
     *  Emitted when contents of the editor changes.
     */
	void contentChanged();
    
    /*!
     *  Increase the amount of references to a parameter with the matching ID.
     *
     *      @param [in] id  ID of the parameter, whose references are being increased.
     */
    void increaseReferences(QString id);

    /*!
     *  Decrease the amount of references to a parameter with a matching ID.
     *
     *      @param [in] id  ID of the parameter, whose references are being decreased.
     */
    void decreaseReferences(QString id);

private slots:

	//! Handler when user changes the name of the component instance.
    void onNameChanged();

	//! Handler when user changes the display name of the component instance.
    void onDisplayNameChanged();

	//! Handler when user changes the description of the component instance.
    void onDescriptionChanged();

    //! Handler when the user changes any property values.
    void onPropertyValuesChanged();

    //! Handler when the user changes the file set reference.
    void onFileSetRefChanged(QString const& fileSetRef);

    //! Updates the file set reference when it is changed from the component.
    void updateFileSetRef(QString const& fileSetRef);

private:
	//! No copying.
	ComponentInstanceEditor(const ComponentInstanceEditor& other);
    void setupLayout();

	//! No assignment.
	ComponentInstanceEditor& operator=(const ComponentInstanceEditor& other);
    
	//! Pointer to the component instance being edited.
	ComponentItem* component_;

	//! The widget to display the vlnv of the component instance
	VLNVDisplayer* vlnvDisplayer_;

	//! The widget to set the instance name, display name and description.
	NameGroupBox* nameGroup_;

    QLabel* activeViewLabel_;

	//! The widget to set the configurable elements of a component instance.
    ComponentInstanceConfigurableElementsEditor* configurableElements_;

    //! SW group.
    QGroupBox* swGroup_;

    //! File set reference combo box.
    QComboBox* fileSetRefCombo_;

    //! Property value editor for SW properties.
    PropertyValueEditor* propertyValueEditor_;

	//! Pointer to the generic edit provider that manages the undo/redo stack.
	QSharedPointer<IEditProvider> editProvider_;

    //! The parameter finder for component instances.
    QSharedPointer<ComponentParameterFinder> instanceFinder_;

    //! The parameter finder for configurable element values.
    QSharedPointer<ConfigurableElementFinder> elementFinder_;

    //! The parameter finder for component instances.
    QSharedPointer<TopComponentParameterFinder> topFinder_;

    //! The parameter finder for design.
    QSharedPointer<ListParameterFinder> designParameterFinder_;

    //! The current top component.
    QSharedPointer<Component> topComponent_;

    //! The design containing the component instance.
    QSharedPointer<Design> containingDesign_;
};

#endif // COMPONENTINSTANCEEDITOR_H
