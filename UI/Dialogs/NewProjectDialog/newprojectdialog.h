#ifndef NEWPROJECTDIALOG_H
#define NEWPROJECTDIALOG_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class NewProjectDialog;
}

class Project;
class NewProjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewProjectDialog(QWidget *parent = 0);
    ~NewProjectDialog();

private:
    Ui::NewProjectDialog *ui;

protected:
    QSettings settings_;

private:
    void setupLayout();

protected:
    /*!
     *  Generates a project that matches the contents of the editor.
     */
    Project getProject() const;

signals:
    //! Signaled when new project should be created.
    void createProject(Project const& project, QString const& directory);

private slots:
    /*!
     *  Called when the user presses OK.
     */
    virtual void accept();
};

#endif // NEWPROJECTDIALOG_H
