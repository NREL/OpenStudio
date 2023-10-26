/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/MaterialPropertyMoisturePenetrationDepthSettings.hpp"
#include "../../model/MaterialPropertyMoisturePenetrationDepthSettings_Impl.hpp"
#include "../../model/Material.hpp"
#include "../../model/Material_Impl.hpp"

#include <utilities/idd/MaterialProperty_MoisturePenetrationDepth_Settings_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateMaterialPropertyMoisturePenetrationDepthSettings(MaterialPropertyMoisturePenetrationDepthSettings& modelObject) {
    IdfObject idfObject = createAndRegisterIdfObject(openstudio::IddObjectType::MaterialProperty_MoisturePenetrationDepth_Settings, modelObject);

    idfObject.setString(MaterialProperty_MoisturePenetrationDepth_SettingsFields::Name, modelObject.material().nameString());

    idfObject.setDouble(MaterialProperty_MoisturePenetrationDepth_SettingsFields::WaterVaporDiffusionResistanceFactor,
                        modelObject.waterVaporDiffusionResistanceFactor());

    idfObject.setDouble(MaterialProperty_MoisturePenetrationDepth_SettingsFields::MoistureEquationCoefficienta,
                        modelObject.moistureEquationCoefficientA());

    idfObject.setDouble(MaterialProperty_MoisturePenetrationDepth_SettingsFields::MoistureEquationCoefficientb,
                        modelObject.moistureEquationCoefficientB());

    idfObject.setDouble(MaterialProperty_MoisturePenetrationDepth_SettingsFields::MoistureEquationCoefficientc,
                        modelObject.moistureEquationCoefficientC());

    idfObject.setDouble(MaterialProperty_MoisturePenetrationDepth_SettingsFields::MoistureEquationCoefficientd,
                        modelObject.moistureEquationCoefficientD());

    boost::optional<double> value;

    if (modelObject.isSurfaceLayerPenetrationDepthAutocalculated()) {
      idfObject.setString(MaterialProperty_MoisturePenetrationDepth_SettingsFields::SurfaceLayerPenetrationDepth, "Autocalculate");
    } else if ((value = modelObject.surfaceLayerPenetrationDepth())) {
      idfObject.setDouble(MaterialProperty_MoisturePenetrationDepth_SettingsFields::SurfaceLayerPenetrationDepth, value.get());
    }

    if (modelObject.isDeepLayerPenetrationDepthAutocalculated()) {
      idfObject.setString(MaterialProperty_MoisturePenetrationDepth_SettingsFields::DeepLayerPenetrationDepth, "Autocalculate");
    } else if ((value = modelObject.deepLayerPenetrationDepth())) {
      idfObject.setDouble(MaterialProperty_MoisturePenetrationDepth_SettingsFields::DeepLayerPenetrationDepth, value.get());
    }

    idfObject.setDouble(MaterialProperty_MoisturePenetrationDepth_SettingsFields::CoatingLayerThickness, modelObject.coatingLayerThickness());

    idfObject.setDouble(MaterialProperty_MoisturePenetrationDepth_SettingsFields::CoatingLayerWaterVaporDiffusionResistanceFactor,
                        modelObject.coatingLayerWaterVaporDiffusionResistanceFactor());

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
