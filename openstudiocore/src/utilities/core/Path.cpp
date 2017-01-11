/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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
