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

#include "ConstructionsController.hpp"
#include "ConstructionsView.hpp"

#include "../model/CFactorUndergroundWallConstruction.hpp"
#include "../model/Component.hpp"
#include "../model/Component_Impl.hpp"
#include "../model/ComponentData.hpp"
#include "../model/ComponentData_Impl.hpp"
#include "../model/Construction.hpp"
#include "../model/ConstructionBase.hpp"
#include "../model/ConstructionBase_Impl.hpp"
#include "../model/ConstructionWithInternalSource.hpp"
#include "../model/DefaultConstructionSet.hpp"
#include "../model/DefaultConstructionSet_Impl.hpp"
#include "../model/FFactorGroundFloorConstruction.hpp"
#include "../model/WindowDataFile.hpp"

#include "../utilities/core/Logger.hpp"
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {

ConstructionsController::ConstructionsController(bool isIP, const model::Model& model)
  : ModelSubTabController(new ConstructionsView(isIP,model), model)
{
}

ConstructionsController::~ConstructionsController()
{
}

void ConstructionsController::onAddObject(const openstudio::IddObjectType& iddObjectType)
{
  switch(iddObjectType.value()){
    case IddObjectType::OS_Construction:
      openstudio::model::Construction(this->model());
      break;
    case IddObjectType::OS_Construction_InternalSource:
      openstudio::model::ConstructionWithInternalSource(this->model());
      break;
    case IddObjectType::OS_Construction_CfactorUndergroundWall:
      openstudio::model::CFactorUndergroundWallConstruction(this->model());
      break;
    case IddObjectType::OS_Construction_FfactorGroundFloor:
      openstudio::model::FFactorGroundFloorConstruction(this->model());
      break;
    case IddObjectType::OS_Construction_WindowDataFile:
      openstudio::model::WindowDataFile(this->model());
      break;
    default:
      LOG_FREE_AND_THROW("ConstructionsController", "Unknown IddObjectType '" << iddObjectType.valueName() << "'");
  }
}

void ConstructionsController::onCopyObject(const openstudio::model::ModelObject& modelObject)
{
  modelObject.clone(this->model());
}

void ConstructionsController::onRemoveObject(openstudio::model::ModelObject modelObject)
{
  modelObject.remove();
}

void ConstructionsController::onReplaceObject(openstudio::model::ModelObject modelObject, const OSItemId& replacementItemId)
{
  // not yet implemented
}

void ConstructionsController::onPurgeObjects(const openstudio::IddObjectType& iddObjectType)
{
  this->model().purgeUnusedResourceObjects(iddObjectType);
}

void ConstructionsController::onDrop(const OSItemId& itemId)
{
  boost::optional<model::ModelObject> modelObject = this->getModelObject(itemId);
  if (modelObject){
    if(modelObject->optionalCast<model::ConstructionBase>()){
      if (this->fromComponentLibrary(itemId)){
        modelObject = modelObject->clone(this->model());
      }
    }
  }else{
    boost::optional<model::Component> component = this->getComponent(itemId);
    if (component){
      if (component->primaryObject().optionalCast<model::ModelObject>()){
        this->model().insertComponent(*component);
      }
    }
  }
}

void ConstructionsController::onInspectItem(OSItem* item)
{
}

} // openstudio

