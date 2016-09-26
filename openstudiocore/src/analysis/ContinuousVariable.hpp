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

#ifndef ANALYSIS_CONTINUOUSVARIABLE_HPP
#define ANALYSIS_CONTINUOUSVARIABLE_HPP

#include "AnalysisAPI.hpp"
#include "InputVariable.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  class ContinuousVariable_Impl;

} // detail

/** A ContinuousVariable is an InputVariable whose value is a real number, perhaps
 *  restricted to lie within some bounds. */
class ANALYSIS_API ContinuousVariable : public InputVariable {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~ContinuousVariable() {}

  //@}
  /** @name Getters and Queries */
  //@{

  /** Returns the minimum, or lower bound, on the ContinuousVariable, if it exists. */
  boost::optional<double> minimum() const;

  /** Returns the maximum, or upper bound, on the ContinuousVariable, if it exists. */
  boost::optional<double> maximum() const;

  /** Returns the increment to be applied to this ContinuousVariable when using minimum() and
   *  maximum() to create discrete values. */
  boost::optional<double> increment() const;

  /** Returns the number of steps to be used with minimum() and maximum() to create discrete
   *  values. */
  boost::optional<int> nSteps() const;

  /** Returns true if value in [minimum,maximum]. */
  bool isFeasible(double value) const;

  /** Returns a vector of incremental values, if minimum() && maximum() &&
   *  (increment() || nSteps()). There are two exceptional cases. If minimum() > maximum(),
   *  then an empty vector is returned. If equal(minimum(),maximum()), then a single value
   *  is returned. */
  std::vector<double> incrementalValues() const;

  //@}
  /** @name Setters */
  //@{

  void setMinimum(double minimum);

  void setMaximum(double maximum);

  /** Sets increment and nullifies nSteps. */
  void setIncrement(double increment);

  /** Sets nSteps and nullifies increment. */
  void setNSteps(int nSteps);

  void clearMinimum();

  void clearMaximum();

  void clearIncrement();

  void clearNSteps();

  //@}
  /** @name Actions */
  //@{

  /** Truncates value to [minimum,maximum]. If minimum > maximum, returns boost::none. */
  boost::optional<double> truncate(double value) const;

  //@}
 protected:
  /// @cond
  typedef detail::ContinuousVariable_Impl ImplType;

  friend class AnalysisObject;
  friend class detail::AnalysisObject_Impl;

  explicit ContinuousVariable(std::shared_ptr<detail::ContinuousVariable_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.analysis.ContinuousVariable");
};

/** \relates ContinuousVariable */
typedef boost::optional<ContinuousVariable> OptionalContinuousVariable;

/** \relates ContinuousVariable */
typedef std::vector<ContinuousVariable> ContinuousVariableVector;

} // analysis
} // openstudio

#endif // ANALYSIS_CONTINUOUSVARIABLE_HPP
