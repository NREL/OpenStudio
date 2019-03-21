/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef MODEL_SIZINGZONE_IMPL_HPP
#define MODEL_SIZINGZONE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"


namespace openstudio {
namespace model {

class ThermalZone;

namespace detail {

  /** SizingZone_Impl is a ModelObject_Impl that is the implementation class for SizingZone.*/
  class MODEL_API SizingZone_Impl : public ModelObject_Impl {

   public:
    /** @name Constructors and Destructors */
    //@{

    SizingZone_Impl(const IdfObject& idfObject,
                    Model_Impl* model,
                    bool keepHandle);

    SizingZone_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                    Model_Impl* model,
                    bool keepHandle);

    SizingZone_Impl(const SizingZone_Impl& other,
                    Model_Impl* model,
                    bool keepHandle);

    virtual ~SizingZone_Impl() {}

    //@}

    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    //@}
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

    double designZoneAirDistributionEffectivenessinCoolingMode() const;

    bool isDesignZoneAirDistributionEffectivenessinCoolingModeDefaulted() const;

    double designZoneAirDistributionEffectivenessinHeatingMode() const;

    bool isDesignZoneAirDistributionEffectivenessinHeatingModeDefaulted() const;

    bool accountforDedicatedOutdoorAirSystem() const;

    std::string dedicatedOutdoorAirSystemControlStrategy() const;

    boost::optional<double> dedicatedOutdoorAirLowSetpointTemperatureforDesign() const;

    bool isDedicatedOutdoorAirLowSetpointTemperatureforDesignAutosized() const;

    boost::optional<double> dedicatedOutdoorAirHighSetpointTemperatureforDesign() const;

    bool isDedicatedOutdoorAirHighSetpointTemperatureforDesignAutosized() const;

    boost::optional<double> autosizedDedicatedOutdoorAirLowSetpointTemperatureforDesign() const ;

    boost::optional<double> autosizedDedicatedOutdoorAirHighSetpointTemperatureforDesign() const ;

    void autosize();

    void applySizingValues();

    virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

    virtual std::vector<std::string> emsInternalVariableNames() const override;

    //@}
    /** @name Setters */
    //@{

    bool setThermalZone(const ThermalZone& thermalZone);

    bool setZoneCoolingDesignSupplyAirTemperatureInputMethod(const std::string &value);

    bool setZoneCoolingDesignSupplyAirTemperature(double zoneCoolingDesignSupplyAirTemperature);

    bool setZoneCoolingDesignSupplyAirTemperatureDifference(double value);

    bool setZoneHeatingDesignSupplyAirTemperatureInputMethod(const std::string &value);

    bool setZoneHeatingDesignSupplyAirTemperature(double zoneHeatingDesignSupplyAirTemperature);

    bool setZoneHeatingDesignSupplyAirTemperatureDifference(double value);

    bool setZoneCoolingDesignSupplyAirHumidityRatio(double zoneCoolingDesignSupplyAirHumidityRatio);

    bool setZoneHeatingDesignSupplyAirHumidityRatio(double zoneHeatingDesignSupplyAirHumidityRatio);

    bool setZoneHeatingSizingFactor(boost::optional<double> zoneHeatingSizingFactor);

    void resetZoneHeatingSizingFactor();

    bool setZoneCoolingSizingFactor(boost::optional<double> zoneCoolingSizingFactor);

    void resetZoneCoolingSizingFactor();

    bool setCoolingDesignAirFlowMethod(const std::string &coolingDesignAirFlowMethod);

    void resetCoolingDesignAirFlowMethod();

    bool setCoolingDesignAirFlowRate(double coolingDesignAirFlowRate);

    void resetCoolingDesignAirFlowRate();

    bool setCoolingMinimumAirFlowperZoneFloorArea(double coolingMinimumAirFlowperZoneFloorArea);

    void resetCoolingMinimumAirFlowperZoneFloorArea();

    bool setCoolingMinimumAirFlow(double coolingMinimumAirFlow);

    void resetCoolingMinimumAirFlow();

    bool setCoolingMinimumAirFlowFraction(double coolingMinimumAirFlowFraction);

    void resetCoolingMinimumAirFlowFraction();

    bool setHeatingDesignAirFlowMethod(std::string heatingDesignAirFlowMethod);

    void resetHeatingDesignAirFlowMethod();

    bool setHeatingDesignAirFlowRate(double heatingDesignAirFlowRate);

    void resetHeatingDesignAirFlowRate();

    bool setHeatingMaximumAirFlowperZoneFloorArea(double heatingMaximumAirFlowperZoneFloorArea);

    void resetHeatingMaximumAirFlowperZoneFloorArea();

    bool setHeatingMaximumAirFlow(double heatingMaximumAirFlow);

    void resetHeatingMaximumAirFlow();

    bool setHeatingMaximumAirFlowFraction(double heatingMaximumAirFlowFraction);

    void resetHeatingMaximumAirFlowFraction();

    bool setDesignZoneAirDistributionEffectivenessinCoolingMode(double designZoneAirDistributionEffectivenessinCoolingMode);

    void resetDesignZoneAirDistributionEffectivenessinCoolingMode();

    bool setDesignZoneAirDistributionEffectivenessinHeatingMode(double designZoneAirDistributionEffectivenessinHeatingMode);

    void resetDesignZoneAirDistributionEffectivenessinHeatingMode();

    bool setAccountforDedicatedOutdoorAirSystem(bool accountforDedicatedOutdoorAirSystem);

    bool setDedicatedOutdoorAirSystemControlStrategy(std::string dedicatedOutdoorAirSystemControlStrategy);

    bool setDedicatedOutdoorAirLowSetpointTemperatureforDesign(boost::optional<double> dedicatedOutdoorAirLowSetpointTemperatureforDesign);

    void autosizeDedicatedOutdoorAirLowSetpointTemperatureforDesign();

    bool setDedicatedOutdoorAirHighSetpointTemperatureforDesign(boost::optional<double> dedicatedOutdoorAirHighSetpointTemperatureforDesign);

    void autosizeDedicatedOutdoorAirHighSetpointTemperatureforDesign();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.SizingZone");

    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    boost::optional<ThermalZone> optionalThermalZone() const;

    std::vector<std::string> coolingDesignAirFlowMethodValues() const;
    std::vector<std::string> heatingDesignAirFlowMethodValues() const;
    boost::optional<ModelObject> thermalZoneAsModelObject() const;

    bool setThermalZoneAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SIZINGZONE_IMPL_HPP