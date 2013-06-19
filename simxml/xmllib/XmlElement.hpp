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
#ifndef XMLELEMENT_HPP
#define XMLELEMENT_HPP

#include <XmlDoc.hpp>

#include <string>
#include <vector>

#include <QSharedPointer>

class QDomDocument;
class QDomElement;

namespace xmllib {

class XmlElement {
public:

  virtual ~XmlElement();

  bool isNull() const;

  bool remove();

  XmlDoc xmlDoc() const;

  std::string elementName() const;

  std::string refId() const;

  std::ostream& operator<<(std::ostream& os) const;

protected:
  
  friend class XmlDoc;

  XmlElement(QSharedPointer<QDomElement> impl, const XmlDoc& doc);

  QSharedPointer<QDomElement> impl() const;

private:

  QSharedPointer<QDomElement> m_impl;
  XmlDoc m_doc;

};

} // xmllib

#endif //XMLELEMENT_HPP