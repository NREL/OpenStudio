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

#include <analysis/ModelRulesetContinuousVariable.hpp>
#include <analysis/ModelRulesetContinuousVariable_Impl.hpp>

#include <analysis/DataPoint.hpp>
#include <analysis/DiscreteVariable.hpp>
#include <analysis/ModelRulesetPerturbation.hpp>
#include <analysis/NullPerturbation.hpp>
#include <analysis/Problem.hpp>

#include <ruleset/ModelRuleset.hpp>
#include <ruleset/ModelRule.hpp>
#include <ruleset/ModelObjectFilterClause.hpp>
#include <ruleset/ModelObjectFilterClause_Impl.hpp>
#include <ruleset/ModelObjectFilterType.hpp>
#include <ruleset/ModelObjectActionSetAttribute.hpp>

#include <runmanager/lib/WorkItem.hpp>

#include <model/Model.hpp>

#include <utilities/math/FloatCompare.hpp>
#include <utilities/data/Attribute.hpp>
#include <utilities/core/Assert.hpp>
#include <utilities/core/FileReference.hpp>

#include <utilities/core/Containers.hpp>

namespace openstudio {
namespace analysis {

namespace detail {

  ModelRulesetContinuousVariable_Impl::ModelRulesetContinuousVariable_Impl(
      const std::string& name,
      std::vector<ruleset::ModelObjectFilterClause> modelObjectFilters,
      const std::string& attributeName)
    : ContinuousVariable_Impl(name),
      m_modelObjectFilters(modelObjectFilters),
      m_attributeName(attributeName)
  {}

  ModelRulesetContinuousVariable_Impl::ModelRulesetContinuousVariable_Impl(
      const UUID& uuid,
      const UUID& versionUUID,
      const std::string& name,
      const std::string& displayName,
      const std::string& description,
      const boost::optional<UncertaintyDescription>& udesc,
      std::vector<ruleset::ModelObjectFilterClause> modelObjectFilters,
      const std::string& attributeName,
      boost::optional<double> minimum,
      boost::optional<double> maximum,
      boost::optional<double> increment,
      boost::optional<int> nSteps)
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
      m_modelObjectFilters(modelObjectFilters),
      m_attributeName(attributeName)
  {}

  ModelRulesetContinuousVariable_Impl::ModelRulesetContinuousVariable_Impl(
      const ModelRulesetContinuousVariable_Impl &other)
    : ContinuousVariable_Impl(other),
      m_modelObjectFilters(other.m_modelObjectFilters),
      m_attributeName(other.attributeName())
  {
    // TODO: Clone the filters.
  }

  AnalysisObject ModelRulesetContinuousVariable_Impl::clone() const {
    boost::shared_ptr<ModelRulesetContinuousVariable_Impl> impl(new ModelRulesetContinuousVariable_Impl(*this));
    return ModelRulesetContinuousVariable(impl);
  }

  double ModelRulesetContinuousVariable_Impl::getValue(const DataPoint& dataPoint) const {
    boost::optional<double> result;
    if (model::OptionalModel oModel = dataPoint.model()) {
      model::Model model = *oModel;
      model::ModelObjectVector objects = ruleset::applyTypeFilters(model,m_modelObjectFilters);
      LOG(Debug,"Checking " << objects.size() << " of " << model.numObjects()
          << " ModelObjects in attempt to evaluate value of " << m_attributeName << ".");
      model::ModelObjectVector::iterator it = objects.begin();
      while (it != objects.end()) {
        bool erased = false;
        model::ModelObject object = *it;
        BOOST_FOREACH(const ruleset::ModelObjectFilterClause& filter, m_modelObjectFilters) {
          if (!filter.check(object)) {
            it = objects.erase(it);
            erased = true;
            break;
          }
        }
        if (!erased) {
          Attribute attribute = object.getAttribute(m_attributeName).get();
          double thisValue = attribute.valueAsDouble();
          if (!result) {
            result = thisValue;
          }
          else {
            if (!equal(*result,thisValue)) {
              LOG_AND_THROW("ModelRulesetContinuousVariable '" << name()
                            << "' has an ambiguous value.");
            }
          }
          ++it;
        }
      }
    }
    else {
      OptionalInt index = dataPoint.problem().getVariableIndexByUUID(uuid());
      OS_ASSERT(index);
      return dataPoint.variableValues()[*index].toDouble();
    }
    return result.get();
  }

  boost::optional<FileReferenceType> ModelRulesetContinuousVariable_Impl::inputFileType() const {
    return FileReferenceType(FileReferenceType::OSM);
  }

  boost::optional<FileReferenceType> ModelRulesetContinuousVariable_Impl::outputFileType() const {
    return FileReferenceType(FileReferenceType::OSM);
  }

  bool ModelRulesetContinuousVariable_Impl::isValid(const QVariant& value) const {
    if (value.type() == QVariant::Double) {
      return isFeasible(value.toDouble());
    }
    return false;
  }

  bool ModelRulesetContinuousVariable_Impl::isValid(const UncertaintyDescription& udesc) const {
    UncertaintyDescriptionTypeVector validTypes = UncertaintyDescription::validTypes(VariableValueType::Continuous);
    return (std::find(validTypes.begin(),validTypes.end(),udesc.type()) != validTypes.end());
  }

  runmanager::WorkItem ModelRulesetContinuousVariable_Impl::createWorkItem(
      const QVariant& value,
      const openstudio::path& rubyIncludeDirectory) const
  {
    OS_ASSERT(value.type() == QVariant::Double);
    ruleset::ModelRuleset ruleset = getModelRuleset(value.toDouble());
    return runmanager::WorkItem(ruleset);
  }

  std::string ModelRulesetContinuousVariable_Impl::attributeName() const {
    return m_attributeName;
  }

  ruleset::ModelRuleset ModelRulesetContinuousVariable_Impl::getModelRuleset(
      boost::optional<double> value) const
  {
    std::stringstream ss;
    ss << name();
    if (value) {
      ss << " " << *value;
    }
    LOG(Trace,"Constructing rule for " << name() << "'s getModelRuleset.")
    ruleset::ModelRule rule(ss.str());
    LOG(Trace,"Casting model object filters to FilterClause and adding to rule.")
    rule.add(castVector<ruleset::FilterClause>(m_modelObjectFilters));
    if (value) {
      LOG(Trace,"Initialized value, so adding set attribute action.");
      rule.add(ruleset::ModelObjectActionSetAttribute(m_attributeName,*value));
      LOG(Debug,"Constructing ruleset for " << name() << " with value " << *value << ".");
    }
    LOG(Trace,"Constructing one-rule ruleset.");
    ruleset::ModelRuleset result(rule.name());
    result.add(rule);
    return result;
  }

  DiscreteVariable ModelRulesetContinuousVariable_Impl::discretize(
      bool includeNullPerturbation) const
  {
    return discretize(incrementalValues(),includeNullPerturbation);
  }

  DiscreteVariable ModelRulesetContinuousVariable_Impl::discretize(
      const std::vector<double>& values,
      bool includeNullPerturbation) const
  {
    DiscretePerturbationVector perturbations;
    if (includeNullPerturbation) {
      perturbations.push_back(NullPerturbation());
    }
    BOOST_FOREACH(double value,values) {
      ruleset::ModelRuleset ruleset = getModelRuleset(value);
      perturbations.push_back(ModelRulesetPerturbation(ruleset));
    }
    return DiscreteVariable(name(),perturbations);
  }

} // detail

ModelRulesetContinuousVariable::ModelRulesetContinuousVariable(
    const std::string& name,
    std::vector<ruleset::ModelObjectFilterClause> modelObjectFilters,
    const std::string& attributeName)
  : ContinuousVariable(boost::shared_ptr<detail::ModelRulesetContinuousVariable_Impl>(
        new detail::ModelRulesetContinuousVariable_Impl(name,modelObjectFilters,attributeName)))
{}

ModelRulesetContinuousVariable::ModelRulesetContinuousVariable(
    const UUID& uuid,
    const UUID& versionUUID,
    const std::string& name,
    const std::string& displayName,
    const std::string& description,
    const boost::optional<UncertaintyDescription>& udesc,
    std::vector<ruleset::ModelObjectFilterClause> modelObjectFilters,
    const std::string& attributeName,
    boost::optional<double> minimum,
    boost::optional<double> maximum,
    boost::optional<double> increment,
    boost::optional<int> nSteps)
  : ContinuousVariable(boost::shared_ptr<detail::ModelRulesetContinuousVariable_Impl>(
        new detail::ModelRulesetContinuousVariable_Impl(uuid,
                                                        versionUUID,
                                                        name,
                                                        displayName,
                                                        description,
                                                        udesc,
                                                        modelObjectFilters,
                                                        attributeName,
                                                        minimum,
                                                        maximum,
                                                        increment,
                                                        nSteps)))
{}

std::string ModelRulesetContinuousVariable::attributeName() const {
  return getImpl<detail::ModelRulesetContinuousVariable_Impl>()->attributeName();
}

ruleset::ModelRuleset ModelRulesetContinuousVariable::getModelRuleset(
      boost::optional<double> value) const
{
  return getImpl<detail::ModelRulesetContinuousVariable_Impl>()->getModelRuleset(value);
}

DiscreteVariable ModelRulesetContinuousVariable::discretize(bool includeNullPerturbation) const {
  return getImpl<detail::ModelRulesetContinuousVariable_Impl>()->discretize(includeNullPerturbation);
}

DiscreteVariable ModelRulesetContinuousVariable::discretize(const std::vector<double>& values,
                            bool includeNullPerturbation) const
{
  return getImpl<detail::ModelRulesetContinuousVariable_Impl>()->discretize(values,includeNullPerturbation);
}

/// @cond
ModelRulesetContinuousVariable::ModelRulesetContinuousVariable(boost::shared_ptr<detail::ModelRulesetContinuousVariable_Impl> impl)
  : ContinuousVariable(impl)
{}
/// @endcond

} // analysis
} // openstudio

