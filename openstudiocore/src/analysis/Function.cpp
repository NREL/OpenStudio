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

#include "Function.hpp"
#include "Function_Impl.hpp"

#include "InputVariable.hpp"
#include "InputVariable_Impl.hpp"
#include "LinearFunction.hpp"
#include "LinearFunction_Impl.hpp"
#include "WorkflowStep.hpp"
#include "WorkflowStep_Impl.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  Function_Impl::Function_Impl(const std::string& name,
                               const std::vector<Variable>& variables)
    : AnalysisObject_Impl(name), m_variables(variables)
  {
    for (Variable& variable : m_variables) {
      variable.onChange();
      connectChild(variable,false);
    }
  }

  Function_Impl::Function_Impl(const UUID& uuid,
                               const UUID& versionUUID,
                               const std::string& name,
                               const std::string& displayName,
                               const std::string& description,
                               const std::vector<Variable>& variables)
    : AnalysisObject_Impl(uuid,versionUUID,name,displayName,description),
      m_variables(variables)
  {
    for (Variable& variable : m_variables) {
      connectChild(variable,false);
    }
  }

  Function_Impl::Function_Impl(const Function_Impl &other)
    : AnalysisObject_Impl(other)
  {
    for (const Variable& var : other.variables()) {
      m_variables.push_back(var.clone().cast<Variable>());
      connectChild(m_variables.back(),false);
    }
  }

  std::vector<Variable> Function_Impl::variables() const {
    return m_variables;
  }

  bool Function_Impl::doNotParent(const Variable& variable) const {
    // if input variable parented by workflow step, leave parentage as-is.
    // do go ahead and connect signals so dirty signal travels up and down.
    // note that this is an 80% solution--may need to provide better fix later.
    if (OptionalInputVariable inputVariable = variable.optionalCast<InputVariable>()) {
      if (OptionalAnalysisObject inputVariableParent = inputVariable->parent()) {
        if (OptionalWorkflowStep inputVariableParentAsWorkflowStep = inputVariableParent->optionalCast<WorkflowStep>()) {
          return true;
        }
      }
    }
    return false;
  }

  Function Function_Impl::factoryFromVariant(const QVariant& variant, const VersionString& version) {
    QVariantMap map = variant.toMap();

    if (!map.contains("function_type")) {
      LOG_AND_THROW("Unable to find Function in expected location.");
    }

    std::string functionType = map["function_type"].toString().toStdString();
    if (functionType == "LinearFunction") {
      return LinearFunction_Impl::fromVariant(variant,version);
    }

    LOG_AND_THROW("Unexpected function_type " << functionType << ".");
  }

} // detail

std::vector<Variable> Function::variables() const {
  return getImpl<detail::Function_Impl>()->variables();
}

double Function::getValue(const DataPoint& dataPoint) const {
  return getImpl<detail::Function_Impl>()->getValue(dataPoint);
}

/// @cond
Function::Function(std::shared_ptr<detail::Function_Impl> impl)
  : AnalysisObject(impl)
{}
/// @endcond

} // analysis
} // openstudio

