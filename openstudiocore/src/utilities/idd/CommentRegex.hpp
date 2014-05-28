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
  UTILITIES_API const boost::regex &whitespaceOnlyLine();

  /** Returns regex that matches a single line that only contains whitespace and an optional
   *  Idd/Idf comment. There is one sub-expression: 
   *
   *  matches[1] = comment text with '!' stripped off */
  UTILITIES_API const boost::regex &commentWhitespaceOnlyLine();

  /** Returns regex that matches a single line that only contains whitespace an an optional
   *  IdfEditor-style Idd/Idf comment. There is one sub-expression:
   *
   *  matches[1] = comment text with '!-' stripped off */
  UTILITIES_API const boost::regex &editorCommentWhitespaceOnlyLine();

  /** Returns regex that matches 0 or more lines that only contain whitespace characters. The
   *  regex does not contain any sub-expressions. */
  UTILITIES_API const boost::regex &whitespaceOnlyBlock();

  /** Returns regex that matches a block of lines that only contains whitespace and Idd/Idf 
   *  comments. The regex does not contain any sub-expressions. */
  UTILITIES_API const boost::regex &commentWhitespaceOnlyBlock();

  /** Returns regex that matches a block of lines that only contains whitespace and IdfEditor 
   *  style Idd/Idf comments. The regex does not contain any sub-expressions. */
  UTILITIES_API const boost::regex &editorCommentWhitespaceOnlyBlock();

  namespace detail {
    struct CommentRegexInitializer : StaticInitializer<CommentRegexInitializer>
    {
      static void initialize()
      {
        whitespaceOnlyLine();
        commentWhitespaceOnlyLine();
        editorCommentWhitespaceOnlyLine();
        whitespaceOnlyBlock();
        commentWhitespaceOnlyBlock();
        editorCommentWhitespaceOnlyBlock();
      }
    };

    struct MakeSureCommentRegexInitializerIsInitialized
    {
      MakeSureCommentRegexInitializerIsInitialized()
      {
      }

      CommentRegexInitializer m_i;
    };

  }


} // commentRegex
} // openstudio

#endif // UTILITIES_IDD_COMMENTREGEX_HPP
