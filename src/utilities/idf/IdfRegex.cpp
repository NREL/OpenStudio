/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "IdfRegex.hpp"

namespace openstudio {
namespace idfRegex {

  // todo, I have capitalized all these functions as they are static, is that right
  const std::string& newLinestring() {
    static const std::string result("\n");
    return result;
  }

  // Match an optional new line
  // matches[1], before new line and new line
  // matches[2], after new line
  const std::string& optionalNewLinestring() {
    static const std::string result("([^\\n]*[\\n]?)(.*)");
    return result;
  }

  // Match comment only line
  // matches[1], the comment
  // matches[2], after new line
  const boost::regex& commentOnlyLine() {
    static const boost::regex result("^[\\s\\t]*[!]([^\\n]*)[\\n]?(.*)");
    return result;
  }

  // Match content and comment
  // matches[1], the content (may include multiple fields)
  // matches[2], the comment
  const boost::regex& contentAndCommentLine() {
    static const boost::regex result("^([^!,;]*)[,;]?[\\s\\t]*[!]?(.*)");  //("^((?:[^!,;]*[,;])*)[\\s\\t]*[!]?(.*)");
    return result;
  }

  // Match object end in the idf
  const boost::regex& objectEnd() {
    static const boost::regex result("^[^!]*?[;].*");
    return result;
  }

  // Match line with an object type and name
  // matches[1], before first separator
  // matches[2], after first separator and before second separator
  const boost::regex& objectTypeAndName() {
    static const boost::regex result("^([^!]*?)[,;]([^!]*?)[,;]");
    return result;
  }

  // Match line with either a ',' or a ';' that are not preceded by '!'
  // matches[1], before separator
  // matches[2], after separator and before new line
  // matches[3], after new line
  const boost::regex& line() {
    static const boost::regex result("^([^!]*?)[,;]" + optionalNewLinestring());
    return result;
  }

  // Match an autosize field
  const boost::regex& autosize() {
    static const boost::regex result("autosize", boost::regex_constants::icase);
    return result;
  }

  // Match an autocalculate field
  const boost::regex& autocalculate() {
    static const boost::regex result("autocalculate", boost::regex_constants::icase);
    return result;
  }

  // Match line with '##def' preceded only by whitespace
  // matches[1], the imf section name
  const boost::regex& imfSection() {
    static const boost::regex result("^[\\s\\t]*##[\\s\\t]*def[\\s\\t]*(.*?)\\[(.*?)\\]");
    return result;
  }

  const boost::regex& imfSectionEnd() {
    static const boost::regex result("^[\\s\\t]*##[\\s\\t]*enddef[\\s\\t]*");
    return result;
  }

}  // namespace idfRegex
}  // namespace openstudio
