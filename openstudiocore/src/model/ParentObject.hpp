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
