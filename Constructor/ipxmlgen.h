#ifndef IPXMLGEN_H
#define IPXMLGEN_H

#include <QObject>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QDomDocument>
#include <QDomElement>
#include <QDebug>
#include <QtXml>
#include <QTableWidget>
#include <QComboBox>
#include "basexmlmgr.h"
#include "ipxactgenerator.h"

class IPXmlGen : public BaseXMLMgr
{
//private:
       // IPXACTGeneratorForIO generator;
public:
    IPXmlGen();

    void LoadGenXmlInfo(QString xmlFile, QString &vendor, QString &name, QString &version, QString &description, QString &author);

    QString GetSavePath(const QString &vendor, const QString &name, const QString &version)
    {
        QString savepath = libraryPath +"/" +vendor + "/ip/"+ name + "/" + version + "/";
        return savepath;
    }

    QString GetIPDataXMLPath(const QString &vendor, const QString &name, const QString &version)
    {
        QString savepath = GetSavePath(vendor, name, version);
        QString outfile = savepath + name+".absDef."+version + ".xml";
        return outfile;
    }

    QString GetGeneralXMLPath(const QString &vendor, const QString &name, const QString &version)
    {
        QString savepath = GetSavePath(vendor, name, version);
        QString outfile = savepath + name+"."+version + ".xml";
        return outfile;
    }

    // Function to append a child element with text to a parent element
    void appendXmlElement(QDomDocument &doc, QDomElement &parent, const QString &tagName, const QString &text) {
        QDomElement element = doc.createElement(tagName);
        QDomText textNode = doc.createTextNode(text);
        element.appendChild(textNode);
        parent.appendChild(element);
    }

    // Function to append child elements to a parent element
    void appendChildren(QDomDocument &doc, QDomElement &parent, const QString &vendor, const QString &name, const QString &version, const QString &description, const QString& author)
    {
        appendXmlElement(doc, parent, "ipxact:vendor", vendor);
        appendXmlElement(doc, parent, "ipxact:library", "io");
        appendXmlElement(doc, parent, "ipxact:name", name);
        appendXmlElement(doc, parent, "ipxact:version", version);
        appendXmlElement(doc, parent, "ipxact:directConnection", "");
        appendXmlElement(doc, parent, "ipxact:broadcast", "");
        appendXmlElement(doc, parent, "ipxact:isAddressable", "");
        appendXmlElement(doc, parent, "ipxact:description", description);
        appendXmlElement(doc, parent, "ipxact:vendorExtensions", author);  //<---author외에도 더 저장해도 됨

        // Add more elements here as needed
    }

    void MakeIPDataXml(QTableWidget* table, const QString &vendor, const QString &name, const QString &version, const QString &description, QString& author)
      {
                  // 일반 정보 설정
          IPXACTGeneral general;
          general.vendor = "MyVendor";
          general.name = "MyName";
          general.version = "1.0";
          general.description = "Sample Description";
          general.author = "John Doe";


          //genfile 만들어야  --할일
          //generator.setGeneralInfo(general);

          // IPXACTPort 객체 생성 및 설정
          IPXACTPort port1;
          port1.logicalName = "Name 1";
          port1.description = "Port1 Description";
          port1.qualifier.isAddress = true;
          port1.qualifier.isData = true;


          port1.onMaster.presence = "Port1MasterPresence";
          port1.onMaster.width = 8;
          port1.onMaster.direction = "out";
          port1.onSlave.presence = "Port1SlavePresence";
          port1.onSlave.width = 8;
          port1.onSlave.direction = "in";
          port1.defaultValue = "0";

          port1.qualifier.isNull = false;
          port1.onMaster.isNull = true;
          port1.onSlave.isNull = true;
          port1.onSystem.isNull = true;

/*
          // 최종 XML 문서 생성 - 할일
          QDomDocument document = generator.generateFullXML();

          //------------------------------------------------------------
          // Convert the document to a string and print it
          QString xmlString = document.toString();

          QString savepath = GetSavePath(vendor, name, version);
          QString outfile = MakeIPDataMainXml(vendor, name, version);  //....  savepath + name+".absDef."+version + ".xml";

          if(CreateFolder(savepath, outfile))
              saveStringToFile(xmlString, outfile);

*/
      }


    void MakeGeneralXml(const QString &vendor, const QString &name, const QString &version, const QString &description, QString& author)
    {

        QDomDocument doc;
        QDomProcessingInstruction header = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
        doc.appendChild(header);

        QDomElement root = doc.createElement("ipxact:busDefinition");
        root.setAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
        root.setAttribute("xmlns:ipxact", "http://www.accellera.org/XMLSchema/IPXACT/1685-2014");
        root.setAttribute("xmlns:kactus2", "http://kactus2.cs.tut.fi");
        root.setAttribute("xsi:schemaLocation", "http://www.accellera.org/XMLSchema/IPXACT/1685-2014 http://www.accellera.org/XMLSchema/IPXACT/1685-2014/index.xsd");

        // Append child elements to root element
        appendChildren(doc, root, vendor, name, version, description, author); // root 엘리먼트 이하에 각 엘리먼트를 추가

        // Create and set the root element of the document
        doc.appendChild(root); //root를 도큐먼트에 최종 추가

        // Convert the document to a string and print it
        QString xmlString = doc.toString();

        QString savepath = GetSavePath(vendor, name, version);
        QString outfile = GetGeneralXMLPath(vendor, name, version); //savepath + name+"."+version + ".xml";

        if(CreateFolder(savepath, outfile))
            saveStringToFile(xmlString, outfile);
    }

    QString MakeIP_FileSetXml(QTableWidget* table /*, const QString &vendor, const QString &name, const QString &version, const QString &description, QString& author*/)
    {
        return "";
    }

    QString MakeIP_PortMapParamXml(QTableWidget* table /*, const QString &vendor, const QString &name, const QString &version, const QString &description, QString& author*/)
    {
        return "";
    }

    /* 데이터 읽어 오기*/

    bool LoadIP_GeneralInfo( QString &vendor, QString &name, QString &version, QString &description, QString& author)
    {
        return true;
    }

    bool LoadIP_FileSetData(QTableWidget* table1,  QTableWidget* table2)
    {
        return true;
    }

    bool LoadIP_PortMapParamsData(QTableWidget* table1,  QTableWidget* table2,  QTableWidget* table3)
    {
        return true;
    }

};




#endif // IPXMLGEN_H
