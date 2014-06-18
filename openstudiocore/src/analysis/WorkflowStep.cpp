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

#include "WorkflowStep.hpp"
#include "WorkflowStep_Impl.hpp"

#include "InputVariable_Impl.hpp"
#include "MeasureGroup.hpp"
#include "MeasureGroup_Impl.hpp"
#include "Problem.hpp"
#include "Problem_Impl.hpp"

#include "../runmanager/lib/JSON.hpp"
#include "../runmanager/lib/RubyJobUtils.hpp"

#include "../utilities/core/Assert.hpp"

#include "../utilities/core/FileReference.hpp"
#include "../utilities/core/PathHelpers.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  WorkflowStep_Impl::WorkflowStep_Impl(const InputVariable& inputVariable)
    : AnalysisObject_Impl(inputVariable.name() + " Workflow Step"),
      m_inputVariable(inputVariable)
  {
    OS_ASSERT(!inputVariable.parent());
    m_inputVariable->onChange();
    connectChild(m_inputVariable.get(),false);
  }

  WorkflowStep_Impl::WorkflowStep_Impl(const runmanager::WorkItem& workItem)
    : AnalysisObject_Impl(workItem.type.valueName() + " Workflow Step"),
      m_workItem(workItem)
  {}

  WorkflowStep_Impl::WorkflowStep_Impl(const boost::optional<InputVariable>& inputVariable,
                                       const boost::optional<runmanager::WorkItem>& workItem)
    : AnalysisObject_Impl(""),
      m_inputVariable(inputVariable),
      m_workItem(workItem)
  {
    OS_ASSERT(inputVariable || workItem);
    OS_ASSERT(!(inputVariable && workItem));
    if (isInputVariable()) {
      setName(inputVariable->name() + " Workflow Step");
      // deserialization constructor, so do not call m_inputVariable->onChange()
      connectChild(m_inputVariable.get(),false);
    }
    else {
      setName(workItem->type.valueName() + " Workflow Step");
    }
  }

  WorkflowStep_Impl::WorkflowStep_Impl(const WorkflowStep_Impl& other)
    : AnalysisObject_Impl(other),
      m_workItem(other.m_workItem)
  {
    if (other.isInputVariable()) {
      OS_ASSERT(!m_workItem);
      m_inputVariable = other.inputVariable().clone().cast<InputVariable>();
      connectChild(m_inputVariable.get(),false);
    }
    else {
      OS_ASSERT(m_workItem);
    }
  }

  AnalysisObject WorkflowStep_Impl::clone() const {
    std::shared_ptr<WorkflowStep_Impl> impl(new WorkflowStep_Impl(*this));
    WorkflowStep result(impl);
    if (result.isInputVariable()) {
      result.inputVariable().setParent(result);
    }
    return result;
  }

  bool WorkflowStep_Impl::isInputVariable() const {
    return m_inputVariable;
  }

  bool WorkflowStep_Impl::isWorkItem() const {
    return m_workItem;
  }

  InputVariable WorkflowStep_Impl::inputVariable() const {
    return m_inputVariable.get();
  }

  runmanager::WorkItem WorkflowStep_Impl::workItem() const {
    return m_workItem.get();
  }

  runmanager::JobType WorkflowStep_Impl::workItemType() const {
    return m_workItem->type;
  }

  boost::optional<FileReferenceType> WorkflowStep_Impl::inputFileType() const {
    if (isInputVariable()) {
      return m_inputVariable->inputFileType();
    }
    else {
      return m_workItem->inputFileType();
    }
  }

  boost::optional<FileReferenceType> WorkflowStep_Impl::outputFileType() const {
    if (isInputVariable()) {
      return m_inputVariable->outputFileType();
    }
    else {
      return m_workItem->outputFileType();
    }
  }

  bool WorkflowStep_Impl::fileTypesAreCompatible(
      const boost::optional<FileReferenceType>& proposedInputFileType,
      const boost::optional<FileReferenceType>& proposedOutputFileType) const
  {
    if ((!proposedInputFileType && !inputFileType()) ||
        (proposedInputFileType && inputFileType() && (proposedInputFileType.get() == inputFileType().get())))
    {
      // input types are the same
      if ((!proposedOutputFileType && !outputFileType()) ||
          (proposedOutputFileType && outputFileType() && (proposedOutputFileType.get() == outputFileType().get())))
      {
        // output types are the same
        return true; // trivially ok
      }
    }
    // check with parent - how responsible!
    if (parent()) {
      WorkflowStep copyOfThis = getPublicObject<WorkflowStep>();
      return parent()->cast<Problem>().fileTypesAreCompatible(copyOfThis,
                                                              proposedInputFileType,
                                                              proposedOutputFileType);
    }
    return true;
  }

  bool WorkflowStep_Impl::set(const InputVariable& inputVariable) {
    if (!fileTypesAreCompatible(inputVariable.inputFileType(),
                                inputVariable.outputFileType()))
    {
      return false;
    }
    if (inputVariable.parent()) {
      return false;
    }

    if (isInputVariable()) {
      disconnectChild(*m_inputVariable);
    }
    m_inputVariable = inputVariable;
    m_inputVariable->onChange();
    connectChild(m_inputVariable.get(),true);
    onChange(AnalysisObject_Impl::InvalidatesDataPoints);
    return true;
  }

  bool WorkflowStep_Impl::set(const runmanager::WorkItem& workItem) {
    if (!fileTypesAreCompatible(workItem.inputFileType(),workItem.outputFileType())) {
      return false;
    }

    bool displacesVariable(false);
    if (isInputVariable()) {
      disconnectChild(*m_inputVariable);
      displacesVariable = true;
      m_inputVariable.reset();
    }
    m_workItem = workItem;
    if (displacesVariable) {
      onChange(AnalysisObject_Impl::InvalidatesDataPoints);
    }
    else {
      onChange(AnalysisObject_Impl::InvalidatesResults);
    }
    return true;
  }

  QVariant WorkflowStep_Impl::toVariant() const {
    QVariant result;

    if (isInputVariable()) {
      result = inputVariable().toVariant();
    }
    else {
      QVariantMap workItemData = runmanager::detail::JSON::toVariant(workItem()).toMap();
      workItemData["workflow_step_type"] = "WorkItem";
      result = QVariant(workItemData);
    }

    return result;
  }

  WorkflowStep WorkflowStep_Impl::factoryFromVariant(const QVariant& variant, const VersionString& version)
  {
    QVariantMap map = variant.toMap();

    if (!map.contains("workflow_step_type")) {
      LOG_AND_THROW("Unable to find WorkflowStep in expected location.");
    }

    std::string workflowStepType = map["workflow_step_type"].toString().toStdString();
    if (workflowStepType == "WorkItem") {
      return WorkflowStep(OptionalInputVariable(),
                          runmanager::detail::JSON::toWorkItem(variant,version));
    }
    if (workflowStepType == "MeasureGroup") {
      return WorkflowStep(MeasureGroup_Impl::fromVariant(variant,version),
                          boost::optional<runmanager::WorkItem>());
    }

    // workflowStepType == "Measure" is handled by Problem_Impl
    LOG_AND_THROW("Unexpected workflow_step_type " << workflowStepType << ".");
    return OptionalWorkflowStep().get();
  }

  void WorkflowStep_Impl::updateInputPathData(const openstudio::path& originalBase,
                                              const openstudio::path& newBase)
  {
    if (isInputVariable()) {
      m_inputVariable->getImpl<detail::InputVariable_Impl>()->updateInputPathData(originalBase,newBase);
    }
    else {
      // WorkItem -- Try to update files.
      if (workItemType() == runmanager::JobType::UserScript || workItemType() == runmanager::JobType::Ruby) {
        LOG(Debug, "Updating paths for ruby / userscript job. Keyname: " + m_workItem->jobkeyname);
        runmanager::RubyJobBuilder rjb(*m_workItem,originalBase,newBase);
        m_workItem = rjb.toWorkItem();
      }
      else {
        for (runmanager::FileInfo& info : m_workItem->files.files()) {
          // update fullPath
          openstudio::path temp = relocatePath(info.fullPath,originalBase,newBase);
          if (!temp.empty()) {
            info.fullPath = temp;
          }

          // update requiredFiles
          // ETH@20130815 -- Skip for now. I think these are usually not absolute paths ...
        }
      }
    }
  }

} // detail

WorkflowStep::WorkflowStep(const InputVariable& inputVariable)
  : AnalysisObject(inputVariable.asWorkflowStep().getImpl<detail::WorkflowStep_Impl>())
{}

WorkflowStep::WorkflowStep(const runmanager::WorkItem& workItem)
  : AnalysisObject(std::shared_ptr<detail::WorkflowStep_Impl>(
                     new detail::WorkflowStep_Impl(workItem)))
{}

WorkflowStep::WorkflowStep(const boost::optional<InputVariable>& inputVariable,
                           const boost::optional<runmanager::WorkItem>& workItem)
  : AnalysisObject(std::shared_ptr<detail::WorkflowStep_Impl>(
                     new detail::WorkflowStep_Impl(inputVariable,workItem)))
{
  if (isInputVariable()) {
    WorkflowStep copyOfThis(getImpl<detail::WorkflowStep_Impl>());
    inputVariable->setParent(copyOfThis);
  }
}

bool WorkflowStep::isInputVariable() const {
  return getImpl<detail::WorkflowStep_Impl>()->isInputVariable();
}

bool WorkflowStep::isWorkItem() const {
  return getImpl<detail::WorkflowStep_Impl>()->isWorkItem();
}

InputVariable WorkflowStep::inputVariable() const {
  return getImpl<detail::WorkflowStep_Impl>()->inputVariable();
}

runmanager::WorkItem WorkflowStep::workItem() const {
  return getImpl<detail::WorkflowStep_Impl>()->workItem();
}

runmanager::JobType WorkflowStep::workItemType() const {
  return getImpl<detail::WorkflowStep_Impl>()->workItemType();
}

boost::optional<FileReferenceType> WorkflowStep::inputFileType() const {
  return getImpl<detail::WorkflowStep_Impl>()->inputFileType();
}

boost::optional<FileReferenceType> WorkflowStep::outputFileType() const {
  return getImpl<detail::WorkflowStep_Impl>()->outputFileType();
}

bool WorkflowStep::fileTypesAreCompatible(
    const boost::optional<FileReferenceType>& proposedInputFileType,
    const boost::optional<FileReferenceType>& proposedOutputFileType) const
{
  return getImpl<detail::WorkflowStep_Impl>()->fileTypesAreCompatible(proposedInputFileType,proposedOutputFileType);
}

bool WorkflowStep::set(const InputVariable& inputVariable) {
  return getImpl<detail::WorkflowStep_Impl>()->set(inputVariable);
}

bool WorkflowStep::set(const runmanager::WorkItem& workItem) {
  return getImpl<detail::WorkflowStep_Impl>()->set(workItem);
}

/// @cond
WorkflowStep::WorkflowStep(std::shared_ptr<detail::WorkflowStep_Impl> impl)
  : AnalysisObject(impl)
{}
/// @endcond

void WorkflowStep::onChange() {
  if (isInputVariable()) {
    inputVariable().onChange();
  }
  else {
    getImpl<detail::WorkflowStep_Impl>()->onChange(detail::AnalysisObject_Impl::Benign);
  }
}

} // analysis
} // openstudio
