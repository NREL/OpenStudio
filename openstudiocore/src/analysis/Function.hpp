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

#ifndef ANALYSIS_FUNCTION_HPP
#define ANALYSIS_FUNCTION_HPP

#include "AnalysisAPI.hpp"
#include "AnalysisObject.hpp"

namespace openstudio {
namespace analysis {

class Variable;
class DataPoint;
class Problem;
class OptimizationProblem;

namespace detail {

  class Function_Impl;
  class Problem_Impl;
  class OptimizationProblem_Impl;

} // detail

/** Function is an abstract AnalysisObject that registers a set of \link Variable Variables
 *  \endlink whose values can be retrieved from a DataPoint, and requires classes derived from it
 *  to implement Function::getValue. Function values are generally evaluated after a DataPoint
 *  is simulated, and may be used to direct the course of an Algorithm. */
class ANALYSIS_API Function : public AnalysisObject {
 public:
  /** @name Constructors */
  //@{

  virtual ~Function() {}

  //@}
  /** @name Getters */
  //@{

  std::vector<Variable> variables() const;

  double getValue(const DataPoint& dataPoint) const;

  //@}
 protected:
  /// @cond
  typedef detail::Function_Impl ImplType;

  friend class AnalysisObject;
  friend class detail::AnalysisObject_Impl;

  explicit Function(std::shared_ptr<detail::Function_Impl> impl);

  friend class Problem; // to call setParent
  friend class OptimizationProblem; // to call setParent
  friend class detail::Problem_Impl; // to call setParent
  friend class detail::OptimizationProblem_Impl; // to call setParent
  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.analysis.Function");
};

/** \relates Function*/
typedef boost::optional<Function> OptionalFunction;

/** \relates Function*/
typedef std::vector<Function> FunctionVector;

} // analysis
} // openstudio

#endif // ANALYSIS_FUNCTION_HPP
