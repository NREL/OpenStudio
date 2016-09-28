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

