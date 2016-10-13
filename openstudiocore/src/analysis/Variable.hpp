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

#ifndef ANALYSIS_VARIABLE_HPP
#define ANALYSIS_VARIABLE_HPP

#include "AnalysisAPI.hpp"
#include "AnalysisObject.hpp"

namespace openstudio {
namespace analysis {

class DataPoint;
class LinearFunction;

namespace detail {

  class Function_Impl;
  class LinearFunction_Impl;
  class Variable_Impl;

} // detail

/** Variable is an AnalysisObject that serves as a base class for InputVariable and
 *  OutputVariable. */
class ANALYSIS_API Variable : public AnalysisObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~Variable() {}

  //@}
  /** @name Getters and Queries */
  //@{

  // TODO - Think about generalizing this to return an Attribute. Then we could more properly
  // handle integer values and Quantities.
  /** Get the variable value from a dataPoint. Throws an openstudio::Exception if the Variable
   *  cannot be evaluated for dataPoint. */
  double getValue(const DataPoint& dataPoint) const;

  //@}
 protected:
  /// @cond
  typedef detail::Variable_Impl ImplType;

  friend class AnalysisObject;
  friend class detail::AnalysisObject_Impl;

  explicit Variable(std::shared_ptr<detail::Variable_Impl> impl);

  friend class detail::Function_Impl; // calls Variable::onChange
  friend class LinearFunction; // calls Variable::setParent
  friend class detail::LinearFunction_Impl; // calls Variable::setParent
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.analysis.Variable");
};

/** \relates Variable*/
typedef boost::optional<Variable> OptionalVariable;

/** \relates Variable*/
typedef std::vector<Variable> VariableVector;

} // analysis
} // openstudio

#endif // ANALYSIS_VARIABLE_HPP

