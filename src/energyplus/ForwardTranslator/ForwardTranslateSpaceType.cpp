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

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/Space.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/ThermalZone.hpp"
// Loads
#include "../../model/InternalMass.hpp"
#include "../../model/People.hpp"
#include "../../model/Lights.hpp"
#include "../../model/Luminaire.hpp"
#include "../../model/ElectricEquipment.hpp"
#include "../../model/ElectricEquipmentITEAirCooled.hpp"
#include "../../model/GasEquipment.hpp"
#include "../../model/HotWaterEquipment.hpp"
#include "../../model/SteamEquipment.hpp"
#include "../../model/OtherEquipment.hpp"
#include "../../model/SpaceInfiltrationDesignFlowRate.hpp"
#include "../../model/SpaceInfiltrationEffectiveLeakageArea.hpp"
#include "../../model/SpaceInfiltrationFlowCoefficient.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/WorkspaceObjectOrder.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"

#include <iterator>
#include <utilities/idd/ZoneList_FieldEnums.hxx>
#include <utilities/idd/SpaceList_FieldEnums.hxx>

#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  std::string ForwardTranslator::zoneListNameForSpaceType(const SpaceType& modelObject) const {
    if (m_forwardTranslatorOptions.excludeSpaceTranslation()) {
      return modelObject.nameString();
    } else {
      return modelObject.nameString() + " ZoneList";
    }
  }

  boost::optional<IdfObject> ForwardTranslator::translateSpaceType(SpaceType& modelObject) {
    std::vector<Space> spaces = modelObject.spaces();

    // check if this is a dummy space type meant to prevent inheriting building space type
    // TODO: why is that needed in the first place? Also, children() doesn't include DesignSpecificationOutdoorAir!
    if (m_forwardTranslatorOptions.excludeSpaceTranslation()) {
      std::vector<ModelObject> children = modelObject.children();
      if (children.empty()) {
        LOG(Info, "SpaceType " << modelObject.name().get() << " has no children, it will not be translated");
        return boost::none;
      }
    }

    OptionalIdfObject idfObject;

    auto makeZoneList = [this, &modelObject, &spaces](bool registerIt) -> IdfObject {
      boost::optional<IdfObject> idfObject;
      if (registerIt) {
        idfObject = createAndRegisterIdfObject(openstudio::IddObjectType::ZoneList, modelObject);
      } else {
        idfObject = m_idfObjects.emplace_back(openstudio::IddObjectType::ZoneList);
      }
      idfObject->setName(zoneListNameForSpaceType(modelObject));

      // Unique zone names
      std::set<std::string> zoneNames;
      for (const Space& space : spaces) {
        if (auto thermalZone_ = space.thermalZone()) {
          zoneNames.insert(thermalZone_->nameString());
        }
      }

      idfObject->clearExtensibleGroups();
      for (const std::string& zoneName : zoneNames) {
        idfObject->pushExtensibleGroup(std::vector<std::string>(1, zoneName));
      }

      return idfObject.get();
    };

    if (m_forwardTranslatorOptions.excludeSpaceTranslation()) {

      idfObject = makeZoneList(true);

    } else {

      idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::SpaceList, modelObject);

      // Space names are already unique
      idfObject->clearExtensibleGroups();
      for (const auto& s : spaces) {
        idfObject->pushExtensibleGroup(std::vector<std::string>(1, s.nameString()));
      }
    }

    // Translate all SpaceType loads
    auto translateSpaceLoads = [this](auto loads) {
      std::sort(loads.begin(), loads.end(), WorkspaceObjectNameLess());
      for (auto& load : loads) {
        translateAndMapModelObject(load);
      }
    };

    translateSpaceLoads(modelObject.internalMass());
    translateSpaceLoads(modelObject.lights());
    translateSpaceLoads(modelObject.luminaires());
    translateSpaceLoads(modelObject.people());
    translateSpaceLoads(modelObject.electricEquipment());
    translateSpaceLoads(modelObject.electricEquipmentITEAirCooled());
    translateSpaceLoads(modelObject.gasEquipment());
    translateSpaceLoads(modelObject.hotWaterEquipment());
    translateSpaceLoads(modelObject.steamEquipment());
    translateSpaceLoads(modelObject.otherEquipment());

    // in translateModelPrivate, we have hard assigned the infiltration objects to each Space.
    // In E+ these objects are Zone objects, they do not accept Space
    // Technically this one accepts a Zone or ZoneList
    // translateSpaceLoads(modelObject.spaceInfiltrationDesignFlowRates());
    // These two DO NOT ACCEPT a ZoneList, only a Zone
    // translateSpaceLoads(modelObject.spaceInfiltrationEffectiveLeakageAreas());
    // translateSpaceLoads(modelObject.spaceInfiltrationFlowCoefficients());

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
