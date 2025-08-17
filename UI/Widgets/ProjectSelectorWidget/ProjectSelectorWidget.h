/*
 *      Created on: 11.1.2024
 *      Author: P.H.G
 *      Filename: ProjectSelectorWidget.h
 *      Project: AWM
 *      Description :
 *          Widget for selecting a path under project locations
 */

#ifndef PROJECTSELECTORWIDGET_H
#define PROJECTSELECTORWIDGET_H

#include <QWidget>
#include <QSharedPointer>
#include <QPushButton>

class ProjectPathSelector;

//-----------------------------------------------------------------------------
//! Widget for selecting a path under project locations
//-----------------------------------------------------------------------------
class ProjectSelectorWidget : public QWidget
{
	Q_OBJECT

public:

    /*!
     *  Constructor.
     *      
     *      @param [in] parent The parent widget.
     */
    ProjectSelectorWidget(QWidget *parent = 0);
	
	//! \brief The destructor.
    ~ProjectSelectorWidget();

    /*!
     *  Get the edited path.
     *      
     *      @return Path in the edit field.
     */
    QString getPath() const;

    /*!
     *  Updates active libraries in combobox and enables path to be constructed using vlnv.
     */
    void reset();

    /*!
     *  Checks if the path is valid.
     *      
     *      @return True, if the path is in an active project, otherwise false.
     */
    bool isValid() const;

signals:

    //! \brief Emitted when the project path changes.
    void contentChanged();

public slots:

    //! Called when browse button is clicked.
    virtual void onBrowse();

    //! Called when vlnv is used to set the path. Path is appended to the active project path.
    virtual void updatePath(QString const& path);

private:
	
	//! \brief No copying
    ProjectSelectorWidget(const ProjectSelectorWidget& other);

	//! \brief No assignment
    ProjectSelectorWidget& operator=(const ProjectSelectorWidget& other);

    //! Creates the layout for the widget.
    void setupLayout();

    //! \brief Used to select the project path.
    ProjectPathSelector* projectSelector_;

    //! The browse button.
    QPushButton* browseButton_;

    //! Flag for indicating if the path set using browse or if vlnv should be used to set the path.
    bool directorySet_;

};

#endif // PROJECTSELECTORWIDGET_H
