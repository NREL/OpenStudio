/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/FoundationKivaSettings.hpp"
#include "../../model/FoundationKivaSettings_Impl.hpp"

#include <utilities/idd/Foundation_Kiva_Settings_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateFoundationKivaSettings(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::Foundation_Kiva_Settings) {
      LOG(Error, "WorkspaceObject is not IddObjectType: FoundationKivaSettings");
      return boost::none;
    }

    FoundationKivaSettings foundationKivaSettings = m_model.getUniqueModelObject<FoundationKivaSettings>();

    OptionalString s;
    OptionalDouble d;

    s = workspaceObject.name();
    if (s) {
      foundationKivaSettings.setName(*s);
    }

    d = workspaceObject.getDouble(Foundation_Kiva_SettingsFields::SoilConductivity);
    if (d) {
      foundationKivaSettings.setSoilConductivity(*d);
    }

    d = workspaceObject.getDouble(Foundation_Kiva_SettingsFields::SoilDensity);
    if (d) {
      foundationKivaSettings.setSoilDensity(*d);
    }

    d = workspaceObject.getDouble(Foundation_Kiva_SettingsFields::SoilSpecificHeat);
    if (d) {
      foundationKivaSettings.setSoilSpecificHeat(*d);
    }

    d = workspaceObject.getDouble(Foundation_Kiva_SettingsFields::GroundSolarAbsorptivity);
    if (d) {
      foundationKivaSettings.setGroundSolarAbsorptivity(*d);
    }

    d = workspaceObject.getDouble(Foundation_Kiva_SettingsFields::GroundThermalAbsorptivity);
    if (d) {
      foundationKivaSettings.setGroundThermalAbsorptivity(*d);
    }

    d = workspaceObject.getDouble(Foundation_Kiva_SettingsFields::GroundSurfaceRoughness);
    if (d) {
      foundationKivaSettings.setGroundSurfaceRoughness(*d);
    }

    d = workspaceObject.getDouble(Foundation_Kiva_SettingsFields::FarFieldWidth);
    if (d) {
      foundationKivaSettings.setFarFieldWidth(*d);
    }

    s = workspaceObject.getString(Foundation_Kiva_SettingsFields::DeepGroundBoundaryCondition);
    if (s) {
      foundationKivaSettings.setDeepGroundBoundaryCondition(*s);
    }

    d = workspaceObject.getDouble(Foundation_Kiva_SettingsFields::DeepGroundDepth);
    if (d) {
      foundationKivaSettings.setDeepGroundDepth(*d);
    }

    d = workspaceObject.getDouble(Foundation_Kiva_SettingsFields::MinimumCellDimension);
    if (d) {
      foundationKivaSettings.setMinimumCellDimension(*d);
    }

    d = workspaceObject.getDouble(Foundation_Kiva_SettingsFields::MaximumCellGrowthCoefficient);
    if (d) {
      foundationKivaSettings.setMaximumCellGrowthCoefficient(*d);
    }

    s = workspaceObject.getString(Foundation_Kiva_SettingsFields::SimulationTimestep);
    if (s) {
      foundationKivaSettings.setSimulationTimestep(*s);
    }

    return foundationKivaSettings;
  }

}  // namespace energyplus

}  // namespace openstudio
