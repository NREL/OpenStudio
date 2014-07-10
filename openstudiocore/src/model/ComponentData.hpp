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
