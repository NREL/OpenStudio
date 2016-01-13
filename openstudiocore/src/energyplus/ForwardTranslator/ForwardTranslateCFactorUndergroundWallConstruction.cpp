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

#include "../../model/CFactorUndergroundWallConstruction.hpp"
#include "../../model/LifeCycleCost.hpp"

#include <utilities/idd/OS_Construction_CfactorUndergroundWall_FieldEnums.hxx>

#include <utilities/idd/Construction_CfactorUndergroundWall_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateCFactorUndergroundWallConstruction( model::CFactorUndergroundWallConstruction & modelObject )
{
  IdfObject idfObject( openstudio::IddObjectType::Construction_CfactorUndergroundWall);
  m_idfObjects.push_back(idfObject);

  for (LifeCycleCost lifeCycleCost : modelObject.lifeCycleCosts()){
    translateAndMapModelObject(lifeCycleCost);
  }

  idfObject.setString(Construction_CfactorUndergroundWallFields::Name, modelObject.name().get());

  OptionalDouble d = modelObject.getDouble(OS_Construction_CfactorUndergroundWallFields::CFactor, false);
  if (d){
    idfObject.setDouble(Construction_CfactorUndergroundWallFields::CFactor, *d);
  }else{
    LOG(Error, "Missing required input 'C-Factor' for Construction:CfactorUndergroundWall named '" << modelObject.name().get() << "'");
  }

  d = modelObject.getDouble(OS_Construction_CfactorUndergroundWallFields::Height, false);
  if (d){
    idfObject.setDouble(Construction_CfactorUndergroundWallFields::Height, *d);
  }else{
    LOG(Error, "Missing required input 'Height' for Construction:CfactorUndergroundWall named '" << modelObject.name().get() << "'");
  }

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

