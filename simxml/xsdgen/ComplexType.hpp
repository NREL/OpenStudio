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
#ifndef XSDGEN_COMPLEXTYPE_HPP
#define XSDGEN_COMPLEXTYPE_HPP

#include <Element.hpp>

#include <string>
#include <vector>
#include <ostream>

#include <QSharedPointer>

class QDomElement;

class XsdFile;

class ComplexType {
public:

  ComplexType(const QDomElement& element, const std::string& defaultNamespace, const std::string& defaultPrefix);

  virtual ~ComplexType();

  std::string prefix() const;

  std::string name() const;

  std::string isAbstract() const;

  std::string base() const; 

  std::vector<std::string> bases() const; 

  std::vector<std::string> makeBaseList(const XsdFile& xsdFile);

  std::string model() const;

  std::vector<std::string> attributes() const;

  std::vector<Element> elements() const;

  std::string qualifiedName() const;

  bool operator<(const ComplexType& other) const;

private:

  void parse(const std::string& defaultNamespace, const std::string& defaultPrefix);

  QSharedPointer<QDomElement> m_element;

  std::string m_prefix;
  std::string m_name;
  std::string m_isAbstract;
  std::string m_base; // base class
  std::vector<std::string>  m_bases; // list of all base classes
  bool m_haveBases;
  std::string m_model; // choice or sequence
  std::vector<std::string> m_attributes;
  std::vector<Element> m_elements;
};

#endif //XSDGEN_COMPLEXTYPE_HPP