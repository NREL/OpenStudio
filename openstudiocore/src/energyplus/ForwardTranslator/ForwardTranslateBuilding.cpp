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

#include "../../model/Building.hpp"
#include "../../model/Building_Impl.hpp"
#include "../../model/Site.hpp"
#include "../../model/Site_Impl.hpp"
#include "../../model/SimulationControl.hpp"
#include "../../model/SimulationControl_Impl.hpp"
#include "../../model/ShadingSurfaceGroup.hpp"
#include "../../model/LifeCycleCost.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/WorkspaceObjectOrder.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/Building_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateBuilding( Building & modelObject )
{
  Model model = modelObject.model();

  IdfObject idfObject(IddObjectType::Building);
  m_idfObjects.push_back(idfObject);

  for (LifeCycleCost lifeCycleCost : modelObject.lifeCycleCosts()){
    translateAndMapModelObject(lifeCycleCost);
  }
  
  OptionalString optS = modelObject.name();
  if( optS )
  {
    idfObject.setName(stripOS2(*optS));
  }

  if (!modelObject.isNorthAxisDefaulted()){
    idfObject.setDouble(openstudio::BuildingFields::NorthAxis, modelObject.northAxis());
  }

  // terrain comes from Site
  OptionalSite site = model.getOptionalUniqueModelObject<Site>();
  if (site){
    if (!site->isTerrainDefaulted()){
      idfObject.setString(openstudio::BuildingFields::Terrain, site->terrain());
    }
  }

  // these fields come from SimulationControl
  OptionalSimulationControl simulationControl = model.getOptionalUniqueModelObject<SimulationControl>();
  if (simulationControl){
    if (!simulationControl->isLoadsConvergenceToleranceValueDefaulted()){
      idfObject.setDouble(openstudio::BuildingFields::LoadsConvergenceToleranceValue, simulationControl->loadsConvergenceToleranceValue());
    }

    if (!simulationControl->isTemperatureConvergenceToleranceValueDefaulted()){
      idfObject.setDouble(openstudio::BuildingFields::TemperatureConvergenceToleranceValue, simulationControl->temperatureConvergenceToleranceValue());
    }

    if (!simulationControl->isSolarDistributionDefaulted()){
      idfObject.setString(openstudio::BuildingFields::SolarDistribution, simulationControl->solarDistribution());
    }

    if (!simulationControl->isMaximumNumberofWarmupDaysDefaulted()){
      idfObject.setInt(openstudio::BuildingFields::MaximumNumberofWarmupDays, simulationControl->maximumNumberofWarmupDays());
    }

    if (!simulationControl->isMinimumNumberofWarmupDaysDefaulted()) {
      idfObject.setInt(openstudio::BuildingFields::MinimumNumberofWarmupDays, simulationControl->minimumNumberofWarmupDays());
    }
  }

  // translate shading groups
  ShadingSurfaceGroupVector shadingSurfaceGroups = modelObject.shadingSurfaceGroups();
  std::sort(shadingSurfaceGroups.begin(), shadingSurfaceGroups.end(), WorkspaceObjectNameLess());
  for (ShadingSurfaceGroup& shadingSurfaceGroup : shadingSurfaceGroups){
    translateAndMapModelObject(shadingSurfaceGroup);
  }

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

