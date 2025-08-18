//-----------------------------------------------------------------------------
// File: ComponentEditorByProject.cpp
//-----------------------------------------------------------------------------
// Project: AWM
// Author: P.H.G
// Date: 17.07.2024
//
// Description:
// The editor to edit/packet IP-Xact components.
//-----------------------------------------------------------------------------

#include <GlobalBuildOptions.h>

#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>

#include <IPXACTmodels/common/TagData.h>

#include <IPXACTmodels/Component/Component.h>
#include <IPXACTmodels/Component/FileSet.h>
#include <IPXACTmodels/Component/validators/ComponentValidator.h>

#include <KactusAPI/LibraryInterface.h>
#include <KactusAPI/ParameterCache.h>
#include <KactusAPI/IPXactSystemVerilogParser.h>
#include <KactusAPI/ComponentAndInstantiationsParameterFinder.h>

#include <KactusAPI/BusInterfaceInterface.h>
#include <KactusAPI/BusInterfaceInterfaceFactory.h>
#include <KactusAPI/AbstractionTypeInterface.h>
#include <KactusAPI/PortMapInterface.h>

#include <UI/ComponentEditor/treeStructure/componenteditorrootitem.h>
#include <UI/ComponentEditor/treeStructure/componenteditorgeneralitem.h>
#include <UI/ComponentEditor/treeStructure/componenteditorfilesetsitem.h>
#include <UI/ComponentEditor/treeStructure/componenteditorparametersitem.h>
//#include <UI/ComponentEditor/treeStructure/componenteditormemmapsitem.h>
//#include <UI/ComponentEditor/treeStructure/componenteditoraddrspacesitem.h>

#if COMPONENTEDITORVIEWSITEM_DISABLE_20231229 == 0
#include <UI/ComponentEditor/treeStructure/componenteditorviewsitem.h>
#endif

#include <UI/ComponentEditor/treeStructure/componenteditorportsitem.h>

#if COMPONENTEDITORBUSINTERFACESITEM_DISABLE_20231229 == 0
#include <UI/ComponentEditor/treeStructure/componenteditorbusinterfacesitem.h>
#endif

//#include <UI/ComponentEditor/treeStructure/componenteditorchannelsitem.h>
//#include <UI/ComponentEditor/treeStructure/ComponentEditorChoicesItem.h>
//#include <UI/ComponentEditor/treeStructure/componenteditorcpusitem.h>
//#include <UI/ComponentEditor/treeStructure/componenteditorotherclocksitem.h>
//#include <UI/ComponentEditor/treeStructure/ResetTypesItem.h>
//#include <UI/ComponentEditor/treeStructure/componenteditorcominterfacesitem.h>
//#include <UI/ComponentEditor/treeStructure/componenteditorapiinterfacesitem.h>
//#include <UI/ComponentEditor/treeStructure/componenteditorswpropertiesitem.h>
//#include <UI/ComponentEditor/treeStructure/ComponentEditorSystemViewsItem.h>
//#include <UI/ComponentEditor/treeStructure/RemapStatesItem.h>
#include <UI/ComponentEditor/treeStructure/componenteditortreemodel.h>
//#include <UI/ComponentEditor/treeStructure/InstantiationsItem.h>
//#include <UI/ComponentEditor/treeStructure/ComponentEditorIndirectInterfacesItem.h>
#include <UI/ComponentEditor/treeStructure/componenteditoritem.h>
#include <UI/ComponentEditor/treeStructure/ComponentEditorTreeSortProxyModel.h>
#include <UI/ComponentEditor/treeStructure/ComponentEditorTreeDelegate.h>

#include <UI/ComponentEditor/parameterReferenceTree/ComponentParameterReferenceTree.h>
#include <UI/ComponentEditor/parameterReferenceTree/ParameterReferenceTreeWindow.h>

#include <UI/ComponentEditor/general/generaleditor.h>

#include <UI/ComponentEditor/itemeditor.h>
#include <UI/ComponentEditor/itemvisualizer.h>

#include <UI/Dialogs/newObjectDialog/newobjectdialog.h>
#include <UI/Dialogs/comboSelector/comboselector.h>

#include "ComponentEditorByProject.h"


//-----------------------------------------------------------------------------
// Function: ComponentEditorByProject::ComponentEditorByProject()
//-----------------------------------------------------------------------------
ComponentEditorByProject::ComponentEditorByProject(LibraryInterface* designInterface,
                                                   LibraryInterface* libInterface,
                                                   QSharedPointer<Component> component,
                                                   QWidget *parent):
    TabDocument(parent, DOC_PROTECTION_SUPPORT),
    designInterface_(designInterface),
    libHandler_(libInterface),
    component_(component),
    navigationSplitter_(Qt::Horizontal, this),
    editorVisualizerSplitter_(Qt::Horizontal, &navigationSplitter_),
    navigationModel_(this),
    navigationView_(designInterface_, component->getVlnv(), &navigationSplitter_),
    proxy_(this),
    editorSlot_(&editorVisualizerSplitter_),
    visualizerSlot_(&editorVisualizerSplitter_),
    parameterFinder_(new ParameterCache(component_)),
    fullParameterFinder_(),
    referenceCounter_(0),
    expressionFormatter_(new ExpressionFormatter(parameterFinder_)),
    expressionParser_(new IPXactSystemVerilogParser(parameterFinder_)),
    validator_(expressionParser_, designInterface_),
    parameterReferenceTree_(0),
    parameterReferenceWindow_()
{
    fullParameterFinder_ = QSharedPointer<ComponentAndInstantiationsParameterFinder>(
        new ComponentAndInstantiationsParameterFinder(component));

    referenceCounter_ = QSharedPointer<ComponentParameterReferenceCounter>(
        new ComponentParameterReferenceCounter(fullParameterFinder_, component_));

    QSharedPointer<ExpressionFormatter> fullFormatter(new ExpressionFormatter(fullParameterFinder_));
    parameterReferenceTree_ =
        new ComponentParameterReferenceTree(component_, fullFormatter, referenceCounter_);
    parameterReferenceWindow_ = new ParameterReferenceTreeWindow(parameterReferenceTree_, this);

    supportedWindows_ |= TabDocument::VENDOREXTENSIONWINDOW;

    // these can be used when debugging to identify the objects
	setObjectName(tr("ComponentEditor"));
	navigationSplitter_.setObjectName(tr("NavigationSplitter"));
	editorVisualizerSplitter_.setObjectName(tr("EditorVisualizerSplitter"));
	editorSlot_.setObjectName(tr("EditorSlot"));
	visualizerSlot_.setObjectName(tr("VisualizerSlot"));

	Q_ASSERT(component_);
    Q_ASSERT(designInterface_);
	Q_ASSERT(libHandler_);

	// set the name and type for the tab
    setDocumentName( QString("[%3] %1 (%2) ").arg(component_->getVlnv().getName()).arg(
        component_->getVlnv().getVersion()).arg(designInterface_->getType()) );

    if (component_->getImplementation() == KactusAttribute::HW)
    {
	    setDocumentType(tr("HW Component"));
    }
    else if (component_->getImplementation() == KactusAttribute::SW)
    {
        setDocumentType(tr("SW Component"));
    }
    else if (component_->getImplementation() == KactusAttribute::SYSTEM)
    {
        setDocumentType(tr("Unmapped System"));
    }

    addRelatedVLNV(component_->getVlnv());

    setupLayout();

	// set the component to be displayed in the navigation model
    navigationModel_.setRootItem(createNavigationRootForComponent(component_));

	QSettings settings;
	setRowVisibility(settings);
	
    // Set source model for the proxy.
    proxy_.setSourceModel(&navigationModel_);

    navigationView_.setItemDelegate(new ComponentEditorTreeDelegate(this));

	// connect the view with the model (proxy) and sort.
	navigationView_.setModel(&proxy_);
    navigationView_.sortByColumn(0, Qt::AscendingOrder);

	// when starting the component editor open the general editor.
	onItemActivated(proxy_.index(0, 0, QModelIndex()));

	// navigation model may request an item to be expanded
	connect(&navigationModel_, SIGNAL(expandItem(const QModelIndex&)),
		this, SLOT(onExpand(const QModelIndex&)), Qt::UniqueConnection);

	connect(&navigationView_, SIGNAL(activated(const QModelIndex&)),
		this, SLOT(onItemActivated(const QModelIndex&)), Qt::UniqueConnection);

	connect(&navigationModel_, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
		    this, SIGNAL(contentChanged()), Qt::UniqueConnection);
    connect(&navigationModel_, SIGNAL(contentChanged()), this, SIGNAL(contentChanged()), Qt::UniqueConnection);
    connect(&navigationModel_, SIGNAL(helpUrlRequested(QString const&)),
            this, SIGNAL(helpUrlRequested(QString const&)), Qt::UniqueConnection);
	connect(&navigationModel_, SIGNAL(errorMessage(const QString&)),
		this, SLOT(onErrorDialog(const QString&)), Qt::UniqueConnection);

    connect(&navigationModel_, SIGNAL(openCSource(QString const&, QSharedPointer<Component>)),
            this, SIGNAL(openCSource(QString const&, QSharedPointer<Component>)), Qt::UniqueConnection);

	connect(&navigationModel_, SIGNAL(selectItem(const QModelIndex&)),
		this, SLOT(onNavigationTreeSelection(const QModelIndex&)), Qt::UniqueConnection);
	connect(&navigationModel_, SIGNAL(openDesign(const VLNV&, const QString&)),
		this, SIGNAL(openDesign(const VLNV&, const QString&)), Qt::UniqueConnection);
	connect(&navigationModel_, SIGNAL(openBus(const VLNV&)),
		this, SIGNAL(openBus(const VLNV&)), Qt::UniqueConnection);
	connect(&navigationModel_, SIGNAL(openComDefinition(const VLNV&)),
		this, SIGNAL(openComDefinition(const VLNV&)), Qt::UniqueConnection);
	connect(&navigationModel_, SIGNAL(openSWDesign(const VLNV&, const QString&)),
		this, SIGNAL(openSWDesign(const VLNV&, const QString&)), Qt::UniqueConnection);
	connect(&navigationModel_, SIGNAL(openSystemDesign(const VLNV&, const QString&)),
		this, SIGNAL(openSystemDesign(const VLNV&, const QString&)), Qt::UniqueConnection);
}

//-----------------------------------------------------------------------------
// Function: ComponentEditorByProject::~ComponentEditorByProject()
//-----------------------------------------------------------------------------
ComponentEditorByProject::~ComponentEditorByProject()
{

}

//-----------------------------------------------------------------------------
// Function: ComponentEditorByProject::getIdentifyingVLNV()
//-----------------------------------------------------------------------------
VLNV ComponentEditorByProject::getIdentifyingVLNV() const
{
    return getDocumentVLNV();
}

//-----------------------------------------------------------------------------
// Function: ComponentEditorByProject::getDocumentVLNV()
//-----------------------------------------------------------------------------
VLNV ComponentEditorByProject::getDocumentVLNV() const
{
	return component_->getVlnv();
}

//-----------------------------------------------------------------------------
// Function: ComponentEditorByProject::isHWImplementation()
//-----------------------------------------------------------------------------
bool ComponentEditorByProject::isHWImplementation() const
{
    return component_->getImplementation() == KactusAttribute::HW;
}

//-----------------------------------------------------------------------------
// Function: ComponentEditorByProject::refresh()
//-----------------------------------------------------------------------------
void ComponentEditorByProject::refresh()
{
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

	// remember the locked state
	bool locked = isProtected();
	
	// clear the slots 
	editorSlot_.setWidget(NULL);
	visualizerSlot_.setWidget(NULL);

	// get the VLNV of the component
	VLNV compVLNV = component_->getVlnv();

	// get the original model of the component
    QSharedPointer<Component> comp = designInterface_->getModel<Component>(compVLNV);
	Q_ASSERT(comp.isNull() == false);

	// rebuild the navigation tree
	navigationModel_.setRootItem(createNavigationRootForComponent(comp));
	component_.clear();
	component_ = comp;

    referenceCounter_->setComponent(component_);
    parameterReferenceTree_->setComponent(component_);

    parameterFinder_->setComponent(comp);
    fullParameterFinder_->setComponent(comp);

	// open the general editor.
	onItemActivated(proxy_.index(0, 0, QModelIndex()));

	// the document is no longer modified
	setModified(false);
	TabDocument::refresh();

	// set the protection state to same as before refreshing
	setProtection(locked);

	QApplication::restoreOverrideCursor();
}

//-----------------------------------------------------------------------------
// Function: ComponentEditorByProject::applySettings()
//-----------------------------------------------------------------------------
void ComponentEditorByProject::applySettings(QSettings& settings)
{
	setRowVisibility(settings);
}

//-----------------------------------------------------------------------------
// Function: ComponentEditorByProject::getHwItemNames()
//-----------------------------------------------------------------------------
QStringList ComponentEditorByProject::getHwItemNames()
{
	QStringList itemNames;

	itemNames << "File_sets" << "Choices" << "Parameters" << "Memory_maps" << 
		"Address_spaces" << "Instantiations" << "Views" << "Software_views" << "System_views" << "Ports" <<
        "Bus_interfaces" << "Indirect_interfaces" << "Channels" << "Remap_states" << "Cpus" <<
        "Other_clock_drivers" << "Reset_types" << "COM_interfaces" << "Software_properties";

	return itemNames;
}

//-----------------------------------------------------------------------------
// Function: ComponentEditorByProject::getSwItemNames()
//-----------------------------------------------------------------------------
QStringList ComponentEditorByProject::getSwItemNames()
{
	QStringList itemNames;

	itemNames << "File_sets" << "Choices" << "Parameters" << "Software_views" << "COM_interfaces" << 
		"API_interfaces" << "Software_properties";

	return itemNames;
}

//-----------------------------------------------------------------------------
// Function: ComponentEditorByProject::validate()
//-----------------------------------------------------------------------------
bool ComponentEditorByProject::validate(QVector<QString>& errorList)
{
    if (!validator_.validate(component_))
    {
        validator_.findErrorsIn(errorList, component_);
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------
// Function: ComponentEditorByProject::save()
//-----------------------------------------------------------------------------
bool ComponentEditorByProject::save()
{
    if (designInterface_->writeModelToFile(component_))
    {
        navigationModel_.clearItemsModified();
		return TabDocument::save();
	}
	else
    {
//		emit errorMessage(tr("Component was not saved to disk."));
		return false;
	}
}

//-----------------------------------------------------------------------------
// Function: ComponentEditorByProject::saveAs()
//-----------------------------------------------------------------------------
bool ComponentEditorByProject::saveAs()
{
	// Ask the user for a new VLNV along with attributes and directory.
    KactusAttribute::ProductHierarchy prodHier = component_->getHierarchy();
    KactusAttribute::Firmness firmness = component_->getFirmness();
    QVector<TagData> tags = component_->getTags();

	VLNV vlnv;
	QString directory;

    if (component_->getImplementation() == KactusAttribute::HW)
    {
	    if (!NewObjectDialog::saveAsDialog(
            parentWidget(), designInterface_, component_->getVlnv(), prodHier, firmness, tags, vlnv, directory))
        {
		    return false;
	    }
    }
    else
    {
        if (!NewObjectDialog::saveAsDialog(parentWidget(), designInterface_, component_->getVlnv(), vlnv, directory))
        {
            return false;
        }
    }

	// save pointer to the old component
	QSharedPointer<Component> oldComponent = component_;

	// create copies of the objects so saving is not done to the original component
	component_ = QSharedPointer<Component>(new Component(*component_));

	// make sure the vlnv type is correct
	VLNV compVLNV = vlnv;
	compVLNV.setType(VLNV::COMPONENT);

	// update the vlnv
	component_->setVlnv(vlnv);
    component_->setHierarchy(prodHier);
    component_->setFirmness(firmness);

	// get the paths to the original xml file
    QFileInfo sourceInfo(designInterface_->getPath(oldComponent->getVlnv()));
	QString sourcePath = sourceInfo.absolutePath();

	// update the file paths and copy necessary files
    updateComponentFiles(component_, oldComponent, sourcePath, directory);

	// Write the component to a file.
    if (designInterface_->writeModelToFile(directory, component_))
    {
		setDocumentName(compVLNV.getName() + " (" + compVLNV.getVersion() + ")");
        navigationModel_.clearItemsModified();
		return TabDocument::saveAs();
	}
	else
    {
//		emit errorMessage(tr("Component was not saved to disk."));
		return false;
	}

    clearRelatedVLNVs();
    addRelatedVLNV(compVLNV);

    return true;
}

//-----------------------------------------------------------------------------
// Function: ComponentEditorByProject::onNavigationTreeSelection()
//-----------------------------------------------------------------------------
void ComponentEditorByProject::onNavigationTreeSelection( const QModelIndex& index )
{
	navigationView_.setCurrentIndex(index);
	onItemActivated(index);
}

//-----------------------------------------------------------------------------
// Function: ComponentEditorByProject::onExpand()
//-----------------------------------------------------------------------------
void ComponentEditorByProject::onExpand(const QModelIndex& index)
{    
    // Expand parents until root is hit.            
    QModelIndex parentIndex = index;
    while(parentIndex.isValid())
    {
        navigationView_.expand(proxy_.mapFromSource(parentIndex));
        parentIndex = parentIndex.parent();
    }    
}

//-----------------------------------------------------------------------------
// Function: ComponentEditorByProject::onErrorDialog()
//-----------------------------------------------------------------------------
void ComponentEditorByProject::onErrorDialog(const QString& message)
{
    QMessageBox::warning(this, tr("Component editor"), message);
}

//-----------------------------------------------------------------------------
// Function: ComponentEditorByProject::onItemActivated()
//-----------------------------------------------------------------------------
void ComponentEditorByProject::onItemActivated( const QModelIndex& index )
{
    // If tree proxy model index is used, the item must be retrieved from the source model.
    const ComponentEditorTreeProxyModel* indexModel =
        dynamic_cast<const ComponentEditorTreeProxyModel*>(index.model());
	ComponentEditorItem* item = 0;
    if ( indexModel == 0 )
    {
        item = static_cast<ComponentEditorItem*>(index.internalPointer());
    }
    else
    {
        item = static_cast<ComponentEditorItem*>(proxy_.mapToSource(index).internalPointer());
    }

    Q_ASSERT(item);

	QList<int> editorVisualizerSizes;

	ItemEditor* editor = item->editor();
	if (editor)
    {
		// the width of the previous editor
		QWidget* prevEditor = editorSlot_.getWidget();
		if (prevEditor)
        {
			int prevWidth = prevEditor->size().width();
			editorVisualizerSizes.append(prevWidth);
		}
		// if there was no previous editor then use the size hint
		else
        {
			editorVisualizerSizes.append(editor->sizeHint().width());
		}
		editor->refresh();
	}
	// if there is no editor then hide the editor slot
	else
    {
		editorVisualizerSizes.append(0);
	}
	editorSlot_.setWidget(editor);

	ItemVisualizer* visualizer = item->visualizer();
	if (visualizer)
    {
		// the width of the previous visualizer
		QWidget* prevVisualizer = visualizerSlot_.getWidget();
		if (prevVisualizer) {
			int prevWidth = prevVisualizer->size().width();
			editorVisualizerSizes.append(prevWidth);
		}
		// if there was no previous visualizer then use the size hint
		else
        {
			editorVisualizerSizes.append(visualizer->sizeHint().width());
		}
	}
	// if there is no visualizer then hide the visualizer slot
	else
    {
		editorVisualizerSizes.append(0);
	}
	visualizerSlot_.setWidget(visualizer);

	editorVisualizerSplitter_.setSizes(editorVisualizerSizes);
}

//-----------------------------------------------------------------------------
// Function: ComponentEditorByProject::setProtection()
//-----------------------------------------------------------------------------
void ComponentEditorByProject::setProtection( bool locked )
{
    TabDocument::setProtection(locked);

    // tell tree items to change the state of the editors
    navigationModel_.setLocked(locked);
}

//-----------------------------------------------------------------------------
// Function: ComponentEditorByProject::setRowVisibility()
//-----------------------------------------------------------------------------
void ComponentEditorByProject::setRowVisibility(QSettings& settings)
{
    QString workSpace = settings.value("Workspaces/CurrentWorkspace").toString();

    settings.beginGroup("Workspaces/" + workSpace + "/ComponentEditorFilters");

    if (isHWImplementation())
    {
        settings.beginGroup("HW");
        settings.beginGroup(KactusAttribute::hierarchyToString(component_->getHierarchy()));
    }
    else
    {
        settings.beginGroup("SW");
    }

    // List of the hidden rows in component editor.
    QStringList hiddenRows;
    for (QString name : settings.childKeys()) 
    {
        if (settings.value(name, true).toBool() == false)
        {
            QString wordReplaced = name.replace("_"," ");
            hiddenRows.append(wordReplaced);
        }
    }

    if (isHWImplementation())
    {
        // End hierarchy group.
        settings.endGroup();
    }

    // End hardware or software group.
    settings.endGroup();

    // End Workspace/CurrentWorkspace/ComponentEditorFilters group.
    settings.endGroup();

    proxy_.setRowVisibility( hiddenRows );
}

//-----------------------------------------------------------------------------
// Function: ComponentEditorByProject::createNavigationRootForComponent()
//-----------------------------------------------------------------------------
QSharedPointer<ComponentEditorRootItem> ComponentEditorByProject::createNavigationRootForComponent(
    QSharedPointer<Component> component)
{
    ComponentEditorRootItem* root = new ComponentEditorRootItem(designInterface_, component, &navigationModel_);

    QSharedPointer<ComponentEditorGeneralItem> generalItem(
        new ComponentEditorGeneralItem(&navigationModel_, designInterface_, component, root));

    connect(generalItem.data(), SIGNAL(changeVendorExtensions(QString const&, QSharedPointer<Extendable>)),
        this, SIGNAL(changeVendorExtensions(QString const&, QSharedPointer<Extendable>)), Qt::UniqueConnection);

    root->addChildItem(generalItem);

    GeneralEditor* genEditor = static_cast<GeneralEditor*>(root->child(0)->editor());

    connect(genEditor, SIGNAL(hierarchyChanged(QSettings&)), this, SLOT(setRowVisibility(QSettings&)));

    root->addChildItem(QSharedPointer<ComponentEditorFileSetsItem>( new ComponentEditorFileSetsItem(
        &navigationModel_, designInterface_, component, referenceCounter_, parameterFinder_,
        expressionParser_, expressionFormatter_, root)));

    if (component->getImplementation() == KactusAttribute::HW)
    {

#if COMPONENTEDITORCHOICESITEM_DISABLE_20231229 == 0
        root->addChildItem(QSharedPointer<ComponentEditorChoicesItem>(
            new ComponentEditorChoicesItem(&navigationModel_, designInterface_, component, expressionParser_, root)));
#else
        // ComponentEditorChoicesItem remove
#endif

        QSharedPointer<ComponentEditorParametersItem> parametersItem(new ComponentEditorParametersItem(
            &navigationModel_, designInterface_, component, referenceCounter_, parameterFinder_, expressionParser_,
            expressionFormatter_, root));

        root->addChildItem(parametersItem);

        connect(parametersItem.data(), SIGNAL(openReferenceTree(QString const&, QString const&)),
            parameterReferenceWindow_, SLOT(openReferenceTree(QString const&, QString const)),
            Qt::UniqueConnection);

#if COMPONENTEDITORMEMMAPSITEM_DISABLE_20231229 == 0
        root->addChildItem(QSharedPointer<ComponentEditorMemMapsItem>(new ComponentEditorMemMapsItem(
            &navigationModel_, designInterface_, component, referenceCounter_, parameterFinder_, expressionFormatter_,
            expressionParser_, root)));
#else
        // ComponentEditorMemMapsItem remove
#endif

#if COMPONENTEDITORADDRSPACESITEM_DISABLE_20231229 == 0

        root->addChildItem(QSharedPointer<ComponentEditorAddrSpacesItem>(new ComponentEditorAddrSpacesItem(
            &navigationModel_, designInterface_, component, referenceCounter_, parameterFinder_, expressionFormatter_,
            expressionParser_, root)));

#else
        // ComponentEditorAddrSpacesItem remove
#endif
    }

#if INSTANTIATIONSITEM_DISABLE_20231229 == 0
    QSharedPointer<InstantiationsItem> instantiationsItem (new InstantiationsItem(&navigationModel_, designInterface_,
        component, referenceCounter_, parameterFinder_, expressionFormatter_, expressionParser_, root));

    root->addChildItem(instantiationsItem);

    connect(instantiationsItem.data(), SIGNAL(openReferenceTree(QString const&, QString const&)),
        parameterReferenceWindow_, SLOT(openReferenceTree(QString const&, QString const&)), Qt::UniqueConnection);
#else
    // InstantiationsItem remove
#endif

#if COMPONENTEDITORVIEWSITEM_DISABLE_20231229 == 0
    QSharedPointer<ComponentEditorViewsItem> viewsItem(new ComponentEditorViewsItem(&navigationModel_, designInterface_,
        component, referenceCounter_, parameterFinder_, expressionFormatter_, expressionParser_, root));

    root->addChildItem(viewsItem);

    connect(viewsItem.data(), SIGNAL(openReferenceTree(QString const&, QString const&)),
        parameterReferenceWindow_, SLOT(openReferenceTree(QString const&, QString const&)), Qt::UniqueConnection);
#else
    // ComponentEditorViewsItem remove
#endif

#if COMPONENTEDITORSYSTEMVIEWSITEM_DISABLE_20231229 == 0
    if (component->getImplementation() != KactusAttribute::SW)
    {
        root->addChildItem(QSharedPointer<ComponentEditorSystemViewsItem>(new ComponentEditorSystemViewsItem(
            &navigationModel_, designInterface_, component, parameterFinder_, expressionParser_, expressionFormatter_,
            root)));
    }
#else
    // ComponentEditorSystemViewsItem remove
#endif

    if (component->getImplementation() == KactusAttribute::HW)
    {
        BusInterfaceInterface* busInterface = BusInterfaceInterfaceFactory::createBusInterface(
            parameterFinder_, expressionFormatter_, expressionParser_, component, libHandler_);

        AbstractionTypeInterface* absTypeInterface = busInterface->getAbstractionTypeInterface();
        if (absTypeInterface)
        {
            PortMapInterface* portMapInterface =
                dynamic_cast<PortMapInterface*>(absTypeInterface->getPortMapInterface());

            if (portMapInterface)
            {
                QSharedPointer<ComponentEditorPortsItem> portsItem(new ComponentEditorPortsItem(&navigationModel_,
                    libHandler_, component, referenceCounter_, parameterFinder_, expressionFormatter_,
                    expressionParser_, busInterface, root));

                root->addChildItem(portsItem);
                connect(portsItem.data(), SIGNAL(createInterface()),
                    root, SLOT(onInterfaceAdded()), Qt::UniqueConnection);

                connect(portsItem.data(),
                    SIGNAL(changeVendorExtensions(QString const&, QSharedPointer<Extendable>)),
                    this, SIGNAL(changeVendorExtensions(QString const&, QSharedPointer<Extendable>)),
                    Qt::UniqueConnection);

#if COMPONENTEDITORBUSINTERFACESITEM_DISABLE_20231229 == 0
                QSharedPointer<ComponentEditorBusInterfacesItem> busInterfaceItem(
                    new ComponentEditorBusInterfacesItem(busInterface, portMapInterface, &navigationModel_,
                        libHandler_, component, referenceCounter_, parameterFinder_, expressionFormatter_,
                        expressionParser_, root, parentWidget()));

                root->addChildItem(busInterfaceItem);

                connect(busInterfaceItem.data(), SIGNAL(openReferenceTree(QString const&, QString const&)),
                    parameterReferenceWindow_, SLOT(openReferenceTree(QString const&, QString const&)),
                    Qt::UniqueConnection);
#else
                // ComponentEditorBusInterfacesItem remove
#endif

#if COMPONENTEDITORINDIRECTINTERFACESITEM_DISABLE_20231229 == 0
                QSharedPointer<ComponentEditorIndirectInterfacesItem> indirectInterfacesItem(
                    QSharedPointer<ComponentEditorIndirectInterfacesItem>(
                        new ComponentEditorIndirectInterfacesItem(&navigationModel_, libHandler_, component,
                            referenceCounter_, parameterFinder_, expressionFormatter_, expressionParser_,
                            busInterface, root, parentWidget())));

                root->addChildItem(indirectInterfacesItem);

                connect(indirectInterfacesItem.data(), SIGNAL(openReferenceTree(QString const&, QString const&)),
                    parameterReferenceWindow_, SLOT(openReferenceTree(QString const&, QString const&)),
                    Qt::UniqueConnection);
#else
                // ComponentEditorIndirectInterfacesItem remove
#endif
            }
        }

#if COMPONENTEDITORCHANNELSITEM_DISABLE_20231229 == 0
        root->addChildItem(QSharedPointer<ComponentEditorChannelsItem>(
            new ComponentEditorChannelsItem(&navigationModel_, designInterface_, component, expressionParser_, root)));
#else
        // ComponentEditorChannelsItem remove
#endif

#if REMAPSTATESITEM_DISABLE_20231229 == 0
        root->addChildItem(QSharedPointer<RemapStatesItem>(
            new RemapStatesItem(&navigationModel_, designInterface_, component, referenceCounter_, parameterFinder_,
            expressionFormatter_, expressionParser_, root)));
#else
        // RemapStatesItem remove
#endif

#if COMPONENTEDITORCPUSITEM_DISABLE_20231229 == 0
        root->addChildItem(QSharedPointer<ComponentEditorCpusItem>(
            new ComponentEditorCpusItem(&navigationModel_, designInterface_, component, expressionParser_, root)));
#else
        // ComponentEditorCpusItem remove
#endif

#if COMPONENTEDITOROTHERCLOCKSITEM_DISABLE_20231229 == 0
        root->addChildItem(QSharedPointer<ComponentEditorOtherClocksItem>(
            new ComponentEditorOtherClocksItem(&navigationModel_, designInterface_, component, expressionParser_, root)));
#else
        // ComponentEditorOtherClocksItem remove
#endif

#if RESETTYPESITEM_DISABLE_20231229 == 0
        root->addChildItem(QSharedPointer<ResetTypesItem>(
            new ResetTypesItem(&navigationModel_, designInterface_, component, validator_, root)));
#else
        // ResetTypesItem remove
#endif
    }
    else if (component->getImplementation() == KactusAttribute::SW)
    {
#if COMPONENTEDITORAPIINTERFACESITEM_DISABLE_20231229 == 0
        root->addChildItem(QSharedPointer<ComponentEditorAPIInterfacesItem>(
            new ComponentEditorAPIInterfacesItem(&navigationModel_, designInterface_, component, root)));
#else
        // ComponentEditorAPIInterfacesItem remove
#endif
    }  

#if COMPONENTEDITORCOMINTERFACESITEM_DISABLE_20231229 == 0
    root->addChildItem(QSharedPointer<ComponentEditorComInterfacesItem>(
        new ComponentEditorComInterfacesItem(&navigationModel_, designInterface_, component, root)));
#else
    // ComponentEditorComInterfacesItem remove
#endif

#if COMPONENTEDITORSWPROPERTIESITEM_DISABLE_20231229 == 0
    root->addChildItem(QSharedPointer<ComponentEditorSWPropertiesItem>(
        new ComponentEditorSWPropertiesItem(&navigationModel_, designInterface_, component, root)));
#else
    // ComponentEditorSWPropertiesItem remove
#endif

    return QSharedPointer<ComponentEditorRootItem>(root);
}

//-----------------------------------------------------------------------------
// Function: ComponentEditorByProject::setupLayout()
//-----------------------------------------------------------------------------
void ComponentEditorByProject::setupLayout()
{
    // the top layout contains only the navigation splitter
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(&navigationSplitter_);
    layout->setContentsMargins(5, 5, 5, 5);

    // editor visualizer splitter contains the editor- and visualizer slots
    editorVisualizerSplitter_.addWidget(&editorSlot_);
    editorVisualizerSplitter_.addWidget(&visualizerSlot_);

    // navigation splitter contains the navigation tree and the other splitter
    navigationSplitter_.addWidget(&navigationView_);
    navigationSplitter_.addWidget(&editorVisualizerSplitter_);
    navigationSplitter_.setStretchFactor(1, 1);

    // The navigation tree takes 1/5 of the space available and editor and 
    // visualizer take the rest
    QList<int> navigationSize;
    navigationSize.append(ComponentTreeView::DEFAULT_WIDTH);
    navigationSize.append(4 * ComponentTreeView::DEFAULT_WIDTH);
    navigationSplitter_.setSizes(navigationSize);
}

//-----------------------------------------------------------------------------
// Function: ComponentEditorByProject::updateComponentFiles()
//-----------------------------------------------------------------------------
void ComponentEditorByProject::updateComponentFiles(QSharedPointer<Component> targetComponent,
    QSharedPointer<Component> otherComponent, QString const& sourcePath, QString const& targetPath)
{
    // can't operate on the same component
    if (targetComponent == otherComponent)
    {
        return;
    }

    // get the files from the other component
    for (QString const& file : getComponentFileNames(otherComponent))
    {
        // get the absolute path to the file
        QDir source(sourcePath);
        QString absoluteSource = source.absoluteFilePath(file);

        // if file is located under the source directory
        if (!file.contains(QString("../")))
        {

            QDir target(targetPath);
            QString absoluteTarget = target.absoluteFilePath(file);

            QFileInfo targetInfo(absoluteTarget);

            target.mkpath(targetInfo.absolutePath());
            QFile::copy(absoluteSource, absoluteTarget);

        }
        // if file is higher in directory hierarchy than the source directory
        else
        {
            // update the file name
            changeFileName(file, absoluteSource, targetComponent);
        }
    }
}

//-----------------------------------------------------------------------------
// Function: ComponentEditorByProject::getComponentFileNames()
//-----------------------------------------------------------------------------
QStringList ComponentEditorByProject::getComponentFileNames(QSharedPointer<Component> component) const
{
    QStringList fileNames;
    for (QSharedPointer<FileSet> fileSet : *component->getFileSets())
    {
        fileNames.append(fileSet->getFileNames());
    }

    return fileNames;
}

//-----------------------------------------------------------------------------
// Function: ComponentEditorByProject::changeFileName()
//-----------------------------------------------------------------------------
void ComponentEditorByProject::changeFileName(QString const& from, QString const& to, QSharedPointer<Component> component)
    const
{
    for (QSharedPointer<FileSet> fileSet : *component->getFileSets())
    {
        if (fileSet->contains(from))
        {
            fileSet->changeFileName(from, to);
            return;
        }
    }
}

//-----------------------------------------------------------------------------
// Function: ComponentEditorByProject::openItemEditor()
//-----------------------------------------------------------------------------
void ComponentEditorByProject::openItemEditor(QVector<QString> itemIdentifierChain)
{
    QModelIndex itemIndex = navigationModel_.getIndexOfItem(itemIdentifierChain);
    if (itemIndex.isValid())
    {
        onExpand(itemIndex);
        onItemActivated(itemIndex);
    }
}

//-----------------------------------------------------------------------------
// Function: ComponentEditorByProject::getComponent()
//-----------------------------------------------------------------------------
QSharedPointer<Component> ComponentEditorByProject::getComponent() const
{
    return component_;
}
