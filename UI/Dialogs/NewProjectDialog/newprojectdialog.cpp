#include "newprojectdialog.h"
#include "ui_newprojectdialog.h"

#include <QDir>
#include <QFileInfo>

#include <KactusAPI/Project.h>

#include "UI/MessageBox/awmmessagebox.h"


NewProjectDialog::NewProjectDialog(QWidget *parent) :
    QDialog(parent),
    settings_(this),
    ui(new Ui::NewProjectDialog)
{
    ui->setupUi(this);

    setupLayout();

    this->setWindowTitle(tr("New Project"));
}

NewProjectDialog::~NewProjectDialog()
{
    delete ui;
}

//-----------------------------------------------------------------------------
// Function: NewProjectDialog::setupLayout()
//-----------------------------------------------------------------------------
void NewProjectDialog::setupLayout()
{
    connect(ui->okButton_, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->cancelButton_, &QPushButton::clicked, this, &QDialog::reject);
}

//-----------------------------------------------------------------------------
// Function: NewProjectDialog::getProject()
//-----------------------------------------------------------------------------
Project NewProjectDialog::getProject() const
{
    return Project(ui->nameLineEdit_->text(), ui->vendorLineEdit_->text(), ui->versionLineEdit_->text());
}

//-----------------------------------------------------------------------------
// Function: NewProjectDialog::accept()
//-----------------------------------------------------------------------------
void NewProjectDialog::accept()
{
    if (ui->nameLineEdit_->text().isEmpty()) {
        AWMMessageBox::warning(this, QCoreApplication::applicationName(), tr("Project name field must be filled"));
        ui->nameLineEdit_->setFocus();
        return;
    }

    if (ui->vendorLineEdit_->text().isEmpty()) {
        AWMMessageBox::warning(this, QCoreApplication::applicationName(), tr("Design vendor field must be filled"));
        ui->vendorLineEdit_->setFocus();
        return;
    }

    if (ui->versionLineEdit_->text().isEmpty()) {
        AWMMessageBox::warning(this, QCoreApplication::applicationName(), tr("Design version field must be filled"));
        ui->versionLineEdit_->setFocus();
        return;
    }

    QString rootDirectory = settings_.value(QStringLiteral("ROOT/Location")).toString();

    QString projectPath = rootDirectory + "/project/" + ui->nameLineEdit_->text();

    if (!QFileInfo(projectPath).isAbsolute())
    {
        projectPath = QFileInfo(projectPath).absoluteFilePath();
    }

    QDir dir(projectPath);

    if (dir.exists()) {
        AWMMessageBox::warning(this, QCoreApplication::applicationName(), tr("Project name aleady occupied"));
        ui->nameLineEdit_->setFocus();
        return;
    }

    emit createProject(getProject(), projectPath);

    QDialog::accept();
}
