/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PUMPVARIABLESPEEDCONDENSATE_HPP
#define MODEL_PUMPVARIABLESPEEDCONDENSATE_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class ThermalZone;

  namespace detail {

    class PumpVariableSpeedCondensate_Impl;

  }  // namespace detail

  /** PumpVariableSpeedCondensate is a StraightComponent that wraps the OpenStudio IDD object
 *  'OS:Pump:VariableSpeed:Condensate'. */
  class MODEL_API PumpVariableSpeedCondensate : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit PumpVariableSpeedCondensate(const Model& model);

    virtual ~PumpVariableSpeedCondensate() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    PumpVariableSpeedCondensate(const PumpVariableSpeedCondensate& other) = default;
    PumpVariableSpeedCondensate(PumpVariableSpeedCondensate&& other) = default;
    PumpVariableSpeedCondensate& operator=(const PumpVariableSpeedCondensate&) = default;
    PumpVariableSpeedCondensate& operator=(PumpVariableSpeedCondensate&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> designPowerSizingMethodValues();

    /** @name Getters */
    //@{

    boost::optional<double> ratedSteamVolumeFlowRate() const;

    bool isRatedSteamVolumeFlowRateAutosized() const;

    double ratedPumpHead() const;

    boost::optional<double> ratedPowerConsumption() const;

    bool isRatedPowerConsumptionAutosized() const;

    double motorEfficiency() const;

    double fractionofMotorInefficienciestoFluidStream() const;

    double coefficient1ofthePartLoadPerformanceCurve() const;

    double coefficient2ofthePartLoadPerformanceCurve() const;

    double coefficient3ofthePartLoadPerformanceCurve() const;

    double coefficient4ofthePartLoadPerformanceCurve() const;

    boost::optional<Schedule> pumpFlowRateSchedule() const;

    boost::optional<ThermalZone> zone() const;

    double skinLossRadiativeFraction() const;

    std::string designPowerSizingMethod() const;

    double designElectricPowerPerUnitFlowRate() const;

    double designShaftPowerPerUnitFlowRatePerUnitHead() const;

    std::string endUseSubcategory() const;

    //@}
    /** @name Setters */
    //@{

    bool setRatedSteamVolumeFlowRate(double ratedSteamVolumeFlowRate);

    void resetRatedSteamVolumeFlowRate();

    void autosizeRatedSteamVolumeFlowRate();

    bool setRatedPumpHead(double ratedPumpHead);

    bool setRatedPowerConsumption(double ratedPowerConsumption);

    void resetRatedPowerConsumption();

    void autosizeRatedPowerConsumption();

    bool setMotorEfficiency(double motorEfficiency);

    bool setFractionofMotorInefficienciestoFluidStream(double fractionofMotorInefficienciestoFluidStream);

    bool setCoefficient1ofthePartLoadPerformanceCurve(double coefficient1ofthePartLoadPerformanceCurve);

    bool setCoefficient2ofthePartLoadPerformanceCurve(double coefficient2ofthePartLoadPerformanceCurve);

    bool setCoefficient3ofthePartLoadPerformanceCurve(double coefficient3ofthePartLoadPerformanceCurve);

    bool setCoefficient4ofthePartLoadPerformanceCurve(double coefficient4ofthePartLoadPerformanceCurve);

    bool setPumpFlowRateSchedule(Schedule& schedule);

    void resetPumpFlowRateSchedule();

    bool setDesignPowerSizingMethod(const std::string& designPowerSizingMethod);

    bool setDesignElectricPowerPerUnitFlowRate(double designElectricPowerPerUnitFlowRate);

    bool setDesignShaftPowerPerUnitFlowRatePerUnitHead(double designShaftPowerPerUnitFlowRatePerUnitHead);

    bool setZone(const ThermalZone& thermalZone);

    void resetZone();

    bool setSkinLossRadiativeFraction(double skinLossRadiativeFraction);

    bool setEndUseSubcategory(const std::string& endUseSubcategory);

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedRatedSteamVolumeFlowRate() const;

    boost::optional<double> autosizedRatedPowerConsumption() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::PumpVariableSpeedCondensate_Impl;

    friend class detail::PumpVariableSpeedCondensate_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    explicit PumpVariableSpeedCondensate(std::shared_ptr<detail::PumpVariableSpeedCondensate_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.PumpVariableSpeedCondensate");
  };

  /** \relates PumpVariableSpeedCondensate*/
  using OptionalPumpVariableSpeedCondensate = boost::optional<PumpVariableSpeedCondensate>;

  /** \relates PumpVariableSpeedCondensate*/
  using PumpVariableSpeedCondensateVector = std::vector<PumpVariableSpeedCondensate>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PUMPVARIABLESPEEDCONDENSATE_HPP
