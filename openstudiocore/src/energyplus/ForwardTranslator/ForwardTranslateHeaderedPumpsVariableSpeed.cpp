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

#include "../ForwardTranslator.hpp"
#include "../../model/HeaderedPumpsVariableSpeed.hpp"
#include "../../model/HeaderedPumpsVariableSpeed_Impl.hpp"
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/HeaderedPumps_VariableSpeed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateHeaderedPumpsVariableSpeed( 
    HeaderedPumpsVariableSpeed & modelObject)
{
  IdfObject idfObject(IddObjectType::HeaderedPumps_VariableSpeed);
  m_idfObjects.push_back(idfObject);

  // Name
  if( auto s = modelObject.name() ) {
    idfObject.setName(*s);
  }

  // InletNodeName
  if( auto mo = modelObject.inletModelObject() ) {
    if( auto node = mo->optionalCast<Node>() ) {
      idfObject.setString(HeaderedPumps_VariableSpeedFields::InletNodeName,node->name().get());
    }
  }

  // OutletNodeName
  if( auto mo = modelObject.outletModelObject() ) {
    if( auto node = mo->optionalCast<Node>() ) {
      idfObject.setString(HeaderedPumps_VariableSpeedFields::OutletNodeName,node->name().get());
    }
  }

  // TotalRatedFlowRate
  if( modelObject.isTotalRatedFlowRateAutosized() ) {
    idfObject.setString(HeaderedPumps_VariableSpeedFields::TotalDesignFlowRate,"Autosize");
  } else if ( auto value = modelObject.totalRatedFlowRate() ) {
    idfObject.setDouble(HeaderedPumps_VariableSpeedFields::TotalDesignFlowRate,value.get());
  }

  // NumberofPumpsinBank
  {
    auto value = modelObject.numberofPumpsinBank();
    idfObject.setInt(HeaderedPumps_VariableSpeedFields::NumberofPumpsinBank,value);
  }

  // FlowSequencingControlScheme
  {
    auto value = modelObject.flowSequencingControlScheme();
    idfObject.setString(HeaderedPumps_VariableSpeedFields::FlowSequencingControlScheme,value);
  }

  // RatedPumpHead
  {
    auto value = modelObject.ratedPumpHead();
    idfObject.setDouble(HeaderedPumps_VariableSpeedFields::DesignPumpHead,value);
  }

  // RatedPowerConsumption
  if( modelObject.isRatedPowerConsumptionAutosized() ) {
    idfObject.setString(HeaderedPumps_VariableSpeedFields::DesignPowerConsumption,"Autosize");
  } else if( auto value = modelObject.ratedPowerConsumption() ) {
    idfObject.setDouble(HeaderedPumps_VariableSpeedFields::DesignPowerConsumption,value.get());
  }

  // MotorEfficiency
  {
    auto value = modelObject.motorEfficiency();
    idfObject.setDouble(HeaderedPumps_VariableSpeedFields::MotorEfficiency,value);
  }

  // FractionofMotorInefficienciestoFluidStream
  {
    auto value = modelObject.fractionofMotorInefficienciestoFluidStream();
    idfObject.setDouble(HeaderedPumps_VariableSpeedFields::FractionofMotorInefficienciestoFluidStream,value);
  }

  // Coefficient1ofthePartLoadPerformanceCurve
  {
    auto value = modelObject.coefficient1ofthePartLoadPerformanceCurve();
    idfObject.setDouble(HeaderedPumps_VariableSpeedFields::Coefficient1ofthePartLoadPerformanceCurve,value);
  }

  // Coefficient2ofthePartLoadPerformanceCurve
  {
    auto value = modelObject.coefficient2ofthePartLoadPerformanceCurve();
    idfObject.setDouble(HeaderedPumps_VariableSpeedFields::Coefficient2ofthePartLoadPerformanceCurve,value);
  }

  // Coefficient3ofthePartLoadPerformanceCurve
  {
    auto value = modelObject.coefficient3ofthePartLoadPerformanceCurve();
    idfObject.setDouble(HeaderedPumps_VariableSpeedFields::Coefficient3ofthePartLoadPerformanceCurve,value);
  }

  // Coefficient4ofthePartLoadPerformanceCurve
  {
    auto value = modelObject.coefficient4ofthePartLoadPerformanceCurve();
    idfObject.setDouble(HeaderedPumps_VariableSpeedFields::Coefficient4ofthePartLoadPerformanceCurve,value);
  }

  // MinimumFlowRateFraction
  {
    auto value = modelObject.minimumFlowRateFraction();
    idfObject.setDouble(HeaderedPumps_VariableSpeedFields::MinimumFlowRateFraction,value);
  }

  // PumpControlType
  {
    auto value = modelObject.pumpControlType();
    idfObject.setString(HeaderedPumps_VariableSpeedFields::PumpControlType,value);
  }

  // PumpFlowRateScheduleName
  if( auto s = modelObject.pumpFlowRateSchedule() ) {
    idfObject.setString(HeaderedPumps_VariableSpeedFields::PumpFlowRateScheduleName,s->name().get());
  }

  // ZoneName
  if( auto z = modelObject.thermalZone() ) {
    idfObject.setString(HeaderedPumps_VariableSpeedFields::ZoneName,z->name().get());
  }

  // SkinLossRadiativeFraction
  {
    auto value = modelObject.skinLossRadiativeFraction();
    idfObject.setDouble(HeaderedPumps_VariableSpeedFields::SkinLossRadiativeFraction,value);
  }

  return idfObject;
}

} // energyplus
} // openstudio
