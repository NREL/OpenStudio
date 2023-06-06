/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/AirLoopHVACSupplyPlenum.hpp"
#include "../../model/AirLoopHVACSupplyPlenum_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/AirTerminalSingleDuctConstantVolumeNoReheat.hpp"
#include "../../model/AirTerminalSingleDuctConstantVolumeNoReheat_Impl.hpp"
#include <utilities/idd/AirLoopHVAC_SupplyPlenum_FieldEnums.hxx>
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateAirLoopHVACSupplyPlenum(AirLoopHVACSupplyPlenum& modelObject) {
    OptionalModelObject temp;
    OptionalString optS;
    std::string s;

    IdfObject idfObject(openstudio::IddObjectType::AirLoopHVAC_SupplyPlenum);

    m_idfObjects.push_back(idfObject);

    // Name
    s = modelObject.name().get();
    idfObject.setName(s);

    // Inlet Node Name
    if (boost::optional<model::ModelObject> node = modelObject.inletModelObject()) {
      idfObject.setString(openstudio::AirLoopHVAC_SupplyPlenumFields::InletNodeName, node->name().get());
    }

    // ZoneName
    // and
    // ZoneNodeName

    if (boost::optional<model::ThermalZone> zone = modelObject.thermalZone()) {
      if (boost::optional<IdfObject> _zone = translateAndMapModelObject(zone.get())) {
        idfObject.setString(AirLoopHVAC_SupplyPlenumFields::ZoneName, _zone->name().get());

        model::Node node = zone->zoneAirNode();
        idfObject.setString(AirLoopHVAC_SupplyPlenumFields::ZoneNodeName, node.name().get());
      }
    }

    // OutletNodeName

    std::vector<ModelObject> outletModelObjects = modelObject.outletModelObjects();
    for (const auto& outletModelObject : outletModelObjects) {
      if (OptionalAirTerminalSingleDuctConstantVolumeNoReheat airTerminal =
            outletModelObject.optionalCast<AirTerminalSingleDuctConstantVolumeNoReheat>()) {
        if (boost::optional<model::ModelObject> node = airTerminal->outletModelObject()) {
          IdfExtensibleGroup eg = idfObject.pushExtensibleGroup();
          eg.setString(AirLoopHVAC_SupplyPlenumExtensibleFields::OutletNodeName, node->name().get());
        }
      } else {
        IdfExtensibleGroup eg = idfObject.pushExtensibleGroup();
        eg.setString(AirLoopHVAC_SupplyPlenumExtensibleFields::OutletNodeName, outletModelObject.name().get());
      }
    }

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
