/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/InternalMass.hpp"
#include "../../model/InternalMass_Impl.hpp"
#include "../../model/InternalMassDefinition.hpp"
#include "../../model/InternalMassDefinition_Impl.hpp"
#include "../../model/People.hpp"
#include "../../model/People_Impl.hpp"
#include "../../model/PeopleDefinition.hpp"
#include "../../model/PeopleDefinition_Impl.hpp"
#include "../../model/Lights.hpp"
#include "../../model/Lights_Impl.hpp"
#include "../../model/LightsDefinition.hpp"
#include "../../model/LightsDefinition_Impl.hpp"
#include "../../model/Luminaire.hpp"
#include "../../model/Luminaire_Impl.hpp"
#include "../../model/LuminaireDefinition.hpp"
#include "../../model/LuminaireDefinition_Impl.hpp"
#include "../../model/ElectricEquipment.hpp"
#include "../../model/ElectricEquipment_Impl.hpp"
#include "../../model/ElectricEquipmentDefinition.hpp"
#include "../../model/ElectricEquipmentDefinition_Impl.hpp"
#include "../../model/GasEquipment.hpp"
#include "../../model/GasEquipment_Impl.hpp"
#include "../../model/GasEquipmentDefinition.hpp"
#include "../../model/GasEquipmentDefinition_Impl.hpp"
#include "../../model/HotWaterEquipment.hpp"
#include "../../model/HotWaterEquipment_Impl.hpp"
#include "../../model/SteamEquipment.hpp"
#include "../../model/SteamEquipment_Impl.hpp"
#include "../../model/OtherEquipment.hpp"
#include "../../model/OtherEquipment_Impl.hpp"
#include "../../model/SpaceInfiltrationDesignFlowRate.hpp"
#include "../../model/SpaceInfiltrationDesignFlowRate_Impl.hpp"
#include "../../model/SpaceInfiltrationEffectiveLeakageArea.hpp"
#include "../../model/SpaceInfiltrationEffectiveLeakageArea_Impl.hpp"
#include "../../model/DesignSpecificationOutdoorAir.hpp"
#include "../../model/DesignSpecificationOutdoorAir_Impl.hpp"


#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/WorkspaceObjectOrder.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"

#include <utilities/idd/ZoneList_FieldEnums.hxx>

#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateSpaceType( SpaceType & modelObject )
{
  std::vector<Space> spaces = modelObject.spaces();

  // check if this is a dummy space type meant to prevent inheriting building space type
  std::vector<ModelObject> children = modelObject.children();
  if (children.empty()){
    LOG(Info, "SpaceType " << modelObject.name().get() << " has no children, it will not be translated");
    return boost::none;
  }

  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::ZoneList, 
                                                       modelObject);

  std::set<std::string> zoneNames;
  for (const Space& space : spaces){
    boost::optional<ThermalZone> thermalZone = space.thermalZone();
    if (thermalZone){
      zoneNames.insert(thermalZone->name().get());
    }
  }

  idfObject.clearExtensibleGroups();
  for (const std::string& zoneName : zoneNames){
    idfObject.pushExtensibleGroup(std::vector<std::string>(1, zoneName));
  }

  // translate internal mass
  InternalMassVector internalMasses = modelObject.internalMass();
  std::sort(internalMasses.begin(), internalMasses.end(), WorkspaceObjectNameLess());
  for (InternalMass& internalMass : internalMasses){
    translateAndMapModelObject(internalMass);
  }

  // translate lights
  LightsVector lights = modelObject.lights();
  std::sort(lights.begin(), lights.end(), WorkspaceObjectNameLess());
  for (Lights& light : lights){
    translateAndMapModelObject(light);
  }

  // translate luminaires
  LuminaireVector luminaires = modelObject.luminaires();
  std::sort(luminaires.begin(), luminaires.end(), WorkspaceObjectNameLess());
  for (Luminaire& luminaire : luminaires){
    translateAndMapModelObject(luminaire);
  }

  // translate people
  PeopleVector people = modelObject.people();
  std::sort(people.begin(), people.end(), WorkspaceObjectNameLess());
  for (People& person : people){
    translateAndMapModelObject(person);
  }

  // translate electric equipment
  ElectricEquipmentVector electricEquipment = modelObject.electricEquipment();
  std::sort(electricEquipment.begin(), electricEquipment.end(), WorkspaceObjectNameLess());
  for (ElectricEquipment& equipment : electricEquipment){
    translateAndMapModelObject(equipment);
  }

  // translate gas equipment
  GasEquipmentVector gasEquipment = modelObject.gasEquipment();
  std::sort(gasEquipment.begin(), gasEquipment.end(), WorkspaceObjectNameLess());
  for (GasEquipment& equipment : gasEquipment){
    translateAndMapModelObject(equipment);
  }

  // translate hot water equipment
  HotWaterEquipmentVector hotWaterEquipment = modelObject.hotWaterEquipment();
  std::sort(hotWaterEquipment.begin(), hotWaterEquipment.end(), WorkspaceObjectNameLess());
  for (HotWaterEquipment& equipment : hotWaterEquipment){
    translateAndMapModelObject(equipment);
  }

  // translate steam equipment
  SteamEquipmentVector steamEquipment = modelObject.steamEquipment();
  std::sort(steamEquipment.begin(), steamEquipment.end(), WorkspaceObjectNameLess());
  for (SteamEquipment& equipment : steamEquipment){
    translateAndMapModelObject(equipment);
  }

  // translate other equipment
  OtherEquipmentVector otherEquipment = modelObject.otherEquipment();
  std::sort(otherEquipment.begin(), otherEquipment.end(), WorkspaceObjectNameLess());
  for (OtherEquipment& equipment : otherEquipment){
    translateAndMapModelObject(equipment);
  }

  // translate SpaceInfiltration_DesignFlowRate
  SpaceInfiltrationDesignFlowRateVector spaceInfiltrationDesignFlowRates = modelObject.spaceInfiltrationDesignFlowRates();
  std::sort(spaceInfiltrationDesignFlowRates.begin(), spaceInfiltrationDesignFlowRates.end(), WorkspaceObjectNameLess());
  for (SpaceInfiltrationDesignFlowRate& spaceInfiltrationDesignFlowRate : spaceInfiltrationDesignFlowRates){
    translateAndMapModelObject(spaceInfiltrationDesignFlowRate);
  }

  // translate SpaceInfiltration_EffectiveLeakageArea
  SpaceInfiltrationEffectiveLeakageAreaVector spaceInfiltrationEffectiveLeakageAreas = modelObject.spaceInfiltrationEffectiveLeakageAreas();
  std::sort(spaceInfiltrationEffectiveLeakageAreas.begin(), spaceInfiltrationEffectiveLeakageAreas.end(), WorkspaceObjectNameLess());
  for (SpaceInfiltrationEffectiveLeakageArea& spaceInfiltrationEffectiveLeakageArea : spaceInfiltrationEffectiveLeakageAreas){
    translateAndMapModelObject(spaceInfiltrationEffectiveLeakageArea);
  }

  return idfObject;
}

} // energyplus

} // openstudio

