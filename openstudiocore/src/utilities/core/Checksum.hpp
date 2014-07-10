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

#ifndef UTILITIES_CORE_CHECKSUM_HPP
#define UTILITIES_CORE_CHECKSUM_HPP

#include "Path.hpp"
#include "../UtilitiesAPI.hpp"

#include <string>
#include <ostream>

namespace openstudio {

  /// return 8 character hex checksum of string
  UTILITIES_API std::string checksum(const std::string& s);

  /// return 8 character hex checksum of istream
  UTILITIES_API std::string checksum(std::istream& is);

  /// return 8 character hex checksum of file contents
  UTILITIES_API std::string checksum(const path& p);

} // openstudio


#endif // UTILITIES_CORE_CHECKSUM_HPP
