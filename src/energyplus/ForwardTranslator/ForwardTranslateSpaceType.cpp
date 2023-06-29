/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
