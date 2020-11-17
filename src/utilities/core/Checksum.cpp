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

#include "Checksum.hpp"

#include <sstream>

#include <boost/crc.hpp>

namespace openstudio {

namespace detail {

  bool checksumIgnore(char c) {
    // ignore all whitespace
    //bool result = (c == ' ' || c == '\t' || c == '\n' ||  c == '\v' || c == '\f' || c == '\r');

    // ignore just line feed
    bool result = (c == '\r');

    return result;
  }
}  // namespace detail

/// return 8 character hex checksum of string
std::string checksum(const std::string& s) {
  std::stringstream ss(s);
  return checksum(ss);
}

/// return 8 character hex checksum of istream
std::string checksum(std::istream& is) {
  boost::crc_32_type crc;
  do {
    const std::streamsize n = 1024;
    char buffer[n];
    is.read(buffer, n);
    std::streamsize readSize = is.gcount();
    //crc.process_bytes(buffer, readSize);

    size_t stringSize = static_cast<size_t>(readSize);
    std::string str(buffer, stringSize);
    str.erase(std::remove_if(str.begin(), str.end(), openstudio::detail::checksumIgnore), str.end());
    stringSize = str.size();

    crc.process_bytes(str.c_str(), stringSize);
  } while (is);

  std::stringstream ss;
  ss << std::hex << std::uppercase << crc.checksum();
  std::string result = "00000000";
  std::string checksum = ss.str();
  result.replace(8 - checksum.size(), checksum.size(), checksum);

  return result;
}

/// return 8 character hex checksum of file contents
std::string checksum(const path& p) {
  std::string result = "00000000";
  try {
    openstudio::filesystem::ifstream ifs(p, std::ios_base::binary);
    if (ifs) {
      result = checksum(ifs);
    }
  } catch (...) {
  }
  return result;
}

int crc16(const char* ptr, int count) {
  // Simulate CRC-CCITT
  boost::crc_basic<16> crc_ccitt1(0x1021, 0xFFFF, 0, false, false);
  crc_ccitt1.process_bytes(ptr, count);
  int result = crc_ccitt1.checksum();
  return result;
}

}  // namespace openstudio
