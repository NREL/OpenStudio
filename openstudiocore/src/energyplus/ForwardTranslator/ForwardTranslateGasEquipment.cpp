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
#include "../../model/GasEquipment.hpp"
#include "../../model/GasEquipment_Impl.hpp"
#include "../../model/GasEquipmentDefinition.hpp"
#include "../../model/GasEquipmentDefinition_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/LifeCycleCost.hpp"

#include <utilities/idd/GasEquipment_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateGasEquipment( GasEquipment& modelObject )
{
  IdfObject idfObject(openstudio::IddObjectType::GasEquipment);
  m_idfObjects.push_back(idfObject);

  for (LifeCycleCost lifeCycleCost : modelObject.lifeCycleCosts()){
    translateAndMapModelObject(lifeCycleCost);
  }

  GasEquipmentDefinition definition = modelObject.gasEquipmentDefinition();

  idfObject.setString(GasEquipmentFields::Name, modelObject.name().get());

  boost::optional<Space> space = modelObject.space();
  boost::optional<SpaceType> spaceType = modelObject.spaceType();
  if (space){
    boost::optional<ThermalZone> thermalZone = space->thermalZone();
    if (thermalZone){
      idfObject.setString(GasEquipmentFields::ZoneorZoneListName, thermalZone->name().get());
    }
  }else if(spaceType){
    idfObject.setString(GasEquipmentFields::ZoneorZoneListName, spaceType->name().get());
  }

  boost::optional<Schedule> schedule = modelObject.schedule();
  if (schedule){
    idfObject.setString(GasEquipmentFields::ScheduleName, schedule->name().get());
  }

  idfObject.setString(GasEquipmentFields::DesignLevelCalculationMethod, definition.designLevelCalculationMethod());

  double multiplier = modelObject.multiplier();

  OptionalDouble d = definition.designLevel();
  if (d){
    idfObject.setDouble(GasEquipmentFields::DesignLevel, (*d)*multiplier);
  }

  d = definition.wattsperSpaceFloorArea();
  if (d){
    idfObject.setDouble(GasEquipmentFields::PowerperZoneFloorArea, (*d)*multiplier);
  }

  d = definition.wattsperPerson();
  if (d){
    idfObject.setDouble(GasEquipmentFields::PowerperPerson, (*d)*multiplier);
  }

  if (!definition.isFractionLatentDefaulted()){
    idfObject.setDouble(GasEquipmentFields::FractionLatent, definition.fractionLatent());
  }

  if (!definition.isFractionRadiantDefaulted()){
    idfObject.setDouble(GasEquipmentFields::FractionRadiant, definition.fractionRadiant());
  }

  if (!definition.isFractionLostDefaulted()){
    idfObject.setDouble(GasEquipmentFields::FractionLost, definition.fractionLost());
  }

  if (!definition.isCarbonDioxideGenerationRateDefaulted()){
    idfObject.setDouble(GasEquipmentFields::CarbonDioxideGenerationRate, definition.carbonDioxideGenerationRate());
  }

  if (!modelObject.isEndUseSubcategoryDefaulted()){
    idfObject.setString(GasEquipmentFields::EndUseSubcategory, modelObject.endUseSubcategory());
  }

  return idfObject;
}

} // energyplus

} // openstudio

