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

#ifndef RULESET_FILTERCLAUSE_HPP
#define RULESET_FILTERCLAUSE_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/Clause.hpp>

namespace openstudio {
namespace ruleset {

namespace detail {
  class FilterClause_Impl;
} // detail

/** A FilterClause Clause is used to determine whether a rule is applicable, and if it is, to which
 *  exact data objects it is applicable. FilterClause is essentially abstract, as it does not have 
 *  a public constructor. */
class RULESET_API FilterClause : public Clause {
 public:

  /// @cond
  typedef detail::FilterClause_Impl ImplType;
  /// @endcond
  
  /** @name Constructors and Destructors */
  //@{
  
  virtual ~FilterClause() {}
  
  //@}  

 protected:
  /// @cond
  friend class detail::FilterClause_Impl;
  friend class RulesetObject;

  /** Protected constructor from impl. */
  FilterClause(boost::shared_ptr<detail::FilterClause_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("Ruleset.FilterClause");
};

/** \relates FilterClause */
typedef std::vector<FilterClause> FilterClauseVector;

} // ruleset
} // openstudio

#endif // RULESET_FILTERCLAUSE_HPP
