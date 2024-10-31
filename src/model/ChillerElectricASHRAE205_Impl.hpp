/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CHILLERELECTRICASHRAE205_IMPL_HPP
#define MODEL_CHILLERELECTRICASHRAE205_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "WaterToWaterComponent_Impl.hpp"

namespace openstudio {

namespace model {

  class ExternalFile;
  class Node;
  class ThermalZone;
  class Schedule;

  namespace detail {

    /** ChillerElectricASHRAE205_Impl is a WaterToWaterComponent_Impl that is the implementation class for ChillerElectricASHRAE205.*/
    class MODEL_API ChillerElectricASHRAE205_Impl : public WaterToWaterComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ChillerElectricASHRAE205_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ChillerElectricASHRAE205_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ChillerElectricASHRAE205_Impl(const ChillerElectricASHRAE205_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ChillerElectricASHRAE205_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      // chilledWaterLoop
      virtual boost::optional<PlantLoop> plantLoop() const override;
      virtual unsigned supplyInletPort() const override;
      virtual unsigned supplyOutletPort() const override;

      // condenserWaterLoop
      virtual boost::optional<PlantLoop> secondaryPlantLoop() const override;
      virtual unsigned demandInletPort() const override;
      virtual unsigned demandOutletPort() const override;

      // heatRecoveryLoop
      virtual boost::optional<PlantLoop> tertiaryPlantLoop() const override;
      virtual unsigned tertiaryInletPort() const override;
      virtual unsigned tertiaryOutletPort() const override;

      virtual std::vector<HVACComponent> edges(const boost::optional<HVACComponent>& prev) override;

      /** This function will perform a check if trying to add it to a node that is on the demand side of a plant loop.
       * If:
       *     - the node is on the demand side of a loop
       *     - the node isn't on the current condenser water loop itself
       *     - the chiller doesn't already have a heat recovery (tertiary) loop,
       * then it tries to add it to the Tertiary loop.
       * In all other cases, it will call the base class' method WaterToWaterComponent_Impl::addToNode()
       * If this is connecting to the demand side of a loop (not tertiary), will set the chiller condenserType to WaterCooled
       */
      virtual bool addToNode(Node& node) override;

      /* Restricts addToTertiaryNode to a node that is on the demand side of a plant loop (tertiary = Heat Recovery Loop) */
      virtual bool addToTertiaryNode(Node& node) override;

      virtual ModelObject clone(Model model) const override;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      ExternalFile representationFile() const;

      std::string performanceInterpolationMethod() const;

      boost::optional<double> ratedCapacity() const;
      bool isRatedCapacityAutosized() const;

      double sizingFactor() const;

      std::string ambientTemperatureIndicator() const;

      boost::optional<Schedule> ambientTemperatureSchedule() const;

      boost::optional<ThermalZone> ambientTemperatureZone() const;

      boost::optional<std::string> ambientTemperatureOutdoorAirNodeName() const;

      boost::optional<double> chilledWaterMaximumRequestedFlowRate() const;
      bool isChilledWaterMaximumRequestedFlowRateAutosized() const;

      boost::optional<double> condenserMaximumRequestedFlowRate() const;
      bool isCondenserMaximumRequestedFlowRateAutosized() const;

      std::string chillerFlowMode() const;

      boost::optional<double> oilCoolerDesignFlowRate() const;

      boost::optional<double> auxiliaryCoolingDesignFlowRate() const;

      std::string endUseSubcategory() const;
      bool isEndUseSubcategoryDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setRepresentationFile(const ExternalFile& externalFile);

      bool setPerformanceInterpolationMethod(const std::string& performanceInterpolationMethod);

      bool setRatedCapacity(double ratedCapacity);
      void autosizeRatedCapacity();

      bool setSizingFactor(double sizingFactor);

      bool setAmbientTemperatureIndicator(const std::string& ambientTemperatureIndicator);

      bool setAmbientTemperatureSchedule(Schedule& schedule);
      void resetAmbientTemperatureSchedule();

      bool setAmbientTemperatureZone(const ThermalZone& thermalZone);
      void resetAmbientTemperatureZone();

      bool setAmbientTemperatureOutdoorAirNodeName(const std::string& ambientTemperatureOutdoorAirNodeName);
      void resetAmbientTemperatureOutdoorAirNodeName();

      bool setChilledWaterMaximumRequestedFlowRate(double chilledWaterMaximumRequestedFlowRate);
      void autosizeChilledWaterMaximumRequestedFlowRate();

      bool setCondenserMaximumRequestedFlowRate(double condenserMaximumRequestedFlowRate);
      void autosizeCondenserMaximumRequestedFlowRate();

      bool setChillerFlowMode(const std::string& chillerFlowMode);

      bool setOilCoolerDesignFlowRate(double oilCoolerDesignFlowRate);
      void resetOilCoolerDesignFlowRate();

      bool setAuxiliaryCoolingDesignFlowRate(double auxiliaryCoolingDesignFlowRate);
      void resetAuxiliaryCoolingDesignFlowRate();

      bool setEndUseSubcategory(const std::string& endUseSubcategory);
      void resetEndUseSubcategory();

      //@}
      /** @name Other */
      //@{

      boost::optional<double> autosizedRatedCapacity();

      boost::optional<double> autosizedChilledWaterMaximumRequestedFlowRate();

      boost::optional<double> autosizedCondenserMaximumRequestedFlowRate();

      /** Convenience Function to return the Chilled Water Loop (chiller on supply) **/
      boost::optional<PlantLoop> chilledWaterLoop() const;
      // Same as supplyInletModelObject, but cast to a Node
      boost::optional<Node> chilledWaterInletNode() const;
      boost::optional<Node> chilledWaterOutletNode() const;

      /** Convenience Function to return the Condenser Water Loop (chiller on demand side) **/
      boost::optional<PlantLoop> condenserWaterLoop() const;
      boost::optional<Node> condenserInletNode() const;
      boost::optional<Node> condenserOutletNode() const;

      // Not implemented in E+ 22.2.0, but reserved as a Tertiary Loop for now
      boost::optional<PlantLoop> heatRecoveryLoop() const;
      boost::optional<Node> heatRecoveryInletNode() const;
      boost::optional<Node> heatRecoveryOutletNode() const;

      /*static*/ unsigned oilCoolerInletPort() const;
      boost::optional<ModelObject> oilCoolerInletModelObject() const;
      /*static*/ unsigned oilCoolerOutletPort() const;
      boost::optional<ModelObject> oilCoolerOutletModelObject() const;
      boost::optional<PlantLoop> oilCoolerLoop() const;
      bool addDemandBranchOnOilCoolerLoop(PlantLoop& plantLoop);
      bool addToOilCoolerLoopNode(Node& node);
      bool removeFromOilCoolerLoop();
      boost::optional<Node> oilCoolerInletNode() const;
      boost::optional<Node> oilCoolerOutletNode() const;

      /*static*/ unsigned auxiliaryInletPort() const;
      boost::optional<ModelObject> auxiliaryInletModelObject() const;
      /*static*/ unsigned auxiliaryOutletPort() const;
      boost::optional<ModelObject> auxiliaryOutletModelObject() const;
      bool addDemandBranchOnAuxiliaryLoop(PlantLoop& plantLoop);
      bool addToAuxiliaryLoopNode(Node& node);
      bool removeFromAuxiliaryLoop();
      boost::optional<PlantLoop> auxiliaryLoop() const;
      boost::optional<Node> auxiliaryInletNode() const;
      boost::optional<Node> auxiliaryOutletNode() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ChillerElectricASHRAE205");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<ExternalFile> optionalRepresentationFile() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CHILLERELECTRICASHRAE205_IMPL_HPP
