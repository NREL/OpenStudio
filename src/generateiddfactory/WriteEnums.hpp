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

#ifndef GENERATEIDDFACTORY_WRITEENUMS_HPP
#define GENERATEIDDFACTORY_WRITEENUMS_HPP

#include <iostream>
#include <iomanip>
#include <cmath>
#include <sstream>
#include <exception>

namespace openstudio {

template <typename Container>
void writeDomain(std::ostream& t_os, const std::string& t_name, const Container& t_values, bool in_class) {

  std::string spacing = "";
  if (in_class) {
    spacing = "  ";
    t_os << spacing << "enum domain {" << '\n';
  } else {
    t_os << "enum " << t_name << "::domain : int {" << '\n';
  }

  for (const auto& val : t_values) {
    t_os << spacing << "  " << val.first << "," << '\n';
  }
  t_os << spacing << "};" << '\n' << '\n';
}

template <typename Container>
void writeMembers(std::ostream& t_os, const std::string& t_name, const Container& t_values, bool in_class) {

  if (in_class) {
    // We write the data as members

    t_os << "  static constexpr size_t num_fields = " << t_values.size() << ";" << '\n'
         << '\n'
         << "  static constexpr std::array<EnumField, num_fields> fields = {{" << '\n';

    for (const auto& val : t_values) {
      t_os << "    {" << t_name << "::" << val.first << ", \"" << val.first << "\", \"" << val.second << "\"}," << '\n';
    }
    t_os << "  }};" << '\n';

    t_os << '\n' << "  static constexpr std::array<int, num_fields> values = {\n";
    for (const auto& val : t_values) {
      t_os << "    " << t_name << "::" << val.first << ",\n";
    }

    t_os << "  };" << '\n'
         << '\n'
         << "  static constexpr openstudio::span<EnumField> getFields() {" << '\n'
         << "    return openstudio::span<EnumField>(fields);" << '\n'
         << "  };" << '\n'
         << '\n'
         << "  static constexpr openstudio::span<int> getDomainValues() {" << '\n'
         << "    return openstudio::span<int>(values);" << '\n'
         << "  };" << '\n'
         << '\n';

  } else {
    t_os << "static constexpr std::array<EnumField, " << t_values.size() << "> fields_" << t_name << " = {{" << '\n';
    for (const auto& val : t_values) {
      t_os << "  {" << t_name << "::" << val.first << ", \"" << val.first << "\", \"" << val.second << "\"}," << '\n';
    }
    t_os << "}};" << '\n';

    t_os << "constexpr openstudio::span<EnumField> " << t_name << "::getFields() {" << '\n'
         << "  return openstudio::span<EnumField>(fields_" << t_name << ");" << '\n'
         << "};" << '\n'
         << '\n';

    t_os << "static constexpr std::array<int, " << t_values.size() << "> values_" << t_name << " = {{" << '\n';
    for (const auto& val : t_values) {
      t_os << "  " << t_name << "::" << val.first << ",\n";
    }
    t_os << "}};" << '\n';

    t_os << "constexpr openstudio::span<EnumField> " << t_name << "::getDomainValues() {" << '\n'
         << "  return openstudio::span<EnumField>(values_" << t_name << ");" << '\n'
         << "};" << '\n'
         << '\n';
  }
}

template <typename Container>
void writeEnumFast(std::ostream& t_os, const std::string& t_name, const Container& t_values) {
  t_os << "#ifdef SWIG " << '\n'
       << "  OPENSTUDIO_ENUM(" << t_name << ", )" << '\n'
       << "#else" << '\n'
       << "class " << t_name << " : public ::EnumBase<" << t_name << "> {" << '\n'
       << " public: " << '\n';

  writeDomain(t_os, t_name, t_values, true);

  t_os << "  constexpr " << t_name << "()" << '\n'
       << "   : EnumBase<" << t_name << ">(" << t_values.front().first << ") {} " << '\n'
       << '\n'
       << "  constexpr " << t_name << "(const std::string_view t_name) " << '\n'
       << "   : EnumBase<" << t_name << ">(t_name) {} " << '\n'
       << '\n'
       << "  constexpr " << t_name << "(int t_value) " << '\n'
       << "   : EnumBase<" << t_name << ">(t_value) {} " << '\n'
       << '\n'
       << "  constexpr static std::string_view enumName() {" << '\n'
       << "    return \"" << t_name << "\";" << '\n'
       << "  }" << '\n'
       << '\n'
       << "  constexpr domain value() const {" << '\n'
       << "    return static_cast<domain>(EnumBase<" << t_name << ">::value());" << '\n'
       << "  }" << '\n'
       << '\n'
       << " private:" << '\n'
       << "  friend class EnumBase<" << t_name << ">;" << '\n'
       << "\n";

  writeMembers(t_os, t_name, t_values, true);

  t_os << '\n'
       << "};" << '\n'
       << '\n'
       << '\n'
       << "inline std::ostream &operator<<(std::ostream &os, const " << t_name << " &e) {" << '\n'
       << "  return os << e.valueName() << \"(\" << e.value() << \")\";" << '\n'
       << "}" << '\n'
       << '\n'
       << "typedef boost::optional<" << t_name << "> Optional" << t_name << " ;" << '\n'
       << '\n'
       << "#endif";
}
template <typename Container>
void writeEnum(std::ostream& t_os, const std::string& t_name, const Container& t_values) {
  // write IddObjectType enum. is very large, so split into 7 groups.
  auto groupSize = static_cast<unsigned>(std::ceil(static_cast<double>(t_values.size()) / 7.0));
  unsigned n = 0;  // number of objects written so far--will start with Catchall and UserCustom
  t_os << "OPENSTUDIO_ENUM7( " << t_name << " ," << '\n';

  for (const auto& val : t_values) {
    if (n > 0 && (n % groupSize) == 0) {
      t_os << "  ," << '\n';
    }

    // writes the enum value (name and description)
    t_os << "  ((" << val.first << ")";

    if (!val.second.empty()) {
      t_os << "(" << val.second << ")";
    }

    t_os << ")" << '\n';
    ++n;
  }

  t_os << ");" << '\n';
}

}  // namespace openstudio

#endif  // GENERATEIDDFACTORY_WRITEENUMS_HPP
