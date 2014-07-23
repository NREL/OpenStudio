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

#ifndef UTILITIES_CORE_STRING_HPP
#define UTILITIES_CORE_STRING_HPP

#include "../UtilitiesAPI.hpp"

#include <string>
#include <vector>

#include <QString>
#include <QTextStream>
#include <QMetaType>

/** \file String.hpp
 *  
 *  All strings are assumed to be UTF-8 encoded std::string.  Note that length of the std::string
 *  may therefore not match number of characters in the std::string. */

namespace openstudio {

  /** string to std::string. */
  UTILITIES_API std::string toString(const std::string& s);

  /** char* to std::string. */
  UTILITIES_API std::string toString(const char* s);

  /** wstring to std::string. */
  UTILITIES_API std::string toString(const std::wstring& w);

  /** wchar_t* to std::string. */
  UTILITIES_API std::string toString(const wchar_t* w);

  /** QString to UTF-8 encoded std::string. */
  UTILITIES_API std::string toString(const QString& q);

  /** Double to std::string at full precision. */
  UTILITIES_API std::string toString(double v);

  /** Load data in istream into string. */
  UTILITIES_API std::string toString(std::istream& s);

  /** QString to wstring. */
  UTILITIES_API std::wstring toWString(const QString& q);

  /** UTF-8 encoded std::string to QString. */
  UTILITIES_API QString toQString(const std::string& s);

  /** wstring to QString. */
  UTILITIES_API QString toQString(const std::wstring& w);

} // openstudio

// declare these types so we can use them as properties
Q_DECLARE_METATYPE(std::string);
Q_DECLARE_METATYPE(std::vector<std::string>);

// allow string to be written to QTextStream
UTILITIES_API QTextStream& operator<<(QTextStream& qts, const std::string& s);

namespace openstudio {
namespace detail {

  // register meta datatypes
  struct StringMetaTypeInitializer{
    StringMetaTypeInitializer();
  };
}
}

#endif // UTILITIES_CORE_STRING_HPP
