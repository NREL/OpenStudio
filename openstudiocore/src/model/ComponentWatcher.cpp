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

#include "ComponentWatcher.hpp"
#include "ComponentWatcher_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"

#include "ModelObject.hpp"
#include "ModelObject_Impl.hpp"

#include "ComponentData_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Compare.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ComponentWatcher_Impl::ComponentWatcher_Impl(ComponentData& componentData)
    : m_componentData(componentData)
  {
    // get componentObjects and connect signals
    ModelObject_ImplPtr implPtr;
    if (componentData.numComponentObjects() < 1) {
      LOG_AND_THROW("Invalid ComponentData object--no objects listed in contents.");
    }
    for (unsigned i = 0, n = componentData.numComponentObjects(); i < n; ++i) {
      // get object
      OptionalModelObject omo = componentData.getComponentObject(i);
      if (!omo) {
        LOG_AND_THROW("Invalid ComponentData object--blank entry in contents list.");
      }
      ModelObject object = *omo;
      m_componentObjects.push_back(object);
      // connect to object's signals
      implPtr = object.getImpl<ModelObject_Impl>();
      // data change
      bool connected = connect(implPtr.get(),SIGNAL(onDataChange()),SLOT(dataChange()));
      if (!connected) {
        LOG_AND_THROW("Cannot connect onDataChange() signal.");
      }
      // name change
      connected = connect(implPtr.get(),SIGNAL(onNameChange()),SLOT(nameChange()));
      if (!connected) {
        LOG_AND_THROW("Cannot connect onNameChange() signal.");
      }
      // relationship change
      connected = connect(implPtr.get(),
                          SIGNAL(onRelationshipChange(int,Handle,Handle)),
                          SLOT(relationshipChange(int,Handle,Handle)));
      if (!connected) {
        LOG_AND_THROW("Cannot connect onRelationshipChange() signal.");
      }
      // remove
      connected = connect(implPtr.get(),
                          SIGNAL(onRemoveFromWorkspace(Handle)),
                          SLOT(objectRemove(Handle)));
      if (!connected) {
        LOG_AND_THROW("Cannot connect onRemoveFromWorkspace() signal.");
      }
    }
    // connect signals for ComponentData, ComponentDataTags, and ComponentDataAttributes
    implPtr = componentData.getImpl<ModelObject_Impl>();
    // component data change
    bool connected = connect(implPtr.get(),
                             SIGNAL(onDataChange()),
                             SLOT(componentDataChange()));
    if (!connected) {
      LOG_AND_THROW("Cannot connect onDataChange() signal.");
    }
    // component data remove
    connected = connect(implPtr.get(),
                        SIGNAL(onRemoveFromWorkspace(Handle)),
                        SLOT(objectRemove(Handle)));
    if (!connected) {
      LOG_AND_THROW("Cannot connect onRemoveFromWorkspace() signal.");
    }
    // connect to addWorkspaceObject signal
    std::shared_ptr<Model_Impl> modelImplPtr = m_componentData.model().getImpl<Model_Impl>();
    connected = connect(modelImplPtr.get(),
                        SIGNAL(addWorkspaceObject(const WorkspaceObject&, const openstudio::IddObjectType&, const openstudio::UUID&)),
                        SLOT(objectAdd(const WorkspaceObject&)));
    if (!connected) {
      LOG_AND_THROW("Cannot connect addWorkspaceObject() signal.");
    }
  }

  ComponentWatcher ComponentWatcher_Impl::componentWatcher() const {
    ComponentWatcher result(std::const_pointer_cast<ComponentWatcher_Impl>(shared_from_this()));
    return result;
  }

  ComponentData ComponentWatcher_Impl::componentData() const {
    return m_componentData;
  }

  void ComponentWatcher_Impl::dataChange() {
    mf_changeComponentVersion();
  }

  void ComponentWatcher_Impl::nameChange() {
    mf_changeComponentVersion();
  }

  void ComponentWatcher_Impl::componentDataChange() {
    mf_changeComponentVersion();
    return;
  }

  void ComponentWatcher_Impl::relationshipChange(int index,Handle newHandle,Handle oldHandle) {
    OS_ASSERT(newHandle != oldHandle);
    // if oldHandle is in the Component nullify the component
    HandleVector hs = getHandles<ModelObject>(m_componentObjects);
    if (std::find(hs.begin(),hs.end(),oldHandle) != hs.end()) {
      mf_removeComponent();
      return;
    }
    // otherwise, change version
    mf_changeComponentVersion();
    return;
  }

  void ComponentWatcher_Impl::objectRemove(const Handle& handleOfRemovedObject) {
    // no ComponentData means no Component to watch
    if (handleOfRemovedObject == m_componentData.handle()) {
      emit obsolete(componentWatcher());
      return;
    }
    // if removedObject is the primary componentObject, nullify the component.
    OS_ASSERT(m_componentObjects.size() > 0);
    if (handleOfRemovedObject == m_componentObjects[0].handle()) {
      mf_removeComponent();
      return;
    }
    // if removedObject is a componentObject, remove from the vector and refresh
    // component contents
    auto it = std::find_if(m_componentObjects.begin(),
        m_componentObjects.end(),std::bind(handleEquals<ModelObject,Handle>,std::placeholders::_1,handleOfRemovedObject));
    if (it != m_componentObjects.end()) {
      OS_ASSERT(it != m_componentObjects.begin());
      m_componentObjects.erase(it);
      mf_refreshComponentContents(false);
      return;
    }
    return;
  }

  void ComponentWatcher_Impl::objectAdd(const WorkspaceObject& addedObject) {
    /*IddObjectType type =*/ addedObject.iddObject().type();
    return;
  }

  void ComponentWatcher_Impl::mf_changeComponentVersion() {
    // disconnect componentDataChange slot to avoid endless loop
    std::shared_ptr<ModelObject_Impl> implPtr = m_componentData.getImpl<ModelObject_Impl>();
    bool ok = disconnect(implPtr.get(),SIGNAL(onDataChange()),this,SLOT(componentDataChange()));
    OS_ASSERT(ok);

    // change version UUID
    m_componentData.createVersionUUID();

    // reconnect componentDataChange
    ok = connect(implPtr.get(),SIGNAL(onDataChange()),SLOT(componentDataChange()));
    OS_ASSERT(ok);
  }

  void ComponentWatcher_Impl::mf_refreshComponentContents(bool logWarnings) {
    // disconnect componentDataChange slot to avoid endless loop
    std::shared_ptr<ModelObject_Impl> implPtr = m_componentData.getImpl<ModelObject_Impl>();
    bool ok = disconnect(implPtr.get(),SIGNAL(onDataChange()),this,SLOT(componentDataChange()));
    OS_ASSERT(ok);

    // \todo logWarnings
    m_componentData.clearExtensibleGroups();
    for (const ModelObject& object : m_componentObjects) {
      m_componentData.getImpl<ComponentData_Impl>()->registerObject(object);
    }

    // reconnect componentDataChange
    ok = connect(implPtr.get(),SIGNAL(onDataChange()),SLOT(componentDataChange()));
    OS_ASSERT(ok);
  }

  void ComponentWatcher_Impl::mf_removeComponent() {
    m_componentData.remove(); // triggers emit through slot objectRemove
  }

} // detail

ComponentWatcher::ComponentWatcher(ComponentData& componentData)
  : m_impl(new detail::ComponentWatcher_Impl(componentData))
{}

ComponentData ComponentWatcher::componentData() const {
  return m_impl->componentData();
}

bool ComponentWatcher::operator==(const ComponentWatcher& other) {
  return m_impl == other.m_impl;
}

bool ComponentWatcher::operator!=(const ComponentWatcher& other) {
  return (!operator==(other));
}

/// @cond
std::shared_ptr<detail::ComponentWatcher_Impl> ComponentWatcher::getImpl() const {
  return m_impl;
}

ComponentWatcher::ComponentWatcher(std::shared_ptr<detail::ComponentWatcher_Impl> impl)
  : m_impl(impl)
{}
/// @endcond

} // model
} // openstudio
