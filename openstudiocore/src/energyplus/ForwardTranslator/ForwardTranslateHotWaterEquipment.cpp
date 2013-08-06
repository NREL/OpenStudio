/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <energyplus/ForwardTranslator.hpp>

#include <model/Model.hpp>
#include <model/HotWaterEquipment.hpp>
#include <model/HotWaterEquipment_Impl.hpp>
#include <model/HotWaterEquipmentDefinition.hpp>
#include <model/HotWaterEquipmentDefinition_Impl.hpp>
#include <model/Space.hpp>
#include <model/Space_Impl.hpp>
#include <model/SpaceType.hpp>
#include <model/SpaceType_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/LifeCycleCost.hpp>

#include <utilities/idd/HotWaterEquipment_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateHotWaterEquipment(
    HotWaterEquipment& modelObject)
{
  IdfObject idfObject(openstudio::IddObjectType::HotWaterEquipment);
  m_idfObjects.push_back(idfObject);

  BOOST_FOREACH(LifeCycleCost lifeCycleCost, modelObject.lifeCycleCosts()){
    translateAndMapModelObject(lifeCycleCost);
  }

  HotWaterEquipmentDefinition definition = modelObject.hotWaterEquipmentDefinition();

  idfObject.setString(HotWaterEquipmentFields::Name, modelObject.name().get());

  boost::optional<Space> space = modelObject.space();
  boost::optional<SpaceType> spaceType = modelObject.spaceType();
  if (space){
    boost::optional<ThermalZone> thermalZone = space->thermalZone();
    if (thermalZone){
      idfObject.setString(HotWaterEquipmentFields::ZoneorZoneListName, thermalZone->name().get());
    }
  }else if(spaceType){
    idfObject.setString(HotWaterEquipmentFields::ZoneorZoneListName, spaceType->name().get());
  }

  boost::optional<Schedule> schedule = modelObject.schedule();
  if (schedule){
    idfObject.setString(HotWaterEquipmentFields::ScheduleName, schedule->name().get());
  }

  idfObject.setString(HotWaterEquipmentFields::DesignLevelCalculationMethod, definition.designLevelCalculationMethod());

  double multiplier = modelObject.multiplier();

  OptionalDouble d = definition.designLevel();
  if (d){
    idfObject.setDouble(HotWaterEquipmentFields::DesignLevel, (*d)*multiplier);
  }

  d = definition.wattsperSpaceFloorArea();
  if (d){
    idfObject.setDouble(HotWaterEquipmentFields::PowerperZoneFloorArea, (*d)*multiplier);
  }

  d = definition.wattsperPerson();
  if (d){
    idfObject.setDouble(HotWaterEquipmentFields::PowerperPerson, (*d)*multiplier);
  }

  if (!definition.isFractionLatentDefaulted()){
    idfObject.setDouble(HotWaterEquipmentFields::FractionLatent, definition.fractionLatent());
  }

  if (!definition.isFractionRadiantDefaulted()){
    idfObject.setDouble(HotWaterEquipmentFields::FractionRadiant, definition.fractionRadiant());
  }

  if (!definition.isFractionLostDefaulted()){
    idfObject.setDouble(HotWaterEquipmentFields::FractionLost, definition.fractionLost());
  }

  if (!modelObject.isEndUseSubcategoryDefaulted()){
    idfObject.setString(HotWaterEquipmentFields::EndUseSubcategory, modelObject.endUseSubcategory());
  }

  return idfObject;
}

} // energyplus

} // openstudio

