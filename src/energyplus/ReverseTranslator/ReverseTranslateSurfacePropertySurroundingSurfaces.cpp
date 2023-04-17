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
