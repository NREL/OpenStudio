/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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

