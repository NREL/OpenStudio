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
  connect(subTabView, &ModelSubTabView::modelObjectSelected, this, &ModelSubTabController::modelObjectSelected);

  connect(subTabView, &ModelSubTabView::dropZoneItemClicked, this, &ModelSubTabController::dropZoneItemClicked);

  connect(subTabView, &ModelSubTabView::dropZoneItemSelected, this, &ModelSubTabController::dropZoneItemSelected);
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
  // get selected items
  auto modelSubTabView = qobject_cast<ModelSubTabView *>(subTabView());
  auto modelObjectInspectorView = modelSubTabView->modelObjectInspectorView();

  if (modelObjectInspectorView->supportsMultipleObjectSelection())
  {
    for (auto &obj : modelObjectInspectorView->selectedObjects())
    {
      onRemoveObject(obj);
    }
  } else {
    ModelObjectItem* modelObjectItem = qobject_cast<ModelObjectItem*>(item);
    OS_ASSERT(modelObjectItem);
    model::ModelObject modelObject = modelObjectItem->modelObject();
    if (!modelObject.handle().isNull()){
      onRemoveObject(modelObject);
    }
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
  // get selected items
  auto modelSubTabView = qobject_cast<ModelSubTabView *>(subTabView());
  auto modelObjectInspectorView = modelSubTabView->modelObjectInspectorView();

  openstudio::IddObjectType currentIddObjectType = this->currentIddObjectType();
  if (modelObjectInspectorView->supportsMultipleObjectSelection())
  {
    // Always make at least one
    if (!modelObjectInspectorView->selectedObjects().size()) {
      onAddObject(currentIddObjectType);
    }
    else {
      for (auto &obj : modelObjectInspectorView->selectedObjects())
      {
        onAddObject(obj);
      }
    }
  }
  else {
    onAddObject(currentIddObjectType);
  }
}

void ModelSubTabController::onCopyItem()
{
  // get selected items
  auto modelSubTabView = qobject_cast<ModelSubTabView *>(subTabView());
  auto modelObjectInspectorView = modelSubTabView->modelObjectInspectorView();

  if (modelObjectInspectorView->supportsMultipleObjectSelection())
  {
    for (auto &obj : modelObjectInspectorView->selectedObjects())
    {
      if (!obj.handle().isNull()){
       onCopyObject(obj);
     }
    }
  }
  else {
    boost::optional<openstudio::model::ModelObject> selectedModelObject = this->selectedModelObject();
    if (selectedModelObject && !selectedModelObject->handle().isNull()){
      onCopyObject(*selectedModelObject);
    }
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

