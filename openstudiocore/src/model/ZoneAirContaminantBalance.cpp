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

#include "ZoneAirContaminantBalance.hpp"
#include "ZoneAirContaminantBalance_Impl.hpp"
#include "SimulationControl.hpp"
#include "SimulationControl_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/OS_ZoneAirContaminantBalance_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ZoneAirContaminantBalance_Impl::ZoneAirContaminantBalance_Impl(const IdfObject& idfObject,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ZoneAirContaminantBalance::iddObjectType());
  }

  ZoneAirContaminantBalance_Impl::ZoneAirContaminantBalance_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ZoneAirContaminantBalance::iddObjectType());
  }

  ZoneAirContaminantBalance_Impl::ZoneAirContaminantBalance_Impl(const ZoneAirContaminantBalance_Impl& other,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
  }

  IddObjectType ZoneAirContaminantBalance_Impl::iddObjectType() const {
    return ZoneAirContaminantBalance::iddObjectType();
  }

  // return the parent object in the hierarchy
  boost::optional<ParentObject> ZoneAirContaminantBalance_Impl::parent() const
  {
    boost::optional<ParentObject> result(model().getUniqueModelObject<SimulationControl>());
    return result;
  }

  // set the parent, child may have to call methods on the parent
  bool ZoneAirContaminantBalance_Impl::setParent(ParentObject& newParent)
  {
    if (newParent.optionalCast<SimulationControl>() && (newParent.model() == model())) {
      return true;
    }
    return false;
  }

  // Get all output variable names that could be associated with this object.
  const std::vector<std::string>& ZoneAirContaminantBalance_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  std::vector<ScheduleTypeKey> ZoneAirContaminantBalance_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_ZoneAirContaminantBalanceFields::OutdoorCarbonDioxideScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneAirContaminantBalance","Outdoor Carbon Dioxide"));
    }
    return result;
  }

  bool ZoneAirContaminantBalance_Impl::carbonDioxideConcentration() const {
    boost::optional<std::string> value = getString(OS_ZoneAirContaminantBalanceFields::CarbonDioxideConcentration,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }

  bool ZoneAirContaminantBalance_Impl::isCarbonDioxideConcentrationDefaulted() const {
    return isEmpty(OS_ZoneAirContaminantBalanceFields::CarbonDioxideConcentration);
  }

  boost::optional<Schedule> ZoneAirContaminantBalance_Impl::outdoorCarbonDioxideSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneAirContaminantBalanceFields::OutdoorCarbonDioxideScheduleName);
  }

  void ZoneAirContaminantBalance_Impl::setCarbonDioxideConcentration(bool carbonDioxideConcentration) {
    setBooleanFieldValue(OS_ZoneAirContaminantBalanceFields::CarbonDioxideConcentration, carbonDioxideConcentration);
  }

  void ZoneAirContaminantBalance_Impl::resetCarbonDioxideConcentration() {
    bool result = setString(OS_ZoneAirContaminantBalanceFields::CarbonDioxideConcentration, "");
    OS_ASSERT(result);
  }

  bool ZoneAirContaminantBalance_Impl::setOutdoorCarbonDioxideSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ZoneAirContaminantBalanceFields::OutdoorCarbonDioxideScheduleName,
                              "ZoneAirContaminantBalance",
                              "Outdoor Carbon Dioxide",
                              schedule);
    return result;
  }

  void ZoneAirContaminantBalance_Impl::resetOutdoorCarbonDioxideSchedule() {
    bool result = setString(OS_ZoneAirContaminantBalanceFields::OutdoorCarbonDioxideScheduleName, "");
    OS_ASSERT(result);
  }

  boost::optional<ModelObject> ZoneAirContaminantBalance_Impl::outdoorCarbonDioxideScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = outdoorCarbonDioxideSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  bool ZoneAirContaminantBalance_Impl::setOutdoorCarbonDioxideScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setOutdoorCarbonDioxideSchedule(schedule);
      }
      else {
        return false;
      }
    }
    else {
      resetOutdoorCarbonDioxideSchedule();
    }
    return true;
  }

} // detail

IddObjectType ZoneAirContaminantBalance::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ZoneAirContaminantBalance);
}

bool ZoneAirContaminantBalance::carbonDioxideConcentration() const {
  return getImpl<detail::ZoneAirContaminantBalance_Impl>()->carbonDioxideConcentration();
}

bool ZoneAirContaminantBalance::isCarbonDioxideConcentrationDefaulted() const {
  return getImpl<detail::ZoneAirContaminantBalance_Impl>()->isCarbonDioxideConcentrationDefaulted();
}

boost::optional<Schedule> ZoneAirContaminantBalance::outdoorCarbonDioxideSchedule() const {
  return getImpl<detail::ZoneAirContaminantBalance_Impl>()->outdoorCarbonDioxideSchedule();
}

void ZoneAirContaminantBalance::setCarbonDioxideConcentration(bool carbonDioxideConcentration) {
  getImpl<detail::ZoneAirContaminantBalance_Impl>()->setCarbonDioxideConcentration(carbonDioxideConcentration);
}

void ZoneAirContaminantBalance::resetCarbonDioxideConcentration() {
  getImpl<detail::ZoneAirContaminantBalance_Impl>()->resetCarbonDioxideConcentration();
}

bool ZoneAirContaminantBalance::setOutdoorCarbonDioxideSchedule(Schedule& schedule) {
  return getImpl<detail::ZoneAirContaminantBalance_Impl>()->setOutdoorCarbonDioxideSchedule(schedule);
}

void ZoneAirContaminantBalance::resetOutdoorCarbonDioxideSchedule() {
  getImpl<detail::ZoneAirContaminantBalance_Impl>()->resetOutdoorCarbonDioxideSchedule();
}

/// @cond
ZoneAirContaminantBalance::ZoneAirContaminantBalance(std::shared_ptr<detail::ZoneAirContaminantBalance_Impl> impl)
  : ModelObject(std::move(impl))
{}

ZoneAirContaminantBalance::ZoneAirContaminantBalance(Model& model)
  : ModelObject(ZoneAirContaminantBalance::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ZoneAirContaminantBalance_Impl>());
}

/// @endcond

} // model
} // openstudio

