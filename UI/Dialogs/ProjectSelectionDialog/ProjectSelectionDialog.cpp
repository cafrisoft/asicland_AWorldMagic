/*
 *      Created on: 11.1.2024
 *      Author: P.H.G
 *      Filename: ProjectSelectionDialog.cpp
 *      Project: AWM
 *      Description :
 *          Dialog for selecting the items.
 */

#include "ProjectSelectionDialog.h"

#include "ProjectSelectionListItem.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QIcon>
#include <QLabel>

//-----------------------------------------------------------------------------
// Function: ProjectSelectionDialog::ProjectSelectionDialog()
//-----------------------------------------------------------------------------
ProjectSelectionDialog::ProjectSelectionDialog(QString const& okButtonText, QString const& iconPath,
    QString const& okIconPath, QString const& windowTitle, QWidget* parent, Qt::WindowFlags f):
QDialog(parent, f),
dialogIconPath_(iconPath),
okButton_(okButtonText, this),
cancelButton_(tr("Cancel"), this)
//checkAllBox_(tr("Select/deselect all"), this),
//itemList_(this),
//busDefinitionIcon_(":resources/Res/bus-def.png"),
//abstractionDefinition_(":resources/Res/abs-def.png"),
//componentIcon_(":resources/Res/hw-component.png"),
//designIcon_(":resources/Res/hw-design.png"),
//designConfigurationIcon_(":resources/Res/configuration.png"),
//generatorChainIcon_(":resources/Res/generatorChain.png"),
//abstractorIcon_(":resources/Res/connect.png"),
//catalogIcon_(":resources/Res/catalog.png"),
//comDefinitionIcon_(":resources/Res/new-com_definition.png"),
//apiDefinitionIcon_(":resources/Res/new-api_definition.png"),
//fileIcon_(":resources/Res/file.png")
{
    okButton_.setIcon(QIcon(okIconPath));
//    checkAllBox_.setChecked(true);

    setWindowTitle(windowTitle);
    connectSignals();
}

//-----------------------------------------------------------------------------
// Function: ProjectSelectionDialog::~ProjectSelectionDialog()
//-----------------------------------------------------------------------------
ProjectSelectionDialog::~ProjectSelectionDialog()
{

}

//-----------------------------------------------------------------------------
// Function: ProjectSelectionDialog::connectSignals()
//-----------------------------------------------------------------------------
void ProjectSelectionDialog::connectSignals()
{
    connect(&okButton_, SIGNAL(clicked(bool)), this, SLOT(accept()), Qt::UniqueConnection);
    connect(&cancelButton_, SIGNAL(clicked(bool)), this, SLOT(reject()), Qt::UniqueConnection);
//    connect(&checkAllBox_, SIGNAL(stateChanged(int)), this, SLOT(onCheckAllChanged(int)), Qt::UniqueConnection);
//    connect(&itemList_, SIGNAL(itemClicked(QListWidgetItem*)),
//        this, SLOT(onItemClicked(QListWidgetItem*)), Qt::UniqueConnection);
}

//-----------------------------------------------------------------------------
// Function: ProjectSelectionDialog::sizeHint()
//-----------------------------------------------------------------------------
QSize ProjectSelectionDialog::sizeHint() const
{
    return QSize(700, 200);
}

//-----------------------------------------------------------------------------
// Function: ProjectSelectionDialog::createItem()
//-----------------------------------------------------------------------------
//void ProjectSelectionDialog::createItem(const QString& path, const VLNV& vlnv, bool locked)
//{
//    if (!itemExistsInList(path, vlnv))
//    {
//        QIcon usedIcon = getUsedIcon(vlnv);

//        ProjectSelectionListItem* newItem = new ProjectSelectionListItem(usedIcon, path, locked, vlnv);
//        itemList_.addItem(newItem);
//    }
//}

//-----------------------------------------------------------------------------
// Function: ProjectSelectionDialog::itemExistsInList()
//-----------------------------------------------------------------------------
//bool ProjectSelectionDialog::itemExistsInList(QString const& itemPath, VLNV const& itemVLNV) const
//{
//    for (int i = 0; i < itemList_.count(); ++i)
//    {
//        ProjectSelectionListItem* listItem = dynamic_cast<ProjectSelectionListItem*>(itemList_.item(i));
//        if (listItem && listItem->getPath() == itemPath && listItem->getVLNV() == itemVLNV)
//        {
//            return true;
//        }
//    }

//    return false;
//}

//-----------------------------------------------------------------------------
// Function: ProjectSelectionDialog::getUsedIcon()
//-----------------------------------------------------------------------------
//QIcon ProjectSelectionDialog::getUsedIcon(const VLNV& itemVLNV) const
//{
//    QIcon usedIcon = fileIcon_;

//    if (itemVLNV.isValid())
//    {
//        if (itemVLNV.getType() == VLNV::BUSDEFINITION)
//        {
//            usedIcon = busDefinitionIcon_;
//        }
//        else if (itemVLNV.getType() == VLNV::ABSTRACTIONDEFINITION)
//        {
//            usedIcon = abstractionDefinition_;
//        }
//        else if (itemVLNV.getType() == VLNV::COMPONENT)
//        {
//            usedIcon = componentIcon_;
//        }
//        else if (itemVLNV.getType() == VLNV::DESIGN)
//        {
//            usedIcon = designIcon_;
//        }
//        else if (itemVLNV.getType() == VLNV::DESIGNCONFIGURATION)
//        {
//            usedIcon = designConfigurationIcon_;
//        }
//        else if (itemVLNV.getType() == VLNV::GENERATORCHAIN)
//        {
//            usedIcon = generatorChainIcon_;
//        }
//        else if (itemVLNV.getType() == VLNV::ABSTRACTOR)
//        {
//            usedIcon = abstractorIcon_;
//        }
//        else if (itemVLNV.getType() == VLNV::CATALOG)
//        {
//            usedIcon = catalogIcon_;
//        }
//        else if (itemVLNV.getType() == VLNV::COMDEFINITION)
//        {
//            usedIcon = comDefinitionIcon_;
//        }
//        else if (itemVLNV.getType() == VLNV::APIDEFINITION)
//        {
//            usedIcon = apiDefinitionIcon_;
//        }
//    }

//    return usedIcon;
//}

//-----------------------------------------------------------------------------
// Function: ProjectSelectionDialog::onCheckAllChanged()
//-----------------------------------------------------------------------------

//void ProjectSelectionDialog::onCheckAllChanged(int checkAllState)
//{
//    Qt::CheckState newCheckState = Qt::Checked;
//    if (checkAllState == Qt::Unchecked)
//    {
//        newCheckState = Qt::Unchecked;
//    }

//    for (int listIndex = 0; listIndex < itemList_.count(); ++listIndex)
//    {
//        ProjectSelectionListItem* listItem = dynamic_cast<ProjectSelectionListItem*>(itemList_.item(listIndex));
//        if (listItem && !listItem->itemIsLocked())
//        {
//            listItem->setCheckState(newCheckState);
//        }
//    }
//}
//-----------------------------------------------------------------------------
// Function: ProjectSelectionDialog::onItemClicked()
//-----------------------------------------------------------------------------
//void ProjectSelectionDialog::onItemClicked(QListWidgetItem* currentItem)
//{
//    ProjectSelectionListItem* listItem = dynamic_cast<ProjectSelectionListItem*>(currentItem);
//    if (listItem && !listItem->itemIsLocked())
//    {
//        Qt::CheckState currentItemCheckState = listItem->checkState();
//        if (currentItemCheckState == Qt::Checked)
//        {
//            listItem->setCheckState(Qt::Unchecked);
//        }
//        else
//        {
//            listItem->setCheckState(Qt::Checked);
//        }
//    }
//}

//-----------------------------------------------------------------------------
// Function: ProjectSelectionDialog::getItemsToExport()
//-----------------------------------------------------------------------------
//QVector<ProjectSelectionListItem*> ProjectSelectionDialog::getSelectedItems() const
//{
//    QVector<ProjectSelectionListItem*> exportedItems;

//    for (int listIndex = 0; listIndex < itemList_.count(); ++listIndex)
//    {
//        ProjectSelectionListItem* exportItem = dynamic_cast<ProjectSelectionListItem*>(itemList_.item(listIndex));
//        if (exportItem && exportItem->checkState() == Qt::Checked)
//        {
//            exportedItems.append(exportItem);
//        }
//    }

//    return exportedItems;
//}

//-----------------------------------------------------------------------------
// Function: ProjectSelectionDialog::enableOkButton()
//-----------------------------------------------------------------------------
void ProjectSelectionDialog::enableOkButton()
{
    okButton_.setEnabled(true);
}

//-----------------------------------------------------------------------------
// Function: ProjectSelectionDialog::disableOkButton()
//-----------------------------------------------------------------------------
void ProjectSelectionDialog::disableOkButton()
{
    okButton_.setEnabled(false);
}

//-----------------------------------------------------------------------------
// Function: ProjectSelectionDialog::setupIntroWidget()
//-----------------------------------------------------------------------------
QWidget* ProjectSelectionDialog::setupIntroWidget(QString const& introName, QString const& introText) const
{
    QLabel* introLabel = new QLabel(introName);
    QLabel* introTextLabel = new QLabel(introText);
    QFont introFont = introLabel->font();
    introFont.setBold(true);
    introLabel->setFont(introFont);

    QLabel* iconLabel = new QLabel();
    iconLabel->setPixmap(QPixmap(dialogIconPath_));

    QVBoxLayout* introTextLayout = new QVBoxLayout();
    introTextLayout->addWidget(introLabel);
    introTextLayout->addWidget(introTextLabel);

    QHBoxLayout* mainIntroLayout = new QHBoxLayout();
    mainIntroLayout->addLayout(introTextLayout, 1);
    mainIntroLayout->addWidget(iconLabel);

    QWidget* introWidget = new QWidget();
    introWidget->setStyleSheet("background-color: white");
    introWidget->setLayout(mainIntroLayout);

    return introWidget;
}

//-----------------------------------------------------------------------------
// Function: ProjectSelectionDialog::setupButtonLayout()
//-----------------------------------------------------------------------------
QHBoxLayout* ProjectSelectionDialog::setupButtonLayout()
{
    QHBoxLayout* buttonLayout = new QHBoxLayout();
//    buttonLayout->addWidget(&checkAllBox_, 0, Qt::AlignLeft);
    buttonLayout->addStretch();
    buttonLayout->addWidget(&okButton_, 0, Qt::AlignRight);
    buttonLayout->addWidget(&cancelButton_, 0, Qt::AlignRight);

    return buttonLayout;
}

//-----------------------------------------------------------------------------
// Function: ProjectSelectionDialog::getItemList()
//-----------------------------------------------------------------------------
//QListWidget* ProjectSelectionDialog::getItemList()
//{
//    return &itemList_;
//}
