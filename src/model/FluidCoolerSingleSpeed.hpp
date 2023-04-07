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

#ifndef MODEL_FLUIDCOOLERSINGLESPEED_HPP
#define MODEL_FLUIDCOOLERSINGLESPEED_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Node;

  namespace detail {

    class FluidCoolerSingleSpeed_Impl;

  }  // namespace detail

  /** FluidCoolerSingleSpeed is a StraightComponent that wraps the OpenStudio IDD object 'OS:FluidCooler:SingleSpeed'. */
  class MODEL_API FluidCoolerSingleSpeed : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit FluidCoolerSingleSpeed(const Model& model);

    virtual ~FluidCoolerSingleSpeed() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    FluidCoolerSingleSpeed(const FluidCoolerSingleSpeed& other) = default;
    FluidCoolerSingleSpeed(FluidCoolerSingleSpeed&& other) = default;
    FluidCoolerSingleSpeed& operator=(const FluidCoolerSingleSpeed&) = default;
    FluidCoolerSingleSpeed& operator=(FluidCoolerSingleSpeed&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> performanceInputMethodValues();

    /** @name Getters */
    //@{

    std::string performanceInputMethod() const;

    boost::optional<double> designAirFlowRateUfactorTimesAreaValue() const;

    bool isDesignAirFlowRateUfactorTimesAreaValueAutosized() const;

    double nominalCapacity() const;

    double designEnteringWaterTemperature() const;

    double designEnteringAirTemperature() const;

    double designEnteringAirWetbulbTemperature() const;

    boost::optional<double> designWaterFlowRate() const;

    bool isDesignWaterFlowRateAutosized() const;

    boost::optional<double> designAirFlowRate() const;

    bool isDesignAirFlowRateAutosized() const;

    boost::optional<double> designAirFlowRateFanPower() const;

    bool isDesignAirFlowRateFanPowerAutosized() const;

    boost::optional<Node> outdoorAirInletNode() const;

    //@}
    /** @name Setters */
    //@{

    bool setPerformanceInputMethod(const std::string& performanceInputMethod);

    bool setDesignAirFlowRateUfactorTimesAreaValue(double designAirFlowRateUfactorTimesAreaValue);

    void autosizeDesignAirFlowRateUfactorTimesAreaValue();

    bool setNominalCapacity(double nominalCapacity);

    bool setDesignEnteringWaterTemperature(double designEnteringWaterTemperature);

    bool setDesignEnteringAirTemperature(double designEnteringAirTemperature);

    bool setDesignEnteringAirWetbulbTemperature(double designEnteringAirWetbulbTemperature);

    bool setDesignWaterFlowRate(double designWaterFlowRate);

    void autosizeDesignWaterFlowRate();

    bool setDesignAirFlowRate(double designAirFlowRate);

    void autosizeDesignAirFlowRate();

    bool setDesignAirFlowRateFanPower(double designAirFlowRateFanPower);

    void autosizeDesignAirFlowRateFanPower();

    bool setOutdoorAirInletNode(const Node& node);

    void resetOutdoorAirInletNode();

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedDesignAirFlowRateUfactorTimesAreaValue() const;

    boost::optional<double> autosizedDesignWaterFlowRate() const;

    boost::optional<double> autosizedDesignAirFlowRate() const;

    boost::optional<double> autosizedDesignAirFlowRateFanPower() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::FluidCoolerSingleSpeed_Impl;

    explicit FluidCoolerSingleSpeed(std::shared_ptr<detail::FluidCoolerSingleSpeed_Impl> impl);

    friend class detail::FluidCoolerSingleSpeed_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.FluidCoolerSingleSpeed");
  };

  /** \relates FluidCoolerSingleSpeed*/
  using OptionalFluidCoolerSingleSpeed = boost::optional<FluidCoolerSingleSpeed>;

  /** \relates FluidCoolerSingleSpeed*/
  using FluidCoolerSingleSpeedVector = std::vector<FluidCoolerSingleSpeed>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_FLUIDCOOLERSINGLESPEED_HPP
