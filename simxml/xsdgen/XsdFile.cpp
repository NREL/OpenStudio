/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/
#include <XsdFile.hpp>

#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QUuid>
#include <QIODevice>
#include <QXmlQuery>
#include <QXmlResultItems>
#include <QFileInfo>
#include <QDir>

#include <QDomNodeModel>

#include <iostream>


XsdFile::XsdFile(const std::string& path)
 : m_path(path), m_doc(new QDomDocument())
{
  QFile file(QString::fromStdString(path));
  if (file.open(QIODevice::ReadOnly)) {
    if (m_doc->setContent(&file, true)) {
      parse();
    }
    file.close();
  }  
}

XsdFile::~XsdFile()
{
}

std::string XsdFile::path() const
{
  return m_path;
}

std::string XsdFile::defaultNamespace() const
{
  return m_defaultNamespace;
}

std::string XsdFile::defaultPrefix() const
{
  return m_defaultPrefix;
}

QSharedPointer<Element> XsdFile::findElement(const std::string& qualifiedName) const
{
  QSharedPointer<Element> result;
  std::map<std::string, Element>::const_iterator it = m_elementMap.find(qualifiedName);
  if (it != m_elementMap.end()){
    result = QSharedPointer<Element>(new Element(it->second));
    return result;
  }

  // check imports
  for(std::vector<XsdFile>::const_iterator jt = m_imports.begin(), jtend = m_imports.end(); jt != jtend; ++jt){
    result = jt->findElement(qualifiedName);
    if (result){
      return result;
    }
  }

  return result;
}

QSharedPointer<SimpleType> XsdFile::findSimpleType(const std::string& qualifiedName) const
{
  QSharedPointer<SimpleType> result;
  std::map<std::string, SimpleType>::const_iterator it = m_simpleTypeMap.find(qualifiedName);
  if (it != m_simpleTypeMap.end()){
    result = QSharedPointer<SimpleType>(new SimpleType(it->second));
    return result;
  }

  // check imports
  for(std::vector<XsdFile>::const_iterator jt = m_imports.begin(), jtend = m_imports.end(); jt != jtend; ++jt){
    result = jt->findSimpleType(qualifiedName);
    if (result){
      return result;
    }
  }

  return result;
}

QSharedPointer<ComplexType> XsdFile::findComplexType(const std::string& qualifiedName) const
{
  QSharedPointer<ComplexType> result;
  std::map<std::string, ComplexType>::const_iterator it = m_complexTypeMap.find(qualifiedName);
  if (it != m_complexTypeMap.end()){
    result = QSharedPointer<ComplexType>(new ComplexType(it->second));
    return result;
  }

  // check imports
  for(std::vector<XsdFile>::const_iterator jt = m_imports.begin(), jtend = m_imports.end(); jt != jtend; ++jt){
    result = jt->findComplexType(qualifiedName);
    if (result){
      return result;
    }
  }

  return result;
}

std::vector<std::string> XsdFile::importNames() const
{
  return m_importNames;
}

std::vector<Element> XsdFile::elements() const
{
  std::vector<Element> result;
  for (std::map<std::string, Element>::const_iterator it = m_elementMap.begin(), itend = m_elementMap.end(); it != itend; ++it){
    result.push_back(it->second);
  }
  return result;
}

std::vector<SimpleType> XsdFile::simpleTypes() const
{
  std::vector<SimpleType> result;
  for (std::map<std::string, SimpleType>::const_iterator it = m_simpleTypeMap.begin(), itend = m_simpleTypeMap.end(); it != itend; ++it){
    result.push_back(it->second);
  }
  return result;
}

std::vector<ComplexType> XsdFile::complexTypes() const
{
  std::vector<ComplexType> result;
  for (std::map<std::string, ComplexType>::const_iterator it = m_complexTypeMap.begin(), itend = m_complexTypeMap.end(); it != itend; ++it){
    result.push_back(it->second);
  }
  return result;
}

void XsdFile::parse()
{
  QXmlQuery query;
  QDomNodeModel model(query.namePool(), *m_doc);

  // get namespaces

  /* Apparently xmlns attributes are not really attributes so this does not work

  std::map<std::string, std::string> namespaceToPrefixMap;
  QDomNamedNodeMap attributes = m_doc->documentElement().attributes();
  for(int i = 0; i < attributes.count(); ++i){
    QDomAttr attribute = attributes.item(i).toAttr();
    if (attribute.name().startsWith("xmlns:")){
      QStringList nameParts = attribute.name().split(":");
      std::string prefix = nameParts.at(1).toStdString();
      std::string _namespace = attribute.value().toStdString();

      namespaceToPrefixMap[_namespace] = prefix;
    }
  }
  */

  m_defaultNamespace = m_doc->documentElement().attribute("targetAttribute").toStdString();
  m_defaultPrefix = QFileInfo(QString::fromStdString(m_path)).baseName().toStdString();

  QString queryString = "/xs:schema/xs:import";
  query.setFocus(QXmlItem(model.fromDomNode(m_doc->documentElement())));
  query.setQuery(queryString, QUrl(QString::fromStdString(m_path)));
  if (query.isValid()) {

    QXmlResultItems items;
    query.evaluateTo(&items);
    QXmlItem item(items.next());
    while (!item.isNull()) {
      QDomElement e = model.toDomNode(item.toNodeModelIndex()).toElement();

      std::string importName = e.attribute("schemaLocation").toStdString();
      
      m_importNames.push_back(importName);

      std::string importPath = QFileInfo(QString::fromStdString(m_path)).dir().filePath(QString::fromStdString(importName)).toStdString();

      m_imports.push_back(XsdFile(importPath));

      // get next item
      item = items.next();
    }
  }

  queryString = "/xs:schema/xs:element";
  query.setFocus(QXmlItem(model.fromDomNode(m_doc->documentElement())));
  query.setQuery(queryString, QUrl(QString::fromStdString(m_path)));
  if (query.isValid()) {

    QXmlResultItems items;
    query.evaluateTo(&items);
    QXmlItem item(items.next());
    while (!item.isNull()) {
      QDomElement e = model.toDomNode(item.toNodeModelIndex()).toElement();

      Element element(e, m_defaultNamespace, m_defaultPrefix, true);

      m_elementMap.insert(std::pair<std::string, Element>(element.qualifiedName(), element));

      // get next item
      item = items.next();
    }
  }

  queryString = "/xs:schema/xs:simpleType";
  query.setFocus(QXmlItem(model.fromDomNode(m_doc->documentElement())));
  query.setQuery(queryString, QUrl(QString::fromStdString(m_path)));
  if (query.isValid()) {

    QXmlResultItems items;
    query.evaluateTo(&items);
    QXmlItem item(items.next());
    while (!item.isNull()) {
      QDomElement e = model.toDomNode(item.toNodeModelIndex()).toElement();

      SimpleType simpleType(e, m_defaultNamespace, m_defaultPrefix);

      m_simpleTypeMap.insert(std::pair<std::string, SimpleType>(simpleType.qualifiedName(), simpleType));

      // get next item
      item = items.next();
    }
  }

  queryString = "/xs:schema/xs:complexType";
  query.setFocus(QXmlItem(model.fromDomNode(m_doc->documentElement())));
  query.setQuery(queryString, QUrl(QString::fromStdString(m_path)));
  if (query.isValid()) {

    QXmlResultItems items;
    query.evaluateTo(&items);
    QXmlItem item(items.next());
    while (!item.isNull()) {
      QDomElement e = model.toDomNode(item.toNodeModelIndex()).toElement();

      ComplexType complexType(e, m_defaultNamespace, m_defaultPrefix);
      
      m_complexTypeMap.insert(std::pair<std::string, ComplexType>(complexType.qualifiedName(), complexType));

      // get next item
      item = items.next();
    }
  }

  for (std::map<std::string, ComplexType>::iterator it = m_complexTypeMap.begin(), itend = m_complexTypeMap.end(); it!=itend; ++it){
    it->second.makeBaseList(*this);
  }
 
}