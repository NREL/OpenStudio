/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "../Assert.hpp"

namespace sharedfromthistest {
struct Object_Impl : public std::enable_shared_from_this<Object_Impl>
{
  Object_Impl(std::shared_ptr<int> counter);
  ~Object_Impl();
  void increment();
  void incrementFromThis();
  std::shared_ptr<int> m_counter;

  struct null_deleter
  {
    void operator()(void const*) const {}
  };
};

struct Object
{
  Object(std::shared_ptr<Object_Impl> impl) : m_impl(impl) {
    OS_ASSERT(m_impl);
    std::weak_ptr<Object_Impl> weakImpl(impl);
    OS_ASSERT(weakImpl.lock());
  }

  ~Object() = default;

  void increment() {
    m_impl->increment();
  }

  void incrementFromThis() {
    m_impl->incrementFromThis();
  }

  std::shared_ptr<Object_Impl> m_impl;
};

Object_Impl::Object_Impl(std::shared_ptr<int> counter) : m_counter(counter) {
  OS_ASSERT(m_counter);
}

Object_Impl::~Object_Impl() {

  Object object(std::shared_ptr<Object_Impl>(this, Object_Impl::null_deleter()));
  object.increment();
}

void Object_Impl::increment() {
  ++(*m_counter);
}

void Object_Impl::incrementFromThis() {
  Object object(this->shared_from_this());
  object.increment();
}

}  // namespace sharedfromthistest
TEST(SharedFromThis, Destructor) {
  std::shared_ptr<int> counter(new int(0));
  ASSERT_TRUE(counter.get());
  EXPECT_EQ(0, *counter);

  {
    sharedfromthistest::Object object(std::shared_ptr<sharedfromthistest::Object_Impl>(new sharedfromthistest::Object_Impl(counter)));
    EXPECT_EQ(0, *counter);
    object.increment();
    EXPECT_EQ(1, *counter);
    object.incrementFromThis();
    EXPECT_EQ(2, *counter);
  }

  EXPECT_EQ(3, *counter);
}
