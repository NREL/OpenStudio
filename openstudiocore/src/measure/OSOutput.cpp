/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#include "OSOutput.hpp"

#include "../utilities/core/Assert.hpp"

#include <boost/lexical_cast.hpp>
#include <sstream>

namespace openstudio {
namespace measure {


OSOutput OSOutput::makeBoolOutput(const std::string& name, bool modelDependent)
{
  OSOutput result(name, OSOutputType::Boolean, modelDependent);
  return result;
}

OSOutput OSOutput::makeDoubleOutput(const std::string& name, bool modelDependent)
{
  OSOutput result(name, OSOutputType::Double, modelDependent);
  return result;
}

OSOutput OSOutput::makeIntegerOutput(const std::string& name, bool modelDependent)
{
  OSOutput result(name, OSOutputType::Integer, modelDependent);
  return result;
}

OSOutput OSOutput::makeStringOutput(const std::string& name, bool modelDependent)
{
  OSOutput result(name, OSOutputType::String, modelDependent);
  return result;
}

std::string OSOutput::name() const {
  return m_name;
}

std::string OSOutput::displayName() const {
  return m_displayName;
}

std::string OSOutput::shortName() const {
  return m_shortName;
}

boost::optional<std::string> OSOutput::description() const {
  return m_description;
}

OSOutputType OSOutput::type() const {
  return m_type;
}

boost::optional<std::string> OSOutput::units() const {
  return m_units;
}

bool OSOutput::modelDependent() const {
  return m_modelDependent;
}

void OSOutput::setDisplayName(const std::string& displayName) {
  m_displayName = displayName;
}

void OSOutput::setShortName(const std::string& shortName) {
  m_shortName = shortName;
}

void OSOutput::setDescription(const std::string& description)
{
  m_description = description;
}

void OSOutput::setUnits(const std::string& units)
{
  m_units = units;
}

std::string OSOutput::print() const {
  std::stringstream ss;

  // name
  ss << name();
  if (!displayName().empty()) {
    ss << " (" << displayName() << ")";
  }
  ss << std::endl;

  // type and required
  ss << type().valueName() << std::endl;

  return ss.str();

}

OSOutput::OSOutput()
{}

OSOutput::OSOutput(const std::string& name,
                   const OSOutputType& type,
                   bool modelDependent)
  : m_name(name),
    m_displayName(name),
    m_shortName(name),
    m_type(type),
    m_modelDependent(modelDependent)
{}

std::ostream& operator<<(std::ostream& os, const OSOutput& output) {
  os << output.print();
  return os;
}

} // measure
} // openstudio
