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

#ifndef RULESET_STANDARDSFILTERAVAILABLEMODEL_HPP
#define RULESET_STANDARDSFILTERAVAILABLEMODEL_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/StandardsFilterClause.hpp>

#include <ruleset/StandardsRulesetEnums.hpp>

namespace openstudio {
namespace ruleset {

namespace detail {
  class StandardsFilterAvailableModel_Impl;
} // detail

/** A StandardsFilterAvailableModel StandardsFilterClause matches a model either by 
 *  StandardsRulesetModelType or by id. (Use of the constructor by id is an advanced feature, and 
 *  is not for general use.) Several filters and actions refer to instances of 
 *  StandardsFilterAvailableModel, sometimes optionally, so that the writer of the rule can be
 *  clear about the model being queried or modified. */
class RULESET_API StandardsFilterAvailableModel : public StandardsFilterClause {
 public:
  /// @cond
  typedef detail::StandardsFilterAvailableModel_Impl ImplType;

  static std::string xmlElementName();
  /// @endcond

  /** @name Constructors and Destructors */
  //@{

  /** Not for general use. */
  StandardsFilterAvailableModel(const std::string& id);

  /** Constructor provided for deserialization; not for general use. */
  StandardsFilterAvailableModel(const std::string& id, 
                                const UUID& uuid, 
                                const UUID& versionUUID);

  StandardsFilterAvailableModel(const StandardsRulesetModelType& modelType);

  /** Constructor provided for deserialization; not for general use. */
  StandardsFilterAvailableModel(const StandardsRulesetModelType& modelType, 
                                const UUID& uuid, 
                                const UUID& versionUUID);

  /** Constructor provided for deserialization; not for general use. */
  StandardsFilterAvailableModel(const QDomElement& element);

  //@}
  /** @name Getters */
  //@{

  std::string id() const;

  boost::optional<StandardsRulesetModelType> modelType() const;

  //@}
 protected:
  /// @cond
  friend class detail::StandardsFilterAvailableModel_Impl;
  friend class RulesetObject;

  /** Protected constructor from impl. */
  StandardsFilterAvailableModel(boost::shared_ptr<detail::StandardsFilterAvailableModel_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("Ruleset.StandardsFilterAvailableModel");
};

/** \relates StandardsFilterAvailableModel */
typedef boost::optional<StandardsFilterAvailableModel> OptionalStandardsFilterAvailableModel;

/** \relates StandardsFilterAvailableModel */
typedef std::vector<StandardsFilterAvailableModel> StandardsFilterAvailableModelVector;

} // ruleset
} // openstudio

#endif // RULESET_STANDARDSFILTERAVAILABLEMODEL_HPP
