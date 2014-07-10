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

#ifndef ANALYSIS_ALGORITHM_HPP
#define ANALYSIS_ALGORITHM_HPP

#include "AnalysisAPI.hpp"
#include "AnalysisObject.hpp"

namespace openstudio {
namespace analysis {

class AlgorithmOptions;
class Problem;
class Analysis;

namespace detail {

  class Algorithm_Impl;
  class AlgorithmOptions_Impl;
  class Analysis_Impl;

} // detail

/** Algorithm is an abstract AnalysisObject that is the base class for all \link OpenStudioAlgorithm
 *  OpenStudioAlgorithms\endlink and \link DakotaAlgorithm DakotaAlgorithms\endlink. The primary
 *  job of an Algorithm is to maintain its iteration count and its AlgorithmOptions, and determine
 *  whether it is applicable to a given Problem.
 */
class ANALYSIS_API Algorithm : public AnalysisObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~Algorithm() {}

  //@}
  /** @name Getters and Queries */
  //@{

  /** Returns true if the Algorithm ran to completion. */
  bool isComplete() const;

  /** Returns true if the Algorithm ran to completion, but failed in some way. Perhaps not all
   *  simulations completed successfully, or output results could not be retrieved for some other
   *  reason. */
  bool failed() const;

  /** Returns the iteration number, which is incremented during the process of running an Analysis.
   *  After construction, iter() == -1. If an algorithm runs the baseline point before doing
   *  anything else, that iteration is iter 0. Otherwise, iter 1, 2, etc. are used. */
  int iter() const;

  /** AlgorithmOptions shares its data on assignment and copy construction. Therefore,
   *  .options().setMaxIter(3) would set the maximum number of iterations for this Algorithm object
   *  to 3. (There is no need for a .setOptions(const AlgorithmOptions&) method.) */
  AlgorithmOptions options() const;

  /** Returns true if Algorithm can operate on problem. For example, DesignOfExperiments can work
   *  on \link Problem Problems \endlink for which allVariablesAreDiscrete, and
   *  SequentialSearch requires an OptimizationProblem with two objective functions and
   *  allVariablesAreDiscrete. */
  bool isCompatibleProblemType(const Problem& problem) const;

  //@}
 protected:
  /// @cond
  typedef detail::Algorithm_Impl ImplType;

  friend class AnalysisObject;
  friend class detail::AnalysisObject_Impl;

  explicit Algorithm(std::shared_ptr<detail::Algorithm_Impl> impl);

  /** To be called by all concrete constructors. */
  void createCallbackForOptions() const;
  /// @endcond

  friend class Analysis; // for calling setParent
  friend class detail::Analysis_Impl;

  /** Resets algorithm back to its initial state. Called by Analysis when results are cleared. */
  void reset();

  friend class detail::AlgorithmOptions_Impl;

  void resetComplete();

 private:

  REGISTER_LOGGER("openstudio.analysis.Algorithm");
};

/** \relates Algorithm*/
typedef boost::optional<Algorithm> OptionalAlgorithm;

/** \relates Algorithm*/
typedef std::vector<Algorithm> AlgorithmVector;

} // analysis
} // openstudio

#endif // ANALYSIS_ALGORITHM_HPP

