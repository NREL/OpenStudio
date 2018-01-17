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

#ifndef ANALYSIS_DAKOTAALGORITHM_IMPL_HPP
#define ANALYSIS_DAKOTAALGORITHM_IMPL_HPP

#include "AnalysisAPI.hpp"
#include "Algorithm_Impl.hpp"

#include "../runmanager/lib/Job.hpp"

#include "../utilities/core/FileReference.hpp"

namespace openstudio {
namespace analysis {

class Analysis;
class DakotaAlgorithmOptions;
class DataPoint;
class DakotaParametersFile;
class UncertaintyDescriptionType;

namespace detail {

  /** DakotaAlgorithm_Impl is a Algorithm_Impl that is the implementation class for
   *  DakotaAlgorithm. */
  class ANALYSIS_API DakotaAlgorithm_Impl : public Algorithm_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    DakotaAlgorithm_Impl(const std::string& name, const DakotaAlgorithmOptions& options);

    // TODO: Add other items necessary for construction.
    DakotaAlgorithm_Impl(const UUID& uuid,
                         const UUID& versionUUID,
                         const std::string& name,
                         const std::string& displayName,
                         const std::string& description,
                         bool complete,
                         bool failed,
                         int iter,
                         const DakotaAlgorithmOptions& options,
                         const boost::optional<FileReference>& restartFileReference,
                         const boost::optional<FileReference>& outFileReference, 
                         const boost::optional<openstudio::runmanager::Job>& job);

    DakotaAlgorithm_Impl(const DakotaAlgorithm_Impl& other);

    virtual ~DakotaAlgorithm_Impl() {}

    //@}
    /** @name Getters and Queries */
    //@{

    /** Returns true if the algorithm requires continuous variables. The Problem class uses this
     *  method to determine whether to represent OpenStudio DiscreteVariables as continuous
     *  variables when communicating with DAKOTA. If the desired behavior is to fail, rather than
     *  to fake continuity, then isCompatibleProblemType should return false if Problem contains
     *  non-trivial (> 1 perturbation) discrete variables. */
    virtual bool requiresContinuousVariables() const;

    virtual bool isCompatible(const UncertaintyDescriptionType& udescType) const;

    /** Returns a reference to the DAKOTA restart file, if possible. Should exist if this algorithm
     *  is used by an analysis that has been run using the AnalysisDriver. */
    boost::optional<FileReference> restartFileReference() const;

    /** Returns a reference to the DAKOTA out file, if possible. Should exist if this algorithm
     *  is used by an analysis that has been run using the AnalysisDriver. */
    boost::optional<FileReference> outFileReference() const;

    /** Returns the RunManager job if possible. */
    boost::optional<runmanager::Job> job() const;

    //@}
    /** @name Actions */
    //@{

    virtual std::string dakotaInFileDescription() const = 0;

    /** Tries to create a new data point from dakotaParametersFile, returning the new point, or an
     *  equivalent point already present in analysis. Also updates this algorithm's internal iter
     *  counter. */
    virtual boost::optional<DataPoint> createNextDataPoint(Analysis& analysis,
                                                           const DakotaParametersFile& params);

    //@}
    /** @name Protected in or Absent from Public Class */
    //@{

    virtual void reset() override;

    /** Called after Dakota job is constructed. As a side effect, sets iter to 0. */
    void setRestartFileReference(const FileReference& file);

    /** Called after Dakota job is constructed. As a side effect, sets iter to 0. */
    void setOutFileReference(const FileReference& file);

    /** Sets the RunManager job. */
    void setJob(const runmanager::Job& job);

    virtual QVariant toVariant() const override;

    //@}
   private:
    boost::optional<FileReference> m_restartFileReference;
    boost::optional<FileReference> m_outFileReference;
    boost::optional<runmanager::Job> m_job; 

    REGISTER_LOGGER("openstudio.analysis.DakotaAlgorithm");
  };

} // detail

} // analysis
} // openstudio

#endif // ANALYSIS_DAKOTAALGORITHM_IMPL_HPP

