/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef UTILITIES_DATA_VARIANT_HPP
#define UTILITIES_DATA_VARIANT_HPP

#include "../UtilitiesAPI.hpp"

#include "../core/Enum.hpp"
#include "../core/Logger.hpp"

#include <optional>
#include <variant>

namespace openstudio {

// clang-format off

/** \class VariantType
 *  \brief A listing of data types that can be held in a Variant.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual macro
 *  call is:
 *  \code
OPENSTUDIO_ENUM(VariantType,
  ((Boolean)(Boolean)(0))
  ((Double)(Double)(1))
  ((Integer)(Integer)(2))
  ((String)(String)(3))
);
 *  \endcode */
OPENSTUDIO_ENUM(VariantType,
  ((Boolean)(Boolean)(0))
  ((Double)(Double)(1))
  ((Integer)(Integer)(2))
  ((String)(String)(3))
);

// clang-format on

/** A Variant is a flexible type that can hold several types of values.
 */
class UTILITIES_API Variant
{
 public:
  /// constructors
  explicit Variant();
  explicit Variant(bool value);
  explicit Variant(double value);
  explicit Variant(int value);
  explicit Variant(const char* value);
  explicit Variant(const std::string& value);

  /// get the variant value type
  VariantType variantType() const;

  /// get value as a bool
  bool valueAsBoolean() const;

  /// get value as int
  int valueAsInteger() const;

  /// get value as double
  double valueAsDouble() const;

  /// get value as string
  std::string valueAsString() const;

 private:
  REGISTER_LOGGER("openstudio.Variant");

  VariantType m_type;
  std::variant<bool, int, double, std::string> m_value;
};

/** \relates Variant */
using OptionalVariant = std::optional<Variant>;

/** \relates Variant */
using VariantVector = std::vector<Variant>;

UTILITIES_API std::ostream& operator<<(std::ostream& os, const Variant& variant);

}  // namespace openstudio

#endif  // UTILITIES_DATA_VARIANT_HPP
