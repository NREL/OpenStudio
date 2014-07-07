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

#include <gtest/gtest.h>

#include "../Containers.hpp"
#include "../String.hpp"
#include "../StringHelpers.hpp"
#include "../Path.hpp"

#include <boost/regex.hpp>

#include <QTextStream>

#include <iostream>

using std::string;
using namespace openstudio;
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

TEST(String, UnderscoreCase) {
  EXPECT_EQ("hello_world",toUnderscoreCase("hello world"));
  EXPECT_EQ("hello_world",toUnderscoreCase("hello_world"));
  EXPECT_EQ("hello_world",toUnderscoreCase("hello  World"));
  EXPECT_EQ("hello_world",toUnderscoreCase("Hello World"));
}

TEST(String,NeatStrings) {
  double value = 1.0;
  // as is, should print as "1" and show me no digits past the decimal point
  std::string str = toString(value);
  EXPECT_EQ("1",str);
  EXPECT_EQ(0u,numFractionalDigits(str));
  // can add digits if I want
  EXPECT_EQ("1.0",toNeatString(value,1));
  EXPECT_EQ("1.000",toNeatString(value,3));

  value = 16891690157329.2819;
  // too many significant figures -- cut down to 5 and see how it looks
  str = toString(toNumSigFigs(value,5));
  EXPECT_EQ("16892000000000",str);
  str = toNeatStringBySigFigs(value,6);
  EXPECT_EQ("16,891,700,000,000",str);
  EXPECT_EQ(0u,numFractionalDigits(str));
  str = toNeatString(toNumSigFigs(value,5));
  EXPECT_EQ("16,892,000,000,000",str);
  EXPECT_EQ(0u,numFractionalDigits(str));

  value = -0.0001892962;
  str = toString(toNumSigFigs(value,2));
  EXPECT_EQ("-0.00019",str);
  str = toNeatStringBySigFigs(value,2);
  EXPECT_EQ("-0.00019",str);
  EXPECT_EQ(5u,numFractionalDigits(str));
  str = toNeatString(value,5);
  EXPECT_EQ("-0.00019",str);
  EXPECT_EQ(5u,numFractionalDigits(str));
  
  value = -1326.32;
  str = toNeatString(toNumSigFigs(value,3),1);
  EXPECT_EQ("-1,330.0",str);
  EXPECT_EQ(1u,numFractionalDigits(str));
}

TEST(String,NumFractionalDigits) {
  DoubleVector values;
  values.push_back(128196.198);
  values.push_back(19671.281);
  values.push_back(218528.28);
  values.push_back(192.186);

  std::pair<unsigned,unsigned> result = numFractionalDigits(values,3u);
  EXPECT_EQ(0u,result.first); EXPECT_EQ(0u,result.second);

  result = numFractionalDigits(values,4u);
  EXPECT_EQ(0u,result.first); EXPECT_EQ(1u,result.second);

  result = numFractionalDigits(values,7u);
  EXPECT_EQ(1u,result.first); EXPECT_EQ(4u,result.second);

  values.clear();
  values.push_back(0.189678);
  values.push_back(0.001869168);
  values.push_back(0.7198);

  result = numFractionalDigits(values,2u);
  EXPECT_EQ(2u,result.first); EXPECT_EQ(4u,result.second);

  result = numFractionalDigits(values,8u);
  EXPECT_EQ(8u,result.first); EXPECT_EQ(10u,result.second);

  values.clear();
  values.push_back(0.07592);
  values.push_back(198.82);
  values.push_back(210.28);
  values.push_back(0.628);

  result = numFractionalDigits(values,2u);
  EXPECT_EQ(0u,result.first); EXPECT_EQ(3u,result.second);

  result = numFractionalDigits(values,3u);
  EXPECT_EQ(0u,result.first); EXPECT_EQ(4u,result.second);

  result = numFractionalDigits(values,5u);
  EXPECT_EQ(2u,result.first); EXPECT_EQ(6u,result.second);  

  values.clear();
  values.push_back(0.0);

  result = numFractionalDigits(values,2u);
  EXPECT_EQ(1u,result.first); EXPECT_EQ(1u,result.second);

  result = numFractionalDigits(values,5u);
  EXPECT_EQ(4u,result.first); EXPECT_EQ(4u,result.second);
}

