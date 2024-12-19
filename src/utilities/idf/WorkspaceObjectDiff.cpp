/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "WorkspaceObjectDiff.hpp"
#include "WorkspaceObjectDiff_Impl.hpp"

#include "../core/Assert.hpp"

namespace openstudio {
namespace detail {

  WorkspaceObjectDiff_Impl::WorkspaceObjectDiff_Impl(unsigned index, boost::optional<std::string> oldValue, boost::optional<std::string> newValue,
                                                     boost::optional<UUID> oldHandle, boost::optional<UUID> newHandle)
    : IdfObjectDiff_Impl(index, oldValue, newValue), m_oldHandle(oldHandle), m_newHandle(newHandle) {}

  boost::optional<UUID> WorkspaceObjectDiff_Impl::oldHandle() const {
    return m_oldHandle;
  }

  boost::optional<UUID> WorkspaceObjectDiff_Impl::newHandle() const {
    return m_newHandle;
  }

}  // namespace detail

WorkspaceObjectDiff::WorkspaceObjectDiff(unsigned index, boost::optional<std::string> oldValue, boost::optional<std::string> newValue,
                                         boost::optional<UUID> oldHandle, boost::optional<UUID> newHandle)
  : IdfObjectDiff(
      std::shared_ptr<detail::WorkspaceObjectDiff_Impl>(new detail::WorkspaceObjectDiff_Impl(index, oldValue, newValue, oldHandle, newHandle))) {
  OS_ASSERT(getImpl<detail::WorkspaceObjectDiff_Impl>());
}

WorkspaceObjectDiff::WorkspaceObjectDiff(const std::shared_ptr<detail::WorkspaceObjectDiff_Impl>& impl) : IdfObjectDiff(std::move(impl)) {
  OS_ASSERT(getImpl<detail::WorkspaceObjectDiff_Impl>());
}

boost::optional<UUID> WorkspaceObjectDiff::oldHandle() const {
  return getImpl<detail::WorkspaceObjectDiff_Impl>()->oldHandle();
}

boost::optional<UUID> WorkspaceObjectDiff::newHandle() const {
  return getImpl<detail::WorkspaceObjectDiff_Impl>()->newHandle();
}

}  // namespace openstudio
