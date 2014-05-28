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

#ifndef ANALYSIS_OPTIMIZATIONPROBLEM_HPP
#define ANALYSIS_OPTIMIZATIONPROBLEM_HPP

#include "AnalysisAPI.hpp"
#include "Problem.hpp"

namespace openstudio {
namespace analysis {

class Function;

namespace detail {

  class OptimizationProblem_Impl;

} // detail

/** OptimizationProblem is a Problem that has objective functions in addition to variables,
 *  response functions, and a simulation workflow. The goal of solving an OptimizationProblem is to
 *  find the combination of variable values that minimizes one or more objective functions. If
 *  numObjectiveFunctions > 1, then any non-dominated point may be deemed desirable by a decision
 *  maker. (A non-dominated point is a point for which no objective function can be improved
 *  (decreased) without harming (increasing) another objective. OptimizationProblem ensures that
 *  \link OptimizationDataPoint OptimizationDataPoints \endlink are populated with their objective
 *  function values following successful simulation. */
class ANALYSIS_API OptimizationProblem : public Problem {
 public:
  /** @name Constructors and Destructors */
  //@{

  OptimizationProblem(const std::string& name,
                      const std::vector<Function>& objectives,
                      const std::vector<WorkflowStep>& workflow);

  OptimizationProblem(const std::string& name,
                      const std::vector<Function>& objectives,
                      const std::vector<WorkflowStep>& workflow,
                      const std::vector<Function>& responses);

  /** Construct from objectives, variables, responses and simulationWorkflow. In this context,
   *  response functions are design metrics of interest that are not being optimized over.
   *  \deprecated */
  OptimizationProblem(const std::string& name,
                      const std::vector<Function>& objectives,
                      const std::vector<Variable>& variables,
                      const std::vector<Function>& responses,
                      const runmanager::Workflow& simulationWorkflow);

  /** \overload \deprecated */
  OptimizationProblem(const std::string& name,
                      const std::vector<Function>& objectives,
                      const std::vector<Variable>& variables,
                      const runmanager::Workflow& simulationWorkflow);

  /** Constructor provided for deserialization; not for general use. */
  OptimizationProblem(const UUID& uuid,
                      const UUID& versionUUID,
                      const std::string& name,
                      const std::string& displayName,
                      const std::string& description,
                      const std::vector<Function>& objectives,
                      const std::vector<WorkflowStep>& workflow,
                      const std::vector<Function>& responses);

  virtual ~OptimizationProblem() {}

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
 protected:
  /// @cond
  typedef detail::OptimizationProblem_Impl ImplType;

  friend class AnalysisObject;
  friend class detail::AnalysisObject_Impl;
  friend class detail::OptimizationProblem_Impl;

  explicit OptimizationProblem(std::shared_ptr<detail::OptimizationProblem_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.analysis.OptimizationProblem");
};

/** \relates OptimizationProblem*/
typedef boost::optional<OptimizationProblem> OptionalOptimizationProblem;

/** \relates OptimizationProblem*/
typedef std::vector<OptimizationProblem> OptimizationProblemVector;

} // analysis
} // openstudio

#endif // ANALYSIS_OPTIMIZATIONPROBLEM_HPP

