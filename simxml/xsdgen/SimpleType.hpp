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
#ifndef XSDGEN_SIMPLETYPE_HPP
#define XSDGEN_SIMPLETYPE_HPP

#include <string>
#include <vector>
#include <ostream>

#include <QSharedPointer>

class QDomElement;

/** A SimpleType is a basic type such as xs:int.*/
class SimpleType {

public:

  SimpleType(const QDomElement& element, const std::string& defaultNamespace, const std::string& defaultPrefix);

  virtual ~SimpleType();

  std::string prefix() const;

  std::string name() const;

  std::string type() const;

  std::string restriction() const;

  std::string list() const;

  std::string qualifiedName() const;

  bool operator<(const SimpleType& other) const;

private:
  
  void parse(const std::string& defaultNamespace, const std::string& defaultPrefix);

  QSharedPointer<QDomElement> m_element;

  std::string m_prefix;
  std::string m_name;
  std::string m_type;
  std::string m_restriction;
  std::string m_list;
};

#endif //XSDGEN_SIMPLETYPE_HPP