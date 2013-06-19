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

#ifndef RULESET_STANDARDSACTIONCLONEMODEL_HPP
#define RULESET_STANDARDSACTIONCLONEMODEL_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/StandardsActionClause.hpp>

#include <ruleset/StandardsRulesetEnums.hpp>

namespace openstudio {
namespace ruleset {

class StandardsFilterAvailableModel;

namespace detail {
  class StandardsActionCloneModel_Impl;
} // detail

/** A StandardsActionCloneModel StandardsActionClause specifies that a given model type is to 
 *  be cloned, and that the clone should be labeled with the StandardsRulesetModelType 
 *  modelTypeOfClone(). */
class RULESET_API StandardsActionCloneModel : public StandardsActionClause {
 public:
  /// @cond
  typedef detail::StandardsActionCloneModel_Impl ImplType;

  static std::string xmlElementName();
  /// @endcond

  /** @name Constructors and Destructors */
  //@{

  /** Will create a new model labeled as type modelTypeOfClone. */
  StandardsActionCloneModel(const StandardsFilterAvailableModel& modelToClone,
                            const StandardsRulesetModelType& modelTypeOfClone);

  /** Constructor provided for deserialization; not for general use. */
  StandardsActionCloneModel(const StandardsFilterAvailableModel& modelToClone,
                            const StandardsRulesetModelType& modelTypeOfClone,
                            const UUID& uuid,
                            const UUID& versionUUID);

  //@}
  /** @name Getters */
  //@{

  StandardsFilterAvailableModel modelToClone() const;

  StandardsRulesetModelType modelTypeOfClone() const;

  //@}
  /** @name Setters */
  //@{

  void setModelTypeOfClone(const StandardsRulesetModelType& value);

  //@}
 protected:
  /// @cond
  friend class detail::StandardsActionCloneModel_Impl;
  friend class RulesetObject;

  // Protected model setter for deepClone.
  void mf_setModelToClone(const StandardsFilterAvailableModel& filter);

  /** Protected constructor from impl. */
  StandardsActionCloneModel(boost::shared_ptr<detail::StandardsActionCloneModel_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("Ruleset.StandardsActionCloneModel");
};

/** \relates StandardsActionCloneModel */
typedef boost::optional<StandardsActionCloneModel> OptionalStandardsActionCloneModel;

/** \relates StandardsActionCloneModel */
typedef std::vector<StandardsActionCloneModel> StandardsActionCloneModelVector;

} // ruleset
} // openstudio

#endif // RULESET_STANDARDSACTIONCLONEMODEL_HPP
