/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../Macro.hpp"
#include "../Logger.hpp"
#include "../Finder.hpp"

using std::string;
using openstudio::NameFinder;
using openstudio::findByName;
using openstudio::findIndexByName;

using std::find_if;

///////////////////////////////////////////////////////////////////////////////
// *** HELPER CLASSES ***
///////////////////////////////////////////////////////////////////////////////

class Object
{
 public:
  COMMON_PTR_TYPEDEFS(Object)
  typedef boost::optional<Object> Opt;
  Object(const string& name, unsigned number) : m_name(name), m_number(number) {}
  static Ptr create(const string& name, unsigned number) {
    return Ptr(new Object(name, number));
  }
  const string& name() const {
    return m_name;
  }
  unsigned number() const {
    return m_number;
  }

 private:
  string m_name;
  unsigned m_number;
};

///////////////////////////////////////////////////////////////////////////////
// *** BEGIN TESTS ***
///////////////////////////////////////////////////////////////////////////////

TEST(Finder, NameFinder_Ptr) {
  LOG_FREE(Info, "Finder", "Entering NameFinder_Ptr")

  typedef std::vector<Object::Ptr> VectorType;
  VectorType haystack;

  Object::Ptr needle = Object::create("Billy", 1);
  Object::Ptr hay = Object::create("Mary", 1);

  // finder
  NameFinder<Object> finder("Billy");

  // not found
  // cppcheck-suppress knownEmptyContainer
  VectorType::const_iterator it = find_if(haystack.begin(), haystack.end(), finder);
  EXPECT_TRUE(haystack.end() == it);

  // not found
  haystack.push_back(hay);
  it = find_if(haystack.begin(), haystack.end(), finder);
  EXPECT_TRUE(haystack.end() == it);

  // found
  haystack.push_back(needle);
  it = find_if(haystack.begin(), haystack.end(), finder);
  EXPECT_TRUE(haystack.end() != it);

  // now search in reverse order
  VectorType::const_reverse_iterator it2 = find_if(haystack.rbegin(), haystack.rend(), finder);
  VectorType::const_reverse_iterator rend = haystack.rend();
  EXPECT_TRUE(rend != it2);

  // two needles are the same
  EXPECT_EQ(static_cast<unsigned int>(1), (*it)->number());
  EXPECT_EQ(static_cast<unsigned int>(1), (*it2)->number());

  // add more hay and another needle
  Object::Ptr needle2 = Object::create("Billy", 2);
  haystack.push_back(hay);
  haystack.push_back(needle2);
  haystack.push_back(hay);

  // found
  it = find_if(haystack.begin(), haystack.end(), finder);
  EXPECT_TRUE(haystack.end() != it);

  // now search in reverse order
  it2 = find_if(haystack.rbegin(), haystack.rend(), finder);
  rend = haystack.rend();
  EXPECT_TRUE(rend != it2);

  // two needles are not the same anymore
  EXPECT_EQ(static_cast<unsigned int>(1), (*it)->number());
  EXPECT_EQ(static_cast<unsigned int>(2), (*it2)->number());

  // not found
  it = find_if(haystack.begin(), haystack.end(), NameFinder<Object>("Sally"));
  EXPECT_TRUE(haystack.end() == it);

  LOG_FREE(Info, "Finder", "Leaving NameFinder_Ptr")
}

TEST(Finder, NameFinder_POD) {
  LOG_FREE(Info, "Finder", "Entering NameFinder_POD")

  typedef std::vector<Object> VectorType;
  VectorType haystack;

  Object needle = Object("Billy", 1);
  Object hay = Object("Mary", 1);

  // finder
  NameFinder<Object> finder("Billy");

  // not found
  // cppcheck-suppress knownEmptyContainer
  VectorType::const_iterator it = find_if(haystack.begin(), haystack.end(), finder);
  EXPECT_TRUE(haystack.end() == it);

  // not found
  haystack.push_back(hay);
  it = find_if(haystack.begin(), haystack.end(), finder);
  EXPECT_TRUE(haystack.end() == it);

  // found
  haystack.push_back(needle);
  it = find_if(haystack.begin(), haystack.end(), finder);
  EXPECT_TRUE(haystack.end() != it);

  // now search in reverse order
  VectorType::const_reverse_iterator it2 = find_if(haystack.rbegin(), haystack.rend(), finder);
  VectorType::const_reverse_iterator rend = haystack.rend();
  EXPECT_TRUE(rend != it2);

  // two needles are the same
  EXPECT_EQ(static_cast<unsigned int>(1), it->number());
  EXPECT_EQ(static_cast<unsigned int>(1), it2->number());

  // add more hay and another needle
  Object needle2("Billy", 2);
  haystack.push_back(hay);
  haystack.push_back(needle2);
  haystack.push_back(hay);

  // found
  it = find_if(haystack.begin(), haystack.end(), finder);
  EXPECT_TRUE(haystack.end() != it);

  // now search in reverse order
  it2 = find_if(haystack.rbegin(), haystack.rend(), finder);
  rend = haystack.rend();
  EXPECT_TRUE(rend != it2);

  // two needles are not the same anymore
  EXPECT_EQ(static_cast<unsigned int>(1), it->number());
  EXPECT_EQ(static_cast<unsigned int>(2), it2->number());

  // not found
  it = find_if(haystack.begin(), haystack.end(), NameFinder<Object>("Sally"));
  EXPECT_TRUE(haystack.end() == it);

  LOG_FREE(Info, "Finder", "Leaving NameFinder_POD")
}

TEST(Finder, findByName_Ptr) {
  LOG_FREE(Info, "Finder", "Entering findByName_Ptr")

  typedef std::vector<Object::Ptr> VectorType;
  VectorType haystack;
  Object::Ptr obj;

  // not found
  obj = findByName(haystack, "Billy");
  EXPECT_FALSE(obj);

  // not found
  obj = findByName(haystack, "Mary");
  EXPECT_FALSE(obj);

  // not found
  obj = findByName(haystack, "Sally");
  EXPECT_FALSE(obj);

  // insert
  Object::Ptr needle = Object::create("Billy", 1);
  Object::Ptr hay = Object::create("Mary", 1);
  haystack.push_back(needle);
  haystack.push_back(hay);

  // found
  obj = findByName(haystack, "Billy");
  ASSERT_TRUE(obj.get());
  EXPECT_TRUE("Billy" == obj->name());
  EXPECT_EQ(static_cast<unsigned int>(1), obj->number());

  // found
  obj = findByName(haystack, "Mary");
  ASSERT_TRUE(obj.get());
  EXPECT_TRUE("Mary" == obj->name());
  EXPECT_EQ(static_cast<unsigned int>(1), obj->number());

  // not found
  obj = findByName(haystack, "Sally");
  EXPECT_FALSE(obj);

  // insert another needle
  Object::Ptr needle2 = Object::create("Billy", 2);
  haystack.push_back(needle2);

  // found the first one, not the last
  obj = findByName(haystack, "Billy");
  ASSERT_TRUE(obj.get());
  EXPECT_TRUE("Billy" == obj->name());
  EXPECT_EQ(static_cast<unsigned int>(1), obj->number());

  // check that we found the first one
  EXPECT_TRUE(needle == obj);
  EXPECT_TRUE(needle != needle2);
  EXPECT_TRUE(needle2 != obj);

  LOG_FREE(Info, "Finder", "Leaving findByName_Ptr")
}

TEST(Finder, findByName_POD) {
  LOG_FREE(Info, "Finder", "findByName_POD findByName_Ptr")

  typedef std::vector<Object> VectorType;
  VectorType haystack;
  Object::Opt obj;

  // not found
  obj = findByName(haystack, "Billy");
  EXPECT_FALSE(obj);

  // not found
  obj = findByName(haystack, "Mary");
  EXPECT_FALSE(obj);

  // not found
  obj = findByName(haystack, "Sally");
  EXPECT_FALSE(obj);

  // insert
  Object needle("Billy", 1);
  Object hay("Mary", 1);
  haystack.push_back(needle);
  haystack.push_back(hay);

  // found
  obj = findByName(haystack, "Billy");
  ASSERT_TRUE(obj);
  EXPECT_TRUE("Billy" == obj->name());
  EXPECT_EQ(static_cast<unsigned int>(1), obj->number());

  // found
  obj = findByName(haystack, "Mary");
  ASSERT_TRUE(obj);
  EXPECT_TRUE("Mary" == obj->name());
  EXPECT_EQ(static_cast<unsigned int>(1), obj->number());

  // not found
  obj = findByName(haystack, "Sally");
  EXPECT_FALSE(obj);

  // insert another needle
  Object needle2("Billy", 2);
  haystack.push_back(needle2);

  // found the first one, not the last
  obj = findByName(haystack, "Billy");
  ASSERT_TRUE(obj);
  EXPECT_TRUE("Billy" == obj->name());
  EXPECT_EQ(static_cast<unsigned int>(1), obj->number());

  LOG_FREE(Info, "Finder", "Leaving findByName_POD")
}

TEST(Finder, findIndexByName_POD) {
  typedef std::vector<Object> VectorType;
  VectorType haystack;
  boost::optional<int> index;

  // not found
  index = findIndexByName(haystack, "Billy");
  EXPECT_FALSE(index);

  // not found
  index = findIndexByName(haystack, "Mary");
  EXPECT_FALSE(index);

  // not found
  index = findIndexByName(haystack, "Sally");
  EXPECT_FALSE(index);

  // insert
  Object needle("Billy", 1);
  Object hay("Mary", 1);
  haystack.push_back(needle);
  haystack.push_back(hay);

  // found
  index = findIndexByName(haystack, "Billy");
  ASSERT_TRUE(index);
  EXPECT_EQ(0, *index);

  // found
  index = findIndexByName(haystack, "Mary");
  ASSERT_TRUE(index);
  EXPECT_EQ(1, *index);

  // not found
  index = findIndexByName(haystack, "Sally");
  EXPECT_FALSE(index);
}
