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

#include "../ReverseTranslator.hpp"

#include "../../model/ElectricEquipment.hpp"
#include "../../model/ElectricEquipment_Impl.hpp"
#include "../../model/ElectricEquipmentDefinition.hpp"
#include "../../model/ElectricEquipmentDefinition_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/ElectricEquipment_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateElectricEquipment( const WorkspaceObject & workspaceObject )
{
  if( workspaceObject.iddObject().type() != IddObjectType::ElectricEquipment ){
    LOG(Error, "WorkspaceObject is not IddObjectType: ElectricEquipment");
    return boost::none;
  }

  // create the definition
  openstudio::model::ElectricEquipmentDefinition definition(m_model);
  
  OptionalString s = workspaceObject.name();
  if(s){
    definition.setName(*s + " Definition");
  }

  s = workspaceObject.getString(openstudio::ElectricEquipmentFields::DesignLevelCalculationMethod, true);
  OS_ASSERT(s);

  OptionalDouble d;
  if (istringEqual("EquipmentLevel", *s)){
    d = workspaceObject.getDouble(openstudio::ElectricEquipmentFields::DesignLevel);
    if (d){
      definition.setDesignLevel(*d);
    }else{
      LOG(Error, "EquipmentLevel value not found for workspace object " << workspaceObject);
    }
  }else if(istringEqual("Watts/Area", *s)){
    d = workspaceObject.getDouble(openstudio::ElectricEquipmentFields::WattsperZoneFloorArea);
    if (d){
      definition.setWattsperSpaceFloorArea(*d);
    }else{
      LOG(Error, "Watts/Area value not found for workspace object " << workspaceObject);
    }
  }else if(istringEqual("Watts/Person", *s)){
    d = workspaceObject.getDouble(openstudio::ElectricEquipmentFields::WattsperPerson);
    if (d){
      definition.setWattsperPerson(*d);
    }else{
      LOG(Error, "Watts/Person value not found for workspace object " << workspaceObject);
    }
  }else{
    LOG(Error, "Unknown DesignLevelCalculationMethod value for workspace object" << workspaceObject);
  }

  d = workspaceObject.getDouble(openstudio::ElectricEquipmentFields::FractionLatent);
  if (d){
    definition.setFractionLatent(*d);
  }

  d = workspaceObject.getDouble(openstudio::ElectricEquipmentFields::FractionRadiant);
  if (d){
    definition.setFractionRadiant(*d);
  }

  d = workspaceObject.getDouble(openstudio::ElectricEquipmentFields::FractionLost);
  if (d){
    definition.setFractionLost(*d);
  }

  // create the instance
  ElectricEquipment electricEquipment(definition);

  s = workspaceObject.name();
  if(s){
    electricEquipment.setName(*s);
  }

  OptionalWorkspaceObject target = workspaceObject.getTarget(openstudio::ElectricEquipmentFields::ZoneorZoneListName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (modelObject->optionalCast<Space>()){
        electricEquipment.setSpace(modelObject->cast<Space>());
      }else if (modelObject->optionalCast<SpaceType>()){
        electricEquipment.setSpaceType(modelObject->cast<SpaceType>());
      }
    }
  }

  target = workspaceObject.getTarget(openstudio::ElectricEquipmentFields::ScheduleName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (OptionalSchedule intermediate = modelObject->optionalCast<Schedule>()){
        Schedule schedule = *intermediate;
        electricEquipment.setSchedule(schedule);
      }
    }
  }

  s = workspaceObject.getString(openstudio::ElectricEquipmentFields::EndUseSubcategory);
  if(s){
    electricEquipment.setEndUseSubcategory(*s);
  }
       
  return electricEquipment;
}

} // energyplus

} // openstudio

