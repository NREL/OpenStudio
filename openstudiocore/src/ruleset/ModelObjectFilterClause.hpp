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

#ifndef RULESET_MODELOBJECTFILTERCLAUSE_HPP
#define RULESET_MODELOBJECTFILTERCLAUSE_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/FilterClause.hpp>

namespace openstudio {
namespace model {
  class ModelObject;
}

namespace ruleset {

namespace detail {
  class ModelObjectFilterClause_Impl;
} // detail

/** ModelObjectFilterClause is a FilterClause that acts on model::ModelObjects. It is essentially
 *  an abstract object, as it has no public constructors. */
class RULESET_API ModelObjectFilterClause : public FilterClause {
 public:

  /// @cond
  typedef detail::ModelObjectFilterClause_Impl ImplType;
  /// @endcond
  
  /** @name Constructors and Destructors */
  //@{
  
  virtual ~ModelObjectFilterClause() {}
  
  //@}
  /** @name Actions */
  //@{

  /** Virtually implemented method that determines whether modelObject meets the criteria of this
   *  ModelObjectFilterClause. */
  bool check(openstudio::model::ModelObject& modelObject) const;

  //@}
 protected:
  /// @cond
  friend class detail::ModelObjectFilterClause_Impl;
  friend class RulesetObject;

  /** Protected constructor from impl. */
  ModelObjectFilterClause(boost::shared_ptr<detail::ModelObjectFilterClause_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("Ruleset.ModelObjectFilterClause");
};

/** \relates ModelObjectFilterClause */
typedef std::vector<ModelObjectFilterClause> ModelObjectFilterClauseVector;

} // ruleset
} // openstudio

#endif // RULESET_MODELOBJECTFILTERCLAUSE_HPP
