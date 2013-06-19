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

#ifndef ANALYSIS_MODELRULESETCONTINUOUSVARIABLE_IMPL_HPP
#define ANALYSIS_MODELRULESETCONTINUOUSVARIABLE_IMPL_HPP

#include <analysis/AnalysisAPI.hpp>
#include <analysis/ContinuousVariable_Impl.hpp>

#include <ruleset/ModelObjectFilterClause.hpp>

namespace openstudio {
namespace ruleset {
  class ModelRuleset;
}
namespace analysis {

class ModelRulesetContinuousVariable;
class DiscreteVariable;

namespace detail {

  /** ModelRulesetContinuousVariable_Impl is a ContinuousVariable_Impl that is the implementation class for ModelRulesetContinuousVariable.*/
  class ANALYSIS_API ModelRulesetContinuousVariable_Impl : public ContinuousVariable_Impl {
    Q_OBJECT;
   public:
    /** @name Constructors and Destructors */
    //@{

    ModelRulesetContinuousVariable_Impl(
        const std::string& name,
        std::vector<ruleset::ModelObjectFilterClause> modelObjectFilters,
        const std::string& attributeName);

    /** Constructor provided for deserialization; not for general use. */
    ModelRulesetContinuousVariable_Impl(
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
        boost::optional<int> nSteps);

    ModelRulesetContinuousVariable_Impl(const ModelRulesetContinuousVariable_Impl& other);

    virtual AnalysisObject clone() const;

    virtual ~ModelRulesetContinuousVariable_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    /** Get the variable value from a dataPoint. Throws an openstudio::Exception if the Variable
     *  cannot be evaluated for dataPoint. */
    virtual double getValue(const DataPoint& dataPoint) const;

    virtual boost::optional<FileReferenceType> inputFileType() const;

    virtual boost::optional<FileReferenceType> outputFileType() const;

    /** Returns true if value is valid for this Variable. If returns false, createJob(value) should
     *  throw. */
    virtual bool isValid(const QVariant& value) const;

    /** Returns true if udesc is valid for this Variable. If returns false,
     *  setUncertaintyDescription will fail. */
    virtual bool isValid(const UncertaintyDescription& udesc) const;

    virtual runmanager::WorkItem createWorkItem(const QVariant& value,
                                                const openstudio::path& rubyIncludeDirectory) const;

    //@}
    /** @name Getters and Queries */
    //@{

    std::string attributeName() const;

    //@}
    /** @name Actions */
    //@{

    ruleset::ModelRuleset getModelRuleset(boost::optional<double> value) const;

    /** Create a DiscreteVariable by instantiating this Variable for each value in
     *  incrementalValues(). */
    DiscreteVariable discretize(bool includeNullPerturbation) const;

    /** Create a DiscreteVariable by instantiating this Variable for each value in values. */
    DiscreteVariable discretize(const std::vector<double>& values,
                                bool includeNullPerturbation) const;

    //@}
   private:
    REGISTER_LOGGER("openstudio.analysis.ModelRulesetContinuousVariable");

    std::vector<ruleset::ModelObjectFilterClause> m_modelObjectFilters;
    std::string m_attributeName;
  };

} // detail

} // model
} // openstudio

#endif // ANALYSIS_MODELRULESETCONTINUOUSVARIABLE_IMPL_HPP

