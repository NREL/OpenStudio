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

#ifndef RULESET_STANDARDSATTRIBUTECHECKRULE_IMPL_HPP
#define RULESET_STANDARDSATTRIBUTECHECKRULE_IMPL_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/StandardsRule_Impl.hpp>

namespace openstudio {
namespace ruleset {

class StandardsAttributeCheckRule;
class StandardsFilterRulesetOption;
class StandardsActionRuleResult;

namespace detail {

  class RULESET_API StandardsAttributeCheckRule_Impl : public StandardsRule_Impl {
    Q_OBJECT;
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Will throw if the ObjectType of any of the qualifyingFilters is not the same as 
     *  ruleResultAction.attributeTest().object().objectType(), or in the 
     *  DataDictionary::objectTypesUniqueWithinOneModel() list. All 
     *  StandardsFilterObjectAttributes must also refer to non-conflicting 
     *  StandardsFilterAvailableModels. */
    StandardsAttributeCheckRule_Impl(
      const std::vector<StandardsFilterRulesetOption>& optionFilters,
      const std::vector<StandardsFilterObjectAttribute>& qualifyingFilters,
      const StandardsActionRuleResult& ruleResultAction);

    StandardsAttributeCheckRule_Impl(const std::string& name, 
                                     bool active, 
                                     const UUID& uuid, 
                                     const UUID& versionUUID,
                                     const std::vector<FilterClause>& filters, 
                                     const std::vector<ActionClause>& actions);

    StandardsAttributeCheckRule_Impl(const QDomElement& element);

    /** Returns companion to this rule, which is identical except for oppositite 
     *  StandardsActionRuleResult, and negated ruleResultAction().attributeTest(). */
    StandardsAttributeCheckRule createCompanionRule() const;

    //@}
    /** @name Getters */
    //@{

    virtual std::vector<StandardsRule> impliedRules() const;

    /** Returns the ObjectType associated with ruleResultAction().attributeTest(). */
    standardsinterface::ObjectType objectType() const;

    /** Returns all of the \link StandardsFilterObjectAttribute StandardsFilterObjectAttributes 
     *  \endlink not associated with ruleResultAction(), ordered by 
     *  DataDictionary::objectTypesUniqueWithinOneModel(), then primaryObjectType(). */
    std::vector<StandardsFilterObjectAttribute> qualifyingFilters() const;

    /** Returns the unique StandardsActionRuleResult in this rule. The associated filter may be 
     *  accessed by 
     *
     *  \code 
     *  aRule.ruleResultAction().referencedFilters()[0].cast<StandardsFilterObjectAttribute>();
     *  \endcode 
     */
    StandardsActionRuleResult ruleResultAction() const;

    //@}
    /** @name Setters */
    //@{

    virtual bool add(const FilterClause& filter);

    virtual bool add(const ActionClause& action);

    virtual bool remove(const FilterClause& filter);

    virtual bool remove(const ActionClause& action);

    virtual unsigned clearFilters();

    virtual unsigned clearActions();

    void createDescriptiveName();

    //@}
    /** @name Serialization */
    //@{

    virtual std::string xmlElementName() const;

    virtual void writeValues(QDomDocument& doc, QDomElement& element) const;

    //@}
    /** @name Queries */
    //@{

    /** Equality as determined by deep comparison of data members. */
    virtual bool equivalentData(const RulesetObject& other) const;

    //@}
   protected:
    mutable std::vector<standardsinterface::ObjectType> m_objectTypes; // lazy cache

   private:
    REGISTER_LOGGER("Ruleset.StandardsAttributeCheckRule");

    bool mf_consistentModel(const StandardsFilterObjectAttribute& attributeFilter) const;

    const std::vector<standardsinterface::ObjectType>& mf_objectTypes() const;
  };

} // detail

} // ruleset
} // openstudio

#endif // RULESET_STANDARDSATTRIBUTECHECKRULE_IMPL_HPP
