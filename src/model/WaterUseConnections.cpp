/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "Model.hpp"
#include "WaterUseConnections.hpp"
#include "WaterUseConnections_Impl.hpp"
#include "WaterUseEquipment.hpp"
#include "WaterUseEquipment_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "PlantLoop.hpp"
#include "PlantLoop_Impl.hpp"
#include <utilities/idd/IddFactory.hxx>

#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"
#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/OS_WaterUse_Connections_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {

namespace model {

  namespace detail {

    WaterUseConnections_Impl::WaterUseConnections_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == WaterUseConnections::iddObjectType());
    }

    WaterUseConnections_Impl::WaterUseConnections_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == WaterUseConnections::iddObjectType());
    }

    WaterUseConnections_Impl::WaterUseConnections_Impl(const WaterUseConnections_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& WaterUseConnections_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Water Use Connections Hot Water Mass Flow Rate",
                                                   "Water Use Connections Cold Water Mass Flow Rate",
                                                   "Water Use Connections Total Mass Flow Rate",
                                                   "Water Use Connections Drain Water Mass Flow Rate",
                                                   "Water Use Connections Heat Recovery Mass Flow Rate",
                                                   "Water Use Connections Hot Water Volume Flow Rate",
                                                   "Water Use Connections Cold Water Volume Flow Rate",
                                                   "Water Use Connections Total Volume Flow Rate",
                                                   "Water Use Connections Hot Water Volume",
                                                   "Water Use Connections Cold Water Volume",
                                                   "Water Use Connections Total Volume",
                                                   "Water Use Connections Hot Water Temperature",
                                                   "Water Use Connections Cold Water Temperature",
                                                   "Water Use Connections Drain Water Temperature",
                                                   "Water Use Connections Return Water Temperature",
                                                   "Water Use Connections Waste Water Temperature",
                                                   "Water Use Connections Heat Recovery Water Temperature",
                                                   "Water Use Connections Heat Recovery Effectiveness",
                                                   "Water Use Connections Heat Recovery Rate",
                                                   "Water Use Connections Heat Recovery Energy",
                                                   "Water Use Connections Plant Hot Water Energy"};
      return result;
    }

    std::vector<ScheduleTypeKey> WaterUseConnections_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_WaterUse_ConnectionsFields::HotWaterSupplyTemperatureScheduleName) != e) {
        result.push_back(ScheduleTypeKey("WaterUseConnections", "Hot Water Supply Temperature"));
      }
      if (std::find(b, e, OS_WaterUse_ConnectionsFields::ColdWaterSupplyTemperatureScheduleName) != e) {
        result.push_back(ScheduleTypeKey("WaterUseConnections", "Cold Water Supply Temperature"));
      }
      return result;
    }

    IddObjectType WaterUseConnections_Impl::iddObjectType() const {
      return WaterUseConnections::iddObjectType();
    }

    boost::optional<Schedule> WaterUseConnections_Impl::hotWaterSupplyTemperatureSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_WaterUse_ConnectionsFields::HotWaterSupplyTemperatureScheduleName);
    }

    boost::optional<Schedule> WaterUseConnections_Impl::coldWaterSupplyTemperatureSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_WaterUse_ConnectionsFields::ColdWaterSupplyTemperatureScheduleName);
    }

    bool WaterUseConnections_Impl::setHotWaterSupplyTemperatureSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_WaterUse_ConnectionsFields::HotWaterSupplyTemperatureScheduleName, "WaterUseConnections",
                                "Hot Water Supply Temperature", schedule);
      return result;
    }

    void WaterUseConnections_Impl::resetHotWaterSupplyTemperatureSchedule() {
      bool result = setString(OS_WaterUse_ConnectionsFields::HotWaterSupplyTemperatureScheduleName, "");
      OS_ASSERT(result);
    }

    bool WaterUseConnections_Impl::setColdWaterSupplyTemperatureSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_WaterUse_ConnectionsFields::ColdWaterSupplyTemperatureScheduleName, "WaterUseConnections",
                                "Cold Water Supply Temperature", schedule);
      return result;
    }

    void WaterUseConnections_Impl::resetColdWaterSupplyTemperatureSchedule() {
      bool result = setString(OS_WaterUse_ConnectionsFields::ColdWaterSupplyTemperatureScheduleName, "");
      OS_ASSERT(result);
    }

    boost::optional<ModelObject> WaterUseConnections_Impl::hotWaterSupplyTemperatureScheduleAsModelObject() const {
      OptionalModelObject result;
      OptionalSchedule intermediate = hotWaterSupplyTemperatureSchedule();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    boost::optional<ModelObject> WaterUseConnections_Impl::coldWaterSupplyTemperatureScheduleAsModelObject() const {
      OptionalModelObject result;
      OptionalSchedule intermediate = coldWaterSupplyTemperatureSchedule();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    bool WaterUseConnections_Impl::setHotWaterSupplyTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
        if (intermediate) {
          return setHotWaterSupplyTemperatureSchedule(*intermediate);
        } else {
          return false;
        }
      } else {
        resetHotWaterSupplyTemperatureSchedule();
      }
      return true;
    }

    bool WaterUseConnections_Impl::setColdWaterSupplyTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
        if (intermediate) {
          return setColdWaterSupplyTemperatureSchedule(*intermediate);
        } else {
          return false;
        }
      } else {
        resetColdWaterSupplyTemperatureSchedule();
      }
      return true;
    }

    unsigned WaterUseConnections_Impl::inletPort() const {
      return OS_WaterUse_ConnectionsFields::InletNodeName;
    }

    unsigned WaterUseConnections_Impl::outletPort() const {
      return OS_WaterUse_ConnectionsFields::OutletNodeName;
    }

    std::vector<WaterUseEquipment> WaterUseConnections_Impl::waterUseEquipment() const {
      std::vector<WaterUseEquipment> result;

      std::vector<IdfExtensibleGroup> groups = extensibleGroups();

      for (const auto& elem : groups) {
        auto group = elem.cast<WorkspaceExtensibleGroup>();

        boost::optional<WorkspaceObject> wo = group.getTarget(OS_WaterUse_ConnectionsExtensibleFields::WaterUseEquipmentName);

        if (wo) {
          auto equipment = wo->cast<WaterUseEquipment>();

          result.push_back(equipment);
        }
      }

      return result;
    }

    bool WaterUseConnections_Impl::addWaterUseEquipment(const WaterUseEquipment& waterUseEquipment) {
      bool result = false;

      if (waterUseEquipment.model() == model()) {
        auto group = getObject<WaterUseConnections>().pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();

        group.setPointer(OS_WaterUse_ConnectionsExtensibleFields::WaterUseEquipmentName, waterUseEquipment.handle());

        result = true;
      }

      return result;
    }

    bool WaterUseConnections_Impl::removeWaterUseEquipment(WaterUseEquipment& _waterUseEquipment) {
      std::vector<WaterUseEquipment> equipment = waterUseEquipment();

      int i = 0;

      for (const auto& elem : equipment) {
        if (elem.handle() == _waterUseEquipment.handle()) {
          eraseExtensibleGroup(i);

          return true;
        }

        i++;
      }

      return false;
    }

    bool WaterUseConnections_Impl::addToNode(Node& node) {
      if (boost::optional<PlantLoop> plant = node.plantLoop()) {
        if (plant->demandComponent(node.handle())) {
          return StraightComponent_Impl::addToNode(node);
        }
      }

      return false;
    }

    std::string WaterUseConnections_Impl::drainWaterHeatExchangerType() const {
      boost::optional<std::string> value = getString(OS_WaterUse_ConnectionsFields::DrainWaterHeatExchangerType, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool WaterUseConnections_Impl::setDrainWaterHeatExchangerType(const std::string& drainWaterHeatExchangerType) {
      bool result = setString(OS_WaterUse_ConnectionsFields::DrainWaterHeatExchangerType, drainWaterHeatExchangerType);
      return result;
    }

    std::string WaterUseConnections_Impl::drainWaterHeatExchangerDestination() const {
      boost::optional<std::string> value = getString(OS_WaterUse_ConnectionsFields::DrainWaterHeatExchangerDestination, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool WaterUseConnections_Impl::setDrainWaterHeatExchangerDestination(const std::string& drainWaterHeatExchangerDestination) {
      bool result = setString(OS_WaterUse_ConnectionsFields::DrainWaterHeatExchangerDestination, drainWaterHeatExchangerDestination);
      return result;
    }

    boost::optional<double> WaterUseConnections_Impl::drainWaterHeatExchangerUFactorTimesArea() const {
      return getDouble(OS_WaterUse_ConnectionsFields::DrainWaterHeatExchangerUFactorTimesArea, true);
    }

    bool WaterUseConnections_Impl::setDrainWaterHeatExchangerUFactorTimesArea(double drainWaterHeatExchangerUFactorTimesArea) {
      bool result = setDouble(OS_WaterUse_ConnectionsFields::DrainWaterHeatExchangerUFactorTimesArea, drainWaterHeatExchangerUFactorTimesArea);
      return result;
    }

    void WaterUseConnections_Impl::resetDrainWaterHeatExchangerUFactorTimesArea() {
      bool result = setString(OS_WaterUse_ConnectionsFields::DrainWaterHeatExchangerUFactorTimesArea, "");
      OS_ASSERT(result);
    }

    ComponentType WaterUseConnections_Impl::componentType() const {
      return ComponentType::None;
    }

    std::vector<FuelType> WaterUseConnections_Impl::coolingFuelTypes() const {
      return {};
    }

    std::vector<FuelType> WaterUseConnections_Impl::heatingFuelTypes() const {
      return {};
    }

    std::vector<AppGFuelType> WaterUseConnections_Impl::appGHeatingFuelTypes() const {
      return {};
    }

  }  // namespace detail

  WaterUseConnections::WaterUseConnections(const Model& model) : StraightComponent(WaterUseConnections::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::WaterUseConnections_Impl>());

    setDrainWaterHeatExchangerType("None");
    setDrainWaterHeatExchangerDestination("Plant");
  }

  IddObjectType WaterUseConnections::iddObjectType() {
    static const IddObjectType result(IddObjectType::OS_WaterUse_Connections);
    return result;
  }

  boost::optional<Schedule> WaterUseConnections::hotWaterSupplyTemperatureSchedule() const {
    return getImpl<detail::WaterUseConnections_Impl>()->hotWaterSupplyTemperatureSchedule();
  }

  boost::optional<Schedule> WaterUseConnections::coldWaterSupplyTemperatureSchedule() const {
    return getImpl<detail::WaterUseConnections_Impl>()->coldWaterSupplyTemperatureSchedule();
  }

  bool WaterUseConnections::setHotWaterSupplyTemperatureSchedule(Schedule& hotWaterSupplyTemperatureSchedule) {
    return getImpl<detail::WaterUseConnections_Impl>()->setHotWaterSupplyTemperatureSchedule(hotWaterSupplyTemperatureSchedule);
  }

  void WaterUseConnections::resetHotWaterSupplyTemperatureSchedule() {
    getImpl<detail::WaterUseConnections_Impl>()->resetHotWaterSupplyTemperatureSchedule();
  }

  bool WaterUseConnections::setColdWaterSupplyTemperatureSchedule(Schedule& coldWaterSupplyTemperatureSchedule) {
    return getImpl<detail::WaterUseConnections_Impl>()->setColdWaterSupplyTemperatureSchedule(coldWaterSupplyTemperatureSchedule);
  }

  void WaterUseConnections::resetColdWaterSupplyTemperatureSchedule() {
    getImpl<detail::WaterUseConnections_Impl>()->resetColdWaterSupplyTemperatureSchedule();
  }

  unsigned WaterUseConnections::inletPort() const {
    return getImpl<detail::WaterUseConnections_Impl>()->inletPort();
  }

  unsigned WaterUseConnections::outletPort() const {
    return getImpl<detail::WaterUseConnections_Impl>()->outletPort();
  }

  std::vector<WaterUseEquipment> WaterUseConnections::waterUseEquipment() const {
    return getImpl<detail::WaterUseConnections_Impl>()->waterUseEquipment();
  }

  bool WaterUseConnections::addWaterUseEquipment(const WaterUseEquipment& waterUseEquipment) {
    return getImpl<detail::WaterUseConnections_Impl>()->addWaterUseEquipment(waterUseEquipment);
  }

  bool WaterUseConnections::removeWaterUseEquipment(WaterUseEquipment& waterUseEquipment) {
    return getImpl<detail::WaterUseConnections_Impl>()->removeWaterUseEquipment(waterUseEquipment);
  }

  std::string WaterUseConnections::drainWaterHeatExchangerType() const {
    return getImpl<detail::WaterUseConnections_Impl>()->drainWaterHeatExchangerType();
  }

  bool WaterUseConnections::setDrainWaterHeatExchangerType(const std::string& drainWaterHeatExchangerType) {
    return getImpl<detail::WaterUseConnections_Impl>()->setDrainWaterHeatExchangerType(drainWaterHeatExchangerType);
  }

  std::string WaterUseConnections::drainWaterHeatExchangerDestination() const {
    return getImpl<detail::WaterUseConnections_Impl>()->drainWaterHeatExchangerDestination();
  }

  bool WaterUseConnections::setDrainWaterHeatExchangerDestination(const std::string& drainWaterHeatExchangerDestination) {
    return getImpl<detail::WaterUseConnections_Impl>()->setDrainWaterHeatExchangerDestination(drainWaterHeatExchangerDestination);
  }

  boost::optional<double> WaterUseConnections::drainWaterHeatExchangerUFactorTimesArea() const {
    return getImpl<detail::WaterUseConnections_Impl>()->drainWaterHeatExchangerUFactorTimesArea();
  }

  bool WaterUseConnections::setDrainWaterHeatExchangerUFactorTimesArea(double drainWaterHeatExchangerUFactorTimesArea) {
    return getImpl<detail::WaterUseConnections_Impl>()->setDrainWaterHeatExchangerUFactorTimesArea(drainWaterHeatExchangerUFactorTimesArea);
  }

  void WaterUseConnections::resetDrainWaterHeatExchangerUFactorTimesArea() {
    getImpl<detail::WaterUseConnections_Impl>()->resetDrainWaterHeatExchangerUFactorTimesArea();
  }

  /// @cond
  WaterUseConnections::WaterUseConnections(std::shared_ptr<detail::WaterUseConnections_Impl> impl) : StraightComponent(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
