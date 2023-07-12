/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
