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

#include "SpaceInfiltrationEffectiveLeakageArea.hpp"
#include "SpaceInfiltrationEffectiveLeakageArea_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Space.hpp"
#include "Space_Impl.hpp"
#include "SpaceType.hpp"
#include "SpaceType_Impl.hpp"
#include "DefaultScheduleSet.hpp"
#include "DefaultScheduleSet_Impl.hpp"
#include "LifeCycleCost.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_SpaceInfiltration_EffectiveLeakageArea_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  SpaceInfiltrationEffectiveLeakageArea_Impl::SpaceInfiltrationEffectiveLeakageArea_Impl(
      const IdfObject& idfObject,
      Model_Impl* model,
      bool keepHandle)
    : SpaceLoad_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SpaceInfiltrationEffectiveLeakageArea::iddObjectType());
  }

  SpaceInfiltrationEffectiveLeakageArea_Impl::SpaceInfiltrationEffectiveLeakageArea_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other,
      Model_Impl* model,
      bool keepHandle)
    : SpaceLoad_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SpaceInfiltrationEffectiveLeakageArea::iddObjectType());
  }

  SpaceInfiltrationEffectiveLeakageArea_Impl::SpaceInfiltrationEffectiveLeakageArea_Impl(
      const SpaceInfiltrationEffectiveLeakageArea_Impl& other,
      Model_Impl* model,
      bool keepHandle)
    : SpaceLoad_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& SpaceInfiltrationEffectiveLeakageArea_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SpaceInfiltrationEffectiveLeakageArea_Impl::iddObjectType() const {
    return SpaceInfiltrationEffectiveLeakageArea::iddObjectType();
  }

  std::vector<ScheduleTypeKey> SpaceInfiltrationEffectiveLeakageArea_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_SpaceInfiltration_EffectiveLeakageAreaFields::ScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("SpaceInfiltrationEffectiveLeakageArea","Infiltration"));
    }
    return result;
  }

  bool SpaceInfiltrationEffectiveLeakageArea_Impl::hardSize()
  {
    boost::optional<Space> space = this->space();
    if (!space){
      return false;
    }
    return false;
  }

  bool SpaceInfiltrationEffectiveLeakageArea_Impl::hardApplySchedules()
  {
    bool result = false;
    boost::optional<Schedule> schedule = this->schedule();
    if (schedule){
      result = this->setSchedule(*schedule);
    }
    return result;
  }

  bool SpaceInfiltrationEffectiveLeakageArea_Impl::isAbsolute() const
  {
    return true;
  }

  double SpaceInfiltrationEffectiveLeakageArea_Impl::effectiveAirLeakageArea() const {
    boost::optional<double> value = getDouble(OS_SpaceInfiltration_EffectiveLeakageAreaFields::EffectiveAirLeakageArea,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity SpaceInfiltrationEffectiveLeakageArea_Impl::getEffectiveAirLeakageArea(bool returnIP) const {
    OptionalDouble value = effectiveAirLeakageArea();
    OSOptionalQuantity result = getQuantityFromDouble(OS_SpaceInfiltration_EffectiveLeakageAreaFields::EffectiveAirLeakageArea, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  double SpaceInfiltrationEffectiveLeakageArea_Impl::stackCoefficient() const {
    boost::optional<double> value = getDouble(OS_SpaceInfiltration_EffectiveLeakageAreaFields::StackCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity SpaceInfiltrationEffectiveLeakageArea_Impl::getStackCoefficient(bool returnIP) const {
    OptionalDouble value = stackCoefficient();
    OSOptionalQuantity result = getQuantityFromDouble(OS_SpaceInfiltration_EffectiveLeakageAreaFields::StackCoefficient, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  double SpaceInfiltrationEffectiveLeakageArea_Impl::windCoefficient() const {
    boost::optional<double> value = getDouble(OS_SpaceInfiltration_EffectiveLeakageAreaFields::WindCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity SpaceInfiltrationEffectiveLeakageArea_Impl::getWindCoefficient(bool returnIP) const {
    OptionalDouble value = windCoefficient();
    OSOptionalQuantity result = getQuantityFromDouble(OS_SpaceInfiltration_EffectiveLeakageAreaFields::WindCoefficient, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool SpaceInfiltrationEffectiveLeakageArea_Impl::setEffectiveAirLeakageArea(double effectiveAirLeakageArea) {
    bool result = setDouble(OS_SpaceInfiltration_EffectiveLeakageAreaFields::EffectiveAirLeakageArea, effectiveAirLeakageArea);
    return result;
  }

  bool SpaceInfiltrationEffectiveLeakageArea_Impl::setEffectiveAirLeakageArea(const Quantity& effectiveAirLeakageArea) {
    OptionalDouble value = getDoubleFromQuantity(OS_SpaceInfiltration_EffectiveLeakageAreaFields::EffectiveAirLeakageArea,effectiveAirLeakageArea);
    if (!value) {
      return false;
    }
    return setEffectiveAirLeakageArea(value.get());
  }

  bool SpaceInfiltrationEffectiveLeakageArea_Impl::setStackCoefficient(double stackCoefficient) {
    bool result = setDouble(OS_SpaceInfiltration_EffectiveLeakageAreaFields::StackCoefficient, stackCoefficient);
    return result;
  }

  bool SpaceInfiltrationEffectiveLeakageArea_Impl::setStackCoefficient(const Quantity& stackCoefficient) {
    OptionalDouble value = getDoubleFromQuantity(OS_SpaceInfiltration_EffectiveLeakageAreaFields::StackCoefficient,stackCoefficient);
    if (!value) {
      return false;
    }
    return setStackCoefficient(value.get());
  }

  bool SpaceInfiltrationEffectiveLeakageArea_Impl::setWindCoefficient(double windCoefficient) {
    bool result = setDouble(OS_SpaceInfiltration_EffectiveLeakageAreaFields::WindCoefficient, windCoefficient);
    return result;
  }

  bool SpaceInfiltrationEffectiveLeakageArea_Impl::setWindCoefficient(const Quantity& windCoefficient) {
    OptionalDouble value = getDoubleFromQuantity(OS_SpaceInfiltration_EffectiveLeakageAreaFields::WindCoefficient,windCoefficient);
    if (!value) {
      return false;
    }
    return setWindCoefficient(value.get());
  }

  int SpaceInfiltrationEffectiveLeakageArea_Impl::spaceIndex() const {
    return OS_SpaceInfiltration_EffectiveLeakageAreaFields::SpaceorSpaceTypeName;
  }

  boost::optional<Schedule> SpaceInfiltrationEffectiveLeakageArea_Impl::schedule() const
  {
    boost::optional<Schedule> result = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_SpaceInfiltration_EffectiveLeakageAreaFields::ScheduleName);
    if (!result){
      // search upwards
      OptionalSpace space = this->space();
      OptionalSpaceType spaceType = this->spaceType();
      if (space){
        result = space->getDefaultSchedule(DefaultScheduleType::InfiltrationSchedule);
      }else if (spaceType){
        result = spaceType->getDefaultSchedule(DefaultScheduleType::InfiltrationSchedule);
      }
    }
    return result;
  }

  bool SpaceInfiltrationEffectiveLeakageArea_Impl::isScheduleDefaulted() const
  {
    return isEmpty(OS_SpaceInfiltration_EffectiveLeakageAreaFields::ScheduleName);
  }

  bool SpaceInfiltrationEffectiveLeakageArea_Impl::setSchedule(Schedule& schedule)
  {
    bool result = ModelObject_Impl::setSchedule(OS_SpaceInfiltration_EffectiveLeakageAreaFields::ScheduleName,
                                                "SpaceInfiltrationEffectiveLeakageArea",
                                                "Infiltration",
                                                schedule);
    return result;
  }

  void SpaceInfiltrationEffectiveLeakageArea_Impl::resetSchedule()
  {
    bool result = setString(OS_SpaceInfiltration_EffectiveLeakageAreaFields::ScheduleName, "");
    OS_ASSERT(result);
  }

  openstudio::Quantity SpaceInfiltrationEffectiveLeakageArea_Impl::effectiveAirLeakageArea_SI() const {
    return getEffectiveAirLeakageArea(false);
  }

  openstudio::Quantity SpaceInfiltrationEffectiveLeakageArea_Impl::effectiveAirLeakageArea_IP() const {
    return getEffectiveAirLeakageArea(true);
  }

  openstudio::Quantity SpaceInfiltrationEffectiveLeakageArea_Impl::stackCoefficient_SI() const {
    return getStackCoefficient(false);
  }

  openstudio::Quantity SpaceInfiltrationEffectiveLeakageArea_Impl::stackCoefficient_IP() const {
    return getStackCoefficient(true);
  }

  openstudio::Quantity SpaceInfiltrationEffectiveLeakageArea_Impl::windCoefficient_SI() const {
    return getWindCoefficient(false);
  }

  openstudio::Quantity SpaceInfiltrationEffectiveLeakageArea_Impl::windCoefficient_IP() const {
    return getWindCoefficient(true);
  }

  boost::optional<ModelObject> SpaceInfiltrationEffectiveLeakageArea_Impl::scheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = schedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  bool SpaceInfiltrationEffectiveLeakageArea_Impl::setScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
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

SpaceInfiltrationEffectiveLeakageArea::SpaceInfiltrationEffectiveLeakageArea(const Model& model)
  : SpaceLoad(SpaceInfiltrationEffectiveLeakageArea::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SpaceInfiltrationEffectiveLeakageArea_Impl>());
  bool test = setEffectiveAirLeakageArea(0.1);
  OS_ASSERT(test);
  test = setStackCoefficient(0.1);
  OS_ASSERT(test);
  test = setWindCoefficient(0.1);
  OS_ASSERT(test);
}

IddObjectType SpaceInfiltrationEffectiveLeakageArea::iddObjectType() {
  IddObjectType result(IddObjectType::OS_SpaceInfiltration_EffectiveLeakageArea);
  return result;
}

boost::optional<Schedule> SpaceInfiltrationEffectiveLeakageArea::schedule() const {
  return getImpl<detail::SpaceInfiltrationEffectiveLeakageArea_Impl>()->schedule();
}

bool SpaceInfiltrationEffectiveLeakageArea::isScheduleDefaulted() const {
  return getImpl<detail::SpaceInfiltrationEffectiveLeakageArea_Impl>()->isScheduleDefaulted();
}

double SpaceInfiltrationEffectiveLeakageArea::effectiveAirLeakageArea() const {
  return getImpl<detail::SpaceInfiltrationEffectiveLeakageArea_Impl>()->effectiveAirLeakageArea();
}

Quantity SpaceInfiltrationEffectiveLeakageArea::getEffectiveAirLeakageArea(bool returnIP) const {
  return getImpl<detail::SpaceInfiltrationEffectiveLeakageArea_Impl>()->getEffectiveAirLeakageArea(returnIP);
}

double SpaceInfiltrationEffectiveLeakageArea::stackCoefficient() const {
  return getImpl<detail::SpaceInfiltrationEffectiveLeakageArea_Impl>()->stackCoefficient();
}

Quantity SpaceInfiltrationEffectiveLeakageArea::getStackCoefficient(bool returnIP) const {
  return getImpl<detail::SpaceInfiltrationEffectiveLeakageArea_Impl>()->getStackCoefficient(returnIP);
}

double SpaceInfiltrationEffectiveLeakageArea::windCoefficient() const {
  return getImpl<detail::SpaceInfiltrationEffectiveLeakageArea_Impl>()->windCoefficient();
}

Quantity SpaceInfiltrationEffectiveLeakageArea::getWindCoefficient(bool returnIP) const {
  return getImpl<detail::SpaceInfiltrationEffectiveLeakageArea_Impl>()->getWindCoefficient(returnIP);
}

bool SpaceInfiltrationEffectiveLeakageArea::setSchedule(Schedule& schedule) {
  return getImpl<detail::SpaceInfiltrationEffectiveLeakageArea_Impl>()->setSchedule(schedule);
}

void SpaceInfiltrationEffectiveLeakageArea::resetSchedule() {
  getImpl<detail::SpaceInfiltrationEffectiveLeakageArea_Impl>()->resetSchedule();
}

bool SpaceInfiltrationEffectiveLeakageArea::setEffectiveAirLeakageArea(double effectiveAirLeakageArea) {
  return getImpl<detail::SpaceInfiltrationEffectiveLeakageArea_Impl>()->setEffectiveAirLeakageArea(effectiveAirLeakageArea);
}

bool SpaceInfiltrationEffectiveLeakageArea::setEffectiveAirLeakageArea(const Quantity& effectiveAirLeakageArea) {
  return getImpl<detail::SpaceInfiltrationEffectiveLeakageArea_Impl>()->setEffectiveAirLeakageArea(effectiveAirLeakageArea);
}

bool SpaceInfiltrationEffectiveLeakageArea::setStackCoefficient(double stackCoefficient) {
  return getImpl<detail::SpaceInfiltrationEffectiveLeakageArea_Impl>()->setStackCoefficient(stackCoefficient);
}

bool SpaceInfiltrationEffectiveLeakageArea::setStackCoefficient(const Quantity& stackCoefficient) {
  return getImpl<detail::SpaceInfiltrationEffectiveLeakageArea_Impl>()->setStackCoefficient(stackCoefficient);
}

bool SpaceInfiltrationEffectiveLeakageArea::setWindCoefficient(double windCoefficient) {
  return getImpl<detail::SpaceInfiltrationEffectiveLeakageArea_Impl>()->setWindCoefficient(windCoefficient);
}

bool SpaceInfiltrationEffectiveLeakageArea::setWindCoefficient(const Quantity& windCoefficient) {
  return getImpl<detail::SpaceInfiltrationEffectiveLeakageArea_Impl>()->setWindCoefficient(windCoefficient);
}

/// @cond
SpaceInfiltrationEffectiveLeakageArea::SpaceInfiltrationEffectiveLeakageArea(
    std::shared_ptr<detail::SpaceInfiltrationEffectiveLeakageArea_Impl> impl)
  : SpaceLoad(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

