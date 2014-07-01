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

#include "Checksum.hpp"

#include <ios>
#include <sstream>

#include <boost/crc.hpp> 
#include <boost/filesystem/fstream.hpp>

namespace openstudio {

  namespace detail {

    bool checksumIgnore(char c) 
    { 
      // ignore all whitespace
      //bool result = (c == ' ' || c == '\t' || c == '\n' ||  c == '\v'	|| c == '\f' || c == '\r');

      // ignore just line feed
      bool result = (c == '\r');

      return result;
    }
  }

  /// return 8 character hex checksum of string
  std::string checksum(const std::string& s)
  {
    std::stringstream ss(s);
    return checksum(ss);
  }

  /// return 8 character hex checksum of istream
  std::string checksum(std::istream& is)
  {
    boost::crc_32_type  crc;
    do{
      const std::streamsize n = 1024;
      char  buffer[n];
      is.read(buffer, n);
      std::streamsize readSize = is.gcount();
      //crc.process_bytes(buffer, readSize);

      size_t stringSize = static_cast<size_t>(readSize);
      std::string str(buffer, stringSize);
      str.erase( std::remove_if( str.begin(), str.end(), openstudio::detail::checksumIgnore ), str.end() );
      stringSize = str.size();
      
      crc.process_bytes(str.c_str(), stringSize);
    } while ( is );
    
    std::stringstream ss;
    ss << std::hex << std::uppercase << crc.checksum();
    std::string result = "00000000";
    std::string checksum = ss.str();
    result.replace(8-checksum.size(), checksum.size(), checksum);

    return result;
  }

  /// return 8 character hex checksum of file contents
  std::string checksum(const path& p)
  { 
    std::string result = "00000000";
    try{
      boost::filesystem::ifstream  ifs(p, std::ios_base::binary );
      if ( ifs ){
        result = checksum(ifs);
      }
    }catch(...){
    }
    return result;
  }

} // openstudio
