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

#ifndef RULESET_MODELOBJECTFILTERTYPE_HPP
#define RULESET_MODELOBJECTFILTERTYPE_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/ModelObjectFilterClause.hpp>

class QDomDocument;
class QDomElement;

namespace openstudio {

class IddObjectType;

namespace model {
  class Model;
}

namespace ruleset {

namespace detail {
  class ModelObjectFilterType_Impl;
} // detail

/** ModelObjectFilterType is a concrete ModelObjectFilterClause that may be used to get all of the
 *  model::ModelObjects of a particular type (IddObjectType) out of a model::Model. */
class RULESET_API ModelObjectFilterType : public ModelObjectFilterClause {
 public:

  /// @cond
  typedef detail::ModelObjectFilterType_Impl ImplType;
  /// @endcond

  static std::string xmlElementName();

  /** @name Constructors and Destructors */
  //@{

  ModelObjectFilterType(const openstudio::IddObjectType& iddObjectType);

  /** Constructor provided for deserialization; not for general use. */
  ModelObjectFilterType(const openstudio::IddObjectType& iddObjectType,
                        const UUID& uuid,
                        const UUID& versionUUID);

  /** Constructor provided for deserialization; not for general use. */
  explicit ModelObjectFilterType(const QDomElement& element);
  
  virtual ~ModelObjectFilterType() {}
  
  //@}
  /** @name Getters */
  //@{

  openstudio::IddObjectType iddObjectType() const;

  //@}
 protected:
  /// @cond
  friend class detail::ModelObjectFilterType_Impl;
  friend class RulesetObject;

  /** Protected constructor from impl. */
  ModelObjectFilterType(const boost::shared_ptr<detail::ModelObjectFilterType_Impl>& impl);

  ///@endcond
 private:
  REGISTER_LOGGER("Ruleset.ModelObjectFilterType");
};

/** \relates ModelObjectFilterType */
typedef boost::optional<ModelObjectFilterType> OptionalModelObjectFilterType;

/** \relates ModelObjectFilterType */
typedef std::vector<ModelObjectFilterType> ModelObjectFilterTypeVector;

/** Returns the ModelObjects that pass all \link ModelObjectFilterType ModelObjectFilterTypes 
 *  \endlink in filters. (The return value will always be .empty() if there are multiple, 
 *  conflicting \link ModelObjectFilterType ModelObjectFilterTypes \endlink in filters.) */
std::vector<model::ModelObject> RULESET_API applyTypeFilters(
    const model::Model& model,
    const std::vector<ModelObjectFilterClause>& filters);

} // ruleset
} // openstudio

#endif // RULESET_MODELOBJECTFILTERTYPE_HPP
