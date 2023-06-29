/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/FoundationKiva.hpp"
#include "../../model/FoundationKiva_Impl.hpp"
#include "../../model/Material.hpp"
#include "../../model/Material_Impl.hpp"
#include "../../model/ConstructionBase.hpp"
#include "../../model/ConstructionBase_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/Foundation_Kiva_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateFoundationKiva(FoundationKiva& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::Foundation_Kiva);

    m_idfObjects.push_back(idfObject);

    idfObject.setString(Foundation_KivaFields::Name, modelObject.name().get());

    boost::optional<double> value;

    if ((value = modelObject.initialIndoorAirTemperature())) {
      idfObject.setDouble(Foundation_KivaFields::InitialIndoorAirTemperature, value.get());
    }

    boost::optional<Material> interiorHorizontalInsulationMaterial = modelObject.interiorHorizontalInsulationMaterial();
    if (interiorHorizontalInsulationMaterial) {
      idfObject.setString(Foundation_KivaFields::InteriorHorizontalInsulationMaterialName, interiorHorizontalInsulationMaterial->name().get());
      idfObject.setDouble(Foundation_KivaFields::InteriorHorizontalInsulationDepth, modelObject.interiorHorizontalInsulationDepth());
    }

    if ((value = modelObject.interiorHorizontalInsulationWidth())) {
      idfObject.setDouble(Foundation_KivaFields::InteriorHorizontalInsulationWidth, value.get());
    }

    boost::optional<Material> interiorVerticalInsulationMaterial = modelObject.interiorVerticalInsulationMaterial();
    if (interiorVerticalInsulationMaterial) {
      idfObject.setString(Foundation_KivaFields::InteriorVerticalInsulationMaterialName, interiorVerticalInsulationMaterial->name().get());
    }

    if ((value = modelObject.interiorVerticalInsulationDepth())) {
      idfObject.setDouble(Foundation_KivaFields::InteriorVerticalInsulationDepth, value.get());
    }

    boost::optional<Material> exteriorHorizontalInsulationMaterial = modelObject.exteriorHorizontalInsulationMaterial();
    if (exteriorHorizontalInsulationMaterial) {
      idfObject.setString(Foundation_KivaFields::ExteriorHorizontalInsulationMaterialName, exteriorHorizontalInsulationMaterial->name().get());
      idfObject.setDouble(Foundation_KivaFields::ExteriorHorizontalInsulationWidth, modelObject.exteriorHorizontalInsulationWidth());
    }

    if ((value = modelObject.exteriorHorizontalInsulationDepth())) {
      idfObject.setDouble(Foundation_KivaFields::ExteriorHorizontalInsulationDepth, value.get());
    }

    boost::optional<Material> exteriorVerticalInsulationMaterial = modelObject.exteriorVerticalInsulationMaterial();
    if (exteriorVerticalInsulationMaterial) {
      idfObject.setString(Foundation_KivaFields::ExteriorVerticalInsulationMaterialName, exteriorVerticalInsulationMaterial->name().get());
    }

    if ((value = modelObject.exteriorVerticalInsulationDepth())) {
      idfObject.setDouble(Foundation_KivaFields::ExteriorVerticalInsulationDepth, value.get());
    }

    idfObject.setDouble(Foundation_KivaFields::WallHeightAboveGrade, modelObject.wallHeightAboveGrade());

    idfObject.setDouble(Foundation_KivaFields::WallDepthBelowSlab, modelObject.wallDepthBelowSlab());

    boost::optional<ConstructionBase> footingWallConstruction = modelObject.footingWallConstruction();
    if (footingWallConstruction) {
      idfObject.setString(Foundation_KivaFields::FootingWallConstructionName, footingWallConstruction->name().get());
    }

    boost::optional<Material> footingMaterial = modelObject.footingMaterial();
    if (footingMaterial) {
      idfObject.setString(Foundation_KivaFields::FootingMaterialName, footingMaterial->name().get());
      idfObject.setDouble(Foundation_KivaFields::FootingDepth, modelObject.footingDepth());
    }

    //UserDefinedCustomBlocks
    std::vector<CustomBlock> customBlocks = modelObject.customBlocks();
    if (!customBlocks.empty()) {
      for (const CustomBlock& customBlock : customBlocks) {
        auto eg = idfObject.pushExtensibleGroup();
        eg.setString(Foundation_KivaExtensibleFields::CustomBlockMaterialName, customBlock.material().name().get());
        eg.setDouble(Foundation_KivaExtensibleFields::CustomBlockDepth, customBlock.depth());
        eg.setDouble(Foundation_KivaExtensibleFields::CustomBlockXPosition, customBlock.xPosition());
        eg.setDouble(Foundation_KivaExtensibleFields::CustomBlockZPosition, customBlock.zPosition());
      }
    }

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
