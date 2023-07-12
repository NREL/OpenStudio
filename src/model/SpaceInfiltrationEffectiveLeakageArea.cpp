/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "SpaceInfiltrationEffectiveLeakageArea.hpp"
#include "SpaceInfiltrationEffectiveLeakageArea_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Space.hpp"
#include "Space_Impl.hpp"
#include "SpaceType.hpp"
#include "SpaceType_Impl.hpp"
#include "DefaultScheduleSet.hpp"
#include "DefaultScheduleSet_Impl.hpp"
#include "LifeCycleCost.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_SpaceInfiltration_EffectiveLeakageArea_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    SpaceInfiltrationEffectiveLeakageArea_Impl::SpaceInfiltrationEffectiveLeakageArea_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                           bool keepHandle)
      : SpaceLoad_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == SpaceInfiltrationEffectiveLeakageArea::iddObjectType());
    }

    SpaceInfiltrationEffectiveLeakageArea_Impl::SpaceInfiltrationEffectiveLeakageArea_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                           Model_Impl* model, bool keepHandle)
      : SpaceLoad_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == SpaceInfiltrationEffectiveLeakageArea::iddObjectType());
    }

    SpaceInfiltrationEffectiveLeakageArea_Impl::SpaceInfiltrationEffectiveLeakageArea_Impl(const SpaceInfiltrationEffectiveLeakageArea_Impl& other,
                                                                                           Model_Impl* model, bool keepHandle)
      : SpaceLoad_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& SpaceInfiltrationEffectiveLeakageArea_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType SpaceInfiltrationEffectiveLeakageArea_Impl::iddObjectType() const {
      return SpaceInfiltrationEffectiveLeakageArea::iddObjectType();
    }

    std::vector<ScheduleTypeKey> SpaceInfiltrationEffectiveLeakageArea_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_SpaceInfiltration_EffectiveLeakageAreaFields::ScheduleName) != e) {
        result.push_back(ScheduleTypeKey("SpaceInfiltrationEffectiveLeakageArea", "Infiltration"));
      }
      return result;
    }

    bool SpaceInfiltrationEffectiveLeakageArea_Impl::hardSize() {
      boost::optional<Space> space = this->space();
      if (!space) {
        return false;
      }
      return false;
    }

    bool SpaceInfiltrationEffectiveLeakageArea_Impl::hardApplySchedules() {
      bool result = false;
      boost::optional<Schedule> schedule = this->schedule();
      if (schedule) {
        result = this->setSchedule(*schedule);
      }
      return result;
    }

    bool SpaceInfiltrationEffectiveLeakageArea_Impl::isAbsolute() const {
      return true;
    }

    double SpaceInfiltrationEffectiveLeakageArea_Impl::effectiveAirLeakageArea() const {
      boost::optional<double> value = getDouble(OS_SpaceInfiltration_EffectiveLeakageAreaFields::EffectiveAirLeakageArea, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SpaceInfiltrationEffectiveLeakageArea_Impl::stackCoefficient() const {
      boost::optional<double> value = getDouble(OS_SpaceInfiltration_EffectiveLeakageAreaFields::StackCoefficient, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SpaceInfiltrationEffectiveLeakageArea_Impl::windCoefficient() const {
      boost::optional<double> value = getDouble(OS_SpaceInfiltration_EffectiveLeakageAreaFields::WindCoefficient, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SpaceInfiltrationEffectiveLeakageArea_Impl::setEffectiveAirLeakageArea(double effectiveAirLeakageArea) {
      bool result = setDouble(OS_SpaceInfiltration_EffectiveLeakageAreaFields::EffectiveAirLeakageArea, effectiveAirLeakageArea);
      return result;
    }

    bool SpaceInfiltrationEffectiveLeakageArea_Impl::setStackCoefficient(double stackCoefficient) {
      bool result = setDouble(OS_SpaceInfiltration_EffectiveLeakageAreaFields::StackCoefficient, stackCoefficient);
      return result;
    }

    bool SpaceInfiltrationEffectiveLeakageArea_Impl::setWindCoefficient(double windCoefficient) {
      bool result = setDouble(OS_SpaceInfiltration_EffectiveLeakageAreaFields::WindCoefficient, windCoefficient);
      return result;
    }

    int SpaceInfiltrationEffectiveLeakageArea_Impl::spaceIndex() const {
      return OS_SpaceInfiltration_EffectiveLeakageAreaFields::SpaceorSpaceTypeName;
    }

    boost::optional<Schedule> SpaceInfiltrationEffectiveLeakageArea_Impl::schedule() const {
      boost::optional<Schedule> result =
        getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_SpaceInfiltration_EffectiveLeakageAreaFields::ScheduleName);
      if (!result) {
        // search upwards
        OptionalSpace space = this->space();
        OptionalSpaceType spaceType = this->spaceType();
        if (space) {
          result = space->getDefaultSchedule(DefaultScheduleType::InfiltrationSchedule);
        } else if (spaceType) {
          result = spaceType->getDefaultSchedule(DefaultScheduleType::InfiltrationSchedule);
        }
      }
      return result;
    }

    bool SpaceInfiltrationEffectiveLeakageArea_Impl::isScheduleDefaulted() const {
      return isEmpty(OS_SpaceInfiltration_EffectiveLeakageAreaFields::ScheduleName);
    }

    bool SpaceInfiltrationEffectiveLeakageArea_Impl::setSchedule(Schedule& schedule) {
      bool result = ModelObject_Impl::setSchedule(OS_SpaceInfiltration_EffectiveLeakageAreaFields::ScheduleName,
                                                  "SpaceInfiltrationEffectiveLeakageArea", "Infiltration", schedule);
      return result;
    }

    void SpaceInfiltrationEffectiveLeakageArea_Impl::resetSchedule() {
      bool result = setString(OS_SpaceInfiltration_EffectiveLeakageAreaFields::ScheduleName, "");
      OS_ASSERT(result);
    }

    boost::optional<ModelObject> SpaceInfiltrationEffectiveLeakageArea_Impl::scheduleAsModelObject() const {
      OptionalModelObject result;
      OptionalSchedule intermediate = schedule();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    bool SpaceInfiltrationEffectiveLeakageArea_Impl::setScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
        if (intermediate) {
          Schedule schedule(*intermediate);
          return setSchedule(schedule);
        } else {
          return false;
        }
      } else {
        resetSchedule();
      }
      return true;
    }

  }  // namespace detail

  SpaceInfiltrationEffectiveLeakageArea::SpaceInfiltrationEffectiveLeakageArea(const Model& model)
    : SpaceLoad(SpaceInfiltrationEffectiveLeakageArea::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::SpaceInfiltrationEffectiveLeakageArea_Impl>());
    bool test = setEffectiveAirLeakageArea(0.1);
    OS_ASSERT(test);
    test = setStackCoefficient(0.1);
    OS_ASSERT(test);
    test = setWindCoefficient(0.1);
    OS_ASSERT(test);
  }

  IddObjectType SpaceInfiltrationEffectiveLeakageArea::iddObjectType() {
    IddObjectType result(IddObjectType::OS_SpaceInfiltration_EffectiveLeakageArea);
    return result;
  }

  boost::optional<Schedule> SpaceInfiltrationEffectiveLeakageArea::schedule() const {
    return getImpl<detail::SpaceInfiltrationEffectiveLeakageArea_Impl>()->schedule();
  }

  bool SpaceInfiltrationEffectiveLeakageArea::isScheduleDefaulted() const {
    return getImpl<detail::SpaceInfiltrationEffectiveLeakageArea_Impl>()->isScheduleDefaulted();
  }

  double SpaceInfiltrationEffectiveLeakageArea::effectiveAirLeakageArea() const {
    return getImpl<detail::SpaceInfiltrationEffectiveLeakageArea_Impl>()->effectiveAirLeakageArea();
  }

  double SpaceInfiltrationEffectiveLeakageArea::stackCoefficient() const {
    return getImpl<detail::SpaceInfiltrationEffectiveLeakageArea_Impl>()->stackCoefficient();
  }

  double SpaceInfiltrationEffectiveLeakageArea::windCoefficient() const {
    return getImpl<detail::SpaceInfiltrationEffectiveLeakageArea_Impl>()->windCoefficient();
  }

  bool SpaceInfiltrationEffectiveLeakageArea::setSchedule(Schedule& schedule) {
    return getImpl<detail::SpaceInfiltrationEffectiveLeakageArea_Impl>()->setSchedule(schedule);
  }

  void SpaceInfiltrationEffectiveLeakageArea::resetSchedule() {
    getImpl<detail::SpaceInfiltrationEffectiveLeakageArea_Impl>()->resetSchedule();
  }

  bool SpaceInfiltrationEffectiveLeakageArea::setEffectiveAirLeakageArea(double effectiveAirLeakageArea) {
    return getImpl<detail::SpaceInfiltrationEffectiveLeakageArea_Impl>()->setEffectiveAirLeakageArea(effectiveAirLeakageArea);
  }

  bool SpaceInfiltrationEffectiveLeakageArea::setStackCoefficient(double stackCoefficient) {
    return getImpl<detail::SpaceInfiltrationEffectiveLeakageArea_Impl>()->setStackCoefficient(stackCoefficient);
  }

  bool SpaceInfiltrationEffectiveLeakageArea::setWindCoefficient(double windCoefficient) {
    return getImpl<detail::SpaceInfiltrationEffectiveLeakageArea_Impl>()->setWindCoefficient(windCoefficient);
  }

  /// @cond
  SpaceInfiltrationEffectiveLeakageArea::SpaceInfiltrationEffectiveLeakageArea(
    std::shared_ptr<detail::SpaceInfiltrationEffectiveLeakageArea_Impl> impl)
    : SpaceLoad(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
