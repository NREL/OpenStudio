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

#include "LoadsController.hpp"
#include "LoadsView.hpp"

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

#include "../utilities/core/Assert.hpp"

#include <QMessageBox>

#include "../utilities/core/Logger.hpp"

#include <utilities/idd/IddEnums.hxx>

namespace openstudio {

LoadsController::LoadsController(bool isIP, const model::Model& model)
  : ModelSubTabController(new LoadsView(isIP, model), model)
{
  connect(this, &LoadsController::toggleUnitsClicked, static_cast<ModelSubTabView*>(subTabView()), &ModelSubTabView::toggleUnitsClicked);
}

LoadsController::~LoadsController()
{
}

void LoadsController::onAddObject(const openstudio::IddObjectType& iddObjectType)
{
  switch(iddObjectType.value()){
    case IddObjectType::OS_People_Definition:
      openstudio::model::PeopleDefinition(this->model());
      break;
    case IddObjectType::OS_InternalMass_Definition:
      openstudio::model::InternalMassDefinition(this->model());
      break;
    case IddObjectType::OS_Lights_Definition:
      openstudio::model::LightsDefinition(this->model());
      break;
    case IddObjectType::OS_Luminaire_Definition:
      openstudio::model::LuminaireDefinition(this->model());
      break;
    case IddObjectType::OS_ElectricEquipment_Definition:
      openstudio::model::ElectricEquipmentDefinition(this->model());
      break;
    case IddObjectType::OS_GasEquipment_Definition:
      openstudio::model::GasEquipmentDefinition(this->model());
      break;
    case IddObjectType::OS_SteamEquipment_Definition:
      openstudio::model::SteamEquipmentDefinition(this->model());
      break;
    case IddObjectType::OS_OtherEquipment_Definition:
      openstudio::model::OtherEquipmentDefinition(this->model());
      break;
    case IddObjectType::OS_WaterUse_Equipment_Definition:
      openstudio::model::WaterUseEquipmentDefinition(this->model());
      break;
    default:
      LOG_FREE(Error, "LoadsController", "Unknown IddObjectType '" << iddObjectType.valueName() << "'");
  }
}

void LoadsController::onCopyObject(const openstudio::model::ModelObject& modelObject)
{
  modelObject.clone(this->model());
}

void LoadsController::onRemoveObject(openstudio::model::ModelObject modelObject)
{
  boost::optional<model::SpaceLoadDefinition> spaceLoadDefinition = modelObject.optionalCast<model::SpaceLoadDefinition>();
  if (spaceLoadDefinition){

    unsigned numInstances = spaceLoadDefinition->instances().size();
    if (numInstances > 0){
      QMessageBox msgBox(subTabView());
      msgBox.setText("There are " + QString::number(numInstances) + " instances that reference this definition.");
      msgBox.setInformativeText("Do you want to remove this definition and all of its instances?");
      msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
      msgBox.setDefaultButton(QMessageBox::Yes);
      int ret = msgBox.exec();
      if (ret == QMessageBox::No){
        return;
      }
    }
    modelObject.remove();
  }
}

void LoadsController::onReplaceObject(openstudio::model::ModelObject modelObject, const OSItemId& replacementItemId)
{
  // not yet implemented
}

void LoadsController::onPurgeObjects(const openstudio::IddObjectType& iddObjectType)
{
  this->model().purgeUnusedResourceObjects(iddObjectType);
}

void LoadsController::onDrop(const OSItemId& itemId)
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

void LoadsController::onInspectItem(OSItem* item)
{
}

void LoadsController::toggleUnits(bool displayIP)
{
}

} // openstudio
