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

#ifndef RULESET_ACTIONCLAUSE_HPP
#define RULESET_ACTIONCLAUSE_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/Clause.hpp>

class QDomElement;

namespace openstudio {
namespace ruleset {

namespace detail {
  class ActionClause_Impl;
} // detail

/** An ActionClause Clause performs some action if it is incorporated into a Rule whose 
 *  \link FilterClause FilterClauses \endlink are all matched by a given data instance. 
 *  ActionClause is essentially abstract, as it does not have a public constructor. */
class RULESET_API ActionClause : public Clause {
 public:
  /// @cond
  typedef detail::ActionClause_Impl ImplType;
  /// @endcond
  
  /** @name Constructors and Destructors */
  //@{
  
  virtual ~ActionClause() {}
  
  //@}
 protected:
  /// @cond
  friend class detail::ActionClause_Impl;
  friend class RulesetObject;

  /** Protected constructor from impl. */
  ActionClause(boost::shared_ptr<detail::ActionClause_Impl> impl);
  
  /// @endcond
 private:
  REGISTER_LOGGER("Ruleset.ActionClause");
};

/** \relates ActionClause */
typedef std::vector<ActionClause> ActionClauseVector;

} // ruleset
} // openstudio

#endif // RULESET_ACTIONCLAUSE_HPP
