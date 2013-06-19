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

#ifndef RULESET_STANDARDSRULE_HPP
#define RULESET_STANDARDSRULE_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/Rule.hpp>

#include <standardsinterface/DataDictionary.hpp>

namespace openstudio {
namespace ruleset {

class StandardsRulesetPrintMode;
class StandardsFilterObjectAttribute;
class StandardsActionRuleResult;

namespace detail {
  class StandardsRule_Impl;
} // detail

/** A StandardsRule is a Rule written using \link StandardsFilterClause StandardsFilterClauses 
 *  \endlink and \link StandardsActionClause StandardsActionClauses \endlink. \link StandardsRule
 *  StandardsRules \endlink are collected into \link StandardsRuleset StandardsRulesets \endlink,
 *  which can be applied to building energy models using the ruleset::RulesEngine. */
class RULESET_API StandardsRule : public Rule {
 public:
  /// @cond
  typedef detail::StandardsRule_Impl ImplType;

  static std::string xmlElementName();
  /// @endcond 

  /** @name Constructors and Destructors */
  //@{

  StandardsRule(const std::string& name);

  /** Constructor provided for deserialization; not for general use. */
  StandardsRule(const std::string& name, 
                bool active, 
                const UUID& uuid, 
                const UUID& versionUUID,
                const std::vector<FilterClause>& filters, 
                const std::vector<ActionClause>& actions);

  /** Constructor provided for deserialization; not for general use. */
  StandardsRule(const QDomElement& element);
  
  virtual ~StandardsRule() {}

  //@}
  /** @name Getters */
  //@{

  /** Returns rules needed by the ruleset::RulesEngine to be able to properly evaluate this 
   *  StandardsRule. Implied rules take care of basic mechanics like retrieving model objects 
   *  and attribute values. */
  std::vector<StandardsRule> impliedRules() const;

  //@}
  /** @name Serialization */
  //@{

  /** Print as CLIPS rule. If mode == StandardsRulesetPrintMode::Engine, implied 
   *  clauses not accessible through filters() and actions() may be printed. */
  std::string print(const StandardsRulesetPrintMode& mode) const;

  //@}
  /** @name Queries */
  //@{

  /** Same as containsEquivalentData except that the attribute values do not have to match. */
  bool containsEquivalentAttributeType(const StandardsFilterObjectAttribute& attributeFilter) const;

  /** Equivalent to constructing a StandardsFilterObjectBooleanAttribute, then calling 
   *  containsEquivalentAttributeType with that filter as the argument. Care is taken to try to 
   *  construct the test filter using an appropriate StandardsFilterAvailableModel. */
  bool containsEquivalentAttributeType(const standardsinterface::ObjectType& objectType,
                                       const standardsinterface::BooleanAttribute& attribute) const;

  /** Equivalent to constructing a StandardsFilterObjectDoubleAttribute, then calling 
   *  containsEquivalentAttributeType with that filter as the argument. Care is taken to try to 
   *  construct the test filter using an appropriate StandardsFilterAvailableModel. */
  bool containsEquivalentAttributeType(const standardsinterface::ObjectType& objectType,
                                       const standardsinterface::DoubleAttribute& attribute) const;

  /** Equivalent to constructing a StandardsFilterObjectEnumAttribute, then calling 
   *  containsEquivalentAttributeType with that filter as the argument. Care is taken to try to 
   *  construct the test filter using an appropriate StandardsFilterAvailableModel. */
  bool containsEquivalentAttributeType(const standardsinterface::ObjectType& objectType,
                                       const standardsinterface::EnumAttribute& attribute) const;

  /** Equivalent to constructing a StandardsFilterObjectIntegerAttribute, then calling 
   *  containsEquivalentAttributeType with that filter as the argument. Care is taken to try to 
   *  construct the test filter using an appropriate StandardsFilterAvailableModel. */
  bool containsEquivalentAttributeType(const standardsinterface::ObjectType& objectType,
                                       const standardsinterface::IntegerAttribute& attribute) const;

  //@}
 protected:
  /// @cond
  friend class detail::StandardsRule_Impl;
  friend class detail::RulesetObject_Impl;
  friend class RulesetObject;

  /** Protected constructor from impl. */
  StandardsRule(boost::shared_ptr<detail::StandardsRule_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("Ruleset.StandardsRule");
};

/** \relates StandardsRule */
typedef boost::optional<StandardsRule> OptionalStandardsRule;

/** \relates StandardsRule */
typedef std::vector<StandardsRule> StandardsRuleVector;

} // ruleset
} // openstudio

#endif // RULESET_STANDARDSRULE_HPP
