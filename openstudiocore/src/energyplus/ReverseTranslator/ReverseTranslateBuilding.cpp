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

#include "../ReverseTranslator.hpp"

#include "../../model/Building.hpp"
#include "../../model/Building_Impl.hpp"
#include "../../model/Site.hpp"
#include "../../model/Site_Impl.hpp"
#include "../../model/SimulationControl.hpp"
#include "../../model/SimulationControl_Impl.hpp"

#include <utilities/idd/Building_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateBuilding( const WorkspaceObject & workspaceObject )
{
  if( workspaceObject.iddObject().type() != IddObjectType::Building ){
    LOG(Error, "WorkspaceObject is not IddObjectType: Building");
    return boost::none;
  }

  // ensure that site and simulation control have been mapped
  for (const WorkspaceObject& siteObject : m_workspace.getObjectsByType(IddObjectType::Site_Location)){
    translateAndMapWorkspaceObject(siteObject);
  }

  for (const WorkspaceObject& simControlObject : m_workspace.getObjectsByType(IddObjectType::SimulationControl)){
    translateAndMapWorkspaceObject(simControlObject);
  }

  // create the building
  openstudio::model::Building building = m_model.getUniqueModelObject<Building>();
  
  OptionalString s = workspaceObject.name();
  if(s){
    building.setName(*s);
  }

  OptionalDouble d = workspaceObject.getDouble(openstudio::BuildingFields::NorthAxis);
  if( d ){
    building.setNorthAxis(*d);
  }

  // fields that go to site
  s = workspaceObject.getString(openstudio::BuildingFields::Terrain);
  if (s){
    boost::optional<Site> site = m_model.getOptionalUniqueModelObject<Site>();
    if (site){
      site->setTerrain(*s);
    }
  }

  // fields that go to simulation control
  d = workspaceObject.getDouble(openstudio::BuildingFields::LoadsConvergenceToleranceValue);
  if(d){
    boost::optional<SimulationControl> simulationControl = m_model.getUniqueModelObject<SimulationControl>();
    if (simulationControl){
      simulationControl->setLoadsConvergenceToleranceValue(*d);
    }
  }

  d = workspaceObject.getDouble(openstudio::BuildingFields::TemperatureConvergenceToleranceValue);
  if(d){
    boost::optional<SimulationControl> simulationControl = m_model.getUniqueModelObject<SimulationControl>();
    if (simulationControl){
      simulationControl->setTemperatureConvergenceToleranceValue(*d);
    }
  }

  s = workspaceObject.getString(openstudio::BuildingFields::SolarDistribution);
  if(s){
    boost::optional<SimulationControl> simulationControl = m_model.getUniqueModelObject<SimulationControl>();
    if (simulationControl){
      simulationControl->setSolarDistribution(*s);
    }
  }

  OptionalInt i = workspaceObject.getInt(openstudio::BuildingFields::MaximumNumberofWarmupDays);
  if(i){
    boost::optional<SimulationControl> simulationControl = m_model.getUniqueModelObject<SimulationControl>();
    if (simulationControl){
      simulationControl->setMaximumNumberofWarmupDays(*i);
    }
  }

  i = workspaceObject.getInt(openstudio::BuildingFields::MinimumNumberofWarmupDays);
  if (i) {
    boost::optional<SimulationControl> simulationControl = m_model.getUniqueModelObject<SimulationControl>();
    if (simulationControl){
      simulationControl->setMinimumNumberofWarmupDays(*i);
    }
  }

  return building;
}

} // energyplus

} // openstudio

