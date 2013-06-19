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
#include <SimpleType.hpp>

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


SimpleType::SimpleType(const QDomElement& element, const std::string& defaultNamespace, const std::string& defaultPrefix)
  : m_element(new QDomElement(element))
{
  parse(defaultNamespace, defaultPrefix);
}

SimpleType::~SimpleType()
{
}

std::string SimpleType::prefix() const
{
  return m_prefix;
}

std::string SimpleType::name() const
{
  return m_name;
}

std::string SimpleType::type() const
{
  return m_type;
}

std::string SimpleType::restriction() const
{
  return m_restriction;
}

std::string SimpleType::list() const
{
  return m_list;
}

std::string SimpleType::qualifiedName() const
{
  return m_prefix + ":" + m_name;
}

bool SimpleType::operator<(const SimpleType& other) const
{
  return (this->qualifiedName() < other.qualifiedName());
}

void SimpleType::parse(const std::string& defaultNamespace, const std::string& defaultPrefix)
{

  QStringList nameParts = m_element->attribute("name").split(":");
  if (nameParts.size() == 1){
    m_prefix = defaultPrefix;
    m_name = nameParts.at(0).toStdString();
  }else if (nameParts.size() == 2){
    m_prefix = nameParts.at(0).toStdString();
    m_name = nameParts.at(1).toStdString();
  }

  QDomElement listContent = m_element->firstChildElement("list");
  if (!listContent.isNull()){
    m_list = listContent.attribute("itemType").toStdString();
  }

  QDomElement restrictionContent = m_element->firstChildElement("restriction");
  if (!restrictionContent.isNull()){
    m_restriction = restrictionContent.attribute("base").toStdString();
  }

}
