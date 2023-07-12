/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
