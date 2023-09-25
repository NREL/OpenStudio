/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
  using Opt = boost::optional<Object>;
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

  using VectorType = std::vector<Object::Ptr>;
  VectorType haystack;

  Object::Ptr needle = Object::create("Billy", 1);
  Object::Ptr hay = Object::create("Mary", 1);

  // finder
  NameFinder<Object> finder("Billy");

  // not found
  // cppcheck-suppress knownEmptyContainer
  auto it = find_if(haystack.begin(), haystack.end(), finder);
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
  auto it2 = find_if(haystack.rbegin(), haystack.rend(), finder);
  auto rend = haystack.rend();
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

  using VectorType = std::vector<Object>;
  VectorType haystack;

  Object needle = Object("Billy", 1);
  Object hay = Object("Mary", 1);

  // finder
  NameFinder<Object> finder("Billy");

  // not found
  // cppcheck-suppress knownEmptyContainer
  auto it = find_if(haystack.begin(), haystack.end(), finder);
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
  auto it2 = find_if(haystack.rbegin(), haystack.rend(), finder);
  auto rend = haystack.rend();
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

  using VectorType = std::vector<Object::Ptr>;
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

  using VectorType = std::vector<Object>;
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
  using VectorType = std::vector<Object>;
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
