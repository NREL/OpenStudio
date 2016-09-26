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

#ifndef ANALYSIS_NULLMEASURE_HPP
#define ANALYSIS_NULLMEASURE_HPP

#include "AnalysisAPI.hpp"
#include "Measure.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  class NullMeasure_Impl;

} // detail

/** NullMeasure is a Measure that does not change the input model in any way.
 *  It should be used if one of the design options for the DiscreteVariable is to leave the seed 
 *  model unchanged. */
class ANALYSIS_API NullMeasure : public Measure {
 public:
  /** @name Constructors and Destructors */
  //@{

  NullMeasure(bool isSelected=true);

  /** Constructor provided for deserialization; not for general use. */
  NullMeasure(const UUID& uuid,
              const UUID& versionUUID,
              const std::string& name,
              const std::string& displayName,
              const std::string& description,
              bool isSelected);

  virtual ~NullMeasure() {}

  //@}
 protected:
  /// @cond
  typedef detail::NullMeasure_Impl ImplType;

  friend class AnalysisObject;
  friend class detail::AnalysisObject_Impl;
  friend class detail::NullMeasure_Impl;

  explicit NullMeasure(std::shared_ptr<detail::NullMeasure_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.analysis.NullMeasure");
};

/** \relates NullMeasure*/
typedef boost::optional<NullMeasure> OptionalNullMeasure;

/** \relates NullMeasure*/
typedef std::vector<NullMeasure> NullMeasureVector;

// ETH@20130723 - These typedefs are just to preserve the old names for the near to
// medium-term future (to avoid breaking existing scripts).

/** \relates NullMeasure*/
typedef NullMeasure NullPerturbation;

/** \relates NullMeasure*/
typedef boost::optional<NullMeasure> OptionalNullPerturbation;

/** \relates NullMeasure*/
typedef std::vector<NullMeasure> NullPerturbationVector;

} // analysis
} // openstudio

#endif // ANALYSIS_NULLMEASURE_HPP

