/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PEOPLE_IMPL_HPP
#define MODEL_PEOPLE_IMPL_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadInstance_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class PeopleDefinition;
  class People;

  namespace detail {

    /** People_Impl is a SpaceLoadInstance_Impl that is the implementation class for People.*/
    class MODEL_API People_Impl : public SpaceLoadInstance_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      People_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      People_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      People_Impl(const People_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~People_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual bool hardSize() override;

      virtual bool hardApplySchedules() override;

      virtual double multiplier() const override;

      virtual bool isMultiplierDefaulted() const override;

      virtual bool isAbsolute() const override;

      //@}
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

      // TODO: Handle Non-Extensible IddField Surface Name/Angle Factor List Name.

      /** Returns the work efficiency schedule. */
      boost::optional<Schedule> workEfficiencySchedule() const;

      /** Returns the clothing insulation schedule. */
      boost::optional<Schedule> clothingInsulationSchedule() const;

      /** Returns the air velocity schedule. */
      boost::optional<Schedule> airVelocitySchedule() const;

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

      /** Sets the definition of this instance. */
      virtual bool setDefinition(const SpaceLoadDefinition& definition) override;

      /** Sets the number of people Schedule. */
      bool setNumberofPeopleSchedule(Schedule& schedule);

      /** Resets the number of people Schedule. */
      void resetNumberofPeopleSchedule();

      /** Sets the activity Schedule. */
      bool setActivityLevelSchedule(Schedule& schedule);

      /** Resets the activity Schedule. */
      void resetActivityLevelSchedule();

      // TODO: Handle Non-Extensible IddField Surface Name/Angle Factor List Name.

      /** Sets the work efficiency Schedule. */
      bool setWorkEfficiencySchedule(Schedule& schedule);

      /** Resets the work efficiency Schedule. */
      void resetWorkEfficiencySchedule();

      /** Sets the clothing insulation Schedule. */
      bool setClothingInsulationSchedule(Schedule& schedule);

      /** Resets the clothing insulation Schedule. */
      void resetClothingInsulationSchedule();

      /** Sets the air velocity Schedule. */
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

      boost::optional<double> numberOfPeople() const;

      boost::optional<double> peoplePerFloorArea() const;

      boost::optional<double> spaceFloorAreaPerPerson() const;

      /** Returns the number of people represented by this instance, assuming floorArea (m^2). */
      double getNumberOfPeople(double floorArea) const;

      /** Returns the people/m^2 represented by this instance, assuming floorArea (m^2). */
      double getPeoplePerFloorArea(double floorArea) const;

      /** Returns the m^2/person represented by this instance, assuming floorArea (m^2). */
      double getFloorAreaPerPerson(double floorArea) const;

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      //@}
     protected:
      // index of the space name
      virtual int spaceIndex() const override;

      // index of the definition name
      virtual int definitionIndex() const override;

     private:
      REGISTER_LOGGER("openstudio.model.People");

      boost::optional<ModelObject> peopleDefinitionAsModelObject() const;
      boost::optional<ModelObject> numberofPeopleScheduleAsModelObject() const;
      boost::optional<ModelObject> activityLevelScheduleAsModelObject() const;
      boost::optional<ModelObject> workEfficiencyScheduleAsModelObject() const;
      boost::optional<ModelObject> clothingInsulationScheduleAsModelObject() const;
      boost::optional<ModelObject> airVelocityScheduleAsModelObject() const;
      boost::optional<ModelObject> ankleLevelAirVelocityScheduleAsModelObject() const;

      bool setPeopleDefinitionAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setNumberofPeopleScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setActivityLevelScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setWorkEfficiencyScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setClothingInsulationScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setAirVelocityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setAnkleLevelAirVelocityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PEOPLE_IMPL_HPP
