//-----------------------------------------------------------------------------
// File: ApiDefinitionReader.h
//-----------------------------------------------------------------------------
// Project: Kactus 2
// Author: Janne Virtanen
// Date: 28.01.2016
//
// Description:
// XML reader class for Kactus2 API definition.
//-----------------------------------------------------------------------------

#ifndef APIDEFINITIONREADER_H
#define APIDEFINITIONREADER_H

#include <IPXACTmodels/common/DocumentReader.h>
#include <IPXACTmodels/ipxactmodels_global.h>

#include <QDomNode>
#include <QSharedPointer>

class ApiDefinition;

//-----------------------------------------------------------------------------
//! XML reader class for IP-XACT Parameter element.
//-----------------------------------------------------------------------------
//class IPXACTMODELS_EXPORT ApiDefinitionReader : public DocumentReader
class ApiDefinitionReader : public DocumentReader
{
public:

	//! The constructor.
	ApiDefinitionReader();

	//! The destructor.
	virtual ~ApiDefinitionReader();

    /*!
     *  Creates a Api definition from XML description.
     *
     *      @param [in] document   The XML document to create the Api definition from.
     *
     *      @return The created Api definition.
     */
    QSharedPointer<ApiDefinition> createApiDefinitionFrom(QDomNode const& document) const;

    //! Disable copying.
    ApiDefinitionReader(ApiDefinitionReader const& rhs) = delete;
    ApiDefinitionReader& operator=(ApiDefinitionReader const& rhs) = delete;

private:

    /*!
     *  Parses all found data types from the given XML node.
     *
     *      @param [in] node The source XML node.
     */
    void parseDataTypes(QDomNode& node, QSharedPointer<ApiDefinition> apiDefinition) const;

    /*!
     *  Parses all found API functions from the given XML node.
     *
     *      @param [in] node The source XML node.
     */
    void parseFunctions(QDomNode& node, QSharedPointer<ApiDefinition> apiDefinition) const;
};

#endif // ApiDEFINITIONREADER_H
