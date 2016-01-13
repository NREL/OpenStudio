/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#include "../ReverseTranslator.hpp"
#include "../GeometryTranslator.hpp"

#include "../../model/ShadingSurface.hpp"
#include "../../model/ShadingSurface_Impl.hpp"
#include "../../model/ShadingSurfaceGroup.hpp"
#include "../../model/ShadingSurfaceGroup_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/Shading_Building_Detailed_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateShadingBuildingDetailed( const WorkspaceObject & workspaceObject )
{
  if( workspaceObject.iddObject().type() != IddObjectType::Shading_Building_Detailed ){
    LOG(Error, "WorkspaceObject is not IddObjectType: Shading:Building:Detailed");
    return boost::none;
  }

  openstudio::Point3dVector vertices = getVertices(Shading_Building_DetailedFields::NumberofVertices + 1, workspaceObject);
 
  boost::optional<ShadingSurface> shadingSurface;
  try{
    shadingSurface = ShadingSurface(vertices, m_model);
  }catch(const std::exception&){
    LOG(Error, "Cannot create ShadingSurface for object: " << workspaceObject);
    return boost::none;
  }

  OptionalString s = workspaceObject.name();
  if(s) {
    shadingSurface->setName(*s);
  }

  // look for first site level shading surface group
  OptionalShadingSurfaceGroup shadingSurfaceGroup;
  for (const ShadingSurfaceGroup& group : m_model.getConcreteModelObjects<ShadingSurfaceGroup>()){
    if (istringEqual("Building", group.shadingSurfaceType())){
      shadingSurfaceGroup = group;
      break;
    }
  }

  // if not found make one
  if (!shadingSurfaceGroup){
    shadingSurfaceGroup = ShadingSurfaceGroup(m_model);
    shadingSurfaceGroup->setShadingSurfaceType("Building");
  }

  shadingSurface->setShadingSurfaceGroup(*shadingSurfaceGroup);

  OptionalWorkspaceObject target = workspaceObject.getTarget(openstudio::Shading_Building_DetailedFields::TransmittanceScheduleName);
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

