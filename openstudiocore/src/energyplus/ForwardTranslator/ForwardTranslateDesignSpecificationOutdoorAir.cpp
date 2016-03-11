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
#include "../../model/DesignSpecificationOutdoorAir.hpp"
#include "../../model/DesignSpecificationOutdoorAir_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/DesignSpecification_OutdoorAir_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateDesignSpecificationOutdoorAir( DesignSpecificationOutdoorAir & modelObject )
{
  boost::optional<std::string> s;
  boost::optional<double> value;

  IdfObject idfObject(IddObjectType::DesignSpecification_OutdoorAir);

  idfObject.setString(DesignSpecification_OutdoorAirFields::Name, modelObject.name().get());

  std::string outdoorAirMethod = modelObject.outdoorAirMethod();
  idfObject.setString(DesignSpecification_OutdoorAirFields::OutdoorAirMethod, outdoorAirMethod);

  double flowPerPerson = modelObject.outdoorAirFlowperPerson();
  double flowPerArea = modelObject.outdoorAirFlowperFloorArea();
  double flowPerZone = modelObject.outdoorAirFlowRate();
  double ach = modelObject.outdoorAirFlowAirChangesperHour();

  if (istringEqual(outdoorAirMethod, "Sum") || istringEqual(outdoorAirMethod, "Maximum")){

    idfObject.setDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowperPerson, flowPerPerson);
    idfObject.setDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowperZoneFloorArea, flowPerArea);
    idfObject.setDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowperZone, flowPerZone);
    idfObject.setDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowAirChangesperHour, ach);

  }else{
    LOG(Error, "Unknown OutdoorAirMethod '" << outdoorAirMethod << "' specified for OS:DesignSpecification:OutdoorAir named '" << modelObject.name().get() << "'");
    return boost::none;
  }

  boost::optional<Schedule> schedule = modelObject.outdoorAirFlowRateFractionSchedule();
  if (schedule){
    idfObject.setString(DesignSpecification_OutdoorAirFields::OutdoorAirFlowRateFractionScheduleName, schedule->name().get());
  }

  m_idfObjects.push_back(idfObject);
  
  return idfObject;
}

} // energyplus

} // openstudio

