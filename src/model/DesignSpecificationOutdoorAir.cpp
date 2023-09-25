/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "DesignSpecificationOutdoorAir.hpp"
#include "DesignSpecificationOutdoorAir_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_DesignSpecification_OutdoorAir_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    DesignSpecificationOutdoorAir_Impl::DesignSpecificationOutdoorAir_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ResourceObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == DesignSpecificationOutdoorAir::iddObjectType());
    }

    DesignSpecificationOutdoorAir_Impl::DesignSpecificationOutdoorAir_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                           bool keepHandle)
      : ResourceObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == DesignSpecificationOutdoorAir::iddObjectType());
    }

    DesignSpecificationOutdoorAir_Impl::DesignSpecificationOutdoorAir_Impl(const DesignSpecificationOutdoorAir_Impl& other, Model_Impl* model,
                                                                           bool keepHandle)
      : ResourceObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& DesignSpecificationOutdoorAir_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      // Not appropriate: no specific output
      return result;
    }

    IddObjectType DesignSpecificationOutdoorAir_Impl::iddObjectType() const {
      return DesignSpecificationOutdoorAir::iddObjectType();
    }

    std::vector<ScheduleTypeKey> DesignSpecificationOutdoorAir_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowRateFractionScheduleName) != e) {
        result.push_back(ScheduleTypeKey("DesignSpecificationOutdoorAir", "Outdoor Air Flow Rate"));
      }
      return result;
    }

    std::string DesignSpecificationOutdoorAir_Impl::outdoorAirMethod() const {
      boost::optional<std::string> value = getString(OS_DesignSpecification_OutdoorAirFields::OutdoorAirMethod, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool DesignSpecificationOutdoorAir_Impl::isOutdoorAirMethodDefaulted() const {
      return isEmpty(OS_DesignSpecification_OutdoorAirFields::OutdoorAirMethod);
    }

    double DesignSpecificationOutdoorAir_Impl::outdoorAirFlowperPerson() const {
      boost::optional<double> value = getDouble(OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowperPerson, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool DesignSpecificationOutdoorAir_Impl::isOutdoorAirFlowperPersonDefaulted() const {
      return isEmpty(OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowperPerson);
    }

    double DesignSpecificationOutdoorAir_Impl::outdoorAirFlowperFloorArea() const {
      boost::optional<double> value = getDouble(OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowperFloorArea, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool DesignSpecificationOutdoorAir_Impl::isOutdoorAirFlowperFloorAreaDefaulted() const {
      return isEmpty(OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowperFloorArea);
    }

    double DesignSpecificationOutdoorAir_Impl::outdoorAirFlowRate() const {
      boost::optional<double> value = getDouble(OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowRate, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool DesignSpecificationOutdoorAir_Impl::isOutdoorAirFlowRateDefaulted() const {
      return isEmpty(OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowRate);
    }

    double DesignSpecificationOutdoorAir_Impl::outdoorAirFlowAirChangesperHour() const {
      boost::optional<double> value = getDouble(OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowAirChangesperHour, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool DesignSpecificationOutdoorAir_Impl::isOutdoorAirFlowAirChangesperHourDefaulted() const {
      return isEmpty(OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowAirChangesperHour);
    }

    boost::optional<Schedule> DesignSpecificationOutdoorAir_Impl::outdoorAirFlowRateFractionSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowRateFractionScheduleName);
    }

    bool DesignSpecificationOutdoorAir_Impl::setOutdoorAirMethod(const std::string& outdoorAirMethod) {
      bool result = setString(OS_DesignSpecification_OutdoorAirFields::OutdoorAirMethod, outdoorAirMethod);
      return result;
    }

    void DesignSpecificationOutdoorAir_Impl::resetOutdoorAirMethod() {
      bool result = setString(OS_DesignSpecification_OutdoorAirFields::OutdoorAirMethod, "");
      OS_ASSERT(result);
    }

    bool DesignSpecificationOutdoorAir_Impl::setOutdoorAirFlowperPerson(double outdoorAirFlowperPerson) {
      bool result = setDouble(OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowperPerson, outdoorAirFlowperPerson);
      return result;
    }

    void DesignSpecificationOutdoorAir_Impl::resetOutdoorAirFlowperPerson() {
      bool result = setString(OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowperPerson, "");
      OS_ASSERT(result);
    }

    bool DesignSpecificationOutdoorAir_Impl::setOutdoorAirFlowperFloorArea(double outdoorAirFlowperFloorArea) {
      bool result = setDouble(OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowperFloorArea, outdoorAirFlowperFloorArea);
      return result;
    }

    void DesignSpecificationOutdoorAir_Impl::resetOutdoorAirFlowperFloorArea() {
      bool result = setString(OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowperFloorArea, "");
      OS_ASSERT(result);
    }

    bool DesignSpecificationOutdoorAir_Impl::setOutdoorAirFlowRate(double outdoorAirFlowRate) {
      bool result = setDouble(OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowRate, outdoorAirFlowRate);
      return result;
    }

    void DesignSpecificationOutdoorAir_Impl::resetOutdoorAirFlowRate() {
      bool result = setString(OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowRate, "");
      OS_ASSERT(result);
    }

    bool DesignSpecificationOutdoorAir_Impl::setOutdoorAirFlowAirChangesperHour(double outdoorAirFlowAirChangesperHour) {
      bool result = setDouble(OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowAirChangesperHour, outdoorAirFlowAirChangesperHour);
      return result;
    }

    void DesignSpecificationOutdoorAir_Impl::resetOutdoorAirFlowAirChangesperHour() {
      bool result = setString(OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowAirChangesperHour, "");
      OS_ASSERT(result);
    }

    bool DesignSpecificationOutdoorAir_Impl::setOutdoorAirFlowRateFractionSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowRateFractionScheduleName, "DesignSpecificationOutdoorAir",
                                "Outdoor Air Flow Rate", schedule);
      return result;
    }

    void DesignSpecificationOutdoorAir_Impl::resetOutdoorAirFlowRateFractionSchedule() {
      bool result = setString(OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowRateFractionScheduleName, "");
      OS_ASSERT(result);
    }

    std::vector<std::string> DesignSpecificationOutdoorAir_Impl::outdoorAirMethodValues() const {
      return DesignSpecificationOutdoorAir::outdoorAirMethodValues();
    }

    boost::optional<ModelObject> DesignSpecificationOutdoorAir_Impl::outdoorAirFlowRateFractionScheduleAsModelObject() const {
      OptionalModelObject result;
      OptionalSchedule intermediate = outdoorAirFlowRateFractionSchedule();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    bool DesignSpecificationOutdoorAir_Impl::setOutdoorAirFlowRateFractionScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
        if (intermediate) {
          Schedule schedule(*intermediate);
          return setOutdoorAirFlowRateFractionSchedule(schedule);
        } else {
          return false;
        }
      } else {
        resetOutdoorAirFlowRateFractionSchedule();
      }
      return true;
    }

  }  // namespace detail

  DesignSpecificationOutdoorAir::DesignSpecificationOutdoorAir(const Model& model)
    : ResourceObject(DesignSpecificationOutdoorAir::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::DesignSpecificationOutdoorAir_Impl>());
  }

  IddObjectType DesignSpecificationOutdoorAir::iddObjectType() {
    IddObjectType result(IddObjectType::OS_DesignSpecification_OutdoorAir);
    return result;
  }

  std::vector<std::string> DesignSpecificationOutdoorAir::outdoorAirMethodValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_DesignSpecification_OutdoorAirFields::OutdoorAirMethod);
  }

  std::vector<std::string> DesignSpecificationOutdoorAir::validOutdoorAirMethodValues() {
    return DesignSpecificationOutdoorAir::outdoorAirMethodValues();
  }

  std::string DesignSpecificationOutdoorAir::outdoorAirMethod() const {
    return getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->outdoorAirMethod();
  }

  bool DesignSpecificationOutdoorAir::isOutdoorAirMethodDefaulted() const {
    return getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->isOutdoorAirMethodDefaulted();
  }

  double DesignSpecificationOutdoorAir::outdoorAirFlowperPerson() const {
    return getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->outdoorAirFlowperPerson();
  }

  bool DesignSpecificationOutdoorAir::isOutdoorAirFlowperPersonDefaulted() const {
    return getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->isOutdoorAirFlowperPersonDefaulted();
  }

  double DesignSpecificationOutdoorAir::outdoorAirFlowperFloorArea() const {
    return getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->outdoorAirFlowperFloorArea();
  }

  bool DesignSpecificationOutdoorAir::isOutdoorAirFlowperFloorAreaDefaulted() const {
    return getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->isOutdoorAirFlowperFloorAreaDefaulted();
  }

  double DesignSpecificationOutdoorAir::outdoorAirFlowRate() const {
    return getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->outdoorAirFlowRate();
  }

  bool DesignSpecificationOutdoorAir::isOutdoorAirFlowRateDefaulted() const {
    return getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->isOutdoorAirFlowRateDefaulted();
  }

  double DesignSpecificationOutdoorAir::outdoorAirFlowAirChangesperHour() const {
    return getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->outdoorAirFlowAirChangesperHour();
  }

  bool DesignSpecificationOutdoorAir::isOutdoorAirFlowAirChangesperHourDefaulted() const {
    return getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->isOutdoorAirFlowAirChangesperHourDefaulted();
  }

  boost::optional<Schedule> DesignSpecificationOutdoorAir::outdoorAirFlowRateFractionSchedule() const {
    return getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->outdoorAirFlowRateFractionSchedule();
  }

  bool DesignSpecificationOutdoorAir::setOutdoorAirMethod(const std::string& outdoorAirMethod) {
    return getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->setOutdoorAirMethod(outdoorAirMethod);
  }

  void DesignSpecificationOutdoorAir::resetOutdoorAirMethod() {
    getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->resetOutdoorAirMethod();
  }

  bool DesignSpecificationOutdoorAir::setOutdoorAirFlowperPerson(double outdoorAirFlowperPerson) {
    return getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->setOutdoorAirFlowperPerson(outdoorAirFlowperPerson);
  }

  void DesignSpecificationOutdoorAir::resetOutdoorAirFlowperPerson() {
    getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->resetOutdoorAirFlowperPerson();
  }

  bool DesignSpecificationOutdoorAir::setOutdoorAirFlowperFloorArea(double outdoorAirFlowperFloorArea) {
    return getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->setOutdoorAirFlowperFloorArea(outdoorAirFlowperFloorArea);
  }

  void DesignSpecificationOutdoorAir::resetOutdoorAirFlowperFloorArea() {
    getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->resetOutdoorAirFlowperFloorArea();
  }

  bool DesignSpecificationOutdoorAir::setOutdoorAirFlowRate(double outdoorAirFlowRate) {
    return getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->setOutdoorAirFlowRate(outdoorAirFlowRate);
  }

  void DesignSpecificationOutdoorAir::resetOutdoorAirFlowRate() {
    getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->resetOutdoorAirFlowRate();
  }

  bool DesignSpecificationOutdoorAir::setOutdoorAirFlowAirChangesperHour(double outdoorAirFlowAirChangesperHour) {
    return getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->setOutdoorAirFlowAirChangesperHour(outdoorAirFlowAirChangesperHour);
  }

  void DesignSpecificationOutdoorAir::resetOutdoorAirFlowAirChangesperHour() {
    getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->resetOutdoorAirFlowAirChangesperHour();
  }

  bool DesignSpecificationOutdoorAir::setOutdoorAirFlowRateFractionSchedule(Schedule& schedule) {
    return getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->setOutdoorAirFlowRateFractionSchedule(schedule);
  }

  void DesignSpecificationOutdoorAir::resetOutdoorAirFlowRateFractionSchedule() {
    getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->resetOutdoorAirFlowRateFractionSchedule();
  }

  /// @cond
  DesignSpecificationOutdoorAir::DesignSpecificationOutdoorAir(std::shared_ptr<detail::DesignSpecificationOutdoorAir_Impl> impl)
    : ResourceObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
