/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/DaylightingDeviceTubular.hpp"
#include "../../model/SubSurface.hpp"
#include "../../model/ConstructionBase.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/DaylightingDevice_Tubular_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateDaylightingDeviceTubular(model::DaylightingDeviceTubular& modelObject) {
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::DaylightingDevice_Tubular, modelObject);

    SubSurface subSurfaceDome = modelObject.subSurfaceDome();
    if (boost::optional<IdfObject> subSurfDome = translateAndMapModelObject(subSurfaceDome)) {
      idfObject.setString(DaylightingDevice_TubularFields::DomeName, subSurfDome->name().get());
    }

    SubSurface subSurfaceDiffuser = modelObject.subSurfaceDiffuser();
    if (boost::optional<IdfObject> subSurfDiffuser = translateAndMapModelObject(subSurfaceDiffuser)) {
      idfObject.setString(DaylightingDevice_TubularFields::DiffuserName, subSurfDiffuser->name().get());
    }

    ConstructionBase construction = modelObject.construction();
    idfObject.setString(DaylightingDevice_TubularFields::ConstructionName, construction.name().get());

    idfObject.setDouble(DaylightingDevice_TubularFields::Diameter, modelObject.diameter());

    idfObject.setDouble(DaylightingDevice_TubularFields::TotalLength, modelObject.totalLength());

    idfObject.setDouble(DaylightingDevice_TubularFields::EffectiveThermalResistance, modelObject.effectiveThermalResistance());

    std::vector<TransitionZone> transitionZones = modelObject.transitionZones();
    if (!transitionZones.empty()) {
      for (const TransitionZone& transitionZone : transitionZones) {
        auto eg = idfObject.pushExtensibleGroup();
        eg.setString(DaylightingDevice_TubularExtensibleFields::TransitionZoneName, transitionZone.thermalZone().name().get());
        eg.setDouble(DaylightingDevice_TubularExtensibleFields::TransitionZoneLength, transitionZone.length());
      }
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
