/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "CommentRegex.hpp"

namespace openstudio {
namespace commentRegex {

  const boost::regex& whitespaceOnlyLine() {
    static const boost::regex result("^[\\h]*$");
    return result;
  }

  const boost::regex& commentWhitespaceOnlyLine() {
    static const boost::regex result("^[\\h]*(?:!([^\\n\\r\\v]*))?$");
    return result;
  }

  const boost::regex& editorCommentWhitespaceOnlyLine() {
    static const boost::regex result("^[\\h]*(?:!-([^\\n\\r\\v]*))?$");
    return result;
  }

  const boost::regex& whitespaceOnlyBlock() {
    static const boost::regex result("[\\s]*");
    return result;
  }

  const boost::regex& commentWhitespaceOnlyBlock() {
    static const boost::regex result("(?:^[\\h]*(?:![^\\v\\n]*)?[\\v\\n]?)*");
    return result;
  }

  const boost::regex& editorCommentWhitespaceOnlyBlock() {
    static const boost::regex result("(?:^[\\h]*(?:!-[^\\v\\n]*)?[\\v\\n]?)*");
    return result;
  }

}  // namespace commentRegex
}  // namespace openstudio
