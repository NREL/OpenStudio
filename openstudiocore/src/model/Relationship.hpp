/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

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
