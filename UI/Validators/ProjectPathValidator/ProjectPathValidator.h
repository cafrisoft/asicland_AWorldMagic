/*
 *  	Created on: 11.1.2024
 *     Author: P.H.G
 * 	Filename: ProjectPathValidator.h
 *		Project: AWM
 */

#ifndef PROJECTPATHVALIDATOR_H
#define PROJECTPATHVALIDATOR_H

#include <QRegularExpressionValidator>

/*! \brief Validator that is used when editing a save path for a new item.
 * 
 */
class ProjectPathValidator : public QRegularExpressionValidator {
	Q_OBJECT

public:

	/*! \brief The constructor
	 *
	 * \param parent Pointer to the owner of this validator.
	 *
	*/
    ProjectPathValidator(QObject *parent);
	
	//! \brief The destructor
    virtual ~ProjectPathValidator() = default;

    //! \brief No copying
    ProjectPathValidator(const ProjectPathValidator& other) = delete;

    //! \brief No assignment
    ProjectPathValidator& operator=(const ProjectPathValidator& other) = delete;

	/*! \brief Validates a given input.
	 *
	 * \param input Input to validate.
	 * \param pos   Length set to input, if input is not matched.
	 *
     * \return Acceptable, if input is valid, otherwise Invalid. Intermediate is never returned.
	*/
    virtual QValidator::State validate(QString& input, int& pos) const;

public slots:

	void setUnmodifiablePath(const QString& path);

};

#endif // PROJECTPATHVALIDATOR_H
