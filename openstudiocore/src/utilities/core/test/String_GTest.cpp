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
//  openstudio::filesystem::create_directory(dir);
//
//  // write out, file is close on scope exit
//  {
//    openstudio::filesystem::ofstream ofs(dir / toPath(str + ".txt"));
//    boost::archive::text_oarchive out(ofs);
//    out << boost::serialization::make_nvp("str", str);
//    out << boost::serialization::make_nvp("dir", dir);
//  }
//  {
//    openstudio::filesystem::ofstream ofs(dir / toPath(str + ".xml"));
//    boost::archive::xml_oarchive out(ofs);
//    out << boost::serialization::make_nvp("str", str);
//    out << boost::serialization::make_nvp("dir", dir);
//  }
//  {
//    openstudio::filesystem::ofstream ofs(dir / toPath(str + ".bin"));
//    boost::archive::binary_oarchive out(ofs);
//    out << boost::serialization::make_nvp("str", str);
//    out << boost::serialization::make_nvp("dir", dir);
//  }
//
//  // read in
//  std::string newStr;
//  path newDir;
//  {
//    openstudio::filesystem::ifstream ifs(dir / toPath(str + ".txt"));
//    boost::archive::text_iarchive in(ifs);
//    in >> boost::serialization::make_nvp("str", newStr);
//    in >> boost::serialization::make_nvp("dir", newDir);
//    ASSERT_EQ(str, newStr);
//    ASSERT_TRUE(dir == newDir);
//  }
//  {
//    openstudio::filesystem::ifstream ifs(dir / toPath(str + ".xml"));
//    boost::archive::xml_iarchive in(ifs);
//    in >> boost::serialization::make_nvp("str", newStr);
//    in >> boost::serialization::make_nvp("dir", newDir);
//    ASSERT_EQ(str, newStr);
//    ASSERT_TRUE(dir == newDir);
//  }
//  {
//    openstudio::filesystem::ifstream ifs(dir / toPath(str + ".bin"));
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

  EXPECT_EQ("headline_cnn_news", toUnderscoreCase("headline cnn news"));
  EXPECT_EQ("headline_cnn_news", toUnderscoreCase("HEADLINE CNN NEWS"));
  EXPECT_EQ("headline_cnn_news", toUnderscoreCase("Headline CNN News"));
  EXPECT_EQ("headline_cnn_news", toUnderscoreCase("HeadlineCNNNews"));
  EXPECT_EQ("headline_cnn_news", toUnderscoreCase("Headline_CNN_News"));
  EXPECT_EQ("headline_cnn_news", toUnderscoreCase("Headline-CNN-News"));

  EXPECT_EQ("allcaps", toUnderscoreCase("ALLCAPS"));

  EXPECT_EQ("n_packages", toUnderscoreCase("nPackages"));

  EXPECT_EQ("ashrae_9012007_hvac_system_5_packaged_va_vwithwithhotwaterreheat", toUnderscoreCase("ASHRAE9012007HVACSystem5PackagedVAVwithwithhotwaterreheat"));

  EXPECT_EQ("openstudio_results", toUnderscoreCase("OpenStudioResults"));
  EXPECT_EQ("run_energyplus", toUnderscoreCase("RunEnergyPlus"));
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

TEST(String, StringSplit)
{
  std::vector<std::string> results = splitString("This,is,the,unambiguous,case",',');
  ASSERT_EQ(5, results.size());
  EXPECT_EQ("This", results[0]);
  EXPECT_EQ("is", results[1]);
  EXPECT_EQ("the", results[2]);
  EXPECT_EQ("unambiguous", results[3]);
  EXPECT_EQ("case", results[4]);

  results = splitString(",This,is,more,ambiguous", ',');
  ASSERT_EQ(5, results.size());
  EXPECT_TRUE(results[0].empty());
  EXPECT_EQ("This", results[1]);
  EXPECT_EQ("is", results[2]);
  EXPECT_EQ("more", results[3]);
  EXPECT_EQ("ambiguous", results[4]);

  results = splitString(",Even,more,ambiguous,", ',');
  ASSERT_EQ(5, results.size());
  EXPECT_TRUE(results[0].empty());
  EXPECT_EQ("Even", results[1]);
  EXPECT_EQ("more", results[2]);
  EXPECT_EQ("ambiguous", results[3]);
  EXPECT_TRUE(results[4].empty());

  //Empty string case
  results = splitString("", ',');
  ASSERT_EQ(0, results.size());

  //Very little to do test
  results = splitString(",", ',');
  ASSERT_EQ(2, results.size());

  results = splitString("This is not the string you are looking for", ',');
  ASSERT_EQ(1, results.size());
  EXPECT_EQ("This is not the string you are looking for", results[0]);

}


TEST(String, SplitEMSLineToTokens)
{
  // Very simple case
  std::vector<std::string> tokens = splitEMSLineToTokens("SET Var1 = 10");
  ASSERT_EQ(2, tokens.size());
  EXPECT_EQ("Var1", tokens[0]);
  EXPECT_EQ("10", tokens[1]);

  // More complicated: Set is a statement keyword, Hour is an ERL dynamic variable
  // and @TrendValue is a function.
  tokens = splitEMSLineToTokens("Set  Var1 =(Hour  *(@TrendValue Var1   2)) /(Var2)");
  ASSERT_EQ(4, tokens.size());
  EXPECT_EQ("Var1", tokens[0]);
  EXPECT_EQ("Var1", tokens[1]);
  EXPECT_EQ("2", tokens[2]);
  EXPECT_EQ("Var2", tokens[3]);
}

