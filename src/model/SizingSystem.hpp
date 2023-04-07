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

#ifndef MODEL_SIZINGSYSTEM_HPP
#define MODEL_SIZINGSYSTEM_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  class AirLoopHVAC;

  namespace detail {

    class SizingSystem_Impl;
    class AirLoopHVAC_Impl;

  }  // namespace detail

  /** SizingSystem is a ModelObject that wraps the OpenStudio IDD object 'OS:Sizing:System'. */
  class MODEL_API SizingSystem : public ModelObject
  {
   public:
    explicit SizingSystem(const Model& model, const AirLoopHVAC& airLoopHVAC);

    virtual ~SizingSystem() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SizingSystem(const SizingSystem& other) = default;
    SizingSystem(SizingSystem&& other) = default;
    SizingSystem& operator=(const SizingSystem&) = default;
    SizingSystem& operator=(SizingSystem&&) = default;

    static IddObjectType iddObjectType();

    static std::vector<std::string> validTypeofLoadtoSizeOnValues();

    static std::vector<std::string> validSizingOptionValues();

    static std::vector<std::string> validCoolingDesignAirFlowMethodValues();

    static std::vector<std::string> validHeatingDesignAirFlowMethodValues();

    static std::vector<std::string> validSystemOutdoorAirMethodValues();

    std::string typeofLoadtoSizeOn() const;

    bool isTypeofLoadtoSizeOnDefaulted() const;

    boost::optional<double> designOutdoorAirFlowRate() const;

    bool isDesignOutdoorAirFlowRateDefaulted() const;

    bool isDesignOutdoorAirFlowRateAutosized() const;

    boost::optional<double> centralHeatingMaximumSystemAirFlowRatio() const;

    bool isCentralHeatingMaximumSystemAirFlowRatioDefaulted() const;

    bool isCentralHeatingMaximumSystemAirFlowRatioAutosized() const;

    double preheatDesignTemperature() const;

    double preheatDesignHumidityRatio() const;

    double precoolDesignTemperature() const;

    double precoolDesignHumidityRatio() const;

    double centralCoolingDesignSupplyAirTemperature() const;

    double centralHeatingDesignSupplyAirTemperature() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Type of Zone Sum to Use" **/
    std::string sizingOption() const;

    bool isSizingOptionDefaulted() const;

    bool allOutdoorAirinCooling() const;

    bool isAllOutdoorAirinCoolingDefaulted() const;

    bool allOutdoorAirinHeating() const;

    bool isAllOutdoorAirinHeatingDefaulted() const;

    double centralCoolingDesignSupplyAirHumidityRatio() const;

    bool isCentralCoolingDesignSupplyAirHumidityRatioDefaulted() const;

    double centralHeatingDesignSupplyAirHumidityRatio() const;

    bool isCentralHeatingDesignSupplyAirHumidityRatioDefaulted() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Cooling Supply Air Flow Rate Method" **/
    std::string coolingDesignAirFlowMethod() const;

    bool isCoolingDesignAirFlowMethodDefaulted() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Cooling Supply Air Flow Rate" **/
    double coolingDesignAirFlowRate() const;

    bool isCoolingDesignAirFlowRateDefaulted() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Heating Supply Air Flow Rate Method" **/
    std::string heatingDesignAirFlowMethod() const;

    bool isHeatingDesignAirFlowMethodDefaulted() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Heating Supply Air Flow Rate" **/
    double heatingDesignAirFlowRate() const;

    bool isHeatingDesignAirFlowRateDefaulted() const;

    std::string systemOutdoorAirMethod() const;

    bool isSystemOutdoorAirMethodDefaulted() const;

    double zoneMaximumOutdoorAirFraction() const;

    double coolingSupplyAirFlowRatePerFloorArea() const;

    double coolingFractionofAutosizedCoolingSupplyAirFlowRate() const;

    double coolingSupplyAirFlowRatePerUnitCoolingCapacity() const;

    double heatingSupplyAirFlowRatePerFloorArea() const;

    double heatingFractionofAutosizedHeatingSupplyAirFlowRate() const;

    double heatingFractionofAutosizedCoolingSupplyAirFlowRate() const;

    double heatingSupplyAirFlowRatePerUnitHeatingCapacity() const;

    std::string coolingDesignCapacityMethod() const;

    boost::optional<double> coolingDesignCapacity() const;

    bool isCoolingDesignCapacityAutosized() const;

    double coolingDesignCapacityPerFloorArea() const;

    double fractionofAutosizedCoolingDesignCapacity() const;

    std::string heatingDesignCapacityMethod() const;

    boost::optional<double> heatingDesignCapacity() const;

    bool isHeatingDesignCapacityAutosized() const;

    double heatingDesignCapacityPerFloorArea() const;

    double fractionofAutosizedHeatingDesignCapacity() const;

    std::string centralCoolingCapacityControlMethod() const;

    boost::optional<double> occupantDiversity() const;

    bool isOccupantDiversityAutosized() const;

    bool setTypeofLoadtoSizeOn(const std::string& typeofLoadtoSizeOn);

    void resetTypeofLoadtoSizeOn();

    bool setDesignOutdoorAirFlowRate(double designOutdoorAirFlowRate);

    void resetDesignOutdoorAirFlowRate();

    void autosizeDesignOutdoorAirFlowRate();

    bool setCentralHeatingMaximumSystemAirFlowRatio(double centralHeatingMaximumSystemAirFlowRatio);

    void resetCentralHeatingMaximumSystemAirFlowRatio();

    void autosizeCentralHeatingMaximumSystemAirFlowRatio();

    bool setPreheatDesignTemperature(double preheatDesignTemperature);

    bool setPreheatDesignHumidityRatio(double preheatDesignHumidityRatio);

    bool setPrecoolDesignTemperature(double precoolDesignTemperature);

    bool setPrecoolDesignHumidityRatio(double precoolDesignHumidityRatio);

    bool setCentralCoolingDesignSupplyAirTemperature(double centralCoolingDesignSupplyAirTemperature);

    bool setCentralHeatingDesignSupplyAirTemperature(double centralHeatingDesignSupplyAirTemperature);

    bool setSizingOption(const std::string& sizingOption);

    void resetSizingOption();

    bool setAllOutdoorAirinCooling(bool allOutdoorAirinCooling);

    void resetAllOutdoorAirinCooling();

    bool setAllOutdoorAirinHeating(bool allOutdoorAirinHeating);

    void resetAllOutdoorAirinHeating();

    bool setCentralCoolingDesignSupplyAirHumidityRatio(double centralCoolingDesignSupplyAirHumidityRatio);

    void resetCentralCoolingDesignSupplyAirHumidityRatio();

    bool setCentralHeatingDesignSupplyAirHumidityRatio(double centralHeatingDesignSupplyAirHumidityRatio);

    void resetCentralHeatingDesignSupplyAirHumidityRatio();

    bool setCoolingDesignAirFlowMethod(const std::string& coolingDesignAirFlowMethod);

    void resetCoolingDesignAirFlowMethod();

    bool setCoolingDesignAirFlowRate(double coolingDesignAirFlowRate);

    void resetCoolingDesignAirFlowRate();

    bool setHeatingDesignAirFlowMethod(const std::string& heatingDesignAirFlowMethod);

    void resetHeatingDesignAirFlowMethod();

    bool setHeatingDesignAirFlowRate(double heatingDesignAirFlowRate);

    void resetHeatingDesignAirFlowRate();

    bool setSystemOutdoorAirMethod(const std::string& systemOutdoorAirMethod);

    void resetSystemOutdoorAirMethod();

    bool setZoneMaximumOutdoorAirFraction(double zoneMaximumOutdoorAirFraction);

    bool setCoolingSupplyAirFlowRatePerFloorArea(double coolingSupplyAirFlowRatePerFloorArea);

    bool setCoolingFractionofAutosizedCoolingSupplyAirFlowRate(double coolingFractionofAutosizedCoolingSupplyAirFlowRate);

    bool setCoolingSupplyAirFlowRatePerUnitCoolingCapacity(double coolingSupplyAirFlowRatePerUnitCoolingCapacity);

    bool setHeatingSupplyAirFlowRatePerFloorArea(double heatingSupplyAirFlowRatePerFloorArea);

    bool setHeatingFractionofAutosizedHeatingSupplyAirFlowRate(double heatingFractionofAutosizedHeatingSupplyAirFlowRate);

    bool setHeatingFractionofAutosizedCoolingSupplyAirFlowRate(double heatingFractionofAutosizedCoolingSupplyAirFlowRate);

    bool setHeatingSupplyAirFlowRatePerUnitHeatingCapacity(double heatingSupplyAirFlowRatePerUnitHeatingCapacity);

    bool setCoolingDesignCapacityMethod(const std::string& coolingDesignCapacityMethod);

    bool setCoolingDesignCapacity(double coolingDesignCapacity);

    void autosizeCoolingDesignCapacity();

    bool setCoolingDesignCapacityPerFloorArea(double coolingDesignCapacityPerFloorArea);

    bool setFractionofAutosizedCoolingDesignCapacity(double fractionofAutosizedCoolingDesignCapacity);

    bool setHeatingDesignCapacityMethod(const std::string& heatingDesignCapacityMethod);

    bool setHeatingDesignCapacity(double heatingDesignCapacity);

    void autosizeHeatingDesignCapacity();

    bool setHeatingDesignCapacityPerFloorArea(double heatingDesignCapacityPerFloorArea);

    bool setFractionofAutosizedHeatingDesignCapacity(double fractionofAutosizedHeatingDesignCapacity);

    bool setCentralCoolingCapacityControlMethod(const std::string& centralCoolingCapacityControlMethod);

    bool setOccupantDiversity(double occupantDiversity);

    void autosizeOccupantDiversity();

    AirLoopHVAC airLoopHVAC() const;

    boost::optional<double> autosizedDesignOutdoorAirFlowRate() const;
    boost::optional<double> autosizedCentralHeatingMaximumSystemAirFlowRatio() const;
    boost::optional<double> autosizedCoolingDesignCapacity() const;
    boost::optional<double> autosizedHeatingDesignCapacity() const;
    boost::optional<double> autosizedOccupantDiversity() const;

    void autosize();

    void applySizingValues();

   protected:
    /// @cond

    bool setAirLoopHVAC(const AirLoopHVAC& airLoopHVAC);

    using ImplType = detail::SizingSystem_Impl;

    explicit SizingSystem(std::shared_ptr<detail::SizingSystem_Impl> impl);

    friend class detail::SizingSystem_Impl;
    friend class Model;
    friend class IdfObject;
    friend class AirLoopHVAC;
    friend class detail::AirLoopHVAC_Impl;
    friend class openstudio::detail::IdfObject_Impl;

    /// @endcond

   private:
    REGISTER_LOGGER("openstudio.model.SizingSystem");
  };

  /** \relates SizingSystem*/
  using OptionalSizingSystem = boost::optional<SizingSystem>;

  /** \relates SizingSystem*/
  using SizingSystemVector = std::vector<SizingSystem>;

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_SIZINGSYSTEM_HPP
