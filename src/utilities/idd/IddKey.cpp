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

#include "IddKey.hpp"
#include "IddKey_Impl.hpp"

#include "IddRegex.hpp"

namespace openstudio {

namespace detail {

  IddKey_Impl::IddKey_Impl() {}

  /// equality operator
  bool IddKey_Impl::operator==(const IddKey_Impl& other) const {
    return ((this == &other) || ((m_name == other.m_name) && (m_properties == other.m_properties)));
  }

  /// get name
  std::string IddKey_Impl::name() const {
    return m_name;
  }

  const IddKeyProperties& IddKey_Impl::properties() const {
    return m_properties;
  }

  std::shared_ptr<IddKey_Impl> IddKey_Impl::load(const std::string& name, const std::string& text) {

    std::shared_ptr<IddKey_Impl> result;
    IddKey_Impl iddKeyImpl(name);

    try {
      iddKeyImpl.parse(text);
    } catch (...) {
      return result;
    }

    result = std::shared_ptr<IddKey_Impl>(new IddKey_Impl(iddKeyImpl));
    return result;
  }

  std::ostream& IddKey_Impl::print(std::ostream& os) const {
    os << "       \\key " << m_name << std::endl;
    return os;
  }

  // PRIVATE

  IddKey_Impl::IddKey_Impl(const std::string& name) : m_name(name) {}

  void IddKey_Impl::parse(const std::string& text) {
    boost::smatch matches;
    if (boost::regex_search(text, matches, iddRegex::contentAndCommentLine())) {
      std::string keyName(matches[1].first, matches[1].second);
      boost::trim(keyName);
      if (!boost::equals(name(), keyName)) {
        LOG_AND_THROW("Key name '" << keyName << "' does not match expected '" << name() << "'");
      };

      std::string note(matches[2].first, matches[2].second);
      m_properties.note = note;
    } else {
      LOG_AND_THROW("Key name could not be determined from text '" << text << "'");
    }
  }

}  // namespace detail

IddKey::IddKey() : m_impl(std::shared_ptr<detail::IddKey_Impl>(new detail::IddKey_Impl())) {}

IddKey::IddKey(const IddKey& other) : m_impl(other.m_impl) {}

bool IddKey::operator==(const IddKey& other) const {
  return (*m_impl == *(other.m_impl));
}

bool IddKey::operator!=(const IddKey& other) const {
  return !(*m_impl == *(other.m_impl));
}

std::string IddKey::name() const {
  return m_impl->name();
}

const IddKeyProperties& IddKey::properties() const {
  return m_impl->properties();
}

OptionalIddKey IddKey::load(const std::string& name, const std::string& text) {
  std::shared_ptr<detail::IddKey_Impl> p = detail::IddKey_Impl::load(name, text);
  if (p) {
    return IddKey(p);
  } else {
    return boost::none;
  }
}

std::ostream& IddKey::print(std::ostream& os) const {
  return m_impl->print(os);
}

// PRIVATE

IddKey::IddKey(const std::shared_ptr<detail::IddKey_Impl>& impl) : m_impl(impl) {}

// NON-MEMBER FUNCTIONS

std::ostream& operator<<(std::ostream& os, const IddKey& iddKey) {
  return iddKey.print(os);
}

}  // namespace openstudio
