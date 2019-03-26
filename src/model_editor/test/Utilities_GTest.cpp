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

#include <gtest/gtest.h>

#include <resources.hxx>

#include "ModelEditorFixture.hpp"
#include "../Utilities.hpp"

#include <clocale>

using openstudio::toString;
using openstudio::toQString;
using openstudio::toPath;


TEST_F(ModelEditorFixture, SimpleConversions)
{
  std::string s("Hello world");
  const char* const cStr = "Hello world";
  const wchar_t* const wStr = L"Hello world";
  std::wstring w(L"Hello world");
  QString q("Hello world");
  openstudio::path p = toPath("Hello world");

  EXPECT_EQ(s, s);
  EXPECT_EQ(s, cStr);
  EXPECT_EQ(s, toString(w));
  EXPECT_EQ(s, toString(wStr));
  EXPECT_EQ(s, toString(q));
  EXPECT_EQ(s, toString(p));
  EXPECT_EQ(s, toString(toQString(s)));
  EXPECT_EQ(s, toString(toPath(s)));
  EXPECT_EQ(s, toString(toQString(toPath(s))));
  EXPECT_TRUE(q == toQString(s));
  EXPECT_TRUE(q == toQString(p));
  EXPECT_TRUE(p == toPath(s));
  EXPECT_TRUE(p == toPath(q));
}


TEST_F(ModelEditorFixture, Path_Conversions)
{
  std::string s;
  std::wstring w;
  openstudio::path p;

  s = std::string("basic_path");
  w = std::wstring(L"basic_path");
  p = openstudio::path(s);
  EXPECT_EQ(s, toQString(p).toStdString());
  EXPECT_EQ(s, toQString(toString(p)).toStdString());
  EXPECT_EQ(s, std::string(toQString(toString(p)).toUtf8()));
  EXPECT_EQ(s, toPath(toQString(toString(p))).string());
  EXPECT_EQ(w, toQString(p).toStdWString());
  //EXPECT_EQ(w, toString(p));
  EXPECT_EQ(w, toQString(toString(p)).toStdWString());
  //EXPECT_EQ(w, std::wstring(toQString(toString(p)).toUtf16()));
  EXPECT_EQ(w, toPath(toQString(toString(p))).wstring());

  p = openstudio::path(w);
  EXPECT_EQ(s, toQString(p).toStdString());
  EXPECT_EQ(s, toQString(toString(p)).toStdString());
  EXPECT_EQ(s, std::string(toQString(toString(p)).toUtf8()));
  EXPECT_EQ(s, toPath(toQString(toString(p))).string());

  p = openstudio::path(w);
  EXPECT_EQ(s, toQString(p).toStdString());
  EXPECT_EQ(s, toQString(toString(p)).toStdString());
  EXPECT_EQ(s, std::string(toQString(toString(p)).toUtf8()));
  EXPECT_EQ(s, toPath(toQString(toString(p))).string());

  // http://utf8everywhere.org/


  // from http://www.nubaria.com/en/blog/?p=289

  // Chinese characters for "zhongwen" ("Chinese language").
  const char kChineseSampleText[] = { -28, -72, -83, -26, -106, -121, 0 };

  // Arabic "al-arabiyya" ("Arabic").
  const char kArabicSampleText[] = { -40, -89, -39, -124, -40, -71, -40, -79, -40, -88, -39, -118, -40, -87, 0 };

  // Spanish word "canon" with an "n" with "~" on top and an "o" with an acute accent.
  const char kSpanishSampleText[] = { 99, 97, -61, -79, -61, -77, 110, 0 };

  std::string t;
  QString q;

  t = std::string(kChineseSampleText);
  q = QString::fromUtf8(t.c_str());
  p = toPath(t);
  EXPECT_EQ(t, std::string(toQString(p).toUtf8()));
  EXPECT_EQ(t, toString(p));
  EXPECT_EQ(t, toString(toQString(toString(p))));
  EXPECT_EQ(t, std::string(toQString(toString(p)).toUtf8()));
  EXPECT_EQ(t, toString(toPath(toQString(toString(p)))));

  t = std::string(kArabicSampleText);
  q = QString::fromUtf8(t.c_str());
  p = toPath(t);
  EXPECT_EQ(t, std::string(toQString(p).toUtf8()));
  EXPECT_EQ(t, toString(toQString(toString(p))));
  EXPECT_EQ(t, std::string(toQString(toString(p)).toUtf8()));
  EXPECT_EQ(t, toString(toPath(toQString(toString(p)))));

  t = std::string(kSpanishSampleText);
  q = QString::fromUtf8(t.c_str());
  p = toPath(t);
  EXPECT_EQ(t, std::string(toQString(p).toUtf8()));
  EXPECT_EQ(t, toString(toQString(toString(p))));
  EXPECT_EQ(t, std::string(toQString(toString(p)).toUtf8()));
  EXPECT_EQ(t, toString(toPath(toQString(toString(p)))));

}

