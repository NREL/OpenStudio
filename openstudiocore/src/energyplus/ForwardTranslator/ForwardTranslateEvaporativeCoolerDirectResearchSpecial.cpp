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
#include "../../model/EvaporativeCoolerDirectResearchSpecial.hpp"
#include "../../model/EvaporativeCoolerDirectResearchSpecial_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/core/Logger.hpp"
#include <utilities/idd/EvaporativeCooler_Direct_ResearchSpecial_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateEvaporativeCoolerDirectResearchSpecial( EvaporativeCoolerDirectResearchSpecial & modelObject )
{
  OptionalString s;
  OptionalDouble optD;
  OptionalModelObject temp;
  double value;

  IdfObject idfObject(IddObjectType::EvaporativeCooler_Direct_ResearchSpecial);

  m_idfObjects.push_back(idfObject);

  // Name
  s = modelObject.name();
  if(s)
  {
    idfObject.setName(*s);
  }

  // AvailabilityScheduleName
  Schedule sched = modelObject.availableSchedule();
  translateAndMapModelObject(sched);
  idfObject.setString(EvaporativeCooler_Direct_ResearchSpecialFields::AvailabilityScheduleName,sched.name().get());

  // CoolerEffectiveness
  value = modelObject.coolerDesignEffectiveness();
  idfObject.setDouble(EvaporativeCooler_Direct_ResearchSpecialFields::CoolerDesignEffectiveness,value);

  // RecirculatingWaterPumpPowerConsumption
  if ( modelObject.isRecirculatingWaterPumpPowerConsumptionAutosized() ) {
    idfObject.setString(EvaporativeCooler_Direct_ResearchSpecialFields::RecirculatingWaterPumpDesignPower,"autosize");
  } else if( (optD = modelObject.recirculatingWaterPumpPowerConsumption()) ) {
    idfObject.setDouble(EvaporativeCooler_Direct_ResearchSpecialFields::RecirculatingWaterPumpDesignPower, optD.get());
  }

  // Primary Air Design Flow Rate
  if ( modelObject.isPrimaryAirDesignFlowRateAutosized() ) {
    idfObject.setString(EvaporativeCooler_Direct_ResearchSpecialFields::PrimaryAirDesignFlowRate,"Autosize");
  } else if( (optD = modelObject.primaryAirDesignFlowRate()) ) {
    idfObject.setDouble(EvaporativeCooler_Direct_ResearchSpecialFields::PrimaryAirDesignFlowRate, optD.get());
  }

  // AirInletNodeName
  temp = modelObject.inletModelObject();
  if(temp)
  {
    s = temp->name();
    if(s)
    {
      idfObject.setString(EvaporativeCooler_Direct_ResearchSpecialFields::AirInletNodeName,*s);
    }
  }

  // AirOutletNodeName
  temp = modelObject.outletModelObject();
  if(temp)
  {
    s = temp->name();
    if(s)
    {
      idfObject.setString(EvaporativeCooler_Direct_ResearchSpecialFields::AirOutletNodeName,*s);
    }
  }

  // SensorNodeName
  temp = modelObject.sensorNode();
  if(temp)
  {
    s = temp->name();
    if(s)
    {
      idfObject.setString(EvaporativeCooler_Direct_ResearchSpecialFields::SensorNodeName,*s);
    }
  }

  // DriftLossFraction
  value = modelObject.driftLossFraction();
  idfObject.setDouble(EvaporativeCooler_Direct_ResearchSpecialFields::DriftLossFraction,value);

  // BlowdownConcentrationRatio
  value = modelObject.blowdownConcentrationRatio();
  if( value < 2 )
  {
    idfObject.setDouble(EvaporativeCooler_Direct_ResearchSpecialFields::BlowdownConcentrationRatio,2.0);
  }
  else
  {
    idfObject.setDouble(EvaporativeCooler_Direct_ResearchSpecialFields::BlowdownConcentrationRatio,value);
  }

  // EffectivenessFlowRatioModifierCurveName
  if( auto curve = modelObject.effectivenessFlowRatioModifierCurve() ) {
    auto _curve = translateAndMapModelObject(curve.get());
    OS_ASSERT(_curve);
    idfObject.setString(EvaporativeCooler_Direct_ResearchSpecialFields::EffectivenessFlowRatioModifierCurveName,_curve->name().get());
  }

  // WaterPumpPowerSizingFactor
  value = modelObject.waterPumpPowerSizingFactor();
  idfObject.setDouble(EvaporativeCooler_Direct_ResearchSpecialFields::WaterPumpPowerSizingFactor,value);

  // WaterPumpPowerModifierCurveName
  if( auto curve = modelObject.waterPumpPowerModifierCurve() ) {
    auto _curve = translateAndMapModelObject(curve.get());
    OS_ASSERT(_curve);
    idfObject.setString(EvaporativeCooler_Direct_ResearchSpecialFields::WaterPumpPowerModifierCurveName,_curve->name().get());
  }

  // Evaporative Operation Minimum Drybulb Temperature (Double)
  idfObject.setDouble(EvaporativeCooler_Direct_ResearchSpecialFields::EvaporativeOperationMinimumDrybulbTemperature,
                      modelObject.evaporativeOperationMinimumDrybulbTemperature());


  // Evaporative Operation Maximum Limit Wetbulb Temperature (Double)
  idfObject.setDouble(EvaporativeCooler_Direct_ResearchSpecialFields::EvaporativeOperationMaximumLimitWetbulbTemperature,
                      modelObject.evaporativeOperationMaximumLimitWetbulbTemperature());

  // Evaporative Operation Maximum Limit Drybulb Temperature (Double)
  idfObject.setDouble(EvaporativeCooler_Direct_ResearchSpecialFields::EvaporativeOperationMaximumLimitDrybulbTemperature,
                      modelObject.evaporativeOperationMaximumLimitDrybulbTemperature());

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

