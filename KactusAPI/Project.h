/*
 *      Created on: 11.1.2024
 *      Author: P.H.G
 *      Filename: Project.h
 *      Project: AWM
 *      Description :
 *          .
 */

#ifndef PROJECT_H
#define PROJECT_H

#include <QString>
#include <QStringList>
#include <QVector>

//-----------------------------------------------------------------------------
//! A class to hold Project information
//-----------------------------------------------------------------------------
//class IPXACTMODELS_EXPORT Project
class Project
{
public:
  	/*! The default constructor
	 *
     * Constructs an invalid project instance.
	*/
    Project();

	/*! The Constructor.
     *
	 *      @param [in] name    The name of the item.
     *      @param [in] vendor  The name of the vendor.
     *      @param [in] version The version number of the item.
	 */
    Project(QString const& name, QString const& vendor, QString const& version);

	/*! Copy constructor
	 *
     *      @param [in] other The project to be copied.
	 */
    Project(Project const& other) = default;

	//! The destructor
    ~Project() = default;

    /*! Get the name of the IP item in project.
     *
     *      @return The name of the IP item in project.
     */
    QString getName() const;

    /*! Set the name for the project.
     *
     *      @param [in] name The new name.
    */
    void setName(QString const& name);

    /*! Get the vendor of the project.
     *
     *      @return The name of the vendor.
     */
    QString getVendor() const;

    /*! Set the vendor for the project.
     *
     *      @param [in] vendor The vendor name to set.
    */
    void setVendor(QString const& vendor);

    /*! Get the version of the project.
     *
     *      @return the The version of the project.
     */
    QString getVersion() const;

    /*! Set the version of the project.
     *
     *      @param [in] version The new version.
    */
    void setVersion(QString const& version);

    /*!
     *  Checks if the project is empty.
     *
     *      @returns True if all project fields are empty, otherwise false.
     */
    bool isEmpty() const;

    /*! Clear the project (make all fields empty and type = INVALID)
	 *
	*/
	void clear();

    /*! Checks if the project is valid or not.
	 *
     *      @return True, if the project tag is valid, otherwise false.
	*/
	bool isValid() const;

    /*! Check if the project is valid.
	 *
	 *      @param [in] errorList The list to add the possible error messages to.
	 *      @param [in] parentIdentifier String from parent to help to identify the location of the error.
	 *
	 *      @return bool True if the state is valid and writing is possible.
	*/
    bool isValid(QVector<QString>& errors, QString const& parentIdentifier) const;

private:
	//! The name of the IP block
	QString name_;

    //! The name of the vendor
    QString vendor_;

	//! The version number of the block
    QString version_;
};

#endif /* PROJECT_H */
