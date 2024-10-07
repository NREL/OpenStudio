/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PLANTLOOP_IMPL_HPP
#define MODEL_PLANTLOOP_IMPL_HPP

#include "Loop_Impl.hpp"

namespace openstudio {

namespace model {

  class Node;
  class HVACComponent;
  class Mixer;
  class Splitter;
  class SizingPlant;
  class PlantEquipmentOperationScheme;
  class PlantEquipmentOperationHeatingLoad;
  class PlantEquipmentOperationCoolingLoad;
  class AvailabilityManager;
  class AvailabilityManagerAssignmentList;

  namespace detail {

    class Model_Impl;

    class MODEL_API PlantLoop_Impl : public Loop_Impl
    {
     public:
      PlantLoop_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      PlantLoop_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      PlantLoop_Impl(const PlantLoop_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~PlantLoop_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      /**
   * This method creates the basic, barebone, PlantLoop topology:
   * - Supply inlet & oulet nodes,
   * - Supply splitter & mixer,
   * - A Suply branch with a node (Connector Node)
   * - Demand inlet & outlet nodes,
   * - Demand splitter & mixer,
   * - A Demand branch with a node (Branch Node)
   */
      virtual void createTopology() override;

      std::string loadDistributionScheme();

      bool setLoadDistributionScheme(const std::string& scheme);

      std::string fluidType();

      bool setFluidType(const std::string& value);

      bool setGlycolConcentration(int glycolConcentration);

      int glycolConcentration() const;

      double maximumLoopTemperature();

      bool setMaximumLoopTemperature(double value);

      double minimumLoopTemperature();

      bool setMinimumLoopTemperature(double value);

      boost::optional<double> maximumLoopFlowRate();

      bool setMaximumLoopFlowRate(double value);

      bool isMaximumLoopFlowRateAutosized();

      void autosizeMaximumLoopFlowRate();

      boost::optional<double> minimumLoopFlowRate();

      bool setMinimumLoopFlowRate(double value);

      bool isMinimumLoopFlowRateAutosized();

      void autosizeMinimumLoopFlowRate();

      boost::optional<double> plantLoopVolume();

      bool setPlantLoopVolume(double value);

      bool isPlantLoopVolumeAutocalculated();

      void autocalculatePlantLoopVolume();

      std::string commonPipeSimulation() const;

      bool isCommonPipeSimulationDefaulted() const;

      bool setCommonPipeSimulation(const std::string& value);

      void resetCommonPipeSimulation();

      boost::optional<PlantEquipmentOperationHeatingLoad> plantEquipmentOperationHeatingLoad() const;

      bool setPlantEquipmentOperationHeatingLoad(const boost::optional<PlantEquipmentOperationHeatingLoad>& plantOperation);

      void resetPlantEquipmentOperationHeatingLoad();

      boost::optional<PlantEquipmentOperationCoolingLoad> plantEquipmentOperationCoolingLoad() const;

      bool setPlantEquipmentOperationCoolingLoad(const boost::optional<PlantEquipmentOperationCoolingLoad>& plantOperation);

      void resetPlantEquipmentOperationCoolingLoad();

      boost::optional<PlantEquipmentOperationScheme> primaryPlantEquipmentOperationScheme() const;

      bool setPrimaryPlantEquipmentOperationScheme(const boost::optional<PlantEquipmentOperationScheme>& plantOperation);

      void resetPrimaryPlantEquipmentOperationScheme();

      bool setPlantEquipmentOperationHeatingLoadSchedule(Schedule&);

      void resetPlantEquipmentOperationHeatingLoadSchedule();

      boost::optional<Schedule> plantEquipmentOperationHeatingLoadSchedule() const;

      bool setPlantEquipmentOperationCoolingLoadSchedule(Schedule&);

      boost::optional<Schedule> plantEquipmentOperationCoolingLoadSchedule() const;

      void resetPlantEquipmentOperationCoolingLoadSchedule();

      bool setPrimaryPlantEquipmentOperationSchemeSchedule(Schedule&);

      void resetPrimaryPlantEquipmentOperationSchemeSchedule();

      boost::optional<Schedule> primaryPlantEquipmentOperationSchemeSchedule() const;

      bool setComponentSetpointOperationSchemeSchedule(Schedule&);

      boost::optional<Schedule> componentSetpointOperationSchemeSchedule() const;

      void resetComponentSetpointOperationSchemeSchedule();

      virtual std::vector<openstudio::IdfObject> remove() override;

      /**
   * This method will clone a Plant Loop with the following rationale:
   * - Handle all non-branch components from both the supply and the demand side
   * - Handle branch components on the supply side (between supply splitter and mixer)
   * - Clone any SetpointManagers and add them to the correct location
   * - If the component that is cloned is connected to another PlantLoop, we try to connect the clone
   *   to the same other PlantLoop (if comp is on the supply side, we add a demand branch to the other plantloop)
   */
      virtual ModelObject clone(Model model) const override;

      unsigned supplyInletPort() const;

      unsigned supplyOutletPort() const;

      unsigned demandInletPort() const;

      unsigned demandOutletPort() const;

      virtual Node supplyInletNode() const override;

      virtual Node supplyOutletNode() const override;

      virtual std::vector<Node> supplyOutletNodes() const override;

      virtual Node demandInletNode() const override;

      virtual std::vector<Node> demandInletNodes() const override;

      virtual Node demandOutletNode() const override;

      Mixer supplyMixer() const;

      bool setSupplyMixer(Mixer const& mixer);

      Splitter supplySplitter() const;

      bool setSupplySplitter(Splitter const& splitter);

      virtual Mixer demandMixer() const override;

      bool setDemandMixer(Mixer const& mixer);

      virtual Splitter demandSplitter() const override;

      bool setDemandSplitter(Splitter const& splitter);

      bool addSupplyBranchForComponent(HVACComponent component);

      bool removeSupplyBranchWithComponent(HVACComponent hvacComponent);

      bool addDemandBranchForComponent(HVACComponent component, bool tertiary = false);

      bool removeDemandBranchWithComponent(HVACComponent hvacComponent);

      Node loopTemperatureSetpointNode();

      bool setLoopTemperatureSetpointNode(Node& node);

      std::vector<ModelObject> children() const override;

      SizingPlant sizingPlant() const;

      boost::optional<double> autosizedMaximumLoopFlowRate() const;

      boost::optional<double> autosizedPlantLoopVolume() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

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

     private:
      bool isDemandBranchEmpty();

      bool removeBranchWithComponent(HVACComponent component, Splitter splitter, Mixer mixer, bool isSupplyComponent);

      REGISTER_LOGGER("openstudio.model.PlantLoop");
    };

  }  // namespace detail

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_PLANTLOOP_IMPL_HPP
