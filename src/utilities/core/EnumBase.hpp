/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef UTILITIES_CORE_ENUMBASE_HPP
#define UTILITIES_CORE_ENUMBASE_HPP

#include "ASCIIStrings.hpp"
#include "Span.hpp"

#include <stdexcept>

#include <fmt/format.h>

struct EnumField
{
  int value;
  std::string_view name;
  std::string_view description;
};

/** Base class for OPENSTUDIO_ENUMs. Comparison operators use the underlying (integer) value. */
template <typename Enum>
class EnumBase
{
 public:
  /** Default constructor. EnumBase default constructor never actually called;
     * OPENSTUDIO_ENUM- generated classes have default constructors that set the
     * value to the first item in the enumerated list. */
  EnumBase() = delete;

  /** Construct from t_value. Throws std::runtime_error if t_value is not in
     * the domain. */
  constexpr EnumBase(int t_value) : m_value(lookupValue(t_value)) {}

  /** Construct from t_value. Throws std::runtime_error if t_value does not
     * match a name or a description (case insensitive). */
  constexpr EnumBase(const std::string_view t_value) : m_value(lookupValue(t_value)) {}

  /** Returns the name associated with t_value, if it exists. Otherwise,
     * throws std::runtime_error. */
  constexpr static std::string_view valueName(int t_value) {
    for (auto& e : Enum::getFields()) {
      if (e.value == t_value) {
        return e.name;
      }
    }
    throw std::runtime_error(fmt::format("Invalid domain for OpenStudio Enum {}", Enum::enumName()));
  }

  /** Returns the description associated with t_value, if it exists.
     * Otherwise, throws std::runtime_error. */
  constexpr static std::string_view valueDescription(int t_value) {
    for (auto& e : Enum::getFields()) {
      if (e.value == t_value) {
        return e.description;
      }
    }

    throw std::runtime_error(fmt::format("Invalid domain for OpenStudio Enum {}", Enum::enumName()));
  }

  /** Returns the set of all values in this enum's domain. */
  static const constexpr openstudio::span<int>& getValues() {
    return Enum::getDomainValues();
  }

  /** Returns this instance's current value (as an integer). */
  constexpr int value() const {
    return m_value;
  }

  /** Returns the name associated with this instance's current value. */
  constexpr std::string_view valueName() const {
    return valueName(m_value);
  }

  /** Returns the description associated with this instance's current value.
     */
  constexpr std::string_view valueDescription() const {
    return valueDescription(m_value);
  }

  /** Set this instance's value to t_value. Throws std::runtime_error if
     *  t_value is not in the domain. */
  constexpr void setValue(int t_value) {
    m_value = lookupValue(t_value);
  }

  constexpr bool operator==(const Enum& other) const {
    return (m_value == other.m_value);
  }
  constexpr bool operator!=(const Enum& other) const {
    return (m_value != other.m_value);
  }
  constexpr bool operator>(const Enum& other) const {
    return (m_value > other.m_value);
  }
  constexpr bool operator>=(const Enum& other) const {
    return (m_value >= other.m_value);
  }
  constexpr bool operator<(const Enum& other) const {
    return (m_value < other.m_value);
  }
  constexpr bool operator<=(const Enum& other) const {
    return (m_value <= other.m_value);
  }

  /** Returns the (integer) value associated with t_name, as determined by
     * case-insensitive comparison to the enumerated names and descriptions. */
  constexpr int lookupValue(std::string_view t_name) {

    // Lookup by name first
    for (const auto& e : Enum::getFields()) {
      // if (ascii_to_lower_copy(t_name) == ascii_to_lower_copy(e.name)) {
      if (openstudio::ascii_istringEqual(t_name, e.name)) {
        return e.value;
      }
    }

    for (const auto& e : Enum::getFields()) {
      if (openstudio::ascii_istringEqual(t_name, e.description)) {
        return e.value;
      }
    }

    throw std::runtime_error(fmt::format("Unknown OpenStudio Enum Value = {} for Enum {}", t_name, Enum::enumName()));
  }

  /** Returns t_value if it is in the domain. Otherwise throws
     * std::runtime_error. */
  constexpr int lookupValue(int t_value) {
    for (const auto& e : Enum::getFields()) {
      if (e.value == t_value) {
        return e.value;
      }
    }
    throw std::runtime_error(fmt::format("Unknown OpenStudio Enum Value = {} for Enum {}", t_value, Enum::enumName()));
  }

 protected:
  int m_value;
};

#endif  // UTILITIES_CORE_ENUMBASE_HPP
