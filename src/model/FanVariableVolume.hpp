/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_FANVARIABLEVOLUME_HPP
#define MODEL_FANVARIABLEVOLUME_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class AirflowNetworkFan;
  class FanSystemModel;
  class Schedule;

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

    //@}
    /** @name Other */
    //@{

    AirflowNetworkFan getAirflowNetworkFan();
    boost::optional<AirflowNetworkFan> airflowNetworkFan() const;

    boost::optional<double> autosizedMaximumFlowRate() const;

    // Convenience function to turn this FanVariableVolume to an equivalent FanSystemModel
    FanSystemModel convertToFanSystemModel() const;

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
