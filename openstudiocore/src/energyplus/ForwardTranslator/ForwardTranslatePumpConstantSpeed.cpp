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
#include <model/PumpConstantSpeed.hpp>
#include <model/PumpConstantSpeed_Impl.hpp>
#include <model/Model.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/Curve.hpp>
#include <model/Curve_Impl.hpp>
#include <utilities/core/Assert.hpp>
#include <utilities/idd/Pump_ConstantSpeed_FieldEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translatePumpConstantSpeed( 
    PumpConstantSpeed& modelObject)
{
  boost::optional<std::string> s;
  boost::optional<double> value;
  OptionalSchedule schedule;

  IdfObject idfObject(IddObjectType::Pump_ConstantSpeed);

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
      idfObject.setString(Pump_ConstantSpeedFields::InletNodeName,node->name().get());
    }
  }

  // OutletNodeName

  if( boost::optional<ModelObject> mo = modelObject.outletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      idfObject.setString(Pump_ConstantSpeedFields::OutletNodeName,node->name().get());
    }
  }

  // RatedFlowRate

  if( modelObject.isRatedFlowRateAutosized() )
  {
    idfObject.setString(Pump_ConstantSpeedFields::RatedFlowRate,"Autosize");
  }
  else if( (value = modelObject.ratedFlowRate()) )
  {
    idfObject.setDouble(Pump_ConstantSpeedFields::RatedFlowRate,value.get());
  }

  // RatedPumpHead

  if( (value = modelObject.ratedPumpHead()) )
  {
    idfObject.setDouble(Pump_ConstantSpeedFields::RatedPumpHead,value.get());
  }

  // RatedPowerConsumption

  if( modelObject.isRatedPowerConsumptionAutosized() )
  {
    idfObject.setString(Pump_ConstantSpeedFields::RatedPowerConsumption,"Autosize");
  }
  else if( (value = modelObject.ratedPowerConsumption()) )
  {
    idfObject.setDouble(Pump_ConstantSpeedFields::RatedPowerConsumption,value.get());
  }

  // MotorEfficiency

  if( (value = modelObject.motorEfficiency()) )
  {
    idfObject.setDouble(Pump_ConstantSpeedFields::MotorEfficiency,value.get());
  }

  // FractionofMotorInefficienciestoFluidStream  

  if( (value = modelObject.fractionofMotorInefficienciestoFluidStream()) )
  {
    idfObject.setDouble(Pump_ConstantSpeedFields::FractionofMotorInefficienciestoFluidStream,value.get());
  } 

  // PumpControlType

  if( (s = modelObject.pumpControlType()) )
  {
    idfObject.setString(Pump_ConstantSpeedFields::PumpControlType,s.get());
  }

  // PumpFlowRateSchedule

  if ((schedule = modelObject.pumpFlowRateSchedule())) {
    idfObject.setString(Pump_ConstantSpeedFields::PumpFlowRateScheduleName,schedule->name().get());
  }

  // PumpCurve

  if (OptionalCurve curve = modelObject.pumpCurve()) {
    idfObject.setString(Pump_ConstantSpeedFields::PumpCurveName,curve->name().get());
  }

  // ImpellerDiameter

  if ((value = modelObject.impellerDiameter())) {
    idfObject.setDouble(Pump_ConstantSpeedFields::ImpellerDiameter,value.get());
  }

  // SkinLossRadiativeFraction

  if ((value = modelObject.skinLossRadiativeFraction())) {
    idfObject.setDouble(Pump_ConstantSpeedFields::SkinLossRadiativeFraction,value.get());
  }

  return idfObject;
}

} // energyplus
} // openstudio
