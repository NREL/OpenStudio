/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#ifndef MODEL_RELATIONSHIP_HPP
#define MODEL_RELATIONSHIP_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

#include "../utilities/core/Logger.hpp"
#include "../utilities/core/Optional.hpp"

#include <QVariant>
#include <QMetaProperty>

#include <string>

namespace openstudio {
namespace model {

namespace detail {
  class ModelObject_Impl;
}

/** Relationship describes relationships that a ModelObject may have with other \link ModelObject ModelObjects \endlink.
 *
 *  Relationships of a ModelObject are similar to Attributes of a ModelObject, they provide named access to object methods
 *  through the QObject Q_PROPERTY system.  However, while Attributes of a ModelObject return simple data types (e.g. PlanarSurface
 *  has an Attribute named 'netArea' that returns a double), Relationships return related ModelObjects.  A Relationship is constructed
 *  with a primary ModelObject and describes relationships that the primary ModelObject may have with other ModelObjects. A Relationship
 *  can be singular (e.g. a Surface object relates to 0 or 1 Construction objects) or plural (e.g. a Zone relates to any number of Lights objects).
 *  A Relationship object can be used to get these related ModelObjects by name.
 */
class MODEL_API Relationship {
 public:
  /// Returns the primary ModelObject.
  ModelObject modelObject() const;

  /// Returns the name of the relationship.
  std::string name() const;

  /// Returns true if there is a write method for this relationship
  bool isSettable() const;

  /// Returns true if this Relationship concerns 0-1 other ModelObjects, 
  /// otherwise this Relationship can access an unlimited number of other ModelObjects.
  bool isSingular() const;

  /// Return the related ModelObject if available, will throw if Relationship is not singular.
  boost::optional<ModelObject> relatedModelObject() const;

  /// Return all related ModelObjects, will throw if Relationship is singular.
  std::vector<ModelObject> relatedModelObjects() const;

  /// Resets the related ModelObject and forms relationship with the new ModelObject, 
  /// throws if Relationship is not singular.
  bool setRelatedModelObject(boost::optional<ModelObject> relatedModelObject);

  /** Inserts component into Model and sets related model object on the primary object. */
  bool setRelatedModelObject(const Component& component);

  /// Resets the related ModelObject, throws if Relationship is not singular.
  /// Equivalent to setRelatedModelObject(boost::none)
  bool resetRelatedModelObject();

  /// Clears all related ModelObjects and then forms relationships with the new ModelObjects, 
  /// throws if Relationship is singular.
  bool setRelatedModelObjects(std::vector<ModelObject> relatedModelObjects);

  /// Clears all related ModelObjects, throws if Relationship is singular.
  /// Equivalent to setRelatedModelObject(std::vector<ModelObject>())
  bool clearRelatedModelObjects();

 protected:

  /// Creates a Relationship object to access the named relationships of the given ModelObject.
  Relationship(const ModelObject& object, const std::string& name);

  /// @cond

  friend class detail::ModelObject_Impl;

 private:
  ModelObject m_object;
  std::string m_name;
  QMetaProperty m_qMetaProperty;

  void assertSingular(bool singular) const;

  REGISTER_LOGGER("openstudio.model.Relationship");

  /// @endcond
};

/** \relates Relationship */
typedef std::vector<Relationship> RelationshipVector;

/** \relates Relationship */
typedef boost::optional<Relationship> OptionalRelationship;

} // model
} // openstudio

#endif // MODEL_RELATIONSHIP_HPP
