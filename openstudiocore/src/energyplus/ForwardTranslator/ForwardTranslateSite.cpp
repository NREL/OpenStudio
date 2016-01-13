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

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/Site.hpp"
#include "../../model/Site_Impl.hpp"
#include "../../model/ShadingSurfaceGroup.hpp"

#include <utilities/idd/Site_Location_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateSite( Site & modelObject )
{
  IdfObject site = createAndRegisterIdfObject(openstudio::IddObjectType::Site_Location,
                                              modelObject);

  OptionalString optS = modelObject.name();
  if(optS) {
    site.setName(*optS);
  }

  OptionalDouble od = modelObject.latitude();
  if(od) {
    site.setDouble(Site_LocationFields::Latitude, *od);
  }

  od = modelObject.longitude();
  if(od) {
    site.setDouble(Site_LocationFields::Longitude, *od);
  }

  od = modelObject.timeZone();
  if(od) {
    site.setDouble(Site_LocationFields::TimeZone, *od);
  }

  od = modelObject.elevation();
  if(od) {
    site.setDouble(Site_LocationFields::Elevation, *od);
  }

  // translate shading groups
  ShadingSurfaceGroupVector shadingSurfaceGroups = modelObject.shadingSurfaceGroups();
  std::sort(shadingSurfaceGroups.begin(), shadingSurfaceGroups.end(), WorkspaceObjectNameLess());
  for (ShadingSurfaceGroup& shadingSurfaceGroup : shadingSurfaceGroups){
    translateAndMapModelObject(shadingSurfaceGroup);
  }

  return boost::optional<IdfObject>(site);
}

} // energyplus

} // openstudio

