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

#include "TypedFeature.hpp"

namespace openstudio {
namespace model {

TypedFeature::TypedFeature() : m_name(""), m_datatype(TYPEDFEATUREDATATYPE_STRING), m_str_value("")
{}

TypedFeature::TypedFeature(const std::string &name, double value) {
  m_name = name;
  m_datatype = TYPEDFEATUREDATATYPE_DOUBLE;
  m_double_value = value;
}

TypedFeature::TypedFeature(const std::string &name, int value) {
  m_name = name;
  m_datatype = TYPEDFEATUREDATATYPE_INT;
  m_int_value = value;
}

TypedFeature::TypedFeature(const std::string &name, bool value) {
  m_name = name;
  m_datatype = TYPEDFEATUREDATATYPE_BOOL;
  m_bool_value = value;
}

TypedFeature::TypedFeature(const std::string &name, const std::string &value) {
  m_name = name;
  m_datatype = TYPEDFEATUREDATATYPE_STRING;
  m_str_value = value;
}

TypedFeature::TypedFeature(const std::string &name, const char *value) {
  m_name = name;
  m_datatype = TYPEDFEATUREDATATYPE_STRING;
  m_str_value = std::string(value);
}

void TypedFeature::setName(const std::string &name) {
  m_name = name;
}

void TypedFeature::setValue(double value) {
  m_datatype = TYPEDFEATUREDATATYPE_DOUBLE;
  m_double_value = value;
}

void TypedFeature::setValue(int value) {
  m_datatype = TYPEDFEATUREDATATYPE_INT;
  m_int_value = value;
}

void TypedFeature::setValue(bool value) {
  m_datatype = TYPEDFEATUREDATATYPE_BOOL;
  m_bool_value = value;
}

void TypedFeature::setValue(const char *value) {
  m_datatype = TYPEDFEATUREDATATYPE_STRING;
  m_str_value = std::string(value);
}

void TypedFeature::setValue(const std::string &value) {
  m_datatype = TYPEDFEATUREDATATYPE_STRING;
  m_str_value = value;
}

TypedFeatureDataType TypedFeature::getDataType() const {
  return m_datatype;
}

std::string TypedFeature::getName() const {
  return m_name;
}

boost::optional<double> TypedFeature::getValueAsDouble() const {
  boost::optional<double> return_value;
  if (m_datatype == TYPEDFEATUREDATATYPE_DOUBLE) {
    return_value = m_double_value;
  }
  return return_value;
}

boost::optional<int> TypedFeature::getValueAsInteger() const {
  boost::optional<int> return_value;
  if (m_datatype == TYPEDFEATUREDATATYPE_INT) {
    return_value = m_int_value;
  }
  return return_value;
}

boost::optional<bool> TypedFeature::getValueAsBool() const {
  boost::optional<bool> return_value;
  if (m_datatype == TYPEDFEATUREDATATYPE_BOOL) {
    return_value = m_bool_value;
  }
  return return_value;
}

boost::optional<std::string> TypedFeature::getValueAsString() const {
  boost::optional<std::string> return_value;
  if (m_datatype == TYPEDFEATUREDATATYPE_STRING) {
    return_value = m_str_value;
  }
  return return_value;
}

} // model
} //openstudio
