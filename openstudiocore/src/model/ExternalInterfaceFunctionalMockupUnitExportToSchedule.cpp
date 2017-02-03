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

#include "ExternalInterfaceFunctionalMockupUnitExportToSchedule.hpp"
#include "ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeLimits_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ExternalInterface_FunctionalMockupUnitExport_To_Schedule_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl::ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl(const IdfObject& idfObject,
                                                                                                                         Model_Impl* model,
                                                                                                                         bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ExternalInterfaceFunctionalMockupUnitExportToSchedule::iddObjectType());
  }

  ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl::ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                                         Model_Impl* model,
                                                                                                                         bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ExternalInterfaceFunctionalMockupUnitExportToSchedule::iddObjectType());
  }

  ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl::ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl(const ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl& other,
                                                                                                                         Model_Impl* model,
                                                                                                                         bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl::iddObjectType() const {
    return ExternalInterfaceFunctionalMockupUnitExportToSchedule::iddObjectType();
  }

  boost::optional<ScheduleTypeLimits> ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl::scheduleTypeLimits() const {
    return getObject<ModelObject>().getModelObjectTarget<ScheduleTypeLimits>(OS_ExternalInterface_FunctionalMockupUnitExport_To_ScheduleFields::ScheduleTypeLimitsNames);
  }

  std::string ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl::fMUVariableName() const {
    boost::optional<std::string> value = getString(OS_ExternalInterface_FunctionalMockupUnitExport_To_ScheduleFields::FMUVariableName,true);
    if (value) {
      return value.get();
    }
    return "";
  }

  double ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl::initialValue() const {
    boost::optional<double> value = getDouble(OS_ExternalInterface_FunctionalMockupUnitExport_To_ScheduleFields::InitialValue,true);
    if (value) {
      return value.get();
    }
    return -9999;
  }

  bool ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl::setScheduleTypeLimits(const ScheduleTypeLimits& scheduleTypeLimits) {
    bool result = setPointer(OS_ExternalInterface_FunctionalMockupUnitExport_To_ScheduleFields::ScheduleTypeLimitsNames, scheduleTypeLimits.handle());
    return result;
  }

  void ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl::resetScheduleTypeLimits() {
    bool result = setString(OS_ExternalInterface_FunctionalMockupUnitExport_To_ScheduleFields::ScheduleTypeLimitsNames, "");
    OS_ASSERT(result);
  }

  void ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl::setFMUVariableName(const std::string& fMUVariableName) {
    bool result = setString(OS_ExternalInterface_FunctionalMockupUnitExport_To_ScheduleFields::FMUVariableName, fMUVariableName);
    OS_ASSERT(result);
  }

  void ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl::setInitialValue(double initialValue) {
    bool result = setDouble(OS_ExternalInterface_FunctionalMockupUnitExport_To_ScheduleFields::InitialValue, initialValue);
    OS_ASSERT(result);
  }

} // detail

ExternalInterfaceFunctionalMockupUnitExportToSchedule::ExternalInterfaceFunctionalMockupUnitExportToSchedule(const Model& model,
                                                                                                             const Schedule& schedule,
                                                                                                             const std::string& fMUVariableName,
                                                                                                             double initialValue)
  : ModelObject(ExternalInterfaceFunctionalMockupUnitExportToSchedule::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl>());

  bool ok = getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl>()->setName(schedule.nameString());
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s Name to " << schedule.nameString() << ".");
  }
  setFMUVariableName(fMUVariableName);
  setInitialValue(initialValue);
}

IddObjectType ExternalInterfaceFunctionalMockupUnitExportToSchedule::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ExternalInterface_FunctionalMockupUnitExport_To_Schedule);
}

boost::optional<ScheduleTypeLimits> ExternalInterfaceFunctionalMockupUnitExportToSchedule::scheduleTypeLimits() const {
  return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl>()->scheduleTypeLimits();
}

std::string ExternalInterfaceFunctionalMockupUnitExportToSchedule::fMUVariableName() const {
  return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl>()->fMUVariableName();
}

double ExternalInterfaceFunctionalMockupUnitExportToSchedule::initialValue() const {
  return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl>()->initialValue();
}

bool ExternalInterfaceFunctionalMockupUnitExportToSchedule::setScheduleTypeLimits(const ScheduleTypeLimits& scheduleTypeLimits) {
  return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl>()->setScheduleTypeLimits(scheduleTypeLimits);
}

void ExternalInterfaceFunctionalMockupUnitExportToSchedule::resetScheduleTypeLimits() {
  getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl>()->resetScheduleTypeLimits();
}

void ExternalInterfaceFunctionalMockupUnitExportToSchedule::setFMUVariableName(const std::string& fMUVariableName) {
  getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl>()->setFMUVariableName(fMUVariableName);
}

void ExternalInterfaceFunctionalMockupUnitExportToSchedule::setInitialValue(double initialValue) {
  getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl>()->setInitialValue(initialValue);
}

/// @cond
ExternalInterfaceFunctionalMockupUnitExportToSchedule::ExternalInterfaceFunctionalMockupUnitExportToSchedule(std::shared_ptr<detail::ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

