/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "Utilities.hpp"

namespace openstudio {
  /** QString to UTF-8 encoded std::string. */
  std::string toString(const QString& q)
  {
    const QByteArray& qb = q.toUtf8();
    return std::string(qb.data());
  }



  /** QString to wstring. */
  std::wstring toWString(const QString& q)
  {
#if (defined (_WIN32) || defined (_WIN64))
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
#if (defined (_WIN32) || defined (_WIN64))
    static_assert(sizeof(wchar_t) == sizeof(unsigned short), "Wide characters must have the same size as unsigned shorts");
    return QString::fromUtf16(reinterpret_cast<const unsigned short *>(w.data()), w.length());
#else
    return QString::fromStdWString(w);
#endif

  }

  UUID toUUID(const QString &str)
  {
    return toUUID(toString(str));
  }

  QString toQString(const UUID& uuid)
  {
    return toQString(toString(uuid));
  }

  /** path to QString. */
  QString toQString(const path& p)
  {
#if (defined (_WIN32) || defined (_WIN64))
    return QString::fromStdWString(p.generic_wstring());
#endif
    return QString::fromUtf8(p.generic_string().c_str());
  }

  /** QString to path*/
  path toPath(const QString& q)
  {
#if (defined (_WIN32) || defined (_WIN64))
    return path(q.toStdWString());
#endif

    return path(q.toStdString());
  }


}