/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_UNITS_IDDUNITSTRING_HPP
#define UTILITIES_UNITS_IDDUNITSTRING_HPP

#include "../UtilitiesAPI.hpp"

#include <string>

namespace openstudio {

class UTILITIES_API IddUnitString
{
 public:
  explicit IddUnitString(const std::string& s);
  ~IddUnitString() = default;

  std::string toStandardUnitString() const;

  std::string prettyString() const;

 private:
  IddUnitString();

  std::string m_original;
  std::string m_converted;
};

}  // namespace openstudio

#endif  // UTILITIES_UNITS_IDDUNITSTRING_HPP
