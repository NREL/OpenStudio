/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PARENTOBJECT_HPP
#define MODEL_PARENTOBJECT_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class ParentObject_Impl;
  }

  /** ParentObject is an abstract class derived from ModelObject.
 *
 *  \todo It is an open question if the functionality of ParentObject should be merged
 *  with ModelObject and ParentObject deprecated.
 */
  class MODEL_API ParentObject : public ModelObject
  {
   public:
    virtual ~ParentObject() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ParentObject(const ParentObject& other) = default;
    ParentObject(ParentObject&& other) = default;
    ParentObject& operator=(const ParentObject&) = default;
    ParentObject& operator=(ParentObject&&) = default;

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

    using ImplType = detail::ParentObject_Impl;

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
  using OptionalParentObject = boost::optional<ParentObject>;

  // vector of ParentObject
  using ParentObjectVector = std::vector<ParentObject>;

  /** Returns object and all other objects accessible by calling children() on encountered
 *  \link ParentObject ParentObjects \endlink. LifeCycleCost and AdditionalProperties objects
 *  are not technically children but may be included by setting the optional second argument to true. \relates ParentObject
 *  An additional switch can exclude ResourceObject which have a directUseCount > 1, which is especially useful for remove() so you don't remove
 *  ResourceObjects (such as Curve) if they are actually used by another object than the one you're deleting */
  MODEL_API std::vector<ModelObject> getRecursiveChildren(const ParentObject& object, bool includeLifeCycleCostsAndAdditionalProperties = false,
                                                          bool includeUsedResources = true);

  /** Returns all objects in getRecursiveChildren(object) that can be cast to type T. */
  template <typename T>
  std::vector<T> getSubsetOfRecursiveChildren(const ParentObject& object) {
    std::vector<ModelObject> allChildren = getRecursiveChildren(object);
    std::vector<T> result;
    for (const ModelObject& child : allChildren) {
      boost::optional<T> candidate = child.optionalCast<T>();
      if (candidate) {
        result.push_back(*candidate);
      }
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
      if (candidate) {
        result.push_back(*candidate);
      }
    }
    return result;
  }

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PARENTOBJECT_HPP
