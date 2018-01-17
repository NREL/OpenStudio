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

#ifndef ANALYSIS_DESIGNOFEXPERIMENTS_IMPL_HPP
#define ANALYSIS_DESIGNOFEXPERIMENTS_IMPL_HPP

#include "AnalysisAPI.hpp"
#include "OpenStudioAlgorithm_Impl.hpp"

#include "DesignOfExperiments.hpp"

namespace openstudio {
namespace analysis {

class DesignOfExperimentsOptions;

namespace detail {

  /** DesignOfExperiments_Impl is a Algorithm_Impl that is the implementation class for DesignOfExperiments.*/
  class ANALYSIS_API DesignOfExperiments_Impl : public OpenStudioAlgorithm_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit DesignOfExperiments_Impl(const DesignOfExperimentsOptions& options);

    /** Constructor provided for deserialization; not for general use. */
    DesignOfExperiments_Impl(const UUID& uuid,
                             const UUID& versionUUID,
                             const std::string& displayName,
                             const std::string& description,
                             bool complete,
                             bool failed,
                             int iter,
                             const DesignOfExperimentsOptions& options);

    DesignOfExperiments_Impl(const DesignOfExperiments_Impl& other);

    virtual ~DesignOfExperiments_Impl() {}

    virtual AnalysisObject clone() const override;

    //@}
    /** @name Virtual Methods */
    //@{

    /** Returns true if Algorithm can operate on problem. For example, DesignOfExperiments can work
     *  on all \link Problem Problems \endlink, but SequentialSearch requires an OptimizationProblem
     *  with two objective functions and \link DiscreteVariable DiscreteVariables\endlink. */
    virtual bool isCompatibleProblemType(const Problem& problem) const override;

    /** Create the next iteration of work for analysis in the form of constructed but incomplete
     *  \link DataPoint DataPoints \endlink. Throws openstudio::Exception if analysis.algorithm()
     *  != *this. */
    virtual int createNextIteration(Analysis& analysis) override;

    //@}
    /** @name Getters and Queries */
    //@{

    DesignOfExperimentsOptions designOfExperimentsOptions() const;

    //@}
    /** @name Absent or Protected in Public Class */
    //@{

    virtual QVariant toVariant() const override;

    static DesignOfExperiments fromVariant(const QVariant& variant, const VersionString& version);

    //@}
   private:
    REGISTER_LOGGER("openstudio.analysis.DesignOfExperiments");
  };

} // detail

} // model
} // openstudio

#endif // ANALYSIS_DESIGNOFEXPERIMENTS_IMPL_HPP

