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

#include "IdfRegex.hpp"

namespace openstudio{
namespace idfRegex{

  // todo, I have capitalized all these functions as they are static, is that right
  const std::string &newLinestring(){
    const static std::string result("\n");
    return result;
  }

  // Match an optional new line
  // matches[1], before new line and new line
  // matches[2], after new line
  const std::string &optionalNewLinestring(){
    const static std::string result("([^\\n]*[\\n]?)(.*)");
    return result;
  }

  // Match comment only line
  // matches[1], the comment
  // matches[2], after new line
  const boost::regex &commentOnlyLine(){
    const static boost::regex result("^[\\s\\t]*[!]([^\\n]*)[\\n]?(.*)");
    return result; 
  }

  // Match content and comment 
  // matches[1], the content (may include multiple fields)
  // matches[2], the comment
  const boost::regex &contentAndCommentLine(){
    const static boost::regex result("^([^!,;]*)[,;]?[\\s\\t]*[!]?(.*)"); //("^((?:[^!,;]*[,;])*)[\\s\\t]*[!]?(.*)");
    return result; 
  }

  // Match object end in the idf
  const boost::regex &objectEnd(){
    const static boost::regex result("^[^!]*?[;].*");
    return result; 
  }

  // Match line with an object type and name
  // matches[1], before first separator
  // matches[2], after first separator and before second separator
  const boost::regex &objectTypeAndName(){
    const static boost::regex result("^([^!]*?)[,;]([^!]*?)[,;]");
    return result; 
  }

  // Match line with either a ',' or a ';' that are not preceded by '!'
  // matches[1], before separator
  // matches[2], after separator and before new line
  // matches[3], after new line
  const boost::regex &line(){
    const static boost::regex result("^([^!]*?)[,;]" + optionalNewLinestring());
    return result; 
  }

  // Match an autosize field
  const boost::regex &autosize(){
    const static boost::regex result("autosize", boost::regex_constants::icase);
    return result; 
  }

  // Match an autocalculate field
  const boost::regex &autocalculate(){
    const static boost::regex result("autocalculate", boost::regex_constants::icase);
    return result; 
  }

  // Match line with '##def' preceded only by whitespace
  // matches[1], the imf section name
  const boost::regex &imfSection(){
    const static boost::regex result("^[\\s\\t]*##[\\s\\t]*def[\\s\\t]*(.*?)\\[(.*?)\\]");
    return result; 
  }

  const boost::regex &imfSectionEnd() {
    const static boost::regex result("^[\\s\\t]*##[\\s\\t]*enddef[\\s\\t]*");
    return result;
  }

} // idfRegex
} // openstudio


