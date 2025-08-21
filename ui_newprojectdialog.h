/********************************************************************************
** Form generated from reading UI file 'newprojectdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWPROJECTDIALOG_H
#define UI_NEWPROJECTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_NewProjectDialog
{
public:
    QLabel *titleLabel_;
    QWidget *formLayoutWidget;
    QFormLayout *mainFormLayout_;
    QLineEdit *nameLineEdit_;
    QLabel *nameLabel_;
    QLabel *vendorLabel_;
    QLineEdit *vendorLineEdit_;
    QLabel *versionLabel_;
    QLineEdit *versionLineEdit_;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *okButton_;
    QPushButton *cancelButton_;

    void setupUi(QDialog *NewProjectDialog)
    {
        if (NewProjectDialog->objectName().isEmpty())
            NewProjectDialog->setObjectName(QString::fromUtf8("NewProjectDialog"));
        NewProjectDialog->resize(480, 270);
        titleLabel_ = new QLabel(NewProjectDialog);
        titleLabel_->setObjectName(QString::fromUtf8("titleLabel_"));
        titleLabel_->setGeometry(QRect(10, 20, 151, 20));
        QFont font;
        font.setPointSize(16);
        font.setBold(true);
        font.setWeight(QFont::Weight(75));
        titleLabel_->setFont(font);
        formLayoutWidget = new QWidget(NewProjectDialog);
        formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(20, 60, 441, 141));
        mainFormLayout_ = new QFormLayout(formLayoutWidget);
        mainFormLayout_->setObjectName(QString::fromUtf8("mainFormLayout_"));
        mainFormLayout_->setHorizontalSpacing(10);
        mainFormLayout_->setContentsMargins(1, 0, 0, 0);
        nameLineEdit_ = new QLineEdit(formLayoutWidget);
        nameLineEdit_->setObjectName(QString::fromUtf8("nameLineEdit_"));

        mainFormLayout_->setWidget(1, QFormLayout::FieldRole, nameLineEdit_);

        nameLabel_ = new QLabel(formLayoutWidget);
        nameLabel_->setObjectName(QString::fromUtf8("nameLabel_"));
        QFont font1;
        font1.setBold(false);
        font1.setWeight(QFont::Weight(50));
        nameLabel_->setFont(font1);
        nameLabel_->setLayoutDirection(Qt::LeftToRight);
        nameLabel_->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        mainFormLayout_->setWidget(1, QFormLayout::LabelRole, nameLabel_);

        vendorLabel_ = new QLabel(formLayoutWidget);
        vendorLabel_->setObjectName(QString::fromUtf8("vendorLabel_"));
        vendorLabel_->setFont(font1);
        vendorLabel_->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        mainFormLayout_->setWidget(2, QFormLayout::LabelRole, vendorLabel_);

        vendorLineEdit_ = new QLineEdit(formLayoutWidget);
        vendorLineEdit_->setObjectName(QString::fromUtf8("vendorLineEdit_"));

        mainFormLayout_->setWidget(2, QFormLayout::FieldRole, vendorLineEdit_);

        versionLabel_ = new QLabel(formLayoutWidget);
        versionLabel_->setObjectName(QString::fromUtf8("versionLabel_"));
        versionLabel_->setFont(font1);
        versionLabel_->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        mainFormLayout_->setWidget(3, QFormLayout::LabelRole, versionLabel_);

        versionLineEdit_ = new QLineEdit(formLayoutWidget);
        versionLineEdit_->setObjectName(QString::fromUtf8("versionLineEdit_"));

        mainFormLayout_->setWidget(3, QFormLayout::FieldRole, versionLineEdit_);

        horizontalLayoutWidget = new QWidget(NewProjectDialog);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(270, 200, 191, 51));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        okButton_ = new QPushButton(horizontalLayoutWidget);
        okButton_->setObjectName(QString::fromUtf8("okButton_"));
        okButton_->setAutoRepeatInterval(100);

        horizontalLayout->addWidget(okButton_);

        cancelButton_ = new QPushButton(horizontalLayoutWidget);
        cancelButton_->setObjectName(QString::fromUtf8("cancelButton_"));

        horizontalLayout->addWidget(cancelButton_);

        QWidget::setTabOrder(nameLineEdit_, vendorLineEdit_);
        QWidget::setTabOrder(vendorLineEdit_, versionLineEdit_);
        QWidget::setTabOrder(versionLineEdit_, okButton_);
        QWidget::setTabOrder(okButton_, cancelButton_);

        retranslateUi(NewProjectDialog);

        QMetaObject::connectSlotsByName(NewProjectDialog);
    } // setupUi

    void retranslateUi(QDialog *NewProjectDialog)
    {
        NewProjectDialog->setWindowTitle(QCoreApplication::translate("NewProjectDialog", "AWorld Magic\342\204\242", nullptr));
        titleLabel_->setText(QCoreApplication::translate("NewProjectDialog", "Create Project", nullptr));
        nameLabel_->setText(QCoreApplication::translate("NewProjectDialog", "Project Name", nullptr));
        vendorLabel_->setText(QCoreApplication::translate("NewProjectDialog", "Design Vendor", nullptr));
        versionLabel_->setText(QCoreApplication::translate("NewProjectDialog", "Design Version", nullptr));
        okButton_->setText(QCoreApplication::translate("NewProjectDialog", "Create", nullptr));
        cancelButton_->setText(QCoreApplication::translate("NewProjectDialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class NewProjectDialog: public Ui_NewProjectDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWPROJECTDIALOG_H
