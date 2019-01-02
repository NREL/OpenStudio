/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "Variant.hpp"


namespace openstudio {

  Variant::Variant()
    : m_type(VariantType::Boolean), m_value(false)
  {
  }

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
    if (m_type == VariantType::Integer){
      return (double)valueAsInteger();
    }
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

