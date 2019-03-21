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

#include "../../model/PumpVariableSpeed.hpp"
#include "../../model/PumpVariableSpeed_Impl.hpp"
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"

#include "../../utilities/core/Assert.hpp"

#include <utilities/idd/Pump_VariableSpeed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translatePumpVariableSpeed(
    PumpVariableSpeed& modelObject)
{
  boost::optional<std::string> s;
  boost::optional<double> value;
  OptionalSchedule schedule;

  IdfObject idfObject(IddObjectType::Pump_VariableSpeed);

  m_idfObjects.push_back(idfObject);

  // Name

  s = modelObject.name();
  if( s )
  {
    idfObject.setName(*s);
  }

  // InletNodeName

  if( boost::optional<ModelObject> mo = modelObject.inletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      idfObject.setString(Pump_VariableSpeedFields::InletNodeName,node->name().get());
    }
  }

  // OutletNodeName

  if( boost::optional<ModelObject> mo = modelObject.outletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      idfObject.setString(Pump_VariableSpeedFields::OutletNodeName,node->name().get());
    }
  }

  // RatedFlowRate

  if( modelObject.isRatedFlowRateAutosized() )
  {
    idfObject.setString(Pump_VariableSpeedFields::DesignMaximumFlowRate,"Autosize");
  }
  else if( (value = modelObject.ratedFlowRate()) )
  {
    idfObject.setDouble(Pump_VariableSpeedFields::DesignMaximumFlowRate,value.get());
  }

  // RatedPumpHead

  if( (value = modelObject.ratedPumpHead()) )
  {
    idfObject.setDouble(Pump_VariableSpeedFields::DesignPumpHead,value.get());
  }

  // RatedPowerConsumption

  if( modelObject.isRatedPowerConsumptionAutosized() )
  {
    idfObject.setString(Pump_VariableSpeedFields::DesignPowerConsumption,"Autosize");
  }
  else if( (value = modelObject.ratedPowerConsumption()) )
  {
    idfObject.setDouble(Pump_VariableSpeedFields::DesignPowerConsumption,value.get());
  }

  // MotorEfficiency

  if( (value = modelObject.motorEfficiency()) )
  {
    idfObject.setDouble(Pump_VariableSpeedFields::MotorEfficiency,value.get());
  }

  // FractionofMotorInefficienciestoFluidStream

  if( (value = modelObject.fractionofMotorInefficienciestoFluidStream()) )
  {
    idfObject.setDouble(Pump_VariableSpeedFields::FractionofMotorInefficienciestoFluidStream,value.get());
  }

  // Coefficient1ofthePartLoadPerformanceCurve

  if( (value = modelObject.coefficient1ofthePartLoadPerformanceCurve()) )
  {
    idfObject.setDouble(Pump_VariableSpeedFields::Coefficient1ofthePartLoadPerformanceCurve,value.get());
  }

  // Coefficient2ofthePartLoadPerformanceCurve

  if( (value = modelObject.coefficient2ofthePartLoadPerformanceCurve()) )
  {
    idfObject.setDouble(Pump_VariableSpeedFields::Coefficient2ofthePartLoadPerformanceCurve,value.get());
  }

  // Coefficient3ofthePartLoadPerformanceCurve

  if( (value = modelObject.coefficient3ofthePartLoadPerformanceCurve()) )
  {
    idfObject.setDouble(Pump_VariableSpeedFields::Coefficient3ofthePartLoadPerformanceCurve,value.get());
  }

  // Coefficient4ofthePartLoadPerformanceCurve

  if( (value = modelObject.coefficient4ofthePartLoadPerformanceCurve()) )
  {
    idfObject.setDouble(Pump_VariableSpeedFields::Coefficient4ofthePartLoadPerformanceCurve,value.get());
  }

  // MinimumFlowRate

  if( (value = modelObject.minimumFlowRate()) )
  {
    idfObject.setDouble(Pump_VariableSpeedFields::DesignMinimumFlowRate,value.get());
  }

  // PumpControlType

  if( (s = modelObject.pumpControlType()) )
  {
    idfObject.setString(Pump_VariableSpeedFields::PumpControlType,s.get());
  }

  // PumpFlowRateSchedule

  if ((schedule = modelObject.pumpFlowRateSchedule())) {
    idfObject.setString(Pump_VariableSpeedFields::PumpFlowRateScheduleName,schedule->name().get());
  }

  // PumpCurve

  if (OptionalCurve curve = modelObject.pumpCurve()) {
    idfObject.setString(Pump_VariableSpeedFields::PumpCurveName,curve->name().get());
  }

  // ImpellerDiameter

  if ((value = modelObject.impellerDiameter())) {
    idfObject.setDouble(Pump_VariableSpeedFields::ImpellerDiameter,value.get());
  }

  // VFDControlType

  if ((s = modelObject.vFDControlType())) {
    idfObject.setString(Pump_VariableSpeedFields::VFDControlType,s.get());
  }

  // PumpRPMSchedule

  if ((schedule = modelObject.pumpRPMSchedule())) {
    idfObject.setString(Pump_VariableSpeedFields::PumprpmScheduleName,schedule->name().get());
  }

  // MinimumPressureSchedule

  if ((schedule = modelObject.minimumPressureSchedule())) {
    idfObject.setString(Pump_VariableSpeedFields::MinimumPressureSchedule,schedule->name().get());
  }

  // MaximumPressureSchedule

  if ((schedule = modelObject.maximumPressureSchedule())) {
    idfObject.setString(Pump_VariableSpeedFields::MaximumPressureSchedule,schedule->name().get());
  }

  // MinimumRPMSchedule

  if ((schedule = modelObject.minimumRPMSchedule())) {
    idfObject.setString(Pump_VariableSpeedFields::MinimumRPMSchedule,schedule->name().get());
  }

  // MaximumRPMSchedule

  if ((schedule = modelObject.maximumRPMSchedule())) {
    idfObject.setString(Pump_VariableSpeedFields::MaximumRPMSchedule,schedule->name().get());
  }

  if ((value = modelObject.skinLossRadiativeFraction())) {
    idfObject.setDouble(Pump_VariableSpeedFields::SkinLossRadiativeFraction,value.get());
  }

  {
    s = modelObject.designPowerSizingMethod();
    idfObject.setString(Pump_VariableSpeedFields::DesignPowerSizingMethod,s.get());
  }


  {
    value = modelObject.designElectricPowerPerUnitFlowRate();
    idfObject.setDouble(Pump_VariableSpeedFields::DesignElectricPowerperUnitFlowRate,value.get());
  }


  {
    value = modelObject.designShaftPowerPerUnitFlowRatePerUnitHead();
    idfObject.setDouble(Pump_VariableSpeedFields::DesignShaftPowerperUnitFlowRateperUnitHead,value.get());
  }

  {
    value = modelObject.designMinimumFlowRateFraction();
    idfObject.setDouble(Pump_VariableSpeedFields::DesignMinimumFlowRateFraction,value.get());
  }

  // End Use Subcategory
  idfObject.setString(Pump_VariableSpeedFields::EndUseSubcategory, modelObject.endUseSubcategory());

  return idfObject;
}

} // energyplus
} // openstudio
