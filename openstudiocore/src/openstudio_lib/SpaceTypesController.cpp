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

#include "SpaceTypesController.hpp"

#include "SpaceTypesView.hpp"

#include "../model/Component.hpp"
#include "../model/ComponentData.hpp"
#include "../model/ComponentData_Impl.hpp"
#include "../model/Component_Impl.hpp"
#include "../model/ElectricEquipment.hpp"
#include "../model/ElectricEquipmentDefinition.hpp"
#include "../model/ElectricEquipmentDefinition_Impl.hpp"
#include "../model/ElectricEquipment_Impl.hpp"
#include "../model/GasEquipment.hpp"
#include "../model/GasEquipmentDefinition.hpp"
#include "../model/GasEquipmentDefinition_Impl.hpp"
#include "../model/GasEquipment_Impl.hpp"
#include "../model/InternalMass.hpp"
#include "../model/InternalMassDefinition.hpp"
#include "../model/InternalMassDefinition_Impl.hpp"
#include "../model/InternalMass_Impl.hpp"
#include "../model/Lights.hpp"
#include "../model/LightsDefinition.hpp"
#include "../model/LightsDefinition_Impl.hpp"
#include "../model/Lights_Impl.hpp"
#include "../model/Luminaire.hpp"
#include "../model/LuminaireDefinition.hpp"
#include "../model/LuminaireDefinition_Impl.hpp"
#include "../model/Luminaire_Impl.hpp"
#include "../model/Model.hpp"
#include "../model/OtherEquipment.hpp"
#include "../model/OtherEquipmentDefinition.hpp"
#include "../model/OtherEquipmentDefinition_Impl.hpp"
#include "../model/OtherEquipment_Impl.hpp"
#include "../model/People.hpp"
#include "../model/PeopleDefinition.hpp"
#include "../model/PeopleDefinition_Impl.hpp"
#include "../model/People_Impl.hpp"
#include "../model/Space.hpp"
#include "../model/SpaceLoad.hpp"
#include "../model/SpaceLoadDefinition.hpp"
#include "../model/SpaceLoadDefinition_Impl.hpp"
#include "../model/SpaceLoadInstance.hpp"
#include "../model/SpaceLoad_Impl.hpp"
#include "../model/SpaceType.hpp"
#include "../model/SpaceType_Impl.hpp"
#include "../model/SteamEquipment.hpp"
#include "../model/SteamEquipmentDefinition.hpp"
#include "../model/SteamEquipmentDefinition_Impl.hpp"
#include "../model/SteamEquipment_Impl.hpp"
#include "../model/WaterUseEquipment.hpp"
#include "../model/WaterUseEquipmentDefinition.hpp"
#include "../model/WaterUseEquipmentDefinition_Impl.hpp"
#include "../model/WaterUseEquipment_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include "../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {

  SpaceTypesController::SpaceTypesController(bool isIP,
    const model::Model& model)
    : ModelSubTabController(new SpaceTypesView(isIP, model), model)
{
}

void SpaceTypesController::onAddObject(const openstudio::IddObjectType& iddObjectType)
{
  OS_ASSERT(IddObjectType::OS_SpaceType == iddObjectType.value());
  openstudio::model::SpaceType(this->model());
}

void SpaceTypesController::onAddObject(const openstudio::model::ModelObject& modelObject)
{
  if (IddObjectType::OS_SpaceType == modelObject.iddObjectType().value()) {
    openstudio::model::SpaceType(this->model());
    return;
  }
  
  boost::optional<model::ParentObject> parent = modelObject.parent();
  OS_ASSERT(parent);

  // Expect a load from the gridview loads tab
  switch (modelObject.iddObjectType().value()){
    case IddObjectType::OS_People:
      openstudio::model::People(openstudio::model::PeopleDefinition(this->model())).setParent(*parent);
      break;
    case IddObjectType::OS_InternalMass:
      openstudio::model::InternalMass(openstudio::model::InternalMassDefinition(this->model())).setParent(*parent);
      break;
    case IddObjectType::OS_Lights:
      openstudio::model::Lights(openstudio::model::LightsDefinition(this->model())).setParent(*parent);
      break;
    case IddObjectType::OS_Luminaire:
      openstudio::model::Luminaire(openstudio::model::LuminaireDefinition(this->model())).setParent(*parent);
      break;
    case IddObjectType::OS_ElectricEquipment:
      openstudio::model::ElectricEquipment(openstudio::model::ElectricEquipmentDefinition(this->model())).setParent(*parent);
      break;
    case IddObjectType::OS_GasEquipment:
      openstudio::model::GasEquipment(openstudio::model::GasEquipmentDefinition(this->model())).setParent(*parent);
      break;
    case IddObjectType::OS_SteamEquipment:
      openstudio::model::SteamEquipment(openstudio::model::SteamEquipmentDefinition(this->model())).setParent(*parent);
      break;
    case IddObjectType::OS_OtherEquipment:
      openstudio::model::OtherEquipment(openstudio::model::OtherEquipmentDefinition(this->model())).setParent(*parent);
      break;
    case IddObjectType::OS_WaterUse_Equipment:
      openstudio::model::WaterUseEquipment(openstudio::model::WaterUseEquipmentDefinition(this->model())).setParent(*parent);
      break;
    default:
      // Should not get here
      OS_ASSERT(false);
      LOG_FREE(Error, "LoadsController", "Unknown IddObjectType '" << modelObject.iddObjectType().valueName() << "'");
  }

}

void SpaceTypesController::onCopyObject(const openstudio::model::ModelObject& modelObject)
{
  modelObject.clone(this->model());
}

void SpaceTypesController::onRemoveObject(openstudio::model::ModelObject modelObject)
{
  modelObject.remove();
}

void SpaceTypesController::onReplaceObject(openstudio::model::ModelObject modelObject, const OSItemId& replacementItemId)
{
  // not yet implemented
}

void SpaceTypesController::onPurgeObjects(const openstudio::IddObjectType& iddObjectType)
{
  for (model::SpaceType spaceType : this->model().getConcreteModelObjects<model::SpaceType>()){
    if (spaceType.spaces().empty()){
      spaceType.remove();
    }
  }
}

void SpaceTypesController::onDrop(const OSItemId& itemId)
{
  boost::optional<model::ModelObject> modelObject = this->getModelObject(itemId);
  if (modelObject){
    if (modelObject->optionalCast<model::SpaceType>()){
      if (this->fromComponentLibrary(itemId)){
        modelObject = modelObject->clone(this->model());
      }
    }
  }else{
    boost::optional<model::Component> component = this->getComponent(itemId);
    if (component){
      if (component->primaryObject().optionalCast<model::SpaceType>()){
        boost::optional<model::ComponentData> componentData = this->model().insertComponent(*component);
      }
    }
  }
}

void SpaceTypesController::onInspectItem(OSItem* item)
{
}

} // openstudio

