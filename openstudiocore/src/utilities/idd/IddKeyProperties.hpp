/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
*  All rights reserved.
*  
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*  
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*  
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#ifndef UTILITIES_IDD_IDDKEYPROPERTIES_HPP
#define UTILITIES_IDD_IDDKEYPROPERTIES_HPP

#include "../UtilitiesAPI.hpp"

#include <string>

namespace openstudio{

/** IddKeyProperties is an all public class describing the properties
 *   of an IddKey object. */
struct UTILITIES_API IddKeyProperties {
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

} // openstudio

#endif // UTILITIES_IDD_IDDKEYPROPERTIES_HPP
