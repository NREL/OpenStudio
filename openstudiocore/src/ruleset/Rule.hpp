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

#ifndef RULESET_RULE_HPP
#define RULESET_RULE_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/RulesetObject.hpp>
#include "FilterClause.hpp"
#include "ActionClause.hpp"

namespace openstudio {
namespace ruleset {

class FilterClause;
class ActionClause;

namespace detail {
  class Rule_Impl;
} // detail

/** A Rule RulesetObject consists of a number of \link FilterClause FilterClauses \endlink and 
 *  \link ActionClause ActionClauses \endlink. Clauses may be added and removed at will, and 
 *  retrieved by type (using RulesetObject::optionalCast). A Rule can be disabled, which means that
 *  it will never be activated when a containing Ruleset is applied to a model::Model. */
class RULESET_API Rule : public RulesetObject {
 public:

  /// @cond
  typedef detail::Rule_Impl ImplType;
  /// @endcond

  /** @name Constructors and Destructors */
  //@{

  /** Constructor provided for deserialization; not for general use. */
  static boost::optional<Rule> factoryFromXml(const QDomElement& element);
  
  virtual ~Rule() {}

  //@}
  /** @name Getters */
  //@{

  std::string name() const;

  bool active() const;

  std::vector<FilterClause> filters() const;

  /** Get all the filters that can be cast to type T. */
  template <typename T>
  std::vector<T> getFilters() const {
    std::vector<T> result;
    std::vector<FilterClause> candidates = filters();
    for (std::vector<FilterClause>::const_iterator it = candidates.begin(), 
         itEnd = candidates.end(); it != itEnd; ++it) 
    {
      boost::optional<T> oFilter = it->optionalCast<T>();
      if (oFilter) { result.push_back(*oFilter); }
    }
    return result;
  }

  std::vector<ActionClause> actions() const;

  /** Get all the actions that can be cast to type T. */
  template <typename T>
  std::vector<T> getActions() const {
    std::vector<T> result;
    std::vector<ActionClause> candidates = actions();
    for (std::vector<ActionClause>::const_iterator it = candidates.begin(), 
         itEnd = candidates.end(); it != itEnd; ++it) 
    {
      boost::optional<T> oAction = it->optionalCast<T>();
      if (oAction) { result.push_back(*oAction); }
    }
    return result;
  }

  //}@
  /** @name Setters */
  //@{

  void setName(const std::string& name);

  /** After this call rule.active() == true. */
  void activate();

  /** After this call rule.active() == false. */
  void disable();

  /** Add filter. May also add related clauses. */
  bool add(const FilterClause& filter);

  /** Add filters. May also add related clauses. */
  bool add(const std::vector<FilterClause>& filters);

  /** Add action. May also add related clauses. */
  bool add(const ActionClause& action);

  /** Remove filter. May also remove related clauses. */
  bool remove(const FilterClause& filter);

  /** Remove action. May also remove related clauses. */
  bool remove(const ActionClause& action);

  /** Remove all filters. After this call, rule.numFilters() == 0. */
  unsigned clearFilters();

  /** Remove all actions. After this call, rule.numActions() == 0. */
  unsigned clearActions();

  //@}
  /** @name Queries */
  //@{

  unsigned numFilters() const;

  unsigned numActions() const;

  /** Returns true if one of filters() == filter. Checks exact equality, meaning that the filters 
   *  have the sample underlying impl object. */
  bool contains(const FilterClause& filter) const;

  /** Returns true if there is a FilterClause in this rule for which filter.equivalentData returns
   *  true. */
  bool containsEquivalentData(const FilterClause& filter) const;

  /** Returns true if one of actions() == action. Checks exact equality, meaning that the actions 
   *  have the sample underlying impl object. */
  bool contains(const ActionClause& action) const;

  /** Returns true if there is an ActionClause in this rule for which action.equivalentData returns
   *  true. */
  bool containsEquivalentData(const ActionClause& action) const;

  //@}
 protected:
  /// @cond
  friend class RulesetObject;
  friend class detail::RulesetObject_Impl;
  friend class detail::Rule_Impl;

  /** Protected constructor from impl. */
  Rule(const boost::shared_ptr<detail::Rule_Impl>& impl);

  /// @endcond
 private:

  REGISTER_LOGGER("Ruleset.Rule");
};

/** \relates Rule */
typedef std::vector<Rule> RuleVector;

} // ruleset
} // openstudio

#endif // RULESET_RULE_HPP
