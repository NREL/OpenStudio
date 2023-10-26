/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
      os << "       \\memo " << line << '\n';
    }
  }

  if (unique) {
    os << "       \\unique-object" << '\n';
  }

  if (required) {
    os << "       \\required-object" << '\n';
  }

  if (obsolete) {
    os << "       \\obsolete" << '\n';
  }

  if (hasURL) {
    os << "       \\url-object" << '\n';
  }
  if (extensible) {
    os << "       \\extensible:" << numExtensible << '\n';
  }

  if (!format.empty()) {
    os << "       \\format " << format << '\n';
  }

  if (minFields > 0) {
    os << "       \\min-fields " << minFields << '\n';
  }

  if (maxFields) {
    os << "       \\max-fields " << *maxFields << '\n';
  }

  return os;
}

}  // namespace openstudio
