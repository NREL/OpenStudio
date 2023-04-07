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

#ifndef MODEL_HEADEREDPUMPSCONSTANTSPEED_HPP
#define MODEL_HEADEREDPUMPSCONSTANTSPEED_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class ThermalZone;

  namespace detail {

    class HeaderedPumpsConstantSpeed_Impl;

  }  // namespace detail

  /** HeaderedPumpsConstantSpeed is a StraightComponent that wraps the OpenStudio IDD object 'OS:HeaderedPumps:ConstantSpeed'. */
  class MODEL_API HeaderedPumpsConstantSpeed : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit HeaderedPumpsConstantSpeed(const Model& model);

    virtual ~HeaderedPumpsConstantSpeed() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    HeaderedPumpsConstantSpeed(const HeaderedPumpsConstantSpeed& other) = default;
    HeaderedPumpsConstantSpeed(HeaderedPumpsConstantSpeed&& other) = default;
    HeaderedPumpsConstantSpeed& operator=(const HeaderedPumpsConstantSpeed&) = default;
    HeaderedPumpsConstantSpeed& operator=(HeaderedPumpsConstantSpeed&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> flowSequencingControlSchemeValues();

    static std::vector<std::string> pumpControlTypeValues();

    static std::vector<std::string> designPowerSizingMethodValues();

    /** @name Getters */
    //@{

    /** In EnergyPlus 8.5.0 and above this property maps to the EnergyPlus field "Total Design Flow Rate" **/
    boost::optional<double> totalRatedFlowRate() const;

    bool isTotalRatedFlowRateAutosized() const;

    int numberofPumpsinBank() const;

    std::string flowSequencingControlScheme() const;

    /** In EnergyPlus 8.5.0 and above this property maps to the EnergyPlus field "Design Pump Head" **/
    double ratedPumpHead() const;

    /** In EnergyPlus 8.5.0 and above this property maps to the EnergyPlus field "Design Power Consumption" **/
    boost::optional<double> ratedPowerConsumption() const;

    bool isRatedPowerConsumptionAutosized() const;

    double motorEfficiency() const;

    double fractionofMotorInefficienciestoFluidStream() const;

    std::string pumpControlType() const;

    boost::optional<Schedule> pumpFlowRateSchedule() const;

    boost::optional<ThermalZone> thermalZone() const;

    double skinLossRadiativeFraction() const;

    std::string designPowerSizingMethod() const;

    double designElectricPowerPerUnitFlowRate() const;

    double designShaftPowerPerUnitFlowRatePerUnitHead() const;

    std::string endUseSubcategory() const;

    //@}
    /** @name Setters */
    //@{

    bool setTotalRatedFlowRate(double totalRatedFlowRate);

    void autosizeTotalRatedFlowRate();

    bool setNumberofPumpsinBank(int numberofPumpsinBank);

    bool setFlowSequencingControlScheme(const std::string& flowSequencingControlScheme);

    bool setRatedPumpHead(double ratedPumpHead);

    bool setRatedPowerConsumption(double ratedPowerConsumption);

    void autosizeRatedPowerConsumption();

    bool setMotorEfficiency(double motorEfficiency);

    bool setFractionofMotorInefficienciestoFluidStream(double fractionofMotorInefficienciestoFluidStream);

    bool setPumpControlType(const std::string& pumpControlType);

    bool setPumpFlowRateSchedule(Schedule& schedule);

    void resetPumpFlowRateSchedule();

    bool setThermalZone(const ThermalZone& thermalZone);

    void resetThermalZone();

    bool setSkinLossRadiativeFraction(double skinLossRadiativeFraction);

    bool setDesignPowerSizingMethod(const std::string& designPowerSizingMethod);

    bool setDesignElectricPowerPerUnitFlowRate(double designElectricPowerPerUnitFlowRate);

    bool setDesignShaftPowerPerUnitFlowRatePerUnitHead(double designShaftPowerPerUnitFlowRatePerUnitHead);

    bool setEndUseSubcategory(const std::string& endUseSubcategory);

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedTotalRatedFlowRate() const;

    boost::optional<double> autosizedRatedPowerConsumption() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::HeaderedPumpsConstantSpeed_Impl;

    explicit HeaderedPumpsConstantSpeed(std::shared_ptr<detail::HeaderedPumpsConstantSpeed_Impl> impl);

    friend class detail::HeaderedPumpsConstantSpeed_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.HeaderedPumpsConstantSpeed");
  };

  /** \relates HeaderedPumpsConstantSpeed*/
  using OptionalHeaderedPumpsConstantSpeed = boost::optional<HeaderedPumpsConstantSpeed>;

  /** \relates HeaderedPumpsConstantSpeed*/
  using HeaderedPumpsConstantSpeedVector = std::vector<HeaderedPumpsConstantSpeed>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_HEADEREDPUMPSCONSTANTSPEED_HPP
