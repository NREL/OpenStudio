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

#include "Comments.hpp"
#include "CommentRegex.hpp"

#include <boost/algorithm/string.hpp>

#include <sstream>

namespace openstudio {

namespace detail {

  std::string makeCommentHelper(const std::string& str,
                                const std::string& linePrefix,
                                int indentSize)
  {
    // istream for parsing, ostream for composing comment
    std::stringstream ss(str), comment;

    // helper variables
    boost::smatch m;
    std::string line;
    bool first = true;
    bool commentStarted = false;
    unsigned blankLineQueue = 0; // add blank lines in groups

    // parse and compose comment line-by-line
    while (std::getline(ss,line)) {
      // capture comment already marked as such
      if (boost::regex_match(line,m,commentRegex::editorCommentWhitespaceOnlyLine())) {
        line = std::string(m[1].first,m[1].second);
      }
      if (boost::regex_match(line,m,commentRegex::commentWhitespaceOnlyLine())) {
        line = std::string(m[1].first,m[1].second);
      }
      // handle blank lines separately
      if (boost::regex_match(line,commentRegex::whitespaceOnlyLine())) {
        if (commentStarted) { ++blankLineQueue; } // may need to prepend with linePrefix
        else {
          // no comment yet--leave as pure blank line
          if (!first) { comment << std::endl; }
        }
      }
      else {
        // line has non-whitespace characters. prepend each queued blank line with linePrefix
        for (unsigned i = 0; i < blankLineQueue; ++i) {
          comment << std::endl << linePrefix;
        }
        blankLineQueue = 0;

        // align lefthand edge if not indented by at least indentSize
        std::string temp = boost::trim_left_copy(line);
        if (temp.size() > line.size() - indentSize) { line = temp; }
        // go to next line if this is not the first line
        if (!first) { comment << std::endl; }
        // write out comment line
        comment << linePrefix << line;
        commentStarted = true;
      }
      first = false;
    }

    // write out remaining blank lines w/o commenting them
    for (unsigned i = 0; i < blankLineQueue; ++i) {
      comment << std::endl;
    }

    // append std::endl if str ends with \n or \r
    if (!str.empty()) {
      boost::regex re("[\\n\\r]");
      if (boost::regex_match(str.end()-1,str.end(),re)) {
        comment << std::endl;
      }
    }

    return comment.str();

  }

} // detail

std::string makeComment(const std::string& str) {

  // make sure each line starts with !
  boost::smatch m;
  if (boost::regex_match(str,m,commentRegex::commentWhitespaceOnlyBlock())) {
    // already does, return as-is
    return str;
  }

  // prepend '! ' to each line, as needed
  return detail::makeCommentHelper(str,"! ",2);
}

std::string makeIdfEditorComment(const std::string& str) {
  // make sure each line starts with !-
  boost::smatch m;
  if (boost::regex_match(str,m,commentRegex::editorCommentWhitespaceOnlyBlock())) {
    // already does, return as-is
    return str;
  }

  // prepend '!- ' to each line, as needed
  return detail::makeCommentHelper(str,"!- ",2);
}

} // openstudio
