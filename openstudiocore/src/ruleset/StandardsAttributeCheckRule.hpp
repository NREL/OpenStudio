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

#ifndef RULESET_STANDARDSATTRIBUTECHECKRULE_HPP
#define RULESET_STANDARDSATTRIBUTECHECKRULE_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/StandardsRule.hpp>

#include <utilities/core/Path.hpp>

namespace openstudio {
namespace ruleset {

class StandardsFilterRulesetOption;
class StandardsFilterObjectAttribute;
class StandardsActionRuleResult;

namespace detail {
  class StandardsAttributeCheckRule_Impl;
} // detail

/** A StandardsAttributeCheckRule is a StandardsRule that passes or fails based on the value of an
 *  object's attribute. StandardsAttributeCheckRule adds semantics and behavior, not data, to 
 *  StandardsRule. A StandardsAttributeCheckRule may filter based on 
 *  \link StandardsRulesetOption StandardsRulesetOptions \endlink and various \link 
 *  StandardsFilterObjectAttribute StandardsFilterObjectAttributes \endlink, with the restriction 
 *  that at most one standardsinterface::ObjectType not in the list 
 *  DataDictionary::objectTypesUniqueWithinOneModel() may be referenced. An object of this type 
 *  is essentially two rules in one, as both the StandardsActionRuleSatisfied and 
 *  StandardsActionRuleViolated rules will be built by the engine, with the appropriate predicate
 *  reversals accomplished by applying StandardsFilterObjectAttribute::negate() to 
 *  ruleResultAction().attributeTest(). */
class RULESET_API StandardsAttributeCheckRule : public StandardsRule {
 public:
  /// @cond
  typedef detail::StandardsAttributeCheckRule_Impl ImplType;

  static std::string xmlElementName();
  /// @endcond

  /** @name Constructors and Destructors */
  //@{

  /** Will throw if the ObjectType of any of the qualifyingFilters is not the same as 
   *  ruleResultAction.attributeTest().object().objectType(), or in the 
   *  DataDictionary::objectTypesUniqueWithinOneModel() list. All 
   *  StandardsFilterObjectAttributes must also refer to non-conflicting 
   *  StandardsFilterAvailableModels. */
  StandardsAttributeCheckRule(
      const std::vector<StandardsFilterRulesetOption>& optionFilters,
      const std::vector<StandardsFilterObjectAttribute>& qualifyingFilters,
      const StandardsActionRuleResult& ruleResultAction);

  /** Constructor provided for deserialization; not for general use. */
  StandardsAttributeCheckRule(const std::string& name, 
                              bool active, 
                              const UUID& uuid, 
                              const UUID& versionUUID,
                              const std::vector<FilterClause>& filters, 
                              const std::vector<ActionClause>& actions);

  /** Constructor provided for deserialization; not for general use. */
  StandardsAttributeCheckRule(const QDomElement& element);

  /** Returns companion to this rule, which is identical except for having the opposite 
   *  StandardsActionRuleResult, and a negated ruleResultAction().attributeTest(). */
  StandardsAttributeCheckRule createCompanionRule() const;

  //@}
  /** @name Getters */
  //@{

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
   *  aRule.ruleResultAction().attributeTest();
   *  \endcode
   *
   *  or
   *
   *  \code 
   *  aRule.ruleResultAction().referencedFilters()[0].cast<StandardsFilterObjectAttribute>();
   *  \endcode 
   */
  StandardsActionRuleResult ruleResultAction() const;

  //@}
  /** @name Setters */
  //@{

  void createDescriptiveName();

  //@}
 protected:
  /// @cond
  friend class detail::StandardsAttributeCheckRule_Impl;
  friend class detail::RulesetObject_Impl;
  friend class RulesetObject;

  /** Protected constructor from impl. */
  StandardsAttributeCheckRule(boost::shared_ptr<detail::StandardsAttributeCheckRule_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("Ruleset.StandardsAttributeCheckRule");
};

/** \relates StandardsAttributeCheckRule */
typedef boost::optional<StandardsAttributeCheckRule> OptionalStandardsAttributeCheckRule;

/** \relates StandardsAttributeCheckRule */
typedef std::vector<StandardsAttributeCheckRule> StandardsAttributeCheckRuleVector;

/** Load \link StandardsAttributeCheckRule StandardsAttributeCheckRules \endlink from csv file. 
 *  The format is (number of columns may vary, items in [] are optional):
 *
 *  \code
 *  [StandardsRulesetOption name_1, value_1,,...,]
 *  [StandardsRulesetOption name_2, value_2,,...,]
 *  [...]
 *  [StandardsRulesetOption name_n, value_n,,...,]
 *  "Rule Type Descriptor"        ,        ,,...,
 *                                ,        ,,...,
 *  ObjectType_1                  , ...    , ObjectType_n ,, ObjectTypeForPass_1
 *  Attribute_1                   , ...    , Attribute_n  ,, AttributeForPass_1
 *  [Predicate_1]                 , ...    , [Predicate_n],, [PredicateForPass_1]
 *  [Units_1]                     , ...    , [Units_n]    ,, [UnitsForPass_1]
 *  Value_11                      , ...    , Value_1n     ,, ValueForPass_11
 *  Value_21                      , ...    , Value_2n     ,, ValueForPass_21
 *  ...
 *  Value_m1                      , ...    , Value_mn     ,, ValueForPass_m1
 *  \endcode
 *
 *  If writeSI is true, then another csv file, with the same path and name as p, except that "_SI"
 *  is added as a suffix to the filename, is written out. This new file is identical to p, except 
 *  that all quantities (value, unit pairs) are converted to SI units if possible. It is these rules
 *  (the SI versions) that are actually returned by the function.
 *
 *  \relates StandardsAttributeCheckRule */
RULESET_API std::vector<StandardsAttributeCheckRule> loadStandardsAttributeCheckRulesFromFile(
    const openstudio::path& p,bool writeSI=false);

} // ruleset
} // openstudio

#endif // RULESET_STANDARDSATTRIBUTECHECKRULE_HPP
