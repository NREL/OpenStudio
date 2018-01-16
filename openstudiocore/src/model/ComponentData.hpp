/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef MODEL_COMPONENTDATA_HPP
#define MODEL_COMPONENTDATA_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

#include "../utilities/core/UUID.hpp"

namespace openstudio {
namespace model {

namespace detail {
  class ComponentData_Impl;
} // detail

/** ComponentData is a ModelObject that wraps the OpenStudio IDD object 'OS:ComponentData'. It is
 *  used in \link Component Components\endlink to provide a contents list and data (unique
 *  identifiers and timestamps) that link the Component back to the Building Component Library
 *  (BCL). It is used in \link Model Models \endlink to indicate which objects are associated
 *  with which Components. */
class MODEL_API ComponentData : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~ComponentData() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  /** The unique identifier of the component. This identifier is generated on initial construction
   *  with ModelObject::createComponent, and then maintained as-is for the life of the Component.
   *  It is used to track the Component across multiple models, on disk (.osc and .osm), in the
   *  project::ProjectDatabase, and in local and online BCLs. */
  UUID uuid() const;

  /** Returns the unique identifier of the current version of the component. This UUID is
   *  automatically regenerated when any Component data changes. */
  UUID versionUUID() const;

  /** Timestamp at time of creation (when the uuid was initially generated). Unix timestamp from
   *  January 1, 1970 00:00 GMT, in seconds. */
  boost::optional<int> creationTimestamp() const;

  /** Timestamp of last versioning event (creation of versionUUID). Unix timestamp from January 1,
   *  1970 00:00 GMT, in seconds. */
  boost::optional<int> versionTimestamp() const;

  /** Returns the primary object of this component, which defines the Component's overall type.
   *  This method is equivalent to getComponentObject(0). */
  ModelObject primaryComponentObject() const;

  /** Returns all of the \link ModelObject ModelObjects\endlink that are part of this Component.
   *  The primaryComponentObject() will be listed first. */
  std::vector<ModelObject> componentObjects() const;

  /** Returns the ModelObject at objectIndex in the contents. Throws an openstudio::Exception if
   *  objectIndex >= numComponentObjects. */
  ModelObject getComponentObject(unsigned objectIndex) const;

  //@}
  /** @name Setters */
  //@{

  /** Creates and returns a new value for versionUUID(). */
  UUID createVersionUUID();

  //@}
  /** @name Queries */
  //@{

  /** Returns the number of objects in the Component described by this object. Must be >= 1. */
  unsigned numComponentObjects() const;

  //@}
 protected:
  /// @cond
  typedef detail::ComponentData_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  // constructor
  explicit ComponentData(const Model& model);

  // constructor
  explicit ComponentData(std::shared_ptr<detail::ComponentData_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ComponentData");
};

/** \relates ComponentData */
typedef boost::optional<ComponentData> OptionalComponentData;

/** \relates ComponentData */
typedef std::vector<ComponentData> ComponentDataVector;

} // model
} // openstudio

#endif // MODEL_COMPONENTDATA_HPP
