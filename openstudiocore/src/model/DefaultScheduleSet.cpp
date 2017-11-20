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

#include "DefaultScheduleSet.hpp"
#include "DefaultScheduleSet_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"

#include <utilities/idd/OS_DefaultScheduleSet_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  DefaultScheduleSet_Impl::DefaultScheduleSet_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ResourceObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == DefaultScheduleSet::iddObjectType());
  }

  DefaultScheduleSet_Impl::DefaultScheduleSet_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                   Model_Impl* model,
                                                   bool keepHandle)
    : ResourceObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == DefaultScheduleSet::iddObjectType());
  }

  DefaultScheduleSet_Impl::DefaultScheduleSet_Impl(const DefaultScheduleSet_Impl& other,
                                                   Model_Impl* model,
                                                   bool keepHandle)
    : ResourceObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& DefaultScheduleSet_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType DefaultScheduleSet_Impl::iddObjectType() const {
    return DefaultScheduleSet::iddObjectType();
  }

  std::vector<ScheduleTypeKey> DefaultScheduleSet_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_DefaultScheduleSetFields::HoursofOperationScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("DefaultScheduleSet","Hours of Operation"));
    }
    if (std::find(b,e,OS_DefaultScheduleSetFields::NumberofPeopleScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("DefaultScheduleSet","Number of People"));
    }
    if (std::find(b,e,OS_DefaultScheduleSetFields::PeopleActivityLevelScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("DefaultScheduleSet","People Activity Level"));
    }
    if (std::find(b,e,OS_DefaultScheduleSetFields::LightingScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("DefaultScheduleSet","Lighting"));
    }
    if (std::find(b,e,OS_DefaultScheduleSetFields::ElectricEquipmentScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("DefaultScheduleSet","Electric Equipment"));
    }
    if (std::find(b,e,OS_DefaultScheduleSetFields::GasEquipmentScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("DefaultScheduleSet","Gas Equipment"));
    }
    if (std::find(b,e,OS_DefaultScheduleSetFields::HotWaterEquipmentScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("DefaultScheduleSet","Hot Water Equipment"));
    }
    if (std::find(b,e,OS_DefaultScheduleSetFields::InfiltrationScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("DefaultScheduleSet","Infiltration"));
    }
    if (std::find(b,e,OS_DefaultScheduleSetFields::SteamEquipmentScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("DefaultScheduleSet","Steam Equipment"));
    }
    if (std::find(b,e,OS_DefaultScheduleSetFields::OtherEquipmentScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("DefaultScheduleSet","Other Equipment"));
    }
    return result;
  }

  boost::optional<Schedule> DefaultScheduleSet_Impl::hoursofOperationSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_DefaultScheduleSetFields::HoursofOperationScheduleName);
  }

  boost::optional<Schedule> DefaultScheduleSet_Impl::numberofPeopleSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_DefaultScheduleSetFields::NumberofPeopleScheduleName);
  }

  boost::optional<Schedule> DefaultScheduleSet_Impl::peopleActivityLevelSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_DefaultScheduleSetFields::PeopleActivityLevelScheduleName);
  }

  boost::optional<Schedule> DefaultScheduleSet_Impl::lightingSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_DefaultScheduleSetFields::LightingScheduleName);
  }

  boost::optional<Schedule> DefaultScheduleSet_Impl::electricEquipmentSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_DefaultScheduleSetFields::ElectricEquipmentScheduleName);
  }

  boost::optional<Schedule> DefaultScheduleSet_Impl::gasEquipmentSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_DefaultScheduleSetFields::GasEquipmentScheduleName);
  }

  boost::optional<Schedule> DefaultScheduleSet_Impl::hotWaterEquipmentSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_DefaultScheduleSetFields::HotWaterEquipmentScheduleName);
  }

  boost::optional<Schedule> DefaultScheduleSet_Impl::infiltrationSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_DefaultScheduleSetFields::InfiltrationScheduleName);
  }

  boost::optional<Schedule> DefaultScheduleSet_Impl::steamEquipmentSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_DefaultScheduleSetFields::SteamEquipmentScheduleName);
  }

  boost::optional<Schedule> DefaultScheduleSet_Impl::otherEquipmentSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_DefaultScheduleSetFields::OtherEquipmentScheduleName);
  }

  bool DefaultScheduleSet_Impl::setHoursofOperationSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_DefaultScheduleSetFields::HoursofOperationScheduleName,
                              "DefaultScheduleSet",
                              "Hours of Operation",
                              schedule);
    return result;
  }

  void DefaultScheduleSet_Impl::resetHoursofOperationSchedule() {
    bool result = setString(OS_DefaultScheduleSetFields::HoursofOperationScheduleName, "");
    OS_ASSERT(result);
  }

  bool DefaultScheduleSet_Impl::setNumberofPeopleSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_DefaultScheduleSetFields::NumberofPeopleScheduleName,
                              "DefaultScheduleSet",
                              "Number of People",
                              schedule);
    return result;
  }

  void DefaultScheduleSet_Impl::resetNumberofPeopleSchedule() {
    bool result = setString(OS_DefaultScheduleSetFields::NumberofPeopleScheduleName, "");
    OS_ASSERT(result);
  }

  bool DefaultScheduleSet_Impl::setPeopleActivityLevelSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_DefaultScheduleSetFields::PeopleActivityLevelScheduleName,
                              "DefaultScheduleSet",
                              "People Activity Level",
                              schedule);
    return result;
  }

  void DefaultScheduleSet_Impl::resetPeopleActivityLevelSchedule() {
    bool result = setString(OS_DefaultScheduleSetFields::PeopleActivityLevelScheduleName, "");
    OS_ASSERT(result);
  }

  bool DefaultScheduleSet_Impl::setLightingSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_DefaultScheduleSetFields::LightingScheduleName,
                              "DefaultScheduleSet",
                              "Lighting",
                              schedule);
    return result;
  }

  void DefaultScheduleSet_Impl::resetLightingSchedule() {
    bool result = setString(OS_DefaultScheduleSetFields::LightingScheduleName, "");
    OS_ASSERT(result);
  }

  bool DefaultScheduleSet_Impl::setElectricEquipmentSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_DefaultScheduleSetFields::ElectricEquipmentScheduleName,
                              "DefaultScheduleSet",
                              "Electric Equipment",
                              schedule);
    return result;
  }

  void DefaultScheduleSet_Impl::resetElectricEquipmentSchedule() {
    bool result = setString(OS_DefaultScheduleSetFields::ElectricEquipmentScheduleName, "");
    OS_ASSERT(result);
  }

  bool DefaultScheduleSet_Impl::setGasEquipmentSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_DefaultScheduleSetFields::GasEquipmentScheduleName,
                              "DefaultScheduleSet",
                              "Gas Equipment",
                              schedule);
    return result;
  }

  void DefaultScheduleSet_Impl::resetGasEquipmentSchedule() {
    bool result = setString(OS_DefaultScheduleSetFields::GasEquipmentScheduleName, "");
    OS_ASSERT(result);
  }

  bool DefaultScheduleSet_Impl::setHotWaterEquipmentSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_DefaultScheduleSetFields::HotWaterEquipmentScheduleName,
                              "DefaultScheduleSet",
                              "Hot Water Equipment",
                              schedule);
    return result;
  }

  void DefaultScheduleSet_Impl::resetHotWaterEquipmentSchedule() {
    bool result = setString(OS_DefaultScheduleSetFields::HotWaterEquipmentScheduleName, "");
    OS_ASSERT(result);
  }

  bool DefaultScheduleSet_Impl::setInfiltrationSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_DefaultScheduleSetFields::InfiltrationScheduleName,
                              "DefaultScheduleSet",
                              "Infiltration",
                              schedule);
    return result;
  }

  void DefaultScheduleSet_Impl::resetInfiltrationSchedule() {
    bool result = setString(OS_DefaultScheduleSetFields::InfiltrationScheduleName, "");
    OS_ASSERT(result);
  }

  bool DefaultScheduleSet_Impl::setSteamEquipmentSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_DefaultScheduleSetFields::SteamEquipmentScheduleName,
                              "DefaultScheduleSet",
                              "Steam Equipment",
                              schedule);
    return result;
  }

  void DefaultScheduleSet_Impl::resetSteamEquipmentSchedule() {
    bool result = setString(OS_DefaultScheduleSetFields::SteamEquipmentScheduleName, "");
    OS_ASSERT(result);
  }

  bool DefaultScheduleSet_Impl::setOtherEquipmentSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_DefaultScheduleSetFields::OtherEquipmentScheduleName,
                              "DefaultScheduleSet",
                              "Other Equipment",
                              schedule);
    return result;
  }

  void DefaultScheduleSet_Impl::resetOtherEquipmentSchedule() {
    bool result = setString(OS_DefaultScheduleSetFields::OtherEquipmentScheduleName, "");
    OS_ASSERT(result);
  }

  boost::optional<Schedule> DefaultScheduleSet_Impl::getDefaultSchedule(const DefaultScheduleType& defaultScheduleType) const
  {
    unsigned index = defaultScheduleType.value() + 1;
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(index);
  }

  void DefaultScheduleSet_Impl::merge(const DefaultScheduleSet& other)
  {
    boost::optional<Schedule> schedule;

    if (!(this->hoursofOperationSchedule())){
      schedule = other.hoursofOperationSchedule();
      if (schedule){
        this->setHoursofOperationSchedule(*schedule);
      }
    }

    if (!(this->numberofPeopleSchedule())){
      schedule = other.numberofPeopleSchedule();
      if (schedule){
        this->setNumberofPeopleSchedule(*schedule);
      }
    }

    if (!(this->peopleActivityLevelSchedule())){
      schedule = other.peopleActivityLevelSchedule();
      if (schedule){
        this->setPeopleActivityLevelSchedule(*schedule);
      }
    }

    if (!(this->lightingSchedule())){
      schedule = other.lightingSchedule();
      if (schedule){
        this->setLightingSchedule(*schedule);
      }
    }

    if (!(this->electricEquipmentSchedule())){
      schedule = other.electricEquipmentSchedule();
      if (schedule){
        this->setElectricEquipmentSchedule(*schedule);
      }
    }

    if (!(this->gasEquipmentSchedule())){
      schedule = other.gasEquipmentSchedule();
      if (schedule){
        this->setGasEquipmentSchedule(*schedule);
      }
    }

    if (!(this->hotWaterEquipmentSchedule())){
      schedule = other.hotWaterEquipmentSchedule();
      if (schedule){
        this->setHotWaterEquipmentSchedule(*schedule);
      }
    }

    if (!(this->steamEquipmentSchedule())){
      schedule = other.steamEquipmentSchedule();
      if (schedule){
        this->setSteamEquipmentSchedule(*schedule);
      }
    }

    if (!(this->otherEquipmentSchedule())){
      schedule = other.otherEquipmentSchedule();
      if (schedule){
        this->setOtherEquipmentSchedule(*schedule);
      }
    }

    if (!(this->infiltrationSchedule())){
      schedule = other.infiltrationSchedule();
      if (schedule){
        this->setInfiltrationSchedule(*schedule);
      }
    }

  }

  boost::optional<ModelObject> DefaultScheduleSet_Impl::hoursofOperationScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = hoursofOperationSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> DefaultScheduleSet_Impl::numberofPeopleScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = numberofPeopleSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> DefaultScheduleSet_Impl::peopleActivityLevelScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = peopleActivityLevelSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> DefaultScheduleSet_Impl::lightingScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = lightingSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> DefaultScheduleSet_Impl::electricEquipmentScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = electricEquipmentSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> DefaultScheduleSet_Impl::gasEquipmentScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = gasEquipmentSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> DefaultScheduleSet_Impl::hotWaterEquipmentScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = hotWaterEquipmentSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> DefaultScheduleSet_Impl::infiltrationScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = infiltrationSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> DefaultScheduleSet_Impl::steamEquipmentScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = steamEquipmentSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> DefaultScheduleSet_Impl::otherEquipmentScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = otherEquipmentSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  bool DefaultScheduleSet_Impl::setHoursofOperationScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setHoursofOperationSchedule(schedule);
      }
      else {
        return false;
      }
    }
    else {
      resetHoursofOperationSchedule();
    }
    return true;
  }

  bool DefaultScheduleSet_Impl::setNumberofPeopleScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
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

  bool DefaultScheduleSet_Impl::setPeopleActivityLevelScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setPeopleActivityLevelSchedule(schedule);
      }
      else {
        return false;
      }
    }
    else {
      resetPeopleActivityLevelSchedule();
    }
    return true;
  }

  bool DefaultScheduleSet_Impl::setLightingScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setLightingSchedule(schedule);
      }
      else {
        return false;
      }
    }
    else {
      resetLightingSchedule();
    }
    return true;
  }

  bool DefaultScheduleSet_Impl::setElectricEquipmentScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setElectricEquipmentSchedule(schedule);
      }
      else {
        return false;
      }
    }
    else {
      resetElectricEquipmentSchedule();
    }
    return true;
  }

  bool DefaultScheduleSet_Impl::setGasEquipmentScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setGasEquipmentSchedule(schedule);
      }
      else {
        return false;
      }
    }
    else {
      resetGasEquipmentSchedule();
    }
    return true;
  }

  bool DefaultScheduleSet_Impl::setHotWaterEquipmentScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setHotWaterEquipmentSchedule(schedule);
      }
      else {
        return false;
      }
    }
    else {
      resetHotWaterEquipmentSchedule();
    }
    return true;
  }

  bool DefaultScheduleSet_Impl::setInfiltrationScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setInfiltrationSchedule(schedule);
      }
      else {
        return false;
      }
    }
    else {
      resetInfiltrationSchedule();
    }
    return true;
  }

  bool DefaultScheduleSet_Impl::setSteamEquipmentScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setSteamEquipmentSchedule(schedule);
      }
      else {
        return false;
      }
    }
    else {
      resetSteamEquipmentSchedule();
    }
    return true;
  }

  bool DefaultScheduleSet_Impl::setOtherEquipmentScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setOtherEquipmentSchedule(schedule);
      }
      else {
        return false;
      }
    }
    else {
      resetOtherEquipmentSchedule();
    }
    return true;
  }

} // detail

DefaultScheduleSet::DefaultScheduleSet(const Model& model)
  : ResourceObject(DefaultScheduleSet::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::DefaultScheduleSet_Impl>());
}

IddObjectType DefaultScheduleSet::iddObjectType() {
  IddObjectType result(IddObjectType::OS_DefaultScheduleSet);
  return result;
}

boost::optional<Schedule> DefaultScheduleSet::hoursofOperationSchedule() const
{
  return getImpl<detail::DefaultScheduleSet_Impl>()->hoursofOperationSchedule();
}

bool DefaultScheduleSet::setHoursofOperationSchedule(Schedule& schedule)
{
  return getImpl<detail::DefaultScheduleSet_Impl>()->setHoursofOperationSchedule(schedule);
}

void DefaultScheduleSet::resetHoursofOperationSchedule()
{
  getImpl<detail::DefaultScheduleSet_Impl>()->resetHoursofOperationSchedule();
}

boost::optional<Schedule> DefaultScheduleSet::numberofPeopleSchedule() const
{
  return getImpl<detail::DefaultScheduleSet_Impl>()->numberofPeopleSchedule();
}

bool DefaultScheduleSet::setNumberofPeopleSchedule(Schedule& schedule)
{
  return getImpl<detail::DefaultScheduleSet_Impl>()->setNumberofPeopleSchedule(schedule);
}

void DefaultScheduleSet::resetNumberofPeopleSchedule()
{
  getImpl<detail::DefaultScheduleSet_Impl>()->resetNumberofPeopleSchedule();
}

boost::optional<Schedule> DefaultScheduleSet::peopleActivityLevelSchedule() const
{
  return getImpl<detail::DefaultScheduleSet_Impl>()->peopleActivityLevelSchedule();
}

bool DefaultScheduleSet::setPeopleActivityLevelSchedule(Schedule& schedule)
{
  return getImpl<detail::DefaultScheduleSet_Impl>()->setPeopleActivityLevelSchedule(schedule);
}

void DefaultScheduleSet::resetPeopleActivityLevelSchedule()
{
  getImpl<detail::DefaultScheduleSet_Impl>()->resetPeopleActivityLevelSchedule();
}

boost::optional<Schedule> DefaultScheduleSet::lightingSchedule() const
{
  return getImpl<detail::DefaultScheduleSet_Impl>()->lightingSchedule();
}

bool DefaultScheduleSet::setLightingSchedule(Schedule& schedule)
{
  return getImpl<detail::DefaultScheduleSet_Impl>()->setLightingSchedule(schedule);
}

void DefaultScheduleSet::resetLightingSchedule()
{
  getImpl<detail::DefaultScheduleSet_Impl>()->resetLightingSchedule();
}

boost::optional<Schedule> DefaultScheduleSet::electricEquipmentSchedule() const
{
  return getImpl<detail::DefaultScheduleSet_Impl>()->electricEquipmentSchedule();
}

bool DefaultScheduleSet::setElectricEquipmentSchedule(Schedule& schedule)
{
  return getImpl<detail::DefaultScheduleSet_Impl>()->setElectricEquipmentSchedule(schedule);
}

void DefaultScheduleSet::resetElectricEquipmentSchedule()
{
  getImpl<detail::DefaultScheduleSet_Impl>()->resetElectricEquipmentSchedule();
}

boost::optional<Schedule> DefaultScheduleSet::gasEquipmentSchedule() const
{
  return getImpl<detail::DefaultScheduleSet_Impl>()->gasEquipmentSchedule();
}

bool DefaultScheduleSet::setGasEquipmentSchedule(Schedule& schedule)
{
  return getImpl<detail::DefaultScheduleSet_Impl>()->setGasEquipmentSchedule(schedule);
}

void DefaultScheduleSet::resetGasEquipmentSchedule()
{
  getImpl<detail::DefaultScheduleSet_Impl>()->resetGasEquipmentSchedule();
}

boost::optional<Schedule> DefaultScheduleSet::hotWaterEquipmentSchedule() const
{
  return getImpl<detail::DefaultScheduleSet_Impl>()->hotWaterEquipmentSchedule();
}

bool DefaultScheduleSet::setHotWaterEquipmentSchedule(Schedule& schedule)
{
  return getImpl<detail::DefaultScheduleSet_Impl>()->setHotWaterEquipmentSchedule(schedule);
}

void DefaultScheduleSet::resetHotWaterEquipmentSchedule()
{
  getImpl<detail::DefaultScheduleSet_Impl>()->resetHotWaterEquipmentSchedule();
}

boost::optional<Schedule> DefaultScheduleSet::steamEquipmentSchedule() const
{
  return getImpl<detail::DefaultScheduleSet_Impl>()->steamEquipmentSchedule();
}

bool DefaultScheduleSet::setSteamEquipmentSchedule(Schedule& schedule)
{
  return getImpl<detail::DefaultScheduleSet_Impl>()->setSteamEquipmentSchedule(schedule);
}

void DefaultScheduleSet::resetSteamEquipmentSchedule()
{
  getImpl<detail::DefaultScheduleSet_Impl>()->resetSteamEquipmentSchedule();
}

boost::optional<Schedule> DefaultScheduleSet::otherEquipmentSchedule() const
{
  return getImpl<detail::DefaultScheduleSet_Impl>()->otherEquipmentSchedule();
}

bool DefaultScheduleSet::setOtherEquipmentSchedule(Schedule& schedule)
{
  return getImpl<detail::DefaultScheduleSet_Impl>()->setOtherEquipmentSchedule(schedule);
}

void DefaultScheduleSet::resetOtherEquipmentSchedule()
{
  getImpl<detail::DefaultScheduleSet_Impl>()->resetOtherEquipmentSchedule();
}

boost::optional<Schedule> DefaultScheduleSet::infiltrationSchedule() const
{
  return getImpl<detail::DefaultScheduleSet_Impl>()->infiltrationSchedule();
}

bool DefaultScheduleSet::setInfiltrationSchedule(Schedule& schedule)
{
  return getImpl<detail::DefaultScheduleSet_Impl>()->setInfiltrationSchedule(schedule);
}

void DefaultScheduleSet::resetInfiltrationSchedule()
{
  getImpl<detail::DefaultScheduleSet_Impl>()->resetInfiltrationSchedule();
}

boost::optional<Schedule> DefaultScheduleSet::getDefaultSchedule(const DefaultScheduleType& defaultScheduleType) const
{
  return getImpl<detail::DefaultScheduleSet_Impl>()->getDefaultSchedule(defaultScheduleType);
}

void DefaultScheduleSet::merge(const DefaultScheduleSet& other)
{
  getImpl<detail::DefaultScheduleSet_Impl>()->merge(other);
}

/// @cond
DefaultScheduleSet::DefaultScheduleSet(std::shared_ptr<detail::DefaultScheduleSet_Impl> impl)
  : ResourceObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

