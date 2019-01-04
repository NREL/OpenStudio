/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "WorkspaceObjectWatcher.hpp"
#include "WorkspaceObject_Impl.hpp"

namespace openstudio {

WorkspaceObjectWatcher::WorkspaceObjectWatcher(const openstudio::WorkspaceObject& workspaceObject)
  : IdfObjectWatcher(workspaceObject.cast<IdfObject>()),
    m_relationshipChanged(false), m_removedFromWorkspace(false)
{
  detail::WorkspaceObject_ImplPtr objectImpl = workspaceObject.getImpl<detail::WorkspaceObject_Impl>();

  objectImpl.get()->detail::WorkspaceObject_Impl::onRelationshipChange.connect<WorkspaceObjectWatcher, &WorkspaceObjectWatcher::relationshipChange>(this);

  objectImpl.get()->detail::WorkspaceObject_Impl::onRemoveFromWorkspace.connect<WorkspaceObjectWatcher, &WorkspaceObjectWatcher::removedFromWorkspace>(this);
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

void WorkspaceObjectWatcher::onRelationshipChange(int index, Handle newHandle,  Handle oldHandle)
{
  // onChange will be emitted with onRelationshipChange, that will set dirty
}

void WorkspaceObjectWatcher::onRemoveFromWorkspace(Handle handle)
{
  // onChange will not be emitted with onRemoveFromWorkspace, dirty will not be set
}

void WorkspaceObjectWatcher::relationshipChange(int index, Handle newHandle, Handle oldHandle)
{
  m_relationshipChanged = true;

  if (enabled()){
    onRelationshipChange(index, newHandle, oldHandle);
  }
}

void WorkspaceObjectWatcher::removedFromWorkspace(const Handle &handle)
{
  m_removedFromWorkspace = true;

  if (enabled()){
    onRemoveFromWorkspace(handle);
  }
}

} // openstudio
