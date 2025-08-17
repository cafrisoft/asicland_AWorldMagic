/* 
 *  	Created on: 11.1.2024
 *     Author: P.H.G
 * 	Filename: ProjectPathEditor.cpp
 *		Project: AWM
 */

#include "ProjectPathEditor.h"

//-----------------------------------------------------------------------------
// Function: ProjectPathEditor()
//-----------------------------------------------------------------------------
ProjectPathEditor::ProjectPathEditor(QWidget *parent):
QLineEdit(parent),
unmodifiablePath_() {

}

//-----------------------------------------------------------------------------
// Function: ~ProjectPathEditor()
//-----------------------------------------------------------------------------
ProjectPathEditor::~ProjectPathEditor() {
}

//-----------------------------------------------------------------------------
// Function: setUnmodifiablePath()
//-----------------------------------------------------------------------------
void ProjectPathEditor::setUnmodifiablePath( const QString& path ) {
	unmodifiablePath_ = path;
}

//-----------------------------------------------------------------------------
// Function: keyPressEvent()
//-----------------------------------------------------------------------------
void ProjectPathEditor::keyPressEvent( QKeyEvent* event ) {

	// if user clicks delete or back space
	if (event->key() == Qt::Key_Backspace || event->key() == Qt::Key_Delete) {

		// if the current path is the minimum path
		if (text() == unmodifiablePath_) {
			
			// the minimum path is not changed
			event->accept();
			return;
		}
	}

	// normally use the base class implementation
	QLineEdit::keyPressEvent(event);
    emit contentChanged();
}
