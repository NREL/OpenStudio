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

#include "../ForwardTranslator.hpp"
#include "../../model/AvailabilityManagerOptimumStart.hpp"
#include "../../model/AvailabilityManagerOptimumStart_Impl.hpp"
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/FanConstantVolume.hpp"
#include "../../model/FanConstantVolume_Impl.hpp"
#include "../../model/FanVariableVolume.hpp"
#include "../../model/FanVariableVolume_Impl.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVAC_Impl.hpp"
#include "../../utilities/core/Assert.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include <utilities/idd/AvailabilityManager_OptimumStart_FieldEnums.hxx>
#include <utilities/idd/ZoneList_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateAvailabilityManagerOptimumStart(AvailabilityManagerOptimumStart& modelObject) {
    IdfObject idfObject(IddObjectType::AvailabilityManager_OptimumStart);
    m_idfObjects.push_back(idfObject);

    boost::optional<AirLoopHVAC> airLoopHVAC;
    if (auto loop = modelObject.loop()) {
      airLoopHVAC = loop->optionalCast<model::AirLoopHVAC>();
    }

    // Name
    if (auto s = modelObject.name()) {
      idfObject.setName(*s);
    }

    // ApplicabilityScheduleName
    {
      auto schedule = modelObject.applicabilitySchedule();
      idfObject.setString(AvailabilityManager_OptimumStartFields::ApplicabilityScheduleName, schedule.name().get());
    }

    // FanScheduleName
    if (airLoopHVAC) {
      // Fan schedules are set to match the availabilitySchedule in the translator
      idfObject.setString(AvailabilityManager_OptimumStartFields::FanScheduleName, airLoopHVAC->availabilitySchedule().name().get());
    }

    // ControlType
    auto controlType = modelObject.controlType();
    idfObject.setString(AvailabilityManager_OptimumStartFields::ControlType, controlType);

    if (istringEqual(controlType, "ControlZone")) {
      if (auto zone = modelObject.controlZone()) {
        idfObject.setString(AvailabilityManager_OptimumStartFields::ControlZoneName, zone->name().get());
      }
    } else if (istringEqual(controlType, "MaximumofZoneList")) {
      if (airLoopHVAC) {
        IdfObject zoneList(IddObjectType::ZoneList);
        auto zoneListName = modelObject.name().get() + " Control Zones List";
        zoneList.setName(zoneListName);
        m_idfObjects.push_back(zoneList);
        for (const auto& zone : airLoopHVAC->thermalZones()) {
          auto eg = zoneList.pushExtensibleGroup();
          eg.setString(ZoneListExtensibleFields::ZoneName, zone.name().get());
        }
        idfObject.setString(AvailabilityManager_OptimumStartFields::ZoneListName, zoneListName);
      }
    }

    // MaximumValueforOptimumStartTime
    {
      auto value = modelObject.maximumValueforOptimumStartTime();
      idfObject.setDouble(AvailabilityManager_OptimumStartFields::MaximumValueforOptimumStartTime, value);
    }

    // ControlAlgorithm
    {
      auto value = modelObject.controlAlgorithm();
      idfObject.setString(AvailabilityManager_OptimumStartFields::ControlAlgorithm, value);
    }

    // ConstantTemperatureGradientduringCooling
    {
      auto value = modelObject.constantTemperatureGradientduringCooling();
      idfObject.setDouble(AvailabilityManager_OptimumStartFields::ConstantTemperatureGradientduringCooling, value);
    }

    // ConstantTemperatureGradientduringHeating
    {
      auto value = modelObject.constantTemperatureGradientduringHeating();
      idfObject.setDouble(AvailabilityManager_OptimumStartFields::ConstantTemperatureGradientduringHeating, value);
    }

    // InitialTemperatureGradientduringCooling
    {
      auto value = modelObject.initialTemperatureGradientduringCooling();
      idfObject.setDouble(AvailabilityManager_OptimumStartFields::InitialTemperatureGradientduringCooling, value);
    }

    // InitialTemperatureGradientduringHeating
    {
      auto value = modelObject.initialTemperatureGradientduringHeating();
      idfObject.setDouble(AvailabilityManager_OptimumStartFields::InitialTemperatureGradientduringHeating, value);
    }

    // ConstantStartTime
    {
      auto value = modelObject.constantStartTime();
      idfObject.setDouble(AvailabilityManager_OptimumStartFields::ConstantStartTime, value);
    }

    // NumberofPreviousDays
    {
      auto value = modelObject.numberofPreviousDays();
      idfObject.setUnsigned(AvailabilityManager_OptimumStartFields::NumberofPreviousDays, value);
    }

    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio
