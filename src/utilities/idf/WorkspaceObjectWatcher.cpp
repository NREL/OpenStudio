/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "WorkspaceObjectWatcher.hpp"
#include "WorkspaceObject_Impl.hpp"

namespace openstudio {

WorkspaceObjectWatcher::WorkspaceObjectWatcher(const openstudio::WorkspaceObject& workspaceObject)
  : IdfObjectWatcher(workspaceObject.cast<IdfObject>()), m_relationshipChanged(false), m_removedFromWorkspace(false) {
  detail::WorkspaceObject_ImplPtr objectImpl = workspaceObject.getImpl<detail::WorkspaceObject_Impl>();

  objectImpl.get()->detail::WorkspaceObject_Impl::onRelationshipChange.connect<WorkspaceObjectWatcher, &WorkspaceObjectWatcher::relationshipChange>(
    this);

  objectImpl.get()
    ->detail::WorkspaceObject_Impl::onRemoveFromWorkspace.connect<WorkspaceObjectWatcher, &WorkspaceObjectWatcher::removedFromWorkspace>(this);
}

bool WorkspaceObjectWatcher::relationshipChanged() const {
  return m_relationshipChanged;
}

bool WorkspaceObjectWatcher::removedFromWorkspace() const {
  return m_removedFromWorkspace;
}

void WorkspaceObjectWatcher::clearState() {
  m_relationshipChanged = false;
  m_removedFromWorkspace = false;
  IdfObjectWatcher::clearState();
}

void WorkspaceObjectWatcher::onRelationshipChange(int index, Handle newHandle, Handle oldHandle) {
  // onChange will be emitted with onRelationshipChange, that will set dirty
}

void WorkspaceObjectWatcher::onRemoveFromWorkspace(Handle handle) {
  // onChange will not be emitted with onRemoveFromWorkspace, dirty will not be set
}

void WorkspaceObjectWatcher::relationshipChange(int index, Handle newHandle, Handle oldHandle) {
  m_relationshipChanged = true;

  if (enabled()) {
    onRelationshipChange(index, newHandle, oldHandle);
  }
}

void WorkspaceObjectWatcher::removedFromWorkspace(const Handle& handle) {
  m_removedFromWorkspace = true;

  if (enabled()) {
    onRemoveFromWorkspace(handle);
  }
}

}  // namespace openstudio
