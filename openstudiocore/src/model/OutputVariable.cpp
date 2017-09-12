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

#include "OutputVariable.hpp"
#include "OutputVariable_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "EnergyManagementSystemSensor.hpp"
#include "EnergyManagementSystemSensor_Impl.hpp"

#include "Model.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Output_Variable_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/sql/SqlFileEnums.hpp"
#include "../utilities/sql/SqlFile.hpp"

#include "../utilities/core/Assert.hpp"

using openstudio::Handle;
using openstudio::OptionalHandle;
using openstudio::HandleVector;
using openstudio::IdfObject;
using openstudio::WorkspaceObject;
using openstudio::OptionalWorkspaceObject;
using openstudio::WorkspaceObjectVector;
using openstudio::Workspace;

using std::string;

namespace openstudio {
namespace model {

namespace detail {

  OutputVariable_Impl::OutputVariable_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ModelObject_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == OutputVariable::iddObjectType());
  }

  OutputVariable_Impl::OutputVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == OutputVariable::iddObjectType());
  }

  OutputVariable_Impl::OutputVariable_Impl(const OutputVariable_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& OutputVariable_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType OutputVariable_Impl::iddObjectType() const {
    return OutputVariable::iddObjectType();
  }

  std::vector<openstudio::IdfObject> OutputVariable_Impl::remove() {
    //Note: Cant do /object-list implementation for EMS Sensor since Auto Naming of Objects causes issues.
    //      Instead, doing an /alpha getString implementation so we need to manually remove any referring Sensors
    const Model m = this->model();

    std::vector<EnergyManagementSystemSensor> objects = m.getConcreteModelObjects<EnergyManagementSystemSensor>();
    for (auto & sensor : objects) {
      if (sensor.outputVariable()) {
        if (sensor.outputVariable().get().variableName() == this->variableName()) {
          sensor.remove();
        }
      }
    }

    return ModelObject_Impl::remove();
  }

  std::vector<ScheduleTypeKey> OutputVariable_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Output_VariableFields::ScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("OutputVariable","Active Hours"));
    }
    return result;
  }

  std::string OutputVariable_Impl::keyValue() const
  {
    OptionalString result = getString(OS_Output_VariableFields::KeyValue, true);
    OS_ASSERT(result);
    return result.get();
  }

  bool OutputVariable_Impl::isKeyValueDefaulted() const
  {
    return isEmpty(OS_Output_VariableFields::KeyValue);
  }

  std::string OutputVariable_Impl::variableName() const
  {
    OptionalString result = getString(OS_Output_VariableFields::VariableName, true);
    OS_ASSERT(result);
    return result.get();
  }

  std::string OutputVariable_Impl::reportingFrequency() const
  {
    OptionalString result = getString(OS_Output_VariableFields::ReportingFrequency, true);
    OS_ASSERT(result);
    return result.get();
  }

  bool OutputVariable_Impl::isReportingFrequencyDefaulted() const
  {
    return isEmpty(OS_Output_VariableFields::ReportingFrequency);
  }

  bool OutputVariable_Impl::setKeyValue(const std::string& keyValue)
  {
    return setString(OS_Output_VariableFields::KeyValue, keyValue);
  }

  void OutputVariable_Impl::resetKeyValue()
  {
    bool test = setString(OS_Output_VariableFields::KeyValue, "");
    OS_ASSERT(test);
  }

  bool OutputVariable_Impl::setVariableName(const std::string& variableName)
  {
    return setString(OS_Output_VariableFields::VariableName, variableName);
  }

  bool OutputVariable_Impl::setReportingFrequency(const std::string& reportingFrequency)
  {
    return setString(OS_Output_VariableFields::ReportingFrequency, reportingFrequency);
  }

  void OutputVariable_Impl::resetReportingFrequency()
  {
    bool test = setString(OS_Output_VariableFields::ReportingFrequency, "");
    OS_ASSERT(test);
  }

  boost::optional<Schedule> OutputVariable_Impl::schedule() const
  {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Output_VariableFields::ScheduleName);
  }

  bool OutputVariable_Impl::setSchedule(Schedule& schedule) {
    bool result = ModelObject_Impl::setSchedule(OS_Output_VariableFields::ScheduleName,
                                                "OutputVariable",
                                                "Active Hours",
                                                schedule);
    return result;
  }

  void OutputVariable_Impl::resetSchedule()
  {
    this->setString(OS_Output_VariableFields::ScheduleName, "");
  }

  boost::optional<ModelObject> OutputVariable_Impl::scheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = schedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  bool OutputVariable_Impl::setScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
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


  std::vector<std::string> OutputVariable_Impl::reportingFrequencyValues() const
  {
    return OutputVariable::reportingFrequencyValues();
  }
} // detail

OutputVariable::OutputVariable(const std::string& variableName, const Model& model)
  : ModelObject(OutputVariable::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::OutputVariable_Impl>());
  bool test = getImpl<detail::OutputVariable_Impl>()->setVariableName(variableName);
  OS_ASSERT(test);
}

OutputVariable::OutputVariable(std::shared_ptr<detail::OutputVariable_Impl> impl)
  : ModelObject(std::move(impl))
{}

IddObjectType OutputVariable::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Output_Variable);
  return result;
}

std::vector<std::string> OutputVariable::reportingFrequencyValues()
{
  std::vector<std::string> retvals;

  std::map<int, std::string> names = openstudio::ReportingFrequency::getNames();

  for (std::map<int, std::string>::const_iterator itr = names.begin();
      itr != names.end();
      ++itr)
  {
    retvals.push_back(itr->second);
  }

  return retvals;
}

std::string OutputVariable::keyValue() const
{
  return getImpl<detail::OutputVariable_Impl>()->keyValue();
}

bool OutputVariable::isKeyValueDefaulted() const
{
  return getImpl<detail::OutputVariable_Impl>()->isKeyValueDefaulted();
}

std::string OutputVariable::variableName() const
{
  return getImpl<detail::OutputVariable_Impl>()->variableName();
}

std::string OutputVariable::reportingFrequency() const
{
  return getImpl<detail::OutputVariable_Impl>()->reportingFrequency();
}



bool OutputVariable::isReportingFrequencyDefaulted() const
{
  return getImpl<detail::OutputVariable_Impl>()->isReportingFrequencyDefaulted();
}

bool OutputVariable::setKeyValue(const std::string& keyValue)
{
  return getImpl<detail::OutputVariable_Impl>()->setKeyValue(keyValue);
}

void OutputVariable::resetKeyValue()
{
  getImpl<detail::OutputVariable_Impl>()->resetKeyValue();
}

bool OutputVariable::setVariableName(const std::string& variableName)
{
  return getImpl<detail::OutputVariable_Impl>()->setVariableName(variableName);
}

bool OutputVariable::setReportingFrequency(const std::string& reportingFrequency)
{
  return getImpl<detail::OutputVariable_Impl>()->setReportingFrequency(reportingFrequency);
}

void OutputVariable::resetReportingFrequency()
{
  getImpl<detail::OutputVariable_Impl>()->resetReportingFrequency();
}

boost::optional<Schedule> OutputVariable::schedule() const
{
  return getImpl<detail::OutputVariable_Impl>()->schedule();
}

bool OutputVariable::setSchedule(Schedule& schedule)
{
  return getImpl<detail::OutputVariable_Impl>()->setSchedule(schedule);
}

void OutputVariable::resetSchedule()
{
  getImpl<detail::OutputVariable_Impl>()->resetSchedule();
}

std::vector<IdfObject> OutputVariable::remove() {
  return getImpl<detail::OutputVariable_Impl>()->remove();
}

} // model
} // openstudio
