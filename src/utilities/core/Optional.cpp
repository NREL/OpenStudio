/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "Optional.hpp"

namespace openstudio {

std::ostream& operator<<(std::ostream& os, const boost::optional<double>& x) {
  if (x) {
    os << x.get();
  }
  return os;
}

std::ostream& operator<<(std::ostream& os, const boost::optional<unsigned>& x) {
  if (x) {
    os << x.get();
  }
  return os;
}

std::ostream& operator<<(std::ostream& os, const boost::optional<int>& x) {
  if (x) {
    os << x.get();
  }
  return os;
}

std::ostream& operator<<(std::ostream& os, const boost::optional<std::string>& x) {
  if (x) {
    os << x.get();
  }
  return os;
}

}  // namespace openstudio
