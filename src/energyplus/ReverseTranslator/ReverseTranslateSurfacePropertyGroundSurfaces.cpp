/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/SurfacePropertyGroundSurfaces.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/SurfaceProperty_GroundSurfaces_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateSurfacePropertyGroundSurfaces(const WorkspaceObject& workspaceObject) {

    // Instantiate an object of the class to store the values,
    openstudio::model::SurfacePropertyGroundSurfaces modelObject(m_model);

    // Name
    if (boost::optional<std::string> _name = workspaceObject.name()) {
      modelObject.setName(_name.get());
    }

    // Extensible groups
    for (const IdfExtensibleGroup& eg : workspaceObject.extensibleGroups()) {
      auto weg = eg.cast<WorkspaceExtensibleGroup>();

      auto groundSurfaceName = weg.getString(SurfaceProperty_GroundSurfacesExtensibleFields::GroundSurfaceName).get();
      if (groundSurfaceName.empty()) {
        LOG(Warn, "Extensible group " << eg.groupIndex() << "(0-indexed) has an empty Ground Surface Name field, skipping group");
        continue;
      }

      // This has a default of 0, so roll with it
      auto viewFactor = weg.getDouble(SurfaceProperty_GroundSurfacesExtensibleFields::GroundSurfaceViewFactor).get();

      boost::optional<Schedule> tempSch_;
      boost::optional<Schedule> refSch_;

      if (auto wo_ = weg.getTarget(SurfaceProperty_GroundSurfacesExtensibleFields::GroundSurfaceTemperatureScheduleName)) {
        if (auto mo_ = translateAndMapWorkspaceObject(wo_.get())) {
          if (boost::optional<Schedule> sch_ = mo_->optionalCast<Schedule>()) {
            tempSch_ = sch_;
          } else {
            LOG(Warn, "Extensible group " << eg.groupIndex() << "(0-indexed) has a wrong type for Temperature Schedule, expected Schedule, got "
                                          << tempSch_->briefDescription());
          }
        } else {
          LOG(Warn, "Could not translate Temperature Schedule for Extensible group " << eg.groupIndex());
        }
      }

      if (auto wo_ = weg.getTarget(SurfaceProperty_GroundSurfacesExtensibleFields::GroundSurfaceReflectanceScheduleName)) {
        if (auto mo_ = translateAndMapWorkspaceObject(wo_.get())) {
          if (boost::optional<Schedule> sch_ = mo_->optionalCast<Schedule>()) {
            refSch_ = sch_;
          } else {
            LOG(Warn, "Extensible group " << eg.groupIndex() << "(0-indexed) has a wrong type for Reflectance Schedule, expected Schedule, got "
                                          << tempSch_->briefDescription());
          }
        } else {
          LOG(Warn, "Could not translate Reflectance Schedule for Extensible group " << eg.groupIndex());
        }
      }

      modelObject.addGroundSurfaceGroup(groundSurfaceName, viewFactor, tempSch_, refSch_);
    }

    return modelObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
