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

#ifndef RULESET_STANDARDSFILTERCLAUSE_HPP
#define RULESET_STANDARDSFILTERCLAUSE_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/FilterClause.hpp>

namespace openstudio {
namespace ruleset {

namespace detail {
  class StandardsFilterClause_Impl;
} // detail

/** A StandardsFilterClause is a FilterClause that may be used in \link StandardsRule 
 *  StandardsRules \endlink. \link StandardsFilterClause StandardsFilterClauses \endlink may refer 
 *  to other StandardsFilterClauses, and may also imply StandardsFilterClauses. Such relationships
 *  are defined by classes derived from StandardsFilterClause and are not directly settable. */
class RULESET_API StandardsFilterClause : public FilterClause {
 public:
  /// @cond
  typedef detail::StandardsFilterClause_Impl ImplType;
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

  /** \link StandardsFilterClause StandardsFilterClauses \endlink used to construct this Clause, 
   *  and/or define variables used by this Clause. */
  std::vector<StandardsFilterClause> referencedFilters() const;

  /** \link StandardsFilterClause StandardsFilterClauses \endlink implied by this Clause, that is, 
   *  referenced and needed by CLIPS, but not necessary for the user to specify. These clauses are 
   *  generated and printed on the fly as needed. */
  std::vector<StandardsFilterClause> impliedFilters() const;

  /** Get the factName associated with this filter. Contains a UUID to ensure uniqueness across 
   *  a CLIPS rule construct. */
  std::string factName() const;

  //@}
  /** @name Setters */
  //@{

  /** Any referenced filters whose UUIDs match preferredInstances[i] are to be removed and 
   *  replaced with preferredInstances[i]. This method assists in reconstruction of \link 
   *  StandardsRule StandardsRules \endlink stored in a project::ProjectDatabase, and is not
   *  for general use. */
  void replaceReferencedFilters(std::vector<StandardsFilterClause>& preferredInstances);

  //@}
  /** @name Serialization */
  //@{

  /** Print as CLIPS statement. */
  std::string print() const;

  /** Prints the retract action clause for the fact associated with this filter; not for general 
   *  use. */
  std::string printRetractAction() const;

  //@}
 protected:
  /// @cond
  friend class detail::StandardsFilterClause_Impl;
  friend class RulesetObject;

  /** Protected constructor from impl. */
  StandardsFilterClause(boost::shared_ptr<detail::StandardsFilterClause_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("Ruleset.StandardsFilterClause");
};

/** \relates StandardsFilterClause */
typedef boost::optional<StandardsFilterClause> OptionalStandardsFilterClause;

/** \relates StandardsFilterClause */
typedef std::vector<StandardsFilterClause> StandardsFilterClauseVector;

} // ruleset
} // openstudio

#endif // RULESET_STANDARDSFILTERCLAUSE_HPP
