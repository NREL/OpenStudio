/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/ConstructionWithInternalSource.hpp"
#include "../../model/ConstructionWithInternalSource_Impl.hpp"
#include "../../model/OpaqueMaterial.hpp"
#include "../../model/OpaqueMaterial_Impl.hpp"
#include "../../model/FenestrationMaterial.hpp"
#include "../../model/FenestrationMaterial_Impl.hpp"
#include "../../model/ModelPartitionMaterial.hpp"
#include "../../model/ModelPartitionMaterial_Impl.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/Construction_FieldEnums.hxx>
#include <utilities/idd/ConstructionProperty_InternalHeatSource_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateConstructionWithInternalSource(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::ConstructionProperty_InternalHeatSource) {
      LOG(Error, "WorkspaceObject is not IddObjectType: ConstructionProperty_InternalHeatSource");
      return boost::none;
    }

    // Construction Name
    OptionalWorkspaceObject wo_construction = workspaceObject.getTarget(ConstructionProperty_InternalHeatSourceFields::ConstructionName);
    if (!wo_construction) {
      LOG(Error, workspaceObject.briefDescription() << " does not reference any Construction. It will not be translated");
      return boost::none;
    }

    openstudio::model::ConstructionWithInternalSource construction(m_model);

    OptionalString s;
    OptionalDouble d;
    OptionalInt i;
    OptionalWorkspaceObject target;

    // Name is from the "Construction" object, not the ConstructionProperty:InternalHeatSource
    s = wo_construction->name();
    if (s) {
      construction.setName(*s);
    }

    i = workspaceObject.getInt(ConstructionProperty_InternalHeatSourceFields::ThermalSourcePresentAfterLayerNumber);
    if (i) {
      construction.setSourcePresentAfterLayerNumber(*i);
    }

    i = workspaceObject.getInt(ConstructionProperty_InternalHeatSourceFields::TemperatureCalculationRequestedAfterLayerNumber);
    if (i) {
      construction.setTemperatureCalculationRequestedAfterLayerNumber(*i);
    }

    i = workspaceObject.getInt(ConstructionProperty_InternalHeatSourceFields::DimensionsfortheCTFCalculation);
    if (i) {
      construction.setDimensionsForTheCTFCalculation(*i);
    }

    d = workspaceObject.getDouble(ConstructionProperty_InternalHeatSourceFields::TubeSpacing);
    if (d) {
      construction.setTubeSpacing(*d);
    }

    d = workspaceObject.getDouble(ConstructionProperty_InternalHeatSourceFields::TwoDimensionalTemperatureCalculationPosition);
    if (d) {
      construction.setTwoDimensionalTemperatureCalculationPosition(*d);
    }

    // get extensible groups FROM THE CONSTRUCTION object for layers
    unsigned n = wo_construction->numExtensibleGroups();
    // now we get the workspace objects and try to find them and place them in the model object
    // Loop over all the fields except the first, which is the name
    OptionalWorkspaceObject owo;
    OptionalModelObject temp;
    for (unsigned i = 0; i < n; ++i) {
      owo = wo_construction->getExtensibleGroup(i).cast<WorkspaceExtensibleGroup>().getTarget(ConstructionExtensibleFields::Layer);
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

    // We register this object in the map so it's not translated twice
    m_workspaceToModelMap.insert(std::make_pair(workspaceObject.handle(), construction));

    return construction;
  }

}  // namespace energyplus

}  // namespace openstudio
