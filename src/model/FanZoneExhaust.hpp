/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_FANZONEEXHAUST_HPP
#define MODEL_FANZONEEXHAUST_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class AirflowNetworkZoneExhaustFan;
  class AirflowNetworkCrack;

  namespace detail {

    class FanZoneExhaust_Impl;

  }  // namespace detail

  /** FanZoneExhaust is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:Fan:ZoneExhaust'. */
  class MODEL_API FanZoneExhaust : public ZoneHVACComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit FanZoneExhaust(const Model& model);

    virtual ~FanZoneExhaust() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    FanZoneExhaust(const FanZoneExhaust& other) = default;
    FanZoneExhaust(FanZoneExhaust&& other) = default;
    FanZoneExhaust& operator=(const FanZoneExhaust&) = default;
    FanZoneExhaust& operator=(FanZoneExhaust&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> systemAvailabilityManagerCouplingModeValues();

    /** @name Getters */
    //@{

    boost::optional<Schedule> availabilitySchedule() const;

    double fanTotalEfficiency() const;

    /** Deprecated, forwards to fanTotalEfficiency */
    double fanEfficiency() const;

    double pressureRise() const;

    boost::optional<double> maximumFlowRate() const;

    std::string endUseSubcategory() const;

    boost::optional<Schedule> flowFractionSchedule() const;

    std::string systemAvailabilityManagerCouplingMode() const;

    boost::optional<Schedule> minimumZoneTemperatureLimitSchedule() const;

    boost::optional<Schedule> balancedExhaustFractionSchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setFanTotalEfficiency(double fanTotalEfficiency);

    /** Deprecated, forwards to setFanTotalEfficiency */
    bool setFanEfficiency(double fanTotalEfficiency);

    bool setPressureRise(double pressureRise);

    bool setMaximumFlowRate(double maximumFlowRate);

    void resetMaximumFlowRate();

    bool setEndUseSubcategory(const std::string& endUseSubcategory);

    bool setFlowFractionSchedule(Schedule& schedule);

    void resetFlowFractionSchedule();

    bool setSystemAvailabilityManagerCouplingMode(const std::string& systemAvailabilityManagerCouplingMode);

    bool setMinimumZoneTemperatureLimitSchedule(Schedule& schedule);

    void resetMinimumZoneTemperatureLimitSchedule();

    bool setBalancedExhaustFractionSchedule(Schedule& schedule);

    void resetBalancedExhaustFractionSchedule();

    //@}
    /** @name Other */
    //@{

    AirflowNetworkZoneExhaustFan getAirflowNetworkZoneExhaustFan(const AirflowNetworkCrack& crack);
    boost::optional<AirflowNetworkZoneExhaustFan> airflowNetworkZoneExhaustFan() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::FanZoneExhaust_Impl;

    explicit FanZoneExhaust(std::shared_ptr<detail::FanZoneExhaust_Impl> impl);

    friend class detail::FanZoneExhaust_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.FanZoneExhaust");
  };

  /** \relates FanZoneExhaust*/
  using OptionalFanZoneExhaust = boost::optional<FanZoneExhaust>;

  /** \relates FanZoneExhaust*/
  using FanZoneExhaustVector = std::vector<FanZoneExhaust>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_FANZONEEXHAUST_HPP
