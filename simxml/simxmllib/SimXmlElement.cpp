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
#include <SimXmlElement.hpp>
#include <SimXmlDoc.hpp>

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

#include <QDomNodeModel>

#include <iostream>
#include <sstream>

namespace simxml {

SimXmlElement::SimXmlElement(QSharedPointer<QDomElement> impl, const SimXmlDoc& doc)
  : xmllib::XmlElement(impl, doc)
{
}

SimXmlElement::~SimXmlElement()
{
}

SimXmlDoc SimXmlElement::simXmlDoc() const
{
  SimXmlDoc simXmlDoc(this->xmlDoc());
  return simXmlDoc;
}

std::string SimXmlElement::getChildText(const std::string& childName) const
{
  std::string result;

  if (isNull()){
    return result;
  }
  
  QString queryString = simQuery(childName + "[1]");

  QXmlQuery query;
  QDomNodeModel model(query.namePool(), *(simXmlDoc().impl()));
  query.setFocus(QXmlItem(model.fromDomNode(*(impl()))));
  query.setQuery(queryString, QUrl(QString::fromStdString(simXmlDoc().path())));

  if (query.isValid()) {

    QXmlResultItems items;

    query.evaluateTo(&items);

    QXmlItem item(items.next());

    if (!item.isNull()) {

      QDomElement elem = model.toDomNode(item.toNodeModelIndex()).toElement();

      result = elem.text().toStdString();
    }
  }

  return result;
}
  
bool SimXmlElement::getChildBool(const std::string& childName) const
{
  bool result = false;
  std::string text = getChildText(childName);
  if (text == "true"){
    result = true;
  }
  return result;
}

int SimXmlElement::getChildInt(const std::string& childName) const
{
  std::string text = getChildText(childName);
  int result = QString::fromStdString(text).toInt();
  return result;
}

double SimXmlElement::getChildDouble(const std::string& childName) const
{
  std::string text = getChildText(childName);
  double result = QString::fromStdString(text).toDouble();
  return result;
}

std::vector<int> SimXmlElement::getChildIntegerList(const std::string& childName) const
{
  std::vector<int> result;
  std::vector<std::string> parts = xmllib::split(getChildText(childName), ' ');
  for (std::vector<std::string>::const_iterator it = parts.begin(), itend = parts.end(); it != itend; ++it){
    result.push_back(QString::fromStdString(*it).toInt());
  } 
  return result;
}

std::vector<double> SimXmlElement::getChildDoubleList(const std::string& childName) const
{
  std::vector<double> result;
  std::vector<std::string> parts = xmllib::split(getChildText(childName), ' ');
  for (std::vector<std::string>::const_iterator it = parts.begin(), itend = parts.end(); it != itend; ++it){
    result.push_back(QString::fromStdString(*it).toDouble());
  } 
  return result;
}

bool SimXmlElement::setChildText(const std::string& childName, const std::string& childText)
{
  // insert in alphabetical order
  QDomNode n = impl()->firstChild();
  QDomNode lessThanNode;

  while (!n.isNull()) {
    if (n.isElement()) {
      QDomElement e = n.toElement();
      
      if (e.tagName().toStdString() == childName){
        // if element name is equal to childName
        QDomText textNode = simXmlDoc().impl()->createTextNode(QString::fromStdString(childText));

        QDomNode oldChild = e.firstChild();
        if (oldChild.isNull()){
          e.appendChild(textNode);
        }else{
          e.replaceChild(textNode, oldChild);
        }
        return true;

      }else if (childName < e.tagName().toStdString()){
        // if element name is less than childName
        lessThanNode = n;
      }
    }
    n = n.nextSibling();
  }

  QDomElement childElement = simXmlDoc().impl()->createElement(QString::fromStdString(childName));
  QDomText textNode = simXmlDoc().impl()->createTextNode(QString::fromStdString(childText));
  childElement.appendChild(textNode);

  if (!lessThanNode.isNull()){
    impl()->insertBefore(childElement, lessThanNode);
    return true;
  }

  impl()->appendChild(childElement);
  return true;
}

bool SimXmlElement::setChildBool(const std::string& childName, bool value)
{
  if (value){
    return setChildText(childName, "true"); 
  }
  return setChildText(childName, "false"); 
}

bool SimXmlElement::setChildInt(const std::string& childName, int value)
{
  std::stringstream ss;
  ss << value;
  return setChildText(childName, ss.str()); 
}

bool SimXmlElement::setChildDouble(const std::string& childName, double value)
{
  std::stringstream ss;
  ss << value;
  return setChildText(childName, ss.str()); 
}

bool SimXmlElement::setChildIntegerList(const std::string& childName, const std::vector<int>& value)
{
  std::stringstream ss;
  for (std::vector<int>::const_iterator it = value.begin(), itend = value.end(); it != itend; ++it){
    ss << *it;
    if (it != itend-1){
      ss << " ";
    }
  }
  return setChildText(childName, ss.str());
}

bool SimXmlElement::setChildDoubleList(const std::string& childName, const std::vector<double>& value)
{
  std::stringstream ss;
  for (std::vector<double>::const_iterator it = value.begin(), itend = value.end(); it != itend; ++it){
    ss << *it;
    if (it != itend-1){
      ss << " ";
    }
  }
  return setChildText(childName, ss.str());
}

} // simxml