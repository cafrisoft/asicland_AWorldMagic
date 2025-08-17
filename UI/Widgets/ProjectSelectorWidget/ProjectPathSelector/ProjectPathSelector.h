/* 
 *  	Created on: 11.1.2024
 *     Author: P.H.G
 * 	Filename: ProjectPathSelector.h
 *		Project: AWM
 */

#ifndef PROJECTPATHSELECTOR_H
#define PROJECTPATHSELECTOR_H

#include <UI/Widgets/ProjectSelectorWidget/ProjectPathEditor/ProjectPathEditor.h>
#include <UI/Validators/ProjectPathValidator/ProjectPathValidator.h>

#include <QComboBox>
#include <QSettings>
#include <QStringList>

/*! \brief Combo box to select one of the previously set project locations.
 *
 */
class ProjectPathSelector : public QComboBox {
	Q_OBJECT

public:

	/*! \brief The constructor
	 *
	 * \param parent Pointer to the owner of this combo box.
	 *
	*/
        ProjectPathSelector(QWidget *parent);
	
	//! \brief The destructor
        virtual ~ProjectPathSelector();

    bool isValid();

	/*! \brief Get the currently selected base location.
	 *
         * \return QString containing the selected project location.
	*/
    QString currentLocation() const;

    /*!
     *  Searches the active project locations.
     */
    void refresh();

    /*!
     *  Gets the active project paths.
     *
     *      @return     The active project locations.
     */
    QStringList getProjectLocations();

    /*!
     *  Checks if there is a project for a given path.
     *
     *      @param [in] path   The path to search for.
     *
     *      @return     True, if the path is under any of the active libraries, otherwise false.
     */
    bool hasIndexFor(QString const& path);

    /*!
     *  Sets the index for the corresponding project for a given path.
     *
     *      @param [in] path   The path under a project.
     *
     *      @return     True, if the path is under any of the active libraries and can be set, otherwise false.
     */
    bool setIndexFor(QString const& path);

signals:

    //! \brief Emitted when the project path changes.
    void contentChanged();

private slots:

	/*! \brief Handler for index changes on combo box.
	 *
	*/
	void onIndexChanged();

private:
	
	//! \brief No copying
        ProjectPathSelector(const ProjectPathSelector& other);

	//! \brief No assignment
        ProjectPathSelector& operator=(const ProjectPathSelector& other);

	//! \brief Contains the Kactus2 settings.
	QSettings settings_;

	//! \brief Validator used for the editable combo box.
    ProjectPathValidator* validator_;

        //! \brief the project locations from the settings.
        QStringList projectLocations_;

	//! \brief The editor to display and edit the path.
        ProjectPathEditor* pathEditor_;
};

#endif // PROJECTPATHSELECTOR_H
