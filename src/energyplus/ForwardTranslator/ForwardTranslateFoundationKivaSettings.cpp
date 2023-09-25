/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/FoundationKivaSettings.hpp"
#include "../../model/FoundationKivaSettings_Impl.hpp"

#include <utilities/idd/Foundation_Kiva_Settings_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateFoundationKivaSettings(FoundationKivaSettings& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::Foundation_Kiva_Settings);

    m_idfObjects.push_back(idfObject);

    idfObject.setDouble(Foundation_Kiva_SettingsFields::SoilConductivity, modelObject.soilConductivity());

    idfObject.setDouble(Foundation_Kiva_SettingsFields::SoilDensity, modelObject.soilDensity());

    idfObject.setDouble(Foundation_Kiva_SettingsFields::SoilSpecificHeat, modelObject.soilSpecificHeat());

    idfObject.setDouble(Foundation_Kiva_SettingsFields::GroundSolarAbsorptivity, modelObject.groundSolarAbsorptivity());

    idfObject.setDouble(Foundation_Kiva_SettingsFields::GroundThermalAbsorptivity, modelObject.groundThermalAbsorptivity());

    idfObject.setDouble(Foundation_Kiva_SettingsFields::GroundSurfaceRoughness, modelObject.groundSurfaceRoughness());

    idfObject.setDouble(Foundation_Kiva_SettingsFields::FarFieldWidth, modelObject.farFieldWidth());

    idfObject.setString(Foundation_Kiva_SettingsFields::DeepGroundBoundaryCondition, modelObject.deepGroundBoundaryCondition());

    boost::optional<double> value;

    if (modelObject.isDeepGroundDepthAutocalculated()) {
      idfObject.setString(Foundation_Kiva_SettingsFields::DeepGroundDepth, "Autocalculate");
    } else if ((value = modelObject.deepGroundDepth())) {
      idfObject.setDouble(Foundation_Kiva_SettingsFields::DeepGroundDepth, value.get());
    }

    idfObject.setDouble(Foundation_Kiva_SettingsFields::MinimumCellDimension, modelObject.minimumCellDimension());

    idfObject.setDouble(Foundation_Kiva_SettingsFields::MaximumCellGrowthCoefficient, modelObject.maximumCellGrowthCoefficient());

    idfObject.setString(Foundation_Kiva_SettingsFields::SimulationTimestep, modelObject.simulationTimestep());

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
