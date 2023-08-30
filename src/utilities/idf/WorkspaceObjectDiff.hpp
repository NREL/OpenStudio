/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_IDF_WORKSPACEOBJECTDIFF_HPP
#define UTILITIES_IDF_WORKSPACEOBJECTDIFF_HPP

#include "IdfObjectDiff.hpp"

#include "../UtilitiesAPI.hpp"

#include "../core/UUID.hpp"

namespace openstudio {
namespace detail {
  class WorkspaceObjectDiff_Impl;
}

/** WorkspaceObjectDiff represents a change to an WorkspaceObjectDiff.
**/
class UTILITIES_API WorkspaceObjectDiff : public IdfObjectDiff
{
 public:
  WorkspaceObjectDiff(unsigned index, boost::optional<std::string> oldValue, boost::optional<std::string> newValue, boost::optional<UUID> oldHandle,
                      boost::optional<UUID> newHandle);

  /// get the old handle if there was one, use oldValue to determine if field existed or not
  boost::optional<UUID> oldHandle() const;

  /// get the new handle if there is one, use newValue to determine if field exists or not
  boost::optional<UUID> newHandle() const;

 private:
  friend class IdfObjectDiff;

  using ImplType = detail::WorkspaceObjectDiff_Impl;

  WorkspaceObjectDiff(const std::shared_ptr<detail::WorkspaceObjectDiff_Impl>& impl);
};

}  // namespace openstudio

#endif  // UTILITIES_IDF_WORKSPACEOBJECTDIFF_HPP
