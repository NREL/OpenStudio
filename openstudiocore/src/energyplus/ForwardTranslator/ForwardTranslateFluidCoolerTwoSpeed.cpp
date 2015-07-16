/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

  OptionalString s;
  OptionalDouble d;
  OptionalModelObject mo;

  ///////////////////////////////////////////////////////////////////////////
  s = modelObject.name();
  if (s)
  {
    idfObject.setName(*s);
  }
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  idfObject.setString(openstudio::FluidCooler_TwoSpeedFields::PerformanceInputMethod, modelObject.performanceInputMethod());
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  d = modelObject.highFanSpeedUfactorTimesAreaValue();
  if (d)
  {
    idfObject.setDouble(openstudio::FluidCooler_TwoSpeedFields::HighFanSpeedUfactorTimesAreaValue, *d);
  }
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  if (modelObject.isLowFanSpeedUfactorTimesAreaValueAutosized())
  {
    idfObject.setString(openstudio::FluidCooler_TwoSpeedFields::LowFanSpeedUfactorTimesAreaValue, "Autosize");
  }
  else
  {
    d = modelObject.lowFanSpeedUfactorTimesAreaValue();
    if (d)
    {
      idfObject.setDouble(openstudio::FluidCooler_TwoSpeedFields::LowFanSpeedUfactorTimesAreaValue, *d);
    }
  }
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  idfObject.setDouble(openstudio::FluidCooler_TwoSpeedFields::LowFanSpeedUFactorTimesAreaSizingFactor, modelObject.lowFanSpeedUFactorTimesAreaSizingFactor());
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  idfObject.setDouble(openstudio::FluidCooler_TwoSpeedFields::HighSpeedNominalCapacity, modelObject.highSpeedNominalCapacity());
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  if (modelObject.isLowSpeedNominalCapacityAutosized())
  {
    idfObject.setString(openstudio::FluidCooler_TwoSpeedFields::LowSpeedNominalCapacity, "Autosize");
  }
  else
  {
    d = modelObject.lowSpeedNominalCapacity();
    if (d)
    {
      idfObject.setDouble(openstudio::FluidCooler_TwoSpeedFields::LowSpeedNominalCapacity, *d);
    }
  }
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  idfObject.setDouble(openstudio::FluidCooler_TwoSpeedFields::LowSpeedNominalCapacitySizingFactor, modelObject.lowSpeedNominalCapacitySizingFactor());
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  idfObject.setDouble(openstudio::FluidCooler_TwoSpeedFields::DesignEnteringWaterTemperature, modelObject.designEnteringWaterTemperature());
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  idfObject.setDouble(openstudio::FluidCooler_TwoSpeedFields::DesignEnteringAirTemperature, modelObject.designEnteringAirTemperature());
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  idfObject.setDouble(openstudio::FluidCooler_TwoSpeedFields::DesignEnteringAirWetbulbTemperature, modelObject.designEnteringAirWetbulbTemperature());
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  if (modelObject.isDesignWaterFlowRateAutosized())
  {
    idfObject.setString(openstudio::FluidCooler_TwoSpeedFields::DesignWaterFlowRate, "Autosize");
  }
  else
  {
    d = modelObject.designWaterFlowRate();
    if (d)
    {
      idfObject.setDouble(openstudio::FluidCooler_TwoSpeedFields::DesignWaterFlowRate, *d);
    }
  }
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  if (modelObject.isHighFanSpeedAirFlowRateAutosized())
  {
    idfObject.setString(openstudio::FluidCooler_TwoSpeedFields::HighFanSpeedAirFlowRate, "Autosize");
  }
  else
  {
    d = modelObject.highFanSpeedAirFlowRate();
    if (d)
    {
      idfObject.setDouble(openstudio::FluidCooler_TwoSpeedFields::HighFanSpeedAirFlowRate, *d);
    }
  }
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  if (modelObject.isHighFanSpeedFanPowerAutosized())
  {
    idfObject.setString(openstudio::FluidCooler_TwoSpeedFields::HighFanSpeedFanPower, "Autosize");
  }
  else
  {
    d = modelObject.highFanSpeedFanPower();
    if (d)
    {
      idfObject.setDouble(openstudio::FluidCooler_TwoSpeedFields::HighFanSpeedFanPower, *d);
    }
  }
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  if (modelObject.isLowFanSpeedAirFlowRateAutosized())
  {
    idfObject.setString(openstudio::FluidCooler_TwoSpeedFields::LowFanSpeedAirFlowRate, "Autosize");
  }
  else
  {
    d = modelObject.lowFanSpeedAirFlowRate();
    if (d)
    {
      idfObject.setDouble(openstudio::FluidCooler_TwoSpeedFields::LowFanSpeedAirFlowRate, *d);
    }
  }
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  idfObject.setDouble(openstudio::FluidCooler_TwoSpeedFields::LowFanSpeedAirFlowRateSizingFactor, modelObject.lowFanSpeedAirFlowRateSizingFactor());
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  if (modelObject.isLowFanSpeedFanPowerAutosized())
  {
    idfObject.setString(openstudio::FluidCooler_TwoSpeedFields::LowFanSpeedFanPower, "Autosize");
  }
  else
  {
    d = modelObject.lowFanSpeedFanPower();
    if (d)
    {
      idfObject.setDouble(openstudio::FluidCooler_TwoSpeedFields::LowFanSpeedFanPower, *d);
    }
  }
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  idfObject.setDouble(openstudio::FluidCooler_TwoSpeedFields::LowFanSpeedFanPowerSizingFactor, modelObject.lowFanSpeedFanPowerSizingFactor());
  ///////////////////////////////////////////////////////////////////////////

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

