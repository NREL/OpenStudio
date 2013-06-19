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
#ifndef XMLDOC_HPP
#define XMLDOC_HPP

#include <string>
#include <vector>

#include <QSharedPointer>

class QDomDocument;
class QDomElement;
class QString;

namespace xmllib {

class XmlElement;

std::vector<std::string> split(const std::string& input, const char delim);

class XmlDoc {
public:

  XmlDoc(const XmlDoc& other);

  virtual ~XmlDoc();

  std::string path() const;

  bool write(const std::string& path);

  bool isNull() const;

  std::ostream& operator<<(std::ostream& os) const;

protected:

  friend class XmlElement;

  XmlDoc(const std::string& path, QSharedPointer<QDomDocument> impl);

  QSharedPointer<QDomDocument> impl() const;

private:

  std::string m_path;
  QSharedPointer<QDomDocument> m_impl;

};

} //xmllib

#endif //XMLDOC_HPP