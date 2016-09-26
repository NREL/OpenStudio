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

