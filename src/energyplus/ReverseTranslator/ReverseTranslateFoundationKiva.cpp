/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/FoundationKiva.hpp"
#include "../../model/FoundationKiva_Impl.hpp"
#include "../../model/Material.hpp"
#include "../../model/Material_Impl.hpp"
#include "../../model/ConstructionBase.hpp"
#include "../../model/ConstructionBase_Impl.hpp"

#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/Foundation_Kiva_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateFoundationKiva(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::Foundation_Kiva) {
      LOG(Error, "WorkspaceObject is not IddObjectType: FoundationKiva");
      return boost::none;
    }

    openstudio::model::FoundationKiva foundationKiva(m_model);

    OptionalString s;
    OptionalDouble d;
    OptionalWorkspaceObject target;

    s = workspaceObject.name();
    if (s) {
      foundationKiva.setName(*s);
    }

    d = workspaceObject.getDouble(Foundation_KivaFields::InitialIndoorAirTemperature);
    if (d) {
      foundationKiva.setInitialIndoorAirTemperature(*d);
    }

    target = workspaceObject.getTarget(openstudio::Foundation_KivaFields::InteriorHorizontalInsulationMaterialName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (modelObject->optionalCast<Material>()) {
          foundationKiva.setInteriorHorizontalInsulationMaterial(modelObject->cast<Material>());
        }
      }
    }

    d = workspaceObject.getDouble(Foundation_KivaFields::InteriorHorizontalInsulationDepth);
    if (d) {
      foundationKiva.setInteriorHorizontalInsulationDepth(*d);
    }

    d = workspaceObject.getDouble(Foundation_KivaFields::InteriorHorizontalInsulationWidth);
    if (d) {
      foundationKiva.setInteriorHorizontalInsulationWidth(*d);
    }

    target = workspaceObject.getTarget(openstudio::Foundation_KivaFields::InteriorVerticalInsulationMaterialName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (modelObject->optionalCast<Material>()) {
          foundationKiva.setInteriorVerticalInsulationMaterial(modelObject->cast<Material>());
        }
      }
    }

    d = workspaceObject.getDouble(Foundation_KivaFields::InteriorVerticalInsulationDepth);
    if (d) {
      foundationKiva.setInteriorVerticalInsulationDepth(*d);
    }

    target = workspaceObject.getTarget(openstudio::Foundation_KivaFields::ExteriorHorizontalInsulationMaterialName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (modelObject->optionalCast<Material>()) {
          foundationKiva.setExteriorHorizontalInsulationMaterial(modelObject->cast<Material>());
        }
      }
    }

    d = workspaceObject.getDouble(Foundation_KivaFields::ExteriorHorizontalInsulationDepth);
    if (d) {
      foundationKiva.setExteriorHorizontalInsulationDepth(*d);
    }

    d = workspaceObject.getDouble(Foundation_KivaFields::ExteriorHorizontalInsulationWidth);
    if (d) {
      foundationKiva.setExteriorHorizontalInsulationWidth(*d);
    }

    target = workspaceObject.getTarget(openstudio::Foundation_KivaFields::ExteriorVerticalInsulationMaterialName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (modelObject->optionalCast<Material>()) {
          foundationKiva.setExteriorVerticalInsulationMaterial(modelObject->cast<Material>());
        }
      }
    }

    d = workspaceObject.getDouble(Foundation_KivaFields::ExteriorVerticalInsulationDepth);
    if (d) {
      foundationKiva.setExteriorVerticalInsulationDepth(*d);
    }

    d = workspaceObject.getDouble(Foundation_KivaFields::WallHeightAboveGrade);
    if (d) {
      foundationKiva.setWallHeightAboveGrade(*d);
    }

    d = workspaceObject.getDouble(Foundation_KivaFields::WallDepthBelowSlab);
    if (d) {
      foundationKiva.setWallDepthBelowSlab(*d);
    }

    target = workspaceObject.getTarget(openstudio::Foundation_KivaFields::FootingWallConstructionName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (modelObject->optionalCast<ConstructionBase>()) {
          foundationKiva.setFootingWallConstruction(modelObject->cast<ConstructionBase>());
        }
      }
    }

    target = workspaceObject.getTarget(openstudio::Foundation_KivaFields::FootingMaterialName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (modelObject->optionalCast<Material>()) {
          foundationKiva.setFootingMaterial(modelObject->cast<Material>());
        }
      }
    }

    d = workspaceObject.getDouble(Foundation_KivaFields::FootingDepth);
    if (d) {
      foundationKiva.setFootingDepth(*d);
    }

    // get extensible groups for custom blocks
    for (const IdfExtensibleGroup& idfGroup : workspaceObject.extensibleGroups()) {
      auto workspaceGroup = idfGroup.cast<WorkspaceExtensibleGroup>();
      OptionalWorkspaceObject target = workspaceGroup.getTarget(Foundation_KivaExtensibleFields::CustomBlockMaterialName);
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      OptionalDouble depth = workspaceGroup.getDouble(Foundation_KivaExtensibleFields::CustomBlockDepth);
      OptionalDouble xPosition = workspaceGroup.getDouble(Foundation_KivaExtensibleFields::CustomBlockXPosition);
      OptionalDouble zPosition = workspaceGroup.getDouble(Foundation_KivaExtensibleFields::CustomBlockZPosition);

      // add the custom block
      foundationKiva.addCustomBlock(modelObject->cast<Material>(), *depth, *xPosition, *zPosition);
    }

    return foundationKiva;
  }

}  // namespace energyplus

}  // namespace openstudio
