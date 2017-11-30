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

#include "Lights.hpp"
#include "Lights_Impl.hpp"

#include "LightsDefinition.hpp"
#include "LightsDefinition_Impl.hpp"
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

#include <utilities/idd/OS_Lights_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  Lights_Impl::Lights_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : SpaceLoadInstance_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == Lights::iddObjectType());
  }

  Lights_Impl::Lights_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                           Model_Impl* model,
                           bool keepHandle)
    : SpaceLoadInstance_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == Lights::iddObjectType());
  }

  Lights_Impl::Lights_Impl(const Lights_Impl& other,
                           Model_Impl* model,
                           bool keepHandle)
    : SpaceLoadInstance_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& Lights_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty())
    {
      result.push_back("Lights Electric Power");
      result.push_back("Lights Radiant Heat Gain");
      result.push_back("Lights Radiant Heating Rate");
      result.push_back("Lights Visible Radiation Heating Energy");
      result.push_back("Lights Visible Radiation Heating Rate");
      result.push_back("Lights Convective Heating Energy");
      result.push_back("Lights Convective Heating Rate");
      result.push_back("Lights Return Air Heating Energy");
      result.push_back("Lights Return Air Heating Rate");
      result.push_back("Lights Total Heating Energy");
      result.push_back("Lights Total Heating Rate");
      result.push_back("Lights Electric Energy");

      // Reported in ThermalZone
      //result.push_back("Zone Lights Electric Power");
      //result.push_back("Zone Lights Radiant Heating Energy");
      //result.push_back("Zone Lights Radiant Heating Rate");
      //result.push_back("Zone Lights Visible Radiation Heating Energy");
      //result.push_back("Zone Lights Visible Radiation Heating Rate");
      //result.push_back("Zone Lights Convective Heating Energy");
      //result.push_back("Zone Lights Convective Heating Rate");
      //result.push_back("Zone Lights Return Air Heating Energy");
      //result.push_back("Zone Lights Return Air Heating Rate");
      //result.push_back("Zone Lights Total Heating Energy");
      //result.push_back("Zone Lights Total Heating Rate");
      //result.push_back("Zone Lights Electric Energy");
    }
    return result;
  }


  IddObjectType Lights_Impl::iddObjectType() const {
    return Lights::iddObjectType();
  }

  std::vector<ScheduleTypeKey> Lights_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_LightsFields::ScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("Lights","Lighting"));
    }
    return result;
  }

  bool Lights_Impl::hardSize()
  {
    boost::optional<Space> space = this->space();
    if (!space){
      return false;
    }

    this->makeUnique();

    LightsDefinition lightsDefinition = this->lightsDefinition();
    for (LifeCycleCost cost : lightsDefinition.lifeCycleCosts()){
      cost.convertToCostPerEach();
    }

    boost::optional<double> lightingLevel = lightsDefinition.lightingLevel();
    if (lightingLevel){
      return true;
    }

    boost::optional<double> wattsperSpaceFloorArea = lightsDefinition.wattsperSpaceFloorArea();
    if (wattsperSpaceFloorArea){
      return lightsDefinition.setLightingLevel(*wattsperSpaceFloorArea * space->floorArea());
    }

    boost::optional<double> wattsperPerson = lightsDefinition.wattsperPerson();
    if (wattsperPerson){
      return lightsDefinition.setLightingLevel(*wattsperPerson * space->numberOfPeople());
    }

    return false;
  }

  bool Lights_Impl::hardApplySchedules()
  {
    bool result = false;
    boost::optional<Schedule> schedule = this->schedule();
    if (schedule){
      result = this->setSchedule(*schedule);
    }
    return result;
  }

  double Lights_Impl::fractionReplaceable() const {
    boost::optional<double> value = getDouble(OS_LightsFields::FractionReplaceable,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool Lights_Impl::isFractionReplaceableDefaulted() const {
    return isEmpty(OS_LightsFields::FractionReplaceable);
  }

  double Lights_Impl::multiplier() const {
    boost::optional<double> value = getDouble(OS_LightsFields::Multiplier,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool Lights_Impl::isMultiplierDefaulted() const {
    return isEmpty(OS_LightsFields::Multiplier);
  }

  bool Lights_Impl::isAbsolute() const {
    LightsDefinition definition = lightsDefinition();
    if (definition.lightingLevel()) {
      return true;
    }
    return false;
  }

  std::string Lights_Impl::endUseSubcategory() const {
    boost::optional<std::string> value = getString(OS_LightsFields::EndUseSubcategory,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool Lights_Impl::isEndUseSubcategoryDefaulted() const {
    return isEmpty(OS_LightsFields::EndUseSubcategory);
  }

  bool Lights_Impl::setFractionReplaceable(double fractionReplaceable) {
    bool result = setDouble(OS_LightsFields::FractionReplaceable, fractionReplaceable);
    return result;
  }

  void Lights_Impl::resetFractionReplaceable() {
    bool result = setString(OS_LightsFields::FractionReplaceable, "");
    OS_ASSERT(result);
  }

  bool Lights_Impl::setMultiplier(double multiplier) {
    bool result = setDouble(OS_LightsFields::Multiplier, multiplier);
    return result;
  }

  void Lights_Impl::resetMultiplier() {
    bool result = setString(OS_LightsFields::Multiplier, "");
    OS_ASSERT(result);
  }

  bool Lights_Impl::setEndUseSubcategory(std::string endUseSubcategory) {
    bool result = setString(OS_LightsFields::EndUseSubcategory, endUseSubcategory);
    OS_ASSERT(result);
    return result;
  }

  void Lights_Impl::resetEndUseSubcategory() {
    bool result = setString(OS_LightsFields::EndUseSubcategory, "");
    OS_ASSERT(result);
  }

  int Lights_Impl::spaceIndex() const {
    return OS_LightsFields::SpaceorSpaceTypeName;
  }

  int Lights_Impl::definitionIndex() const {
    return OS_LightsFields::LightsDefinitionName;
  }

  boost::optional<Schedule> Lights_Impl::schedule() const
  {
    boost::optional<Schedule> result = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_LightsFields::ScheduleName);
    if (!result){
      // search upwards
      OptionalSpace space = this->space();
      OptionalSpaceType spaceType = this->spaceType();
      if (space){
        result = space->getDefaultSchedule(DefaultScheduleType::LightingSchedule);
      }else if (spaceType){
        result = spaceType->getDefaultSchedule(DefaultScheduleType::LightingSchedule);
      }
    }
    return result;
  }

  bool Lights_Impl::isScheduleDefaulted() const
  {
    return isEmpty(OS_LightsFields::ScheduleName);
  }

  bool Lights_Impl::setSchedule(Schedule& schedule) {
    bool result = ModelObject_Impl::setSchedule(OS_LightsFields::ScheduleName,
                                                "Lights",
                                                "Lighting",
                                                schedule);
    return result;
  }

  void Lights_Impl::resetSchedule()
  {
    this->setString(OS_LightsFields::ScheduleName, "");
  }

  LightsDefinition Lights_Impl::lightsDefinition() const
  {
    return this->definition().cast<LightsDefinition>();
  }

  bool Lights_Impl::setLightsDefinition(const LightsDefinition& definition)
  {
    return this->setPointer(this->definitionIndex(), definition.handle());
  }

  bool Lights_Impl::setDefinition(const SpaceLoadDefinition& definition)
  {
    bool result = false;
    boost::optional<LightsDefinition> lightsDefinition = definition.optionalCast<LightsDefinition>();
    if (lightsDefinition){
      result = setLightsDefinition(*lightsDefinition);
    }
    return result;
  }

  boost::optional<double> Lights_Impl::lightingLevel() const {
    OptionalDouble temp = lightsDefinition().lightingLevel();
    if (temp) {
      return temp.get() * multiplier();
    }
    return temp;
  }

  boost::optional<double> Lights_Impl::powerPerFloorArea() const {
    OptionalDouble temp = lightsDefinition().wattsperSpaceFloorArea();
    if (temp) {
      return temp.get() * multiplier();
    }
    return temp;
  }

  boost::optional<double> Lights_Impl::powerPerPerson() const {
    OptionalDouble temp = lightsDefinition().wattsperPerson();
    if (temp) {
      return temp.get() * multiplier();
    }
    return temp;
  }

  double Lights_Impl::getLightingPower(double floorArea, double numPeople) const {
    return lightsDefinition().getLightingPower(floorArea,numPeople) * multiplier();
  }

  double Lights_Impl::getPowerPerFloorArea(double floorArea, double numPeople) const {
    return lightsDefinition().getPowerPerFloorArea(floorArea,numPeople) * multiplier();
  }

  double Lights_Impl::getPowerPerPerson(double floorArea, double numPeople) const {
    return lightsDefinition().getPowerPerPerson(floorArea,numPeople) * multiplier();
  }

  boost::optional<ModelObject> Lights_Impl::scheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = schedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> Lights_Impl::lightsDefinitionAsModelObject() const {
    OptionalModelObject result = lightsDefinition();
    return result;
  }

  bool Lights_Impl::setScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
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

  bool Lights_Impl::setLightsDefinitionAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalLightsDefinition intermediate = modelObject->optionalCast<LightsDefinition>();
      if (intermediate) {
        return setLightsDefinition(*intermediate);
      }
    }
    return false;
  }

} // detail

Lights::Lights(const LightsDefinition& lightsDefinition)
  : SpaceLoadInstance(Lights::iddObjectType(),lightsDefinition)
{
  OS_ASSERT(getImpl<detail::Lights_Impl>());

  /*
   *Schedule sch = this->model().alwaysOnDiscreteSchedule();
   *bool test = this->setSchedule(sch);
   *OS_ASSERT(test);
   *test = this->setMultiplier(1.0);
   *OS_ASSERT(test);
   */
  bool test = this->setEndUseSubcategory("General");
  OS_ASSERT(test);
  test = this->setFractionReplaceable(1.0);
  OS_ASSERT(test);
}

IddObjectType Lights::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Lights);
  return result;
}

double Lights::fractionReplaceable() const {
  return getImpl<detail::Lights_Impl>()->fractionReplaceable();
}

bool Lights::isFractionReplaceableDefaulted() const {
  return getImpl<detail::Lights_Impl>()->isFractionReplaceableDefaulted();
}

std::string Lights::endUseSubcategory() const {
  return getImpl<detail::Lights_Impl>()->endUseSubcategory();
}

bool Lights::isEndUseSubcategoryDefaulted() const {
  return getImpl<detail::Lights_Impl>()->isEndUseSubcategoryDefaulted();
}

bool Lights::setFractionReplaceable(double fractionReplaceable) {
  return getImpl<detail::Lights_Impl>()->setFractionReplaceable(fractionReplaceable);
}

void Lights::resetFractionReplaceable() {
  getImpl<detail::Lights_Impl>()->resetFractionReplaceable();
}

bool Lights::setMultiplier(double multiplier) {
  return getImpl<detail::Lights_Impl>()->setMultiplier(multiplier);
}

void Lights::resetMultiplier() {
  getImpl<detail::Lights_Impl>()->resetMultiplier();
}

bool Lights::setEndUseSubcategory(std::string endUseSubcategory) {
  return getImpl<detail::Lights_Impl>()->setEndUseSubcategory(endUseSubcategory);
}

void Lights::resetEndUseSubcategory() {
  getImpl<detail::Lights_Impl>()->resetEndUseSubcategory();
}

boost::optional<Schedule> Lights::schedule() const
{
  return getImpl<detail::Lights_Impl>()->schedule();
}

bool Lights::isScheduleDefaulted() const
{
  return getImpl<detail::Lights_Impl>()->isScheduleDefaulted();
}

bool Lights::setSchedule(Schedule& schedule)
{
  return getImpl<detail::Lights_Impl>()->setSchedule(schedule);
}

void Lights::resetSchedule()
{
  getImpl<detail::Lights_Impl>()->resetSchedule();
}

LightsDefinition Lights::lightsDefinition() const
{
  return getImpl<detail::Lights_Impl>()->lightsDefinition();
}

bool Lights::setLightsDefinition(const LightsDefinition& lightsDefinition)
{
  return getImpl<detail::Lights_Impl>()->setLightsDefinition(lightsDefinition);
}

boost::optional<double> Lights::lightingLevel() const {
  return getImpl<detail::Lights_Impl>()->lightingLevel();
}

boost::optional<double> Lights::powerPerFloorArea() const {
  return getImpl<detail::Lights_Impl>()->powerPerFloorArea();
}

boost::optional<double> Lights::powerPerPerson() const {
  return getImpl<detail::Lights_Impl>()->powerPerPerson();
}

double Lights::getLightingPower(double floorArea, double numPeople) const {
  return getImpl<detail::Lights_Impl>()->getLightingPower(floorArea,numPeople);
}

double Lights::getPowerPerFloorArea(double floorArea, double numPeople) const {
  return getImpl<detail::Lights_Impl>()->getPowerPerFloorArea(floorArea,numPeople);
}

double Lights::getPowerPerPerson(double floorArea, double numPeople) const {
  return getImpl<detail::Lights_Impl>()->getPowerPerPerson(floorArea,numPeople);
}

/// @cond
Lights::Lights(std::shared_ptr<detail::Lights_Impl> impl)
  : SpaceLoadInstance(std::move(impl))
{}
/// @endcond


} // model
} // openstudio

