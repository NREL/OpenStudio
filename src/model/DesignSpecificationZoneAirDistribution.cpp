/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "DesignSpecificationZoneAirDistribution.hpp"
#include "DesignSpecificationZoneAirDistribution_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include <utilities/idd/OS_DesignSpecification_ZoneAirDistribution_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    DesignSpecificationZoneAirDistribution_Impl::DesignSpecificationZoneAirDistribution_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                             bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == DesignSpecificationZoneAirDistribution::iddObjectType());
    }

    DesignSpecificationZoneAirDistribution_Impl::DesignSpecificationZoneAirDistribution_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                             Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == DesignSpecificationZoneAirDistribution::iddObjectType());
    }

    DesignSpecificationZoneAirDistribution_Impl::DesignSpecificationZoneAirDistribution_Impl(const DesignSpecificationZoneAirDistribution_Impl& other,
                                                                                             Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& DesignSpecificationZoneAirDistribution_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType DesignSpecificationZoneAirDistribution_Impl::iddObjectType() const {
      return DesignSpecificationZoneAirDistribution::iddObjectType();
    }

    std::vector<ScheduleTypeKey> DesignSpecificationZoneAirDistribution_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_DesignSpecification_ZoneAirDistributionFields::ZoneAirDistributionEffectivenessSchedule) != e) {
        result.push_back(ScheduleTypeKey("DesignSpecificationZoneAirDistribution", "Zone Air Distribution Effectiveness Schedule"));
      }
      return result;
    }

    double DesignSpecificationZoneAirDistribution_Impl::zoneAirDistributionEffectivenessinCoolingMode() const {
      boost::optional<double> value =
        getDouble(OS_DesignSpecification_ZoneAirDistributionFields::ZoneAirDistributionEffectivenessinCoolingMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool DesignSpecificationZoneAirDistribution_Impl::isZoneAirDistributionEffectivenessinCoolingModeDefaulted() const {
      return isEmpty(OS_DesignSpecification_ZoneAirDistributionFields::ZoneAirDistributionEffectivenessinCoolingMode);
    }

    double DesignSpecificationZoneAirDistribution_Impl::zoneAirDistributionEffectivenessinHeatingMode() const {
      boost::optional<double> value =
        getDouble(OS_DesignSpecification_ZoneAirDistributionFields::ZoneAirDistributionEffectivenessinHeatingMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool DesignSpecificationZoneAirDistribution_Impl::isZoneAirDistributionEffectivenessinHeatingModeDefaulted() const {
      return isEmpty(OS_DesignSpecification_ZoneAirDistributionFields::ZoneAirDistributionEffectivenessinHeatingMode);
    }

    boost::optional<Schedule> DesignSpecificationZoneAirDistribution_Impl::zoneAirDistributionEffectivenessSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(
        OS_DesignSpecification_ZoneAirDistributionFields::ZoneAirDistributionEffectivenessSchedule);
    }

    double DesignSpecificationZoneAirDistribution_Impl::zoneSecondaryRecirculationFraction() const {
      boost::optional<double> value = getDouble(OS_DesignSpecification_ZoneAirDistributionFields::ZoneSecondaryRecirculationFraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool DesignSpecificationZoneAirDistribution_Impl::isZoneSecondaryRecirculationFractionDefaulted() const {
      return isEmpty(OS_DesignSpecification_ZoneAirDistributionFields::ZoneSecondaryRecirculationFraction);
    }

    bool DesignSpecificationZoneAirDistribution_Impl::setZoneAirDistributionEffectivenessinCoolingMode(
      double zoneAirDistributionEffectivenessinCoolingMode) {
      bool result = setDouble(OS_DesignSpecification_ZoneAirDistributionFields::ZoneAirDistributionEffectivenessinCoolingMode,
                              zoneAirDistributionEffectivenessinCoolingMode);
      return result;
    }

    void DesignSpecificationZoneAirDistribution_Impl::resetZoneAirDistributionEffectivenessinCoolingMode() {
      bool result = setString(OS_DesignSpecification_ZoneAirDistributionFields::ZoneAirDistributionEffectivenessinCoolingMode, "");
      OS_ASSERT(result);
    }

    bool DesignSpecificationZoneAirDistribution_Impl::setZoneAirDistributionEffectivenessinHeatingMode(
      double zoneAirDistributionEffectivenessinHeatingMode) {
      bool result = setDouble(OS_DesignSpecification_ZoneAirDistributionFields::ZoneAirDistributionEffectivenessinHeatingMode,
                              zoneAirDistributionEffectivenessinHeatingMode);
      return result;
    }

    void DesignSpecificationZoneAirDistribution_Impl::resetZoneAirDistributionEffectivenessinHeatingMode() {
      bool result = setString(OS_DesignSpecification_ZoneAirDistributionFields::ZoneAirDistributionEffectivenessinHeatingMode, "");
      OS_ASSERT(result);
    }

    bool DesignSpecificationZoneAirDistribution_Impl::setZoneAirDistributionEffectivenessSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_DesignSpecification_ZoneAirDistributionFields::ZoneAirDistributionEffectivenessSchedule,
                                "DesignSpecificationZoneAirDistribution", "Zone Air Distribution Effectiveness Schedule", schedule);
      return result;
    }

    void DesignSpecificationZoneAirDistribution_Impl::resetZoneAirDistributionEffectivenessSchedule() {
      bool result = setString(OS_DesignSpecification_ZoneAirDistributionFields::ZoneAirDistributionEffectivenessSchedule, "");
      OS_ASSERT(result);
    }

    bool DesignSpecificationZoneAirDistribution_Impl::setZoneSecondaryRecirculationFraction(double zoneSecondaryRecirculationFraction) {
      bool result =
        setDouble(OS_DesignSpecification_ZoneAirDistributionFields::ZoneSecondaryRecirculationFraction, zoneSecondaryRecirculationFraction);
      return result;
    }

    void DesignSpecificationZoneAirDistribution_Impl::resetZoneSecondaryRecirculationFraction() {
      bool result = setString(OS_DesignSpecification_ZoneAirDistributionFields::ZoneSecondaryRecirculationFraction, "");
      OS_ASSERT(result);
    }

    boost::optional<ModelObject> DesignSpecificationZoneAirDistribution_Impl::zoneAirDistributionEffectivenessScheduleAsModelObject() const {
      OptionalModelObject result;
      OptionalSchedule intermediate = zoneAirDistributionEffectivenessSchedule();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    bool DesignSpecificationZoneAirDistribution_Impl::setZoneAirDistributionEffectivenessScheduleAsModelObject(
      const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
        if (intermediate) {
          Schedule schedule(*intermediate);
          return setZoneAirDistributionEffectivenessSchedule(schedule);
        } else {
          return false;
        }
      } else {
        resetZoneAirDistributionEffectivenessSchedule();
      }
      return true;
    }

  }  // namespace detail

  DesignSpecificationZoneAirDistribution::DesignSpecificationZoneAirDistribution(const Model& model)
    : ModelObject(DesignSpecificationZoneAirDistribution::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::DesignSpecificationZoneAirDistribution_Impl>());
  }

  IddObjectType DesignSpecificationZoneAirDistribution::iddObjectType() {
    return {IddObjectType::OS_DesignSpecification_ZoneAirDistribution};
  }

  double DesignSpecificationZoneAirDistribution::zoneAirDistributionEffectivenessinCoolingMode() const {
    return getImpl<detail::DesignSpecificationZoneAirDistribution_Impl>()->zoneAirDistributionEffectivenessinCoolingMode();
  }

  bool DesignSpecificationZoneAirDistribution::isZoneAirDistributionEffectivenessinCoolingModeDefaulted() const {
    return getImpl<detail::DesignSpecificationZoneAirDistribution_Impl>()->isZoneAirDistributionEffectivenessinCoolingModeDefaulted();
  }

  double DesignSpecificationZoneAirDistribution::zoneAirDistributionEffectivenessinHeatingMode() const {
    return getImpl<detail::DesignSpecificationZoneAirDistribution_Impl>()->zoneAirDistributionEffectivenessinHeatingMode();
  }

  bool DesignSpecificationZoneAirDistribution::isZoneAirDistributionEffectivenessinHeatingModeDefaulted() const {
    return getImpl<detail::DesignSpecificationZoneAirDistribution_Impl>()->isZoneAirDistributionEffectivenessinHeatingModeDefaulted();
  }

  boost::optional<Schedule> DesignSpecificationZoneAirDistribution::zoneAirDistributionEffectivenessSchedule() const {
    return getImpl<detail::DesignSpecificationZoneAirDistribution_Impl>()->zoneAirDistributionEffectivenessSchedule();
  }

  double DesignSpecificationZoneAirDistribution::zoneSecondaryRecirculationFraction() const {
    return getImpl<detail::DesignSpecificationZoneAirDistribution_Impl>()->zoneSecondaryRecirculationFraction();
  }

  bool DesignSpecificationZoneAirDistribution::isZoneSecondaryRecirculationFractionDefaulted() const {
    return getImpl<detail::DesignSpecificationZoneAirDistribution_Impl>()->isZoneSecondaryRecirculationFractionDefaulted();
  }

  bool
    DesignSpecificationZoneAirDistribution::setZoneAirDistributionEffectivenessinCoolingMode(double zoneAirDistributionEffectivenessinCoolingMode) {
    return getImpl<detail::DesignSpecificationZoneAirDistribution_Impl>()->setZoneAirDistributionEffectivenessinCoolingMode(
      zoneAirDistributionEffectivenessinCoolingMode);
  }

  void DesignSpecificationZoneAirDistribution::resetZoneAirDistributionEffectivenessinCoolingMode() {
    getImpl<detail::DesignSpecificationZoneAirDistribution_Impl>()->resetZoneAirDistributionEffectivenessinCoolingMode();
  }

  bool
    DesignSpecificationZoneAirDistribution::setZoneAirDistributionEffectivenessinHeatingMode(double zoneAirDistributionEffectivenessinHeatingMode) {
    return getImpl<detail::DesignSpecificationZoneAirDistribution_Impl>()->setZoneAirDistributionEffectivenessinHeatingMode(
      zoneAirDistributionEffectivenessinHeatingMode);
  }

  void DesignSpecificationZoneAirDistribution::resetZoneAirDistributionEffectivenessinHeatingMode() {
    getImpl<detail::DesignSpecificationZoneAirDistribution_Impl>()->resetZoneAirDistributionEffectivenessinHeatingMode();
  }

  bool DesignSpecificationZoneAirDistribution::setZoneAirDistributionEffectivenessSchedule(Schedule& schedule) {
    return getImpl<detail::DesignSpecificationZoneAirDistribution_Impl>()->setZoneAirDistributionEffectivenessSchedule(schedule);
  }

  void DesignSpecificationZoneAirDistribution::resetZoneAirDistributionEffectivenessSchedule() {
    getImpl<detail::DesignSpecificationZoneAirDistribution_Impl>()->resetZoneAirDistributionEffectivenessSchedule();
  }

  bool DesignSpecificationZoneAirDistribution::setZoneSecondaryRecirculationFraction(double zoneSecondaryRecirculationFraction) {
    return getImpl<detail::DesignSpecificationZoneAirDistribution_Impl>()->setZoneSecondaryRecirculationFraction(zoneSecondaryRecirculationFraction);
  }

  void DesignSpecificationZoneAirDistribution::resetZoneSecondaryRecirculationFraction() {
    getImpl<detail::DesignSpecificationZoneAirDistribution_Impl>()->resetZoneSecondaryRecirculationFraction();
  }

  /// @cond
  DesignSpecificationZoneAirDistribution::DesignSpecificationZoneAirDistribution(
    std::shared_ptr<detail::DesignSpecificationZoneAirDistribution_Impl> impl)
    : ModelObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
