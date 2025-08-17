#include "xmlhandler.h"

XmlHandler::XmlHandler()
{

}

bool XmlHandler::newXml() {
    QString initialContent = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<root></root>";
    return doc.setContent(initialContent);
}

bool XmlHandler::loadXml(const QString &path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    if (!doc.setContent(&file)) {
        file.close();
        return false;
    }

    file.close();
    return true;
}

bool XmlHandler::saveXml(const QString &path) {
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    QTextStream ts(&file);
    ts << doc.toString();

    file.close();
    return true;
}

void XmlHandler::modifyElement(const QString &tagName, const QString &newValue) {
    QDomElement root = doc.documentElement();
    QDomNodeList list = root.elementsByTagName(tagName);
    for (int i = 0; i < list.count(); i++) {
        QDomElement elem = list.at(i).toElement();
        elem.firstChild().setNodeValue(newValue);
    }
}

//handler.modifyGrandchildElement("parent", "child", "newValue");
void XmlHandler::modifyGrandchildElement(const QString &parentTag, const QString &childTag, const QString &newValue) {
    QDomElement root = doc.documentElement();
    QDomNodeList parentList = root.elementsByTagName(parentTag);

    for (int i = 0; i < parentList.count(); i++) {
        QDomElement parentElem = parentList.at(i).toElement();
        QDomNodeList childList = parentElem.elementsByTagName(childTag);

        for (int j = 0; j < childList.count(); j++) {
            QDomElement childElem = childList.at(j).toElement();
            childElem.firstChild().setNodeValue(newValue);
        }
    }
}


// "parent" 태그에 새로운 "child" 노드를 추가합니다.
//handler.appendChildNode("parent", "child", "nodeValue");

void XmlHandler::appendChildNode(const QString &parentTag, const QString &newChildTag, const QString &nodeValue) {
    QDomElement root = doc.documentElement();
    QDomNodeList list = root.elementsByTagName(parentTag);

    for (int i = 0; i < list.count(); i++) {
        QDomElement parentElem = list.at(i).toElement();

        QDomElement newChild = doc.createElement(newChildTag);
        QDomText textNode = doc.createTextNode(nodeValue);
        newChild.appendChild(textNode);

        parentElem.appendChild(newChild);
    }
}

// "parent" 태그 아래의 "child" 태그에 새로운 "grandChild" 노드를 추가합니다.
//handler.appendGrandchildNode("parent", "child", "grandChild", "grandChildValue");

void XmlHandler::appendGrandchildNode(const QString &grandParentTag, const QString &parentTag, const QString &newChildTag, const QString &nodeValue) {
    QDomElement root = doc.documentElement();
    QDomNodeList grandParentList = root.elementsByTagName(grandParentTag);

    for (int i = 0; i < grandParentList.count(); i++) {
        QDomElement grandParentElem = grandParentList.at(i).toElement();
        QDomNodeList parentList = grandParentElem.elementsByTagName(parentTag);

        for (int j = 0; j < parentList.count(); j++) {
            QDomElement parentElem = parentList.at(j).toElement();

            QDomElement newChild = doc.createElement(newChildTag);
            QDomText textNode = doc.createTextNode(nodeValue);
            newChild.appendChild(textNode);

            parentElem.appendChild(newChild);
        }
    }
}

void XmlHandler::appendChildAttribute(const QString &childTag, const QString &attributeName, const QString &attributeValue) {
    QDomElement root = doc.documentElement();
    QDomNodeList list = root.elementsByTagName(childTag);

    for (int i = 0; i < list.count(); i++) {
        QDomElement childElem = list.at(i).toElement();
        childElem.setAttribute(attributeName, attributeValue);
    }
}

/*
 *  appendChildAttribute: 특정 child 태그에 속성을 추가합니다.
    appendGrandchildAttribute: 특정 grand child 태그에 속성을 추가합니다.
    modifyChildAttribute: 특정 child 태그의 속성을 수정합니다.
    modifyGrandchildAttribute: 특정 grand child 태그의 속성을 수정합니다.


    // "child" 태그에 새로운 속성을 추가하거나 수정합니다.
    handler.appendChildAttribute("child", "attributeName", "attributeValue");
    handler.modifyChildAttribute("child", "attributeName", "newAttributeValue");

    // "parent" 태그 아래의 "grandChild" 태그에 새로운 속성을 추가하거나 수정합니다.
    handler.appendGrandchildAttribute("parent", "grandChild", "attributeName", "attributeValue");
    handler.modifyGrandchildAttribute("parent", "grandChild", "attributeName", "newAttributeValue");

    */


void XmlHandler::appendGrandchildAttribute(const QString &parentTag, const QString &grandChildTag, const QString &attributeName, const QString &attributeValue) {
    QDomElement root = doc.documentElement();
    QDomNodeList parentList = root.elementsByTagName(parentTag);

    for (int i = 0; i < parentList.count(); i++) {
        QDomElement parentElem = parentList.at(i).toElement();
        QDomNodeList grandChildList = parentElem.elementsByTagName(grandChildTag);

        for (int j = 0; j < grandChildList.count(); j++) {
            QDomElement grandChildElem = grandChildList.at(j).toElement();
            grandChildElem.setAttribute(attributeName, attributeValue);
        }
    }
}

void XmlHandler::modifyChildAttribute(const QString &childTag, const QString &attributeName, const QString &newAttributeValue) {
    appendChildAttribute(childTag, attributeName, newAttributeValue);  // 기존의 함수를 재사용
}

void XmlHandler::modifyGrandchildAttribute(const QString &parentTag, const QString &grandChildTag, const QString &attributeName, const QString &newAttributeValue) {
    appendGrandchildAttribute(parentTag, grandChildTag, attributeName, newAttributeValue);  // 기존의 함수를 재사용
}

bool XmlHandler::getElement(const QString &tagName, QDomNode& node, QString &childName, QString &childContent) {
    QDomElement root = doc.documentElement();
    QDomNodeList list = root.elementsByTagName(tagName);
    for (int i = 0; i < list.count(); i++) {
        QDomElement elem = list.at(i).toElement();
        node = elem.firstChild();
        if(!node.isNull()) { // 첫 번째 자식이 있는지 확인합니다.
            childName = node.nodeName(); // 자식 요소의 이름
            childContent = node.toElement().text(); // 자식 요소의 내용
            return true;
        }
    }
    return false;
}
