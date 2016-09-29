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

#ifndef ANALYSIS_SEQUENTIALSEARCH_HPP
#define ANALYSIS_SEQUENTIALSEARCH_HPP

#include "AnalysisAPI.hpp"
#include "OpenStudioAlgorithm.hpp"

namespace openstudio {

namespace analysis {

class SequentialSearchOptions;
class DataPoint;
class OptimizationDataPoint;

namespace detail {

  class SequentialSearch_Impl;

} // detail

/** SequentialSearch is an OpenStudioAlgorithm that can be used to solve \link OptimizationProblem 
 *  OptimizationProblems \endlink with two objective functions and discrete variables. It is a 
 *  greedy heuristic method that traces out the minimum curve of one objective function while 
 *  constantly improving the other objective. */
class ANALYSIS_API SequentialSearch : public OpenStudioAlgorithm {
 public:
  /** @name Constructors and Destructors */
  //@{

  SequentialSearch(const SequentialSearchOptions& options);

  /** Constructor provided for deserialization; not for general use. */
  SequentialSearch(const UUID& uuid,
                   const UUID& versionUUID,
                   const std::string& displayName,
                   const std::string& description,
                   bool complete,
                   bool failed,
                   int iter,
                   const SequentialSearchOptions& options);

  virtual ~SequentialSearch() {}

  //@}

  static std::string standardName();

   /** @name Getters and Queries */
  //@{

  SequentialSearchOptions sequentialSearchOptions() const;

  //@}
  /** @name Actions */
  //@{

  /** Get the "swoosh" curve relative to objective function i, i == 0, or i == 1. Throws an
   *  openstudio::Exception if i is not equal to 0 or 1, or if analysis.algorithm() != *this. 
   *  Recalculates the curve, so changes DataPoint tags. This is the method used by 
   *  createNextIteration to determine which DataPoint should be explored next. If the algorithm
   *  has not finished running, the return vector .back() will not have the "explored" tag, but 
   *  all other returned points will. */
  std::vector<OptimizationDataPoint> getMinimumCurve(unsigned i,
                                                     Analysis& analysis) const;

  /** Get the Pareto front (set of non-dominated points). Throws an openstudio::Exception if 
   *  analysis.algorithm() != *this. May change DataPoint tags. This algorithm is independent of 
   *  getMinimumCurve. */
  std::vector<OptimizationDataPoint> getParetoFront(Analysis& analysis) const;

  /** Returns all "one-off" perturbations of dataPoint. Used by createNextIteration to generate 
   *  new \link OptimizationDataPoint OptimizationDataPoints \endlink (which are only added to the
   *  Analysis if they have not yet been run). This constitutes "exploration" of a dataPoint. The
   *  "one-off" perturbations are constructed by starting with dataPoint.variableValues() each 
   *  time, and then making a single valid change. Thus, each candidate combination will be the
   *  same as dataPoint.variableValues(), except for the value of one variable, which may be set 
   *  to any of its other valid values. */
  std::vector< std::vector<QVariant> > getCandidateCombinations(const DataPoint& dataPoint) const;

  //@}
 protected:
  /// @cond
  typedef detail::SequentialSearch_Impl ImplType;

  friend class AnalysisObject;
  friend class detail::AnalysisObject_Impl;
  friend class detail::SequentialSearch_Impl;

  explicit SequentialSearch(std::shared_ptr<detail::SequentialSearch_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.analysis.SequentialSearch");
};

/** \relates SequentialSearch*/
typedef boost::optional<SequentialSearch> OptionalSequentialSearch;

/** \relates SequentialSearch*/
typedef std::vector<SequentialSearch> SequentialSearchVector;

} // analysis
} // openstudio

#endif // ANALYSIS_SEQUENTIALSEARCH_HPP

