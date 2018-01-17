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

#ifndef ANALYSIS_DDACEALGORITHM_HPP
#define ANALYSIS_DDACEALGORITHM_HPP

#include "AnalysisAPI.hpp"
#include "DakotaAlgorithm.hpp"

namespace openstudio {
namespace analysis {

class DDACEAlgorithmOptions;

namespace detail {

  class DDACEAlgorithm_Impl;

} // detail

/** DDACEAlgorithm is a DakotaAlgorithm that implements dakotaInFileDescription so as to specify
 *  a particular Distributed Design and Analysis for Computer Experiments (DDACE) method and 
 *  related options. This family of algorithms works best with continuous variables, but we have 
 *  made it so most of the methods (all but DDACEAlgorithmType::central_composite, 
 *  DDACEAlgorithmType::box_behnken, and DDACEAlgorithmType::grid) will also operate on discrete
 *  variables (by presenting them as continuous variables to DAKOTA). */
class ANALYSIS_API DDACEAlgorithm : public DakotaAlgorithm {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Construct from options. */
  explicit DDACEAlgorithm(const DDACEAlgorithmOptions& options);

  /** Constructor provided for deserialization; not for general use. */
  DDACEAlgorithm(const UUID& uuid,
                 const UUID& versionUUID,
                 const std::string& displayName,
                 const std::string& description,
                 bool complete,
                 bool failed,
                 int iter,
                 const DDACEAlgorithmOptions& options,
                 const boost::optional<FileReference>& restartFileReference,
                 const boost::optional<FileReference>& outFileReference,
                 const boost::optional<openstudio::runmanager::Job>& job);

  virtual ~DDACEAlgorithm() {}

  //@}
  /** @name Getters and Queries */
  //@{

  DDACEAlgorithmOptions ddaceAlgorithmOptions() const;

  //@}
 protected:
  /// @cond
  typedef detail::DDACEAlgorithm_Impl ImplType;

  explicit DDACEAlgorithm(std::shared_ptr<detail::DDACEAlgorithm_Impl> impl);

  friend class detail::DDACEAlgorithm_Impl;
  friend class AnalysisObject;
  friend class detail::AnalysisObject_Impl;

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.analysis.DDACEAlgorithm");
};

/** \relates DDACEAlgorithm*/
typedef boost::optional<DDACEAlgorithm> OptionalDDACEAlgorithm;

/** \relates DDACEAlgorithm*/
typedef std::vector<DDACEAlgorithm> DDACEAlgorithmVector;

} // analysis
} // openstudio

#endif // ANALYSIS_DDACEALGORITHM_HPP

