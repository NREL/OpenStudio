/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#include <utilities/document/DocumentRegex.hpp>

namespace openstudio {

boost::regex titlePlaceholder() {
  const static boost::regex rgx("\\$\\{TITLE\\}");
  return rgx;
}

boost::regex authorPlaceholder() {
  const static boost::regex rgx("\\$\\{AUTHOR\\}");
  return rgx;
}

boost::regex superscript() {
  static boost::regex rgx("([\\d\\l\\u])(\\^(?:([\\d\\l\\u]+)|\\{(-?[\\d\\l\\u\\*\\+\\\\\\- ]+)\\}))");
  return rgx;
}

boost::regex subscript() {
  static boost::regex rgx("([\\d\\l\\u])(_(?:([\\d\\l\\u]+)|\\{(-?[\\d\\l\\u\\*\\+\\\\\\- ]+)\\}))");
  return rgx;
}

boost::regex underscore() {
  static boost::regex rgx("(\\\\_)");
  return rgx;
}

std::string formatText(const std::string& str,DocumentFormat fmt) {
  std::string wStr = formatSuperAndSubscripts(str,fmt);
  if (fmt == DocumentFormat::LaTeX) {
    wStr = boost::regex_replace(wStr,boost::regex("%"),"\\\\$&");
    wStr = boost::regex_replace(wStr,boost::regex("&"),"\\\\$&");
  }
  if (fmt == DocumentFormat::XHTML) {
    wStr = boost::regex_replace(wStr,boost::regex("$"),"<br/>");
  }
  return wStr;
}

std::string formatSuperAndSubscripts(const std::string& str,DocumentFormat fmt) {
  if (fmt == DocumentFormat::LaTeX) {
    std::string wStr(str);
    wStr = boost::regex_replace(wStr,superscript(),"$1\\\\ensuremath{$2}");
    wStr = boost::regex_replace(wStr,subscript(),"$1\\\\ensuremath{$2}");
    return wStr;
  }
  if (fmt == DocumentFormat::XHTML) {
    std::string wStr(str);
    wStr = boost::regex_replace(wStr,superscript(),"$1<sup>$3$4</sup>");
    wStr = boost::regex_replace(wStr,subscript(),"$1<sub>$3$4</sub>");
    wStr = boost::regex_replace(wStr,underscore(),"_");
    return wStr;
  }
  return str;
}

std::string formatUnitString(const std::string& str,DocumentFormat fmt) {
  std::string result = formatSuperAndSubscripts(str,fmt);
  boost::regex asterix("\\*");
  if (fmt == DocumentFormat::LaTeX) {
    result = boost::regex_replace(result,asterix,"\\\\ensuremath{\\\\cdot}");
  }
  if (fmt == DocumentFormat::XHTML) {
    result = boost::regex_replace(result,asterix,"&middot;");
  }
  return result;
}

std::string formatUnderscore(const std::string& str) {
  std::string wStr(str);
  wStr = boost::regex_replace(wStr,boost::regex("_"),"\\\\_");
  return wStr;
}

} // openstudio 
