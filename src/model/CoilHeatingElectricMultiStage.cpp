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
#include "CoilHeatingElectricMultiStage.hpp"
#include "CoilHeatingElectricMultiStage_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "CoilHeatingElectricMultiStageStageData.hpp"
#include "CoilHeatingElectricMultiStageStageData_Impl.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include <utilities/idd/OS_Coil_Heating_Electric_MultiStage_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"
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
      static const std::vector<std::string> result{"TODO"};
      return result;
    }

    IddObjectType CoilHeatingElectricMultiStage_Impl::iddObjectType() const {
      return CoilHeatingElectricMultiStage::iddObjectType();
    }

    std::vector<ScheduleTypeKey> CoilHeatingElectricMultiStage_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
      if (std::find(b, e, OS_Coil_Heating_Electric_MultiStageFields::AvailabilitySchedule) != e) {
        result.push_back(ScheduleTypeKey("CoilHeatingElectricMultiStage", "Availability Schedule"));
      }
      return result;
    }

    boost::optional<Schedule> CoilHeatingElectricMultiStage_Impl::availabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Heating_Electric_MultiStageFields::AvailabilitySchedule);
    }

    bool CoilHeatingElectricMultiStage_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_Coil_Heating_Electric_MultiStageFields::AvailabilitySchedule, "CoilHeatingElectricMultiStage",
                                "Availability Schedule", schedule);
      return result;
    }

    void CoilHeatingElectricMultiStage_Impl::resetAvailabilitySchedule() {
      bool result = setString(OS_Coil_Heating_Electric_MultiStageFields::AvailabilitySchedule, "");
      OS_ASSERT(result);
    }

    unsigned CoilHeatingElectricMultiStage_Impl::inletPort() const {
      return OS_Coil_Heating_Electric_MultiStageFields::AirInletNode;
    }

    unsigned CoilHeatingElectricMultiStage_Impl::outletPort() const {
      return OS_Coil_Heating_Electric_MultiStageFields::AirOutletNode;
    }

    ModelObject CoilHeatingElectricMultiStage_Impl::clone(Model model) const {
      auto t_clone = StraightComponent_Impl::clone(model).cast<CoilHeatingElectricMultiStage>();

      auto t_stages = stages();
      for (auto stage : t_stages) {
        auto stageClone = stage.clone(model).cast<CoilHeatingElectricMultiStageStageData>();
        t_clone.addStage(stageClone);
      }
      return t_clone;
    }

    std::vector<ModelObject> CoilHeatingElectricMultiStage_Impl::children() const {
      return subsetCastVector<ModelObject>(stages());
    }

    std::vector<CoilHeatingElectricMultiStageStageData> CoilHeatingElectricMultiStage_Impl::stages() const {
      std::vector<CoilHeatingElectricMultiStageStageData> result;
      auto groups = extensibleGroups();
      for (auto group : groups) {
        auto target = group.cast<WorkspaceExtensibleGroup>().getTarget(OS_Coil_Heating_Electric_MultiStageExtensibleFields::Stage);
        if (target) {
          if (auto stage = target->optionalCast<CoilHeatingElectricMultiStageStageData>()) {
            result.push_back(stage.get());
          }
        }
      }
      return result;
    }

    void CoilHeatingElectricMultiStage_Impl::addStage(CoilHeatingElectricMultiStageStageData& stage) {
      auto group = getObject<ModelObject>().pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();
      group.setPointer(OS_Coil_Heating_Electric_MultiStageExtensibleFields::Stage, stage.handle());
    }

    boost::optional<HVACComponent> CoilHeatingElectricMultiStage_Impl::containingHVACComponent() const {
      // AirLoopHVACUnitarySystem
      std::vector<AirLoopHVACUnitarySystem> airLoopUnitarySystems = this->model().getConcreteModelObjects<AirLoopHVACUnitarySystem>();

      for (const auto& airLoopUnitarySystem : airLoopUnitarySystems) {
        if (boost::optional<HVACComponent> heatingCoil = airLoopHVACUnitarySystem.heatingCoil()) {
          if (heatingCoil.handle() == this->handle()) {
            return airLoopUnitarySystem;
          }
        }
        if (boost::optional<HVACComponent> suppHeatingCoil = airLoopHVACUnitarySystem.supplementalHeatingCoil()) {
          if (suppHeatingCoil->handle() == this->handle()) {
            return airLoopHVACUnitarySystem;
          }
        }
      }

      // AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed
      std::vector<AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed> airLoopUnitaryHeatPumpAirToAirMultiSpeeds =
        this->model().getConcreteModelObjects<AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed>();

      for (const auto& airLoopUnitaryHeatPumpAirToAirMultiSpeed : airLoopUnitaryHeatPumpAirToAirMultiSpeeds) {
        if (boost::optional<HVACComponent> heatingCoil = airLoopUnitaryHeatPumpAirToAirMultiSpeed.heatingCoil()) {
          if (heatingCoil.handle() == this->handle()) {
            return airLoopUnitaryHeatPumpAirToAirMultiSpeed;
          }
        }
      }

      return boost::none;
    }

    bool CoilHeatingElectricMultiStage_Impl::addToNode(Node& node) {
      return false;
    }

  }  // namespace detail

  CoilHeatingElectricMultiStage::CoilHeatingElectricMultiStage(const Model& model)
    : StraightComponent(CoilHeatingElectricMultiStage::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CoilHeatingElectricMultiStage_Impl>());
  }

  IddObjectType CoilHeatingElectricMultiStage::iddObjectType() {
    return IddObjectType(IddObjectType::OS_Coil_Heating_Electric_MultiStage);
  }

  boost::optional<Schedule> CoilHeatingElectricMultiStage::availabilitySchedule() const {
    return getImpl<detail::CoilHeatingElectricMultiStage_Impl>()->availabilitySchedule();
  }

  bool CoilHeatingElectricMultiStage::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::CoilHeatingElectricMultiStage_Impl>()->setAvailabilitySchedule(schedule);
  }

  void CoilHeatingElectricMultiStage::resetAvailabilitySchedule() {
    getImpl<detail::CoilHeatingElectricMultiStage_Impl>()->resetAvailabilitySchedule();
  }

  std::vector<CoilHeatingElectricMultiStageStageData> CoilHeatingElectricMultiStage::stages() const {
    return getImpl<detail::CoilHeatingElectricMultiStage_Impl>()->stages();
  }

  void CoilHeatingElectricMultiStage::addStage(CoilHeatingElectricMultiStageStageData& stage) {
    getImpl<detail::CoilHeatingElectricMultiStage_Impl>()->addStage(stage);
  }

  /// @cond
  CoilHeatingElectricMultiStage::CoilHeatingElectricMultiStage(std::shared_ptr<detail::CoilHeatingElectricMultiStage_Impl> impl)
    : StraightComponent(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
