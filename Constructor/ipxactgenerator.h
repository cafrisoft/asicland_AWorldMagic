#ifndef IPXACTGENERATOR_H
#define IPXACTGENERATOR_H

#include <QList>
#include <QString>
#include <QDomDocument>

struct IPXACTGeneral {
    QString vendor;
    QString name;
    QString version;
    QString description;
    QString author; // If you need it for vendorExtensions
};

struct IPXACTQualifier {
    bool isNull;
    bool isAddress;
    bool isData;
    QString qulifierType;
};

struct IPXACTOn {
    bool isNull;
    QString presence;
    int width;
    QString direction;
};

struct IPXACTSystem {
    bool isNull;
    QString presence;
    int width;
    QString direction;
    QString group;
};

struct IPXACTDriver {
    bool isNull;
    QString typeAtt;
    bool isDriver;
};

struct IPXACTPort {
    QString logicalName;
    QString description;
    QString systemGroup;

    //--wire 시작
    IPXACTQualifier qualifier;
    IPXACTOn onMaster;
    IPXACTOn onSlave;
    IPXACTSystem onSystem; //todo
    IPXACTDriver driver;
    //--wire 끝
    QString defaultValue;
};

class IPXACTGeneratorForIO
{
public:
    IPXACTGeneratorForIO(){};

private:
    QList<IPXACTPort> ports;
    IPXACTGeneral generalInfo;

public:
    void addPort(const IPXACTPort& port) {
        bool found = false;

        // Check if the port with the same logicalName already exists in the list
        for (IPXACTPort &existingPort : ports) {
            if (existingPort.logicalName == port.logicalName) {
                found = true;

                // ===========  Update the existing port values =================

                if(existingPort.onMaster.isNull && !port.onMaster.isNull){
                    existingPort.onMaster.presence  = port.onMaster.presence;
                    existingPort.onMaster.width     = port.onMaster.width;
                    existingPort.onMaster.direction = port.onMaster.direction;
                    existingPort.onMaster.isNull = false;
                }

                if(existingPort.onSlave.isNull && !port.onSlave.isNull){
                    existingPort.onSlave.presence  = port.onSlave.presence;
                    existingPort.onSlave.width     = port.onSlave.width;
                    existingPort.onSlave.direction = port.onSlave.direction;
                    existingPort.onSlave.isNull = false;
                }

                if(existingPort.onSystem.isNull && !port.onSystem.isNull){
                    existingPort.onSystem.presence  = port.onSystem.presence;
                    existingPort.onSystem.width     = port.onSystem.width;
                    existingPort.onSystem.direction = port.onSystem.direction;
                    existingPort.onSystem.isNull = false;
                }

                if(existingPort.qualifier.isNull && !port.qualifier.isNull){
                    existingPort.qualifier.isData       = port.qualifier.isData;
                    existingPort.qualifier.isAddress    = port.qualifier.isAddress;
                    existingPort.qualifier.isNull = false;
                }

                break;
            }
        }

        // If the port with the same logicalName was not found in the list, append it
        if (!found) {
            ports.append(port);
        }
    }

    void initPorts() {
        ports.clear();
    }

public:
    // ... (이전에 정의한 함수들)

    void setGeneralInfo(const IPXACTGeneral& general) {
        generalInfo = general;
    }

    QDomDocument generateFullXML() {
        QDomDocument document;

        QDomProcessingInstruction header = document.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
        document.appendChild(header);

        QDomElement root = document.createElement("ipxact:abstractionDefinition");
        root.setAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
        root.setAttribute("xmlns:ipxact", "http://www.accellera.org/XMLSchema/IPXACT/1685-2014");
        root.setAttribute("xmlns:kactus2", "http://kactus2.cs.tut.fi");
        root.setAttribute("xsi:schemaLocation", "http://www.accellera.org/XMLSchema/IPXACT/1685-2014 http://www.accellera.org/XMLSchema/IPXACT/1685-2014/index.xsd");
        document.appendChild(root);

        QDomElement vendorElem = document.createElement("ipxact:vendor");
        vendorElem.appendChild(document.createTextNode(generalInfo.vendor));
        root.appendChild(vendorElem);

        QDomElement libraryElem = document.createElement("ipxact:library");
        libraryElem.appendChild(document.createTextNode("io"));
        root.appendChild(libraryElem);

        QDomElement nameElem = document.createElement("ipxact:name");
        nameElem.appendChild(document.createTextNode(generalInfo.name));
        root.appendChild(nameElem);

        QDomElement versionElem = document.createElement("ipxact:version");
        versionElem.appendChild(document.createTextNode(generalInfo.version));
        root.appendChild(versionElem);

        QDomElement busTypeElem = document.createElement("ipxact:busType");
        busTypeElem.setAttribute("vendor", generalInfo.vendor);
        busTypeElem.setAttribute("library", "io");
        busTypeElem.setAttribute("name", generalInfo.name);
        busTypeElem.setAttribute("version", generalInfo.version);
        root.appendChild(busTypeElem);

        QDomElement portsElem = document.createElement("ipxact:ports");

        // each port from the ports QList to portsElem
        for (const IPXACTPort& port : ports) {
            QDomElement portElement = document.createElement("ipxact:port");

            QDomElement logicalNameElement = document.createElement("ipxact:logicalName");
            logicalNameElement.appendChild(document.createTextNode(port.logicalName));
            portElement.appendChild(logicalNameElement);

            QDomElement descriptionElement = document.createElement("ipxact:description");
            descriptionElement.appendChild(document.createTextNode(port.description));
            portElement.appendChild(descriptionElement);

            QDomElement systemGroupElement = document.createElement("ipxact:systemGroup");
            systemGroupElement.appendChild(document.createTextNode(port.systemGroup));
            portElement.appendChild(systemGroupElement);


            QDomElement wireElement = document.createElement("ipxact:wire");

            // 1. qualifier

            QDomElement qualifierElement = document.createElement("ipxact:qualifier");

            QDomElement isAddressElement = document.createElement("ipxact:isAddress");
            isAddressElement.appendChild(document.createTextNode(port.qualifier.isAddress ? "true" : "false"));
            QDomElement isDataElement = document.createElement("ipxact:isData");
            isDataElement.appendChild(document.createTextNode(port.qualifier.isData ? "true" : "false"));
            QDomElement qulifierTypeElement = document.createElement("ipxact:qulifierType"); //<--
            qulifierTypeElement.appendChild(document.createTextNode(port.qualifier.qulifierType));

            qualifierElement.appendChild(isAddressElement);
            qualifierElement.appendChild(isDataElement);
            qualifierElement.appendChild(qulifierTypeElement);

            wireElement.appendChild(qualifierElement);

            //2.onMaster

            if (port.onMaster.isNull==false) {
                QDomElement Master_Element = document.createElement("ipxact:onMaster");

                QDomElement Master_presense = document.createElement("ipxact:presence");
                Master_presense.appendChild(document.createTextNode(port.onMaster.presence));
                QDomElement Master_width = document.createElement("ipxact:width");
                Master_width.appendChild(document.createTextNode(QString::number(port.onMaster.width)));
                QDomElement Master_direction = document.createElement("ipxact:direction");
                Master_direction.appendChild(document.createTextNode(port.onMaster.direction));

                Master_Element.appendChild(Master_presense);
                Master_Element.appendChild(Master_width);
                Master_Element.appendChild(Master_direction);
                wireElement.appendChild(Master_Element);
            }

            //3.onSlave

            if (port.onSlave.isNull==false) {
                QDomElement Slave_Element = document.createElement("ipxact:onSlave");
                QDomElement Slave_presense = document.createElement("ipxact:presence");
                Slave_presense.appendChild(document.createTextNode(port.onSlave.presence));
                QDomElement Slave_width = document.createElement("ipxact:width");
                Slave_width.appendChild(document.createTextNode(QString::number(port.onSlave.width)));
                QDomElement Save_direction = document.createElement("ipxact:direction");
                Save_direction.appendChild(document.createTextNode(port.onSlave.direction));

                Slave_Element.appendChild(Slave_presense);
                Slave_Element.appendChild(Slave_width);
                Slave_Element.appendChild(Save_direction);
                wireElement.appendChild(Slave_Element);
            }

            if (port.onSystem.isNull==false) {
                QDomElement System_Element = document.createElement("ipxact:onSystem");
                QDomElement System_resense = document.createElement("ipxact:presence");
                System_resense.appendChild(document.createTextNode(port.onSystem.presence));
                QDomElement System_width = document.createElement("ipxact:width");
                System_width.appendChild(document.createTextNode(QString::number(port.onSystem.width)));
                QDomElement System_direction = document.createElement("ipxact:direction");
                System_direction.appendChild(document.createTextNode(port.onSystem.direction));

                System_Element.appendChild(System_resense);
                System_Element.appendChild(System_width);
                System_Element.appendChild(System_direction);
                wireElement.appendChild(System_Element);
            }

            //<ipxact:requiresDriver driverType="clock">true</ipxact:requiresDriver>
            if(!port.driver.isNull)
            {
                QDomElement Driver_Element = document.createElement("ipxact:requiresDriver");
                Driver_Element.setAttribute("driverType", port.driver.typeAtt);  // Adding the attribute here

                if(port.driver.isDriver)
                {
                    Driver_Element.appendChild(document.createTextNode("true"));
                }
                else
                {
                    Driver_Element.appendChild(document.createTextNode("false"));
                }

                 wireElement.appendChild(Driver_Element);
            }


//            portsElem.appendChild(portElement);

                          // 뒷부분
            QDomElement defaultValueElement = document.createElement("ipxact:defaultValue");
            defaultValueElement.appendChild(document.createTextNode(port.defaultValue));
            wireElement.appendChild(defaultValueElement);

            portElement.appendChild(wireElement);

            portsElem.appendChild(portElement);
        }

        root.appendChild(portsElem);


        QDomElement descriptionElem = document.createElement("ipxact:description");
        descriptionElem.appendChild(document.createTextNode(generalInfo.description));
        root.appendChild(descriptionElem);

        QDomElement vendorExtensionsElem = document.createElement("ipxact:vendorExtensions");
        // Example: adding the author. Adjust as needed.
        QDomElement authorElem = document.createElement("Author");
        authorElem.appendChild(document.createTextNode(generalInfo.author));
        vendorExtensionsElem.appendChild(authorElem);
        root.appendChild(vendorExtensionsElem);

        return document;
    }
};

#endif // IPXACTGENERATOR_H
