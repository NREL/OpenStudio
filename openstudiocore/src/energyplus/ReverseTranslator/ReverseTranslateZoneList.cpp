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

#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"

#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/ZoneList_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateZoneList( const WorkspaceObject & workspaceObject )
{
   if( workspaceObject.iddObject().type() != IddObjectType::Zone ){
    LOG(Error, "WorkspaceObject is not IddObjectType: Zone");
    return boost::none;
  }

  openstudio::model::SpaceType spaceType( m_model );

  OptionalString s = workspaceObject.name();
  if(s){
    spaceType.setName(*s);
  }

  for (const IdfExtensibleGroup& idfGroup : workspaceObject.extensibleGroups()){
    WorkspaceExtensibleGroup workspaceGroup = idfGroup.cast<WorkspaceExtensibleGroup>();
    
    OptionalWorkspaceObject target = workspaceGroup.getTarget(0);
    if (target){
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject){
        if (modelObject->optionalCast<Space>()){
          Space space = modelObject->cast<Space>();

          if (space.spaceType()){
            LOG(Warn, "Overriding previously assigned SpaceType for Space '" << space.name().get() << "'");
          }

          space.setSpaceType(spaceType);

        }
      }
    }
  }

  return spaceType;
}

} // energyplus

} // openstudio

