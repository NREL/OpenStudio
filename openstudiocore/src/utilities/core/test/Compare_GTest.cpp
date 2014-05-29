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

#include "../Compare.hpp"
#include "../Logger.hpp"

#include <set>
#include <vector>
#include <utility> // for pair

using std::string;
using openstudio::istringEqual;
using openstudio::IstringCompare;
using openstudio::IstringPairCompare;
using openstudio::checkPtrVecEqual;
using openstudio::firstOfPairEqual;
using openstudio::secondOfPairEqual;
using openstudio::VersionString;
using std::shared_ptr;


TEST(Compare, istringEqual)
{
  LOG_FREE(Info, "Compare", "Entering istringEqual")

  // make sure insensitive string compare works
  EXPECT_TRUE(istringEqual("lorem ipsum", "lorem ipsum"));
  EXPECT_TRUE(istringEqual("lorem ipsum", "LOREM IPSUM"));
  EXPECT_TRUE(istringEqual("lorem ipsum", "lOrEm IpsUm"));
  EXPECT_TRUE(istringEqual("lorem ipsum", "LoReM iPSuM"));
  EXPECT_FALSE(istringEqual("lorem ipsum", "dolor sit amet"));

  LOG_FREE(Info, "Compare", "Leaving istringEqual")
}

TEST(Compare, istringEqualMixed)
{
  LOG_FREE(Info, "Compare", "Entering istringEqualMixed")

  EXPECT_TRUE(istringEqual("Hello World \xd1\x88\xd0\xbd\xd1\x83\xd1\x80",
                           "Hello World \xd1\x88\xd0\xbd\xd1\x83\xd1\x80"));

  EXPECT_TRUE(istringEqual("Hello World \xd1\x88\xd0\xbd\xd1\x83\xd1\x80",
                           "HELLO WORLD \xd1\x88\xd0\xbd\xd1\x83\xd1\x80"));

  EXPECT_TRUE(istringEqual("Hello World \xd1\x88\xd0\xbd\xd1\x83\xd1\x80",
                           "HeLLo wOrld \xd1\x88\xd0\xbd\xd1\x83\xd1\x80"));

  EXPECT_FALSE(istringEqual("Hello World \xd1\x88\xd0\xbd\xd1\x83\xd1\x80",
                            "Hello World \xd1\x88\xd0\xbd\xd1\x83"));

  EXPECT_FALSE(istringEqual("Hello World \xd1\x88\xd0\xbd\xd1\x83\xd1\x80",
                            "Hello World \xd1\x80\xd1\x88\xd0\xbd\xd1\x83"));

  LOG_FREE(Info, "Compare", "Leaving istringEqualMixed")
}


TEST(Compare, IstringCompare)
{
  LOG_FREE(Info, "Compare", "Entering IstringEqual")

  // make sure insensitive string compare functor works for sets
  typedef std::set<string, IstringCompare> SetType;
  SetType words;
  words.insert("lorem ipsum");

  SetType::const_iterator it = words.find("lorem ipsum");
  ASSERT_TRUE(words.end() != it);

  SetType::const_iterator it2 = words.find("LOREM IPSUM");
  EXPECT_TRUE(it == it2);

  it2 = words.find("lOrEm IpsUm");
  EXPECT_TRUE(it == it2);

  it2 = words.find("LoReM iPSuM");
  EXPECT_TRUE(it == it2);

  it2 = words.find("dolor sit amet");
  EXPECT_TRUE(words.end() == it2);

  LOG_FREE(Info, "Compare", "Leaving IstringEqual")
}

TEST(Compare, IstringPairCompare)
{
  LOG_FREE(Info, "Compare", "Entering IstringPairCompare")

  // make sure insensitive string compare functor works for sets of pairs
  typedef std::pair<string, string> PairType;
  typedef std::set<PairType, IstringPairCompare> SetType;
  SetType words;
  words.insert(PairType("lorem", "ipsum"));

  SetType::const_iterator it = words.find(PairType("lorem", "ipsum"));
  ASSERT_TRUE(words.end() != it);

  SetType::const_iterator it2 = words.find(PairType("lorem", "IPSUM"));
  EXPECT_TRUE(it == it2);

  it2 = words.find(PairType("LOREM", "ipsum"));
  EXPECT_TRUE(it == it2);

  it2 = words.find(PairType("LOREM", "IPSUM"));
  EXPECT_TRUE(it == it2);

  it2 = words.find(PairType("lOrEm", "IpsUm"));
  EXPECT_TRUE(it == it2);

  it2 = words.find(PairType("LoReM", "iPSuM"));
  EXPECT_TRUE(it == it2);

  it2 = words.find(PairType("dolor", "amet"));
  EXPECT_TRUE(words.end() == it2);

  LOG_FREE(Info, "Compare", "Leaving IstringPairCompare")
}

TEST(Compare, checkPtrVecEqual)
{
  LOG_FREE(Info, "Compare", "Entering IstringPairCompare")

  typedef std::vector<shared_ptr<double> > VectorType;

  VectorType v1;
  VectorType v2;
  EXPECT_TRUE(checkPtrVecEqual(v1, v2)); // equal when empty
  EXPECT_TRUE(checkPtrVecEqual(v1, v1)); // equal to self
  EXPECT_TRUE(checkPtrVecEqual(v2, v2)); // equal to self

  shared_ptr<double> p = shared_ptr<double>(new double(1));
  v1.push_back(p);
  v2.push_back(p);
  EXPECT_TRUE(checkPtrVecEqual(v1, v2)); // euqal
  EXPECT_TRUE(checkPtrVecEqual(v1, v1)); // equal to self
  EXPECT_TRUE(checkPtrVecEqual(v2, v2)); // equal to self

  p = shared_ptr<double>(new double(2));
  v1.push_back(p);
  v2.push_back(p);
  EXPECT_TRUE(checkPtrVecEqual(v1, v2)); // equal
  EXPECT_TRUE(checkPtrVecEqual(v1, v1)); // equal to self
  EXPECT_TRUE(checkPtrVecEqual(v2, v2)); // equal to self

  shared_ptr<double> p1 = shared_ptr<double>(new double(3));
  shared_ptr<double> p2 = shared_ptr<double>(new double(3));
  v1.push_back(p1);
  v2.push_back(p2);
  EXPECT_TRUE(checkPtrVecEqual(v1, v2)); // equal (comparing values)
  EXPECT_TRUE(checkPtrVecEqual(v1, v1)); // equal to self
  EXPECT_TRUE(checkPtrVecEqual(v2, v2)); // equal to self

  p1 = shared_ptr<double>(new double(4));
  v1.push_back(p1);
  EXPECT_FALSE(checkPtrVecEqual(v1, v2)); // unequal
  EXPECT_TRUE(checkPtrVecEqual(v1, v1)); // equal to self
  EXPECT_TRUE(checkPtrVecEqual(v2, v2)); // equal to self

  p2 = shared_ptr<double>(new double(4));
  v2.push_back(p2);
  EXPECT_TRUE(checkPtrVecEqual(v1, v2)); // equal again
  EXPECT_TRUE(checkPtrVecEqual(v1, v1)); // equal to self
  EXPECT_TRUE(checkPtrVecEqual(v2, v2)); // equal to self

  v1.push_back(shared_ptr<double>());
  v2.push_back(shared_ptr<double>());
  EXPECT_FALSE(checkPtrVecEqual(v1, v2)); // null pointer causes failure
  EXPECT_FALSE(checkPtrVecEqual(v1, v1)); // null pointer causes failure
  EXPECT_FALSE(checkPtrVecEqual(v2, v2)); // null pointer causes failure

  LOG_FREE(Info, "Compare", "Leaving IstringPairCompare")
}

TEST(Compare, EqualityForOneHalfOfPair)
{
  std::pair<std::string,int> p1("Employee",32);
  std::pair<int,bool> p2(-246,true);
  std::pair<bool,std::string> p3(false,"Hello");

  ASSERT_TRUE(firstOfPairEqual(p1,std::string("Employee")));
  ASSERT_FALSE(secondOfPairEqual(p1,55));
  ASSERT_TRUE(secondOfPairEqual(p2,true));
  ASSERT_FALSE(secondOfPairEqual(p3,std::string("World")));
  ASSERT_FALSE(firstOfPairEqual(p2,0));
}

TEST(Compare,VersionString) {
  EXPECT_TRUE(VersionString("3.1.2") < VersionString("3.2.0"));
  EXPECT_TRUE(VersionString("3.1") < VersionString("3.2.3"));
  EXPECT_FALSE(VersionString("3.1") < VersionString("3.1.3"));
  EXPECT_FALSE(VersionString("3.1") > VersionString("3.1.3"));
  EXPECT_TRUE(VersionString("3.1") == VersionString("3.1.3")); // not the best outcome

  EXPECT_TRUE(VersionString("3.1.2").fidelityEqual(VersionString("3.2.0")));
  EXPECT_FALSE(VersionString("3.1").fidelityEqual(VersionString("3.1.3")));

  EXPECT_TRUE(VersionString("7.1.0.32891") != VersionString("7.0"));
  EXPECT_TRUE(VersionString("7.1.0.32891") == VersionString("7.1"));
  EXPECT_FALSE(VersionString("7.1.0.32891").fidelityEqual(VersionString("7.1")));

  EXPECT_TRUE(VersionString("0.7.0") <= VersionString("0.7"));
  EXPECT_TRUE(VersionString("0.3.1") <= VersionString("0.7"));

  EXPECT_TRUE(VersionString("1.0.0") >= VersionString("0.9.1"));
}
