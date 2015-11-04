/**********************************************************************
*  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

  connect(m_model->getImpl<model::detail::Model_Impl>().get(),
    static_cast<void (model::detail::Model_Impl::*)(std::shared_ptr<detail::WorkspaceObject_Impl>, const IddObjectType &, const UUID &) const>(&model::detail::Model_Impl::addWorkspaceObject),
    this,
    &ModelObjectVectorController::objectAdded,
    Qt::QueuedConnection);
  
  connect(m_model->getImpl<model::detail::Model_Impl>().get(),
    static_cast<void (model::detail::Model_Impl::*)(std::shared_ptr<detail::WorkspaceObject_Impl>, const IddObjectType &, const UUID &) const>(&model::detail::Model_Impl::removeWorkspaceObject),
    this,
    &ModelObjectVectorController::objectRemoved,
    Qt::QueuedConnection);
  
  connect(m_modelObject->getImpl<model::detail::ModelObject_Impl>().get(), &model::detail::ModelObject_Impl::onRelationshipChange,
          this, &ModelObjectVectorController::changeRelationship);

  connect(m_modelObject->getImpl<model::detail::ModelObject_Impl>().get(), &model::detail::ModelObject_Impl::onDataChange,
          this, &ModelObjectVectorController::dataChange);

  connect(m_modelObject->getImpl<model::detail::ModelObject_Impl>().get(), &model::detail::ModelObject_Impl::onChange,
          this, &ModelObjectVectorController::change);
}

void ModelObjectVectorController::attachModel(const model::Model& model)
{
  if (m_model){
    disconnect(m_model->getImpl<model::detail::Model_Impl>().get());
    m_model.reset();
  }
  
  m_model = model;

  connect(m_model->getImpl<model::detail::Model_Impl>().get(),
    static_cast<void (model::detail::Model_Impl::*)(std::shared_ptr<detail::WorkspaceObject_Impl>, const IddObjectType &, const UUID &) const>(&model::detail::Model_Impl::addWorkspaceObject),
    this,
    &ModelObjectVectorController::objectAdded,
    Qt::QueuedConnection);
  
  connect(m_model->getImpl<model::detail::Model_Impl>().get(),
    static_cast<void (model::detail::Model_Impl::*)(std::shared_ptr<detail::WorkspaceObject_Impl>, const IddObjectType &, const UUID &) const>(&model::detail::Model_Impl::removeWorkspaceObject),
    this,
    &ModelObjectVectorController::objectRemoved,
    Qt::QueuedConnection);
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

  connect(modelObject.getImpl<model::detail::ModelObject_Impl>().get(), &model::detail::ModelObject_Impl::onRelationshipChange,
          this, &ModelObjectVectorController::changeRelationship);
}

void ModelObjectVectorController::detach()
{
  if (m_modelObject){
    disconnect(m_modelObject->getImpl<model::detail::ModelObject_Impl>().get());
    m_modelObject.reset();
  }

  if (m_model){
    disconnect(m_model->getImpl<model::detail::Model_Impl>().get());
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
      disconnect(modelObject.getImpl<model::detail::ModelObject_Impl>().get());
    }else{
      newVector.push_back(*it);
    }
  }
  m_otherModelObjects.swap(newVector);
}

void ModelObjectVectorController::detachOtherModelObjects()
{
  for (const model::ModelObject& modelObject : m_otherModelObjects){
    disconnect(modelObject.getImpl<model::detail::ModelObject_Impl>().get());
  }
  m_otherModelObjects.clear();
}

void ModelObjectVectorController::changeRelationship(int index, Handle newHandle, Handle oldHandle)
{
  if (newHandle != oldHandle){
    model::detail::ModelObject_Impl* impl = qobject_cast<model::detail::ModelObject_Impl*>(this->sender());
    if (impl){
      onChangeRelationship(impl->getObject<model::ModelObject>(), index, newHandle, oldHandle);
    }
  }
}

void ModelObjectVectorController::dataChange()
{
  QObject* sender = this->sender();
  model::detail::ModelObject_Impl* impl = qobject_cast<model::detail::ModelObject_Impl*>(sender);
  if (impl){
    onDataChange(impl->getObject<model::ModelObject>());
  }
}

void ModelObjectVectorController::change()
{
  QObject* sender = this->sender();
  model::detail::ModelObject_Impl* impl = qobject_cast<model::detail::ModelObject_Impl*>(sender);
  if (impl){
    onChange(impl->getObject<model::ModelObject>());
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

