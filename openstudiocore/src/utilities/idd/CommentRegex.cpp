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

#include "CommentRegex.hpp"

namespace openstudio {
namespace commentRegex {

const boost::regex &whitespaceOnlyLine() {
  const static boost::regex result("^[\\h]*$");
  return result;
}

const boost::regex &commentWhitespaceOnlyLine() {
  const static boost::regex result("^[\\h]*(?:!([^\\n\\r\\v]*))?$");
  return result;
}

const boost::regex &editorCommentWhitespaceOnlyLine() {
  const static boost::regex result("^[\\h]*(?:!-([^\\n\\r\\v]*))?$");
  return result;
}

const boost::regex &whitespaceOnlyBlock() {
  const static boost::regex result("[\\s]*");
  return result;
}

const boost::regex &commentWhitespaceOnlyBlock() {
  const static boost::regex result("(?:^[\\h]*(?:![^\\v\\n]*)?[\\v\\n]?)*");
  return result;
}

const boost::regex &editorCommentWhitespaceOnlyBlock() {
  const static boost::regex result("(?:^[\\h]*(?:!-[^\\v\\n]*)?[\\v\\n]?)*");
  return result;
}

} // commentRegex
} // openstudio
