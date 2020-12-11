/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "IddObjectProperties.hpp"

#include <sstream>

namespace openstudio {

/// default constructor
IddObjectProperties::IddObjectProperties()
  : unique(false),
    required(false),
    obsolete(false),
    hasURL(false),
    extensible(false),
    numExtensible(0),
    numExtensibleGroupsRequired(0),
    minFields(0) {}

// ETH@20100329 Is the purpose to identify exactly equal properties, or objects that behave
// the same? If the latter, might ignore memo, and even numExtensibleGroupsRequired, and
// minFields. Keeping it simple for now.
/// equality operator
bool IddObjectProperties::operator==(const IddObjectProperties& other) const {

  return ((this == &other)
          || ((memo == other.memo) && (unique == other.unique) && (required == other.required) && (obsolete == other.obsolete)
              && (hasURL == other.hasURL) && (extensible == other.extensible) && (numExtensible == other.numExtensible)
              && (numExtensibleGroupsRequired == other.numExtensibleGroupsRequired) && (format == other.format) && (minFields == other.minFields)
              && (maxFields == other.maxFields)));
}

bool IddObjectProperties::operator!=(const IddObjectProperties& other) const {
  return !(*this == other);
}

/// print
std::ostream& IddObjectProperties::print(std::ostream& os) const {
  if (!memo.empty()) {
    std::stringstream ss(memo);
    std::string line;
    while (!ss.eof()) {
      std::getline(ss, line);
      os << "       \\memo " << line << std::endl;
    }
  }

  if (unique) {
    os << "       \\unique-object" << std::endl;
  }

  if (required) {
    os << "       \\required-object" << std::endl;
  }

  if (obsolete) {
    os << "       \\obsolete" << std::endl;
  }

  if (hasURL) {
    os << "       \\url-object" << std::endl;
  }
  if (extensible) {
    os << "       \\extensible:" << numExtensible << std::endl;
  }

  if (!format.empty()) {
    os << "       \\format " << format << std::endl;
  }

  if (minFields > 0) {
    os << "       \\min-fields " << minFields << std::endl;
  }

  if (maxFields) {
    os << "       \\max-fields " << *maxFields << std::endl;
  }

  return os;
}

}  // namespace openstudio
