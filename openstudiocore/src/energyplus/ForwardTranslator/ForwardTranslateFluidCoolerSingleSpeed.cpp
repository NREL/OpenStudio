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
      idfObject.setString(openstudio::FluidCooler_SingleSpeedFields::WaterInletNodeName, *s);
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
      idfObject.setString(openstudio::FluidCooler_SingleSpeedFields::WaterOutletNodeName, *s);
    }
  }
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  idfObject.setString(openstudio::FluidCooler_SingleSpeedFields::PerformanceInputMethod, modelObject.performanceInputMethod());
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  d = modelObject.designAirFlowRateUfactorTimesAreaValue();
  if (d)
  {
    idfObject.setString(openstudio::FluidCooler_SingleSpeedFields::DesignAirFlowRateUfactorTimesAreaValue, toString(*d));
  }
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  idfObject.setString(openstudio::FluidCooler_SingleSpeedFields::NominalCapacity, toString(modelObject.nominalCapacity()));
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  idfObject.setString(openstudio::FluidCooler_SingleSpeedFields::DesignEnteringWaterTemperature, toString(modelObject.designEnteringWaterTemperature()));
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  idfObject.setString(openstudio::FluidCooler_SingleSpeedFields::DesignEnteringAirTemperature, toString(modelObject.designEnteringAirTemperature()));
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  idfObject.setString(openstudio::FluidCooler_SingleSpeedFields::DesignEnteringAirWetbulbTemperature, toString(modelObject.designEnteringAirWetbulbTemperature()));
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  d = modelObject.designWaterFlowRate();
  if (d)
  {
    idfObject.setString(openstudio::FluidCooler_SingleSpeedFields::DesignWaterFlowRate, toString(*d));
  }
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  d = modelObject.designAirFlowRate();
  if (d)
  {
    idfObject.setString(openstudio::FluidCooler_SingleSpeedFields::DesignAirFlowRate, toString(*d));
  }
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  d = modelObject.designAirFlowRateFanPower();
  if (d)
  {
    idfObject.setString(openstudio::FluidCooler_SingleSpeedFields::DesignAirFlowRateFanPower, toString(*d));
  }
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  mo = modelObject.outdoorAirInletNode();
  if (mo)
  {
    s = mo->name();
    if (s)
    {
      idfObject.setString(openstudio::FluidCooler_SingleSpeedFields::OutdoorAirInletNodeName, *s);
    }
  }
  ///////////////////////////////////////////////////////////////////////////

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

