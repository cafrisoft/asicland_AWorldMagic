/*
 *      Created on: 10.6.2024
 *      Author: P.H.G
 *      Filename: ProjectLibrary.h
 *      Project: AWM
 *      Description :
 *          .
 */

#ifndef PROJECTLIBRARY_H
#define PROJECTLIBRARY_H

#include <QString>
#include <QStringList>
#include <QVector>

//-----------------------------------------------------------------------------
//! A class to hold ProjectLibrary information
//-----------------------------------------------------------------------------
//class IPXACTMODELS_EXPORT ProjectLibrary
class ProjectLibrary
{
public:
  	/*! The default constructor
	 *
     * Constructs an invalid project library instance.
	*/
    ProjectLibrary();

	/*! The Constructor.
     *
	 *      @param [in] name    The name of the item.
     *      @param [in] vendor  The name of the vendor.
     *      @param [in] version The version number of the item.
	 */
    ProjectLibrary(QString const& name, QString const& vendor, QString const& version);

	/*! Copy constructor
	 *
     *      @param [in] other The ProjectLibrary to be copied.
	 */
    ProjectLibrary(ProjectLibrary const& other) = default;

	//! The destructor
    ~ProjectLibrary() = default;

    /*! Get the name of the IP item in project library.
     *
     *      @return The name of the IP item in project library.
     */
    QString getName() const;

    /*! Set the name for the project library.
     *
     *      @param [in] name The new name.
    */
    void setName(QString const& name);

    /*! Get the vendor of the project library.
     *
     *      @return The name of the vendor.
     */
    QString getVendor() const;

    /*! Set the vendor for the project library.
     *
     *      @param [in] vendor The vendor name to set.
    */
    void setVendor(QString const& vendor);

    /*! Get the version of the project library.
     *
     *      @return the The version of the project library.
     */
    QString getVersion() const;

    /*! Set the version of the project library.
     *
     *      @param [in] version The new version.
    */
    void setVersion(QString const& version);

    /*!
     *  Checks if the project library is empty.
     *
     *      @returns True if all project library fields are empty, otherwise false.
     */
    bool isEmpty() const;

    /*! Clear the project library (make all fields empty and type = INVALID)
	 *
	*/
	void clear();

    /*! Checks if the project library is valid or not.
	 *
     *      @return True, if the project library tag is valid, otherwise false.
	*/
	bool isValid() const;

    /*! Check if the project library is valid.
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

#endif /* PROJECTLIBRARY_H */
