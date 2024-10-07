/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AVAILABILITYMANAGERNIGHTVENTILATION_HPP
#define MODEL_AVAILABILITYMANAGERNIGHTVENTILATION_HPP

#include "ModelAPI.hpp"
#include "AvailabilityManager.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class ThermalZone;

  namespace detail {

    class AvailabilityManagerNightVentilation_Impl;

  }  // namespace detail

  /** AvailabilityManagerNightVentilation is a AvailabilityManager that wraps the OpenStudio IDD object 'OS:AvailabilityManager:NightVentilation'. */
  class MODEL_API AvailabilityManagerNightVentilation : public AvailabilityManager
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit AvailabilityManagerNightVentilation(const Model& model);

    virtual ~AvailabilityManagerNightVentilation() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AvailabilityManagerNightVentilation(const AvailabilityManagerNightVentilation& other) = default;
    AvailabilityManagerNightVentilation(AvailabilityManagerNightVentilation&& other) = default;
    AvailabilityManagerNightVentilation& operator=(const AvailabilityManagerNightVentilation&) = default;
    AvailabilityManagerNightVentilation& operator=(AvailabilityManagerNightVentilation&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    Schedule applicabilitySchedule() const;

    boost::optional<Schedule> ventilationTemperatureSchedule() const;

    double ventilationTemperatureDifference() const;

    double ventilationTemperatureLowLimit() const;

    double nightVentingFlowFraction() const;

    boost::optional<ThermalZone> controlZone() const;

    //@}
    /** @name Setters */
    //@{

    bool setApplicabilitySchedule(Schedule& schedule);

    bool setVentilationTemperatureSchedule(Schedule& schedule);

    void resetVentilationTemperatureSchedule();

    bool setVentilationTemperatureDifference(double ventilationTemperatureDifference);

    bool setVentilationTemperatureLowLimit(double ventilationTemperatureLowLimit);

    bool setNightVentingFlowFraction(double nightVentingFlowFraction);

    bool setControlZone(const ThermalZone& thermalZone);

    void resetControlZone();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::AvailabilityManagerNightVentilation_Impl;

    explicit AvailabilityManagerNightVentilation(std::shared_ptr<detail::AvailabilityManagerNightVentilation_Impl> impl);

    friend class detail::AvailabilityManagerNightVentilation_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AvailabilityManagerNightVentilation");
  };

  /** \relates AvailabilityManagerNightVentilation*/
  using OptionalAvailabilityManagerNightVentilation = boost::optional<AvailabilityManagerNightVentilation>;

  /** \relates AvailabilityManagerNightVentilation*/
  using AvailabilityManagerNightVentilationVector = std::vector<AvailabilityManagerNightVentilation>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AVAILABILITYMANAGERNIGHTVENTILATION_HPP
