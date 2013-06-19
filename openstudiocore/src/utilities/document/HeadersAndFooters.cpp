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

#include <utilities/document/HeadersAndFooters.hpp>

#include <sstream>

namespace openstudio {

std::string basicLaTeXReportHeader() {
  std::stringstream ss;
  ss << "\\documentclass[12pt,letterpaper]{report}" << std::endl
     << std::endl
     << "\\usepackage{geometry}" << std::endl
     << "\\geometry{letterpaper,margin=1in}" << std::endl
     << std::endl
     << "\\title{${TITLE}}" << std::endl
     << "\\author{${AUTHOR}}" << std::endl
     << std::endl
     << "\\begin{document}" << std::endl
     << std::endl
     << "\\maketitle" << std::endl;
  return ss.str();
}

std::string basicLaTeXArticleHeader() {
  std::stringstream ss;
  ss << "\\documentclass[12pt,letterpaper]{article}" << std::endl
     << std::endl
     << "\\usepackage{geometry}" << std::endl
     << "\\geometry{letterpaper,margin=1in}" << std::endl
     << std::endl
     << "\\title{${TITLE}}" << std::endl
     << "\\author{${AUTHOR}}" << std::endl
     << std::endl
     << "\\begin{document}" << std::endl
     << std::endl
     << "\\maketitle" << std::endl;
  return ss.str();
}

std::string basicLaTeXFooter() {
  std::stringstream ss;
  ss << std::endl << "\\end{document}" << std::endl;
  return ss.str();
}

std::string basicXHTMLHeader() {
  std::stringstream ss;
  ss << "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\"" << std::endl
     << "\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">" << std::endl
     << std::endl
     << "<html>" << std::endl
     << "<head>" << std::endl
     << "<title>${TITLE}</title>" << std::endl
     << "<meta name=\"author\" content=\"${AUTHOR}\" />" << std::endl
     << "</head>" << std::endl
     << std::endl
     << "<body>" << std::endl;
  return ss.str();
}

std::string cssXHTMLHeader(const std::string& cssPath) {
  std::stringstream ss;
  ss << "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\"" << std::endl
     << "\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">" << std::endl
     << std::endl
     << "<html>" << std::endl
     << "<head>" << std::endl
     << "<title>${TITLE}</title>" << std::endl
     << "<meta name=\"author\" content=\"${AUTHOR}\" />" << std::endl
     << "<link rel=\"stylesheet\" href=\"" << cssPath << "\" type=\"text/css\" />" << std::endl
     << "</head>" << std::endl
     << std::endl
     << "<body>" << std::endl;
  return ss.str();
}

std::string basicXHTMLFooter() {
  std::stringstream ss;
  ss << std::endl
     << "</body>" << std::endl
     << "</html>" << std::endl;
  return ss.str();
}

} // openstudio
