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
#include <utilities/idd/HeatPump_WaterToWater_EquationFit_Cooling_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateHeatPumpWaterToWaterEquationFitCooling( HeatPumpWaterToWaterEquationFitCooling & modelObject )
{
  IdfObject idfObject(IddObjectType::HeatPump_WaterToWater_EquationFit_Cooling);
  boost::optional<double> optvalue;

  m_idfObjects.push_back(idfObject);

  if( auto s = modelObject.name() ) {
    idfObject.setName(*s);
  }

  if( auto value = modelObject.supplyOutletModelObject() ) {
    idfObject.setString(HeatPump_WaterToWater_EquationFit_CoolingFields::LoadSideOutletNodeName,value->name().get());
  }

  if( auto value = modelObject.supplyInletModelObject() ) {
    idfObject.setString(HeatPump_WaterToWater_EquationFit_CoolingFields::LoadSideInletNodeName,value->name().get());
  }

  if( auto value = modelObject.demandOutletModelObject() ) {
    idfObject.setString(HeatPump_WaterToWater_EquationFit_CoolingFields::SourceSideOutletNodeName,value->name().get());
  }

  if( auto value = modelObject.demandInletModelObject() ) {
    idfObject.setString(HeatPump_WaterToWater_EquationFit_CoolingFields::SourceSideInletNodeName,value->name().get());
  }

  if (modelObject.isReferenceLoadSideFlowRateAutosized())
  {
    idfObject.setString(HeatPump_WaterToWater_EquationFit_CoolingFields::ReferenceLoadSideFlowRate, "Autosize");
  }
  else if ((optvalue = modelObject.referenceLoadSideFlowRate()))
  {
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_CoolingFields::ReferenceLoadSideFlowRate, optvalue.get());
  }

  if (modelObject.isReferenceSourceSideFlowRateAutosized())
  {
    idfObject.setString(HeatPump_WaterToWater_EquationFit_CoolingFields::ReferenceSourceSideFlowRate, "Autosize");
  }
  else if ((optvalue = modelObject.referenceSourceSideFlowRate()))
  {
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_CoolingFields::ReferenceSourceSideFlowRate, optvalue.get());
  }

  if (modelObject.isRatedCoolingCapacityAutosized())
  {
    idfObject.setString(HeatPump_WaterToWater_EquationFit_CoolingFields::ReferenceCoolingCapacity, "Autosize");
  }
  else if ((optvalue = modelObject.ratedCoolingCapacity()))
  {
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_CoolingFields::ReferenceCoolingCapacity, optvalue.get());
  }

  if (modelObject.isRatedCoolingPowerConsumptionAutosized())
  {
    idfObject.setString(HeatPump_WaterToWater_EquationFit_CoolingFields::ReferenceCoolingPowerConsumption, "Autosize");
  }
  else if ((optvalue = modelObject.ratedCoolingPowerConsumption()))
  {
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_CoolingFields::ReferenceCoolingPowerConsumption, optvalue.get());
  }

  {
    auto value = modelObject.coolingCapacityCoefficient1();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCapacityCoefficient1,value);
  }

  {
    auto value = modelObject.coolingCapacityCoefficient2();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCapacityCoefficient2,value);
  }

  {
    auto value = modelObject.coolingCapacityCoefficient3();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCapacityCoefficient3,value);
  }

  {
    auto value = modelObject.coolingCapacityCoefficient4();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCapacityCoefficient4,value);
  }

  {
    auto value = modelObject.coolingCapacityCoefficient5();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCapacityCoefficient5,value);
  }

  {
    auto value = modelObject.coolingCompressorPowerCoefficient1();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCompressorPowerCoefficient1,value);
  }

  {
    auto value = modelObject.coolingCompressorPowerCoefficient2();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCompressorPowerCoefficient2,value);
  }

  {
    auto value = modelObject.coolingCompressorPowerCoefficient3();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCompressorPowerCoefficient3,value);
  }

  {
    auto value = modelObject.coolingCompressorPowerCoefficient4();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCompressorPowerCoefficient4,value);
  }

  {
    auto value = modelObject.coolingCompressorPowerCoefficient5();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCompressorPowerCoefficient5,value);
  }

  {
    auto value = modelObject.referenceCoefficientofPerformance();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_CoolingFields::ReferenceCoefficientofPerformance, value);
  }

  {
    auto value = modelObject.sizingFactor();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_CoolingFields::SizingFactor, value);
  }

  boost::optional<HeatPumpWaterToWaterEquationFitHeating> companion = modelObject.companionHeatingHeatPump();

  if (companion)
  {
    boost::optional<IdfObject> _companion = translateAndMapModelObject(companion.get());
    if (_companion)
    {
      idfObject.setString(HeatPump_WaterToWater_EquationFit_CoolingFields::CompanionHeatingHeatPumpName, _companion->name().get());
    }
  }

  return idfObject;
}

} // energyplus

} // openstudio

