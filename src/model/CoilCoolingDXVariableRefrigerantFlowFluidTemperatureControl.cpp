/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl.hpp"
#include "CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl.hpp"

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
#include <utilities/idd/OS_Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControl_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

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
      static const std::vector<std::string> result{
        "Cooling Coil Total Cooling Rate",      "Cooling Coil Total Cooling Energy",        "Cooling Coil Sensible Cooling Rate",
        "Cooling Coil Sensible Cooling Energy", "Cooling Coil Latent Cooling Rate",         "Cooling Coil Latent Cooling Energy",
        "Cooling Coil Runtime Fraction",        "Cooling Coil VRF Evaporating Temperature", "Cooling Coil VRF Super Heating Degrees"};
      return result;
    }

    IddObjectType CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::iddObjectType() const {
      return CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl::iddObjectType();
    }

    std::vector<ScheduleTypeKey>
      CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
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
      return getAutosizedValue("Design Size Rated Total Cooling Capacity", "W");
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
      return getAutosizedValue("Design Size Rated Sensible Heat Ratio", "");
    }

    double CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::indoorUnitReferenceSuperheating() const {
      boost::optional<double> value =
        getDouble(OS_Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::IndoorUnitReferenceSuperheating, true);
      OS_ASSERT(value);
      return value.get();
    }

    Curve CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::indoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve() const {
      boost::optional<Curve> value = optionalIndoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Indoor Unit Evaporating Temperature Function of Superheating Curve attached.");
      }
      return value.get();
    }

    bool CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::AvailabilitySchedule,
                                "CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl", "Availability Schedule", schedule);
      return result;
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
      const Curve& curve) {
      bool result = setPointer(
        OS_Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::IndoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve,
        curve.handle());
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

    boost::optional<Curve>
      CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::optionalIndoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::IndoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve);
    }

    ModelObject CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::clone(Model model) const {
      return HVACComponent_Impl::clone(model);
    }

    std::vector<ModelObject> CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::children() const {
      std::vector<ModelObject> result;

      result.push_back(indoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve());

      return result;
    }

    boost::optional<double> CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::autosizedRatedAirFlowRate() const {
      return getAutosizedValue("Design Size Rated Air Flow Rate", "m3/s");
    }

    ComponentType CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::componentType() const {
      return ComponentType::Cooling;
    }

    std::vector<FuelType> CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::coolingFuelTypes() const {
      return {FuelType::Electricity};
    }

    std::vector<FuelType> CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::heatingFuelTypes() const {
      return {};
    }

    std::vector<AppGFuelType> CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl::appGHeatingFuelTypes() const {
      return {};
    }

  }  // namespace detail

  CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl::CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl(const Model& model)
    : HVACComponent(CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl>());

    bool ok = true;
    Schedule schedule = model.alwaysOnDiscreteSchedule();
    ok = setAvailabilitySchedule(schedule);
    OS_ASSERT(ok);
    autosizeRatedTotalCoolingCapacity();
    autosizeRatedSensibleHeatRatio();
    ok = setIndoorUnitReferenceSuperheating(5.0);
    OS_ASSERT(ok);
    CurveQuadratic indoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve(model);
    indoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve.setName("VRFIUEvapTempCurve");
    indoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve.setCoefficient1Constant(0);
    indoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve.setCoefficient2x(0.843);
    indoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve.setCoefficient3xPOW2(0);
    indoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve.setMinimumValueofx(0);
    indoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve.setMaximumValueofx(15);
    ok = setIndoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve(indoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve);
    OS_ASSERT(ok);
  }

  IddObjectType CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl::iddObjectType() {
    return IddObjectType(IddObjectType::OS_Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControl);
  }

  Schedule CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl::availabilitySchedule() const {
    return getImpl<detail::CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl>()->availabilitySchedule();
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

  Curve CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl::indoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve() const {
    return getImpl<detail::CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->indoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve();
  }

  bool CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setAvailabilitySchedule(schedule);
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

  bool
    CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl::setIndoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve(const Curve& curve) {
    return getImpl<detail::CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->setIndoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve(curve);
  }

  boost::optional<double> CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl::autosizedRatedAirFlowRate() const {
    return getImpl<detail::CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl>()->autosizedRatedAirFlowRate();
  }

  /// @cond
  CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl::CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl(
    std::shared_ptr<detail::CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl> impl)
    : HVACComponent(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
