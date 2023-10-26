/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "AirTerminalSingleDuctVAVHeatAndCoolNoReheat.hpp"
#include "AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "PortList.hpp"
#include "PortList_Impl.hpp"
#include "Mixer.hpp"
#include "Mixer_Impl.hpp"
#include "Splitter.hpp"
#include "Splitter_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include <utilities/idd/OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheat_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                                       bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AirTerminalSingleDuctVAVHeatAndCoolNoReheat::iddObjectType());
    }

    AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AirTerminalSingleDuctVAVHeatAndCoolNoReheat::iddObjectType());
    }

    AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl(
      const AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{// These apply to all AirTerminals
                                                   "Zone Air Terminal Sensible Heating Energy", "Zone Air Terminal Sensible Heating Rate",
                                                   "Zone Air Terminal Sensible Cooling Energy", "Zone Air Terminal Sensible Cooling Rate",

                                                   "Zone Air Terminal VAV Damper Position",     "Zone Air Terminal Outdoor Air Volume Flow Rate"};
      return result;
    }

    IddObjectType AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::iddObjectType() const {
      return AirTerminalSingleDuctVAVHeatAndCoolNoReheat::iddObjectType();
    }

    std::vector<ScheduleTypeKey> AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::AvailabilitySchedule) != e) {
        result.push_back(ScheduleTypeKey("AirTerminalSingleDuctVAVHeatAndCoolNoReheat", "Availability Schedule"));
      }
      if (std::find(b, e, OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::MinimumAirFlowTurndownScheduleName) != e) {
        result.push_back(ScheduleTypeKey("AirTerminalSingleDuctVAVHeatAndCoolNoReheat", "Minimum Air Flow Turndown"));
      }
      return result;
    }

    boost::optional<Schedule> AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::availabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::AvailabilitySchedule);
    }

    boost::optional<double> AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::maximumAirFlowRate() const {
      return getDouble(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::MaximumAirFlowRate, true);
    }

    bool AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::isMaximumAirFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::MaximumAirFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    double AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::zoneMinimumAirFlowFraction() const {
      boost::optional<double> value = getDouble(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::ZoneMinimumAirFlowFraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Schedule> AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::minimumAirFlowTurndownSchedule() const {
      return this->getObject<AirTerminalSingleDuctVAVHeatAndCoolNoReheat>().getModelObjectTarget<Schedule>(
        OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::MinimumAirFlowTurndownScheduleName);
    }

    boost::optional<ModelObject> AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::minimumAirFlowTurndownScheduleAsModelObject() const {
      OptionalModelObject result;
      OptionalSchedule intermediate = minimumAirFlowTurndownSchedule();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    bool AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::AvailabilitySchedule,
                                "AirTerminalSingleDuctVAVHeatAndCoolNoReheat", "Availability Schedule", schedule);
      return result;
    }

    void AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::resetAvailabilitySchedule() {
      bool result = setString(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::AvailabilitySchedule, "");
      OS_ASSERT(result);
    }

    bool AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::setMaximumAirFlowRate(boost::optional<double> maximumAirFlowRate) {
      bool result(false);
      if (maximumAirFlowRate) {
        result = setDouble(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::MaximumAirFlowRate, maximumAirFlowRate.get());
      }
      return result;
    }

    void AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::autosizeMaximumAirFlowRate() {
      bool result = setString(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::MaximumAirFlowRate, "autosize");
      OS_ASSERT(result);
    }

    bool AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::setZoneMinimumAirFlowFraction(double zoneMinimumAirFlowFraction) {
      bool result = setDouble(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::ZoneMinimumAirFlowFraction, zoneMinimumAirFlowFraction);
      return result;
    }

    bool AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::setMinimumAirFlowTurndownSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::MinimumAirFlowTurndownScheduleName,
                                "AirTerminalSingleDuctVAVReheat", "Minimum Air Flow Turndown", schedule);
      return result;
    }

    void AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::resetMinimumAirFlowTurndownSchedule() {
      bool result = setString(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::MinimumAirFlowTurndownScheduleName, "");
      OS_ASSERT(result);
    }

    bool AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::setMinimumAirFlowTurndownScheduleAsModelObject(
      const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
        if (intermediate) {
          Schedule schedule(*intermediate);
          return setMinimumAirFlowTurndownSchedule(schedule);
        } else {
          return false;
        }
      } else {
        resetMinimumAirFlowTurndownSchedule();
      }
      return true;
    }

    unsigned AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::inletPort() const {
      return OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::AirInlet;
    }

    unsigned AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::outletPort() const {
      return OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::AirOutlet;
    }

    bool AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::addToNode(Node& node) {
      Model _model = node.model();

      if (boost::optional<ModelObject> outlet = node.outletModelObject()) {
        boost::optional<ThermalZone> thermalZone;

        if (boost::optional<PortList> portList = outlet->optionalCast<PortList>()) {
          thermalZone = portList->thermalZone();
        }

        if (thermalZone || (outlet->optionalCast<Mixer>() && node.airLoopHVAC())) {
          if (boost::optional<ModelObject> inlet = node.inletModelObject()) {
            if (boost::optional<Splitter> splitter = inlet->optionalCast<Splitter>()) {
              boost::optional<ModelObject> sourceModelObject = inlet;
              boost::optional<unsigned> sourcePort = node.connectedObjectPort(node.inletPort());

              if (sourcePort && sourceModelObject) {
                Node inletNode(_model);

                _model.connect(sourceModelObject.get(), sourcePort.get(), inletNode, inletNode.inletPort());

                _model.connect(inletNode, inletNode.outletPort(), this->getObject<ModelObject>(), this->inletPort());

                _model.connect(this->getObject<ModelObject>(), outletPort(), node, node.inletPort());

                if (thermalZone) {
                  auto mo = this->getObject<AirTerminalSingleDuctVAVHeatAndCoolNoReheat>();

                  thermalZone->addEquipment(mo);
                }

                return true;
              }
            }
          }
        }
      }

      return false;
    }

    std::vector<IdfObject> AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::remove() {
      Model _model = this->model();
      auto thisObject = this->getObject<ModelObject>();

      boost::optional<ModelObject> sourceModelObject = this->inletModelObject();
      boost::optional<unsigned> sourcePort = this->connectedObjectPort(this->inletPort());

      boost::optional<ModelObject> targetModelObject = this->outletModelObject();
      boost::optional<unsigned> targetPort = this->connectedObjectPort(this->outletPort());

      std::vector<ThermalZone> thermalZones = _model.getConcreteModelObjects<ThermalZone>();
      for (auto& thermalZone : thermalZones) {
        std::vector<ModelObject> equipment = thermalZone.equipment();

        if (std::find(equipment.begin(), equipment.end(), thisObject) != equipment.end()) {
          thermalZone.removeEquipment(thisObject);

          break;
        }
      }

      if (sourcePort && sourceModelObject && targetPort && targetModelObject) {
        if (boost::optional<Node> inletNode = sourceModelObject->optionalCast<Node>()) {
          if (boost::optional<ModelObject> source2ModelObject = inletNode->inletModelObject()) {
            if (boost::optional<unsigned> source2Port = inletNode->connectedObjectPort(inletNode->inletPort())) {
              _model.connect(source2ModelObject.get(), source2Port.get(), targetModelObject.get(), targetPort.get());

              inletNode->disconnect();
              inletNode->remove();

              return StraightComponent_Impl::remove();
            }
          }
        }
      }

      model().disconnect(getObject<ModelObject>(), inletPort());
      model().disconnect(getObject<ModelObject>(), outletPort());

      return StraightComponent_Impl::remove();
    }

    bool AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::isRemovable() const {
      return true;
    }

    boost::optional<double> AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::autosizedMaximumAirFlowRate() const {
      return getAutosizedValue("Design Size Maximum Air Flow Rate", "m3/s");
    }

    void AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::autosize() {
      autosizeMaximumAirFlowRate();
    }

    void AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedMaximumAirFlowRate();
      if (val) {
        setMaximumAirFlowRate(val.get());
      }
    }

    ComponentType AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::componentType() const {
      if (auto a_ = airLoopHVAC()) {
        return a_->componentType();
      }

      return ComponentType::None;
    }

    std::vector<FuelType> AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::coolingFuelTypes() const {
      if (auto a_ = airLoopHVAC()) {
        return a_->coolingFuelTypes();
      }
      return {};
    }

    std::vector<FuelType> AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::heatingFuelTypes() const {
      if (auto a_ = airLoopHVAC()) {
        return a_->heatingFuelTypes();
      }
      return {};
    }

    std::vector<AppGFuelType> AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl::appGHeatingFuelTypes() const {
      if (auto a_ = airLoopHVAC()) {
        return a_->appGHeatingFuelTypes();
      }
      return {};
    }

  }  // namespace detail

  AirTerminalSingleDuctVAVHeatAndCoolNoReheat::AirTerminalSingleDuctVAVHeatAndCoolNoReheat(const Model& model)
    : StraightComponent(AirTerminalSingleDuctVAVHeatAndCoolNoReheat::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl>());

    autosizeMaximumAirFlowRate();
    setZoneMinimumAirFlowFraction(0.0);
  }

  IddObjectType AirTerminalSingleDuctVAVHeatAndCoolNoReheat::iddObjectType() {
    return {IddObjectType::OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheat};
  }

  boost::optional<Schedule> AirTerminalSingleDuctVAVHeatAndCoolNoReheat::availabilitySchedule() const {
    return getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl>()->availabilitySchedule();
  }

  boost::optional<double> AirTerminalSingleDuctVAVHeatAndCoolNoReheat::maximumAirFlowRate() const {
    return getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl>()->maximumAirFlowRate();
  }

  bool AirTerminalSingleDuctVAVHeatAndCoolNoReheat::isMaximumAirFlowRateAutosized() const {
    return getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl>()->isMaximumAirFlowRateAutosized();
  }

  double AirTerminalSingleDuctVAVHeatAndCoolNoReheat::zoneMinimumAirFlowFraction() const {
    return getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl>()->zoneMinimumAirFlowFraction();
  }

  boost::optional<Schedule> AirTerminalSingleDuctVAVHeatAndCoolNoReheat::minimumAirFlowTurndownSchedule() const {
    return getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl>()->minimumAirFlowTurndownSchedule();
  }

  bool AirTerminalSingleDuctVAVHeatAndCoolNoReheat::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl>()->setAvailabilitySchedule(schedule);
  }

  void AirTerminalSingleDuctVAVHeatAndCoolNoReheat::resetAvailabilitySchedule() {
    getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl>()->resetAvailabilitySchedule();
  }

  bool AirTerminalSingleDuctVAVHeatAndCoolNoReheat::setMaximumAirFlowRate(double maximumAirFlowRate) {
    return getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl>()->setMaximumAirFlowRate(maximumAirFlowRate);
  }

  void AirTerminalSingleDuctVAVHeatAndCoolNoReheat::autosizeMaximumAirFlowRate() {
    getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl>()->autosizeMaximumAirFlowRate();
  }

  bool AirTerminalSingleDuctVAVHeatAndCoolNoReheat::setZoneMinimumAirFlowFraction(double zoneMinimumAirFlowFraction) {
    return getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl>()->setZoneMinimumAirFlowFraction(zoneMinimumAirFlowFraction);
  }

  bool AirTerminalSingleDuctVAVHeatAndCoolNoReheat::setMinimumAirFlowTurndownSchedule(Schedule& schedule) {
    return getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl>()->setMinimumAirFlowTurndownSchedule(schedule);
  }

  void AirTerminalSingleDuctVAVHeatAndCoolNoReheat::resetMinimumAirFlowTurndownSchedule() {
    getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl>()->resetMinimumAirFlowTurndownSchedule();
  }

  /// @cond
  AirTerminalSingleDuctVAVHeatAndCoolNoReheat::AirTerminalSingleDuctVAVHeatAndCoolNoReheat(
    std::shared_ptr<detail::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl> impl)
    : StraightComponent(std::move(impl)) {}
  /// @endcond

  boost::optional<double> AirTerminalSingleDuctVAVHeatAndCoolNoReheat::autosizedMaximumAirFlowRate() const {
    return getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl>()->autosizedMaximumAirFlowRate();
  }

}  // namespace model
}  // namespace openstudio
