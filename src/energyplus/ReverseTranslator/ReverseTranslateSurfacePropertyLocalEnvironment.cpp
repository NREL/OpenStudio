/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/SurfacePropertyLocalEnvironment.hpp"

#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"
#include "../../model/SubSurface.hpp"
#include "../../model/SubSurface_Impl.hpp"

#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include "../../model/SurfacePropertySurroundingSurfaces.hpp"
#include "../../model/SurfacePropertySurroundingSurfaces_Impl.hpp"

// #include "../../model/OutdoorAirNode.hpp"
// #include "../../model/OutdoorAirNode_Impl.hpp"

#include <utilities/idd/SurfaceProperty_LocalEnvironment_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateSurfacePropertyLocalEnvironment(const WorkspaceObject& workspaceObject) {
    boost::optional<WorkspaceObject> wo_;
    boost::optional<ModelObject> mo_;

    boost::optional<ModelObject> mo_planarSurface;

    boost::optional<SurfacePropertyLocalEnvironment> localEnv_;
    // Exterior Surface Name: Optional Object
    if ((wo_ = workspaceObject.getTarget(SurfaceProperty_LocalEnvironmentFields::ExteriorSurfaceName))) {
      if ((mo_ = translateAndMapWorkspaceObject(wo_.get()))) {
        if (boost::optional<Surface> _exteriorSurface = mo_->optionalCast<Surface>()) {
          localEnv_ = openstudio::model::SurfacePropertyLocalEnvironment(_exteriorSurface.get());
        } else if (boost::optional<SubSurface> _exteriorSurface = mo_->optionalCast<SubSurface>()) {
          localEnv_ = openstudio::model::SurfacePropertyLocalEnvironment(_exteriorSurface.get());
        } else {
          LOG(Error, workspaceObject.briefDescription()
                       << " has a wrong type for 'Exterior Surface Name', expected Surface or SubSurface, got " << mo_->briefDescription());
          return boost::none;
        }
      } else {
        LOG(Error, workspaceObject.briefDescription() << " has a wrong type for 'Exterior Surface Name'");
        return boost::none;
      }
    } else {
      LOG(Error, workspaceObject.briefDescription() << " does not have an 'Exterior Surface Name' attached.");
      return boost::none;
    }

    SurfacePropertyLocalEnvironment modelObject = localEnv_.get();

    // Name
    modelObject.setName(workspaceObject.nameString());

    // External Shading Fraction Schedule Name: Optional Object
    if ((wo_ = workspaceObject.getTarget(SurfaceProperty_LocalEnvironmentFields::ExternalShadingFractionScheduleName))) {
      if ((mo_ = translateAndMapWorkspaceObject(wo_.get()))) {
        if (auto externalShadingFractionSchedule_ = mo_->optionalCast<Schedule>()) {
          modelObject.setExternalShadingFractionSchedule(externalShadingFractionSchedule_.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'External Shading Fraction Schedule Name'");
        }
      }
    }
    // Surrounding Surfaces Object Name: Optional Object
    if ((wo_ = workspaceObject.getTarget(SurfaceProperty_LocalEnvironmentFields::SurroundingSurfacesObjectName))) {
      if ((mo_ = translateAndMapWorkspaceObject(wo_.get()))) {
        if (auto sp_ = mo_->optionalCast<SurfacePropertySurroundingSurfaces>()) {
          modelObject.setSurfacePropertySurroundingSurfaces(sp_.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Surrounding Surfaces Object Name'");
        }
      }
    }

    // Outdoor Air Node Name: Optional Object
    // if ((wo_ = workspaceObject.getTarget(SurfaceProperty_LocalEnvironmentFields::OutdoorAirNodeName))) {
    //   if ((mo_ = translateAndMapWorkspaceObject(wo_.get()))) {
    //     if (boost::optional<OutdoorAirNode> _outdoorAirNode = mo_->optionalCast<OutdoorAirNode>()) {
    //       modelObject.setOutdoorAirNode(_outdoorAirNode.get());
    //     } else {
    //       LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Outdoor Air Node Name'");
    //     }
    //   }
    // }

    return modelObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
