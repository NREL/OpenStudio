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
#include "../../model/ElectricEquipment.hpp"
#include "../../model/ElectricEquipment_Impl.hpp"
#include "../../model/ElectricEquipmentDefinition.hpp"
#include "../../model/ElectricEquipmentDefinition_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/LifeCycleCost.hpp"

#include <utilities/idd/ElectricEquipment_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateElectricEquipment( ElectricEquipment & modelObject )
{
  IdfObject idfObject(openstudio::IddObjectType::ElectricEquipment);
  m_idfObjects.push_back(idfObject);

  for (LifeCycleCost lifeCycleCost : modelObject.lifeCycleCosts()){
    translateAndMapModelObject(lifeCycleCost);
  }

  ElectricEquipmentDefinition definition = modelObject.electricEquipmentDefinition();

  idfObject.setString(ElectricEquipmentFields::Name, modelObject.name().get());

  boost::optional<Space> space = modelObject.space();
  boost::optional<SpaceType> spaceType = modelObject.spaceType();
  if (space){
    boost::optional<ThermalZone> thermalZone = space->thermalZone();
    if (thermalZone){
      idfObject.setString(ElectricEquipmentFields::ZoneorZoneListName, thermalZone->name().get());
    }
  }else if(spaceType){
    idfObject.setString(ElectricEquipmentFields::ZoneorZoneListName, spaceType->name().get());
  }

  boost::optional<Schedule> schedule = modelObject.schedule();
  if (schedule){
    idfObject.setString(ElectricEquipmentFields::ScheduleName, schedule->name().get());
  }

  idfObject.setString(ElectricEquipmentFields::DesignLevelCalculationMethod, definition.designLevelCalculationMethod());

  double multiplier = modelObject.multiplier();

  OptionalDouble d = definition.designLevel();
  if (d){
    idfObject.setDouble(ElectricEquipmentFields::DesignLevel, (*d)*multiplier);
  }

  d = definition.wattsperSpaceFloorArea();
  if (d){
    idfObject.setDouble(ElectricEquipmentFields::WattsperZoneFloorArea, (*d)*multiplier);
  }

  d = definition.wattsperPerson();
  if (d){
    idfObject.setDouble(ElectricEquipmentFields::WattsperPerson, (*d)*multiplier);
  }

  if (!definition.isFractionLatentDefaulted()){
    idfObject.setDouble(ElectricEquipmentFields::FractionLatent, definition.fractionLatent());
  }

  if (!definition.isFractionRadiantDefaulted()){
    idfObject.setDouble(ElectricEquipmentFields::FractionRadiant, definition.fractionRadiant());
  }

  if (!definition.isFractionLostDefaulted()){
    idfObject.setDouble(ElectricEquipmentFields::FractionLost, definition.fractionLost());
  }

  if (!modelObject.isEndUseSubcategoryDefaulted()){
    idfObject.setString(ElectricEquipmentFields::EndUseSubcategory, modelObject.endUseSubcategory());
  }

  return idfObject;
}

} // energyplus

} // openstudio

