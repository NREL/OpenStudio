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

#ifndef UTILITIES_IDD_COMMENTS_HPP
#define UTILITIES_IDD_COMMENTS_HPP

#include "../UtilitiesAPI.hpp"
#include <string>

/** \file Comments.hpp 
 *  
 *  Contains helper functions for processing Idd and Idf comments. */

namespace openstudio {

namespace detail {
  std::string makeCommentHelper(const std::string& str,
                                const std::string& linePrefix,
                                int indentSize);
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

} // openstudio

#endif // UTILITIES_IDD_COMMENTS_HPP
