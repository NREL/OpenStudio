/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

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

