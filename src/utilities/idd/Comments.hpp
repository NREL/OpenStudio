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

#ifndef UTILITIES_IDD_COMMENTS_HPP
#define UTILITIES_IDD_COMMENTS_HPP

#include "../UtilitiesAPI.hpp"
#include <string>

/** \file Comments.hpp
 *
 *  Contains helper functions for processing Idd and Idf comments. */

namespace openstudio {

namespace detail {
  std::string makeCommentHelper(const std::string& str, const std::string& linePrefix, int indentSize);
}

/** Appends '! ' to each line of str, if necessary. No changes are made if each line with
 *  text is already prefixed with a '!'. If modifications are necessary,
 *  \li blank lines before and after all text blocks are retained as blank lines
 *  \li blank lines that fall between two text blocks are prefixed with '!' so that the
 *      returned string contains at most one comment block
 *  \li we boost::trim_left lines of text when that operation does not change the length of
 *      the string by more than one character; otherwise, we preserve the original
 *      indentation */
UTILITIES_API std::string makeComment(const std::string& str);

/** Appends '!- ' to each line of str, if necessary. No changes are made if each line with
 *  text is already prefixed with a '!'. If modifications are necessary,
 *  \li blank lines before and after all text blocks are retained as blank lines
 *  \li blank lines that fall between two text blocks are prefixed with '!' so that the
 *      returned string contains at most one comment block
 *  \li we boost::trim_left lines of text when that operation does not change the length of
 *      the string by more than one character; otherwise, we preserve the original
 *      indentation */
UTILITIES_API std::string makeIdfEditorComment(const std::string& str);

}  // namespace openstudio

#endif  // UTILITIES_IDD_COMMENTS_HPP
