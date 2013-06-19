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

#ifndef RULESET_STANDARDSACTIONCLAUSE_HPP
#define RULESET_STANDARDSACTIONCLAUSE_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/ActionClause.hpp>

namespace openstudio {
namespace ruleset {

class StandardsRule;
class StandardsFilterClause;

namespace detail {
  class StandardsActionClause_Impl;
} // detail

/** A StandardsActionClause is an ActionClause for use in \link StandardsRule StandardsRules 
 *  \endlink. \link StandardsActionClause StandardsActionClauses \endlink typically reference one 
 *  or more \link StandardsFilterClause StandardsFilterClasues \endlink, which specify the data
 *  relevant to or modified by the action. */
class RULESET_API StandardsActionClause : public ActionClause {
 public:
  /// @cond
  typedef detail::StandardsActionClause_Impl ImplType;
  /// @endcond

  /** @name Constructors and Destructors */
  //@{

  /** Creates a clone of this object with disconnected data. Related objects are retained as-is 
   *  (data shared with this object). */
  RulesetObject clone() const;

  /** Creates a clone of this object with deeply disconnected data. Related objects are also 
   *  cloned. */
  RulesetObject deepClone() const;

  //@}
  /** @name Getters */
  //@{

  /** \link StandardsFilterClause StandardsFilterClauses \endlink used to construct this Clause, and/or define
   *  variables used by this Clause. */
  std::vector<StandardsFilterClause> referencedFilters() const;

  /** \link StandardsFilterClause StandardsFilterClauses \endlink implied by this Clause, that is referenced, and
   *  needed by CLIPS, but not necessary for the user to specify. These clauses are generated and
   *  printed on the fly as needed. */
  std::vector<StandardsFilterClause> impliedFilters() const;

  //@}
  /** @name Setters */
  //@{

  /** Any referenced filters whose UUIDs match preferredInstances[i] are to be removed and 
   *  replaced with preferredInstances[i]. This method assists in reconstruction of \link 
   *  StandardsRule StandardsRules \endlink stored in a project::ProjectDatabase. */
  void replaceReferencedFilters(const std::vector<StandardsFilterClause>& preferredInstances);

  //@}
  /** @name Serialization */
  //@{

  /** Print as CLIPS statement. */
  std::string print() const;

  /** Print as CLIPS statement, use information in rule (especially name) as desired. */
  std::string print(const StandardsRule& rule) const;

  //@}
 protected:
  /// @cond
  friend class detail::StandardsActionClause_Impl;
  friend class RulesetObject;

  /** Protected constructor from impl. */
  StandardsActionClause(boost::shared_ptr<detail::StandardsActionClause_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("Ruleset.StandardsActionClause");
};

/** \relates StandardsActionClause */
typedef std::vector<StandardsActionClause> StandardsActionClauseVector;

} // ruleset
} // openstudio

#endif // RULESET_STANDARDSACTIONCLAUSE_HPP
