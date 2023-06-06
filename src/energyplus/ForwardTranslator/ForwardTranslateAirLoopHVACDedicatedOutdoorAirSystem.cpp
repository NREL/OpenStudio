/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/AirLoopHVACDedicatedOutdoorAirSystem.hpp"
#include "../../model/AirLoopHVACDedicatedOutdoorAirSystem_Impl.hpp"
#include "../../model/AirLoopHVACOutdoorAirSystem.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/AirLoopHVACZoneMixer.hpp"
#include "../../model/AirLoopHVACZoneSplitter.hpp"
#include "../../model/HeatExchangerAirToAirSensibleAndLatent.hpp"
#include "../../model/HeatExchangerAirToAirSensibleAndLatent_Impl.hpp"

#include <utilities/idd/AirLoopHVAC_DedicatedOutdoorAirSystem_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_Mixer_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_Splitter_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateAirLoopHVACDedicatedOutdoorAirSystem(model::AirLoopHVACDedicatedOutdoorAirSystem& modelObject) {
    boost::optional<double> value;
    IdfObject idfObject(IddObjectType::AirLoopHVAC_DedicatedOutdoorAirSystem);

    m_idfObjects.push_back(idfObject);

    // Name
    std::string name = modelObject.name().get();
    idfObject.setString(openstudio::AirLoopHVAC_DedicatedOutdoorAirSystemFields::Name, name);

    // Availability Schedule Name
    Schedule schedule = modelObject.availabilitySchedule();
    if (boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule)) {
      idfObject.setString(AirLoopHVAC_DedicatedOutdoorAirSystemFields::AvailabilityScheduleName, _schedule->name().get());
    }

    // AirLoopHVAC:OutdoorAirSystem Name
    AirLoopHVACOutdoorAirSystem oaSystem = modelObject.airLoopHVACOutdoorAirSystem();
    if (boost::optional<IdfObject> _oaSystem = translateAndMapModelObject(oaSystem)) {
      idfObject.setString(AirLoopHVAC_DedicatedOutdoorAirSystemFields::AirLoopHVAC_OutdoorAirSystemName, _oaSystem->name().get());
    }

    // AirLoopHVAC:Mixer Name
    std::string mixerName(modelObject.nameString() + " Mixer");
    idfObject.setString(AirLoopHVAC_DedicatedOutdoorAirSystemFields::AirLoopHVAC_MixerName, mixerName);
    IdfObject idfMixer(openstudio::IddObjectType::AirLoopHVAC_Mixer);
    idfMixer.setString(AirLoopHVAC_MixerFields::Name, mixerName);

    boost::optional<std::string> outletNodeName;

    for (const auto& oaComponent : oaSystem.oaComponents()) {
      if (boost::optional<HeatExchangerAirToAirSensibleAndLatent> hx = oaComponent.optionalCast<HeatExchangerAirToAirSensibleAndLatent>()) {
        if (boost::optional<ModelObject> secondaryAirInletModelObject = hx->secondaryAirInletModelObject()) {
          outletNodeName = secondaryAirInletModelObject->name();
        }
      }
    }

    if (!outletNodeName) {
      outletNodeName = idfMixer.name().get() + " Outlet";
    }

    idfMixer.setString(AirLoopHVAC_MixerFields::OutletNodeName, outletNodeName.get());

    // AirLoopHVAC:Splitter Name
    std::string splitterName(modelObject.nameString() + " Splitter");
    idfObject.setString(AirLoopHVAC_DedicatedOutdoorAirSystemFields::AirLoopHVAC_SplitterName, splitterName);
    IdfObject idfSplitter(openstudio::IddObjectType::AirLoopHVAC_Splitter);
    idfSplitter.setString(AirLoopHVAC_SplitterFields::Name, splitterName);
    idfSplitter.setString(AirLoopHVAC_SplitterFields::InletNodeName, oaSystem.outdoorAirModelObject().get().nameString());

    // Preheat Design Temperature
    if ((value = modelObject.preheatDesignTemperature())) {
      idfObject.setDouble(AirLoopHVAC_DedicatedOutdoorAirSystemFields::PreheatDesignTemperature, value.get());
    }

    // Preheat Design Humidity Ratio
    if ((value = modelObject.preheatDesignHumidityRatio())) {
      idfObject.setDouble(AirLoopHVAC_DedicatedOutdoorAirSystemFields::PreheatDesignHumidityRatio, value.get());
    }

    // Precool Design Temperature
    if ((value = modelObject.precoolDesignTemperature())) {
      idfObject.setDouble(AirLoopHVAC_DedicatedOutdoorAirSystemFields::PrecoolDesignTemperature, value.get());
    }

    // Precool Design Humidity Ratio
    if ((value = modelObject.precoolDesignHumidityRatio())) {
      idfObject.setDouble(AirLoopHVAC_DedicatedOutdoorAirSystemFields::PrecoolDesignHumidityRatio, value.get());
    }

    // Number of AirLoopHVAC
    idfObject.setInt(AirLoopHVAC_DedicatedOutdoorAirSystemFields::NumberofAirLoopHVAC, modelObject.numberofAirLoops());

    // AirLoopHVAC x Name
    boost::optional<AirLoopHVACOutdoorAirSystem> oas;
    for (const auto& airLoop : modelObject.airLoops()) {
      auto eg = idfObject.pushExtensibleGroup();
      eg.setString(AirLoopHVAC_DedicatedOutdoorAirSystemExtensibleFields::AirLoopHVACName, airLoop.nameString());

      oas = airLoop.airLoopHVACOutdoorAirSystem();
      OS_ASSERT(oas);

      // AirLoopHVAC:Mixer Name
      auto egMixer = idfMixer.pushExtensibleGroup();
      egMixer.setString(AirLoopHVAC_MixerExtensibleFields::InletNodeName, oas->outboardReliefNode().get().nameString());

      // AirLoopHVAC:Splitter Name
      auto egSplitter = idfSplitter.pushExtensibleGroup();
      egSplitter.setString(AirLoopHVAC_SplitterExtensibleFields::OutletNodeName, oas->outboardOANode().get().nameString());
    }

    m_idfObjects.push_back(idfMixer);
    m_idfObjects.push_back(idfSplitter);

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
