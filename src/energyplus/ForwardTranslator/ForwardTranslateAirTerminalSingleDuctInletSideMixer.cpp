/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

// copy from AirTerminalSingleDuctVAVReheat
#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/AirTerminalSingleDuctInletSideMixer.hpp"
#include "../../model/AirTerminalSingleDuctInletSideMixer_Impl.hpp"
#include "../../model/HVACComponent.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/ZoneHVACComponent.hpp"
#include "../../model/ZoneHVACComponent_Impl.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/DesignSpecificationOutdoorAir.hpp"
#include "../../model/Space.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"

#include <utilities/idd/AirTerminal_SingleDuct_Mixer_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_AirDistributionUnit_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateAirTerminalSingleDuctInletSideMixer(AirTerminalSingleDuctInletSideMixer& modelObject) {
    OptionalModelObject temp;
    OptionalString optS;
    boost::optional<std::string> s;

    std::string baseName = modelObject.name().get();

    IdfObject _airDistributionUnit(openstudio::IddObjectType::ZoneHVAC_AirDistributionUnit);
    _airDistributionUnit.setName("ADU " + baseName);  //ADU: Air Distribution Unit

    IdfObject idfObject(openstudio::IddObjectType::AirTerminal_SingleDuct_Mixer);

    idfObject.setName(baseName);

    m_idfObjects.push_back(_airDistributionUnit);

    m_idfObjects.push_back(idfObject);

    boost::optional<std::string> inletNodeName;
    boost::optional<std::string> outletNodeName;

    if (boost::optional<ModelObject> inletModelObject = modelObject.inletModelObject()) {
      if (boost::optional<Node> inletNode = inletModelObject->optionalCast<Node>()) {
        inletNodeName = inletNode->name().get();
      }
    }

    if (boost::optional<ModelObject> outletModelObject = modelObject.outletModelObject()) {
      if (boost::optional<Node> outletNode = outletModelObject->optionalCast<Node>()) {
        outletNodeName = outletNode->name().get();
      }
    }

    if (outletNodeName && inletNodeName) {
      idfObject.setString(AirTerminal_SingleDuct_MixerFields::MixerOutletNodeName, outletNodeName.get());
      idfObject.setString(AirTerminal_SingleDuct_MixerFields::MixerPrimaryAirInletNodeName, inletNodeName.get());
    }

    if (boost::optional<AirLoopHVAC> airLoopHVAC = modelObject.airLoopHVAC()) {
      std::vector<ZoneHVACComponent> zoneHVACs =
        subsetCastVector<ZoneHVACComponent>(airLoopHVAC->demandComponents(modelObject, airLoopHVAC->demandOutletNode()));
      if (!zoneHVACs.empty()) {
        ZoneHVACComponent zoneHVAC = zoneHVACs.front();
        if (boost::optional<IdfObject> _zoneHVAC = translateAndMapModelObject(zoneHVAC)) {
          idfObject.setString(AirTerminal_SingleDuct_MixerFields::ZoneHVACUnitObjectType, _zoneHVAC->iddObject().name());
          idfObject.setString(AirTerminal_SingleDuct_MixerFields::ZoneHVACUnitObjectName, _zoneHVAC->name().get());
        }
      }
    }

    idfObject.setString(AirTerminal_SingleDuct_MixerFields::MixerConnectionType, "InletSide");

    // TerminalUnitSecondaryAirInletNodeName
    if (boost::optional<Node> secondaryInletNode = modelObject.secondaryAirInletNode()) {
      idfObject.setString(AirTerminal_SingleDuct_MixerFields::MixerSecondaryAirInletNodeName, secondaryInletNode->name().get());
    }

    // ControlForOutdoorAir
    {
      if (modelObject.controlForOutdoorAir()) {
        bool dsoa_found = false;
        if (auto airLoopHVAC = modelObject.airLoopHVAC()) {
          auto zones = airLoopHVAC->demandComponents(modelObject, airLoopHVAC->demandOutletNode(), model::ThermalZone::iddObjectType());
          if (!zones.empty()) {
            auto zone = zones.front();
            auto spaces = zone.cast<model::ThermalZone>().spaces();
            if (!spaces.empty()) {
              if (auto designSpecificationOutdoorAir = spaces.front().designSpecificationOutdoorAir()) {
                dsoa_found = true;
                idfObject.setString(AirTerminal_SingleDuct_MixerFields::DesignSpecificationOutdoorAirObjectName,
                                    designSpecificationOutdoorAir->name().get());
              }
            }
          }
        }
        if (!dsoa_found) {
          LOG(Warn, "Cannot set the 'Design Specification Outdoor Air' (DSOA) object for "
                      << modelObject.briefDescription()
                      << ". No DSOA was found but 'Control for Outdoor' was set to 'Yes'. Ensure the zone's Space/SpaceType has a DSOA attached.");
        }
      }
    }

    {
      auto value = modelObject.perPersonVentilationRateMode();
      idfObject.setString(AirTerminal_SingleDuct_MixerFields::PerPersonVentilationRateMode, value);
    }

    // Populate fields for AirDistributionUnit
    if (boost::optional<ModelObject> outletNode = modelObject.outletModelObject()) {
      _airDistributionUnit.setString(ZoneHVAC_AirDistributionUnitFields::AirDistributionUnitOutletNodeName, outletNode->name().get());
    }
    _airDistributionUnit.setString(ZoneHVAC_AirDistributionUnitFields::AirTerminalObjectType, idfObject.iddObject().name());
    _airDistributionUnit.setString(ZoneHVAC_AirDistributionUnitFields::AirTerminalName, idfObject.name().get());

    return _airDistributionUnit;
  }

}  // namespace energyplus

}  // namespace openstudio
