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

#ifndef ANALYSIS_OPENSTUDIOALGORITHM_HPP
#define ANALYSIS_OPENSTUDIOALGORITHM_HPP

#include "AnalysisAPI.hpp"
#include "Algorithm.hpp"

namespace openstudio {
namespace analysis {

class Analysis;

namespace detail {

  class OpenStudioAlgorithm_Impl;

} // detail

/** OpenStudioAlgorithm is an Algorithm whose logic is directly encoded in the implementation 
 *  of createNextIteration. */
class ANALYSIS_API OpenStudioAlgorithm : public Algorithm {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~OpenStudioAlgorithm() {}

  //@}
  /** @name Actions */
  //@{
  
  /** Create the next iteration of work for analysis in the form of constructed but incomplete 
   *  \link DataPoint DataPoints \endlink. Throws openstudio::Exception if analysis.algorithm() 
   *  != *this. Returns the number of DataPoints added. */
  int createNextIteration(Analysis& analysis);

  //@}
 protected:
  /// @cond
  typedef detail::OpenStudioAlgorithm_Impl ImplType;

  explicit OpenStudioAlgorithm(std::shared_ptr<detail::OpenStudioAlgorithm_Impl> impl);

  friend class detail::OpenStudioAlgorithm_Impl;
  friend class AnalysisObject;
  friend class detail::AnalysisObject_Impl;

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.analysis.OpenStudioAlgorithm");
};

/** \relates OpenStudioAlgorithm*/
typedef boost::optional<OpenStudioAlgorithm> OptionalOpenStudioAlgorithm;

/** \relates OpenStudioAlgorithm*/
typedef std::vector<OpenStudioAlgorithm> OpenStudioAlgorithmVector;

} // analysis
} // openstudio

#endif // ANALYSIS_OPENSTUDIOALGORITHM_HPP

