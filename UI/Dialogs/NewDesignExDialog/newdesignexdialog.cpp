#include "newdesignexdialog.h"
#include "ui_newdesignexdialog.h"

#include <QFileInfo>

#include "UI/MessageBox/awmmessagebox.h"

#include <IPXACTmodels/common/VLNV.h>


NewDesignExDialog::NewDesignExDialog(QWidget *parent) :
    QDialog(parent),
    settings_(this),
    ui(new Ui::NewDesignExDialog)
{
    ui->setupUi(this);

    setupLayout();

    this->setWindowTitle(tr("New Design"));
}

NewDesignExDialog::~NewDesignExDialog()
{
    delete ui;
}

//-----------------------------------------------------------------------------
// Function: NewDesignExDialog::setupLayout()
//-----------------------------------------------------------------------------
void NewDesignExDialog::setupLayout()
{
    connect(ui->okButton_, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->cancelButton_, &QPushButton::clicked, this, &QDialog::reject);
}

//-----------------------------------------------------------------------------
// Function: NewDesignExDialog::getVLNV()
//-----------------------------------------------------------------------------
VLNV NewDesignExDialog::getVLNV() const
{
    return VLNV(VLNV::COMPONENT, ui->vendorLineEdit_->text(), ui->libraryComboBox_->currentText(), ui->nameLineEdit_->text(), ui->versionLineEdit_->text());
}

//-----------------------------------------------------------------------------
// Function: NewDesignExDialog::accept()
//-----------------------------------------------------------------------------
void NewDesignExDialog::accept()
{
    if (ui->vendorLineEdit_->text().isEmpty()) {
        AWMMessageBox::warning(this, QCoreApplication::applicationName(), tr("Vendor field must be filled"));
        ui->vendorLineEdit_->setFocus();
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

    emit createDesign(getVLNV());

    QDialog::accept();
}
