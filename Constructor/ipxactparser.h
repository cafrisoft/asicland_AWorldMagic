#ifndef IPXACTPARSER_H
#define IPXACTPARSER_H


#include <QDomDocument>
#include <QFile>
#include <QDebug>
#include "ipxactgenerator.h"


class IPXACTParser {
private:
    IPXACTGeneral generalInfo;

public:
    IPXACTParser() {}
    QList<IPXACTPort> ports;

    bool parse(const QString &filePath) {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            return false;
        }

        QDomDocument doc;
        if (!doc.setContent(&file)) {
            file.close();
            return false;
        }
        file.close();

        QDomElement root = doc.documentElement();
        if (root.tagName() != "ipxact:abstractionDefinition") {
            return false;
        }

        generalInfo.vendor = root.firstChildElement("ipxact:vendor").text();
        generalInfo.name = root.firstChildElement("ipxact:name").text();
        generalInfo.version = root.firstChildElement("ipxact:version").text();
        generalInfo.description = root.firstChildElement("ipxact:description").text();
        // generalInfo.author = root.firstChildElement("ipxact:author").text();   -- author사용안함.. 없음

        QDomNodeList portList = root.elementsByTagName("ipxact:port");
        int _count = portList.size();
        for (int i = 0; i < portList.size(); ++i) {



            QDomElement portElement = portList.at(i).toElement();

            IPXACTPort port;
            port.logicalName = portElement.firstChildElement("ipxact:logicalName").text();
            port.description = portElement.firstChildElement("ipxact:description").text();
            port.systemGroup = portElement.firstChildElement("ipxact:systemGroup").text();//<-

            QDomElement wireElement = portElement.firstChildElement("ipxact:wire");


            if (!wireElement.isNull()) {

                QDomElement qualifier = wireElement.firstChildElement("ipxact:qualifier");
                port.qualifier.qulifierType = "none";
                if (qualifier.isNull()) {
                    // "ipxact:qualifier" 요소를 찾을 수 없습니다.
                    qDebug() << "ipxact:qualifier element not found!";
                    port.qualifier.isNull = true;
                    port.qualifier.isAddress = false;
                    port.qualifier.isData = false;

                } else {
                    port.qualifier.isNull = false;

                    QString boolString = qualifier.firstChildElement("ipxact:isAddress").text();
                    port.qualifier.isAddress = (boolString == "true" || boolString == "1");
                    boolString = qualifier.firstChildElement("ipxact:isData").text();
                    port.qualifier.isData = (boolString == "true" || boolString == "1");
                    port.qualifier.qulifierType = qualifier.firstChildElement("ipxact:qulifierType").text();
                }


                //--------------------Driver -----------------------


                // "ipxact:requiresDriver" 태그를 찾습니다.
                QDomElement driverElement = wireElement.firstChildElement("ipxact:requiresDriver");

                port.driver.isNull = true;
                if (!driverElement.isNull()) {
                    // "driverType" 속성의 값을 가져옵니다.
                    port.driver.typeAtt = driverElement.attribute("driverType");
                    port.driver.isDriver = driverElement.text()=="true";
                    port.driver.isNull = false;
                }


                //--------------------slave, master-----------------------

                QDomElement masterElement = wireElement.firstChildElement("ipxact:onMaster");
                if (masterElement.isNull()) {
                    // "ipxact:onMaster" 요소를 찾을 수 없습니다.
                    qDebug() << "ipxact:onMaster element not found!";
                    port.onMaster.isNull = true;

                } else {
                    port.onMaster.isNull = false;
                    port.onMaster.presence = masterElement.firstChildElement("ipxact:presence").text();
                    port.onMaster.width = masterElement.firstChildElement("ipxact:width").text().toInt();
                    port.onMaster.direction = masterElement.firstChildElement("ipxact:direction").text();
                }


                QDomElement slaveElement = wireElement.firstChildElement("ipxact:onSlave");
                if (slaveElement.isNull()) {
                    qDebug() << "ipxact:onSlave element not found!";
                    port.onSlave.isNull = true;

                } else {
                    port.onSlave.isNull = false;
                    port.onSlave.presence = slaveElement.firstChildElement("ipxact:presence").text();
                    port.onSlave.width = slaveElement.firstChildElement("ipxact:width").text().toInt();
                    port.onSlave.direction = slaveElement.firstChildElement("ipxact:direction").text();
                }

                QDomElement systemElement = wireElement.firstChildElement("ipxact:onSystem");
                if (systemElement.isNull()) {
                    qDebug() << "ipxact:onSystem element not found!";
                    port.onSystem.isNull = true;

                } else {
                    port.onSystem.isNull = false;
                    port.onSystem.presence = slaveElement.firstChildElement("ipxact:presence").text();
                    port.onSystem.width = slaveElement.firstChildElement("ipxact:width").text().toInt();
                    port.onSystem.direction = slaveElement.firstChildElement("ipxact:direction").text();
                }


                QDomElement defaltElem = wireElement.firstChildElement("ipxact:defaultValue");//.text().toInt();
                if (defaltElem.isNull()) {
                    qDebug() << "ipxact:onMaster element not found!";
                    port.defaultValue = "";

                } else {
                    port.defaultValue = defaltElem.text();
                }

            }


            ports.append(port);
        }

        return true;
    }

    IPXACTGeneral getGeneralInfo() const {
        return generalInfo;
    }

    QList<IPXACTPort> getPorts() const {
        return ports;
    }
};





#endif // IPXACTPARSER_H
