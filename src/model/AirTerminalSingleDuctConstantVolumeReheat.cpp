/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "AirTerminalSingleDuctConstantVolumeReheat.hpp"
#include "AirTerminalSingleDuctConstantVolumeReheat_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "HVACComponent.hpp"
#include "HVACComponent_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "PortList.hpp"
#include "PortList_Impl.hpp"
#include "AirLoopHVACZoneSplitter.hpp"
#include "AirLoopHVACZoneSplitter_Impl.hpp"
#include "AirLoopHVACZoneMixer.hpp"
#include "AirLoopHVACZoneMixer_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Model.hpp"
#include "AirflowNetworkEquivalentDuct.hpp"
#include "AirflowNetworkEquivalentDuct_Impl.hpp"

#include <utilities/idd/OS_AirTerminal_SingleDuct_ConstantVolume_Reheat_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Compare.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    AirTerminalSingleDuctConstantVolumeReheat_Impl::AirTerminalSingleDuctConstantVolumeReheat_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                                   bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AirTerminalSingleDuctConstantVolumeReheat::iddObjectType());
    }

    AirTerminalSingleDuctConstantVolumeReheat_Impl::AirTerminalSingleDuctConstantVolumeReheat_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AirTerminalSingleDuctConstantVolumeReheat::iddObjectType());
    }

    AirTerminalSingleDuctConstantVolumeReheat_Impl::AirTerminalSingleDuctConstantVolumeReheat_Impl(
      const AirTerminalSingleDuctConstantVolumeReheat_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& AirTerminalSingleDuctConstantVolumeReheat_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{
        // These apply to all AirTerminals
        "Zone Air Terminal Sensible Heating Energy", "Zone Air Terminal Sensible Heating Rate", "Zone Air Terminal Sensible Cooling Energy",
        "Zone Air Terminal Sensible Cooling Rate"

        // No specific output for this terminal
      };
      return result;
    }

    IddObjectType AirTerminalSingleDuctConstantVolumeReheat_Impl::iddObjectType() const {
      return AirTerminalSingleDuctConstantVolumeReheat::iddObjectType();
    }

    std::vector<ScheduleTypeKey> AirTerminalSingleDuctConstantVolumeReheat_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      // TODO: Check schedule display names.
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::AvailabilityScheduleName) != e) {
        result.push_back(ScheduleTypeKey("AirTerminalSingleDuctConstantVolumeReheat", "Availability"));
      }
      return result;
    }

    Schedule AirTerminalSingleDuctConstantVolumeReheat_Impl::availabilitySchedule() const {
      boost::optional<Schedule> value = optionalAvailabilitySchedule();
      if (!value) {
        // it is an error if we get here, however we don't want to crash
        // so we hook up to global always on schedule
        LOG(Error, "Required availability schedule not set, using 'Always On' schedule");
        value = this->model().alwaysOnDiscreteSchedule();
        OS_ASSERT(value);
        const_cast<AirTerminalSingleDuctConstantVolumeReheat_Impl*>(this)->setAvailabilitySchedule(*value);
        value = optionalAvailabilitySchedule();
      }
      OS_ASSERT(value);
      return value.get();
    }

    unsigned AirTerminalSingleDuctConstantVolumeReheat_Impl::inletPort() const {
      return OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::AirInletNodeName;
    }

    unsigned AirTerminalSingleDuctConstantVolumeReheat_Impl::outletPort() const {
      return OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::AirOutletNodeName;
    }

    bool AirTerminalSingleDuctConstantVolumeReheat_Impl::addToNode(Node& node) {
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
                  auto mo = this->getObject<AirTerminalSingleDuctConstantVolumeReheat>();

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

    std::vector<IdfObject> AirTerminalSingleDuctConstantVolumeReheat_Impl::remove() {
      Model _model = this->model();
      auto thisObject = this->getObject<ModelObject>();

      HVACComponent _reheatCoil = reheatCoil();

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

              if (boost::optional<PlantLoop> loop = _reheatCoil.plantLoop()) {
                loop->removeDemandBranchWithComponent(_reheatCoil);
              }

              return StraightComponent_Impl::remove();
            }
          }
        }
      }

      model().disconnect(getObject<ModelObject>(), inletPort());
      model().disconnect(getObject<ModelObject>(), outletPort());

      if (boost::optional<PlantLoop> loop = _reheatCoil.plantLoop()) {
        loop->removeDemandBranchWithComponent(_reheatCoil);
      }

      return StraightComponent_Impl::remove();
    }

    bool AirTerminalSingleDuctConstantVolumeReheat_Impl::isRemovable() const {
      return true;
    }

    std::vector<ModelObject> AirTerminalSingleDuctConstantVolumeReheat_Impl::children() const {
      std::vector<ModelObject> result;
      if (OptionalHVACComponent intermediate = optionalReheatCoil()) {
        result.push_back(*intermediate);
      }
      std::vector<AirflowNetworkEquivalentDuct> myAFNItems =
        getObject<ModelObject>().getModelObjectSources<AirflowNetworkEquivalentDuct>(AirflowNetworkEquivalentDuct::iddObjectType());
      result.insert(result.end(), myAFNItems.begin(), myAFNItems.end());
      return result;
    }

    ModelObject AirTerminalSingleDuctConstantVolumeReheat_Impl::clone(Model model) const {
      auto modelObjectClone = StraightComponent_Impl::clone(model).cast<AirTerminalSingleDuctConstantVolumeReheat>();

      HVACComponent coil = this->reheatCoil();

      auto coilClone = coil.clone(model).cast<HVACComponent>();

      modelObjectClone.setReheatCoil(coilClone);

      return std::move(modelObjectClone);
    }

    boost::optional<double> AirTerminalSingleDuctConstantVolumeReheat_Impl::maximumAirFlowRate() const {
      return getDouble(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MaximumAirFlowRate, true);
    }

    bool AirTerminalSingleDuctConstantVolumeReheat_Impl::isMaximumAirFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MaximumAirFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    HVACComponent AirTerminalSingleDuctConstantVolumeReheat_Impl::reheatCoil() const {
      boost::optional<HVACComponent> coil = optionalReheatCoil();
      OS_ASSERT(coil);
      return coil.get();
    }

    boost::optional<HVACComponent> AirTerminalSingleDuctConstantVolumeReheat_Impl::optionalReheatCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::ReheatCoilName);
    }

    boost::optional<double> AirTerminalSingleDuctConstantVolumeReheat_Impl::maximumHotWaterorSteamFlowRate() const {
      return getDouble(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MaximumHotWaterorSteamFlowRate, true);
    }

    bool AirTerminalSingleDuctConstantVolumeReheat_Impl::isMaximumHotWaterorSteamFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MaximumHotWaterorSteamFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    double AirTerminalSingleDuctConstantVolumeReheat_Impl::minimumHotWaterorSteamFlowRate() const {
      boost::optional<double> value = getDouble(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MinimumHotWaterorSteamFlowRate, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirTerminalSingleDuctConstantVolumeReheat_Impl::isMinimumHotWaterorSteamFlowRateDefaulted() const {
      return isEmpty(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MinimumHotWaterorSteamFlowRate);
    }

    double AirTerminalSingleDuctConstantVolumeReheat_Impl::convergenceTolerance() const {
      boost::optional<double> value = getDouble(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::ConvergenceTolerance, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirTerminalSingleDuctConstantVolumeReheat_Impl::isConvergenceToleranceDefaulted() const {
      return isEmpty(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::ConvergenceTolerance);
    }

    double AirTerminalSingleDuctConstantVolumeReheat_Impl::maximumReheatAirTemperature() const {
      boost::optional<double> value = getDouble(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MaximumReheatAirTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirTerminalSingleDuctConstantVolumeReheat_Impl::isMaximumReheatAirTemperatureDefaulted() const {
      return isEmpty(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MaximumReheatAirTemperature);
    }

    bool AirTerminalSingleDuctConstantVolumeReheat_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::AvailabilityScheduleName,
                                "AirTerminalSingleDuctConstantVolumeReheat", "Availability", schedule);
      return result;
    }

    bool AirTerminalSingleDuctConstantVolumeReheat_Impl::setMaximumAirFlowRate(boost::optional<double> maximumAirFlowRate) {
      bool result(false);
      if (maximumAirFlowRate) {
        result = setDouble(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MaximumAirFlowRate, maximumAirFlowRate.get());
      } else {
        resetMaximumAirFlowRate();
        result = true;
      }
      return result;
    }

    void AirTerminalSingleDuctConstantVolumeReheat_Impl::resetMaximumAirFlowRate() {
      bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MaximumAirFlowRate, "");
      OS_ASSERT(result);
    }

    void AirTerminalSingleDuctConstantVolumeReheat_Impl::autosizeMaximumAirFlowRate() {
      bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MaximumAirFlowRate, "autosize");
      OS_ASSERT(result);
    }

    bool AirTerminalSingleDuctConstantVolumeReheat_Impl::setReheatCoil(const HVACComponent& coil) {
      bool result = false;

      switch (coil.iddObject().type().value()) {
        case openstudio::IddObjectType::OS_Coil_Heating_Gas: {
          result = true;
          break;
        }
        case openstudio::IddObjectType::OS_Coil_Heating_Water: {
          result = true;
          break;
        }
        case openstudio::IddObjectType::OS_Coil_Heating_Electric: {
          result = true;
          break;
        }
        default: {
          LOG(Warn, "Unsupported or invalid IddObjectType: '" << coil.iddObject().name() << "'");
          result = false;
        }
      }

      if (result) {
        result = this->setPointer(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::ReheatCoilName, coil.handle());
      }

      return result;
    }

    void AirTerminalSingleDuctConstantVolumeReheat_Impl::resetReheatCoil() {
      bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::ReheatCoilName, "");
      OS_ASSERT(result);
    }

    bool AirTerminalSingleDuctConstantVolumeReheat_Impl::setMaximumHotWaterorSteamFlowRate(boost::optional<double> maximumHotWaterorSteamFlowRate) {
      bool result(false);
      if (maximumHotWaterorSteamFlowRate) {
        result =
          setDouble(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MaximumHotWaterorSteamFlowRate, maximumHotWaterorSteamFlowRate.get());
      } else {
        resetMaximumHotWaterorSteamFlowRate();
        result = true;
      }
      return result;
    }

    void AirTerminalSingleDuctConstantVolumeReheat_Impl::resetMaximumHotWaterorSteamFlowRate() {
      bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MaximumHotWaterorSteamFlowRate, "");
      OS_ASSERT(result);
    }

    void AirTerminalSingleDuctConstantVolumeReheat_Impl::autosizeMaximumHotWaterorSteamFlowRate() {
      bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MaximumHotWaterorSteamFlowRate, "autosize");
      OS_ASSERT(result);
    }

    bool AirTerminalSingleDuctConstantVolumeReheat_Impl::setMinimumHotWaterorSteamFlowRate(double minimumHotWaterorSteamFlowRate) {
      bool result = setDouble(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MinimumHotWaterorSteamFlowRate, minimumHotWaterorSteamFlowRate);
      return result;
    }

    void AirTerminalSingleDuctConstantVolumeReheat_Impl::resetMinimumHotWaterorSteamFlowRate() {
      bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MinimumHotWaterorSteamFlowRate, "");
      OS_ASSERT(result);
    }

    bool AirTerminalSingleDuctConstantVolumeReheat_Impl::setConvergenceTolerance(double convergenceTolerance) {
      bool result = setDouble(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::ConvergenceTolerance, convergenceTolerance);
      return result;
    }

    void AirTerminalSingleDuctConstantVolumeReheat_Impl::resetConvergenceTolerance() {
      bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::ConvergenceTolerance, "");
      OS_ASSERT(result);
    }

    bool AirTerminalSingleDuctConstantVolumeReheat_Impl::setMaximumReheatAirTemperature(double maximumReheatAirTemperature) {
      bool result = setDouble(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MaximumReheatAirTemperature, maximumReheatAirTemperature);
      return result;
    }

    void AirTerminalSingleDuctConstantVolumeReheat_Impl::resetMaximumReheatAirTemperature() {
      bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MaximumReheatAirTemperature, "");
      OS_ASSERT(result);
    }

    boost::optional<Schedule> AirTerminalSingleDuctConstantVolumeReheat_Impl::optionalAvailabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirTerminal_SingleDuct_ConstantVolume_ReheatFields::AvailabilityScheduleName);
    }

    boost::optional<ModelObject> AirTerminalSingleDuctConstantVolumeReheat_Impl::availabilityScheduleAsModelObject() const {
      OptionalModelObject result = availabilitySchedule();
      return result;
    }

    boost::optional<ModelObject> AirTerminalSingleDuctConstantVolumeReheat_Impl::reheatCoilAsModelObject() const {
      OptionalModelObject result;
      OptionalHVACComponent intermediate = reheatCoil();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    bool AirTerminalSingleDuctConstantVolumeReheat_Impl::setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
        if (intermediate) {
          Schedule schedule(*intermediate);
          return setAvailabilitySchedule(schedule);
        }
      }
      return false;
    }

    bool AirTerminalSingleDuctConstantVolumeReheat_Impl::setReheatCoilAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalHVACComponent intermediate = modelObject->optionalCast<HVACComponent>();
        if (intermediate) {
          HVACComponent coil(*intermediate);
          return setReheatCoil(coil);
        } else {
          return false;
        }
      } else {
        resetReheatCoil();
      }
      return true;
    }

    AirflowNetworkEquivalentDuct AirTerminalSingleDuctConstantVolumeReheat_Impl::getAirflowNetworkEquivalentDuct(double length, double diameter) {
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

    boost::optional<AirflowNetworkEquivalentDuct> AirTerminalSingleDuctConstantVolumeReheat_Impl::airflowNetworkEquivalentDuct() const {
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

    boost::optional<double> AirTerminalSingleDuctConstantVolumeReheat_Impl::autosizedMaximumAirFlowRate() const {
      return getAutosizedValue("Design Size Maximum Air Flow Rate", "m3/s");
    }

    boost::optional<double> AirTerminalSingleDuctConstantVolumeReheat_Impl::autosizedMaximumHotWaterorSteamFlowRate() const {
      return getAutosizedValue("Design Size Maximum Reheat Water Flow Rate", "m3/s");
    }

    void AirTerminalSingleDuctConstantVolumeReheat_Impl::autosize() {
      autosizeMaximumAirFlowRate();
      autosizeMaximumHotWaterorSteamFlowRate();
    }

    void AirTerminalSingleDuctConstantVolumeReheat_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedMaximumAirFlowRate();
      if (val) {
        setMaximumAirFlowRate(val.get());
      }

      val = autosizedMaximumHotWaterorSteamFlowRate();
      if (val) {
        setMaximumHotWaterorSteamFlowRate(val.get());
      }
    }

    ComponentType AirTerminalSingleDuctConstantVolumeReheat_Impl::componentType() const {
      ComponentType loopType = ComponentType::None;
      if (auto a_ = airLoopHVAC()) {
        loopType = a_->componentType();
      }
      if ((loopType == ComponentType::Both) or (loopType == ComponentType::Cooling)) {
        return ComponentType::Both;
      }

      return ComponentType::Heating;
    }

    std::vector<FuelType> AirTerminalSingleDuctConstantVolumeReheat_Impl::coolingFuelTypes() const {
      if (auto a_ = airLoopHVAC()) {
        return a_->coolingFuelTypes();
      }
      return {};
    }

    std::vector<FuelType> AirTerminalSingleDuctConstantVolumeReheat_Impl::heatingFuelTypes() const {
      std::set<FuelType> result;
      for (auto ft : reheatCoil().heatingFuelTypes()) {
        result.insert(ft);
      }
      if (auto a_ = airLoopHVAC()) {
        for (auto ft : a_->heatingFuelTypes()) {
          result.insert(ft);
        }
      }
      return {result.begin(), result.end()};
    }

    std::vector<AppGFuelType> AirTerminalSingleDuctConstantVolumeReheat_Impl::appGHeatingFuelTypes() const {
      std::set<AppGFuelType> result;
      for (auto ft : reheatCoil().appGHeatingFuelTypes()) {
        result.insert(ft);
      }
      if (auto a_ = airLoopHVAC()) {
        for (auto ft : a_->appGHeatingFuelTypes()) {
          result.insert(ft);
        }
      }
      return {result.begin(), result.end()};
    }

  }  // namespace detail

  AirTerminalSingleDuctConstantVolumeReheat::AirTerminalSingleDuctConstantVolumeReheat(const Model& model, Schedule& availabilitySchedule,
                                                                                       HVACComponent& coil)
    : StraightComponent(AirTerminalSingleDuctConstantVolumeReheat::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>());

    bool test = setAvailabilitySchedule(availabilitySchedule);
    if (!test) {
      remove();
      LOG_AND_THROW("Unable to construct " << briefDescription() << ", because cannot set its " << "availability schedule to "
                                           << availabilitySchedule.briefDescription() << ".");
    }

    this->autosizeMaximumAirFlowRate();

    this->setReheatCoil(coil);

    this->autosizeMaximumHotWaterorSteamFlowRate();

    this->setMinimumHotWaterorSteamFlowRate(0.0);

    this->setConvergenceTolerance(0.001);

    this->setMaximumReheatAirTemperature(35.0);
  }

  IddObjectType AirTerminalSingleDuctConstantVolumeReheat::iddObjectType() {
    return {IddObjectType::OS_AirTerminal_SingleDuct_ConstantVolume_Reheat};
  }

  Schedule AirTerminalSingleDuctConstantVolumeReheat::availabilitySchedule() const {
    return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->availabilitySchedule();
  }

  boost::optional<double> AirTerminalSingleDuctConstantVolumeReheat::maximumAirFlowRate() const {
    return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->maximumAirFlowRate();
  }

  bool AirTerminalSingleDuctConstantVolumeReheat::isMaximumAirFlowRateAutosized() const {
    return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->isMaximumAirFlowRateAutosized();
  }

  HVACComponent AirTerminalSingleDuctConstantVolumeReheat::reheatCoil() const {
    return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->reheatCoil();
  }

  boost::optional<double> AirTerminalSingleDuctConstantVolumeReheat::maximumHotWaterorSteamFlowRate() const {
    return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->maximumHotWaterorSteamFlowRate();
  }

  bool AirTerminalSingleDuctConstantVolumeReheat::isMaximumHotWaterorSteamFlowRateAutosized() const {
    return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->isMaximumHotWaterorSteamFlowRateAutosized();
  }

  double AirTerminalSingleDuctConstantVolumeReheat::minimumHotWaterorSteamFlowRate() const {
    return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->minimumHotWaterorSteamFlowRate();
  }

  bool AirTerminalSingleDuctConstantVolumeReheat::isMinimumHotWaterorSteamFlowRateDefaulted() const {
    return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->isMinimumHotWaterorSteamFlowRateDefaulted();
  }

  double AirTerminalSingleDuctConstantVolumeReheat::convergenceTolerance() const {
    return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->convergenceTolerance();
  }

  bool AirTerminalSingleDuctConstantVolumeReheat::isConvergenceToleranceDefaulted() const {
    return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->isConvergenceToleranceDefaulted();
  }

  double AirTerminalSingleDuctConstantVolumeReheat::maximumReheatAirTemperature() const {
    return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->maximumReheatAirTemperature();
  }

  bool AirTerminalSingleDuctConstantVolumeReheat::isMaximumReheatAirTemperatureDefaulted() const {
    return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->isMaximumReheatAirTemperatureDefaulted();
  }

  bool AirTerminalSingleDuctConstantVolumeReheat::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->setAvailabilitySchedule(schedule);
  }

  bool AirTerminalSingleDuctConstantVolumeReheat::setMaximumAirFlowRate(double maximumAirFlowRate) {
    return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->setMaximumAirFlowRate(maximumAirFlowRate);
  }

  void AirTerminalSingleDuctConstantVolumeReheat::resetMaximumAirFlowRate() {
    getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->resetMaximumAirFlowRate();
  }

  void AirTerminalSingleDuctConstantVolumeReheat::autosizeMaximumAirFlowRate() {
    getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->autosizeMaximumAirFlowRate();
  }

  bool AirTerminalSingleDuctConstantVolumeReheat::setReheatCoil(const HVACComponent& coil) {
    return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->setReheatCoil(coil);
  }

  void AirTerminalSingleDuctConstantVolumeReheat::resetReheatCoil() {
    getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->resetReheatCoil();
  }

  bool AirTerminalSingleDuctConstantVolumeReheat::setMaximumHotWaterorSteamFlowRate(double maximumHotWaterorSteamFlowRate) {
    return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->setMaximumHotWaterorSteamFlowRate(maximumHotWaterorSteamFlowRate);
  }

  void AirTerminalSingleDuctConstantVolumeReheat::resetMaximumHotWaterorSteamFlowRate() {
    getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->resetMaximumHotWaterorSteamFlowRate();
  }

  void AirTerminalSingleDuctConstantVolumeReheat::autosizeMaximumHotWaterorSteamFlowRate() {
    getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->autosizeMaximumHotWaterorSteamFlowRate();
  }

  bool AirTerminalSingleDuctConstantVolumeReheat::setMinimumHotWaterorSteamFlowRate(double minimumHotWaterorSteamFlowRate) {
    return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->setMinimumHotWaterorSteamFlowRate(minimumHotWaterorSteamFlowRate);
  }

  void AirTerminalSingleDuctConstantVolumeReheat::resetMinimumHotWaterorSteamFlowRate() {
    getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->resetMinimumHotWaterorSteamFlowRate();
  }

  bool AirTerminalSingleDuctConstantVolumeReheat::setConvergenceTolerance(double convergenceTolerance) {
    return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->setConvergenceTolerance(convergenceTolerance);
  }

  void AirTerminalSingleDuctConstantVolumeReheat::resetConvergenceTolerance() {
    getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->resetConvergenceTolerance();
  }

  bool AirTerminalSingleDuctConstantVolumeReheat::setMaximumReheatAirTemperature(double maximumReheatAirTemperature) {
    return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->setMaximumReheatAirTemperature(maximumReheatAirTemperature);
  }

  void AirTerminalSingleDuctConstantVolumeReheat::resetMaximumReheatAirTemperature() {
    getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->resetMaximumReheatAirTemperature();
  }

  AirflowNetworkEquivalentDuct AirTerminalSingleDuctConstantVolumeReheat::getAirflowNetworkEquivalentDuct(double length, double diameter) {
    return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->getAirflowNetworkEquivalentDuct(length, diameter);
  }

  boost::optional<AirflowNetworkEquivalentDuct> AirTerminalSingleDuctConstantVolumeReheat::airflowNetworkEquivalentDuct() const {
    return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->airflowNetworkEquivalentDuct();
  }

  /// @cond
  AirTerminalSingleDuctConstantVolumeReheat::AirTerminalSingleDuctConstantVolumeReheat(
    std::shared_ptr<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl> impl)
    : StraightComponent(std::move(impl)) {}
  /// @endcond

  boost::optional<double> AirTerminalSingleDuctConstantVolumeReheat::autosizedMaximumAirFlowRate() const {
    return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->autosizedMaximumAirFlowRate();
  }

  boost::optional<double> AirTerminalSingleDuctConstantVolumeReheat::autosizedMaximumHotWaterorSteamFlowRate() const {
    return getImpl<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl>()->autosizedMaximumHotWaterorSteamFlowRate();
  }

}  // namespace model
}  // namespace openstudio
