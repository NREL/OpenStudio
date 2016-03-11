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

#include "../../model/SolarCollectorPerformancePhotovoltaicThermalSimple.hpp"
#include "../../model/Schedule.hpp"

#include <utilities/idd/SolarCollectorPerformance_PhotovoltaicThermal_Simple_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateSolarCollectorPerformancePhotovoltaicThermalSimple(model::SolarCollectorPerformancePhotovoltaicThermalSimple & modelObject)
{
  IdfObject idfObject(openstudio::IddObjectType::SolarCollectorPerformance_PhotovoltaicThermal_Simple);

  m_idfObjects.push_back(idfObject);

  boost::optional<double> d;

  idfObject.setString(SolarCollectorPerformance_PhotovoltaicThermal_SimpleFields::Name, modelObject.name().get());

  idfObject.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_SimpleFields::FractionofSurfaceAreawithActiveThermalCollector, modelObject.fractionOfSurfaceAreaWithActiveThermalCollector());

  std::string thermalConversionEfficiencyInputModeType = modelObject.thermalConversionEfficiencyInputModeType();
  idfObject.setString(SolarCollectorPerformance_PhotovoltaicThermal_SimpleFields::ThermalConversionEfficiencyInputModeType, thermalConversionEfficiencyInputModeType);

  if (istringEqual("Scheduled", thermalConversionEfficiencyInputModeType)){
    boost::optional<Schedule> schedule = modelObject.thermalConversionEfficiencySchedule();
    if (schedule){
      boost::optional<IdfObject> relatedIdfObject = translateAndMapModelObject(*schedule);
      OS_ASSERT(relatedIdfObject);
      idfObject.setString(SolarCollectorPerformance_PhotovoltaicThermal_SimpleFields::ThermalConversionEfficiencyScheduleName, relatedIdfObject->name().get());
    } else{
      LOG(Error, "SolarCollectorPerformance:PhotovoltaicThermal:Simple object named '" << modelObject.name().get() << "' has ThermalConversionEfficiencyInputModeType of Scheduled but has no schedule attached.");
    }
  } else {
    d = modelObject.thermalConversionEfficiency();
    if (d){
      idfObject.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_SimpleFields::ValueforThermalConversionEfficiencyifFixed, *d);
    }else{
      LOG(Error, "SolarCollectorPerformance:PhotovoltaicThermal:Simple object named '" << modelObject.name().get() << "' has ThermalConversionEfficiencyInputModeType of Fixed but has no value set.");
    }
  }

  idfObject.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_SimpleFields::FrontSurfaceEmittance, modelObject.frontSurfaceEmittance());

  return idfObject;
}

} // energyplus

} // openstudio

