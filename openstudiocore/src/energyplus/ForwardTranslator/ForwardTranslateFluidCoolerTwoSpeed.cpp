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

