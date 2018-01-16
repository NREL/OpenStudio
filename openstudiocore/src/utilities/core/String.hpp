/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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
