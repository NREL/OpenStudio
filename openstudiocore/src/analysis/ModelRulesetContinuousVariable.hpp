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

#ifndef ANALYSIS_MODELRULESETCONTINUOUSVARIABLE_HPP
#define ANALYSIS_MODELRULESETCONTINUOUSVARIABLE_HPP

#include <analysis/AnalysisAPI.hpp>
#include <analysis/ContinuousVariable.hpp>

namespace openstudio {

namespace ruleset {
  class ModelObjectFilterClause;
  class ModelRuleset;
}

namespace analysis {

class DiscreteVariable;

namespace detail {

  class ModelRulesetContinuousVariable_Impl;

} // detail

/** ModelRulesetContinuousVariable is a ContinuousVariable that is set by setting attributeName
 *  for all model::ModelObjects that pass the ruleset::ModelObjectFilterClauses. The variable
 *  value can also be retrieved from a model::Model, assuming that each model::ModelObject that
 *  passes the filter has the same value for attributeName. Therefore,
 *  ModelRulesetContinuousVariable can be used in a Problem, and also in \link Function
 *  Functions \endlink (the latter requires getValue to work in all circumstances).
 *  ModelRulesetContinuousVariable can also be discretized to create a DiscreteVariable. */
class ANALYSIS_API ModelRulesetContinuousVariable : public ContinuousVariable {
 public:
  /** @name Constructors and Destructors */
  //@{

  ModelRulesetContinuousVariable(const std::string& name,
                                 std::vector<ruleset::ModelObjectFilterClause> modelObjectFilters,
                                 const std::string& attributeName);

  /** Constructor provided for deserialization; not for general use. */
  ModelRulesetContinuousVariable(const UUID& uuid,
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
                                 boost::optional<int> nSteps);

  virtual ~ModelRulesetContinuousVariable() {}

  //@}
  /** @name Getters and Queries */
  //@{

  /** Returns attibuteName that is applied to model::ModelObjects passing this variable's
   *  ruleset::ModelObjectFilterClauses. */
  std::string attributeName() const;

  //@}
  /** @name Actions */
  //@{

  /** One rule ruleset consisting of ruleset::ModelObjectFilterClauses used to construct the
   *  variable, plus action setting attributeName() to value, if value != boost::none. */
  ruleset::ModelRuleset getModelRuleset(boost::optional<double> value) const;

  /** Create a DiscreteVariable by creating a ModelRulesetDiscretePerturbation from
   *  getModelRuleset called for each value in incrementalValues(). If includeNullPerturbation,
   *  the first perturbation in the returned variable will be a NullPerturbation(). */
  DiscreteVariable discretize(bool includeNullPerturbation=false) const;

  /** Create a DiscreteVariable by creating a ModelRulesetDiscretePerturbation from
   *  getModelRuleset called for each value in values. If includeNullPerturbation, the first
   *  perturbation in the returned variable will be a NullPerturbation(). */
  DiscreteVariable discretize(const std::vector<double>& values,
                              bool includeNullPerturbation=false) const;

  //@}
 protected:
  /// @cond
  typedef detail::ModelRulesetContinuousVariable_Impl ImplType;

  friend class AnalysisObject;
  friend class detail::AnalysisObject_Impl;
  friend class detail::ModelRulesetContinuousVariable_Impl;

  explicit ModelRulesetContinuousVariable(boost::shared_ptr<detail::ModelRulesetContinuousVariable_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.analysis.ModelRulesetContinuousVariable");
};

/** \relates ModelRulesetContinuousVariable*/
typedef boost::optional<ModelRulesetContinuousVariable> OptionalModelRulesetContinuousVariable;

/** \relates ModelRulesetContinuousVariable*/
typedef std::vector<ModelRulesetContinuousVariable> ModelRulesetContinuousVariableVector;

} // analysis
} // openstudio

#endif // ANALYSIS_MODELRULESETCONTINUOUSVARIABLE_HPP

