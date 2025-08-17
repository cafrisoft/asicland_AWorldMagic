/*
 *  	Created on: 11.1.2024
 *     Author: P.H.G
 * 	Filename: ProjectPathEditor.h
 *		Project: AWM
 */

#ifndef PROJECTPATHEDITOR_H
#define PROJECTPATHEDITOR_H

#include <QLineEdit>
#include <QString>
#include <QKeyEvent>

/*! \brief Line edit to manually set the path to save the new item to.
 *
 */
class ProjectPathEditor : public QLineEdit {
	Q_OBJECT

public:

	/*! \brief The constructor
	 *
	 * \param parent Pointer to the owner of this line editor.
	 *
	*/
    ProjectPathEditor(QWidget *parent);

	//! \brief The destructor
    virtual ~ProjectPathEditor();

	/*! \brief Set the minimum path.
	 *
	 * \param path The path to set.
	 *
	*/
	void setUnmodifiablePath(const QString& path);

signals:

    //! \brief Emitted when the text in editor changes.
    void contentChanged();

protected:

	/*! \brief Handler for key press events.
	 *
	 * \param event Pointer to the key event.
	 *
	*/
	virtual void keyPressEvent(QKeyEvent* event);

private:
	
	//! \brief No copying
    ProjectPathEditor(const ProjectPathEditor& other);

	//! \brief No assignment
    ProjectPathEditor& operator=(const ProjectPathEditor& other);

	//! \brief The minimum path that can not be changed.
	QString unmodifiablePath_;
};

#endif // PROJECTPATHEDITOR_H
