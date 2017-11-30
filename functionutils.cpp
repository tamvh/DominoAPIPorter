#include "functionutils.h"
#include <QDebug>

FunctionUtils::FunctionUtils(QObject *parent) : QObject(parent)
{

}

json FunctionUtils::convertXmlToJson(const QString &xmlText, const QString &fromNode)
{
    QDomDocument doc;
    doc.setContent(xmlText);
    QDomNodeList nodeList = doc.elementsByTagName(fromNode);
    if (nodeList.size() > 0) {
        return convertXmlToJson(nodeList.at(0));
    }
    return {};
}

json FunctionUtils::convertXmlToJson(const QDomNode &xmlNode)
{
    json resultJson;
    convertNode(xmlNode, resultJson[xmlNode.nodeName().toStdString()]);
    return resultJson;
}

bool FunctionUtils::convertNode(const QDomNode &xmlNode, json &resultJson)
{
    bool hasAttr = convertAttributeXml(xmlNode, resultJson);
    if (checkChildTextNodeXml(xmlNode)) {
        if (hasAttr) {
            resultJson["#text"] = xmlNode.childNodes().at(0).nodeValue().toStdString();
        } else {
            resultJson[xmlNode.nodeName().toStdString()] = xmlNode.childNodes().at(0).nodeValue().toStdString();
        }

        return true;
    }

    QHash<QString, int> multiNodeHash;
    QHash<QString, int> singleNodeHash;
    analyticNodeList(xmlNode, multiNodeHash, singleNodeHash);

    QDomNodeList childs = xmlNode.childNodes();
    for (int i = 0 ; i < childs.size(); i++) {
        if (multiNodeHash.contains(childs.at(i).nodeName()) == true) {
            if (resultJson[childs.at(i).nodeName().toStdString()].is_null()) {
                resultJson[childs.at(i).nodeName().toStdString()] = json::array();
            }
            if (checkChildTextNodeXml(childs.at(i))) {
                resultJson[childs.at(i).nodeName().toStdString()].push_back(childs.at(i).childNodes().at(0).nodeValue().toStdString());
            } else {
                json jsonEle;
                convertNode(childs.at(i), jsonEle);
                resultJson[childs.at(i).nodeName().toStdString()].push_back(jsonEle);
            }
        } else {
            if (checkChildTextNodeXml(childs.at(i))) {
                convertNode(childs.at(i), resultJson);
            } else {
                convertNode(childs.at(i), resultJson[childs.at(i).nodeName().toStdString()]);
            }
        }
    }

    return true;
}

bool FunctionUtils::convertAttributeXml(const QDomNode &xmlNode, json &resultJson)
{
    const QDomNamedNodeMap attributes = xmlNode.attributes();
    bool hasAtt = false;
    for (int i = 0; i < attributes.size(); i++) {
        resultJson["@" + attributes.item(i).nodeName().toStdString()] = attributes.item(i).nodeValue().toStdString();
        hasAtt = true;
    }

    return hasAtt;
}

bool FunctionUtils::checkChildTextNodeXml(const QDomNode &xmlNode)
{
    if (xmlNode.childNodes().size() == 1) {
        return xmlNode.childNodes().at(0).nodeType() == QDomNode::NodeType::TextNode;
    }
    return false;
}

void FunctionUtils::analyticNodeList(const QDomNode &xmlNode, QHash<QString, int> &multiNodeHash, QHash<QString, int> &singleNodeHash)
{
    const QDomNodeList nodeList = xmlNode.childNodes();
    bool single = false;
    for (int i = 0; i < nodeList.size(); i++) {
        single = true;
        if (multiNodeHash.contains(nodeList.at(i).nodeName()) == false &&
                singleNodeHash.contains(nodeList.at(i).nodeName()) == false) {
            singleNodeHash[nodeList.at(i).nodeName()] = 1;
        } else if (singleNodeHash.contains(nodeList.at(i).nodeName()) == false){
            multiNodeHash[nodeList.at(i).nodeName()] += 1;
        } else {
            singleNodeHash.remove(nodeList.at(i).nodeName());
            multiNodeHash[nodeList.at(i).nodeName()] = 2;
        }
    }
}

QDomDocument FunctionUtils::convertJsonToXml(const json &infoJson)
{
    QDomDocument doc;

    if (infoJson.is_object() && infoJson.size() == 1) {
        QDomElement ele = doc.createElement(QString::fromStdString(infoJson.begin().key()));
        convertJson(infoJson.begin().value(), doc, ele);
        doc.appendChild(ele);
    }

    return doc;
}

bool FunctionUtils::convertJson(const json &infoJson, QDomDocument &doc, QDomNode &xmlNode)
{
    if (infoJson.is_primitive()) {
        xmlNode.appendChild(doc.createTextNode(getValue(infoJson)));
    } else if (infoJson.is_object()) {
        for (json::const_iterator it = infoJson.begin(); it != infoJson.end(); ++it) {
            if (it->is_object()) {
                QDomElement childNode = doc.createElement(QString::fromStdString(it.key()));
                convertJson(it.value(), doc, childNode);
                xmlNode.appendChild(childNode);
            } else if (it->is_array()) {
                for (json::const_iterator arrIt = it.value().begin(); arrIt != it.value().end(); ++arrIt) {
                    QDomElement childNode = doc.createElement(QString::fromStdString(it.key()));
                    convertJson(arrIt.value(), doc, childNode);
                    xmlNode.appendChild(childNode);
                }
            } else if (it->is_primitive() && it->is_null() == false) {
                if (checkIsAttribute(it.key())) {

                    xmlNode.toElement().setAttribute(QString::fromStdString(it.key().substr(1, it.key().length())),
                                                     QString::fromStdString(it.value()));
                } else if (checkIsTextNode(it.key())) {
                    xmlNode.appendChild(doc.createTextNode(getValue(it.value())));
                } else {
                    QDomElement childNode = doc.createElement(QString::fromStdString(it.key()));
                    childNode.appendChild(doc.createTextNode(getValue(it.value())));
                    xmlNode.appendChild(childNode);
                }
            } else if (it->is_null()){
                xmlNode.appendChild(doc.createElement(QString::fromStdString(it.key())));
            }
        }
    } else if (infoJson.is_array()) {
        //not support
    }

    return true;
}

bool FunctionUtils::checkIsAttribute(const std::string &name)
{
    return name.find('@') == 0;
}

bool FunctionUtils::checkIsTextNode(const std::string &name)
{
    return name.find("#text") == 0;
}

QString FunctionUtils::getValue(const json &infoJson)
{
    QString data;
    switch (infoJson.type()) {
        case json::value_t::null:
        break;
        case json::value_t::boolean:
        data = QString::number(infoJson.get<bool>());
        break;
    case json::value_t::number_integer:
        data = QString::number(infoJson.get<int>());
    break;
    case json::value_t::number_unsigned:
        data = QString::number(infoJson.get<unsigned int>());
    break;
    case json::value_t::number_float:
        data = QString::number(infoJson.get<float>());
    break;
    case json::value_t::string:
        data = QString::fromStdString(infoJson.get<std::string>());
    break;
    }
    return data;
}
