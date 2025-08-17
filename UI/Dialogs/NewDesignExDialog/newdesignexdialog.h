#ifndef NEWDESIGNEXDIALOG_H
#define NEWDESIGNEXDIALOG_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class NewDesignExDialog;
}

class VLNV;
class NewDesignExDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewDesignExDialog(QWidget *parent = 0);
    ~NewDesignExDialog();

private:
    Ui::NewDesignExDialog *ui;

protected:
    QSettings settings_;

private:
    void setupLayout();

protected:
    /*!
     *  Generates a VLNV that matches the contents of the editor.
     */
    VLNV getVLNV() const;

signals:
    //! Signaled when new project should be created.
    void createDesign(VLNV const& project);

private slots:
    /*!
     *  Called when the user presses OK.
     */
    virtual void accept();
};

#endif // NEWDESIGNEXDIALOG_H
