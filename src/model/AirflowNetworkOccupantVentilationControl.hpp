/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKOCCUPANTVENTILATIONCONTROL_HPP
#define MODEL_AIRFLOWNETWORKOCCUPANTVENTILATIONCONTROL_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  // TODO: Check the following class names against object getters and setters.
  class Curve;
  class Schedule;

  namespace detail {

    class AirflowNetworkOccupantVentilationControl_Impl;

  }  // namespace detail

  /** AirflowNetworkOccupantVentilationControl is a ModelObject that wraps the OpenStudio IDD object 'OS:AirflowNetworkOccupantVentilationControl'. */
  class MODEL_API AirflowNetworkOccupantVentilationControl : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{
    // Remove this ctor unless a suitable default curve can be found
    // explicit AirflowNetworkOccupantVentilationControl(const Model& model);
    /** Construct an occupant ventilation control object with a low temperature curve. */
    AirflowNetworkOccupantVentilationControl(const Model& model, const Curve& curve);

    virtual ~AirflowNetworkOccupantVentilationControl() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirflowNetworkOccupantVentilationControl(const AirflowNetworkOccupantVentilationControl& other) = default;
    AirflowNetworkOccupantVentilationControl(AirflowNetworkOccupantVentilationControl&& other) = default;
    AirflowNetworkOccupantVentilationControl& operator=(const AirflowNetworkOccupantVentilationControl&) = default;
    AirflowNetworkOccupantVentilationControl& operator=(AirflowNetworkOccupantVentilationControl&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{
    /** Returns the minimum opening time. */
    double minimumOpeningTime() const;
    /** Returns true if the minimum opening time is defaulted. */
    bool isMinimumOpeningTimeDefaulted() const;
    /** Returns the minimum closing time. */
    double minimumClosingTime() const;
    /** Returns true if the minimum closing time is defaulted. */
    bool isMinimumClosingTimeDefaulted() const;

    /** Returns the low temperature thermal comfort curve. */
    Curve thermalComfortLowTemperatureCurve() const;
    /** Returns the boundary temperature between the low and high temperature thermal comfort curves. */
    double thermalComfortTemperatureBoundaryPoint() const;
    /** Returns true if the boundary temperature is defaulted. */
    bool isThermalComfortTemperatureBoundaryPointDefaulted() const;

    /** Returns the high temperature thermal comfort curve. */
    boost::optional<Curve> thermalComfortHighTemperatureCurve() const;
    /** Returns the maximum PPD threshold. */
    double maximumPredictedPercentageofDissatisfiedThreshold() const;
    /** Returns true if the maximum PPD threshold is defaulted. */
    bool isMaximumPredictedPercentageofDissatisfiedThresholdDefaulted() const;
    /** Returns true if an occupancy check is to be performed as part of the opening probability check. */
    bool occupancyCheck() const;
    /** Returns true if the occupancy check is defaulted. */
    bool isOccupancyCheckDefaulted() const;

    /** Returns the opening probability schedule, if any. */
    boost::optional<Schedule> openingProbabilitySchedule() const;

    /** Returns the closing probability schedule, if any. */
    boost::optional<Schedule> closingProbabilitySchedule() const;

    //@}
    /** @name Setters */
    //@{
    /** Sets the minimum opening time. */
    bool setMinimumOpeningTime(double minimumOpeningTime);
    /** Resets the minimum opening time. */
    void resetMinimumOpeningTime();
    /** Sets the minimum closing time. */
    bool setMinimumClosingTime(double minimumClosingTime);
    /** Resets the minimum closing time. */
    void resetMinimumClosingTime();

    /** Sets the low temperature thermal comfort curve. */
    bool setThermalComfortLowTemperatureCurve(const Curve& curve);

    //void resetThermalComfortLowTemperatureCurve();
    /** Sets the temperature boundary point. */
    bool setThermalComfortTemperatureBoundaryPoint(double thermalComfortTemperatureBoundaryPoint);
    /** Resets the temperature boundary point. */
    void resetThermalComfortTemperatureBoundaryPoint();

    /** Sets the high temperature thermal comfort curve. */
    bool setThermalComfortHighTemperatureCurve(const Curve& curve);
    /** Resets the high temperature thermal comfort curve. */
    void resetThermalComfortHighTemperatureCurve();
    /** Sets the maximum PPD threshold. */
    bool setMaximumPredictedPercentageofDissatisfiedThreshold(double maximumPredictedPercentageofDissatisfiedThreshold);
    /** Resets the maximum PPD threshold. */
    void resetMaximumPredictedPercentageofDissatisfiedThreshold();
    /** Sets the occupancy check. */
    void setOccupancyCheck(bool occupancyCheck);
    /** Resets the occupancy check. */
    void resetOccupancyCheck();

    /** Sets the opening probability schedule. */
    bool setOpeningProbabilitySchedule(Schedule& schedule);
    /** Resets the opening probability schedule. */
    void resetOpeningProbabilitySchedule();

    /** Sets the closing probability schedule. */
    bool setClosingProbabilitySchedule(Schedule& schedule);
    /** Resets the closing probability schedule. */
    void resetClosingProbabilitySchedule();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirflowNetworkOccupantVentilationControl_Impl;

    explicit AirflowNetworkOccupantVentilationControl(std::shared_ptr<detail::AirflowNetworkOccupantVentilationControl_Impl> impl);

    friend class detail::AirflowNetworkOccupantVentilationControl_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirflowNetworkOccupantVentilationControl");
  };

  /** \relates AirflowNetworkOccupantVentilationControl*/
  using OptionalAirflowNetworkOccupantVentilationControl = boost::optional<AirflowNetworkOccupantVentilationControl>;

  /** \relates AirflowNetworkOccupantVentilationControl*/
  using AirflowNetworkOccupantVentilationControlVector = std::vector<AirflowNetworkOccupantVentilationControl>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKOCCUPANTVENTILATIONCONTROL_HPP
