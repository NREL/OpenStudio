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

#include "HotWaterEquipment.hpp"
#include "HotWaterEquipment_Impl.hpp"

#include "HotWaterEquipmentDefinition.hpp"
#include "HotWaterEquipmentDefinition_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Space.hpp"
#include "Space_Impl.hpp"
#include "SpaceType.hpp"
#include "SpaceType_Impl.hpp"
#include "DefaultScheduleSet.hpp"
#include "DefaultScheduleSet_Impl.hpp"
#include "LifeCycleCost.hpp"

#include <utilities/idd/OS_HotWaterEquipment_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  HotWaterEquipment_Impl::HotWaterEquipment_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : SpaceLoadInstance_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == HotWaterEquipment::iddObjectType());
  }

  HotWaterEquipment_Impl::HotWaterEquipment_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                 Model_Impl* model,
                                                 bool keepHandle)
    : SpaceLoadInstance_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == HotWaterEquipment::iddObjectType());
  }

  HotWaterEquipment_Impl::HotWaterEquipment_Impl(const HotWaterEquipment_Impl& other,
                                                 Model_Impl* model,
                                                 bool keepHandle)
    : SpaceLoadInstance_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& HotWaterEquipment_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType HotWaterEquipment_Impl::iddObjectType() const {
    return HotWaterEquipment::iddObjectType();
  }

  std::vector<ScheduleTypeKey> HotWaterEquipment_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_HotWaterEquipmentFields::ScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("HotWaterEquipment","Hot Water Equipment"));
    }
    return result;
  }

  bool HotWaterEquipment_Impl::hardSize()
  {
    boost::optional<Space> space = this->space();
    if (!space){
      return false;
    }

    this->makeUnique();

    HotWaterEquipmentDefinition hotWaterEquipmentDefinition = this->hotWaterEquipmentDefinition();
    for (LifeCycleCost cost : hotWaterEquipmentDefinition.lifeCycleCosts()){
      cost.convertToCostPerEach();
    }

    boost::optional<double> designLevel = hotWaterEquipmentDefinition.designLevel();
    if (designLevel){
      return true;
    }

    boost::optional<double> wattsperSpaceFloorArea = hotWaterEquipmentDefinition.wattsperSpaceFloorArea();
    if (wattsperSpaceFloorArea){
      return hotWaterEquipmentDefinition.setDesignLevel(*wattsperSpaceFloorArea * space->floorArea());
    }

    boost::optional<double> wattsperPerson = hotWaterEquipmentDefinition.wattsperPerson();
    if (wattsperPerson){
      return hotWaterEquipmentDefinition.setDesignLevel(*wattsperPerson * space->numberOfPeople());
    }

    return false;
  }

  bool HotWaterEquipment_Impl::hardApplySchedules()
  {
    bool result = false;
    boost::optional<Schedule> schedule = this->schedule();
    if (schedule){
      result = this->setSchedule(*schedule);
    }
    return result;
  }

  double HotWaterEquipment_Impl::multiplier() const {
    boost::optional<double> value = getDouble(OS_HotWaterEquipmentFields::Multiplier,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool HotWaterEquipment_Impl::isMultiplierDefaulted() const {
    return isEmpty(OS_HotWaterEquipmentFields::Multiplier);
  }

  bool HotWaterEquipment_Impl::isAbsolute() const {
    HotWaterEquipmentDefinition definition = hotWaterEquipmentDefinition();
    if (definition.designLevel()) {
      return true;
    }
    return false;
  }

  std::string HotWaterEquipment_Impl::endUseSubcategory() const {
    boost::optional<std::string> value = getString(OS_HotWaterEquipmentFields::EndUseSubcategory,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool HotWaterEquipment_Impl::isEndUseSubcategoryDefaulted() const {
    return isEmpty(OS_HotWaterEquipmentFields::EndUseSubcategory);
  }

  bool HotWaterEquipment_Impl::setMultiplier(double multiplier) {
    bool result = setDouble(OS_HotWaterEquipmentFields::Multiplier, multiplier);
    return result;
  }

  void HotWaterEquipment_Impl::resetMultiplier() {
    bool result = setString(OS_HotWaterEquipmentFields::Multiplier, "");
    OS_ASSERT(result);
  }

  void HotWaterEquipment_Impl::setEndUseSubcategory(std::string endUseSubcategory) {
    bool result = setString(OS_HotWaterEquipmentFields::EndUseSubcategory, endUseSubcategory);
    OS_ASSERT(result);
  }

  void HotWaterEquipment_Impl::resetEndUseSubcategory() {
    bool result = setString(OS_HotWaterEquipmentFields::EndUseSubcategory, "");
    OS_ASSERT(result);
  }

  int HotWaterEquipment_Impl::spaceIndex() const {
    return OS_HotWaterEquipmentFields::SpaceorSpaceTypeName;
  }

  int HotWaterEquipment_Impl::definitionIndex() const {
    return OS_HotWaterEquipmentFields::HotWaterEquipmentDefinitionName;
  }

  boost::optional<Schedule> HotWaterEquipment_Impl::schedule() const
  {
    boost::optional<Schedule> result = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_HotWaterEquipmentFields::ScheduleName);
    if (!result){
      // search upwards
      OptionalSpace space = this->space();
      OptionalSpaceType spaceType = this->spaceType();
      if (space){
        result = space->getDefaultSchedule(DefaultScheduleType::HotWaterEquipmentSchedule);
      } else if (spaceType){
        result = spaceType->getDefaultSchedule(DefaultScheduleType::HotWaterEquipmentSchedule);
      }
    }
    return result;
  }

  bool HotWaterEquipment_Impl::isScheduleDefaulted() const
  {
    return isEmpty(OS_HotWaterEquipmentFields::ScheduleName);
  }

  bool HotWaterEquipment_Impl::setSchedule(Schedule& schedule) {
    bool result = ModelObject_Impl::setSchedule(
                      OS_HotWaterEquipmentFields::ScheduleName,
                      "HotWaterEquipment",
                      "Hot Water Equipment",
                      schedule);
    return result;
  }

  void HotWaterEquipment_Impl::resetSchedule() {
    bool result = setString(OS_HotWaterEquipmentFields::ScheduleName, "");
    OS_ASSERT(result);
  }

  HotWaterEquipmentDefinition HotWaterEquipment_Impl::hotWaterEquipmentDefinition() const
  {
    return this->definition().cast<HotWaterEquipmentDefinition>();
  }

  bool HotWaterEquipment_Impl::setHotWaterEquipmentDefinition(const HotWaterEquipmentDefinition& definition)
  {
    return this->setPointer(this->definitionIndex(), definition.handle());
  }

  bool HotWaterEquipment_Impl::setDefinition(const SpaceLoadDefinition& definition)
  {
    bool result = false;
    boost::optional<HotWaterEquipmentDefinition> hotWaterEquipmentDefinition = definition.optionalCast<HotWaterEquipmentDefinition>();
    if (hotWaterEquipmentDefinition){
      result = setHotWaterEquipmentDefinition(*hotWaterEquipmentDefinition);
    }
    return result;
  }

  double HotWaterEquipment_Impl::getDesignLevel(double floorArea, double numPeople) const {
    return hotWaterEquipmentDefinition().getDesignLevel(floorArea,numPeople) * multiplier();
  }

  double HotWaterEquipment_Impl::getPowerPerFloorArea(double floorArea, double numPeople) const
  {
    return hotWaterEquipmentDefinition().getPowerPerFloorArea(floorArea,numPeople) * multiplier();
  }

  double HotWaterEquipment_Impl::getPowerPerPerson(double floorArea, double numPeople) const {
    return hotWaterEquipmentDefinition().getPowerPerPerson(floorArea,numPeople) * multiplier();
  }

  boost::optional<ModelObject> HotWaterEquipment_Impl::hotWaterEquipmentDefinitionAsModelObject() const {
    OptionalModelObject result = hotWaterEquipmentDefinition();
    return result;
  }

  boost::optional<ModelObject> HotWaterEquipment_Impl::scheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = schedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  bool HotWaterEquipment_Impl::setHotWaterEquipmentDefinitionAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalHotWaterEquipmentDefinition intermediate = modelObject->optionalCast<HotWaterEquipmentDefinition>();
      if (intermediate) {
        return setHotWaterEquipmentDefinition(*intermediate);
      }
    }
    return false;
  }

  bool HotWaterEquipment_Impl::setScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
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

HotWaterEquipment::HotWaterEquipment(const HotWaterEquipmentDefinition& hotWaterEquipmentDefinition)
  : SpaceLoadInstance(HotWaterEquipment::iddObjectType(),hotWaterEquipmentDefinition)
{
  OS_ASSERT(getImpl<detail::HotWaterEquipment_Impl>());
}

IddObjectType HotWaterEquipment::iddObjectType() {
  IddObjectType result(IddObjectType::OS_HotWaterEquipment);
  return result;
}

std::string HotWaterEquipment::endUseSubcategory() const {
  return getImpl<detail::HotWaterEquipment_Impl>()->endUseSubcategory();
}

bool HotWaterEquipment::isEndUseSubcategoryDefaulted() const {
  return getImpl<detail::HotWaterEquipment_Impl>()->isEndUseSubcategoryDefaulted();
}

bool HotWaterEquipment::setMultiplier(double multiplier) {
  return getImpl<detail::HotWaterEquipment_Impl>()->setMultiplier(multiplier);
}

void HotWaterEquipment::resetMultiplier() {
  getImpl<detail::HotWaterEquipment_Impl>()->resetMultiplier();
}

void HotWaterEquipment::setEndUseSubcategory(std::string endUseSubcategory) {
  getImpl<detail::HotWaterEquipment_Impl>()->setEndUseSubcategory(endUseSubcategory);
}

void HotWaterEquipment::resetEndUseSubcategory() {
  getImpl<detail::HotWaterEquipment_Impl>()->resetEndUseSubcategory();
}

boost::optional<Schedule> HotWaterEquipment::schedule() const
{
  return getImpl<detail::HotWaterEquipment_Impl>()->schedule();
}

bool HotWaterEquipment::isScheduleDefaulted() const
{
  return getImpl<detail::HotWaterEquipment_Impl>()->isScheduleDefaulted();
}

bool HotWaterEquipment::setSchedule(Schedule& schedule)
{
  return getImpl<detail::HotWaterEquipment_Impl>()->setSchedule(schedule);
}

void HotWaterEquipment::resetSchedule()
{
  getImpl<detail::HotWaterEquipment_Impl>()->resetSchedule();
}

HotWaterEquipmentDefinition HotWaterEquipment::hotWaterEquipmentDefinition() const
{
  return getImpl<detail::HotWaterEquipment_Impl>()->hotWaterEquipmentDefinition();
}

bool HotWaterEquipment::setHotWaterEquipmentDefinition(const HotWaterEquipmentDefinition& hotWaterEquipmentDefinition)
{
  return getImpl<detail::HotWaterEquipment_Impl>()->setHotWaterEquipmentDefinition(hotWaterEquipmentDefinition);
}

double HotWaterEquipment::getDesignLevel(double floorArea, double numPeople) const {
  return getImpl<detail::HotWaterEquipment_Impl>()->getDesignLevel(floorArea,numPeople);
}

double HotWaterEquipment::getPowerPerFloorArea(double floorArea, double numPeople) const {
  return getImpl<detail::HotWaterEquipment_Impl>()->getPowerPerFloorArea(floorArea,numPeople);
}

double HotWaterEquipment::getPowerPerPerson(double floorArea, double numPeople) const {
  return getImpl<detail::HotWaterEquipment_Impl>()->getPowerPerPerson(floorArea,numPeople);
}

/// @cond
HotWaterEquipment::HotWaterEquipment(std::shared_ptr<detail::HotWaterEquipment_Impl> impl)
  : SpaceLoadInstance(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

