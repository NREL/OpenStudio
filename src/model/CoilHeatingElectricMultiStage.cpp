/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "CoilHeatingElectricMultiStage.hpp"
#include "CoilHeatingElectricMultiStage_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "CoilHeatingElectricMultiStageStageData.hpp"
#include "CoilHeatingElectricMultiStageStageData_Impl.hpp"
#include "AirLoopHVACUnitarySystem.hpp"
#include "AirLoopHVACUnitarySystem_Impl.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/OS_Coil_Heating_Electric_MultiStage_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"
#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    CoilHeatingElectricMultiStage_Impl::CoilHeatingElectricMultiStage_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == CoilHeatingElectricMultiStage::iddObjectType());
    }

    CoilHeatingElectricMultiStage_Impl::CoilHeatingElectricMultiStage_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                           bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == CoilHeatingElectricMultiStage::iddObjectType());
    }

    CoilHeatingElectricMultiStage_Impl::CoilHeatingElectricMultiStage_Impl(const CoilHeatingElectricMultiStage_Impl& other, Model_Impl* model,
                                                                           bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& CoilHeatingElectricMultiStage_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Heating Coil Heating Energy", "Heating Coil Heating Rate", "Heating Coil Electric Consumption",
                                                   "Heating Coil Electricity Rate"};
      return result;
    }

    IddObjectType CoilHeatingElectricMultiStage_Impl::iddObjectType() const {
      return CoilHeatingElectricMultiStage::iddObjectType();
    }

    std::vector<ScheduleTypeKey> CoilHeatingElectricMultiStage_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_Coil_Heating_Electric_MultiStageFields::AvailabilitySchedule) != e) {
        result.emplace_back("CoilHeatingElectricMultiStage", "Availability Schedule");
      }
      return result;
    }

    unsigned CoilHeatingElectricMultiStage_Impl::inletPort() const {
      return OS_Coil_Heating_Electric_MultiStageFields::AirInletNode;
    }

    unsigned CoilHeatingElectricMultiStage_Impl::outletPort() const {
      return OS_Coil_Heating_Electric_MultiStageFields::AirOutletNode;
    }

    ModelObject CoilHeatingElectricMultiStage_Impl::clone(Model model) const {
      auto t_clone = StraightComponent_Impl::clone(model).cast<CoilHeatingElectricMultiStage>();

      // Deal with the stages: we want them cloned, so first clear and then clone + assign each
      t_clone.removeAllStages();

      for (const auto& stage : stages()) {
        auto stageClone = stage.clone(model).cast<CoilHeatingElectricMultiStageStageData>();
        t_clone.addStage(stageClone);
      }
      return std::move(t_clone);
    }

    std::vector<ModelObject> CoilHeatingElectricMultiStage_Impl::children() const {
      return subsetCastVector<ModelObject>(stages());
    }

    boost::optional<HVACComponent> CoilHeatingElectricMultiStage_Impl::containingHVACComponent() const {
      // AirLoopHVACUnitarySystem
      std::vector<AirLoopHVACUnitarySystem> airLoopHVACUnitarySystems = this->model().getConcreteModelObjects<AirLoopHVACUnitarySystem>();

      for (const auto& airLoopHVACUnitarySystem : airLoopHVACUnitarySystems) {
        if (boost::optional<HVACComponent> heatingCoil = airLoopHVACUnitarySystem.heatingCoil()) {
          if (heatingCoil->handle() == this->handle()) {
            return airLoopHVACUnitarySystem;
          }
        }
        if (boost::optional<HVACComponent> suppHeatingCoil = airLoopHVACUnitarySystem.supplementalHeatingCoil()) {
          if (suppHeatingCoil->handle() == this->handle()) {
            return airLoopHVACUnitarySystem;
          }
        }
      }

      // AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed
      std::vector<AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed> airLoopHVACUnitaryHeatPumpAirToAirMultiSpeeds =
        this->model().getConcreteModelObjects<AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed>();

      for (const auto& airLoopHVACUnitaryHeatPumpAirToAirMultiSpeed : airLoopHVACUnitaryHeatPumpAirToAirMultiSpeeds) {
        if (boost::optional<HVACComponent> heatingCoil = airLoopHVACUnitaryHeatPumpAirToAirMultiSpeed.heatingCoil()) {
          if (heatingCoil->handle() == this->handle()) {
            return airLoopHVACUnitaryHeatPumpAirToAirMultiSpeed;
          }
        }
      }

      return boost::none;
    }

    bool CoilHeatingElectricMultiStage_Impl::addToNode(Node& /*node*/) {
      return false;
    }

    boost::optional<Schedule> CoilHeatingElectricMultiStage_Impl::optionalAvailabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Heating_Electric_MultiStageFields::AvailabilitySchedule);
    }

    Schedule CoilHeatingElectricMultiStage_Impl::availabilitySchedule() const {
      boost::optional<Schedule> value = optionalAvailabilitySchedule();
      if (!value) {
        // it is an error if we get here, however we don't want to crash
        // so we hook up to global always on schedule
        LOG(Error, "Required availability schedule not set, using 'Always On' schedule");
        value = this->model().alwaysOnDiscreteSchedule();
        OS_ASSERT(value);
        const_cast<CoilHeatingElectricMultiStage_Impl*>(this)->setAvailabilitySchedule(*value);
        value = optionalAvailabilitySchedule();
      }
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilHeatingElectricMultiStage_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_Coil_Heating_Electric_MultiStageFields::AvailabilitySchedule, "CoilHeatingElectricMultiStage",
                                "Availability Schedule", schedule);
      return result;
    }

    unsigned CoilHeatingElectricMultiStage_Impl::numberOfStages() const {
      return numExtensibleGroups();
    }

    std::vector<CoilHeatingElectricMultiStageStageData> CoilHeatingElectricMultiStage_Impl::stages() const {
      std::vector<CoilHeatingElectricMultiStageStageData> result;
      for (const auto& group : extensibleGroups()) {
        if (auto target = group.cast<WorkspaceExtensibleGroup>().getTarget(OS_Coil_Heating_Electric_MultiStageExtensibleFields::Stage)) {
          if (auto stage = target->optionalCast<CoilHeatingElectricMultiStageStageData>()) {
            result.push_back(stage.get());
          }
        }
      }
      return result;
    }

    boost::optional<unsigned> CoilHeatingElectricMultiStage_Impl::stageIndex(const CoilHeatingElectricMultiStageStageData& stage) const {

      boost::optional<unsigned> result;

      auto egs = castVector<WorkspaceExtensibleGroup>(extensibleGroups());
      auto h = openstudio::toString(stage.handle());
      auto it = std::find_if(egs.begin(), egs.end(), [&](const WorkspaceExtensibleGroup& eg) {
        return (eg.getField(OS_Coil_Heating_Electric_MultiStageExtensibleFields::Stage).get() == h);
      });

      // If found, we compute the index by using std::distance between the start of vector and the iterator returned by std::find_if
      if (it != egs.end()) {
        result = std::distance(egs.begin(), it) + 1;
      }

      return result;
    }

    bool CoilHeatingElectricMultiStage_Impl::addStage(const CoilHeatingElectricMultiStageStageData& stage) {
      if (auto _c = stage.parentCoil()) {
        if (this->handle() == _c->handle()) {
          return true;  // already the case
        } else {
          LOG(Error, "For " << briefDescription() << " cannot add " << stage.briefDescription()
                            << " since this Stage is already in use by another coil ('" << _c->nameString() << "').");
          return false;
        }
      }
      auto group = getObject<ModelObject>().pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();
      if (group.empty()) {
        LOG(Error, "You have reached the maximum number of stages (=" << numberOfStages() << "), occurred for " << briefDescription() << ".");
        return false;
      }
      bool result = group.setPointer(OS_Coil_Heating_Electric_MultiStageExtensibleFields::Stage, stage.handle());
      if (!result) {
        // Something went wrong, so erase the new extensible group
        getObject<ModelObject>().eraseExtensibleGroup(group.groupIndex());
      }
      return result;
    }

    bool CoilHeatingElectricMultiStage_Impl::setStageIndex(const CoilHeatingElectricMultiStageStageData& stage, unsigned index) {
      boost::optional<unsigned> idx = stageIndex(stage);
      if (!idx) {
        LOG(Warn, "For " << briefDescription() << " cannot set the index of stage " << stage.briefDescription() << " since it is not part of it.");
        return false;
      }

      // TODO: we could just set via string instead of doing a ton of typechecking below...

      std::vector<CoilHeatingElectricMultiStageStageData> stageVector = stages();

      if (index > stageVector.size()) {
        LOG(Warn, "Requested a stage index of " << index << " to be assigned to " << stage.briefDescription() << ", but " << briefDescription()
                                                << " only has " << stageVector.size() << " stages, resetting to that.");
        index = stageVector.size();
      } else if (index < 1) {
        LOG(Warn, "Requested a stage index of " << index << " < 1 to be assigned to " << stage.briefDescription() << ", resetting to 1");
        index = 1;
      }

      stageVector.erase(stageVector.begin() + idx.get() - 1);  // stageIndex is 1-indexed, and vector is 0-indexed

      stageVector.insert(stageVector.begin() + (index - 1), stage);

      return setStages(stageVector);
    }

    bool CoilHeatingElectricMultiStage_Impl::addStage(const CoilHeatingElectricMultiStageStageData& stage, unsigned index) {
      bool ok = addStage(stage);
      if (!ok) {
        return false;
      }
      ok = setStageIndex(stage, index);
      return ok;
    }

    bool CoilHeatingElectricMultiStage_Impl::setStages(const std::vector<CoilHeatingElectricMultiStageStageData>& stages) {
      // Clear the extensible groups, and redo them
      bool ok = true;
      clearExtensibleGroups();
      for (const CoilHeatingElectricMultiStageStageData& s : stages) {
        ok &= addStage(s);
      }
      return ok;
    }

    void CoilHeatingElectricMultiStage_Impl::removeAllStages() {
      clearExtensibleGroups();
    }

    bool CoilHeatingElectricMultiStage_Impl::removeStage(const CoilHeatingElectricMultiStageStageData& stage) {
      boost::optional<unsigned> idx = stageIndex(stage);
      if (!idx) {
        LOG(Warn, "For " << briefDescription() << " cannot remove stage " << stage.briefDescription() << " since it is not part of it.");
        return false;
      }

      return removeStage(idx.get());
    }

    bool CoilHeatingElectricMultiStage_Impl::removeStage(unsigned index) {
      bool result = false;
      if ((index > 0) && (index <= numberOfStages())) {
        getObject<ModelObject>().eraseExtensibleGroup(index - 1);
        result = true;
      }
      return result;
    }

    ComponentType CoilHeatingElectricMultiStage_Impl::componentType() const {
      return ComponentType::Heating;
    }

    std::vector<FuelType> CoilHeatingElectricMultiStage_Impl::coolingFuelTypes() const {
      return {};
    }

    std::vector<FuelType> CoilHeatingElectricMultiStage_Impl::heatingFuelTypes() const {
      return {FuelType::Electricity};
    }

    std::vector<AppGFuelType> CoilHeatingElectricMultiStage_Impl::appGHeatingFuelTypes() const {
      return {AppGFuelType::Electric};
    }

  }  // namespace detail

  CoilHeatingElectricMultiStage::CoilHeatingElectricMultiStage(const Model& model)
    : StraightComponent(CoilHeatingElectricMultiStage::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CoilHeatingElectricMultiStage_Impl>());

    auto always_on = model.alwaysOnDiscreteSchedule();
    bool ok = setAvailabilitySchedule(always_on);
    OS_ASSERT(ok);
  }

  IddObjectType CoilHeatingElectricMultiStage::iddObjectType() {
    return IddObjectType(IddObjectType::OS_Coil_Heating_Electric_MultiStage);
  }

  Schedule CoilHeatingElectricMultiStage::availabilitySchedule() const {
    return getImpl<detail::CoilHeatingElectricMultiStage_Impl>()->availabilitySchedule();
  }

  bool CoilHeatingElectricMultiStage::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::CoilHeatingElectricMultiStage_Impl>()->setAvailabilitySchedule(schedule);
  }

  unsigned CoilHeatingElectricMultiStage::numberOfStages() const {
    return getImpl<detail::CoilHeatingElectricMultiStage_Impl>()->numberOfStages();
  }

  boost::optional<unsigned> CoilHeatingElectricMultiStage::stageIndex(const CoilHeatingElectricMultiStageStageData& stage) const {
    return getImpl<detail::CoilHeatingElectricMultiStage_Impl>()->stageIndex(stage);
  }

  std::vector<CoilHeatingElectricMultiStageStageData> CoilHeatingElectricMultiStage::stages() const {
    return getImpl<detail::CoilHeatingElectricMultiStage_Impl>()->stages();
  }

  bool CoilHeatingElectricMultiStage::addStage(const CoilHeatingElectricMultiStageStageData& stage) {
    return getImpl<detail::CoilHeatingElectricMultiStage_Impl>()->addStage(stage);
  }

  bool CoilHeatingElectricMultiStage::addStage(const CoilHeatingElectricMultiStageStageData& stage, unsigned index) {
    return getImpl<detail::CoilHeatingElectricMultiStage_Impl>()->addStage(stage, index);
  }

  bool CoilHeatingElectricMultiStage::setStageIndex(const CoilHeatingElectricMultiStageStageData& stage, unsigned index) {
    return getImpl<detail::CoilHeatingElectricMultiStage_Impl>()->setStageIndex(stage, index);
  }

  bool CoilHeatingElectricMultiStage::setStages(const std::vector<CoilHeatingElectricMultiStageStageData>& stages) {
    return getImpl<detail::CoilHeatingElectricMultiStage_Impl>()->setStages(stages);
  }

  void CoilHeatingElectricMultiStage::removeAllStages() {
    getImpl<detail::CoilHeatingElectricMultiStage_Impl>()->removeAllStages();
  }

  bool CoilHeatingElectricMultiStage::removeStage(const CoilHeatingElectricMultiStageStageData& stage) {
    return getImpl<detail::CoilHeatingElectricMultiStage_Impl>()->removeStage(stage);
  }

  bool CoilHeatingElectricMultiStage::removeStage(unsigned index) {
    return getImpl<detail::CoilHeatingElectricMultiStage_Impl>()->removeStage(index);
  }

  /// @cond
  CoilHeatingElectricMultiStage::CoilHeatingElectricMultiStage(std::shared_ptr<detail::CoilHeatingElectricMultiStage_Impl> impl)
    : StraightComponent(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
