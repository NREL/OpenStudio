/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

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
