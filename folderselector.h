#ifndef FOLDERSELECTOR_H
#define FOLDERSELECTOR_H

#include <QFileDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QApplication>
#include <QWidget>

class FolderSelector : public QDialog
{
    Q_OBJECT

public:
    FolderSelector(QWidget *parent = nullptr) : QDialog(parent) {
        QVBoxLayout *layout = new QVBoxLayout(this);

        QPushButton *button = new QPushButton("Select Folder", this);
        label = new QLabel("Selected folder will be displayed here.", this);

        layout->addWidget(button);
        layout->addWidget(label);

        connect(button, &QPushButton::clicked, this, &FolderSelector::onButtonClicked);
    }

private slots:
    void onButtonClicked() {
        QString dir = QFileDialog::getExistingDirectory(this, "Select a directory",
                                                        "/home",
                                                        QFileDialog::ShowDirsOnly
                                                        | QFileDialog::DontResolveSymlinks);
        if(!dir.isEmpty()) {
            label->setText(dir);
        }
    }

private:
    QLabel *label;
};

#endif // FOLDERSELECTOR_H
