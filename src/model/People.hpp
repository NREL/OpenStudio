/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PEOPLE_HPP
#define MODEL_PEOPLE_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadInstance.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class PeopleDefinition;

  namespace detail {

    class People_Impl;

  }  // namespace detail

  /** People is a SpaceLoadInstance that wraps the OpenStudio IDD object 'OS_People'. Its fields are
 *  derived from the EnergyPlus IDD object 'People'. \sa PeopleDefinition
 */
  class MODEL_API People : public SpaceLoadInstance
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit People(const PeopleDefinition& peopleDefinition);

    virtual ~People() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    People(const People& other) = default;
    People(People&& other) = default;
    People& operator=(const People&) = default;
    People& operator=(People&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    /** Gets the PeopleDefinition object. */
    PeopleDefinition peopleDefinition() const;

    /** Returns the number of people schedule.  If this object does not
   *  specify a schedule this function will search the hierarchy. */
    boost::optional<Schedule> numberofPeopleSchedule() const;

    /** Returns true if this object does not specify a number of people schedule directly. */
    bool isNumberofPeopleScheduleDefaulted() const;

    /** Returns the activity schedule.  If this object does not
   *  specify a schedule this function will search the hierarchy. */
    boost::optional<Schedule> activityLevelSchedule() const;

    /** Returns true if this object does not specify an activity directly. */
    bool isActivityLevelScheduleDefaulted() const;

    /** Returns the work efficiency schedule. */
    boost::optional<Schedule> workEfficiencySchedule() const;

    /** Returns the clothing insulation schedule. */
    boost::optional<Schedule> clothingInsulationSchedule() const;

    /** Returns the air velocity schedule. */
    boost::optional<Schedule> airVelocitySchedule() const;

    /** Returns the ankle-level air velocity schedule. */
    boost::optional<Schedule> ankleLevelAirVelocitySchedule() const;

    double coldStressTemperatureThreshold() const;
    bool isColdStressTemperatureThresholdDefaulted() const;

    double heatStressTemperatureThreshold() const;
    bool isHeatStressTemperatureThresholdDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    /** Sets the PeopleDefinition object. */
    bool setPeopleDefinition(const PeopleDefinition& definition);

    /** Sets the number of people Schedule. Returns false if schedule has a ScheduleTypeLimits
   *  that is incompatible with this use. Sets the ScheduleTypeLimits of schedule if there is
   *  none. */
    bool setNumberofPeopleSchedule(Schedule& schedule);

    /** Resets the number of people Schedule. */
    void resetNumberofPeopleSchedule();

    /** Sets the activity Schedule. Returns false if schedule has a ScheduleTypeLimits
   *  that is incompatible with this use. Sets the ScheduleTypeLimits of schedule if there is
   *  none. */
    bool setActivityLevelSchedule(Schedule& schedule);

    /** Resets the activity Schedule. */
    void resetActivityLevelSchedule();

    /** Sets the work efficiency Schedule. Returns false if schedule has a ScheduleTypeLimits
   *  that is incompatible with this use. Sets the ScheduleTypeLimits of schedule if there is
   *  none. */
    bool setWorkEfficiencySchedule(Schedule& schedule);

    /** Resets the work efficiency Schedule. */
    void resetWorkEfficiencySchedule();

    /** Sets the clothing insulation Schedule. Returns false if schedule has a ScheduleTypeLimits
   *  that is incompatible with this use. Sets the ScheduleTypeLimits of schedule if there is
   *  none. */
    bool setClothingInsulationSchedule(Schedule& schedule);

    /** Resets the clothing insulation Schedule. */
    void resetClothingInsulationSchedule();

    /** Sets the air velocity Schedule. Returns false if schedule has a ScheduleTypeLimits
   *  that is incompatible with this use. Sets the ScheduleTypeLimits of schedule if there is
   *  none. */
    bool setAirVelocitySchedule(Schedule& schedule);

    /** Resets the air velocity Schedule. */
    void resetAirVelocitySchedule();

    bool setMultiplier(double multiplier);

    void resetMultiplier();

    bool setAnkleLevelAirVelocitySchedule(Schedule& schedule);
    void resetAnkleLevelAirVelocitySchedule();

    bool setColdStressTemperatureThreshold(double coldStressTemperatureThreshold);
    void resetColdStressTemperatureThreshold();

    bool setHeatStressTemperatureThreshold(double heatStressTemperatureThreshold);
    void resetHeatStressTemperatureThreshold();

    //@}
    /** @name Other */
    //@{

    /** Returns the numberOfPeople if possible based on the underlying data of
   *  peopleDefinition(). */
    boost::optional<double> numberOfPeople() const;

    /** Returns the peoplePerFloorArea if possible based on the underlying data of
   *  peopleDefinition(). */
    boost::optional<double> peoplePerFloorArea() const;

    /** Returns the spaceFloorAreaPerPerson if possible based on the underlying data of
   *  peopleDefinition(). */
    boost::optional<double> spaceFloorAreaPerPerson() const;

    /** Returns the number of people represented by this instance, assuming floorArea (m^2). */
    double getNumberOfPeople(double floorArea) const;

    /** Returns the people/m^2 represented by this instance, assuming floorArea (m^2). */
    double getPeoplePerFloorArea(double floorArea) const;

    /** Returns the m^2/person represented by this instance, assuming floorArea (m^2). */
    double getFloorAreaPerPerson(double floorArea) const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::People_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    explicit People(std::shared_ptr<detail::People_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.People");
  };

  /** \relates People*/
  using OptionalPeople = boost::optional<People>;

  /** \relates People*/
  using PeopleVector = std::vector<People>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PEOPLE_HPP
