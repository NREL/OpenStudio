/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/Construction.hpp"
#include "../../model/Construction_Impl.hpp"
#include "../../model/Material.hpp"
#include "../../model/Material_Impl.hpp"
#include "../../model/OpaqueMaterial.hpp"
#include "../../model/OpaqueMaterial_Impl.hpp"
#include "../../model/FenestrationMaterial.hpp"
#include "../../model/FenestrationMaterial_Impl.hpp"
#include "../../model/ModelPartitionMaterial.hpp"
#include "../../model/ModelPartitionMaterial_Impl.hpp"

#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/Construction_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateConstruction(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::Construction) {
      LOG(Error, "WorkspaceObject is not IddObjectType: Construction");
      return boost::none;
    }

    // E+ 9.5.0: Check if the construction is referenced by a ConstructionProperty:InternalHeatSource
    std::vector<WorkspaceObject> constructionProps = workspaceObject.getSources(IddObjectType::ConstructionProperty_InternalHeatSource);
    if (!constructionProps.empty()) {
      LOG(Info, "Construction '" << workspaceObject.nameString() << "' will be translated to a ConstructionWithInternalSource'");
      if (constructionProps.size() > 1) {
        LOG(Warn, "Construction '" << workspaceObject.nameString() << "' is referenced by more than one ConstructionProperty:InternalHeatSource."
                                   << "Check IDF file for validity. Only the first one will be used.");
      }
      return translateConstructionWithInternalSource(constructionProps[0]);
    }

    // Otherwise, business as usual
    Construction construction(m_model);
    OS_ASSERT(construction.numLayers() == 0u);
    OptionalString optS = workspaceObject.name();
    if (optS) {
      construction.setName(*optS);
    }

    unsigned n = workspaceObject.numExtensibleGroups();
    // now we get the workspace objects and try to find them and place them in the model object
    // Loop over all the fields except the first, which is the name
    OptionalWorkspaceObject owo;
    OptionalModelObject temp;
    for (unsigned i = 0; i < n; ++i) {
      owo = workspaceObject.getExtensibleGroup(i).cast<WorkspaceExtensibleGroup>().getTarget(ConstructionExtensibleFields::Layer);
      if (owo) {
        temp = translateAndMapWorkspaceObject(*owo);
      }
      if (!(owo && temp)) {
        LOG(Error, "Finding Construction Layer in workspace failed.");
        construction.remove();
        return boost::none;
      }
      // Assuming names of materials are unique
      OptionalMaterial mat = temp->optionalCast<Material>();

      bool inserted(false);
      if (mat) {
        OptionalOpaqueMaterial opaqueMaterial = mat->optionalCast<OpaqueMaterial>();
        if (opaqueMaterial) {
          inserted = construction.insertLayer(i, *opaqueMaterial);
        } else {
          OptionalFenestrationMaterial fenestrationMaterial = mat->optionalCast<FenestrationMaterial>();
          if (fenestrationMaterial) {
            inserted = construction.insertLayer(i, *fenestrationMaterial);
          } else {
            OptionalModelPartitionMaterial modelPartitionMaterial = mat->optionalCast<ModelPartitionMaterial>();
            if (modelPartitionMaterial) {
              if (construction.numLayers() == 0u) {
                inserted = construction.setLayer(*modelPartitionMaterial);
              }
            }
          }
        }
      }

      if (!inserted) {
        LOG(Error, "Insertion of Construction Layer failed.");
        construction.remove();
        return boost::none;
      }
    }

    return construction;
  }

}  // namespace energyplus

}  // namespace openstudio
