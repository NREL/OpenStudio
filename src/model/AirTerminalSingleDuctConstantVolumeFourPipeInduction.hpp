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

#ifndef MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMEFOURPIPEINDUCTION_HPP
#define MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMEFOURPIPEINDUCTION_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class Node;

  namespace detail {

    class AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl;

  }  // namespace detail

  /** AirTerminalSingleDuctConstantVolumeFourPipeInduction is a StraightComponent that wraps the OpenStudio IDD object 'OS:AirTerminal:SingleDuct:ConstantVolume:FourPipeInduction'. */
  class MODEL_API AirTerminalSingleDuctConstantVolumeFourPipeInduction : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit AirTerminalSingleDuctConstantVolumeFourPipeInduction(const Model& model, HVACComponent& heatingCoil);

    virtual ~AirTerminalSingleDuctConstantVolumeFourPipeInduction() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirTerminalSingleDuctConstantVolumeFourPipeInduction(const AirTerminalSingleDuctConstantVolumeFourPipeInduction& other) = default;
    AirTerminalSingleDuctConstantVolumeFourPipeInduction(AirTerminalSingleDuctConstantVolumeFourPipeInduction&& other) = default;
    AirTerminalSingleDuctConstantVolumeFourPipeInduction& operator=(const AirTerminalSingleDuctConstantVolumeFourPipeInduction&) = default;
    AirTerminalSingleDuctConstantVolumeFourPipeInduction& operator=(AirTerminalSingleDuctConstantVolumeFourPipeInduction&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    boost::optional<Schedule> availabilitySchedule() const;

    boost::optional<double> maximumTotalAirFlowRate() const;

    bool isMaximumTotalAirFlowRateAutosized() const;

    double inductionRatio() const;

    bool isInductionRatioDefaulted() const;

    HVACComponent heatingCoil() const;

    boost::optional<double> maximumHotWaterFlowRate() const;

    bool isMaximumHotWaterFlowRateAutosized() const;

    double minimumHotWaterFlowRate() const;

    bool isMinimumHotWaterFlowRateDefaulted() const;

    double heatingConvergenceTolerance() const;

    bool isHeatingConvergenceToleranceDefaulted() const;

    boost::optional<HVACComponent> coolingCoil() const;

    boost::optional<double> maximumColdWaterFlowRate() const;

    bool isMaximumColdWaterFlowRateAutosized() const;

    double minimumColdWaterFlowRate() const;

    bool isMinimumColdWaterFlowRateDefaulted() const;

    double coolingConvergenceTolerance() const;

    bool isCoolingConvergenceToleranceDefaulted() const;

    boost::optional<Node> inducedAirInletNode() const;

    unsigned inducedAirInletPort() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setMaximumTotalAirFlowRate(double maximumTotalAirFlowRate);

    void autosizeMaximumTotalAirFlowRate();

    bool setInductionRatio(double inductionRatio);

    void resetInductionRatio();

    bool setHeatingCoil(const HVACComponent& heatingCoil);

    bool setMaximumHotWaterFlowRate(double maximumHotWaterFlowRate);

    void resetMaximumHotWaterFlowRate();

    void autosizeMaximumHotWaterFlowRate();

    bool setMinimumHotWaterFlowRate(double minimumHotWaterFlowRate);

    void resetMinimumHotWaterFlowRate();

    bool setHeatingConvergenceTolerance(double heatingConvergenceTolerance);

    void resetHeatingConvergenceTolerance();

    bool setCoolingCoil(const HVACComponent& coolingCoil);

    void resetCoolingCoil();

    bool setMaximumColdWaterFlowRate(double maximumColdWaterFlowRate);

    void resetMaximumColdWaterFlowRate();

    void autosizeMaximumColdWaterFlowRate();

    bool setMinimumColdWaterFlowRate(double minimumColdWaterFlowRate);

    void resetMinimumColdWaterFlowRate();

    bool setCoolingConvergenceTolerance(double coolingConvergenceTolerance);

    void resetCoolingConvergenceTolerance();

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedMaximumTotalAirFlowRate() const;

    boost::optional<double> autosizedMaximumHotWaterFlowRate() const;

    boost::optional<double> autosizedMaximumColdWaterFlowRate() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl;

    explicit AirTerminalSingleDuctConstantVolumeFourPipeInduction(
      std::shared_ptr<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl> impl);

    friend class detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirTerminalSingleDuctConstantVolumeFourPipeInduction");
  };

  /** \relates AirTerminalSingleDuctConstantVolumeFourPipeInduction*/
  using OptionalAirTerminalSingleDuctConstantVolumeFourPipeInduction = boost::optional<AirTerminalSingleDuctConstantVolumeFourPipeInduction>;

  /** \relates AirTerminalSingleDuctConstantVolumeFourPipeInduction*/
  using AirTerminalSingleDuctConstantVolumeFourPipeInductionVector = std::vector<AirTerminalSingleDuctConstantVolumeFourPipeInduction>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMEFOURPIPEINDUCTION_HPP
