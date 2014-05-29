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

#include "IddKey.hpp"
#include "IddKey_Impl.hpp"

#include "IddKeyProperties.hpp"
#include "IddRegex.hpp"

#include <boost/algorithm/string.hpp>

namespace openstudio {

namespace detail {

  IddKey_Impl::IddKey_Impl()
  {}

  /// equality operator
  bool IddKey_Impl::operator==(const IddKey_Impl& other) const {
    return ((this == &other) ||
            ((m_name == other.m_name) &&
             (m_properties == other.m_properties)));
  }

  /// get name
  std::string IddKey_Impl::name() const
  {
    return m_name;
  }

  const IddKeyProperties& IddKey_Impl::properties() const
  {
    return m_properties;
  }

  std::shared_ptr<IddKey_Impl> IddKey_Impl::load(const std::string& name,
                                                   const std::string& text) {

    std::shared_ptr<IddKey_Impl> result;
    IddKey_Impl iddKeyImpl(name);

    try { iddKeyImpl.parse(text); }
    catch (...) { return result; }

    result = std::shared_ptr<IddKey_Impl>(new IddKey_Impl(iddKeyImpl));
    return result;
  }

  std::ostream& IddKey_Impl::print(std::ostream& os) const
  {
    os << "       \\key " << m_name << std::endl;
    return os;
  }

  // PRIVATE

  IddKey_Impl::IddKey_Impl(const std::string& name) : m_name(name) {}

  void IddKey_Impl::parse(const std::string& text)
  {
    boost::smatch matches;
    if (boost::regex_search(text, matches, iddRegex::contentAndCommentLine())){
      std::string keyName(matches[1].first, matches[1].second); boost::trim(keyName);
      if (!boost::equals(name(), keyName)) {
        LOG_AND_THROW("Key name '" << keyName << "' does not match expected '" << name() << "'");
      };

      std::string note(matches[2].first, matches[2].second);
      m_properties.note = note;
    }else{
      LOG_AND_THROW("Key name could not be determined from text '" << text << "'");
    }
  }


} // detail

IddKey::IddKey()
{
  m_impl = std::shared_ptr<detail::IddKey_Impl>(new detail::IddKey_Impl());
}

IddKey::IddKey(const IddKey& other)
  : m_impl(other.m_impl)
{}

bool IddKey::operator==(const IddKey& other) const {
  return (*m_impl == *(other.m_impl));
}

bool IddKey::operator!=(const IddKey& other) const {
  return !(*m_impl == *(other.m_impl));
}

std::string IddKey::name() const
{
  return m_impl->name();
}

const IddKeyProperties& IddKey::properties() const
{
  return m_impl->properties();
}

OptionalIddKey IddKey::load(const std::string& name, const std::string& text) {
  std::shared_ptr<detail::IddKey_Impl> p = detail::IddKey_Impl::load(name,text);
  if (p) { return IddKey(p); }
  else { return boost::none; }
}

std::ostream& IddKey::print(std::ostream& os) const
{
  return m_impl->print(os);
}

// PRIVATE

IddKey::IddKey(const std::shared_ptr<detail::IddKey_Impl>& impl) : m_impl(impl) {}

// NON-MEMBER FUNCTIONS

std::ostream& operator<<(std::ostream& os, const IddKey& iddKey)
{
  return iddKey.print(os);
}

} // openstudio
