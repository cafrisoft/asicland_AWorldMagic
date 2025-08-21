#include <IPXACTmodels/common/VLNV.h>

#ifdef _WIN32
#include <common01/SettingManager.h>
#else
#include <Common/SettingManager.h>
#endif

#include "UI/MessageBox/awmmessagebox.h"

#include "newipdialog.h"
#include "ui_newipdialog.h"


NewIPDialog::NewIPDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewIPDialog)
{
    ui->setupUi(this);

    setupLayout();

    this->setWindowTitle(tr("New IP"));
}

NewIPDialog::~NewIPDialog()
{
    delete ui;
}

//-----------------------------------------------------------------------------
// Function: NewIPDialog::setupLayout()
//-----------------------------------------------------------------------------
void NewIPDialog::setupLayout()
{
    connect(ui->okButton_, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->cancelButton_, &QPushButton::clicked, this, &QDialog::reject);
}

//-----------------------------------------------------------------------------
// Function: NewIPDialog::accept()
//-----------------------------------------------------------------------------
void NewIPDialog::accept()
{
    if (ui->vendorLineEdit_->text().isEmpty()) {
        AWMMessageBox::warning(this, QCoreApplication::applicationName(), tr("Vendor field must be filled"));
        ui->vendorLineEdit_->setFocus();
        return;
    }

    if (ui->libraryLineEdit_->text().isEmpty()) {
        AWMMessageBox::warning(this, QCoreApplication::applicationName(), tr("Library field must be filled"));
        ui->libraryLineEdit_->setFocus();
        return;
    }


    if (ui->nameLineEdit_->text().isEmpty()) {
        AWMMessageBox::warning(this, QCoreApplication::applicationName(), tr("Name field must be filled"));
        ui->nameLineEdit_->setFocus();
        return;
    }

    if (ui->versionLineEdit_->text().isEmpty()) {
        AWMMessageBox::warning(this, QCoreApplication::applicationName(), tr("Version field must be filled"));
        ui->versionLineEdit_->setFocus();
        return;
    }

    QString vendor = ui->vendorLineEdit_->text();
    QString library = ui->libraryLineEdit_->text();
    QString name = ui->nameLineEdit_->text();
    QString version = ui->versionLineEdit_->text();

    QString path = SettingManager::getDefaultLibraryLocation();
    QString directory = path + "/" + vendor + "/" + library + "/" + name + "/" + version;

    VLNV vlnv;
    vlnv.setType(VLNV::COMPONENT);
    vlnv.setVendor(vendor);
    vlnv.setLibrary(library);
    vlnv.setName(name);
    vlnv.setVersion(version);

    KactusAttribute::ProductHierarchy prodHier = KactusAttribute::ProductHierarchy::FLAT;
    KactusAttribute::Firmness firmness = KactusAttribute::Firmness::MUTABLE;
    QVector<TagData> tags;

    emit createComponent(prodHier, firmness, tags, vlnv, directory);
//    emit createComponent(prodHier, firmness, tags, vlnv);

    QDialog::accept();
}

//-----------------------------------------------------------------------------
// Function: NewIPDialog::getVendor()
//-----------------------------------------------------------------------------
QString NewIPDialog::getVendor() const
{
    return ui->vendorLineEdit_->text();
}

//-----------------------------------------------------------------------------
// Function: NewIPDialog::getLibrary()
//-----------------------------------------------------------------------------
QString NewIPDialog::getLibrary() const
{
    return ui->libraryLineEdit_->text();
}

//-----------------------------------------------------------------------------
// Function: NewIPDialog::getName()
//-----------------------------------------------------------------------------
QString NewIPDialog::getName() const
{
    return ui->nameLineEdit_->text();
}

//-----------------------------------------------------------------------------
// Function: NewIPDialog::getVersion()
//-----------------------------------------------------------------------------
QString NewIPDialog::getVersion() const
{
    return ui->versionLineEdit_->text();
}
