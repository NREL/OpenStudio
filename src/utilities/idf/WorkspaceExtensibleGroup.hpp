/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_IDF_WORKSPACEEXTENSIBLEGROUP_HPP
#define UTILITIES_IDF_WORKSPACEEXTENSIBLEGROUP_HPP

#include "../UtilitiesAPI.hpp"
#include "../idd/ExtensibleIndex.hpp"
#include "IdfExtensibleGroup.hpp"

#include "../core/Logger.hpp"
#include "../core/Optional.hpp"
#include "../core/Containers.hpp"

#include <boost/optional.hpp>

namespace openstudio {

namespace detail {
  class WorkspaceObject_Impl;
}
class WorkspaceObject;

/** WorkspaceExtensibleGroup wraps a set of extensible fields in a WorkspaceObject. */
class UTILITIES_API WorkspaceExtensibleGroup : public IdfExtensibleGroup
{
 public:
  virtual ~WorkspaceExtensibleGroup() override = default;

  /** @name Getters */
  //@{

  /** Returns the object pointed to by field index, if it exists. */
  boost::optional<WorkspaceObject> getTarget(unsigned fieldIndex) const;

  /** Returns all objects pointed to by this object. */
  std::vector<WorkspaceObject> getTargets() const;

  /** Returns the fieldIndex of the field in this extensible group that points to targetHandle,
   *  if possible. */
  std::vector<unsigned> getSourceFieldIndices(const Handle& targetHandle) const;

  //@}
  /** @name Setters */
  //@{

  /** Sets the pointer at fieldIndex to point to target, if possible. The field must be of
   *  object-list type, and target must be valid (in the containing Workspace, and if the strictness
   *  is greater than enums::None, of a proper type). */
  bool setPointer(unsigned fieldIndex, const Handle& targetHandle);

  bool setPointer(unsigned fieldIndex, const Handle& targetHandle, bool checkValidity);

  //@}
  /** @name Queries */
  //@{

  /** Returns true if this ExtensibleGroup points to another object. To qualify, there must be at
   *  least one pointer field holding a non-null pointer. */
  bool isSource() const;

  /** Returns true if this object can point to another object. To qualify, there must be at least
   *  one object-list field. */
  bool canBeSource() const;

  //@}

 protected:
  using ImplType = detail::WorkspaceObject_Impl;

  friend class IdfExtensibleGroup;

  WorkspaceExtensibleGroup(std::shared_ptr<detail::WorkspaceObject_Impl> impl, unsigned index);

 private:
  /** Private default constructor. */
  WorkspaceExtensibleGroup();

  REGISTER_LOGGER("openstudio.WorkspaceExtensibleGroup");
};

}  // namespace openstudio

#endif  // UTILITIES_IDF_WORKSPACEEXTENSIBLEGROUP_HPP
