/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_CORE_CHECKSUM_HPP
#define UTILITIES_CORE_CHECKSUM_HPP

#include "Path.hpp"
#include "../UtilitiesAPI.hpp"

#include <string>
#include <ostream>

namespace openstudio {

/// return 8 character hex checksum of string. Pass by copy since we strip the \r
UTILITIES_API std::string checksum(std::string s);

/// return 8 character hex checksum of istream
UTILITIES_API std::string checksum(std::istream& is);

/// return 8 character hex checksum of file contents
UTILITIES_API std::string checksum(const path& p);

/// returns the CRC-16 checksum of the first len bytes of data.  Replaces Qt implementation qChecksum.
UTILITIES_API int crc16(const char* ptr, int count);

}  // namespace openstudio

#endif  // UTILITIES_CORE_CHECKSUM_HPP
