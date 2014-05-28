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

#ifndef ANALYSIS_SEQUENTIALSEARCHOPTIONS_HPP
#define ANALYSIS_SEQUENTIALSEARCHOPTIONS_HPP

#include "AnalysisAPI.hpp"
#include "AlgorithmOptions.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  class SequentialSearchOptions_Impl;

} // detail

/** SequentialSearchOptions is an options class for SequentialSearch, derived from 
 *  AlgorithmOptions. \relates SequentialSearch */
class ANALYSIS_API SequentialSearchOptions : public AlgorithmOptions {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Constructor. Required argument objectiveToMinimizeFirst is the index of the objective 
   *  function (0 or 1) for which the minimum curve will be constructed. This means that the 
   *  SequentialSearch algorithm will first work towards locating the point that minimizes this
   *  objective function, in the direction of also minimizing the other objective function. After
   *  achieving this minimum point, objectiveToMinimizeFirst will be allowed to increase, while
   *  the other objective function continues to be driven towards its minimum. The final 
   *  MinimumCurve(objectiveToMinimizeFirst) will contain the convex hull of the overall 
   *  ParetoFront; the values of objectiveToMinimizeFirst will first decrease, then increase; and 
   *  the other objective function will monotonically decrease. 
   *
   *  In NREL analyses, objectiveToMinimizeFirst typically refers to an economic objective, and 
   *  the other objective function is typically an energy metric. In this way the final curve 
   *  traces out minimum cost designs for each level of energy use that improves on the baseline
   *  (existing building or code minimum). */
  explicit SequentialSearchOptions(int objectiveToMinimizeFirst);

  /** Constructor provided for deserialization; not for general use. */
  explicit SequentialSearchOptions(const std::vector<Attribute>& options);

  virtual ~SequentialSearchOptions() {}

  //@}
  /** @name Getters and Queries */
  //@{

  int objectiveToMinimizeFirst() const;

  //@}
 protected:
  /// @cond
  typedef detail::SequentialSearchOptions_Impl ImplType;

  explicit SequentialSearchOptions(std::shared_ptr<detail::SequentialSearchOptions_Impl> impl);

  friend class detail::SequentialSearchOptions_Impl;
  friend class AlgorithmOptions;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.analysis.SequentialSearchOptions");
};

/** \relates SequentialSearchOptions*/
typedef boost::optional<SequentialSearchOptions> OptionalSequentialSearchOptions;

/** \relates SequentialSearchOptions*/
typedef std::vector<SequentialSearchOptions> SequentialSearchOptionsVector;

} // analysis
} // openstudio

#endif // ANALYSIS_SEQUENTIALSEARCHOPTIONS_HPP

