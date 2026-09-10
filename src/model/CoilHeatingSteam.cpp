/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "CoilHeatingSteam.hpp"
#include "CoilHeatingSteam_Impl.hpp"
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
#include "ZoneHVACTerminalUnitVariableRefrigerantFlow.hpp"
#include "ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl.hpp"
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
#include "Model.hpp"
#include <utilities/idd/OS_Coil_Heating_Steam_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Compare.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    CoilHeatingSteam_Impl::CoilHeatingSteam_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : WaterToAirComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == CoilHeatingSteam::iddObjectType());
    }

    CoilHeatingSteam_Impl::CoilHeatingSteam_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : WaterToAirComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == CoilHeatingSteam::iddObjectType());
    }

    CoilHeatingSteam_Impl::CoilHeatingSteam_Impl(const CoilHeatingSteam_Impl& other, Model_Impl* model, bool keepHandle)
      : WaterToAirComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& CoilHeatingSteam_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Heating Coil Heating Energy",           "Total Steam Coil Heating Rate",
                                                   "Heating Coil Steam Trap Loss Rate",     "Heating Coil Steam Inlet Temperature",
                                                   "Heating Coil Steam Outlet Temperature", "Heating Coil Steam Mass Flow Rate"};
      return result;
    }

    bool CoilHeatingSteam_Impl::addToNode(Node& node) {
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

    bool CoilHeatingSteam_Impl::removeFromPlantLoop() {
      if (boost::optional<ControllerWaterCoil> controller = this->controllerWaterCoil()) {
        controller->remove();
      }

      return WaterToAirComponent_Impl::removeFromPlantLoop();
    }

    std::vector<openstudio::IdfObject> CoilHeatingSteam_Impl::remove() {
      if (isRemovable()) {
        return WaterToAirComponent_Impl::remove();
      }

      return {};
    }

    ModelObject CoilHeatingSteam_Impl::clone(Model model) const {
      return WaterToAirComponent_Impl::clone(model);
    }

    IddObjectType CoilHeatingSteam_Impl::iddObjectType() const {
      return CoilHeatingSteam::iddObjectType();
    }

    std::vector<ModelObject> CoilHeatingSteam_Impl::children() const {
      std::vector<ModelObject> result;
      return result;
    }

    std::vector<ScheduleTypeKey> CoilHeatingSteam_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_Coil_Heating_SteamFields::AvailabilityScheduleName) != e) {
        result.emplace_back("CoilHeatingSteam", "Availability");
      }
      return result;
    }

    Schedule CoilHeatingSteam_Impl::availabilitySchedule() const {
      OptionalSchedule value = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Heating_SteamFields::AvailabilityScheduleName);
      if (!value) {
        // it is an error if we get here, however we don't want to crash
        // so we hook up to global always on schedule
        LOG(Error, "Required availability schedule not set, using 'Always On' schedule");
        value = this->model().alwaysOnDiscreteSchedule();
        OS_ASSERT(value);
        const_cast<CoilHeatingSteam_Impl*>(this)->setAvailabilitySchedule(*value);
        value = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Heating_SteamFields::AvailabilityScheduleName);
      }
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilHeatingSteam_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_Coil_Heating_SteamFields::AvailabilityScheduleName, "CoilHeatingSteam", "Availability", schedule);
      return result;
    }

    boost::optional<double> CoilHeatingSteam_Impl::maximumSteamFlowRate() {
      return getDouble(openstudio::OS_Coil_Heating_SteamFields::MaximumSteamFlowRate);
    }

    bool CoilHeatingSteam_Impl::setMaximumSteamFlowRate(double value) {
      return setDouble(openstudio::OS_Coil_Heating_SteamFields::MaximumSteamFlowRate, value);
    }

    bool CoilHeatingSteam_Impl::isMaximumSteamFlowRateAutosized() {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Coil_Heating_SteamFields::MaximumSteamFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    void CoilHeatingSteam_Impl::autosizeMaximumSteamFlowRate() {
      setString(OS_Coil_Heating_SteamFields::MaximumSteamFlowRate, "Autosize");
    }

    double CoilHeatingSteam_Impl::degreeofSubCooling() const {
      boost::optional<double> value = getDouble(OS_Coil_Heating_SteamFields::DegreeofSubCooling, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilHeatingSteam_Impl::setDegreeofSubCooling(double degreeofSubCooling) {
      bool result = setDouble(OS_Coil_Heating_SteamFields::DegreeofSubCooling, degreeofSubCooling);
      OS_ASSERT(result);
      return result;
    }

    double CoilHeatingSteam_Impl::degreeofLoopSubCooling() const {
      boost::optional<double> value = getDouble(OS_Coil_Heating_SteamFields::DegreeofLoopSubCooling, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilHeatingSteam_Impl::setDegreeofLoopSubCooling(double degreeofLoopSubCooling) {
      bool result = setDouble(OS_Coil_Heating_SteamFields::DegreeofLoopSubCooling, degreeofLoopSubCooling);
      OS_ASSERT(result);
      return result;
    }

    std::string CoilHeatingSteam_Impl::coilControlType() const {
      boost::optional<std::string> value = getString(OS_Coil_Heating_SteamFields::CoilControlType, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilHeatingSteam_Impl::setCoilControlType(const std::string& coilControlType) {
      const bool result = setString(OS_Coil_Heating_SteamFields::CoilControlType, coilControlType);
      return result;
    }

    boost::optional<Node> CoilHeatingSteam_Impl::temperatureSetpointNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Node>(OS_Coil_Heating_SteamFields::TemperatureSetpointNodeName);
    }

    bool CoilHeatingSteam_Impl::setTemperatureSetpointNode(Node& temperatureSetpointNode) {
      return setPointer(OS_Coil_Heating_SteamFields::TemperatureSetpointNodeName, temperatureSetpointNode.handle());
    }

    void CoilHeatingSteam_Impl::resetTemperatureSetpointNode() {
      bool result = setString(OS_Coil_Heating_SteamFields::TemperatureSetpointNodeName, "");
      OS_ASSERT(result);
    }

    unsigned CoilHeatingSteam_Impl::airInletPort() const {
      return OS_Coil_Heating_SteamFields::AirInletNodeName;
    }

    unsigned CoilHeatingSteam_Impl::airOutletPort() const {
      return OS_Coil_Heating_SteamFields::AirOutletNodeName;
    }

    unsigned CoilHeatingSteam_Impl::waterInletPort() const {
      return OS_Coil_Heating_SteamFields::WaterInletNodeName;
    }

    unsigned CoilHeatingSteam_Impl::waterOutletPort() const {
      return OS_Coil_Heating_SteamFields::WaterOutletNodeName;
    }

    boost::optional<HVACComponent> CoilHeatingSteam_Impl::containingHVACComponent() const {
      // Process all types that might contain a CoilHeatingSteam object.

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

    boost::optional<ZoneHVACComponent> CoilHeatingSteam_Impl::containingZoneHVACComponent() const {
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

      // ZoneHVACTerminalUnitVariableRefrigerantFlow

      std::vector<ZoneHVACTerminalUnitVariableRefrigerantFlow> zoneHVACTerminalUnitVariableRefrigerantFlow;

      zoneHVACTerminalUnitVariableRefrigerantFlow = this->model().getConcreteModelObjects<ZoneHVACTerminalUnitVariableRefrigerantFlow>();

      for (const auto& elem : zoneHVACTerminalUnitVariableRefrigerantFlow) {
        if (boost::optional<HVACComponent> coil = elem.supplementalHeatingCoil()) {
          if (coil->handle() == this->handle()) {
            return elem;
          }
        }
      }

      return boost::none;
    }

    boost::optional<ModelObject> CoilHeatingSteam_Impl::availabilityScheduleAsModelObject() const {
      OptionalModelObject result = availabilitySchedule();
      return result;
    }

    bool CoilHeatingSteam_Impl::setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
        if (intermediate) {
          Schedule schedule(*intermediate);
          return setAvailabilitySchedule(schedule);
        }
      }
      return false;
    }

    boost::optional<double> CoilHeatingSteam_Impl::autosizedMaximumSteamFlowRate() const {
      return getAutosizedValue("Design Size Maximum Steam Flow Rate", "m3/s");
    }

    void CoilHeatingSteam_Impl::autosize() {
      autosizeMaximumSteamFlowRate();
    }

    void CoilHeatingSteam_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedMaximumSteamFlowRate();
      if (val) {
        setMaximumSteamFlowRate(val.get());
      }
    }

    ComponentType CoilHeatingSteam_Impl::componentType() const {
      return ComponentType::Heating;
    }

    std::vector<FuelType> CoilHeatingSteam_Impl::coolingFuelTypes() const {
      return {};
    }

    std::vector<FuelType> CoilHeatingSteam_Impl::heatingFuelTypes() const {
      if (auto p_ = plantLoop()) {
        return p_->heatingFuelTypes();
      }
      return {};
    }

    std::vector<AppGFuelType> CoilHeatingSteam_Impl::appGHeatingFuelTypes() const {
      if (auto p_ = plantLoop()) {
        return p_->appGHeatingFuelTypes();
      }
      return {};
    }

  }  // namespace detail

  CoilHeatingSteam::CoilHeatingSteam(const Model& model, Schedule& schedule) : WaterToAirComponent(CoilHeatingSteam::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CoilHeatingSteam_Impl>());

    setAvailabilitySchedule(schedule);
    autosizeMaximumSteamFlowRate();
    setDegreeofSubCooling(5.0);
    setDegreeofLoopSubCooling(20.0);
    setCoilControlType("ZoneLoadControl");
  }

  CoilHeatingSteam::CoilHeatingSteam(const Model& model) : WaterToAirComponent(CoilHeatingSteam::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CoilHeatingSteam_Impl>());

    auto schedule = model.alwaysOnDiscreteSchedule();
    setAvailabilitySchedule(schedule);
    autosizeMaximumSteamFlowRate();
    setDegreeofSubCooling(5.0);
    setDegreeofLoopSubCooling(20.0);
    setCoilControlType("ZoneLoadControl");
  }

  CoilHeatingSteam::CoilHeatingSteam(std::shared_ptr<detail::CoilHeatingSteam_Impl> p) : WaterToAirComponent(std::move(p)) {}

  Schedule CoilHeatingSteam::availabilitySchedule() const {
    return getImpl<detail::CoilHeatingSteam_Impl>()->availabilitySchedule();
  }

  bool CoilHeatingSteam::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::CoilHeatingSteam_Impl>()->setAvailabilitySchedule(schedule);
  }

  boost::optional<double> CoilHeatingSteam::maximumSteamFlowRate() {
    return getImpl<detail::CoilHeatingSteam_Impl>()->maximumSteamFlowRate();
  }

  bool CoilHeatingSteam::setMaximumSteamFlowRate(double value) {
    return getImpl<detail::CoilHeatingSteam_Impl>()->setMaximumSteamFlowRate(value);
  }

  bool CoilHeatingSteam::isMaximumSteamFlowRateAutosized() {
    return getImpl<detail::CoilHeatingSteam_Impl>()->isMaximumSteamFlowRateAutosized();
  }

  void CoilHeatingSteam::autosizeMaximumSteamFlowRate() {
    getImpl<detail::CoilHeatingSteam_Impl>()->autosizeMaximumSteamFlowRate();
  }

  double CoilHeatingSteam::degreeofSubCooling() const {
    return getImpl<detail::CoilHeatingSteam_Impl>()->degreeofSubCooling();
  }

  bool CoilHeatingSteam::setDegreeofSubCooling(double degreeofSubCooling) {
    return getImpl<detail::CoilHeatingSteam_Impl>()->setDegreeofSubCooling(degreeofSubCooling);
  }

  double CoilHeatingSteam::degreeofLoopSubCooling() const {
    return getImpl<detail::CoilHeatingSteam_Impl>()->degreeofLoopSubCooling();
  }

  bool CoilHeatingSteam::setDegreeofLoopSubCooling(double degreeofLoopSubCooling) {
    return getImpl<detail::CoilHeatingSteam_Impl>()->setDegreeofLoopSubCooling(degreeofLoopSubCooling);
  }

  std::string CoilHeatingSteam::coilControlType() const {
    return getImpl<detail::CoilHeatingSteam_Impl>()->coilControlType();
  }

  bool CoilHeatingSteam::setCoilControlType(const std::string& coilControlType) {
    return getImpl<detail::CoilHeatingSteam_Impl>()->setCoilControlType(coilControlType);
  }

  boost::optional<Node> CoilHeatingSteam::temperatureSetpointNode() const {
    return getImpl<detail::CoilHeatingSteam_Impl>()->temperatureSetpointNode();
  }

  bool CoilHeatingSteam::setTemperatureSetpointNode(Node& temperatureSetpointNode) {
    return getImpl<detail::CoilHeatingSteam_Impl>()->setTemperatureSetpointNode(temperatureSetpointNode);
  }

  void CoilHeatingSteam::resetTemperatureSetpointNode() {
    getImpl<detail::CoilHeatingSteam_Impl>()->resetTemperatureSetpointNode();
  }

  IddObjectType CoilHeatingSteam::iddObjectType() {
    IddObjectType result(IddObjectType::OS_Coil_Heating_Steam);
    return result;
  }

  boost::optional<ControllerWaterCoil> CoilHeatingSteam::controllerWaterCoil() {
    return getImpl<detail::CoilHeatingSteam_Impl>()->controllerWaterCoil();
  }

  boost::optional<double> CoilHeatingSteam::autosizedMaximumSteamFlowRate() const {
    return getImpl<detail::CoilHeatingSteam_Impl>()->autosizedMaximumSteamFlowRate();
  }

}  // namespace model
}  // namespace openstudio
