/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_IDD_IDDKEYPROPERTIES_HPP
#define UTILITIES_IDD_IDDKEYPROPERTIES_HPP

#include "../UtilitiesAPI.hpp"

#include <string>

namespace openstudio {

/** IddKeyProperties is an all public class describing the properties
 *   of an IddKey object. */
struct UTILITIES_API IddKeyProperties
{
 public:
  /// constructor
  IddKeyProperties();

  /// copy constructor
  IddKeyProperties(const IddKeyProperties& other);

  /// equality operator
  bool operator==(const IddKeyProperties& other) const;
  bool operator!=(const IddKeyProperties& other) const;

  /// any notes
  std::string note;
};

}  // namespace openstudio

#endif  // UTILITIES_IDD_IDDKEYPROPERTIES_HPP
