/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "AirflowNetworkZone.hpp"
#include "AirflowNetworkZone_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "AirflowNetworkOccupantVentilationControl.hpp"
#include "AirflowNetworkOccupantVentilationControl_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AirflowNetworkZone_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    AirflowNetworkZone_Impl::AirflowNetworkZone_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : AirflowNetworkNode_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AirflowNetworkZone::iddObjectType());
    }

    AirflowNetworkZone_Impl::AirflowNetworkZone_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : AirflowNetworkNode_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AirflowNetworkZone::iddObjectType());
    }

    AirflowNetworkZone_Impl::AirflowNetworkZone_Impl(const AirflowNetworkZone_Impl& other, Model_Impl* model, bool keepHandle)
      : AirflowNetworkNode_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& AirflowNetworkZone_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType AirflowNetworkZone_Impl::iddObjectType() const {
      return AirflowNetworkZone::iddObjectType();
    }

    std::vector<ScheduleTypeKey> AirflowNetworkZone_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_AirflowNetworkZoneFields::VentilationControlZoneTemperatureSetpointScheduleName) != e) {
        result.emplace_back("AirflowNetworkZone", "Ventilation Control Zone Temperature Setpoint");
      }
      if (std::find(b, e, OS_AirflowNetworkZoneFields::VentingAvailabilityScheduleName) != e) {
        result.emplace_back("AirflowNetworkZone", "Venting Availability");
      }
      return result;
    }

    ThermalZone AirflowNetworkZone_Impl::thermalZone() const {
      boost::optional<ThermalZone> value = optionalThermalZone();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Thermal Zone attached.");
      }
      return value.get();
    }

    std::string AirflowNetworkZone_Impl::ventilationControlMode() const {
      boost::optional<std::string> value = getString(OS_AirflowNetworkZoneFields::VentilationControlMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirflowNetworkZone_Impl::isVentilationControlModeDefaulted() const {
      return isEmpty(OS_AirflowNetworkZoneFields::VentilationControlMode);
    }

    boost::optional<Schedule> AirflowNetworkZone_Impl::ventilationControlZoneTemperatureSetpointSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(
        OS_AirflowNetworkZoneFields::VentilationControlZoneTemperatureSetpointScheduleName);
    }

    double AirflowNetworkZone_Impl::minimumVentingOpenFactor() const {
      boost::optional<double> value = getDouble(OS_AirflowNetworkZoneFields::MinimumVentingOpenFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirflowNetworkZone_Impl::isMinimumVentingOpenFactorDefaulted() const {
      return isEmpty(OS_AirflowNetworkZoneFields::MinimumVentingOpenFactor);
    }

    double AirflowNetworkZone_Impl::indoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor() const {
      boost::optional<double> value =
        getDouble(OS_AirflowNetworkZoneFields::IndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirflowNetworkZone_Impl::isIndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactorDefaulted() const {
      return isEmpty(OS_AirflowNetworkZoneFields::IndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor);
    }

    double AirflowNetworkZone_Impl::indoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor() const {
      boost::optional<double> value =
        getDouble(OS_AirflowNetworkZoneFields::IndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirflowNetworkZone_Impl::isIndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactorDefaulted() const {
      return isEmpty(OS_AirflowNetworkZoneFields::IndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor);
    }

    double AirflowNetworkZone_Impl::indoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor() const {
      boost::optional<double> value =
        getDouble(OS_AirflowNetworkZoneFields::IndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirflowNetworkZone_Impl::isIndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactorDefaulted() const {
      return isEmpty(OS_AirflowNetworkZoneFields::IndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor);
    }

    double AirflowNetworkZone_Impl::indoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor() const {
      boost::optional<double> value =
        getDouble(OS_AirflowNetworkZoneFields::IndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirflowNetworkZone_Impl::isIndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactorDefaulted() const {
      return isEmpty(OS_AirflowNetworkZoneFields::IndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor);
    }

    boost::optional<Schedule> AirflowNetworkZone_Impl::ventingAvailabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirflowNetworkZoneFields::VentingAvailabilityScheduleName);
    }

    std::string AirflowNetworkZone_Impl::singleSidedWindPressureCoefficientAlgorithm() const {
      boost::optional<std::string> value = getString(OS_AirflowNetworkZoneFields::SingleSidedWindPressureCoefficientAlgorithm, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirflowNetworkZone_Impl::isSingleSidedWindPressureCoefficientAlgorithmDefaulted() const {
      return isEmpty(OS_AirflowNetworkZoneFields::SingleSidedWindPressureCoefficientAlgorithm);
    }

    double AirflowNetworkZone_Impl::facadeWidth() const {
      boost::optional<double> value = getDouble(OS_AirflowNetworkZoneFields::FacadeWidth, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirflowNetworkZone_Impl::isFacadeWidthDefaulted() const {
      return isEmpty(OS_AirflowNetworkZoneFields::FacadeWidth);
    }

    boost::optional<AirflowNetworkOccupantVentilationControl> AirflowNetworkZone_Impl::occupantVentilationControl() const {
      return getObject<ModelObject>().getModelObjectTarget<AirflowNetworkOccupantVentilationControl>(
        OS_AirflowNetworkZoneFields::OccupantVentilationControlName);
    }

    bool AirflowNetworkZone_Impl::setThermalZone(const ThermalZone& thermalZone) {
      bool result = setPointer(OS_AirflowNetworkZoneFields::ThermalZoneName, thermalZone.handle());
      return result;
    }

    void AirflowNetworkZone_Impl::resetThermalZone() {
      bool result = setString(OS_AirflowNetworkZoneFields::ThermalZoneName, "");
      OS_ASSERT(result);
    }

    bool AirflowNetworkZone_Impl::setVentilationControlMode(const std::string& ventilationControlMode) {
      bool result = setString(OS_AirflowNetworkZoneFields::VentilationControlMode, ventilationControlMode);
      return result;
    }

    void AirflowNetworkZone_Impl::resetVentilationControlMode() {
      bool result = setString(OS_AirflowNetworkZoneFields::VentilationControlMode, "");
      OS_ASSERT(result);
    }

    bool AirflowNetworkZone_Impl::setVentilationControlZoneTemperatureSetpointSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_AirflowNetworkZoneFields::VentilationControlZoneTemperatureSetpointScheduleName, "AirflowNetworkZone",
                                "Ventilation Control Zone Temperature Setpoint", schedule);
      return result;
    }

    void AirflowNetworkZone_Impl::resetVentilationControlZoneTemperatureSetpointSchedule() {
      bool result = setString(OS_AirflowNetworkZoneFields::VentilationControlZoneTemperatureSetpointScheduleName, "");
      OS_ASSERT(result);
    }

    bool AirflowNetworkZone_Impl::setMinimumVentingOpenFactor(double minimumVentingOpenFactor) {
      bool result = setDouble(OS_AirflowNetworkZoneFields::MinimumVentingOpenFactor, minimumVentingOpenFactor);
      return result;
    }

    void AirflowNetworkZone_Impl::resetMinimumVentingOpenFactor() {
      bool result = setString(OS_AirflowNetworkZoneFields::MinimumVentingOpenFactor, "");
      OS_ASSERT(result);
    }

    bool AirflowNetworkZone_Impl::setIndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor(
      double indoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor) {
      bool result = setDouble(OS_AirflowNetworkZoneFields::IndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor,
                              indoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor);
      return result;
    }

    void AirflowNetworkZone_Impl::resetIndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor() {
      bool result = setString(OS_AirflowNetworkZoneFields::IndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor, "");
      OS_ASSERT(result);
    }

    bool AirflowNetworkZone_Impl::setIndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor(
      double indoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor) {
      bool result = setDouble(OS_AirflowNetworkZoneFields::IndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor,
                              indoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor);
      return result;
    }

    void AirflowNetworkZone_Impl::resetIndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor() {
      bool result = setString(OS_AirflowNetworkZoneFields::IndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor, "");
      OS_ASSERT(result);
    }

    bool AirflowNetworkZone_Impl::setIndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor(
      double indoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor) {
      bool result = setDouble(OS_AirflowNetworkZoneFields::IndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor,
                              indoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor);
      return result;
    }

    void AirflowNetworkZone_Impl::resetIndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor() {
      bool result = setString(OS_AirflowNetworkZoneFields::IndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor, "");
      OS_ASSERT(result);
    }

    bool AirflowNetworkZone_Impl::setIndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor(
      double indoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor) {
      bool result = setDouble(OS_AirflowNetworkZoneFields::IndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor,
                              indoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor);
      return result;
    }

    void AirflowNetworkZone_Impl::resetIndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor() {
      bool result = setString(OS_AirflowNetworkZoneFields::IndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor, "");
      OS_ASSERT(result);
    }

    bool AirflowNetworkZone_Impl::setVentingAvailabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_AirflowNetworkZoneFields::VentingAvailabilityScheduleName, "AirflowNetworkZone", "Venting Availability", schedule);
      return result;
    }

    void AirflowNetworkZone_Impl::resetVentingAvailabilitySchedule() {
      bool result = setString(OS_AirflowNetworkZoneFields::VentingAvailabilityScheduleName, "");
      OS_ASSERT(result);
    }

    bool AirflowNetworkZone_Impl::setSingleSidedWindPressureCoefficientAlgorithm(const std::string& singleSidedWindPressureCoefficientAlgorithm) {
      bool result = setString(OS_AirflowNetworkZoneFields::SingleSidedWindPressureCoefficientAlgorithm, singleSidedWindPressureCoefficientAlgorithm);
      return result;
    }

    void AirflowNetworkZone_Impl::resetSingleSidedWindPressureCoefficientAlgorithm() {
      bool result = setString(OS_AirflowNetworkZoneFields::SingleSidedWindPressureCoefficientAlgorithm, "");
      OS_ASSERT(result);
    }

    bool AirflowNetworkZone_Impl::setFacadeWidth(double facadeWidth) {
      bool result = setDouble(OS_AirflowNetworkZoneFields::FacadeWidth, facadeWidth);
      return result;
    }

    void AirflowNetworkZone_Impl::resetFacadeWidth() {
      bool result = setString(OS_AirflowNetworkZoneFields::FacadeWidth, "");
      OS_ASSERT(result);
    }

    bool AirflowNetworkZone_Impl::setOccupantVentilationControl(
      const AirflowNetworkOccupantVentilationControl& airflowNetworkOccupantVentilationControl) {
      bool result = setPointer(OS_AirflowNetworkZoneFields::OccupantVentilationControlName, airflowNetworkOccupantVentilationControl.handle());
      return result;
    }

    void AirflowNetworkZone_Impl::resetOccupantVentilationControl() {
      bool result = setString(OS_AirflowNetworkZoneFields::OccupantVentilationControlName, "");
      OS_ASSERT(result);
    }

    boost::optional<ThermalZone> AirflowNetworkZone_Impl::optionalThermalZone() const {
      return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_AirflowNetworkZoneFields::ThermalZoneName);
    }

  }  // namespace detail

  AirflowNetworkZone::AirflowNetworkZone(const Model& model, const Handle& handle) : AirflowNetworkNode(AirflowNetworkZone::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirflowNetworkZone_Impl>());
    bool ok = getImpl<detail::AirflowNetworkZone_Impl>()->setPointer(OS_AirflowNetworkZoneFields::ThermalZoneName, handle);
    OS_ASSERT(ok);
  }

  IddObjectType AirflowNetworkZone::iddObjectType() {
    return {IddObjectType::OS_AirflowNetworkZone};
  }

  std::vector<std::string> AirflowNetworkZone::ventilationControlModeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_AirflowNetworkZoneFields::VentilationControlMode);
  }

  std::vector<std::string> AirflowNetworkZone::singleSidedWindPressureCoefficientAlgorithmValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_AirflowNetworkZoneFields::SingleSidedWindPressureCoefficientAlgorithm);
  }

  ThermalZone AirflowNetworkZone::thermalZone() const {
    return getImpl<detail::AirflowNetworkZone_Impl>()->thermalZone();
  }

  std::string AirflowNetworkZone::ventilationControlMode() const {
    return getImpl<detail::AirflowNetworkZone_Impl>()->ventilationControlMode();
  }

  bool AirflowNetworkZone::isVentilationControlModeDefaulted() const {
    return getImpl<detail::AirflowNetworkZone_Impl>()->isVentilationControlModeDefaulted();
  }

  boost::optional<Schedule> AirflowNetworkZone::ventilationControlZoneTemperatureSetpointSchedule() const {
    return getImpl<detail::AirflowNetworkZone_Impl>()->ventilationControlZoneTemperatureSetpointSchedule();
  }

  double AirflowNetworkZone::minimumVentingOpenFactor() const {
    return getImpl<detail::AirflowNetworkZone_Impl>()->minimumVentingOpenFactor();
  }

  bool AirflowNetworkZone::isMinimumVentingOpenFactorDefaulted() const {
    return getImpl<detail::AirflowNetworkZone_Impl>()->isMinimumVentingOpenFactorDefaulted();
  }

  double AirflowNetworkZone::indoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor() const {
    return getImpl<detail::AirflowNetworkZone_Impl>()->indoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor();
  }

  bool AirflowNetworkZone::isIndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactorDefaulted() const {
    return getImpl<detail::AirflowNetworkZone_Impl>()->isIndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactorDefaulted();
  }

  double AirflowNetworkZone::indoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor() const {
    return getImpl<detail::AirflowNetworkZone_Impl>()->indoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor();
  }

  bool AirflowNetworkZone::isIndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactorDefaulted() const {
    return getImpl<detail::AirflowNetworkZone_Impl>()->isIndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactorDefaulted();
  }

  double AirflowNetworkZone::indoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor() const {
    return getImpl<detail::AirflowNetworkZone_Impl>()->indoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor();
  }

  bool AirflowNetworkZone::isIndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactorDefaulted() const {
    return getImpl<detail::AirflowNetworkZone_Impl>()->isIndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactorDefaulted();
  }

  double AirflowNetworkZone::indoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor() const {
    return getImpl<detail::AirflowNetworkZone_Impl>()->indoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor();
  }

  bool AirflowNetworkZone::isIndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactorDefaulted() const {
    return getImpl<detail::AirflowNetworkZone_Impl>()->isIndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactorDefaulted();
  }

  boost::optional<Schedule> AirflowNetworkZone::ventingAvailabilitySchedule() const {
    return getImpl<detail::AirflowNetworkZone_Impl>()->ventingAvailabilitySchedule();
  }

  std::string AirflowNetworkZone::singleSidedWindPressureCoefficientAlgorithm() const {
    return getImpl<detail::AirflowNetworkZone_Impl>()->singleSidedWindPressureCoefficientAlgorithm();
  }

  bool AirflowNetworkZone::isSingleSidedWindPressureCoefficientAlgorithmDefaulted() const {
    return getImpl<detail::AirflowNetworkZone_Impl>()->isSingleSidedWindPressureCoefficientAlgorithmDefaulted();
  }

  double AirflowNetworkZone::facadeWidth() const {
    return getImpl<detail::AirflowNetworkZone_Impl>()->facadeWidth();
  }

  bool AirflowNetworkZone::isFacadeWidthDefaulted() const {
    return getImpl<detail::AirflowNetworkZone_Impl>()->isFacadeWidthDefaulted();
  }

  boost::optional<AirflowNetworkOccupantVentilationControl> AirflowNetworkZone::occupantVentilationControl() const {
    return getImpl<detail::AirflowNetworkZone_Impl>()->occupantVentilationControl();
  }

  bool AirflowNetworkZone::setThermalZone(const ThermalZone& thermalZone) {
    return getImpl<detail::AirflowNetworkZone_Impl>()->setThermalZone(thermalZone);
  }

  void AirflowNetworkZone::resetThermalZone() {
    getImpl<detail::AirflowNetworkZone_Impl>()->resetThermalZone();
  }

  bool AirflowNetworkZone::setVentilationControlMode(const std::string& ventilationControlMode) {
    return getImpl<detail::AirflowNetworkZone_Impl>()->setVentilationControlMode(ventilationControlMode);
  }

  void AirflowNetworkZone::resetVentilationControlMode() {
    getImpl<detail::AirflowNetworkZone_Impl>()->resetVentilationControlMode();
  }

  bool AirflowNetworkZone::setVentilationControlZoneTemperatureSetpointSchedule(Schedule& schedule) {
    return getImpl<detail::AirflowNetworkZone_Impl>()->setVentilationControlZoneTemperatureSetpointSchedule(schedule);
  }

  void AirflowNetworkZone::resetVentilationControlZoneTemperatureSetpointSchedule() {
    getImpl<detail::AirflowNetworkZone_Impl>()->resetVentilationControlZoneTemperatureSetpointSchedule();
  }

  bool AirflowNetworkZone::setMinimumVentingOpenFactor(double minimumVentingOpenFactor) {
    return getImpl<detail::AirflowNetworkZone_Impl>()->setMinimumVentingOpenFactor(minimumVentingOpenFactor);
  }

  void AirflowNetworkZone::resetMinimumVentingOpenFactor() {
    getImpl<detail::AirflowNetworkZone_Impl>()->resetMinimumVentingOpenFactor();
  }

  bool AirflowNetworkZone::setIndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor(
    double indoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor) {
    return getImpl<detail::AirflowNetworkZone_Impl>()->setIndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor(
      indoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor);
  }

  void AirflowNetworkZone::resetIndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor() {
    getImpl<detail::AirflowNetworkZone_Impl>()->resetIndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor();
  }

  bool AirflowNetworkZone::setIndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor(
    double indoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor) {
    return getImpl<detail::AirflowNetworkZone_Impl>()->setIndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor(
      indoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor);
  }

  void AirflowNetworkZone::resetIndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor() {
    getImpl<detail::AirflowNetworkZone_Impl>()->resetIndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor();
  }

  bool AirflowNetworkZone::setIndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor(
    double indoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor) {
    return getImpl<detail::AirflowNetworkZone_Impl>()->setIndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor(
      indoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor);
  }

  void AirflowNetworkZone::resetIndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor() {
    getImpl<detail::AirflowNetworkZone_Impl>()->resetIndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor();
  }

  bool AirflowNetworkZone::setIndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor(
    double indoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor) {
    return getImpl<detail::AirflowNetworkZone_Impl>()->setIndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor(
      indoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor);
  }

  void AirflowNetworkZone::resetIndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor() {
    getImpl<detail::AirflowNetworkZone_Impl>()->resetIndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor();
  }

  bool AirflowNetworkZone::setVentingAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::AirflowNetworkZone_Impl>()->setVentingAvailabilitySchedule(schedule);
  }

  void AirflowNetworkZone::resetVentingAvailabilitySchedule() {
    getImpl<detail::AirflowNetworkZone_Impl>()->resetVentingAvailabilitySchedule();
  }

  bool AirflowNetworkZone::setSingleSidedWindPressureCoefficientAlgorithm(const std::string& singleSidedWindPressureCoefficientAlgorithm) {
    return getImpl<detail::AirflowNetworkZone_Impl>()->setSingleSidedWindPressureCoefficientAlgorithm(singleSidedWindPressureCoefficientAlgorithm);
  }

  void AirflowNetworkZone::resetSingleSidedWindPressureCoefficientAlgorithm() {
    getImpl<detail::AirflowNetworkZone_Impl>()->resetSingleSidedWindPressureCoefficientAlgorithm();
  }

  bool AirflowNetworkZone::setFacadeWidth(double facadeWidth) {
    return getImpl<detail::AirflowNetworkZone_Impl>()->setFacadeWidth(facadeWidth);
  }

  void AirflowNetworkZone::resetFacadeWidth() {
    getImpl<detail::AirflowNetworkZone_Impl>()->resetFacadeWidth();
  }

  bool AirflowNetworkZone::setOccupantVentilationControl(const AirflowNetworkOccupantVentilationControl& airflowNetworkOccupantVentilationControl) {
    return getImpl<detail::AirflowNetworkZone_Impl>()->setOccupantVentilationControl(airflowNetworkOccupantVentilationControl);
  }

  void AirflowNetworkZone::resetOccupantVentilationControl() {
    getImpl<detail::AirflowNetworkZone_Impl>()->resetOccupantVentilationControl();
  }

  /// @cond
  AirflowNetworkZone::AirflowNetworkZone(std::shared_ptr<detail::AirflowNetworkZone_Impl> impl) : AirflowNetworkNode(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
