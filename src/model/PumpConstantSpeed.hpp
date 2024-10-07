/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PUMPCONSTANTSPEED_HPP
#define MODEL_PUMPCONSTANTSPEED_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class Curve;
  class ThermalZone;

  namespace detail {

    class PumpConstantSpeed_Impl;

  }  // namespace detail

  /** PumpConstantSpeed is a StraightComponent that wraps the OpenStudio IDD object 'OS:Pump:ConstantSpeed'. */
  class MODEL_API PumpConstantSpeed : public StraightComponent
  {
   public:
    explicit PumpConstantSpeed(const Model& model);

    virtual ~PumpConstantSpeed() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    PumpConstantSpeed(const PumpConstantSpeed& other) = default;
    PumpConstantSpeed(PumpConstantSpeed&& other) = default;
    PumpConstantSpeed& operator=(const PumpConstantSpeed&) = default;
    PumpConstantSpeed& operator=(PumpConstantSpeed&&) = default;

    static IddObjectType iddObjectType();

    static std::vector<std::string> pumpControlTypeValues();

    static std::vector<std::string> designPowerSizingMethodValues();

    /** In EnergyPlus 8.5.0 and above this property maps to the EnergyPlus field "Design Flow Rate" **/
    boost::optional<double> ratedFlowRate() const;

    bool isRatedFlowRateAutosized() const;

    bool setRatedFlowRate(double ratedFlowRate);

    void resetRatedFlowRate();

    void autosizeRatedFlowRate();

    /** In EnergyPlus 8.5.0 and above this property maps to the EnergyPlus field "Design Pump Head" **/
    double ratedPumpHead() const;

    bool isRatedPumpHeadDefaulted() const;

    bool setRatedPumpHead(double ratedPumpHead);

    void resetRatedPumpHead();

    /** In EnergyPlus 8.5.0 and above this property maps to the EnergyPlus field "Design Power Consumption" **/
    boost::optional<double> ratedPowerConsumption() const;

    bool isRatedPowerConsumptionAutosized() const;

    bool setRatedPowerConsumption(double ratedPowerConsumption);

    void resetRatedPowerConsumption();

    void autosizeRatedPowerConsumption();

    double motorEfficiency() const;

    bool isMotorEfficiencyDefaulted() const;

    bool setMotorEfficiency(double motorEfficiency);

    void resetMotorEfficiency();

    double fractionofMotorInefficienciestoFluidStream() const;

    bool isFractionofMotorInefficienciestoFluidStreamDefaulted() const;

    bool setFractionofMotorInefficienciestoFluidStream(double fractionofMotorInefficienciestoFluidStream);

    void resetFractionofMotorInefficienciestoFluidStream();

    std::string pumpControlType() const;

    bool isPumpControlTypeDefaulted() const;

    bool setPumpControlType(const std::string& pumpControlType);

    void resetPumpControlType();

    boost::optional<Schedule> pumpFlowRateSchedule() const;

    bool setPumpFlowRateSchedule(Schedule& schedule);

    void resetPumpFlowRateSchedule();

    boost::optional<Curve> pumpCurve() const;

    bool setPumpCurve(const Curve& curve);

    void resetPumpCurve();

    boost::optional<double> impellerDiameter() const;

    bool setImpellerDiameter(double impellerDiameter);

    void resetImpellerDiameter();

    boost::optional<double> rotationalSpeed() const;

    bool setRotationalSpeed(double rotationalSpeed);

    void resetRotationalSpeed();

    // TODO: this should be renamed ThermalZone for consistency
    boost::optional<ThermalZone> zone() const;

    boost::optional<double> skinLossRadiativeFraction() const;

    bool setZone(const ThermalZone& thermalZone);

    void resetZone();

    bool setSkinLossRadiativeFraction(double skinLossRadiativeFraction);

    void resetSkinLossRadiativeFraction();

    boost::optional<double> autosizedRatedFlowRate() const;

    boost::optional<double> autosizedRatedPowerConsumption() const;

    std::string designPowerSizingMethod() const;

    bool setDesignPowerSizingMethod(const std::string& designPowerSizingMethod);

    double designElectricPowerPerUnitFlowRate() const;

    bool setDesignElectricPowerPerUnitFlowRate(double designElectricPowerPerUnitFlowRate);

    double designShaftPowerPerUnitFlowRatePerUnitHead() const;

    bool setDesignShaftPowerPerUnitFlowRatePerUnitHead(double designShaftPowerPerUnitFlowRatePerUnitHead);

    std::string endUseSubcategory() const;

    bool setEndUseSubcategory(const std::string& endUseSubcategory);

   protected:
    /// @cond
    using ImplType = detail::PumpConstantSpeed_Impl;

    explicit PumpConstantSpeed(std::shared_ptr<detail::PumpConstantSpeed_Impl> impl);

    friend class detail::PumpConstantSpeed_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    /// @endcond

   private:
    REGISTER_LOGGER("openstudio.model.PumpConstantSpeed");
  };

  /** \relates PumpConstantSpeed*/
  using OptionalPumpConstantSpeed = boost::optional<PumpConstantSpeed>;

  /** \relates PumpConstantSpeed*/
  using PumpConstantSpeedVector = std::vector<PumpConstantSpeed>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PUMPCONSTANTSPEED_HPP
