/*
 *      Created on: 11.1.2024
 *      Author: P.H.G
 *      Filename: ProjectSettingsDelegate.h
 *      Project: AWM
 *      Description :
 *          This delegate is used to draw the check marks in the middle of default and active project columns.
 */

#ifndef PROJECTSETTINGSDELEGATE_H
#define PROJECTSETTINGSDELEGATE_H

#include <QStyledItemDelegate>

//-----------------------------------------------------------------------------
//! This delegate is used to draw the check marks in the middle of default and active project columns.
//-----------------------------------------------------------------------------
class ProjectSettingsDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:

	/*!
     *  The constructor. 
	 * 
	 *      @param [in] parent  Pointer to the owner of the delegate.
     */
    ProjectSettingsDelegate(QObject *parent);
	
	/*!
     *  The destructor.
     */
    virtual ~ProjectSettingsDelegate();

	/*!
	 *  Paint the check marks in the middle of the active and default columns.
	 *
	 *      @param [in] painter     The selected painter.
	 *      @param [in] option      Options of the painter.
	 *      @param [in] index       Current table index.
	 */
	virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:

	//! No copying
    ProjectSettingsDelegate(const ProjectSettingsDelegate& other);

	//! No assignment
    ProjectSettingsDelegate& operator=(const ProjectSettingsDelegate& other);
};

#endif // PROJECTSETTINGSDELEGATE_H
