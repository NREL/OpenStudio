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

#include <gtest/gtest.h>

#include <utilities/core/String.hpp>
#include <utilities/core/StringHelpers.hpp>
#include <utilities/core/Path.hpp>
#include <utilities/core/Serialization.hpp>

#include <boost/regex.hpp>

#include <QTextStream>

#include <iostream>

using std::string;
using openstudio::path;
using openstudio::toString;
using openstudio::toQString;
using openstudio::toPath;
using openstudio::toCamelCase;
using openstudio::toUpperCamelCase;
using openstudio::toLowerCamelCase;
using boost::serialization::make_nvp;
using std::ios;
using boost::regex;
using boost::regex_match;

// To convert strings to unicode,, use http://babelfish.yahoo.com/translate_txt to translate text,
// http://www.onlinefunctions.com/function/utf8.htm, encode, then translate to hex

// 'string' (Russian, UTF-8): "\xd1\x88\xd0\xbd\xd1\x83\xd1\x80"
// 'string' (Russian, UTF-16): L"\x0448\x043d\x0443\x0440"

TEST(String, SimpleConversions)
{
  std::string s("Hello world");
  const char* const cStr = "Hello world";
  const wchar_t* const wStr = L"Hello world";
  std::wstring w(L"Hello world");
  QString q("Hello world");
  path p = toPath("Hello world");

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

//TEST(String, UnicodeConversions)
//{
//  std::string s("\xd1\x88\xd0\xbd\xd1\x83\xd1\x80");
//  const char* const cStr = "\xd1\x88\xd0\xbd\xd1\x83\xd1\x80";
//  const wchar_t* const wStr = L"\x0448\x043d\x0443\x0440";
//  std::wstring w(L"\x0448\x043d\x0443\x0440");
//  QString q = QString::fromUtf8("\xd1\x88\xd0\xbd\xd1\x83\xd1\x80");
//  path p = toPath("\xd1\x88\xd0\xbd\xd1\x83\xd1\x80");
//
//  EXPECT_EQ(s, s);
//  EXPECT_EQ(s, cStr);
//  EXPECT_EQ(s, toString(w));
//  EXPECT_EQ(s, toString(wStr));
//  EXPECT_EQ(s, toString(q));
//  EXPECT_EQ(s, toString(p));
//  EXPECT_EQ(s, toString(toQString(s)));
//  EXPECT_EQ(s, toString(toPath(s)));
//  EXPECT_TRUE(q == toQString(s));
//  EXPECT_TRUE(q == toQString(p));
//  EXPECT_TRUE(p == toPath(s));
//  EXPECT_TRUE(p == toPath(q));
//}
//
//TEST(String, MixedConversions)
//{
//  std::string s("Hello World \xd1\x88\xd0\xbd\xd1\x83\xd1\x80");
//  const char* const cStr = "Hello World \xd1\x88\xd0\xbd\xd1\x83\xd1\x80";
//  const wchar_t* const wStr = L"Hello World \x0448\x043d\x0443\x0440";
//  std::wstring w(L"Hello World \x0448\x043d\x0443\x0440");
//  QString q = QString::fromUtf8("Hello World \xd1\x88\xd0\xbd\xd1\x83\xd1\x80");
//  path p = toPath("Hello World \xd1\x88\xd0\xbd\xd1\x83\xd1\x80");
//
//  EXPECT_EQ(s, s);
//  EXPECT_EQ(s, cStr);
//  EXPECT_EQ(s, toString(w));
//  EXPECT_EQ(s, toString(wStr));
//  EXPECT_EQ(s, toString(q));
//  EXPECT_EQ(s, toString(p));
//  EXPECT_EQ(s, toString(toQString(s)));
//  EXPECT_EQ(s, toString(toPath(s)));
//  EXPECT_TRUE(q == toQString(s));
//  EXPECT_TRUE(q == toQString(p));
//  EXPECT_TRUE(p == toPath(s));
//  EXPECT_TRUE(p == toPath(q));
//}
//
//// Serialization
//TEST(String, Serialization)
//{
//  // Hello World + std::string (in Russian)
//  std::string str("Hello World \xd1\x88 \xd0\xbd \xd1\x83 \xd1\x80");
//  path dir = toPath("Hello World \xd1\x88 \xd0\xbd \xd1\x83 \xd1\x80");
//  boost::filesystem::create_directory(dir);
//
//  // write out, file is close on scope exit
//  {
//    boost::filesystem::ofstream ofs(dir / toPath(str + ".txt"));
//    boost::archive::text_oarchive out(ofs);
//    out << boost::serialization::make_nvp("str", str);
//    out << boost::serialization::make_nvp("dir", dir);
//  }
//  {
//    boost::filesystem::ofstream ofs(dir / toPath(str + ".xml"));
//    boost::archive::xml_oarchive out(ofs);
//    out << boost::serialization::make_nvp("str", str);
//    out << boost::serialization::make_nvp("dir", dir);
//  }
//  {
//    boost::filesystem::ofstream ofs(dir / toPath(str + ".bin"));
//    boost::archive::binary_oarchive out(ofs);
//    out << boost::serialization::make_nvp("str", str);
//    out << boost::serialization::make_nvp("dir", dir);
//  }
// 
//  // read in
//  std::string newStr;  
//  path newDir;
//  {
//    boost::filesystem::ifstream ifs(dir / toPath(str + ".txt"));
//    boost::archive::text_iarchive in(ifs);
//    in >> boost::serialization::make_nvp("str", newStr);
//    in >> boost::serialization::make_nvp("dir", newDir);
//    ASSERT_EQ(str, newStr);
//    ASSERT_TRUE(dir == newDir);
//  }
//  {
//    boost::filesystem::ifstream ifs(dir / toPath(str + ".xml"));
//    boost::archive::xml_iarchive in(ifs);
//    in >> boost::serialization::make_nvp("str", newStr);
//    in >> boost::serialization::make_nvp("dir", newDir);
//    ASSERT_EQ(str, newStr);
//    ASSERT_TRUE(dir == newDir);
//  }
//  {
//    boost::filesystem::ifstream ifs(dir / toPath(str + ".bin"));
//    boost::archive::binary_iarchive in(ifs);
//    in >> boost::serialization::make_nvp("str", newStr);
//    in >> boost::serialization::make_nvp("dir", newDir);
//    ASSERT_EQ(str, newStr);
//    ASSERT_TRUE(dir == newDir);
//  }
//  
//}

TEST(String, Regex)
{
  std::string haystack("Hello World");

  boost::regex needle;

  needle = boost::regex("Hello");
  EXPECT_TRUE(boost::regex_search(haystack, needle));

  needle = boost::regex("World");
  EXPECT_TRUE(boost::regex_search(haystack, needle));

  needle = boost::regex("o W");
  EXPECT_TRUE(boost::regex_search(haystack, needle));

  needle = boost::regex("^Hello World$");
  EXPECT_TRUE(boost::regex_search(haystack, needle));
}

TEST(String, MixedRegex)
{
  std::string haystack("Hello World \xd1\x88\xd0\xbd\xd1\x83\xd1\x80");

  boost::regex needle;

  needle = boost::regex("Hello");
  EXPECT_TRUE(boost::regex_search(haystack, needle));

  needle = boost::regex("\xd1\x88");
  EXPECT_TRUE(boost::regex_search(haystack, needle));

  needle = boost::regex("\xd1");
  EXPECT_TRUE(boost::regex_search(haystack, needle));

  needle = boost::regex("^Hello World \xd1\x88\xd0\xbd\xd1\x83\xd1\x80$");
  EXPECT_TRUE(boost::regex_search(haystack, needle));
}

TEST(String, CamelCase) {
  EXPECT_EQ("helloWorld",toCamelCase("hello world"));
  EXPECT_EQ("helloWorld",toCamelCase("hello_world"));
  EXPECT_EQ("helloWorld",toCamelCase("hello  World"));
  EXPECT_EQ("HelloWorld",toCamelCase("Hello World"));
}

TEST(String, UpperCamelCase) {
  EXPECT_EQ("HelloWorld",toUpperCamelCase("hello world"));
  EXPECT_EQ("HelloWorld",toUpperCamelCase("hello_world"));
  EXPECT_EQ("HelloWorld",toUpperCamelCase("hello  World"));
  EXPECT_EQ("HelloWorld",toUpperCamelCase("Hello World"));
}

TEST(String, LowerCamelCase) {
  EXPECT_EQ("helloWorld",toLowerCamelCase("hello world"));
  EXPECT_EQ("helloWorld",toLowerCamelCase("hello_world"));
  EXPECT_EQ("helloWorld",toLowerCamelCase("hello  World"));
  EXPECT_EQ("helloWorld",toLowerCamelCase("Hello World"));
}
