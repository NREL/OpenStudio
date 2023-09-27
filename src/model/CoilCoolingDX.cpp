/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "CoilCoolingDX.hpp"
#include "CoilCoolingDX_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include "CoilCoolingDXCurveFitPerformance.hpp"
#include "CoilCoolingDXCurveFitPerformance_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "AirLoopHVACUnitarySystem.hpp"
#include "AirLoopHVACUnitarySystem_Impl.hpp"
#include "AirLoopHVACOutdoorAirSystem.hpp"
#include "AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "AirLoopHVACDedicatedOutdoorAirSystem.hpp"
#include "AirLoopHVACDedicatedOutdoorAirSystem_Impl.hpp"
#include "AirflowNetworkEquivalentDuct.hpp"
#include "AirflowNetworkEquivalentDuct_Impl.hpp"
#include "CoilSystemCoolingDXHeatExchangerAssisted.hpp"
#include "CoilSystemCoolingDXHeatExchangerAssisted_Impl.hpp"
#include "ZoneHVACPackagedTerminalAirConditioner.hpp"
#include "ZoneHVACPackagedTerminalAirConditioner_Impl.hpp"
#include "ZoneHVACPackagedTerminalHeatPump.hpp"
#include "ZoneHVACPackagedTerminalHeatPump_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Coil_Cooling_DX_FieldEnums.hxx>
#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    CoilCoolingDX_Impl::CoilCoolingDX_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == CoilCoolingDX::iddObjectType());
    }

    CoilCoolingDX_Impl::CoilCoolingDX_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == CoilCoolingDX::iddObjectType());
    }

    CoilCoolingDX_Impl::CoilCoolingDX_Impl(const CoilCoolingDX_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& CoilCoolingDX_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Cooling Coil Total Cooling Rate",
                                                   "Cooling Coil Total Cooling Energy",
                                                   "Cooling Coil Sensible Cooling Rate",
                                                   "Cooling Coil Sensible Cooling Energy",
                                                   "Cooling Coil Latent Cooling Rate",
                                                   "Cooling Coil Latent Cooling Energy",
                                                   "Cooling Coil Electricity Rate",
                                                   "Cooling Coil Electricity Energy",
                                                   "Cooling Coil Runtime Fraction",
                                                   "SubcoolReheat Cooling Coil Operation Mode",
                                                   "SubcoolReheat Cooling Coil Operation Mode Ratio",
                                                   "SubcoolReheat Cooling Coil Recovered Heat Energy Rate",
                                                   "SubcoolReheat Cooling Coil Recovered Heat Energy"};
      return result;
    }

    IddObjectType CoilCoolingDX_Impl::iddObjectType() const {
      return CoilCoolingDX::iddObjectType();
    }

    std::vector<ScheduleTypeKey> CoilCoolingDX_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_Coil_Cooling_DXFields::AvailabilitySchedule) != e) {
        result.push_back(ScheduleTypeKey("CoilCoolingDX", "Availability Schedule"));
      }
      return result;
    }

    unsigned CoilCoolingDX_Impl::inletPort() const {
      return OS_Coil_Cooling_DXFields::EvaporatorInletNode;
    }

    unsigned CoilCoolingDX_Impl::outletPort() const {
      return OS_Coil_Cooling_DXFields::EvaporatorOutletNode;
    }

    ModelObject CoilCoolingDX_Impl::clone(Model model) const {
      return StraightComponent_Impl::clone(model);

      // The CoilCoolingDXCurveFitPerformance is a ResourceObject so it'll be handled by the ModelObject::clone
      // (StraightComponent_Impl::clone calls HVACComponent_Impl::clone, which skips ParentObject_Impl::clone and goes directly to ModelObject::clone)
      // auto performanceClone = performanceObject().clone(model).cast<CoilCoolingDXCurveFitPerformance>();
      // t_clone.setPerformanceObject(performanceClone);
    }

    std::vector<ModelObject> CoilCoolingDX_Impl::children() const {
      std::vector<ModelObject> result;

      // This is a ResourceObject, so it shouldn't really be a child except for OS App / IG
      // result.push_back(performanceObject());

      std::vector<AirflowNetworkEquivalentDuct> myAFNItems =
        getObject<ModelObject>().getModelObjectSources<AirflowNetworkEquivalentDuct>(AirflowNetworkEquivalentDuct::iddObjectType());
      result.insert(result.end(), myAFNItems.begin(), myAFNItems.end());

      return result;
    }

    boost::optional<HVACComponent> CoilCoolingDX_Impl::containingHVACComponent() const {
      // AirLoopHVACUnitarySystem
      {
        auto systems = this->model().getConcreteModelObjects<AirLoopHVACUnitarySystem>();

        for (auto const& system : systems) {
          if (auto coolingCoil = system.coolingCoil()) {
            if (coolingCoil->handle() == this->handle()) {
              return system;
            }
          }
        }
      }

      // CoilSystemCoolingDXHeatExchangerAssisted
      {
        auto coilSystems = this->model().getConcreteModelObjects<CoilSystemCoolingDXHeatExchangerAssisted>();
        for (const auto& coilSystem : coilSystems) {
          if (coilSystem.coolingCoil().handle() == this->handle()) {
            return coilSystem;
          }
        }
      }

      return boost::none;
    }

    boost::optional<ZoneHVACComponent> CoilCoolingDX_Impl::containingZoneHVACComponent() const {
      // ZoneHVACPackagedTerminalAirConditioner

      std::vector<ZoneHVACPackagedTerminalAirConditioner> zoneHVACPackagedTerminalAirConditioners =
        this->model().getConcreteModelObjects<ZoneHVACPackagedTerminalAirConditioner>();

      for (const auto& zoneHVACPackagedTerminalAirConditioner : zoneHVACPackagedTerminalAirConditioners) {
        if (boost::optional<HVACComponent> coil = zoneHVACPackagedTerminalAirConditioner.coolingCoil()) {
          if (coil->handle() == this->handle()) {
            return zoneHVACPackagedTerminalAirConditioner;
          }
        }
      }

      // ZoneHVACPackagedTerminalHeatPump

      std::vector<ZoneHVACPackagedTerminalHeatPump> zoneHVACPackagedTerminalHeatPumps =
        this->model().getConcreteModelObjects<ZoneHVACPackagedTerminalHeatPump>();

      for (const auto& zoneHVACPackagedTerminalHeatPump : zoneHVACPackagedTerminalHeatPumps) {
        if (boost::optional<HVACComponent> coil = zoneHVACPackagedTerminalHeatPump.coolingCoil()) {
          if (coil->handle() == this->handle()) {
            return zoneHVACPackagedTerminalHeatPump;
          }
        }
      }

      return boost::none;
    }

    bool CoilCoolingDX_Impl::addToNode(Node& node) {
      if (boost::optional<AirLoopHVAC> airLoop = node.airLoopHVAC()) {
        if (!airLoop->demandComponent(node.handle())) {
          // TODO: JM 2019-03-12 I'm not sure we shouldn't just restrict to ANY containingHVACComponent (disallow if part of a UnitarySystem)
          auto t_containingHVACComponent = containingHVACComponent();
          if (t_containingHVACComponent && t_containingHVACComponent->optionalCast<CoilSystemCoolingDXHeatExchangerAssisted>()) {
            LOG(Warn,
                this->briefDescription() << " cannot be connected directly when it's part of a parent CoilSystemCoolingDXHeatExchangerAssisted. "
                                            "Please call CoilSystemCoolingDXHeatExchangerAssisted::addToNode instead");
          } else {
            return StraightComponent_Impl::addToNode(node);
          }
        }
      } else if (boost::optional<AirLoopHVACOutdoorAirSystem> oas = node.airLoopHVACOutdoorAirSystem()) {
        if (oas->airLoopHVACDedicatedOutdoorAirSystem()) {
          return StraightComponent_Impl::addToNode(node);
        }
      }

      return false;
    }

    boost::optional<Schedule> CoilCoolingDX_Impl::optionalAvailabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Cooling_DXFields::AvailabilitySchedule);
    }

    Schedule CoilCoolingDX_Impl::availabilitySchedule() const {
      boost::optional<Schedule> value = optionalAvailabilitySchedule();
      if (!value) {
        // it is an error if we get here, however we don't want to crash
        // so we hook up to global always on schedule
        LOG(Error, "Required availability schedule not set, using 'Always On' schedule");
        value = this->model().alwaysOnDiscreteSchedule();
        OS_ASSERT(value);
        const_cast<CoilCoolingDX_Impl*>(this)->setAvailabilitySchedule(*value);
        value = optionalAvailabilitySchedule();
      }
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<ThermalZone> CoilCoolingDX_Impl::condenserZone() const {
      return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_Coil_Cooling_DXFields::CondenserZone);
    }

    boost::optional<CoilCoolingDXCurveFitPerformance> CoilCoolingDX_Impl::optionalPerformanceObject() const {
      return getObject<ModelObject>().getModelObjectTarget<CoilCoolingDXCurveFitPerformance>(OS_Coil_Cooling_DXFields::PerformanceObject);
    }

    CoilCoolingDXCurveFitPerformance CoilCoolingDX_Impl::performanceObject() const {
      boost::optional<CoilCoolingDXCurveFitPerformance> value = optionalPerformanceObject();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Performance Object attached.");
      }
      return value.get();
    }

    // boost::optional<std::string> CoilCoolingDX_Impl::condensateCollectionWaterStorageTankName() const {
    //   return getString(OS_Coil_Cooling_DXFields::CondensateCollectionWaterStorageTankName,true);
    // }

    // boost::optional<std::string> CoilCoolingDX_Impl::evaporativeCondenserSupplyWaterStorageTankName() const {
    //   return getString(OS_Coil_Cooling_DXFields::EvaporativeCondenserSupplyWaterStorageTankName,true);
    // }

    bool CoilCoolingDX_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_Coil_Cooling_DXFields::AvailabilitySchedule, "CoilCoolingDX", "Availability Schedule", schedule);
      return result;
    }

    bool CoilCoolingDX_Impl::setCondenserZone(const ThermalZone& thermalZone) {
      bool result = setPointer(OS_Coil_Cooling_DXFields::CondenserZone, thermalZone.handle());
      return result;
    }

    void CoilCoolingDX_Impl::resetCondenserZone() {
      bool result = setString(OS_Coil_Cooling_DXFields::CondenserZone, "");
      OS_ASSERT(result);
    }

    //boost::optional<std::string> CoilCoolingDX_Impl::condenserInletNodeName() const {
    //return getString(OS_Coil_Cooling_DXFields::CondenserInletNodeName, false, true);
    //}

    //bool CoilCoolingDX_Impl::setCondenserInletNodeName(const std::string& condenserInletNodeName) {
    //bool result = setString(OS_Coil_Cooling_DXFields::CondenserInletNodeName, condenserInletNodeName);
    //return result;
    //}

    //void CoilCoolingDX_Impl::resetCondenserInletNodeName() {
    //bool result = setString(OS_Coil_Cooling_DXFields::CondenserInletNodeName, "");
    //OS_ASSERT(result);
    //}

    //boost::optional<std::string> CoilCoolingDX_Impl::condenserOutletNodeName() const {
    //return getString(OS_Coil_Cooling_DXFields::CondenserOutletNodeName, false, true);
    //}

    //bool CoilCoolingDX_Impl::setCondenserOutletNodeName(const std::string& condenserOutletNodeName) {
    //bool result = setString(OS_Coil_Cooling_DXFields::CondenserOutletNodeName, condenserOutletNodeName);
    //return result;
    //}

    //void CoilCoolingDX_Impl::resetCondenserOutletNodeName() {
    //bool result = setString(OS_Coil_Cooling_DXFields::CondenserOutletNodeName, "");
    //OS_ASSERT(result);
    //}

    bool CoilCoolingDX_Impl::setPerformanceObject(const CoilCoolingDXCurveFitPerformance& coilCoolingDXCurveFitPerformance) {
      bool result = setPointer(OS_Coil_Cooling_DXFields::PerformanceObject, coilCoolingDXCurveFitPerformance.handle());
      return result;
    }

    // bool CoilCoolingDX_Impl::setCondensateCollectionWaterStorageTankName(const std::string& condensateCollectionWaterStorageTankName) {
    //   bool result = setString(OS_Coil_Cooling_DXFields::CondensateCollectionWaterStorageTankName, condensateCollectionWaterStorageTankName);
    //   return result;
    // }

    // void CoilCoolingDX_Impl::resetCondensateCollectionWaterStorageTankName() {
    //   bool result = setString(OS_Coil_Cooling_DXFields::CondensateCollectionWaterStorageTankName, "");
    //   OS_ASSERT(result);
    // }

    // bool CoilCoolingDX_Impl::setEvaporativeCondenserSupplyWaterStorageTankName(const std::string& evaporativeCondenserSupplyWaterStorageTankName) {
    //   bool result = setString(OS_Coil_Cooling_DXFields::EvaporativeCondenserSupplyWaterStorageTankName, evaporativeCondenserSupplyWaterStorageTankName);
    //   return result;
    // }

    // void CoilCoolingDX_Impl::resetEvaporativeCondenserSupplyWaterStorageTankName() {
    //   bool result = setString(OS_Coil_Cooling_DXFields::EvaporativeCondenserSupplyWaterStorageTankName, "");
    //   OS_ASSERT(result);
    // }

    ComponentType CoilCoolingDX_Impl::componentType() const {
      return ComponentType::Cooling;
    }

    std::vector<FuelType> CoilCoolingDX_Impl::coolingFuelTypes() const {
      return {FuelType::Electricity};
    }

    std::vector<FuelType> CoilCoolingDX_Impl::heatingFuelTypes() const {
      return {};
    }

    std::vector<AppGFuelType> CoilCoolingDX_Impl::appGHeatingFuelTypes() const {
      return {};
    }

  }  // namespace detail

  CoilCoolingDX::CoilCoolingDX(const Model& model, const CoilCoolingDXCurveFitPerformance& coilCoolingDXCurveFitPerformance)
    : StraightComponent(CoilCoolingDX::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CoilCoolingDX_Impl>());

    auto always_on = model.alwaysOnDiscreteSchedule();
    bool ok = setAvailabilitySchedule(always_on);
    OS_ASSERT(ok);
    ok = setPerformanceObject(coilCoolingDXCurveFitPerformance);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s coilCoolingDXCurveFitPerformance to "
                                     << coilCoolingDXCurveFitPerformance.briefDescription() << ".");
    }
  }

  IddObjectType CoilCoolingDX::iddObjectType() {
    return {IddObjectType::OS_Coil_Cooling_DX};
  }

  Schedule CoilCoolingDX::availabilitySchedule() const {
    return getImpl<detail::CoilCoolingDX_Impl>()->availabilitySchedule();
  }

  boost::optional<ThermalZone> CoilCoolingDX::condenserZone() const {
    return getImpl<detail::CoilCoolingDX_Impl>()->condenserZone();
  }

  CoilCoolingDXCurveFitPerformance CoilCoolingDX::performanceObject() const {
    return getImpl<detail::CoilCoolingDX_Impl>()->performanceObject();
  }

  bool CoilCoolingDX::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::CoilCoolingDX_Impl>()->setAvailabilitySchedule(schedule);
  }

  bool CoilCoolingDX::setCondenserZone(const ThermalZone& thermalZone) {
    return getImpl<detail::CoilCoolingDX_Impl>()->setCondenserZone(thermalZone);
  }

  void CoilCoolingDX::resetCondenserZone() {
    getImpl<detail::CoilCoolingDX_Impl>()->resetCondenserZone();
  }

  bool CoilCoolingDX::setPerformanceObject(const CoilCoolingDXCurveFitPerformance& coilCoolingDXCurveFitPerformance) {
    return getImpl<detail::CoilCoolingDX_Impl>()->setPerformanceObject(coilCoolingDXCurveFitPerformance);
  }

  //boost::optional<std::string> CoilCoolingDX::condenserInletNodeName() const {
  //return getImpl<detail::CoilCoolingDX_Impl>()->condenserInletNodeName();
  //}

  //bool CoilCoolingDX::setCondenserInletNodeName(const std::string& condenserInletNodeName) {
  //return getImpl<detail::CoilCoolingDX_Impl>()->setCondenserInletNodeName(condenserInletNodeName);
  //}

  //void CoilCoolingDX::resetCondenserInletNodeName() {
  //getImpl<detail::CoilCoolingDX_Impl>()->resetCondenserInletNodeName();
  //}

  //bool CoilCoolingDX::setCondenserOutletNodeName(const std::string& condenserOutletNodeName) {
  //return getImpl<detail::CoilCoolingDX_Impl>()->setCondenserOutletNodeName(condenserOutletNodeName);
  //}

  //boost::optional<std::string> CoilCoolingDX::condenserOutletNodeName() const {
  //return getImpl<detail::CoilCoolingDX_Impl>()->condenserOutletNodeName();
  //}

  //void CoilCoolingDX::resetCondenserOutletNodeName() {
  //getImpl<detail::CoilCoolingDX_Impl>()->resetCondenserOutletNodeName();
  //}

  // boost::optional<std::string> CoilCoolingDX::condensateCollectionWaterStorageTankName() const {
  //   return getImpl<detail::CoilCoolingDX_Impl>()->condensateCollectionWaterStorageTankName();
  // }
  //
  // bool CoilCoolingDX::setCondensateCollectionWaterStorageTankName(const std::string& condensateCollectionWaterStorageTankName) {
  //   return getImpl<detail::CoilCoolingDX_Impl>()->setCondensateCollectionWaterStorageTankName(condensateCollectionWaterStorageTankName);
  // }
  //
  // void CoilCoolingDX::resetCondensateCollectionWaterStorageTankName() {
  //   getImpl<detail::CoilCoolingDX_Impl>()->resetCondensateCollectionWaterStorageTankName();
  // }
  //
  // boost::optional<std::string> CoilCoolingDX::evaporativeCondenserSupplyWaterStorageTankName() const {
  //   return getImpl<detail::CoilCoolingDX_Impl>()->evaporativeCondenserSupplyWaterStorageTankName();
  // }
  //
  // bool CoilCoolingDX::setEvaporativeCondenserSupplyWaterStorageTankName(const std::string& evaporativeCondenserSupplyWaterStorageTankName) {
  //   return getImpl<detail::CoilCoolingDX_Impl>()->setEvaporativeCondenserSupplyWaterStorageTankName(evaporativeCondenserSupplyWaterStorageTankName);
  // }
  //
  // void CoilCoolingDX::resetEvaporativeCondenserSupplyWaterStorageTankName() {
  //   getImpl<detail::CoilCoolingDX_Impl>()->resetEvaporativeCondenserSupplyWaterStorageTankName();
  // }

  /// @cond
  CoilCoolingDX::CoilCoolingDX(std::shared_ptr<detail::CoilCoolingDX_Impl> impl) : StraightComponent(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
