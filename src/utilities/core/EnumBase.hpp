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

#ifndef UTILITIES_CORE_ENUMBASE_HPP
#define UTILITIES_CORE_ENUMBASE_HPP

#include "StaticInitializer.hpp"

#include <boost/preprocessor.hpp>
#include <boost/optional.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/case_conv.hpp>

#include <map>
#include <set>
#include <stdexcept>
#include <vector>

/** Base class for OPENSTUDIO_ENUMs. Comparison operators use the underlying (integer) value. */
template <typename Enum>
class EnumBase : public ::StaticInitializer<Enum>
{
 public:
  /** Default constructor. EnumBase default constructor never actually called; OPENSTUDIO_ENUM-
       *  generated classes have default constructors that set the value to the first item in the
       *  enumerated list. */
  EnumBase() = delete;

  /** Construct from t_value. Throws std::runtime_error if t_value is not in the domain. */
  EnumBase(int t_value) : m_value(lookupValue(t_value)) {}

  /** Construct from t_value. Throws std::runtime_error if t_value does not match a name or
       *  a description (case insensitive). */
  EnumBase(const std::string& t_value) : m_value(lookupValue(t_value)) {}

  /** Returns the name associated with t_value, if it exists. Otherwise, throws
       *  std::runtime_error. */
  static std::string valueName(int t_value) {
    const auto& m = getNames();
    auto itr = m.find(t_value);
    if (itr == m.end()) {
      throw std::runtime_error("Invalid domain for OpenStudio Enum " + Enum::enumName());
    }
    return itr->second;
  }

  /** Returns the description associated with t_value, if it exists. Otherwise, throws
       *  std::runtime_error. */
  static std::string valueDescription(int t_value) {
    const std::map<int, std::string>& m = getDescriptions();
    auto itr = m.find(t_value);
    if (itr == m.end()) {
      return valueName(t_value);
    }
    return itr->second;
  }

  /** Returns the set of all values in this enum's domain. */
  static const std::set<int>& getValues() {
    static std::set<int> values(buildValues());
    return values;
  }

  /** Returns this instance's current value (as an integer). */
  int value() const {
    return m_value;
  }

  /** Returns the name associated with this instance's current value. */
  std::string valueName() const {
    return valueName(m_value);
  }

  /** Returns the description associated with this instance's current value. */
  std::string valueDescription() const {
    return valueDescription(m_value);
  }

  /** Set this instance's value to t_value. Throws std::runtime_error if
       *  t_value is not in the domain. */
  void setValue(int t_value) {
    m_value = lookupValue(t_value);
  }

  bool operator==(const EnumBase<Enum>& other) const {
    return (m_value == other.m_value);
  }
  bool operator!=(const EnumBase<Enum>& other) const {
    return (m_value != other.m_value);
  }
  bool operator>(const EnumBase<Enum>& other) const {
    return (m_value > other.m_value);
  }
  bool operator>=(const EnumBase<Enum>& other) const {
    return (m_value >= other.m_value);
  }
  bool operator<(const EnumBase<Enum>& other) const {
    return (m_value < other.m_value);
  }
  bool operator<=(const EnumBase<Enum>& other) const {
    return (m_value <= other.m_value);
  }

  static void initialize() {
    getValues();
    getLookupMap();
    getNames();
    getDescriptions();
  }

  /** Returns the (integer) value associated with t_name, as determined by case-insensitive
       *  comparison to the enumerated names and descriptions. */
  int lookupValue(std::string t_name) {
    boost::algorithm::to_upper(t_name);
    const auto& names = getLookupMap();
    auto itr = names.find(t_name);
    if (itr != names.end()) {
      return itr->second;
    }
    throw std::runtime_error("Unknown OpenStudio Enum Value '" + t_name + "' for Enum " + Enum::enumName());
  }

  /** Returns t_value if it is in the domain. Otherwise throws std::runtime_error. */
  int lookupValue(int t_value) {
    const std::set<int>& values = getValues();
    if (values.count(t_value) != 0) {
      return t_value;
    } else {
      throw std::runtime_error("Unknown OpenStudio Enum Value = " + std::to_string(t_value) + " for Enum " + Enum::enumName());
    }
  }

  static const std::map<std::string, int>& getLookupMap() {
    static const std::map<std::string, int> m = buildLookupMap();
    return m;
  }

  static const std::map<int, std::string>& getNames() {
    static const std::map<int, std::string> names = buildStrings(false);
    return names;
  }

  static const std::map<int, std::string>& getDescriptions() {
    static const std::map<int, std::string> descriptions = buildStrings(true);
    return descriptions;
  }

 protected:
  int m_value;

 private:
  static std::map<int, std::string> buildStrings(bool d) {
    auto strings(Enum::buildStringVec(d));
    std::map<int, std::string> m;

    for (const auto& s : strings) {
      m[s.second] = s.first;
    }

    return m;
  }

  static std::map<std::string, int> buildLookupMap() {

    const auto& m = getNames();
    const auto& d = getDescriptions();

    std::map<std::string, int> retval;
    for (const auto& name : m) {
      retval[boost::algorithm::to_upper_copy(name.second)] = name.first;
    }

    for (const auto& description : d) {
      retval[boost::algorithm::to_upper_copy(description.second)] = description.first;
    }

    return retval;
  }

  static std::set<int> buildValues() {
    const auto& names = getNames();

    std::set<int> retvals;

    for (const auto& name : names) {
      retvals.insert(name.first);
    }

    return retvals;
  }
};

#endif  // UTILITIES_CORE_ENUMBASE_HPP
