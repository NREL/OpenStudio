/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

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

  OptionalModelObject ReverseTranslator::translateBuilding(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::Building) {
      LOG(Error, "WorkspaceObject is not IddObjectType: Building");
      return boost::none;
    }

    // ensure that site and simulation control have been mapped
    for (const WorkspaceObject& siteObject : m_workspace.getObjectsByType(IddObjectType::Site_Location)) {
      translateAndMapWorkspaceObject(siteObject);
    }

    for (const WorkspaceObject& simControlObject : m_workspace.getObjectsByType(IddObjectType::SimulationControl)) {
      translateAndMapWorkspaceObject(simControlObject);
    }

    // create the building
    openstudio::model::Building building = m_model.getUniqueModelObject<Building>();

    OptionalString s = workspaceObject.name();
    if (s) {
      building.setName(*s);
    }

    OptionalDouble d = workspaceObject.getDouble(openstudio::BuildingFields::NorthAxis);
    if (d) {
      building.setNorthAxis(*d);
    }

    // fields that go to site
    s = workspaceObject.getString(openstudio::BuildingFields::Terrain);
    if (s) {
      boost::optional<Site> site = m_model.getOptionalUniqueModelObject<Site>();
      if (site) {
        site->setTerrain(*s);
      }
    }

    // fields that go to simulation control
    d = workspaceObject.getDouble(openstudio::BuildingFields::LoadsConvergenceToleranceValue);
    if (d) {
      boost::optional<SimulationControl> simulationControl = m_model.getUniqueModelObject<SimulationControl>();
      if (simulationControl) {
        simulationControl->setLoadsConvergenceToleranceValue(*d);
      }
    }

    d = workspaceObject.getDouble(openstudio::BuildingFields::TemperatureConvergenceToleranceValue);
    if (d) {
      boost::optional<SimulationControl> simulationControl = m_model.getUniqueModelObject<SimulationControl>();
      if (simulationControl) {
        simulationControl->setTemperatureConvergenceToleranceValue(*d);
      }
    }

    s = workspaceObject.getString(openstudio::BuildingFields::SolarDistribution);
    if (s) {
      boost::optional<SimulationControl> simulationControl = m_model.getUniqueModelObject<SimulationControl>();
      if (simulationControl) {
        simulationControl->setSolarDistribution(*s);
      }
    }

    OptionalInt i = workspaceObject.getInt(openstudio::BuildingFields::MaximumNumberofWarmupDays);
    if (i) {
      boost::optional<SimulationControl> simulationControl = m_model.getUniqueModelObject<SimulationControl>();
      if (simulationControl) {
        simulationControl->setMaximumNumberofWarmupDays(*i);
      }
    }

    i = workspaceObject.getInt(openstudio::BuildingFields::MinimumNumberofWarmupDays);
    if (i) {
      boost::optional<SimulationControl> simulationControl = m_model.getUniqueModelObject<SimulationControl>();
      if (simulationControl) {
        simulationControl->setMinimumNumberofWarmupDays(*i);
      }
    }

    return building;
  }

}  // namespace energyplus

}  // namespace openstudio
