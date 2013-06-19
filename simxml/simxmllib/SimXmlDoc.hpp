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
#ifndef SIMXMLDOC_HPP
#define SIMXMLDOC_HPP

#include <XmlDoc.hpp>

#include <string>
#include <vector>

#include <QSharedPointer>

class QDomDocument;
class QDomElement;
class QString;

namespace simxml {

class SimXmlElement;

QString simQuery(const std::string& query);

class SimXmlDoc : public xmllib::XmlDoc {
public:

  SimXmlDoc();

  static SimXmlDoc load(const std::string& path);

  virtual ~SimXmlDoc();

  SimXmlElement addElement(const std::string& elementName);

  std::vector<SimXmlElement> getElements(const std::string& elementName) const;

  SimXmlElement findElement(const std::string& refId) const;
  SimXmlElement findElement(const std::string& elementName, const std::string& refId) const;

  std::vector<SimXmlElement> findElementsWithChildValue(const std::string& childName, const std::string& childValue) const;
  std::vector<SimXmlElement> findElementsWithChildValue(const std::string& elementName, const std::string& childName, const std::string& childValue) const;

private:

  friend class SimXmlElement;

  SimXmlDoc(const std::string& path, QSharedPointer<QDomDocument> impl);

  SimXmlDoc(const XmlDoc& doc);

};

} // simxml


#endif //SIMXMLDOC_HPP