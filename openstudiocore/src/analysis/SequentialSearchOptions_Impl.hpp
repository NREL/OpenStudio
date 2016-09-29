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

    virtual AlgorithmOptions clone() const override;

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

