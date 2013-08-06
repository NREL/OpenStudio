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

#include <model/GasEquipment.hpp>
#include <model/GasEquipment_Impl.hpp>

#include <model/GasEquipmentDefinition.hpp>
#include <model/GasEquipmentDefinition_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/Space.hpp>
#include <model/Space_Impl.hpp>
#include <model/SpaceType.hpp>
#include <model/SpaceType_Impl.hpp>
#include <model/DefaultScheduleSet.hpp>
#include <model/DefaultScheduleSet_Impl.hpp>
#include <model/LifeCycleCost.hpp>

#include <utilities/idd/OS_GasEquipment_FieldEnums.hxx>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model {

namespace detail {

  GasEquipment_Impl::GasEquipment_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : SpaceLoadInstance_Impl(idfObject,model,keepHandle)
  {
    BOOST_ASSERT(idfObject.iddObject().type() == GasEquipment::iddObjectType());
  }

  GasEquipment_Impl::GasEquipment_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                       Model_Impl* model,
                                       bool keepHandle)
    : SpaceLoadInstance_Impl(other,model,keepHandle)
  {
    BOOST_ASSERT(other.iddObject().type() == GasEquipment::iddObjectType());
  }

  GasEquipment_Impl::GasEquipment_Impl(const GasEquipment_Impl& other,
                                       Model_Impl* model,
                                       bool keepHandle)
    : SpaceLoadInstance_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& GasEquipment_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType GasEquipment_Impl::iddObjectType() const {
    return GasEquipment::iddObjectType();
  }

  std::vector<ScheduleTypeKey> GasEquipment_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_GasEquipmentFields::ScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("GasEquipment","Gas Equipment"));
    }
    return result;
  }

  bool GasEquipment_Impl::hardSize()
  {
    boost::optional<Space> space = this->space();
    if (!space){
      return false;
    }

    this->makeUnique();

    GasEquipmentDefinition gasEquipmentDefinition = this->gasEquipmentDefinition();
    BOOST_FOREACH(LifeCycleCost cost, gasEquipmentDefinition.lifeCycleCosts()){
      cost.convertToCostPerEach();
    }

    boost::optional<double> designLevel = gasEquipmentDefinition.designLevel();
    if (designLevel){
      return true;
    }

    boost::optional<double> wattsperSpaceFloorArea = gasEquipmentDefinition.wattsperSpaceFloorArea();
    if (wattsperSpaceFloorArea){
      return gasEquipmentDefinition.setDesignLevel(*wattsperSpaceFloorArea * space->floorArea());
    }

    boost::optional<double> wattsperPerson = gasEquipmentDefinition.wattsperPerson();
    if (wattsperPerson){
      return gasEquipmentDefinition.setDesignLevel(*wattsperPerson * space->numberOfPeople());
    }

    return false;
  }

  bool GasEquipment_Impl::hardApplySchedules()
  {
    bool result = false;
    boost::optional<Schedule> schedule = this->schedule();
    if (schedule){
      result = this->setSchedule(*schedule);
    }
    return result;
  }

  double GasEquipment_Impl::multiplier() const {
    boost::optional<double> value = getDouble(OS_GasEquipmentFields::Multiplier,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool GasEquipment_Impl::isMultiplierDefaulted() const {
    return isEmpty(OS_GasEquipmentFields::Multiplier);
  }

  std::string GasEquipment_Impl::endUseSubcategory() const {
    boost::optional<std::string> value = getString(OS_GasEquipmentFields::EndUseSubcategory,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool GasEquipment_Impl::isEndUseSubcategoryDefaulted() const {
    return isEmpty(OS_GasEquipmentFields::EndUseSubcategory);
  }

  bool GasEquipment_Impl::setMultiplier(double multiplier) {
    bool result = false;
    result = setDouble(OS_GasEquipmentFields::Multiplier, multiplier);
    return result;
  }

  void GasEquipment_Impl::resetMultiplier() {
    bool result = setString(OS_GasEquipmentFields::Multiplier, "");
    BOOST_ASSERT(result);
  }

  void GasEquipment_Impl::setEndUseSubcategory(std::string endUseSubcategory) {
    bool result = false;
    result = setString(OS_GasEquipmentFields::EndUseSubcategory, endUseSubcategory);
    BOOST_ASSERT(result);
  }

  void GasEquipment_Impl::resetEndUseSubcategory() {
    bool result = setString(OS_GasEquipmentFields::EndUseSubcategory, "");
    BOOST_ASSERT(result);
  }

  int GasEquipment_Impl::spaceIndex() const {
    return OS_GasEquipmentFields::SpaceorSpaceTypeName;
  }

  int GasEquipment_Impl::definitionIndex() const {
    return OS_GasEquipmentFields::GasEquipmentDefinitionName;
  }

  boost::optional<Schedule> GasEquipment_Impl::schedule() const
  {
    boost::optional<Schedule> result = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_GasEquipmentFields::ScheduleName);
    if (!result){
      // search upwards
      OptionalSpace space = this->space();
      OptionalSpaceType spaceType = this->spaceType();
      if (space){
        result = space->getDefaultSchedule(DefaultScheduleType::GasEquipmentSchedule);
      }else if (spaceType){
        result = spaceType->getDefaultSchedule(DefaultScheduleType::GasEquipmentSchedule);
      }
    }
    return result;
  }

  bool GasEquipment_Impl::isScheduleDefaulted() const
  {
    return isEmpty(OS_GasEquipmentFields::ScheduleName);
  }

  bool GasEquipment_Impl::setSchedule(Schedule& schedule) {
    bool result = ModelObject_Impl::setSchedule(OS_GasEquipmentFields::ScheduleName,
                                                "GasEquipment",
                                                "Gas Equipment",
                                                schedule);
    return result;
  }

  void GasEquipment_Impl::resetSchedule() {
    bool result = setString(OS_GasEquipmentFields::ScheduleName, "");
    BOOST_ASSERT(result);
  }

  GasEquipmentDefinition GasEquipment_Impl::gasEquipmentDefinition() const
  {
    return this->definition().cast<GasEquipmentDefinition>();
  }

  bool GasEquipment_Impl::setGasEquipmentDefinition(const GasEquipmentDefinition& definition)
  {
    return this->setPointer(this->definitionIndex(), definition.handle());
  }

  bool GasEquipment_Impl::setDefinition(const SpaceLoadDefinition& definition)
  {
    bool result = false;
    boost::optional<GasEquipmentDefinition> gasEquipmentDefinition = definition.optionalCast<GasEquipmentDefinition>();
    if (gasEquipmentDefinition){
      result = setGasEquipmentDefinition(*gasEquipmentDefinition);
    }
    return result;
  }

  boost::optional<double> GasEquipment_Impl::designLevel() const {
    OptionalDouble result = gasEquipmentDefinition().designLevel();
    if (result) {
      return result.get() * multiplier();
    }
    return result;
  }

  boost::optional<double> GasEquipment_Impl::powerPerFloorArea() const {
    OptionalDouble result = gasEquipmentDefinition().wattsperSpaceFloorArea();
    if (result) {
      return result.get() * multiplier();
    }
    return result;
  }

  boost::optional<double> GasEquipment_Impl::powerPerPerson() const {
    OptionalDouble result = gasEquipmentDefinition().wattsperPerson();
    if (result) {
      return result.get() * multiplier();
    }
    return result;
  }

  double GasEquipment_Impl::getDesignLevel(double floorArea, double numPeople) const {
    return gasEquipmentDefinition().getDesignLevel(floorArea,numPeople) * multiplier();
  }

  double GasEquipment_Impl::getPowerPerFloorArea(double floorArea, double numPeople) const
  {
    return gasEquipmentDefinition().getPowerPerFloorArea(floorArea,numPeople) * multiplier();
  }

  double GasEquipment_Impl::getPowerPerPerson(double floorArea, double numPeople) const {
    return gasEquipmentDefinition().getPowerPerPerson(floorArea,numPeople) * multiplier();
  }

  boost::optional<ModelObject> GasEquipment_Impl::gasEquipmentDefinitionAsModelObject() const {
    OptionalModelObject result = gasEquipmentDefinition();
    return result;
  }

  boost::optional<ModelObject> GasEquipment_Impl::scheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = schedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  bool GasEquipment_Impl::setGasEquipmentDefinitionAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalGasEquipmentDefinition intermediate = modelObject->optionalCast<GasEquipmentDefinition>();
      if (intermediate) {
        return setGasEquipmentDefinition(*intermediate);
      }
    }
    return false;
  }

  bool GasEquipment_Impl::setScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
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

} // detail

GasEquipment::GasEquipment(const GasEquipmentDefinition& gasEquipmentDefinition)
  : SpaceLoadInstance(GasEquipment::iddObjectType(),gasEquipmentDefinition)
{
  BOOST_ASSERT(getImpl<detail::GasEquipment_Impl>());
}

IddObjectType GasEquipment::iddObjectType() {
  IddObjectType result(IddObjectType::OS_GasEquipment);
  return result;
}

std::string GasEquipment::endUseSubcategory() const {
  return getImpl<detail::GasEquipment_Impl>()->endUseSubcategory();
}

bool GasEquipment::isEndUseSubcategoryDefaulted() const {
  return getImpl<detail::GasEquipment_Impl>()->isEndUseSubcategoryDefaulted();
}

bool GasEquipment::setMultiplier(double multiplier) {
  return getImpl<detail::GasEquipment_Impl>()->setMultiplier(multiplier);
}

void GasEquipment::resetMultiplier() {
  getImpl<detail::GasEquipment_Impl>()->resetMultiplier();
}

void GasEquipment::setEndUseSubcategory(std::string endUseSubcategory) {
  getImpl<detail::GasEquipment_Impl>()->setEndUseSubcategory(endUseSubcategory);
}

void GasEquipment::resetEndUseSubcategory() {
  getImpl<detail::GasEquipment_Impl>()->resetEndUseSubcategory();
}

boost::optional<Schedule> GasEquipment::schedule() const
{
  return getImpl<detail::GasEquipment_Impl>()->schedule();
}

bool GasEquipment::isScheduleDefaulted() const
{
  return getImpl<detail::GasEquipment_Impl>()->isScheduleDefaulted();
}

bool GasEquipment::setSchedule(Schedule& schedule)
{
  return getImpl<detail::GasEquipment_Impl>()->setSchedule(schedule);
}

void GasEquipment::resetSchedule()
{
  getImpl<detail::GasEquipment_Impl>()->resetSchedule();
}

GasEquipmentDefinition GasEquipment::gasEquipmentDefinition() const
{
  return getImpl<detail::GasEquipment_Impl>()->gasEquipmentDefinition();
}

bool GasEquipment::setGasEquipmentDefinition(const GasEquipmentDefinition& gasEquipmentDefinition)
{
  return getImpl<detail::GasEquipment_Impl>()->setGasEquipmentDefinition(gasEquipmentDefinition);
}

boost::optional<double> GasEquipment::designLevel() const {
  return getImpl<detail::GasEquipment_Impl>()->designLevel();
}

boost::optional<double> GasEquipment::powerPerFloorArea() const {
  return getImpl<detail::GasEquipment_Impl>()->powerPerFloorArea();
}

boost::optional<double> GasEquipment::powerPerPerson() const {
  return getImpl<detail::GasEquipment_Impl>()->powerPerPerson();
}

double GasEquipment::getDesignLevel(double floorArea, double numPeople) const {
  return getImpl<detail::GasEquipment_Impl>()->getDesignLevel(floorArea,numPeople);
}

double GasEquipment::getPowerPerFloorArea(double floorArea, double numPeople) const {
  return getImpl<detail::GasEquipment_Impl>()->getPowerPerFloorArea(floorArea,numPeople);
}

double GasEquipment::getPowerPerPerson(double floorArea, double numPeople) const {
  return getImpl<detail::GasEquipment_Impl>()->getPowerPerPerson(floorArea,numPeople);
}

/// @cond
GasEquipment::GasEquipment(boost::shared_ptr<detail::GasEquipment_Impl> impl)
  : SpaceLoadInstance(impl)
{}
/// @endcond


} // model
} // openstudio

