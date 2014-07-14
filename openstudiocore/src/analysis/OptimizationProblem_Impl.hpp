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

    virtual AnalysisObject clone() const;

    //@}
    /** Virtual Methods */
    //@{

    virtual bool isValid(const DataPoint& dataPoint) const;

    /** Returns a description of this problem for using in dakota.in files. */
    virtual std::string dakotaInFileDescription() const;

    virtual boost::optional<DataPoint> createDataPoint(
        const std::vector<QVariant>& variableValues) const;

    virtual boost::optional<DataPoint> createDataPoint(
        const std::vector<DiscretePerturbation>& perturbations) const;

    /** Attemps to open and parse dakotaParametersFile, extract variable values and other relevant
     *  information, and create a new DataPoint. Returns that DataPoint if possible; returns
     *  boost::none otherwise. */
    virtual boost::optional<DataPoint> createDataPoint(
        const openstudio::path& dakotaParametersFile) const;

    virtual void updateDataPoint(DataPoint& dataPoint,
                                 const runmanager::Job& completedJob) const;

    /** Returns the DAKOTA results file string corresponding to dataPoint, if dataPoint completed
     *  successfully. Returns boost::none otherwise. */
    virtual boost::optional<std::string> getDakotaResultsFile(const DataPoint& dataPoint) const;

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

    virtual QVariant toVariant() const;

    static OptimizationProblem fromVariant(const QVariant& variant, const VersionString& version);

    /// Relocate path data from originalBase to newBase.
    virtual void updateInputPathData(const openstudio::path& originalBase,
                                     const openstudio::path& newBase);

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

