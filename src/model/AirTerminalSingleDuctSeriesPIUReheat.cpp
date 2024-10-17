/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "AirTerminalSingleDuctSeriesPIUReheat.hpp"
#include "AirTerminalSingleDuctSeriesPIUReheat_Impl.hpp"
#include "AirLoopHVACReturnPlenum.hpp"
#include "AirLoopHVACReturnPlenum_Impl.hpp"
#include "FanVariableVolume.hpp"
#include "FanVariableVolume_Impl.hpp"
#include "FanConstantVolume.hpp"
#include "FanConstantVolume_Impl.hpp"
#include "FanOnOff.hpp"
#include "FanOnOff_Impl.hpp"
#include "FanSystemModel.hpp"
#include "FanSystemModel_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "PortList.hpp"
#include "PortList_Impl.hpp"
#include "Mixer.hpp"
#include "Mixer_Impl.hpp"
#include "Splitter.hpp"
#include "Splitter_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "HVACComponent.hpp"
#include "HVACComponent_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include <utilities/idd/OS_AirTerminal_SingleDuct_SeriesPIU_Reheat_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    AirTerminalSingleDuctSeriesPIUReheat_Impl::AirTerminalSingleDuctSeriesPIUReheat_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                         bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AirTerminalSingleDuctSeriesPIUReheat::iddObjectType());
    }

    AirTerminalSingleDuctSeriesPIUReheat_Impl::AirTerminalSingleDuctSeriesPIUReheat_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                         Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AirTerminalSingleDuctSeriesPIUReheat::iddObjectType());
    }

    AirTerminalSingleDuctSeriesPIUReheat_Impl::AirTerminalSingleDuctSeriesPIUReheat_Impl(const AirTerminalSingleDuctSeriesPIUReheat_Impl& other,
                                                                                         Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& AirTerminalSingleDuctSeriesPIUReheat_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{
        // These apply to all AirTerminals
        "Zone Air Terminal Sensible Heating Energy",
        "Zone Air Terminal Sensible Heating Rate",
        "Zone Air Terminal Sensible Cooling Energy",
        "Zone Air Terminal Sensible Cooling Rate",

        // Specific output for this one
        "Zone Air Terminal Current Operation Control Stage",
        "Zone Air Terminal Heating Energy",
        "Zone Air Terminal Heating Rate",
        "Zone Air Terminal Outdoor Air Volume Flow Rate",
        "Zone Air Terminal Outlet Discharge Air Temperature",
        "Zone Air Terminal Primary Air Mass Flow Rate",
        "Zone Air Terminal Primary Damper Position",
        "Zone Air Terminal Secondary Air Mass Flow Rate",
        "Zone Air Terminal Sensible Cooling Energy",
        "Zone Air Terminal Sensible Cooling Rate",
        "Zone Air Terminal Total Air Mass Flow Rate",
      };
      return result;
    }

    IddObjectType AirTerminalSingleDuctSeriesPIUReheat_Impl::iddObjectType() const {
      return AirTerminalSingleDuctSeriesPIUReheat::iddObjectType();
    }

    std::vector<ScheduleTypeKey> AirTerminalSingleDuctSeriesPIUReheat_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_AirTerminal_SingleDuct_SeriesPIU_ReheatFields::AvailabilityScheduleName) != e) {
        result.push_back(ScheduleTypeKey("AirTerminalSingleDuctSeriesPIUReheat", "Availability"));
      }
      return result;
    }

    boost::optional<Schedule> AirTerminalSingleDuctSeriesPIUReheat_Impl::availabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirTerminal_SingleDuct_SeriesPIU_ReheatFields::AvailabilityScheduleName);
    }

    boost::optional<double> AirTerminalSingleDuctSeriesPIUReheat_Impl::maximumAirFlowRate() const {
      return getDouble(OS_AirTerminal_SingleDuct_SeriesPIU_ReheatFields::MaximumAirFlowRate, true);
    }

    bool AirTerminalSingleDuctSeriesPIUReheat_Impl::isMaximumAirFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_AirTerminal_SingleDuct_SeriesPIU_ReheatFields::MaximumAirFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> AirTerminalSingleDuctSeriesPIUReheat_Impl::maximumPrimaryAirFlowRate() const {
      return getDouble(OS_AirTerminal_SingleDuct_SeriesPIU_ReheatFields::MaximumPrimaryAirFlowRate, true);
    }

    bool AirTerminalSingleDuctSeriesPIUReheat_Impl::isMaximumPrimaryAirFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_AirTerminal_SingleDuct_SeriesPIU_ReheatFields::MaximumPrimaryAirFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> AirTerminalSingleDuctSeriesPIUReheat_Impl::minimumPrimaryAirFlowFraction() const {
      return getDouble(OS_AirTerminal_SingleDuct_SeriesPIU_ReheatFields::MinimumPrimaryAirFlowFraction, true);
    }

    bool AirTerminalSingleDuctSeriesPIUReheat_Impl::isMinimumPrimaryAirFlowFractionAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_AirTerminal_SingleDuct_SeriesPIU_ReheatFields::MinimumPrimaryAirFlowFraction, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    HVACComponent AirTerminalSingleDuctSeriesPIUReheat_Impl::fan() const {
      boost::optional<HVACComponent> value = optionalFan();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Fan attached.");
      }
      return value.get();
    }

    HVACComponent AirTerminalSingleDuctSeriesPIUReheat_Impl::reheatCoil() const {
      boost::optional<HVACComponent> value = optionalReheatCoil();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Reheat Coil attached.");
      }
      return value.get();
    }

    boost::optional<double> AirTerminalSingleDuctSeriesPIUReheat_Impl::maximumHotWaterorSteamFlowRate() const {
      return getDouble(OS_AirTerminal_SingleDuct_SeriesPIU_ReheatFields::MaximumHotWaterorSteamFlowRate, true);
    }

    bool AirTerminalSingleDuctSeriesPIUReheat_Impl::isMaximumHotWaterorSteamFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_AirTerminal_SingleDuct_SeriesPIU_ReheatFields::MaximumHotWaterorSteamFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    double AirTerminalSingleDuctSeriesPIUReheat_Impl::minimumHotWaterorSteamFlowRate() const {
      boost::optional<double> value = getDouble(OS_AirTerminal_SingleDuct_SeriesPIU_ReheatFields::MinimumHotWaterorSteamFlowRate, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirTerminalSingleDuctSeriesPIUReheat_Impl::convergenceTolerance() const {
      boost::optional<double> value = getDouble(OS_AirTerminal_SingleDuct_SeriesPIU_ReheatFields::ConvergenceTolerance, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string AirTerminalSingleDuctSeriesPIUReheat_Impl::fanControlType() const {
      boost::optional<std::string> value = getString(OS_AirTerminal_SingleDuct_SeriesPIU_ReheatFields::FanControlType, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirTerminalSingleDuctSeriesPIUReheat_Impl::minimumFanTurnDownRatio() const {
      boost::optional<double> value = getDouble(OS_AirTerminal_SingleDuct_SeriesPIU_ReheatFields::MinimumFanTurnDownRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string AirTerminalSingleDuctSeriesPIUReheat_Impl::heatingControlType() const {
      boost::optional<std::string> value = getString(OS_AirTerminal_SingleDuct_SeriesPIU_ReheatFields::HeatingControlType, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirTerminalSingleDuctSeriesPIUReheat_Impl::designHeatingDischargeAirTemperature() const {
      boost::optional<double> value = getDouble(OS_AirTerminal_SingleDuct_SeriesPIU_ReheatFields::DesignHeatingDischargeAirTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirTerminalSingleDuctSeriesPIUReheat_Impl::highLimitHeatingDischargeAirTemperature() const {
      boost::optional<double> value = getDouble(OS_AirTerminal_SingleDuct_SeriesPIU_ReheatFields::HighLimitHeatingDischargeAirTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirTerminalSingleDuctSeriesPIUReheat_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_AirTerminal_SingleDuct_SeriesPIU_ReheatFields::AvailabilityScheduleName, "AirTerminalSingleDuctSeriesPIUReheat",
                                "Availability", schedule);
      return result;
    }

    void AirTerminalSingleDuctSeriesPIUReheat_Impl::resetAvailabilitySchedule() {
      bool result = setString(OS_AirTerminal_SingleDuct_SeriesPIU_ReheatFields::AvailabilityScheduleName, "");
      OS_ASSERT(result);
    }

    bool AirTerminalSingleDuctSeriesPIUReheat_Impl::setMaximumAirFlowRate(boost::optional<double> maximumAirFlowRate) {
      bool result(false);
      if (maximumAirFlowRate) {
        result = setDouble(OS_AirTerminal_SingleDuct_SeriesPIU_ReheatFields::MaximumAirFlowRate, maximumAirFlowRate.get());
      }
      return result;
    }

    void AirTerminalSingleDuctSeriesPIUReheat_Impl::autosizeMaximumAirFlowRate() {
      bool result = setString(OS_AirTerminal_SingleDuct_SeriesPIU_ReheatFields::MaximumAirFlowRate, "autosize");
      OS_ASSERT(result);
    }

    bool AirTerminalSingleDuctSeriesPIUReheat_Impl::setMaximumPrimaryAirFlowRate(boost::optional<double> maximumPrimaryAirFlowRate) {
      bool result(false);
      if (maximumPrimaryAirFlowRate) {
        result = setDouble(OS_AirTerminal_SingleDuct_SeriesPIU_ReheatFields::MaximumPrimaryAirFlowRate, maximumPrimaryAirFlowRate.get());
      }
      return result;
    }

    void AirTerminalSingleDuctSeriesPIUReheat_Impl::autosizeMaximumPrimaryAirFlowRate() {
      bool result = setString(OS_AirTerminal_SingleDuct_SeriesPIU_ReheatFields::MaximumPrimaryAirFlowRate, "autosize");
      OS_ASSERT(result);
    }

    bool AirTerminalSingleDuctSeriesPIUReheat_Impl::setMinimumPrimaryAirFlowFraction(boost::optional<double> minimumPrimaryAirFlowFraction) {
      bool result(false);
      if (minimumPrimaryAirFlowFraction) {
        result = setDouble(OS_AirTerminal_SingleDuct_SeriesPIU_ReheatFields::MinimumPrimaryAirFlowFraction, minimumPrimaryAirFlowFraction.get());
      }
      return result;
    }

    void AirTerminalSingleDuctSeriesPIUReheat_Impl::autosizeMinimumPrimaryAirFlowFraction() {
      bool result = setString(OS_AirTerminal_SingleDuct_SeriesPIU_ReheatFields::MinimumPrimaryAirFlowFraction, "autosize");
      OS_ASSERT(result);
    }

    bool AirTerminalSingleDuctSeriesPIUReheat_Impl::setFan(const HVACComponent& fan) {
      bool result = setPointer(OS_AirTerminal_SingleDuct_SeriesPIU_ReheatFields::FanName, fan.handle());

      if (result) {
        if (fan.iddObjectType() == IddObjectType::OS_Fan_ConstantVolume) {
          setFanControlType("ConstantSpeed");
        }
      }

      return result;
    }

    bool AirTerminalSingleDuctSeriesPIUReheat_Impl::setReheatCoil(const HVACComponent& coil) {
      bool result = setPointer(OS_AirTerminal_SingleDuct_SeriesPIU_ReheatFields::ReheatCoilName, coil.handle());
      return result;
    }

    bool AirTerminalSingleDuctSeriesPIUReheat_Impl::setMaximumHotWaterorSteamFlowRate(boost::optional<double> maximumHotWaterorSteamFlowRate) {
      bool result(false);
      if (maximumHotWaterorSteamFlowRate) {
        result = setDouble(OS_AirTerminal_SingleDuct_SeriesPIU_ReheatFields::MaximumHotWaterorSteamFlowRate, maximumHotWaterorSteamFlowRate.get());
      } else {
        resetMaximumHotWaterorSteamFlowRate();
        result = true;
      }
      OS_ASSERT(result);
      return result;
    }

    void AirTerminalSingleDuctSeriesPIUReheat_Impl::resetMaximumHotWaterorSteamFlowRate() {
      bool result = setString(OS_AirTerminal_SingleDuct_SeriesPIU_ReheatFields::MaximumHotWaterorSteamFlowRate, "");
      OS_ASSERT(result);
    }

    void AirTerminalSingleDuctSeriesPIUReheat_Impl::autosizeMaximumHotWaterorSteamFlowRate() {
      bool result = setString(OS_AirTerminal_SingleDuct_SeriesPIU_ReheatFields::MaximumHotWaterorSteamFlowRate, "autosize");
      OS_ASSERT(result);
    }

    bool AirTerminalSingleDuctSeriesPIUReheat_Impl::setMinimumHotWaterorSteamFlowRate(double minimumHotWaterorSteamFlowRate) {
      bool result = setDouble(OS_AirTerminal_SingleDuct_SeriesPIU_ReheatFields::MinimumHotWaterorSteamFlowRate, minimumHotWaterorSteamFlowRate);
      return result;
    }

    bool AirTerminalSingleDuctSeriesPIUReheat_Impl::setConvergenceTolerance(double convergenceTolerance) {
      bool result = setDouble(OS_AirTerminal_SingleDuct_SeriesPIU_ReheatFields::ConvergenceTolerance, convergenceTolerance);
      return result;
    }

    bool AirTerminalSingleDuctSeriesPIUReheat_Impl::setFanControlType(const std::string& fanControlType) {
      auto hvacComponent = fan();
      if (hvacComponent.iddObjectType() == IddObjectType::OS_Fan_ConstantVolume) {
        if (istringEqual(fanControlType, "VariableSpeed")) {
          return false;
        }
      }

      bool result = setString(OS_AirTerminal_SingleDuct_SeriesPIU_ReheatFields::FanControlType, fanControlType);
      return result;
    }

    bool AirTerminalSingleDuctSeriesPIUReheat_Impl::setMinimumFanTurnDownRatio(double minimumFanTurnDownRatio) {
      bool result = setDouble(OS_AirTerminal_SingleDuct_SeriesPIU_ReheatFields::MinimumFanTurnDownRatio, minimumFanTurnDownRatio);
      return result;
    }

    bool AirTerminalSingleDuctSeriesPIUReheat_Impl::setHeatingControlType(const std::string& heatingControlType) {
      bool result = setString(OS_AirTerminal_SingleDuct_SeriesPIU_ReheatFields::HeatingControlType, heatingControlType);
      return result;
    }

    bool AirTerminalSingleDuctSeriesPIUReheat_Impl::setDesignHeatingDischargeAirTemperature(double designHeatingDischargeAirTemperature) {
      bool result =
        setDouble(OS_AirTerminal_SingleDuct_SeriesPIU_ReheatFields::DesignHeatingDischargeAirTemperature, designHeatingDischargeAirTemperature);
      return result;
    }

    bool AirTerminalSingleDuctSeriesPIUReheat_Impl::setHighLimitHeatingDischargeAirTemperature(double highLimitHeatingDischargeAirTemperature) {
      bool result =
        setDouble(OS_AirTerminal_SingleDuct_SeriesPIU_ReheatFields::HighLimitHeatingDischargeAirTemperature, highLimitHeatingDischargeAirTemperature);
      return result;
    }

    boost::optional<HVACComponent> AirTerminalSingleDuctSeriesPIUReheat_Impl::optionalFan() const {
      return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_AirTerminal_SingleDuct_SeriesPIU_ReheatFields::FanName);
    }

    boost::optional<HVACComponent> AirTerminalSingleDuctSeriesPIUReheat_Impl::optionalReheatCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_AirTerminal_SingleDuct_SeriesPIU_ReheatFields::ReheatCoilName);
    }

    unsigned AirTerminalSingleDuctSeriesPIUReheat_Impl::inletPort() const {
      return OS_AirTerminal_SingleDuct_SeriesPIU_ReheatFields::SupplyAirInletNode;
    }

    unsigned AirTerminalSingleDuctSeriesPIUReheat_Impl::outletPort() const {
      return OS_AirTerminal_SingleDuct_SeriesPIU_ReheatFields::OutletNode;
    }

    unsigned AirTerminalSingleDuctSeriesPIUReheat_Impl::secondaryAirInletPort() const {
      return OS_AirTerminal_SingleDuct_SeriesPIU_ReheatFields::SecondaryAirInletNode;
    }

    boost::optional<Node> AirTerminalSingleDuctSeriesPIUReheat_Impl::secondaryAirInletNode() const {
      boost::optional<ModelObject> mo = connectedObject(secondaryAirInletPort());

      if (mo) {
        if (boost::optional<Node> node = mo->optionalCast<Node>()) {
          return node;
        }
      }

      return boost::none;
    }

    bool AirTerminalSingleDuctSeriesPIUReheat_Impl::addToNode(Node& node) {
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

                auto thisObject = getObject<ModelObject>();

                _model.connect(sourceModelObject.get(), sourcePort.get(), inletNode, inletNode.inletPort());

                _model.connect(inletNode, inletNode.outletPort(), thisObject, inletPort());

                _model.connect(thisObject, outletPort(), node, node.inletPort());

                if (thermalZone) {
                  Node secondaryInletNode(_model);

                  PortList exhaustPortList = thermalZone->exhaustPortList();

                  _model.connect(exhaustPortList, exhaustPortList.nextPort(), secondaryInletNode, secondaryInletNode.inletPort());

                  _model.connect(secondaryInletNode, secondaryInletNode.outletPort(), thisObject, secondaryAirInletPort());

                  auto mo = this->getObject<ModelObject>();

                  thermalZone->addEquipment(mo);
                }

                if (auto airLoopHVAC = node.airLoopHVAC()) {
                  auto schedule = airLoopHVAC->availabilitySchedule();
                  setFanAvailabilitySchedule(schedule);
                }

                return true;
              }
            }
          }
        }
      }

      return false;
    }

    std::vector<ModelObject> AirTerminalSingleDuctSeriesPIUReheat_Impl::children() const {
      std::vector<ModelObject> result;
      if (OptionalHVACComponent intermediate = optionalReheatCoil()) {
        result.push_back(*intermediate);
      }
      if (OptionalHVACComponent intermediate = optionalFan()) {
        result.push_back(*intermediate);
      }
      return result;
    }

    ModelObject AirTerminalSingleDuctSeriesPIUReheat_Impl::clone(Model model) const {
      auto modelObjectClone = StraightComponent_Impl::clone(model).cast<AirTerminalSingleDuctSeriesPIUReheat>();

      modelObjectClone.setString(modelObjectClone.secondaryAirInletPort(), "");

      // clone coil

      HVACComponent coil = this->reheatCoil();

      auto coilClone = coil.clone(model).cast<HVACComponent>();

      modelObjectClone.setReheatCoil(coilClone);

      // clone fan

      HVACComponent fan = this->fan();

      auto fanClone = fan.clone(model).cast<HVACComponent>();

      modelObjectClone.setFan(fanClone);

      return std::move(modelObjectClone);
    }

    bool AirTerminalSingleDuctSeriesPIUReheat_Impl::setInducedAirPlenumZone(ThermalZone& plenumZone) {
      bool result = true;

      if (!plenumZone.isPlenum()) {
        result = false;
      }

      boost::optional<Node> t_secondaryAirInletNode;
      if (result) {
        t_secondaryAirInletNode = secondaryAirInletNode();
        if (!t_secondaryAirInletNode) {
          result = false;
        }
      }

      boost::optional<AirLoopHVACReturnPlenum> plenum;
      if (result) {
        plenum = plenumZone.getImpl<detail::ThermalZone_Impl>()->airLoopHVACReturnPlenum();
        if (!plenum) {
          result = false;
        }
      }

      if (result) {
        Model t_model = model();
        PortList pl = plenum->getImpl<detail::AirLoopHVACReturnPlenum_Impl>()->inducedAirOutletPortList();
        t_model.connect(pl, pl.nextPort(), t_secondaryAirInletNode.get(), t_secondaryAirInletNode->inletPort());
      }

      return result;
    }

    bool AirTerminalSingleDuctSeriesPIUReheat_Impl::setFanAvailabilitySchedule(Schedule& schedule) {
      auto component = fan();
      // TODO: from E+ 9.2.0 IDD: \note Fan type must be Fan:SystemModel or Fan:ConstantVolume
      if (auto constantFan = component.optionalCast<FanConstantVolume>()) {
        return constantFan->setAvailabilitySchedule(schedule);
      } else if (auto systemModelFan = component.optionalCast<FanSystemModel>()) {
        return systemModelFan->setAvailabilitySchedule(schedule);
      } else if (auto onOffFan = component.optionalCast<FanOnOff>()) {
        return onOffFan->setAvailabilitySchedule(schedule);
      } else if (auto variableFan = component.optionalCast<FanVariableVolume>()) {
        return variableFan->setAvailabilitySchedule(schedule);
      } else {
        // Should never get here!
        LOG(Error, "Unknown assigned Fan Type ('" << component.iddObjectType().valueName() << "') for " << briefDescription());
        return false;
      }
    }

    std::vector<IdfObject> AirTerminalSingleDuctSeriesPIUReheat_Impl::remove() {
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

      if (boost::optional<Node> secondaryAirInletNode = this->secondaryAirInletNode()) {
        secondaryAirInletNode->disconnect();
        secondaryAirInletNode->remove();
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

    boost::optional<double> AirTerminalSingleDuctSeriesPIUReheat_Impl::autosizedMaximumAirFlowRate() const {
      return getAutosizedValue("Design Size Maximum Air Flow Rate", "m3/s");
    }

    boost::optional<double> AirTerminalSingleDuctSeriesPIUReheat_Impl::autosizedMaximumPrimaryAirFlowRate() const {
      return getAutosizedValue("Design Size Maximum Primary Air Flow Rate", "m3/s");
    }

    boost::optional<double> AirTerminalSingleDuctSeriesPIUReheat_Impl::autosizedMinimumPrimaryAirFlowFraction() const {
      return getAutosizedValue("Design Size Minimum Primary Air Flow Fraction", "");
    }

    boost::optional<double> AirTerminalSingleDuctSeriesPIUReheat_Impl::autosizedMaximumHotWaterorSteamFlowRate() const {
      return getAutosizedValue("Design Size Maximum Reheat Water Flow Rate", "m3/s");
    }

    void AirTerminalSingleDuctSeriesPIUReheat_Impl::autosize() {
      autosizeMaximumAirFlowRate();
      autosizeMaximumPrimaryAirFlowRate();
      autosizeMinimumPrimaryAirFlowFraction();
      autosizeMaximumHotWaterorSteamFlowRate();
    }

    void AirTerminalSingleDuctSeriesPIUReheat_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedMaximumAirFlowRate();
      if (val) {
        setMaximumAirFlowRate(val.get());
      }

      val = autosizedMaximumPrimaryAirFlowRate();
      if (val) {
        setMaximumPrimaryAirFlowRate(val.get());
      }

      val = autosizedMinimumPrimaryAirFlowFraction();
      if (val) {
        setMinimumPrimaryAirFlowFraction(val.get());
      }

      val = autosizedMaximumHotWaterorSteamFlowRate();
      if (val) {
        setMaximumHotWaterorSteamFlowRate(val.get());
      }
    }

    ComponentType AirTerminalSingleDuctSeriesPIUReheat_Impl::componentType() const {
      ComponentType loopType = ComponentType::None;
      if (auto a_ = airLoopHVAC()) {
        loopType = a_->componentType();
      }
      if ((loopType == ComponentType::Both) or (loopType == ComponentType::Cooling)) {
        return ComponentType::Both;
      }

      return ComponentType::Heating;
    }

    std::vector<FuelType> AirTerminalSingleDuctSeriesPIUReheat_Impl::coolingFuelTypes() const {
      if (auto a_ = airLoopHVAC()) {
        return a_->coolingFuelTypes();
      }
      return {};
    }

    std::vector<FuelType> AirTerminalSingleDuctSeriesPIUReheat_Impl::heatingFuelTypes() const {
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

    std::vector<AppGFuelType> AirTerminalSingleDuctSeriesPIUReheat_Impl::appGHeatingFuelTypes() const {
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

  AirTerminalSingleDuctSeriesPIUReheat::AirTerminalSingleDuctSeriesPIUReheat(const Model& model, HVACComponent& fan, HVACComponent& reheatCoil)
    : StraightComponent(AirTerminalSingleDuctSeriesPIUReheat::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>());

    bool ok = setFan(fan);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s Fan to " << fan.briefDescription() << ".");
    }
    setReheatCoil(reheatCoil);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s Reheat Coil to " << reheatCoil.briefDescription() << ".");
    }

    autosizeMaximumAirFlowRate();
    autosizeMaximumPrimaryAirFlowRate();
    autosizeMinimumPrimaryAirFlowFraction();
    autosizeMaximumHotWaterorSteamFlowRate();
    setMinimumHotWaterorSteamFlowRate(0.0);
    setConvergenceTolerance(0.001);
    setFanControlType("ConstantSpeed");
    setMinimumFanTurnDownRatio(0.3);
    setHeatingControlType("Staged");
    setDesignHeatingDischargeAirTemperature(32.1);
    setHighLimitHeatingDischargeAirTemperature(37.7);
  }

  IddObjectType AirTerminalSingleDuctSeriesPIUReheat::iddObjectType() {
    return {IddObjectType::OS_AirTerminal_SingleDuct_SeriesPIU_Reheat};
  }

  std::vector<std::string> AirTerminalSingleDuctSeriesPIUReheat::fanControlTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_AirTerminal_SingleDuct_SeriesPIU_ReheatFields::FanControlType);
  }

  std::vector<std::string> AirTerminalSingleDuctSeriesPIUReheat::heatingControlTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_AirTerminal_SingleDuct_SeriesPIU_ReheatFields::HeatingControlType);
  }

  boost::optional<Schedule> AirTerminalSingleDuctSeriesPIUReheat::availabilitySchedule() const {
    return getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->availabilitySchedule();
  }

  boost::optional<double> AirTerminalSingleDuctSeriesPIUReheat::maximumAirFlowRate() const {
    return getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->maximumAirFlowRate();
  }

  bool AirTerminalSingleDuctSeriesPIUReheat::isMaximumAirFlowRateAutosized() const {
    return getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->isMaximumAirFlowRateAutosized();
  }

  boost::optional<double> AirTerminalSingleDuctSeriesPIUReheat::maximumPrimaryAirFlowRate() const {
    return getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->maximumPrimaryAirFlowRate();
  }

  bool AirTerminalSingleDuctSeriesPIUReheat::isMaximumPrimaryAirFlowRateAutosized() const {
    return getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->isMaximumPrimaryAirFlowRateAutosized();
  }

  boost::optional<double> AirTerminalSingleDuctSeriesPIUReheat::minimumPrimaryAirFlowFraction() const {
    return getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->minimumPrimaryAirFlowFraction();
  }

  bool AirTerminalSingleDuctSeriesPIUReheat::isMinimumPrimaryAirFlowFractionAutosized() const {
    return getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->isMinimumPrimaryAirFlowFractionAutosized();
  }

  HVACComponent AirTerminalSingleDuctSeriesPIUReheat::fan() const {
    return getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->fan();
  }

  HVACComponent AirTerminalSingleDuctSeriesPIUReheat::reheatCoil() const {
    return getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->reheatCoil();
  }

  boost::optional<double> AirTerminalSingleDuctSeriesPIUReheat::maximumHotWaterorSteamFlowRate() const {
    return getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->maximumHotWaterorSteamFlowRate();
  }

  bool AirTerminalSingleDuctSeriesPIUReheat::isMaximumHotWaterorSteamFlowRateAutosized() const {
    return getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->isMaximumHotWaterorSteamFlowRateAutosized();
  }

  double AirTerminalSingleDuctSeriesPIUReheat::minimumHotWaterorSteamFlowRate() const {
    return getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->minimumHotWaterorSteamFlowRate();
  }

  double AirTerminalSingleDuctSeriesPIUReheat::convergenceTolerance() const {
    return getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->convergenceTolerance();
  }

  std::string AirTerminalSingleDuctSeriesPIUReheat::fanControlType() const {
    return getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->fanControlType();
  }

  double AirTerminalSingleDuctSeriesPIUReheat::minimumFanTurnDownRatio() const {
    return getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->minimumFanTurnDownRatio();
  }

  std::string AirTerminalSingleDuctSeriesPIUReheat::heatingControlType() const {
    return getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->heatingControlType();
  }

  double AirTerminalSingleDuctSeriesPIUReheat::designHeatingDischargeAirTemperature() const {
    return getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->designHeatingDischargeAirTemperature();
  }

  double AirTerminalSingleDuctSeriesPIUReheat::highLimitHeatingDischargeAirTemperature() const {
    return getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->highLimitHeatingDischargeAirTemperature();
  }

  bool AirTerminalSingleDuctSeriesPIUReheat::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->setAvailabilitySchedule(schedule);
  }

  void AirTerminalSingleDuctSeriesPIUReheat::resetAvailabilitySchedule() {
    getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->resetAvailabilitySchedule();
  }

  bool AirTerminalSingleDuctSeriesPIUReheat::setMaximumAirFlowRate(double maximumAirFlowRate) {
    return getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->setMaximumAirFlowRate(maximumAirFlowRate);
  }

  void AirTerminalSingleDuctSeriesPIUReheat::autosizeMaximumAirFlowRate() {
    getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->autosizeMaximumAirFlowRate();
  }

  bool AirTerminalSingleDuctSeriesPIUReheat::setMaximumPrimaryAirFlowRate(double maximumPrimaryAirFlowRate) {
    return getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->setMaximumPrimaryAirFlowRate(maximumPrimaryAirFlowRate);
  }

  void AirTerminalSingleDuctSeriesPIUReheat::autosizeMaximumPrimaryAirFlowRate() {
    getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->autosizeMaximumPrimaryAirFlowRate();
  }

  bool AirTerminalSingleDuctSeriesPIUReheat::setMinimumPrimaryAirFlowFraction(double minimumPrimaryAirFlowFraction) {
    return getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->setMinimumPrimaryAirFlowFraction(minimumPrimaryAirFlowFraction);
  }

  void AirTerminalSingleDuctSeriesPIUReheat::autosizeMinimumPrimaryAirFlowFraction() {
    getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->autosizeMinimumPrimaryAirFlowFraction();
  }

  bool AirTerminalSingleDuctSeriesPIUReheat::setFan(const HVACComponent& fan) {
    return getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->setFan(fan);
  }

  bool AirTerminalSingleDuctSeriesPIUReheat::setReheatCoil(const HVACComponent& coil) {
    return getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->setReheatCoil(coil);
  }

  bool AirTerminalSingleDuctSeriesPIUReheat::setMaximumHotWaterorSteamFlowRate(double maximumHotWaterorSteamFlowRate) {
    return getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->setMaximumHotWaterorSteamFlowRate(maximumHotWaterorSteamFlowRate);
  }

  void AirTerminalSingleDuctSeriesPIUReheat::resetMaximumHotWaterorSteamFlowRate() {
    getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->resetMaximumHotWaterorSteamFlowRate();
  }

  void AirTerminalSingleDuctSeriesPIUReheat::autosizeMaximumHotWaterorSteamFlowRate() {
    getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->autosizeMaximumHotWaterorSteamFlowRate();
  }

  bool AirTerminalSingleDuctSeriesPIUReheat::setMinimumHotWaterorSteamFlowRate(double minimumHotWaterorSteamFlowRate) {
    return getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->setMinimumHotWaterorSteamFlowRate(minimumHotWaterorSteamFlowRate);
  }

  bool AirTerminalSingleDuctSeriesPIUReheat::setConvergenceTolerance(double convergenceTolerance) {
    return getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->setConvergenceTolerance(convergenceTolerance);
  }

  bool AirTerminalSingleDuctSeriesPIUReheat::setFanControlType(const std::string& fanControlType) {
    return getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->setFanControlType(fanControlType);
  }

  bool AirTerminalSingleDuctSeriesPIUReheat::setMinimumFanTurnDownRatio(double minimumFanTurnDownRatio) {
    return getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->setMinimumFanTurnDownRatio(minimumFanTurnDownRatio);
  }

  bool AirTerminalSingleDuctSeriesPIUReheat::setHeatingControlType(const std::string& heatingControlType) {
    return getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->setHeatingControlType(heatingControlType);
  }

  bool AirTerminalSingleDuctSeriesPIUReheat::setDesignHeatingDischargeAirTemperature(double designHeatingDischargeAirTemperature) {
    return getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->setDesignHeatingDischargeAirTemperature(
      designHeatingDischargeAirTemperature);
  }

  bool AirTerminalSingleDuctSeriesPIUReheat::setHighLimitHeatingDischargeAirTemperature(double highLimitHeatingDischargeAirTemperature) {
    return getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->setHighLimitHeatingDischargeAirTemperature(
      highLimitHeatingDischargeAirTemperature);
  }

  unsigned AirTerminalSingleDuctSeriesPIUReheat::secondaryAirInletPort() const {
    return getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->secondaryAirInletPort();
  }

  boost::optional<Node> AirTerminalSingleDuctSeriesPIUReheat::secondaryAirInletNode() const {
    return getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->secondaryAirInletNode();
  }

  /// @cond
  AirTerminalSingleDuctSeriesPIUReheat::AirTerminalSingleDuctSeriesPIUReheat(std::shared_ptr<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl> impl)
    : StraightComponent(std::move(impl)) {}
  /// @endcond

  boost::optional<double> AirTerminalSingleDuctSeriesPIUReheat::autosizedMaximumAirFlowRate() const {
    return getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->autosizedMaximumAirFlowRate();
  }

  boost::optional<double> AirTerminalSingleDuctSeriesPIUReheat::autosizedMaximumPrimaryAirFlowRate() const {
    return getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->autosizedMaximumPrimaryAirFlowRate();
  }

  boost::optional<double> AirTerminalSingleDuctSeriesPIUReheat::autosizedMinimumPrimaryAirFlowFraction() const {
    return getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->autosizedMinimumPrimaryAirFlowFraction();
  }

  boost::optional<double> AirTerminalSingleDuctSeriesPIUReheat::autosizedMaximumHotWaterorSteamFlowRate() const {
    return getImpl<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->autosizedMaximumHotWaterorSteamFlowRate();
  }

}  // namespace model
}  // namespace openstudio
