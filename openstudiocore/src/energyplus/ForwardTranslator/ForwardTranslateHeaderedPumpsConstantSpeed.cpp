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
#include "../../model/HeaderedPumpsConstantSpeed.hpp"
#include "../../model/HeaderedPumpsConstantSpeed_Impl.hpp"
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/HeaderedPumps_ConstantSpeed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateHeaderedPumpsConstantSpeed( 
    HeaderedPumpsConstantSpeed & modelObject)
{
  IdfObject idfObject(IddObjectType::HeaderedPumps_ConstantSpeed);
  m_idfObjects.push_back(idfObject);

  // Name
  if( auto s = modelObject.name() ) {
    idfObject.setName(*s);
  }

  // InletNodeName
  if( auto mo = modelObject.inletModelObject() ) {
    if( auto node = mo->optionalCast<Node>() ) {
      idfObject.setString(HeaderedPumps_ConstantSpeedFields::InletNodeName,node->name().get());
    }
  }

  // OutletNodeName
  if( auto mo = modelObject.outletModelObject() ) {
    if( auto node = mo->optionalCast<Node>() ) {
      idfObject.setString(HeaderedPumps_ConstantSpeedFields::OutletNodeName,node->name().get());
    }
  }

  // TotalRatedFlowRate
  if( modelObject.isTotalRatedFlowRateAutosized() ) {
    idfObject.setString(HeaderedPumps_ConstantSpeedFields::TotalDesignFlowRate,"Autosize");
  } else if ( auto value = modelObject.totalRatedFlowRate() ) {
    idfObject.setDouble(HeaderedPumps_ConstantSpeedFields::TotalDesignFlowRate,value.get());
  }

  // NumberofPumpsinBank
  {
    auto value = modelObject.numberofPumpsinBank();
    idfObject.setInt(HeaderedPumps_ConstantSpeedFields::NumberofPumpsinBank,value);
  }

  // FlowSequencingControlScheme
  {
    auto value = modelObject.flowSequencingControlScheme();
    idfObject.setString(HeaderedPumps_ConstantSpeedFields::FlowSequencingControlScheme,value);
  }

  // RatedPumpHead
  {
    auto value = modelObject.ratedPumpHead();
    idfObject.setDouble(HeaderedPumps_ConstantSpeedFields::DesignPumpHead,value);
  }

  // RatedPowerConsumption
  if( modelObject.isRatedPowerConsumptionAutosized() ) {
    idfObject.setString(HeaderedPumps_ConstantSpeedFields::DesignPowerConsumption,"Autosize");
  } else if( auto value = modelObject.ratedPowerConsumption() ) {
    idfObject.setDouble(HeaderedPumps_ConstantSpeedFields::DesignPowerConsumption,value.get());
  }

  // MotorEfficiency
  {
    auto value = modelObject.motorEfficiency();
    idfObject.setDouble(HeaderedPumps_ConstantSpeedFields::MotorEfficiency,value);
  }

  // FractionofMotorInefficienciestoFluidStream
  {
    auto value = modelObject.fractionofMotorInefficienciestoFluidStream();
    idfObject.setDouble(HeaderedPumps_ConstantSpeedFields::FractionofMotorInefficienciestoFluidStream,value);
  }

  // PumpControlType
  {
    auto value = modelObject.pumpControlType();
    idfObject.setString(HeaderedPumps_ConstantSpeedFields::PumpControlType,value);
  }

  // PumpFlowRateScheduleName
  if( auto s = modelObject.pumpFlowRateSchedule() ) {
    idfObject.setString(HeaderedPumps_ConstantSpeedFields::PumpFlowRateScheduleName,s->name().get());
  }

  // ZoneName
  if( auto z = modelObject.thermalZone() ) {
    idfObject.setString(HeaderedPumps_ConstantSpeedFields::ZoneName,z->name().get());
  }

  // SkinLossRadiativeFraction
  {
    auto value = modelObject.skinLossRadiativeFraction();
    idfObject.setDouble(HeaderedPumps_ConstantSpeedFields::SkinLossRadiativeFraction,value);
  }

  return idfObject;
}

} // energyplus
} // openstudio
