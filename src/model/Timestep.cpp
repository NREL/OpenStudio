/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "Timestep.hpp"
#include "Timestep_Impl.hpp"
#include "SimulationControl.hpp"
#include "SimulationControl_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "ScheduleDay.hpp"
#include "ScheduleDay_Impl.hpp"
#include "../utilities/core/Assert.hpp"
#include <utilities/idd/OS_Timestep_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {

namespace model {

  namespace detail {

    Timestep_Impl::Timestep_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle) : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == Timestep::iddObjectType());
    }

    Timestep_Impl::Timestep_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == Timestep::iddObjectType());
    }

    Timestep_Impl::Timestep_Impl(const Timestep_Impl& other, Model_Impl* model, bool keepHandle) : ModelObject_Impl(other, model, keepHandle) {}

    IddObjectType Timestep_Impl::iddObjectType() const {
      return Timestep::iddObjectType();
    }

    // return the parent object in the hierarchy
    boost::optional<ParentObject> Timestep_Impl::parent() const {
      boost::optional<ParentObject> result(model().getUniqueModelObject<SimulationControl>());
      return result;
    }

    // set the parent, child may have to call methods on the parent
    bool Timestep_Impl::setParent(ParentObject& newParent) {
      if (newParent.optionalCast<SimulationControl>() && (newParent.model() == model())) {
        return true;
      }
      return false;
    }

    // TODO: remove
    const std::vector<std::string>& Timestep_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      // Not appropriate: no specific output
      return result;
    }

    int Timestep_Impl::numberOfTimestepsPerHour() const {
      boost::optional<int> value = getInt(OS_TimestepFields::NumberofTimestepsperHour, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool Timestep_Impl::isNumberOfTimestepsPerHourDefaulted() const {
      return isEmpty(OS_TimestepFields::NumberofTimestepsperHour);
    }

    bool Timestep_Impl::setNumberOfTimestepsPerHour(int numberOfTimestepsPerHour) {
      if (numberOfTimestepsPerHour != this->numberOfTimestepsPerHour()) {
        for (auto& sch_day : model().getConcreteModelObjects<ScheduleDay>()) {
          sch_day.getImpl<ScheduleDay_Impl>()->clearCachedTimeSeries();
        }
      }
      bool result = setInt(OS_TimestepFields::NumberofTimestepsperHour, numberOfTimestepsPerHour);
      return result;
    }

    void Timestep_Impl::resetNumberOfTimestepsPerHour() {
      for (auto& sch_day : model().getConcreteModelObjects<ScheduleDay>()) {
        sch_day.getImpl<ScheduleDay_Impl>()->clearCachedTimeSeries();
      }
      bool result = setString(OS_TimestepFields::NumberofTimestepsperHour, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  /// constructor
  Timestep::Timestep(const Model& model) : ModelObject(Timestep::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::Timestep_Impl>());

    setNumberOfTimestepsPerHour(6);
  }

  // constructor
  Timestep::Timestep(std::shared_ptr<detail::Timestep_Impl> impl) : ModelObject(std::move(impl)) {}

  IddObjectType Timestep::iddObjectType() {
    IddObjectType result(IddObjectType::OS_Timestep);
    return result;
  }

  int Timestep::numberOfTimestepsPerHour() const {
    return getImpl<detail::Timestep_Impl>()->numberOfTimestepsPerHour();
  }

  bool Timestep::isNumberOfTimestepsPerHourDefaulted() const {
    return getImpl<detail::Timestep_Impl>()->isNumberOfTimestepsPerHourDefaulted();
  }

  bool Timestep::setNumberOfTimestepsPerHour(int numberOfTimestepsPerHour) {
    return getImpl<detail::Timestep_Impl>()->setNumberOfTimestepsPerHour(numberOfTimestepsPerHour);
  }

  void Timestep::resetNumberOfTimestepsPerHour() {
    getImpl<detail::Timestep_Impl>()->resetNumberOfTimestepsPerHour();
  }

}  // namespace model
}  // namespace openstudio
