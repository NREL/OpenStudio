/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "CoilHeatingDXMultiSpeed.hpp"
#include "CoilHeatingDXMultiSpeed_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "CoilHeatingDXMultiSpeedStageData.hpp"
#include "CoilHeatingDXMultiSpeedStageData_Impl.hpp"
#include "AirLoopHVACUnitarySystem.hpp"
#include "AirLoopHVACUnitarySystem_Impl.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include "AirflowNetworkEquivalentDuct.hpp"
#include "AirflowNetworkEquivalentDuct_Impl.hpp"
#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Coil_Heating_DX_MultiSpeed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"
#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    CoilHeatingDXMultiSpeed_Impl::CoilHeatingDXMultiSpeed_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == CoilHeatingDXMultiSpeed::iddObjectType());
    }

    CoilHeatingDXMultiSpeed_Impl::CoilHeatingDXMultiSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                               bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == CoilHeatingDXMultiSpeed::iddObjectType());
    }

    CoilHeatingDXMultiSpeed_Impl::CoilHeatingDXMultiSpeed_Impl(const CoilHeatingDXMultiSpeed_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& CoilHeatingDXMultiSpeed_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{
        "Heating Coil Total Heating Rate",
        "Heating Coil Total Heating Energy",
        "Heating Coil Electricity Rate",
        "Heating Coil Electricity Energy",
        "Heating Coil Defrost Electricity Rate",
        "Heating Coil Defrost Electricity Energy",
        "Heating Coil Defrost Gas Rate",
        "Heating Coil Defrost Gas Energy",
        "Heating Coil Crankcase Heater Electricity Rate",
        "Heating Coil Crankcase Heater Electricity Energy",
        "Heating Coil Runtime Fraction"
        // TODO: Implement this
        // If Fuel Type is not Electricity:
        // "Heating Coil <Fuel Type> Rate",
        // "Heating Coil <Fuel Type> Energy"

      };
      return result;
    }

    IddObjectType CoilHeatingDXMultiSpeed_Impl::iddObjectType() const {
      return CoilHeatingDXMultiSpeed::iddObjectType();
    }

    std::vector<ScheduleTypeKey> CoilHeatingDXMultiSpeed_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      // TODO: Check schedule display names.
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_Coil_Heating_DX_MultiSpeedFields::AvailabilityScheduleName) != e) {
        result.push_back(ScheduleTypeKey("CoilHeatingDXMultiSpeed", "Availability"));
      }
      return result;
    }

    boost::optional<Schedule> CoilHeatingDXMultiSpeed_Impl::availabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Heating_DX_MultiSpeedFields::AvailabilityScheduleName);
    }

    double CoilHeatingDXMultiSpeed_Impl::minimumOutdoorDryBulbTemperatureforCompressorOperation() const {
      boost::optional<double> value = getDouble(OS_Coil_Heating_DX_MultiSpeedFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> CoilHeatingDXMultiSpeed_Impl::outdoorDryBulbTemperaturetoTurnOnCompressor() const {
      return getDouble(OS_Coil_Heating_DX_MultiSpeedFields::OutdoorDryBulbTemperaturetoTurnOnCompressor, true);
    }

    double CoilHeatingDXMultiSpeed_Impl::crankcaseHeaterCapacity() const {
      boost::optional<double> value = getDouble(OS_Coil_Heating_DX_MultiSpeedFields::CrankcaseHeaterCapacity, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilHeatingDXMultiSpeed_Impl::maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const {
      boost::optional<double> value =
        getDouble(OS_Coil_Heating_DX_MultiSpeedFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Curve> CoilHeatingDXMultiSpeed_Impl::defrostEnergyInputRatioFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Heating_DX_MultiSpeedFields::DefrostEnergyInputRatioFunctionofTemperatureCurveName);
    }

    double CoilHeatingDXMultiSpeed_Impl::maximumOutdoorDryBulbTemperatureforDefrostOperation() const {
      boost::optional<double> value = getDouble(OS_Coil_Heating_DX_MultiSpeedFields::MaximumOutdoorDryBulbTemperatureforDefrostOperation, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string CoilHeatingDXMultiSpeed_Impl::defrostStrategy() const {
      boost::optional<std::string> value = getString(OS_Coil_Heating_DX_MultiSpeedFields::DefrostStrategy, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string CoilHeatingDXMultiSpeed_Impl::defrostControl() const {
      boost::optional<std::string> value = getString(OS_Coil_Heating_DX_MultiSpeedFields::DefrostControl, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilHeatingDXMultiSpeed_Impl::defrostTimePeriodFraction() const {
      boost::optional<double> value = getDouble(OS_Coil_Heating_DX_MultiSpeedFields::DefrostTimePeriodFraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> CoilHeatingDXMultiSpeed_Impl::resistiveDefrostHeaterCapacity() const {
      return getDouble(OS_Coil_Heating_DX_MultiSpeedFields::ResistiveDefrostHeaterCapacity, true);
    }

    bool CoilHeatingDXMultiSpeed_Impl::isResistiveDefrostHeaterCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Coil_Heating_DX_MultiSpeedFields::ResistiveDefrostHeaterCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    bool CoilHeatingDXMultiSpeed_Impl::applyPartLoadFractiontoSpeedsGreaterthan1() const {
      boost::optional<std::string> value = getString(OS_Coil_Heating_DX_MultiSpeedFields::ApplyPartLoadFractiontoSpeedsGreaterthan1, true);
      OS_ASSERT(value);
      return openstudio::istringEqual(value.get(), "Yes");
    }

    std::string CoilHeatingDXMultiSpeed_Impl::fuelType() const {
      boost::optional<std::string> value = getString(OS_Coil_Heating_DX_MultiSpeedFields::FuelType, true);
      OS_ASSERT(value);
      return value.get();
    }

    int CoilHeatingDXMultiSpeed_Impl::regionnumberforCalculatingHSPF() const {
      boost::optional<int> value = getInt(OS_Coil_Heating_DX_MultiSpeedFields::RegionnumberforCalculatingHSPF, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilHeatingDXMultiSpeed_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_Coil_Heating_DX_MultiSpeedFields::AvailabilityScheduleName, "CoilHeatingDXMultiSpeed", "Availability", schedule);
      return result;
    }

    void CoilHeatingDXMultiSpeed_Impl::resetAvailabilitySchedule() {
      bool result = setString(OS_Coil_Heating_DX_MultiSpeedFields::AvailabilityScheduleName, "");
      OS_ASSERT(result);
    }

    bool CoilHeatingDXMultiSpeed_Impl::setMinimumOutdoorDryBulbTemperatureforCompressorOperation(
      double minimumOutdoorDryBulbTemperatureforCompressorOperation) {
      bool result = setDouble(OS_Coil_Heating_DX_MultiSpeedFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation,
                              minimumOutdoorDryBulbTemperatureforCompressorOperation);
      OS_ASSERT(result);
      return result;
    }

    bool CoilHeatingDXMultiSpeed_Impl::setOutdoorDryBulbTemperaturetoTurnOnCompressor(
      boost::optional<double> outdoorDryBulbTemperaturetoTurnOnCompressor) {
      bool result(false);
      if (outdoorDryBulbTemperaturetoTurnOnCompressor) {
        result = setDouble(OS_Coil_Heating_DX_MultiSpeedFields::OutdoorDryBulbTemperaturetoTurnOnCompressor,
                           outdoorDryBulbTemperaturetoTurnOnCompressor.get());
      } else {
        resetOutdoorDryBulbTemperaturetoTurnOnCompressor();
        result = true;
      }
      OS_ASSERT(result);
      return result;
    }

    void CoilHeatingDXMultiSpeed_Impl::resetOutdoorDryBulbTemperaturetoTurnOnCompressor() {
      bool result = setString(OS_Coil_Heating_DX_MultiSpeedFields::OutdoorDryBulbTemperaturetoTurnOnCompressor, "");
      OS_ASSERT(result);
    }

    bool CoilHeatingDXMultiSpeed_Impl::setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity) {
      bool result = setDouble(OS_Coil_Heating_DX_MultiSpeedFields::CrankcaseHeaterCapacity, crankcaseHeaterCapacity);
      return result;
    }

    bool CoilHeatingDXMultiSpeed_Impl::setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(
      double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation) {
      bool result = setDouble(OS_Coil_Heating_DX_MultiSpeedFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation,
                              maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);
      return result;
    }

    bool CoilHeatingDXMultiSpeed_Impl::setDefrostEnergyInputRatioFunctionofTemperatureCurve(const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(OS_Coil_Heating_DX_MultiSpeedFields::DefrostEnergyInputRatioFunctionofTemperatureCurveName, curve.get().handle());
      } else {
        resetDefrostEnergyInputRatioFunctionofTemperatureCurve();
        result = true;
      }
      return result;
    }

    void CoilHeatingDXMultiSpeed_Impl::resetDefrostEnergyInputRatioFunctionofTemperatureCurve() {
      bool result = setString(OS_Coil_Heating_DX_MultiSpeedFields::DefrostEnergyInputRatioFunctionofTemperatureCurveName, "");
      OS_ASSERT(result);
    }

    bool CoilHeatingDXMultiSpeed_Impl::setMaximumOutdoorDryBulbTemperatureforDefrostOperation(
      double maximumOutdoorDryBulbTemperatureforDefrostOperation) {
      bool result = setDouble(OS_Coil_Heating_DX_MultiSpeedFields::MaximumOutdoorDryBulbTemperatureforDefrostOperation,
                              maximumOutdoorDryBulbTemperatureforDefrostOperation);
      return result;
    }

    bool CoilHeatingDXMultiSpeed_Impl::setDefrostStrategy(const std::string& defrostStrategy) {
      bool result = setString(OS_Coil_Heating_DX_MultiSpeedFields::DefrostStrategy, defrostStrategy);
      return result;
    }

    bool CoilHeatingDXMultiSpeed_Impl::setDefrostControl(const std::string& defrostControl) {
      bool result = setString(OS_Coil_Heating_DX_MultiSpeedFields::DefrostControl, defrostControl);
      return result;
    }

    bool CoilHeatingDXMultiSpeed_Impl::setDefrostTimePeriodFraction(double defrostTimePeriodFraction) {
      bool result = setDouble(OS_Coil_Heating_DX_MultiSpeedFields::DefrostTimePeriodFraction, defrostTimePeriodFraction);
      return result;
    }

    bool CoilHeatingDXMultiSpeed_Impl::setResistiveDefrostHeaterCapacity(boost::optional<double> resistiveDefrostHeaterCapacity) {
      bool result(false);
      if (resistiveDefrostHeaterCapacity) {
        result = setDouble(OS_Coil_Heating_DX_MultiSpeedFields::ResistiveDefrostHeaterCapacity, resistiveDefrostHeaterCapacity.get());
      }
      return result;
    }

    void CoilHeatingDXMultiSpeed_Impl::autosizeResistiveDefrostHeaterCapacity() {
      bool result = setString(OS_Coil_Heating_DX_MultiSpeedFields::ResistiveDefrostHeaterCapacity, "autosize");
      OS_ASSERT(result);
    }

    bool CoilHeatingDXMultiSpeed_Impl::setApplyPartLoadFractiontoSpeedsGreaterthan1(bool applyPartLoadFractiontoSpeedsGreaterthan1) {
      return setBooleanFieldValue(OS_Coil_Heating_DX_MultiSpeedFields::ApplyPartLoadFractiontoSpeedsGreaterthan1,
                                  applyPartLoadFractiontoSpeedsGreaterthan1);
      ;
    }

    bool CoilHeatingDXMultiSpeed_Impl::setFuelType(const std::string& fuelType) {
      bool result = setString(OS_Coil_Heating_DX_MultiSpeedFields::FuelType, fuelType);
      return result;
    }

    bool CoilHeatingDXMultiSpeed_Impl::setRegionnumberforCalculatingHSPF(int regionnumberforCalculatingHSPF) {
      bool result = setInt(OS_Coil_Heating_DX_MultiSpeedFields::RegionnumberforCalculatingHSPF, regionnumberforCalculatingHSPF);
      return result;
    }

    unsigned CoilHeatingDXMultiSpeed_Impl::inletPort() const {
      return OS_Coil_Heating_DX_MultiSpeedFields::AirInletNodeName;
    }

    unsigned CoilHeatingDXMultiSpeed_Impl::outletPort() const {
      return OS_Coil_Heating_DX_MultiSpeedFields::AirOutletNodeName;
    }

    ModelObject CoilHeatingDXMultiSpeed_Impl::clone(Model model) const {
      auto t_clone = StraightComponent_Impl::clone(model).cast<CoilHeatingDXMultiSpeed>();

      for (const auto& stage : stages()) {
        auto stageClone = stage.clone(model).cast<CoilHeatingDXMultiSpeedStageData>();
        t_clone.addStage(stageClone);
      }
      return std::move(t_clone);
    }

    std::vector<ModelObject> CoilHeatingDXMultiSpeed_Impl::children() const {
      std::vector<ModelObject> children = subsetCastVector<ModelObject>(stages());

      if (auto const curve = defrostEnergyInputRatioFunctionofTemperatureCurve()) {
        children.push_back(curve.get());
      }

      std::vector<AirflowNetworkEquivalentDuct> myAFNItems =
        getObject<ModelObject>().getModelObjectSources<AirflowNetworkEquivalentDuct>(AirflowNetworkEquivalentDuct::iddObjectType());
      children.insert(children.end(), myAFNItems.begin(), myAFNItems.end());

      return children;
    }

    unsigned CoilHeatingDXMultiSpeed_Impl::numberOfStages() const {
      return numExtensibleGroups();
    }

    std::vector<CoilHeatingDXMultiSpeedStageData> CoilHeatingDXMultiSpeed_Impl::stages() const {
      std::vector<CoilHeatingDXMultiSpeedStageData> result;
      for (const auto& group : extensibleGroups()) {
        if (auto target = group.cast<WorkspaceExtensibleGroup>().getTarget(OS_Coil_Heating_DX_MultiSpeedExtensibleFields::Stage)) {
          if (auto stage = target->optionalCast<CoilHeatingDXMultiSpeedStageData>()) {
            result.push_back(stage.get());
          }
        }
      }
      return result;
    }

    boost::optional<unsigned> CoilHeatingDXMultiSpeed_Impl::stageIndex(const CoilHeatingDXMultiSpeedStageData& stage) const {

      boost::optional<unsigned> result;

      auto egs = castVector<WorkspaceExtensibleGroup>(extensibleGroups());
      auto h = openstudio::toString(stage.handle());
      auto it = std::find_if(egs.begin(), egs.end(), [&](const WorkspaceExtensibleGroup& eg) {
        return (eg.getField(OS_Coil_Heating_DX_MultiSpeedExtensibleFields::Stage).get() == h);
      });

      // If found, we compute the index by using std::distance between the start of vector and the iterator returned by std::find_if
      if (it != egs.end()) {
        result = std::distance(egs.begin(), it) + 1;
      }

      return result;
    }

    bool CoilHeatingDXMultiSpeed_Impl::addStage(const CoilHeatingDXMultiSpeedStageData& stage) {
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
      bool result = group.setPointer(OS_Coil_Heating_DX_MultiSpeedExtensibleFields::Stage, stage.handle());
      if (!result) {
        // Something went wrong, so erase the new extensible group
        getObject<ModelObject>().eraseExtensibleGroup(group.groupIndex());
      }
      return result;
    }

    bool CoilHeatingDXMultiSpeed_Impl::setStageIndex(const CoilHeatingDXMultiSpeedStageData& stage, unsigned index) {
      boost::optional<unsigned> idx = stageIndex(stage);
      if (!idx) {
        LOG(Warn, "For " << briefDescription() << " cannot set the index of stage " << stage.briefDescription() << " since it is not part of it.");
        return false;
      }

      // TODO: we could just set via string instead of doing a ton of typechecking below...

      std::vector<CoilHeatingDXMultiSpeedStageData> stageVector = stages();

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

    bool CoilHeatingDXMultiSpeed_Impl::addStage(const CoilHeatingDXMultiSpeedStageData& stage, unsigned index) {
      bool ok = addStage(stage);
      if (!ok) {
        return false;
      }
      ok = setStageIndex(stage, index);
      return ok;
    }

    bool CoilHeatingDXMultiSpeed_Impl::setStages(const std::vector<CoilHeatingDXMultiSpeedStageData>& stages) {
      // Clear the extensible groups, and redo them
      bool ok = true;
      clearExtensibleGroups();
      for (const CoilHeatingDXMultiSpeedStageData& s : stages) {
        ok &= addStage(s);
      }
      return ok;
    }

    void CoilHeatingDXMultiSpeed_Impl::removeAllStages() {
      clearExtensibleGroups();
    }

    bool CoilHeatingDXMultiSpeed_Impl::removeStage(const CoilHeatingDXMultiSpeedStageData& stage) {
      boost::optional<unsigned> idx = stageIndex(stage);
      if (!idx) {
        LOG(Warn, "For " << briefDescription() << " cannot remove stage " << stage.briefDescription() << " since it is not part of it.");
        return false;
      }

      return removeStage(idx.get());
    }

    bool CoilHeatingDXMultiSpeed_Impl::removeStage(unsigned index) {
      bool result = false;
      if ((index > 0) && (index <= numberOfStages())) {
        getObject<ModelObject>().eraseExtensibleGroup(index - 1);
        result = true;
      }
      return result;
    }

    boost::optional<HVACComponent> CoilHeatingDXMultiSpeed_Impl::containingHVACComponent() const {
      // AirLoopHVACUnitarySystem
      {
        auto systems = this->model().getConcreteModelObjects<AirLoopHVACUnitarySystem>();

        for (auto const& system : systems) {
          if (auto heatingCoil = system.heatingCoil()) {
            if (heatingCoil->handle() == this->handle()) {
              return system;
            }
          }
        }
      }

      // AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed
      {
        auto systems = this->model().getConcreteModelObjects<AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed>();

        for (const auto& system : systems) {
          auto heatingCoil = system.heatingCoil();
          if (heatingCoil.handle() == this->handle()) {
            return system;
          }
        }
      }
      return boost::none;
    }

    bool CoilHeatingDXMultiSpeed_Impl::addToNode(Node& /*node*/) {
      return false;
    }

    AirflowNetworkEquivalentDuct CoilHeatingDXMultiSpeed_Impl::getAirflowNetworkEquivalentDuct(double length, double diameter) {
      boost::optional<AirflowNetworkEquivalentDuct> opt = airflowNetworkEquivalentDuct();
      if (opt) {
        if (opt->airPathLength() != length) {
          opt->setAirPathLength(length);
        }
        if (opt->airPathHydraulicDiameter() != diameter) {
          opt->setAirPathHydraulicDiameter(diameter);
        }
      }
      return AirflowNetworkEquivalentDuct(model(), length, diameter, handle());
    }

    boost::optional<AirflowNetworkEquivalentDuct> CoilHeatingDXMultiSpeed_Impl::airflowNetworkEquivalentDuct() const {
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

    boost::optional<double> CoilHeatingDXMultiSpeed_Impl::autosizedResistiveDefrostHeaterCapacity() const {
      return getAutosizedValue("Design Size Resistive Defrost Heater Capacity", "");
    }

    void CoilHeatingDXMultiSpeed_Impl::autosize() {
      autosizeResistiveDefrostHeaterCapacity();
    }

    void CoilHeatingDXMultiSpeed_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedResistiveDefrostHeaterCapacity();
      if (val) {
        setResistiveDefrostHeaterCapacity(val.get());
      }
    }

    ComponentType CoilHeatingDXMultiSpeed_Impl::componentType() const {
      return ComponentType::Heating;
    }

    std::vector<FuelType> CoilHeatingDXMultiSpeed_Impl::coolingFuelTypes() const {
      return {};
    }

    std::vector<FuelType> CoilHeatingDXMultiSpeed_Impl::heatingFuelTypes() const {
      return {FuelType::Electricity};
    }

    std::vector<AppGFuelType> CoilHeatingDXMultiSpeed_Impl::appGHeatingFuelTypes() const {
      return {AppGFuelType::HeatPump};
    }

  }  // namespace detail

  CoilHeatingDXMultiSpeed::CoilHeatingDXMultiSpeed(const Model& model) : StraightComponent(CoilHeatingDXMultiSpeed::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CoilHeatingDXMultiSpeed_Impl>());

    bool ok = true;
    auto always_on = model.alwaysOnDiscreteSchedule();
    ok = setAvailabilitySchedule(always_on);
    OS_ASSERT(ok);
    ok = setMinimumOutdoorDryBulbTemperatureforCompressorOperation(-8.0);
    OS_ASSERT(ok);
    ok = setCrankcaseHeaterCapacity(0.0);
    OS_ASSERT(ok);
    ok = setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(10.0);
    OS_ASSERT(ok);
    ok = setMaximumOutdoorDryBulbTemperatureforDefrostOperation(5.0);
    OS_ASSERT(ok);
    ok = setDefrostStrategy("Resistive");
    OS_ASSERT(ok);
    ok = setDefrostControl("OnDemand");
    OS_ASSERT(ok);
    ok = setDefrostTimePeriodFraction(0.058333);
    OS_ASSERT(ok);
    autosizeResistiveDefrostHeaterCapacity();
    ok = setApplyPartLoadFractiontoSpeedsGreaterthan1(false);
    OS_ASSERT(ok);
    ok = setFuelType("NaturalGas");
    OS_ASSERT(ok);
    ok = setRegionnumberforCalculatingHSPF(4);
    OS_ASSERT(ok);
  }

  IddObjectType CoilHeatingDXMultiSpeed::iddObjectType() {
    return {IddObjectType::OS_Coil_Heating_DX_MultiSpeed};
  }

  std::vector<std::string> CoilHeatingDXMultiSpeed::defrostStrategyValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Coil_Heating_DX_MultiSpeedFields::DefrostStrategy);
  }

  std::vector<std::string> CoilHeatingDXMultiSpeed::defrostControlValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Coil_Heating_DX_MultiSpeedFields::DefrostControl);
  }

  std::vector<std::string> CoilHeatingDXMultiSpeed::fuelTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Coil_Heating_DX_MultiSpeedFields::FuelType);
  }

  boost::optional<Schedule> CoilHeatingDXMultiSpeed::availabilitySchedule() const {
    return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->availabilitySchedule();
  }

  double CoilHeatingDXMultiSpeed::minimumOutdoorDryBulbTemperatureforCompressorOperation() const {
    return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->minimumOutdoorDryBulbTemperatureforCompressorOperation();
  }

  boost::optional<double> CoilHeatingDXMultiSpeed::outdoorDryBulbTemperaturetoTurnOnCompressor() const {
    return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->outdoorDryBulbTemperaturetoTurnOnCompressor();
  }

  double CoilHeatingDXMultiSpeed::crankcaseHeaterCapacity() const {
    return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->crankcaseHeaterCapacity();
  }

  double CoilHeatingDXMultiSpeed::maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const {
    return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation();
  }

  boost::optional<Curve> CoilHeatingDXMultiSpeed::defrostEnergyInputRatioFunctionofTemperatureCurve() const {
    return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->defrostEnergyInputRatioFunctionofTemperatureCurve();
  }

  double CoilHeatingDXMultiSpeed::maximumOutdoorDryBulbTemperatureforDefrostOperation() const {
    return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->maximumOutdoorDryBulbTemperatureforDefrostOperation();
  }

  std::string CoilHeatingDXMultiSpeed::defrostStrategy() const {
    return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->defrostStrategy();
  }

  std::string CoilHeatingDXMultiSpeed::defrostControl() const {
    return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->defrostControl();
  }

  double CoilHeatingDXMultiSpeed::defrostTimePeriodFraction() const {
    return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->defrostTimePeriodFraction();
  }

  boost::optional<double> CoilHeatingDXMultiSpeed::resistiveDefrostHeaterCapacity() const {
    return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->resistiveDefrostHeaterCapacity();
  }

  bool CoilHeatingDXMultiSpeed::isResistiveDefrostHeaterCapacityAutosized() const {
    return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->isResistiveDefrostHeaterCapacityAutosized();
  }

  bool CoilHeatingDXMultiSpeed::applyPartLoadFractiontoSpeedsGreaterthan1() const {
    return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->applyPartLoadFractiontoSpeedsGreaterthan1();
  }

  std::string CoilHeatingDXMultiSpeed::fuelType() const {
    return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->fuelType();
  }

  int CoilHeatingDXMultiSpeed::regionnumberforCalculatingHSPF() const {
    return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->regionnumberforCalculatingHSPF();
  }

  bool CoilHeatingDXMultiSpeed::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->setAvailabilitySchedule(schedule);
  }

  void CoilHeatingDXMultiSpeed::resetAvailabilitySchedule() {
    getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->resetAvailabilitySchedule();
  }

  bool CoilHeatingDXMultiSpeed::setMinimumOutdoorDryBulbTemperatureforCompressorOperation(
    double minimumOutdoorDryBulbTemperatureforCompressorOperation) {
    return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->setMinimumOutdoorDryBulbTemperatureforCompressorOperation(
      minimumOutdoorDryBulbTemperatureforCompressorOperation);
  }

  bool CoilHeatingDXMultiSpeed::setOutdoorDryBulbTemperaturetoTurnOnCompressor(double outdoorDryBulbTemperaturetoTurnOnCompressor) {
    return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->setOutdoorDryBulbTemperaturetoTurnOnCompressor(
      outdoorDryBulbTemperaturetoTurnOnCompressor);
  }

  void CoilHeatingDXMultiSpeed::resetOutdoorDryBulbTemperaturetoTurnOnCompressor() {
    getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->resetOutdoorDryBulbTemperaturetoTurnOnCompressor();
  }

  bool CoilHeatingDXMultiSpeed::setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity) {
    return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->setCrankcaseHeaterCapacity(crankcaseHeaterCapacity);
  }

  bool CoilHeatingDXMultiSpeed::setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(
    double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation) {
    return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(
      maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);
  }

  bool CoilHeatingDXMultiSpeed::setDefrostEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve) {
    return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->setDefrostEnergyInputRatioFunctionofTemperatureCurve(curve);
  }

  void CoilHeatingDXMultiSpeed::resetDefrostEnergyInputRatioFunctionofTemperatureCurve() {
    getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->resetDefrostEnergyInputRatioFunctionofTemperatureCurve();
  }

  bool CoilHeatingDXMultiSpeed::setMaximumOutdoorDryBulbTemperatureforDefrostOperation(double maximumOutdoorDryBulbTemperatureforDefrostOperation) {
    return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->setMaximumOutdoorDryBulbTemperatureforDefrostOperation(
      maximumOutdoorDryBulbTemperatureforDefrostOperation);
  }

  bool CoilHeatingDXMultiSpeed::setDefrostStrategy(const std::string& defrostStrategy) {
    return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->setDefrostStrategy(defrostStrategy);
  }

  bool CoilHeatingDXMultiSpeed::setDefrostControl(const std::string& defrostControl) {
    return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->setDefrostControl(defrostControl);
  }

  bool CoilHeatingDXMultiSpeed::setDefrostTimePeriodFraction(double defrostTimePeriodFraction) {
    return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->setDefrostTimePeriodFraction(defrostTimePeriodFraction);
  }

  bool CoilHeatingDXMultiSpeed::setResistiveDefrostHeaterCapacity(double resistiveDefrostHeaterCapacity) {
    return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->setResistiveDefrostHeaterCapacity(resistiveDefrostHeaterCapacity);
  }

  void CoilHeatingDXMultiSpeed::autosizeResistiveDefrostHeaterCapacity() {
    getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->autosizeResistiveDefrostHeaterCapacity();
  }

  bool CoilHeatingDXMultiSpeed::setApplyPartLoadFractiontoSpeedsGreaterthan1(bool applyPartLoadFractiontoSpeedsGreaterthan1) {
    return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->setApplyPartLoadFractiontoSpeedsGreaterthan1(applyPartLoadFractiontoSpeedsGreaterthan1);
  }

  bool CoilHeatingDXMultiSpeed::setFuelType(const std::string& fuelType) {
    return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->setFuelType(fuelType);
  }

  bool CoilHeatingDXMultiSpeed::setRegionnumberforCalculatingHSPF(int regionnumberforCalculatingHSPF) {
    return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->setRegionnumberforCalculatingHSPF(regionnumberforCalculatingHSPF);
  }

  unsigned CoilHeatingDXMultiSpeed::numberOfStages() const {
    return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->numberOfStages();
  }

  boost::optional<unsigned> CoilHeatingDXMultiSpeed::stageIndex(const CoilHeatingDXMultiSpeedStageData& stage) const {
    return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->stageIndex(stage);
  }

  std::vector<CoilHeatingDXMultiSpeedStageData> CoilHeatingDXMultiSpeed::stages() const {
    return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->stages();
  }

  bool CoilHeatingDXMultiSpeed::addStage(const CoilHeatingDXMultiSpeedStageData& stage) {
    return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->addStage(stage);
  }

  bool CoilHeatingDXMultiSpeed::addStage(const CoilHeatingDXMultiSpeedStageData& stage, unsigned index) {
    return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->addStage(stage, index);
  }

  bool CoilHeatingDXMultiSpeed::setStageIndex(const CoilHeatingDXMultiSpeedStageData& stage, unsigned index) {
    return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->setStageIndex(stage, index);
  }

  bool CoilHeatingDXMultiSpeed::setStages(const std::vector<CoilHeatingDXMultiSpeedStageData>& stages) {
    return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->setStages(stages);
  }

  void CoilHeatingDXMultiSpeed::removeAllStages() {
    getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->removeAllStages();
  }

  bool CoilHeatingDXMultiSpeed::removeStage(const CoilHeatingDXMultiSpeedStageData& stage) {
    return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->removeStage(stage);
  }

  bool CoilHeatingDXMultiSpeed::removeStage(unsigned index) {
    return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->removeStage(index);
  }

  AirflowNetworkEquivalentDuct CoilHeatingDXMultiSpeed::getAirflowNetworkEquivalentDuct(double length, double diameter) {
    return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->getAirflowNetworkEquivalentDuct(length, diameter);
  }

  boost::optional<AirflowNetworkEquivalentDuct> CoilHeatingDXMultiSpeed::airflowNetworkEquivalentDuct() const {
    return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->airflowNetworkEquivalentDuct();
  }

  /// @cond
  CoilHeatingDXMultiSpeed::CoilHeatingDXMultiSpeed(std::shared_ptr<detail::CoilHeatingDXMultiSpeed_Impl> impl) : StraightComponent(std::move(impl)) {}
  /// @endcond

  boost::optional<double> CoilHeatingDXMultiSpeed::autosizedResistiveDefrostHeaterCapacity() const {
    return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->autosizedResistiveDefrostHeaterCapacity();
  }

}  // namespace model
}  // namespace openstudio
