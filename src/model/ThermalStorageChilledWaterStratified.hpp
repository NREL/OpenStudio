/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_THERMALSTORAGECHILLEDWATERSTRATIFIED_HPP
#define MODEL_THERMALSTORAGECHILLEDWATERSTRATIFIED_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class ThermalZone;

  namespace detail {

    class ThermalStorageChilledWaterStratified_Impl;

  }  // namespace detail

  /** ThermalStorageChilledWaterStratified is a WaterToWaterComponent that wraps the OpenStudio IDD object 'OS:ThermalStorage:ChilledWater:Stratified'. */
  class MODEL_API ThermalStorageChilledWaterStratified : public WaterToWaterComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ThermalStorageChilledWaterStratified(const Model& model);

    virtual ~ThermalStorageChilledWaterStratified() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ThermalStorageChilledWaterStratified(const ThermalStorageChilledWaterStratified& other) = default;
    ThermalStorageChilledWaterStratified(ThermalStorageChilledWaterStratified&& other) = default;
    ThermalStorageChilledWaterStratified& operator=(const ThermalStorageChilledWaterStratified&) = default;
    ThermalStorageChilledWaterStratified& operator=(ThermalStorageChilledWaterStratified&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> tankShapeValues();

    static std::vector<std::string> ambientTemperatureIndicatorValues();

    static std::vector<std::string> inletModeValues();

    /** @name Getters */
    //@{

    double tankVolume() const;

    double tankHeight() const;

    std::string tankShape() const;

    boost::optional<double> tankPerimeter() const;

    boost::optional<Schedule> setpointTemperatureSchedule() const;

    double deadbandTemperatureDifference() const;

    boost::optional<double> temperatureSensorHeight() const;

    boost::optional<double> minimumTemperatureLimit() const;

    boost::optional<double> nominalCoolingCapacity() const;

    std::string ambientTemperatureIndicator() const;

    boost::optional<Schedule> ambientTemperatureSchedule() const;

    boost::optional<ThermalZone> ambientTemperatureThermalZone() const;

    boost::optional<std::string> ambientTemperatureOutdoorAirNodeName() const;

    boost::optional<double> uniformSkinLossCoefficientperUnitAreatoAmbientTemperature() const;

    double useSideHeatTransferEffectiveness() const;

    boost::optional<Schedule> useSideAvailabilitySchedule() const;

    boost::optional<double> useSideInletHeight() const;

    bool isUseSideInletHeightAutocalculated() const;

    double useSideOutletHeight() const;

    boost::optional<double> useSideDesignFlowRate() const;

    bool isUseSideDesignFlowRateAutosized() const;

    double sourceSideHeatTransferEffectiveness() const;

    boost::optional<Schedule> sourceSideAvailabilitySchedule() const;

    double sourceSideInletHeight() const;

    boost::optional<double> sourceSideOutletHeight() const;

    bool isSourceSideOutletHeightAutocalculated() const;

    boost::optional<double> sourceSideDesignFlowRate() const;

    bool isSourceSideDesignFlowRateAutosized() const;

    double tankRecoveryTime() const;

    std::string inletMode() const;

    int numberofNodes() const;

    double additionalDestratificationConductivity() const;

    double node1AdditionalLossCoefficient() const;

    double node2AdditionalLossCoefficient() const;

    double node3AdditionalLossCoefficient() const;

    double node4AdditionalLossCoefficient() const;

    double node5AdditionalLossCoefficient() const;

    double node6AdditionalLossCoefficient() const;

    double node7AdditionalLossCoefficient() const;

    double node8AdditionalLossCoefficient() const;

    double node9AdditionalLossCoefficient() const;

    double node10AdditionalLossCoefficient() const;

    //@}
    /** @name Setters */
    //@{

    bool setTankVolume(double tankVolume);

    bool setTankHeight(double tankHeight);

    bool setTankShape(const std::string& tankShape);

    bool setTankPerimeter(double tankPerimeter);

    void resetTankPerimeter();

    bool setSetpointTemperatureSchedule(Schedule& schedule);

    void resetSetpointTemperatureSchedule();

    bool setDeadbandTemperatureDifference(double deadbandTemperatureDifference);

    bool setTemperatureSensorHeight(double temperatureSensorHeight);

    void resetTemperatureSensorHeight();

    bool setMinimumTemperatureLimit(double minimumTemperatureLimit);

    void resetMinimumTemperatureLimit();

    bool setNominalCoolingCapacity(double nominalCoolingCapacity);

    void resetNominalCoolingCapacity();

    bool setAmbientTemperatureIndicator(const std::string& ambientTemperatureIndicator);

    bool setAmbientTemperatureSchedule(Schedule& schedule);

    void resetAmbientTemperatureSchedule();

    bool setAmbientTemperatureThermalZone(const ThermalZone& thermalZone);

    void resetAmbientTemperatureThermalZone();

    bool setAmbientTemperatureOutdoorAirNodeName(const std::string& ambientTemperatureOutdoorAirNodeName);

    void resetAmbientTemperatureOutdoorAirNodeName();

    bool setUniformSkinLossCoefficientperUnitAreatoAmbientTemperature(double uniformSkinLossCoefficientperUnitAreatoAmbientTemperature);

    void resetUniformSkinLossCoefficientperUnitAreatoAmbientTemperature();

    bool setUseSideHeatTransferEffectiveness(double useSideHeatTransferEffectiveness);

    bool setUseSideAvailabilitySchedule(Schedule& schedule);

    void resetUseSideAvailabilitySchedule();

    bool setUseSideInletHeight(double useSideInletHeight);

    void autocalculateUseSideInletHeight();

    bool setUseSideOutletHeight(double useSideOutletHeight);

    bool setUseSideDesignFlowRate(double useSideDesignFlowRate);

    void autosizeUseSideDesignFlowRate();

    bool setSourceSideHeatTransferEffectiveness(double sourceSideHeatTransferEffectiveness);

    bool setSourceSideAvailabilitySchedule(Schedule& schedule);

    void resetSourceSideAvailabilitySchedule();

    bool setSourceSideInletHeight(double sourceSideInletHeight);

    bool setSourceSideOutletHeight(double sourceSideOutletHeight);

    void autocalculateSourceSideOutletHeight();

    bool setSourceSideDesignFlowRate(double sourceSideDesignFlowRate);

    void autosizeSourceSideDesignFlowRate();

    bool setTankRecoveryTime(double tankRecoveryTime);

    bool setInletMode(const std::string& inletMode);

    bool setNumberofNodes(int numberofNodes);

    bool setAdditionalDestratificationConductivity(double additionalDestratificationConductivity);

    bool setNode1AdditionalLossCoefficient(double node1AdditionalLossCoefficient);

    bool setNode2AdditionalLossCoefficient(double node2AdditionalLossCoefficient);

    bool setNode3AdditionalLossCoefficient(double node3AdditionalLossCoefficient);

    bool setNode4AdditionalLossCoefficient(double node4AdditionalLossCoefficient);

    bool setNode5AdditionalLossCoefficient(double node5AdditionalLossCoefficient);

    bool setNode6AdditionalLossCoefficient(double node6AdditionalLossCoefficient);

    bool setNode7AdditionalLossCoefficient(double node7AdditionalLossCoefficient);

    bool setNode8AdditionalLossCoefficient(double node8AdditionalLossCoefficient);

    bool setNode9AdditionalLossCoefficient(double node9AdditionalLossCoefficient);

    bool setNode10AdditionalLossCoefficient(double node10AdditionalLossCoefficient);

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedUseSideDesignFlowRate() const;

    boost::optional<double> autosizedSourceSideDesignFlowRate() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::ThermalStorageChilledWaterStratified_Impl;

    explicit ThermalStorageChilledWaterStratified(std::shared_ptr<detail::ThermalStorageChilledWaterStratified_Impl> impl);

    friend class detail::ThermalStorageChilledWaterStratified_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ThermalStorageChilledWaterStratified");
  };

  /** \relates ThermalStorageChilledWaterStratified*/
  using OptionalThermalStorageChilledWaterStratified = boost::optional<ThermalStorageChilledWaterStratified>;

  /** \relates ThermalStorageChilledWaterStratified*/
  using ThermalStorageChilledWaterStratifiedVector = std::vector<ThermalStorageChilledWaterStratified>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_THERMALSTORAGECHILLEDWATERSTRATIFIED_HPP
