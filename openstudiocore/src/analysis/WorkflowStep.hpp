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

#ifndef ANALYSIS_WORKFLOWSTEP_HPP
#define ANALYSIS_WORKFLOWSTEP_HPP

#include "AnalysisAPI.hpp"
#include "AnalysisObject.hpp"

namespace openstudio {

class FileReferenceType;

namespace runmanager {
  class JobType;
  class WorkItem;
}

namespace analysis {

class InputVariable;
class OptimizationProblem;
class Problem;

namespace detail {
  class InputVariable_Impl;
  class Problem_Impl;
  class WorkflowStep_Impl;
};

/** WorkflowStep is an AnalysisObject that describes an individual step in a Problem's
 *  overall Workflow. Workflow is not a standard AnalysisObject in that its individual
 *  identifying data (name, UUIDs, description) is not retained, rather, UIs should
 *  defer to its composite data, which is either an InputVariable or a
 *  runmanager::WorkItem. */
class ANALYSIS_API WorkflowStep : public AnalysisObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  WorkflowStep(const InputVariable& inputVariable);

  WorkflowStep(const runmanager::WorkItem& workItem);

  /** Constructor provided for deserialization; not for general use. */
  WorkflowStep(const boost::optional<InputVariable>& inputVariable,
               const boost::optional<runmanager::WorkItem>& workItem);

  virtual ~WorkflowStep() {}

  //@}
  /** @name Getters and Queries */
  //@{

  bool isInputVariable() const;

  bool isWorkItem() const;

  /** Returns this step's InputVariable. Modifications will automatically apply to and
   *  be registered by this WorkflowStep. Throws if not isInputVariable. */
  InputVariable inputVariable() const;

  /** Returns a dis-attached copy of this step's WorkItem. To change this step's data,
   *  modify the returned WorkItem and then pass the edited copy into set. Throws if not
   *  isWorkItem(). */
  runmanager::WorkItem workItem() const;

  /** Shortcut method for retrieving this WorkItem's runmanager::JobType. Throws if not
   *  isWorkItem(). */
  runmanager::JobType workItemType() const;

  /** Returns the type of the primary input file for this WorkflowStep. */
  boost::optional<FileReferenceType> inputFileType() const;

  /** Returns the type of the primary output file for this WorkflowStep. */
  boost::optional<FileReferenceType> outputFileType() const;

  /** Returns true if the proposedInputFileType and proposedOutputFileType would be acceptable
   *  for the current use of this WorflowStep. Returns false otherwise. */
  bool fileTypesAreCompatible(const boost::optional<FileReferenceType>& proposedInputFileType,
                              const boost::optional<FileReferenceType>& proposedOutputFileType) const;

  //@}
  /** @name Setters */
  //@{

  /** Sets this step to use inputVariable if such a change is compatible with this
   *  WorkflowStep's current use (in its parent Problem). */
  bool set(const InputVariable& inputVariable);

  /** Sets this step to use workItem if such a change is compatible with this
   *  WorkflowStep's current use (in its parent Problem). */
  bool set(const runmanager::WorkItem& workItem);

  //@}
 protected:
  /// @cond
  typedef detail::WorkflowStep_Impl ImplType;

  friend class AnalysisObject;
  friend class detail::AnalysisObject_Impl;
  friend class detail::WorkflowStep_Impl;

  explicit WorkflowStep(std::shared_ptr<detail::WorkflowStep_Impl> impl);

  friend class OptimizationProblem; // calls setParent
  friend class Problem; // calls setParent
  friend class detail::Problem_Impl; // calls onChange
  friend class detail::InputVariable_Impl; // constructs its own WorkflowStep
  /// @endcond

  /** Specialization of public method onChange so WorkflowStep can make the call on InputVariable
   *  (its child) if appropriate. */
  virtual void onChange();
 private:
  REGISTER_LOGGER("openstudio.analysis.WorkflowStep");
};

/** \relates WorkflowStep */
typedef boost::optional<WorkflowStep> OptionalWorkflowStep;

/** \relates WorkflowStep */
typedef std::vector<WorkflowStep> WorkflowStepVector;

} // analysis
} // openstudio

#endif // ANALYSIS_WORKFLOWSTEP_HPP
