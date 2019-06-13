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

  // End Use Subcategory
  idfObject.setString(HeaderedPumps_ConstantSpeedFields::EndUseSubcategory, modelObject.endUseSubcategory());

  return idfObject;
}

} // energyplus
} // openstudio
