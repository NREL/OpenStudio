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

#ifndef MODEL_PARENTOBJECT_HPP
#define MODEL_PARENTOBJECT_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

namespace detail{
  class ParentObject_Impl;
}

/** ParentObject is an abstract class derived from ModelObject.
 *   
 *  \todo It is an open question if the functionality of ParentObject should be merged
 *  with ModelObject and ParentObject deprecated.
 */
class MODEL_API ParentObject : public ModelObject {
 public:
  virtual ~ParentObject() {}

  /** @name Constructors and Destructors */
  //@{
  
  //@}
  /** @name Getters */
  //@{

  //@}
  /** @name Setters */
  //@{

  //@}

  /// Return direct child objects in the Model.
  std::vector<ModelObject> children() const;

  // DLM@20110614: this should be static on the wrapper, abstract for the impl.
  /// Returns a vector of allowable children types.
  std::vector<IddObjectType> allowableChildTypes() const;

 protected:

  /** @name Constructors and Destructors */
  //@{

  /// Constructs a new ParentObject object in the model.
  ParentObject(IddObjectType type, const Model& model);
  
  //@}

  /// @cond

  typedef detail::ParentObject_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  friend class detail::ParentObject_Impl;

  explicit ParentObject(std::shared_ptr<detail::ParentObject_Impl> impl);

 private:

  REGISTER_LOGGER("openstudio.model.ParentObject");

  /// @endcond

};

// optional ParentObject
typedef boost::optional<ParentObject> OptionalParentObject;

// vector of ParentObject
typedef std::vector<ParentObject> ParentObjectVector;

/** Returns object and all other objects accessible by calling children() on encountered 
 *  \link ParentObject ParentObjects \endlink. LifeCycleCost objects are not technically children but may be included 
 *  by setting the optional second argument to true. \relates ParentObject */
MODEL_API std::vector<ModelObject> getRecursiveChildren(const ParentObject& object, bool includeLifeCycleCosts = false);

/** Returns all objects in getRecursiveChildren(object) that can be cast to type T. */
template <typename T>
std::vector<T> getSubsetOfRecursiveChildren(const ParentObject& object) {
  std::vector<ModelObject> allChildren = getRecursiveChildren(object);
  std::vector<T> result;
  for (const ModelObject& child : allChildren) {
    boost::optional<T> candidate = child.optionalCast<T>();
    if (candidate) { result.push_back(*candidate); }
  }
  return result;
}

/** Returns object and all other objects accessible by calling children() and resources() on
 *  encountered \link ModelObject ModelObjects \endlink. \relates ParentObject */
MODEL_API std::vector<ModelObject> getRecursiveChildrenAndResources(const ModelObject& object);

/** Returns all objects in getRecursiveChildrenAndResources(object) that can be cast to type T. */
template <typename T>
std::vector<T> getSubsetOfRecursiveChildrenAndResources(const ModelObject& object) {
  std::vector<ModelObject> allChildrenAndResources = getRecursiveChildrenAndResources(object);
  std::vector<T> result;
  for (const ModelObject& childOrResource : allChildrenAndResources) {
    boost::optional<T> candidate = childOrResource.optionalCast<T>();
    if (candidate) { result.push_back(*candidate); }
  }
  return result;
}

} // model
} // openstudio

#endif // MODEL_PARENTOBJECT_HPP
