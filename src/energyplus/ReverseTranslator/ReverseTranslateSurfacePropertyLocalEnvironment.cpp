/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
