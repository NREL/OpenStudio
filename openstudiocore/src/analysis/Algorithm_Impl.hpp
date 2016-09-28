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

#ifndef ANALYSIS_ALGORITHM_IMPL_HPP
#define ANALYSIS_ALGORITHM_IMPL_HPP

#include "AnalysisAPI.hpp"
#include "AnalysisObject_Impl.hpp"

#include "AlgorithmOptions.hpp"

namespace openstudio {
namespace analysis {

class Problem;

namespace detail {

  /** Algorithm_Impl is a AnalysisObject_Impl that is the implementation class for Algorithm.*/
  class ANALYSIS_API Algorithm_Impl : public AnalysisObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Algorithm name is set by class, not user. */
    Algorithm_Impl(const std::string& name,const AlgorithmOptions& options);

    /** Constructor provided for deserialization; not for general use. */
    Algorithm_Impl(const UUID& uuid,
                   const UUID& versionUUID,
                   const std::string& name,
                   const std::string& displayName,
                   const std::string& description,
                   bool complete,
                   bool failed,
                   int iter,
                   const AlgorithmOptions& options);

    Algorithm_Impl(const Algorithm_Impl& other);

    virtual ~Algorithm_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    /** Disallow overwriting Algorithm names. */
    virtual void setName(const std::string& newName) override;

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

    AlgorithmOptions options() const;

    /** Returns true if Algorithm can operate on problem. For example, DesignOfExperiments can work
     *  on all \link Problem Problems \endlink, but SequentialSearch requires an OptimizationProblem
     *  with two objective functions and \link DiscreteVariable DiscreteVariables\endlink. */
    virtual bool isCompatibleProblemType(const Problem& problem) const = 0;

    //@}
    /** @name Absent or Protected in Public Class */
    //@{

    void markComplete();

    /** Reset the complete flag. Used when clearing maximum iterations or simulations, for
     *  instance. */
    void resetComplete();

    void markFailed();

    void resetFailed();

    virtual void reset();

    virtual QVariant toVariant() const override;

    static Algorithm factoryFromVariant(const QVariant& variant, const VersionString& version);

    //@}
   protected:
    bool m_complete;
    bool m_failed;
    int m_iter;
    AlgorithmOptions m_options;
   private:
    REGISTER_LOGGER("openstudio.analysis.Algorithm");
  };

} // detail

} // model
} // openstudio

#endif // ANALYSIS_ALGORITHM_IMPL_HPP

