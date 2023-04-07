/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef MODEL_WATERHEATERMIXED_HPP
#define MODEL_WATERHEATERMIXED_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class CurveCubic;
  class ThermalZone;
  class WaterHeaterSizing;
  class Node;
  class PlantLoop;

  namespace detail {

    class WaterHeaterMixed_Impl;

  }  // namespace detail

  /** WaterHeaterMixed is a WaterToWaterComponent that wraps the OpenStudio IDD object
 *  'OS:WaterHeater:Mixed'. */
  class MODEL_API WaterHeaterMixed : public WaterToWaterComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit WaterHeaterMixed(const Model& model);

    virtual ~WaterHeaterMixed() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    WaterHeaterMixed(const WaterHeaterMixed& other) = default;
    WaterHeaterMixed(WaterHeaterMixed&& other) = default;
    WaterHeaterMixed& operator=(const WaterHeaterMixed&) = default;
    WaterHeaterMixed& operator=(WaterHeaterMixed&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> heaterControlTypeValues();

    static std::vector<std::string> heaterFuelTypeValues();

    static std::vector<std::string> offCycleParasiticFuelTypeValues();

    static std::vector<std::string> onCycleParasiticFuelTypeValues();

    static std::vector<std::string> ambientTemperatureIndicatorValues();

    static std::vector<std::string> sourceSideFlowControlModeValues();

    /** @name Getters */
    //@{

    boost::optional<double> tankVolume() const;

    bool isTankVolumeDefaulted() const;

    bool isTankVolumeAutosized() const;

    boost::optional<Schedule> setpointTemperatureSchedule() const;

    double deadbandTemperatureDifference() const;

    bool isDeadbandTemperatureDifferenceDefaulted() const;

    boost::optional<double> maximumTemperatureLimit() const;

    std::string heaterControlType() const;

    bool isHeaterControlTypeDefaulted() const;

    boost::optional<double> heaterMaximumCapacity() const;

    bool isHeaterMaximumCapacityAutosized() const;

    boost::optional<double> heaterMinimumCapacity() const;

    double heaterIgnitionMinimumFlowRate() const;

    bool isHeaterIgnitionMinimumFlowRateDefaulted() const;

    double heaterIgnitionDelay() const;

    bool isHeaterIgnitionDelayDefaulted() const;

    std::string heaterFuelType() const;

    boost::optional<double> heaterThermalEfficiency() const;

    boost::optional<CurveCubic> partLoadFactorCurve() const;

    double offCycleParasiticFuelConsumptionRate() const;

    bool isOffCycleParasiticFuelConsumptionRateDefaulted() const;

    boost::optional<std::string> offCycleParasiticFuelType() const;

    double offCycleParasiticHeatFractiontoTank() const;

    bool isOffCycleParasiticHeatFractiontoTankDefaulted() const;

    double onCycleParasiticFuelConsumptionRate() const;

    bool isOnCycleParasiticFuelConsumptionRateDefaulted() const;

    boost::optional<std::string> onCycleParasiticFuelType() const;

    double onCycleParasiticHeatFractiontoTank() const;

    bool isOnCycleParasiticHeatFractiontoTankDefaulted() const;

    std::string ambientTemperatureIndicator() const;

    boost::optional<Schedule> ambientTemperatureSchedule() const;

    boost::optional<ThermalZone> ambientTemperatureThermalZone() const;

    boost::optional<std::string> ambientTemperatureOutdoorAirNodeName() const;

    boost::optional<double> offCycleLossCoefficienttoAmbientTemperature() const;

    double offCycleLossFractiontoThermalZone() const;

    bool isOffCycleLossFractiontoThermalZoneDefaulted() const;

    boost::optional<double> onCycleLossCoefficienttoAmbientTemperature() const;

    double onCycleLossFractiontoThermalZone() const;

    bool isOnCycleLossFractiontoThermalZoneDefaulted() const;

    boost::optional<double> peakUseFlowRate() const;

    boost::optional<Schedule> useFlowRateFractionSchedule() const;

    boost::optional<Schedule> coldWaterSupplyTemperatureSchedule() const;

    double useSideEffectiveness() const;

    bool isUseSideEffectivenessDefaulted() const;

    double sourceSideEffectiveness() const;

    bool isSourceSideEffectivenessDefaulted() const;

    boost::optional<double> useSideDesignFlowRate() const;

    bool isUseSideDesignFlowRateDefaulted() const;

    bool isUseSideDesignFlowRateAutosized() const;

    boost::optional<double> sourceSideDesignFlowRate() const;

    bool isSourceSideDesignFlowRateDefaulted() const;

    bool isSourceSideDesignFlowRateAutosized() const;

    double indirectWaterHeatingRecoveryTime() const;

    bool isIndirectWaterHeatingRecoveryTimeDefaulted() const;

    std::string sourceSideFlowControlMode() const;

    boost::optional<Schedule> indirectAlternateSetpointTemperatureSchedule() const;

    std::string endUseSubcategory() const;

    //@}
    /** @name Setters */
    //@{

    bool setTankVolume(double tankVolume);

    void resetTankVolume();

    void autosizeTankVolume();

    bool setSetpointTemperatureSchedule(Schedule& setpointTemperatureSchedule);

    void resetSetpointTemperatureSchedule();

    bool setDeadbandTemperatureDifference(double deadbandTemperatureDifference);

    void resetDeadbandTemperatureDifference();

    bool setMaximumTemperatureLimit(double maximumTemperatureLimit);

    void resetMaximumTemperatureLimit();

    bool setHeaterControlType(const std::string& heaterControlType);

    void resetHeaterControlType();

    bool setHeaterMaximumCapacity(double heaterMaximumCapacity);

    void resetHeaterMaximumCapacity();

    void autosizeHeaterMaximumCapacity();

    bool setHeaterMinimumCapacity(double heaterMinimumCapacity);

    void resetHeaterMinimumCapacity();

    bool setHeaterIgnitionMinimumFlowRate(double heaterIgnitionMinimumFlowRate);

    void resetHeaterIgnitionMinimumFlowRate();

    bool setHeaterIgnitionDelay(double heaterIgnitionDelay);

    void resetHeaterIgnitionDelay();

    bool setHeaterFuelType(const std::string& heaterFuelType);

    bool setHeaterThermalEfficiency(double heaterThermalEfficiency);

    void resetHeaterThermalEfficiency();

    bool setPartLoadFactorCurve(const CurveCubic& partLoadFactorCurve);

    void resetPartLoadFactorCurve();

    bool setOffCycleParasiticFuelConsumptionRate(double offCycleParasiticFuelConsumptionRate);

    void resetOffCycleParasiticFuelConsumptionRate();

    bool setOffCycleParasiticFuelType(const std::string& offCycleParasiticFuelType);

    void resetOffCycleParasiticFuelType();

    bool setOffCycleParasiticHeatFractiontoTank(double offCycleParasiticHeatFractiontoTank);

    void resetOffCycleParasiticHeatFractiontoTank();

    bool setOnCycleParasiticFuelConsumptionRate(double onCycleParasiticFuelConsumptionRate);

    void resetOnCycleParasiticFuelConsumptionRate();

    bool setOnCycleParasiticFuelType(const std::string& onCycleParasiticFuelType);

    void resetOnCycleParasiticFuelType();

    bool setOnCycleParasiticHeatFractiontoTank(double onCycleParasiticHeatFractiontoTank);

    void resetOnCycleParasiticHeatFractiontoTank();

    bool setAmbientTemperatureIndicator(const std::string& ambientTemperatureIndicator);

    bool setAmbientTemperatureSchedule(Schedule& ambientTemperatureSchedule);

    void resetAmbientTemperatureSchedule();

    bool setAmbientTemperatureThermalZone(const ThermalZone& ambientTemperatureThermalZone);

    void resetAmbientTemperatureThermalZone();

    bool setAmbientTemperatureOutdoorAirNodeName(const std::string& ambientTemperatureOutdoorAirNodeName);

    void resetAmbientTemperatureOutdoorAirNodeName();

    bool setOffCycleLossCoefficienttoAmbientTemperature(double offCycleLossCoefficienttoAmbientTemperature);

    void resetOffCycleLossCoefficienttoAmbientTemperature();

    bool setOffCycleLossFractiontoThermalZone(double offCycleLossFractiontoThermalZone);

    void resetOffCycleLossFractiontoThermalZone();

    bool setOnCycleLossCoefficienttoAmbientTemperature(double onCycleLossCoefficienttoAmbientTemperature);

    void resetOnCycleLossCoefficienttoAmbientTemperature();

    bool setOnCycleLossFractiontoThermalZone(double onCycleLossFractiontoThermalZone);

    void resetOnCycleLossFractiontoThermalZone();

    bool setPeakUseFlowRate(double peakUseFlowRate);

    void resetPeakUseFlowRate();

    bool setUseFlowRateFractionSchedule(Schedule& useFlowRateFractionSchedule);

    void resetUseFlowRateFractionSchedule();

    bool setColdWaterSupplyTemperatureSchedule(Schedule& coldWaterSupplyTemperatureSchedule);

    void resetColdWaterSupplyTemperatureSchedule();

    bool setUseSideEffectiveness(double useSideEffectiveness);

    void resetUseSideEffectiveness();

    bool setSourceSideEffectiveness(double sourceSideEffectiveness);

    void resetSourceSideEffectiveness();

    bool setUseSideDesignFlowRate(double useSideDesignFlowRate);

    void resetUseSideDesignFlowRate();

    void autosizeUseSideDesignFlowRate();

    bool setSourceSideDesignFlowRate(double sourceSideDesignFlowRate);

    void resetSourceSideDesignFlowRate();

    void autosizeSourceSideDesignFlowRate();

    bool setIndirectWaterHeatingRecoveryTime(double indirectWaterHeatingRecoveryTime);

    void resetIndirectWaterHeatingRecoveryTime();

    boost::optional<double> autosizedTankVolume() const;

    boost::optional<double> autosizedHeaterMaximumCapacity() const;

    boost::optional<double> autosizedUseSideDesignFlowRate() const;

    boost::optional<double> autosizedSourceSideDesignFlowRate() const;

    /* This will not accept 'IndirectHeatAlternateSetpoint' as a control mode, you should instead use 'setIndirectAlternateSetpointTemperatureSchedule'.
   * For any other modes ('StorageTank', 'IndirectHeatPrimarySetpoint'), this resets the indirect alternate setpoint temperature schedule
   */
    bool setSourceSideFlowControlMode(const std::string& sourceSideFlowControlMode);

    /* This will automatically switch the Source Side Flow Control Mode to 'IndirectHeatAlternateSetpoint' */
    bool setIndirectAlternateSetpointTemperatureSchedule(Schedule& indirectAlternateSetpointTemperatureSchedule);

    /* This will automatically reset the Source Side Flow Control Mode to default 'IndirectHeatPrimarySetpoint' */
    void resetIndirectAlternateSetpointTemperatureSchedule();

    bool setEndUseSubcategory(const std::string& endUseSubcategory);

    //@}
    /** @name Other */
    //@{

    WaterHeaterSizing waterHeaterSizing() const;

    // A helper method to specifically connect the tank on its Source Side, regardless of whether it is on the demand or supply side (without
    // disconnecting the Use Side, in case it's a supply node given)
    bool addToSourceSideNode(Node& node);

    // This is a more conveniently named method to return the PlantLoop connected to the Use Side Inlet/Outlet Nodes,
    // which is necessarilly on the supply side. It does exactly the same as the overriden secondaryPlantLoop() method
    boost::optional<PlantLoop> useSidePlantLoop() const;

    // This is a more conveniently named helper to return the PlantLoop connected to the Source Side Inlet/Outlet Nodes,
    // whether that is on the demand or supply side. It does exactly the same as the overriden plantLoop() method
    boost::optional<PlantLoop> sourceSidePlantLoop() const;

    // Convenience name for removeFromSecondaryPlantLoop()
    bool removeFromSourceSidePlantLoop();

    boost::optional<ModelObject> useSideInletModelObject() const;

    boost::optional<ModelObject> useSideOutletModelObject() const;

    boost::optional<ModelObject> sourceSideInletModelObject() const;

    boost::optional<ModelObject> sourceSideOutletModelObject() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::WaterHeaterMixed_Impl;

    explicit WaterHeaterMixed(std::shared_ptr<detail::WaterHeaterMixed_Impl> impl);

    friend class detail::WaterHeaterMixed_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    /// @endcond

   private:
    REGISTER_LOGGER("openstudio.model.WaterHeaterMixed");
  };

  /** \relates WaterHeaterMixed*/
  using OptionalWaterHeaterMixed = boost::optional<WaterHeaterMixed>;

  /** \relates WaterHeaterMixed*/
  using WaterHeaterMixedVector = std::vector<WaterHeaterMixed>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_WATERHEATERMIXED_HPP
