/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/SurfacePropertySurroundingSurfaces.hpp"

#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/SurfaceProperty_SurroundingSurfaces_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateSurfacePropertySurroundingSurfaces(const WorkspaceObject& workspaceObject) {
    boost::optional<WorkspaceObject> wo_;
    boost::optional<ModelObject> mo_;

    openstudio::model::SurfacePropertySurroundingSurfaces modelObject(m_model);

    // Name
    modelObject.setName(workspaceObject.nameString());

    // Sky View Factor: Optional Double
    if (boost::optional<double> skyViewFactor_ = workspaceObject.getDouble(SurfaceProperty_SurroundingSurfacesFields::SkyViewFactor)) {
      modelObject.setSkyViewFactor(skyViewFactor_.get());
    }

    // Sky Temperature Schedule Name: Optional Object
    if ((wo_ = workspaceObject.getTarget(SurfaceProperty_SurroundingSurfacesFields::SkyTemperatureScheduleName))) {
      if ((mo_ = translateAndMapWorkspaceObject(wo_.get()))) {
        if (auto skyTemperatureSchedule_ = mo_->optionalCast<Schedule>()) {
          modelObject.setSkyTemperatureSchedule(skyTemperatureSchedule_.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Sky Temperature Schedule Name'");
        }
      }
    }
    // Ground View Factor: Optional Double
    if (boost::optional<double> groundViewFactor_ = workspaceObject.getDouble(SurfaceProperty_SurroundingSurfacesFields::GroundViewFactor)) {
      modelObject.setGroundViewFactor(groundViewFactor_.get());
    }

    // Ground Temperature Schedule Name: Optional Object
    if ((wo_ = workspaceObject.getTarget(SurfaceProperty_SurroundingSurfacesFields::GroundTemperatureScheduleName))) {
      if ((mo_ = translateAndMapWorkspaceObject(wo_.get()))) {
        if (boost::optional<Schedule> _groundTemperatureSchedule = mo_->optionalCast<Schedule>()) {
          modelObject.setGroundTemperatureSchedule(_groundTemperatureSchedule.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Ground Temperature Schedule Name'");
        }
      }
    }

    // Extensible groups
    for (const IdfExtensibleGroup& eg : workspaceObject.extensibleGroups()) {
      auto weg = eg.cast<WorkspaceExtensibleGroup>();

      auto surroundingSurfaceName = weg.getString(SurfaceProperty_SurroundingSurfacesExtensibleFields::SurroundingSurfaceName).get();
      if (surroundingSurfaceName.empty()) {
        LOG(Warn, "Extensible group " << eg.groupIndex() << "(0-indexed) has an empty Surrounding Surface Name field, skipping group");
      }

      // This has a default of 0, so roll with it
      auto viewFactor = weg.getDouble(SurfaceProperty_SurroundingSurfacesExtensibleFields::SurroundingSurfaceViewFactor).get();

      if ((wo_ = weg.getTarget(SurfaceProperty_SurroundingSurfacesExtensibleFields::SurroundingSurfaceTemperatureScheduleName))) {
        if ((mo_ = translateAndMapWorkspaceObject(wo_.get()))) {
          if (boost::optional<Schedule> tempSch_ = mo_->optionalCast<Schedule>()) {
            modelObject.addSurroundingSurfaceGroup(surroundingSurfaceName, viewFactor, tempSch_.get());
          } else {
            LOG(Warn, "Extensible group " << eg.groupIndex() << "(0-indexed) has a wrong type for Temperature Schedule, expected Schedule, got "
                                          << tempSch_->briefDescription());
          }
        } else {
          LOG(Warn, "Could not translate Temperature Schedule for Extensible group " << eg.groupIndex());
        }
      } else {
        LOG(Warn, "Extensible group " << eg.groupIndex() << "(0-indexed) is missed the Temperature Schedule");
      }
    }

    return modelObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
