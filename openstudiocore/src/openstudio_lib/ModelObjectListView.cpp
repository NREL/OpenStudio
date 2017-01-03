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

#include "ModelObjectListView.hpp"
#include "ModelObjectItem.hpp"
#include "OSAppBase.hpp"
#include "BCLComponentItem.hpp"

#include "../model/Model_Impl.hpp"
#include "../model/ModelObject_Impl.hpp"
#include "../model/ZoneHVACComponent.hpp"
#include "../model/ZoneHVACComponent_Impl.hpp"
#include "../model/HVACComponent.hpp"
#include "../model/HVACComponent_Impl.hpp"
#include "../model/UtilityBill.hpp"
#include "../model/UtilityBill_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/bcl/LocalBCL.hpp"

#include <iostream>

namespace openstudio {

ModelObjectListController::ModelObjectListController(const openstudio::IddObjectType& iddObjectType,
                                                     const model::Model& model,
                                                     bool showLocalBCL)
  : m_iddObjectType(iddObjectType), m_model(model), m_showLocalBCL(showLocalBCL)
{

  // model.getImpl<model::detail::Model_Impl>().get()->addWorkspaceObjectPtr.connect<ModelObjectListController, &ModelObjectListController::objectAdded>(this);
  connect(OSAppBase::instance(), &OSAppBase::workspaceObjectAddedPtr, this, &ModelObjectListController::objectAdded, Qt::QueuedConnection);

  //model.getImpl<model::detail::Model_Impl>().get()->removeWorkspaceObjectPtr.connect<ModelObjectListController, &ModelObjectListController::objectRemoved>(this);
  connect(OSAppBase::instance(), &OSAppBase::workspaceObjectRemovedPtr, this, &ModelObjectListController::objectRemoved, Qt::QueuedConnection);

}

IddObjectType ModelObjectListController::iddObjectType() const
{
  return m_iddObjectType;
}

void ModelObjectListController::objectAdded(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> impl, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle)
{
  if (iddObjectType == m_iddObjectType){
    std::vector<OSItemId> ids = this->makeVector();
    emit itemIds(ids);

    for (const OSItemId& id : ids){
      if (id.itemId() == toQString(impl->handle())){
        emit selectedItemId(id);
        break;
      }
    }
  }
}

void ModelObjectListController::objectRemoved(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> impl, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle)
{
  if (iddObjectType == m_iddObjectType){
    emit itemIds(makeVector());
  }
}

std::vector<OSItemId> ModelObjectListController::makeVector()
{
  std::vector<OSItemId> result;

  if( m_showLocalBCL )
  {
    std::vector<std::pair<std::string, std::string> > pairs;
    pairs.push_back(std::make_pair<std::string,std::string>("OpenStudio Type",m_iddObjectType.valueDescription()));

    // get BCL results
    std::vector<BCLComponent> bclresults = LocalBCL::instance().componentAttributeSearch(pairs);

    // sort by name
    std::sort(bclresults.begin(), bclresults.end(), BCLComponentNameGreater());

    for( auto it = bclresults.begin();
         it != bclresults.end();
         ++it )
    {
      result.push_back(bclComponentToItemId(*it));
    }
  }

  // get objects by type
  std::vector<WorkspaceObject> workspaceObjects = m_model.getObjectsByType(m_iddObjectType);

  // sort by name
  std::sort(workspaceObjects.begin(), workspaceObjects.end(), WorkspaceObjectNameGreater());

  for (WorkspaceObject workspaceObject : workspaceObjects){
    if (!workspaceObject.handle().isNull()){
      openstudio::model::ModelObject modelObject = workspaceObject.cast<openstudio::model::ModelObject>();
      if(boost::optional<model::HVACComponent> hvacComponent = modelObject.optionalCast<model::HVACComponent>()) {
        if( (! hvacComponent->containingHVACComponent()) && (! hvacComponent->containingZoneHVACComponent()) ) {
          result.push_back(modelObjectToItemId(hvacComponent.get(), false));
        }
      } else {
        result.push_back(modelObjectToItemId(modelObject, false));
      }
    }
  }

  return result;
}

ModelObjectListView::ModelObjectListView(const openstudio::IddObjectType& iddObjectType,
                                         const model::Model& model,
                                         bool addScrollArea,
                                         bool showLocalBCL,
                                         QWidget * parent )
  : OSItemList(new ModelObjectListController(iddObjectType, model,showLocalBCL), addScrollArea, parent)
{
}

boost::optional<openstudio::model::ModelObject> ModelObjectListView::selectedModelObject() const
{
  OSItem* selectedItem = this->selectedItem();
  ModelObjectItem* modelObjectItem = qobject_cast<ModelObjectItem*>(selectedItem);
  if (modelObjectItem){
    return modelObjectItem->modelObject();
  }
  return boost::none;
}

IddObjectType ModelObjectListView::iddObjectType() const
{
  OSVectorController* vectorController = this->vectorController();
  ModelObjectListController* modelObjectListController = qobject_cast<ModelObjectListController*>(vectorController);
  OS_ASSERT(modelObjectListController);
  return modelObjectListController->iddObjectType();
}

} // openstudio

