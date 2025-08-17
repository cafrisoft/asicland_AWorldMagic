#include <QDir>
#include <QFileInfo>

#include <IPXACTmodels/common/VLNV.h>

//#include <Common/SettingManager.h>

#include "UI/MessageBox/awmmessagebox.h"

#include "newiodialog.h"
#include "ui_newiodialog.h"


NewIODialog::NewIODialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewIODialog)
{
    ui->setupUi(this);

    setupLayout();

    this->setWindowTitle(tr("New I/O"));
}

NewIODialog::~NewIODialog()
{
    delete ui;
}

//-----------------------------------------------------------------------------
// Function: NewIODialog::setupLayout()
//-----------------------------------------------------------------------------
void NewIODialog::setupLayout()
{
    connect(ui->okButton_, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->cancelButton_, &QPushButton::clicked, this, &QDialog::reject);
}

//-----------------------------------------------------------------------------
// Function: NewIODialog::accept()
//-----------------------------------------------------------------------------
void NewIODialog::accept()
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

//    QString path = SettingManager::getDefaultLibraryLocation();
//    QString directory = path + "/" + vendor + "/" + library + "/" + name + "/" + version;

    VLNV vlnv;
    vlnv.setType(VLNV::COMPONENT);
    vlnv.setVendor(vendor);
    vlnv.setLibrary(library);
    vlnv.setName(name);
    vlnv.setVersion(version);

//    emit createBus(vlnv, directory);
    emit createBus(vlnv);

    QDialog::accept();
}
