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

#include "../../model/SurfaceControlMovableInsulation.hpp"
#include "../../model/SurfaceControlMovableInsulation_Impl.hpp"
#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"
#include "../../model/Material.hpp"
#include "../../model/Material_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/SurfaceControl_MovableInsulation_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateSurfaceControlMovableInsulation(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::SurfaceControl_MovableInsulation) {
      LOG(Error, "WorkspaceObject is not IddObjectType: SurfaceControlMovableInsulation");
      return boost::none;
    }

    // Surface and Material are both required fields and not defaulted in the model Ctor, so make sure we can find them
    // before we instantiate a SCMI ModelObject
    OptionalWorkspaceObject target;

    boost::optional<Surface> surface;
    if ((target = workspaceObject.getTarget(openstudio::SurfaceControl_MovableInsulationFields::SurfaceName))) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (modelObject->optionalCast<Surface>()) {
          surface = modelObject->cast<Surface>();
        }
      }
    } else {
      LOG(Error, "Could not find surface attached to SurfaceControlMovableInsulation object");
      return boost::none;
    }

    boost::optional<Material> material;
    if ((target = workspaceObject.getTarget(openstudio::SurfaceControl_MovableInsulationFields::MaterialName))) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (modelObject->optionalCast<Material>()) {
          material = modelObject->cast<Material>();
        }
      }
    } else {
      LOG(Error, "Could not find material for SurfaceControlMovableInsulation object attached to " << surface->briefDescription());
      return boost::none;
    }

    SurfaceControlMovableInsulation surfaceControlMovableInsulation(*surface, *material);

    // Required-fields but defaulted in model Ctor: Log an Info if not found, but continue processing
    if (boost::optional<std::string> insulationType = workspaceObject.getString(openstudio::SurfaceControl_MovableInsulationFields::InsulationType)) {
      surfaceControlMovableInsulation.setInsulationType(*insulationType);
    } else {
      LOG(Info, "Defaulting Insulation Type for SurfaceControlMovableInsulation object attached to " << surface->briefDescription());
    }

    if ((target = workspaceObject.getTarget(openstudio::SurfaceControl_MovableInsulationFields::ScheduleName))) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject && modelObject->optionalCast<Schedule>()) {
        Schedule schedule = modelObject->cast<Schedule>();
        surfaceControlMovableInsulation.setSchedule(schedule);
      }
    } else {
      LOG(Info, "Defaulting Schedule Name for SurfaceControlMovableInsulation object attached to " << surface->briefDescription());
    }

    return surfaceControlMovableInsulation;
  }

}  // namespace energyplus

}  // namespace openstudio
