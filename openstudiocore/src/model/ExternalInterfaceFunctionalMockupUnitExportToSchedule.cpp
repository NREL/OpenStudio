/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "ExternalInterfaceFunctionalMockupUnitExportToSchedule.hpp"
#include "ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeLimits_Impl.hpp"
#include "Model.hpp"

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
    : Schedule_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ExternalInterfaceFunctionalMockupUnitExportToSchedule::iddObjectType());
  }

  ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl::ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                                         Model_Impl* model,
                                                                                                                         bool keepHandle)
    : Schedule_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ExternalInterfaceFunctionalMockupUnitExportToSchedule::iddObjectType());
  }

  ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl::ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl(const ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl& other,
                                                                                                                         Model_Impl* model,
                                                                                                                         bool keepHandle)
    : Schedule_Impl(other,model,keepHandle)
  {}

  // return the parent object in the hierarchy
  boost::optional<ParentObject> ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl::parent() const {
    return boost::optional<ParentObject>();
  }

  // return any children objects in the hierarchy
  std::vector<ModelObject> ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl::children() const {
    std::vector<ModelObject> result;
    return result;
  }

  const std::vector<std::string>& ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    return result;
  }

  IddObjectType ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl::iddObjectType() const {
    return ExternalInterfaceFunctionalMockupUnitExportToSchedule::iddObjectType();
  }

  boost::optional<ScheduleTypeLimits> ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl::scheduleTypeLimits() const {
    return getObject<ModelObject>().getModelObjectTarget<ScheduleTypeLimits>(OS_ExternalInterface_FunctionalMockupUnitExport_To_ScheduleFields::ScheduleTypeLimitsName);
  }

  void ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl::ensureNoLeapDays() {
    LOG(Warn, "Ensure no leap days is not yet implemented for schedule compact");
  }

  std::vector<double> ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl::values() const {
    DoubleVector result;
    result.push_back(initialValue());
    return result;
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
    if (scheduleTypeLimits.model() != model()) {
      return false;
    }
    if (!candidateIsCompatibleWithCurrentUse(scheduleTypeLimits)) {
      return false;
    }
    return setPointer(OS_ExternalInterface_FunctionalMockupUnitExport_To_ScheduleFields::ScheduleTypeLimitsName, scheduleTypeLimits.handle());
  }

  bool ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl::resetScheduleTypeLimits() {
    if (okToResetScheduleTypeLimits()) {
      return setString(OS_ExternalInterface_FunctionalMockupUnitExport_To_ScheduleFields::ScheduleTypeLimitsName, "");
    }
    return false;
  }

  bool ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl::setFMUVariableName(const std::string& fMUVariableName) {
    bool result = setString(OS_ExternalInterface_FunctionalMockupUnitExport_To_ScheduleFields::FMUVariableName, fMUVariableName);
    OS_ASSERT(result);
    return result;
  }

  bool ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl::setInitialValue(double initialValue) {
    bool result = setDouble(OS_ExternalInterface_FunctionalMockupUnitExport_To_ScheduleFields::InitialValue, initialValue);
    OS_ASSERT(result);
    return result;
  }

} // detail

ExternalInterfaceFunctionalMockupUnitExportToSchedule::ExternalInterfaceFunctionalMockupUnitExportToSchedule(const Model& model,
                                                                                                             const std::string& fMUVariableName,
                                                                                                             double initialValue)
  : Schedule(ExternalInterfaceFunctionalMockupUnitExportToSchedule::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl>());

  setFMUVariableName(fMUVariableName);
  setInitialValue(initialValue);
}

IddObjectType ExternalInterfaceFunctionalMockupUnitExportToSchedule::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ExternalInterface_FunctionalMockupUnitExport_To_Schedule);
}

std::string ExternalInterfaceFunctionalMockupUnitExportToSchedule::fMUVariableName() const {
  return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl>()->fMUVariableName();
}

double ExternalInterfaceFunctionalMockupUnitExportToSchedule::initialValue() const {
  return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl>()->initialValue();
}

bool ExternalInterfaceFunctionalMockupUnitExportToSchedule::setFMUVariableName(const std::string& fMUVariableName) {
  return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl>()->setFMUVariableName(fMUVariableName);
}

bool ExternalInterfaceFunctionalMockupUnitExportToSchedule::setInitialValue(double initialValue) {
  return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl>()->setInitialValue(initialValue);
}

/// @cond
ExternalInterfaceFunctionalMockupUnitExportToSchedule::ExternalInterfaceFunctionalMockupUnitExportToSchedule(std::shared_ptr<detail::ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl> impl)
  : Schedule(impl)
{}
/// @endcond

} // model
} // openstudio
