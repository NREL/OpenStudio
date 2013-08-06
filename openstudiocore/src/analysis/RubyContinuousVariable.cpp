/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <analysis/RubyContinuousVariable.hpp>
#include <analysis/RubyContinuousVariable_Impl.hpp>

#include <analysis/DataPoint.hpp>
#include <analysis/Problem.hpp>
#include <analysis/Problem_Impl.hpp>
#include <analysis/RubyPerturbation_Impl.hpp>
#include <analysis/WorkflowStep.hpp>
#include <analysis/WorkflowStep_Impl.hpp>

#include <runmanager/lib/RubyJobUtils.hpp>
#include <runmanager/lib/WorkItem.hpp>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace analysis {

namespace detail {

  RubyContinuousVariable_Impl::RubyContinuousVariable_Impl(
      const std::string& name,
      const ruleset::UserScriptArgument& argument,
      const RubyPerturbation& perturbation)
    : ContinuousVariable_Impl(name),
      m_argument(argument),
      m_perturbation(perturbation)
  {
    ruleset::UserScriptArgumentType argType = m_argument.type();
    if (!((argType == ruleset::UserScriptArgumentType::Double) ||
          (argType == ruleset::UserScriptArgumentType::Quantity)))
    {
      LOG_AND_THROW("RubyContinuousVariables can only be constructed from Double or Quantity "
                    << "UserScriptArguments.")
    }
    connectChild(m_perturbation,false);
  }

  RubyContinuousVariable_Impl::RubyContinuousVariable_Impl(
      const UUID& uuid,
      const UUID& versionUUID,
      const std::string& name,
      const std::string& displayName,
      const std::string& description,
      const boost::optional<UncertaintyDescription>& udesc,
      boost::optional<double> minimum,
      boost::optional<double> maximum,
      boost::optional<double> increment,
      boost::optional<int> nSteps,
      const ruleset::UserScriptArgument& argument,
      const RubyPerturbation& perturbation)
    : ContinuousVariable_Impl(uuid,
                              versionUUID,
                              name,
                              displayName,
                              description,
                              udesc,
                              minimum,
                              maximum,
                              increment,
                              nSteps),
      m_argument(argument),
      m_perturbation(perturbation)
  {
    connectChild(m_perturbation,false);
  }

  RubyContinuousVariable_Impl::RubyContinuousVariable_Impl(const RubyContinuousVariable_Impl &other)
    : ContinuousVariable_Impl(other),
      m_argument(other.argument().clone()),
      m_perturbation(other.perturbation().clone().cast<RubyPerturbation>())
  {
    connectChild(m_perturbation,false);
  }

  AnalysisObject RubyContinuousVariable_Impl::clone() const {
    boost::shared_ptr<RubyContinuousVariable_Impl> impl(new RubyContinuousVariable_Impl(*this));
    RubyContinuousVariable result(impl);
    RubyPerturbation perturbation = result.perturbation();
    perturbation.setParent(result);
    return result;
  }

  double RubyContinuousVariable_Impl::getValue(const DataPoint& dataPoint) const {
    OptionalInt index = dataPoint.problem().getVariableIndexByUUID(uuid());
    OS_ASSERT(index);
    return dataPoint.variableValues()[*index].toDouble();
  }

  boost::optional<FileReferenceType> RubyContinuousVariable_Impl::inputFileType() const {
    return perturbation().inputFileType();
  }

  boost::optional<FileReferenceType> RubyContinuousVariable_Impl::outputFileType() const {
    return perturbation().outputFileType();
  }

  bool RubyContinuousVariable_Impl::isValid(const QVariant& value) const {
    if (value.type() == QVariant::Double) {
      return isFeasible(value.toDouble());
    }
    return false;
  }

  bool RubyContinuousVariable_Impl::isValid(const UncertaintyDescription& udesc) const {
    UncertaintyDescriptionTypeVector validTypes = UncertaintyDescription::validTypes(VariableValueType::Continuous);
    return (std::find(validTypes.begin(),validTypes.end(),udesc.type()) != validTypes.end());
  }

  runmanager::WorkItem RubyContinuousVariable_Impl::createWorkItem(
      const QVariant& value,
      const openstudio::path& rubyIncludeDirectory) const
  {
    // This is only complete if this is the only RubyContinuousVariable using this
    // RubyPerturbation.
    RubyPerturbation pertClone = perturbation().clone().cast<RubyPerturbation>();
    ruleset::OSArgument arg = argument().clone();
    arg.setValue(value.toDouble());
    pertClone.setArgument(arg);
    return pertClone.createWorkItem(rubyIncludeDirectory);
  }

  ruleset::UserScriptArgument RubyContinuousVariable_Impl::argument() const {
    return m_argument;
  }

  RubyPerturbation RubyContinuousVariable_Impl::perturbation() const {
    return m_perturbation;
  }

  void RubyContinuousVariable_Impl::setArgument(const ruleset::OSArgument& argument) {
    m_argument = argument;
    onChange(AnalysisObject_Impl::InvalidatesResults);
  }

  bool RubyContinuousVariable_Impl::setRubyPerturbation(const RubyPerturbation& perturbation) {
    if (!fileTypesAreCompatible(m_perturbation,
                                perturbation.inputFileType(),
                                perturbation.outputFileType()))
    {
      return false;
    }
    disconnectChild(m_perturbation);
    m_perturbation = perturbation;
    connectChild(m_perturbation,true);
    onChange(AnalysisObject_Impl::InvalidatesResults);
    return true;
  }

  bool RubyContinuousVariable_Impl::fileTypesAreCompatible(
      const RubyPerturbation& childRubyPerturbation,
      const boost::optional<FileReferenceType>& proposedInputFileType,
      const boost::optional<FileReferenceType>& proposedOutputFileType) const
  {
    // RubyPerturbation stands alone in this variable, so only need to check with problem.
    if (OptionalAnalysisObject parent = this->parent()) {
      if (!(parent->cast<WorkflowStep>().fileTypesAreCompatible(proposedInputFileType,
                                                                proposedOutputFileType)))
      {
        return false;
      }
    }
    return true;
  }

} // detail

RubyContinuousVariable::RubyContinuousVariable(const std::string& name,
                                               const ruleset::UserScriptArgument& argument,
                                               const RubyPerturbation& perturbation)
  : ContinuousVariable(boost::shared_ptr<detail::RubyContinuousVariable_Impl>(
        new detail::RubyContinuousVariable_Impl(name,argument,perturbation)))
{
  RubyContinuousVariable copyOfThis(getImpl<detail::RubyContinuousVariable_Impl>());
  perturbation.setParent(copyOfThis);
}

RubyContinuousVariable::RubyContinuousVariable(const UUID& uuid,
                                               const UUID& versionUUID,
                                               const std::string& name,
                                               const std::string& displayName,
                                               const std::string& description,
                                               const boost::optional<UncertaintyDescription>& udesc,
                                               boost::optional<double> minimum,
                                               boost::optional<double> maximum,
                                               boost::optional<double> increment,
                                               boost::optional<int> nSteps,
                                               const ruleset::UserScriptArgument& argument,
                                               const RubyPerturbation& perturbation)
  : ContinuousVariable(boost::shared_ptr<detail::RubyContinuousVariable_Impl>(
        new detail::RubyContinuousVariable_Impl(uuid,
                                                versionUUID,
                                                name,
                                                displayName,
                                                description,
                                                udesc,
                                                minimum,
                                                maximum,
                                                increment,
                                                nSteps,
                                                argument,
                                                perturbation)))
{
  RubyContinuousVariable copyOfThis(getImpl<detail::RubyContinuousVariable_Impl>());
  perturbation.setParent(copyOfThis);
}

ruleset::UserScriptArgument RubyContinuousVariable::argument() const {
  return getImpl<detail::RubyContinuousVariable_Impl>()->argument();
}

RubyPerturbation RubyContinuousVariable::perturbation() const {
  return getImpl<detail::RubyContinuousVariable_Impl>()->perturbation();
}

void RubyContinuousVariable::setArgument(const ruleset::OSArgument& argument) {
  getImpl<detail::RubyContinuousVariable_Impl>()->setArgument(argument);
}

bool RubyContinuousVariable::setRubyPerturbation(const RubyPerturbation& perturbation) {
  return getImpl<detail::RubyContinuousVariable_Impl>()->setRubyPerturbation(perturbation);
}

/// @cond
RubyContinuousVariable::RubyContinuousVariable(boost::shared_ptr<detail::RubyContinuousVariable_Impl> impl)
  : ContinuousVariable(impl)
{}

bool RubyContinuousVariable::fileTypesAreCompatible(
    const RubyPerturbation& childRubyPerturbation,
    const boost::optional<FileReferenceType>& proposedInputFileType,
    const boost::optional<FileReferenceType>& proposedOutputFileType) const
{
  return getImpl<detail::RubyContinuousVariable_Impl>()->fileTypesAreCompatible(childRubyPerturbation,
                                                                                proposedInputFileType,
                                                                                proposedOutputFileType);
}
/// @endcond

} // analysis
} // openstudio
