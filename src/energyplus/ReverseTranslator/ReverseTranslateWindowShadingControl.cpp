/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/ShadingControl.hpp"
#include "../../model/ShadingControl_Impl.hpp"
#include "../../model/Construction.hpp"
#include "../../model/Construction_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ShadingMaterial.hpp"
#include "../../model/ShadingMaterial_Impl.hpp"
#include "../../model/SubSurface.hpp"
#include "../../model/SubSurface_Impl.hpp"

#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/WindowShadingControl_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {
namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateWindowShadingControl(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::WindowShadingControl) {
      LOG(Error, "WorkspaceObject is not IddObjectType: WindowShadingControl");
      return boost::none;
    }

    OptionalWorkspaceObject target;

    boost::optional<Construction> construction;
    if ((target = workspaceObject.getTarget(WindowShadingControlFields::ConstructionwithShadingName))) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (modelObject->optionalCast<Construction>()) {
          construction = modelObject->cast<Construction>();
        }
      }
    }

    boost::optional<ShadingMaterial> shadingMaterial;
    if ((target = workspaceObject.getTarget(WindowShadingControlFields::ShadingDeviceMaterialName))) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (modelObject->optionalCast<ShadingMaterial>()) {
          shadingMaterial = modelObject->cast<ShadingMaterial>();
        }
      }
    }

    boost::optional<ShadingControl> shadingControl;
    if (construction) {
      shadingControl = openstudio::model::ShadingControl(*construction);
    } else if (shadingMaterial) {
      shadingControl = openstudio::model::ShadingControl(*shadingMaterial);
    } else {
      LOG(Error, workspaceObject.briefDescription() << " does not appear to have a valid Construction or Shading Material Name attached to it");
      return boost::none;
    }

    if (shadingControl) {

      OptionalString s;
      OptionalDouble d;

      s = workspaceObject.name();
      if (s) {
        shadingControl->setName(*s);
      }

      s = workspaceObject.getString(WindowShadingControlFields::ShadingType);
      if (s) {
        if (istringEqual("InteriorBlind", *s)) {
          shadingControl->setShadingType("InteriorDaylightRedirectionDevice");
        } else {
          shadingControl->setShadingType(*s);
        }
      }

      s = workspaceObject.getString(WindowShadingControlFields::ShadingControlType);
      if (s) {
        shadingControl->setShadingControlType(*s);
      }

      if ((target = workspaceObject.getTarget(WindowShadingControlFields::ScheduleName))) {
        OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
        if (modelObject) {
          if (modelObject->optionalCast<Schedule>()) {
            shadingControl->setSchedule(modelObject->cast<Schedule>());
          }
        }
      }

      d = workspaceObject.getDouble(WindowShadingControlFields::Setpoint);
      if (s) {
        shadingControl->setSetpoint(*d);
      }

      s = workspaceObject.getString(WindowShadingControlFields::GlareControlIsActive);
      if (s) {
        if (istringEqual(*s, "Yes")) {
          shadingControl->setGlareControlIsActive(true);
        } else if (istringEqual(*s, "No")) {
          shadingControl->setGlareControlIsActive(false);
        }
      }

      s = workspaceObject.getString(WindowShadingControlFields::TypeofSlatAngleControlforBlinds);
      if (s) {
        shadingControl->setTypeofSlatAngleControlforBlinds(*s);
      }

      if ((target = workspaceObject.getTarget(WindowShadingControlFields::SlatAngleScheduleName))) {
        OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
        if (modelObject) {
          if (modelObject->optionalCast<Schedule>()) {
            shadingControl->setSlatAngleSchedule(modelObject->cast<Schedule>());
          }
        }
      }

      d = workspaceObject.getDouble(WindowShadingControlFields::Setpoint2);
      if (s) {
        shadingControl->setSetpoint2(*d);
      }

      s = workspaceObject.getString(WindowShadingControlFields::MultipleSurfaceControlType);
      if (s) {
        shadingControl->setMultipleSurfaceControlType(*s);
      }

      // get extensible groups for sub surfaces
      for (const IdfExtensibleGroup& idfGroup : workspaceObject.extensibleGroups()) {
        auto workspaceGroup = idfGroup.cast<WorkspaceExtensibleGroup>();
        OptionalWorkspaceObject target = workspaceGroup.getTarget(WindowShadingControlExtensibleFields::FenestrationSurfaceName);
        OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);

        // add the sub surface
        shadingControl->addSubSurface(modelObject->cast<SubSurface>());
      }

      return shadingControl.get();
    } else {
      LOG(Error, "Unknown error translating " << workspaceObject.briefDescription());

      return boost::none;
    }
  }

}  // namespace energyplus
}  // namespace openstudio
