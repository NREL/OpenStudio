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

#ifndef MODEL_FANVARIABLEVOLUME_HPP
#define MODEL_FANVARIABLEVOLUME_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class AirflowNetworkFan;

  namespace detail {

    class FanVariableVolume_Impl;

  }  // namespace detail

  /** FanVariableVolume is a StraightComponent that wraps the OpenStudio IDD object
 *  'OS:Fan:VariableVolume'. */
  class MODEL_API FanVariableVolume : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    FanVariableVolume(const Model& model, Schedule& schedule);

    FanVariableVolume(const Model& model);

    virtual ~FanVariableVolume() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    FanVariableVolume(const FanVariableVolume& other) = default;
    FanVariableVolume(FanVariableVolume&& other) = default;
    FanVariableVolume& operator=(const FanVariableVolume&) = default;
    FanVariableVolume& operator=(FanVariableVolume&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> fanPowerMinimumFlowRateInputMethodValues();

    /** \deprecated */
    static std::vector<std::string> validFanPowerMinimumFlowRateInputMethodValues();

    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    double fanTotalEfficiency() const;
    double fanEfficiency() const;

    bool isFanTotalEfficiencyDefaulted() const;
    bool isFanEfficiencyDefaulted() const;

    double pressureRise() const;

    boost::optional<double> maximumFlowRate() const;

    bool isMaximumFlowRateAutosized() const;

    std::string fanPowerMinimumFlowRateInputMethod() const;

    bool isFanPowerMinimumFlowRateInputMethodDefaulted() const;

    double fanPowerMinimumFlowFraction() const;

    bool isFanPowerMinimumFlowFractionDefaulted() const;

    boost::optional<double> fanPowerMinimumAirFlowRate() const;

    double motorEfficiency() const;

    bool isMotorEfficiencyDefaulted() const;

    double motorInAirstreamFraction() const;

    bool isMotorInAirstreamFractionDefaulted() const;

    boost::optional<double> fanPowerCoefficient1() const;

    boost::optional<double> fanPowerCoefficient2() const;

    boost::optional<double> fanPowerCoefficient3() const;

    boost::optional<double> fanPowerCoefficient4() const;

    boost::optional<double> fanPowerCoefficient5() const;

    std::string endUseSubcategory() const;

    bool isEndUseSubcategoryDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setFanTotalEfficiency(double fanTotalEfficiency);
    void resetFanTotalEfficiency();

    bool setFanEfficiency(double fanTotalEfficiency);
    void resetFanEfficiency();

    bool setPressureRise(double pressureRise);

    bool setMaximumFlowRate(double maximumFlowRate);

    void resetMaximumFlowRate();

    void autosizeMaximumFlowRate();

    bool setFanPowerMinimumFlowRateInputMethod(const std::string& fanPowerMinimumFlowRateInputMethod);

    void resetFanPowerMinimumFlowRateInputMethod();

    bool setFanPowerMinimumFlowFraction(double fanPowerMinimumFlowFraction);

    void resetFanPowerMinimumFlowFraction();

    bool setFanPowerMinimumAirFlowRate(double fanPowerMinimumAirFlowRate);

    void resetFanPowerMinimumAirFlowRate();

    bool setMotorEfficiency(double motorEfficiency);

    void resetMotorEfficiency();

    bool setMotorInAirstreamFraction(double motorInAirstreamFraction);

    void resetMotorInAirstreamFraction();

    bool setFanPowerCoefficient1(double fanPowerCoefficient1);

    void resetFanPowerCoefficient1();

    bool setFanPowerCoefficient2(double fanPowerCoefficient2);

    void resetFanPowerCoefficient2();

    bool setFanPowerCoefficient3(double fanPowerCoefficient3);

    void resetFanPowerCoefficient3();

    bool setFanPowerCoefficient4(double fanPowerCoefficient4);

    void resetFanPowerCoefficient4();

    bool setFanPowerCoefficient5(double fanPowerCoefficient5);

    void resetFanPowerCoefficient5();

    bool setEndUseSubcategory(const std::string& endUseSubcategory);

    void resetEndUseSubcategory();

    AirflowNetworkFan getAirflowNetworkFan();
    boost::optional<AirflowNetworkFan> airflowNetworkFan() const;

    boost::optional<double> autosizedMaximumFlowRate() const;
    //@}
   protected:
    /// @cond
    using ImplType = detail::FanVariableVolume_Impl;

    friend class detail::FanVariableVolume_Impl;

    friend class Model;

    friend class openstudio::IdfObject;

    explicit FanVariableVolume(std::shared_ptr<detail::FanVariableVolume_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.FanVariableVolume");
  };

  /** \relates FanVariableVolume*/
  using OptionalFanVariableVolume = boost::optional<FanVariableVolume>;

  /** \relates FanVariableVolume*/
  using FanVariableVolumeVector = std::vector<FanVariableVolume>;

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_FANVARIABLEVOLUME_HPP
