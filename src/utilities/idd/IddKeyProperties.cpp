/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "IddKeyProperties.hpp"

namespace openstudio {

IddKeyProperties::IddKeyProperties() = default;

IddKeyProperties::IddKeyProperties(const IddKeyProperties& other) = default;

bool IddKeyProperties::operator==(const IddKeyProperties& other) const {
  return ((this == &other) || (note == other.note));
}
bool IddKeyProperties::operator!=(const IddKeyProperties& other) const {
  return !(*this == other);
}

}  // namespace openstudio
