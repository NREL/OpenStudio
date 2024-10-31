/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_FANONOFF_HPP
#define MODEL_FANONOFF_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class Curve;
  class AirflowNetworkFan;

  namespace detail {

    class FanOnOff_Impl;

  }  // namespace detail

  /** FanOnOff is a StraightComponent that wraps the OpenStudio IDD object 'OS:Fan:OnOff'. */
  class MODEL_API FanOnOff : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit FanOnOff(const Model& model, Schedule& availabilitySchedule, Curve& fanPowerRatioFunctionofSpeedRatioCurve,
                      Curve& fanEfficiencyRatioFunctionofSpeedRatioCurve);

    explicit FanOnOff(const Model& model, Schedule& availabilitySchedule);

    explicit FanOnOff(const Model& model);

    virtual ~FanOnOff() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    FanOnOff(const FanOnOff& other) = default;
    FanOnOff(FanOnOff&& other) = default;
    FanOnOff& operator=(const FanOnOff&) = default;
    FanOnOff& operator=(FanOnOff&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    double fanTotalEfficiency() const;
    bool isFanTotalEfficiencyDefaulted() const;

    /** Deprecrated in favor of fanTotalEfficiency **/
    double fanEfficiency() const;
    bool isFanEfficiencyDefaulted() const;

    double pressureRise() const;

    boost::optional<double> maximumFlowRate() const;

    bool isMaximumFlowRateAutosized() const;

    double motorEfficiency() const;

    bool isMotorEfficiencyDefaulted() const;

    boost::optional<double> motorInAirstreamFraction() const;

    std::string endUseSubcategory() const;

    bool isEndUseSubcategoryDefaulted() const;

    /** @name Setters */

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setFanTotalEfficiency(double fanTotalEfficiency);
    void resetFanTotalEfficiency();

    bool setFanEfficiency(double fanTotalEfficiency);
    void resetFanEfficiency();

    bool setPressureRise(double pressureRise);

    bool setMaximumFlowRate(double maximumFlowRate);

    void resetMaximumFlowRate();

    void autosizeMaximumFlowRate();

    bool setMotorEfficiency(double motorEfficiency);

    void resetMotorEfficiency();

    bool setMotorInAirstreamFraction(double motorInAirstreamFraction);

    void resetMotorInAirstreamFraction();

    bool setEndUseSubcategory(const std::string& endUseSubcategory);

    void resetEndUseSubcategory();

    //@}

    /** @name Curves */

    Curve fanPowerRatioFunctionofSpeedRatioCurve() const;
    bool setFanPowerRatioFunctionofSpeedRatioCurve(const Curve& curve);

    Curve fanEfficiencyRatioFunctionofSpeedRatioCurve() const;
    bool setFanEfficiencyRatioFunctionofSpeedRatioCurve(const Curve& curve);

    //@{

    AirflowNetworkFan getAirflowNetworkFan();
    boost::optional<AirflowNetworkFan> airflowNetworkFan() const;

    boost::optional<double> autosizedMaximumFlowRate() const;
    //@}
   protected:
    /// @cond
    using ImplType = detail::FanOnOff_Impl;

    explicit FanOnOff(std::shared_ptr<detail::FanOnOff_Impl> impl);

    friend class detail::FanOnOff_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.FanOnOff");
  };

  /** \relates FanOnOff*/
  using OptionalFanOnOff = boost::optional<FanOnOff>;

  /** \relates FanOnOff*/
  using FanOnOffVector = std::vector<FanOnOff>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_FANONOFF_HPP
