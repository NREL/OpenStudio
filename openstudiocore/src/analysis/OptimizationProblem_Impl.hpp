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

#ifndef ANALYSIS_OPTIMIZATIONPROBLEM_IMPL_HPP
#define ANALYSIS_OPTIMIZATIONPROBLEM_IMPL_HPP

#include "AnalysisAPI.hpp"
#include "Problem_Impl.hpp"

#include "Function.hpp"

namespace openstudio {
namespace analysis {

class OptimizationProblem;

namespace detail {

  /** OptimizationProblem_Impl is a Problem_Impl that is the implementation class for OptimizationProblem.*/
  class ANALYSIS_API OptimizationProblem_Impl : public Problem_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    OptimizationProblem_Impl(const std::string& name,
                             const std::vector<Function>& objectives,
                             const std::vector<WorkflowStep>& workflow);

    OptimizationProblem_Impl(const std::string& name,
                             const std::vector<Function>& objectives,
                             const std::vector<WorkflowStep>& workflow,
                             const std::vector<Function>& responses);

    /** Construct from objectives, variables, responses and simulationWorkflow. In this context,
     *  response functions are design metrics of interest that are not being optimized over.
     *  \deprecated */
    OptimizationProblem_Impl(const std::string& name,
                             const std::vector<Function>& objectives,
                             const std::vector<Variable>& variables,
                             const std::vector<Function>& responses,
                             const runmanager::Workflow& simulationWorkflow);

    /** \overload \deprecated */
    OptimizationProblem_Impl(const std::string& name,
                             const std::vector<Function>& objectives,
                             const std::vector<Variable>& variables,
                             const runmanager::Workflow& simulationWorkflow);

    /** Constructor provided for deserialization; not for general use. */
    OptimizationProblem_Impl(const UUID& uuid,
                             const UUID& versionUUID,
                             const std::string& name,
                             const std::string& displayName,
                             const std::string& description,
                             const std::vector<Function>& objectives,
                             const std::vector<WorkflowStep>& workflow,
                             const std::vector<Function>& responses);

    OptimizationProblem_Impl(const OptimizationProblem_Impl& other);

    virtual ~OptimizationProblem_Impl() {}

    virtual AnalysisObject clone() const override;

    //@}
    /** Virtual Methods */
    //@{

    virtual bool isValid(const DataPoint& dataPoint) const override;

    /** Returns a description of this problem for using in dakota.in files. */
    virtual std::string dakotaInFileDescription() const;

    virtual boost::optional<DataPoint> createDataPoint(
        const std::vector<QVariant>& variableValues) const override;

    virtual boost::optional<DataPoint> createDataPoint(
        const std::vector<DiscretePerturbation>& perturbations) const override;

    /** Attemps to open and parse dakotaParametersFile, extract variable values and other relevant
     *  information, and create a new DataPoint. Returns that DataPoint if possible; returns
     *  boost::none otherwise. */
    virtual boost::optional<DataPoint> createDataPoint(
        const openstudio::path& dakotaParametersFile) const;

    virtual void updateDataPoint(DataPoint& dataPoint,
                                 const runmanager::Job& completedJob) const override;

    /** Returns the DAKOTA results file string corresponding to dataPoint, if dataPoint completed
     *  successfully. Returns boost::none otherwise. */
    virtual boost::optional<std::string> getDakotaResultsFile(const DataPoint& dataPoint) const override;

    //@}
    /** @name Getters and Queries */
    //@{

    std::vector<Function> objectives() const;

    int numObjectives() const;

    //@}
    /** @name Setters */
    //@{

    void pushObjective(const Function& objective);

    bool insertObjective(int index, const Function& objective);

    bool eraseObjective(const Function& objective);

    bool swapObjectives(const Function& objective1, const Function& objective2);

    /** Sets objectives. */
    void setObjectiveFunctions(const std::vector<Function>& objectives);

    /** Clear all objective functions. */
    void clearObjectiveFunctions();

    //@}
    /** @name Protected in or Absent from Public Class */
    //@{

    virtual QVariant toVariant() const override;

    static OptimizationProblem fromVariant(const QVariant& variant, const VersionString& version);

    /// Relocate path data from originalBase to newBase.
    virtual void updateInputPathData(const openstudio::path& originalBase,
                                     const openstudio::path& newBase) override;

    //@}
   protected:
    std::vector<Function> m_objectives;
   private:
    REGISTER_LOGGER("openstudio.analysis.OptimizationProblem");
  };

} // detail

} // model
} // openstudio

#endif // ANALYSIS_OPTIMIZATIONPROBLEM_IMPL_HPP

