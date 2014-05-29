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

#include "IdfObjectDiff.hpp"
#include "IdfObjectDiff_Impl.hpp"

#include "../core/Assert.hpp"

namespace openstudio {
  namespace detail {

    IdfObjectDiff_Impl::IdfObjectDiff_Impl()
    {}

    IdfObjectDiff_Impl::IdfObjectDiff_Impl(boost::optional<unsigned> index, boost::optional<std::string> oldValue, boost::optional<std::string> newValue)
      : m_index(index), m_oldValue(oldValue), m_newValue(newValue)
    {}

    IdfObjectDiff_Impl::~IdfObjectDiff_Impl()
    {}

    boost::optional<unsigned> IdfObjectDiff_Impl::index() const
    {
      return m_index;
    }

    bool IdfObjectDiff_Impl::isNull() const
    {
      bool result = true;
      if (m_oldValue && m_newValue){
        result = (m_oldValue.get() == m_newValue.get());
      }else{
        result = (m_oldValue == m_newValue);
      }
      return result;
    }

    boost::optional<std::string> IdfObjectDiff_Impl::oldValue() const
    {
      return m_oldValue;
    }

    boost::optional<std::string> IdfObjectDiff_Impl::newValue() const
    {
      return m_newValue;
    }

  } // detail

  IdfObjectDiff::IdfObjectDiff()
    : m_impl(std::shared_ptr<detail::IdfObjectDiff_Impl>(new detail::IdfObjectDiff_Impl()))
  {
    OS_ASSERT(m_impl);
  }

  IdfObjectDiff::IdfObjectDiff(boost::optional<unsigned> index, boost::optional<std::string> oldValue, boost::optional<std::string> newValue)
    : m_impl(std::shared_ptr<detail::IdfObjectDiff_Impl>(new detail::IdfObjectDiff_Impl(index, oldValue, newValue)))
  {
    OS_ASSERT(m_impl);
  }

  IdfObjectDiff::IdfObjectDiff(const std::shared_ptr<detail::IdfObjectDiff_Impl>& impl)
    : m_impl(impl)
  {
    OS_ASSERT(m_impl);
  }

  boost::optional<unsigned> IdfObjectDiff::index() const
  {
    return m_impl->index();
  }

  bool IdfObjectDiff::isNull() const
  {
    return m_impl->isNull();
  }

  boost::optional<std::string> IdfObjectDiff::oldValue() const
  {
    return m_impl->oldValue();
  }

  boost::optional<std::string> IdfObjectDiff::newValue() const
  {
    return m_impl->newValue();
  }

} // openstudio
