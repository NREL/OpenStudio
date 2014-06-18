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

#ifndef ANALYSIS_WORKFLOWSTEP_IMPL_HPP
#define ANALYSIS_WORKFLOWSTEP_IMPL_HPP

#include "AnalysisAPI.hpp"
#include "AnalysisObject_Impl.hpp"

#include "InputVariable.hpp"

#include "../runmanager/lib/WorkItem.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  /** WorkflowStep_Impl is an AnalysisObject_Impl that is the implementation class for
   *  WorkflowStep. */
  class ANALYSIS_API WorkflowStep_Impl : public AnalysisObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    WorkflowStep_Impl(const InputVariable& inputVariable);

    WorkflowStep_Impl(const runmanager::WorkItem& workItem);

    /** Constructor provided for deserialization; not for general use. */
    WorkflowStep_Impl(const boost::optional<InputVariable>& inputVariable,
                      const boost::optional<runmanager::WorkItem>& workItem);

    WorkflowStep_Impl(const WorkflowStep_Impl& other);

    virtual ~WorkflowStep_Impl() {}

    virtual AnalysisObject clone() const;

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
    /** @name Protected in or Absent from Public Class */
    //@{

    virtual QVariant toVariant() const;

    static WorkflowStep factoryFromVariant(const QVariant& variant, const VersionString& version);

    /// Relocate path data from originalBase to newBase.
    virtual void updateInputPathData(const openstudio::path& originalBase,
                                     const openstudio::path& newBase);

    //@}
   private:
    REGISTER_LOGGER("openstudio.analysis.WorkflowStep")

    boost::optional<InputVariable> m_inputVariable;
    boost::optional<runmanager::WorkItem> m_workItem;
  };

} // detail

} // analysis
} // openstudio

#endif // ANALYSIS_WORKFLOWSTEP_IMPL_HPP
