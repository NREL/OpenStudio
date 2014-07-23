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

#include "String.hpp"

#include <sstream>
#include <iomanip>
#include <limits>

namespace openstudio {

/** char* to std::string. */
std::string toString(const char* s)
{ 
  return std::string(s);
}

/** string to std::string. */
std::string toString(const std::string& s)
{ 
  return s;
}

/** wchar_t* to std::string. */
std::string toString(const wchar_t* w)
{
  return toString(std::wstring(w));
}

/** wstring to std::string. */
std::string toString(const std::wstring& w)
{
  return toString(toQString(w));
}

/** QString to UTF-8 encoded std::string. */
std::string toString(const QString& q) 
{
  const QByteArray& qb = q.toUtf8();
  return std::string(qb.data());
}

std::string toString(double v) {
  std::stringstream ss;
  ss << std::setprecision(std::numeric_limits<double>::digits10) << v;
  return ss.str();
}

std::string toString(std::istream& s) {
  // istream -> string code from
  // http://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
  std::string contents;
  s.seekg(0, std::ios::end);
  contents.resize(s.tellg());
  s.seekg(0, std::ios::beg);
  s.read(&contents[0], contents.size());
  return contents;
}

/** QString to wstring. */
std::wstring toWString(const QString& q)
{
#if defined(Q_OS_WIN)
  static_assert(sizeof(wchar_t) == sizeof(unsigned short), "Wide characters must have the same size as unsigned shorts");
  std::wstring w(reinterpret_cast<const wchar_t *>(q.utf16()), q.length());
  return w;
#else
  std::wstring w = q.toStdWString();
  return w;
#endif
}

/** UTF-8 encoded std::string to QString. */
QString toQString(const std::string& s) 
{
  return QString::fromUtf8(s.c_str());
}

/** wstring to QString. */
QString toQString(const std::wstring& w)
{
#if defined(Q_OS_WIN)
  static_assert(sizeof(wchar_t) == sizeof(unsigned short), "Wide characters must have the same size as unsigned shorts");
  return QString::fromUtf16(reinterpret_cast<const unsigned short *>(w.data()), w.length());
#else
  return QString::fromStdWString(w);
#endif

}

namespace detail{
  StringMetaTypeInitializer::StringMetaTypeInitializer()
  {
    // int type;
    /* type =*/ qRegisterMetaType<std::string>("std::string");
    /* type =*/ qRegisterMetaType<std::vector<std::string> >("std::vector<std::string>");
  }
  static StringMetaTypeInitializer __stringMetaTypeInitializer__;
}

} // openstudio

// allow string to be written to QTextStream
QTextStream& operator<<(QTextStream& qts, const std::string& s)
{
  qts << openstudio::toQString(s);
  return qts;
}
