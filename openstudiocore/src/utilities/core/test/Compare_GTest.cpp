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

  auto it = words.find("lorem ipsum");
  ASSERT_TRUE(words.end() != it);

  auto it2 = words.find("LOREM IPSUM");
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

  auto it = words.find(PairType("lorem", "ipsum"));
  ASSERT_TRUE(words.end() != it);

  auto it2 = words.find(PairType("lorem", "IPSUM"));
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
