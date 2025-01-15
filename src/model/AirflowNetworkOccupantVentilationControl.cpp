/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "AirflowNetworkOccupantVentilationControl.hpp"
#include "AirflowNetworkOccupantVentilationControl_Impl.hpp"

#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AirflowNetworkOccupantVentilationControl_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    AirflowNetworkOccupantVentilationControl_Impl::AirflowNetworkOccupantVentilationControl_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                                 bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AirflowNetworkOccupantVentilationControl::iddObjectType());
    }

    AirflowNetworkOccupantVentilationControl_Impl::AirflowNetworkOccupantVentilationControl_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AirflowNetworkOccupantVentilationControl::iddObjectType());
    }

    AirflowNetworkOccupantVentilationControl_Impl::AirflowNetworkOccupantVentilationControl_Impl(
      const AirflowNetworkOccupantVentilationControl_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& AirflowNetworkOccupantVentilationControl_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType AirflowNetworkOccupantVentilationControl_Impl::iddObjectType() const {
      return AirflowNetworkOccupantVentilationControl::iddObjectType();
    }

    std::vector<ScheduleTypeKey> AirflowNetworkOccupantVentilationControl_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_AirflowNetworkOccupantVentilationControlFields::OpeningProbabilityScheduleName) != e) {
        result.emplace_back("AirflowNetworkOccupantVentilationControl", "Opening Probability");
      }
      if (std::find(b, e, OS_AirflowNetworkOccupantVentilationControlFields::ClosingProbabilityScheduleName) != e) {
        result.emplace_back("AirflowNetworkOccupantVentilationControl", "Closing Probability");
      }
      return result;
    }

    double AirflowNetworkOccupantVentilationControl_Impl::minimumOpeningTime() const {
      boost::optional<double> value = getDouble(OS_AirflowNetworkOccupantVentilationControlFields::MinimumOpeningTime, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirflowNetworkOccupantVentilationControl_Impl::isMinimumOpeningTimeDefaulted() const {
      return isEmpty(OS_AirflowNetworkOccupantVentilationControlFields::MinimumOpeningTime);
    }

    double AirflowNetworkOccupantVentilationControl_Impl::minimumClosingTime() const {
      boost::optional<double> value = getDouble(OS_AirflowNetworkOccupantVentilationControlFields::MinimumClosingTime, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirflowNetworkOccupantVentilationControl_Impl::isMinimumClosingTimeDefaulted() const {
      return isEmpty(OS_AirflowNetworkOccupantVentilationControlFields::MinimumClosingTime);
    }

    Curve AirflowNetworkOccupantVentilationControl_Impl::thermalComfortLowTemperatureCurve() const {
      boost::optional<Curve> value = getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirflowNetworkOccupantVentilationControlFields::ThermalComfortLowTemperatureCurveName);
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Thermal Comfort Low Temperature Curve attached.");
      }
      return value.get();
    }

    double AirflowNetworkOccupantVentilationControl_Impl::thermalComfortTemperatureBoundaryPoint() const {
      boost::optional<double> value = getDouble(OS_AirflowNetworkOccupantVentilationControlFields::ThermalComfortTemperatureBoundaryPoint, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirflowNetworkOccupantVentilationControl_Impl::isThermalComfortTemperatureBoundaryPointDefaulted() const {
      return isEmpty(OS_AirflowNetworkOccupantVentilationControlFields::ThermalComfortTemperatureBoundaryPoint);
    }

    boost::optional<Curve> AirflowNetworkOccupantVentilationControl_Impl::thermalComfortHighTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirflowNetworkOccupantVentilationControlFields::ThermalComfortHighTemperatureCurveName);
    }

    double AirflowNetworkOccupantVentilationControl_Impl::maximumPredictedPercentageofDissatisfiedThreshold() const {
      boost::optional<double> value =
        getDouble(OS_AirflowNetworkOccupantVentilationControlFields::MaximumPredictedPercentageofDissatisfiedThreshold, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirflowNetworkOccupantVentilationControl_Impl::isMaximumPredictedPercentageofDissatisfiedThresholdDefaulted() const {
      return isEmpty(OS_AirflowNetworkOccupantVentilationControlFields::MaximumPredictedPercentageofDissatisfiedThreshold);
    }

    bool AirflowNetworkOccupantVentilationControl_Impl::occupancyCheck() const {
      boost::optional<std::string> value = getString(OS_AirflowNetworkOccupantVentilationControlFields::OccupancyCheck, true);
      OS_ASSERT(value);
      return openstudio::istringEqual(value.get(), "Yes");
    }

    bool AirflowNetworkOccupantVentilationControl_Impl::isOccupancyCheckDefaulted() const {
      return isEmpty(OS_AirflowNetworkOccupantVentilationControlFields::OccupancyCheck);
    }

    boost::optional<Schedule> AirflowNetworkOccupantVentilationControl_Impl::openingProbabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(
        OS_AirflowNetworkOccupantVentilationControlFields::OpeningProbabilityScheduleName);
    }

    boost::optional<Schedule> AirflowNetworkOccupantVentilationControl_Impl::closingProbabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(
        OS_AirflowNetworkOccupantVentilationControlFields::ClosingProbabilityScheduleName);
    }

    bool AirflowNetworkOccupantVentilationControl_Impl::setMinimumOpeningTime(double minimumOpeningTime) {
      bool result = setDouble(OS_AirflowNetworkOccupantVentilationControlFields::MinimumOpeningTime, minimumOpeningTime);
      return result;
    }

    void AirflowNetworkOccupantVentilationControl_Impl::resetMinimumOpeningTime() {
      bool result = setString(OS_AirflowNetworkOccupantVentilationControlFields::MinimumOpeningTime, "");
      OS_ASSERT(result);
    }

    bool AirflowNetworkOccupantVentilationControl_Impl::setMinimumClosingTime(double minimumClosingTime) {
      bool result = setDouble(OS_AirflowNetworkOccupantVentilationControlFields::MinimumClosingTime, minimumClosingTime);
      return result;
    }

    void AirflowNetworkOccupantVentilationControl_Impl::resetMinimumClosingTime() {
      bool result = setString(OS_AirflowNetworkOccupantVentilationControlFields::MinimumClosingTime, "");
      OS_ASSERT(result);
    }

    bool AirflowNetworkOccupantVentilationControl_Impl::setThermalComfortLowTemperatureCurve(const Curve& curve) {
      bool result = setPointer(OS_AirflowNetworkOccupantVentilationControlFields::ThermalComfortLowTemperatureCurveName, curve.handle());
      return result;
    }

    //void AirflowNetworkOccupantVentilationControl_Impl::resetThermalComfortLowTemperatureCurve()
    //{
    //  bool result = setString(OS_AirflowNetworkOccupantVentilationControlFields::ThermalComfortLowTemperatureCurveName, "");
    //  OS_ASSERT(result);
    //}

    bool AirflowNetworkOccupantVentilationControl_Impl::setThermalComfortTemperatureBoundaryPoint(double thermalComfortTemperatureBoundaryPoint) {
      bool result =
        setDouble(OS_AirflowNetworkOccupantVentilationControlFields::ThermalComfortTemperatureBoundaryPoint, thermalComfortTemperatureBoundaryPoint);
      return result;
    }

    void AirflowNetworkOccupantVentilationControl_Impl::resetThermalComfortTemperatureBoundaryPoint() {
      bool result = setString(OS_AirflowNetworkOccupantVentilationControlFields::ThermalComfortTemperatureBoundaryPoint, "");
      OS_ASSERT(result);
    }

    bool AirflowNetworkOccupantVentilationControl_Impl::setThermalComfortHighTemperatureCurve(const Curve& curve) {
      bool result = setPointer(OS_AirflowNetworkOccupantVentilationControlFields::ThermalComfortHighTemperatureCurveName, curve.handle());
      return result;
    }

    void AirflowNetworkOccupantVentilationControl_Impl::resetThermalComfortHighTemperatureCurve() {
      bool result = setString(OS_AirflowNetworkOccupantVentilationControlFields::ThermalComfortHighTemperatureCurveName, "");
      OS_ASSERT(result);
    }

    bool AirflowNetworkOccupantVentilationControl_Impl::setMaximumPredictedPercentageofDissatisfiedThreshold(
      double maximumPredictedPercentageofDissatisfiedThreshold) {
      bool result = setDouble(OS_AirflowNetworkOccupantVentilationControlFields::MaximumPredictedPercentageofDissatisfiedThreshold,
                              maximumPredictedPercentageofDissatisfiedThreshold);
      return result;
    }

    void AirflowNetworkOccupantVentilationControl_Impl::resetMaximumPredictedPercentageofDissatisfiedThreshold() {
      bool result = setString(OS_AirflowNetworkOccupantVentilationControlFields::MaximumPredictedPercentageofDissatisfiedThreshold, "");
      OS_ASSERT(result);
    }

    void AirflowNetworkOccupantVentilationControl_Impl::setOccupancyCheck(bool occupancyCheck) {
      bool result = false;
      if (occupancyCheck) {
        result = setString(OS_AirflowNetworkOccupantVentilationControlFields::OccupancyCheck, "Yes");
      } else {
        result = setString(OS_AirflowNetworkOccupantVentilationControlFields::OccupancyCheck, "No");
      }
      OS_ASSERT(result);
    }

    void AirflowNetworkOccupantVentilationControl_Impl::resetOccupancyCheck() {
      bool result = setString(OS_AirflowNetworkOccupantVentilationControlFields::OccupancyCheck, "");
      OS_ASSERT(result);
    }

    bool AirflowNetworkOccupantVentilationControl_Impl::setOpeningProbabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_AirflowNetworkOccupantVentilationControlFields::OpeningProbabilityScheduleName,
                                "AirflowNetworkOccupantVentilationControl", "Opening Probability", schedule);
      return result;
    }

    void AirflowNetworkOccupantVentilationControl_Impl::resetOpeningProbabilitySchedule() {
      bool result = setString(OS_AirflowNetworkOccupantVentilationControlFields::OpeningProbabilityScheduleName, "");
      OS_ASSERT(result);
    }

    bool AirflowNetworkOccupantVentilationControl_Impl::setClosingProbabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_AirflowNetworkOccupantVentilationControlFields::ClosingProbabilityScheduleName,
                                "AirflowNetworkOccupantVentilationControl", "Closing Probability", schedule);
      return result;
    }

    void AirflowNetworkOccupantVentilationControl_Impl::resetClosingProbabilitySchedule() {
      bool result = setString(OS_AirflowNetworkOccupantVentilationControlFields::ClosingProbabilityScheduleName, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  AirflowNetworkOccupantVentilationControl::AirflowNetworkOccupantVentilationControl(const Model& model, const Curve& curve)
    : ModelObject(AirflowNetworkOccupantVentilationControl::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirflowNetworkOccupantVentilationControl_Impl>());
    bool ok = setThermalComfortLowTemperatureCurve(curve);
    OS_ASSERT(ok);
  }

  IddObjectType AirflowNetworkOccupantVentilationControl::iddObjectType() {
    return {IddObjectType::OS_AirflowNetworkOccupantVentilationControl};
  }

  double AirflowNetworkOccupantVentilationControl::minimumOpeningTime() const {
    return getImpl<detail::AirflowNetworkOccupantVentilationControl_Impl>()->minimumOpeningTime();
  }

  bool AirflowNetworkOccupantVentilationControl::isMinimumOpeningTimeDefaulted() const {
    return getImpl<detail::AirflowNetworkOccupantVentilationControl_Impl>()->isMinimumOpeningTimeDefaulted();
  }

  double AirflowNetworkOccupantVentilationControl::minimumClosingTime() const {
    return getImpl<detail::AirflowNetworkOccupantVentilationControl_Impl>()->minimumClosingTime();
  }

  bool AirflowNetworkOccupantVentilationControl::isMinimumClosingTimeDefaulted() const {
    return getImpl<detail::AirflowNetworkOccupantVentilationControl_Impl>()->isMinimumClosingTimeDefaulted();
  }

  Curve AirflowNetworkOccupantVentilationControl::thermalComfortLowTemperatureCurve() const {
    return getImpl<detail::AirflowNetworkOccupantVentilationControl_Impl>()->thermalComfortLowTemperatureCurve();
  }

  double AirflowNetworkOccupantVentilationControl::thermalComfortTemperatureBoundaryPoint() const {
    return getImpl<detail::AirflowNetworkOccupantVentilationControl_Impl>()->thermalComfortTemperatureBoundaryPoint();
  }

  bool AirflowNetworkOccupantVentilationControl::isThermalComfortTemperatureBoundaryPointDefaulted() const {
    return getImpl<detail::AirflowNetworkOccupantVentilationControl_Impl>()->isThermalComfortTemperatureBoundaryPointDefaulted();
  }

  boost::optional<Curve> AirflowNetworkOccupantVentilationControl::thermalComfortHighTemperatureCurve() const {
    return getImpl<detail::AirflowNetworkOccupantVentilationControl_Impl>()->thermalComfortHighTemperatureCurve();
  }

  double AirflowNetworkOccupantVentilationControl::maximumPredictedPercentageofDissatisfiedThreshold() const {
    return getImpl<detail::AirflowNetworkOccupantVentilationControl_Impl>()->maximumPredictedPercentageofDissatisfiedThreshold();
  }

  bool AirflowNetworkOccupantVentilationControl::isMaximumPredictedPercentageofDissatisfiedThresholdDefaulted() const {
    return getImpl<detail::AirflowNetworkOccupantVentilationControl_Impl>()->isMaximumPredictedPercentageofDissatisfiedThresholdDefaulted();
  }

  bool AirflowNetworkOccupantVentilationControl::occupancyCheck() const {
    return getImpl<detail::AirflowNetworkOccupantVentilationControl_Impl>()->occupancyCheck();
  }

  bool AirflowNetworkOccupantVentilationControl::isOccupancyCheckDefaulted() const {
    return getImpl<detail::AirflowNetworkOccupantVentilationControl_Impl>()->isOccupancyCheckDefaulted();
  }

  boost::optional<Schedule> AirflowNetworkOccupantVentilationControl::openingProbabilitySchedule() const {
    return getImpl<detail::AirflowNetworkOccupantVentilationControl_Impl>()->openingProbabilitySchedule();
  }

  boost::optional<Schedule> AirflowNetworkOccupantVentilationControl::closingProbabilitySchedule() const {
    return getImpl<detail::AirflowNetworkOccupantVentilationControl_Impl>()->closingProbabilitySchedule();
  }

  bool AirflowNetworkOccupantVentilationControl::setMinimumOpeningTime(double minimumOpeningTime) {
    return getImpl<detail::AirflowNetworkOccupantVentilationControl_Impl>()->setMinimumOpeningTime(minimumOpeningTime);
  }

  void AirflowNetworkOccupantVentilationControl::resetMinimumOpeningTime() {
    getImpl<detail::AirflowNetworkOccupantVentilationControl_Impl>()->resetMinimumOpeningTime();
  }

  bool AirflowNetworkOccupantVentilationControl::setMinimumClosingTime(double minimumClosingTime) {
    return getImpl<detail::AirflowNetworkOccupantVentilationControl_Impl>()->setMinimumClosingTime(minimumClosingTime);
  }

  void AirflowNetworkOccupantVentilationControl::resetMinimumClosingTime() {
    getImpl<detail::AirflowNetworkOccupantVentilationControl_Impl>()->resetMinimumClosingTime();
  }

  bool AirflowNetworkOccupantVentilationControl::setThermalComfortLowTemperatureCurve(const Curve& curve) {
    return getImpl<detail::AirflowNetworkOccupantVentilationControl_Impl>()->setThermalComfortLowTemperatureCurve(curve);
  }

  //void AirflowNetworkOccupantVentilationControl::resetThermalComfortLowTemperatureCurve() {
  //  getImpl<detail::AirflowNetworkOccupantVentilationControl_Impl>()->resetThermalComfortLowTemperatureCurve();
  //}

  bool AirflowNetworkOccupantVentilationControl::setThermalComfortTemperatureBoundaryPoint(double thermalComfortTemperatureBoundaryPoint) {
    return getImpl<detail::AirflowNetworkOccupantVentilationControl_Impl>()->setThermalComfortTemperatureBoundaryPoint(
      thermalComfortTemperatureBoundaryPoint);
  }

  void AirflowNetworkOccupantVentilationControl::resetThermalComfortTemperatureBoundaryPoint() {
    getImpl<detail::AirflowNetworkOccupantVentilationControl_Impl>()->resetThermalComfortTemperatureBoundaryPoint();
  }

  bool AirflowNetworkOccupantVentilationControl::setThermalComfortHighTemperatureCurve(const Curve& curve) {
    return getImpl<detail::AirflowNetworkOccupantVentilationControl_Impl>()->setThermalComfortHighTemperatureCurve(curve);
  }

  void AirflowNetworkOccupantVentilationControl::resetThermalComfortHighTemperatureCurve() {
    getImpl<detail::AirflowNetworkOccupantVentilationControl_Impl>()->resetThermalComfortHighTemperatureCurve();
  }

  bool AirflowNetworkOccupantVentilationControl::setMaximumPredictedPercentageofDissatisfiedThreshold(
    double maximumPredictedPercentageofDissatisfiedThreshold) {
    return getImpl<detail::AirflowNetworkOccupantVentilationControl_Impl>()->setMaximumPredictedPercentageofDissatisfiedThreshold(
      maximumPredictedPercentageofDissatisfiedThreshold);
  }

  void AirflowNetworkOccupantVentilationControl::resetMaximumPredictedPercentageofDissatisfiedThreshold() {
    getImpl<detail::AirflowNetworkOccupantVentilationControl_Impl>()->resetMaximumPredictedPercentageofDissatisfiedThreshold();
  }

  void AirflowNetworkOccupantVentilationControl::setOccupancyCheck(bool occupancyCheck) {
    getImpl<detail::AirflowNetworkOccupantVentilationControl_Impl>()->setOccupancyCheck(occupancyCheck);
  }

  void AirflowNetworkOccupantVentilationControl::resetOccupancyCheck() {
    getImpl<detail::AirflowNetworkOccupantVentilationControl_Impl>()->resetOccupancyCheck();
  }

  bool AirflowNetworkOccupantVentilationControl::setOpeningProbabilitySchedule(Schedule& schedule) {
    return getImpl<detail::AirflowNetworkOccupantVentilationControl_Impl>()->setOpeningProbabilitySchedule(schedule);
  }

  void AirflowNetworkOccupantVentilationControl::resetOpeningProbabilitySchedule() {
    getImpl<detail::AirflowNetworkOccupantVentilationControl_Impl>()->resetOpeningProbabilitySchedule();
  }

  bool AirflowNetworkOccupantVentilationControl::setClosingProbabilitySchedule(Schedule& schedule) {
    return getImpl<detail::AirflowNetworkOccupantVentilationControl_Impl>()->setClosingProbabilitySchedule(schedule);
  }

  void AirflowNetworkOccupantVentilationControl::resetClosingProbabilitySchedule() {
    getImpl<detail::AirflowNetworkOccupantVentilationControl_Impl>()->resetClosingProbabilitySchedule();
  }

  /// @cond
  AirflowNetworkOccupantVentilationControl::AirflowNetworkOccupantVentilationControl(
    std::shared_ptr<detail::AirflowNetworkOccupantVentilationControl_Impl> impl)
    : ModelObject(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
