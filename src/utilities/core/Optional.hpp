/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_CORE_OPTIONAL_HPP
#define UTILITIES_CORE_OPTIONAL_HPP

#include "../UtilitiesAPI.hpp"

#include <boost/optional.hpp>

#include <string>
#include <ostream>

namespace openstudio {
// note: boost::optional<bool> is a bad idea, don't do it
using OptionalDouble = boost::optional<double>;
using OptionalUnsigned = boost::optional<unsigned int>;
using OptionalInt = boost::optional<int>;
using OptionalString = boost::optional<std::string>;

UTILITIES_API std::ostream& operator<<(std::ostream& os, const boost::optional<double>& x);
UTILITIES_API std::ostream& operator<<(std::ostream& os, const boost::optional<unsigned>& x);
UTILITIES_API std::ostream& operator<<(std::ostream& os, const boost::optional<int>& x);
UTILITIES_API std::ostream& operator<<(std::ostream& os, const boost::optional<std::string>& x);
}  // namespace openstudio

#endif  // UTILITIES_CORE_OPTIONAL_HPP
