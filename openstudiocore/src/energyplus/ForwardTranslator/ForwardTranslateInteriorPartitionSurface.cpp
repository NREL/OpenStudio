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
#include "../../model/InteriorPartitionSurface.hpp"
#include "../../model/InteriorPartitionSurface_Impl.hpp"
#include "../../model/InteriorPartitionSurfaceGroup.hpp"
#include "../../model/Space.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ConstructionBase.hpp"
#include "../../model/ConstructionBase_Impl.hpp"
#include "../../model/DaylightingDeviceShelf.hpp"

#include <utilities/idd/InternalMass_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateInteriorPartitionSurface( model::InteriorPartitionSurface & modelObject )
{
  if (!modelObject.converttoInternalMass()){
    return boost::none;
  }

  // will get translated with light shelf
  if (modelObject.daylightingDeviceShelf()){
    return boost::none;
  }

  if (modelObject.isAirWall()){
    return boost::none;
  }

  boost::optional<ConstructionBase> construction = modelObject.construction();
  if (!construction){
    return boost::none;
  }

  IdfObject idfObject(openstudio::IddObjectType::InternalMass);

  m_idfObjects.push_back(idfObject);

  idfObject.setString(InternalMassFields::Name, modelObject.name().get());

  idfObject.setString(InternalMassFields::ConstructionName, construction->name().get());

  double multiplier = 1.0;
  boost::optional<InteriorPartitionSurfaceGroup> group = modelObject.interiorPartitionSurfaceGroup();
  if (group){
    multiplier = group->multiplier();
    boost::optional<Space> space = group->space();
    if (space){
      boost::optional<ThermalZone> thermalZone = space->thermalZone();
      if (thermalZone){
        idfObject.setString(InternalMassFields::ZoneName, thermalZone->name().get());
      }
    }
  }

  boost::optional<double> surfaceArea = modelObject.surfaceArea();
  if (!surfaceArea){
    surfaceArea = modelObject.grossArea();
  }

  idfObject.setDouble(InternalMassFields::SurfaceArea, multiplier*(*surfaceArea));

  return idfObject;
}

} // energyplus

} // openstudio

