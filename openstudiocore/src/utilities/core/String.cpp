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

#include "String.hpp"

#include "Logger.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/numeric/conversion/cast.hpp>

#include <sstream>
#include <iomanip>
#include <limits>
#include <cmath>
#include <cfloat>

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

  std::string result;

  // Return Infinity or NaN as strings, getDouble in IdfObject will fail to convert to double which will cause setDouble to fail
  if (std::isinf(v)) {
    if (v < 0){
      result = "-Infinity";
    } else{
      result = "Infinity";
    }
  } else if (std::isnan(v)) {
    result = "NaN";
  } else {

    std::stringstream ss;
    ss << std::setprecision(std::numeric_limits<double>::digits10) << v;
    result = ss.str();

  }

  return result;
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
