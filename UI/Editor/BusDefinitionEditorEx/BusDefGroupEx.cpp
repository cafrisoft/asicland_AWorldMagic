//-----------------------------------------------------------------------------
// File: BusDefGroupEx.cpp
//-----------------------------------------------------------------------------
// Project: AWM
// Author: P.H.G
// Date: 19.06.2024
//
// Description:
// BusDefGroupEx contains elements to set the properties of a Bus Definition.
//-----------------------------------------------------------------------------

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QRegularExpressionValidator>
#include <QRegularExpression>
#include <QSizePolicy>
#include <QLabel>

#include <IPXACTmodels/utilities/BusDefinitionUtils.h>

#include <KactusAPI/LibraryInterface.h>

#include <UI/Widgets/vlnvDisplayer/vlnvdisplayer.h>
//#include <UI/Widgets/vlnvEditor/vlnveditor.h>

#include "BusDefGroupEx.h"

//-----------------------------------------------------------------------------
// Function: BusDefGroupEx::BusDefGroupEx()
//-----------------------------------------------------------------------------
BusDefGroupEx::BusDefGroupEx(LibraryInterface* libraryHandler, QWidget *parent):
    QWidget(parent),
    library_(libraryHandler),
    busDef_(),
    //directConnection_(tr("Allow direct connections"), this),
    //isBroadcast_(tr("Support broadcast"), this),
    //isAddressable_(tr("Addressable bus"), this),
    //maxMastersEditor_(this),
    //maxSlavesEditor_(this),
    //systemGroupEditor_(libraryHandler, this),
    authorEditor_(this),
    descriptionEditor_(this),
    vlnvDisplay_(new VLNVDisplayer(this, VLNV()))
//extendEditor_(new VLNVEditor(VLNV::BUSDEFINITION, libraryHandler, parent, this))
{
//    vlnvDisplay_->setTitle(QStringLiteral("Bus definition"));
//    extendEditor_->setTitle(QStringLiteral("Extended bus definition"));
//    extendEditor_->setMandatory(false);

//    QRegularExpression numberExpression(QString("[0-9]*"));
//    QRegularExpressionValidator* numberValidator = new QRegularExpressionValidator(numberExpression, this);
//    maxMastersEditor_.setValidator(numberValidator);
//    maxSlavesEditor_.setValidator(numberValidator);

#if QT_VERSION > QT_VERSION_CHECK(5,3,0)
//    maxMastersEditor_.setPlaceholderText(tr("unbound"));
//    maxSlavesEditor_.setPlaceholderText(tr("unbound"));
#endif

    setupLayout();

//	connect(&maxMastersEditor_, SIGNAL(editingFinished()), this, SLOT(onMastersChanged()), Qt::UniqueConnection);
//	connect(&maxSlavesEditor_, SIGNAL(editingFinished()),	this, SLOT(onSlavesChanged()), Qt::UniqueConnection);
	 
//	connect(&directConnection_, SIGNAL(toggled(bool)),
//        this, SLOT(onDirectConnectionChanged(bool)), Qt::UniqueConnection);
//    connect(&isBroadcast_, SIGNAL(toggled(bool)), this, SLOT(onIsBroadcastChanged(bool)), Qt::UniqueConnection);
//	connect(&isAddressable_, SIGNAL(toggled(bool)),
//        this, SLOT(onIsAddressableChanged(bool)), Qt::UniqueConnection);

//    connect(&systemGroupEditor_, SIGNAL(contentChanged()),
//        this, SLOT(onSystemNamesChanged()), Qt::UniqueConnection);

    connect(&authorEditor_, SIGNAL(editingFinished()), this, SLOT(onAuthorChanged()), Qt::UniqueConnection);

    connect(&descriptionEditor_, SIGNAL(textChanged()), this, SLOT(onDescriptionChanged()), Qt::UniqueConnection);

//    connect(extendEditor_, SIGNAL(vlnvEdited()), this, SLOT(onExtendChanged()), Qt::UniqueConnection);
}

//-----------------------------------------------------------------------------
// Function: BusDefGroupEx::setBusDef()
//-----------------------------------------------------------------------------
void BusDefGroupEx::setBusDef( QSharedPointer<BusDefinition> busDef )
{
	busDef_ = busDef;
    vlnvDisplay_->setVLNV(busDef_->getVlnv());
//    extendEditor_->setVLNV(busDef_->getExtends());

    if (busDef_->getExtends().isValid())
    {
        setupExtendedBus();
    }

//    directConnection_.setChecked(busDef_->getDirectConnection());
//    isBroadcast_.setChecked(busDef_->getBroadcast().toBool());
//	isAddressable_.setChecked(busDef_->getIsAddressable());

//    maxMastersEditor_.setText(busDef_->getMaxMasters());
//	maxSlavesEditor_.setText(busDef_->getMaxSlaves());

//    systemGroupEditor_.setItems(busDef_);

    authorEditor_.setText(busDef_->getAuthor());

    descriptionEditor_.setPlainText(busDef_->getDescription());
}

//-----------------------------------------------------------------------------
// Function: BusDefGroupEx::onDirectConnectionChanged()
//-----------------------------------------------------------------------------
//void BusDefGroupEx::onDirectConnectionChanged(bool checked)
//{
//	busDef_->setDirectConnection(checked);
//	emit contentChanged();
//}

//-----------------------------------------------------------------------------
// Function: BusDefGroupEx::onIsBroadcastChanged()
//-----------------------------------------------------------------------------
//void BusDefGroupEx::onIsBroadcastChanged(bool checked)
//{
//    busDef_->setBroadcast(checked);
//    emit contentChanged();
//}

//-----------------------------------------------------------------------------
// Function: BusDefGroupEx::onIsAddressableChanged()
//-----------------------------------------------------------------------------
//void BusDefGroupEx::onIsAddressableChanged(bool checked)
//{
//	busDef_->setIsAddressable(checked);
//	emit contentChanged();
//}

//-----------------------------------------------------------------------------
// Function: BusDefGroupEx::onMastersChanged()
//-----------------------------------------------------------------------------
//void BusDefGroupEx::onMastersChanged()
//{
//    busDef_->setMaxMasters(maxMastersEditor_.text());
//	emit contentChanged();
//}

//-----------------------------------------------------------------------------
// Function: BusDefGroupEx::onSlavesChanged()
//-----------------------------------------------------------------------------
//void BusDefGroupEx::onSlavesChanged()
//{
//	busDef_->setMaxSlaves(maxSlavesEditor_.text());
//	emit contentChanged();
//}

//-----------------------------------------------------------------------------
// Function: BusDefGroupEx::onSystemNamesChanged()
//-----------------------------------------------------------------------------
//void BusDefGroupEx::onSystemNamesChanged()
//{
//    QStringList systemGroupNames;
//    for (int i = 0; i < systemGroupEditor_.count(); ++i)
//    {
//        QListWidgetItem* systemItem = systemGroupEditor_.item(i);
//        QString systemName = systemItem->text();
//        if (!systemGroupNames.contains(systemName))
//        {
//            systemGroupNames.append(systemName);
//        }
//    }

//    busDef_->setSystemGroupNames(systemGroupNames);

//    emit contentChanged();
//}

//-----------------------------------------------------------------------------
// Function: BusDefGroupEx::onAuthorChanged()
//-----------------------------------------------------------------------------
void BusDefGroupEx::onAuthorChanged()
{
    busDef_->setAuthor(authorEditor_.text());
    emit contentChanged();
}

//-----------------------------------------------------------------------------
// Function: BusDefGroupEx::onDescriptionChanged()
//-----------------------------------------------------------------------------
void BusDefGroupEx::onDescriptionChanged()
{
    busDef_->setDescription(descriptionEditor_.toPlainText());
	emit contentChanged();
}

//-----------------------------------------------------------------------------
// Function: BusDefGroupEx::onExtendChanged()
//-----------------------------------------------------------------------------
//void BusDefGroupEx::onExtendChanged()
//{
//    VLNV newExtendVLNV = extendEditor_->getVLNV();

//    removeSystemGroupsFromExtendedDefinition();

//    busDef_->setExtends(extendEditor_->getVLNV());
//    setupExtendedBus();

////    systemGroupEditor_.setItems(busDef_);
////    onSystemNamesChanged();

//    emit contentChanged();
//}

//-----------------------------------------------------------------------------
// Function: BusDefGroupEx::removeSystemGroupsFromExtendedDefinition()
//-----------------------------------------------------------------------------
//void BusDefGroupEx::removeSystemGroupsFromExtendedDefinition()
//{
//    QSharedPointer<const BusDefinition> extendedBus = getExtendedBus(busDef_);
//    if (extendedBus)
//    {
//        QStringList currentSystemGroups = busDef_->getSystemGroupNames();
//        QStringList extendSystemGroups = BusDefinitionUtils::getSystemGroups(extendedBus, library_);
//        for (auto extendSystem : extendSystemGroups)
//        {
//            if (currentSystemGroups.contains(extendSystem))
//            {
//                currentSystemGroups.removeAll(extendSystem);
//            }
//        }

//        busDef_->setSystemGroupNames(currentSystemGroups);
//    }
//}

//-----------------------------------------------------------------------------
// Function: BusDefGroupEx::setupLayout()
//-----------------------------------------------------------------------------
void BusDefGroupEx::setupLayout()
{
//    QFormLayout* masterSlaveLayout = new QFormLayout();
//    masterSlaveLayout->addRow(tr("Max masters on bus:"), &maxMastersEditor_);
//    masterSlaveLayout->addRow(tr("Max slaves on bus:"), &maxSlavesEditor_);

//    QGroupBox* selectionGroup = new QGroupBox(tr("Constraints"), this);

//    QVBoxLayout* selectionsLayout = new QVBoxLayout(selectionGroup);
//    selectionsLayout->addWidget(&directConnection_);
//    selectionsLayout->addWidget(&isBroadcast_);
//    selectionsLayout->addWidget(&isAddressable_);
//    selectionsLayout->addLayout(masterSlaveLayout);
//    selectionsLayout->addStretch();


    QLabel *authorLabel = new QLabel(tr("Author:"), this);
    QLabel *descriptionLabel = new QLabel(tr("Library:"), this);

    QFormLayout* extendLayout = new QFormLayout();
    extendLayout->addRow(authorLabel, &authorEditor_);
    extendLayout->addRow(descriptionLabel, &descriptionEditor_);

    QGroupBox* descriptionGroup = new QGroupBox(tr(""), this);

    QVBoxLayout* descriptionLayout = new QVBoxLayout(descriptionGroup);
//    descriptionLayout->addWidget(&authorEditor_);
//    descriptionLayout->addWidget(&descriptionEditor_);
    descriptionLayout->addLayout(extendLayout);
    descriptionLayout->addStretch(1);

//    QGroupBox* systemGroupBox = new QGroupBox(tr("System group names"), this);

//    QVBoxLayout* systemGroupLayout = new QVBoxLayout(systemGroupBox);
//    systemGroupLayout->addWidget(&systemGroupEditor_);

    QGridLayout* topLayout = new QGridLayout(this);
    topLayout->addWidget(vlnvDisplay_, 0, 0, 1, 1);
//    topLayout->addWidget(extendEditor_, 0, 1, 1, 1);
//    topLayout->addWidget(descriptionGroup, 0, 1, 1, 1);
    topLayout->addWidget(descriptionGroup, 1, 0, 1, 1);
//    topLayout->addWidget(systemGroupBox, 1, 1, 1, 1);
//    topLayout->addWidget(selectionGroup, 2, 0, 1, 1);

    topLayout->setColumnStretch(0, 1);
    topLayout->setColumnStretch(1, 1);

    topLayout->setRowStretch(1, 1);

//    maxMastersEditor_.setSizePolicy(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
//    maxSlavesEditor_.setSizePolicy(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
}

//-----------------------------------------------------------------------------
// Function: BusDefGroupEx::setupExtendedBus()
//-----------------------------------------------------------------------------
void BusDefGroupEx::setupExtendedBus()
{
    QSharedPointer<const BusDefinition> extendedBus = getExtendedBus(busDef_);

    if (extendedBus)
    {
        extendBusDefinition(extendedBus);
        authorEditor_.setText(extendedBus->getAuthor());
        descriptionEditor_.setPlaceholderText(extendedBus->getDescription());

        return;
    }

    removeBusExtension();

    authorEditor_.setText(QString());
    descriptionEditor_.setPlaceholderText(QString());
}

//-----------------------------------------------------------------------------
// Function: BusDefGroupEx::getExtendedBus()
//-----------------------------------------------------------------------------
QSharedPointer<const BusDefinition> BusDefGroupEx::getExtendedBus(QSharedPointer<const BusDefinition> busDefinition)
    const
{
    VLNV extendedVLNV = busDefinition->getExtends();
    if (extendedVLNV.isValid() && extendedVLNV.getType() == VLNV::BUSDEFINITION)
    {
        QSharedPointer<const Document> extendedDocument = library_->getModelReadOnly(extendedVLNV);
        if (extendedDocument)
        {
            QSharedPointer<const BusDefinition> extendedBus = extendedDocument.dynamicCast<const BusDefinition>();
            if (extendedBus)
            {
                return extendedBus;
            }
        }
    }

    return QSharedPointer<const BusDefinition>();
}

//-----------------------------------------------------------------------------
// Function: BusDefGroupEx::extendBusDefinition()
//-----------------------------------------------------------------------------
void BusDefGroupEx::extendBusDefinition(QSharedPointer<const BusDefinition> extendedBus)
{
//    directConnection_.setChecked(extendedBus->getDirectConnection());
//    isBroadcast_.setChecked(extendedBus->getBroadcast().toBool());
//    isAddressable_.setChecked(extendedBus->getIsAddressable());

//    directConnection_.setDisabled(true);
//    isBroadcast_.setDisabled(true);
//    isAddressable_.setDisabled(true);
}

//-----------------------------------------------------------------------------
// Function: BusDefGroupEx::removeBusExtension()
//-----------------------------------------------------------------------------
void BusDefGroupEx::removeBusExtension()
{
//    directConnection_.setDisabled(false);
//    isBroadcast_.setDisabled(false);
//    isAddressable_.setDisabled(false);
}
