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

#include "ModelSubTabController.hpp"
#include "ModelSubTabView.hpp"
#include "ModelObjectInspectorView.hpp"
#include "ModelObjectItem.hpp"
#include "ModelObjectListView.hpp"
#include "ModelObjectTypeListView.hpp"
#include "OSItemSelector.hpp"
#include "OSItemSelectorButtons.hpp"
#include "OSAppBase.hpp"
#include "OSDocument.hpp"

#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/Component.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {

ModelSubTabController::ModelSubTabController(ModelSubTabView* subTabView, const model::Model & model)
  : SubTabController(subTabView),
    m_model(model)
{
  bool isConnected = connect(subTabView,
                             SIGNAL(modelObjectSelected(model::OptionalModelObject &, bool )),
                             this,
                             SIGNAL(modelObjectSelected(model::OptionalModelObject &, bool )));
  OS_ASSERT(isConnected);
}

openstudio::model::Model ModelSubTabController::model() const
{
  return m_model;
}

bool ModelSubTabController::fromModel(const OSItemId& itemId) const
{
  return OSAppBase::instance()->currentDocument()->fromModel(itemId);
}

bool ModelSubTabController::fromComponentLibrary(const OSItemId& itemId) const
{
  return OSAppBase::instance()->currentDocument()->fromComponentLibrary(itemId);
}

boost::optional<model::ModelObject> ModelSubTabController::getModelObject(const OSItemId& itemId) const
{
  return OSAppBase::instance()->currentDocument()->getModelObject(itemId);
}

boost::optional<model::Component> ModelSubTabController::getComponent(const OSItemId& itemId) const
{
  return OSAppBase::instance()->currentDocument()->getComponent(itemId);
}

void ModelSubTabController::onRemoveItem(OSItem* item)
{
  ModelObjectItem* modelObjectItem = qobject_cast<ModelObjectItem*>(item);
  OS_ASSERT(modelObjectItem);
  model::ModelObject modelObject = modelObjectItem->modelObject();
  if (!modelObject.handle().isNull()){
    onRemoveObject(modelObject);
  }
}

void ModelSubTabController::onReplaceItem(OSItem* item, const OSItemId& replacementItemId)
{
  ModelObjectItem* modelObjectItem = qobject_cast<ModelObjectItem*>(item);
  OS_ASSERT(modelObjectItem);
  model::ModelObject modelObject = modelObjectItem->modelObject();
  if (!modelObject.handle().isNull()){
    onReplaceObject(modelObject, replacementItemId);
  }
}

void ModelSubTabController::onAddItem()
{
  openstudio::IddObjectType currentIddObjectType = this->currentIddObjectType();
  onAddObject(currentIddObjectType);
}

void ModelSubTabController::onCopyItem()
{
  boost::optional<openstudio::model::ModelObject> selectedModelObject = this->selectedModelObject();
  if (selectedModelObject && !selectedModelObject->handle().isNull()){
    onCopyObject(*selectedModelObject);
  }
}


void ModelSubTabController::onPurgeItems()
{
  openstudio::IddObjectType currentIddObjectType = this->currentIddObjectType();
  onPurgeObjects(currentIddObjectType);
}

boost::optional<openstudio::model::ModelObject> ModelSubTabController::selectedModelObject() const
{
  boost::optional<openstudio::model::ModelObject> result;

  const OSItemSelector* itemSelector = subTabView()->itemSelector();

  const ModelObjectListView* modelObjectListView = qobject_cast<const ModelObjectListView*>(itemSelector);
  if (modelObjectListView){
    result = modelObjectListView->selectedModelObject();
  }

  const ModelObjectTypeListView* modelObjectTypeListView = qobject_cast<const ModelObjectTypeListView*>(itemSelector);
  if (modelObjectTypeListView){
    result = modelObjectTypeListView->selectedModelObject();
  }

  return result;
}

openstudio::IddObjectType ModelSubTabController::currentIddObjectType() const
{
  openstudio::IddObjectType result;

  const OSItemSelector* itemSelector = subTabView()->itemSelector();

  const ModelObjectListView* modelObjectListView = qobject_cast<const ModelObjectListView*>(itemSelector);
  if (modelObjectListView){
    result = modelObjectListView->iddObjectType();
  }

  const ModelObjectTypeListView* modelObjectTypeListView = qobject_cast<const ModelObjectTypeListView*>(itemSelector);
  if (modelObjectTypeListView){
    result = modelObjectTypeListView->currentIddObjectType();
  }

  return result;
}



} // openstudio

