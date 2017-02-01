/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "ExternalInterfaceSchedule.hpp"
#include "ExternalInterfaceSchedule_Impl.hpp"

#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeLimits_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ExternalInterface_Schedule_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ExternalInterfaceSchedule_Impl::ExternalInterfaceSchedule_Impl(const IdfObject& idfObject,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ExternalInterfaceSchedule::iddObjectType());
  }

  ExternalInterfaceSchedule_Impl::ExternalInterfaceSchedule_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ExternalInterfaceSchedule::iddObjectType());
  }

  ExternalInterfaceSchedule_Impl::ExternalInterfaceSchedule_Impl(const ExternalInterfaceSchedule_Impl& other,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ExternalInterfaceSchedule_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ExternalInterfaceSchedule_Impl::iddObjectType() const {
    return ExternalInterfaceSchedule::iddObjectType();
  }

  boost::optional<ScheduleTypeLimits> ExternalInterfaceSchedule_Impl::scheduleTypeLimits() const {
    return getObject<ModelObject>().getModelObjectTarget<ScheduleTypeLimits>(OS_ExternalInterface_ScheduleFields::ScheduleTypeLimitsName);
  }

  double ExternalInterfaceSchedule_Impl::initialValue() const {
    boost::optional<double> value = getDouble(OS_ExternalInterface_ScheduleFields::InitialValue,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ExternalInterfaceSchedule_Impl::setScheduleTypeLimits(const ScheduleTypeLimits& scheduleTypeLimits) {
    bool result = setPointer(OS_ExternalInterface_ScheduleFields::ScheduleTypeLimitsName, scheduleTypeLimits.handle());
    return result;
  }

  void ExternalInterfaceSchedule_Impl::resetScheduleTypeLimits() {
    bool result = setString(OS_ExternalInterface_ScheduleFields::ScheduleTypeLimitsName, "");
    OS_ASSERT(result);
  }

  void ExternalInterfaceSchedule_Impl::setInitialValue(double initialValue) {
    bool result = setDouble(OS_ExternalInterface_ScheduleFields::InitialValue, initialValue);
    OS_ASSERT(result);
  }

} // detail

ExternalInterfaceSchedule::ExternalInterfaceSchedule(const Model& model)
  : ModelObject(ExternalInterfaceSchedule::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ExternalInterfaceSchedule_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  // setInitialValue();
}

IddObjectType ExternalInterfaceSchedule::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ExternalInterface_Schedule);
}

boost::optional<ScheduleTypeLimits> ExternalInterfaceSchedule::scheduleTypeLimits() const {
  return getImpl<detail::ExternalInterfaceSchedule_Impl>()->scheduleTypeLimits();
}

double ExternalInterfaceSchedule::initialValue() const {
  return getImpl<detail::ExternalInterfaceSchedule_Impl>()->initialValue();
}

bool ExternalInterfaceSchedule::setScheduleTypeLimits(const ScheduleTypeLimits& scheduleTypeLimits) {
  return getImpl<detail::ExternalInterfaceSchedule_Impl>()->setScheduleTypeLimits(scheduleTypeLimits);
}

void ExternalInterfaceSchedule::resetScheduleTypeLimits() {
  getImpl<detail::ExternalInterfaceSchedule_Impl>()->resetScheduleTypeLimits();
}

void ExternalInterfaceSchedule::setInitialValue(double initialValue) {
  getImpl<detail::ExternalInterfaceSchedule_Impl>()->setInitialValue(initialValue);
}

/// @cond
ExternalInterfaceSchedule::ExternalInterfaceSchedule(std::shared_ptr<detail::ExternalInterfaceSchedule_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

