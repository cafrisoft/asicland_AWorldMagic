#ifndef IOXMLGEN_H
#define IOXMLGEN_H

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

class IOXmlGen: public BaseXMLMgr
{
private:
        IPXACTGeneratorForIO generator;
public:
    IOXmlGen();

    QString GetSavePath(const QString &vendor, const QString &name, const QString &version)
    {
        QString savepath = libraryPath +"/" +vendor + "/io/"+ name + "/" + version + "/";
        return savepath;
    }

    QString GetWireXMLPath(const QString &vendor, const QString &name, const QString &version)
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

    void MakeGenXml(const QString &vendor, const QString &name, const QString &version, const QString &description, QString& author)
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

    void MakeWireXml2(QTableWidget* table, const QString &vendor, const QString &name, const QString &version, const QString &description, QString& author)
      {
                  // 일반 정보 설정
          IPXACTGeneral general;
          general.vendor = "MyVendor";
          general.name = "MyName";
          general.version = "1.0";
          general.description = "Sample Description";
          general.author = "John Doe";
          generator.setGeneralInfo(general);

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


                     // 최종 XML 문서 생성
          QDomDocument document = generator.generateFullXML();

          //------------------------------------------------------------
          // Convert the document to a string and print it
          QString xmlString = document.toString();

          QString savepath = GetSavePath(vendor, name, version);
          QString outfile = GetWireXMLPath(vendor, name, version);  //....  savepath + name+".absDef."+version + ".xml";

          if(CreateFolder(savepath, outfile))
              saveStringToFile(xmlString, outfile);

      }



    void MakeWireXml(QTableWidget* table, const QString &vendor, const QString &name, const QString &version, const QString &description, QString& author)
    {

        QComboBox *modeComboBox = new QComboBox();
        modeComboBox->addItems({"master", "slave", "system"});
        QComboBox *directionComboBox = new QComboBox();
        directionComboBox->addItems({"out", "in", "inout"});
        QComboBox *driverComboBox = new QComboBox();
        driverComboBox->addItems({"none","clock", "singleShot", "any"});
        QComboBox *qualifierComboBox = new QComboBox();
        qualifierComboBox->addItems({"none","address", "data", "address,data", "clock", "reset"});

        QComboBox *preperenceComboBox = new QComboBox();
        preperenceComboBox->addItems({"none","illegal", "required", "optional"});


        QString text = "";
//..QString text = modeComboBox->itemText(index);
        enum MyModeType { master, slave, system };

                 // 일반 정보 설정
        IPXACTGeneral general;
        general.vendor = vendor;
        general.name = name;
        general.version = version;
        general.description = description;
        general.author = author;
        generator.setGeneralInfo(general);

        // IPXACTPort 객체 생성 및 설정

        IPXACTPort port1;
        port1.defaultValue = "0";

        /////////////////
        int rowCount = table->rowCount();
        int colCount = table->columnCount();

        QComboBox* comboBox = nullptr;
        for (int row = 0; row < rowCount; ++row)
        {
            port1.onMaster.presence = "";
            port1.onMaster.width = 0;
            port1.onMaster.direction = "";
            port1.onSlave.presence = "";
            port1.onSlave.width = 0;
            port1.onSlave.direction = "";

            MyModeType m_type = system;

            port1.description = "";
            port1.qualifier.isAddress = false;
            port1.qualifier.isData = false;

                         //초기화 - 아직 레코드가 없다고 간주한다.
            port1.qualifier.isNull = false;
            port1.onMaster.isNull = true;
            port1.onSlave.isNull = true;
            port1.onSystem.isNull = true;


            for (int col = 0; col < colCount; ++col) {
                comboBox = nullptr;
                QTableWidgetItem *item = table->item(row, col);
                if(item == nullptr)
                {
                    QWidget* widget = table->cellWidget(row, col);
                    comboBox = qobject_cast<QComboBox*>(widget);
                }

                // QTableWidget에서 셀에 아이템이 할당되지 않았을 경우 nullptr을 반환할 수 있으므로 체크 필요
                if (item || comboBox) {
                    switch (col) {
                    case 0:
                        port1.logicalName = item->text();
                        if(port1.logicalName.isEmpty())
                            continue;

                        port1.logicalName = port1.logicalName.trimmed();
                        break;

                    case 1: //mode
                        if(comboBox->currentText().contains("master"))
                        {
                            port1.onMaster.isNull = false;
                            m_type = master;
                        }
                        else if(comboBox->currentText().contains("slave"))
                        {
                            port1.onSlave.isNull = false;
                            m_type = slave;
                        }
                        else if(comboBox->currentText().contains("system"))
                        {
                            port1.onSystem.isNull = false;
                            m_type = system;
                        }
                        break;

                    case 2: //presen
                        if(comboBox)
                        {
                            switch (m_type) {
                            case master:
                                port1.onMaster.presence = comboBox->currentText();
                                break;
                            case slave:
                                port1.onSlave.presence = comboBox->currentText();
                                break;
                            case system:
                                port1.onSystem.presence = comboBox->currentText();
                                break;
                            default:
                                break;
                            }
                        }
                        break;

                    case 3:
                        //comboBox = qobject_cast<QComboBox*>(table->cellWidget(row, col));
                        if(comboBox)
                        {
                            switch (m_type) {
                            case master:
                                port1.onMaster.direction = comboBox->currentText();
                                break;
                            case slave:
                                port1.onSlave.direction = comboBox->currentText();
                                break;
                            case system:
                                port1.onSystem.direction = comboBox->currentText();
                                break;
                            default:
                                break;
                            }
                        }
                        break;

                    case 4:
                        if(!item->text().isEmpty())
                        {
                            bool ok;
                            int number =  item->text().toInt(&ok);

                            if(ok) {
                            switch (m_type) {
                            case master:
                                port1.onMaster.width = number;
                                break;
                            case slave:
                                port1.onSlave.width = number;
                                break;
                            case system:
                                port1.onSystem.width = number;
                                break;
                            default:
                                break;
                            }
                          }
                        }
                        break;

                    case 5: //defult value
                            port1.defaultValue = item->text();
                        break;

                    case 6: //<ipxact:requiresDriver driverType="singleShot">true</ipxact:requiresDriver>
                        port1.driver.isNull = false;
                        port1.driver.isDriver = true;
                        //comboBox = qobject_cast<QComboBox*>(table->cellWidget(row, col));
                        if(comboBox){
                            port1.driver.typeAtt = comboBox->currentText();
                            if(!port1.driver.typeAtt.contains("none"))
                                port1.driver.isDriver = true;
                        }
                        break;

                    case 7:
                        port1.qualifier.isAddress = false;
                        port1.qualifier.isData = false;
                        port1.qualifier.isNull = false;

                        //comboBox = qobject_cast<QComboBox*>(table->cellWidget(row, col));
                        if(comboBox){

                            QString kind = comboBox->currentText();
                            port1.qualifier.isAddress = false;
                            port1.qualifier.qulifierType = kind;

                            if(kind.contains("none"))
                                port1.qualifier.isNull = true;
                            else if(kind.contains("address"))
                                port1.qualifier.isAddress = true;
                            else if(kind.contains("data"))
                                port1.qualifier.isData = true;

                            //  "address,data", "clock", "reset"  - 일 경우는 false
                        }
                        break;

                    case 8:
                            port1.systemGroup = item->text();
                        break;
                    case 9:
                            port1.description = item->text();
                        break;
                    }

                if(col == colCount-1)
                    generator.addPort(port1);

                } //if
            }
        } // for


                // 최종 XML 문서 생성 //........중요
        QDomDocument document = generator.generateFullXML();

        //------------------------------------------------------------
        // Convert the document to a string and print it
        QString xmlString = document.toString();

        QString savepath = GetSavePath(vendor, name, version);
        QString outfile = GetWireXMLPath(vendor, name, version);  //....  savepath + name+".absDef."+version + ".xml";

        if(CreateFolder(savepath, outfile))
            saveStringToFile(xmlString, outfile);

    }


    void LoadGenXmlInfo(QString xmlFile, QString &vendor, QString &name, QString &version, QString &description, QString &author);
};



#endif // IOXMLGEN_H
