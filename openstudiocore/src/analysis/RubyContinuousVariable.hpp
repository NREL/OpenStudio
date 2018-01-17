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

#ifndef ANALYSIS_RUBYCONTINUOUSVARIABLE_HPP
#define ANALYSIS_RUBYCONTINUOUSVARIABLE_HPP

#include "AnalysisAPI.hpp"
#include "ContinuousVariable.hpp"

namespace openstudio {

namespace ruleset {
  class OSArgument;
}

namespace analysis {

class RubyMeasure;

namespace detail {

  class RubyContinuousVariable_Impl;
  class RubyMeasure_Impl;

} // detail

/** RubyContinuousVariable is a ContinuousVariable. */
class ANALYSIS_API RubyContinuousVariable : public ContinuousVariable {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Construct from argument and measure. The RubyMeasure should not yet have argument
   *  set. It should have all fixed (non-variable) arguments set. */
  RubyContinuousVariable(const std::string& name,
                         const ruleset::OSArgument& argument,
                         const RubyMeasure& measure);

  /** Constructor provided for deserialization; not for general use. */
  RubyContinuousVariable(const UUID& uuid,
                         const UUID& versionUUID,
                         const std::string& name,
                         const std::string& displayName,
                         const std::string& description,
                         const boost::optional<UncertaintyDescription>& udesc,
                         boost::optional<double> minimum,
                         boost::optional<double> maximum,
                         boost::optional<double> increment,
                         boost::optional<int> nSteps,
                         const ruleset::OSArgument& argument,
                         const RubyMeasure& measure);

  virtual ~RubyContinuousVariable() {}

  //@}
  /** @name Getters and Queries */
  //@{

  ruleset::OSArgument argument() const;

  RubyMeasure measure() const;

  /** \deprecated */
  RubyMeasure perturbation() const;

  //@}
  /** @name Setters */
  //@{

  void setArgument(const ruleset::OSArgument& argument);

  bool setRubyMeasure(const RubyMeasure& measure);

  /** \deprecated */
  bool setRubyPerturbation(const RubyMeasure& measure);

  //@}
 protected:
  /// @cond
  typedef detail::RubyContinuousVariable_Impl ImplType;

  friend class AnalysisObject;
  friend class detail::AnalysisObject_Impl;
  friend class detail::RubyContinuousVariable_Impl;

  explicit RubyContinuousVariable(std::shared_ptr<detail::RubyContinuousVariable_Impl> impl);

  friend class detail::RubyMeasure_Impl;

  bool fileTypesAreCompatible(const RubyMeasure& childRubyMeasure,
                              const boost::optional<FileReferenceType>& proposedInputFileType,
                              const boost::optional<FileReferenceType>& proposedOutputFileType) const;

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.analysis.RubyContinuousVariable");
};

/** \relates RubyContinuousVariable*/
typedef boost::optional<RubyContinuousVariable> OptionalRubyContinuousVariable;

/** \relates RubyContinuousVariable*/
typedef std::vector<RubyContinuousVariable> RubyContinuousVariableVector;

} // analysis
} // openstudio

#endif // ANALYSIS_RUBYCONTINUOUSVARIABLE_HPP

