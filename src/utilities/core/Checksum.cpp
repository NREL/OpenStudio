/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "Checksum.hpp"

#include <sstream>

#include <boost/crc.hpp>
#include <fmt/format.h>

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
std::string checksum(std::string s) {
  s.erase(std::remove_if(s.begin(), s.end(), openstudio::detail::checksumIgnore), s.end());
  boost::crc_32_type crc;
  crc.process_bytes(s.data(), s.length());
  return fmt::format("{:0>8X}", crc.checksum());
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

    auto stringSize = static_cast<size_t>(readSize);
    std::string str(buffer, stringSize);
    str.erase(std::remove_if(str.begin(), str.end(), openstudio::detail::checksumIgnore), str.end());
    stringSize = str.size();

    crc.process_bytes(str.c_str(), stringSize);
  } while (is);

  return fmt::format("{:0>8X}", crc.checksum());
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
