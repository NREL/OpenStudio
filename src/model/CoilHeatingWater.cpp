/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "CoilHeatingWater.hpp"
#include "CoilHeatingWater_Impl.hpp"
#include "ControllerWaterCoil.hpp"
#include "ControllerWaterCoil_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ZoneHVACComponent.hpp"
#include "ZoneHVACComponent_Impl.hpp"
#include "ZoneHVACFourPipeFanCoil.hpp"
#include "ZoneHVACFourPipeFanCoil_Impl.hpp"
#include "ZoneHVACPackagedTerminalAirConditioner.hpp"
#include "ZoneHVACPackagedTerminalAirConditioner_Impl.hpp"
#include "ZoneHVACPackagedTerminalHeatPump.hpp"
#include "ZoneHVACPackagedTerminalHeatPump_Impl.hpp"
#include "ZoneHVACWaterToAirHeatPump.hpp"
#include "ZoneHVACWaterToAirHeatPump_Impl.hpp"
#include "ZoneHVACUnitHeater.hpp"
#include "ZoneHVACUnitHeater_Impl.hpp"
#include "ZoneHVACUnitVentilator.hpp"
#include "ZoneHVACUnitVentilator_Impl.hpp"
#include "AirLoopHVACUnitarySystem.hpp"
#include "AirLoopHVACUnitarySystem_Impl.hpp"
#include "AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass.hpp"
#include "AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "ScheduleCompact.hpp"
#include "ScheduleCompact_Impl.hpp"
#include "AirTerminalSingleDuctConstantVolumeReheat.hpp"
#include "AirTerminalSingleDuctConstantVolumeReheat_Impl.hpp"
#include "AirTerminalSingleDuctVAVReheat.hpp"
#include "AirTerminalSingleDuctVAVReheat_Impl.hpp"
#include "AirTerminalSingleDuctVAVHeatAndCoolReheat.hpp"
#include "AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl.hpp"
#include "AirTerminalSingleDuctParallelPIUReheat.hpp"
#include "AirTerminalSingleDuctParallelPIUReheat_Impl.hpp"
#include "AirTerminalSingleDuctSeriesPIUReheat.hpp"
#include "AirTerminalSingleDuctSeriesPIUReheat_Impl.hpp"
#include "AirTerminalSingleDuctConstantVolumeFourPipeInduction.hpp"
#include "AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl.hpp"
#include "AirflowNetworkEquivalentDuct.hpp"
#include "AirflowNetworkEquivalentDuct_Impl.hpp"
#include "Model.hpp"
#include <utilities/idd/OS_Coil_Heating_Water_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Compare.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    CoilHeatingWater_Impl::CoilHeatingWater_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : WaterToAirComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == CoilHeatingWater::iddObjectType());
    }

    CoilHeatingWater_Impl::CoilHeatingWater_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : WaterToAirComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == CoilHeatingWater::iddObjectType());
    }

    CoilHeatingWater_Impl::CoilHeatingWater_Impl(const CoilHeatingWater_Impl& other, Model_Impl* model, bool keepHandle)
      : WaterToAirComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& CoilHeatingWater_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Heating Coil Heating Energy", "Heating Coil Source Side Heat Transfer Energy",
                                                   "Heating Coil Heating Rate", "Heating Coil U Factor Times Area Value"};
      return result;
    }

    bool CoilHeatingWater_Impl::addToNode(Node& node) {
      bool success = WaterToAirComponent_Impl::addToNode(node);

      if (success && (!containingHVACComponent()) && (!containingZoneHVACComponent())) {
        if (boost::optional<ModelObject> _waterInletModelObject = waterInletModelObject()) {
          if (auto oldController = controllerWaterCoil()) {
            if (!openstudio::istringEqual(oldController->action().get(), "Normal")) {
              LOG(Warn, briefDescription()
                          << " has an existing ControllerWaterCoil with action set to something else than 'Normal'. Make sure this is what you want");
            }
          } else {
            Model t_model = model();
            ControllerWaterCoil controller(t_model);
            controller.getImpl<ControllerWaterCoil_Impl>()->setWaterCoil(getObject<HVACComponent>());
            controller.setAction("Normal");
          }
        }
      }

      return success;
    }

    bool CoilHeatingWater_Impl::removeFromPlantLoop() {
      if (boost::optional<ControllerWaterCoil> controller = this->controllerWaterCoil()) {
        controller->remove();
      }

      return WaterToAirComponent_Impl::removeFromPlantLoop();
    }

    std::vector<openstudio::IdfObject> CoilHeatingWater_Impl::remove() {
      if (isRemovable()) {
        return WaterToAirComponent_Impl::remove();
      }

      return {};
    }

    ModelObject CoilHeatingWater_Impl::clone(Model model) const {
      return WaterToAirComponent_Impl::clone(model);
    }

    IddObjectType CoilHeatingWater_Impl::iddObjectType() const {
      return CoilHeatingWater::iddObjectType();
    }

    std::vector<ModelObject> CoilHeatingWater_Impl::children() const {
      std::vector<ModelObject> result;
      std::vector<AirflowNetworkEquivalentDuct> myAFNItems =
        getObject<ModelObject>().getModelObjectSources<AirflowNetworkEquivalentDuct>(AirflowNetworkEquivalentDuct::iddObjectType());
      result.insert(result.end(), myAFNItems.begin(), myAFNItems.end());
      return result;
    }

    std::vector<ScheduleTypeKey> CoilHeatingWater_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_Coil_Heating_WaterFields::AvailabilityScheduleName) != e) {
        result.emplace_back("CoilHeatingWater", "Availability");
      }
      return result;
    }

    Schedule CoilHeatingWater_Impl::availabilitySchedule() const {
      OptionalSchedule value = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Heating_WaterFields::AvailabilityScheduleName);
      if (!value) {
        // it is an error if we get here, however we don't want to crash
        // so we hook up to global always on schedule
        LOG(Error, "Required availability schedule not set, using 'Always On' schedule");
        value = this->model().alwaysOnDiscreteSchedule();
        OS_ASSERT(value);
        const_cast<CoilHeatingWater_Impl*>(this)->setAvailabilitySchedule(*value);
        value = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Heating_WaterFields::AvailabilityScheduleName);
      }
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilHeatingWater_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_Coil_Heating_WaterFields::AvailabilityScheduleName, "CoilHeatingWater", "Availability", schedule);
      return result;
    }

    boost::optional<double> CoilHeatingWater_Impl::uFactorTimesAreaValue() {
      return getDouble(openstudio::OS_Coil_Heating_WaterFields::UFactorTimesAreaValue);
    }

    bool CoilHeatingWater_Impl::setUFactorTimesAreaValue(double value) {
      return setDouble(openstudio::OS_Coil_Heating_WaterFields::UFactorTimesAreaValue, value);
      ;
    }

    bool CoilHeatingWater_Impl::isUFactorTimesAreaValueAutosized() {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Coil_Heating_WaterFields::UFactorTimesAreaValue, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    void CoilHeatingWater_Impl::autosizeUFactorTimesAreaValue() {
      setString(OS_Coil_Heating_WaterFields::UFactorTimesAreaValue, "Autosize");
    }

    boost::optional<double> CoilHeatingWater_Impl::maximumWaterFlowRate() {
      return getDouble(openstudio::OS_Coil_Heating_WaterFields::MaximumWaterFlowRate);
    }

    bool CoilHeatingWater_Impl::setMaximumWaterFlowRate(double value) {
      return setDouble(openstudio::OS_Coil_Heating_WaterFields::MaximumWaterFlowRate, value);
      ;
    }

    bool CoilHeatingWater_Impl::isMaximumWaterFlowRateAutosized() {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Coil_Heating_WaterFields::MaximumWaterFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    void CoilHeatingWater_Impl::autosizeMaximumWaterFlowRate() {
      setString(OS_Coil_Heating_WaterFields::MaximumWaterFlowRate, "Autosize");
    }

    std::string CoilHeatingWater_Impl::performanceInputMethod() {
      return getString(openstudio::OS_Coil_Heating_WaterFields::PerformanceInputMethod, true).get();
    }

    bool CoilHeatingWater_Impl::setPerformanceInputMethod(const std::string& value) {
      return setString(openstudio::OS_Coil_Heating_WaterFields::PerformanceInputMethod, value);
      ;
    }

    boost::optional<double> CoilHeatingWater_Impl::ratedCapacity() {
      return getDouble(openstudio::OS_Coil_Heating_WaterFields::RatedCapacity);
    }

    bool CoilHeatingWater_Impl::setRatedCapacity(double value) {
      return setDouble(openstudio::OS_Coil_Heating_WaterFields::RatedCapacity, value);
      ;
    }

    bool CoilHeatingWater_Impl::isRatedCapacityAutosized() {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Coil_Heating_WaterFields::RatedCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    void CoilHeatingWater_Impl::autosizeRatedCapacity() {
      setString(OS_Coil_Heating_WaterFields::RatedCapacity, "Autosize");
    }

    double CoilHeatingWater_Impl::ratedInletWaterTemperature() {
      return getDouble(openstudio::OS_Coil_Heating_WaterFields::RatedInletWaterTemperature, true).get();
    }

    bool CoilHeatingWater_Impl::setRatedInletWaterTemperature(double value) {
      return setDouble(openstudio::OS_Coil_Heating_WaterFields::RatedInletWaterTemperature, value);
      ;
    }

    double CoilHeatingWater_Impl::ratedInletAirTemperature() {
      return getDouble(openstudio::OS_Coil_Heating_WaterFields::RatedInletAirTemperature, true).get();
    }

    bool CoilHeatingWater_Impl::setRatedInletAirTemperature(double value) {
      return setDouble(openstudio::OS_Coil_Heating_WaterFields::RatedInletAirTemperature, value);
      ;
    }

    double CoilHeatingWater_Impl::ratedOutletWaterTemperature() {
      return getDouble(openstudio::OS_Coil_Heating_WaterFields::RatedOutletWaterTemperature, true).get();
    }

    bool CoilHeatingWater_Impl::setRatedOutletWaterTemperature(double value) {
      return setDouble(openstudio::OS_Coil_Heating_WaterFields::RatedOutletWaterTemperature, value);
      ;
    }

    double CoilHeatingWater_Impl::ratedOutletAirTemperature() {
      return getDouble(openstudio::OS_Coil_Heating_WaterFields::RatedOutletAirTemperature, true).get();
    }

    bool CoilHeatingWater_Impl::setRatedOutletAirTemperature(double value) {
      return setDouble(openstudio::OS_Coil_Heating_WaterFields::RatedOutletAirTemperature, value);
      ;
    }

    double CoilHeatingWater_Impl::ratedRatioForAirAndWaterConvection() {
      return getDouble(openstudio::OS_Coil_Heating_WaterFields::RatedRatioforAirandWaterConvection, true).get();
    }

    bool CoilHeatingWater_Impl::setRatedRatioForAirAndWaterConvection(double value) {
      return setDouble(openstudio::OS_Coil_Heating_WaterFields::RatedRatioforAirandWaterConvection, value);
      ;
    }

    unsigned CoilHeatingWater_Impl::airInletPort() const {
      return OS_Coil_Heating_WaterFields::AirInletNodeName;
    }

    unsigned CoilHeatingWater_Impl::airOutletPort() const {
      return OS_Coil_Heating_WaterFields::AirOutletNodeName;
    }

    unsigned CoilHeatingWater_Impl::waterInletPort() const {
      return OS_Coil_Heating_WaterFields::WaterInletNodeName;
    }

    unsigned CoilHeatingWater_Impl::waterOutletPort() const {
      return OS_Coil_Heating_WaterFields::WaterOutletNodeName;
    }

    boost::optional<HVACComponent> CoilHeatingWater_Impl::containingHVACComponent() const {
      // Process all types that might contain a CoilHeatingWater object.

      // Here is the list of AirTerminals and AirLoopHVACUnitary that are in OpenStudio as of 2.3.0

      // Can have a heating coil (and are implemented below)
      // * AirTerminalSingleDuctConstantVolumeFourPipeInduction
      // * AirTerminalSingleDuctConstantVolumeReheat
      // * AirTerminalSingleDuctParallelPIUReheat
      // * AirTerminalSingleDuctSeriesPIUReheat
      // * AirTerminalSingleDuctVAVHeatAndCoolReheat
      // * AirTerminalSingleDuctVAVReheat
      // * AirLoopHVACUnitarySystem
      // * AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass
      // * AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed

      // Cannot have a heating coil:
      // * AirTerminalDualDuctVAV
      // * AirTerminalSingleDuctConstantVolumeCooledBeam
      // * AirTerminalSingleDuctInletSideMixer
      // * AirTerminalSingleDuctConstantVolumeNoReheat (previously named AirTerminalSingleDuctUncontrolled)
      // * AirTerminalSingleDuctVAVHeatAndCoolNoReheat
      // * AirTerminalSingleDuctVAVNoReheat

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

      // AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass
      std::vector<AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass> bypassSystems =
        this->model().getConcreteModelObjects<AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass>();

      for (const auto& bypassSystem : bypassSystems) {
        if (boost::optional<HVACComponent> heatingCoil = bypassSystem.heatingCoil()) {
          if (heatingCoil->handle() == this->handle()) {
            return bypassSystem;
          }
        }
      }

      // AirTerminalSingleDuctVAVReheat

      std::vector<AirTerminalSingleDuctVAVReheat> airTerminalSingleDuctVAVReheatObjects =
        this->model().getConcreteModelObjects<AirTerminalSingleDuctVAVReheat>();

      for (const auto& airTerminalSingleDuctVAVReheatObject : airTerminalSingleDuctVAVReheatObjects) {
        if (boost::optional<HVACComponent> coil = airTerminalSingleDuctVAVReheatObject.reheatCoil()) {
          if (coil->handle() == this->handle()) {
            return airTerminalSingleDuctVAVReheatObject;
          }
        }
      }

      // AirTerminalSingleDuctVAVHeatAndCoolReheat
      std::vector<AirTerminalSingleDuctVAVHeatAndCoolReheat> airTerminalSingleDuctVAVHeatAndCoolReheatObjects =
        this->model().getConcreteModelObjects<AirTerminalSingleDuctVAVHeatAndCoolReheat>();

      for (const auto& airTerminalSingleDuctVAVHeatAndCoolReheatObject : airTerminalSingleDuctVAVHeatAndCoolReheatObjects) {
        // Not an optional
        if (airTerminalSingleDuctVAVHeatAndCoolReheatObject.reheatCoil().handle() == this->handle()) {
          return airTerminalSingleDuctVAVHeatAndCoolReheatObject;
        }
      }

      // AirTerminalSingleDuctConstantVolumeReheat

      std::vector<AirTerminalSingleDuctConstantVolumeReheat> airTerminalSingleDuctConstantVolumeReheatObjects =
        this->model().getConcreteModelObjects<AirTerminalSingleDuctConstantVolumeReheat>();

      for (const auto& airTerminalSingleDuctConstantVolumeReheatObject : airTerminalSingleDuctConstantVolumeReheatObjects) {
        if (boost::optional<HVACComponent> coil = airTerminalSingleDuctConstantVolumeReheatObject.reheatCoil()) {
          // Not an optional actually...
          if (coil->handle() == this->handle()) {
            return airTerminalSingleDuctConstantVolumeReheatObject;
          }
        }
      }

      // AirTerminalSingleDuctSeriesPIUReheat

      std::vector<AirTerminalSingleDuctSeriesPIUReheat> airTerminalSingleDuctSeriesPIUReheatObjects =
        this->model().getConcreteModelObjects<AirTerminalSingleDuctSeriesPIUReheat>();

      for (const auto& airTerminalSingleDuctSeriesPIUReheatObject : airTerminalSingleDuctSeriesPIUReheatObjects) {
        // Not an optional
        if (airTerminalSingleDuctSeriesPIUReheatObject.reheatCoil().handle() == this->handle()) {
          return airTerminalSingleDuctSeriesPIUReheatObject;
        }
      }

      // AirTerminalSingleDuctParallelPIUReheat

      std::vector<AirTerminalSingleDuctParallelPIUReheat> airTerminalSingleDuctParallelPIUReheatObjects =
        this->model().getConcreteModelObjects<AirTerminalSingleDuctParallelPIUReheat>();

      for (const auto& airTerminalSingleDuctParallelPIUReheatObject : airTerminalSingleDuctParallelPIUReheatObjects) {
        // Not an optional
        if (airTerminalSingleDuctParallelPIUReheatObject.reheatCoil().handle() == this->handle()) {
          return airTerminalSingleDuctParallelPIUReheatObject;
        }
      }

      // AirTerminalSingleDuctConstantVolumeFourPipeInduction
      std::vector<AirTerminalSingleDuctConstantVolumeFourPipeInduction> fourPipeSystems =
        this->model().getConcreteModelObjects<AirTerminalSingleDuctConstantVolumeFourPipeInduction>();

      for (const auto& fourPipeSystem : fourPipeSystems) {
        // Not an optional
        if (fourPipeSystem.heatingCoil().handle() == this->handle()) {
          return fourPipeSystem;
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
          auto supHeatingCoil = system.supplementalHeatingCoil();
          if (supHeatingCoil.handle() == this->handle()) {
            return system;
          }
        }
      }

      return boost::none;
    }

    boost::optional<ZoneHVACComponent> CoilHeatingWater_Impl::containingZoneHVACComponent() const {
      // ZoneHVACFourPipeFanCoil

      std::vector<ZoneHVACFourPipeFanCoil> zoneHVACFourPipeFanCoils;

      zoneHVACFourPipeFanCoils = this->model().getConcreteModelObjects<ZoneHVACFourPipeFanCoil>();

      for (const auto& zoneHVACFourPipeFanCoil : zoneHVACFourPipeFanCoils) {
        if (boost::optional<HVACComponent> coil = zoneHVACFourPipeFanCoil.heatingCoil()) {
          if (coil->handle() == this->handle()) {
            return zoneHVACFourPipeFanCoil;
          }
        }
      }

      // ZoneHVACPackagedTerminalAirConditioner

      std::vector<ZoneHVACPackagedTerminalAirConditioner> zoneHVACPackagedTerminalAirConditioners;

      zoneHVACPackagedTerminalAirConditioners = this->model().getConcreteModelObjects<ZoneHVACPackagedTerminalAirConditioner>();

      for (const auto& zoneHVACPackagedTerminalAirConditioner : zoneHVACPackagedTerminalAirConditioners) {
        if (boost::optional<HVACComponent> coil = zoneHVACPackagedTerminalAirConditioner.heatingCoil()) {
          if (coil->handle() == this->handle()) {
            return zoneHVACPackagedTerminalAirConditioner;
          }
        }
      }

      // ZoneHVACPackagedTerminalHeatPump

      std::vector<ZoneHVACPackagedTerminalHeatPump> zoneHVACPackagedTerminalHeatPumps;

      zoneHVACPackagedTerminalHeatPumps = this->model().getConcreteModelObjects<ZoneHVACPackagedTerminalHeatPump>();

      for (const auto& zoneHVACPackagedTerminalHeatPump : zoneHVACPackagedTerminalHeatPumps) {
        if (boost::optional<HVACComponent> coil = zoneHVACPackagedTerminalHeatPump.supplementalHeatingCoil()) {
          if (coil->handle() == this->handle()) {
            return zoneHVACPackagedTerminalHeatPump;
          }
        }
      }

      // ZoneHVACWaterToAirHeatPump

      std::vector<ZoneHVACWaterToAirHeatPump> zoneHVACWaterToAirHeatPumps;

      zoneHVACWaterToAirHeatPumps = this->model().getConcreteModelObjects<ZoneHVACWaterToAirHeatPump>();

      for (const auto& zoneHVACWaterToAirHeatPump : zoneHVACWaterToAirHeatPumps) {
        if (boost::optional<HVACComponent> coil = zoneHVACWaterToAirHeatPump.supplementalHeatingCoil()) {
          if (coil->handle() == this->handle()) {
            return zoneHVACWaterToAirHeatPump;
          }
        }
      }

      // ZoneHVACUnitHeater

      std::vector<ZoneHVACUnitHeater> zoneHVACUnitHeater;

      zoneHVACUnitHeater = this->model().getConcreteModelObjects<ZoneHVACUnitHeater>();

      for (const auto& elem : zoneHVACUnitHeater) {
        if (boost::optional<HVACComponent> coil = elem.heatingCoil()) {
          if (coil->handle() == this->handle()) {
            return elem;
          }
        }
      }

      // ZoneHVACUnitVentilator

      std::vector<ZoneHVACUnitVentilator> zoneHVACUnitVentilator;

      zoneHVACUnitVentilator = this->model().getConcreteModelObjects<ZoneHVACUnitVentilator>();

      for (const auto& elem : zoneHVACUnitVentilator) {
        if (boost::optional<HVACComponent> coil = elem.heatingCoil()) {
          if (coil->handle() == this->handle()) {
            return elem;
          }
        }
      }

      return boost::none;
    }

    boost::optional<ModelObject> CoilHeatingWater_Impl::availabilityScheduleAsModelObject() const {
      OptionalModelObject result = availabilitySchedule();
      return result;
    }

    bool CoilHeatingWater_Impl::setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
        if (intermediate) {
          Schedule schedule(*intermediate);
          return setAvailabilitySchedule(schedule);
        }
      }
      return false;
    }

    AirflowNetworkEquivalentDuct CoilHeatingWater_Impl::getAirflowNetworkEquivalentDuct(double length, double diameter) {
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

    boost::optional<AirflowNetworkEquivalentDuct> CoilHeatingWater_Impl::airflowNetworkEquivalentDuct() const {
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
    boost::optional<double> CoilHeatingWater_Impl::autosizedUFactorTimesAreaValue() const {
      return getAutosizedValue("Design Size U-Factor Times Area Value", "W/K");
    }

    boost::optional<double> CoilHeatingWater_Impl::autosizedMaximumWaterFlowRate() const {
      return getAutosizedValue("Design Size Maximum Water Flow Rate", "m3/s");
    }

    boost::optional<double> CoilHeatingWater_Impl::autosizedRatedCapacity() const {
      return getAutosizedValue("Design Size Rated Capacity", "W");
    }

    void CoilHeatingWater_Impl::autosize() {
      autosizeUFactorTimesAreaValue();
      autosizeMaximumWaterFlowRate();
      autosizeRatedCapacity();
    }

    void CoilHeatingWater_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedUFactorTimesAreaValue();
      if (val) {
        setUFactorTimesAreaValue(val.get());
      }

      val = autosizedMaximumWaterFlowRate();
      if (val) {
        setMaximumWaterFlowRate(val.get());
      }

      val = autosizedRatedCapacity();
      if (val) {
        setRatedCapacity(val.get());
      }
    }

    ComponentType CoilHeatingWater_Impl::componentType() const {
      return ComponentType::Heating;
    }

    std::vector<FuelType> CoilHeatingWater_Impl::coolingFuelTypes() const {
      return {};
    }

    std::vector<FuelType> CoilHeatingWater_Impl::heatingFuelTypes() const {
      if (auto p_ = plantLoop()) {
        return p_->heatingFuelTypes();
      }
      return {};
    }

    std::vector<AppGFuelType> CoilHeatingWater_Impl::appGHeatingFuelTypes() const {
      if (auto p_ = plantLoop()) {
        return p_->appGHeatingFuelTypes();
      }
      return {};
    }

  }  // namespace detail

  CoilHeatingWater::CoilHeatingWater(const Model& model, Schedule& availableSchedule)
    : WaterToAirComponent(CoilHeatingWater::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CoilHeatingWater_Impl>());

    setAvailableSchedule(availableSchedule);
  }

  CoilHeatingWater::CoilHeatingWater(const Model& model) : WaterToAirComponent(CoilHeatingWater::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CoilHeatingWater_Impl>());

    auto availableSchedule = model.alwaysOnDiscreteSchedule();
    setAvailableSchedule(availableSchedule);
  }

  CoilHeatingWater::CoilHeatingWater(std::shared_ptr<detail::CoilHeatingWater_Impl> p) : WaterToAirComponent(std::move(p)) {}

  Schedule CoilHeatingWater::availabilitySchedule() const {
    return getImpl<detail::CoilHeatingWater_Impl>()->availabilitySchedule();
  }

  Schedule CoilHeatingWater::availableSchedule() const {
    return getImpl<detail::CoilHeatingWater_Impl>()->availabilitySchedule();
  }

  bool CoilHeatingWater::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::CoilHeatingWater_Impl>()->setAvailabilitySchedule(schedule);
  }

  bool CoilHeatingWater::setAvailableSchedule(Schedule& schedule) {
    return getImpl<detail::CoilHeatingWater_Impl>()->setAvailabilitySchedule(schedule);
  }

  boost::optional<double> CoilHeatingWater::uFactorTimesAreaValue() {
    return getImpl<detail::CoilHeatingWater_Impl>()->uFactorTimesAreaValue();
  }

  bool CoilHeatingWater::setUFactorTimesAreaValue(double value) {
    return getImpl<detail::CoilHeatingWater_Impl>()->setUFactorTimesAreaValue(value);
  }

  bool CoilHeatingWater::isUFactorTimesAreaValueAutosized() {
    return getImpl<detail::CoilHeatingWater_Impl>()->isUFactorTimesAreaValueAutosized();
  }

  void CoilHeatingWater::autosizeUFactorTimesAreaValue() {
    getImpl<detail::CoilHeatingWater_Impl>()->autosizeUFactorTimesAreaValue();
  }

  boost::optional<double> CoilHeatingWater::maximumWaterFlowRate() {
    return getImpl<detail::CoilHeatingWater_Impl>()->maximumWaterFlowRate();
  }

  bool CoilHeatingWater::setMaximumWaterFlowRate(double value) {
    return getImpl<detail::CoilHeatingWater_Impl>()->setMaximumWaterFlowRate(value);
  }

  bool CoilHeatingWater::isMaximumWaterFlowRateAutosized() {
    return getImpl<detail::CoilHeatingWater_Impl>()->isMaximumWaterFlowRateAutosized();
  }

  void CoilHeatingWater::autosizeMaximumWaterFlowRate() {
    getImpl<detail::CoilHeatingWater_Impl>()->autosizeMaximumWaterFlowRate();
  }

  std::string CoilHeatingWater::performanceInputMethod() {
    return getImpl<detail::CoilHeatingWater_Impl>()->performanceInputMethod();
  }

  bool CoilHeatingWater::setPerformanceInputMethod(const std::string& value) {
    return getImpl<detail::CoilHeatingWater_Impl>()->setPerformanceInputMethod(value);
  }

  boost::optional<double> CoilHeatingWater::ratedCapacity() {
    return getImpl<detail::CoilHeatingWater_Impl>()->ratedCapacity();
  }

  bool CoilHeatingWater::setRatedCapacity(double value) {
    return getImpl<detail::CoilHeatingWater_Impl>()->setRatedCapacity(value);
  }

  bool CoilHeatingWater::isRatedCapacityAutosized() {
    return getImpl<detail::CoilHeatingWater_Impl>()->isRatedCapacityAutosized();
  }

  void CoilHeatingWater::autosizeRatedCapacity() {
    getImpl<detail::CoilHeatingWater_Impl>()->autosizeRatedCapacity();
  }

  double CoilHeatingWater::ratedInletWaterTemperature() {
    return getImpl<detail::CoilHeatingWater_Impl>()->ratedInletWaterTemperature();
  }

  bool CoilHeatingWater::setRatedInletWaterTemperature(double value) {
    return getImpl<detail::CoilHeatingWater_Impl>()->setRatedInletWaterTemperature(value);
  }

  double CoilHeatingWater::ratedInletAirTemperature() {
    return getImpl<detail::CoilHeatingWater_Impl>()->ratedInletAirTemperature();
  }

  bool CoilHeatingWater::setRatedInletAirTemperature(double value) {
    return getImpl<detail::CoilHeatingWater_Impl>()->setRatedInletAirTemperature(value);
  }

  double CoilHeatingWater::ratedOutletWaterTemperature() {
    return getImpl<detail::CoilHeatingWater_Impl>()->ratedOutletWaterTemperature();
  }

  bool CoilHeatingWater::setRatedOutletWaterTemperature(double value) {
    return getImpl<detail::CoilHeatingWater_Impl>()->setRatedOutletWaterTemperature(value);
  }

  double CoilHeatingWater::ratedOutletAirTemperature() {
    return getImpl<detail::CoilHeatingWater_Impl>()->ratedOutletAirTemperature();
  }

  bool CoilHeatingWater::setRatedOutletAirTemperature(double value) {
    return getImpl<detail::CoilHeatingWater_Impl>()->setRatedOutletAirTemperature(value);
  }

  double CoilHeatingWater::ratedRatioForAirAndWaterConvection() {
    return getImpl<detail::CoilHeatingWater_Impl>()->ratedRatioForAirAndWaterConvection();
  }

  bool CoilHeatingWater::setRatedRatioForAirAndWaterConvection(double value) {
    return getImpl<detail::CoilHeatingWater_Impl>()->setRatedRatioForAirAndWaterConvection(value);
  }

  IddObjectType CoilHeatingWater::iddObjectType() {
    IddObjectType result(IddObjectType::OS_Coil_Heating_Water);
    return result;
  }

  boost::optional<ControllerWaterCoil> CoilHeatingWater::controllerWaterCoil() {
    return getImpl<detail::CoilHeatingWater_Impl>()->controllerWaterCoil();
  }

  AirflowNetworkEquivalentDuct CoilHeatingWater::getAirflowNetworkEquivalentDuct(double length, double diameter) {
    return getImpl<detail::CoilHeatingWater_Impl>()->getAirflowNetworkEquivalentDuct(length, diameter);
  }

  boost::optional<AirflowNetworkEquivalentDuct> CoilHeatingWater::airflowNetworkEquivalentDuct() const {
    return getImpl<detail::CoilHeatingWater_Impl>()->airflowNetworkEquivalentDuct();
  }

  boost::optional<double> CoilHeatingWater::autosizedUFactorTimesAreaValue() const {
    return getImpl<detail::CoilHeatingWater_Impl>()->autosizedUFactorTimesAreaValue();
  }

  boost::optional<double> CoilHeatingWater::autosizedMaximumWaterFlowRate() const {
    return getImpl<detail::CoilHeatingWater_Impl>()->autosizedMaximumWaterFlowRate();
  }

  boost::optional<double> CoilHeatingWater::autosizedRatedCapacity() const {
    return getImpl<detail::CoilHeatingWater_Impl>()->autosizedRatedCapacity();
  }

}  // namespace model
}  // namespace openstudio
