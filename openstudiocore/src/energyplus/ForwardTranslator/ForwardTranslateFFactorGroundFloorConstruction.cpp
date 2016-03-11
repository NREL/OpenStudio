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

#include "../../model/FFactorGroundFloorConstruction.hpp"
#include "../../model/LifeCycleCost.hpp"

#include <utilities/idd/OS_Construction_FfactorGroundFloor_FieldEnums.hxx>

#include <utilities/idd/Construction_FfactorGroundFloor_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateFFactorGroundFloorConstruction( FFactorGroundFloorConstruction & modelObject )
{
  IdfObject idfObject( openstudio::IddObjectType::Construction_FfactorGroundFloor);
  m_idfObjects.push_back(idfObject);

  for (LifeCycleCost lifeCycleCost : modelObject.lifeCycleCosts()){
    translateAndMapModelObject(lifeCycleCost);
  }

  idfObject.setString(Construction_FfactorGroundFloorFields::Name, modelObject.name().get());

  OptionalDouble d = modelObject.getDouble(OS_Construction_FfactorGroundFloorFields::FFactor, false);
  if (d){
    idfObject.setDouble(Construction_FfactorGroundFloorFields::FFactor, *d);
  }else{
    LOG(Error, "Missing required input 'F-Factor' for Construction:FfactorGroundFloor named '" << modelObject.name().get() << "'");
  }

  d = modelObject.getDouble(OS_Construction_FfactorGroundFloorFields::Area, false);
  if (d){
    idfObject.setDouble(Construction_FfactorGroundFloorFields::Area, *d);
  }else{
    LOG(Error, "Missing required input 'Area' for Construction:FfactorGroundFloor named '" << modelObject.name().get() << "'");
  }

  d = modelObject.getDouble(OS_Construction_FfactorGroundFloorFields::PerimeterExposed, false);
  if (d){
    idfObject.setDouble(Construction_FfactorGroundFloorFields::PerimeterExposed, *d);
  }else{
    LOG(Error, "Missing required input 'PerimeterExposed' for Construction:FfactorGroundFloor named '" << modelObject.name().get() << "'");
  }

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

