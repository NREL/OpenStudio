/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AVAILABILITYMANAGERHYBRIDVENTILATION_HPP
#define MODEL_AVAILABILITYMANAGERHYBRIDVENTILATION_HPP

#include "ModelAPI.hpp"
#include "AvailabilityManager.hpp"

namespace openstudio {

namespace model {

  class ThermalZone;
  class Schedule;
  class Curve;
  class ModelObject;

  namespace detail {

    class AvailabilityManagerHybridVentilation_Impl;

  }  // namespace detail

  /** AvailabilityManagerHybridVentilation is a ModelObject that wraps the OpenStudio IDD object 'OS:AvailabilityManager:HybridVentilation'. */
  class MODEL_API AvailabilityManagerHybridVentilation : public AvailabilityManager
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    // Constructor for AvailabilityManagerHybridVentilation.
    // The required "ventilationControlModeSchedule" will be initialized with
    // a new schedule having a constant value of one, meaning the availability manager
    // will operate with temperature control.
    // The required "minimumOutdoorVentilationAirSchedule" will be initialized with a
    // schedule of constant value 0. This schedule is only applicable if control mode is 0,
    // but EnergyPlus requires it to operate.
    explicit AvailabilityManagerHybridVentilation(const Model& model);

    // Constructor for AvailabilityManagerHybridVentilation.
    // The required "ventilationControlModeSchedule" must be provided.
    // The required "minimumOutdoorVentilationAirSchedule" must be provided.
    explicit AvailabilityManagerHybridVentilation(const Model& model, Schedule& ventilationControlModeSchedule,
                                                  Schedule& minimumOutdoorVentilationAirSchedule);

    virtual ~AvailabilityManagerHybridVentilation() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AvailabilityManagerHybridVentilation(const AvailabilityManagerHybridVentilation& other) = default;
    AvailabilityManagerHybridVentilation(AvailabilityManagerHybridVentilation&& other) = default;
    AvailabilityManagerHybridVentilation& operator=(const AvailabilityManagerHybridVentilation&) = default;
    AvailabilityManagerHybridVentilation& operator=(AvailabilityManagerHybridVentilation&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    boost::optional<ThermalZone> controlledZone() const;

    Schedule ventilationControlModeSchedule() const;

    bool useWeatherFileRainIndicators() const;

    double maximumWindSpeed() const;

    double minimumOutdoorTemperature() const;

    double maximumOutdoorTemperature() const;

    double minimumOutdoorEnthalpy() const;

    double maximumOutdoorEnthalpy() const;

    double minimumOutdoorDewpoint() const;

    double maximumOutdoorDewpoint() const;

    double minimumHVACOperationTime() const;

    double minimumVentilationTime() const;

    Schedule minimumOutdoorVentilationAirSchedule() const;

    boost::optional<Curve> openingFactorFunctionofWindSpeedCurve() const;

    boost::optional<Schedule> airflowNetworkControlTypeSchedule() const;

    boost::optional<Schedule> simpleAirflowControlTypeSchedule() const;

    boost::optional<ModelObject> zoneVentilationObject() const;

    //@}
    /** @name Setters */
    //@{

    bool setControlledZone(const ThermalZone& thermalZone);

    void resetControlledZone();

    bool setVentilationControlModeSchedule(Schedule& schedule);

    bool setUseWeatherFileRainIndicators(bool useWeatherFileRainIndicators);

    bool setMaximumWindSpeed(double maximumWindSpeed);

    bool setMinimumOutdoorTemperature(double minimumOutdoorTemperature);

    bool setMaximumOutdoorTemperature(double maximumOutdoorTemperature);

    bool setMinimumOutdoorEnthalpy(double minimumOutdoorEnthalpy);

    bool setMaximumOutdoorEnthalpy(double maximumOutdoorEnthalpy);

    bool setMinimumOutdoorDewpoint(double minimumOutdoorDewpoint);

    bool setMaximumOutdoorDewpoint(double maximumOutdoorDewpoint);

    bool setMinimumOutdoorVentilationAirSchedule(Schedule& schedule);

    bool setOpeningFactorFunctionofWindSpeedCurve(const Curve& curve);

    void resetOpeningFactorFunctionofWindSpeedCurve();

    bool setMinimumHVACOperationTime(double minimumHVACOperationTime);

    bool setMinimumVentilationTime(double minimumVentilationTime);

    bool setAirflowNetworkControlTypeSchedule(Schedule& schedule);
    void resetAirflowNetworkControlTypeSchedule();

    bool setSimpleAirflowControlTypeSchedule(Schedule& schedule);
    void resetSimpleAirflowControlTypeSchedule();

    bool setZoneVentilationObject(const ModelObject& zv);
    void resetZoneVentilationObject();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::AvailabilityManagerHybridVentilation_Impl;

    explicit AvailabilityManagerHybridVentilation(std::shared_ptr<detail::AvailabilityManagerHybridVentilation_Impl> impl);

    friend class detail::AvailabilityManagerHybridVentilation_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AvailabilityManagerHybridVentilation");
  };

  /** \relates AvailabilityManagerHybridVentilation*/
  using OptionalAvailabilityManagerHybridVentilation = boost::optional<AvailabilityManagerHybridVentilation>;

  /** \relates AvailabilityManagerHybridVentilation*/
  using AvailabilityManagerHybridVentilationVector = std::vector<AvailabilityManagerHybridVentilation>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AVAILABILITYMANAGERHYBRIDVENTILATION_HPP
