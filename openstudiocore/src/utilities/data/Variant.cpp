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

#include "Variant.hpp"

#include "../core/Assert.hpp"

namespace openstudio {

  Variant::Variant(bool value)
    : m_type(VariantType::Boolean), m_value(value)
  {
  }

  Variant::Variant(double value)
    : m_type(VariantType::Double), m_value(value)
  {
  }

  Variant::Variant(int value)
    : m_type(VariantType::Integer), m_value(value)
  {
  }

  Variant::Variant(const char* value)
    : m_type(VariantType::String), m_value(std::string(value))
  {
  }

  Variant::Variant(const std::string& value)
    : m_type(VariantType::String), m_value(value)
  {
  }

  VariantType Variant::variantType() const
  {
    return m_type;
  }

  bool Variant::valueAsBoolean() const
  {
    return boost::get<bool>(m_value);
  }

  int Variant::valueAsInteger() const
  {
    return boost::get<int>(m_value);
  }

  double Variant::valueAsDouble() const
  {
    return boost::get<double>(m_value);
  }

  std::string Variant::valueAsString() const
  {
    return boost::get<std::string>(m_value);
  }

  std::ostream& operator<<(std::ostream& os, const Variant& variant)
  {
    VariantType variantType = variant.variantType();
    if (variantType == VariantType::String){
      os << variant.valueAsString();
    } else if (variantType == VariantType::Double){
      os << variant.valueAsDouble();
    } else if (variantType == VariantType::Integer){
      os << variant.valueAsInteger();
    } else if (variantType == VariantType::Boolean){
      if (variant.valueAsBoolean()){
        os << "true";
      } else {
        os << "false";
      }
    }
    return os;
  }

} // openstudio

