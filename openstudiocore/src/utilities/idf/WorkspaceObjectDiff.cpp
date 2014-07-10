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

#include "WorkspaceObjectDiff.hpp"
#include "WorkspaceObjectDiff_Impl.hpp"

#include "../core/Assert.hpp"

namespace openstudio {
  namespace detail {

    WorkspaceObjectDiff_Impl::WorkspaceObjectDiff_Impl(unsigned index, boost::optional<std::string> oldValue, boost::optional<std::string> newValue,
                                                       boost::optional<UUID> oldHandle, boost::optional<UUID> newHandle)
      : IdfObjectDiff_Impl(index, oldValue, newValue),
        m_oldHandle(oldHandle), m_newHandle(newHandle)
    {
    }

    boost::optional<UUID> WorkspaceObjectDiff_Impl::oldHandle() const
    {
      return m_oldHandle;
    }

    boost::optional<UUID> WorkspaceObjectDiff_Impl::newHandle() const
    {
      return m_newHandle;
    }

  }

  WorkspaceObjectDiff::WorkspaceObjectDiff(unsigned index, boost::optional<std::string> oldValue, boost::optional<std::string> newValue,
                                           boost::optional<UUID> oldHandle, boost::optional<UUID> newHandle)
    : IdfObjectDiff(std::shared_ptr<detail::WorkspaceObjectDiff_Impl>(new detail::WorkspaceObjectDiff_Impl(index, oldValue, newValue, oldHandle, newHandle)))
  {
    OS_ASSERT(getImpl<detail::WorkspaceObjectDiff_Impl>());
  }

  WorkspaceObjectDiff::WorkspaceObjectDiff(const std::shared_ptr<detail::WorkspaceObjectDiff_Impl>& impl)
    : IdfObjectDiff(impl)
  {
    OS_ASSERT(getImpl<detail::WorkspaceObjectDiff_Impl>());
  }

  boost::optional<UUID> WorkspaceObjectDiff::oldHandle() const
  {
    return getImpl<detail::WorkspaceObjectDiff_Impl>()->oldHandle();
  }

  boost::optional<UUID> WorkspaceObjectDiff::newHandle() const
  {
    return getImpl<detail::WorkspaceObjectDiff_Impl>()->newHandle();
  }

}
