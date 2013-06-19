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

#ifndef UTILITIES_CORE_PATH_HPP
#define UTILITIES_CORE_PATH_HPP

#include <utilities/UtilitiesAPI.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <ostream>
#include <boost/optional.hpp>


// forward declarations
class QString;
class QTextStream;

namespace openstudio {

#ifdef _WINDOWS
  /** Type File path that varies by OS, templated on std::wstring on Windows.*/
  typedef boost::filesystem::wpath path;

  /** Type directory iterator that varies by OS to match path. */
  typedef boost::filesystem::wdirectory_iterator directory_iterator;

  /** UTF-16 encoded wchar* to path*/
  UTILITIES_API path toPath(const wchar_t* s);

  /** UTF-16 encoded std::wstring to path*/
  UTILITIES_API path toPath(const std::wstring& s);

  // allow wpath to be written to cout on Windows
  UTILITIES_API std::ostream& operator<<(std::ostream& os, const path& p);

#else
  /** Type File path that varies by OS, templated on std::string on POSIX.*/
  typedef boost::filesystem::path path;

  /** Type directory iterator that varies by OS to match path. */
  typedef boost::filesystem::directory_iterator directory_iterator;

#endif

/** path to a temporary directory. */
UTILITIES_API path tempDir();

/** path to std::string. */
UTILITIES_API std::string toString(const path& p);

/** path to QString. */
UTILITIES_API QString toQString(const path& p);

/** UTF-8 encoded char* to path*/
UTILITIES_API path toPath(const char* s);

/** UTF-8 encoded std::string to path*/
UTILITIES_API path toPath(const std::string& s);

/** QString to path*/
UTILITIES_API path toPath(const QString& q);

/** Optional path*/
typedef boost::optional<path> OptionalPath;

typedef std::pair<path,path> PathPair;

/// Return the fully expanded Windows path name ie "c:\Progra~1" into "C:\Program Files"
UTILITIES_API QString longPathName(const QString& path);

} // openstudio

// allow path to be written to QTextStream
QTextStream& operator<<(QTextStream& qts, const openstudio::path& p);


#endif // UTILITIES_CORE_PATH_HPP
