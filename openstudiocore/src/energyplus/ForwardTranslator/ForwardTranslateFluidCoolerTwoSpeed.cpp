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
  mo = modelObject.waterInletNode();
  if (mo)
  {
    s = mo->name();
    if (s)
    {
      idfObject.setString(openstudio::FluidCooler_TwoSpeedFields::WaterInletNodeName, *s);
    }
  }
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  mo = modelObject.waterOutletNode();
  if (mo)
  {
    s = mo->name();
    if (s)
    {
      idfObject.setString(openstudio::FluidCooler_TwoSpeedFields::WaterOutletNodeName, *s);
    }
  }
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  d = modelObject.highFanSpeedUfactorTimesAreaValue();
  if (d)
  {
    idfObject.setString(openstudio::FluidCooler_TwoSpeedFields::HighFanSpeedUfactorTimesAreaValue, toString(*d));
  }
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  d = modelObject.lowFanSpeedUfactorTimesAreaValue();
  if (d)
  {
    idfObject.setString(openstudio::FluidCooler_TwoSpeedFields::LowFanSpeedUfactorTimesAreaValue, toString(*d));
  }
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  idfObject.setString(openstudio::FluidCooler_TwoSpeedFields::LowFanSpeedUFactorTimesAreaSizingFactor, toString(modelObject.lowFanSpeedUFactorTimesAreaSizingFactor()));
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  idfObject.setString(openstudio::FluidCooler_TwoSpeedFields::HighSpeedNominalCapacity, toString(modelObject.highSpeedNominalCapacity()));
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  d = modelObject.lowSpeedNominalCapacity();
  if (d)
  {
    idfObject.setString(openstudio::FluidCooler_TwoSpeedFields::LowSpeedNominalCapacity, toString(*d));
  }
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  idfObject.setString(openstudio::FluidCooler_TwoSpeedFields::LowSpeedNominalCapacitySizingFactor, toString(modelObject.lowSpeedNominalCapacitySizingFactor()));
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  idfObject.setString(openstudio::FluidCooler_TwoSpeedFields::DesignEnteringWaterTemperature, toString(modelObject.designEnteringWaterTemperature()));
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  idfObject.setString(openstudio::FluidCooler_TwoSpeedFields::DesignEnteringAirTemperature, toString(modelObject.designEnteringAirTemperature()));
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  idfObject.setString(openstudio::FluidCooler_TwoSpeedFields::DesignEnteringAirWetbulbTemperature, toString(modelObject.designEnteringAirWetbulbTemperature()));
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  d = modelObject.designWaterFlowRate();
  if (d)
  {
    idfObject.setString(openstudio::FluidCooler_TwoSpeedFields::DesignWaterFlowRate, toString(*d));
  }
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  d = modelObject.highFanSpeedAirFlowRate();
  if (d)
  {
    idfObject.setString(openstudio::FluidCooler_TwoSpeedFields::HighFanSpeedAirFlowRate, toString(*d));
  }
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  d = modelObject.highFanSpeedFanPower();
  if (d)
  {
    idfObject.setString(openstudio::FluidCooler_TwoSpeedFields::HighFanSpeedFanPower, toString(*d));
  }
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  d = modelObject.lowFanSpeedAirFlowRate();
  if (d)
  {
    idfObject.setString(openstudio::FluidCooler_TwoSpeedFields::LowFanSpeedAirFlowRate, toString(*d));
  }
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  idfObject.setString(openstudio::FluidCooler_TwoSpeedFields::LowFanSpeedAirFlowRateSizingFactor, toString(modelObject.lowFanSpeedAirFlowRateSizingFactor()));
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  d = modelObject.lowFanSpeedFanPower();
  if (d)
  {
    idfObject.setString(openstudio::FluidCooler_TwoSpeedFields::LowFanSpeedFanPower, toString(*d));
  }
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  idfObject.setString(openstudio::FluidCooler_TwoSpeedFields::LowFanSpeedFanPowerSizingFactor, toString(modelObject.lowFanSpeedFanPowerSizingFactor()));
  ///////////////////////////////////////////////////////////////////////////

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

