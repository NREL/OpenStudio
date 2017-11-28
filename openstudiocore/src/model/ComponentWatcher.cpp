/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "ComponentWatcher.hpp"
#include "ComponentWatcher_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"

#include "ModelObject.hpp"
#include "ModelObject_Impl.hpp"

#include "ComponentData_Impl.hpp"
#include "../nano/nano_signal_slot.hpp"

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
      implPtr.get()->ModelObject_Impl::onDataChange.connect<ComponentWatcher_Impl, &ComponentWatcher_Impl::dataChange>(this);
      // name change
      implPtr.get()->ModelObject_Impl::onNameChange.connect<ComponentWatcher_Impl, &ComponentWatcher_Impl::nameChange>(this);
      // relationship change
      implPtr.get()->ModelObject_Impl::onRelationshipChange.connect<ComponentWatcher_Impl, &ComponentWatcher_Impl::relationshipChange>(this);
      // remove
      implPtr.get()->ModelObject_Impl::onRemoveFromWorkspace.connect<ComponentWatcher_Impl, &ComponentWatcher_Impl::objectRemove>(this);
    }
    // connect signals for ComponentData, ComponentDataTags, and ComponentDataAttributes
    implPtr = componentData.getImpl<ModelObject_Impl>();
    // component data change
    implPtr.get()->ModelObject_Impl::onDataChange.connect<ComponentWatcher_Impl, &ComponentWatcher_Impl::componentDataChange>(this);
    // component data remove
    implPtr.get()->ModelObject_Impl::onRemoveFromWorkspace.connect<ComponentWatcher_Impl, &ComponentWatcher_Impl::objectRemove>(this);
    // connect to addWorkspaceObject signal
    std::shared_ptr<Model_Impl> modelImplPtr = m_componentData.model().getImpl<Model_Impl>();
    modelImplPtr.get()->Model_Impl::addWorkspaceObject.connect<ComponentWatcher_Impl, &ComponentWatcher_Impl::objectAdd>(this);
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
      this->obsolete.nano_emit(componentWatcher());
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

  void ComponentWatcher_Impl::objectAdd(const WorkspaceObject& addedObject, const openstudio::IddObjectType& type, const openstudio::UUID& uuid) {
    /*IddObjectType type =*/ addedObject.iddObject().type();
    return;
  }

  void ComponentWatcher_Impl::mf_changeComponentVersion() {
    // disconnect componentDataChange slot to avoid endless loop
    std::shared_ptr<ModelObject_Impl> implPtr = m_componentData.getImpl<ModelObject_Impl>();
    implPtr.get()->onDataChange.disconnect<ComponentWatcher_Impl, &ComponentWatcher_Impl::componentDataChange>(this);

    // change version UUID
    m_componentData.createVersionUUID();

    // reconnect componentDataChange
    implPtr.get()->ModelObject_Impl::onDataChange.connect<ComponentWatcher_Impl, &ComponentWatcher_Impl::componentDataChange>(this);
  }

  void ComponentWatcher_Impl::mf_refreshComponentContents(bool logWarnings) {
    // disconnect componentDataChange slot to avoid endless loop
    std::shared_ptr<ModelObject_Impl> implPtr = m_componentData.getImpl<ModelObject_Impl>();
    implPtr.get()->onDataChange.disconnect<ComponentWatcher_Impl, &ComponentWatcher_Impl::componentDataChange>(this);

    // \todo logWarnings
    m_componentData.clearExtensibleGroups();
    for (const ModelObject& object : m_componentObjects) {
      m_componentData.getImpl<ComponentData_Impl>()->registerObject(object);
    }

    // reconnect componentDataChange
    implPtr.get()->ModelObject_Impl::onDataChange.connect<ComponentWatcher_Impl, &ComponentWatcher_Impl::componentDataChange>(this);
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
  : m_impl(std::move(impl))
{}
/// @endcond

} // model
} // openstudio
