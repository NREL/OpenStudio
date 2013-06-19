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
#ifndef XSDGEN_XSDFILE_HPP
#define XSDGEN_XSDFILE_HPP

#include <Element.hpp>
#include <SimpleType.hpp>
#include <ComplexType.hpp>

#include <string>
#include <vector>
#include <map>

#include <QSharedPointer>

class QDomDocument;

/** XsdFile represent an xsd schema.*/
class XsdFile {
public:

  XsdFile(const std::string& path);

  virtual ~XsdFile();

  std::string path() const;

  std::string defaultNamespace() const;

  std::string defaultPrefix() const;

  QSharedPointer<Element> findElement(const std::string& qualifiedName) const;

  QSharedPointer<SimpleType> findSimpleType(const std::string& qualifiedName) const;

  QSharedPointer<ComplexType> findComplexType(const std::string& qualifiedName) const;

  std::vector<std::string> importNames() const;

  std::vector<Element> elements() const;

  std::vector<SimpleType> simpleTypes() const;

  std::vector<ComplexType> complexTypes() const;

private:

  void parse();

  std::string m_path;
  QSharedPointer<QDomDocument> m_doc;

  std::string m_defaultNamespace;
  std::string m_defaultPrefix;
  std::vector<std::string> m_importNames;
  std::vector<XsdFile> m_imports;
  std::map<std::string, Element> m_elementMap;
  std::map<std::string, SimpleType> m_simpleTypeMap;
  std::map<std::string, ComplexType> m_complexTypeMap;

};

#endif //XSDGEN_XSDFILE_HPP