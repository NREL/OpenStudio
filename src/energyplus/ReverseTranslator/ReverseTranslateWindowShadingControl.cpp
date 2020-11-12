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
        WorkspaceExtensibleGroup workspaceGroup = idfGroup.cast<WorkspaceExtensibleGroup>();
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
