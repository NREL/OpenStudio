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

#include "SteamEquipment.hpp"
#include "SteamEquipment_Impl.hpp"

#include "SteamEquipmentDefinition.hpp"
#include "SteamEquipmentDefinition_Impl.hpp"
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

#include <utilities/idd/OS_SteamEquipment_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  SteamEquipment_Impl::SteamEquipment_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : SpaceLoadInstance_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SteamEquipment::iddObjectType());
  }

  SteamEquipment_Impl::SteamEquipment_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
    : SpaceLoadInstance_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SteamEquipment::iddObjectType());
  }

  SteamEquipment_Impl::SteamEquipment_Impl(const SteamEquipment_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
    : SpaceLoadInstance_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& SteamEquipment_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty())
    {
      result.push_back("Steam Equipment District Heating Rate");
      result.push_back("Steam Equipment District Heating Energy");
      result.push_back("Steam Equipment Radiant Heating Energy");
      result.push_back("Steam Equipment Radiant Heating Rate");
      result.push_back("Steam Equipment Convective Heating Energy");
      result.push_back("Steam Equipment Convective Heating Rate");
      result.push_back("Steam Equipment Latent Gain Energy");
      result.push_back("Steam Equipment Latent Gain Rate");
      result.push_back("Steam Equipment Lost Heat Energy");
      result.push_back("Steam Equipment Lost Heat Rate");
      result.push_back("Steam Equipment Total Heating Energy");
      result.push_back("Steam Equipment Total Heating Rate");

      // Reported in ThermalZone
      //result.push_back("Zone Steam Equipment District Heating Rate");
      //result.push_back("Zone Steam Equipment District Heating Energy");
      //result.push_back("Zone Steam Equipment Radiant Heating Energy");
      //result.push_back("Zone Steam Equipment Radiant Heating Rate");
      //result.push_back("Zone Steam Equipment Convective Heating Energy");
      //result.push_back("Zone Steam Equipment Convective Heating Rate");
      //result.push_back("Zone Steam Equipment Latent Gain Energy");
      //result.push_back("Zone Steam Equipment Latent Gain Rate");
      //result.push_back("Zone Steam Equipment Lost Heat Energy");
      //result.push_back("Zone Steam Equipment Lost Heat Rate");
      //result.push_back("Zone Steam Equipment Total Heating Energy");
      //result.push_back("Zone Steam Equipment Total Heating Rate");
    }
    return result;
  }

  IddObjectType SteamEquipment_Impl::iddObjectType() const {
    return SteamEquipment::iddObjectType();
  }

  std::vector<ScheduleTypeKey> SteamEquipment_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_SteamEquipmentFields::ScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("SteamEquipment","Steam Equipment"));
    }
    return result;
  }

  bool SteamEquipment_Impl::hardSize() {
    OptionalSpace space = this->space();
    if (!space) {
      return false;
    }

    makeUnique();

    SteamEquipmentDefinition definition = steamEquipmentDefinition();
    for (LifeCycleCost cost : definition.lifeCycleCosts()){
      cost.convertToCostPerEach();
    }

    if (definition.designLevel()) {
      return true;
    }
    if (OptionalDouble areaDensity = definition.wattsperSpaceFloorArea()) {
      return definition.setDesignLevel(*areaDensity * space->floorArea());
    }
    if (OptionalDouble peopleDensity = definition.wattsperPerson()) {
      return definition.setDesignLevel(*peopleDensity * space->numberOfPeople());
    }

    OS_ASSERT(false);
    return false;
  }

  bool SteamEquipment_Impl::hardApplySchedules() {
    if (OptionalSchedule schedule = this->schedule()) {
      return setSchedule(*schedule);
    }
    return false;
  }

  SteamEquipmentDefinition SteamEquipment_Impl::steamEquipmentDefinition() const {
    return definition().cast<SteamEquipmentDefinition>();
  }

  boost::optional<Schedule> SteamEquipment_Impl::schedule() const {
    OptionalSchedule result = getObject<SteamEquipment>().getModelObjectTarget<Schedule>(OS_SteamEquipmentFields::ScheduleName);
    if (!result) {
      // search upwards
      OptionalSpace space = this->space();
      OptionalSpaceType spaceType = this->spaceType();
      if (space){
        result = space->getDefaultSchedule(DefaultScheduleType::SteamEquipmentSchedule);
      } else if (spaceType){
        result = spaceType->getDefaultSchedule(DefaultScheduleType::SteamEquipmentSchedule);
      }
    }
    return result;
  }

  bool SteamEquipment_Impl::isScheduleDefaulted() const {
    return isEmpty(OS_SteamEquipmentFields::ScheduleName);
  }

  double SteamEquipment_Impl::multiplier() const {
    boost::optional<double> value = getDouble(OS_SteamEquipmentFields::Multiplier,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SteamEquipment_Impl::isMultiplierDefaulted() const {
    return isEmpty(OS_SteamEquipmentFields::Multiplier);
  }

  bool SteamEquipment_Impl::isAbsolute() const {
    SteamEquipmentDefinition definition = steamEquipmentDefinition();
    if (definition.designLevel()) {
      return true;
    }
    return false;
  }

  std::string SteamEquipment_Impl::endUseSubcategory() const {
    boost::optional<std::string> value = getString(OS_SteamEquipmentFields::EndUseSubcategory,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SteamEquipment_Impl::isEndUseSubcategoryDefaulted() const {
    return isEmpty(OS_SteamEquipmentFields::EndUseSubcategory);
  }

  bool SteamEquipment_Impl::setSteamEquipmentDefinition(const SteamEquipmentDefinition& definition) {
    return setPointer(definitionIndex(),definition.handle());
  }

  bool SteamEquipment_Impl::setDefinition(const SpaceLoadDefinition& definition) {
    if (OptionalSteamEquipmentDefinition steamEquipmentDefinition = definition.optionalCast<SteamEquipmentDefinition>()) {
      return setSteamEquipmentDefinition(*steamEquipmentDefinition);
    }
    return false;
  }

  bool SteamEquipment_Impl::setSchedule(Schedule& schedule) {
    bool result = ModelObject_Impl::setSchedule(OS_SteamEquipmentFields::ScheduleName,
                                                "SteamEquipment",
                                                "Steam Equipment",
                                                schedule);
    return result;
  }

  void SteamEquipment_Impl::resetSchedule() {
    bool ok = setString(OS_SteamEquipmentFields::ScheduleName,"");
    OS_ASSERT(ok);
  }

  bool SteamEquipment_Impl::setMultiplier(double multiplier) {
    bool result = setDouble(OS_SteamEquipmentFields::Multiplier, multiplier);
    return result;
  }

  void SteamEquipment_Impl::resetMultiplier() {
    bool result = setString(OS_SteamEquipmentFields::Multiplier, "");
    OS_ASSERT(result);
  }

  bool SteamEquipment_Impl::setEndUseSubcategory(std::string endUseSubcategory) {
    bool result = setString(OS_SteamEquipmentFields::EndUseSubcategory, endUseSubcategory);
    OS_ASSERT(result);
    return result;
  }

  void SteamEquipment_Impl::resetEndUseSubcategory() {
    bool result = setString(OS_SteamEquipmentFields::EndUseSubcategory, "");
    OS_ASSERT(result);
  }

  double SteamEquipment_Impl::getDesignLevel(double floorArea, double numPeople) const {
    return steamEquipmentDefinition().getDesignLevel(floorArea,numPeople) * multiplier();
  }

  double SteamEquipment_Impl::getPowerPerFloorArea(double floorArea, double numPeople) const
  {
    return steamEquipmentDefinition().getPowerPerFloorArea(floorArea,numPeople) * multiplier();
  }

  double SteamEquipment_Impl::getPowerPerPerson(double floorArea, double numPeople) const {
    return steamEquipmentDefinition().getPowerPerPerson(floorArea,numPeople) * multiplier();
  }

  int SteamEquipment_Impl::spaceIndex() const {
    return OS_SteamEquipmentFields::SpaceorSpaceTypeName;
  }

  int SteamEquipment_Impl::definitionIndex() const {
    return OS_SteamEquipmentFields::SteamEquipmentDefinitionName;
  }

  boost::optional<ModelObject> SteamEquipment_Impl::steamEquipmentDefinitionAsModelObject() const {
    OptionalModelObject result = steamEquipmentDefinition();
    return result;
  }

  boost::optional<ModelObject> SteamEquipment_Impl::scheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = schedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  bool SteamEquipment_Impl::setSteamEquipmentDefinitionAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSteamEquipmentDefinition intermediate = modelObject->optionalCast<SteamEquipmentDefinition>();
      if (intermediate) {
        return setSteamEquipmentDefinition(*intermediate);
      }
    }
    return false;
  }

  bool SteamEquipment_Impl::setScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
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

SteamEquipment::SteamEquipment(const SteamEquipmentDefinition& definition)
  : SpaceLoadInstance(SteamEquipment::iddObjectType(),definition)
{
  OS_ASSERT(getImpl<detail::SteamEquipment_Impl>());

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

IddObjectType SteamEquipment::iddObjectType() {
  IddObjectType result(IddObjectType::OS_SteamEquipment);
  return result;
}

SteamEquipmentDefinition SteamEquipment::steamEquipmentDefinition() const {
  return getImpl<detail::SteamEquipment_Impl>()->steamEquipmentDefinition();
}

boost::optional<Schedule> SteamEquipment::schedule() const {
  return getImpl<detail::SteamEquipment_Impl>()->schedule();
}

bool SteamEquipment::isScheduleDefaulted() const {
  return getImpl<detail::SteamEquipment_Impl>()->isScheduleDefaulted();
}

std::string SteamEquipment::endUseSubcategory() const {
  return getImpl<detail::SteamEquipment_Impl>()->endUseSubcategory();
}

bool SteamEquipment::isEndUseSubcategoryDefaulted() const {
  return getImpl<detail::SteamEquipment_Impl>()->isEndUseSubcategoryDefaulted();
}

bool SteamEquipment::setSteamEquipmentDefinition(const SteamEquipmentDefinition& definition) {
  return getImpl<detail::SteamEquipment_Impl>()->setSteamEquipmentDefinition(definition);
}

bool SteamEquipment::setSchedule(Schedule& schedule) {
  return getImpl<detail::SteamEquipment_Impl>()->setSchedule(schedule);
}

void SteamEquipment::resetSchedule() {
  getImpl<detail::SteamEquipment_Impl>()->resetSchedule();
}

bool SteamEquipment::setMultiplier(double multiplier) {
  return getImpl<detail::SteamEquipment_Impl>()->setMultiplier(multiplier);
}

void SteamEquipment::resetMultiplier() {
  getImpl<detail::SteamEquipment_Impl>()->resetMultiplier();
}

bool SteamEquipment::setEndUseSubcategory(std::string endUseSubcategory) {
  return getImpl<detail::SteamEquipment_Impl>()->setEndUseSubcategory(endUseSubcategory);
}

void SteamEquipment::resetEndUseSubcategory() {
  getImpl<detail::SteamEquipment_Impl>()->resetEndUseSubcategory();
}

double SteamEquipment::getDesignLevel(double floorArea, double numPeople) const {
  return getImpl<detail::SteamEquipment_Impl>()->getDesignLevel(floorArea,numPeople);
}

double SteamEquipment::getPowerPerFloorArea(double floorArea, double numPeople) const {
  return getImpl<detail::SteamEquipment_Impl>()->getPowerPerFloorArea(floorArea,numPeople);
}

double SteamEquipment::getPowerPerPerson(double floorArea, double numPeople) const {
  return getImpl<detail::SteamEquipment_Impl>()->getPowerPerPerson(floorArea,numPeople);
}

/// @cond
SteamEquipment::SteamEquipment(std::shared_ptr<detail::SteamEquipment_Impl> impl)
  : SpaceLoadInstance(std::move(impl))
{}
/// @endcond

} // model
} // openstudio
