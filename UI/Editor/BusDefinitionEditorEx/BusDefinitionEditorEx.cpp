//-----------------------------------------------------------------------------
// File: BusDefinitionEditorEx.cpp
//-----------------------------------------------------------------------------
// Project: AWM
// Author: P.H.G
// Date: 19.06.2024
//
// Description:
// Bus defition editor is an editor for Bus- and Abstraction definitions.
//-----------------------------------------------------------------------------

#include <QApplication>
#include <QMenu>
#include <QFile>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QSplitter>
#include <QVBoxLayout>

#include <IPXACTmodels/common/VLNV.h>
#include <IPXACTmodels/BusDefinition/validators/BusDefinitionValidator.h>
#include <IPXACTmodels/AbstractionDefinition/validators/AbstractionDefinitionValidator.h>

#include <KactusAPI/SystemVerilogExpressionParser.h>
#include <KactusAPI/PortAbstractionInterface.h>

#include <UI/Dialogs/newObjectDialog/newobjectdialog.h>
#include <UI/Widgets/summaryLabel/summarylabel.h>

#include "BusDefinitionEditorEx.h"


//-----------------------------------------------------------------------------
// Function: BusDefinitionEditorEx::BusDefinitionEditorEx()
//-----------------------------------------------------------------------------
BusDefinitionEditorEx::BusDefinitionEditorEx(QWidget *parent,
                                             LibraryInterface* libHandler,
                                             QSharedPointer<BusDefinition> busDef,
                                             QSharedPointer<AbstractionDefinition> absDef,
                                             bool disableBusDef):
//    QSharedPointer<BusDefinition> busDef):
    TabDocument(parent, DOC_PROTECTION_SUPPORT),
    libHandler_(libHandler),
    busDef_(busDef),
    busDefGroup_(libHandler, this),
    absDef_(absDef),
    absDefGroup_(libHandler, createPortAbstractionInterface(), createPortAbstractionInterface(), this),
    expressionParser_(new SystemVerilogExpressionParser()),
    busDefinitionValidator_(new BusDefinitionValidator(libHandler, expressionParser_)),
    absDefinitionValidator_(new AbstractionDefinitionValidator(libHandler, expressionParser_)),
    stackedWidgets_(new QStackedWidget(this))
{
    setDocumentType(tr("Bus Definition"));

    if (busDef_) {
        busDefGroup_.setBusDef(busDef_);
    }

    if (absDef_) {
        absDefGroup_.setAbsDef(absDef_);
    }

    if (busDef_ || absDef_) {
        VLNV vlnv = busDef_->getVlnv();
        setDocumentName("[" + libHandler_->getType() + "] " + vlnv.getName() + " (" + vlnv.getVersion() + ")");

        // Open in unlocked mode by default only if the version is draft.
        setProtection(vlnv.getVersion() != "draft");
    }
    else
    {
        setProtection(true);
    }

    setupLayout();

    connect(&busDefGroup_, SIGNAL(contentChanged()), this, SIGNAL(contentChanged()), Qt::UniqueConnection);
    connect(&absDefGroup_, SIGNAL(contentChanged()), this, SIGNAL(contentChanged()), Qt::UniqueConnection);

    connect(&absDefGroup_, SIGNAL(portRemoved(const QString&, const General::InterfaceMode)),
            this, SIGNAL(portRemoved(const QString&, const General::InterfaceMode)), Qt::UniqueConnection);

    connect(&absDefGroup_, SIGNAL(noticeMessage(const QString&)), this, SIGNAL(noticeMessage(const QString&)), Qt::UniqueConnection);
    connect(&absDefGroup_, SIGNAL(errorMessage(const QString&)), this, SIGNAL(errorMessage(const QString&)), Qt::UniqueConnection);
}

//-----------------------------------------------------------------------------
// Function: BusDefinitionEditorEx::getIdentifyingVLNV()
//-----------------------------------------------------------------------------
VLNV BusDefinitionEditorEx::getIdentifyingVLNV() const
{
    return getDocumentVLNV();
}

//-----------------------------------------------------------------------------
// Function: BusDefinitionEditorEx::refresh()
//-----------------------------------------------------------------------------
void BusDefinitionEditorEx::refresh()
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    if (absDef_)
    {
        absDef_ = libHandler_->getModel(absDef_->getVlnv()).dynamicCast<AbstractionDefinition>();
        absDefGroup_.setAbsDef(absDef_);
    }

    if (busDef_)
    {
        busDef_ = libHandler_->getModel(busDef_->getVlnv()).dynamicCast<BusDefinition>();
        busDefGroup_.setBusDef(busDef_);
//        absDefGroup_.setBusDef(busDef_);
    }

    // The document is no longer modified.
    setModified(false);
    TabDocument::refresh();

    QApplication::restoreOverrideCursor();
}

//-----------------------------------------------------------------------------
// Function: setProtection()
//-----------------------------------------------------------------------------
void BusDefinitionEditorEx::setProtection(bool locked)
{
    TabDocument::setProtection(locked);
    setEnabled(!locked);
}

//-----------------------------------------------------------------------------
// Function: BusDefinitionEditorEx::getDocumentVLNV()
//-----------------------------------------------------------------------------
VLNV BusDefinitionEditorEx::getDocumentVLNV() const
{
    // If bus definition is being edited then use it as identifier.
    if (absDef_)
    {
        return absDef_->getVlnv();
    }
    else if (busDef_)
    {
        return busDef_->getVlnv();
    }
    else
    {
        return VLNV();
    }
}

//-----------------------------------------------------------------------------
// Function: BusDefinitionEditorEx::setBusDef()
//-----------------------------------------------------------------------------
void BusDefinitionEditorEx::setBusDef(QSharedPointer<BusDefinition> busDef)
{
    Q_ASSERT_X(busDef, "BusDefinitionEditor setBusDef", "Null Bus Definition pointer given as parameter");

    busDef_ = busDef;

    busDefGroup_.setBusDef(busDef_);
//    absDefGroup_.setBusDef(busDef_);
    busDefGroup_.setDisabled(false);

    VLNV vlnv = busDef_->getVlnv();
    setDocumentName(vlnv.getName() + " (" + vlnv.getVersion() + ")");
}

//-----------------------------------------------------------------------------
// Function: AbstractionDefinitionEditor::setAbsDef()
//-----------------------------------------------------------------------------
void BusDefinitionEditorEx::setAbsDef(QSharedPointer<AbstractionDefinition> absDef)
{
    absDef_ = absDef;

    if (absDef_)
    {
        absDefGroup_.setAbsDef(absDef_);
    }

    absDefGroup_.setDisabled(!absDef_);
}

//-----------------------------------------------------------------------------
// Function: BusDefinitionEditorEx::validate()
//-----------------------------------------------------------------------------
bool BusDefinitionEditorEx::validate(QVector<QString>& errorList)
{
    // if bus definition is being edited
    if (busDefGroup_.isEnabled())
    {
        QVector<QString> busDefinitionErrors;
        busDefinitionValidator_->findErrorsIn(busDefinitionErrors, busDef_);

        if (busDefinitionErrors.isEmpty())
        {
            libHandler_->writeModelToFile(busDef_);
            busDefinitionSaved_ = true;
        }
        else
        {
            errorList.append(busDefinitionErrors);
        }
    }

    // if abstraction definition is being edited
    if (absDefGroup_.isEnabled())
    {
        absDefinitionValidator_->findErrorsIn(errorList, absDef_);
    }

    return errorList.isEmpty();

//    QVector<QString> busDefinitionErrors;
//    busDefinitionValidator_->findErrorsIn(busDefinitionErrors, busDef_);

//    if (busDefinitionErrors.isEmpty() == false)
//    {
//        errorList.append(busDefinitionErrors);
//    }

////    return busDefinitionErrors.isEmpty();

//    // if abstraction definition is being edited
//    if (absDefGroup_.isEnabled())
//    {
//        absDefinitionValidator_->findErrorsIn(errorList, absDef_);
//    }

//    return errorList.isEmpty();
}

//-----------------------------------------------------------------------------
// Function: BusDefinitionEditorEx::save()
//-----------------------------------------------------------------------------
bool BusDefinitionEditorEx::save()
{
    // If bus definition is being edited, save it.
    if (busDefGroup_.isEnabled())
    {
        libHandler_->writeModelToFile(busDef_);
    }


    // If abstraction definition is being edited, save it.
    if (absDefGroup_.isEnabled())
    {
        absDefGroup_.save();
        libHandler_->writeModelToFile(absDef_);
    }

    return TabDocument::save();
}

//-----------------------------------------------------------------------------
// Function: BusDefinitionEditorEx::saveAs()
//-----------------------------------------------------------------------------
bool BusDefinitionEditorEx::saveAs()
{
    // Ask the user for a new VLNV along with the directory.
    VLNV vlnv;

    VLNV busDefVLNV;
    QString busDirectory;

    VLNV absDefVLNV;
    QString absDirectory;

    if (!NewObjectDialog::saveAsDialog(this, libHandler_, busDef_->getVlnv(), vlnv, busDirectory))
    {
        return false;
    }

    busDefVLNV = vlnv;
    busDefVLNV.setType(VLNV::BUSDEFINITION);
    busDef_->setVlnv(busDefVLNV);

    // If also Abstraction Definition is being edited, save as that too.
    if (absDef_)
    {
        // Remove the possible .busDef from the end of the name field.
        QString absDefName = busDefVLNV.getName();
        absDefName = absDefName.remove(".busDef", Qt::CaseInsensitive);

        absDefGroup_.save();

        // Create automatically vlnv for the abstraction definition.
        absDefVLNV = VLNV(VLNV::ABSTRACTIONDEFINITION, vlnv.getVendor(), vlnv.getLibrary(),
                          absDefName + ".absDef", vlnv.getVersion());

        // By default, both bus def and abs def are saved in same directory.
        absDirectory = busDirectory;

        // if the automatic abs def vlnv is taken
        if (libHandler_->contains(absDefVLNV))
        {
            if (!NewObjectDialog::saveAsDialog(this, libHandler_, absDefVLNV, vlnv, absDirectory))
            {
                return false;
            }
            // Save the created abstraction definition vlnv
            absDefVLNV = vlnv;
        }

        absDef_->setVlnv(absDefVLNV);
        absDef_->setBusType(busDefVLNV);

        libHandler_->writeModelToFile(absDirectory, absDef_);
    }

    libHandler_->writeModelToFile(busDirectory, busDef_);
    setDocumentName(vlnv.getName() + " (" + vlnv.getVersion() + ")");
    return TabDocument::saveAs();
}


//-----------------------------------------------------------------------------
// Function: BusDefinitionEditorEx::showBusDefinitionEditor()
//-----------------------------------------------------------------------------
void BusDefinitionEditorEx::showBusDefinitionEditor()
{
    stackedWidgets_->setCurrentIndex(0);
}

//-----------------------------------------------------------------------------
// Function: BusDefinitionEditorEx::showAbstractionDefinitionEditor()
//-----------------------------------------------------------------------------
void BusDefinitionEditorEx::showAbstractionDefinitionEditor()
{
    stackedWidgets_->setCurrentIndex(1);
}

//-----------------------------------------------------------------------------
// Function: BusDefinitionEditorEx::showEvent()
//-----------------------------------------------------------------------------
void BusDefinitionEditorEx::showEvent(QShowEvent* event)
{
    TabDocument::showEvent(event);
    emit helpUrlRequested("definitions/busdefinition.html");
}

//-----------------------------------------------------------------------------
// Function: BusDefinitionEditorEx::setupLayout()
//-----------------------------------------------------------------------------
void BusDefinitionEditorEx::setupLayout()
{
    // bus definision
    QWidget* busDefGroup = new QWidget(this);
    QVBoxLayout* busLayout = new QVBoxLayout(busDefGroup);

    busLayout->addWidget(new SummaryLabel(tr("General"), tr("This is General Information of I/O."), this, true), 0, Qt::AlignLeft | Qt::AlignVCenter);
    busLayout->addWidget(&busDefGroup_);
    busLayout->setContentsMargins(0, 0, 0, 0);

    // Abstraction definition
    QWidget* absDefGroup = new QWidget(this);
    QVBoxLayout* absLayout = new QVBoxLayout(absDefGroup);
    absLayout->addWidget(new SummaryLabel(tr("Wire Port"), tr("Wire Port Information of I/O."), this, true), 0, Qt::AlignLeft | Qt::AlignVCenter);
    absLayout->addWidget(&absDefGroup_);
    absLayout->setContentsMargins(0, 0, 0, 0);

    stackedWidgets_->addWidget(busDefGroup);
    stackedWidgets_->addWidget(absDefGroup);

    // Left menu
    QLabel *menuLabel = new QLabel("<html>\
                                   <a style='font-size: 16px; font-weight: bold;  text-align:cener;'>I/O<br>Detail</a><br>\
                               </html>", this);

    QPushButton *generalButton = new QPushButton("General", this);
//    generalButton->setFixedSize(24, 24);
    connect(generalButton, &QPushButton::clicked, this, &BusDefinitionEditorEx::showBusDefinitionEditor);

//    QMenu *menu = new QMenu(this);
//    menu->addAction(tr("&First Item"));
//    menu->addAction(tr("&Second Item"));
//    menu->addAction(tr("&Third Item"));
//    menu->addAction(tr("F&ourth Item"));
//    generalButton->setMenu(menu);

    QPushButton *wirePortButton = new QPushButton("Wire Port", this);
//    wirePortButton->setFixedSize(24, 24);
    connect(wirePortButton, &QPushButton::clicked, this, &BusDefinitionEditorEx::showAbstractionDefinitionEditor);

    QWidget *menuWidget = new QWidget();
    QVBoxLayout *menuLayout = new QVBoxLayout(menuWidget);

    // 사이즈 설정
    menuWidget->setFixedWidth(200); // 고정된 너비 설정
    menuWidget->setMinimumHeight(400); // 최소 높이 설정
    menuWidget->setMaximumHeight(600); // 최대 높이 설정

    QWidget *mainWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(mainWidget);

    QSplitter *splitter = new QSplitter(Qt::Horizontal, mainWidget);
    splitter->setStyleSheet("QSplitter::handle { background-color: #444444; }");

    menuLayout->addSpacing(5);
    menuLayout->addWidget(menuLabel);
    menuLayout->addSpacing(10);
    menuLayout->addWidget(generalButton);
//    menuLayout->addSpacing(35);
    menuLayout->addWidget(wirePortButton);

    menuLayout->addStretch();

    splitter->addWidget(menuWidget);
    splitter->addWidget(stackedWidgets_);

    mainLayout->addWidget(splitter);

    // 왼쪽 버튼 영역의 크기 고정
    menuWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    this->setLayout(mainLayout);
}

//-----------------------------------------------------------------------------
// Function: BusDefinitionEditorEx::createPortAbstractionInterface()
//-----------------------------------------------------------------------------
PortAbstractionInterface* BusDefinitionEditorEx::createPortAbstractionInterface()
{
    PortAbstractionInterface* portInterface(new PortAbstractionInterface());
    return portInterface;
}
