/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "AirTerminalSingleDuctVAVReheat.hpp"
#include "AirTerminalSingleDuctVAVReheat_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "PortList.hpp"
#include "PortList_Impl.hpp"
#include "HVACComponent.hpp"
#include "HVACComponent_Impl.hpp"
#include "AirLoopHVACZoneSplitter.hpp"
#include "AirLoopHVACZoneSplitter_Impl.hpp"
#include "AirLoopHVACZoneMixer.hpp"
#include "AirLoopHVACZoneMixer_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Model.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "AirflowNetworkEquivalentDuct.hpp"
#include "AirflowNetworkEquivalentDuct_Impl.hpp"
#include <utilities/idd/OS_AirTerminal_SingleDuct_VAV_Reheat_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Compare.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"
#include "../utilities/core/DeprecatedHelpers.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    AirTerminalSingleDuctVAVReheat_Impl::AirTerminalSingleDuctVAVReheat_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AirTerminalSingleDuctVAVReheat::iddObjectType());
    }

    AirTerminalSingleDuctVAVReheat_Impl::AirTerminalSingleDuctVAVReheat_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                             bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AirTerminalSingleDuctVAVReheat::iddObjectType());
    }

    AirTerminalSingleDuctVAVReheat_Impl::AirTerminalSingleDuctVAVReheat_Impl(const AirTerminalSingleDuctVAVReheat_Impl& other, Model_Impl* model,
                                                                             bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    // Get all output variable names that could be associated with this object.
    const std::vector<std::string>& AirTerminalSingleDuctVAVReheat_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{// These apply to all AirTerminals
                                                   "Zone Air Terminal Sensible Heating Energy",     "Zone Air Terminal Sensible Heating Rate",
                                                   "Zone Air Terminal Sensible Cooling Energy",     "Zone Air Terminal Sensible Cooling Rate",

                                                   "Zone Air Terminal VAV Damper Position",         "Zone Air Terminal Minimum Air Flow Fraction",
                                                   "Zone Air Terminal Outdoor Air Volume Flow Rate"};
      return result;
    }

    IddObjectType AirTerminalSingleDuctVAVReheat_Impl::iddObjectType() const {
      return AirTerminalSingleDuctVAVReheat::iddObjectType();
    }

    std::vector<ScheduleTypeKey> AirTerminalSingleDuctVAVReheat_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_AirTerminal_SingleDuct_VAV_ReheatFields::AvailabilityScheduleName) != e) {
        result.push_back(ScheduleTypeKey("AirTerminalSingleDuctVAVReheat", "Availability"));
      }
      if (std::find(b, e, OS_AirTerminal_SingleDuct_VAV_ReheatFields::MinimumAirFlowFractionScheduleName) != e) {
        result.push_back(ScheduleTypeKey("AirTerminalSingleDuctVAVReheat", "Minimum Air Flow Fraction"));
      }
      if (std::find(b, e, OS_AirTerminal_SingleDuct_VAV_ReheatFields::MinimumAirFlowTurndownScheduleName) != e) {
        result.push_back(ScheduleTypeKey("AirTerminalSingleDuctVAVReheat", "Minimum Air Flow Turndown"));
      }
      return result;
    }

    Schedule AirTerminalSingleDuctVAVReheat_Impl::availabilitySchedule() const {
      boost::optional<Schedule> value = optionalAvailabilitySchedule();
      if (!value) {
        // it is an error if we get here, however we don't want to crash
        // so we hook up to global always on schedule
        LOG(Error, "Required availability schedule not set, using 'Always On' schedule");
        value = this->model().alwaysOnDiscreteSchedule();
        OS_ASSERT(value);
        const_cast<AirTerminalSingleDuctVAVReheat_Impl*>(this)->setAvailabilitySchedule(*value);
        value = optionalAvailabilitySchedule();
      }
      OS_ASSERT(value);
      return value.get();
    }

    bool AirTerminalSingleDuctVAVReheat_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result =
        setSchedule(OS_AirTerminal_SingleDuct_VAV_ReheatFields::AvailabilityScheduleName, "AirTerminalSingleDuctVAVReheat", "Availability", schedule);
      return result;
    }

    unsigned AirTerminalSingleDuctVAVReheat_Impl::inletPort() const {
      return OS_AirTerminal_SingleDuct_VAV_ReheatFields::AirInletNodeName;
    }

    unsigned AirTerminalSingleDuctVAVReheat_Impl::outletPort() const {
      return OS_AirTerminal_SingleDuct_VAV_ReheatFields::AirOutletNodeName;
    }

    bool AirTerminalSingleDuctVAVReheat_Impl::addToNode(Node& node) {
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
                  auto mo = this->getObject<AirTerminalSingleDuctVAVReheat>();

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

    std::vector<IdfObject> AirTerminalSingleDuctVAVReheat_Impl::remove() {
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

    bool AirTerminalSingleDuctVAVReheat_Impl::isRemovable() const {
      return true;
    }

    std::vector<ModelObject> AirTerminalSingleDuctVAVReheat_Impl::children() const {
      std::vector<ModelObject> result;
      if (OptionalHVACComponent intermediate = optionalReheatCoil()) {
        result.push_back(*intermediate);
      }
      std::vector<AirflowNetworkEquivalentDuct> myAFNItems =
        getObject<ModelObject>().getModelObjectSources<AirflowNetworkEquivalentDuct>(AirflowNetworkEquivalentDuct::iddObjectType());
      result.insert(result.end(), myAFNItems.begin(), myAFNItems.end());
      return result;
    }

    ModelObject AirTerminalSingleDuctVAVReheat_Impl::clone(Model model) const {
      auto modelObjectClone = StraightComponent_Impl::clone(model).cast<AirTerminalSingleDuctVAVReheat>();

      HVACComponent coil = this->reheatCoil();

      auto coilClone = coil.clone(model).cast<HVACComponent>();

      modelObjectClone.setReheatCoil(coilClone);

      return std::move(modelObjectClone);
    }

    boost::optional<double> AirTerminalSingleDuctVAVReheat_Impl::maximumAirFlowRate() const {
      return this->getDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumAirFlowRate);
    }

    bool AirTerminalSingleDuctVAVReheat_Impl::setMaximumAirFlowRate(double value) {
      return this->setDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumAirFlowRate, value);
    }

    void AirTerminalSingleDuctVAVReheat_Impl::autosizeMaximumAirFlowRate() {
      this->setString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumAirFlowRate, "AutoSize");
    }

    bool AirTerminalSingleDuctVAVReheat_Impl::isMaximumAirFlowRateAutosized() const {
      boost::optional<std::string> text = this->getString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumAirFlowRate);
      if (text) {
        if (istringEqual(text.get(), "Autocalculate")) {
          return true;
        } else if (istringEqual(text.get(), "AutoSize")) {
          return true;
        }
      }

      return false;
    }

    std::string AirTerminalSingleDuctVAVReheat_Impl::zoneMinimumAirFlowInputMethod() {
      return this->getString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::ZoneMinimumAirFlowInputMethod).get();
    }

    bool AirTerminalSingleDuctVAVReheat_Impl::setZoneMinimumAirFlowInputMethod(const std::string& value) {
      if (istringEqual(value, "Constant")) {
        return this->setString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::ZoneMinimumAirFlowInputMethod, "Constant");
      } else if (istringEqual(value, "FixedFlowRate")) {
        return this->setString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::ZoneMinimumAirFlowInputMethod, "FixedFlowRate");
      } else if (istringEqual(value, "Scheduled")) {
        return this->setString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::ZoneMinimumAirFlowInputMethod, "Scheduled");
      } else {
        return false;
      }
    }

    boost::optional<double> AirTerminalSingleDuctVAVReheat_Impl::constantMinimumAirFlowFraction() const {
      return getDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::ConstantMinimumAirFlowFraction);
    }

    bool AirTerminalSingleDuctVAVReheat_Impl::isConstantMinimumAirFlowFractionAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::ConstantMinimumAirFlowFraction, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    bool AirTerminalSingleDuctVAVReheat_Impl::setConstantMinimumAirFlowFraction(double value) {
      return this->setDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::ConstantMinimumAirFlowFraction, value);
    }

    void AirTerminalSingleDuctVAVReheat_Impl::autosizeConstantMinimumAirFlowFraction() {
      bool result = setString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::ConstantMinimumAirFlowFraction, "Autosize");
      OS_ASSERT(result);
    }

    boost::optional<double> AirTerminalSingleDuctVAVReheat_Impl::fixedMinimumAirFlowRate() const {
      return getDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::FixedMinimumAirFlowRate);
    }

    bool AirTerminalSingleDuctVAVReheat_Impl::isFixedMinimumAirFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::FixedMinimumAirFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    bool AirTerminalSingleDuctVAVReheat_Impl::setFixedMinimumAirFlowRate(double value) {
      return this->setDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::FixedMinimumAirFlowRate, value);
    }

    void AirTerminalSingleDuctVAVReheat_Impl::autosizeFixedMinimumAirFlowRate() {
      bool result = setString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::FixedMinimumAirFlowRate, "Autosize");
      OS_ASSERT(result);
    }

    boost::optional<Schedule> AirTerminalSingleDuctVAVReheat_Impl::minimumAirFlowFractionSchedule() const {
      return this->getObject<AirTerminalSingleDuctVAVReheat>().getModelObjectTarget<Schedule>(
        OS_AirTerminal_SingleDuct_VAV_ReheatFields::MinimumAirFlowFractionScheduleName);
    }

    bool AirTerminalSingleDuctVAVReheat_Impl::setMinimumAirFlowFractionSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MinimumAirFlowFractionScheduleName, "AirTerminalSingleDuctVAVReheat",
                                "Minimum Air Flow Fraction", schedule);
      return result;
    }

    void AirTerminalSingleDuctVAVReheat_Impl::resetMinimumAirFlowFractionSchedule() {
      bool result = setString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MinimumAirFlowFractionScheduleName, "");
      OS_ASSERT(result);
    }

    boost::optional<Schedule> AirTerminalSingleDuctVAVReheat_Impl::minimumAirFlowTurndownSchedule() const {
      return this->getObject<AirTerminalSingleDuctVAVReheat>().getModelObjectTarget<Schedule>(
        OS_AirTerminal_SingleDuct_VAV_ReheatFields::MinimumAirFlowTurndownScheduleName);
    }

    bool AirTerminalSingleDuctVAVReheat_Impl::setMinimumAirFlowTurndownSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MinimumAirFlowTurndownScheduleName, "AirTerminalSingleDuctVAVReheat",
                                "Minimum Air Flow Turndown", schedule);
      return result;
    }

    void AirTerminalSingleDuctVAVReheat_Impl::resetMinimumAirFlowTurndownSchedule() {
      bool result = setString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MinimumAirFlowTurndownScheduleName, "");
      OS_ASSERT(result);
    }

    boost::optional<double> AirTerminalSingleDuctVAVReheat_Impl::maximumHotWaterOrSteamFlowRate() {
      return this->getDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumHotWaterorSteamFlowRate);
    }

    bool AirTerminalSingleDuctVAVReheat_Impl::setMaximumHotWaterOrSteamFlowRate(double value) {
      return this->setDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumHotWaterorSteamFlowRate, value);
    }

    void AirTerminalSingleDuctVAVReheat_Impl::autosizeMaximumHotWaterOrSteamFlowRate() {
      this->setString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumHotWaterorSteamFlowRate, "AutoSize");
    }

    bool AirTerminalSingleDuctVAVReheat_Impl::isMaximumHotWaterOrSteamFlowRateAutosized() const {
      boost::optional<std::string> text = this->getString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumHotWaterorSteamFlowRate);
      if (text) {
        if (istringEqual(text.get(), "Autocalculate")) {
          return true;
        } else if (istringEqual(text.get(), "AutoSize")) {
          return true;
        }
      }

      return false;
    }

    double AirTerminalSingleDuctVAVReheat_Impl::minimumHotWaterOrSteamFlowRate() {
      return this->getDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MinimumHotWaterorSteamFlowRate).get();
    }

    bool AirTerminalSingleDuctVAVReheat_Impl::setMinimumHotWaterOrStreamFlowRate(double value) {
      return this->setDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MinimumHotWaterorSteamFlowRate, value);
    }

    double AirTerminalSingleDuctVAVReheat_Impl::convergenceTolerance() {
      return this->getDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::ConvergenceTolerance).get();
    }

    bool AirTerminalSingleDuctVAVReheat_Impl::setConvergenceTolerance(double value) {
      return this->setDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::ConvergenceTolerance, value);
    }

    std::string AirTerminalSingleDuctVAVReheat_Impl::damperHeatingAction() {
      return this->getString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::DamperHeatingAction).get();
    }

    bool AirTerminalSingleDuctVAVReheat_Impl::setDamperHeatingAction(const std::string& value) {
      return setString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::DamperHeatingAction, value);
      ;
    }

    boost::optional<double> AirTerminalSingleDuctVAVReheat_Impl::maximumFlowPerZoneFloorAreaDuringReheat() {
      return this->getDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumFlowperZoneFloorAreaDuringReheat);
    }

    void AirTerminalSingleDuctVAVReheat_Impl::resetMaximumFlowPerZoneFloorAreaDuringReheat() {
      bool result = setString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumFlowperZoneFloorAreaDuringReheat, "");
      OS_ASSERT(result);
    }

    bool AirTerminalSingleDuctVAVReheat_Impl::setMaximumFlowPerZoneFloorAreaDuringReheat(double value) {
      return this->setDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumFlowperZoneFloorAreaDuringReheat, value);
    }

    void AirTerminalSingleDuctVAVReheat_Impl::autosizeMaximumFlowPerZoneFloorAreaDuringReheat() {
      this->setString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumFlowperZoneFloorAreaDuringReheat, "AutoSize");
    }

    bool AirTerminalSingleDuctVAVReheat_Impl::isMaximumFlowPerZoneFloorAreaDuringReheatAutosized() const {
      boost::optional<std::string> text = this->getString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumFlowperZoneFloorAreaDuringReheat);
      if (text) {
        if (istringEqual(text.get(), "Autocalculate")) {
          return true;
        } else if (istringEqual(text.get(), "AutoSize")) {
          return true;
        }
      }

      return false;
    }

    boost::optional<double> AirTerminalSingleDuctVAVReheat_Impl::maximumFlowFractionDuringReheat() {
      return this->getDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumFlowFractionDuringReheat);
    }

    bool AirTerminalSingleDuctVAVReheat_Impl::setMaximumFlowFractionDuringReheat(double value) {
      return this->setDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumFlowFractionDuringReheat, value);
    }

    void AirTerminalSingleDuctVAVReheat_Impl::autosizeMaximumFlowFractionDuringReheat() {
      this->setString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumFlowFractionDuringReheat, "AutoSize");
    }

    bool AirTerminalSingleDuctVAVReheat_Impl::isMaximumFlowFractionDuringReheatAutosized() const {
      boost::optional<std::string> text = this->getString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumFlowFractionDuringReheat);
      if (text) {
        if (istringEqual(text.get(), "Autocalculate")) {
          return true;
        } else if (istringEqual(text.get(), "AutoSize")) {
          return true;
        }
      }

      return false;
    }

    void AirTerminalSingleDuctVAVReheat_Impl::resetMaximumFlowFractionDuringReheat() {
      this->setString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumFlowFractionDuringReheat, "");
    }

    double AirTerminalSingleDuctVAVReheat_Impl::maximumReheatAirTemperature() {
      return this->getDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumReheatAirTemperature).get();
    }

    bool AirTerminalSingleDuctVAVReheat_Impl::setMaximumReheatAirTemperature(double value) {
      return this->setDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumReheatAirTemperature, value);
    }

    HVACComponent AirTerminalSingleDuctVAVReheat_Impl::reheatCoil() const {
      boost::optional<HVACComponent> coil = optionalReheatCoil();
      OS_ASSERT(coil);
      return coil.get();
    }

    bool AirTerminalSingleDuctVAVReheat_Impl::setReheatCoil(HVACComponent& coil) {
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
        result = this->setPointer(OS_AirTerminal_SingleDuct_VAV_ReheatFields::ReheatCoilName, coil.handle());
      }

      return result;
    }

    boost::optional<HVACComponent> AirTerminalSingleDuctVAVReheat_Impl::optionalReheatCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_AirTerminal_SingleDuct_VAV_ReheatFields::ReheatCoilName);
    }

    boost::optional<Schedule> AirTerminalSingleDuctVAVReheat_Impl::optionalAvailabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirTerminal_SingleDuct_VAV_ReheatFields::AvailabilityScheduleName);
    }

    boost::optional<ModelObject> AirTerminalSingleDuctVAVReheat_Impl::availabilityScheduleAsModelObject() const {
      OptionalModelObject result = availabilitySchedule();
      return result;
    }

    boost::optional<ModelObject> AirTerminalSingleDuctVAVReheat_Impl::minimumAirFlowFractionScheduleAsModelObject() const {
      OptionalModelObject result;
      OptionalSchedule intermediate = minimumAirFlowFractionSchedule();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    boost::optional<ModelObject> AirTerminalSingleDuctVAVReheat_Impl::minimumAirFlowTurndownScheduleAsModelObject() const {
      OptionalModelObject result;
      OptionalSchedule intermediate = minimumAirFlowTurndownSchedule();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    bool AirTerminalSingleDuctVAVReheat_Impl::setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
        if (intermediate) {
          Schedule schedule(*intermediate);
          return setAvailabilitySchedule(schedule);
        }
      }
      return false;
    }

    bool AirTerminalSingleDuctVAVReheat_Impl::setMinimumAirFlowFractionScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
        if (intermediate) {
          Schedule schedule(*intermediate);
          return setMinimumAirFlowFractionSchedule(schedule);
        } else {
          return false;
        }
      } else {
        resetMinimumAirFlowFractionSchedule();
      }
      return true;
    }

    bool AirTerminalSingleDuctVAVReheat_Impl::setMinimumAirFlowTurndownScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
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

    bool AirTerminalSingleDuctVAVReheat_Impl::controlForOutdoorAir() const {
      return getBooleanFieldValue(OS_AirTerminal_SingleDuct_VAV_ReheatFields::ControlForOutdoorAir);
    }

    bool AirTerminalSingleDuctVAVReheat_Impl::setControlForOutdoorAir(bool controlForOutdoorAir) {
      return setBooleanFieldValue(OS_AirTerminal_SingleDuct_VAV_ReheatFields::ControlForOutdoorAir, controlForOutdoorAir);
      ;
    }

    AirflowNetworkEquivalentDuct AirTerminalSingleDuctVAVReheat_Impl::getAirflowNetworkEquivalentDuct(double length, double diameter) {
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

    boost::optional<AirflowNetworkEquivalentDuct> AirTerminalSingleDuctVAVReheat_Impl::airflowNetworkEquivalentDuct() const {
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

    boost::optional<double> AirTerminalSingleDuctVAVReheat_Impl::autosizedMaximumAirFlowRate() const {
      return getAutosizedValue("Design Size Maximum Air Flow Rate", "m3/s");
    }

    boost::optional<double> AirTerminalSingleDuctVAVReheat_Impl::autosizedConstantMinimumAirFlowFraction() const {
      return getAutosizedValue("Design Size Constant Minimum Air Flow Fraction", "");
    }

    boost::optional<double> AirTerminalSingleDuctVAVReheat_Impl::autosizedFixedMinimumAirFlowRate() const {
      return getAutosizedValue("Design Size Minimum Air Flow Rate", "m3/s");
    }

    boost::optional<double> AirTerminalSingleDuctVAVReheat_Impl::autosizedMaximumHotWaterOrSteamFlowRate() const {
      return getAutosizedValue("Design Size Maximum Reheat Water Flow Rate", "m3/s");
    }

    boost::optional<double> AirTerminalSingleDuctVAVReheat_Impl::autosizedMaximumFlowPerZoneFloorAreaDuringReheat() const {
      return getAutosizedValue("Design Size Maximum Flow per Zone Floor Area during Reheat", "m3/s-m2");
    }

    boost::optional<double> AirTerminalSingleDuctVAVReheat_Impl::autosizedMaximumFlowFractionDuringReheat() const {
      return getAutosizedValue("Design Size Maximum Flow Fraction during Reheat", "");
    }

    void AirTerminalSingleDuctVAVReheat_Impl::autosize() {
      autosizeMaximumAirFlowRate();
      autosizeConstantMinimumAirFlowFraction();
      autosizeFixedMinimumAirFlowRate();
      autosizeMaximumHotWaterOrSteamFlowRate();
      autosizeMaximumFlowPerZoneFloorAreaDuringReheat();
      autosizeMaximumFlowFractionDuringReheat();
    }

    void AirTerminalSingleDuctVAVReheat_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedMaximumAirFlowRate();
      if (val) {
        setMaximumAirFlowRate(val.get());
      }

      val = autosizedConstantMinimumAirFlowFraction();
      if (val) {
        setConstantMinimumAirFlowFraction(val.get());
      }

      val = autosizedFixedMinimumAirFlowRate();
      if (val) {
        setFixedMinimumAirFlowRate(val.get());
      }

      val = autosizedMaximumHotWaterOrSteamFlowRate();
      if (val) {
        setMaximumHotWaterOrSteamFlowRate(val.get());
      }

      val = autosizedMaximumFlowPerZoneFloorAreaDuringReheat();
      if (val) {
        setMaximumFlowPerZoneFloorAreaDuringReheat(val.get());
      }

      val = autosizedMaximumFlowFractionDuringReheat();
      if (val) {
        setMaximumFlowFractionDuringReheat(val.get());
      }
    }

    ComponentType AirTerminalSingleDuctVAVReheat_Impl::componentType() const {
      ComponentType loopType = ComponentType::None;
      if (auto a_ = airLoopHVAC()) {
        loopType = a_->componentType();
      }
      if ((loopType == ComponentType::Both) or (loopType == ComponentType::Cooling)) {
        return ComponentType::Both;
      }

      return ComponentType::Heating;
    }

    std::vector<FuelType> AirTerminalSingleDuctVAVReheat_Impl::coolingFuelTypes() const {
      if (auto a_ = airLoopHVAC()) {
        return a_->coolingFuelTypes();
      }
      return {};
    }

    std::vector<FuelType> AirTerminalSingleDuctVAVReheat_Impl::heatingFuelTypes() const {
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

    std::vector<AppGFuelType> AirTerminalSingleDuctVAVReheat_Impl::appGHeatingFuelTypes() const {
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

  AirTerminalSingleDuctVAVReheat::AirTerminalSingleDuctVAVReheat(const Model& model, Schedule& availabilitySchedule, HVACComponent& coil)
    : StraightComponent(AirTerminalSingleDuctVAVReheat::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>());

    bool test = setAvailabilitySchedule(availabilitySchedule);
    if (!test) {
      remove();
      LOG_AND_THROW("Unable to construct " << briefDescription() << ", because cannot set its " << "availability schedule to "
                                           << availabilitySchedule.briefDescription() << ".");
    }

    setReheatCoil(coil);

    autosizeMaximumAirFlowRate();

    setZoneMinimumAirFlowInputMethod("Constant");

    setConstantMinimumAirFlowFraction(0.3);

    setFixedMinimumAirFlowRate(0.0);

    setMaximumFlowPerZoneFloorAreaDuringReheat(0.0);

    setMinimumHotWaterOrStreamFlowRate(0.0);

    setConvergenceTolerance(0.001);

    setDamperHeatingAction("Normal");

    autosizeMaximumFlowPerZoneFloorAreaDuringReheat();

    autosizeMaximumFlowFractionDuringReheat();

    setMaximumReheatAirTemperature(35.0);

    autosizeMaximumHotWaterOrSteamFlowRate();

    setControlForOutdoorAir(false);
  }

  Schedule AirTerminalSingleDuctVAVReheat::availabilitySchedule() const {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->availabilitySchedule();
  }

  bool AirTerminalSingleDuctVAVReheat::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->setAvailabilitySchedule(schedule);
  }

  AirTerminalSingleDuctVAVReheat::AirTerminalSingleDuctVAVReheat(std::shared_ptr<detail::AirTerminalSingleDuctVAVReheat_Impl> p)
    : StraightComponent(std::move(p)) {}

  IddObjectType AirTerminalSingleDuctVAVReheat::iddObjectType() {
    return {IddObjectType::OS_AirTerminal_SingleDuct_VAV_Reheat};
  }

  boost::optional<double> AirTerminalSingleDuctVAVReheat::maximumAirFlowRate() const {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->maximumAirFlowRate();
  }

  bool AirTerminalSingleDuctVAVReheat::setMaximumAirFlowRate(double value) {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->setMaximumAirFlowRate(value);
  }

  void AirTerminalSingleDuctVAVReheat::autosizeMaximumAirFlowRate() {
    getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->autosizeMaximumAirFlowRate();
  }

  bool AirTerminalSingleDuctVAVReheat::isMaximumAirFlowRateAutosized() const {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->isMaximumAirFlowRateAutosized();
  }

  std::string AirTerminalSingleDuctVAVReheat::zoneMinimumAirFlowInputMethod() {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->zoneMinimumAirFlowInputMethod();
  }

  std::string AirTerminalSingleDuctVAVReheat::zoneMinimumAirFlowMethod() {
    DEPRECATED_AT_MSG(3, 0, 1, "Use zoneMinimumAirFlowInputMethod instead.");
    return zoneMinimumAirFlowInputMethod();
  }

  bool AirTerminalSingleDuctVAVReheat::setZoneMinimumAirFlowInputMethod(const std::string& value) {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->setZoneMinimumAirFlowInputMethod(value);
  }

  bool AirTerminalSingleDuctVAVReheat::setZoneMinimumAirFlowMethod(const std::string& value) {
    DEPRECATED_AT_MSG(3, 0, 1, "Use setZoneMinimumAirFlowInputMethod instead.");
    return setZoneMinimumAirFlowInputMethod(value);
  }

  boost::optional<double> AirTerminalSingleDuctVAVReheat::constantMinimumAirFlowFraction() const {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->constantMinimumAirFlowFraction();
  }

  bool AirTerminalSingleDuctVAVReheat::isConstantMinimumAirFlowFractionAutosized() const {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->isConstantMinimumAirFlowFractionAutosized();
  }

  bool AirTerminalSingleDuctVAVReheat::setConstantMinimumAirFlowFraction(double value) {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->setConstantMinimumAirFlowFraction(value);
  }

  void AirTerminalSingleDuctVAVReheat::autosizeConstantMinimumAirFlowFraction() {
    getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->autosizeConstantMinimumAirFlowFraction();
  }

  boost::optional<double> AirTerminalSingleDuctVAVReheat::fixedMinimumAirFlowRate() const {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->fixedMinimumAirFlowRate();
  }

  bool AirTerminalSingleDuctVAVReheat::isFixedMinimumAirFlowRateAutosized() const {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->isFixedMinimumAirFlowRateAutosized();
  }

  bool AirTerminalSingleDuctVAVReheat::setFixedMinimumAirFlowRate(double value) {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->setFixedMinimumAirFlowRate(value);
  }

  void AirTerminalSingleDuctVAVReheat::autosizeFixedMinimumAirFlowRate() {
    getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->autosizeFixedMinimumAirFlowRate();
  }

  boost::optional<Schedule> AirTerminalSingleDuctVAVReheat::minimumAirFlowFractionSchedule() const {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->minimumAirFlowFractionSchedule();
  }

  bool AirTerminalSingleDuctVAVReheat::setMinimumAirFlowFractionSchedule(Schedule& schedule) {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->setMinimumAirFlowFractionSchedule(schedule);
  }

  void AirTerminalSingleDuctVAVReheat::resetMinimumAirFlowFractionSchedule() {
    getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->resetMinimumAirFlowFractionSchedule();
  }

  boost::optional<Schedule> AirTerminalSingleDuctVAVReheat::minimumAirFlowTurndownSchedule() const {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->minimumAirFlowTurndownSchedule();
  }

  bool AirTerminalSingleDuctVAVReheat::setMinimumAirFlowTurndownSchedule(Schedule& schedule) {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->setMinimumAirFlowTurndownSchedule(schedule);
  }

  void AirTerminalSingleDuctVAVReheat::resetMinimumAirFlowTurndownSchedule() {
    getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->resetMinimumAirFlowTurndownSchedule();
  }

  boost::optional<double> AirTerminalSingleDuctVAVReheat::maximumHotWaterOrSteamFlowRate() {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->maximumHotWaterOrSteamFlowRate();
  }

  bool AirTerminalSingleDuctVAVReheat::setMaximumHotWaterOrSteamFlowRate(double value) {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->setMaximumHotWaterOrSteamFlowRate(value);
  }

  void AirTerminalSingleDuctVAVReheat::autosizeMaximumHotWaterOrSteamFlowRate() {
    getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->autosizeMaximumHotWaterOrSteamFlowRate();
  }

  bool AirTerminalSingleDuctVAVReheat::isMaximumHotWaterOrSteamFlowRateAutosized() const {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->isMaximumHotWaterOrSteamFlowRateAutosized();
  }

  double AirTerminalSingleDuctVAVReheat::minimumHotWaterOrSteamFlowRate() {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->minimumHotWaterOrSteamFlowRate();
  }

  bool AirTerminalSingleDuctVAVReheat::setMinimumHotWaterOrStreamFlowRate(double value) {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->setMinimumHotWaterOrStreamFlowRate(value);
  }

  double AirTerminalSingleDuctVAVReheat::convergenceTolerance() {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->convergenceTolerance();
  }

  bool AirTerminalSingleDuctVAVReheat::setConvergenceTolerance(double value) {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->setConvergenceTolerance(value);
  }

  std::string AirTerminalSingleDuctVAVReheat::damperHeatingAction() {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->damperHeatingAction();
  }

  bool AirTerminalSingleDuctVAVReheat::setDamperHeatingAction(const std::string& value) {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->setDamperHeatingAction(value);
  }

  boost::optional<double> AirTerminalSingleDuctVAVReheat::maximumFlowPerZoneFloorAreaDuringReheat() {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->maximumFlowPerZoneFloorAreaDuringReheat();
  }

  void AirTerminalSingleDuctVAVReheat::resetMaximumFlowPerZoneFloorAreaDuringReheat() {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->resetMaximumFlowPerZoneFloorAreaDuringReheat();
  }

  bool AirTerminalSingleDuctVAVReheat::setMaximumFlowPerZoneFloorAreaDuringReheat(double value) {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->setMaximumFlowPerZoneFloorAreaDuringReheat(value);
  }

  void AirTerminalSingleDuctVAVReheat::autosizeMaximumFlowPerZoneFloorAreaDuringReheat() {
    getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->autosizeMaximumFlowPerZoneFloorAreaDuringReheat();
  }

  bool AirTerminalSingleDuctVAVReheat::isMaximumFlowPerZoneFloorAreaDuringReheatAutosized() const {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->isMaximumFlowPerZoneFloorAreaDuringReheatAutosized();
  }

  boost::optional<double> AirTerminalSingleDuctVAVReheat::maximumFlowFractionDuringReheat() {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->maximumFlowFractionDuringReheat();
  }

  bool AirTerminalSingleDuctVAVReheat::setMaximumFlowFractionDuringReheat(double value) {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->setMaximumFlowFractionDuringReheat(value);
  }

  void AirTerminalSingleDuctVAVReheat::autosizeMaximumFlowFractionDuringReheat() {
    getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->autosizeMaximumFlowFractionDuringReheat();
  }

  bool AirTerminalSingleDuctVAVReheat::isMaximumFlowFractionDuringReheatAutosized() const {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->isMaximumFlowFractionDuringReheatAutosized();
  }

  void AirTerminalSingleDuctVAVReheat::resetMaximumFlowFractionDuringReheat() {
    getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->resetMaximumFlowFractionDuringReheat();
  }

  double AirTerminalSingleDuctVAVReheat::maximumReheatAirTemperature() {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->maximumReheatAirTemperature();
  }

  bool AirTerminalSingleDuctVAVReheat::setMaximumReheatAirTemperature(double value) {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->setMaximumReheatAirTemperature(value);
  }

  HVACComponent AirTerminalSingleDuctVAVReheat::reheatCoil() const {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->reheatCoil();
  }

  bool AirTerminalSingleDuctVAVReheat::setReheatCoil(HVACComponent& coil) {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->setReheatCoil(coil);
  }

  bool AirTerminalSingleDuctVAVReheat::controlForOutdoorAir() const {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->controlForOutdoorAir();
  }

  bool AirTerminalSingleDuctVAVReheat::setControlForOutdoorAir(bool controlForOutdoorAir) {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->setControlForOutdoorAir(controlForOutdoorAir);
  }

  AirflowNetworkEquivalentDuct AirTerminalSingleDuctVAVReheat::getAirflowNetworkEquivalentDuct(double length, double diameter) {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->getAirflowNetworkEquivalentDuct(length, diameter);
  }

  boost::optional<AirflowNetworkEquivalentDuct> AirTerminalSingleDuctVAVReheat::airflowNetworkEquivalentDuct() const {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->airflowNetworkEquivalentDuct();
  }

  boost::optional<double> AirTerminalSingleDuctVAVReheat::autosizedMaximumAirFlowRate() const {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->autosizedMaximumAirFlowRate();
  }

  boost::optional<double> AirTerminalSingleDuctVAVReheat::autosizedConstantMinimumAirFlowFraction() const {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->autosizedConstantMinimumAirFlowFraction();
  }

  boost::optional<double> AirTerminalSingleDuctVAVReheat::autosizedFixedMinimumAirFlowRate() const {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->autosizedFixedMinimumAirFlowRate();
  }

  boost::optional<double> AirTerminalSingleDuctVAVReheat::autosizedMaximumHotWaterOrSteamFlowRate() const {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->autosizedMaximumHotWaterOrSteamFlowRate();
  }

  boost::optional<double> AirTerminalSingleDuctVAVReheat::autosizedMaximumFlowPerZoneFloorAreaDuringReheat() const {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->autosizedMaximumFlowPerZoneFloorAreaDuringReheat();
  }

  boost::optional<double> AirTerminalSingleDuctVAVReheat::autosizedMaximumFlowFractionDuringReheat() const {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->autosizedMaximumFlowFractionDuringReheat();
  }

}  // namespace model

}  // namespace openstudio
