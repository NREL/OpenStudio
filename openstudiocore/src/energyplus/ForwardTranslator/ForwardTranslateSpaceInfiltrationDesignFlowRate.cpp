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

#include "../../model/Model.hpp"
#include "../../model/SpaceInfiltrationDesignFlowRate.hpp"
#include "../../model/SpaceInfiltrationDesignFlowRate_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/ZoneInfiltration_DesignFlowRate_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateSpaceInfiltrationDesignFlowRate( SpaceInfiltrationDesignFlowRate& modelObject )
{
  IdfObject idfObject(openstudio::IddObjectType::ZoneInfiltration_DesignFlowRate );
  m_idfObjects.push_back(idfObject);

  idfObject.setString(ZoneInfiltration_DesignFlowRateFields::Name, modelObject.name().get());

  boost::optional<Space> space = modelObject.space();
  boost::optional<SpaceType> spaceType = modelObject.spaceType();
  if (space){
    boost::optional<ThermalZone> thermalZone = space->thermalZone();
    if (thermalZone){
      idfObject.setString(ZoneInfiltration_DesignFlowRateFields::ZoneorZoneListName, thermalZone->name().get());
    }
  }else if (spaceType){
    idfObject.setString(ZoneInfiltration_DesignFlowRateFields::ZoneorZoneListName, spaceType->name().get());
  }

  boost::optional<Schedule> schedule = modelObject.schedule();
  if (schedule){
    idfObject.setString(ZoneInfiltration_DesignFlowRateFields::ScheduleName, schedule->name().get());
  }

  std::string designFlowRateCalculationMethod = modelObject.designFlowRateCalculationMethod();
  if (istringEqual("Flow/Space", designFlowRateCalculationMethod)){
    designFlowRateCalculationMethod = "Flow/Zone";
  }
  idfObject.setString(ZoneInfiltration_DesignFlowRateFields::DesignFlowRateCalculationMethod, designFlowRateCalculationMethod);

  OptionalDouble d = modelObject.designFlowRate();
  if (d){
    idfObject.setDouble(ZoneInfiltration_DesignFlowRateFields::DesignFlowRate, *d);
  }

  d = modelObject.flowperSpaceFloorArea();
  if (d){
    idfObject.setDouble(ZoneInfiltration_DesignFlowRateFields::FlowperZoneFloorArea, *d);
  }

  d = modelObject.flowperExteriorSurfaceArea();
  if (d){
    idfObject.setDouble(ZoneInfiltration_DesignFlowRateFields::FlowperExteriorSurfaceArea, *d);
  }

  d = modelObject.flowperExteriorWallArea();
  if (d){
    idfObject.setDouble(ZoneInfiltration_DesignFlowRateFields::FlowperExteriorSurfaceArea, *d);
  }

  d = modelObject.airChangesperHour();
  if (d){
    idfObject.setDouble(ZoneInfiltration_DesignFlowRateFields::AirChangesperHour, *d);
  }

  if (!modelObject.isConstantTermCoefficientDefaulted()){
    idfObject.setDouble(ZoneInfiltration_DesignFlowRateFields::ConstantTermCoefficient, modelObject.constantTermCoefficient());
  }

  if (!modelObject.isTemperatureTermCoefficientDefaulted()){
    idfObject.setDouble(ZoneInfiltration_DesignFlowRateFields::TemperatureTermCoefficient, modelObject.temperatureTermCoefficient());
  }

  if (!modelObject.isVelocityTermCoefficientDefaulted()){
    idfObject.setDouble(ZoneInfiltration_DesignFlowRateFields::VelocityTermCoefficient, modelObject.velocityTermCoefficient());
  }

  if (!modelObject.isVelocitySquaredTermCoefficientDefaulted()){
    idfObject.setDouble(ZoneInfiltration_DesignFlowRateFields::VelocitySquaredTermCoefficient, modelObject.velocitySquaredTermCoefficient());
  }

  return idfObject;
}

} // energyplus

} // openstudio

