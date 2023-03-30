/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "CoilHeatingGasMultiStage.hpp"
#include "CoilHeatingGasMultiStage_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "CoilHeatingGasMultiStageStageData.hpp"
#include "CoilHeatingGasMultiStageStageData_Impl.hpp"
#include "AirLoopHVACUnitarySystem.hpp"
#include "AirLoopHVACUnitarySystem_Impl.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/OS_Coil_Heating_Gas_MultiStage_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"
#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    CoilHeatingGasMultiStage_Impl::CoilHeatingGasMultiStage_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == CoilHeatingGasMultiStage::iddObjectType());
    }

    CoilHeatingGasMultiStage_Impl::CoilHeatingGasMultiStage_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                 bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == CoilHeatingGasMultiStage::iddObjectType());
    }

    CoilHeatingGasMultiStage_Impl::CoilHeatingGasMultiStage_Impl(const CoilHeatingGasMultiStage_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& CoilHeatingGasMultiStage_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Heating Coil Heating Energy",
                                                   "Heating Coil Heating Rate",
                                                   "Heating Coil Gas Consumption",
                                                   "Heating Coil Gas Consumption Rate",
                                                   "Heating Coil Electric Consumption",
                                                   "Heating Coil Electricity Rate",
                                                   "Heating Coil Runtime Fraction",
                                                   "Heating Coil Parasitic Gas Consumption",
                                                   "Heating Coil Parasitic Gas Consumption Rate"};
      return result;
    }

    IddObjectType CoilHeatingGasMultiStage_Impl::iddObjectType() const {
      return CoilHeatingGasMultiStage::iddObjectType();
    }

    std::vector<ScheduleTypeKey> CoilHeatingGasMultiStage_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_Coil_Heating_Gas_MultiStageFields::AvailabilitySchedule) != e) {
        result.emplace_back("CoilHeatingGasMultiStage", "Availability Schedule");
      }
      return result;
    }

    unsigned CoilHeatingGasMultiStage_Impl::inletPort() const {
      return OS_Coil_Heating_Gas_MultiStageFields::AirInletNode;
    }

    unsigned CoilHeatingGasMultiStage_Impl::outletPort() const {
      return OS_Coil_Heating_Gas_MultiStageFields::AirOutletNode;
    }

    ModelObject CoilHeatingGasMultiStage_Impl::clone(Model model) const {
      auto t_clone = StraightComponent_Impl::clone(model).cast<CoilHeatingGasMultiStage>();

      // Deal with the stages: we want them cloned, so first clear and then clone + assign each
      t_clone.removeAllStages();

      for (const auto& stage : stages()) {
        auto stageClone = stage.clone(model).cast<CoilHeatingGasMultiStageStageData>();
        t_clone.addStage(stageClone);
      }
      return std::move(t_clone);
    }

    std::vector<ModelObject> CoilHeatingGasMultiStage_Impl::children() const {
      std::vector<ModelObject> result = subsetCastVector<ModelObject>(stages());

      if (auto const curve = partLoadFractionCorrelationCurve()) {
        result.push_back(curve.get());
      }

      return result;
    }

    boost::optional<HVACComponent> CoilHeatingGasMultiStage_Impl::containingHVACComponent() const {
      // AirLoopHVACUnitarySystem
      std::vector<AirLoopHVACUnitarySystem> airLoopHVACUnitarySystems = this->model().getConcreteModelObjects<AirLoopHVACUnitarySystem>();

      for (const auto& airLoopHVACUnitarySystem : airLoopHVACUnitarySystems) {
        if (boost::optional<HVACComponent> heatingCoil = airLoopHVACUnitarySystem.heatingCoil()) {
          if (heatingCoil->handle() == this->handle()) {
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

    bool CoilHeatingGasMultiStage_Impl::addToNode(Node& node) {
      return false;
    }

    boost::optional<Schedule> CoilHeatingGasMultiStage_Impl::optionalAvailabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Heating_Gas_MultiStageFields::AvailabilitySchedule);
    }

    Schedule CoilHeatingGasMultiStage_Impl::availabilitySchedule() const {
      boost::optional<Schedule> value = optionalAvailabilitySchedule();
      if (!value) {
        // it is an error if we get here, however we don't want to crash
        // so we hook up to global always on schedule
        LOG(Error, "Required availability schedule not set, using 'Always On' schedule");
        value = this->model().alwaysOnDiscreteSchedule();
        OS_ASSERT(value);
        const_cast<CoilHeatingGasMultiStage_Impl*>(this)->setAvailabilitySchedule(*value);
        value = optionalAvailabilitySchedule();
      }
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Curve> CoilHeatingGasMultiStage_Impl::partLoadFractionCorrelationCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Heating_Gas_MultiStageFields::PartLoadFractionCorrelationCurve);
    }

    boost::optional<double> CoilHeatingGasMultiStage_Impl::parasiticGasLoad() const {
      return getDouble(OS_Coil_Heating_Gas_MultiStageFields::ParasiticGasLoad, true);
    }

    bool CoilHeatingGasMultiStage_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result =
        setSchedule(OS_Coil_Heating_Gas_MultiStageFields::AvailabilitySchedule, "CoilHeatingGasMultiStage", "Availability Schedule", schedule);
      return result;
    }

    bool CoilHeatingGasMultiStage_Impl::setPartLoadFractionCorrelationCurve(const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(OS_Coil_Heating_Gas_MultiStageFields::PartLoadFractionCorrelationCurve, curve.get().handle());
      } else {
        resetPartLoadFractionCorrelationCurve();
        result = true;
      }
      return result;
    }

    void CoilHeatingGasMultiStage_Impl::resetPartLoadFractionCorrelationCurve() {
      bool result = setString(OS_Coil_Heating_Gas_MultiStageFields::PartLoadFractionCorrelationCurve, "");
      OS_ASSERT(result);
    }

    bool CoilHeatingGasMultiStage_Impl::setParasiticGasLoad(boost::optional<double> parasiticGasLoad) {
      bool result(false);
      if (parasiticGasLoad) {
        result = setDouble(OS_Coil_Heating_Gas_MultiStageFields::ParasiticGasLoad, parasiticGasLoad.get());
      } else {
        resetParasiticGasLoad();
        result = true;
      }
      OS_ASSERT(result);
      return result;
    }

    void CoilHeatingGasMultiStage_Impl::resetParasiticGasLoad() {
      bool result = setString(OS_Coil_Heating_Gas_MultiStageFields::ParasiticGasLoad, "");
      OS_ASSERT(result);
    }

    unsigned CoilHeatingGasMultiStage_Impl::numberOfStages() const {
      return numExtensibleGroups();
    }

    std::vector<CoilHeatingGasMultiStageStageData> CoilHeatingGasMultiStage_Impl::stages() const {
      std::vector<CoilHeatingGasMultiStageStageData> result;
      for (const auto& group : extensibleGroups()) {
        if (auto target = group.cast<WorkspaceExtensibleGroup>().getTarget(OS_Coil_Heating_Gas_MultiStageExtensibleFields::Stage)) {
          if (auto stage = target->optionalCast<CoilHeatingGasMultiStageStageData>()) {
            result.push_back(stage.get());
          }
        }
      }
      return result;
    }

    boost::optional<unsigned> CoilHeatingGasMultiStage_Impl::stageIndex(const CoilHeatingGasMultiStageStageData& stage) const {

      boost::optional<unsigned> result;

      auto egs = castVector<WorkspaceExtensibleGroup>(extensibleGroups());
      auto h = openstudio::toString(stage.handle());
      auto it = std::find_if(egs.begin(), egs.end(), [&](const WorkspaceExtensibleGroup& eg) {
        return (eg.getField(OS_Coil_Heating_Gas_MultiStageExtensibleFields::Stage).get() == h);
      });

      // If found, we compute the index by using std::distance between the start of vector and the iterator returned by std::find_if
      if (it != egs.end()) {
        result = std::distance(egs.begin(), it) + 1;
      }

      return result;
    }

    bool CoilHeatingGasMultiStage_Impl::addStage(const CoilHeatingGasMultiStageStageData& stage) {
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
      bool result = group.setPointer(OS_Coil_Heating_Gas_MultiStageExtensibleFields::Stage, stage.handle());
      if (!result) {
        // Something went wrong, so erase the new extensible group
        getObject<ModelObject>().eraseExtensibleGroup(group.groupIndex());
      }
      return result;
    }

    bool CoilHeatingGasMultiStage_Impl::setStageIndex(const CoilHeatingGasMultiStageStageData& stage, unsigned index) {
      boost::optional<unsigned> idx = stageIndex(stage);
      if (!idx) {
        LOG(Warn, "For " << briefDescription() << " cannot set the index of stage " << stage.briefDescription() << " since it is not part of it.");
        return false;
      }

      // TODO: we could just set via string instead of doing a ton of typechecking below...

      std::vector<CoilHeatingGasMultiStageStageData> stageVector = stages();

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

    bool CoilHeatingGasMultiStage_Impl::addStage(const CoilHeatingGasMultiStageStageData& stage, unsigned index) {
      bool ok = addStage(stage);
      if (!ok) {
        return false;
      }
      ok = setStageIndex(stage, index);
      return ok;
    }

    bool CoilHeatingGasMultiStage_Impl::setStages(const std::vector<CoilHeatingGasMultiStageStageData>& stages) {
      // Clear the extensible groups, and redo them
      bool ok = true;
      clearExtensibleGroups();
      for (const CoilHeatingGasMultiStageStageData& s : stages) {
        ok &= addStage(s);
      }
      return ok;
    }

    void CoilHeatingGasMultiStage_Impl::removeAllStages() {
      clearExtensibleGroups();
    }

    bool CoilHeatingGasMultiStage_Impl::removeStage(const CoilHeatingGasMultiStageStageData& stage) {
      boost::optional<unsigned> idx = stageIndex(stage);
      if (!idx) {
        LOG(Warn, "For " << briefDescription() << " cannot remove stage " << stage.briefDescription() << " since it is not part of it.");
        return false;
      }

      return removeStage(idx.get());
    }

    bool CoilHeatingGasMultiStage_Impl::removeStage(unsigned index) {
      bool result = false;
      if ((index > 0) && (index <= numberOfStages())) {
        getObject<ModelObject>().eraseExtensibleGroup(index - 1);
        result = true;
      }
      return result;
    }

    ComponentType CoilHeatingGasMultiStage_Impl::componentType() const {
      return ComponentType::Heating;
    }

    std::vector<FuelType> CoilHeatingGasMultiStage_Impl::coolingFuelTypes() const {
      return {};
    }

    std::vector<FuelType> CoilHeatingGasMultiStage_Impl::heatingFuelTypes() const {
      return {FuelType::Gas};
    }

    std::vector<AppGFuelType> CoilHeatingGasMultiStage_Impl::appGHeatingFuelTypes() const {
      return {AppGFuelType::Fuel};
    }

  }  // namespace detail

  CoilHeatingGasMultiStage::CoilHeatingGasMultiStage(const Model& model) : StraightComponent(CoilHeatingGasMultiStage::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CoilHeatingGasMultiStage_Impl>());

    auto always_on = model.alwaysOnDiscreteSchedule();
    bool ok = setAvailabilitySchedule(always_on);
    OS_ASSERT(ok);
  }

  IddObjectType CoilHeatingGasMultiStage::iddObjectType() {
    return {IddObjectType::OS_Coil_Heating_Gas_MultiStage};
  }

  Schedule CoilHeatingGasMultiStage::availabilitySchedule() const {
    return getImpl<detail::CoilHeatingGasMultiStage_Impl>()->availabilitySchedule();
  }

  boost::optional<Curve> CoilHeatingGasMultiStage::partLoadFractionCorrelationCurve() const {
    return getImpl<detail::CoilHeatingGasMultiStage_Impl>()->partLoadFractionCorrelationCurve();
  }

  boost::optional<double> CoilHeatingGasMultiStage::parasiticGasLoad() const {
    return getImpl<detail::CoilHeatingGasMultiStage_Impl>()->parasiticGasLoad();
  }

  bool CoilHeatingGasMultiStage::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::CoilHeatingGasMultiStage_Impl>()->setAvailabilitySchedule(schedule);
  }

  bool CoilHeatingGasMultiStage::setPartLoadFractionCorrelationCurve(const Curve& curve) {
    return getImpl<detail::CoilHeatingGasMultiStage_Impl>()->setPartLoadFractionCorrelationCurve(curve);
  }

  void CoilHeatingGasMultiStage::resetPartLoadFractionCorrelationCurve() {
    getImpl<detail::CoilHeatingGasMultiStage_Impl>()->resetPartLoadFractionCorrelationCurve();
  }

  bool CoilHeatingGasMultiStage::setParasiticGasLoad(double parasiticGasLoad) {
    return getImpl<detail::CoilHeatingGasMultiStage_Impl>()->setParasiticGasLoad(parasiticGasLoad);
  }

  void CoilHeatingGasMultiStage::resetParasiticGasLoad() {
    getImpl<detail::CoilHeatingGasMultiStage_Impl>()->resetParasiticGasLoad();
  }

  unsigned CoilHeatingGasMultiStage::numberOfStages() const {
    return getImpl<detail::CoilHeatingGasMultiStage_Impl>()->numberOfStages();
  }

  boost::optional<unsigned> CoilHeatingGasMultiStage::stageIndex(const CoilHeatingGasMultiStageStageData& stage) const {
    return getImpl<detail::CoilHeatingGasMultiStage_Impl>()->stageIndex(stage);
  }

  std::vector<CoilHeatingGasMultiStageStageData> CoilHeatingGasMultiStage::stages() const {
    return getImpl<detail::CoilHeatingGasMultiStage_Impl>()->stages();
  }

  bool CoilHeatingGasMultiStage::addStage(const CoilHeatingGasMultiStageStageData& stage) {
    return getImpl<detail::CoilHeatingGasMultiStage_Impl>()->addStage(stage);
  }

  bool CoilHeatingGasMultiStage::addStage(const CoilHeatingGasMultiStageStageData& stage, unsigned index) {
    return getImpl<detail::CoilHeatingGasMultiStage_Impl>()->addStage(stage, index);
  }

  bool CoilHeatingGasMultiStage::setStageIndex(const CoilHeatingGasMultiStageStageData& stage, unsigned index) {
    return getImpl<detail::CoilHeatingGasMultiStage_Impl>()->setStageIndex(stage, index);
  }

  bool CoilHeatingGasMultiStage::setStages(const std::vector<CoilHeatingGasMultiStageStageData>& stages) {
    return getImpl<detail::CoilHeatingGasMultiStage_Impl>()->setStages(stages);
  }

  void CoilHeatingGasMultiStage::removeAllStages() {
    getImpl<detail::CoilHeatingGasMultiStage_Impl>()->removeAllStages();
  }

  bool CoilHeatingGasMultiStage::removeStage(const CoilHeatingGasMultiStageStageData& stage) {
    return getImpl<detail::CoilHeatingGasMultiStage_Impl>()->removeStage(stage);
  }

  bool CoilHeatingGasMultiStage::removeStage(unsigned index) {
    return getImpl<detail::CoilHeatingGasMultiStage_Impl>()->removeStage(index);
  }

  /// @cond
  CoilHeatingGasMultiStage::CoilHeatingGasMultiStage(std::shared_ptr<detail::CoilHeatingGasMultiStage_Impl> impl)
    : StraightComponent(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
