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

#ifndef RULESET_MODELOBJECTACTIONCLAUSE_HPP
#define RULESET_MODELOBJECTACTIONCLAUSE_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/ActionClause.hpp>

namespace openstudio {
namespace model {
  class ModelObject;
}

namespace ruleset {

namespace detail {
  class ModelObjectActionClause_Impl;
} // detail

/** A ModelObjectActionClause is an ActionClause that acts directly on model::ModelObjects, 
 *  typically in the context of a ModelRule. It is essentially an abstract class, as it has no 
 *  public constructors. */
class RULESET_API ModelObjectActionClause : public ActionClause {
 public:
  /// @cond
  typedef detail::ModelObjectActionClause_Impl ImplType;
  /// @endcond
  
  /** @name Constructors and Destructors */
  //@{
  
  virtual ~ModelObjectActionClause() {}
  
  //@}
  /** @name Actions */
  //@{

  /** Virtually implemented function that attempts to apply the action to modelObject, returning
   *  true on successful completion, and false otherwise. */
  bool apply(openstudio::model::ModelObject& modelObject) const;

  //@}
 protected:
  /// @cond
  friend class detail::ModelObjectActionClause_Impl;
  friend class RulesetObject;

  /** Protected constructor from impl. */
  ModelObjectActionClause(boost::shared_ptr<detail::ModelObjectActionClause_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("Ruleset.ModelObjectActionClause");
};

/** \relates ModelObjectActionClause */
typedef std::vector<ModelObjectActionClause> ModelObjectActionClauseVector;

} // ruleset
} // openstudio

#endif // RULESET_MODELOBJECTACTIONCLAUSE_HPP
