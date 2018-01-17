/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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
