/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "CoreFixture.hpp"
#include "../Logger.hpp"

#include "../Containers.hpp"
#include "../ContainersMove.hpp"

#include <resources.hxx>
#include <string>
#include <string_view>
#include <memory>  // for make_shared, shared_ptr

using openstudio::StringVector;
using openstudio::eraseEmptyElements;

TEST_F(CoreFixture, StringVector) {
  StringVector sv;
  sv.push_back("");
  sv.push_back("Hello");
  sv.push_back("Guten Tag");
  sv.push_back("");
  sv.push_back("Bonjour");
  sv = eraseEmptyElements(sv);
  ASSERT_EQ(static_cast<unsigned>(3), sv.size());
  EXPECT_EQ("Hello", sv[0]);
  EXPECT_EQ("Guten Tag", sv[1]);
  EXPECT_EQ("Bonjour", sv[2]);
}

namespace detail {
class X_Impl
{
 public:
  X_Impl() {
    ++count;
  };
  X_Impl(std::string t_i) : i(std::move(t_i)) {
    ++count;
  }
  virtual ~X_Impl() {
    --count;
  }

  static size_t howMany() {
    return count;
  }

  [[nodiscard]] std::string get_i() const {
    return i;
  };
  bool set_i(const std::string& t_i) {
    i = t_i;
    return true;
  };

 private:
  std::string i;
  std::string j;
  std::string k;
  std::string l;
  static size_t count;
};
// obligatory definition of count. This goes in an implementation file
size_t X_Impl::count = 0;

class Child_Impl : public X_Impl
{
 public:
  Child_Impl(std::string t_i) : X_Impl(std::move(t_i)) {}
};
}  // namespace detail

class X
{
 public:
  X() : m_impl(std::make_shared<detail::X_Impl>()) {
    LOG(Debug, "Default Ctor");
    ++count;
  };

  X(std::string i) : m_impl(std::make_shared<detail::X_Impl>(std::move(i))) {
    LOG(Debug, "Ctor i");
    ++count;
  };

 protected:
  X(std::shared_ptr<detail::X_Impl> impl) : m_impl(std::move(impl)) {
    LOG(Debug, "X from impl Ctor");
    ++count;
  }

 public:
  X(X const& other) : m_impl(other.m_impl) {
    LOG(Debug, "Copy Ctor");
    ++copied;
    ++count;
  };
  X& operator=(X const& other) {
    if (this == &other) {
      return *this;
    }
    m_impl = other.m_impl;
    LOG(Debug, "Copy Assignment Ctor");
    ++copied;
    ++count;
    return *this;
  };

  X(X&& other) noexcept : m_impl(std::move(other.m_impl)) {
    LOG(Debug, "Move Ctor");
    ++moved;
    ++count;
  };
  X& operator=(X&& other) noexcept {
    m_impl = std::move(other.m_impl);
    LOG(Debug, "Move Assignment Ctor");
    ++moved;
    ++count;
    return *this;
  };

  static size_t howMany() {
    return count - moved;
  }

  static size_t howManyCopied() {
    return copied;
  }

  static size_t howManyMoved() {
    return moved;
  }

  [[nodiscard]] std::string get_i() const {
    return m_impl->get_i();
  };
  bool set_i(const std::string& t_i) {
    return m_impl->set_i(t_i);
  };

  virtual ~X() {
    LOG(Debug, "Destructor");
    --count;
  };

  template <typename T>
  std::shared_ptr<T> getImpl() const {
    return std::dynamic_pointer_cast<T>(m_impl);
  }

  /// cast to type T, can throw std::bad_cast
  template <typename T>
  T cast() const {
    std::shared_ptr<typename T::ImplType> impl = this->getImpl<typename T::ImplType>();
    if (!impl) {
      throw(std::bad_cast());
    }
    return T(std::move(impl));
  }

 protected:
  using ImplType = detail::X_Impl;

 private:
  //  Comment that out, and WithoutReference is faster
  std::shared_ptr<detail::X_Impl> m_impl;
  static size_t count;
  static size_t copied;
  static size_t moved;
  REGISTER_LOGGER("X");
};

// obligatory definition of count. This goes in an implementation file
size_t X::count = 0;
size_t X::copied = 0;
size_t X::moved = 0;

class Child : public X
{
 public:
  Child(std::string i) : X(std::make_shared<detail::Child_Impl>(std::move(i))) {
    LOG(Debug, "Child Ctor i");
  };

 protected:
  using ImplType = detail::Child_Impl;

 private:
  REGISTER_LOGGER("Child");
};

TEST_F(CoreFixture, Concat) {

  EXPECT_EQ(0, detail::X_Impl::howMany());
  EXPECT_EQ(0, X::howMany());
  LOG_FREE(Debug, "Concat", "vec1!");
  X x1("x1");
  std::vector<X> vec1;
  vec1.reserve(2);
  vec1.emplace_back(std::move(x1));
  vec1.emplace_back("x2");
  size_t s1 = vec1.size();

  EXPECT_EQ(s1, detail::X_Impl::howMany());
  EXPECT_EQ(s1, X::howMany());
  EXPECT_EQ(0, X::howManyCopied());
  EXPECT_EQ(1, X::howManyMoved());

  // Doing this, it's copied then immediately deleted
  LOG_FREE(Debug, "Concat", "vec2!");
  std::vector<Child> vec2{{"y1"}, {"y2"}, {"y3"}};
  size_t s2 = vec2.size();
  EXPECT_EQ(s1 + s2, detail::X_Impl::howMany());
  EXPECT_EQ(s1 + s2, X::howMany());
  EXPECT_EQ(3, X::howManyCopied());
  EXPECT_EQ(1, X::howManyMoved());

  LOG_FREE(Debug, "Concat", "vec3!");
  std::vector<X> vec3{{"z1"}};
  size_t s3 = vec3.size();
  EXPECT_EQ(s1 + s2 + s3, detail::X_Impl::howMany());
  EXPECT_EQ(s1 + s2 + s3, X::howMany());
  EXPECT_EQ(4, X::howManyCopied());
  EXPECT_EQ(1, X::howManyMoved());

  LOG_FREE(Debug, "Concat", "vec4!");
  std::vector<Child> vec4;
  vec4.reserve(2);
  vec4.emplace_back("a1");
  vec4.emplace_back("a2");
  size_t s4 = vec4.size();
  EXPECT_EQ(s1 + s2 + s3 + s4, detail::X_Impl::howMany());
  EXPECT_EQ(s1 + s2 + s3 + s4, X::howMany());
  size_t copied = 4;
  size_t moved = 1;
  EXPECT_EQ(copied, X::howManyCopied());
  EXPECT_EQ(moved, X::howManyMoved());

  LOG_FREE(Debug, "Concat", "concat!");
  [[maybe_unused]] auto t = openstudio::concat<X>(vec1, std::move(vec2), vec3, std::move(vec4));
  EXPECT_EQ(s1 + s2 + s3 + s4, detail::X_Impl::howMany());
  EXPECT_EQ(s1 + s2 + s3 + s4 + s1 + s3, X::howMany());
  EXPECT_EQ(copied + s1 + s3, X::howManyCopied());
  EXPECT_EQ(moved + s2 + s4, X::howManyMoved());
}
