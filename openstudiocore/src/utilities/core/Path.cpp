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

#include "Path.hpp"
#include "String.hpp"

#include <QDir>

namespace openstudio {

#ifdef Q_OS_WIN

#include <windows.h>

QString longPathName(const QString& path)
{
  if (path.isEmpty())
    return QString();
  QString maybeShort = QDir::toNativeSeparators(path);
  QByteArray shortName = maybeShort.toLocal8Bit();
  char longPath[MAX_PATH];
  int err = GetLongPathName(shortName.constData(), longPath, MAX_PATH);
  (void)err;
  return QDir::fromNativeSeparators(QString::fromLocal8Bit(longPath));
}

#else

QString longPathName(const QString& path)
{
  return path;
}

#endif


// allow path to be written to cout on Windows
std::ostream& operator<<(std::ostream& os, const path& p)
{
  os << toString(p);
  return os;
}

/** QString to path*/
path toPath(const QString& q)
{
  std::string s = toString(q);
  return path(s);
}

/** path to a temporary directory. */
path tempDir()
{
  return toPath(QDir::tempPath());
}

/** path to UTF-8 encoding. */
std::string toString(const path& p) 
{
  return p.generic_string();
}

/** path to QString. */
QString toQString(const path& p) 
{
  return toQString(toString(p));
}

/** UTF-8 encoded char* to path*/
path toPath(const char* s)
{
  return toPath(toString(s));
}

/** UTF-8 encoded std::string to path*/
path toPath(const std::string& s) 
{
  return toPath(toQString(s));
}

} // openstudio

// allow path to be written to QTextStream
QTextStream& operator<<(QTextStream& qts, const openstudio::path& p)
{
  qts << openstudio::toQString(p);
  return qts;
}
