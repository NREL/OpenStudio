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
