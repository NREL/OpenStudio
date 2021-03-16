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

  ~Object() {}

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
