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

#include "CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl.hpp"
#include "CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "UnivariateFunctions.hpp"
#include "UnivariateFunctions_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControl_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl(
      const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : HVACComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl::iddObjectType());
    }

    CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : HVACComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl::iddObjectType());
    }

    CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl(
      const CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl& other, Model_Impl* model, bool keepHandle)
      : HVACComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::outputVariableNames() const {
      static std::vector<std::string> result;
      if (result.empty()) {
      }
      return result;
    }

    IddObjectType CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::iddObjectType() const {
      return CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl::iddObjectType();
    }

    std::vector<ScheduleTypeKey>
      CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      // TODO: Check schedule display names.
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
      if (std::find(b, e, OS_Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::AvailabilitySchedule) != e) {
        result.push_back(ScheduleTypeKey("CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl", "Availability Schedule"));
      }
      return result;
    }

    Schedule CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::availabilitySchedule() const {
      boost::optional<Schedule> value = optionalAvailabilitySchedule();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Availability Schedule attached.");
      }
      return value.get();
    }

    boost::optional<Connection> CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::coilAirInletNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Connection>(
        OS_Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::CoilAirInletNode);
    }

    boost::optional<Connection> CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::coilAirOutletNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Connection>(
        OS_Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::CoilAirOutletNode);
    }

    boost::optional<double> CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::ratedTotalCoolingCapacity() const {
      return getDouble(OS_Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::RatedTotalCoolingCapacity, true);
    }

    bool CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::isRatedTotalCoolingCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::RatedTotalCoolingCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::autosizedRatedTotalCoolingCapacity() {
      return getAutosizedValue("TODO_CHECK_SQL Rated Total Cooling Capacity", "W");
    }

    boost::optional<double> CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::ratedSensibleHeatRatio() const {
      return getDouble(OS_Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::RatedSensibleHeatRatio, true);
    }

    bool CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::isRatedSensibleHeatRatioAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::RatedSensibleHeatRatio, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::autosizedRatedSensibleHeatRatio() {
      return getAutosizedValue("TODO_CHECK_SQL Rated Sensible Heat Ratio", "");
    }

    double CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::indoorUnitReferenceSuperheating() const {
      boost::optional<double> value =
        getDouble(OS_Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::IndoorUnitReferenceSuperheating, true);
      OS_ASSERT(value);
      return value.get();
    }

    UnivariateFunctions
      CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::indoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve() const {
      boost::optional<UnivariateFunctions> value = optionalIndoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Indoor Unit Evaporating Temperature Functionof Superheating Curve attached.");
      }
      return value.get();
    }

    bool CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::AvailabilitySchedule,
                                "CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl", "Availability Schedule", schedule);
      return result;
    }

    bool CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::setCoilAirInletNode(const Connection& connection) {
      bool result = setPointer(OS_Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::CoilAirInletNode, connection.handle());
      return result;
    }

    void CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::resetCoilAirInletNode() {
      bool result = setString(OS_Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::CoilAirInletNode, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::setCoilAirOutletNode(const Connection& connection) {
      bool result = setPointer(OS_Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::CoilAirOutletNode, connection.handle());
      return result;
    }

    void CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::resetCoilAirOutletNode() {
      bool result = setString(OS_Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::CoilAirOutletNode, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::setRatedTotalCoolingCapacity(double ratedTotalCoolingCapacity) {
      bool result =
        setDouble(OS_Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::RatedTotalCoolingCapacity, ratedTotalCoolingCapacity);
      return result;
    }

    void CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::autosizeRatedTotalCoolingCapacity() {
      bool result = setString(OS_Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::RatedTotalCoolingCapacity, "autosize");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::setRatedSensibleHeatRatio(double ratedSensibleHeatRatio) {
      bool result =
        setDouble(OS_Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::RatedSensibleHeatRatio, ratedSensibleHeatRatio);
      return result;
    }

    void CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::autosizeRatedSensibleHeatRatio() {
      bool result = setString(OS_Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::RatedSensibleHeatRatio, "autosize");
      OS_ASSERT(result);
    }

    bool
      CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::setIndoorUnitReferenceSuperheating(double indoorUnitReferenceSuperheating) {
      bool result = setDouble(OS_Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::IndoorUnitReferenceSuperheating,
                              indoorUnitReferenceSuperheating);
      return result;
    }

    bool CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::setIndoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve(
      const UnivariateFunctions& univariateFunctions) {
      bool result = setPointer(
        OS_Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::IndoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve,
        univariateFunctions.handle());
      return result;
    }

    void CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::autosize() {
      autosizeRatedTotalCoolingCapacity();
      autosizeRatedSensibleHeatRatio();
    }

    void CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedRatedTotalCoolingCapacity();
      if (val) {
        setRatedTotalCoolingCapacity(val.get());
      }

      val = autosizedRatedSensibleHeatRatio();
      if (val) {
        setRatedSensibleHeatRatio(val.get());
      }
    }

    boost::optional<Schedule> CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::optionalAvailabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(
        OS_Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::AvailabilitySchedule);
    }

    boost::optional<UnivariateFunctions>
      CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::optionalIndoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<UnivariateFunctions>(
        OS_Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::IndoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve);
    }

  }  // namespace detail

  CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl::CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl(const Model& model)
    : HVACComponent(CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl>());

    // TODO: Appropriately handle the following required object-list fields.
    //     OS_Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::AvailabilitySchedule
    //     OS_Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::IndoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve
    bool ok = true;
    // ok = setAvailabilitySchedule();
    OS_ASSERT(ok);
    // ok = setRatedTotalCoolingCapacity();
    OS_ASSERT(ok);
    // ok = setRatedSensibleHeatRatio();
    OS_ASSERT(ok);
    // ok = setIndoorUnitReferenceSuperheating();
    OS_ASSERT(ok);
    // ok = setIndoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve();
    OS_ASSERT(ok);
  }

  IddObjectType CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl::iddObjectType() {
    return IddObjectType(IddObjectType::OS_Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControl);
  }

  Schedule CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl::availabilitySchedule() const {
    return getImpl<detail::CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl>()->availabilitySchedule();
  }

  boost::optional<Connection> CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl::coilAirInletNode() const {
    return getImpl<detail::CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl>()->coilAirInletNode();
  }

  boost::optional<Connection> CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl::coilAirOutletNode() const {
    return getImpl<detail::CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl>()->coilAirOutletNode();
  }

  boost::optional<double> CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl::ratedTotalCoolingCapacity() const {
    return getImpl<detail::CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl>()->ratedTotalCoolingCapacity();
  }

  bool CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl::isRatedTotalCoolingCapacityAutosized() const {
    return getImpl<detail::CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl>()->isRatedTotalCoolingCapacityAutosized();
  }

  boost::optional<double> CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl::autosizedRatedTotalCoolingCapacity() {
    return getImpl<detail::CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl>()->autosizedRatedTotalCoolingCapacity();
  }

  boost::optional<double> CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl::ratedSensibleHeatRatio() const {
    return getImpl<detail::CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl>()->ratedSensibleHeatRatio();
  }

  bool CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl::isRatedSensibleHeatRatioAutosized() const {
    return getImpl<detail::CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl>()->isRatedSensibleHeatRatioAutosized();
  }

  boost::optional<double> CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl::autosizedRatedSensibleHeatRatio() {
    return getImpl<detail::CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl>()->autosizedRatedSensibleHeatRatio();
  }

  double CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl::indoorUnitReferenceSuperheating() const {
    return getImpl<detail::CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl>()->indoorUnitReferenceSuperheating();
  }

  UnivariateFunctions
    CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl::indoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve() const {
    return getImpl<detail::CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->indoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve();
  }

  bool CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setAvailabilitySchedule(schedule);
  }

  bool CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl::setCoilAirInletNode(const Connection& connection) {
    return getImpl<detail::CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setCoilAirInletNode(connection);
  }

  void CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl::resetCoilAirInletNode() {
    getImpl<detail::CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl>()->resetCoilAirInletNode();
  }

  bool CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl::setCoilAirOutletNode(const Connection& connection) {
    return getImpl<detail::CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setCoilAirOutletNode(connection);
  }

  void CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl::resetCoilAirOutletNode() {
    getImpl<detail::CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl>()->resetCoilAirOutletNode();
  }

  bool CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl::setRatedTotalCoolingCapacity(double ratedTotalCoolingCapacity) {
    return getImpl<detail::CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setRatedTotalCoolingCapacity(
      ratedTotalCoolingCapacity);
  }

  void CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl::autosizeRatedTotalCoolingCapacity() {
    getImpl<detail::CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl>()->autosizeRatedTotalCoolingCapacity();
  }

  bool CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl::setRatedSensibleHeatRatio(double ratedSensibleHeatRatio) {
    return getImpl<detail::CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setRatedSensibleHeatRatio(ratedSensibleHeatRatio);
  }

  void CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl::autosizeRatedSensibleHeatRatio() {
    getImpl<detail::CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl>()->autosizeRatedSensibleHeatRatio();
  }

  bool CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl::setIndoorUnitReferenceSuperheating(double indoorUnitReferenceSuperheating) {
    return getImpl<detail::CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setIndoorUnitReferenceSuperheating(
      indoorUnitReferenceSuperheating);
  }

  bool CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl::setIndoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve(
    const UnivariateFunctions& univariateFunctions) {
    return getImpl<detail::CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->setIndoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve(univariateFunctions);
  }

  /// @cond
  CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl::CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl(
    std::shared_ptr<detail::CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl> impl)
    : HVACComponent(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
