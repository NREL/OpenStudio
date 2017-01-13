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

#include "ModelObjectVectorController.hpp"
#include "OSAppBase.hpp"
#include "OSDocument.hpp"

#include "../model/ModelObject_Impl.hpp"
#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"

#include "../utilities/core/Assert.hpp"


namespace openstudio {


void ModelObjectVectorController::attach(const model::ModelObject& modelObject)
{
  detach();

  m_modelObject = modelObject;

  attachModel(modelObject.model());

  // m_model->getImpl<model::detail::Model_Impl>().get()->addWorkspaceObjectPtr.connect<ModelObjectVectorController, &ModelObjectVectorController::objectAdded>(this);
  //connect(OSAppBase::instance(), &OSAppBase::workspaceObjectAddedPtr, this, &ModelObjectVectorController::objectAdded, Qt::QueuedConnection);

  //m_model->getImpl<model::detail::Model_Impl>().get()->removeWorkspaceObjectPtr.connect<ModelObjectVectorController, &ModelObjectVectorController::objectRemoved>(this);
  //connect(OSAppBase::instance(), &OSAppBase::workspaceObjectRemovedPtr, this, &ModelObjectVectorController::objectRemoved, Qt::QueuedConnection);

  m_modelObject->getImpl<model::detail::ModelObject_Impl>().get()->onRelationshipChange.connect<ModelObjectVectorController, &ModelObjectVectorController::changeRelationship>(this);

  m_modelObject->getImpl<model::detail::ModelObject_Impl>().get()->onDataChange.connect<ModelObjectVectorController, &ModelObjectVectorController::dataChange>(this);

  m_modelObject->getImpl<model::detail::ModelObject_Impl>().get()->onChange.connect<ModelObjectVectorController, &ModelObjectVectorController::change>(this);
}

void ModelObjectVectorController::attachModel(const model::Model& model)
{
  if (m_model){

    // m_model->getImpl<model::detail::Model_Impl>().get()->addWorkspaceObjectPtr.disconnect<ModelObjectVectorController, &ModelObjectVectorController::objectAdded>(this);
    disconnect(OSAppBase::instance(), &OSAppBase::workspaceObjectAddedPtr, this, &ModelObjectVectorController::objectAdded);

     //m_model->getImpl<model::detail::Model_Impl>().get()->removeWorkspaceObjectPtr.disconnect<ModelObjectVectorController, &ModelObjectVectorController::objectRemoved>(this);
    disconnect(OSAppBase::instance(), &OSAppBase::workspaceObjectRemovedPtr, this, &ModelObjectVectorController::objectRemoved);

    m_model.reset();
  }

  m_model = model;

  // m_model->getImpl<model::detail::Model_Impl>().get()->addWorkspaceObjectPtr.connect<ModelObjectVectorController, &ModelObjectVectorController::objectAdded>(this);
  connect(OSAppBase::instance(), &OSAppBase::workspaceObjectAddedPtr, this, &ModelObjectVectorController::objectAdded, Qt::QueuedConnection);

  //m_model->getImpl<model::detail::Model_Impl>().get()->removeWorkspaceObjectPtr.connect<ModelObjectVectorController, &ModelObjectVectorController::objectRemoved>(this);
  connect(OSAppBase::instance(), &OSAppBase::workspaceObjectRemovedPtr, this, &ModelObjectVectorController::objectRemoved, Qt::QueuedConnection);
}

void ModelObjectVectorController::attachOtherModelObject(const model::ModelObject& modelObject)
{
  // check not already connected
  for (const model::ModelObject& currentModelObject : m_otherModelObjects){
    if (modelObject.handle() == currentModelObject.handle()){
      return;
    }
  }

  m_otherModelObjects.push_back(modelObject);

  modelObject.getImpl<model::detail::ModelObject_Impl>().get()->onRelationshipChange.connect<ModelObjectVectorController, &ModelObjectVectorController::changeRelationship>(this);
}

void ModelObjectVectorController::detach()
{
  if (m_modelObject){
    m_modelObject->getImpl<model::detail::ModelObject_Impl>().get()->onRelationshipChange.disconnect<ModelObjectVectorController, &ModelObjectVectorController::changeRelationship>(this);

    m_modelObject->getImpl<model::detail::ModelObject_Impl>().get()->onDataChange.disconnect<ModelObjectVectorController, &ModelObjectVectorController::dataChange>(this);

    m_modelObject->getImpl<model::detail::ModelObject_Impl>().get()->onChange.disconnect<ModelObjectVectorController, &ModelObjectVectorController::change>(this);

    m_modelObject.reset();
  }


  if (m_model){
    // m_model->getImpl<model::detail::Model_Impl>().get()->addWorkspaceObjectPtr.disconnect<ModelObjectVectorController, &ModelObjectVectorController::objectAdded>(this);
    disconnect(OSAppBase::instance(), &OSAppBase::workspaceObjectAddedPtr, this, &ModelObjectVectorController::objectAdded);

     //m_model->getImpl<model::detail::Model_Impl>().get()->removeWorkspaceObjectPtr.disconnect<ModelObjectVectorController, &ModelObjectVectorController::objectRemoved>(this);
    disconnect(OSAppBase::instance(), &OSAppBase::workspaceObjectRemovedPtr, this, &ModelObjectVectorController::objectRemoved);

    m_model.reset();
  }

  detachOtherModelObjects();
}

void ModelObjectVectorController::detachOtherModelObject(const model::ModelObject& modelObject)
{
  std::vector<model::ModelObject>::const_iterator it = m_otherModelObjects.begin();
  std::vector<model::ModelObject>::const_iterator itend = m_otherModelObjects.end();
  std::vector<model::ModelObject> newVector;
  for (; it != itend; ++it){
    if (it->handle() == modelObject.handle()){
      m_modelObject->getImpl<model::detail::ModelObject_Impl>().get()->onRelationshipChange.disconnect<ModelObjectVectorController, &ModelObjectVectorController::changeRelationship>(this);

      //m_modelObject->getImpl<model::detail::ModelObject_Impl>().get()->onDataChange.disconnect<ModelObjectVectorController, &ModelObjectVectorController::dataChange>(this);

      //m_modelObject->getImpl<model::detail::ModelObject_Impl>().get()->onChange.disconnect<ModelObjectVectorController, &ModelObjectVectorController::change>(this);

    }else{
      newVector.push_back(*it);
    }
  }
  m_otherModelObjects.swap(newVector);
}

void ModelObjectVectorController::detachOtherModelObjects()
{
  for (const model::ModelObject& modelObject : m_otherModelObjects){
    // m_model->getImpl<model::detail::Model_Impl>().get()->addWorkspaceObjectPtr.disconnect<ModelObjectVectorController, &ModelObjectVectorController::objectAdded>(this);
    disconnect(OSAppBase::instance(), &OSAppBase::workspaceObjectAddedPtr, this, &ModelObjectVectorController::objectAdded);

    // m_model->getImpl<model::detail::Model_Impl>().get()->removeWorkspaceObjectPtr.disconnect<ModelObjectVectorController, &ModelObjectVectorController::objectRemoved>(this);
    //connect(OSAppBase::instance(), &OSAppBase::workspaceObjectRemovedPtr, this, &ModelObjectVectorController::objectRemoved, Qt::QueuedConnection);

  }
  m_otherModelObjects.clear();
}

void ModelObjectVectorController::changeRelationship(int index, Handle newHandle, Handle oldHandle)
{
  if (m_modelObject){
    onChangeRelationship(m_modelObject.get(), index, newHandle, oldHandle);
  }
}

void ModelObjectVectorController::dataChange()
{
  if (m_modelObject){
    onDataChange(m_modelObject.get());
  }
}

void ModelObjectVectorController::change()
{
  if (m_modelObject){
    onChange(m_modelObject.get());
  }
}

void ModelObjectVectorController::objectAdded(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> impl, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle)
{
  onObjectAdded(impl->getObject<model::ModelObject>(), iddObjectType, handle);
}

void ModelObjectVectorController::objectRemoved(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> impl, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle)
{
  onObjectRemoved(impl->getObject<model::ModelObject>(), iddObjectType, handle);
}

void ModelObjectVectorController::onChangeRelationship(const openstudio::model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
}

void ModelObjectVectorController::onDataChange(const openstudio::model::ModelObject& modelObject)
{
}

void ModelObjectVectorController::onChange(const openstudio::model::ModelObject& modelObject)
{
}

void ModelObjectVectorController::onObjectAdded(const openstudio::model::ModelObject& modelObject, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle)
{
}

void ModelObjectVectorController::onObjectRemoved(const openstudio::model::ModelObject& modelObject, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle)
{
  detachOtherModelObject(modelObject);
}

bool ModelObjectVectorController::fromModel(const OSItemId& itemId) const
{
  return OSAppBase::instance()->currentDocument()->fromModel(itemId);
}

bool ModelObjectVectorController::fromComponentLibrary(const OSItemId& itemId) const
{
  return OSAppBase::instance()->currentDocument()->fromComponentLibrary(itemId);
}

boost::optional<model::ModelObject> ModelObjectVectorController::getModelObject(const OSItemId& itemId) const
{
  return OSAppBase::instance()->currentDocument()->getModelObject(itemId);
}

boost::optional<model::Component> ModelObjectVectorController::getComponent(const OSItemId& itemId) const
{
  return OSAppBase::instance()->currentDocument()->getComponent(itemId);
}


} // openstudio

