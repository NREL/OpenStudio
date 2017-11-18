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

#include "RenewableEnergyController.hpp"
#include "RenewableEnergyView.hpp"

#include "../model/SpaceLoadDefinition.hpp"
#include "../model/SpaceLoadDefinition_Impl.hpp"
#include "../model/SpaceLoadInstance.hpp"
#include "../model/InternalMassDefinition.hpp"
#include "../model/InternalMassDefinition_Impl.hpp"
#include "../model/LightsDefinition.hpp"
#include "../model/LightsDefinition_Impl.hpp"
#include "../model/LuminaireDefinition.hpp"
#include "../model/LuminaireDefinition_Impl.hpp"
#include "../model/ElectricEquipmentDefinition.hpp"
#include "../model/ElectricEquipmentDefinition_Impl.hpp"
#include "../model/GasEquipmentDefinition.hpp"
#include "../model/GasEquipmentDefinition_Impl.hpp"
#include "../model/SteamEquipmentDefinition.hpp"
#include "../model/SteamEquipmentDefinition_Impl.hpp"
#include "../model/OtherEquipmentDefinition.hpp"
#include "../model/OtherEquipmentDefinition_Impl.hpp"
#include "../model/WaterUseEquipmentDefinition.hpp"
#include "../model/WaterUseEquipmentDefinition_Impl.hpp"
#include "../model/PeopleDefinition.hpp"
#include "../model/PeopleDefinition_Impl.hpp"
#include "../model/Photovoltaic.hpp"
#include "../model/Photovoltaic_Impl.hpp"
#include "../model/PhotovoltaicThermal.hpp"
#include "../model/PhotovoltaicThermal_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <QMessageBox>

#include "../utilities/core/Logger.hpp"

#include <utilities/idd/IddEnums.hxx>

namespace openstudio {

RenewableEnergyController::RenewableEnergyController(bool isIP, const model::Model& model)
  : ModelSubTabController(new RenewableEnergyView(isIP, model), model)
{
  connect(this, &RenewableEnergyController::toggleUnitsClicked, static_cast<ModelSubTabView*>(subTabView()), &ModelSubTabView::toggleUnitsClicked);
}

RenewableEnergyController::~RenewableEnergyController()
{
}

void RenewableEnergyController::onAddObject(const openstudio::IddObjectType& iddObjectType)
{
  switch(iddObjectType.value()){
	  case IddObjectType::OS_Exterior_PV_Thermal:
		  openstudio::model::PhotovoltaicThermal(this->model());
		  break;
	case IddObjectType::OS_Exterior_PV:
		openstudio::model::Photovoltaic(this->model());
		break;
    default:
      LOG_FREE(Error, "RenewableEnergyController", "Unknown IddObjectType '" << iddObjectType.valueName() << "'");
  }
}

void RenewableEnergyController::onCopyObject(const openstudio::model::ModelObject& modelObject)
{
  modelObject.clone(this->model());
}

void RenewableEnergyController::onRemoveObject(openstudio::model::ModelObject modelObject)
{
	boost::optional<model::ResourceObject> spaceLoadDefinition = modelObject.optionalCast<model::ResourceObject>();
	if (spaceLoadDefinition){
		modelObject.remove();
	}
}

void RenewableEnergyController::onReplaceObject(openstudio::model::ModelObject modelObject, const OSItemId& replacementItemId)
{
  // not yet implemented
}

void RenewableEnergyController::onPurgeObjects(const openstudio::IddObjectType& iddObjectType)
{
  this->model().purgeUnusedResourceObjects(iddObjectType);
}

void RenewableEnergyController::onDrop(const OSItemId& itemId)
{
  boost::optional<model::ModelObject> modelObject = this->getModelObject(itemId);
  if(modelObject){
    if(modelObject->optionalCast<model::SpaceLoadDefinition>()){
      if (this->fromComponentLibrary(itemId)){
        modelObject = modelObject->clone(this->model());
      }
    }
  }
}

void RenewableEnergyController::onInspectItem(OSItem* item)
{
}

void RenewableEnergyController::toggleUnits(bool displayIP)
{
}

} // openstudio
