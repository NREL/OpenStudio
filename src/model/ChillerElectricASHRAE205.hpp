/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CHILLERELECTRICASHRAE205_HPP
#define MODEL_CHILLERELECTRICASHRAE205_HPP

#include <model/ModelAPI.hpp>
#include "WaterToWaterComponent.hpp"

namespace openstudio {

namespace model {

  class ExternalFile;
  class Node;
  class ThermalZone;
  class Schedule;

  namespace detail {

    class ChillerElectricASHRAE205_Impl;

  }  // namespace detail

  /** ChillerElectricASHRAE205 is a WaterToWaterComponent that wraps the OpenStudio IDD object 'OS:Chiller:Electric:ASHRAE205'. */
  class MODEL_API ChillerElectricASHRAE205 : public WaterToWaterComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ChillerElectricASHRAE205(const ExternalFile& representationFile);

    virtual ~ChillerElectricASHRAE205() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ChillerElectricASHRAE205(const ChillerElectricASHRAE205& other) = default;
    ChillerElectricASHRAE205(ChillerElectricASHRAE205&& other) = default;
    ChillerElectricASHRAE205& operator=(const ChillerElectricASHRAE205&) = default;
    ChillerElectricASHRAE205& operator=(ChillerElectricASHRAE205&&) = default;

    // TODO: once supported by E+, re-enable
    static constexpr bool isHeatRecoverySupportedByEnergyplus = false;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> performanceInterpolationMethodValues();

    static std::vector<std::string> ambientTemperatureIndicatorValues();

    static std::vector<std::string> chillerFlowModeValues();

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

    // This is done via the setAmbientXXX methods
    // bool setAmbientTemperatureIndicator(const std::string& ambientTemperatureIndicator);

    /** This will make the Ambient Temperature Indicator = 'Schedule' */
    bool setAmbientTemperatureSchedule(Schedule& schedule);
    /** This will reset the Ambient Temperature Indicator = 'Outdoors' */
    void resetAmbientTemperatureSchedule();

    /** This will make the Ambient Temperature Indicator = 'Zone' */
    bool setAmbientTemperatureZone(const ThermalZone& thermalZone);
    /** This will reset the Ambient Temperature Indicator = 'Outdoors' */
    void resetAmbientTemperatureZone();

    /** This will make the Ambient Temperature Indicator = 'Outdoors' */
    bool setAmbientTemperatureOutdoorAirNodeName(const std::string& ambientTemperatureOutdoorAirNodeName);
    /** This will reset the Ambient Temperature Indicator = 'Outdoors'. A default Outdoor Air Node Name will be used in the ForwardTranslator */
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

    unsigned oilCoolerInletPort() const;
    boost::optional<ModelObject> oilCoolerInletModelObject() const;
    boost::optional<Node> oilCoolerInletNode() const;
    unsigned oilCoolerOutletPort() const;
    boost::optional<ModelObject> oilCoolerOutletModelObject() const;
    boost::optional<Node> oilCoolerOutletNode() const;
    boost::optional<PlantLoop> oilCoolerLoop() const;
    bool addDemandBranchOnOilCoolerLoop(PlantLoop& plantLoop);
    bool addToOilCoolerLoopNode(Node& node);
    bool removeFromOilCoolerLoop();

    unsigned auxiliaryInletPort() const;
    boost::optional<ModelObject> auxiliaryInletModelObject() const;
    boost::optional<Node> auxiliaryInletNode() const;
    unsigned auxiliaryOutletPort() const;
    boost::optional<ModelObject> auxiliaryOutletModelObject() const;
    boost::optional<Node> auxiliaryOutletNode() const;
    bool addDemandBranchOnAuxiliaryLoop(PlantLoop& plantLoop);
    bool addToAuxiliaryLoopNode(Node& node);
    bool removeFromAuxiliaryLoop();
    boost::optional<PlantLoop> auxiliaryLoop() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::ChillerElectricASHRAE205_Impl;

    explicit ChillerElectricASHRAE205(std::shared_ptr<detail::ChillerElectricASHRAE205_Impl> impl);

    friend class detail::ChillerElectricASHRAE205_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ChillerElectricASHRAE205");
  };

  /** \relates ChillerElectricASHRAE205*/
  using OptionalChillerElectricASHRAE205 = boost::optional<ChillerElectricASHRAE205>;

  /** \relates ChillerElectricASHRAE205*/
  using ChillerElectricASHRAE205Vector = std::vector<ChillerElectricASHRAE205>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CHILLERELECTRICASHRAE205_HPP
