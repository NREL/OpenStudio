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

#include "InputVariable.hpp"
#include "InputVariable_Impl.hpp"

#include "MeasureGroup.hpp"
#include "MeasureGroup_Impl.hpp"
#include "RubyContinuousVariable.hpp"
#include "RubyContinuousVariable_Impl.hpp"
#include "WorkflowStep.hpp"
#include "WorkflowStep_Impl.hpp"

#include "../runmanager/lib/WorkItem.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/FileReference.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  InputVariable_Impl::InputVariable_Impl(const std::string& name)
    : Variable_Impl(name)
  {}

  InputVariable_Impl::InputVariable_Impl(const UUID& uuid,
                                         const UUID& versionUUID,
                                         const std::string& name,
                                         const std::string& displayName,
                                         const std::string& description,
                                         const boost::optional<UncertaintyDescription>& udesc)
    : Variable_Impl(uuid,versionUUID,name,displayName,description),
      m_uncertaintyDescription(udesc)
  {}

  InputVariable_Impl::InputVariable_Impl(const InputVariable_Impl &other)
    : Variable_Impl(other)
  {
    if (OptionalUncertaintyDescription udesc = other.uncertaintyDescription()) {
      m_uncertaintyDescription = udesc->clone();
    }
  }

  boost::optional<UncertaintyDescription> InputVariable_Impl::uncertaintyDescription() const {
    return m_uncertaintyDescription;
  }

  WorkflowStep InputVariable_Impl::asWorkflowStep() const {
    if (parent()) {
      return parent()->cast<WorkflowStep>();
    }
    InputVariable copyOfThis = getPublicObject<InputVariable>();
    std::shared_ptr<detail::WorkflowStep_Impl> workflowStepImpl(new detail::WorkflowStep_Impl(copyOfThis));
    WorkflowStep result(workflowStepImpl);
    copyOfThis.setParent(result);
    return result;
  }

  bool InputVariable_Impl::setUncertaintyDescription(const UncertaintyDescription& udesc) {
    if (isValid(udesc)) {
      m_uncertaintyDescription = udesc;
      // ETH@20130128 - I am not sure about this designation.
      onChange(AnalysisObject_Impl::InvalidatesDataPoints);
      return true;
    }
    return false;
  }

  void InputVariable_Impl::resetUncertaintyDescription() {
    m_uncertaintyDescription.reset();
    onChange(AnalysisObject_Impl::Benign);
  }

  QVariant InputVariable_Impl::toVariant() const {
    QVariantMap inputVariableData = AnalysisObject_Impl::toVariant().toMap();

    if (OptionalUncertaintyDescription udesc = uncertaintyDescription()) {
      inputVariableData["uncertainty_description"] = analysis::detail::toVariant(udesc.get());
    }

    return QVariant(inputVariableData);
  }

  InputVariable InputVariable_Impl::factoryFromVariant(const QVariant &variant, const VersionString &version)
  {
    QVariantMap map = variant.toMap();

    if (!(map.contains("workflow_step_type") || map.contains("variable_type"))) {
      LOG_AND_THROW("Unable to find InputVariable in expected location.");
    }

    std::string variableType;
    if (map.contains("workflow_step_type")) {
      variableType = map["workflow_step_type"].toString().toStdString();
    }
    else {
      OS_ASSERT(map.contains("variable_type"));
      variableType = map["variable_type"].toString().toStdString();
    }

    if (variableType == "MeasureGroup") {
      return MeasureGroup_Impl::fromVariant(variant,version);
    }
    if (variableType == "RubyContinuousVariable") {
      return RubyContinuousVariable_Impl::fromVariant(variant,version);
    }

    LOG_AND_THROW("Unexpected workflow_step_type or variable_type " << variableType << ".");
    return OptionalInputVariable().get();
  }

  InputVariable InputVariable_Impl::factoryFromVariant(const QVariant &variant,
                                                       const Measure &measure,
                                                       const VersionString &version)
  {
    QVariantMap map = variant.toMap();

    if (!(map.contains("workflow_step_type") || map.contains("variable_type"))) {
      LOG_AND_THROW("Unable to find InputVariable in expected location.");
    }

    std::string variableType;
    if (map.contains("workflow_step_type")) {
      variableType = map["workflow_step_type"].toString().toStdString();
    }
    else {
      OS_ASSERT(map.contains("variable_type"));
      variableType = map["variable_type"].toString().toStdString();
    }

    // This type of constructor is not appropriate for MeasureGroups.
    if (variableType == "RubyContinuousVariable") {
      return RubyContinuousVariable_Impl::fromVariant(variant,measure,version);
    }

    LOG_AND_THROW("Unexpected workflow_step_type or variable_type " << variableType << ".");
    return OptionalInputVariable().get();
  }

} // detail

boost::optional<FileReferenceType> InputVariable::inputFileType() const {
  return getImpl<detail::InputVariable_Impl>()->inputFileType();
}

boost::optional<FileReferenceType> InputVariable::outputFileType() const {
  return getImpl<detail::InputVariable_Impl>()->outputFileType();
}

boost::optional<UncertaintyDescription> InputVariable::uncertaintyDescription() const {
  return getImpl<detail::InputVariable_Impl>()->uncertaintyDescription();
}

bool InputVariable::isValid(const QVariant& value) const {
  return getImpl<detail::InputVariable_Impl>()->isValid(value);
}

bool InputVariable::isValid(const UncertaintyDescription& udesc) const {
  return getImpl<detail::InputVariable_Impl>()->isValid(udesc);
}

WorkflowStep InputVariable::asWorkflowStep() const {
  return getImpl<detail::InputVariable_Impl>()->asWorkflowStep();
}

bool InputVariable::setUncertaintyDescription(const UncertaintyDescription& udesc) {
  return getImpl<detail::InputVariable_Impl>()->setUncertaintyDescription(udesc);
}

void InputVariable::resetUncertaintyDescription() {
  return getImpl<detail::InputVariable_Impl>()->resetUncertaintyDescription();
}

runmanager::WorkItem InputVariable::createWorkItem(const QVariant& value,
                                                   const openstudio::path& rubyIncludeDirectory) const
{
  return getImpl<detail::InputVariable_Impl>()->createWorkItem(value,rubyIncludeDirectory);
}

/// @cond
InputVariable::InputVariable(std::shared_ptr<detail::InputVariable_Impl> impl)
  : Variable(impl)
{}
/// @endcond

} // analysis
} // openstudio
