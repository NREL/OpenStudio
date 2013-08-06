/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <model/ElectricEquipment.hpp>
#include <model/ElectricEquipment_Impl.hpp>

#include <model/ElectricEquipmentDefinition.hpp>
#include <model/ElectricEquipmentDefinition_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/Space.hpp>
#include <model/Space_Impl.hpp>
#include <model/SpaceType.hpp>
#include <model/SpaceType_Impl.hpp>
#include <model/DefaultScheduleSet.hpp>
#include <model/DefaultScheduleSet_Impl.hpp>
#include <model/LifeCycleCost.hpp>

#include <utilities/idd/OS_ElectricEquipment_FieldEnums.hxx>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model {

namespace detail {

  ElectricEquipment_Impl::ElectricEquipment_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : SpaceLoadInstance_Impl(idfObject,model,keepHandle)
  {
    BOOST_ASSERT(idfObject.iddObject().type() == ElectricEquipment::iddObjectType());
  }

  ElectricEquipment_Impl::ElectricEquipment_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                 Model_Impl* model,
                                                 bool keepHandle)
    : SpaceLoadInstance_Impl(other,model,keepHandle)
  {
    BOOST_ASSERT(other.iddObject().type() == ElectricEquipment::iddObjectType());
  }

  ElectricEquipment_Impl::ElectricEquipment_Impl(const ElectricEquipment_Impl& other,
                                                 Model_Impl* model,
                                                 bool keepHandle)
    : SpaceLoadInstance_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ElectricEquipment_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ElectricEquipment_Impl::iddObjectType() const {
    return ElectricEquipment::iddObjectType();
  }

  std::vector<ScheduleTypeKey> ElectricEquipment_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_ElectricEquipmentFields::ScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ElectricEquipment","Electric Equipment"));
    }
    return result;
  }

  bool ElectricEquipment_Impl::hardSize()
  {
    boost::optional<Space> space = this->space();
    if (!space){
      return false;
    }

    this->makeUnique();

    ElectricEquipmentDefinition electricEquipmentDefinition = this->electricEquipmentDefinition();
    BOOST_FOREACH(LifeCycleCost cost, electricEquipmentDefinition.lifeCycleCosts()){
      cost.convertToCostPerEach();
    }

    boost::optional<double> designLevel = electricEquipmentDefinition.designLevel();
    if (designLevel){
      return true;
    }

    boost::optional<double> wattsperSpaceFloorArea = electricEquipmentDefinition.wattsperSpaceFloorArea();
    if (wattsperSpaceFloorArea){
      return electricEquipmentDefinition.setDesignLevel(*wattsperSpaceFloorArea * space->floorArea());
    }

    boost::optional<double> wattsperPerson = electricEquipmentDefinition.wattsperPerson();
    if (wattsperPerson){
      return electricEquipmentDefinition.setDesignLevel(*wattsperPerson * space->numberOfPeople());
    }

    return false;
  }

  bool ElectricEquipment_Impl::hardApplySchedules()
  {
    bool result = false;
    boost::optional<Schedule> schedule = this->schedule();
    if (schedule){
      result = this->setSchedule(*schedule);
    }
    return result;
  }

  double ElectricEquipment_Impl::multiplier() const {
    boost::optional<double> value = getDouble(OS_ElectricEquipmentFields::Multiplier,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool ElectricEquipment_Impl::isMultiplierDefaulted() const {
    return isEmpty(OS_ElectricEquipmentFields::Multiplier);
  }

  std::string ElectricEquipment_Impl::endUseSubcategory() const {
    boost::optional<std::string> value = getString(OS_ElectricEquipmentFields::EndUseSubcategory,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool ElectricEquipment_Impl::isEndUseSubcategoryDefaulted() const {
    return isEmpty(OS_ElectricEquipmentFields::EndUseSubcategory);
  }

  bool ElectricEquipment_Impl::setMultiplier(double multiplier) {
    bool result = false;
    result = setDouble(OS_ElectricEquipmentFields::Multiplier, multiplier);
    return result;
  }

  void ElectricEquipment_Impl::resetMultiplier() {
    bool result = setString(OS_ElectricEquipmentFields::Multiplier, "");
    BOOST_ASSERT(result);
  }

  void ElectricEquipment_Impl::setEndUseSubcategory(std::string endUseSubcategory) {
    bool result = false;
    result = setString(OS_ElectricEquipmentFields::EndUseSubcategory, endUseSubcategory);
    BOOST_ASSERT(result);
  }

  void ElectricEquipment_Impl::resetEndUseSubcategory() {
    bool result = setString(OS_ElectricEquipmentFields::EndUseSubcategory, "");
    BOOST_ASSERT(result);
  }

  int ElectricEquipment_Impl::spaceIndex() const {
    return OS_ElectricEquipmentFields::SpaceorSpaceTypeName;
  }

  int ElectricEquipment_Impl::definitionIndex() const {
    return OS_ElectricEquipmentFields::ElectricEquipmentDefinitionName;
  }

  boost::optional<Schedule> ElectricEquipment_Impl::schedule() const
  {
    boost::optional<Schedule> result = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ElectricEquipmentFields::ScheduleName);
    if (!result){
      // search upwards
      OptionalSpace space = this->space();
      OptionalSpaceType spaceType = this->spaceType();
      if (space){
        result = space->getDefaultSchedule(DefaultScheduleType::ElectricEquipmentSchedule);
      }else if (spaceType){
        result = spaceType->getDefaultSchedule(DefaultScheduleType::ElectricEquipmentSchedule);
      }
    }
    return result;
  }

  bool ElectricEquipment_Impl::isScheduleDefaulted() const
  {
    return isEmpty(OS_ElectricEquipmentFields::ScheduleName);
  }

  bool ElectricEquipment_Impl::setSchedule(Schedule& schedule) {
    bool result = ModelObject_Impl::setSchedule(OS_ElectricEquipmentFields::ScheduleName,
                                                "ElectricEquipment",
                                                "Electric Equipment",
                                                schedule);
    return result;
  }

  void ElectricEquipment_Impl::resetSchedule()
  {
    this->setString(OS_ElectricEquipmentFields::ScheduleName, "");
  }

  ElectricEquipmentDefinition ElectricEquipment_Impl::electricEquipmentDefinition() const
  {
    return this->definition().cast<ElectricEquipmentDefinition>();
  }

  bool ElectricEquipment_Impl::setElectricEquipmentDefinition(const ElectricEquipmentDefinition& definition)
  {
    return this->setPointer(this->definitionIndex(), definition.handle());
  }

  bool ElectricEquipment_Impl::setDefinition(const SpaceLoadDefinition& definition)
  {
    bool result = false;
    boost::optional<ElectricEquipmentDefinition> electricEquipmentDefinition = definition.optionalCast<ElectricEquipmentDefinition>();
    if (electricEquipmentDefinition){
      result = setElectricEquipmentDefinition(*electricEquipmentDefinition);
    }
    return result;
  }

  boost::optional<ModelObject> ElectricEquipment_Impl::scheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = schedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> ElectricEquipment_Impl::electricEquipmentDefinitionAsModelObject() const {
    OptionalModelObject result = electricEquipmentDefinition();
    return result;
  }

  bool ElectricEquipment_Impl::setScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setSchedule(schedule);
      }
      else {
        return false;
      }
    }
    else {
      resetSchedule();
    }
    return true;
  }

  boost::optional<double> ElectricEquipment_Impl::designLevel() const {
    OptionalDouble result = electricEquipmentDefinition().designLevel();
    if (result) {
      return result.get() * multiplier();
    }
    return result;
  }

  boost::optional<double> ElectricEquipment_Impl::powerPerFloorArea() const {
    OptionalDouble result = electricEquipmentDefinition().wattsperSpaceFloorArea();
    if (result) {
      return result.get() * multiplier();
    }
    return result;
  }

  boost::optional<double> ElectricEquipment_Impl::powerPerPerson() const {
    OptionalDouble result = electricEquipmentDefinition().wattsperPerson();
    if (result) {
      return result.get() * multiplier();
    }
    return result;
  }

  double ElectricEquipment_Impl::getDesignLevel(double floorArea, double numPeople) const {
    return electricEquipmentDefinition().getDesignLevel(floorArea,numPeople) * multiplier();
  }

  double ElectricEquipment_Impl::getPowerPerFloorArea(double floorArea, double numPeople) const
  {
    return electricEquipmentDefinition().getPowerPerFloorArea(floorArea,numPeople) * multiplier();
  }

  double ElectricEquipment_Impl::getPowerPerPerson(double floorArea, double numPeople) const {
    return electricEquipmentDefinition().getPowerPerPerson(floorArea,numPeople) * multiplier();
  }

  bool ElectricEquipment_Impl::setElectricEquipmentDefinitionAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalElectricEquipmentDefinition intermediate = modelObject->optionalCast<ElectricEquipmentDefinition>();
      if (intermediate) {
        return setElectricEquipmentDefinition(*intermediate);
      }
    }
    return false;
  }

  boost::optional<ModelObject> SpaceLoadInstance_Impl::definitionAsModelObject() const {
    OptionalModelObject result = definition();
    return result;
  }

  bool SpaceLoadInstance_Impl::setDefinitionAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSpaceLoadDefinition intermediate = modelObject->optionalCast<SpaceLoadDefinition>();
      if (intermediate) {
        return setDefinition(*intermediate);
      }
    }
    return false;
  }

} // detail

ElectricEquipment::ElectricEquipment(const ElectricEquipmentDefinition& electricEquipmentDefinition)
  : SpaceLoadInstance(ElectricEquipment::iddObjectType(),electricEquipmentDefinition)
{
  BOOST_ASSERT(getImpl<detail::ElectricEquipment_Impl>());
}

IddObjectType ElectricEquipment::iddObjectType() {
  IddObjectType result(IddObjectType::OS_ElectricEquipment);
  return result;
}

std::string ElectricEquipment::endUseSubcategory() const {
  return getImpl<detail::ElectricEquipment_Impl>()->endUseSubcategory();
}

bool ElectricEquipment::isEndUseSubcategoryDefaulted() const {
  return getImpl<detail::ElectricEquipment_Impl>()->isEndUseSubcategoryDefaulted();
}

bool ElectricEquipment::setMultiplier(double multiplier) {
  return getImpl<detail::ElectricEquipment_Impl>()->setMultiplier(multiplier);
}

void ElectricEquipment::resetMultiplier() {
  getImpl<detail::ElectricEquipment_Impl>()->resetMultiplier();
}

void ElectricEquipment::setEndUseSubcategory(std::string endUseSubcategory) {
  getImpl<detail::ElectricEquipment_Impl>()->setEndUseSubcategory(endUseSubcategory);
}

void ElectricEquipment::resetEndUseSubcategory() {
  getImpl<detail::ElectricEquipment_Impl>()->resetEndUseSubcategory();
}

boost::optional<Schedule> ElectricEquipment::schedule() const
{
  return getImpl<detail::ElectricEquipment_Impl>()->schedule();
}

bool ElectricEquipment::isScheduleDefaulted() const
{
  return getImpl<detail::ElectricEquipment_Impl>()->isScheduleDefaulted();
}

bool ElectricEquipment::setSchedule(Schedule& schedule)
{
  return getImpl<detail::ElectricEquipment_Impl>()->setSchedule(schedule);
}

void ElectricEquipment::resetSchedule()
{
  getImpl<detail::ElectricEquipment_Impl>()->resetSchedule();
}

ElectricEquipmentDefinition ElectricEquipment::electricEquipmentDefinition() const
{
  return getImpl<detail::ElectricEquipment_Impl>()->electricEquipmentDefinition();
}

bool ElectricEquipment::setElectricEquipmentDefinition(const ElectricEquipmentDefinition& electricEquipmentDefinition)
{
  return getImpl<detail::ElectricEquipment_Impl>()->setElectricEquipmentDefinition(electricEquipmentDefinition);
}

boost::optional<double> ElectricEquipment::designLevel() const {
  return getImpl<detail::ElectricEquipment_Impl>()->designLevel();
}

boost::optional<double> ElectricEquipment::powerPerFloorArea() const {
  return getImpl<detail::ElectricEquipment_Impl>()->powerPerFloorArea();
}

boost::optional<double> ElectricEquipment::powerPerPerson() const {
  return getImpl<detail::ElectricEquipment_Impl>()->powerPerPerson();
}

double ElectricEquipment::getDesignLevel(double floorArea, double numPeople) const {
  return getImpl<detail::ElectricEquipment_Impl>()->getDesignLevel(floorArea,numPeople);
}

double ElectricEquipment::getPowerPerFloorArea(double floorArea, double numPeople) const {
  return getImpl<detail::ElectricEquipment_Impl>()->getPowerPerFloorArea(floorArea,numPeople);
}

double ElectricEquipment::getPowerPerPerson(double floorArea, double numPeople) const {
  return getImpl<detail::ElectricEquipment_Impl>()->getPowerPerPerson(floorArea,numPeople);
}

/// @cond
ElectricEquipment::ElectricEquipment(boost::shared_ptr<detail::ElectricEquipment_Impl> impl)
  : SpaceLoadInstance(impl)
{}
/// @endcond

} // model
} // openstudio

