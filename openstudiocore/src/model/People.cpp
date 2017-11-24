/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "People.hpp"
#include "People_Impl.hpp"

#include "Model.hpp"
#include "PeopleDefinition.hpp"
#include "PeopleDefinition_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include "Space.hpp"
#include "Space_Impl.hpp"
#include "SpaceType.hpp"
#include "SpaceType_Impl.hpp"
#include "DefaultScheduleSet.hpp"
#include "DefaultScheduleSet_Impl.hpp"
#include "LifeCycleCost.hpp"

#include <utilities/idd/OS_People_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  People_Impl::People_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : SpaceLoadInstance_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == People::iddObjectType());
  }

  People_Impl::People_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                           Model_Impl* model,
                           bool keepHandle)
    : SpaceLoadInstance_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == People::iddObjectType());
  }

  People_Impl::People_Impl(const People_Impl& other,
                           Model_Impl* model,
                           bool keepHandle)
    : SpaceLoadInstance_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& People_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){

      result.push_back("People Occupant Count");
      result.push_back("People Radiant Heating Energy");
      result.push_back("People Radiant Heating Rate");
      result.push_back("People Convective Heating Energy");
      result.push_back("People Convective Heating Rate");
      result.push_back("People Sensible Heating Energy");
      result.push_back("People Sensible Heating Rate");
      result.push_back("People Latent Gain Energy");
      result.push_back("People Latent Gain Rate");
      result.push_back("People Total Heating Energy");
      result.push_back("People Total Heating Rate");
      result.push_back("People Air Temperature");
      result.push_back("People Air Relative Humidity");

      // Reported in ThermalZone
      //result.push_back("Zone People Occupant Count");
      //result.push_back("Zone People Radiant Heating Energy");
      //result.push_back("Zone People Radiant Heating Rate");
      //result.push_back("Zone People Convective Heating Energy");
      //result.push_back("Zone People Convective Heating Rate");
      //result.push_back("Zone People Sensible Heating Energy");
      //result.push_back("Zone People Sensible Heating Rate");
      //result.push_back("Zone People Latent Gain Energy");
      //result.push_back("Zone People Latent Gain Rate");
      //result.push_back("Zone People Total Heating Energy");
      //result.push_back("Zone People Total Heating Rate");
      //result.push_back("Zone Thermal Comfort Mean Radiant Temperature");
      //result.push_back("Zone Thermal Comfort Operative Temperature");
      //result.push_back("Zone Thermal Comfort Fanger Model PMV");
      //result.push_back("Zone Thermal Comfort Fanger Model PPD");
      //result.push_back("Zone Thermal Comfort Clothing Surface Temperature");
      //result.push_back("Zone Thermal Comfort Pierce Model Effective Temperature PMV");
      //result.push_back("Zone Thermal Comfort Pierce Model Standard Effective Temperature PMV");
      //result.push_back("Zone Thermal Comfort Pierce Model Discomfort Index");
      //result.push_back("Zone Thermal Comfort Pierce Model Thermal Sensation Index");
      //result.push_back("Zone Thermal Comfort KSU Model Thermal Sensation Index");
      //result.push_back("Zone Thermal Comfort ASHRAE 55 Adaptive Model 80%% Acceptability Status");
      //result.push_back("Zone Thermal Comfort ASHRAE 55 Adaptive Model 90%% Acceptability Status");
      //result.push_back("Zone Thermal Comfort ASHRAE 55 Adaptive Model Running Average Outdoor Air Temperature");
      //result.push_back("Zone Thermal Comfort ASHRAE 55 Adaptive Model Temperature");
      //result.push_back("Zone Thermal Comfort CEN 15251 Adaptive Model Category I Status");
      //result.push_back("Zone Thermal Comfort CEN 15251 Adaptive Model Category II Status");
      //result.push_back("Zone Thermal Comfort CEN 15251 Adaptive Model Category III Status");
      //result.push_back("Zone Thermal Comfort CEN 15251 Adaptive Model Running Average Outdoor Air Temperature");
      //result.push_back("Zone Thermal Comfort CEN 15251 Adaptive Model Temperature");
    }
    return result;
  }

  IddObjectType People_Impl::iddObjectType() const {
    return People::iddObjectType();
  }

  std::vector<ScheduleTypeKey> People_Impl::getScheduleTypeKeys(const Schedule &schedule) const {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_PeopleFields::NumberofPeopleScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("People","Number of People"));
    }
    if (std::find(b,e,OS_PeopleFields::ActivityLevelScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("People","Activity Level"));
    }
    if (std::find(b,e,OS_PeopleFields::WorkEfficiencyScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("People","Work Efficiency"));
    }
    if (std::find(b,e,OS_PeopleFields::ClothingInsulationScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("People","Clothing Insulation"));
    }
    if (std::find(b,e,OS_PeopleFields::AirVelocityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("People","Air Velocity"));
    }
    return result;
  }

  bool People_Impl::hardSize()
  {
    boost::optional<Space> space = this->space();
    if (!space){
      return false;
    }

    this->makeUnique();

    PeopleDefinition peopleDefinition = this->peopleDefinition();
    for (LifeCycleCost cost : peopleDefinition.lifeCycleCosts()){
      cost.convertToCostPerEach();
    }

    boost::optional<double> numberofPeople = peopleDefinition.numberofPeople();
    if (numberofPeople){
      return true;
    }

    boost::optional<double> peopleperSpaceFloorArea = peopleDefinition.peopleperSpaceFloorArea();
    if (peopleperSpaceFloorArea){
      return peopleDefinition.setNumberofPeople(*peopleperSpaceFloorArea * space->floorArea());
    }

    boost::optional<double> spaceFloorAreaperPerson = peopleDefinition.spaceFloorAreaperPerson();
    if (spaceFloorAreaperPerson){
      if (*spaceFloorAreaperPerson == 0){
        return false;
      }else{
        return peopleDefinition.setNumberofPeople(space->floorArea() / *spaceFloorAreaperPerson);
      }
    }

    return false;
  }

  bool People_Impl::hardApplySchedules()
  {
    bool result = false;

    // required by EnergyPlus
    boost::optional<Schedule> schedule = this->numberofPeopleSchedule();
    if (schedule){
      result = this->setNumberofPeopleSchedule(*schedule);
    }

    // required by EnergyPlus
    schedule = this->activityLevelSchedule();
    if (schedule){
      result = result && this->setActivityLevelSchedule(*schedule);
    }else{
      result = false;
    }

    // optional in EnergyPlus
    schedule = this->workEfficiencySchedule();
    if (schedule){
      this->setWorkEfficiencySchedule(*schedule);
    }

    // optional in EnergyPlus
    schedule = this->clothingInsulationSchedule();
    if (schedule){
      this->setClothingInsulationSchedule(*schedule);
    }

    // optional in EnergyPlus
    schedule = this->airVelocitySchedule();
    if (schedule){
      this->setAirVelocitySchedule(*schedule);
    }

    return result;
  }

  double People_Impl::multiplier() const {
    boost::optional<double> value = getDouble(OS_PeopleFields::Multiplier,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool People_Impl::isMultiplierDefaulted() const {
    return isEmpty(OS_PeopleFields::Multiplier);
  }

  bool People_Impl::isAbsolute() const {
    PeopleDefinition definition = peopleDefinition();
    if (definition.numberofPeople()) {
      return true;
    }
    return false;
  }

  bool People_Impl::setMultiplier(double multiplier) {
    bool result = setDouble(OS_PeopleFields::Multiplier, multiplier);
    return result;
  }

  void People_Impl::resetMultiplier() {
    bool result = setString(OS_PeopleFields::Multiplier, "");
    OS_ASSERT(result);
  }

  int People_Impl::spaceIndex() const {
    return OS_PeopleFields::SpaceorSpaceTypeName;
  }

  int People_Impl::definitionIndex() const {
    return OS_PeopleFields::PeopleDefinitionName;
  }

  PeopleDefinition People_Impl::peopleDefinition() const
  {
    return this->definition().cast<PeopleDefinition>();
  }

  bool People_Impl::setPeopleDefinition(const PeopleDefinition& definition)
  {
    return this->setPointer(this->definitionIndex(), definition.handle());
  }

  bool People_Impl::setDefinition(const SpaceLoadDefinition& definition)
  {
    bool result = false;
    boost::optional<PeopleDefinition> peopleDefinition = definition.optionalCast<PeopleDefinition>();
    if (peopleDefinition){
      result = setPeopleDefinition(*peopleDefinition);
    }
    return result;
  }

  boost::optional<Schedule> People_Impl::numberofPeopleSchedule() const
  {
    boost::optional<Schedule> result = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_PeopleFields::NumberofPeopleScheduleName);
    if (!result){
      // search upwards
      OptionalSpace space = this->space();
      OptionalSpaceType spaceType = this->spaceType();
      if (space){
        result = space->getDefaultSchedule(DefaultScheduleType::NumberofPeopleSchedule);
      }else if (spaceType){
        result = spaceType->getDefaultSchedule(DefaultScheduleType::NumberofPeopleSchedule);
      }
    }
    return result;
  }

  bool People_Impl::isNumberofPeopleScheduleDefaulted() const
  {
    return isEmpty(OS_PeopleFields::NumberofPeopleScheduleName);
  }

  bool People_Impl::setNumberofPeopleSchedule(Schedule& schedule) {
    return setSchedule(OS_PeopleFields::NumberofPeopleScheduleName,
                       "People",
                       "Number of People",
                       schedule);
  }

  void People_Impl::resetNumberofPeopleSchedule() {
    bool result = setString(OS_PeopleFields::NumberofPeopleScheduleName, "");
    OS_ASSERT(result);
  }

  boost::optional<Schedule> People_Impl::activityLevelSchedule() const {
    boost::optional<Schedule> result = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_PeopleFields::ActivityLevelScheduleName);
    if (!result){
      // search upwards
      OptionalSpace space = this->space();
      OptionalSpaceType spaceType = this->spaceType();
      if (space){
        result = space->getDefaultSchedule(DefaultScheduleType::PeopleActivityLevelSchedule);
      }else if (spaceType){
        result = spaceType->getDefaultSchedule(DefaultScheduleType::PeopleActivityLevelSchedule);
      }
    }
    return result;
  }

  bool People_Impl::isActivityLevelScheduleDefaulted() const
  {
    return isEmpty(OS_PeopleFields::ActivityLevelScheduleName);
  }

  bool People_Impl::setActivityLevelSchedule(Schedule& schedule) {
    return setSchedule(OS_PeopleFields::ActivityLevelScheduleName,
                       "People",
                       "Activity Level",
                       schedule);
  }

  void People_Impl::resetActivityLevelSchedule() {
    bool result = setString(OS_PeopleFields::ActivityLevelScheduleName, "");
    OS_ASSERT(result);
  }

  boost::optional<Schedule> People_Impl::workEfficiencySchedule() const
  {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_PeopleFields::WorkEfficiencyScheduleName);
  }

  bool People_Impl::setWorkEfficiencySchedule(Schedule& schedule) {
    return setSchedule(OS_PeopleFields::WorkEfficiencyScheduleName,
                       "People",
                       "Work Efficiency",
                       schedule);
  }

  void People_Impl::resetWorkEfficiencySchedule() {
    bool result = setString(OS_PeopleFields::WorkEfficiencyScheduleName, "");
    OS_ASSERT(result);
  }

  boost::optional<Schedule> People_Impl::clothingInsulationSchedule() const
  {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_PeopleFields::ClothingInsulationScheduleName);
  }

  bool People_Impl::setClothingInsulationSchedule(Schedule& schedule) {
    return setSchedule(OS_PeopleFields::ClothingInsulationScheduleName,
                       "People",
                       "Clothing Insulation",
                       schedule);
  }

  void People_Impl::resetClothingInsulationSchedule() {
    bool result = setString(OS_PeopleFields::ClothingInsulationScheduleName, "");
    OS_ASSERT(result);
  }

  boost::optional<Schedule> People_Impl::airVelocitySchedule() const
  {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_PeopleFields::AirVelocityScheduleName);
  }

  bool People_Impl::setAirVelocitySchedule(Schedule& schedule) {
    return setSchedule(OS_PeopleFields::AirVelocityScheduleName,
                       "People",
                       "Air Velocity",
                       schedule);
  }

  void People_Impl::resetAirVelocitySchedule() {
    bool result = setString(OS_PeopleFields::AirVelocityScheduleName, "");
    OS_ASSERT(result);
  }

  boost::optional<double> People_Impl::numberOfPeople() const {
    OptionalDouble temp = peopleDefinition().numberofPeople();
    if (temp) {
      return temp.get() * multiplier();
    }
    return temp;
  }

  boost::optional<double> People_Impl::peoplePerFloorArea() const {
    OptionalDouble temp = peopleDefinition().peopleperSpaceFloorArea();
    if (temp) {
      return temp.get() * multiplier();
    }
    return temp;
  }

  boost::optional<double> People_Impl::spaceFloorAreaPerPerson() const {
    OptionalDouble temp = peoplePerFloorArea();
    if (temp) {
      return 1.0 / temp.get();
    }
    return temp;
  }

  double People_Impl::getNumberOfPeople(double floorArea) const {
    return peopleDefinition().getNumberOfPeople(floorArea) * multiplier();
  }

  double People_Impl::getPeoplePerFloorArea(double floorArea) const {
    return peopleDefinition().getPeoplePerFloorArea(floorArea) * multiplier();
  }

  double People_Impl::getFloorAreaPerPerson(double floorArea) const {
    return peopleDefinition().getFloorAreaPerPerson(floorArea) * multiplier();
  }

  boost::optional<ModelObject> People_Impl::peopleDefinitionAsModelObject() const {
    OptionalModelObject result = peopleDefinition();
    return result;
  }

  boost::optional<ModelObject> People_Impl::numberofPeopleScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = numberofPeopleSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> People_Impl::activityLevelScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = activityLevelSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> People_Impl::workEfficiencyScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = workEfficiencySchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> People_Impl::clothingInsulationScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = clothingInsulationSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> People_Impl::airVelocityScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = airVelocitySchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  bool People_Impl::setPeopleDefinitionAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalPeopleDefinition intermediate = modelObject->optionalCast<PeopleDefinition>();
      if (intermediate) {
        return setPeopleDefinition(*intermediate);
      }
    }
    return false;
  }

  bool People_Impl::setNumberofPeopleScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setNumberofPeopleSchedule(schedule);
      }
      else {
        return false;
      }
    }
    else {
      resetNumberofPeopleSchedule();
    }
    return true;
  }

  bool People_Impl::setActivityLevelScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setActivityLevelSchedule(schedule);
      }
      else {
        return false;
      }
    }
    else {
      resetActivityLevelSchedule();
    }
    return true;
  }

  bool People_Impl::setWorkEfficiencyScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setWorkEfficiencySchedule(schedule);
      }
      else {
        return false;
      }
    }
    else {
      resetWorkEfficiencySchedule();
    }
    return true;
  }

  bool People_Impl::setClothingInsulationScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setClothingInsulationSchedule(schedule);
      }
      else {
        return false;
      }
    }
    else {
      resetClothingInsulationSchedule();
    }
    return true;
  }

  bool People_Impl::setAirVelocityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setAirVelocitySchedule(schedule);
      }
      else {
        return false;
      }
    }
    else {
      resetAirVelocitySchedule();
    }
    return true;
  }

} // detail

People::People(const PeopleDefinition& peopleDefinition)
  : SpaceLoadInstance(People::iddObjectType(),peopleDefinition)
{
  OS_ASSERT(getImpl<detail::People_Impl>());

  bool test = this->setMultiplier(1.0);
  OS_ASSERT(test);
}

IddObjectType People::iddObjectType() {
  IddObjectType result(IddObjectType::OS_People);
  return result;
}

bool People::setMultiplier(double multiplier) {
  return getImpl<detail::People_Impl>()->setMultiplier(multiplier);
}

void People::resetMultiplier() {
  getImpl<detail::People_Impl>()->resetMultiplier();
}

PeopleDefinition People::peopleDefinition() const
{
  return getImpl<detail::People_Impl>()->peopleDefinition();
}

bool People::setPeopleDefinition(const PeopleDefinition& peopleDefinition)
{
  return getImpl<detail::People_Impl>()->setPeopleDefinition(peopleDefinition);
}

boost::optional<Schedule> People::numberofPeopleSchedule() const
{
  return getImpl<detail::People_Impl>()->numberofPeopleSchedule();
}

bool People::isNumberofPeopleScheduleDefaulted() const
{
  return getImpl<detail::People_Impl>()->isNumberofPeopleScheduleDefaulted();
}

bool People::setNumberofPeopleSchedule(Schedule& schedule)
{
  return getImpl<detail::People_Impl>()->setNumberofPeopleSchedule(schedule);
}

void People::resetNumberofPeopleSchedule()
{
  getImpl<detail::People_Impl>()->resetNumberofPeopleSchedule();
}

boost::optional<Schedule> People::activityLevelSchedule() const
{
  return getImpl<detail::People_Impl>()->activityLevelSchedule();
}

bool People::isActivityLevelScheduleDefaulted() const
{
  return getImpl<detail::People_Impl>()->isActivityLevelScheduleDefaulted();
}

bool People::setActivityLevelSchedule(Schedule& schedule)
{
  return getImpl<detail::People_Impl>()->setActivityLevelSchedule(schedule);
}

void People::resetActivityLevelSchedule()
{
  getImpl<detail::People_Impl>()->resetActivityLevelSchedule();
}

boost::optional<Schedule> People::workEfficiencySchedule() const
{
  return getImpl<detail::People_Impl>()->workEfficiencySchedule();
}

bool People::setWorkEfficiencySchedule(Schedule& schedule)
{
  return getImpl<detail::People_Impl>()->setWorkEfficiencySchedule(schedule);
}

void People::resetWorkEfficiencySchedule()
{
  getImpl<detail::People_Impl>()->resetWorkEfficiencySchedule();
}

boost::optional<Schedule> People::clothingInsulationSchedule() const
{
  return getImpl<detail::People_Impl>()->clothingInsulationSchedule();
}

bool People::setClothingInsulationSchedule(Schedule& schedule)
{
  return getImpl<detail::People_Impl>()->setClothingInsulationSchedule(schedule);
}

void People::resetClothingInsulationSchedule()
{
  getImpl<detail::People_Impl>()->resetClothingInsulationSchedule();
}

boost::optional<Schedule> People::airVelocitySchedule() const
{
  return getImpl<detail::People_Impl>()->airVelocitySchedule();
}

bool People::setAirVelocitySchedule(Schedule& schedule)
{
  return getImpl<detail::People_Impl>()->setAirVelocitySchedule(schedule);
}

void People::resetAirVelocitySchedule()
{
  getImpl<detail::People_Impl>()->resetAirVelocitySchedule();
}

boost::optional<double> People::numberOfPeople() const {
  return getImpl<detail::People_Impl>()->numberOfPeople();
}

boost::optional<double> People::peoplePerFloorArea() const {
  return getImpl<detail::People_Impl>()->peoplePerFloorArea();
}

boost::optional<double> People::spaceFloorAreaPerPerson() const {
  return getImpl<detail::People_Impl>()->spaceFloorAreaPerPerson();
}

double People::getNumberOfPeople(double floorArea) const {
  return getImpl<detail::People_Impl>()->getNumberOfPeople(floorArea);
}

double People::getPeoplePerFloorArea(double floorArea) const {
  return getImpl<detail::People_Impl>()->getPeoplePerFloorArea(floorArea);
}

double People::getFloorAreaPerPerson(double floorArea) const {
  return getImpl<detail::People_Impl>()->getFloorAreaPerPerson(floorArea);
}

/// @cond
People::People(std::shared_ptr<detail::People_Impl> impl)
  : SpaceLoadInstance(std::move(impl))
{}
/// @endcond


} // model
} // openstudio

