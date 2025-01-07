/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "FanComponentModel.hpp"
#include "FanComponentModel_Impl.hpp"

#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Model.hpp"
#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "SetpointManagerMixedAir.hpp"

#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

// Ctor
#include "CurveExponentialSkewNormal.hpp"
#include "CurveQuartic.hpp"
#include "CurveFanPressureRise.hpp"
#include "CurveRectangularHyperbola2.hpp"
#include "CurveSigmoid.hpp"
#include "CurveLinear.hpp"
#include "CurveRectangularHyperbola1.hpp"
#include "CurveExponentialDecay.hpp"

// containing HVAC Component
#include "AirLoopHVACUnitarySystem.hpp"
#include "AirLoopHVACUnitarySystem_Impl.hpp"
#include "AirTerminalSingleDuctParallelPIUReheat.hpp"
#include "AirTerminalSingleDuctParallelPIUReheat_Impl.hpp"
#include "AirTerminalSingleDuctSeriesPIUReheat.hpp"
#include "AirTerminalSingleDuctSeriesPIUReheat_Impl.hpp"

// containing ZoneHVAC Component
#include "ZoneHVACEvaporativeCoolerUnit.hpp"
#include "ZoneHVACEvaporativeCoolerUnit_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Fan_ComponentModel_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    FanComponentModel_Impl::FanComponentModel_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == FanComponentModel::iddObjectType());
    }

    FanComponentModel_Impl::FanComponentModel_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == FanComponentModel::iddObjectType());
    }

    FanComponentModel_Impl::FanComponentModel_Impl(const FanComponentModel_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& FanComponentModel_Impl::outputVariableNames() const {

      static const std::vector<std::string> results{"Fan Electricity Rate", "Fan Rise in Air Temperature", "Fan Heat Gain to Air",
                                                    "Fan Electricity Energy", "Fan Air Mass Flow Rate"};
      return results;
    }

    IddObjectType FanComponentModel_Impl::iddObjectType() const {
      return FanComponentModel::iddObjectType();
    }

    std::vector<ScheduleTypeKey> FanComponentModel_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_Fan_ComponentModelFields::AvailabilityScheduleName) != e) {
        result.push_back(ScheduleTypeKey("FanComponentModel", "Availability"));
      }
      return result;
    }

    Schedule FanComponentModel_Impl::availabilitySchedule() const {
      boost::optional<Schedule> value =
        getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Fan_ComponentModelFields::AvailabilityScheduleName);
      if (!value) {
        // it is an error if we get here, however we don't want to crash
        // so we hook up to global always on schedule
        LOG(Error, "Required availability schedule not set, using 'Always On' schedule");
        value = this->model().alwaysOnDiscreteSchedule();
        OS_ASSERT(value);
        const_cast<FanComponentModel_Impl*>(this)->setAvailabilitySchedule(*value);
        value = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Fan_ComponentModelFields::AvailabilityScheduleName);
      }
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> FanComponentModel_Impl::maximumFlowRate() const {
      return getDouble(OS_Fan_ComponentModelFields::MaximumFlowRate, true);
    }

    bool FanComponentModel_Impl::isMaximumFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Fan_ComponentModelFields::MaximumFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> FanComponentModel_Impl::minimumFlowRate() const {
      return getDouble(OS_Fan_ComponentModelFields::MinimumFlowRate, true);
    }

    bool FanComponentModel_Impl::isMinimumFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Fan_ComponentModelFields::MinimumFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    double FanComponentModel_Impl::fanSizingFactor() const {
      boost::optional<double> value = getDouble(OS_Fan_ComponentModelFields::FanSizingFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    double FanComponentModel_Impl::fanWheelDiameter() const {
      boost::optional<double> value = getDouble(OS_Fan_ComponentModelFields::FanWheelDiameter, true);
      OS_ASSERT(value);
      return value.get();
    }

    double FanComponentModel_Impl::fanOutletArea() const {
      boost::optional<double> value = getDouble(OS_Fan_ComponentModelFields::FanOutletArea, true);
      OS_ASSERT(value);
      return value.get();
    }

    double FanComponentModel_Impl::maximumFanStaticEfficiency() const {
      boost::optional<double> value = getDouble(OS_Fan_ComponentModelFields::MaximumFanStaticEfficiency, true);
      OS_ASSERT(value);
      return value.get();
    }

    double FanComponentModel_Impl::eulerNumberatMaximumFanStaticEfficiency() const {
      boost::optional<double> value = getDouble(OS_Fan_ComponentModelFields::EulerNumberatMaximumFanStaticEfficiency, true);
      OS_ASSERT(value);
      return value.get();
    }

    double FanComponentModel_Impl::maximumDimensionlessFanAirflow() const {
      boost::optional<double> value = getDouble(OS_Fan_ComponentModelFields::MaximumDimensionlessFanAirflow, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> FanComponentModel_Impl::motorFanPulleyRatio() const {
      return getDouble(OS_Fan_ComponentModelFields::MotorFanPulleyRatio, true);
    }

    bool FanComponentModel_Impl::isMotorFanPulleyRatioAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Fan_ComponentModelFields::MotorFanPulleyRatio, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> FanComponentModel_Impl::beltMaximumTorque() const {
      return getDouble(OS_Fan_ComponentModelFields::BeltMaximumTorque, true);
    }

    bool FanComponentModel_Impl::isBeltMaximumTorqueAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Fan_ComponentModelFields::BeltMaximumTorque, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    double FanComponentModel_Impl::beltSizingFactor() const {
      boost::optional<double> value = getDouble(OS_Fan_ComponentModelFields::BeltSizingFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    double FanComponentModel_Impl::beltFractionalTorqueTransition() const {
      boost::optional<double> value = getDouble(OS_Fan_ComponentModelFields::BeltFractionalTorqueTransition, true);
      OS_ASSERT(value);
      return value.get();
    }

    double FanComponentModel_Impl::motorMaximumSpeed() const {
      boost::optional<double> value = getDouble(OS_Fan_ComponentModelFields::MotorMaximumSpeed, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> FanComponentModel_Impl::maximumMotorOutputPower() const {
      return getDouble(OS_Fan_ComponentModelFields::MaximumMotorOutputPower, true);
    }

    bool FanComponentModel_Impl::isMaximumMotorOutputPowerAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Fan_ComponentModelFields::MaximumMotorOutputPower, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    double FanComponentModel_Impl::motorSizingFactor() const {
      boost::optional<double> value = getDouble(OS_Fan_ComponentModelFields::MotorSizingFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    double FanComponentModel_Impl::motorInAirstreamFraction() const {
      boost::optional<double> value = getDouble(OS_Fan_ComponentModelFields::MotorInAirstreamFraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string FanComponentModel_Impl::vFDEfficiencyType() const {
      boost::optional<std::string> value = getString(OS_Fan_ComponentModelFields::VFDEfficiencyType, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> FanComponentModel_Impl::maximumVFDOutputPower() const {
      return getDouble(OS_Fan_ComponentModelFields::MaximumVFDOutputPower, true);
    }

    bool FanComponentModel_Impl::isMaximumVFDOutputPowerAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Fan_ComponentModelFields::MaximumVFDOutputPower, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    double FanComponentModel_Impl::vFDSizingFactor() const {
      boost::optional<double> value = getDouble(OS_Fan_ComponentModelFields::VFDSizingFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    Curve FanComponentModel_Impl::fanPressureRiseCurve() const {
      boost::optional<Curve> value = optionalFanPressureRiseCurve();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Fan Pressure Rise Curve attached.");
      }
      return value.get();
    }

    Curve FanComponentModel_Impl::ductStaticPressureResetCurve() const {
      boost::optional<Curve> value = optionalDuctStaticPressureResetCurve();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Duct Static Pressure Reset Curve attached.");
      }
      return value.get();
    }

    Curve FanComponentModel_Impl::normalizedFanStaticEfficiencyCurveNonStallRegion() const {
      boost::optional<Curve> value = optionalNormalizedFanStaticEfficiencyCurveNonStallRegion();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Normalized Fan Static Efficiency Curve Non Stall Region attached.");
      }
      return value.get();
    }

    Curve FanComponentModel_Impl::normalizedFanStaticEfficiencyCurveStallRegion() const {
      boost::optional<Curve> value = optionalNormalizedFanStaticEfficiencyCurveStallRegion();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Normalized Fan Static Efficiency Curve Stall Region attached.");
      }
      return value.get();
    }

    Curve FanComponentModel_Impl::normalizedDimensionlessAirflowCurveNonStallRegion() const {
      boost::optional<Curve> value = optionalNormalizedDimensionlessAirflowCurveNonStallRegion();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Normalized Dimensionless Airflow Curve Non Stall Region attached.");
      }
      return value.get();
    }

    Curve FanComponentModel_Impl::normalizedDimensionlessAirflowCurveStallRegion() const {
      boost::optional<Curve> value = optionalNormalizedDimensionlessAirflowCurveStallRegion();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Normalized Dimensionless Airflow Curve Stall Region attached.");
      }
      return value.get();
    }

    boost::optional<Curve> FanComponentModel_Impl::maximumBeltEfficiencyCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Fan_ComponentModelFields::MaximumBeltEfficiencyCurveName);
    }

    boost::optional<Curve> FanComponentModel_Impl::normalizedBeltEfficiencyCurveRegion1() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Fan_ComponentModelFields::NormalizedBeltEfficiencyCurveNameRegion1);
    }

    boost::optional<Curve> FanComponentModel_Impl::normalizedBeltEfficiencyCurveRegion2() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Fan_ComponentModelFields::NormalizedBeltEfficiencyCurveNameRegion2);
    }

    boost::optional<Curve> FanComponentModel_Impl::normalizedBeltEfficiencyCurveRegion3() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Fan_ComponentModelFields::NormalizedBeltEfficiencyCurveNameRegion3);
    }

    boost::optional<Curve> FanComponentModel_Impl::maximumMotorEfficiencyCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Fan_ComponentModelFields::MaximumMotorEfficiencyCurveName);
    }

    boost::optional<Curve> FanComponentModel_Impl::normalizedMotorEfficiencyCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Fan_ComponentModelFields::NormalizedMotorEfficiencyCurveName);
    }

    boost::optional<Curve> FanComponentModel_Impl::vFDEfficiencyCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Fan_ComponentModelFields::VFDEfficiencyCurveName);
    }

    std::string FanComponentModel_Impl::endUseSubcategory() const {
      boost::optional<std::string> value = getString(OS_Fan_ComponentModelFields::EndUseSubcategory, true);
      OS_ASSERT(value);
      return value.get();
    }

    unsigned FanComponentModel_Impl::inletPort() const {
      return OS_Fan_ComponentModelFields::AirInletNodeName;
    }

    unsigned FanComponentModel_Impl::outletPort() const {
      return OS_Fan_ComponentModelFields::AirOutletNodeName;
    }

    bool FanComponentModel_Impl::addToNode(Node& node) {
      auto oaSystem = node.airLoopHVACOutdoorAirSystem();
      auto airLoop = node.airLoopHVAC();

      if ((airLoop && airLoop->supplyComponent(node.handle())) || (oaSystem && oaSystem->component(node.handle()))) {
        if (StraightComponent_Impl::addToNode(node)) {
          if (airLoop) {
            SetpointManagerMixedAir::updateFanInletOutletNodes(airLoop.get());
          }
          return true;
        }
      }

      return false;
    }

    boost::optional<HVACComponent> FanComponentModel_Impl::containingHVACComponent() const {
      auto t_handle = handle();

      // AirLoopHVACUnitarySystem
      std::vector<AirLoopHVACUnitarySystem> airLoopHVACUnitarySystems = this->model().getConcreteModelObjects<AirLoopHVACUnitarySystem>();

      for (const auto& airLoopHVACUnitarySystem : airLoopHVACUnitarySystems) {
        if (boost::optional<HVACComponent> fan = airLoopHVACUnitarySystem.supplyFan()) {
          if (fan->handle() == t_handle) {
            return airLoopHVACUnitarySystem;
          }
        }
      }

      return boost::none;
    }

    boost::optional<ZoneHVACComponent> FanComponentModel_Impl::containingZoneHVACComponent() const {

      std::vector<ZoneHVACComponent> zoneHVACComponent = this->model().getModelObjects<ZoneHVACComponent>();
      for (const auto& elem : zoneHVACComponent) {
        switch (elem.iddObject().type().value()) {
          case openstudio::IddObjectType::OS_ZoneHVAC_EvaporativeCoolerUnit: {
            auto component = elem.cast<ZoneHVACEvaporativeCoolerUnit>();
            if (component.supplyAirFan().handle() == this->handle()) {
              return elem;
            }
            break;
          }
          default: {
            break;
          }
        }
      }
      return boost::none;
    }

    bool FanComponentModel_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_Fan_ComponentModelFields::AvailabilityScheduleName, "FanComponentModel", "Availability", schedule);
      return result;
    }

    bool FanComponentModel_Impl::setMaximumFlowRate(double maximumFlowRate) {
      bool result = setDouble(OS_Fan_ComponentModelFields::MaximumFlowRate, maximumFlowRate);
      return result;
    }

    void FanComponentModel_Impl::autosizeMaximumFlowRate() {
      bool result = setString(OS_Fan_ComponentModelFields::MaximumFlowRate, "autosize");
      OS_ASSERT(result);
    }

    bool FanComponentModel_Impl::setMinimumFlowRate(double minimumFlowRate) {
      bool result = setDouble(OS_Fan_ComponentModelFields::MinimumFlowRate, minimumFlowRate);
      return result;
    }

    void FanComponentModel_Impl::autosizeMinimumFlowRate() {
      bool result = setString(OS_Fan_ComponentModelFields::MinimumFlowRate, "autosize");
      OS_ASSERT(result);
    }

    bool FanComponentModel_Impl::setFanSizingFactor(double fanSizingFactor) {
      bool result = setDouble(OS_Fan_ComponentModelFields::FanSizingFactor, fanSizingFactor);
      return result;
    }

    bool FanComponentModel_Impl::setFanWheelDiameter(double fanWheelDiameter) {
      bool result = setDouble(OS_Fan_ComponentModelFields::FanWheelDiameter, fanWheelDiameter);
      return result;
    }

    bool FanComponentModel_Impl::setFanOutletArea(double fanOutletArea) {
      bool result = setDouble(OS_Fan_ComponentModelFields::FanOutletArea, fanOutletArea);
      return result;
    }

    bool FanComponentModel_Impl::setMaximumFanStaticEfficiency(double maximumFanStaticEfficiency) {
      bool result = setDouble(OS_Fan_ComponentModelFields::MaximumFanStaticEfficiency, maximumFanStaticEfficiency);
      return result;
    }

    bool FanComponentModel_Impl::setEulerNumberatMaximumFanStaticEfficiency(double eulerNumberatMaximumFanStaticEfficiency) {
      bool result = setDouble(OS_Fan_ComponentModelFields::EulerNumberatMaximumFanStaticEfficiency, eulerNumberatMaximumFanStaticEfficiency);
      return result;
    }

    bool FanComponentModel_Impl::setMaximumDimensionlessFanAirflow(double maximumDimensionlessFanAirflow) {
      bool result = setDouble(OS_Fan_ComponentModelFields::MaximumDimensionlessFanAirflow, maximumDimensionlessFanAirflow);
      return result;
    }

    bool FanComponentModel_Impl::setMotorFanPulleyRatio(double motorFanPulleyRatio) {
      bool result = setDouble(OS_Fan_ComponentModelFields::MotorFanPulleyRatio, motorFanPulleyRatio);
      return result;
    }

    void FanComponentModel_Impl::autosizeMotorFanPulleyRatio() {
      bool result = setString(OS_Fan_ComponentModelFields::MotorFanPulleyRatio, "autosize");
      OS_ASSERT(result);
    }

    bool FanComponentModel_Impl::setBeltMaximumTorque(double beltMaximumTorque) {
      bool result = setDouble(OS_Fan_ComponentModelFields::BeltMaximumTorque, beltMaximumTorque);
      return result;
    }

    void FanComponentModel_Impl::autosizeBeltMaximumTorque() {
      bool result = setString(OS_Fan_ComponentModelFields::BeltMaximumTorque, "autosize");
      OS_ASSERT(result);
    }

    bool FanComponentModel_Impl::setBeltSizingFactor(double beltSizingFactor) {
      bool result = setDouble(OS_Fan_ComponentModelFields::BeltSizingFactor, beltSizingFactor);
      return result;
    }

    bool FanComponentModel_Impl::setBeltFractionalTorqueTransition(double beltFractionalTorqueTransition) {
      bool result = setDouble(OS_Fan_ComponentModelFields::BeltFractionalTorqueTransition, beltFractionalTorqueTransition);
      return result;
    }

    bool FanComponentModel_Impl::setMotorMaximumSpeed(double motorMaximumSpeed) {
      bool result = setDouble(OS_Fan_ComponentModelFields::MotorMaximumSpeed, motorMaximumSpeed);
      return result;
    }

    bool FanComponentModel_Impl::setMaximumMotorOutputPower(double maximumMotorOutputPower) {
      bool result = setDouble(OS_Fan_ComponentModelFields::MaximumMotorOutputPower, maximumMotorOutputPower);
      return result;
    }

    void FanComponentModel_Impl::autosizeMaximumMotorOutputPower() {
      bool result = setString(OS_Fan_ComponentModelFields::MaximumMotorOutputPower, "autosize");
      OS_ASSERT(result);
    }

    bool FanComponentModel_Impl::setMotorSizingFactor(double motorSizingFactor) {
      bool result = setDouble(OS_Fan_ComponentModelFields::MotorSizingFactor, motorSizingFactor);
      return result;
    }

    bool FanComponentModel_Impl::setMotorInAirstreamFraction(double motorInAirstreamFraction) {
      bool result = setDouble(OS_Fan_ComponentModelFields::MotorInAirstreamFraction, motorInAirstreamFraction);
      return result;
    }

    bool FanComponentModel_Impl::setVFDEfficiencyType(const std::string& vFDEfficiencyType) {
      bool result = setString(OS_Fan_ComponentModelFields::VFDEfficiencyType, vFDEfficiencyType);
      return result;
    }

    bool FanComponentModel_Impl::setMaximumVFDOutputPower(double maximumVFDOutputPower) {
      bool result = setDouble(OS_Fan_ComponentModelFields::MaximumVFDOutputPower, maximumVFDOutputPower);
      return result;
    }

    void FanComponentModel_Impl::autosizeMaximumVFDOutputPower() {
      bool result = setString(OS_Fan_ComponentModelFields::MaximumVFDOutputPower, "autosize");
      OS_ASSERT(result);
    }

    bool FanComponentModel_Impl::setVFDSizingFactor(double vFDSizingFactor) {
      bool result = setDouble(OS_Fan_ComponentModelFields::VFDSizingFactor, vFDSizingFactor);
      return result;
    }

    bool FanComponentModel_Impl::setFanPressureRiseCurve(const Curve& bivariateFunctions) {
      bool result = setPointer(OS_Fan_ComponentModelFields::FanPressureRiseCurveName, bivariateFunctions.handle());
      return result;
    }

    bool FanComponentModel_Impl::setDuctStaticPressureResetCurve(const Curve& univariateFunctions) {
      bool result = setPointer(OS_Fan_ComponentModelFields::DuctStaticPressureResetCurveName, univariateFunctions.handle());
      return result;
    }

    bool FanComponentModel_Impl::setNormalizedFanStaticEfficiencyCurveNonStallRegion(const Curve& univariateFunctions) {
      bool result = setPointer(OS_Fan_ComponentModelFields::NormalizedFanStaticEfficiencyCurveNameNonStallRegion, univariateFunctions.handle());
      return result;
    }

    bool FanComponentModel_Impl::setNormalizedFanStaticEfficiencyCurveStallRegion(const Curve& univariateFunctions) {
      bool result = setPointer(OS_Fan_ComponentModelFields::NormalizedFanStaticEfficiencyCurveNameStallRegion, univariateFunctions.handle());
      return result;
    }

    bool FanComponentModel_Impl::setNormalizedDimensionlessAirflowCurveNonStallRegion(const Curve& univariateFunctions) {
      bool result = setPointer(OS_Fan_ComponentModelFields::NormalizedDimensionlessAirflowCurveNameNonStallRegion, univariateFunctions.handle());
      return result;
    }

    bool FanComponentModel_Impl::setNormalizedDimensionlessAirflowCurveStallRegion(const Curve& univariateFunctions) {
      bool result = setPointer(OS_Fan_ComponentModelFields::NormalizedDimensionlessAirflowCurveNameStallRegion, univariateFunctions.handle());
      return result;
    }

    bool FanComponentModel_Impl::setMaximumBeltEfficiencyCurve(const Curve& univariateFunctions) {
      bool result = setPointer(OS_Fan_ComponentModelFields::MaximumBeltEfficiencyCurveName, univariateFunctions.handle());
      return result;
    }

    void FanComponentModel_Impl::resetMaximumBeltEfficiencyCurve() {
      bool result = setString(OS_Fan_ComponentModelFields::MaximumBeltEfficiencyCurveName, "");
      OS_ASSERT(result);
    }

    bool FanComponentModel_Impl::setNormalizedBeltEfficiencyCurveRegion1(const Curve& univariateFunctions) {
      bool result = setPointer(OS_Fan_ComponentModelFields::NormalizedBeltEfficiencyCurveNameRegion1, univariateFunctions.handle());
      return result;
    }

    void FanComponentModel_Impl::resetNormalizedBeltEfficiencyCurveRegion1() {
      bool result = setString(OS_Fan_ComponentModelFields::NormalizedBeltEfficiencyCurveNameRegion1, "");
      OS_ASSERT(result);
    }

    bool FanComponentModel_Impl::setNormalizedBeltEfficiencyCurveRegion2(const Curve& univariateFunctions) {
      bool result = setPointer(OS_Fan_ComponentModelFields::NormalizedBeltEfficiencyCurveNameRegion2, univariateFunctions.handle());
      return result;
    }

    void FanComponentModel_Impl::resetNormalizedBeltEfficiencyCurveRegion2() {
      bool result = setString(OS_Fan_ComponentModelFields::NormalizedBeltEfficiencyCurveNameRegion2, "");
      OS_ASSERT(result);
    }

    bool FanComponentModel_Impl::setNormalizedBeltEfficiencyCurveRegion3(const Curve& univariateFunctions) {
      bool result = setPointer(OS_Fan_ComponentModelFields::NormalizedBeltEfficiencyCurveNameRegion3, univariateFunctions.handle());
      return result;
    }

    void FanComponentModel_Impl::resetNormalizedBeltEfficiencyCurveRegion3() {
      bool result = setString(OS_Fan_ComponentModelFields::NormalizedBeltEfficiencyCurveNameRegion3, "");
      OS_ASSERT(result);
    }

    bool FanComponentModel_Impl::setMaximumMotorEfficiencyCurve(const Curve& univariateFunctions) {
      bool result = setPointer(OS_Fan_ComponentModelFields::MaximumMotorEfficiencyCurveName, univariateFunctions.handle());
      return result;
    }

    void FanComponentModel_Impl::resetMaximumMotorEfficiencyCurve() {
      bool result = setString(OS_Fan_ComponentModelFields::MaximumMotorEfficiencyCurveName, "");
      OS_ASSERT(result);
    }

    bool FanComponentModel_Impl::setNormalizedMotorEfficiencyCurve(const Curve& univariateFunctions) {
      bool result = setPointer(OS_Fan_ComponentModelFields::NormalizedMotorEfficiencyCurveName, univariateFunctions.handle());
      return result;
    }

    void FanComponentModel_Impl::resetNormalizedMotorEfficiencyCurve() {
      bool result = setString(OS_Fan_ComponentModelFields::NormalizedMotorEfficiencyCurveName, "");
      OS_ASSERT(result);
    }

    bool FanComponentModel_Impl::setVFDEfficiencyCurve(const Curve& univariateFunctions) {
      bool result = setPointer(OS_Fan_ComponentModelFields::VFDEfficiencyCurveName, univariateFunctions.handle());
      return result;
    }

    void FanComponentModel_Impl::resetVFDEfficiencyCurve() {
      bool result = setString(OS_Fan_ComponentModelFields::VFDEfficiencyCurveName, "");
      OS_ASSERT(result);
    }

    bool FanComponentModel_Impl::setEndUseSubcategory(const std::string& endUseSubcategory) {
      bool result = setString(OS_Fan_ComponentModelFields::EndUseSubcategory, endUseSubcategory);
      OS_ASSERT(result);
      return result;
    }

    boost::optional<double> FanComponentModel_Impl::autosizedMaximumFlowRate() {
      return getAutosizedValue("Design Size Maximum Flow Rate", "m3/s");
    }

    boost::optional<double> FanComponentModel_Impl::autosizedMinimumFlowRate() {
      // TODO: SQL doesn't contain it really but if it did, that'd be the name
      // If includes "Design Size Maximum Flow Rate and "Design Fan Airflow" only
      return getAutosizedValue("Design Size Minimum Flow Rate", "m3/s");
    }

    boost::optional<double> FanComponentModel_Impl::autosizedMotorFanPulleyRatio() {
      return getAutosizedValue("Drive Ratio", "");
    }

    boost::optional<double> FanComponentModel_Impl::autosizedBeltMaximumTorque() {
      return getAutosizedValue("Design Belt Output Torque", "Nm");
    }

    boost::optional<double> FanComponentModel_Impl::autosizedMaximumMotorOutputPower() {
      return getAutosizedValue("Design Motor Output Power", "W");
    }

    boost::optional<double> FanComponentModel_Impl::autosizedMaximumVFDOutputPower() {
      return getAutosizedValue("Design VFD Output Power", "W");
    }

    void FanComponentModel_Impl::autosize() {
      autosizeMaximumFlowRate();
      autosizeMinimumFlowRate();
      autosizeMotorFanPulleyRatio();
      autosizeBeltMaximumTorque();
      autosizeMaximumMotorOutputPower();
      autosizeMaximumVFDOutputPower();
    }

    void FanComponentModel_Impl::applySizingValues() {
      boost::optional<double> val;

      val = autosizedMaximumFlowRate();
      if (val) {
        setMaximumFlowRate(val.get());
      }

      val = autosizedMinimumFlowRate();
      if (val) {
        setMinimumFlowRate(val.get());
      }

      val = autosizedMotorFanPulleyRatio();
      if (val) {
        setMotorFanPulleyRatio(val.get());
      }

      val = autosizedBeltMaximumTorque();
      if (val) {
        setBeltMaximumTorque(val.get());
      }

      val = autosizedMaximumMotorOutputPower();
      if (val) {
        setMaximumMotorOutputPower(val.get());
      }

      val = autosizedMaximumVFDOutputPower();
      if (val) {
        setMaximumVFDOutputPower(val.get());
      }
    }

    boost::optional<Curve> FanComponentModel_Impl::optionalFanPressureRiseCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Fan_ComponentModelFields::FanPressureRiseCurveName);
    }

    boost::optional<Curve> FanComponentModel_Impl::optionalDuctStaticPressureResetCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Fan_ComponentModelFields::DuctStaticPressureResetCurveName);
    }

    boost::optional<Curve> FanComponentModel_Impl::optionalNormalizedFanStaticEfficiencyCurveNonStallRegion() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Fan_ComponentModelFields::NormalizedFanStaticEfficiencyCurveNameNonStallRegion);
    }

    boost::optional<Curve> FanComponentModel_Impl::optionalNormalizedFanStaticEfficiencyCurveStallRegion() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Fan_ComponentModelFields::NormalizedFanStaticEfficiencyCurveNameStallRegion);
    }

    boost::optional<Curve> FanComponentModel_Impl::optionalNormalizedDimensionlessAirflowCurveNonStallRegion() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Fan_ComponentModelFields::NormalizedDimensionlessAirflowCurveNameNonStallRegion);
    }

    boost::optional<Curve> FanComponentModel_Impl::optionalNormalizedDimensionlessAirflowCurveStallRegion() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Fan_ComponentModelFields::NormalizedDimensionlessAirflowCurveNameStallRegion);
    }

    ComponentType FanComponentModel_Impl::componentType() const {
      return ComponentType::None;
    }

    std::vector<FuelType> FanComponentModel_Impl::coolingFuelTypes() const {
      return {};
    }

    std::vector<FuelType> FanComponentModel_Impl::heatingFuelTypes() const {
      return {};
    }

    std::vector<AppGFuelType> FanComponentModel_Impl::appGHeatingFuelTypes() const {
      return {};
    }

  }  // namespace detail

  bool FanComponentModel::assignDefaultOptionalCurves() {

    Model model = this->model();

    CurveQuartic beltMaxEffMedium(model);
    beltMaxEffMedium.setName("BeltMaxEffMedium");
    beltMaxEffMedium.setCoefficient1Constant(-0.09504);
    beltMaxEffMedium.setCoefficient2x(0.03415);
    beltMaxEffMedium.setCoefficient3xPOW2(-0.008897);
    beltMaxEffMedium.setCoefficient4xPOW3(0.001159);
    beltMaxEffMedium.setCoefficient5xPOW4(-6.132e-05);
    beltMaxEffMedium.setMinimumValueofx(-1.2);
    beltMaxEffMedium.setMaximumValueofx(6.2);
    beltMaxEffMedium.setMinimumCurveOutput(-4.6);
    beltMaxEffMedium.setMaximumCurveOutput(0.0);
    // beltMaxEffMedium.setInputUnitTypeforX("");
    // beltMaxEffMedium.setOutputUnitType("");
    bool ok = setMaximumBeltEfficiencyCurve(beltMaxEffMedium);
    OS_ASSERT(ok);

    CurveRectangularHyperbola2 beltPartLoadRegion1(model);
    beltPartLoadRegion1.setName("BeltPartLoadRegion1");
    beltPartLoadRegion1.setCoefficient1C1(0.920797);
    beltPartLoadRegion1.setCoefficient2C2(0.0262686);
    beltPartLoadRegion1.setCoefficient3C3(0.151594);
    beltPartLoadRegion1.setMinimumValueofx(0.0);
    beltPartLoadRegion1.setMaximumValueofx(1.0);
    beltPartLoadRegion1.setMinimumCurveOutput(0.01);
    beltPartLoadRegion1.setMaximumCurveOutput(1.0);
    // beltPartLoadRegion1.setInputUnitTypeforx("");
    // beltPartLoadRegion1.setOutputUnitType("");
    ok = setNormalizedBeltEfficiencyCurveRegion1(beltPartLoadRegion1);
    OS_ASSERT(ok);

    CurveExponentialDecay beltPartLoadRegion2(model);
    beltPartLoadRegion2.setName("BeltPartLoadRegion2");
    beltPartLoadRegion2.setCoefficient1C1(1.011965);
    beltPartLoadRegion2.setCoefficient2C2(-0.339038);
    beltPartLoadRegion2.setCoefficient3C3(-3.43626);
    beltPartLoadRegion2.setMinimumValueofx(0.0);
    beltPartLoadRegion2.setMaximumValueofx(1.0);
    beltPartLoadRegion2.setMinimumCurveOutput(0.01);
    beltPartLoadRegion2.setMaximumCurveOutput(1.0);
    // beltPartLoadRegion2.setInputUnitTypeforx("");
    // beltPartLoadRegion2.setOutputUnitType("");
    ok = setNormalizedBeltEfficiencyCurveRegion2(beltPartLoadRegion2);
    OS_ASSERT(ok);

    CurveRectangularHyperbola2 beltPartLoadRegion3(model);
    beltPartLoadRegion3.setName("BeltPartLoadRegion3");
    beltPartLoadRegion3.setCoefficient1C1(1.037778);
    beltPartLoadRegion3.setCoefficient2C2(0.0103068);
    beltPartLoadRegion3.setCoefficient3C3(-0.0268146);
    beltPartLoadRegion3.setMinimumValueofx(0.0);
    beltPartLoadRegion3.setMaximumValueofx(1.0);
    beltPartLoadRegion3.setMinimumCurveOutput(0.01);
    beltPartLoadRegion3.setMaximumCurveOutput(1.0);
    // beltPartLoadRegion3.setInputUnitTypeforx("");
    // beltPartLoadRegion3.setOutputUnitType("");
    ok = setNormalizedBeltEfficiencyCurveRegion3(beltPartLoadRegion3);
    OS_ASSERT(ok);

    CurveRectangularHyperbola1 motorMaxEffAvg(model);
    motorMaxEffAvg.setName("MotorMaxEffAvg");
    motorMaxEffAvg.setCoefficient1C1(0.29228);
    motorMaxEffAvg.setCoefficient2C2(3.368739);
    motorMaxEffAvg.setCoefficient3C3(0.762471);
    motorMaxEffAvg.setMinimumValueofx(0.0);
    motorMaxEffAvg.setMaximumValueofx(7.6);
    motorMaxEffAvg.setMinimumCurveOutput(0.01);
    motorMaxEffAvg.setMaximumCurveOutput(1.0);
    // motorMaxEffAvg.setInputUnitTypeforx("");
    // motorMaxEffAvg.setOutputUnitType("");
    ok = setMaximumMotorEfficiencyCurve(motorMaxEffAvg);
    OS_ASSERT(ok);

    CurveRectangularHyperbola2 motorPartLoad(model);
    motorPartLoad.setName("MotorPartLoad");
    motorPartLoad.setCoefficient1C1(1.137209);
    motorPartLoad.setCoefficient2C2(0.0502359);
    motorPartLoad.setCoefficient3C3(-0.0891503);
    motorPartLoad.setMinimumValueofx(0.0);
    motorPartLoad.setMaximumValueofx(1.0);
    motorPartLoad.setMinimumCurveOutput(0.01);
    motorPartLoad.setMaximumCurveOutput(1.0);
    // motorPartLoad.setInputUnitTypeforx("");
    // motorPartLoad.setOutputUnitType("");
    ok = setNormalizedMotorEfficiencyCurve(motorPartLoad);
    OS_ASSERT(ok);

    CurveRectangularHyperbola2 vFDPartLoad(model);
    vFDPartLoad.setName("VFDPartLoad");
    vFDPartLoad.setCoefficient1C1(0.987405);
    vFDPartLoad.setCoefficient2C2(0.0155361);
    vFDPartLoad.setCoefficient3C3(-0.0059365);
    vFDPartLoad.setMinimumValueofx(0.0);
    vFDPartLoad.setMaximumValueofx(1.0);
    vFDPartLoad.setMinimumCurveOutput(0.01);
    vFDPartLoad.setMaximumCurveOutput(1.0);
    // vFDPartLoad.setInputUnitTypeforx("");
    // vFDPartLoad.setOutputUnitType("");
    ok = setVFDEfficiencyCurve(vFDPartLoad);
    OS_ASSERT(ok);

    return true;
  }

  FanComponentModel::FanComponentModel(const Model& model) : StraightComponent(FanComponentModel::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::FanComponentModel_Impl>());

    // Appropriately handle the following required object-list fields.
    //     OS_Fan_ComponentModelFields::AirInletNodeName
    //     OS_Fan_ComponentModelFields::AirOutletNodeName
    //     OS_Fan_ComponentModelFields::AvailabilityScheduleName
    //     OS_Fan_ComponentModelFields::FanPressureRiseCurveName
    //     OS_Fan_ComponentModelFields::DuctStaticPressureResetCurveName
    //     OS_Fan_ComponentModelFields::NormalizedFanStaticEfficiencyCurveNameNonStallRegion
    //     OS_Fan_ComponentModelFields::NormalizedFanStaticEfficiencyCurveNameStallRegion
    //     OS_Fan_ComponentModelFields::NormalizedDimensionlessAirflowCurveNameNonStallRegion
    //     OS_Fan_ComponentModelFields::NormalizedDimensionlessAirflowCurveNameStallRegion
    bool ok = true;

    // ok = setAirInletNode();
    // ok = setAirOutletNode();

    auto availabilitySchedule = model.alwaysOnDiscreteSchedule();
    ok = setAvailabilitySchedule(availabilitySchedule);
    OS_ASSERT(ok);

    // E+ IDD default
    autosizeMaximumFlowRate();
    autosizeMinimumFlowRate();
    ok = setFanSizingFactor(1.0);
    OS_ASSERT(ok);
    // this has an IDD default of 1.0, but it's autosizable
    //ok = setMotorFanPulleyRatio(1.0);
    // OS_ASSERT(ok);
    autosizeMotorFanPulleyRatio();

    ok = setBeltSizingFactor(1.0);
    OS_ASSERT(ok);
    ok = setBeltFractionalTorqueTransition(0.167);
    OS_ASSERT(ok);
    ok = setMotorSizingFactor(1.0);
    OS_ASSERT(ok);
    ok = setMotorInAirstreamFraction(1.0);
    OS_ASSERT(ok);
    ok = setVFDSizingFactor(1.0);
    OS_ASSERT(ok);
    ok = setEndUseSubcategory("General");
    OS_ASSERT(ok);

    // Default from E+ example file: EnergyPlus/testfiles/5ZoneWarmestVFD_FCMAuto.idf, v9.4.0
    ok = setFanWheelDiameter(0.3048);
    OS_ASSERT(ok);
    ok = setFanOutletArea(0.0873288576);
    OS_ASSERT(ok);
    ok = setMaximumFanStaticEfficiency(0.514);
    OS_ASSERT(ok);
    ok = setEulerNumberatMaximumFanStaticEfficiency(9.76);
    OS_ASSERT(ok);
    ok = setMaximumDimensionlessFanAirflow(0.160331811647483);
    OS_ASSERT(ok);

    autosizeBeltMaximumTorque();

    ok = setMotorMaximumSpeed(1800.0);
    OS_ASSERT(ok);

    autosizeMaximumMotorOutputPower();

    autosizeMaximumVFDOutputPower();

    setVFDEfficiencyType("Power");

    // Required Curves
    CurveFanPressureRise vSDExample(model);
    vSDExample.setName("VSD Example");
    vSDExample.setCoefficient1C1(1446.75833497653);
    vSDExample.setCoefficient2C2(0.0);
    vSDExample.setCoefficient3C3(0.0);
    vSDExample.setCoefficient4C4(1.0);
    vSDExample.setMinimumValueofQfan(0.0);
    vSDExample.setMaximumValueofQfan(100.0);
    vSDExample.setMinimumValueofPsm(62.5);
    vSDExample.setMaximumValueofPsm(300.0);
    vSDExample.setMinimumCurveOutput(0.0);
    vSDExample.setMaximumCurveOutput(5000.0);
    ok = setFanPressureRiseCurve(vSDExample);
    OS_ASSERT(ok);

    CurveLinear diagnosticSPR(model);
    diagnosticSPR.setName("DiagnosticSPR");
    diagnosticSPR.setCoefficient1Constant(248.84);
    diagnosticSPR.setCoefficient2x(0.0);
    diagnosticSPR.setMinimumValueofx(0.0);
    diagnosticSPR.setMaximumValueofx(100.0);
    diagnosticSPR.setMinimumCurveOutput(62.5);
    diagnosticSPR.setMaximumCurveOutput(248.84);
    // diagnosticSPR.setInputUnitTypeforX("");
    // diagnosticSPR.setOutputUnitType("");
    ok = setDuctStaticPressureResetCurve(diagnosticSPR);
    OS_ASSERT(ok);

    CurveExponentialSkewNormal fanEff120CPLANormal(model);
    fanEff120CPLANormal.setName("FanEff120CPLANormal");
    fanEff120CPLANormal.setCoefficient1C1(0.072613);
    fanEff120CPLANormal.setCoefficient2C2(0.833213);
    fanEff120CPLANormal.setCoefficient3C3(0.0);
    fanEff120CPLANormal.setCoefficient4C4(0.013911);
    fanEff120CPLANormal.setMinimumValueofx(-4.0);
    fanEff120CPLANormal.setMaximumValueofx(5.0);
    fanEff120CPLANormal.setMinimumCurveOutput(0.1);
    fanEff120CPLANormal.setMaximumCurveOutput(1.0);
    // fanEff120CPLANormal.setInputUnitTypeforx("");
    // fanEff120CPLANormal.setOutputUnitType("");
    ok = setNormalizedFanStaticEfficiencyCurveNonStallRegion(fanEff120CPLANormal);
    OS_ASSERT(ok);

    CurveExponentialSkewNormal fanEff120CPLAStall(model);
    fanEff120CPLAStall.setName("FanEff120CPLAStall");
    fanEff120CPLAStall.setCoefficient1C1(-1.674931);
    fanEff120CPLAStall.setCoefficient2C2(1.980182);
    fanEff120CPLAStall.setCoefficient3C3(0.0);
    fanEff120CPLAStall.setCoefficient4C4(1.84495);
    fanEff120CPLAStall.setMinimumValueofx(-4.0);
    fanEff120CPLAStall.setMaximumValueofx(5.0);
    fanEff120CPLAStall.setMinimumCurveOutput(0.1);
    fanEff120CPLAStall.setMaximumCurveOutput(1.0);
    // fanEff120CPLAStall.setInputUnitTypeforx("");
    // fanEff120CPLAStall.setOutputUnitType("");
    ok = setNormalizedFanStaticEfficiencyCurveStallRegion(fanEff120CPLAStall);
    OS_ASSERT(ok);

    CurveSigmoid fanDimFlowNormal(model);
    fanDimFlowNormal.setName("FanDimFlowNormal");
    fanDimFlowNormal.setCoefficient1C1(0.0);
    fanDimFlowNormal.setCoefficient2C2(1.001423);
    fanDimFlowNormal.setCoefficient3C3(0.123935);
    fanDimFlowNormal.setCoefficient4C4(-0.476026);
    fanDimFlowNormal.setCoefficient5C5(1.0);
    fanDimFlowNormal.setMinimumValueofx(-4.0);
    fanDimFlowNormal.setMaximumValueofx(5.0);
    fanDimFlowNormal.setMinimumCurveOutput(0.05);
    fanDimFlowNormal.setMaximumCurveOutput(1.0);
    // fanDimFlowNormal.setInputUnitTypeforx("");
    // fanDimFlowNormal.setOutputUnitType("");
    ok = setNormalizedDimensionlessAirflowCurveNonStallRegion(fanDimFlowNormal);
    OS_ASSERT(ok);

    CurveSigmoid fanDimFlowStall(model);
    fanDimFlowStall.setName("FanDimFlowStall");
    fanDimFlowStall.setCoefficient1C1(0.0);
    fanDimFlowStall.setCoefficient2C2(5.924993);
    fanDimFlowStall.setCoefficient3C3(-1.91636);
    fanDimFlowStall.setCoefficient4C4(-0.851779);
    fanDimFlowStall.setCoefficient5C5(1.0);
    fanDimFlowStall.setMinimumValueofx(-4.0);
    fanDimFlowStall.setMaximumValueofx(5.0);
    fanDimFlowStall.setMinimumCurveOutput(0.05);
    fanDimFlowStall.setMaximumCurveOutput(1.0);
    // fanDimFlowStall.setInputUnitTypeforx("");
    // fanDimFlowStall.setOutputUnitType("");
    ok = setNormalizedDimensionlessAirflowCurveStallRegion(fanDimFlowStall);
    OS_ASSERT(ok);

    // Optional curves: Not initialized either
    //ok = assignDefaultOptionalCurves();
    //OS_ASSERT(ok);
  }

  FanComponentModel::FanComponentModel(const Model& model, const Curve& fanPressureRiseCurve, const Curve& ductStaticPressureResetCurve,
                                       const Curve& normalizedFanStaticEfficiencyCurveNonStallRegion,
                                       const Curve& normalizedFanStaticEfficiencyCurveStallRegion,
                                       const Curve& normalizedDimensionlessAirflowCurveNonStallRegion,
                                       const Curve& normalizedDimensionlessAirflowCurveStallRegion)
    : StraightComponent(FanComponentModel::iddObjectType(), model) {

    OS_ASSERT(getImpl<detail::FanComponentModel_Impl>());

    // Appropriately handle the following required object-list fields.
    //     OS_Fan_ComponentModelFields::AirInletNodeName
    //     OS_Fan_ComponentModelFields::AirOutletNodeName
    //     OS_Fan_ComponentModelFields::AvailabilityScheduleName
    //     OS_Fan_ComponentModelFields::FanPressureRiseCurveName
    //     OS_Fan_ComponentModelFields::DuctStaticPressureResetCurveName
    //     OS_Fan_ComponentModelFields::NormalizedFanStaticEfficiencyCurveNameNonStallRegion
    //     OS_Fan_ComponentModelFields::NormalizedFanStaticEfficiencyCurveNameStallRegion
    //     OS_Fan_ComponentModelFields::NormalizedDimensionlessAirflowCurveNameNonStallRegion
    //     OS_Fan_ComponentModelFields::NormalizedDimensionlessAirflowCurveNameStallRegion
    bool ok = true;

    // ok = setAirInletNode();
    // ok = setAirOutletNode();

    auto availabilitySchedule = model.alwaysOnDiscreteSchedule();
    ok = setAvailabilitySchedule(availabilitySchedule);
    OS_ASSERT(ok);

    // E+ IDD default
    autosizeMaximumFlowRate();
    autosizeMinimumFlowRate();
    ok = setFanSizingFactor(1.0);
    OS_ASSERT(ok);
    // this has an IDD default of 1.0, but it's autosizable
    //ok = setMotorFanPulleyRatio(1.0);
    // OS_ASSERT(ok);
    autosizeMotorFanPulleyRatio();

    ok = setBeltSizingFactor(1.0);
    OS_ASSERT(ok);
    ok = setBeltFractionalTorqueTransition(0.167);
    OS_ASSERT(ok);
    ok = setMotorSizingFactor(1.0);
    OS_ASSERT(ok);
    ok = setMotorInAirstreamFraction(1.0);
    OS_ASSERT(ok);
    ok = setVFDSizingFactor(1.0);
    OS_ASSERT(ok);
    ok = setEndUseSubcategory("General");
    OS_ASSERT(ok);

    // Default from E+ example file: EnergyPlus/testfiles/5ZoneWarmestVFD_FCMAuto.idf, v9.4.0
    ok = setFanWheelDiameter(0.3048);
    OS_ASSERT(ok);
    ok = setFanOutletArea(0.0873288576);
    OS_ASSERT(ok);
    ok = setMaximumFanStaticEfficiency(0.514);
    OS_ASSERT(ok);
    ok = setEulerNumberatMaximumFanStaticEfficiency(9.76);
    OS_ASSERT(ok);
    ok = setMaximumDimensionlessFanAirflow(0.160331811647483);
    OS_ASSERT(ok);

    autosizeBeltMaximumTorque();

    ok = setMotorMaximumSpeed(1800.0);
    OS_ASSERT(ok);

    autosizeMaximumMotorOutputPower();

    autosizeMaximumVFDOutputPower();

    setVFDEfficiencyType("Power");

    // Required Curves
    ok = setFanPressureRiseCurve(fanPressureRiseCurve);
    OS_ASSERT(ok);

    ok = setDuctStaticPressureResetCurve(ductStaticPressureResetCurve);
    OS_ASSERT(ok);

    ok = setNormalizedFanStaticEfficiencyCurveNonStallRegion(normalizedFanStaticEfficiencyCurveNonStallRegion);
    OS_ASSERT(ok);

    ok = setNormalizedFanStaticEfficiencyCurveStallRegion(normalizedFanStaticEfficiencyCurveStallRegion);
    OS_ASSERT(ok);

    ok = setNormalizedDimensionlessAirflowCurveNonStallRegion(normalizedDimensionlessAirflowCurveNonStallRegion);
    OS_ASSERT(ok);

    ok = setNormalizedDimensionlessAirflowCurveStallRegion(normalizedDimensionlessAirflowCurveStallRegion);
    OS_ASSERT(ok);

    // Note: If the user is calling this explicit Ctor, they probably are trying to avoid creating unecessary curves,
    // so we are respectful of that and therefore not instantiating the optional curves
    // ok = assignDefaultOptionalCurves();
    // OS_ASSERT(ok);
  }

  IddObjectType FanComponentModel::iddObjectType() {
    return {IddObjectType::OS_Fan_ComponentModel};
  }

  std::vector<std::string> FanComponentModel::vFDEfficiencyTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Fan_ComponentModelFields::VFDEfficiencyType);
  }

  Schedule FanComponentModel::availabilitySchedule() const {
    return getImpl<detail::FanComponentModel_Impl>()->availabilitySchedule();
  }

  boost::optional<double> FanComponentModel::maximumFlowRate() const {
    return getImpl<detail::FanComponentModel_Impl>()->maximumFlowRate();
  }

  bool FanComponentModel::isMaximumFlowRateAutosized() const {
    return getImpl<detail::FanComponentModel_Impl>()->isMaximumFlowRateAutosized();
  }

  boost::optional<double> FanComponentModel::autosizedMaximumFlowRate() {
    return getImpl<detail::FanComponentModel_Impl>()->autosizedMaximumFlowRate();
  }

  boost::optional<double> FanComponentModel::minimumFlowRate() const {
    return getImpl<detail::FanComponentModel_Impl>()->minimumFlowRate();
  }

  bool FanComponentModel::isMinimumFlowRateAutosized() const {
    return getImpl<detail::FanComponentModel_Impl>()->isMinimumFlowRateAutosized();
  }

  boost::optional<double> FanComponentModel::autosizedMinimumFlowRate() {
    return getImpl<detail::FanComponentModel_Impl>()->autosizedMinimumFlowRate();
  }

  double FanComponentModel::fanSizingFactor() const {
    return getImpl<detail::FanComponentModel_Impl>()->fanSizingFactor();
  }

  double FanComponentModel::fanWheelDiameter() const {
    return getImpl<detail::FanComponentModel_Impl>()->fanWheelDiameter();
  }

  double FanComponentModel::fanOutletArea() const {
    return getImpl<detail::FanComponentModel_Impl>()->fanOutletArea();
  }

  double FanComponentModel::maximumFanStaticEfficiency() const {
    return getImpl<detail::FanComponentModel_Impl>()->maximumFanStaticEfficiency();
  }

  double FanComponentModel::eulerNumberatMaximumFanStaticEfficiency() const {
    return getImpl<detail::FanComponentModel_Impl>()->eulerNumberatMaximumFanStaticEfficiency();
  }

  double FanComponentModel::maximumDimensionlessFanAirflow() const {
    return getImpl<detail::FanComponentModel_Impl>()->maximumDimensionlessFanAirflow();
  }

  boost::optional<double> FanComponentModel::motorFanPulleyRatio() const {
    return getImpl<detail::FanComponentModel_Impl>()->motorFanPulleyRatio();
  }

  bool FanComponentModel::isMotorFanPulleyRatioAutosized() const {
    return getImpl<detail::FanComponentModel_Impl>()->isMotorFanPulleyRatioAutosized();
  }

  boost::optional<double> FanComponentModel::autosizedMotorFanPulleyRatio() {
    return getImpl<detail::FanComponentModel_Impl>()->autosizedMotorFanPulleyRatio();
  }

  boost::optional<double> FanComponentModel::beltMaximumTorque() const {
    return getImpl<detail::FanComponentModel_Impl>()->beltMaximumTorque();
  }

  bool FanComponentModel::isBeltMaximumTorqueAutosized() const {
    return getImpl<detail::FanComponentModel_Impl>()->isBeltMaximumTorqueAutosized();
  }

  boost::optional<double> FanComponentModel::autosizedBeltMaximumTorque() {
    return getImpl<detail::FanComponentModel_Impl>()->autosizedBeltMaximumTorque();
  }

  double FanComponentModel::beltSizingFactor() const {
    return getImpl<detail::FanComponentModel_Impl>()->beltSizingFactor();
  }

  double FanComponentModel::beltFractionalTorqueTransition() const {
    return getImpl<detail::FanComponentModel_Impl>()->beltFractionalTorqueTransition();
  }

  double FanComponentModel::motorMaximumSpeed() const {
    return getImpl<detail::FanComponentModel_Impl>()->motorMaximumSpeed();
  }

  boost::optional<double> FanComponentModel::maximumMotorOutputPower() const {
    return getImpl<detail::FanComponentModel_Impl>()->maximumMotorOutputPower();
  }

  bool FanComponentModel::isMaximumMotorOutputPowerAutosized() const {
    return getImpl<detail::FanComponentModel_Impl>()->isMaximumMotorOutputPowerAutosized();
  }

  boost::optional<double> FanComponentModel::autosizedMaximumMotorOutputPower() {
    return getImpl<detail::FanComponentModel_Impl>()->autosizedMaximumMotorOutputPower();
  }

  double FanComponentModel::motorSizingFactor() const {
    return getImpl<detail::FanComponentModel_Impl>()->motorSizingFactor();
  }

  double FanComponentModel::motorInAirstreamFraction() const {
    return getImpl<detail::FanComponentModel_Impl>()->motorInAirstreamFraction();
  }

  std::string FanComponentModel::vFDEfficiencyType() const {
    return getImpl<detail::FanComponentModel_Impl>()->vFDEfficiencyType();
  }

  boost::optional<double> FanComponentModel::maximumVFDOutputPower() const {
    return getImpl<detail::FanComponentModel_Impl>()->maximumVFDOutputPower();
  }

  bool FanComponentModel::isMaximumVFDOutputPowerAutosized() const {
    return getImpl<detail::FanComponentModel_Impl>()->isMaximumVFDOutputPowerAutosized();
  }

  boost::optional<double> FanComponentModel::autosizedMaximumVFDOutputPower() {
    return getImpl<detail::FanComponentModel_Impl>()->autosizedMaximumVFDOutputPower();
  }

  double FanComponentModel::vFDSizingFactor() const {
    return getImpl<detail::FanComponentModel_Impl>()->vFDSizingFactor();
  }

  Curve FanComponentModel::fanPressureRiseCurve() const {
    return getImpl<detail::FanComponentModel_Impl>()->fanPressureRiseCurve();
  }

  Curve FanComponentModel::ductStaticPressureResetCurve() const {
    return getImpl<detail::FanComponentModel_Impl>()->ductStaticPressureResetCurve();
  }

  Curve FanComponentModel::normalizedFanStaticEfficiencyCurveNonStallRegion() const {
    return getImpl<detail::FanComponentModel_Impl>()->normalizedFanStaticEfficiencyCurveNonStallRegion();
  }

  Curve FanComponentModel::normalizedFanStaticEfficiencyCurveStallRegion() const {
    return getImpl<detail::FanComponentModel_Impl>()->normalizedFanStaticEfficiencyCurveStallRegion();
  }

  Curve FanComponentModel::normalizedDimensionlessAirflowCurveNonStallRegion() const {
    return getImpl<detail::FanComponentModel_Impl>()->normalizedDimensionlessAirflowCurveNonStallRegion();
  }

  Curve FanComponentModel::normalizedDimensionlessAirflowCurveStallRegion() const {
    return getImpl<detail::FanComponentModel_Impl>()->normalizedDimensionlessAirflowCurveStallRegion();
  }

  boost::optional<Curve> FanComponentModel::maximumBeltEfficiencyCurve() const {
    return getImpl<detail::FanComponentModel_Impl>()->maximumBeltEfficiencyCurve();
  }

  boost::optional<Curve> FanComponentModel::normalizedBeltEfficiencyCurveRegion1() const {
    return getImpl<detail::FanComponentModel_Impl>()->normalizedBeltEfficiencyCurveRegion1();
  }

  boost::optional<Curve> FanComponentModel::normalizedBeltEfficiencyCurveRegion2() const {
    return getImpl<detail::FanComponentModel_Impl>()->normalizedBeltEfficiencyCurveRegion2();
  }

  boost::optional<Curve> FanComponentModel::normalizedBeltEfficiencyCurveRegion3() const {
    return getImpl<detail::FanComponentModel_Impl>()->normalizedBeltEfficiencyCurveRegion3();
  }

  boost::optional<Curve> FanComponentModel::maximumMotorEfficiencyCurve() const {
    return getImpl<detail::FanComponentModel_Impl>()->maximumMotorEfficiencyCurve();
  }

  boost::optional<Curve> FanComponentModel::normalizedMotorEfficiencyCurve() const {
    return getImpl<detail::FanComponentModel_Impl>()->normalizedMotorEfficiencyCurve();
  }

  boost::optional<Curve> FanComponentModel::vFDEfficiencyCurve() const {
    return getImpl<detail::FanComponentModel_Impl>()->vFDEfficiencyCurve();
  }

  std::string FanComponentModel::endUseSubcategory() const {
    return getImpl<detail::FanComponentModel_Impl>()->endUseSubcategory();
  }

  bool FanComponentModel::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::FanComponentModel_Impl>()->setAvailabilitySchedule(schedule);
  }

  bool FanComponentModel::setMaximumFlowRate(double maximumFlowRate) {
    return getImpl<detail::FanComponentModel_Impl>()->setMaximumFlowRate(maximumFlowRate);
  }

  void FanComponentModel::autosizeMaximumFlowRate() {
    getImpl<detail::FanComponentModel_Impl>()->autosizeMaximumFlowRate();
  }

  bool FanComponentModel::setMinimumFlowRate(double minimumFlowRate) {
    return getImpl<detail::FanComponentModel_Impl>()->setMinimumFlowRate(minimumFlowRate);
  }

  void FanComponentModel::autosizeMinimumFlowRate() {
    getImpl<detail::FanComponentModel_Impl>()->autosizeMinimumFlowRate();
  }

  bool FanComponentModel::setFanSizingFactor(double fanSizingFactor) {
    return getImpl<detail::FanComponentModel_Impl>()->setFanSizingFactor(fanSizingFactor);
  }

  bool FanComponentModel::setFanWheelDiameter(double fanWheelDiameter) {
    return getImpl<detail::FanComponentModel_Impl>()->setFanWheelDiameter(fanWheelDiameter);
  }

  bool FanComponentModel::setFanOutletArea(double fanOutletArea) {
    return getImpl<detail::FanComponentModel_Impl>()->setFanOutletArea(fanOutletArea);
  }

  bool FanComponentModel::setMaximumFanStaticEfficiency(double maximumFanStaticEfficiency) {
    return getImpl<detail::FanComponentModel_Impl>()->setMaximumFanStaticEfficiency(maximumFanStaticEfficiency);
  }

  bool FanComponentModel::setEulerNumberatMaximumFanStaticEfficiency(double eulerNumberatMaximumFanStaticEfficiency) {
    return getImpl<detail::FanComponentModel_Impl>()->setEulerNumberatMaximumFanStaticEfficiency(eulerNumberatMaximumFanStaticEfficiency);
  }

  bool FanComponentModel::setMaximumDimensionlessFanAirflow(double maximumDimensionlessFanAirflow) {
    return getImpl<detail::FanComponentModel_Impl>()->setMaximumDimensionlessFanAirflow(maximumDimensionlessFanAirflow);
  }

  bool FanComponentModel::setMotorFanPulleyRatio(double motorFanPulleyRatio) {
    return getImpl<detail::FanComponentModel_Impl>()->setMotorFanPulleyRatio(motorFanPulleyRatio);
  }

  void FanComponentModel::autosizeMotorFanPulleyRatio() {
    getImpl<detail::FanComponentModel_Impl>()->autosizeMotorFanPulleyRatio();
  }

  bool FanComponentModel::setBeltMaximumTorque(double beltMaximumTorque) {
    return getImpl<detail::FanComponentModel_Impl>()->setBeltMaximumTorque(beltMaximumTorque);
  }

  void FanComponentModel::autosizeBeltMaximumTorque() {
    getImpl<detail::FanComponentModel_Impl>()->autosizeBeltMaximumTorque();
  }

  bool FanComponentModel::setBeltSizingFactor(double beltSizingFactor) {
    return getImpl<detail::FanComponentModel_Impl>()->setBeltSizingFactor(beltSizingFactor);
  }

  bool FanComponentModel::setBeltFractionalTorqueTransition(double beltFractionalTorqueTransition) {
    return getImpl<detail::FanComponentModel_Impl>()->setBeltFractionalTorqueTransition(beltFractionalTorqueTransition);
  }

  bool FanComponentModel::setMotorMaximumSpeed(double motorMaximumSpeed) {
    return getImpl<detail::FanComponentModel_Impl>()->setMotorMaximumSpeed(motorMaximumSpeed);
  }

  bool FanComponentModel::setMaximumMotorOutputPower(double maximumMotorOutputPower) {
    return getImpl<detail::FanComponentModel_Impl>()->setMaximumMotorOutputPower(maximumMotorOutputPower);
  }

  void FanComponentModel::autosizeMaximumMotorOutputPower() {
    getImpl<detail::FanComponentModel_Impl>()->autosizeMaximumMotorOutputPower();
  }

  bool FanComponentModel::setMotorSizingFactor(double motorSizingFactor) {
    return getImpl<detail::FanComponentModel_Impl>()->setMotorSizingFactor(motorSizingFactor);
  }

  bool FanComponentModel::setMotorInAirstreamFraction(double motorInAirstreamFraction) {
    return getImpl<detail::FanComponentModel_Impl>()->setMotorInAirstreamFraction(motorInAirstreamFraction);
  }

  bool FanComponentModel::setVFDEfficiencyType(const std::string& vFDEfficiencyType) {
    return getImpl<detail::FanComponentModel_Impl>()->setVFDEfficiencyType(vFDEfficiencyType);
  }

  bool FanComponentModel::setMaximumVFDOutputPower(double maximumVFDOutputPower) {
    return getImpl<detail::FanComponentModel_Impl>()->setMaximumVFDOutputPower(maximumVFDOutputPower);
  }

  void FanComponentModel::autosizeMaximumVFDOutputPower() {
    getImpl<detail::FanComponentModel_Impl>()->autosizeMaximumVFDOutputPower();
  }

  bool FanComponentModel::setVFDSizingFactor(double vFDSizingFactor) {
    return getImpl<detail::FanComponentModel_Impl>()->setVFDSizingFactor(vFDSizingFactor);
  }

  bool FanComponentModel::setFanPressureRiseCurve(const Curve& bivariateFunctions) {
    return getImpl<detail::FanComponentModel_Impl>()->setFanPressureRiseCurve(bivariateFunctions);
  }

  bool FanComponentModel::setDuctStaticPressureResetCurve(const Curve& univariateFunctions) {
    return getImpl<detail::FanComponentModel_Impl>()->setDuctStaticPressureResetCurve(univariateFunctions);
  }

  bool FanComponentModel::setNormalizedFanStaticEfficiencyCurveNonStallRegion(const Curve& univariateFunctions) {
    return getImpl<detail::FanComponentModel_Impl>()->setNormalizedFanStaticEfficiencyCurveNonStallRegion(univariateFunctions);
  }

  bool FanComponentModel::setNormalizedFanStaticEfficiencyCurveStallRegion(const Curve& univariateFunctions) {
    return getImpl<detail::FanComponentModel_Impl>()->setNormalizedFanStaticEfficiencyCurveStallRegion(univariateFunctions);
  }

  bool FanComponentModel::setNormalizedDimensionlessAirflowCurveNonStallRegion(const Curve& univariateFunctions) {
    return getImpl<detail::FanComponentModel_Impl>()->setNormalizedDimensionlessAirflowCurveNonStallRegion(univariateFunctions);
  }

  bool FanComponentModel::setNormalizedDimensionlessAirflowCurveStallRegion(const Curve& univariateFunctions) {
    return getImpl<detail::FanComponentModel_Impl>()->setNormalizedDimensionlessAirflowCurveStallRegion(univariateFunctions);
  }

  bool FanComponentModel::setMaximumBeltEfficiencyCurve(const Curve& univariateFunctions) {
    return getImpl<detail::FanComponentModel_Impl>()->setMaximumBeltEfficiencyCurve(univariateFunctions);
  }

  void FanComponentModel::resetMaximumBeltEfficiencyCurve() {
    getImpl<detail::FanComponentModel_Impl>()->resetMaximumBeltEfficiencyCurve();
  }

  bool FanComponentModel::setNormalizedBeltEfficiencyCurveRegion1(const Curve& univariateFunctions) {
    return getImpl<detail::FanComponentModel_Impl>()->setNormalizedBeltEfficiencyCurveRegion1(univariateFunctions);
  }

  void FanComponentModel::resetNormalizedBeltEfficiencyCurveRegion1() {
    getImpl<detail::FanComponentModel_Impl>()->resetNormalizedBeltEfficiencyCurveRegion1();
  }

  bool FanComponentModel::setNormalizedBeltEfficiencyCurveRegion2(const Curve& univariateFunctions) {
    return getImpl<detail::FanComponentModel_Impl>()->setNormalizedBeltEfficiencyCurveRegion2(univariateFunctions);
  }

  void FanComponentModel::resetNormalizedBeltEfficiencyCurveRegion2() {
    getImpl<detail::FanComponentModel_Impl>()->resetNormalizedBeltEfficiencyCurveRegion2();
  }

  bool FanComponentModel::setNormalizedBeltEfficiencyCurveRegion3(const Curve& univariateFunctions) {
    return getImpl<detail::FanComponentModel_Impl>()->setNormalizedBeltEfficiencyCurveRegion3(univariateFunctions);
  }

  void FanComponentModel::resetNormalizedBeltEfficiencyCurveRegion3() {
    getImpl<detail::FanComponentModel_Impl>()->resetNormalizedBeltEfficiencyCurveRegion3();
  }

  bool FanComponentModel::setMaximumMotorEfficiencyCurve(const Curve& univariateFunctions) {
    return getImpl<detail::FanComponentModel_Impl>()->setMaximumMotorEfficiencyCurve(univariateFunctions);
  }

  void FanComponentModel::resetMaximumMotorEfficiencyCurve() {
    getImpl<detail::FanComponentModel_Impl>()->resetMaximumMotorEfficiencyCurve();
  }

  bool FanComponentModel::setNormalizedMotorEfficiencyCurve(const Curve& univariateFunctions) {
    return getImpl<detail::FanComponentModel_Impl>()->setNormalizedMotorEfficiencyCurve(univariateFunctions);
  }

  void FanComponentModel::resetNormalizedMotorEfficiencyCurve() {
    getImpl<detail::FanComponentModel_Impl>()->resetNormalizedMotorEfficiencyCurve();
  }

  bool FanComponentModel::setVFDEfficiencyCurve(const Curve& univariateFunctions) {
    return getImpl<detail::FanComponentModel_Impl>()->setVFDEfficiencyCurve(univariateFunctions);
  }

  void FanComponentModel::resetVFDEfficiencyCurve() {
    getImpl<detail::FanComponentModel_Impl>()->resetVFDEfficiencyCurve();
  }

  bool FanComponentModel::setEndUseSubcategory(const std::string& endUseSubcategory) {
    return getImpl<detail::FanComponentModel_Impl>()->setEndUseSubcategory(endUseSubcategory);
  }

  /// @cond
  FanComponentModel::FanComponentModel(std::shared_ptr<detail::FanComponentModel_Impl> impl) : StraightComponent(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
