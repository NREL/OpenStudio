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
#include "../../model/Luminaire.hpp"
#include "../../model/Luminaire_Impl.hpp"
#include "../../model/LuminaireDefinition.hpp"
#include "../../model/LuminaireDefinition_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/LifeCycleCost.hpp"

#include <utilities/idd/Lights_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateLuminaire( Luminaire & modelObject )
{
  IdfObject idfObject(openstudio::IddObjectType::Lights);
  m_idfObjects.push_back(idfObject);

  for (LifeCycleCost lifeCycleCost : modelObject.lifeCycleCosts()){
    translateAndMapModelObject(lifeCycleCost);
  }

  LuminaireDefinition definition = modelObject.luminaireDefinition();

  idfObject.setString(LightsFields::Name, modelObject.name().get());

  boost::optional<Space> space = modelObject.space();
  boost::optional<SpaceType> spaceType = modelObject.spaceType();
  if (space){
    boost::optional<ThermalZone> thermalZone = space->thermalZone();
    if (thermalZone){
      idfObject.setString(LightsFields::ZoneorZoneListName, thermalZone->name().get());
    }
  }else if(spaceType){
    idfObject.setString(LightsFields::ZoneorZoneListName, spaceType->name().get());
  }

  boost::optional<Schedule> schedule = modelObject.schedule();
  if (schedule){
    idfObject.setString(LightsFields::ScheduleName, schedule->name().get());
  }

  idfObject.setString(LightsFields::DesignLevelCalculationMethod, "LightingLevel");

  double multiplier = modelObject.multiplier();

  idfObject.setDouble(LightsFields::LightingLevel, multiplier*definition.lightingPower());

  if (!definition.isReturnAirFractionDefaulted()){
    idfObject.setDouble(LightsFields::ReturnAirFraction, definition.returnAirFraction());
  }

  if (!definition.isFractionRadiantDefaulted()){
    idfObject.setDouble(LightsFields::FractionRadiant, definition.fractionRadiant());
  }

  if (!definition.isFractionVisibleDefaulted()){
    idfObject.setDouble(LightsFields::FractionVisible, definition.fractionVisible());
  }

  if (!modelObject.isFractionReplaceableDefaulted()){
    idfObject.setDouble(LightsFields::FractionReplaceable, modelObject.fractionReplaceable());
  }

  if (!modelObject.isEndUseSubcategoryDefaulted()){
    idfObject.setString(LightsFields::EndUseSubcategory, modelObject.endUseSubcategory());
  }

  if (!definition.isReturnAirFractionCalculatedfromPlenumTemperatureDefaulted()){
    idfObject.setDouble(LightsFields::ReturnAirFractionCalculatedfromPlenumTemperature,
                        definition.returnAirFractionCalculatedfromPlenumTemperature());
  }

  if (!definition.isReturnAirFractionFunctionofPlenumTemperatureCoefficient1Defaulted()){
    idfObject.setDouble(LightsFields::ReturnAirFractionFunctionofPlenumTemperatureCoefficient1,
                        definition.returnAirFractionFunctionofPlenumTemperatureCoefficient1());
  }

  if (!definition.isReturnAirFractionFunctionofPlenumTemperatureCoefficient2Defaulted()){
    idfObject.setDouble(LightsFields::ReturnAirFractionFunctionofPlenumTemperatureCoefficient2,
                        definition.returnAirFractionFunctionofPlenumTemperatureCoefficient2());
  }

  return idfObject;
}

} // energyplus

} // openstudio

