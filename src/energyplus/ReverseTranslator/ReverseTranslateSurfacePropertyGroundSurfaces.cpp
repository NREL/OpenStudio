/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
