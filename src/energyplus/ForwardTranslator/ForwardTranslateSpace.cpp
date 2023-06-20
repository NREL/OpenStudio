/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/LifeCycleCost.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/SpaceType.hpp"

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
// Geometry children
#include "../../model/ShadingSurfaceGroup.hpp"
#include "../../model/InteriorPartitionSurfaceGroup.hpp"
#include "../../model/Surface.hpp"

#include <utilities/idd/OS_Space_FieldEnums.hxx>
#include <utilities/idd/Space_FieldEnums.hxx>

#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateSpace(model::Space& modelObject) {

    for (LifeCycleCost& lifeCycleCost : modelObject.lifeCycleCosts()) {
      translateAndMapModelObject(lifeCycleCost);
    }

    if (m_forwardTranslatorOptions.excludeSpaceTranslation()) {
      return boost::none;
    }

    // Space
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::Space, modelObject);

    // ZoneName
    if (boost::optional<ThermalZone> thermalZone = modelObject.thermalZone()) {
      idfObject.setString(SpaceFields::ZoneName, thermalZone->name().get());
    }

    // for CeilingHeight, Volume, FloorArea: only FT if hard set (same logic as for ThermalZone)

    // CeilingHeight
    if (!modelObject.isCeilingHeightDefaulted()) {
      idfObject.setDouble(SpaceFields::CeilingHeight, modelObject.ceilingHeight());
    }

    // Volume
    if (!modelObject.isVolumeDefaulted()) {
      idfObject.setDouble(SpaceFields::Volume, modelObject.volume());
    }

    // FloorArea
    if (!modelObject.isFloorAreaDefaulted()) {
      idfObject.setDouble(SpaceFields::FloorArea, modelObject.floorArea());
    }

    // SpaceType
    if (boost::optional<SpaceType> spaceType_ = modelObject.spaceType()) {
      if (auto idf_spaceType_ = translateAndMapModelObject(spaceType_.get())) {
        idfObject.setString(SpaceFields::SpaceType, idf_spaceType_->nameString());
      }
    }

    // Translate all Space-specific loads (and geometry children)
    auto translateSpaceLoads = [this](auto loads) {
      std::sort(loads.begin(), loads.end(), WorkspaceObjectNameLess());
      for (auto& load : loads) {
        translateAndMapModelObject(load);
      }
    };

    translateSpaceLoads(modelObject.shadingSurfaceGroups());
    translateSpaceLoads(modelObject.interiorPartitionSurfaceGroups());
    translateSpaceLoads(modelObject.surfaces());

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

    // TODO: Technically this stuff maps to a thermal zone, always (can't map to a Space/SpaceList)
    translateSpaceLoads(modelObject.spaceInfiltrationDesignFlowRates());
    translateSpaceLoads(modelObject.spaceInfiltrationEffectiveLeakageAreas());
    translateSpaceLoads(modelObject.spaceInfiltrationFlowCoefficients());

    return idfObject;
  }  // translate function

}  // namespace energyplus
}  // namespace openstudio
