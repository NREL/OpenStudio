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

#ifndef ANALYSIS_PARAMETERSTUDYALGORITHM_HPP
#define ANALYSIS_PARAMETERSTUDYALGORITHM_HPP

#include "AnalysisAPI.hpp"
#include "DakotaAlgorithm.hpp"

namespace openstudio {
namespace analysis {

class ParameterStudyAlgorithmOptions;

namespace detail {

  class ParameterStudyAlgorithm_Impl;

} // detail

/** ParameterStudyAlgorithm is a DakotaAlgorithm that implements dakotaInFileDescription so as to 
 *  specify a particular Parameter Study method and related options. */
class ANALYSIS_API ParameterStudyAlgorithm : public DakotaAlgorithm {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Construct from options. */
  ParameterStudyAlgorithm(const ParameterStudyAlgorithmOptions& options);

  /** Constructor provided for deserialization; not for general use. */
  ParameterStudyAlgorithm(const UUID& uuid,
                          const UUID& versionUUID,
                          const std::string& displayName,
                          const std::string& description,
                          bool complete,
                          bool failed,
                          int iter,
                          const ParameterStudyAlgorithmOptions& options,
                          const boost::optional<FileReference>& restartFileReference,
                          const boost::optional<FileReference>& outFileReference,
                          const boost::optional<openstudio::runmanager::Job>& job);

  virtual ~ParameterStudyAlgorithm() {}

  //@}

  /** @name Getters and Queries */
  //@{

  ParameterStudyAlgorithmOptions parameterStudyAlgorithmOptions() const;

  //@}
 protected:
  /// @cond
  typedef detail::ParameterStudyAlgorithm_Impl ImplType;

  explicit ParameterStudyAlgorithm(std::shared_ptr<detail::ParameterStudyAlgorithm_Impl> impl);

  friend class detail::ParameterStudyAlgorithm_Impl;
  friend class AnalysisObject;
  friend class detail::AnalysisObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.analysis.ParameterStudyAlgorithm");
};

/** \relates ParameterStudyAlgorithm*/
typedef boost::optional<ParameterStudyAlgorithm> OptionalParameterStudyAlgorithm;

/** \relates ParameterStudyAlgorithm*/
typedef std::vector<ParameterStudyAlgorithm> ParameterStudyAlgorithmVector;

} // analysis
} // openstudio

#endif // ANALYSIS_PARAMETERSTUDYALGORITHM_HPP

