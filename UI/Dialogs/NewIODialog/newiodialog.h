#ifndef NEWIODIALOG_H
#define NEWIODIALOG_H

#include <QDialog>

namespace Ui {
class NewIODialog;
}

class VLNV;
class NewIODialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewIODialog(QWidget *parent = 0);
    ~NewIODialog();

private:
    Ui::NewIODialog *ui;

private:
    void setupLayout();

signals:
    //! Signaled when new I/O should be created.
//    void createIO(VLNV const& vlnv, QString const& directory);
    void createBus(VLNV const& vlnv);

private slots:
    /*!
     *  Called when the user presses OK.
     */
    virtual void accept();
};

#endif // NEWIODIALOG_H
