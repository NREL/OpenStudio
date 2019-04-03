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
#include "../../model/FluidCoolerTwoSpeed.hpp"
#include "../../model/FluidCoolerTwoSpeed_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/FluidCooler_TwoSpeed_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateFluidCoolerTwoSpeed(FluidCoolerTwoSpeed & modelObject)
{
  IdfObject idfObject(openstudio::IddObjectType::FluidCooler_TwoSpeed);

  m_idfObjects.push_back(idfObject);

  if( auto value = modelObject.name() ) {
    idfObject.setName(value.get());
  }

  if( auto node = modelObject.inletModelObject() ) {
    idfObject.setString(FluidCooler_TwoSpeedFields::WaterInletNodeName,node->name().get());
  }

  if( auto node = modelObject.outletModelObject() ) {
    idfObject.setString(FluidCooler_TwoSpeedFields::WaterOutletNodeName,node->name().get());
  }

  {
    auto value = modelObject.performanceInputMethod();
    idfObject.setString(FluidCooler_TwoSpeedFields::PerformanceInputMethod,value);
  }

  if( istringEqual("UFactorTimesAreaAndDesignWaterFlowRate",modelObject.performanceInputMethod()) ) {
    if( modelObject.isHighFanSpeedUfactorTimesAreaValueAutosized() ) {
      idfObject.setString(FluidCooler_TwoSpeedFields::HighFanSpeedUfactorTimesAreaValue,"Autosize");
    } else if( auto value = modelObject.highFanSpeedUfactorTimesAreaValue() ) {
      idfObject.setDouble(FluidCooler_TwoSpeedFields::HighFanSpeedUfactorTimesAreaValue,value.get());
    }
  }

  if( istringEqual("UFactorTimesAreaAndDesignWaterFlowRate",modelObject.performanceInputMethod()) ) {
    if( modelObject.isLowFanSpeedUfactorTimesAreaValueAutosized() ) {
      idfObject.setString(FluidCooler_TwoSpeedFields::LowFanSpeedUfactorTimesAreaValue,"Autosize");
    } else if( auto value = modelObject.lowFanSpeedUfactorTimesAreaValue() ) {
      idfObject.setDouble(FluidCooler_TwoSpeedFields::LowFanSpeedUfactorTimesAreaValue,value.get());
    }
  }

  if( istringEqual("UFactorTimesAreaAndDesignWaterFlowRate",modelObject.performanceInputMethod()) ) {
    auto value = modelObject.lowFanSpeedUFactorTimesAreaSizingFactor();
    idfObject.setDouble(FluidCooler_TwoSpeedFields::LowFanSpeedUFactorTimesAreaSizingFactor,value);
  }

  if( istringEqual("NominalCapacity",modelObject.performanceInputMethod()) ) {
    auto value = modelObject.highSpeedNominalCapacity();
    idfObject.setDouble(FluidCooler_TwoSpeedFields::HighSpeedNominalCapacity,value);
  }

  if( istringEqual("NominalCapacity",modelObject.performanceInputMethod()) ) {
    if( modelObject.isLowSpeedNominalCapacityAutosized() ) {
      idfObject.setString(FluidCooler_TwoSpeedFields::LowSpeedNominalCapacity,"Autosize");
    } else if(auto value = modelObject.lowSpeedNominalCapacity()) {
      idfObject.setDouble(FluidCooler_TwoSpeedFields::LowSpeedNominalCapacity,value.get());
    }
  }

  if( istringEqual("NominalCapacity",modelObject.performanceInputMethod()) ) {
    auto value = modelObject.lowSpeedNominalCapacitySizingFactor();
    idfObject.setDouble(FluidCooler_TwoSpeedFields::LowSpeedNominalCapacitySizingFactor,value);
  }

  {
    auto value = modelObject.designEnteringWaterTemperature();
    idfObject.setDouble(FluidCooler_TwoSpeedFields::DesignEnteringWaterTemperature,value);
  }

  {
    auto value = modelObject.designEnteringAirTemperature();
    idfObject.setDouble(FluidCooler_TwoSpeedFields::DesignEnteringAirTemperature,value);
  }

  {
    auto value = modelObject.designEnteringAirWetbulbTemperature();
    idfObject.setDouble(FluidCooler_TwoSpeedFields::DesignEnteringAirWetbulbTemperature,value);
  }

  if( modelObject.isDesignWaterFlowRateAutosized() ) {
    idfObject.setString(FluidCooler_TwoSpeedFields::DesignWaterFlowRate,"Autosize");
  } else if(auto value = modelObject.designWaterFlowRate()) {
    idfObject.setDouble(FluidCooler_TwoSpeedFields::DesignWaterFlowRate,value.get());
  }

  if( modelObject.isHighFanSpeedAirFlowRateAutosized() ) {
    idfObject.setString(FluidCooler_TwoSpeedFields::HighFanSpeedAirFlowRate,"Autosize");
  } else if(auto value = modelObject.highFanSpeedAirFlowRate()) {
    idfObject.setDouble(FluidCooler_TwoSpeedFields::HighFanSpeedAirFlowRate,value.get());
  }

  if( modelObject.isHighFanSpeedFanPowerAutosized() ) {
    idfObject.setString(FluidCooler_TwoSpeedFields::HighFanSpeedFanPower,"Autosize");
  } else if(auto value = modelObject.highFanSpeedFanPower()) {
    idfObject.setDouble(FluidCooler_TwoSpeedFields::HighFanSpeedFanPower,value.get());
  }

  if( modelObject.isLowFanSpeedAirFlowRateAutosized() ) {
    idfObject.setString(FluidCooler_TwoSpeedFields::LowFanSpeedAirFlowRate,"Autosize");
  } else if(auto value = modelObject.lowFanSpeedAirFlowRate()) {
    idfObject.setDouble(FluidCooler_TwoSpeedFields::LowFanSpeedAirFlowRate,value.get());
  }

  {
    auto value = modelObject.lowFanSpeedAirFlowRateSizingFactor();
    idfObject.setDouble(FluidCooler_TwoSpeedFields::LowFanSpeedAirFlowRateSizingFactor,value);
  }

  if( modelObject.isLowFanSpeedFanPowerAutosized() ) {
    idfObject.setString(FluidCooler_TwoSpeedFields::LowFanSpeedFanPower,"Autosize");
  } else if(auto value = modelObject.lowFanSpeedFanPower()) {
    idfObject.setDouble(FluidCooler_TwoSpeedFields::LowFanSpeedFanPower,value.get());
  }

  {
    auto value = modelObject.lowFanSpeedFanPowerSizingFactor();
    idfObject.setDouble(FluidCooler_TwoSpeedFields::LowFanSpeedFanPowerSizingFactor,value);
  }

  if( auto node = modelObject.outdoorAirInletNode() ) {
    idfObject.setString(FluidCooler_TwoSpeedFields::OutdoorAirInletNodeName,node->name().get());
  }

  return idfObject;
}

} // energyplus

} // openstudio

