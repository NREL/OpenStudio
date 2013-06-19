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
#ifndef SIMXMLELEMENT_HPP
#define SIMXMLELEMENT_HPP

#include <XmlElement.hpp>

#include <string>
#include <vector>

#include <QSharedPointer>

class QDomDocument;
class QDomElement;

namespace simxml {

class SimXmlDoc;

class SimXmlElement : public xmllib::XmlElement {
public:

  virtual ~SimXmlElement();

  SimXmlDoc simXmlDoc() const;

  std::string getChildText(const std::string& childName) const;

  bool getChildBool(const std::string& childName) const;

  int getChildInt(const std::string& childName) const;

  double getChildDouble(const std::string& childName) const;

  std::vector<int> getChildIntegerList(const std::string& childName) const;

  std::vector<double> getChildDoubleList(const std::string& childName) const;

  bool setChildText(const std::string& childName, const std::string& childText);  

  bool setChildBool(const std::string& childName, bool value);

  bool setChildInt(const std::string& childName, int value);

  bool setChildDouble(const std::string& childName, double value);

  bool setChildIntegerList(const std::string& childName, const std::vector<int>& value);

  bool setChildDoubleList(const std::string& childName, const std::vector<double>& value);

private:
  
  friend class SimXmlDoc;

  SimXmlElement(QSharedPointer<QDomElement> impl, const SimXmlDoc& doc);
};

} // simxml

#endif //SIMXMLELEMENT_HPP