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
