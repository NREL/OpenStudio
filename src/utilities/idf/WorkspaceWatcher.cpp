/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "WorkspaceWatcher.hpp"
#include "Workspace_Impl.hpp"
#include "../core/Assert.hpp"

namespace openstudio {

WorkspaceWatcher::WorkspaceWatcher(const Workspace& workspace) : m_enabled(true), m_dirty(false), m_objectAdded(false), m_objectRemoved(false) {
  detail::Workspace_ImplPtr wsImpl = workspace.getImpl<detail::Workspace_Impl>();
  wsImpl.get()->detail::Workspace_Impl::onChange.connect<WorkspaceWatcher, &WorkspaceWatcher::change>(this);

  // ideally this would be a queued connection so objects can be initialized before signal is processed
  wsImpl.get()->detail::Workspace_Impl::addWorkspaceObject.connect<WorkspaceWatcher, &WorkspaceWatcher::objectAdd>(this);

  // this signal happens immediately
  wsImpl.get()->detail::Workspace_Impl::removeWorkspaceObject.connect<WorkspaceWatcher, &WorkspaceWatcher::objectRemove>(this);
}

bool WorkspaceWatcher::enabled() const {
  return m_enabled;
}

void WorkspaceWatcher::enable() {
  m_enabled = true;
}

bool WorkspaceWatcher::disable() {
  bool result = m_enabled;
  m_enabled = false;
  return result;
}

bool WorkspaceWatcher::dirty() const {
  return m_dirty;
}

bool WorkspaceWatcher::objectAdded() const {
  return m_objectAdded;
}

bool WorkspaceWatcher::objectRemoved() const {
  return m_objectRemoved;
}

void WorkspaceWatcher::clearState() {
  bool wasDirty = m_dirty;
  m_dirty = false;
  m_objectAdded = false;
  m_objectRemoved = false;

  if (enabled()) {
    if (wasDirty) {
      onBecomeClean();
    }
  }
}

void WorkspaceWatcher::onChangeWorkspace() {}

void WorkspaceWatcher::onBecomeDirty() {}

void WorkspaceWatcher::onBecomeClean() {}

void WorkspaceWatcher::onObjectAdd(const WorkspaceObject& addedObject) {
  OS_ASSERT(addedObject.initialized());
  OS_ASSERT(addedObject.workspace().isMember(addedObject.handle()));
}

void WorkspaceWatcher::onObjectRemove(const WorkspaceObject& removedObject) {
  OS_ASSERT(removedObject.initialized());
  OS_ASSERT(removedObject.workspace().isMember(removedObject.handle()));
}

void WorkspaceWatcher::change() {
  bool wasDirty = m_dirty;
  m_dirty = true;

  if (enabled()) {
    if (!wasDirty) {
      onBecomeDirty();
    }
    onChangeWorkspace();
  }
}

void WorkspaceWatcher::objectAdd(const WorkspaceObject& addedObject, const openstudio::IddObjectType& /*type*/, const openstudio::UUID& /*uuid*/) {
  // Note: Args 2 & 3 are simply to comply with Nano::Signal template parameters
  // let change() handle m_dirty and onChangeWorkspace();
  m_objectAdded = true;

  if (enabled()) {
    if (!addedObject.handle().isNull()) {
      this->onObjectAdd(addedObject);
    }
  }
}

void WorkspaceWatcher::objectRemove(const WorkspaceObject& removedObject, const openstudio::IddObjectType& /*type*/,
                                    const openstudio::UUID& /*uuid*/) {
  // Note: Args 2 & 3 are simply to comply with Nano::Signal template parameters
  // let change() handle m_dirty and onChangeWorkspace();
  m_objectRemoved = true;

  if (enabled()) {
    this->onObjectRemove(removedObject);
  }
}

}  // namespace openstudio
