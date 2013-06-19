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
#include <XmlDoc.hpp>
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
#include <fstream>

namespace xmllib {

std::vector<std::string> split(const std::string& input, const char delim)
{
  std::vector<std::string> result;

  QString temp = QString::fromStdString(input);
  QStringList list = temp.split(delim);

  foreach (const QString &str, list) {
    result.push_back(str.toStdString());
  }

  return result;
}

XmlDoc::XmlDoc(const XmlDoc& other)
  : m_path(other.m_path), m_impl(other.m_impl)
{
}

XmlDoc::XmlDoc(const std::string& path, QSharedPointer<QDomDocument> impl)
  : m_path(path), m_impl(impl)
{
}

XmlDoc::~XmlDoc()
{
}

std::string XmlDoc::path() const
{
  return m_path;
}

bool XmlDoc::write(const std::string& path)
{
  std::ofstream outfile(path.c_str());
  if (!outfile){
    return false;
  }

  m_path = path;

  this->operator<<(outfile);

  outfile.close();
  
  return true;
}

bool XmlDoc::isNull() const
{
  return m_impl.isNull();
}

std::ostream& XmlDoc::operator<<(std::ostream& os) const
{
  os << m_impl->toString().toStdString();
  return os;
}

QSharedPointer<QDomDocument> XmlDoc::impl() const
{
  return m_impl;
}

} // xmllib