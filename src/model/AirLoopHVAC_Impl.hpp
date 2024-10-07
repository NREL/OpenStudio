/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRLOOPHVAC_IMPL_HPP
#define MODEL_AIRLOOPHVAC_IMPL_HPP

#include "Loop_Impl.hpp"
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {
namespace model {

  class AirLoopHVAC;
  class Node;
  class AirLoopHVACOutdoorAirSystem;
  class AirLoopHVACDedicatedOutdoorAirSystem;
  class AirLoopHVACZoneSplitter;
  class AirLoopHVACZoneMixer;
  class AirLoopHVACSupplyPlenum;
  class AirLoopHVACReturnPlenum;
  class ThermalZone;
  class PlantLoop;
  class SizingSystem;
  class StraightComponent;
  class AvailabilityManagerScheduled;
  class AvailabilityManager;
  class AvailabilityManagerAssignmentList;

  namespace detail {

    class Model_Impl;

    class MODEL_API AirLoopHVAC_Impl : public Loop_Impl
    {

     public:
      AirLoopHVAC_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirLoopHVAC_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirLoopHVAC_Impl(const AirLoopHVAC_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AirLoopHVAC_Impl() override = default;

      boost::optional<double> designSupplyAirFlowRate() const;

      bool isDesignSupplyAirFlowRateAutosized() const;

      bool setDesignSupplyAirFlowRate(boost::optional<double> designSupplyAirFlowRate);

      void resetDesignSupplyAirFlowRate();

      void autosizeDesignSupplyAirFlowRate();

      double designReturnAirFlowFractionofSupplyAirFlow() const;

      bool setDesignReturnAirFlowFractionofSupplyAirFlow(double designReturnAirFlowFractionofSupplyAirFlow);

      virtual Node supplyInletNode() const override;

      virtual Node supplyOutletNode() const override;

      virtual std::vector<Node> supplyOutletNodes() const override;

      virtual Node demandInletNode() const override;

      virtual std::vector<Node> demandInletNodes() const override;

      virtual Node demandOutletNode() const override;

      boost::optional<Node> outdoorAirNode() const;

      boost::optional<Node> reliefAirNode() const;

      boost::optional<Node> mixedAirNode() const;

      boost::optional<Node> returnAirNode() const;

      std::vector<ModelObject> oaComponents(openstudio::IddObjectType type = openstudio::IddObjectType("Catchall")) const;

      virtual std::vector<ModelObject> components(openstudio::IddObjectType type = openstudio::IddObjectType("Catchall")) const override;

      boost::optional<AirLoopHVACOutdoorAirSystem> airLoopHVACOutdoorAirSystem() const;

      boost::optional<AirLoopHVACDedicatedOutdoorAirSystem> airLoopHVACDedicatedOutdoorAirSystem() const;

      boost::optional<Splitter> supplySplitter() const;

      bool setSupplySplitter(Splitter const& splitter);

      void resetSupplySplitter();

      bool removeSupplySplitter();

      bool removeSupplySplitter(HVACComponent& hvacComponent);

      boost::optional<Node> supplySplitterInletNode() const;

      std::vector<Node> supplySplitterOutletNodes() const;

      AirLoopHVACZoneMixer zoneMixer() const;

      bool setZoneMixer(Mixer const& mixer);

      AirLoopHVACZoneSplitter zoneSplitter() const;

      bool setZoneSplitter(Splitter const& splitter, int path);

      void resetZoneSplitter(int path);

      std::vector<AirLoopHVACZoneSplitter> zoneSplitters() const;

      bool removeBranchForZone(openstudio::model::ThermalZone& thermalZone);

      virtual std::vector<openstudio::IdfObject> remove() override;

      /**
   * This method will clone an AirLoopHVAC with the following rationale:
   * - Handle all non-branch components from both the supply and the demand side
   * - On the demand side branches, place one terminal of each IddObjectType that is present on the original AirLoopHVAC
   * - Clone any SetpointManagers and add them to the correct location
   * - If the supply component that is cloned is connected to a PlantLoop,
   *   we try to the connect the clone to the same PlantLoop by adding a demand branch
   */
      virtual ModelObject clone(Model model) const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual Splitter demandSplitter() const override;

      virtual Mixer demandMixer() const override;

      boost::optional<HVACComponent> supplyFan() const;

      boost::optional<HVACComponent> returnFan() const;

      boost::optional<HVACComponent> reliefFan() const;

      // Helper to enforce that an ATU type (Single or Dual Duct) matches the AirLoopHVAC Type
      bool isTerminalTypeValid(const HVACComponent& airTerminal);

      bool multiAddBranchForZone(ThermalZone& thermalZone);

      bool multiAddBranchForZone(ThermalZone& thermalZone, HVACComponent& airTerminal);

      bool addBranchForZone(ThermalZone& thermalZone);

      bool addBranchForZone(ThermalZone& thermalZone, HVACComponent& airTerminal);

      bool addBranchForZone(ThermalZone& thermalZone, Splitter& splitter, Mixer& mixer, HVACComponent& airTerminal);

      bool addBranchForZone(ThermalZone& thermalZone, Splitter& splitter, Mixer& mixer);

      // TODO: remove?
      //bool addBranchForZoneImpl(openstudio::model::ThermalZone & thermalZone,
      //                          boost::optional<StraightComponent> & optAirTerminal);

      //bool addBranchForZoneImpl(openstudio::model::ThermalZone & thermalZone,
      //                          boost::optional<HVACComponent> & optAirTerminal);

      bool moveBranchForZone(ThermalZone& thermalZone, Splitter& newSplitter);

      bool moveBranchForZone(ThermalZone& thermalZone, Mixer& newMixer);

      bool addBranchForHVACComponent(HVACComponent& airTerminal);

      SizingSystem sizingSystem() const;

      std::vector<ThermalZone> thermalZones() const;

      virtual std::vector<ModelObject> children() const override;

      Schedule availabilitySchedule() const;

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setNightCycleControlType(const std::string& nightCycle);

      std::string nightCycleControlType() const;

      // boost::optional<Schedule> returnAirBypassFlowTemperatureSetpointSchedule() const;

      // bool setReturnAirBypassFlowTemperatureSetpointSchedule(Schedule & temperatureSetpointSchedule);

      // void resetReturnAirBypassFlowTemperatureSetpointSchedule();

      static bool addBranchForZoneImpl(ThermalZone& thermalZone, AirLoopHVAC& airLoopHVAC, Splitter& splitter, Mixer& mixer, bool removeCurrentZones,
                                       boost::optional<HVACComponent>& optAirTerminal);

      static boost::optional<ThermalZone> zoneForLastBranch(Mixer& mixer);

      static boost::optional<HVACComponent> terminalForLastBranch(Mixer& mixer);

      static boost::optional<PlantLoop> plantForAirTerminal(HVACComponent& airTerminal);

      static bool setPlantForAirTerminal(HVACComponent& airTerminal, PlantLoop& plantLoop);

      unsigned supplyOutletPortA() const;

      unsigned supplyOutletPortB() const;

      unsigned supplyInletPort() const;

      unsigned demandInletPortA() const;

      unsigned demandInletPortB() const;

      unsigned demandOutletPort() const;

      static bool addDualDuctTerminalToNode(HVACComponent& terminal, const unsigned inletPortA, const unsigned inletPortB, const unsigned outletPort,
                                            Node& node);

      static bool removeDualDuctTerminalFromAirLoopHVAC(HVACComponent& terminal, const unsigned inletPortA, const unsigned inletPortB,
                                                        const unsigned outletPort);

      bool isDualDuct() const;

      // AVM
      // Impl_only
      virtual AvailabilityManagerAssignmentList availabilityManagerAssignmentList() const override;

      std::vector<AvailabilityManager> availabilityManagers() const;
      bool setAvailabilityManagers(const std::vector<AvailabilityManager>& avms);
      void resetAvailabilityManagers();

      bool addAvailabilityManager(const AvailabilityManager& availabilityManager);
      bool addAvailabilityManager(const AvailabilityManager& availabilityManager, unsigned priority);

      unsigned availabilityManagerPriority(const AvailabilityManager& availabilityManager) const;
      bool setAvailabilityManagerPriority(const AvailabilityManager& availabilityManager, unsigned priority);

      bool removeAvailabilityManager(const AvailabilityManager& avm);
      bool removeAvailabilityManager(unsigned priority);

      boost::optional<double> autosizedDesignSupplyAirFlowRate() const;

      // Not part of the applySizingValues
      boost::optional<double> autosizedSumMinimumHeatingAirFlowRates() const;

      // Not part of the applySizingValues
      boost::optional<double> autosizedSumAirTerminalMaxAirFlowRate() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      std::vector<HVACComponent> terminals() const;

      /**
   * This method creates the basic, barebone, AirLoopHVAC topology:
   * - Supply inlet & oulet nodes,
   * - Demand inlet & outlet nodes,
   * - Demand splitter & mixer,
   * - A demand branch with a node
   * - A Demand branch with a node (Branch Node)
   */
      virtual void createTopology() override;

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

     private:
      REGISTER_LOGGER("openstudio.model.AirLoopHVAC");

      // TODO: Make these const.
      std::vector<ModelObject> supplyOutletNodesAsModelObjects();
      std::vector<ModelObject> demandInletNodesAsModelObjects();
      boost::optional<ModelObject> demandOutletNodeAsModelObject();
      boost::optional<ModelObject> reliefAirNodeAsModelObject();
      boost::optional<ModelObject> zoneMixerAsModelObject();
      boost::optional<ModelObject> zoneSplitterAsModelObject();
    };

  }  // namespace detail

}  // namespace model

}  // namespace openstudio

#endif
