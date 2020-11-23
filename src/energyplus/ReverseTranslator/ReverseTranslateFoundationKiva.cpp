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
      WorkspaceExtensibleGroup workspaceGroup = idfGroup.cast<WorkspaceExtensibleGroup>();
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
