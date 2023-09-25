/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
