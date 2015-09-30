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

