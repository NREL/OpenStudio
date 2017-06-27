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

#include "GasEquipment.hpp"
#include "GasEquipment_Impl.hpp"

#include "GasEquipmentDefinition.hpp"
#include "GasEquipmentDefinition_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Space.hpp"
#include "Space_Impl.hpp"
#include "SpaceType.hpp"
#include "SpaceType_Impl.hpp"
#include "DefaultScheduleSet.hpp"
#include "DefaultScheduleSet_Impl.hpp"
#include "LifeCycleCost.hpp"

#include <utilities/idd/OS_GasEquipment_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  GasEquipment_Impl::GasEquipment_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : SpaceLoadInstance_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == GasEquipment::iddObjectType());
  }

  GasEquipment_Impl::GasEquipment_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                       Model_Impl* model,
                                       bool keepHandle)
    : SpaceLoadInstance_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == GasEquipment::iddObjectType());
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
    for (LifeCycleCost cost : gasEquipmentDefinition.lifeCycleCosts()){
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
    OS_ASSERT(value);
    return value.get();
  }

  bool GasEquipment_Impl::isMultiplierDefaulted() const {
    return isEmpty(OS_GasEquipmentFields::Multiplier);
  }

  bool GasEquipment_Impl::isAbsolute() const {
    GasEquipmentDefinition definition = gasEquipmentDefinition();
    if (definition.designLevel()) {
      return true;
    }
    return false;
  }

  std::string GasEquipment_Impl::endUseSubcategory() const {
    boost::optional<std::string> value = getString(OS_GasEquipmentFields::EndUseSubcategory,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool GasEquipment_Impl::isEndUseSubcategoryDefaulted() const {
    return isEmpty(OS_GasEquipmentFields::EndUseSubcategory);
  }

  bool GasEquipment_Impl::setMultiplier(double multiplier) {
    bool result = setDouble(OS_GasEquipmentFields::Multiplier, multiplier);
    return result;
  }

  void GasEquipment_Impl::resetMultiplier() {
    bool result = setString(OS_GasEquipmentFields::Multiplier, "");
    OS_ASSERT(result);
  }

  void GasEquipment_Impl::setEndUseSubcategory(std::string endUseSubcategory) {
    bool result = setString(OS_GasEquipmentFields::EndUseSubcategory, endUseSubcategory);
    OS_ASSERT(result);
  }

  void GasEquipment_Impl::resetEndUseSubcategory() {
    bool result = setString(OS_GasEquipmentFields::EndUseSubcategory, "");
    OS_ASSERT(result);
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
    OS_ASSERT(result);
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
  OS_ASSERT(getImpl<detail::GasEquipment_Impl>());
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
GasEquipment::GasEquipment(std::shared_ptr<detail::GasEquipment_Impl> impl)
  : SpaceLoadInstance(std::move(impl))
{}
/// @endcond


} // model
} // openstudio

