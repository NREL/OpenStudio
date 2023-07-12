/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "AirTerminalSingleDuctConstantVolumeNoReheat.hpp"
#include "AirTerminalSingleDuctConstantVolumeNoReheat_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "AirLoopHVACZoneSplitter.hpp"
#include "AirLoopHVACZoneSplitter_Impl.hpp"
#include "AirLoopHVACZoneMixer.hpp"
#include "AirLoopHVACZoneMixer_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "PortList.hpp"
#include "PortList_Impl.hpp"
#include "Model.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include <utilities/idd/OS_AirTerminal_SingleDuct_ConstantVolume_NoReheat_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    AirTerminalSingleDuctConstantVolumeNoReheat_Impl::AirTerminalSingleDuctConstantVolumeNoReheat_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                                       bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AirTerminalSingleDuctConstantVolumeNoReheat::iddObjectType());
    }

    AirTerminalSingleDuctConstantVolumeNoReheat_Impl::AirTerminalSingleDuctConstantVolumeNoReheat_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AirTerminalSingleDuctConstantVolumeNoReheat::iddObjectType());
    }

    AirTerminalSingleDuctConstantVolumeNoReheat_Impl::AirTerminalSingleDuctConstantVolumeNoReheat_Impl(
      const AirTerminalSingleDuctConstantVolumeNoReheat_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    // Get all output variable names that could be associated with this object.
    const std::vector<std::string>& AirTerminalSingleDuctConstantVolumeNoReheat_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Zone Air Terminal Sensible Heating Energy", "Zone Air Terminal Sensible Cooling Energy",
                                                   "Zone Air Terminal Sensible Heating Rate", "Zone Air Terminal Sensible Cooling Rate"};
      return result;
    }

    IddObjectType AirTerminalSingleDuctConstantVolumeNoReheat_Impl::iddObjectType() const {
      return AirTerminalSingleDuctConstantVolumeNoReheat::iddObjectType();
    }

    std::vector<ScheduleTypeKey> AirTerminalSingleDuctConstantVolumeNoReheat_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_AirTerminal_SingleDuct_ConstantVolume_NoReheatFields::AvailabilityScheduleName) != e) {
        result.push_back(ScheduleTypeKey("AirTerminalSingleDuctConstantVolumeNoReheat", "Availability"));
      }
      return result;
    }

    Schedule AirTerminalSingleDuctConstantVolumeNoReheat_Impl::availabilitySchedule() const {
      boost::optional<Schedule> value = optionalAvailabilitySchedule();
      if (!value) {
        // it is an error if we get here, however we don't want to crash
        // so we hook up to global always on schedule
        LOG(Error, "Required availability schedule not set, using 'Always On' schedule");
        value = this->model().alwaysOnDiscreteSchedule();
        OS_ASSERT(value);
        const_cast<AirTerminalSingleDuctConstantVolumeNoReheat_Impl*>(this)->setAvailabilitySchedule(*value);
        value = optionalAvailabilitySchedule();
      }
      OS_ASSERT(value);
      return value.get();
    }

    bool AirTerminalSingleDuctConstantVolumeNoReheat_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_AirTerminal_SingleDuct_ConstantVolume_NoReheatFields::AvailabilityScheduleName,
                                "AirTerminalSingleDuctConstantVolumeNoReheat", "Availability", schedule);
      return result;
    }

    unsigned AirTerminalSingleDuctConstantVolumeNoReheat_Impl::inletPort() const {
      return OS_AirTerminal_SingleDuct_ConstantVolume_NoReheatFields::AirInletNodeName;
    }

    unsigned AirTerminalSingleDuctConstantVolumeNoReheat_Impl::outletPort() const {
      return OS_AirTerminal_SingleDuct_ConstantVolume_NoReheatFields::AirOutletNodeName;
    }

    bool AirTerminalSingleDuctConstantVolumeNoReheat_Impl::addToNode(Node& node) {
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
                  auto mo = this->getObject<AirTerminalSingleDuctConstantVolumeNoReheat>();

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

    std::vector<IdfObject> AirTerminalSingleDuctConstantVolumeNoReheat_Impl::remove() {
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

              // Need to delete the inlet node too
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

    bool AirTerminalSingleDuctConstantVolumeNoReheat_Impl::isRemovable() const {
      return true;
    }

    boost::optional<Schedule> AirTerminalSingleDuctConstantVolumeNoReheat_Impl::optionalAvailabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(
        OS_AirTerminal_SingleDuct_ConstantVolume_NoReheatFields::AvailabilityScheduleName);
    }

    boost::optional<ModelObject> AirTerminalSingleDuctConstantVolumeNoReheat_Impl::availabilityScheduleAsModelObject() const {
      OptionalModelObject result = availabilitySchedule();
      return result;
    }

    bool AirTerminalSingleDuctConstantVolumeNoReheat_Impl::setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
        if (intermediate) {
          Schedule schedule(*intermediate);
          return setAvailabilitySchedule(schedule);
        }
      }
      return false;
    }

    boost::optional<double> AirTerminalSingleDuctConstantVolumeNoReheat_Impl::maximumAirFlowRate() const {
      return getDouble(OS_AirTerminal_SingleDuct_ConstantVolume_NoReheatFields::MaximumAirFlowRate, true);
    }

    bool AirTerminalSingleDuctConstantVolumeNoReheat_Impl::isMaximumAirFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_AirTerminal_SingleDuct_ConstantVolume_NoReheatFields::MaximumAirFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    bool AirTerminalSingleDuctConstantVolumeNoReheat_Impl::setMaximumAirFlowRate(boost::optional<double> maximumAirFlowRate) {
      bool result(false);
      if (maximumAirFlowRate) {
        result = setDouble(OS_AirTerminal_SingleDuct_ConstantVolume_NoReheatFields::MaximumAirFlowRate, maximumAirFlowRate.get());
      }
      return result;
    }

    void AirTerminalSingleDuctConstantVolumeNoReheat_Impl::autosizeMaximumAirFlowRate() {
      bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_NoReheatFields::MaximumAirFlowRate, "autosize");
      OS_ASSERT(result);
    }

    boost::optional<double> AirTerminalSingleDuctConstantVolumeNoReheat_Impl::autosizedMaximumAirFlowRate() const {
      return getAutosizedValue("Design Size Maximum Air Flow Rate", "m3/s");
    }

    void AirTerminalSingleDuctConstantVolumeNoReheat_Impl::autosize() {
      autosizeMaximumAirFlowRate();
    }

    void AirTerminalSingleDuctConstantVolumeNoReheat_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedMaximumAirFlowRate();
      if (val) {
        setMaximumAirFlowRate(val.get());
      }
    }

    std::vector<EMSActuatorNames> AirTerminalSingleDuctConstantVolumeNoReheat_Impl::emsActuatorNames() const {
      std::vector<EMSActuatorNames> actuators{{"AirTerminal:SingleDuct:ConstantVolume:NoReheat", "Mass Flow Rate"}};
      return actuators;
    }

    std::vector<std::string> AirTerminalSingleDuctConstantVolumeNoReheat_Impl::emsInternalVariableNames() const {
      std::vector<std::string> types{"AirTerminal:SingleDuct:ConstantVolume:NoReheat Maximum Mass Flow Rate"};
      return types;
    }

    ComponentType AirTerminalSingleDuctConstantVolumeNoReheat_Impl::componentType() const {
      if (auto a_ = airLoopHVAC()) {
        return a_->componentType();
      }

      return ComponentType::None;
    }

    std::vector<FuelType> AirTerminalSingleDuctConstantVolumeNoReheat_Impl::coolingFuelTypes() const {
      if (auto a_ = airLoopHVAC()) {
        return a_->coolingFuelTypes();
      }
      return {};
    }

    std::vector<FuelType> AirTerminalSingleDuctConstantVolumeNoReheat_Impl::heatingFuelTypes() const {
      if (auto a_ = airLoopHVAC()) {
        return a_->heatingFuelTypes();
      }
      return {};
    }

    std::vector<AppGFuelType> AirTerminalSingleDuctConstantVolumeNoReheat_Impl::appGHeatingFuelTypes() const {
      if (auto a_ = airLoopHVAC()) {
        return a_->appGHeatingFuelTypes();
      }
      return {};
    }

  }  // namespace detail

  AirTerminalSingleDuctConstantVolumeNoReheat::AirTerminalSingleDuctConstantVolumeNoReheat(const Model& model, Schedule& availabilitySchedule)
    : StraightComponent(AirTerminalSingleDuctConstantVolumeNoReheat::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirTerminalSingleDuctConstantVolumeNoReheat_Impl>());

    setString(openstudio::OS_AirTerminal_SingleDuct_ConstantVolume_NoReheatFields::MaximumAirFlowRate, "AutoSize");

    setAvailabilitySchedule(availabilitySchedule);
  }

  Schedule AirTerminalSingleDuctConstantVolumeNoReheat::availabilitySchedule() const {
    return getImpl<detail::AirTerminalSingleDuctConstantVolumeNoReheat_Impl>()->availabilitySchedule();
  }

  bool AirTerminalSingleDuctConstantVolumeNoReheat::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::AirTerminalSingleDuctConstantVolumeNoReheat_Impl>()->setAvailabilitySchedule(schedule);
  }

  IddObjectType AirTerminalSingleDuctConstantVolumeNoReheat::iddObjectType() {
    IddObjectType result(IddObjectType::OS_AirTerminal_SingleDuct_ConstantVolume_NoReheat);
    return result;
  }

  boost::optional<double> AirTerminalSingleDuctConstantVolumeNoReheat::maximumAirFlowRate() const {
    return getImpl<detail::AirTerminalSingleDuctConstantVolumeNoReheat_Impl>()->maximumAirFlowRate();
  }

  bool AirTerminalSingleDuctConstantVolumeNoReheat::isMaximumAirFlowRateAutosized() const {
    return getImpl<detail::AirTerminalSingleDuctConstantVolumeNoReheat_Impl>()->isMaximumAirFlowRateAutosized();
  }

  bool AirTerminalSingleDuctConstantVolumeNoReheat::setMaximumAirFlowRate(double maximumAirFlowRate) {
    return getImpl<detail::AirTerminalSingleDuctConstantVolumeNoReheat_Impl>()->setMaximumAirFlowRate(maximumAirFlowRate);
  }

  void AirTerminalSingleDuctConstantVolumeNoReheat::autosizeMaximumAirFlowRate() {
    getImpl<detail::AirTerminalSingleDuctConstantVolumeNoReheat_Impl>()->autosizeMaximumAirFlowRate();
  }

  boost::optional<double> AirTerminalSingleDuctConstantVolumeNoReheat::autosizedMaximumAirFlowRate() const {
    return getImpl<detail::AirTerminalSingleDuctConstantVolumeNoReheat_Impl>()->autosizedMaximumAirFlowRate();
  }

  /// @cond
  AirTerminalSingleDuctConstantVolumeNoReheat::AirTerminalSingleDuctConstantVolumeNoReheat(
    std::shared_ptr<detail::AirTerminalSingleDuctConstantVolumeNoReheat_Impl> p)
    : StraightComponent(std::move(p)) {}
  // @endcond

}  // namespace model
}  // namespace openstudio
