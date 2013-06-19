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

#ifndef UTILITIES_DOCUMENT_DOCUMENTREGEX_HPP
#define UTILITIES_DOCUMENT_DOCUMENTREGEX_HPP

#include <utilities/UtilitiesAPI.hpp>
#include <utilities/document/DocumentElement.hpp>

#include <boost/regex.hpp>

namespace openstudio {

/** Defines regex used to place a document's title in a header string. */
UTILITIES_API boost::regex titlePlaceholder();

/** Defines regex used to place a document's authors in a header string. */
UTILITIES_API boost::regex authorPlaceholder();

UTILITIES_API boost::regex superscript();

UTILITIES_API boost::regex subscript();

UTILITIES_API std::string formatText(const std::string& str,DocumentFormat fmt);

/** Ensures that underscores will remain as underscores (not changed to subscripts). */
UTILITIES_API std::string formatUnderscore(const std::string& str);

UTILITIES_API std::string formatSuperAndSubscripts(const std::string& str,DocumentFormat fmt);

/** Applies formatSuperAndSubscripts, and replaces '*' with small dot. Does nothing for fmt ==
 *  DocumentFormat::COUT. */
UTILITIES_API std::string formatUnitString(const std::string& str,DocumentFormat fmt);


  namespace detail {

    struct DocumentRegexInitializer : StaticInitializer<DocumentRegexInitializer>
    {
      static void initialize()
      {
        titlePlaceholder();
        authorPlaceholder();
        superscript();
        subscript();
      }
    };

    struct MakeSureDocumentRegexInitializerIsInitialized
    {
      MakeSureDocumentRegexInitializerIsInitialized()
      {
      }

      DocumentRegexInitializer m_i;
    };
  }


} // openstudio

#endif // UTILITIES_DOCUMENT_DOCUMENTREGEX_HPP
