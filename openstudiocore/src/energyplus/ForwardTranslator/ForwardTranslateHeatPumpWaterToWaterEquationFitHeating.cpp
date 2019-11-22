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
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/HeatPumpWaterToWaterEquationFitCooling.hpp"
#include "../../model/HeatPumpWaterToWaterEquationFitCooling_Impl.hpp"
#include "../../model/HeatPumpWaterToWaterEquationFitHeating.hpp"
#include "../../model/HeatPumpWaterToWaterEquationFitHeating_Impl.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/HeatPump_WaterToWater_EquationFit_Heating_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateHeatPumpWaterToWaterEquationFitHeating( HeatPumpWaterToWaterEquationFitHeating & modelObject )
{
  IdfObject idfObject(IddObjectType::HeatPump_WaterToWater_EquationFit_Heating);
  boost::optional<double> optvalue;

  m_idfObjects.push_back(idfObject);

  if( auto s = modelObject.name() ) {
    idfObject.setName(*s);
  }

  if( auto value = modelObject.supplyOutletModelObject() ) {
    idfObject.setString(HeatPump_WaterToWater_EquationFit_HeatingFields::LoadSideOutletNodeName,value->name().get());
  }

  if( auto value = modelObject.supplyInletModelObject() ) {
    idfObject.setString(HeatPump_WaterToWater_EquationFit_HeatingFields::LoadSideInletNodeName,value->name().get());
  }

  if( auto value = modelObject.demandOutletModelObject() ) {
    idfObject.setString(HeatPump_WaterToWater_EquationFit_HeatingFields::SourceSideOutletNodeName,value->name().get());
  }

  if( auto value = modelObject.demandInletModelObject() ) {
    idfObject.setString(HeatPump_WaterToWater_EquationFit_HeatingFields::SourceSideInletNodeName,value->name().get());
  }

  if (modelObject.isReferenceLoadSideFlowRateAutosized())
  {
    idfObject.setString(HeatPump_WaterToWater_EquationFit_HeatingFields::ReferenceLoadSideFlowRate, "Autosize");
  }
  else if ((optvalue = modelObject.referenceLoadSideFlowRate()))
  {
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_HeatingFields::ReferenceLoadSideFlowRate, optvalue.get());
  }

  if (modelObject.isReferenceSourceSideFlowRateAutosized())
  {
    idfObject.setString(HeatPump_WaterToWater_EquationFit_HeatingFields::ReferenceSourceSideFlowRate, "Autosize");
  }
  else if ((optvalue = modelObject.referenceSourceSideFlowRate()))
  {
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_HeatingFields::ReferenceSourceSideFlowRate, optvalue.get());
  }

  if (modelObject.isRatedHeatingCapacityAutosized())
  {
    idfObject.setString(HeatPump_WaterToWater_EquationFit_HeatingFields::ReferenceHeatingCapacity, "Autosize");
  }
  else if ((optvalue = modelObject.ratedHeatingCapacity()))
  {
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_HeatingFields::ReferenceHeatingCapacity, optvalue.get());
  }

  if (modelObject.isRatedHeatingPowerConsumptionAutosized())
  {
    idfObject.setString(HeatPump_WaterToWater_EquationFit_HeatingFields::ReferenceHeatingPowerConsumption, "Autosize");
  }
  else if ((optvalue = modelObject.ratedHeatingPowerConsumption()))
  {
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_HeatingFields::ReferenceHeatingPowerConsumption, optvalue.get());
  }

  {
    auto value = modelObject.heatingCapacityCoefficient1();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCapacityCoefficient1,value);
  }

  {
    auto value = modelObject.heatingCapacityCoefficient2();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCapacityCoefficient2,value);
  }

  {
    auto value = modelObject.heatingCapacityCoefficient3();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCapacityCoefficient3,value);
  }

  {
    auto value = modelObject.heatingCapacityCoefficient4();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCapacityCoefficient4,value);
  }

  {
    auto value = modelObject.heatingCapacityCoefficient5();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCapacityCoefficient5,value);
  }

  {
    auto value = modelObject.heatingCompressorPowerCoefficient1();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCompressorPowerCoefficient1,value);
  }

  {
    auto value = modelObject.heatingCompressorPowerCoefficient2();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCompressorPowerCoefficient2,value);
  }

  {
    auto value = modelObject.heatingCompressorPowerCoefficient3();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCompressorPowerCoefficient3,value);
  }

  {
    auto value = modelObject.heatingCompressorPowerCoefficient4();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCompressorPowerCoefficient4,value);
  }

  {
    auto value = modelObject.heatingCompressorPowerCoefficient5();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCompressorPowerCoefficient5,value);
  }


  {
    auto value = modelObject.referenceCoefficientofPerformance();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_HeatingFields::ReferenceCoefficientofPerformance, value);
  }

  {
    auto value = modelObject.sizingFactor();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_HeatingFields::SizingFactor, value);
  }

  boost::optional<HeatPumpWaterToWaterEquationFitCooling> companion = modelObject.companionCoolingHeatPump();

  if (companion)
  {
    boost::optional<IdfObject> _companion = translateAndMapModelObject(companion.get());
    if (_companion)
    {
      idfObject.setString(HeatPump_WaterToWater_EquationFit_HeatingFields::CompanionCoolingHeatPumpName, _companion->name().get());
    }
  }

  return idfObject;
}

} // energyplus

} // openstudio

