#include "io_xmlmgr.h"
#include <QStandardItem>
#include <QDomDocument>
#include <QDomElement>
#include <QDebug>

/*
IO_XmlMgr::IO_XmlMgr() : BaseXMLMgr()
{
    LoadScriptPath("");

    orgXml.setHorizontalHeaderLabels(QStringList() << "Tag" << "Value");
 }


void IO_XmlMgr::MakeXml(const QString &vendor, const QString &name, const QString &version, const QString &description)
{
    orgXml.clear(); // Clear the existing model

    QStandardItem *rootItem = new QStandardItem("Root");
    orgXml.appendRow(rootItem);

    QStandardItem *vendorItem = new QStandardItem("ipxact:vendor");
    vendorItem->appendRow(new QStandardItem(vendor));
    rootItem->appendRow(vendorItem);

    QStandardItem *libraryItem = new QStandardItem("ipxact:library");
    libraryItem->appendRow(new QStandardItem("io"));
    rootItem->appendRow(libraryItem);

    QStandardItem *nameItem = new QStandardItem("ipxact:name");
    nameItem->appendRow(new QStandardItem(name));
    rootItem->appendRow(nameItem);

    QStandardItem *versionItem = new QStandardItem("ipxact:version");
    versionItem->appendRow(new QStandardItem(version));
    rootItem->appendRow(versionItem);

    QStandardItem *descriptionItem = new QStandardItem("ipxact:description");
    descriptionItem->appendRow(new QStandardItem(description));
    rootItem->appendRow(descriptionItem);
}


QDomElement IO_XmlMgr::XmlFromStandardItem(QDomDocument &doc, QStandardItem *item)
{
    QDomElement element = doc.createElement(item->text());

    for (int row = 0; row < item->rowCount(); ++row) {
        QStandardItem *childItem = item->child(row, 0);
        QDomElement childElement = XmlFromStandardItem(doc, childItem);
        element.appendChild(childElement);
    }

    if (item->hasChildren()) {
        return element;
    } else {
        QDomText textNode = doc.createTextNode(item->text());
        element.appendChild(textNode);
        return element;
    }
}

void IO_XmlMgr::PrintXml()
{
    QDomDocument doc;
    QDomElement root = doc.createElement("ipxact:busDefinition");
    root.setAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
    root.setAttribute("xmlns:ipxact", "http://www.accellera.org/XMLSchema/IPXACT/1685-2014");
    root.setAttribute("xmlns:kactus2", "http://kactus2.cs.tut.fi");
    root.setAttribute("xsi:schemaLocation", "http://www.accellera.org/XMLSchema/IPXACT/1685-2014 http://www.accellera.org/XMLSchema/IPXACT/1685-2014/index.xsd");

    // Append values to rootElement directly
    appendXmlElement(doc, root, "ipxact:vendor", "vendor");
    appendXmlElement(doc, root, "ipxact:library", "library");
    appendXmlElement(doc, root, "ipxact:name", "name");
    appendXmlElement(doc, root, "ipxact:version", "version");
    appendXmlElement(doc, root, "ipxact:description", "description");

    //root.appendChild(rootElement);
    //doc.appendChild(root);

    saveStringToFile(doc.toString(), aMagicRoot + "/myout.xml");

    qDebug() << doc.toString();
}

void IO_XmlMgr::appendXmlElement(QDomDocument &doc, QDomElement &parent, const QString &tagName, const QString &value)
{
    QDomElement element = doc.createElement(tagName);
    QDomText textNode = doc.createTextNode(value);
    element.appendChild(textNode);
    parent.appendChild(element);
}

*/
