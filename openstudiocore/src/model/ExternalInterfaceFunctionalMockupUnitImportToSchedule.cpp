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

#include "ExternalInterfaceFunctionalMockupUnitImportToSchedule.hpp"
#include "ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl.hpp"

#include "ExternalInterfaceFunctionalMockupUnitImport.hpp"
#include "ExternalInterfaceFunctionalMockupUnitImport_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeLimits_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ExternalInterface_FunctionalMockupUnitImport_To_Schedule_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl::ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl(const IdfObject& idfObject,
                                                                                                                         Model_Impl* model,
                                                                                                                         bool keepHandle)
    : Schedule_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ExternalInterfaceFunctionalMockupUnitImportToSchedule::iddObjectType());
  }

  ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl::ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                                         Model_Impl* model,
                                                                                                                         bool keepHandle)
    : Schedule_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ExternalInterfaceFunctionalMockupUnitImportToSchedule::iddObjectType());
  }

  ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl::ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl(const ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl& other,
                                                                                                                         Model_Impl* model,
                                                                                                                         bool keepHandle)
    : Schedule_Impl(other,model,keepHandle)
  {}

  // return the parent object in the hierarchy
  boost::optional<ParentObject> ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl::parent() const {
    return boost::optional<ParentObject>();
  }

  // return any children objects in the hierarchy
  std::vector<ModelObject> ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl::children() const {
    std::vector<ModelObject> result;
    return result;
  }

  const std::vector<std::string>& ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    return result;
  }

  IddObjectType ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl::iddObjectType() const {
    return ExternalInterfaceFunctionalMockupUnitImportToSchedule::iddObjectType();
  }

  boost::optional<ScheduleTypeLimits> ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl::scheduleTypeLimits() const {
    return getObject<ModelObject>().getModelObjectTarget<ScheduleTypeLimits>(OS_ExternalInterface_FunctionalMockupUnitImport_To_ScheduleFields::ScheduleTypeLimitsName);
  }

  void ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl::ensureNoLeapDays() {
    LOG(Warn, "Ensure no leap days is not yet implemented for schedule compact");
  }

  std::vector<double> ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl::values() const {
    DoubleVector result;
    result.push_back(initialValue());
    return result;
  }

  ExternalInterfaceFunctionalMockupUnitImport ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl::fMUFile() const {
    return this->getTarget(OS_ExternalInterface_FunctionalMockupUnitImport_To_ScheduleFields::FMUFileName)->cast<ExternalInterfaceFunctionalMockupUnitImport>();
  }

  std::string ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl::fMUInstanceName() const {
    boost::optional<std::string> value = getString(OS_ExternalInterface_FunctionalMockupUnitImport_To_ScheduleFields::FMUInstanceName,true);
    if (value) {
      return value.get();
    }
    return "";
  }

  std::string ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl::fMUVariableName() const {
    boost::optional<std::string> value = getString(OS_ExternalInterface_FunctionalMockupUnitImport_To_ScheduleFields::FMUVariableName,true);
    if (value) {
      return value.get();
    }
    return "";
  }

  double ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl::initialValue() const {
    boost::optional<double> value = getDouble(OS_ExternalInterface_FunctionalMockupUnitImport_To_ScheduleFields::InitialValue,true);
    if (value) {
      return value.get();
    }
    return -9999;
  }

  bool ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl::setScheduleTypeLimits(const ScheduleTypeLimits& scheduleTypeLimits) {
    if (scheduleTypeLimits.model() != model()) {
      return false;
    }
    if (!candidateIsCompatibleWithCurrentUse(scheduleTypeLimits)) {
      return false;
    }
    return setPointer(OS_ExternalInterface_FunctionalMockupUnitImport_To_ScheduleFields::ScheduleTypeLimitsName, scheduleTypeLimits.handle());
  }

  bool ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl::resetScheduleTypeLimits() {
    if (okToResetScheduleTypeLimits()) {
      return setString(OS_ExternalInterface_FunctionalMockupUnitImport_To_ScheduleFields::ScheduleTypeLimitsName, "");
    }
    return false;
  }

  bool ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl::setFMUFile(const ExternalInterfaceFunctionalMockupUnitImport& fMUFile) {
    bool result = setPointer(OS_ExternalInterface_FunctionalMockupUnitImport_To_ScheduleFields::FMUFileName, fMUFile.handle());
    //TODO in FT use .fMUFile();
    return result;
  }

  bool ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl::setFMUInstanceName(const std::string& fMUInstanceName) {
    bool result = setString(OS_ExternalInterface_FunctionalMockupUnitImport_To_ScheduleFields::FMUInstanceName, fMUInstanceName);
    OS_ASSERT(result);
    return result;
  }

  bool ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl::setFMUVariableName(const std::string& fMUVariableName) {
    bool result = setString(OS_ExternalInterface_FunctionalMockupUnitImport_To_ScheduleFields::FMUVariableName, fMUVariableName);
    OS_ASSERT(result);
    return result;
  }

  bool ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl::setInitialValue(double initialValue) {
    bool result = setDouble(OS_ExternalInterface_FunctionalMockupUnitImport_To_ScheduleFields::InitialValue, initialValue);
    OS_ASSERT(result);
    return result;
  }

} // detail

ExternalInterfaceFunctionalMockupUnitImportToSchedule::ExternalInterfaceFunctionalMockupUnitImportToSchedule(const Model& model,
                                                                                                             const ExternalInterfaceFunctionalMockupUnitImport& fMUFile,
                                                                                                             const std::string& fMUInstanceName,
                                                                                                             const std::string& fMUVariableName,
                                                                                                             double initialValue)
  : Schedule(ExternalInterfaceFunctionalMockupUnitImportToSchedule::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl>());

  bool ok = setFMUFile(fMUFile);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s FMUFileName to "
      << fMUFile.fMUFileName() << ".");
  }
  setFMUInstanceName(fMUInstanceName);
  setFMUVariableName(fMUVariableName);
  setInitialValue(initialValue);
  //TODO move the Forward Translator
  //if (schedule.scheduleTypeLimits()) {
  //  ok = setScheduleTypeLimits(schedule.scheduleTypeLimits().get());
  //}
}

IddObjectType ExternalInterfaceFunctionalMockupUnitImportToSchedule::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ExternalInterface_FunctionalMockupUnitImport_To_Schedule);
}

ExternalInterfaceFunctionalMockupUnitImport ExternalInterfaceFunctionalMockupUnitImportToSchedule::fMUFile() const {
  return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl>()->fMUFile();
}

std::string ExternalInterfaceFunctionalMockupUnitImportToSchedule::fMUInstanceName() const {
  return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl>()->fMUInstanceName();
}

std::string ExternalInterfaceFunctionalMockupUnitImportToSchedule::fMUVariableName() const {
  return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl>()->fMUVariableName();
}

double ExternalInterfaceFunctionalMockupUnitImportToSchedule::initialValue() const {
  return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl>()->initialValue();
}

bool ExternalInterfaceFunctionalMockupUnitImportToSchedule::setFMUFile(const ExternalInterfaceFunctionalMockupUnitImport& fMUFile) {
  return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl>()->setFMUFile(fMUFile);
}

bool ExternalInterfaceFunctionalMockupUnitImportToSchedule::setFMUInstanceName(const std::string& fMUInstanceName) {
  return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl>()->setFMUInstanceName(fMUInstanceName);
}

bool ExternalInterfaceFunctionalMockupUnitImportToSchedule::setFMUVariableName(const std::string& fMUVariableName) {
  return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl>()->setFMUVariableName(fMUVariableName);
}

bool ExternalInterfaceFunctionalMockupUnitImportToSchedule::setInitialValue(double initialValue) {
  return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl>()->setInitialValue(initialValue);
}

/// @cond
ExternalInterfaceFunctionalMockupUnitImportToSchedule::ExternalInterfaceFunctionalMockupUnitImportToSchedule(std::shared_ptr<detail::ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl> impl)
  : Schedule(impl)
{}
/// @endcond

} // model
} // openstudio
