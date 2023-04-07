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

#ifndef MODEL_COILHEATINGWATERBASEBOARD_HPP
#define MODEL_COILHEATINGWATERBASEBOARD_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class CoilHeatingWaterBaseboard_Impl;

  }  // namespace detail

  /** CoilHeatingWaterBaseboard is a StraightComponent that wraps the OpenStudio IDD object 'OS:Coil:Heating:Water:Baseboard'. */
  class MODEL_API CoilHeatingWaterBaseboard : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    CoilHeatingWaterBaseboard(const Model& model);

    virtual ~CoilHeatingWaterBaseboard() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilHeatingWaterBaseboard(const CoilHeatingWaterBaseboard& other) = default;
    CoilHeatingWaterBaseboard(CoilHeatingWaterBaseboard&& other) = default;
    CoilHeatingWaterBaseboard& operator=(const CoilHeatingWaterBaseboard&) = default;
    CoilHeatingWaterBaseboard& operator=(CoilHeatingWaterBaseboard&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> heatingDesignCapacityMethodValues();

    /** @name Getters */
    //@{

    unsigned inletPort() const;

    unsigned outletPort() const;

    std::string heatingDesignCapacityMethod() const;

    boost::optional<double> heatingDesignCapacity() const;

    bool isHeatingDesignCapacityAutosized() const;

    double heatingDesignCapacityPerFloorArea() const;

    double fractionofAutosizedHeatingDesignCapacity() const;

    /** Returns the value of the UFactorTimesAreaValue field. **/
    boost::optional<double> uFactorTimesAreaValue() const;

    bool isUFactorTimesAreaValueDefaulted() const;

    /** Returns true if the UFactorTimesAreaValue field is set to autosize **/
    bool isUFactorTimesAreaValueAutosized() const;

    /** Returns the value of the MaximumWaterFlowRate field. **/
    boost::optional<double> maximumWaterFlowRate() const;

    bool isMaximumWaterFlowRateDefaulted() const;

    bool isMaximumWaterFlowRateAutosized() const;

    double convergenceTolerance() const;

    bool isConvergenceToleranceDefaulted() const;

    //@}
    /** @name Setters */
    //@{
    bool setHeatingDesignCapacityMethod(const std::string& heatingDesignCapacityMethod);

    bool setHeatingDesignCapacity(double heatingDesignCapacity);

    void autosizeHeatingDesignCapacity();

    bool setHeatingDesignCapacityPerFloorArea(double heatingDesignCapacityPerFloorArea);

    bool setFractionofAutosizedHeatingDesignCapacity(double fractionofAutosizedHeatingDesignCapacity);

    bool setUFactorTimesAreaValue(double uFactorTimesAreaValue);

    void resetUFactorTimesAreaValue();

    void autosizeUFactorTimesAreaValue();

    bool setMaximumWaterFlowRate(double maximumWaterFlowRate);

    void resetMaximumWaterFlowRate();

    void autosizeMaximumWaterFlowRate();

    bool setConvergenceTolerance(double convergenceTolerance);

    void resetConvergenceTolerance();

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedHeatingDesignCapacity() const;

    boost::optional<double> autosizedUFactorTimesAreaValue() const;

    boost::optional<double> autosizedMaximumWaterFlowRate() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilHeatingWaterBaseboard_Impl;

    explicit CoilHeatingWaterBaseboard(std::shared_ptr<detail::CoilHeatingWaterBaseboard_Impl> impl);

    friend class detail::CoilHeatingWaterBaseboard_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingWaterBaseboard");
  };

  /** \relates CoilHeatingWaterBaseboard*/
  using OptionalCoilHeatingWaterBaseboard = boost::optional<CoilHeatingWaterBaseboard>;

  /** \relates CoilHeatingWaterBaseboard*/
  using CoilHeatingWaterBaseboardVector = std::vector<CoilHeatingWaterBaseboard>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGWATERBASEBOARD_HPP
