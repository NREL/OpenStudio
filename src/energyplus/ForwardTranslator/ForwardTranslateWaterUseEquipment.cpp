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
#include "../../model/WaterUseEquipment.hpp"
#include "../../model/WaterUseEquipment_Impl.hpp"
#include "../../model/WaterUseEquipmentDefinition.hpp"
#include "../../model/WaterUseEquipmentDefinition_Impl.hpp"
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/LifeCycleCost.hpp"

#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/WaterUse_Equipment_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateWaterUseEquipment(
    WaterUseEquipment & modelObject)
{
  boost::optional<std::string> s;
  boost::optional<double> value;
  OptionalSchedule schedule;

  model::WaterUseEquipmentDefinition definition = modelObject.waterUseEquipmentDefinition();

  IdfObject idfObject(IddObjectType::WaterUse_Equipment);
  m_idfObjects.push_back(idfObject);

  for (LifeCycleCost lifeCycleCost : modelObject.lifeCycleCosts()){
    translateAndMapModelObject(lifeCycleCost);
  }

  // Name

  s = modelObject.name();
  if( s )
  {
    idfObject.setName(*s);
  }

  // EndUseSubcategory

  s = definition.endUseSubcategory();
  if( s )
  {
    idfObject.setString(WaterUse_EquipmentFields::EndUseSubcategory,s.get());
  }

  // PeakFlowRate

  value = definition.peakFlowRate();
  if( value )
  {
    idfObject.setDouble(WaterUse_EquipmentFields::PeakFlowRate,value.get());
  }

  // FlowRateFractionScheduleName

  if( boost::optional<Schedule> s = modelObject.flowRateFractionSchedule() )
  {
    translateAndMapModelObject(s.get());

    idfObject.setString(WaterUse_EquipmentFields::FlowRateFractionScheduleName,s->name().get());
  }

  // TargetTemperatureScheduleName

  if( boost::optional<Schedule> s = definition.targetTemperatureSchedule() )
  {
    translateAndMapModelObject(s.get());

    idfObject.setString(WaterUse_EquipmentFields::TargetTemperatureScheduleName,s->name().get());
  }

  // ZoneName

  if( boost::optional<Space> space = modelObject.space() )
  {
    if( boost::optional<ThermalZone> zone = space->thermalZone() )
    {
      translateAndMapModelObject(zone.get());

      idfObject.setString(WaterUse_EquipmentFields::ZoneName,zone->name().get());
    }
  }

  // SensibleFractionScheduleName

  if( boost::optional<Schedule> s = definition.sensibleFractionSchedule() )
  {
    translateAndMapModelObject(s.get());

    idfObject.setString(WaterUse_EquipmentFields::SensibleFractionScheduleName,s->name().get());
  }

  // LatentFractionScheduleName

  if( boost::optional<Schedule> s = definition.latentFractionSchedule() )
  {
    translateAndMapModelObject(s.get());

    idfObject.setString(WaterUse_EquipmentFields::LatentFractionScheduleName,s->name().get());
  }

  return idfObject;
}

} // energyplus
} // openstudio

