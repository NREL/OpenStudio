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

#include "../../model/Site.hpp"
#include "../../model/Site_Impl.hpp"

#include <utilities/idd/Site_Location_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateSiteLocation( const WorkspaceObject & workspaceObject )
{
 if( workspaceObject.iddObject().type() != IddObjectType::Site_Location ){
   LOG(Error, "WorkspaceObject is not IddObjectType: Site:Location");
    return boost::none;
  }

  Site site = m_model.getUniqueModelObject<Site>();
  OptionalString optS = workspaceObject.name();
  if(optS) {
    site.setName(*optS);
  }

  OptionalDouble od = workspaceObject.getDouble(Site_LocationFields::Latitude);
  if(od) {
    site.setLatitude(*od);
  }

  od = workspaceObject.getDouble(Site_LocationFields::Longitude);
  if(od) {
    site.setLongitude(*od);
  }

  od = workspaceObject.getDouble(Site_LocationFields::TimeZone);
  if(od) {
    site.setTimeZone(*od);
  }

  od = workspaceObject.getDouble(Site_LocationFields::Elevation);
  if(od) {
    site.setElevation(*od);
  }

  return site;
}

} // energyplus

} // openstudio

