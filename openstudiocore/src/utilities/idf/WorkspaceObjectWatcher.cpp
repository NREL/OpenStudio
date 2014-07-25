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

#include "WorkspaceObjectWatcher.hpp"
#include "WorkspaceObject.hpp"
#include "WorkspaceObject_Impl.hpp"
#include "../core/Assert.hpp"

namespace openstudio {

WorkspaceObjectWatcher::WorkspaceObjectWatcher(const openstudio::WorkspaceObject& workspaceObject)
  : IdfObjectWatcher(workspaceObject.cast<IdfObject>()), 
    m_relationshipChanged(false), m_removedFromWorkspace(false)
{
  detail::WorkspaceObject_ImplPtr objectImpl = workspaceObject.getImpl<detail::WorkspaceObject_Impl>();
 
  bool connected = this->connect(objectImpl.get(), 
      SIGNAL(onRelationshipChange(int,Handle,Handle)), 
      SLOT(relationshipChange(int,Handle,Handle)));
  OS_ASSERT(connected);

  connected = this->connect(objectImpl.get(), 
      SIGNAL(onRemoveFromWorkspace(Handle)), 
      SLOT(removedFromWorkspace(Handle)));
  OS_ASSERT(connected);
}

WorkspaceObjectWatcher::~WorkspaceObjectWatcher() {}

bool WorkspaceObjectWatcher::relationshipChanged() const 
{
  return m_relationshipChanged;
}

bool WorkspaceObjectWatcher::removedFromWorkspace() const 
{
  return m_removedFromWorkspace;
}

void WorkspaceObjectWatcher::clearState() 
{
  m_relationshipChanged = false;
  m_removedFromWorkspace = false;
  IdfObjectWatcher::clearState();
}

void WorkspaceObjectWatcher::onRelationshipChange(int index, Handle newHandle,Handle oldHandle) 
{
  // onChange will be emitted with onRelationshipChange, that will set dirty
}

void WorkspaceObjectWatcher::onRemoveFromWorkspace(Handle handle) 
{
  // onChange will not be emitted with onRemoveFromWorkspace, dirty will not be set
}

void WorkspaceObjectWatcher::relationshipChange(int index,Handle newHandle,Handle oldHandle) 
{
  m_relationshipChanged = true;

  if (enabled()){
    onRelationshipChange(index,newHandle,oldHandle);
  }
}

void WorkspaceObjectWatcher::removedFromWorkspace(Handle handle)
{
  m_removedFromWorkspace = true;

  if (enabled()){  
    onRemoveFromWorkspace(handle);
  }
}

} // openstudio
