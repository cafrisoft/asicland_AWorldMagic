/********************************************************************************
** Form generated from reading UI file 'newipdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWIPDIALOG_H
#define UI_NEWIPDIALOG_H

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

class Ui_NewIPDialog
{
public:
    QLabel *titleLabel_;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *okButton_;
    QPushButton *cancelButton_;
    QWidget *formLayoutWidget;
    QFormLayout *mainFormLayout_;
    QLabel *vendorLabel_;
    QLineEdit *vendorLineEdit_;
    QLabel *versionLabel_;
    QLineEdit *versionLineEdit_;
    QLabel *libraryLabel_;
    QLineEdit *libraryLineEdit_;
    QLabel *nameLabel_;
    QLineEdit *nameLineEdit_;

    void setupUi(QDialog *NewIPDialog)
    {
        if (NewIPDialog->objectName().isEmpty())
            NewIPDialog->setObjectName(QString::fromUtf8("NewIPDialog"));
        NewIPDialog->resize(480, 310);
        titleLabel_ = new QLabel(NewIPDialog);
        titleLabel_->setObjectName(QString::fromUtf8("titleLabel_"));
        titleLabel_->setGeometry(QRect(10, 20, 151, 20));
        QFont font;
        font.setPointSize(16);
        font.setBold(true);
        font.setWeight(QFont::Weight(75));
        titleLabel_->setFont(font);
        horizontalLayoutWidget = new QWidget(NewIPDialog);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(270, 240, 191, 51));
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

        formLayoutWidget = new QWidget(NewIPDialog);
        formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(20, 60, 441, 181));
        mainFormLayout_ = new QFormLayout(formLayoutWidget);
        mainFormLayout_->setObjectName(QString::fromUtf8("mainFormLayout_"));
        mainFormLayout_->setHorizontalSpacing(10);
        mainFormLayout_->setContentsMargins(50, 0, 0, 0);
        vendorLabel_ = new QLabel(formLayoutWidget);
        vendorLabel_->setObjectName(QString::fromUtf8("vendorLabel_"));
        QFont font1;
        font1.setBold(false);
        font1.setWeight(QFont::Weight(50));
        vendorLabel_->setFont(font1);
        vendorLabel_->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        mainFormLayout_->setWidget(1, QFormLayout::LabelRole, vendorLabel_);

        vendorLineEdit_ = new QLineEdit(formLayoutWidget);
        vendorLineEdit_->setObjectName(QString::fromUtf8("vendorLineEdit_"));

        mainFormLayout_->setWidget(1, QFormLayout::FieldRole, vendorLineEdit_);

        versionLabel_ = new QLabel(formLayoutWidget);
        versionLabel_->setObjectName(QString::fromUtf8("versionLabel_"));
        versionLabel_->setFont(font1);
        versionLabel_->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        mainFormLayout_->setWidget(4, QFormLayout::LabelRole, versionLabel_);

        versionLineEdit_ = new QLineEdit(formLayoutWidget);
        versionLineEdit_->setObjectName(QString::fromUtf8("versionLineEdit_"));

        mainFormLayout_->setWidget(4, QFormLayout::FieldRole, versionLineEdit_);

        libraryLabel_ = new QLabel(formLayoutWidget);
        libraryLabel_->setObjectName(QString::fromUtf8("libraryLabel_"));
        libraryLabel_->setFont(font1);
        libraryLabel_->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        mainFormLayout_->setWidget(2, QFormLayout::LabelRole, libraryLabel_);

        libraryLineEdit_ = new QLineEdit(formLayoutWidget);
        libraryLineEdit_->setObjectName(QString::fromUtf8("libraryLineEdit_"));

        mainFormLayout_->setWidget(2, QFormLayout::FieldRole, libraryLineEdit_);

        nameLabel_ = new QLabel(formLayoutWidget);
        nameLabel_->setObjectName(QString::fromUtf8("nameLabel_"));
        nameLabel_->setFont(font1);
        nameLabel_->setLayoutDirection(Qt::LeftToRight);
        nameLabel_->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        mainFormLayout_->setWidget(3, QFormLayout::LabelRole, nameLabel_);

        nameLineEdit_ = new QLineEdit(formLayoutWidget);
        nameLineEdit_->setObjectName(QString::fromUtf8("nameLineEdit_"));

        mainFormLayout_->setWidget(3, QFormLayout::FieldRole, nameLineEdit_);

        QWidget::setTabOrder(vendorLineEdit_, libraryLineEdit_);
        QWidget::setTabOrder(libraryLineEdit_, nameLineEdit_);
        QWidget::setTabOrder(nameLineEdit_, versionLineEdit_);
        QWidget::setTabOrder(versionLineEdit_, okButton_);
        QWidget::setTabOrder(okButton_, cancelButton_);

        retranslateUi(NewIPDialog);

        QMetaObject::connectSlotsByName(NewIPDialog);
    } // setupUi

    void retranslateUi(QDialog *NewIPDialog)
    {
        NewIPDialog->setWindowTitle(QCoreApplication::translate("NewIPDialog", "Dialog", nullptr));
        titleLabel_->setText(QCoreApplication::translate("NewIPDialog", "Create IP", nullptr));
        okButton_->setText(QCoreApplication::translate("NewIPDialog", "Create", nullptr));
        cancelButton_->setText(QCoreApplication::translate("NewIPDialog", "Cancel", nullptr));
        vendorLabel_->setText(QCoreApplication::translate("NewIPDialog", "Vendor", nullptr));
        versionLabel_->setText(QCoreApplication::translate("NewIPDialog", "Version", nullptr));
        libraryLabel_->setText(QCoreApplication::translate("NewIPDialog", "Library", nullptr));
        nameLabel_->setText(QCoreApplication::translate("NewIPDialog", "Name", nullptr));
    } // retranslateUi

};

namespace Ui {
    class NewIPDialog: public Ui_NewIPDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWIPDIALOG_H
