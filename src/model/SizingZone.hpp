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

#ifndef MODEL_SIZINGZONE_HPP
#define MODEL_SIZINGZONE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  class ThermalZone;
  class Schedule;

  namespace detail {

    class SizingZone_Impl;

  }  // namespace detail

  /** SizingZone is a ModelObject that wraps the OpenStudio IDD object 'OS:Sizing:Zone'. */
  class MODEL_API SizingZone : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SizingZone(const Model& model, const ThermalZone& thermalZone);

    virtual ~SizingZone() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SizingZone(const SizingZone& other) = default;
    SizingZone(SizingZone&& other) = default;
    SizingZone& operator=(const SizingZone&) = default;
    SizingZone& operator=(SizingZone&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> coolingDesignAirFlowMethodValues();
    static std::vector<std::string> heatingDesignAirFlowMethodValues();
    static std::vector<std::string> zoneCoolingDesignSupplyAirTemperatureInputMethodValues();
    static std::vector<std::string> zoneHeatingDesignSupplyAirTemperatureInputMethodValues();
    static std::vector<std::string> zoneLoadSizingMethodValues();
    static std::vector<std::string> zoneLatentCoolingDesignSupplyAirHumidityRatioInputMethodValues();
    static std::vector<std::string> zoneLatentHeatingDesignSupplyAirHumidityRatioInputMethodValues();

    /** @name Getters */
    //@{

    ThermalZone thermalZone() const;

    std::string zoneCoolingDesignSupplyAirTemperatureInputMethod() const;

    double zoneCoolingDesignSupplyAirTemperature() const;

    double zoneCoolingDesignSupplyAirTemperatureDifference() const;

    std::string zoneHeatingDesignSupplyAirTemperatureInputMethod() const;

    double zoneHeatingDesignSupplyAirTemperature() const;

    double zoneHeatingDesignSupplyAirTemperatureDifference() const;

    double zoneCoolingDesignSupplyAirHumidityRatio() const;

    double zoneHeatingDesignSupplyAirHumidityRatio() const;

    boost::optional<double> zoneHeatingSizingFactor() const;

    boost::optional<double> zoneCoolingSizingFactor() const;

    std::string coolingDesignAirFlowMethod() const;

    bool isCoolingDesignAirFlowMethodDefaulted() const;

    double coolingDesignAirFlowRate() const;

    bool isCoolingDesignAirFlowRateDefaulted() const;

    double coolingMinimumAirFlowperZoneFloorArea() const;

    bool isCoolingMinimumAirFlowperZoneFloorAreaDefaulted() const;

    double coolingMinimumAirFlow() const;

    bool isCoolingMinimumAirFlowDefaulted() const;

    double coolingMinimumAirFlowFraction() const;

    bool isCoolingMinimumAirFlowFractionDefaulted() const;

    std::string heatingDesignAirFlowMethod() const;

    bool isHeatingDesignAirFlowMethodDefaulted() const;

    double heatingDesignAirFlowRate() const;

    bool isHeatingDesignAirFlowRateDefaulted() const;

    double heatingMaximumAirFlowperZoneFloorArea() const;

    bool isHeatingMaximumAirFlowperZoneFloorAreaDefaulted() const;

    double heatingMaximumAirFlow() const;

    bool isHeatingMaximumAirFlowDefaulted() const;

    double heatingMaximumAirFlowFraction() const;

    bool isHeatingMaximumAirFlowFractionDefaulted() const;

    bool accountforDedicatedOutdoorAirSystem() const;

    std::string dedicatedOutdoorAirSystemControlStrategy() const;

    boost::optional<double> dedicatedOutdoorAirLowSetpointTemperatureforDesign() const;

    bool isDedicatedOutdoorAirLowSetpointTemperatureforDesignAutosized() const;

    boost::optional<double> dedicatedOutdoorAirHighSetpointTemperatureforDesign() const;

    bool isDedicatedOutdoorAirHighSetpointTemperatureforDesignAutosized() const;

    std::string zoneLoadSizingMethod() const;

    std::string zoneLatentCoolingDesignSupplyAirHumidityRatioInputMethod() const;

    boost::optional<double> zoneDehumidificationDesignSupplyAirHumidityRatio() const;

    double zoneCoolingDesignSupplyAirHumidityRatioDifference() const;

    std::string zoneLatentHeatingDesignSupplyAirHumidityRatioInputMethod() const;

    boost::optional<double> zoneHumidificationDesignSupplyAirHumidityRatio() const;

    double zoneHumidificationDesignSupplyAirHumidityRatioDifference() const;

    boost::optional<Schedule> zoneHumidistatDehumidificationSetPointSchedule() const;

    boost::optional<Schedule> zoneHumidistatHumidificationSetPointSchedule() const;

    // Fields from DesignSpecification:ZoneAirDistribution

    double designZoneAirDistributionEffectivenessinCoolingMode() const;

    bool isDesignZoneAirDistributionEffectivenessinCoolingModeDefaulted() const;

    double designZoneAirDistributionEffectivenessinHeatingMode() const;

    bool isDesignZoneAirDistributionEffectivenessinHeatingModeDefaulted() const;

    double designZoneSecondaryRecirculationFraction() const;

    bool isDesignZoneSecondaryRecirculationFractionDefaulted() const;

    double designMinimumZoneVentilationEfficiency() const;

    bool isDesignMinimumZoneVentilationEfficiencyDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setZoneCoolingDesignSupplyAirTemperatureInputMethod(const std::string& value);

    bool setZoneCoolingDesignSupplyAirTemperature(double zoneCoolingDesignSupplyAirTemperature);

    bool setZoneCoolingDesignSupplyAirTemperatureDifference(double value);

    bool setZoneHeatingDesignSupplyAirTemperatureInputMethod(const std::string& value);

    bool setZoneHeatingDesignSupplyAirTemperature(double zoneHeatingDesignSupplyAirTemperature);

    bool setZoneHeatingDesignSupplyAirTemperatureDifference(double value);

    bool setZoneCoolingDesignSupplyAirHumidityRatio(double zoneCoolingDesignSupplyAirHumidityRatio);

    bool setZoneHeatingDesignSupplyAirHumidityRatio(double zoneHeatingDesignSupplyAirHumidityRatio);

    bool setZoneHeatingSizingFactor(double zoneHeatingSizingFactor);

    void resetZoneHeatingSizingFactor();

    bool setZoneCoolingSizingFactor(double zoneCoolingSizingFactor);

    void resetZoneCoolingSizingFactor();

    bool setCoolingDesignAirFlowMethod(const std::string& coolingDesignAirFlowMethod);

    void resetCoolingDesignAirFlowMethod();

    bool setCoolingDesignAirFlowRate(double coolingDesignAirFlowRate);

    void resetCoolingDesignAirFlowRate();

    bool setCoolingMinimumAirFlowperZoneFloorArea(double coolingMinimumAirFlowperZoneFloorArea);

    void resetCoolingMinimumAirFlowperZoneFloorArea();

    bool setCoolingMinimumAirFlow(double coolingMinimumAirFlow);

    void resetCoolingMinimumAirFlow();

    bool setCoolingMinimumAirFlowFraction(double coolingMinimumAirFlowFraction);

    void resetCoolingMinimumAirFlowFraction();

    bool setHeatingDesignAirFlowMethod(const std::string& heatingDesignAirFlowMethod);

    void resetHeatingDesignAirFlowMethod();

    bool setHeatingDesignAirFlowRate(double heatingDesignAirFlowRate);

    void resetHeatingDesignAirFlowRate();

    bool setHeatingMaximumAirFlowperZoneFloorArea(double heatingMaximumAirFlowperZoneFloorArea);

    void resetHeatingMaximumAirFlowperZoneFloorArea();

    bool setHeatingMaximumAirFlow(double heatingMaximumAirFlow);

    void resetHeatingMaximumAirFlow();

    bool setHeatingMaximumAirFlowFraction(double heatingMaximumAirFlowFraction);

    void resetHeatingMaximumAirFlowFraction();

    bool setAccountforDedicatedOutdoorAirSystem(bool accountforDedicatedOutdoorAirSystem);

    bool setDedicatedOutdoorAirSystemControlStrategy(const std::string& dedicatedOutdoorAirSystemControlStrategy);

    bool setDedicatedOutdoorAirLowSetpointTemperatureforDesign(double dedicatedOutdoorAirLowSetpointTemperatureforDesign);

    void autosizeDedicatedOutdoorAirLowSetpointTemperatureforDesign();

    bool setDedicatedOutdoorAirHighSetpointTemperatureforDesign(double dedicatedOutdoorAirHighSetpointTemperatureforDesign);

    void autosizeDedicatedOutdoorAirHighSetpointTemperatureforDesign();

    bool setZoneLoadSizingMethod(const std::string& zoneLoadSizingMethod);

    bool setZoneLatentCoolingDesignSupplyAirHumidityRatioInputMethod(const std::string& zoneLatentCoolingDesignSupplyAirHumidityRatioInputMethod);

    bool setZoneDehumidificationDesignSupplyAirHumidityRatio(double zoneDehumidificationDesignSupplyAirHumidityRatio);
    void resetZoneDehumidificationDesignSupplyAirHumidityRatio();

    bool setZoneCoolingDesignSupplyAirHumidityRatioDifference(double zoneCoolingDesignSupplyAirHumidityRatioDifference);

    bool setZoneLatentHeatingDesignSupplyAirHumidityRatioInputMethod(const std::string& zoneLatentHeatingDesignSupplyAirHumidityRatioInputMethod);

    bool setZoneHumidificationDesignSupplyAirHumidityRatio(double zoneHumidificationDesignSupplyAirHumidityRatio);
    void resetZoneHumidificationDesignSupplyAirHumidityRatio();

    bool setZoneHumidificationDesignSupplyAirHumidityRatioDifference(double zoneHumidificationDesignSupplyAirHumidityRatioDifference);

    bool setZoneHumidistatDehumidificationSetPointSchedule(Schedule& schedule);
    void resetZoneHumidistatDehumidificationSetPointSchedule();

    bool setZoneHumidistatHumidificationSetPointSchedule(Schedule& schedule);
    void resetZoneHumidistatHumidificationSetPointSchedule();

    // Fields from DesignSpecification:ZoneAirDistribution

    bool setDesignZoneAirDistributionEffectivenessinCoolingMode(double designZoneAirDistributionEffectivenessinCoolingMode);

    void resetDesignZoneAirDistributionEffectivenessinCoolingMode();

    bool setDesignZoneAirDistributionEffectivenessinHeatingMode(double designZoneAirDistributionEffectivenessinHeatingMode);

    void resetDesignZoneAirDistributionEffectivenessinHeatingMode();

    bool setDesignZoneSecondaryRecirculationFraction(double designZoneSecondaryRecirculationFraction);

    void resetDesignZoneSecondaryRecirculationFraction();

    bool setDesignMinimumZoneVentilationEfficiency(double designMinimumZoneVentilationEfficiency);

    void resetDesignMinimumZoneVentilationEfficiency();

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedDedicatedOutdoorAirLowSetpointTemperatureforDesign() const;

    boost::optional<double> autosizedDedicatedOutdoorAirHighSetpointTemperatureforDesign() const;

    void autosize();

    void applySizingValues();

    //@}
   protected:
    bool setThermalZone(const ThermalZone& thermalZone);

    /// @cond
    using ImplType = detail::SizingZone_Impl;

    explicit SizingZone(std::shared_ptr<detail::SizingZone_Impl> impl);

    friend class detail::SizingZone_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    friend class ThermalZone;

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SizingZone");
  };

  /** \relates SizingZone*/
  using OptionalSizingZone = boost::optional<SizingZone>;

  /** \relates SizingZone*/
  using SizingZoneVector = std::vector<SizingZone>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SIZINGZONE_HPP
