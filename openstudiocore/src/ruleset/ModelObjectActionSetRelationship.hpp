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

#ifndef RULESET_MODELOBJECTACTIONSETRELATIONSHIP_HPP
#define RULESET_MODELOBJECTACTIONSETRELATIONSHIP_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/ModelObjectActionClause.hpp>

#include <utilities/core/Path.hpp>

class QDomDocument;
class QDomElement;

namespace openstudio {

class FileReference;

namespace model {
  class Component;
}

namespace ruleset {

namespace detail {
  class ModelObjectActionSetRelationship_Impl;
}

/** ModelObjectActionSetRelationship is a ModelObjectActionClause that applies model::Components
 *  to model::ModelObjects based on the specified relationshipName(). For instance, 
 *  model::PlanarSurfaces have "construction" relationships with model::ConstructionBase objects, 
 *  and Components whose primaryObject() casts to ConstructionBase. */
class RULESET_API ModelObjectActionSetRelationship : public ModelObjectActionClause {
 public:
  /// @cond
  typedef detail::ModelObjectActionSetRelationship_Impl ImplType;
  /// @endcond

  static std::string xmlElementName();

  /** @name Constructors and Destructors */
  //@{

  ModelObjectActionSetRelationship(const std::string& relationshipName, 
                                   const openstudio::path& componentPath);

  ModelObjectActionSetRelationship(const std::string& relationshipName,
                                   const FileReference& componentReference);

  /** Constructor provided for deserialization; not for general use. */
  ModelObjectActionSetRelationship(const std::string& relationshipName, 
                                   const FileReference& componentReference,
                                   const UUID& uuid, 
                                   const UUID& versionUUID);

  /** Constructor provided for deserialization; not for general use. */
  explicit ModelObjectActionSetRelationship(const QDomElement& element);
  
  virtual ~ModelObjectActionSetRelationship() {}

  //@}
  /** @name Getters */
  //@{

  std::string relationshipName() const;

  openstudio::path componentPath() const;

  FileReference componentReference() const;

  boost::optional<openstudio::model::Component> component() const;

  //@}
 protected:
  /// @cond
  friend class detail::ModelObjectActionSetRelationship_Impl;
  friend class RulesetObject;

  /** Protected constructor from impl. */
  ModelObjectActionSetRelationship(boost::shared_ptr<detail::ModelObjectActionSetRelationship_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("Ruleset.ModelObjectActionSetRelationship");
};

/** \relates ModelObjectActionSetRelationship */
typedef boost::optional<ModelObjectActionSetRelationship> OptionalModelObjectActionSetRelationship;

/** \relates ModelObjectActionSetRelationship */
typedef std::vector<ModelObjectActionSetRelationship> ModelObjectActionSetRelationshipVector;

} // ruleset
} // openstudio

#endif // RULESET_MODELOBJECTACTIONSETRELATIONSHIP_HPP
