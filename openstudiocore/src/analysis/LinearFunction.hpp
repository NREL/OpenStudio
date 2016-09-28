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

#ifndef ANALYSIS_LINEARFUNCTION_HPP
#define ANALYSIS_LINEARFUNCTION_HPP

#include "AnalysisAPI.hpp"
#include "Function.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  class LinearFunction_Impl;

} // detail

/** LinearFunction is a Function of the form a1*x1 + a2*x2 + ... + an*xn where
 *  xi is the value of a Variable at a DataPoint, and ai is a coefficient value. */
class ANALYSIS_API LinearFunction : public Function {
 public:
  /** @name Constructors */
  //@{

  /** Construct linear function of variables. Throws openstudio::Exception if
   *  !coefficients.empty() and variables.size() != coefficients.size(). */
  LinearFunction(const std::string& name,
                 const std::vector<Variable>& variables,
                 const std::vector<double>& coefficients=std::vector<double>());

  LinearFunction(const UUID& uuid,
                 const UUID& versionUUID,
                 const std::string& name,
                 const std::string& displayName,
                 const std::string& description,
                 const std::vector<Variable>& variables,
                 const std::vector<double>& coefficients);

  virtual ~LinearFunction() {}

  //@}
  /** @name Getters */
  //@{

  std::vector<double> coefficients() const;

  //@}
  /** @name Setters */
  //@{

  bool setCoefficients(const std::vector<double>& coefficients);

  //@}
 protected:
  /// @cond
  typedef detail::LinearFunction_Impl ImplType;

  friend class AnalysisObject;
  friend class detail::AnalysisObject_Impl;
  friend class detail::LinearFunction_Impl;

  explicit LinearFunction(std::shared_ptr<detail::LinearFunction_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.analysis.LinearFunction");
};

/** \relates LinearFunction*/
typedef boost::optional<LinearFunction> OptionalLinearFunction;

/** \relates LinearFunction*/
typedef std::vector<LinearFunction> LinearFunctionVector;

} // analysis
} // openstudio

#endif // ANALYSIS_LINEARFUNCTION_HPP
