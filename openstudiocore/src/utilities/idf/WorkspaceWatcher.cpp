/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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
  openstudio::Application::instance().application(false);

  detail::Workspace_ImplPtr wsImpl = work.getImpl<detail::Workspace_Impl>();
  connect(wsImpl.get(), &detail::Workspace_Impl::onChange, this, &WorkspaceWatcher::change);

  // ideally this would be a queued connection so objects can be initialized before signal is processed
  // However, WorkspaceObject is not a QObject.  Instead we use QTimer to give a delay.
  connect(wsImpl.get(),
    static_cast<void (detail::Workspace_Impl::*)(const WorkspaceObject &, const IddObjectType &, const UUID &) const>(&detail::Workspace_Impl::addWorkspaceObject),
    this,
    &WorkspaceWatcher::objectAdd);

  // this signal happens immediately
  connect(wsImpl.get(),
    static_cast<void (detail::Workspace_Impl::*)(const WorkspaceObject &, const IddObjectType &, const UUID &) const>(&detail::Workspace_Impl::removeWorkspaceObject),
    this,
    &WorkspaceWatcher::objectRemove);
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
