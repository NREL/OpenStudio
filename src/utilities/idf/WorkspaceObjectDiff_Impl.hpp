/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_IDF_WORKSPACEOBJECTDIFF_IMPL_HPP
#define UTILITIES_IDF_WORKSPACEOBJECTDIFF_IMPL_HPP

#include "IdfObjectDiff_Impl.hpp"

#include "../UtilitiesAPI.hpp"

#include "../core/UUID.hpp"

namespace openstudio {
namespace detail {

  class UTILITIES_API WorkspaceObjectDiff_Impl : public IdfObjectDiff_Impl
  {
   public:
    WorkspaceObjectDiff_Impl(unsigned index, boost::optional<std::string> oldValue, boost::optional<std::string> newValue,
                             boost::optional<UUID> oldHandle, boost::optional<UUID> newHandle);

    /// get the old handle if there was one
    boost::optional<UUID> oldHandle() const;

    /// get the new handle if there is one
    boost::optional<UUID> newHandle() const;

   private:
    boost::optional<UUID> m_oldHandle;
    boost::optional<UUID> m_newHandle;
  };

}  // namespace detail
}  // namespace openstudio

#endif  // UTILITIES_IDF_WORKSPACEOBJECTDIFF_IMPL_HPP
