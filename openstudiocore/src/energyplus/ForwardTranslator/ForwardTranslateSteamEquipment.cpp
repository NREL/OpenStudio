/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/SteamEquipment.hpp"
#include "../../model/SteamEquipment_Impl.hpp"
#include "../../model/SteamEquipmentDefinition.hpp"
#include "../../model/SteamEquipmentDefinition_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/LifeCycleCost.hpp"

#include <utilities/idd/SteamEquipment_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateSteamEquipment(
    SteamEquipment& modelObject)
{
  IdfObject idfObject(openstudio::IddObjectType::SteamEquipment);
  m_idfObjects.push_back(idfObject);

  for (LifeCycleCost lifeCycleCost : modelObject.lifeCycleCosts()){
    translateAndMapModelObject(lifeCycleCost);
  }

  SteamEquipmentDefinition definition = modelObject.steamEquipmentDefinition();

  idfObject.setString(SteamEquipmentFields::Name, modelObject.name().get());

  boost::optional<Space> space = modelObject.space();
  boost::optional<SpaceType> spaceType = modelObject.spaceType();
  if (space){
    boost::optional<ThermalZone> thermalZone = space->thermalZone();
    if (thermalZone){
      idfObject.setString(SteamEquipmentFields::ZoneorZoneListName, thermalZone->name().get());
    }
  }else if(spaceType){
    idfObject.setString(SteamEquipmentFields::ZoneorZoneListName, spaceType->name().get());
  }

  boost::optional<Schedule> schedule = modelObject.schedule();
  if (schedule){
    idfObject.setString(SteamEquipmentFields::ScheduleName, schedule->name().get());
  }

  idfObject.setString(SteamEquipmentFields::DesignLevelCalculationMethod, definition.designLevelCalculationMethod());

  double multiplier = modelObject.multiplier();

  OptionalDouble d = definition.designLevel();
  if (d){
    idfObject.setDouble(SteamEquipmentFields::DesignLevel, (*d)*multiplier);
  }

  d = definition.wattsperSpaceFloorArea();
  if (d){
    idfObject.setDouble(SteamEquipmentFields::PowerperZoneFloorArea, (*d)*multiplier);
  }

  d = definition.wattsperPerson();
  if (d){
    idfObject.setDouble(SteamEquipmentFields::PowerperPerson, (*d)*multiplier);
  }

  if (!definition.isFractionLatentDefaulted()){
    idfObject.setDouble(SteamEquipmentFields::FractionLatent, definition.fractionLatent());
  }

  if (!definition.isFractionRadiantDefaulted()){
    idfObject.setDouble(SteamEquipmentFields::FractionRadiant, definition.fractionRadiant());
  }

  if (!definition.isFractionLostDefaulted()){
    idfObject.setDouble(SteamEquipmentFields::FractionLost, definition.fractionLost());
  }

  if (!modelObject.isEndUseSubcategoryDefaulted()){
    idfObject.setString(SteamEquipmentFields::EndUseSubcategory, modelObject.endUseSubcategory());
  }

  return idfObject;
}

} // energyplus

} // openstudio

