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
#include "../../model/FluidCoolerSingleSpeed.hpp"
#include "../../model/FluidCoolerSingleSpeed_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/FluidCooler_SingleSpeed_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateFluidCoolerSingleSpeed(FluidCoolerSingleSpeed & modelObject)
{
  IdfObject idfObject(openstudio::IddObjectType::FluidCooler_SingleSpeed);

  m_idfObjects.push_back(idfObject);

  if( auto name = modelObject.name() ) {
    idfObject.setName(name.get());
  }

  if( auto node = modelObject.inletModelObject() ) {
    idfObject.setString(FluidCooler_SingleSpeedFields::WaterInletNodeName,node->name().get());
  }

  if( auto node = modelObject.outletModelObject() ) {
    idfObject.setString(FluidCooler_SingleSpeedFields::WaterOutletNodeName,node->name().get());
  }

  {
    auto value = modelObject.performanceInputMethod();
    idfObject.setString(FluidCooler_SingleSpeedFields::PerformanceInputMethod,value);
  }

  if( istringEqual("UFactorTimesAreaAndDesignWaterFlowRate",modelObject.performanceInputMethod()) ) {
    if( modelObject.isDesignAirFlowRateUfactorTimesAreaValueAutosized() ) {
      idfObject.setString(FluidCooler_SingleSpeedFields::DesignAirFlowRateUfactorTimesAreaValue,"Autosize");
    } else if( auto value = modelObject.designAirFlowRateUfactorTimesAreaValue() ) {
      idfObject.setDouble(FluidCooler_SingleSpeedFields::DesignAirFlowRateUfactorTimesAreaValue,value.get());
    }
  }
  
  if( istringEqual("NominalCapacity",modelObject.performanceInputMethod()) ) {
    auto value = modelObject.nominalCapacity();
    idfObject.setDouble(FluidCooler_SingleSpeedFields::NominalCapacity,value);
  }
  
  {
    auto value = modelObject.designEnteringWaterTemperature();
    idfObject.setDouble(FluidCooler_SingleSpeedFields::DesignEnteringWaterTemperature,value);
  }

  {
    auto value = modelObject.designEnteringAirTemperature();
    idfObject.setDouble(FluidCooler_SingleSpeedFields::DesignEnteringAirTemperature,value);
  }

  {
    auto value = modelObject.designEnteringAirWetbulbTemperature();
    idfObject.setDouble(FluidCooler_SingleSpeedFields::DesignEnteringAirWetbulbTemperature,value);
  }

  if( modelObject.isDesignWaterFlowRateAutosized() ) {
    idfObject.setString(FluidCooler_SingleSpeedFields::DesignWaterFlowRate,"Autosize");
  } else if( auto value = modelObject.designWaterFlowRate() ) {
    idfObject.setDouble(FluidCooler_SingleSpeedFields::DesignWaterFlowRate,value.get());
  }

  if( modelObject.isDesignAirFlowRateAutosized() ) {
    idfObject.setString(FluidCooler_SingleSpeedFields::DesignAirFlowRate,"Autosize");
  } else if( auto value = modelObject.designAirFlowRate() ) {
    idfObject.setDouble(FluidCooler_SingleSpeedFields::DesignAirFlowRate,value.get());
  }
  
  if( modelObject.isDesignAirFlowRateFanPowerAutosized() ) {
    idfObject.setString(FluidCooler_SingleSpeedFields::DesignAirFlowRateFanPower,"Autosize");
  } else if( auto value = modelObject.designAirFlowRateFanPower() ) {
    idfObject.setDouble(FluidCooler_SingleSpeedFields::DesignAirFlowRateFanPower,value.get());
  }

  return idfObject;
}

} // energyplus

} // openstudio

