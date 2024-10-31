/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

    virtual ~AirTerminalSingleDuctConstantVolumeFourPipeInduction() override = default;
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
