/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "../ReverseTranslator.hpp"
#include "../GeometryTranslator.hpp"

#include "../../model/ShadingSurface.hpp"
#include "../../model/ShadingSurface_Impl.hpp"
#include "../../model/ShadingSurfaceGroup.hpp"
#include "../../model/ShadingSurfaceGroup_Impl.hpp"
#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/Shading_Zone_Detailed_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateShadingZoneDetailed( const WorkspaceObject & workspaceObject )
{
  if( workspaceObject.iddObject().type() != IddObjectType::Shading_Zone_Detailed ){
    LOG(Error, "WorkspaceObject is not IddObjectType: Shading:Zone:Detailed");
    return boost::none;
  }

  openstudio::Point3dVector vertices = getVertices(Shading_Zone_DetailedFields::NumberofVertices + 1, workspaceObject);

  boost::optional<ShadingSurface> shadingSurface;
  try{
    shadingSurface = ShadingSurface(vertices, m_model);
  }catch(const std::exception&){
    LOG(Error, "Cannot create ShadingSurface for object: " << workspaceObject);
    return boost::none;
  }

  OptionalString s = workspaceObject.name();
  if (s) {
    shadingSurface->setName(*s);
  }

  OptionalWorkspaceObject target = workspaceObject.getTarget(openstudio::Shading_Zone_DetailedFields::BaseSurfaceName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (modelObject->optionalCast<Surface>()){
        OptionalSpace space = modelObject->cast<Surface>().space();
        if (space){
          ShadingSurfaceGroupVector groups = space->shadingSurfaceGroups();
          if (groups.empty()){
            ShadingSurfaceGroup shadingSurfaceGroup(m_model);
            shadingSurfaceGroup.setSpace(*space);
            groups.push_back(shadingSurfaceGroup);
          }
          shadingSurface->setShadingSurfaceGroup(groups[0]);
        }
      }
    }
  }

  target = workspaceObject.getTarget(openstudio::Shading_Zone_DetailedFields::TransmittanceScheduleName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (OptionalSchedule intermediate = modelObject->optionalCast<Schedule>()){
        Schedule schedule(*intermediate);
        shadingSurface->setTransmittanceSchedule(schedule);
      }
    }
  }

  return shadingSurface.get();
}

} // energyplus

} // openstudio

