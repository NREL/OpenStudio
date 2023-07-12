/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ScheduleBase.hpp"
#include "ScheduleBase_Impl.hpp"

#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeLimits_Impl.hpp"

#include "../utilities/idf/ValidityReport.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    // constructor
    ScheduleBase_Impl::ScheduleBase_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ResourceObject_Impl(idfObject, model, keepHandle) {}

    ScheduleBase_Impl::ScheduleBase_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ResourceObject_Impl(other, model, keepHandle) {}

    ScheduleBase_Impl::ScheduleBase_Impl(const ScheduleBase_Impl& other, Model_Impl* model, bool keepHandles)
      : ResourceObject_Impl(other, model, keepHandles) {}

    bool ScheduleBase_Impl::okToResetScheduleTypeLimits() const {
      // can only reset if not used by object with keys
      for (const ModelObject& user : getObject<ScheduleBase>().getModelObjectSources<ModelObject>()) {
        if (!user.optionalCast<ScheduleBase>()) {
          return false;
        }
      }
      return true;
    }

    bool ScheduleBase_Impl::valuesAreWithinBounds() const {
      if (OptionalScheduleTypeLimits scheduleTypeLimits = this->scheduleTypeLimits()) {
        OptionalDouble minBound = scheduleTypeLimits->lowerLimitValue();
        OptionalDouble maxBound = scheduleTypeLimits->upperLimitValue();
        if (minBound || maxBound) {
          for (double value : values()) {
            if (minBound && (value < *minBound)) {
              return false;
            }
            if (maxBound && (value > *maxBound)) {
              return false;
            }
          }
        }
      }
      return true;
    }

    boost::optional<ModelObject> ScheduleBase_Impl::scheduleTypeLimitsAsModelObject() const {
      OptionalModelObject result;
      OptionalScheduleTypeLimits intermediate = scheduleTypeLimits();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    bool ScheduleBase_Impl::setScheduleTypeLimitsAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalScheduleTypeLimits intermediate = modelObject->optionalCast<ScheduleTypeLimits>();
        if (intermediate) {
          return setScheduleTypeLimits(*intermediate);
        } else {
          return false;
        }
      } else {
        resetScheduleTypeLimits();
      }
      return true;
    }

  }  // namespace detail

  boost::optional<ScheduleTypeLimits> ScheduleBase::scheduleTypeLimits() const {
    return getImpl<detail::ScheduleBase_Impl>()->scheduleTypeLimits();
  }

  bool ScheduleBase::setScheduleTypeLimits(const ScheduleTypeLimits& scheduleTypeLimits) {
    return getImpl<detail::ScheduleBase_Impl>()->setScheduleTypeLimits(scheduleTypeLimits);
  }

  bool ScheduleBase::resetScheduleTypeLimits() {
    return getImpl<detail::ScheduleBase_Impl>()->resetScheduleTypeLimits();
  }

  void ScheduleBase::ensureNoLeapDays() {
    getImpl<detail::ScheduleBase_Impl>()->ensureNoLeapDays();
  }

  /// @cond
  ScheduleBase::ScheduleBase(std::shared_ptr<detail::ScheduleBase_Impl> impl) : ResourceObject(std::move(impl)) {}

  ScheduleBase::ScheduleBase(IddObjectType type, const Model& model) : ResourceObject(type, model) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
