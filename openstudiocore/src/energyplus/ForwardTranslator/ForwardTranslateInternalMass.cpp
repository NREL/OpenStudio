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
#include "../../model/InternalMass.hpp"
#include "../../model/InternalMass_Impl.hpp"
#include "../../model/InternalMassDefinition.hpp"
#include "../../model/InternalMassDefinition_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/ConstructionBase.hpp"
#include "../../model/ConstructionBase_Impl.hpp"

#include <utilities/idd/InternalMass_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateInternalMass( model::InternalMass & modelObject )
{
  // EnergyPlus does not support internal mass objects referencing zone lists

  InternalMassDefinition definition = modelObject.internalMassDefinition();

  boost::optional<ConstructionBase> construction = definition.construction();

  bool prefixSpaceName = false;
  std::vector<Space> spaces;
  boost::optional<Space> space = modelObject.space();
  boost::optional<SpaceType> spaceType = modelObject.spaceType();
  if (space){
    spaces.push_back(*space);
  }else if (spaceType){
    prefixSpaceName = true;
    spaces = spaceType->spaces();
  }

  boost::optional<IdfObject> result;
  if (spaces.empty()){

    // translate detached internal mass object
    IdfObject idfObject(openstudio::IddObjectType::InternalMass);

    m_idfObjects.push_back(idfObject);

    if (construction) {
      idfObject.setString(InternalMassFields::ConstructionName, construction->name().get());
    }

    double surfaceArea = 0;

    OptionalDouble d = definition.surfaceArea();
    if (d){
      surfaceArea += *d;
    }

    d = definition.surfaceAreaperSpaceFloorArea();
    if (d){
      LOG(Error, "Cannot compute space area because InternalMass object does not reference a Space or SpaceType");
    }

    d = definition.surfaceAreaperPerson();
    if (d){
      LOG(Error, "Cannot compute number of people because InternalMass object does not reference a Space or SpaceType");
    }

    double multiplier = modelObject.multiplier();

    idfObject.setDouble(InternalMassFields::SurfaceArea, multiplier*surfaceArea);

    result = idfObject;
  }else{

    // create InternalMass object for each zone
    for (Space space : spaces){

      IdfObject idfObject(openstudio::IddObjectType::InternalMass);

      m_idfObjects.push_back(idfObject);

      if (prefixSpaceName){
        idfObject.setString(InternalMassFields::Name, space.name().get() + " " + modelObject.name().get());
      }else{
        idfObject.setString(InternalMassFields::Name, modelObject.name().get());
      }

      boost::optional<ThermalZone> thermalZone = space.thermalZone();
      if (thermalZone){
        idfObject.setString(InternalMassFields::ZoneName, thermalZone->name().get());
      }

      if (construction) {
        idfObject.setString(InternalMassFields::ConstructionName, construction->name().get());
      }

      double surfaceArea = 0;

      OptionalDouble d = definition.surfaceArea();
      if (d){
        surfaceArea += *d;
      }

      d = definition.surfaceAreaperSpaceFloorArea();
      if (d){
        surfaceArea += (*d)*space.floorArea();
      }

      d = definition.surfaceAreaperPerson();
      if (d){
        surfaceArea += (*d)*space.numberOfPeople();
      }

      double multiplier = modelObject.multiplier();

      idfObject.setDouble(InternalMassFields::SurfaceArea, multiplier*surfaceArea);

      if (!result){
        result = idfObject;
      }
    }
  }

  return result;
}

} // energyplus

} // openstudio

