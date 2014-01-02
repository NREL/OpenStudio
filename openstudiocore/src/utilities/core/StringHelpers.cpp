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

#include <utilities/core/StringHelpers.hpp>

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

namespace openstudio {

std::string toCamelCase(const std::string& s) {
  std::string result = boost::regex_replace(s,boost::regex("[^a-zA-Z0-9]")," ");
  result = boost::regex_replace(result,boost::regex(" ([a-z])"),"\\u$1");
  result = boost::regex_replace(result,boost::regex(" "),"");
  return result;
}

std::string toUpperCamelCase(const std::string& s) {
  std::string result = toCamelCase(s);
  result = boost::regex_replace(result,boost::regex("^([a-z])"),"\\u$1");
  return result;
}

std::string toLowerCamelCase(const std::string& s) {
  std::string result = toCamelCase(s);
  result = boost::regex_replace(result,boost::regex("^([A-Z])"),"\\l$1");
  return result;
}

// ETH@20110614 Implementation copied from IddFileFactoryData.cpp for use in Ruby script.
std::string convertIddName(const std::string& s) {
  std::string result(s);
  boost::trim(result);
  result = boost::regex_replace(result,boost::regex("^100 ?%"),"All");
  result = boost::regex_replace(result,boost::regex("[ \\-]+"),"\\u");
  result = boost::regex_replace(result,boost::regex("[:/,\\(\\)%]+"),"_");
  result = boost::regex_replace(result,boost::regex("[\\.\\?\\]\\[]"),"");
  result = boost::regex_replace(result,boost::regex("="),"_EQUAL_");
  result = boost::regex_replace(result,boost::regex("\\*\\*"),"_POW_");
  result = boost::regex_replace(result,boost::regex("\\+"),"_PLUS_");
  result = boost::regex_replace(result,boost::regex("\\*"),"_TIMES_");
  return result;
}

std::string iddObjectNameToIdfObjectName(const std::string& s) {
  std::string result(s);
  boost::trim(result);
  result = boost::regex_replace(result,boost::regex("OS:"),"");
  result = boost::regex_replace(result,boost::regex(":")," ");
  result = boost::regex_replace(result,boost::regex("([a-z])([A-Z])"),"$1 $2");
  boost::trim(result);
  return result;
}

} // openstudio
