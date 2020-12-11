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

#include "WaterHeaterHeatPumpPumpedCondenser.hpp"
#include "WaterHeaterHeatPumpPumpedCondenser_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "HVACComponent.hpp"
#include "HVACComponent_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_WaterHeater_HeatPump_PumpedCondenser_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    WaterHeaterHeatPumpPumpedCondenser_Impl::WaterHeaterHeatPumpPumpedCondenser_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ZoneHVACComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == WaterHeaterHeatPumpPumpedCondenser::iddObjectType());
    }

    WaterHeaterHeatPumpPumpedCondenser_Impl::WaterHeaterHeatPumpPumpedCondenser_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                     Model_Impl* model, bool keepHandle)
      : ZoneHVACComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == WaterHeaterHeatPumpPumpedCondenser::iddObjectType());
    }

    WaterHeaterHeatPumpPumpedCondenser_Impl::WaterHeaterHeatPumpPumpedCondenser_Impl(const WaterHeaterHeatPumpPumpedCondenser_Impl& other,
                                                                                     Model_Impl* model, bool keepHandle)
      : ZoneHVACComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& WaterHeaterHeatPumpPumpedCondenser_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Water Heater Compressor Part Load Ratio", "Water Heater On Cycle Ancillary Electricity Rate",
                                                   "Water Heater On Cycle Ancillary Electricity Energy",
                                                   "Water Heater Off Cycle Ancillary Electricity Rate",
                                                   "Water Heater Off Cycle Ancillary Electricity Energy"};
      return result;
    }

    IddObjectType WaterHeaterHeatPumpPumpedCondenser_Impl::iddObjectType() const {
      return WaterHeaterHeatPumpPumpedCondenser::iddObjectType();
    }

    std::vector<ScheduleTypeKey> WaterHeaterHeatPumpPumpedCondenser_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
      if (std::find(b, e, OS_WaterHeater_HeatPump_PumpedCondenserFields::AvailabilityScheduleName) != e) {
        result.push_back(ScheduleTypeKey("WaterHeaterHeatPumpPumpedCondenser", "Availability"));
      }
      if (std::find(b, e, OS_WaterHeater_HeatPump_PumpedCondenserFields::CompressorSetpointTemperatureScheduleName) != e) {
        result.push_back(ScheduleTypeKey("WaterHeaterHeatPumpPumpedCondenser", "Compressor Setpoint Temperature"));
      }
      if (std::find(b, e, OS_WaterHeater_HeatPump_PumpedCondenserFields::InletAirTemperatureScheduleName) != e) {
        result.push_back(ScheduleTypeKey("WaterHeaterHeatPumpPumpedCondenser", "Inlet Air Temperature"));
      }
      if (std::find(b, e, OS_WaterHeater_HeatPump_PumpedCondenserFields::InletAirHumidityScheduleName) != e) {
        result.push_back(ScheduleTypeKey("WaterHeaterHeatPumpPumpedCondenser", "Inlet Air Humidity"));
      }
      if (std::find(b, e, OS_WaterHeater_HeatPump_PumpedCondenserFields::CompressorAmbientTemperatureScheduleName) != e) {
        result.push_back(ScheduleTypeKey("WaterHeaterHeatPumpPumpedCondenser", "Compressor Ambient Temperature"));
      }
      if (std::find(b, e, OS_WaterHeater_HeatPump_PumpedCondenserFields::InletAirMixerScheduleName) != e) {
        result.push_back(ScheduleTypeKey("WaterHeaterHeatPumpPumpedCondenser", "Inlet Air Mixer"));
      }
      return result;
    }

    boost::optional<Schedule> WaterHeaterHeatPumpPumpedCondenser_Impl::availabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_WaterHeater_HeatPump_PumpedCondenserFields::AvailabilityScheduleName);
    }

    bool WaterHeaterHeatPumpPumpedCondenser_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_WaterHeater_HeatPump_PumpedCondenserFields::AvailabilityScheduleName, "WaterHeaterHeatPumpPumpedCondenser",
                                "Availability", schedule);
      return result;
    }

    void WaterHeaterHeatPumpPumpedCondenser_Impl::resetAvailabilitySchedule() {
      bool result = setString(OS_WaterHeater_HeatPump_PumpedCondenserFields::AvailabilityScheduleName, "");
      OS_ASSERT(result);
    }

    unsigned WaterHeaterHeatPumpPumpedCondenser_Impl::inletPort() const {
      return OS_WaterHeater_HeatPump_PumpedCondenserFields::AirInletNodeName;
    }

    unsigned WaterHeaterHeatPumpPumpedCondenser_Impl::outletPort() const {
      return OS_WaterHeater_HeatPump_PumpedCondenserFields::AirOutletNodeName;
    }

    ModelObject WaterHeaterHeatPumpPumpedCondenser_Impl::clone(Model model) const {
      std::cout << "boom 1" << std::endl;
      auto newWaterHeater = ZoneHVACComponent_Impl::clone(model).cast<WaterHeaterHeatPumpPumpedCondenser>();

      return newWaterHeater;
    }

    std::vector<IdfObject> WaterHeaterHeatPumpPumpedCondenser_Impl::remove() {

      return ZoneHVACComponent_Impl::remove();
    }

    std::vector<ModelObject> WaterHeaterHeatPumpPumpedCondenser_Impl::children() const {
      std::vector<ModelObject> result;

      return result;
    }

    bool WaterHeaterHeatPumpPumpedCondenser_Impl::addToThermalZone(ThermalZone& thermalZone) {
      bool result = false;

      return result;
    }

  }  // namespace detail

  WaterHeaterHeatPumpPumpedCondenser::WaterHeaterHeatPumpPumpedCondenser(const Model& model)
    : ZoneHVACComponent(WaterHeaterHeatPumpPumpedCondenser::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>());
  }

  WaterHeaterHeatPumpPumpedCondenser::WaterHeaterHeatPumpPumpedCondenser(const Model& model, const ModelObject& dxCoil, const HVACComponent& tank,
                                                                         const HVACComponent& fan, Schedule& compressorSetpointTemperatureSchedule,
                                                                         Schedule& inletAirMixerSchedule)
    : ZoneHVACComponent(WaterHeaterHeatPumpPumpedCondenser::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>());
  }

  IddObjectType WaterHeaterHeatPumpPumpedCondenser::iddObjectType() {
    return IddObjectType(IddObjectType::OS_WaterHeater_HeatPump_PumpedCondenser);
  }

  boost::optional<Schedule> WaterHeaterHeatPumpPumpedCondenser::availabilitySchedule() const {
    return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->availabilitySchedule();
  }

  bool WaterHeaterHeatPumpPumpedCondenser::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->setAvailabilitySchedule(schedule);
  }

  void WaterHeaterHeatPumpPumpedCondenser::resetAvailabilitySchedule() {
    getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->resetAvailabilitySchedule();
  }

  /// @cond
  WaterHeaterHeatPumpPumpedCondenser::WaterHeaterHeatPumpPumpedCondenser(std::shared_ptr<detail::WaterHeaterHeatPumpPumpedCondenser_Impl> impl)
    : ZoneHVACComponent(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
