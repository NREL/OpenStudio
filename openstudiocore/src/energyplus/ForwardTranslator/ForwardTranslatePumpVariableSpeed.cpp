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

#include <model/PumpVariableSpeed.hpp>
#include <model/PumpVariableSpeed_Impl.hpp>
#include <model/Model.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/Curve.hpp>
#include <model/Curve_Impl.hpp>

#include <utilities/core/Assert.hpp>

#include <utilities/idd/Pump_VariableSpeed_FieldEnums.hxx>

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
    idfObject.setString(Pump_VariableSpeedFields::RatedFlowRate,"Autosize");
  }
  else if( (value = modelObject.ratedFlowRate()) )
  {
    idfObject.setDouble(Pump_VariableSpeedFields::RatedFlowRate,value.get());
  }

  // RatedPumpHead

  if( (value = modelObject.ratedPumpHead()) )
  {
    idfObject.setDouble(Pump_VariableSpeedFields::RatedPumpHead,value.get());
  }

  // RatedPowerConsumption

  if( modelObject.isRatedPowerConsumptionAutosized() )
  {
    idfObject.setString(Pump_VariableSpeedFields::RatedPowerConsumption,"Autosize");
  }
  else if( (value = modelObject.ratedPowerConsumption()) )
  {
    idfObject.setDouble(Pump_VariableSpeedFields::RatedPowerConsumption,value.get());
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
    idfObject.setDouble(Pump_VariableSpeedFields::MinimumFlowRate,value.get());
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

  return idfObject;
}

} // energyplus
} // openstudio
