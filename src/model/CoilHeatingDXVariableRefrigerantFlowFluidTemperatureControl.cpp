/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl.hpp"
#include "CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "CurveQuadratic.hpp"
#include "CurveQuadratic_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControl_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl(
      const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : HVACComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl::iddObjectType());
    }

    CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : HVACComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl::iddObjectType());
    }

    CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl(
      const CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl& other, Model_Impl* model, bool keepHandle)
      : HVACComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Heating Coil Heating Rate", "Heating Coil Heating Energy", "Heating Coil Runtime Fraction",
                                                   "Heating Coil VRF Condensing Temperature", "Heating Coil VRF Subcooling Degrees"};
      return result;
    }

    IddObjectType CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::iddObjectType() const {
      return CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl::iddObjectType();
    }

    std::vector<ScheduleTypeKey>
      CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
      if (std::find(b, e, OS_Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::AvailabilitySchedule) != e) {
        result.push_back(ScheduleTypeKey("CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl", "Availability Schedule"));
      }
      return result;
    }

    Schedule CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::availabilitySchedule() const {
      boost::optional<Schedule> value = optionalAvailabilitySchedule();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Availability Schedule attached.");
      }
      return value.get();
    }

    boost::optional<double> CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::ratedTotalHeatingCapacity() const {
      return getDouble(OS_Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::RatedTotalHeatingCapacity, true);
    }

    bool CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::isRatedTotalHeatingCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::RatedTotalHeatingCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::autosizedRatedTotalHeatingCapacity() {
      return getAutosizedValue("Design Size Rated Total Heating Capacity", "W");
    }

    double CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::indoorUnitReferenceSubcooling() const {
      boost::optional<double> value =
        getDouble(OS_Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::IndoorUnitReferenceSubcooling, true);
      OS_ASSERT(value);
      return value.get();
    }

    Curve CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::indoorUnitCondensingTemperatureFunctionofSubcoolingCurve() const {
      boost::optional<Curve> value = optionalIndoorUnitCondensingTemperatureFunctionofSubcoolingCurve();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Indoor Unit Condensing Temperature Function of Subcooling Curve attached.");
      }
      return value.get();
    }

    bool CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::AvailabilitySchedule,
                                "CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl", "Availability Schedule", schedule);
      return result;
    }

    bool CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::setRatedTotalHeatingCapacity(double ratedTotalHeatingCapacity) {
      bool result =
        setDouble(OS_Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::RatedTotalHeatingCapacity, ratedTotalHeatingCapacity);
      return result;
    }

    void CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::autosizeRatedTotalHeatingCapacity() {
      bool result = setString(OS_Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::RatedTotalHeatingCapacity, "autosize");
      OS_ASSERT(result);
    }

    bool CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::setIndoorUnitReferenceSubcooling(double indoorUnitReferenceSubcooling) {
      bool result = setDouble(OS_Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::IndoorUnitReferenceSubcooling,
                              indoorUnitReferenceSubcooling);
      return result;
    }

    bool CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::setIndoorUnitCondensingTemperatureFunctionofSubcoolingCurve(
      const Curve& curve) {
      bool result =
        setPointer(OS_Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::IndoorUnitCondensingTemperatureFunctionofSubcoolingCurve,
                   curve.handle());
      return result;
    }

    void CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::autosize() {
      autosizeRatedTotalHeatingCapacity();
    }

    void CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedRatedTotalHeatingCapacity();
      if (val) {
        setRatedTotalHeatingCapacity(val.get());
      }
    }

    boost::optional<Schedule> CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::optionalAvailabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(
        OS_Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::AvailabilitySchedule);
    }

    boost::optional<Curve>
      CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::optionalIndoorUnitCondensingTemperatureFunctionofSubcoolingCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::IndoorUnitCondensingTemperatureFunctionofSubcoolingCurve);
    }

    ModelObject CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::clone(Model model) const {
      return HVACComponent_Impl::clone(model);
    }

    std::vector<ModelObject> CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::children() const {
      std::vector<ModelObject> result;

      result.push_back(indoorUnitCondensingTemperatureFunctionofSubcoolingCurve());

      return result;
    }

    boost::optional<double> CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::autosizedRatedAirFlowRate() const {
      return getAutosizedValue("Design Size Rated Air Flow Rate", "m3/s");
    }

    ComponentType CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::componentType() const {
      return ComponentType::Heating;
    }

    std::vector<FuelType> CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::coolingFuelTypes() const {
      return {};
    }

    std::vector<FuelType> CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::heatingFuelTypes() const {
      return {FuelType::Electricity};
    }

    std::vector<AppGFuelType> CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::appGHeatingFuelTypes() const {
      return {AppGFuelType::HeatPump};
    }

  }  // namespace detail

  CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl::CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl(const Model& model)
    : HVACComponent(CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl>());

    bool ok = true;
    Schedule schedule = model.alwaysOnDiscreteSchedule();
    ok = setAvailabilitySchedule(schedule);
    OS_ASSERT(ok);
    autosizeRatedTotalHeatingCapacity();
    ok = setIndoorUnitReferenceSubcooling(5.0);
    OS_ASSERT(ok);
    CurveQuadratic indoorUnitCondensingTemperatureFunctionofSubcoolingCurve(model);
    indoorUnitCondensingTemperatureFunctionofSubcoolingCurve.setName("VRFIUCondTempCurve");
    indoorUnitCondensingTemperatureFunctionofSubcoolingCurve.setCoefficient1Constant(-1.85);
    indoorUnitCondensingTemperatureFunctionofSubcoolingCurve.setCoefficient2x(0.411);
    indoorUnitCondensingTemperatureFunctionofSubcoolingCurve.setCoefficient3xPOW2(0.0196);
    indoorUnitCondensingTemperatureFunctionofSubcoolingCurve.setMinimumValueofx(0);
    indoorUnitCondensingTemperatureFunctionofSubcoolingCurve.setMaximumValueofx(20);
    ok = setIndoorUnitCondensingTemperatureFunctionofSubcoolingCurve(indoorUnitCondensingTemperatureFunctionofSubcoolingCurve);
    OS_ASSERT(ok);
  }

  IddObjectType CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl::iddObjectType() {
    return IddObjectType(IddObjectType::OS_Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControl);
  }

  Schedule CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl::availabilitySchedule() const {
    return getImpl<detail::CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl>()->availabilitySchedule();
  }

  boost::optional<double> CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl::ratedTotalHeatingCapacity() const {
    return getImpl<detail::CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl>()->ratedTotalHeatingCapacity();
  }

  bool CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl::isRatedTotalHeatingCapacityAutosized() const {
    return getImpl<detail::CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl>()->isRatedTotalHeatingCapacityAutosized();
  }

  boost::optional<double> CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl::autosizedRatedTotalHeatingCapacity() {
    return getImpl<detail::CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl>()->autosizedRatedTotalHeatingCapacity();
  }

  double CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl::indoorUnitReferenceSubcooling() const {
    return getImpl<detail::CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl>()->indoorUnitReferenceSubcooling();
  }

  Curve CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl::indoorUnitCondensingTemperatureFunctionofSubcoolingCurve() const {
    return getImpl<detail::CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->indoorUnitCondensingTemperatureFunctionofSubcoolingCurve();
  }

  bool CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setAvailabilitySchedule(schedule);
  }

  bool CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl::setRatedTotalHeatingCapacity(double ratedTotalHeatingCapacity) {
    return getImpl<detail::CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setRatedTotalHeatingCapacity(
      ratedTotalHeatingCapacity);
  }

  void CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl::autosizeRatedTotalHeatingCapacity() {
    getImpl<detail::CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl>()->autosizeRatedTotalHeatingCapacity();
  }

  bool CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl::setIndoorUnitReferenceSubcooling(double indoorUnitReferenceSubcooling) {
    return getImpl<detail::CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setIndoorUnitReferenceSubcooling(
      indoorUnitReferenceSubcooling);
  }

  bool CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl::setIndoorUnitCondensingTemperatureFunctionofSubcoolingCurve(const Curve& curve) {
    return getImpl<detail::CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->setIndoorUnitCondensingTemperatureFunctionofSubcoolingCurve(curve);
  }

  boost::optional<double> CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl::autosizedRatedAirFlowRate() const {
    return getImpl<detail::CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl>()->autosizedRatedAirFlowRate();
  }

  /// @cond
  CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl::CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl(
    std::shared_ptr<detail::CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl> impl)
    : HVACComponent(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
