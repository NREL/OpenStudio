/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

    if (m_excludeSpaceTranslation) {
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
    if (modelObject.getDouble(OS_SpaceFields::CeilingHeight)) {
      idfObject.setDouble(SpaceFields::CeilingHeight, modelObject.getDouble(SpaceFields::CeilingHeight).get());
    }

    // Volume
    if (modelObject.getDouble(OS_SpaceFields::Volume)) {
      idfObject.setDouble(SpaceFields::Volume, modelObject.getDouble(OS_SpaceFields::Volume).get());
    }

    // FloorArea
    if (modelObject.getDouble(OS_SpaceFields::FloorArea)) {
      idfObject.setDouble(SpaceFields::FloorArea, modelObject.getDouble(OS_SpaceFields::FloorArea).get());
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
