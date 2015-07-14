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
  idfObject.setString(openstudio::FluidCooler_SingleSpeedFields::PerformanceInputMethod, modelObject.performanceInputMethod());
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  if (modelObject.isDesignAirFlowRateUfactorTimesAreaValueAutosized())
  {
    idfObject.setString(openstudio::FluidCooler_SingleSpeedFields::DesignAirFlowRateUfactorTimesAreaValue, "Autosize");
  }
  else
  {
    d = modelObject.designAirFlowRateUfactorTimesAreaValue();
    if (d)
    {
      idfObject.setDouble(FluidCooler_SingleSpeedFields::DesignAirFlowRateUfactorTimesAreaValue, *d);
    }
  }
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  idfObject.setDouble(openstudio::FluidCooler_SingleSpeedFields::NominalCapacity, modelObject.nominalCapacity());
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  idfObject.setDouble(openstudio::FluidCooler_SingleSpeedFields::DesignEnteringWaterTemperature, modelObject.designEnteringWaterTemperature());
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  idfObject.setDouble(openstudio::FluidCooler_SingleSpeedFields::DesignEnteringAirTemperature, modelObject.designEnteringAirTemperature());
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  idfObject.setDouble(openstudio::FluidCooler_SingleSpeedFields::DesignEnteringAirWetbulbTemperature, modelObject.designEnteringAirWetbulbTemperature());
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  if (modelObject.isDesignWaterFlowRateAutosized())
  {
    idfObject.setString(openstudio::FluidCooler_SingleSpeedFields::DesignWaterFlowRate, "Autosize");
  }
  else
  {
    d = modelObject.designWaterFlowRate();
    if (d)
    {
      idfObject.setDouble(openstudio::FluidCooler_SingleSpeedFields::DesignWaterFlowRate, *d);
    }
  }
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  if (modelObject.isDesignAirFlowRateAutosized())
  {
    idfObject.setString(openstudio::FluidCooler_SingleSpeedFields::DesignAirFlowRate, "Autosize");
  }
  else
  {
    d = modelObject.designAirFlowRate();
    if (d)
    {
      idfObject.setDouble(openstudio::FluidCooler_SingleSpeedFields::DesignAirFlowRate, *d);
    }
  }
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  if (modelObject.isDesignAirFlowRateFanPowerAutosized())
  {
    idfObject.setString(openstudio::FluidCooler_SingleSpeedFields::DesignAirFlowRateFanPower, "Autosize");
  }
  else
  {
    d = modelObject.designAirFlowRateFanPower();
    if (d)
    {
      idfObject.setDouble(openstudio::FluidCooler_SingleSpeedFields::DesignAirFlowRateFanPower, *d);
    }
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

