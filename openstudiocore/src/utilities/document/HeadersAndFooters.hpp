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

#ifndef UTILITIES_DOCUMENT_HEADERSANDFOOTERS_HPP
#define UTILITIES_DOCUMENT_HEADERSANDFOOTERS_HPP

#include <utilities/UtilitiesAPI.hpp>
#include <string>

namespace openstudio {

UTILITIES_API std::string basicLaTeXReportHeader();

UTILITIES_API std::string basicLaTeXArticleHeader();

UTILITIES_API std::string basicLaTeXFooter();

UTILITIES_API std::string basicXHTMLHeader();

UTILITIES_API std::string cssXHTMLHeader(const std::string& cssPath);

UTILITIES_API std::string basicXHTMLFooter();

} // openstudio

#endif // UTILITIES_DOCUMENT_HEADERSANDFOOTERS_HPP
