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

#include "DesignSpecificationOutdoorAir.hpp"
#include "DesignSpecificationOutdoorAir_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_DesignSpecification_OutdoorAir_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  DesignSpecificationOutdoorAir_Impl::DesignSpecificationOutdoorAir_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ResourceObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == DesignSpecificationOutdoorAir::iddObjectType());
  }

  DesignSpecificationOutdoorAir_Impl::DesignSpecificationOutdoorAir_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                         Model_Impl* model,
                                                                         bool keepHandle)
                                                                           : ResourceObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == DesignSpecificationOutdoorAir::iddObjectType());
  }

  DesignSpecificationOutdoorAir_Impl::DesignSpecificationOutdoorAir_Impl(const DesignSpecificationOutdoorAir_Impl& other,
                                                                         Model_Impl* model,
                                                                         bool keepHandle)
                                                                           : ResourceObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& DesignSpecificationOutdoorAir_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType DesignSpecificationOutdoorAir_Impl::iddObjectType() const {
    return DesignSpecificationOutdoorAir::iddObjectType();
  }

  std::vector<ScheduleTypeKey> DesignSpecificationOutdoorAir_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowRateFractionScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("DesignSpecificationOutdoorAir","Outdoor Air Flow Rate"));
    }
    return result;
  }

  std::string DesignSpecificationOutdoorAir_Impl::outdoorAirMethod() const {
    boost::optional<std::string> value = getString(OS_DesignSpecification_OutdoorAirFields::OutdoorAirMethod,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool DesignSpecificationOutdoorAir_Impl::isOutdoorAirMethodDefaulted() const {
    return isEmpty(OS_DesignSpecification_OutdoorAirFields::OutdoorAirMethod);
  }

  double DesignSpecificationOutdoorAir_Impl::outdoorAirFlowperPerson() const {
    boost::optional<double> value = getDouble(OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowperPerson,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity DesignSpecificationOutdoorAir_Impl::getOutdoorAirFlowperPerson(bool returnIP) const {
    OptionalDouble value = outdoorAirFlowperPerson();
    OSOptionalQuantity result = getQuantityFromDouble(OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowperPerson, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool DesignSpecificationOutdoorAir_Impl::isOutdoorAirFlowperPersonDefaulted() const {
    return isEmpty(OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowperPerson);
  }

  double DesignSpecificationOutdoorAir_Impl::outdoorAirFlowperFloorArea() const {
    boost::optional<double> value = getDouble(OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowperFloorArea,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity DesignSpecificationOutdoorAir_Impl::getOutdoorAirFlowperFloorArea(bool returnIP) const {
    OptionalDouble value = outdoorAirFlowperFloorArea();
    OSOptionalQuantity result = getQuantityFromDouble(OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowperFloorArea, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool DesignSpecificationOutdoorAir_Impl::isOutdoorAirFlowperFloorAreaDefaulted() const {
    return isEmpty(OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowperFloorArea);
  }

  double DesignSpecificationOutdoorAir_Impl::outdoorAirFlowRate() const {
    boost::optional<double> value = getDouble(OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity DesignSpecificationOutdoorAir_Impl::getOutdoorAirFlowRate(bool returnIP) const {
    OptionalDouble value = outdoorAirFlowRate();
    OSOptionalQuantity result = getQuantityFromDouble(OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowRate, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool DesignSpecificationOutdoorAir_Impl::isOutdoorAirFlowRateDefaulted() const {
    return isEmpty(OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowRate);
  }

  double DesignSpecificationOutdoorAir_Impl::outdoorAirFlowAirChangesperHour() const {
    boost::optional<double> value = getDouble(OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowAirChangesperHour,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity DesignSpecificationOutdoorAir_Impl::getOutdoorAirFlowAirChangesperHour(bool returnIP) const {
    OptionalDouble value = outdoorAirFlowAirChangesperHour();
    OSOptionalQuantity result = getQuantityFromDouble(OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowAirChangesperHour, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool DesignSpecificationOutdoorAir_Impl::isOutdoorAirFlowAirChangesperHourDefaulted() const {
    return isEmpty(OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowAirChangesperHour);
  }

  boost::optional<Schedule> DesignSpecificationOutdoorAir_Impl::outdoorAirFlowRateFractionSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowRateFractionScheduleName);
  }

  bool DesignSpecificationOutdoorAir_Impl::setOutdoorAirMethod(std::string outdoorAirMethod) {
    bool result = setString(OS_DesignSpecification_OutdoorAirFields::OutdoorAirMethod, outdoorAirMethod);
    return result;
  }

  void DesignSpecificationOutdoorAir_Impl::resetOutdoorAirMethod() {
    bool result = setString(OS_DesignSpecification_OutdoorAirFields::OutdoorAirMethod, "");
    OS_ASSERT(result);
  }

  bool DesignSpecificationOutdoorAir_Impl::setOutdoorAirFlowperPerson(double outdoorAirFlowperPerson) {
    bool result = setDouble(OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowperPerson, outdoorAirFlowperPerson);
    return result;
  }

  bool DesignSpecificationOutdoorAir_Impl::setOutdoorAirFlowperPerson(const Quantity& outdoorAirFlowperPerson) {
    OptionalDouble value = getDoubleFromQuantity(OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowperPerson,outdoorAirFlowperPerson);
    if (!value) {
      return false;
    }
    return setOutdoorAirFlowperPerson(value.get());
  }

  void DesignSpecificationOutdoorAir_Impl::resetOutdoorAirFlowperPerson() {
    bool result = setString(OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowperPerson, "");
    OS_ASSERT(result);
  }

  bool DesignSpecificationOutdoorAir_Impl::setOutdoorAirFlowperFloorArea(double outdoorAirFlowperFloorArea) {
    bool result = setDouble(OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowperFloorArea, outdoorAirFlowperFloorArea);
    return result;
  }

  bool DesignSpecificationOutdoorAir_Impl::setOutdoorAirFlowperFloorArea(const Quantity& outdoorAirFlowperFloorArea) {
    OptionalDouble value = getDoubleFromQuantity(OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowperFloorArea,outdoorAirFlowperFloorArea);
    if (!value) {
      return false;
    }
    return setOutdoorAirFlowperFloorArea(value.get());
  }

  void DesignSpecificationOutdoorAir_Impl::resetOutdoorAirFlowperFloorArea() {
    bool result = setString(OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowperFloorArea, "");
    OS_ASSERT(result);
  }

  bool DesignSpecificationOutdoorAir_Impl::setOutdoorAirFlowRate(double outdoorAirFlowRate) {
    bool result = setDouble(OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowRate, outdoorAirFlowRate);
    return result;
  }

  bool DesignSpecificationOutdoorAir_Impl::setOutdoorAirFlowRate(const Quantity& outdoorAirFlowRate) {
    OptionalDouble value = getDoubleFromQuantity(OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowRate,outdoorAirFlowRate);
    if (!value) {
      return false;
    }
    return setOutdoorAirFlowRate(value.get());
  }

  void DesignSpecificationOutdoorAir_Impl::resetOutdoorAirFlowRate() {
    bool result = setString(OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowRate, "");
    OS_ASSERT(result);
  }

  bool DesignSpecificationOutdoorAir_Impl::setOutdoorAirFlowAirChangesperHour(double outdoorAirFlowAirChangesperHour) {
    bool result = setDouble(OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowAirChangesperHour, outdoorAirFlowAirChangesperHour);
    return result;
  }

  bool DesignSpecificationOutdoorAir_Impl::setOutdoorAirFlowAirChangesperHour(const Quantity& outdoorAirFlowAirChangesperHour) {
    OptionalDouble value = getDoubleFromQuantity(OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowAirChangesperHour,outdoorAirFlowAirChangesperHour);
    if (!value) {
      return false;
    }
    return setOutdoorAirFlowAirChangesperHour(value.get());
  }

  void DesignSpecificationOutdoorAir_Impl::resetOutdoorAirFlowAirChangesperHour() {
    bool result = setString(OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowAirChangesperHour, "");
    OS_ASSERT(result);
  }

  bool DesignSpecificationOutdoorAir_Impl::setOutdoorAirFlowRateFractionSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowRateFractionScheduleName,
                              "DesignSpecificationOutdoorAir",
                              "Outdoor Air Flow Rate",
                              schedule);
    return result;
  }

  void DesignSpecificationOutdoorAir_Impl::resetOutdoorAirFlowRateFractionSchedule() {
    bool result = setString(OS_DesignSpecification_OutdoorAirFields::OutdoorAirFlowRateFractionScheduleName, "");
    OS_ASSERT(result);
  }

  std::vector<std::string> DesignSpecificationOutdoorAir_Impl::outdoorAirMethodValues() const {
    return DesignSpecificationOutdoorAir::outdoorAirMethodValues();
  }

  openstudio::Quantity DesignSpecificationOutdoorAir_Impl::outdoorAirFlowperPerson_SI() const {
    return getOutdoorAirFlowperPerson(false);
  }

  openstudio::Quantity DesignSpecificationOutdoorAir_Impl::outdoorAirFlowperPerson_IP() const {
    return getOutdoorAirFlowperPerson(true);
  }

  openstudio::Quantity DesignSpecificationOutdoorAir_Impl::outdoorAirFlowperFloorArea_SI() const {
    return getOutdoorAirFlowperFloorArea(false);
  }

  openstudio::Quantity DesignSpecificationOutdoorAir_Impl::outdoorAirFlowperFloorArea_IP() const {
    return getOutdoorAirFlowperFloorArea(true);
  }

  openstudio::Quantity DesignSpecificationOutdoorAir_Impl::outdoorAirFlowRate_SI() const {
    return getOutdoorAirFlowRate(false);
  }

  openstudio::Quantity DesignSpecificationOutdoorAir_Impl::outdoorAirFlowRate_IP() const {
    return getOutdoorAirFlowRate(true);
  }

  openstudio::Quantity DesignSpecificationOutdoorAir_Impl::outdoorAirFlowAirChangesperHour_SI() const {
    return getOutdoorAirFlowAirChangesperHour(false);
  }

  openstudio::Quantity DesignSpecificationOutdoorAir_Impl::outdoorAirFlowAirChangesperHour_IP() const {
    return getOutdoorAirFlowAirChangesperHour(true);
  }

  boost::optional<ModelObject> DesignSpecificationOutdoorAir_Impl::outdoorAirFlowRateFractionScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = outdoorAirFlowRateFractionSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  bool DesignSpecificationOutdoorAir_Impl::setOutdoorAirFlowRateFractionScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setOutdoorAirFlowRateFractionSchedule(schedule);
      }
      else {
        return false;
      }
    }
    else {
      resetOutdoorAirFlowRateFractionSchedule();
    }
    return true;
  }

} // detail

DesignSpecificationOutdoorAir::DesignSpecificationOutdoorAir(const Model& model)
  : ResourceObject(DesignSpecificationOutdoorAir::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::DesignSpecificationOutdoorAir_Impl>());
}

IddObjectType DesignSpecificationOutdoorAir::iddObjectType() {
  IddObjectType result(IddObjectType::OS_DesignSpecification_OutdoorAir);
  return result;
}

std::vector<std::string> DesignSpecificationOutdoorAir::outdoorAirMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_DesignSpecification_OutdoorAirFields::OutdoorAirMethod);
}

std::vector<std::string> DesignSpecificationOutdoorAir::validOutdoorAirMethodValues() {
  return DesignSpecificationOutdoorAir::outdoorAirMethodValues();
}

std::string DesignSpecificationOutdoorAir::outdoorAirMethod() const {
  return getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->outdoorAirMethod();
}

bool DesignSpecificationOutdoorAir::isOutdoorAirMethodDefaulted() const {
  return getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->isOutdoorAirMethodDefaulted();
}

double DesignSpecificationOutdoorAir::outdoorAirFlowperPerson() const {
  return getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->outdoorAirFlowperPerson();
}

Quantity DesignSpecificationOutdoorAir::getOutdoorAirFlowperPerson(bool returnIP) const {
  return getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->getOutdoorAirFlowperPerson(returnIP);
}

bool DesignSpecificationOutdoorAir::isOutdoorAirFlowperPersonDefaulted() const {
  return getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->isOutdoorAirFlowperPersonDefaulted();
}

double DesignSpecificationOutdoorAir::outdoorAirFlowperFloorArea() const {
  return getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->outdoorAirFlowperFloorArea();
}

Quantity DesignSpecificationOutdoorAir::getOutdoorAirFlowperFloorArea(bool returnIP) const {
  return getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->getOutdoorAirFlowperFloorArea(returnIP);
}

bool DesignSpecificationOutdoorAir::isOutdoorAirFlowperFloorAreaDefaulted() const {
  return getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->isOutdoorAirFlowperFloorAreaDefaulted();
}

double DesignSpecificationOutdoorAir::outdoorAirFlowRate() const {
  return getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->outdoorAirFlowRate();
}

Quantity DesignSpecificationOutdoorAir::getOutdoorAirFlowRate(bool returnIP) const {
  return getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->getOutdoorAirFlowRate(returnIP);
}

bool DesignSpecificationOutdoorAir::isOutdoorAirFlowRateDefaulted() const {
  return getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->isOutdoorAirFlowRateDefaulted();
}

double DesignSpecificationOutdoorAir::outdoorAirFlowAirChangesperHour() const {
  return getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->outdoorAirFlowAirChangesperHour();
}

Quantity DesignSpecificationOutdoorAir::getOutdoorAirFlowAirChangesperHour(bool returnIP) const {
  return getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->getOutdoorAirFlowAirChangesperHour(returnIP);
}

bool DesignSpecificationOutdoorAir::isOutdoorAirFlowAirChangesperHourDefaulted() const {
  return getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->isOutdoorAirFlowAirChangesperHourDefaulted();
}

boost::optional<Schedule> DesignSpecificationOutdoorAir::outdoorAirFlowRateFractionSchedule() const {
  return getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->outdoorAirFlowRateFractionSchedule();
}

bool DesignSpecificationOutdoorAir::setOutdoorAirMethod(std::string outdoorAirMethod) {
  return getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->setOutdoorAirMethod(outdoorAirMethod);
}

void DesignSpecificationOutdoorAir::resetOutdoorAirMethod() {
  getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->resetOutdoorAirMethod();
}

bool DesignSpecificationOutdoorAir::setOutdoorAirFlowperPerson(double outdoorAirFlowperPerson) {
  return getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->setOutdoorAirFlowperPerson(outdoorAirFlowperPerson);
}

bool DesignSpecificationOutdoorAir::setOutdoorAirFlowperPerson(const Quantity& outdoorAirFlowperPerson) {
  return getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->setOutdoorAirFlowperPerson(outdoorAirFlowperPerson);
}

void DesignSpecificationOutdoorAir::resetOutdoorAirFlowperPerson() {
  getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->resetOutdoorAirFlowperPerson();
}

bool DesignSpecificationOutdoorAir::setOutdoorAirFlowperFloorArea(double outdoorAirFlowperFloorArea) {
  return getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->setOutdoorAirFlowperFloorArea(outdoorAirFlowperFloorArea);
}

bool DesignSpecificationOutdoorAir::setOutdoorAirFlowperFloorArea(const Quantity& outdoorAirFlowperFloorArea) {
  return getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->setOutdoorAirFlowperFloorArea(outdoorAirFlowperFloorArea);
}

void DesignSpecificationOutdoorAir::resetOutdoorAirFlowperFloorArea() {
  getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->resetOutdoorAirFlowperFloorArea();
}

bool DesignSpecificationOutdoorAir::setOutdoorAirFlowRate(double outdoorAirFlowRate) {
  return getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->setOutdoorAirFlowRate(outdoorAirFlowRate);
}

bool DesignSpecificationOutdoorAir::setOutdoorAirFlowRate(const Quantity& outdoorAirFlowRate) {
  return getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->setOutdoorAirFlowRate(outdoorAirFlowRate);
}

void DesignSpecificationOutdoorAir::resetOutdoorAirFlowRate() {
  getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->resetOutdoorAirFlowRate();
}

bool DesignSpecificationOutdoorAir::setOutdoorAirFlowAirChangesperHour(double outdoorAirFlowAirChangesperHour) {
  return getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->setOutdoorAirFlowAirChangesperHour(outdoorAirFlowAirChangesperHour);
}

bool DesignSpecificationOutdoorAir::setOutdoorAirFlowAirChangesperHour(const Quantity& outdoorAirFlowAirChangesperHour) {
  return getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->setOutdoorAirFlowAirChangesperHour(outdoorAirFlowAirChangesperHour);
}

void DesignSpecificationOutdoorAir::resetOutdoorAirFlowAirChangesperHour() {
  getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->resetOutdoorAirFlowAirChangesperHour();
}

bool DesignSpecificationOutdoorAir::setOutdoorAirFlowRateFractionSchedule(Schedule& schedule) {
  return getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->setOutdoorAirFlowRateFractionSchedule(schedule);
}

void DesignSpecificationOutdoorAir::resetOutdoorAirFlowRateFractionSchedule() {
  getImpl<detail::DesignSpecificationOutdoorAir_Impl>()->resetOutdoorAirFlowRateFractionSchedule();
}

/// @cond
DesignSpecificationOutdoorAir::DesignSpecificationOutdoorAir(std::shared_ptr<detail::DesignSpecificationOutdoorAir_Impl> impl)
  : ResourceObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

