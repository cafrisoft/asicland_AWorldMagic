/*
 *      Created on: 11.1.2024
 *      Author: P.H.G
 *      Filename: ImportSelectionDialog.cpp
 *      Project: AWM
 *      Description :
 *          Dialog for selecting the items.
 */

#include "ImportSelectionDialog.h"

#include "ImportSelectionListItem.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QIcon>
#include <QLabel>

//-----------------------------------------------------------------------------
// Function: ImportSelectionDialog::ImportSelectionDialog()
//-----------------------------------------------------------------------------
ImportSelectionDialog::ImportSelectionDialog(QString const& okButtonText, QString const& iconPath,
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
// Function: ImportSelectionDialog::~ImportSelectionDialog()
//-----------------------------------------------------------------------------
ImportSelectionDialog::~ImportSelectionDialog()
{

}

//-----------------------------------------------------------------------------
// Function: ImportSelectionDialog::connectSignals()
//-----------------------------------------------------------------------------
void ImportSelectionDialog::connectSignals()
{
    connect(&okButton_, SIGNAL(clicked(bool)), this, SLOT(accept()), Qt::UniqueConnection);
    connect(&cancelButton_, SIGNAL(clicked(bool)), this, SLOT(reject()), Qt::UniqueConnection);
//    connect(&checkAllBox_, SIGNAL(stateChanged(int)), this, SLOT(onCheckAllChanged(int)), Qt::UniqueConnection);
//    connect(&itemList_, SIGNAL(itemClicked(QListWidgetItem*)),
//        this, SLOT(onItemClicked(QListWidgetItem*)), Qt::UniqueConnection);
}

//-----------------------------------------------------------------------------
// Function: ImportSelectionDialog::sizeHint()
//-----------------------------------------------------------------------------
QSize ImportSelectionDialog::sizeHint() const
{
    return QSize(700, 150);
}

//-----------------------------------------------------------------------------
// Function: ImportSelectionDialog::createItem()
//-----------------------------------------------------------------------------
//void ImportSelectionDialog::createItem(const QString& path, const VLNV& vlnv, bool locked)
//{
//    if (!itemExistsInList(path, vlnv))
//    {
//        QIcon usedIcon = getUsedIcon(vlnv);

//        ImportSelectionListItem* newItem = new ImportSelectionListItem(usedIcon, path, locked, vlnv);
//        itemList_.addItem(newItem);
//    }
//}

//-----------------------------------------------------------------------------
// Function: ImportSelectionDialog::itemExistsInList()
//-----------------------------------------------------------------------------
//bool ImportSelectionDialog::itemExistsInList(QString const& itemPath, VLNV const& itemVLNV) const
//{
//    for (int i = 0; i < itemList_.count(); ++i)
//    {
//        ImportSelectionListItem* listItem = dynamic_cast<ImportSelectionListItem*>(itemList_.item(i));
//        if (listItem && listItem->getPath() == itemPath && listItem->getVLNV() == itemVLNV)
//        {
//            return true;
//        }
//    }

//    return false;
//}

//-----------------------------------------------------------------------------
// Function: ImportSelectionDialog::getUsedIcon()
//-----------------------------------------------------------------------------
//QIcon ImportSelectionDialog::getUsedIcon(const VLNV& itemVLNV) const
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
// Function: ImportSelectionDialog::onCheckAllChanged()
//-----------------------------------------------------------------------------
//void ImportSelectionDialog::onCheckAllChanged(int checkAllState)
//{
//    Qt::CheckState newCheckState = Qt::Checked;
//    if (checkAllState == Qt::Unchecked)
//    {
//        newCheckState = Qt::Unchecked;
//    }

//    for (int listIndex = 0; listIndex < itemList_.count(); ++listIndex)
//    {
//        ImportSelectionListItem* listItem = dynamic_cast<ImportSelectionListItem*>(itemList_.item(listIndex));
//        if (listItem && !listItem->itemIsLocked())
//        {
//            listItem->setCheckState(newCheckState);
//        }
//    }
//}

//-----------------------------------------------------------------------------
// Function: ImportSelectionDialog::onItemClicked()
//-----------------------------------------------------------------------------
//void ImportSelectionDialog::onItemClicked(QListWidgetItem* currentItem)
//{
//    ImportSelectionListItem* listItem = dynamic_cast<ImportSelectionListItem*>(currentItem);
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
// Function: ImportSelectionDialog::getItemsToExport()
//-----------------------------------------------------------------------------
//QVector<ImportSelectionListItem*> ImportSelectionDialog::getSelectedItems() const
//{
//    QVector<ImportSelectionListItem*> exportedItems;

//    for (int listIndex = 0; listIndex < itemList_.count(); ++listIndex)
//    {
//        ImportSelectionListItem* exportItem = dynamic_cast<ImportSelectionListItem*>(itemList_.item(listIndex));
//        if (exportItem && exportItem->checkState() == Qt::Checked)
//        {
//            exportedItems.append(exportItem);
//        }
//    }

//    return exportedItems;
//}

//-----------------------------------------------------------------------------
// Function: ImportSelectionDialog::enableOkButton()
//-----------------------------------------------------------------------------
void ImportSelectionDialog::enableOkButton()
{
    okButton_.setEnabled(true);
}

//-----------------------------------------------------------------------------
// Function: ImportSelectionDialog::disableOkButton()
//-----------------------------------------------------------------------------
void ImportSelectionDialog::disableOkButton()
{
    okButton_.setEnabled(false);
}

//-----------------------------------------------------------------------------
// Function: ImportSelectionDialog::setupIntroWidget()
//-----------------------------------------------------------------------------
QWidget* ImportSelectionDialog::setupIntroWidget(QString const& introName, QString const& introText) const
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
// Function: ImportSelectionDialog::setupButtonLayout()
//-----------------------------------------------------------------------------
QHBoxLayout* ImportSelectionDialog::setupButtonLayout()
{
    QHBoxLayout* buttonLayout = new QHBoxLayout();
//    buttonLayout->addWidget(&checkAllBox_, 0, Qt::AlignLeft);
    buttonLayout->addStretch();
    buttonLayout->addWidget(&okButton_, 0, Qt::AlignRight);
    buttonLayout->addWidget(&cancelButton_, 0, Qt::AlignRight);

    return buttonLayout;
}

//-----------------------------------------------------------------------------
// Function: ImportSelectionDialog::getItemList()
//-----------------------------------------------------------------------------
//QListWidget* ImportSelectionDialog::getItemList()
//{
//    return &itemList_;
//}
