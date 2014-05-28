/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

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
  class MODEL_API People_Impl : public SpaceLoadInstance_Impl {
    Q_OBJECT;
    Q_PROPERTY(bool isNumberofPeopleScheduleDefaulted READ isNumberofPeopleScheduleDefaulted);
    Q_PROPERTY(bool isActivityLevelScheduleDefaulted READ isActivityLevelScheduleDefaulted);
    Q_PROPERTY(double multiplier READ multiplier WRITE setMultiplier RESET resetMultiplier);
    Q_PROPERTY(bool isMultiplierDefaulted READ isMultiplierDefaulted);
    Q_PROPERTY(boost::optional<double> numberOfPeople READ numberOfPeople);
    Q_PROPERTY(boost::optional<double> peoplePerFloorArea READ peoplePerFloorArea);
    Q_PROPERTY(boost::optional<double> spaceFloorAreaPerPerson READ spaceFloorAreaPerPerson);

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> peopleDefinition READ peopleDefinitionAsModelObject WRITE setPeopleDefinitionAsModelObject);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> numberofPeopleSchedule READ numberofPeopleScheduleAsModelObject WRITE setNumberofPeopleScheduleAsModelObject RESET resetNumberofPeopleSchedule);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> activityLevelSchedule READ activityLevelScheduleAsModelObject WRITE setActivityLevelScheduleAsModelObject RESET resetActivityLevelSchedule);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> workEfficiencySchedule READ workEfficiencyScheduleAsModelObject WRITE setWorkEfficiencyScheduleAsModelObject RESET resetWorkEfficiencySchedule);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> clothingInsulationSchedule READ clothingInsulationScheduleAsModelObject WRITE setClothingInsulationScheduleAsModelObject RESET resetClothingInsulationSchedule);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> airVelocitySchedule READ airVelocityScheduleAsModelObject WRITE setAirVelocityScheduleAsModelObject RESET resetAirVelocitySchedule);
   public:
    /** @name Constructors and Destructors */
    //@{

    People_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    People_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                Model_Impl* model,
                bool keepHandle);

    People_Impl(const People_Impl& other,
                Model_Impl* model,
                bool keepHandle);

    virtual ~People_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;
    
    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule &schedule) const;

    virtual bool hardSize();

    virtual bool hardApplySchedules();

    virtual double multiplier() const;

    virtual bool isMultiplierDefaulted() const;

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

    //@}
    /** @name Setters */
    //@{

    /** Sets the PeopleDefinition object. */
    bool setPeopleDefinition(const PeopleDefinition& definition);

    /** Sets the definition of this instance. */
    virtual bool setDefinition(const SpaceLoadDefinition& definition);

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

    //@}
   protected:

    // index of the space name
    virtual int spaceIndex() const;

    // index of the definition name
    virtual int definitionIndex() const;

   private:
    REGISTER_LOGGER("openstudio.model.People");

    boost::optional<ModelObject> peopleDefinitionAsModelObject() const;
    boost::optional<ModelObject> numberofPeopleScheduleAsModelObject() const;
    boost::optional<ModelObject> activityLevelScheduleAsModelObject() const;
    boost::optional<ModelObject> workEfficiencyScheduleAsModelObject() const;
    boost::optional<ModelObject> clothingInsulationScheduleAsModelObject() const;
    boost::optional<ModelObject> airVelocityScheduleAsModelObject() const;

    bool setPeopleDefinitionAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setNumberofPeopleScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setActivityLevelScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setWorkEfficiencyScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setClothingInsulationScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setAirVelocityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_PEOPLE_IMPL_HPP

