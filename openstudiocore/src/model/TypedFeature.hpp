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

#ifndef OPENSTUDIO_TYPEDFEATURE_HPP
#define OPENSTUDIO_TYPEDFEATURE_HPP

#include <string>
#include <boost/optional.hpp>

namespace openstudio {
namespace model {

enum TypedFeatureDataType {
    TYPEDFEATUREDATATYPE_STRING,
    TYPEDFEATUREDATATYPE_DOUBLE,
    TYPEDFEATUREDATATYPE_BOOL,
    TYPEDFEATUREDATATYPE_INT
};

class TypedFeature {
 public:
  TypedFeature();

  TypedFeature(const std::string & name, double value);

  TypedFeature(const std::string & name, int value);

  TypedFeature(const std::string & name, bool value);

  TypedFeature(const std::string & name, const std::string & value);

  TypedFeature(const std::string & name, const char* value);

  void setName(const std::string & name);

  void setValue(double value);

  void setValue(int value);

  void setValue(bool value);

  void setValue(const char* value);

  void setValue(const std::string & value);

  TypedFeatureDataType getDataType() const;

  std::string getName() const;

  boost::optional<double> getValueAsDouble() const;

  boost::optional<int> getValueAsInteger() const;

  boost::optional<bool> getValueAsBool() const;

  boost::optional<std::string> getValueAsString() const;

 private:

  std::string m_name;
  TypedFeatureDataType m_datatype;
  double m_double_value;
  int m_int_value;
  bool m_bool_value;
  std::string m_str_value;

};

} // model
} // openstudio

#endif //OPENSTUDIO_TYPEDFEATURE_HPP
