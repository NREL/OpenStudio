/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"

#include "../../model/DefaultConstructionSet.hpp"
#include "../../model/DefaultConstructionSet_Impl.hpp"

#include "../../model/DefaultSurfaceConstructions.hpp"
#include "../../model/DefaultSubSurfaceConstructions.hpp"
#include "../../model/ConstructionBase.hpp"
#include "../../model/Building.hpp"
#include "../../model/BuildingStory.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/PlanarSurface.hpp"
#include "../../model/Surface.hpp"
#include "../../model/SubSurface.hpp"
#include "../../model/InteriorPartitionSurfaceGroup.hpp"
#include "../../model/InteriorPartitionSurface.hpp"

#include <utilities/idd/ConstructionAssignmentSet_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  // When Space translation is excluded there is no E+ Space, so defaulted constructions would end up unresolved. This function hard-assigns
  // constructions to all surfaces, subsurfaces and interior partitions in the model that have defaulted constructions, using the
  // DefaultConstructionSet at the Space, SpaceType and BuildingStory level.
  void ForwardTranslator::hardApplyConstructionsWhenSpaceTranslationOff(model::Model& model) {
    for (auto& space : model.getConcreteModelObjects<model::Space>()) {

      // Step 1: Collect PlanarSurfaces with defaulted constructions (Surface, SubSurface, InteriorPartitionSurface)
      std::vector<PlanarSurface> planarSurfaces;
      auto surfaces = space.surfaces();
      planarSurfaces.reserve(2 * surfaces.size());  // Initial guess
      for (Surface& surface : surfaces) {
        if (surface.isConstructionDefaulted()) {
          planarSurfaces.push_back(surface);
        }
        for (SubSurface& subSurface : surface.subSurfaces()) {
          if (subSurface.isConstructionDefaulted()) {
            planarSurfaces.push_back(subSurface);
          }
        }
      }

      for (const InteriorPartitionSurfaceGroup& interiorPartitionSurfaceGroup : space.interiorPartitionSurfaceGroups()) {
        for (InteriorPartitionSurface& interiorPartitionSurface : interiorPartitionSurfaceGroup.interiorPartitionSurfaces()) {
          if (interiorPartitionSurface.isConstructionDefaulted()) {
            planarSurfaces.push_back(interiorPartitionSurface);
          }
        }
      }

      if (planarSurfaces.empty()) {
        LOG(Trace, "Space '" << space.nameString() << "' has no surfaces, subsurfaces or interior partitions with defaulted constructions, skipping");
        continue;
      }

      // Gather candidates DCS from Space, SpaceType and BuildingStory, instead of doing it for each planar surface to limit waste.
      // I also don't call Space::getDefaultConstructionWithSearchDistance to avoid looking at the Building/Building SpaceType Level, which would be
      // another waste
      std::vector<DefaultConstructionSet> candidateDCSs;
      if (auto dcs_ = space.defaultConstructionSet()) {
        candidateDCSs.push_back(*dcs_);
      }
      if (auto spaceType_ = space.spaceType(); spaceType_ && !space.isSpaceTypeDefaulted()) {
        if (auto dcs_ = spaceType_->defaultConstructionSet()) {
          candidateDCSs.push_back(*dcs_);
        }
      }
      if (auto buildingStory_ = space.buildingStory()) {
        if (auto dcs_ = buildingStory_->defaultConstructionSet()) {
          candidateDCSs.push_back(*dcs_);
        }
      }

      if (candidateDCSs.empty()) {
        LOG(Trace, "Space '" << space.nameString() << "' has no DefaultConstructionSet at the Space, SpaceType or BuildingStory level, skipping");
        continue;
      }
      for (PlanarSurface& planarSurface : planarSurfaces) {
        for (const auto& dcs_ : candidateDCSs) {
          if (auto construction_ = dcs_.getDefaultConstruction(planarSurface)) {
            planarSurface.setConstruction(*construction_);
            break;  // Stop at the first DCS that provides a construction for this planar surface
          }
        }
      }
    }
  }

  boost::optional<IdfObject> ForwardTranslator::translateDefaultConstructionSet(model::DefaultConstructionSet& modelObject) {

    // Instantiate an IdfObject of the class to store the values
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::ConstructionAssignmentSet, modelObject);
    // Name
    idfObject.setName(modelObject.nameString());

    // Exterior Surface Construction Assignments Name: Optional Object
    if (boost::optional<DefaultSurfaceConstructions> exteriorSurfaceConstructionAssignments_ = modelObject.defaultExteriorSurfaceConstructions()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(*exteriorSurfaceConstructionAssignments_)) {
        idfObject.setString(ConstructionAssignmentSetFields::ExteriorSurfaceConstructionAssignmentsName, wo_->nameString());
      }
    }

    // Interior Surface Construction Assignments Name: Optional Object
    if (boost::optional<DefaultSurfaceConstructions> interiorSurfaceConstructionAssignments_ = modelObject.defaultInteriorSurfaceConstructions()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(*interiorSurfaceConstructionAssignments_)) {
        idfObject.setString(ConstructionAssignmentSetFields::InteriorSurfaceConstructionAssignmentsName, wo_->nameString());
      }
    }

    // Ground Contact Surface Construction Assignments Name: Optional Object
    if (boost::optional<DefaultSurfaceConstructions> groundContactSurfaceConstructionAssignments_ =
          modelObject.defaultGroundContactSurfaceConstructions()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(*groundContactSurfaceConstructionAssignments_)) {
        idfObject.setString(ConstructionAssignmentSetFields::GroundContactSurfaceConstructionAssignmentsName, wo_->nameString());
      }
    }

    // Exterior SubSurface Construction Assignments Name: Optional Object
    if (boost::optional<DefaultSubSurfaceConstructions> exteriorSubSurfaceConstructionAssignments_ =
          modelObject.defaultExteriorSubSurfaceConstructions()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(*exteriorSubSurfaceConstructionAssignments_)) {
        idfObject.setString(ConstructionAssignmentSetFields::ExteriorSubSurfaceConstructionAssignmentsName, wo_->nameString());
      }
    }

    // Interior SubSurface Construction Assignments Name: Optional Object
    if (boost::optional<DefaultSubSurfaceConstructions> interiorSubSurfaceConstructionAssignments_ =
          modelObject.defaultInteriorSubSurfaceConstructions()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(*interiorSubSurfaceConstructionAssignments_)) {
        idfObject.setString(ConstructionAssignmentSetFields::InteriorSubSurfaceConstructionAssignmentsName, wo_->nameString());
      }
    }

    // Interior Partition Construction Name: Optional Object
    if (boost::optional<ConstructionBase> interiorPartitionConstruction_ = modelObject.interiorPartitionConstruction()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(*interiorPartitionConstruction_)) {
        idfObject.setString(ConstructionAssignmentSetFields::InteriorPartitionConstructionName, wo_->nameString());
      }
    }

    // Adiabatic Surface Construction Name: Optional Object
    if (boost::optional<ConstructionBase> adiabaticSurfaceConstruction_ = modelObject.adiabaticSurfaceConstruction()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(*adiabaticSurfaceConstruction_)) {
        idfObject.setString(ConstructionAssignmentSetFields::AdiabaticSurfaceConstructionName, wo_->nameString());
      }
    }

    // NOTE: spaceShadingConstruction, buildingShadingConstruction, and siteShadingConstruction are not in the E+ object

    return idfObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
