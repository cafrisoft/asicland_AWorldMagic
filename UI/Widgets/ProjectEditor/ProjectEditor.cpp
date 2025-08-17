/*
 *      Created on: 11.1.2024
 *      Author: P.H.G
 *      Filename: NewProjectPage.h
 *      Project: AWM
 *      Description :
 *          Project editor group box.
 */

#include "ProjectEditor.h"

#include <KactusAPI/LibraryInterface.h>
#include <KactusAPI/LibraryItem.h>
#include <KactusAPI/LibraryHandler.h>

#include <IPXACTmodels/common/validators/namevalidator.h>
#include <IPXACTmodels/common/validators/NMTokenValidator.h>

#include <IPXACTmodels/kactusExtensions/KactusAttribute.h>

#include <IPXACTmodels/AbstractionDefinition/AbstractionDefinition.h>
#include <IPXACTmodels/designConfiguration/DesignConfiguration.h>

#include <IPXACTmodels/Design/Design.h>

#include <UI/Widgets/assistedLineEdit/AssistedLineEdit.h>

#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QMimeData>
#include <QSharedPointer>
#include <QMessageBox>
#include <QCoreApplication>

//-----------------------------------------------------------------------------
// Function: ProjectEditor::ProjectEditor()
//-----------------------------------------------------------------------------
ProjectEditor::ProjectEditor( LibraryInterface* libHandler, QWidget* parentWnd, QWidget* parent):
QGroupBox(parent),
//contentTypes_(),
dirty_(false),
//dataTree_(),
authorEdit_(0),
//authorMatcher_(),
descriptionEdit_(0),
//descriptionMatcher_(),
nameEdit_(0),
nameExtensionLabel_(this),
//nameMatcher_(),
versionEdit_(0),
//versionMatcher_(),
handler_(libHandler),
implementationFilterEnabled_(false),
implementationFilter_(KactusAttribute::HW)
{
    Q_ASSERT(libHandler != 0);

    nameExtensionLabel_.setVisible(false);

    // Set group box settings.
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    setTitle(tr("Project"));

    // Initialize the widgets and the connections between widgets.
    initWidgets(parentWnd);
    initConnections();

	// accept drops from drag & drop
	setAcceptDrops(true);

//    authorMatcher_.setDataNode(&dataTree_);
}

//-----------------------------------------------------------------------------
// Function: ProjectEditor::~ProjectEditor()
//-----------------------------------------------------------------------------
ProjectEditor::~ProjectEditor()
{
}

//-----------------------------------------------------------------------------
// Function: ProjectEditor::getProject()
//-----------------------------------------------------------------------------
Project ProjectEditor::getProject() const
{
//    return Project(nameEdit_->text(), versionEdit_->text(), authorEdit_->text(), descriptionEdit_->text());
    return Project(nameEdit_->text(), versionEdit_->text(), authorEdit_->text());
}

//-----------------------------------------------------------------------------
// Function: ProjectEditor::isValid()
//-----------------------------------------------------------------------------
bool ProjectEditor::isValid() const
{
    return (!nameEdit_->text().isEmpty() && !versionEdit_->text().isEmpty()  &&
            !authorEdit_->text().isEmpty() && !descriptionEdit_->text().isEmpty());
}

//-----------------------------------------------------------------------------
// Function: ProjectEditor::updateMatcherItems()
//-----------------------------------------------------------------------------
void ProjectEditor::updateMatcherItems()
{
    updateLibraryMatcherItem();
}

//-----------------------------------------------------------------------------
// Function: ProjectEditor::updateLibraryMatcherItem()
//-----------------------------------------------------------------------------
void ProjectEditor::updateLibraryMatcherItem()
{
//    VLNVDataNode const* foundItem = dataTree_.findChild(authorEdit_->text());
//    libraryMatcher_.setDataNode(foundItem);

//    // Update "recursively" also the name matcher item.
//    updateNameMatcherItem();
}

//-----------------------------------------------------------------------------
// Function: ProjectEditor::updateNameMatcherItem()
//-----------------------------------------------------------------------------
void ProjectEditor::updateNameMatcherItem()
{
//    VLNVDataNode const* libItem = libraryMatcher_.getDataNode();
//    VLNVDataNode const* foundItem = 0;

//    if (libItem != 0)
//    {
//        foundItem = libItem->findChild(libraryEdit_->text());
//    }

//    nameMatcher_.setDataNode(foundItem);

//    // Update "recursively" also the version matcher item.
//    updateVersionMatcherItem();
}

//-----------------------------------------------------------------------------
// Function: ProjectEditor::updateVersionMatcherItem()
//-----------------------------------------------------------------------------
void ProjectEditor::updateVersionMatcherItem()
{
//    VLNVDataNode const* nameItem = nameMatcher_.getDataNode();
//    VLNVDataNode const* foundItem = 0;

//    if (nameItem != 0)
//    {
//        foundItem = nameItem->findChild(nameEdit_->text());
//    }

//    versionMatcher_.setDataNode(foundItem);
}

//-----------------------------------------------------------------------------
// Function: ProjectEditor::initWidgets()
//-----------------------------------------------------------------------------
void ProjectEditor::initWidgets(QWidget* parentWnd)
{
    // Create the labels.
    QLabel* nameLabel = new QLabel(tr("Name:"), this);
    QLabel* versionLabel = new QLabel(tr("Version:"), this);
    QLabel* authorLabel = new QLabel(tr("author:"), this);
    QLabel* descriptionLabel = new QLabel(tr("description:"), this);

    nameEdit_ = new AssistedLineEdit(parentWnd, this);
//    nameEdit_->setContentMatcher(&nameMatcher_);
    nameEdit_->setValidator(new NMTokenValidator(this));
    nameEdit_->setProperty("mandatoryField", true);

    versionEdit_ = new AssistedLineEdit(parentWnd, this);
//    versionEdit_->setContentMatcher(&versionMatcher_);
    versionEdit_->setValidator(new NMTokenValidator(this));
    versionEdit_->setProperty("mandatoryField", true);

    // Create the line edits.
    authorEdit_ = new AssistedLineEdit(parentWnd, this);
//    authorEdit_->setContentMatcher(&authorMatcher_);
    authorEdit_->setValidator(new NameValidator(this));
    authorEdit_->setProperty("mandatoryField", true);

    descriptionEdit_ = new AssistedLineEdit(parentWnd, this);
//    descriptionEdit_->setContentMatcher(&descriptionMatcher_);
    descriptionEdit_->setValidator(new NameValidator(this));
    descriptionEdit_->setProperty("mandatoryField", true);

    QHBoxLayout* nameLayout = new QHBoxLayout();
    nameLayout->addWidget(nameEdit_, 1);
    nameLayout->addWidget(&nameExtensionLabel_);
    
    // Create the layout and add the widgets to it.
    QGridLayout* layout = new QGridLayout(this);
    layout->addWidget(nameLabel, 0, 0, 1, 1);
    layout->addLayout(nameLayout, 0, 1, 1, 1);
    layout->addWidget(versionLabel, 1, 0, 1, 1);
    layout->addWidget(versionEdit_, 1, 1, 1, 1);
    layout->addWidget(authorLabel, 2, 0, 1, 1);
    layout->addWidget(authorEdit_, 2, 1, 1, 1);
    layout->addWidget(descriptionLabel, 3, 0, 1, 1);
    layout->addWidget(descriptionEdit_, 3, 1, 1, 1);
}

//-----------------------------------------------------------------------------
// Function: ProjectEditor::initConnections()
//-----------------------------------------------------------------------------
void ProjectEditor::initConnections()
{
    // Connect the signals informing of changes in items.
    connect(nameEdit_, SIGNAL(textChanged(QString const&)),
        this, SIGNAL(contentChanged()), Qt::UniqueConnection);
    connect(versionEdit_, SIGNAL(textChanged(QString const&)),
        this, SIGNAL(contentChanged()), Qt::UniqueConnection);
    connect(authorEdit_, SIGNAL(textChanged(QString const&)),
        this, SIGNAL(contentChanged()), Qt::UniqueConnection);
    connect(descriptionEdit_, SIGNAL(textChanged(QString const&)),
        this, SIGNAL(contentChanged()), Qt::UniqueConnection);

    // Connect the matcher update slots to the textChanged signals.
    connect(nameEdit_, SIGNAL(textChanged(QString const&)),
        this, SLOT(updateVersionMatcherItem()), Qt::UniqueConnection);
    connect(authorEdit_, SIGNAL(textChanged(QString const&)),
        this, SLOT(updateLibraryMatcherItem()), Qt::UniqueConnection);
    connect(descriptionEdit_, SIGNAL(textChanged(QString const&)),
        this, SLOT(updateNameMatcherItem()), Qt::UniqueConnection);
}

//-----------------------------------------------------------------------------
// Function: ProjectEditor::isEmpty()
//-----------------------------------------------------------------------------
bool ProjectEditor::isEmpty() const
{
    return (nameEdit_->text().isEmpty() && versionEdit_->text().isEmpty() &&
            authorEdit_->text().isEmpty() && descriptionEdit_->text().isEmpty());
}

//-----------------------------------------------------------------------------
// Function: ProjectEditor::setName()
//-----------------------------------------------------------------------------
void ProjectEditor::setName(QString const& name)
{
    nameEdit_->setText(name);
}

//-----------------------------------------------------------------------------
// Function: ProjectEditor::setVersion()
//-----------------------------------------------------------------------------
void ProjectEditor::setVersion(QString const& version)
{
    versionEdit_->setText(version);
}

//-----------------------------------------------------------------------------
// Function: ProjectEditor::setAuthor()
//-----------------------------------------------------------------------------
void ProjectEditor::setAuthor(QString const& author)
{
    authorEdit_->setText(author);
}

//-----------------------------------------------------------------------------
// Function: ProjectEditor::setDescription()
//-----------------------------------------------------------------------------
void ProjectEditor::setDescription(QString const& description)
{
    descriptionEdit_->setText(description);
}

//-----------------------------------------------------------------------------
// Function: ProjectEditor::dropEvent()
//-----------------------------------------------------------------------------
void ProjectEditor::dropEvent(QDropEvent* event)
{	
}

//-----------------------------------------------------------------------------
// Function: ProjectEditor::dragEnterEvent()
//-----------------------------------------------------------------------------
void ProjectEditor::dragEnterEvent( QDragEnterEvent* event )
{
}

//-----------------------------------------------------------------------------
// Function: ProjectEditor::setMandatory()
//-----------------------------------------------------------------------------
void ProjectEditor::setMandatory(bool mandatory)
{
    nameEdit_->setProperty("mandatoryField", mandatory);
    versionEdit_->setProperty("mandatoryField", mandatory);
    authorEdit_->setProperty("mandatoryField", mandatory);
    descriptionEdit_->setProperty("mandatoryField", mandatory);
}

//-----------------------------------------------------------------------------
// Function: ProjectEditor::setProject()
//-----------------------------------------------------------------------------
void ProjectEditor::setProject(const Project& project)
{
    if (dirty_)
    {
        updateFiltering();
    }

    // Set the fields according to the project.
    nameEdit_->setText(project.getName());
    versionEdit_->setText(project.getVersion());
    authorEdit_->setText(project.getVendor());
//    descriptionEdit_->setText(project.getDescription());
}

//-----------------------------------------------------------------------------
// Function: ProjectEditor::updateFiltering()
//-----------------------------------------------------------------------------
void ProjectEditor::updateFiltering()
{
//    dataTree_.clear();
//    dataTree_.parse(handler_, contentTypes_);
    dirty_ = false;
}

//-----------------------------------------------------------------------------
// Function: ProjectEditor::addNameExtension()
//-----------------------------------------------------------------------------
void ProjectEditor::addNameExtension(QString const& extension)
{
    if (nameExtensionLabel_.text().isEmpty())
    {
        nameExtensionLabel_.setText(extension);
    }
    else
    {
        nameExtensionLabel_.setText(nameExtensionLabel_.text() + "/" + extension);
    }

    nameExtensionLabel_.setVisible(true);

//    dataTree_.addExtensionFilter(extension);
    dirty_ = true;
}

//-----------------------------------------------------------------------------
// Function: ProjectEditor::showEvent()
//-----------------------------------------------------------------------------
void ProjectEditor::showEvent(QShowEvent* event)
{
    if (dirty_)
    {
        refresh();
    }

    QGroupBox::showEvent(event);
}

//-----------------------------------------------------------------------------
// Function: ProjectEditor::refresh()
//-----------------------------------------------------------------------------
void ProjectEditor::refresh()
{
    updateFiltering();
    updateMatcherItems();
}
