/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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


