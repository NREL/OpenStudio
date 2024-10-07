/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

    virtual ~HeaderedPumpsConstantSpeed() override = default;
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
