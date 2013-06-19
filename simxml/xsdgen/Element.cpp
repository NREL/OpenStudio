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
#include <Element.hpp>

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

Element::Element(const QDomElement& element, const std::string& defaultNamespace, const std::string& defaultPrefix, bool isTopLevel)
  : m_element(new QDomElement(element)), m_isTopLevel(isTopLevel)
{
  parse(defaultNamespace, defaultPrefix);
}

Element::~Element()
{
}
  
bool Element::isTopLevel() const
{
  return m_isTopLevel;
}

std::string Element::prefix() const
{
  return m_prefix;
}

std::string Element::name() const
{
  return m_name;
}

std::string Element::type() const
{
  return m_type;
}

std::string Element::refPrefix() const
{
  return m_refPrefix;
}

std::string Element::refName() const
{
  return m_refName;
}

std::string Element::minOccurs() const
{
  return m_minOccurs;
}

std::string Element::maxOccurs() const
{
  return m_maxOccurs;
}

std::string Element::qualifiedName() const
{
  return m_prefix + ":" + m_name;
}

bool Element::operator<(const Element& other) const
{
  return (this->qualifiedName() < other.qualifiedName());
}

void Element::parse(const std::string& defaultNamespace, const std::string& defaultPrefix)
{
  QStringList nameParts = m_element->attribute("name").split(":");
  if (nameParts.size() == 1){
    m_prefix = defaultPrefix;
    m_name = nameParts.at(0).toStdString();
  }else if (nameParts.size() == 2){
    m_prefix = nameParts.at(0).toStdString();
    m_name = nameParts.at(1).toStdString();
  }

  QStringList refParts = m_element->attribute("ref").split(":");
  if (refParts.size() == 1){
    m_refPrefix = defaultPrefix;
    m_refName = refParts.at(0).toStdString();
  }else if (refParts.size() == 2){
    m_refPrefix = refParts.at(0).toStdString();
    m_refName = refParts.at(1).toStdString();
  }

  m_type = m_element->attribute("type").toStdString();
  m_minOccurs = m_element->attribute("minOccurs").toStdString();
  m_maxOccurs = m_element->attribute("maxOccurs").toStdString();
}
