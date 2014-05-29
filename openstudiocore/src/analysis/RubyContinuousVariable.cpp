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

#include "RubyContinuousVariable.hpp"
#include "RubyContinuousVariable_Impl.hpp"

#include "DataPoint.hpp"
#include "Problem.hpp"
#include "Problem_Impl.hpp"
#include "RubyMeasure_Impl.hpp"
#include "WorkflowStep.hpp"
#include "WorkflowStep_Impl.hpp"

#include "../runmanager/lib/RubyJobUtils.hpp"
#include "../runmanager/lib/WorkItem.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  RubyContinuousVariable_Impl::RubyContinuousVariable_Impl(
      const std::string& name,
      const ruleset::UserScriptArgument& argument,
      const RubyMeasure& measure)
    : ContinuousVariable_Impl(name),
      m_argument(argument),
      m_measure(measure)
  {
    ruleset::UserScriptArgumentType argType = m_argument.type();
    if (!((argType == ruleset::UserScriptArgumentType::Double) ||
          (argType == ruleset::UserScriptArgumentType::Quantity)))
    {
      LOG_AND_THROW("RubyContinuousVariables can only be constructed from Double or Quantity "
                    << "UserScriptArguments.")
    }
    connectChild(m_measure,false);
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
      const RubyMeasure& measure)
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
      m_measure(measure)
  {
    connectChild(m_measure,false);
  }

  RubyContinuousVariable_Impl::RubyContinuousVariable_Impl(const RubyContinuousVariable_Impl &other)
    : ContinuousVariable_Impl(other),
      m_argument(other.argument().clone()),
      m_measure(other.measure().clone().cast<RubyMeasure>())
  {
    connectChild(m_measure,false);
  }

  AnalysisObject RubyContinuousVariable_Impl::clone() const {
    std::shared_ptr<RubyContinuousVariable_Impl> impl(new RubyContinuousVariable_Impl(*this));
    RubyContinuousVariable result(impl);
    RubyMeasure measure = result.measure();
    measure.setParent(result);
    return result;
  }

  double RubyContinuousVariable_Impl::getValue(const DataPoint& dataPoint) const {
    OptionalInt index = dataPoint.problem().getVariableIndexByUUID(uuid());
    OS_ASSERT(index);
    return dataPoint.variableValues()[*index].toDouble();
  }

  boost::optional<FileReferenceType> RubyContinuousVariable_Impl::inputFileType() const {
    return measure().inputFileType();
  }

  boost::optional<FileReferenceType> RubyContinuousVariable_Impl::outputFileType() const {
    return measure().outputFileType();
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
    // RubyMeasure.
    RubyMeasure pertClone = measure().clone().cast<RubyMeasure>();
    ruleset::OSArgument arg = argument().clone();
    arg.setValue(value.toDouble());
    pertClone.setArgument(arg);
    return pertClone.createWorkItem(rubyIncludeDirectory);
  }

  ruleset::UserScriptArgument RubyContinuousVariable_Impl::argument() const {
    return m_argument;
  }

  RubyMeasure RubyContinuousVariable_Impl::measure() const {
    return m_measure;
  }

  void RubyContinuousVariable_Impl::setArgument(const ruleset::OSArgument& argument) {
    m_argument = argument;
    onChange(AnalysisObject_Impl::InvalidatesResults);
  }

  bool RubyContinuousVariable_Impl::setRubyMeasure(const RubyMeasure& measure) {
    if (!fileTypesAreCompatible(m_measure,
                                measure.inputFileType(),
                                measure.outputFileType()))
    {
      return false;
    }
    disconnectChild(m_measure);
    m_measure = measure;
    connectChild(m_measure,true);
    onChange(AnalysisObject_Impl::InvalidatesResults);
    return true;
  }

  bool RubyContinuousVariable_Impl::fileTypesAreCompatible(
      const RubyMeasure& childRubyMeasure,
      const boost::optional<FileReferenceType>& proposedInputFileType,
      const boost::optional<FileReferenceType>& proposedOutputFileType) const
  {
    // RubyMeasure stands alone in this variable, so only need to check with problem.
    if (OptionalAnalysisObject parent = this->parent()) {
      if (!(parent->cast<WorkflowStep>().fileTypesAreCompatible(proposedInputFileType,
                                                                proposedOutputFileType)))
      {
        return false;
      }
    }
    return true;
  }

  QVariant RubyContinuousVariable_Impl::toVariant() const {
    QVariantMap variableData = ContinuousVariable_Impl::toVariant().toMap();

    variableData["variable_type"] = QString("RubyContinuousVariable");
    variableData["argument"] = ruleset::detail::toVariant(argument());

    return QVariant(variableData);
  }

  RubyContinuousVariable RubyContinuousVariable_Impl::fromVariant(const QVariant& variant,
                                                                  const VersionString& version)
  {
    QVariantMap map = variant.toMap();

    // dummy measure. need to do something better sometime (like make sure overload version of
    // this method is always called)
    RubyMeasure dummyMeasure(toPath("*.rb"),
                             FileReferenceType::Unknown,
                             FileReferenceType::Unknown);

    return RubyContinuousVariable(
          toUUID(map["uuid"].toString().toStdString()),
          toUUID(map["version_uuid"].toString().toStdString()),
          map.contains("name") ? map["name"].toString().toStdString() : std::string(),
          map.contains("display_name") ? map["display_name"].toString().toStdString() : std::string(),
          map.contains("description") ? map["description"].toString().toStdString() : std::string(),
          map.contains("uncertainty_description") ? analysis::detail::toUncertaintyDescription(map["uncertainty_description"],version) : OptionalUncertaintyDescription(),
          map.contains("minimum") ? map["minimum"].toDouble() : OptionalDouble(),
          map.contains("maximum") ? map["maximum"].toDouble() : OptionalDouble(),
          map.contains("increment") ? map["increment"].toDouble() : OptionalDouble(),
          map.contains("n_steps") ? map["n_steps"].toInt() : OptionalInt(),
          ruleset::detail::toOSArgument(map["argument"],version),
          dummyMeasure);
  }

  RubyContinuousVariable RubyContinuousVariable_Impl::fromVariant(const QVariant& variant,
                                                                  const Measure& measure,
                                                                  const VersionString& version)
  {
    QVariantMap map = variant.toMap();

    OS_ASSERT(measure.optionalCast<RubyMeasure>());

    return RubyContinuousVariable(
          toUUID(map["uuid"].toString().toStdString()),
          toUUID(map["version_uuid"].toString().toStdString()),
          map.contains("name") ? map["name"].toString().toStdString() : std::string(),
          map.contains("display_name") ? map["display_name"].toString().toStdString() : std::string(),
          map.contains("description") ? map["description"].toString().toStdString() : std::string(),
          map.contains("uncertainty_description") ? analysis::detail::toUncertaintyDescription(map["uncertainty_description"],version) : OptionalUncertaintyDescription(),
          map.contains("minimum") ? map["minimum"].toDouble() : OptionalDouble(),
          map.contains("maximum") ? map["maximum"].toDouble() : OptionalDouble(),
          map.contains("increment") ? map["increment"].toDouble() : OptionalDouble(),
          map.contains("n_steps") ? map["n_steps"].toInt() : OptionalInt(),
          ruleset::detail::toOSArgument(map["argument"],version),
          measure.cast<RubyMeasure>());
  }

  void RubyContinuousVariable_Impl::updateInputPathData(const openstudio::path& originalBase,
                                                        const openstudio::path& newBase)
  {
    m_measure.getImpl<detail::RubyMeasure_Impl>()->updateInputPathData(originalBase,newBase);
  }

} // detail

RubyContinuousVariable::RubyContinuousVariable(const std::string& name,
                                               const ruleset::UserScriptArgument& argument,
                                               const RubyMeasure& measure)
  : ContinuousVariable(std::shared_ptr<detail::RubyContinuousVariable_Impl>(
        new detail::RubyContinuousVariable_Impl(name,argument,measure)))
{
  RubyContinuousVariable copyOfThis(getImpl<detail::RubyContinuousVariable_Impl>());
  measure.setParent(copyOfThis);
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
                                               const RubyMeasure& measure)
  : ContinuousVariable(std::shared_ptr<detail::RubyContinuousVariable_Impl>(
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
                                                measure)))
{
  RubyContinuousVariable copyOfThis(getImpl<detail::RubyContinuousVariable_Impl>());
  measure.setParent(copyOfThis);
}

ruleset::UserScriptArgument RubyContinuousVariable::argument() const {
  return getImpl<detail::RubyContinuousVariable_Impl>()->argument();
}

RubyMeasure RubyContinuousVariable::measure() const {
  return getImpl<detail::RubyContinuousVariable_Impl>()->measure();
}

RubyMeasure RubyContinuousVariable::perturbation() const {
  return getImpl<detail::RubyContinuousVariable_Impl>()->measure();
}

void RubyContinuousVariable::setArgument(const ruleset::OSArgument& argument) {
  getImpl<detail::RubyContinuousVariable_Impl>()->setArgument(argument);
}

bool RubyContinuousVariable::setRubyMeasure(const RubyMeasure& measure) {
  return getImpl<detail::RubyContinuousVariable_Impl>()->setRubyMeasure(measure);
}

bool RubyContinuousVariable::setRubyPerturbation(const RubyMeasure& measure) {
  return getImpl<detail::RubyContinuousVariable_Impl>()->setRubyMeasure(measure);
}

/// @cond
RubyContinuousVariable::RubyContinuousVariable(std::shared_ptr<detail::RubyContinuousVariable_Impl> impl)
  : ContinuousVariable(impl)
{}

bool RubyContinuousVariable::fileTypesAreCompatible(
    const RubyMeasure& childRubyMeasure,
    const boost::optional<FileReferenceType>& proposedInputFileType,
    const boost::optional<FileReferenceType>& proposedOutputFileType) const
{
  return getImpl<detail::RubyContinuousVariable_Impl>()->fileTypesAreCompatible(childRubyMeasure,
                                                                                proposedInputFileType,
                                                                                proposedOutputFileType);
}
/// @endcond

} // analysis
} // openstudio
