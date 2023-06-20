/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "Comments.hpp"
#include "CommentRegex.hpp"

#include <boost/algorithm/string.hpp>

#include <sstream>

namespace openstudio {

namespace detail {

  std::string makeCommentHelper(const std::string& str, const std::string& linePrefix, int indentSize) {
    // istream for parsing, ostream for composing comment
    std::stringstream ss(str);
    std::stringstream comment;

    // helper variables
    boost::smatch m;
    std::string line;
    bool first = true;
    bool commentStarted = false;
    unsigned blankLineQueue = 0;  // add blank lines in groups

    // parse and compose comment line-by-line
    while (std::getline(ss, line)) {
      // capture comment already marked as such
      if (boost::regex_match(line, m, commentRegex::editorCommentWhitespaceOnlyLine())) {
        line = std::string(m[1].first, m[1].second);
      }
      if (boost::regex_match(line, m, commentRegex::commentWhitespaceOnlyLine())) {
        line = std::string(m[1].first, m[1].second);
      }
      // handle blank lines separately
      if (boost::regex_match(line, commentRegex::whitespaceOnlyLine())) {
        if (commentStarted) {
          ++blankLineQueue;
        }  // may need to prepend with linePrefix
        else {
          // no comment yet--leave as pure blank line
          if (!first) {
            comment << '\n';
          }
        }
      } else {
        // line has non-whitespace characters. prepend each queued blank line with linePrefix
        for (unsigned i = 0; i < blankLineQueue; ++i) {
          comment << '\n' << linePrefix;
        }
        blankLineQueue = 0;

        // align lefthand edge if not indented by at least indentSize
        std::string temp = boost::trim_left_copy(line);
        if (temp.size() > line.size() - indentSize) {
          line = temp;
        }
        // go to next line if this is not the first line
        if (!first) {
          comment << '\n';
        }
        // write out comment line
        comment << linePrefix << line;
        commentStarted = true;
      }
      first = false;
    }

    // write out remaining blank lines w/o commenting them
    for (unsigned i = 0; i < blankLineQueue; ++i) {
      comment << '\n';
    }

    // append '\n' if str ends with \n or \r
    if (!str.empty()) {
      boost::regex re("[\\n\\r]");
      if (boost::regex_match(str.end() - 1, str.end(), re)) {
        comment << '\n';
      }
    }

    return comment.str();
  }

}  // namespace detail

std::string makeComment(const std::string& str) {

  // make sure each line starts with !
  boost::smatch m;
  if (boost::regex_match(str, m, commentRegex::commentWhitespaceOnlyBlock())) {
    // already does, return as-is
    return str;
  }

  // prepend '! ' to each line, as needed
  return detail::makeCommentHelper(str, "! ", 2);
}

std::string makeIdfEditorComment(const std::string& str) {
  // make sure each line starts with !-
  boost::smatch m;
  if (boost::regex_match(str, m, commentRegex::editorCommentWhitespaceOnlyBlock())) {
    // already does, return as-is
    return str;
  }

  // prepend '!- ' to each line, as needed
  return detail::makeCommentHelper(str, "!- ", 2);
}

}  // namespace openstudio
