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

#include "DefaultConstructionSetsController.hpp"
#include "DefaultConstructionSetsView.hpp"

#include "../model/DefaultConstructionSet.hpp"
#include "../model/DefaultConstructionSet_Impl.hpp"
#include "../model/DefaultSurfaceConstructions.hpp"
#include "../model/DefaultSubSurfaceConstructions.hpp"

#include "../utilities/core/Logger.hpp"
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {

DefaultConstructionSetsController::DefaultConstructionSetsController(const model::Model& model)
  : ModelSubTabController(new DefaultConstructionSetsView(model, "Default Constructions", false), model)
{
}

DefaultConstructionSetsController::~DefaultConstructionSetsController()
{
}

void DefaultConstructionSetsController::onAddObject(const openstudio::IddObjectType& iddObjectType)
{
  switch(iddObjectType.value()){
    case IddObjectType::OS_DefaultConstructionSet:
      openstudio::model::DefaultConstructionSet(this->model());
      break;
    default:
      LOG_FREE_AND_THROW("DefaultConstructionSetsController", "Unknown IddObjectType '" << iddObjectType.valueName() << "'");
  }
}

void DefaultConstructionSetsController::onCopyObject(const openstudio::model::ModelObject& modelObject)
{
  modelObject.clone(this->model());
}

void DefaultConstructionSetsController::onRemoveObject(openstudio::model::ModelObject modelObject)
{
  boost::optional<model::DefaultConstructionSet> dcs = modelObject.optionalCast<model::DefaultConstructionSet>();
  if (dcs){
    boost::optional<model::DefaultSurfaceConstructions> dsc = dcs->defaultExteriorSurfaceConstructions();
    if (dsc && (dsc->directUseCount(true) == 1)){
      dsc->remove();
    }

    dsc = dcs->defaultInteriorSurfaceConstructions();
    if (dsc && (dsc->directUseCount(true) == 1)){
      dsc->remove();
    }

    dsc = dcs->defaultGroundContactSurfaceConstructions();
    if (dsc && (dsc->directUseCount(true) == 1)){
      dsc->remove();
    }

    boost::optional<model::DefaultSubSurfaceConstructions> dssc = dcs->defaultExteriorSubSurfaceConstructions();
    if (dssc && (dssc->directUseCount(true) == 1)){
      dssc->remove();
    }

    dssc = dcs->defaultInteriorSubSurfaceConstructions();
    if (dssc && (dssc->directUseCount(true) == 1)){
      dssc->remove();
    }
  }

  modelObject.remove();
}

void DefaultConstructionSetsController::onReplaceObject(openstudio::model::ModelObject modelObject, const OSItemId& replacementItemId)
{
  // not yet implemented
}

void DefaultConstructionSetsController::onPurgeObjects(const openstudio::IddObjectType& iddObjectType)
{
  if (iddObjectType == IddObjectType::OS_DefaultConstructionSet){
    this->model().purgeUnusedResourceObjects(IddObjectType::OS_DefaultConstructionSet);
    this->model().purgeUnusedResourceObjects(IddObjectType::OS_DefaultSurfaceConstructions);
    this->model().purgeUnusedResourceObjects(IddObjectType::OS_DefaultSubSurfaceConstructions);
  }else{
    this->model().purgeUnusedResourceObjects(iddObjectType);
  }
}

void DefaultConstructionSetsController::onDrop(const OSItemId& itemId)
{
  boost::optional<model::ModelObject> modelObject = this->getModelObject(itemId);
  if(modelObject){
    if(modelObject->optionalCast<model::DefaultConstructionSet>()){
      if (this->fromComponentLibrary(itemId)){
        modelObject = modelObject->clone(this->model());
      }
    }
  }
}

void DefaultConstructionSetsController::onInspectItem(OSItem* item)
{
}

} // openstudio

