/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ConstructionAirBoundary.hpp"
#include "ConstructionAirBoundary_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Construction_AirBoundary_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ConstructionAirBoundary_Impl::ConstructionAirBoundary_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ConstructionBase_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ConstructionAirBoundary::iddObjectType());
    }

    ConstructionAirBoundary_Impl::ConstructionAirBoundary_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                               bool keepHandle)
      : ConstructionBase_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ConstructionAirBoundary::iddObjectType());
    }

    ConstructionAirBoundary_Impl::ConstructionAirBoundary_Impl(const ConstructionAirBoundary_Impl& other, Model_Impl* model, bool keepHandle)
      : ConstructionBase_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& ConstructionAirBoundary_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      if (result.empty()) {
      }
      return result;
    }

    IddObjectType ConstructionAirBoundary_Impl::iddObjectType() const {
      return ConstructionAirBoundary::iddObjectType();
    }

    std::vector<ScheduleTypeKey> ConstructionAirBoundary_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      // TODO: Check schedule display names.
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_Construction_AirBoundaryFields::SimpleMixingScheduleName) != e) {
        result.push_back(ScheduleTypeKey("ConstructionAirBoundary", "Simple Mixing"));
      }
      return result;
    }

    bool ConstructionAirBoundary_Impl::isOpaque() const {
      return false;
    }

    bool ConstructionAirBoundary_Impl::isFenestration() const {
      return false;
    }

    bool ConstructionAirBoundary_Impl::isSolarDiffusing() const {
      return false;
    }

    bool ConstructionAirBoundary_Impl::isModelPartition() const {
      return true;
    }

    int ConstructionAirBoundary_Impl::renderingColorIndex() const {
      return OS_Construction_AirBoundaryFields::SurfaceRenderingName;
    }

    std::string ConstructionAirBoundary_Impl::airExchangeMethod() const {
      boost::optional<std::string> value = getString(OS_Construction_AirBoundaryFields::AirExchangeMethod, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ConstructionAirBoundary_Impl::isAirExchangeMethodDefaulted() const {
      return isEmpty(OS_Construction_AirBoundaryFields::AirExchangeMethod);
    }

    double ConstructionAirBoundary_Impl::simpleMixingAirChangesPerHour() const {
      boost::optional<double> value = getDouble(OS_Construction_AirBoundaryFields::SimpleMixingAirChangesperHour, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ConstructionAirBoundary_Impl::isSimpleMixingAirChangesPerHourDefaulted() const {
      return isEmpty(OS_Construction_AirBoundaryFields::SimpleMixingAirChangesperHour);
    }

    boost::optional<Schedule> ConstructionAirBoundary_Impl::simpleMixingSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Construction_AirBoundaryFields::SimpleMixingScheduleName);
    }

    bool ConstructionAirBoundary_Impl::setAirExchangeMethod(const std::string& airExchangeMethod) {
      bool result = setString(OS_Construction_AirBoundaryFields::AirExchangeMethod, airExchangeMethod);
      return result;
    }

    void ConstructionAirBoundary_Impl::resetAirExchangeMethod() {
      bool result = setString(OS_Construction_AirBoundaryFields::AirExchangeMethod, "");
      OS_ASSERT(result);
    }

    bool ConstructionAirBoundary_Impl::setSimpleMixingAirChangesPerHour(double simpleMixingAirChangesPerHour) {
      bool result = setDouble(OS_Construction_AirBoundaryFields::SimpleMixingAirChangesperHour, simpleMixingAirChangesPerHour);
      return result;
    }

    void ConstructionAirBoundary_Impl::resetSimpleMixingAirChangesPerHour() {
      bool result = setString(OS_Construction_AirBoundaryFields::SimpleMixingAirChangesperHour, "");
      OS_ASSERT(result);
    }

    bool ConstructionAirBoundary_Impl::setSimpleMixingSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_Construction_AirBoundaryFields::SimpleMixingScheduleName, "ConstructionAirBoundary", "Simple Mixing", schedule);
      return result;
    }

    void ConstructionAirBoundary_Impl::resetSimpleMixingSchedule() {
      bool result = setString(OS_Construction_AirBoundaryFields::SimpleMixingScheduleName, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  ConstructionAirBoundary::ConstructionAirBoundary(const Model& model) : ConstructionBase(ConstructionAirBoundary::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ConstructionAirBoundary_Impl>());

    setSimpleMixingAirChangesPerHour(0.0);
  }

  IddObjectType ConstructionAirBoundary::iddObjectType() {
    return {IddObjectType::OS_Construction_AirBoundary};
  }

  std::vector<std::string> ConstructionAirBoundary::solarAndDaylightingMethodValues() {
    std::vector<std::string> result;
    LOG(Warn, "This method is deprecated. All air boundaries will be modeled using the former 'GroupedZones' option.");
    return result;
  }

  std::vector<std::string> ConstructionAirBoundary::radiantExchangeMethodValues() {
    std::vector<std::string> result;
    LOG(Warn, "This method is deprecated. All air boundaries will be modeled using the former 'GroupedZones' option.");
    return result;
  }

  std::vector<std::string> ConstructionAirBoundary::airExchangeMethodValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Construction_AirBoundaryFields::AirExchangeMethod);
  }

  std::string ConstructionAirBoundary::solarAndDaylightingMethod() const {
    LOG(Warn, "This method is deprecated. All air boundaries will be modeled using the former 'GroupedZones' option.");
    return "GroupedZones";
  }

  bool ConstructionAirBoundary::isSolarAndDaylightingMethodDefaulted() const {
    LOG(Warn, "This method is deprecated. All air boundaries will be modeled using the former 'GroupedZones' option.");
    return true;
  }

  std::string ConstructionAirBoundary::radiantExchangeMethod() const {
    LOG(Warn, "This method is deprecated. All air boundaries will be modeled using the former 'GroupedZones' option.");
    return "GroupedZones";
  }

  bool ConstructionAirBoundary::isRadiantExchangeMethodDefaulted() const {
    LOG(Warn, "This method is deprecated. All air boundaries will be modeled using the former 'GroupedZones' option.");
    return true;
  }

  std::string ConstructionAirBoundary::airExchangeMethod() const {
    return getImpl<detail::ConstructionAirBoundary_Impl>()->airExchangeMethod();
  }

  bool ConstructionAirBoundary::isAirExchangeMethodDefaulted() const {
    return getImpl<detail::ConstructionAirBoundary_Impl>()->isAirExchangeMethodDefaulted();
  }

  double ConstructionAirBoundary::simpleMixingAirChangesPerHour() const {
    return getImpl<detail::ConstructionAirBoundary_Impl>()->simpleMixingAirChangesPerHour();
  }

  bool ConstructionAirBoundary::isSimpleMixingAirChangesPerHourDefaulted() const {
    return getImpl<detail::ConstructionAirBoundary_Impl>()->isSimpleMixingAirChangesPerHourDefaulted();
  }

  boost::optional<Schedule> ConstructionAirBoundary::simpleMixingSchedule() const {
    return getImpl<detail::ConstructionAirBoundary_Impl>()->simpleMixingSchedule();
  }

  bool ConstructionAirBoundary::setSolarAndDaylightingMethod(const std::string& /*solarAndDaylightingMethod*/) {
    LOG(Warn, "This method is deprecated. All air boundaries will be modeled using the former 'GroupedZones' option.");
    return false;
  }

  void ConstructionAirBoundary::resetSolarAndDaylightingMethod() {
    LOG(Warn, "This method is deprecated. All air boundaries will be modeled using the former 'GroupedZones' option.");
  }

  bool ConstructionAirBoundary::setRadiantExchangeMethod(const std::string& /*radiantExchangeMethod*/) {
    LOG(Warn, "This method is deprecated. All air boundaries will be modeled using the former 'GroupedZones' option.");
    return false;
  }

  void ConstructionAirBoundary::resetRadiantExchangeMethod() {
    LOG(Warn, "This method is deprecated. All air boundaries will be modeled using the former 'GroupedZones' option.");
  }

  bool ConstructionAirBoundary::setAirExchangeMethod(const std::string& airExchangeMethod) {
    return getImpl<detail::ConstructionAirBoundary_Impl>()->setAirExchangeMethod(airExchangeMethod);
  }

  void ConstructionAirBoundary::resetAirExchangeMethod() {
    getImpl<detail::ConstructionAirBoundary_Impl>()->resetAirExchangeMethod();
  }

  bool ConstructionAirBoundary::setSimpleMixingAirChangesPerHour(double simpleMixingAirChangesPerHour) {
    return getImpl<detail::ConstructionAirBoundary_Impl>()->setSimpleMixingAirChangesPerHour(simpleMixingAirChangesPerHour);
  }

  void ConstructionAirBoundary::resetSimpleMixingAirChangesPerHour() {
    getImpl<detail::ConstructionAirBoundary_Impl>()->resetSimpleMixingAirChangesPerHour();
  }

  bool ConstructionAirBoundary::setSimpleMixingSchedule(Schedule& schedule) {
    return getImpl<detail::ConstructionAirBoundary_Impl>()->setSimpleMixingSchedule(schedule);
  }

  void ConstructionAirBoundary::resetSimpleMixingSchedule() {
    getImpl<detail::ConstructionAirBoundary_Impl>()->resetSimpleMixingSchedule();
  }

  /// @cond
  ConstructionAirBoundary::ConstructionAirBoundary(std::shared_ptr<detail::ConstructionAirBoundary_Impl> impl) : ConstructionBase(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
