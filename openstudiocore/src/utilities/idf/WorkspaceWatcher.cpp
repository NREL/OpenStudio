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


#include "WorkspaceWatcher.hpp"
#include "Workspace_Impl.hpp"
#include "../core/Application.hpp"
#include "../core/Assert.hpp"

#include <QTimer>

namespace openstudio {

WorkspaceWatcher::WorkspaceWatcher(const Workspace& work)
  : m_enabled(true), m_dirty(false), m_objectAdded(false), m_objectRemoved(false)
{
  // make sure a QApplication exists
  openstudio::Application::instance().application();

  detail::Workspace_ImplPtr wsImpl = work.getImpl<detail::Workspace_Impl>();
  bool connected = connect(wsImpl.get(), SIGNAL(onChange()), SLOT(change()));
  OS_ASSERT(connected);

  // ideally this would be a queued connection so objects can be initialized before signal is processed
  // However, WorkspaceObject is not a QObject.  Instead we use QTimer to give a delay.
  connected = connected && connect(wsImpl.get(),
      SIGNAL(addWorkspaceObject(const WorkspaceObject&, const openstudio::IddObjectType&, const openstudio::UUID&)),
      SLOT(objectAdd(const WorkspaceObject&)));
  OS_ASSERT(connected);

  // this signal happens immediately
  connected = connected && connect(wsImpl.get(),
      SIGNAL(removeWorkspaceObject(const WorkspaceObject&, const openstudio::IddObjectType&, const openstudio::UUID&)),
      SLOT(objectRemove(const WorkspaceObject&)));
  OS_ASSERT(connected);
}

WorkspaceWatcher::~WorkspaceWatcher()
{
}

bool WorkspaceWatcher::enabled() const
{
  return m_enabled;
}

void WorkspaceWatcher::enable()
{
  m_enabled = true;
}

bool WorkspaceWatcher::disable()
{
  bool result = m_enabled;
  m_enabled = false;
  return result;
}

bool WorkspaceWatcher::dirty() const
{
  return m_dirty;
}

bool WorkspaceWatcher::objectAdded() const
{
  return m_objectAdded;
}

bool WorkspaceWatcher::objectRemoved() const
{
  return m_objectRemoved;
}

void WorkspaceWatcher::clearState()
{
  bool wasDirty = m_dirty;
  m_dirty = false;
  m_objectAdded = false;
  m_objectRemoved = false;

  if (enabled()){
    if (wasDirty){
      onBecomeClean();
    }
  }
}

void WorkspaceWatcher::onChangeWorkspace()
{
}

void WorkspaceWatcher::onBecomeDirty()
{
}

void WorkspaceWatcher::onBecomeClean()
{
}

void WorkspaceWatcher::onObjectAdd(const WorkspaceObject& addedObject)
{
  OS_ASSERT(addedObject.initialized());
  OS_ASSERT(addedObject.workspace().isMember(addedObject.handle()));
}

void WorkspaceWatcher::onObjectRemove(const WorkspaceObject& removedObject)
{
  OS_ASSERT(removedObject.initialized());
  OS_ASSERT(removedObject.workspace().isMember(removedObject.handle()));
}

void WorkspaceWatcher::change()
{
  bool wasDirty = m_dirty;
  m_dirty = true;

  if (enabled()){
    if (!wasDirty){
      onBecomeDirty();
    }
    onChangeWorkspace();
  }
}

void WorkspaceWatcher::objectAdd(const WorkspaceObject& addedObject)
{
  // let change() handle m_dirty and onChangeWorkspace();
  m_objectAdded = true;

  if (enabled()){
    m_addedObjects.push_back(addedObject);
    QTimer::singleShot(0,this,SLOT(processAddedObjects()));
  }
}

void WorkspaceWatcher::processAddedObjects()
{
  std::vector<WorkspaceObject> addedObjects;
  std::swap(m_addedObjects, addedObjects);
  OS_ASSERT(m_addedObjects.empty());

  for (const WorkspaceObject& addedObject : addedObjects){
    // check for the case where object has been removed before the add was processed
    if (!addedObject.handle().isNull()){
      this->onObjectAdd(addedObject);
    }
  }

}

void WorkspaceWatcher::objectRemove(const WorkspaceObject& removedObject)
{
  // let change() handle m_dirty and onChangeWorkspace();
  m_objectRemoved = true;

  if (enabled()){
    this->onObjectRemove(removedObject);
  }
}

} // openstudio
