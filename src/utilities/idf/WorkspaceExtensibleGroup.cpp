/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "WorkspaceExtensibleGroup.hpp"
#include "WorkspaceObject.hpp"
#include "WorkspaceObject_Impl.hpp"

using std::shared_ptr;
using std::dynamic_pointer_cast;

namespace openstudio {

// GETTERS

OptionalWorkspaceObject WorkspaceExtensibleGroup::getTarget(unsigned fieldIndex) const {
  if (!isValid(fieldIndex)) {
    return boost::none;
  }
  return getImpl<detail::WorkspaceObject_Impl>()->getTarget(mf_toIndex(fieldIndex));
}

WorkspaceObjectVector WorkspaceExtensibleGroup::getTargets() const {
  WorkspaceObjectVector result;
  if (empty()) {
    return result;
  }
  for (unsigned fieldIndex = 0, n = numFields(); fieldIndex < n; ++fieldIndex) {
    OptionalWorkspaceObject owo = getTarget(fieldIndex);
    if (owo) {
      result.push_back(*owo);
    }
  }
  return result;
}

std::vector<unsigned> WorkspaceExtensibleGroup::getSourceFieldIndices(const Handle& targetHandle) const {
  UnsignedVector result;
  if (empty()) {
    return result;
  }
  UnsignedVector candidates = dynamic_pointer_cast<detail::WorkspaceObject_Impl>(m_impl)->getSourceIndices(targetHandle);
  result = mf_subsetAndToFieldIndices(candidates);
  return result;
}

// SETTERS
bool WorkspaceExtensibleGroup::setPointer(unsigned fieldIndex, const Handle& targetHandle) {
  return setPointer(fieldIndex, targetHandle, true);
}

bool WorkspaceExtensibleGroup::setPointer(unsigned fieldIndex, const Handle& targetHandle, bool checkValidity) {
  if (checkValidity) {
    if (!isValid(fieldIndex)) {
      return false;
    }
  }
  return getImpl<detail::WorkspaceObject_Impl>()->setPointer(mf_toIndex(fieldIndex), targetHandle, checkValidity);
}

// QUERIES

bool WorkspaceExtensibleGroup::isSource() const {
  bool result = false;
  if (empty()) {
    return result;
  }
  for (unsigned fieldIndex = 0, n = numFields(); fieldIndex < n; ++fieldIndex) {
    result = getImpl<detail::WorkspaceObject_Impl>()->isSource(mf_toIndex(fieldIndex));
    if (result) {
      break;
    }
  }
  return result;
}

bool WorkspaceExtensibleGroup::canBeSource() const {
  bool result = false;
  if (empty()) {
    return result;
  }
  for (unsigned fieldIndex = 0, n = numFields(); fieldIndex < n; ++fieldIndex) {
    result = getImpl<detail::WorkspaceObject_Impl>()->canBeSource(mf_toIndex(fieldIndex));
    if (result) {
      break;
    }
  }
  return result;
}

// PROTECTED
WorkspaceExtensibleGroup::WorkspaceExtensibleGroup(std::shared_ptr<detail::WorkspaceObject_Impl> impl, unsigned index)
  : IdfExtensibleGroup(impl, index) {}

}  // namespace openstudio
