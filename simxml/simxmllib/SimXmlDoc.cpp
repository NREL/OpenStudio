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
#include <SimXmlDoc.hpp>
#include <SimXmlElement.hpp>

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
#include <QUuid>

#include <QDomNodeModel>

#include <iostream>

namespace simxml {

QString simQuery(const std::string& query){
  QString result = "declare default element namespace \"http://www.lbl.gov/namespaces/Sim/Model\"; \n\
declare namespace xsi = \"http://www.w3.org/2001/XMLSchema-instance\"; \n\
declare namespace simcore = \"http://www.lbl.gov/namespaces/Sim/SimModelCore\"; \n\
declare namespace simres = \"http://www.lbl.gov/namespaces/Sim/ResourcesGeneral\"; \n\
declare namespace simgeom = \"http://www.lbl.gov/namespaces/Sim/ResourcesGeometry\"; \n\
declare namespace simbldg = \"http://www.lbl.gov/namespaces/Sim/BuildingModel\"; \n\
declare namespace simmep = \"http://www.lbl.gov/namespaces/Sim/MepModel\"; \n";
  result += QString::fromStdString(query);

  return result;
}

SimXmlDoc::SimXmlDoc()
  : xmllib::XmlDoc("", QSharedPointer<QDomDocument>(new QDomDocument()))
{
  QDomProcessingInstruction xmlProcessingInstruction = impl()->createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");
  impl()->appendChild(xmlProcessingInstruction);

  QDomElement simModelElement = impl()->createElement("SimModel");
  simModelElement.setAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
  simModelElement.setAttribute("xmlns", "http://www.lbl.gov/namespaces/Sim/Model");
  simModelElement.setAttribute("xmlns:simcore", "http://www.lbl.gov/namespaces/Sim/SimModelCore");
  simModelElement.setAttribute("xmlns:simres", "http://www.lbl.gov/namespaces/Sim/ResourcesGeneral");
  simModelElement.setAttribute("xmlns:simgeom", "http://www.lbl.gov/namespaces/Sim/ResourcesGeometry");
  simModelElement.setAttribute("xmlns:simbldg", "http://www.lbl.gov/namespaces/Sim/BuildingModel");
  simModelElement.setAttribute("xmlns:simmep", "http://www.lbl.gov/namespaces/Sim/MepModel");
  impl()->appendChild(simModelElement);
}

SimXmlDoc SimXmlDoc::load(const std::string& path)
{
  QFile file(QString::fromStdString(path));
  if (!file.open(QIODevice::ReadOnly)){
     return SimXmlDoc(path, QSharedPointer<QDomDocument>());
  }

  QSharedPointer<QDomDocument> impl(new QDomDocument());
  if (!impl->setContent(&file, true)) {
     file.close();
     return SimXmlDoc(path, QSharedPointer<QDomDocument>());
  }
  file.close();

  return SimXmlDoc(path, impl);
}

SimXmlDoc::SimXmlDoc(const std::string& path, QSharedPointer<QDomDocument> impl)
  : xmllib::XmlDoc(path, impl)
{
}

SimXmlDoc::SimXmlDoc(const XmlDoc& doc)
  : xmllib::XmlDoc(doc)
{
}

SimXmlDoc::~SimXmlDoc()
{
}

SimXmlElement SimXmlDoc::addElement(const std::string& elementName)
{
  QSharedPointer<QDomElement> elementImpl(new QDomElement(impl()->createElement(QString::fromStdString(elementName))));
  elementImpl->setAttribute("RefId", QUuid::createUuid().toString());

  SimXmlElement result(elementImpl, *this);

  QDomElement documentElement = impl()->documentElement();

  // insert in alphabetical order
  QDomNode n = documentElement.firstChild();

  while (!n.isNull()) {
    if (n.isElement()) {
      QDomElement e = n.toElement();
      // if element name is greater than new one
      if (e.tagName().toStdString() > elementName){
        documentElement.insertBefore(*elementImpl, n);
        return result;
      }
    }
    n = n.nextSibling();
  }

  documentElement.appendChild(*elementImpl);
  return result;
}

std::vector<SimXmlElement> SimXmlDoc::getElements(const std::string& elementName) const
{
  std::vector<SimXmlElement> result;

  if (isNull()){
    return result;
  }

  QString queryString = simQuery("/SimModel/" + elementName);

  QXmlQuery query;
  QDomNodeModel model(query.namePool(), *(impl()));
  query.setFocus(QXmlItem(model.fromDomNode(impl()->documentElement())));
  query.setQuery(queryString, QUrl(QString::fromStdString(this->path())));

  if (query.isValid()) {

    QString junk;

    QXmlResultItems items;

    query.evaluateTo(&items);

    QXmlItem item(items.next());

    while (!item.isNull()) {

      QDomElement elem = model.toDomNode(item.toNodeModelIndex()).toElement();

      QSharedPointer<QDomElement> impl(new QDomElement(elem));

      result.push_back(SimXmlElement(impl, *this));

      // get next item
      item = items.next();
    }
  }

  return result;
}

SimXmlElement SimXmlDoc::findElement(const std::string& refId) const
{
  return findElement("*", refId);
}

SimXmlElement SimXmlDoc::findElement(const std::string& elementName, const std::string& refId) const
{
  SimXmlElement result(QSharedPointer<QDomElement>(), *this);

  if (isNull()){
    return result;
  }

  QString queryString = simQuery("/SimModel/" + elementName + "[@RefId='" + refId + "'][1]");

  QXmlQuery query;
  QDomNodeModel model(query.namePool(), *(impl()));
  query.setFocus(QXmlItem(model.fromDomNode(impl()->documentElement())));
  query.setQuery(queryString, QUrl(QString::fromStdString(this->path())));

  if (query.isValid()) {

    QXmlResultItems items;

    query.evaluateTo(&items);

    QXmlItem item(items.next());

    if(!item.isNull()) {

      QDomElement elem = model.toDomNode(item.toNodeModelIndex()).toElement();

      QSharedPointer<QDomElement> impl(new QDomElement(elem));

      result = SimXmlElement(impl, *this);
    }
  }

  return result;
}

std::vector<SimXmlElement> SimXmlDoc::findElementsWithChildValue(const std::string& childName, const std::string& childValue) const
{
  return findElementsWithChildValue("*", childName, childValue);
}

std::vector<SimXmlElement> SimXmlDoc::findElementsWithChildValue(const std::string& elementName, const std::string& childName, const std::string& childValue) const
{
  std::vector<SimXmlElement> result;

  if (isNull()){
    return result;
  }

  QString queryString = simQuery("/SimModel/" + elementName + "[" + childName + "='" + childValue + "']");

  QXmlQuery query;
  QDomNodeModel model(query.namePool(), *(impl()));
  query.setFocus(QXmlItem(model.fromDomNode(impl()->documentElement())));
  query.setQuery(queryString, QUrl(QString::fromStdString(this->path())));

  if (query.isValid()) {

    QXmlResultItems items;

    query.evaluateTo(&items);

    QXmlItem item(items.next());

    while (!item.isNull()) {

      QDomElement elem = model.toDomNode(item.toNodeModelIndex()).toElement();

      QSharedPointer<QDomElement> impl(new QDomElement(elem));

      result.push_back(SimXmlElement(impl, *this));

      // get next item
      item = items.next();
    }
  }

  return result;
}

} // simxml