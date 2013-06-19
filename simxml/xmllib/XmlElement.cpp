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
#include <XmlElement.hpp>

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

namespace xmllib {

XmlElement::XmlElement(QSharedPointer<QDomElement> impl, const XmlDoc& doc)
  : m_impl(impl), m_doc(doc)
{
}

XmlElement::~XmlElement()
{
}

bool XmlElement::isNull() const
{
  return m_impl.isNull();
}

bool XmlElement::remove()
{
  return false;
}

XmlDoc XmlElement::xmlDoc() const
{
  return m_doc;
}

std::string XmlElement::elementName() const
{
  if (isNull()){
    return "";
  }

  QString value = m_impl->tagName();
  return value.toStdString();
}

std::string XmlElement::refId() const
{
  if (isNull()){
    return "";
  }

  QString value = m_impl->attribute("RefId");
  return value.toStdString();
}

std::ostream& XmlElement::operator<<(std::ostream& os) const
{
  QString* string = new QString;
  QTextStream stream(string, QIODevice::WriteOnly);
  stream << *m_impl;
  os << stream.readAll().toStdString();
  delete string;
  return os;
}

QSharedPointer<QDomElement> XmlElement::impl() const
{
  return m_impl;
}

} // xmllib