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

#ifndef RULESET_CLAUSE_HPP
#define RULESET_CLAUSE_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/RulesetObject.hpp>

namespace openstudio {
namespace ruleset {

class FilterClause;

namespace detail {
  class Clause_Impl;
} // detail

/** Clause \link RulesetObject RulesetObjects \endlink are the atomic components of 
 *  \link Rule Rules \endlink */
class RULESET_API Clause : public RulesetObject {
 public:

  /// @cond
  typedef detail::Clause_Impl ImplType;
  /// @endcond

  /** @name Constructors and Destructors */
  //@{

  static boost::optional<Clause> factoryFromXml(const QDomElement& element);
  
  virtual ~Clause() {}

  //@}
 protected:
  /// @cond
  friend class detail::Clause_Impl;
  friend class RulesetObject;

  /** Protected constructor from impl. */
  Clause(boost::shared_ptr<detail::Clause_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("Ruleset.Clause");
};

/** \relates Clause */
typedef std::vector<Clause> ClauseVector;

/** \relates Clause */
typedef std::set<Clause> ClauseSet;

} // ruleset
} // openstudio

#endif // RULESET_CLAUSE_HPP
