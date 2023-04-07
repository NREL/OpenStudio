/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef UTILITIES_IDF_IDFREGEX_HPP
#define UTILITIES_IDF_IDFREGEX_HPP

#include "../UtilitiesAPI.hpp"
#include "../core/StaticInitializer.hpp"
#include <string>
#include <boost/regex.hpp>

namespace openstudio {
namespace idfRegex {

  // todo, I have capitalized all these functions as they are static, is that right
  UTILITIES_API const std::string& newLinestring();

  // Match an optional new line
  // matches[1], before new line and new line
  // matches[2], after new line
  UTILITIES_API const std::string& optionalNewLinestring();

  // Match comment only line
  // matches[1], the comment
  // matches[2], after new line
  UTILITIES_API const boost::regex& commentOnlyLine();

  // Match content and comment
  // matches[1], the content
  // matches[2], the comment
  UTILITIES_API const boost::regex& contentAndCommentLine();

  // Match object end in the idf
  UTILITIES_API const boost::regex& objectEnd();

  // Match line with an object type and name
  // matches[1], before first separator
  // matches[2], after first separator and before second separator
  UTILITIES_API const boost::regex& objectTypeAndName();

  // Match line with either a ',' or a ';' that are not preceded by '!'
  // matches[1], before separator
  // matches[2], after separator and before new line
  // matches[3], after new line
  UTILITIES_API const boost::regex& line();

  // Match an autosize field
  UTILITIES_API const boost::regex& autosize();

  // Match an autocalculate field
  UTILITIES_API const boost::regex& autocalculate();

  // Match line with '##def SomeSection[]' preceded only by whitespace
  // matches[1], the imf section name (e.g. 'SomeSection')
  // matches[2], inside the [] brackets
  UTILITIES_API const boost::regex& imfSection();

  // Match line with '##enddef' preceded and followed only by whitespace
  UTILITIES_API const boost::regex& imfSectionEnd();

}  // namespace idfRegex
}  // namespace openstudio

#endif  //UTILITIES_IDF_IDFREGEX_HPP
