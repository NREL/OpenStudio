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

  OptionalModelObject ReverseTranslator::translateZoneHVACEquipmentList(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::ZoneHVAC_EquipmentList) {
      LOG(Error, "WorkspaceObject is not IddObjectType: ZoneHVAC:EquipmentList");
      return boost::none;
    }

    boost::optional<openstudio::model::ThermalZone> thermalZone;

    std::vector<WorkspaceObject> zoneHVACEquipmentConnections = workspaceObject.getSources(IddObjectType::ZoneHVAC_EquipmentConnections);
    if (zoneHVACEquipmentConnections.size() == 0) {
      LOG(Error, "No ZoneHVAC:EquipmentConnections object associated with a zone. Check that IDF file is correct.");
      return boost::none;
    } else if (zoneHVACEquipmentConnections.size() > 1) {
      LOG(Error, "More than 1 ZoneHVAC:EquipmentConnections objects associated with a zone. Check that IDF file is correct.");
      return boost::none;
    }

    for (const auto& zoneHVACEquipmentConnection : zoneHVACEquipmentConnections) {
      if (boost::optional<std::string> name = zoneHVACEquipmentConnection.getString(ZoneHVAC_EquipmentConnectionsFields::ZoneName)) {
        boost::optional<model::Space> space = m_model.getModelObjectByName<model::Space>(name.get());

        if (space) {
          thermalZone = space->thermalZone();
        }
      }
    }

    // get extensible groups for zone HVAC equipment list
    std::vector<IdfExtensibleGroup> extensibleGroups = workspaceObject.extensibleGroups();

    // loop over extensible groups
    unsigned n = extensibleGroups.size();
    for (unsigned i = 0; i < n; ++i) {
      // define variables
      boost::optional<openstudio::model::ZoneHVACComponent> zoneHVACComponent;
      // get zone equipment object type and zone equipment object name from extensible group
      boost::optional<std::string> zoneEquipmentObjectType =
        extensibleGroups[i].getString(ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentObjectType);
      boost::optional<std::string> zoneEquipmentName = extensibleGroups[i].getString(ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentName);
      // get zone equipment workspace object by type and name
      if (zoneEquipmentObjectType && zoneEquipmentName) {
        OptionalWorkspaceObject zoneEquipmentWorkspaceObject =
          workspaceObject.workspace().getObjectByTypeAndName(IddObjectType(*zoneEquipmentObjectType), *zoneEquipmentName);
        // translate zone equipment workspace object
        if (zoneEquipmentWorkspaceObject) {
          OptionalModelObject zoneEquipmentModelObject = translateAndMapWorkspaceObject(*zoneEquipmentWorkspaceObject);
          // cast zone equipment model object to zone HVAC component
          if (zoneEquipmentModelObject) {
            zoneHVACComponent = zoneEquipmentModelObject->optionalCast<ZoneHVACComponent>();
          }
        }
      }
      // add to thermal zone
      if (zoneHVACComponent && thermalZone) {
        zoneHVACComponent->addToThermalZone(*thermalZone);
      }
    }

    return boost::none;
  }

}  // namespace energyplus

}  // namespace openstudio
