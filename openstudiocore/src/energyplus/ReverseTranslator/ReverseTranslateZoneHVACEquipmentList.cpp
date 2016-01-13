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

#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/ZoneHVACComponent.hpp"
#include "../../model/ZoneHVACComponent_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/ZoneHVAC_EquipmentConnections_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_EquipmentList_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateZoneHVACEquipmentList( const WorkspaceObject & workspaceObject )
{
  if(workspaceObject.iddObject().type() != IddObjectType::ZoneHVAC_EquipmentList){
    LOG(Error, "WorkspaceObject is not IddObjectType: ZoneHVAC:EquipmentList");
     return boost::none;
  }

  boost::optional<openstudio::model::ThermalZone> thermalZone;

  std::vector<WorkspaceObject> zoneHVACEquipmentConnections = workspaceObject.getSources(IddObjectType::ZoneHVAC_EquipmentConnections);
  if (zoneHVACEquipmentConnections.size() == 0){
    LOG(Error,"No ZoneHVAC:EquipmentConnections object associated with a zone. Check that IDF file is correct.");
    return boost::none;
  }else if (zoneHVACEquipmentConnections.size() > 1){
    LOG(Error,"More than 1 ZoneHVAC:EquipmentConnections objects associated with a zone. Check that IDF file is correct.");
    return boost::none;
  }

  for( const auto & zoneHVACEquipmentConnection : zoneHVACEquipmentConnections )
  {
    if( boost::optional<std::string> name = zoneHVACEquipmentConnection.getString(ZoneHVAC_EquipmentConnectionsFields::ZoneName) )
    {
      boost::optional<model::Space> space = m_model.getModelObjectByName<model::Space>(name.get());

      if( space )
      {
        thermalZone = space->thermalZone();
      }
    }
  }

  // get extensible groups for zone HVAC equipment list
  std::vector<IdfExtensibleGroup> extensibleGroups = workspaceObject.extensibleGroups();

  // loop over extensible groups
  unsigned n = extensibleGroups.size();
  for (unsigned i = 0; i < n; ++i){
    // define variables
    boost::optional<openstudio::model::ZoneHVACComponent> zoneHVACComponent;
    // get zone equipment object type and zone equipment object name from extensible group
    boost::optional<std::string> zoneEquipmentObjectType = extensibleGroups[i].getString(ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentObjectType);
    boost::optional<std::string> zoneEquipmentName = extensibleGroups[i].getString(ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentName);
    // get zone equipment workspace object by type and name
    if (zoneEquipmentObjectType && zoneEquipmentName){
      OptionalWorkspaceObject zoneEquipmentWorkspaceObject = workspaceObject.workspace().getObjectByTypeAndName(IddObjectType(*zoneEquipmentObjectType),*zoneEquipmentName);
      // translate zone equipment workspace object
      if (zoneEquipmentWorkspaceObject){
        OptionalModelObject zoneEquipmentModelObject = translateAndMapWorkspaceObject(*zoneEquipmentWorkspaceObject);
        // cast zone equipment model object to zone HVAC component
        if (zoneEquipmentModelObject){
          zoneHVACComponent = zoneEquipmentModelObject->optionalCast<ZoneHVACComponent>();
        }
      }
    }
    // add to thermal zone
    if (zoneHVACComponent && thermalZone){
      zoneHVACComponent->addToThermalZone(*thermalZone);
    }
  }

  return boost::none;
}

} // energyplus

} // openstudio
