/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_IDD_COMMENTREGEX_HPP
#define UTILITIES_IDD_COMMENTREGEX_HPP

#include "../UtilitiesAPI.hpp"
#include "../core/StaticInitializer.hpp"

#include <boost/regex.hpp>

/** \file CommentRegex.hpp
 *
 *  Defines regular expressions used to handle Idd and Idf whitespace and comment blocks. */

namespace openstudio {
namespace commentRegex {

  /** Returns regex that matches a single line that only contains whitespace characters. The
   *  regex does not contain any sub-expressions. */
  UTILITIES_API const boost::regex& whitespaceOnlyLine();

  /** Returns regex that matches a single line that only contains whitespace and an optional
   *  Idd/Idf comment. There is one sub-expression:
   *
   *  matches[1] = comment text with '!' stripped off */
  UTILITIES_API const boost::regex& commentWhitespaceOnlyLine();

  /** Returns regex that matches a single line that only contains whitespace an an optional
   *  IdfEditor-style Idd/Idf comment. There is one sub-expression:
   *
   *  matches[1] = comment text with '!-' stripped off */
  UTILITIES_API const boost::regex& editorCommentWhitespaceOnlyLine();

  /** Returns regex that matches 0 or more lines that only contain whitespace characters. The
   *  regex does not contain any sub-expressions. */
  UTILITIES_API const boost::regex& whitespaceOnlyBlock();

  /** Returns regex that matches a block of lines that only contains whitespace and Idd/Idf
   *  comments. The regex does not contain any sub-expressions. */
  UTILITIES_API const boost::regex& commentWhitespaceOnlyBlock();

  /** Returns regex that matches a block of lines that only contains whitespace and IdfEditor
   *  style Idd/Idf comments. The regex does not contain any sub-expressions. */
  UTILITIES_API const boost::regex& editorCommentWhitespaceOnlyBlock();

}  // namespace commentRegex
}  // namespace openstudio

#endif  // UTILITIES_IDD_COMMENTREGEX_HPP
