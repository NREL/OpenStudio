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

#include <ComplexType.hpp>
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

#include <QDomNodeModel>

#include <iostream>

ComplexType::ComplexType(const QDomElement& element, const std::string& defaultNamespace, const std::string& defaultPrefix)
  : m_element(new QDomElement(element)), m_haveBases(false)
{
  parse(defaultNamespace, defaultPrefix);
}

ComplexType::~ComplexType()
{
}

std::string ComplexType::prefix() const
{
  return m_prefix;
}

std::string ComplexType::name() const
{
  return m_name;
}

std::string ComplexType::isAbstract() const
{
  return m_isAbstract;
}

std::string ComplexType::base() const
{
  return m_base;
}

std::vector<std::string> ComplexType::bases() const
{
  return m_bases;
}

std::vector<std::string> ComplexType::makeBaseList(const XsdFile& xsdFile)
{
  if (m_haveBases){
    return m_bases;
  }

  if (!m_base.empty()){
    m_bases.push_back(m_base);

    QSharedPointer<ComplexType> base = xsdFile.findComplexType(m_base);
    if (base){
      std::vector<std::string> temp = base->makeBaseList(xsdFile);
      m_bases.insert(m_bases.end(), temp.begin(), temp.end());
    }
  }

  m_haveBases = true;

  return m_bases;
}

std::string ComplexType::model() const
{
  return m_model;
}

std::vector<std::string> ComplexType::attributes() const
{
  return m_attributes;
}

std::vector<Element> ComplexType::elements() const
{
  return m_elements;
}

std::string ComplexType::qualifiedName() const
{
  return m_prefix + ":" + m_name;
}

bool ComplexType::operator<(const ComplexType& other) const
{
  return (this->qualifiedName() < other.qualifiedName());
}

void ComplexType::parse(const std::string& defaultNamespace, const std::string& defaultPrefix)
{
  QStringList nameParts = m_element->attribute("name").split(":");
  if (nameParts.size() == 1){
    m_prefix = defaultPrefix;
    m_name = nameParts.at(0).toStdString();
  }else if (nameParts.size() == 2){
    m_prefix = nameParts.at(0).toStdString();
    m_name = nameParts.at(1).toStdString();
  }

  m_isAbstract = m_element->attribute("abstract").toStdString();

  QDomElement complexContent = m_element->firstChildElement("complexContent");
  if (!complexContent.isNull()){

    QDomElement extension = complexContent.firstChildElement("extension");
    if (!extension.isNull()){
      m_base =	extension.attribute("base").toStdString();

      QDomElement sequence = extension.firstChildElement("sequence");
      if (!sequence.isNull()){

        QDomElement element = sequence.firstChildElement("element");
        while (!element.isNull()){

          m_elements.push_back(Element(element, defaultNamespace, defaultPrefix, false));

          element = element.nextSiblingElement("element");
        }
      }
    }
  }

  QDomElement sequence = m_element->firstChildElement("sequence");
  if (!sequence.isNull()){

    QDomElement element = sequence.firstChildElement("element");
    while (!element.isNull()){

      m_elements.push_back(Element(element, defaultNamespace, defaultPrefix, false));

      element = element.nextSiblingElement("element");
    }
  }
}

