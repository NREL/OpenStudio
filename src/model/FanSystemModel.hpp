/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_FANSYSTEMMODEL_HPP
#define MODEL_FANSYSTEMMODEL_HPP

#include <model/ModelAPI.hpp>
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class Curve;
  class ThermalZone;
  class AirflowNetworkFan;

  namespace detail {

    class FanSystemModel_Impl;

  }  // namespace detail

  /** This class implements a speed */
  class MODEL_API FanSystemModelSpeed
  {
   public:
    FanSystemModelSpeed(double flowFraction);

    FanSystemModelSpeed(double flowFraction, double electricPowerFraction);

    double flowFraction() const;
    boost::optional<double> electricPowerFraction() const;

    bool operator<(const FanSystemModelSpeed& other) const;

   private:
    double m_flowFraction;
    boost::optional<double> m_electricPowerFraction;
    REGISTER_LOGGER("openstudio.model.FanSystemModelSpeed");
  };

  // Overload operator<<
  MODEL_API std::ostream& operator<<(std::ostream& out, const openstudio::model::FanSystemModelSpeed& speed);

  /** FanSystemModel is a StraightComponent that wraps the OpenStudio IDD object 'OS:Fan:SystemModel'. */
  class MODEL_API FanSystemModel : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit FanSystemModel(const Model& model);

    virtual ~FanSystemModel() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    FanSystemModel(const FanSystemModel& other) = default;
    FanSystemModel(FanSystemModel&& other) = default;
    FanSystemModel& operator=(const FanSystemModel&) = default;
    FanSystemModel& operator=(FanSystemModel&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> speedControlMethodValues();

    static std::vector<std::string> designPowerSizingMethodValues();

    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    boost::optional<double> designMaximumAirFlowRate() const;
    bool isDesignMaximumAirFlowRateAutosized() const;
    boost::optional<double> autosizedDesignMaximumAirFlowRate();

    std::string speedControlMethod() const;

    double electricPowerMinimumFlowRateFraction() const;

    double designPressureRise() const;

    double motorEfficiency() const;

    double motorInAirStreamFraction() const;

    boost::optional<double> designElectricPowerConsumption() const;
    bool isDesignElectricPowerConsumptionAutosized() const;
    boost::optional<double> autosizedDesignElectricPowerConsumption();

    std::string designPowerSizingMethod() const;

    double electricPowerPerUnitFlowRate() const;

    double electricPowerPerUnitFlowRatePerUnitPressure() const;

    double fanTotalEfficiency() const;

    boost::optional<Curve> electricPowerFunctionofFlowFractionCurve() const;

    boost::optional<double> nightVentilationModePressureRise() const;

    boost::optional<double> nightVentilationModeFlowFraction() const;

    boost::optional<ThermalZone> motorLossZone() const;

    double motorLossRadiativeFraction() const;

    std::string endUseSubcategory() const;

    unsigned numberofSpeeds() const;
    std::vector<FanSystemModelSpeed> speeds() const;

    /** Find a given speed by the flowFraction (key) */
    boost::optional<unsigned> speedIndex(const FanSystemModelSpeed& speed) const;

    boost::optional<FanSystemModelSpeed> getSpeed(unsigned speedIndex) const;

    //@}
    /** @name Setters */
    //@{

    /** If a speed group is already present (= the flowFraction already exists) (cf `speedIndex()`), it will Warn and override the electricPowerFraction value */
    bool addSpeed(const FanSystemModelSpeed& speed);

    // This one leaves the electricPowerFraction blank, so the electricPowerFunctionofFlowFractionCurve is used.
    bool addSpeed(double flowFraction);

    // Overloads, it creates a FanSystemModelSpeed wrapper, then call `addSpeed(const FanSystemModelSpeed&)`
    // This one specifies both
    bool addSpeed(double flowFraction, double electricPowerFraction);

    bool removeSpeed(unsigned speedIndex);

    void removeAllSpeeds();

    // Directly set the speeds from a vector, will delete any existing speeds. Will sort out the speeds too, ascending by flowFraction
    bool setSpeeds(const std::vector<FanSystemModelSpeed>& speeds);

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setDesignMaximumAirFlowRate(double designMaximumAirFlowRate);
    void autosizeDesignMaximumAirFlowRate();

    bool setSpeedControlMethod(const std::string& speedControlMethod);

    bool setElectricPowerMinimumFlowRateFraction(double electricPowerMinimumFlowRateFraction);

    bool setDesignPressureRise(double designPressureRise);

    bool setMotorEfficiency(double motorEfficiency);

    bool setMotorInAirStreamFraction(double motorInAirStreamFraction);

    bool setDesignElectricPowerConsumption(double designElectricPowerConsumption);
    void autosizeDesignElectricPowerConsumption();

    bool setDesignPowerSizingMethod(const std::string& designPowerSizingMethod);

    bool setElectricPowerPerUnitFlowRate(double electricPowerPerUnitFlowRate);

    bool setElectricPowerPerUnitFlowRatePerUnitPressure(double electricPowerPerUnitFlowRatePerUnitPressure);

    bool setFanTotalEfficiency(double fanTotalEfficiency);

    bool setElectricPowerFunctionofFlowFractionCurve(const Curve& univariateFunctions);
    void resetElectricPowerFunctionofFlowFractionCurve();

    bool setNightVentilationModePressureRise(double nightVentilationModePressureRise);
    void resetNightVentilationModePressureRise();

    bool setNightVentilationModeFlowFraction(double nightVentilationModeFlowFraction);
    void resetNightVentilationModeFlowFraction();

    bool setMotorLossZone(const ThermalZone& thermalZone);
    void resetMotorLossZone();

    bool setMotorLossRadiativeFraction(double motorLossRadiativeFraction);

    bool setEndUseSubcategory(const std::string& endUseSubcategory);

    //@}
    /** @name Other */
    //@{

    AirflowNetworkFan getAirflowNetworkFan();
    boost::optional<AirflowNetworkFan> airflowNetworkFan() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::FanSystemModel_Impl;

    explicit FanSystemModel(std::shared_ptr<detail::FanSystemModel_Impl> impl);

    friend class detail::FanSystemModel_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.FanSystemModel");
  };

  /** \relates FanSystemModel*/
  using OptionalFanSystemModel = boost::optional<FanSystemModel>;

  /** \relates FanSystemModel*/
  using FanSystemModelVector = std::vector<FanSystemModel>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_FANSYSTEMMODEL_HPP
