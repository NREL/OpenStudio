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
#ifndef XSDGEN_ELEMENT_HPP
#define XSDGEN_ELEMENT_HPP

#include <string>
#include <vector>
#include <ostream>

#include <QSharedPointer>

class QDomElement;

/** An Element is an instance of a XSD Type. */
class Element {
public:

  Element(const QDomElement& element, const std::string& defaultNamespace, const std::string& defaultPrefix, bool isTopLevel);

  virtual ~Element();

  bool isTopLevel() const;

  std::string prefix() const;

  std::string name() const;

  std::string type() const;

  std::string refPrefix() const;

  std::string refName() const;

  std::string minOccurs() const;

  std::string maxOccurs() const;

  std::string qualifiedName() const;

  bool operator<(const Element& other) const;

private:

  void parse(const std::string& defaultNamespace, const std::string& defaultPrefix);

  QSharedPointer<QDomElement> m_element;

  bool m_isTopLevel;
  std::string m_prefix;
  std::string m_name;
  std::string m_type;
  std::string m_refPrefix;
  std::string m_refName;
  std::string m_minOccurs;
  std::string m_maxOccurs;

};

#endif //XSDGEN_ELEMENT_HPP