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

#include "ElectricEquipment.hpp"
#include "ElectricEquipment_Impl.hpp"

#include "ElectricEquipmentDefinition.hpp"
#include "ElectricEquipmentDefinition_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Space.hpp"
#include "Space_Impl.hpp"
#include "SpaceType.hpp"
#include "SpaceType_Impl.hpp"
#include "DefaultScheduleSet.hpp"
#include "DefaultScheduleSet_Impl.hpp"
#include "LifeCycleCost.hpp"
#include "Model.hpp"

#include <utilities/idd/OS_ElectricEquipment_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ElectricEquipment_Impl::ElectricEquipment_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : SpaceLoadInstance_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ElectricEquipment::iddObjectType());
  }

  ElectricEquipment_Impl::ElectricEquipment_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                 Model_Impl* model,
                                                 bool keepHandle)
    : SpaceLoadInstance_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ElectricEquipment::iddObjectType());
  }

  ElectricEquipment_Impl::ElectricEquipment_Impl(const ElectricEquipment_Impl& other,
                                                 Model_Impl* model,
                                                 bool keepHandle)
    : SpaceLoadInstance_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ElectricEquipment_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty())
    {
      result.push_back("Electric Equipment Electric Power");
      result.push_back("Electric Equipment Electric Energy");
      result.push_back("Electric Equipment Radiant Heating Energy");
      result.push_back("Electric Equipment Radiant Heating Rate");
      result.push_back("Electric Equipment Convective Heating Energy");
      result.push_back("Electric Equipment Convective Heating Rate");
      result.push_back("Electric Equipment Latent Gain Energy");
      result.push_back("Electric Equipment Latent Gain Rate");
      result.push_back("Electric Equipment Lost Heat Energy");
      result.push_back("Electric Equipment Lost Heat Rate");
      result.push_back("Electric Equipment Total Heating Energy");
      result.push_back("Electric Equipment Total Heating Rate");

      // Reported in ThermalZone
      //result.push_back("Zone Electric Equipment Electric Power");
      //result.push_back("Zone Electric Equipment Electric Energy");
      //result.push_back("Zone Electric Equipment Radiant Heating Energy");
      //result.push_back("Zone Electric Equipment Radiant Heating Rate");
      //result.push_back("Zone Electric Equipment Convective Heating Energy");
      //result.push_back("Zone Electric Equipment Convective Heating Rate");
      //result.push_back("Zone Electric Equipment Latent Gain Energy");
      //result.push_back("Zone Electric Equipment Latent Gain Rate");
      //result.push_back("Zone Electric Equipment Lost Heat Energy");
      //result.push_back("Zone Electric Equipment Lost Heat Rate");
      //result.push_back("Zone Electric Equipment Total Heating Energy");
      //result.push_back("Zone Electric Equipment Total Heating Rate");
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
    for (LifeCycleCost cost : electricEquipmentDefinition.lifeCycleCosts()){
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
    OS_ASSERT(value);
    return value.get();
  }

  bool ElectricEquipment_Impl::isMultiplierDefaulted() const {
    return isEmpty(OS_ElectricEquipmentFields::Multiplier);
  }

  bool ElectricEquipment_Impl::isAbsolute() const {
    ElectricEquipmentDefinition definition = electricEquipmentDefinition();
    if (definition.designLevel()) {
      return true;
    }
    return false;
  }

  std::string ElectricEquipment_Impl::endUseSubcategory() const {
    boost::optional<std::string> value = getString(OS_ElectricEquipmentFields::EndUseSubcategory,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ElectricEquipment_Impl::isEndUseSubcategoryDefaulted() const {
    return isEmpty(OS_ElectricEquipmentFields::EndUseSubcategory);
  }

  bool ElectricEquipment_Impl::setMultiplier(double multiplier) {
    bool result = setDouble(OS_ElectricEquipmentFields::Multiplier, multiplier);
    return result;
  }

  void ElectricEquipment_Impl::resetMultiplier() {
    bool result = setString(OS_ElectricEquipmentFields::Multiplier, "");
    OS_ASSERT(result);
  }

  bool ElectricEquipment_Impl::setEndUseSubcategory(std::string endUseSubcategory) {
    bool result = setString(OS_ElectricEquipmentFields::EndUseSubcategory, endUseSubcategory);
    OS_ASSERT(result);
    return result;
  }

  void ElectricEquipment_Impl::resetEndUseSubcategory() {
    bool result = setString(OS_ElectricEquipmentFields::EndUseSubcategory, "");
    OS_ASSERT(result);
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
  OS_ASSERT(getImpl<detail::ElectricEquipment_Impl>());

  /*
   *Schedule sch = this->model().alwaysOnDiscreteSchedule();
   *bool test = this->setSchedule(sch);
   *OS_ASSERT(test);
   *test = this->setMultiplier(1.0);
   *OS_ASSERT(test);
   */
  bool test = this->setEndUseSubcategory("General");
  OS_ASSERT(test);

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

bool ElectricEquipment::setEndUseSubcategory(std::string endUseSubcategory) {
  return getImpl<detail::ElectricEquipment_Impl>()->setEndUseSubcategory(endUseSubcategory);
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
ElectricEquipment::ElectricEquipment(std::shared_ptr<detail::ElectricEquipment_Impl> impl)
  : SpaceLoadInstance(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

