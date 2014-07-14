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

#ifndef ANALYSIS_SEQUENTIALSEARCHOPTIONS_IMPL_HPP
#define ANALYSIS_SEQUENTIALSEARCHOPTIONS_IMPL_HPP

#include "AnalysisAPI.hpp"
#include "AlgorithmOptions_Impl.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  /** SequentialSearchOptions_Impl is an AlgorithmOptions_Impl that is the implementation class for 
   *  SequentialSearchOptions.*/
  class ANALYSIS_API SequentialSearchOptions_Impl : public AlgorithmOptions_Impl {
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
    explicit SequentialSearchOptions_Impl(int objectiveToMinimizeFirst);

    /** Constructor provided for deserialization; not for general use. */
    explicit SequentialSearchOptions_Impl(const std::vector<Attribute>& options);

    virtual ~SequentialSearchOptions_Impl() {}

    virtual AlgorithmOptions clone() const;

    //@}
    /** @name Getters */
    //@{

    int objectiveToMinimizeFirst() const;

    //@}
    /** @name Absent or Protected in Public Class */
    //@{

    static SequentialSearchOptions fromVariant(const QVariant& variant, const VersionString& version);

    //@}
   private:
    REGISTER_LOGGER("openstudio.analysis.SequentialSearchOptions");
  };

} // detail

} // analysis
} // openstudio

#endif // ANALYSIS_SEQUENTIALSEARCHOPTIONS_IMPL_HPP

