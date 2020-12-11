/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "CoilCoolingDXMultiSpeed.hpp"
#include "CoilCoolingDXMultiSpeed_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "CoilCoolingDXMultiSpeedStageData.hpp"
#include "CoilCoolingDXMultiSpeedStageData_Impl.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include "AirflowNetworkEquivalentDuct.hpp"
#include "AirflowNetworkEquivalentDuct_Impl.hpp"
#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Coil_Cooling_DX_MultiSpeed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    CoilCoolingDXMultiSpeed_Impl::CoilCoolingDXMultiSpeed_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == CoilCoolingDXMultiSpeed::iddObjectType());
    }

    CoilCoolingDXMultiSpeed_Impl::CoilCoolingDXMultiSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                               bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == CoilCoolingDXMultiSpeed::iddObjectType());
    }

    CoilCoolingDXMultiSpeed_Impl::CoilCoolingDXMultiSpeed_Impl(const CoilCoolingDXMultiSpeed_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& CoilCoolingDXMultiSpeed_Impl::outputVariableNames() const {
      // TODO: static for now
      static const std::vector<std::string> result{
        "Cooling Coil Total Cooling Rate", "Cooling Coil Total Cooling Energy", "Cooling Coil Sensible Cooling Rate",
        "Cooling Coil Sensible Cooling Energy", "Cooling Coil Latent Cooling Rate", "Cooling Coil Latent Cooling Energy",
        "Cooling Coil Electricity Rate", "Cooling Coil Electricity Energy", "Cooling Coil Runtime Fraction",

        // condenserType = [AirCooled, EvaporativelyCooled]
        // if (this->condenserType() == "EvaporativelyCooled") {
        "Cooling Coil Condenser Inlet Temperature", "Cooling Coil Evaporative Condenser Water Volume",
        "Cooling Coil Evaporative Condenser Pump Electricity Rate", "Cooling Coil Evaporative Condenser Pump Electricity Energy",
        "Cooling Coil Basin Heater Electricity Rate", "Cooling Coil Basin Heater Electricity Energy",
        "Cooling Coil Evaporative Condenser Mains Supply Water Volume"
        // }

        // Storage tank isn't implemented
        // if has storage tank:
        // "Cooling Coil Condensate Volume Flow Rate",
        // "Cooling Coil Condensate Volume",
        //

        // If not part of AirLoopHVAC:UnitaryHeatPump:AirToAir
        // (if part of a heat pump, crankcase heater is reported only for the heating coil):
        // if ( !this->containingHVACComponent().empty() ) {
        // "Cooling Coil Crankcase Heater Electricity Rate",
        // "Cooling Coil Crankcase Heater Electricity Energy",
        // }
        //
        // Additional variables for Coil:Cooling:DX:Multispeed:
        // If Fuel Type is not Electricity:
        // "DX Cooling Coil <Fuel Type> Power",
        // "DX Cooling Coil <Fuel Type> Consumption"
        //
      };
      return result;
    }

    IddObjectType CoilCoolingDXMultiSpeed_Impl::iddObjectType() const {
      return CoilCoolingDXMultiSpeed::iddObjectType();
    }

    std::vector<ScheduleTypeKey> CoilCoolingDXMultiSpeed_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      // TODO: Check schedule display names.
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
      if (std::find(b, e, OS_Coil_Cooling_DX_MultiSpeedFields::AvailabilitySchedule) != e) {
        result.push_back(ScheduleTypeKey("CoilCoolingDXMultiSpeed", "Availability Schedule"));
      }
      if (std::find(b, e, OS_Coil_Cooling_DX_MultiSpeedFields::BasinHeaterOperatingSchedule) != e) {
        result.push_back(ScheduleTypeKey("CoilCoolingDXMultiSpeed", "Basin Heater Operating Schedule"));
      }
      return result;
    }

    boost::optional<Schedule> CoilCoolingDXMultiSpeed_Impl::availabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Cooling_DX_MultiSpeedFields::AvailabilitySchedule);
    }

    std::string CoilCoolingDXMultiSpeed_Impl::condenserType() const {
      boost::optional<std::string> value = getString(OS_Coil_Cooling_DX_MultiSpeedFields::CondenserType, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilCoolingDXMultiSpeed_Impl::applyPartLoadFractiontoSpeedsGreaterthan1() const {
      boost::optional<std::string> value = getString(OS_Coil_Cooling_DX_MultiSpeedFields::ApplyPartLoadFractiontoSpeedsGreaterthan1, true);
      OS_ASSERT(value);
      return openstudio::istringEqual(value.get(), "Yes");
    }

    bool CoilCoolingDXMultiSpeed_Impl::applyLatentDegradationtoSpeedsGreaterthan1() const {
      boost::optional<std::string> value = getString(OS_Coil_Cooling_DX_MultiSpeedFields::ApplyLatentDegradationtoSpeedsGreaterthan1, true);
      OS_ASSERT(value);
      return openstudio::istringEqual(value.get(), "Yes");
    }

    bool CoilCoolingDXMultiSpeed_Impl::isApplyLatentDegradationtoSpeedsGreaterthan1Defaulted() const {
      return isEmpty(OS_Coil_Cooling_DX_MultiSpeedFields::ApplyLatentDegradationtoSpeedsGreaterthan1);
    }

    double CoilCoolingDXMultiSpeed_Impl::crankcaseHeaterCapacity() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_MultiSpeedFields::CrankcaseHeaterCapacity, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilCoolingDXMultiSpeed_Impl::maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const {
      boost::optional<double> value =
        getDouble(OS_Coil_Cooling_DX_MultiSpeedFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilCoolingDXMultiSpeed_Impl::basinHeaterCapacity() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_MultiSpeedFields::BasinHeaterCapacity, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilCoolingDXMultiSpeed_Impl::basinHeaterSetpointTemperature() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_MultiSpeedFields::BasinHeaterSetpointTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Schedule> CoilCoolingDXMultiSpeed_Impl::basinHeaterOperatingSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Cooling_DX_MultiSpeedFields::BasinHeaterOperatingSchedule);
    }

    std::string CoilCoolingDXMultiSpeed_Impl::fuelType() const {
      boost::optional<std::string> value = getString(OS_Coil_Cooling_DX_MultiSpeedFields::FuelType, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilCoolingDXMultiSpeed_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result =
        setSchedule(OS_Coil_Cooling_DX_MultiSpeedFields::AvailabilitySchedule, "CoilCoolingDXMultiSpeed", "Availability Schedule", schedule);
      return result;
    }

    void CoilCoolingDXMultiSpeed_Impl::resetAvailabilitySchedule() {
      bool result = setString(OS_Coil_Cooling_DX_MultiSpeedFields::AvailabilitySchedule, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXMultiSpeed_Impl::setCondenserType(std::string condenserType) {
      bool result = setString(OS_Coil_Cooling_DX_MultiSpeedFields::CondenserType, condenserType);
      return result;
    }

    bool CoilCoolingDXMultiSpeed_Impl::setApplyPartLoadFractiontoSpeedsGreaterthan1(bool applyPartLoadFractiontoSpeedsGreaterthan1) {
      if (applyPartLoadFractiontoSpeedsGreaterthan1) {
        return setString(OS_Coil_Cooling_DX_MultiSpeedFields::ApplyPartLoadFractiontoSpeedsGreaterthan1, "Yes");
      } else {
        return setString(OS_Coil_Cooling_DX_MultiSpeedFields::ApplyPartLoadFractiontoSpeedsGreaterthan1, "No");
        ;
      }
    }

    bool CoilCoolingDXMultiSpeed_Impl::setApplyLatentDegradationtoSpeedsGreaterthan1(bool applyLatentDegradationtoSpeedsGreaterthan1) {
      if (applyLatentDegradationtoSpeedsGreaterthan1) {
        return setString(OS_Coil_Cooling_DX_MultiSpeedFields::ApplyLatentDegradationtoSpeedsGreaterthan1, "Yes");
      } else {
        return setString(OS_Coil_Cooling_DX_MultiSpeedFields::ApplyLatentDegradationtoSpeedsGreaterthan1, "No");
        ;
      }
    }

    void CoilCoolingDXMultiSpeed_Impl::resetApplyLatentDegradationtoSpeedsGreaterthan1() {
      bool result = setString(OS_Coil_Cooling_DX_MultiSpeedFields::ApplyLatentDegradationtoSpeedsGreaterthan1, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXMultiSpeed_Impl::setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity) {
      bool result = setDouble(OS_Coil_Cooling_DX_MultiSpeedFields::CrankcaseHeaterCapacity, crankcaseHeaterCapacity);
      return result;
    }

    bool CoilCoolingDXMultiSpeed_Impl::setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(
      double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation) {
      bool result = setDouble(OS_Coil_Cooling_DX_MultiSpeedFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation,
                              maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);
      return result;
    }

    bool CoilCoolingDXMultiSpeed_Impl::setBasinHeaterCapacity(double basinHeaterCapacity) {
      bool result = setDouble(OS_Coil_Cooling_DX_MultiSpeedFields::BasinHeaterCapacity, basinHeaterCapacity);
      return result;
    }

    bool CoilCoolingDXMultiSpeed_Impl::setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature) {
      bool result = setDouble(OS_Coil_Cooling_DX_MultiSpeedFields::BasinHeaterSetpointTemperature, basinHeaterSetpointTemperature);
      return result;
    }

    bool CoilCoolingDXMultiSpeed_Impl::setBasinHeaterOperatingSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_Coil_Cooling_DX_MultiSpeedFields::BasinHeaterOperatingSchedule, "CoilCoolingDXMultiSpeed",
                                "Basin Heater Operating Schedule", schedule);
      return result;
    }

    void CoilCoolingDXMultiSpeed_Impl::resetBasinHeaterOperatingSchedule() {
      bool result = setString(OS_Coil_Cooling_DX_MultiSpeedFields::BasinHeaterOperatingSchedule, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXMultiSpeed_Impl::setFuelType(std::string fuelType) {
      bool result = setString(OS_Coil_Cooling_DX_MultiSpeedFields::FuelType, fuelType);
      return result;
    }

    double CoilCoolingDXMultiSpeed_Impl::minimumOutdoorDryBulbTemperatureforCompressorOperation() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_MultiSpeedFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilCoolingDXMultiSpeed_Impl::setMinimumOutdoorDryBulbTemperatureforCompressorOperation(
      double minimumOutdoorDryBulbTemperatureforCompressorOperation) {
      bool result = setDouble(OS_Coil_Cooling_DX_MultiSpeedFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation,
                              minimumOutdoorDryBulbTemperatureforCompressorOperation);
      return result;
    }

    unsigned CoilCoolingDXMultiSpeed_Impl::inletPort() const {
      return OS_Coil_Cooling_DX_MultiSpeedFields::AirInletNode;
    }

    unsigned CoilCoolingDXMultiSpeed_Impl::outletPort() const {
      return OS_Coil_Cooling_DX_MultiSpeedFields::AirOutletNode;
    }

    ModelObject CoilCoolingDXMultiSpeed_Impl::clone(Model model) const {
      auto t_clone = StraightComponent_Impl::clone(model).cast<CoilCoolingDXMultiSpeed>();

      auto t_stages = stages();
      for (auto stage : t_stages) {
        auto stageClone = stage.clone(model).cast<CoilCoolingDXMultiSpeedStageData>();
        t_clone.addStage(stageClone);
      }
      return t_clone;
    }

    std::vector<ModelObject> CoilCoolingDXMultiSpeed_Impl::children() const {
      std::vector<ModelObject> result = subsetCastVector<ModelObject>(stages());
      std::vector<AirflowNetworkEquivalentDuct> myAFNItems =
        getObject<ModelObject>().getModelObjectSources<AirflowNetworkEquivalentDuct>(AirflowNetworkEquivalentDuct::iddObjectType());
      result.insert(result.end(), myAFNItems.begin(), myAFNItems.end());
      return result;
    }

    unsigned CoilCoolingDXMultiSpeed_Impl::numberOfStages() const {
      return numExtensibleGroups();
    }

    std::vector<CoilCoolingDXMultiSpeedStageData> CoilCoolingDXMultiSpeed_Impl::stages() const {
      std::vector<CoilCoolingDXMultiSpeedStageData> result;
      auto groups = extensibleGroups();
      for (auto group : groups) {
        auto target = group.cast<WorkspaceExtensibleGroup>().getTarget(OS_Coil_Cooling_DX_MultiSpeedExtensibleFields::Stage);
        if (target) {
          if (auto stage = target->optionalCast<CoilCoolingDXMultiSpeedStageData>()) {
            result.push_back(stage.get());
          }
        }
      }
      return result;
    }

    boost::optional<unsigned> CoilCoolingDXMultiSpeed_Impl::stageIndex(const CoilCoolingDXMultiSpeedStageData& stage) const {

      boost::optional<unsigned> result;

      auto egs = castVector<WorkspaceExtensibleGroup>(extensibleGroups());
      auto h = openstudio::toString(stage.handle());
      auto it = std::find_if(egs.begin(), egs.end(), [&](const WorkspaceExtensibleGroup& eg) {
        return (eg.getField(OS_Coil_Cooling_DX_MultiSpeedExtensibleFields::Stage).get() == h);
      });

      // If found, we compute the index by using std::distance between the start of vector and the iterator returned by std::find_if
      if (it != egs.end()) {
        result = std::distance(egs.begin(), it) + 1;
      }

      return result;
    }

    bool CoilCoolingDXMultiSpeed_Impl::addStage(const CoilCoolingDXMultiSpeedStageData& stage) {
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
      bool result = group.setPointer(OS_Coil_Cooling_DX_MultiSpeedExtensibleFields::Stage, stage.handle());
      if (!result) {
        // Something went wrong, so erase the new extensible group
        getObject<ModelObject>().eraseExtensibleGroup(group.groupIndex());
      }
      return result;
    }

    bool CoilCoolingDXMultiSpeed_Impl::setStageIndex(const CoilCoolingDXMultiSpeedStageData& stage, unsigned index) {
      boost::optional<unsigned> idx = stageIndex(stage);
      if (!idx) {
        LOG(Warn, "For " << briefDescription() << " cannot set the index of stage " << stage.briefDescription() << " since it is not part of it.");
        return false;
      }

      // TODO: we could just set via string instead of doing a ton of typechecking below...

      std::vector<CoilCoolingDXMultiSpeedStageData> stageVector = stages();

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

    bool CoilCoolingDXMultiSpeed_Impl::addStage(const CoilCoolingDXMultiSpeedStageData& stage, unsigned index) {
      bool ok = addStage(stage);
      if (!ok) {
        return false;
      }
      ok = setStageIndex(stage, index);
      return ok;
    }
    bool CoilCoolingDXMultiSpeed_Impl::setStages(const std::vector<CoilCoolingDXMultiSpeedStageData>& stages) {
      // Clear the extensible groups, and redo them
      bool ok = true;
      clearExtensibleGroups();
      for (const CoilCoolingDXMultiSpeedStageData& s : stages) {
        ok &= addStage(s);
      }
      return ok;
    }

    void CoilCoolingDXMultiSpeed_Impl::removeAllStages() {
      clearExtensibleGroups();
    }

    bool CoilCoolingDXMultiSpeed_Impl::removeStage(const CoilCoolingDXMultiSpeedStageData& stage) {
      boost::optional<unsigned> idx = stageIndex(stage);
      if (!idx) {
        LOG(Warn, "For " << briefDescription() << " cannot remove stage " << stage.briefDescription() << " since it is not part of it.");
        return false;
      }

      return removeStage(idx.get());
    }

    bool CoilCoolingDXMultiSpeed_Impl::removeStage(unsigned index) {
      bool result = false;
      if ((index > 0) && (index <= numberOfStages())) {
        getObject<ModelObject>().eraseExtensibleGroup(index - 1);
        result = true;
      }
      return result;
    }

    boost::optional<HVACComponent> CoilCoolingDXMultiSpeed_Impl::containingHVACComponent() const {
      // AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed
      {
        auto systems = this->model().getConcreteModelObjects<AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed>();

        for (const auto& system : systems) {
          auto coolingCoil = system.coolingCoil();
          if (coolingCoil.handle() == this->handle()) {
            return system;
          }
        }
      }
      return boost::none;
    }

    bool CoilCoolingDXMultiSpeed_Impl::addToNode(Node& node) {
      return false;
    }

    AirflowNetworkEquivalentDuct CoilCoolingDXMultiSpeed_Impl::getAirflowNetworkEquivalentDuct(double length, double diameter) {
      boost::optional<AirflowNetworkEquivalentDuct> opt = airflowNetworkEquivalentDuct();
      if (opt) {
        if (opt->airPathLength() != length) {
          opt->setAirPathLength(length);
        }
        if (opt->airPathHydraulicDiameter() != diameter) {
          opt->setAirPathHydraulicDiameter(diameter);
        }
        return opt.get();
      }
      return AirflowNetworkEquivalentDuct(model(), length, diameter, handle());
    }

    boost::optional<AirflowNetworkEquivalentDuct> CoilCoolingDXMultiSpeed_Impl::airflowNetworkEquivalentDuct() const {
      std::vector<AirflowNetworkEquivalentDuct> myAFN =
        getObject<ModelObject>().getModelObjectSources<AirflowNetworkEquivalentDuct>(AirflowNetworkEquivalentDuct::iddObjectType());
      auto count = myAFN.size();
      if (count == 1) {
        return myAFN[0];
      } else if (count > 1) {
        LOG(Warn, briefDescription() << " has more than one AirflowNetwork EquivalentDuct attached, returning first.");
        return myAFN[0];
      }
      return boost::none;
    }

  }  // namespace detail

  CoilCoolingDXMultiSpeed::CoilCoolingDXMultiSpeed(const Model& model) : StraightComponent(CoilCoolingDXMultiSpeed::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CoilCoolingDXMultiSpeed_Impl>());

    setCondenserType("AirCooled");
    setApplyPartLoadFractiontoSpeedsGreaterthan1(false);
    setCrankcaseHeaterCapacity(0.0);
    setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(10.0);
    setMinimumOutdoorDryBulbTemperatureforCompressorOperation(-25.0);  // Per E+ IDD default
    setBasinHeaterCapacity(0.0);
    setBasinHeaterSetpointTemperature(2.0);
    setFuelType("NaturalGas");
  }

  IddObjectType CoilCoolingDXMultiSpeed::iddObjectType() {
    return IddObjectType(IddObjectType::OS_Coil_Cooling_DX_MultiSpeed);
  }

  std::vector<std::string> CoilCoolingDXMultiSpeed::condenserTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Coil_Cooling_DX_MultiSpeedFields::CondenserType);
  }

  std::vector<std::string> CoilCoolingDXMultiSpeed::fuelTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Coil_Cooling_DX_MultiSpeedFields::FuelType);
  }

  boost::optional<Schedule> CoilCoolingDXMultiSpeed::availabilitySchedule() const {
    return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->availabilitySchedule();
  }

  std::string CoilCoolingDXMultiSpeed::condenserType() const {
    return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->condenserType();
  }

  bool CoilCoolingDXMultiSpeed::applyPartLoadFractiontoSpeedsGreaterthan1() const {
    return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->applyPartLoadFractiontoSpeedsGreaterthan1();
  }

  bool CoilCoolingDXMultiSpeed::applyLatentDegradationtoSpeedsGreaterthan1() const {
    return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->applyLatentDegradationtoSpeedsGreaterthan1();
  }

  bool CoilCoolingDXMultiSpeed::isApplyLatentDegradationtoSpeedsGreaterthan1Defaulted() const {
    return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->isApplyLatentDegradationtoSpeedsGreaterthan1Defaulted();
  }

  double CoilCoolingDXMultiSpeed::crankcaseHeaterCapacity() const {
    return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->crankcaseHeaterCapacity();
  }

  double CoilCoolingDXMultiSpeed::maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const {
    return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation();
  }

  double CoilCoolingDXMultiSpeed::basinHeaterCapacity() const {
    return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->basinHeaterCapacity();
  }

  double CoilCoolingDXMultiSpeed::basinHeaterSetpointTemperature() const {
    return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->basinHeaterSetpointTemperature();
  }

  boost::optional<Schedule> CoilCoolingDXMultiSpeed::basinHeaterOperatingSchedule() const {
    return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->basinHeaterOperatingSchedule();
  }

  std::string CoilCoolingDXMultiSpeed::fuelType() const {
    return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->fuelType();
  }

  bool CoilCoolingDXMultiSpeed::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->setAvailabilitySchedule(schedule);
  }

  void CoilCoolingDXMultiSpeed::resetAvailabilitySchedule() {
    getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->resetAvailabilitySchedule();
  }

  bool CoilCoolingDXMultiSpeed::setCondenserType(std::string condenserType) {
    return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->setCondenserType(condenserType);
  }

  bool CoilCoolingDXMultiSpeed::setApplyPartLoadFractiontoSpeedsGreaterthan1(bool applyPartLoadFractiontoSpeedsGreaterthan1) {
    return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->setApplyPartLoadFractiontoSpeedsGreaterthan1(applyPartLoadFractiontoSpeedsGreaterthan1);
  }

  bool CoilCoolingDXMultiSpeed::setApplyLatentDegradationtoSpeedsGreaterthan1(bool applyLatentDegradationtoSpeedsGreaterthan1) {
    return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->setApplyLatentDegradationtoSpeedsGreaterthan1(applyLatentDegradationtoSpeedsGreaterthan1);
  }

  void CoilCoolingDXMultiSpeed::resetApplyLatentDegradationtoSpeedsGreaterthan1() {
    getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->resetApplyLatentDegradationtoSpeedsGreaterthan1();
  }

  bool CoilCoolingDXMultiSpeed::setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity) {
    return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->setCrankcaseHeaterCapacity(crankcaseHeaterCapacity);
  }

  bool CoilCoolingDXMultiSpeed::setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(
    double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation) {
    return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(
      maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);
  }

  bool CoilCoolingDXMultiSpeed::setBasinHeaterCapacity(double basinHeaterCapacity) {
    return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->setBasinHeaterCapacity(basinHeaterCapacity);
  }

  bool CoilCoolingDXMultiSpeed::setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature) {
    return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->setBasinHeaterSetpointTemperature(basinHeaterSetpointTemperature);
  }

  bool CoilCoolingDXMultiSpeed::setBasinHeaterOperatingSchedule(Schedule& schedule) {
    return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->setBasinHeaterOperatingSchedule(schedule);
  }

  void CoilCoolingDXMultiSpeed::resetBasinHeaterOperatingSchedule() {
    getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->resetBasinHeaterOperatingSchedule();
  }

  bool CoilCoolingDXMultiSpeed::setFuelType(std::string fuelType) {
    return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->setFuelType(fuelType);
  }

  double CoilCoolingDXMultiSpeed::minimumOutdoorDryBulbTemperatureforCompressorOperation() const {
    return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->minimumOutdoorDryBulbTemperatureforCompressorOperation();
  }

  bool CoilCoolingDXMultiSpeed::setMinimumOutdoorDryBulbTemperatureforCompressorOperation(
    double minimumOutdoorDryBulbTemperatureforCompressorOperation) {
    return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->setMinimumOutdoorDryBulbTemperatureforCompressorOperation(
      minimumOutdoorDryBulbTemperatureforCompressorOperation);
  }

  unsigned CoilCoolingDXMultiSpeed::numberOfStages() const {
    return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->numberOfStages();
  }

  boost::optional<unsigned> CoilCoolingDXMultiSpeed::stageIndex(const CoilCoolingDXMultiSpeedStageData& stage) const {
    return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->stageIndex(stage);
  }

  std::vector<CoilCoolingDXMultiSpeedStageData> CoilCoolingDXMultiSpeed::stages() const {
    return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->stages();
  }

  bool CoilCoolingDXMultiSpeed::addStage(const CoilCoolingDXMultiSpeedStageData& stage) {
    return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->addStage(stage);
  }

  bool CoilCoolingDXMultiSpeed::addStage(const CoilCoolingDXMultiSpeedStageData& stage, unsigned index) {
    return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->addStage(stage, index);
  }

  bool CoilCoolingDXMultiSpeed::setStageIndex(const CoilCoolingDXMultiSpeedStageData& stage, unsigned index) {
    return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->setStageIndex(stage, index);
  }

  bool CoilCoolingDXMultiSpeed::setStages(const std::vector<CoilCoolingDXMultiSpeedStageData>& stages) {
    return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->setStages(stages);
  }

  void CoilCoolingDXMultiSpeed::removeAllStages() {
    getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->removeAllStages();
  }

  bool CoilCoolingDXMultiSpeed::removeStage(const CoilCoolingDXMultiSpeedStageData& stage) {
    return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->removeStage(stage);
  }

  bool CoilCoolingDXMultiSpeed::removeStage(unsigned index) {
    return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->removeStage(index);
  }

  AirflowNetworkEquivalentDuct CoilCoolingDXMultiSpeed::getAirflowNetworkEquivalentDuct(double length, double diameter) {
    return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->getAirflowNetworkEquivalentDuct(length, diameter);
  }

  boost::optional<AirflowNetworkEquivalentDuct> CoilCoolingDXMultiSpeed::airflowNetworkEquivalentDuct() const {
    return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->airflowNetworkEquivalentDuct();
  }

  /// @cond
  CoilCoolingDXMultiSpeed::CoilCoolingDXMultiSpeed(std::shared_ptr<detail::CoilCoolingDXMultiSpeed_Impl> impl) : StraightComponent(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
