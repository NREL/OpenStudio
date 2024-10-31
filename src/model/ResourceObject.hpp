/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_RESOURCEOBJECT_HPP
#define MODEL_RESOURCEOBJECT_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class ResourceObject_Impl;
  }

  /** ResourceObject is an abstract class derived from ParentObject.
 *
 *  ResourceObject is a pure virtual class for ModelObjects that are shared, globally across a Model.
 *  Examples include Constructions (referenced by many Surfaces), Materials (referenced by many Constructions),
 *  and Schedules (referenced by almost everything).  Methods such as ModelObject::clone and Model::purgeUnusedResourceObjects
 *  handle special cases related to ResourceObjects.
 */
  class MODEL_API ResourceObject : public ParentObject
  {
   public:
    /** Returns the number of objects that use this resource directly. All usages count equally.
   *  For instance, the use of a Material in a LayeredConstruction counts as one use of the
   *  Material object, and usages of that LayeredConstruction do not add to the Material's use
   *  count. If excludeChildren is true then children of this object do not contribute to the use count.*/
    unsigned directUseCount(bool excludeChildren = false) const;

    /** Returns the number of non-ResourceObjects that use this resource either directly or
   *  indirectly. Non-ResourceObjects include Building, Zone, Surface, Lights, and AirLoopHVAC,
   *  for instance. An example of indirect use is the use of a Material object through inclusion
   *  in a Construction. If a particular Material is used in exactly one LayeredConstruction, which
   *  is in turn used by three PlanarSurface objects, then the nonResourceObjectUseCount of both
   *  the Material and the LayeredConstruction is three.
   *  If excludeChildren is true then children of this object do not contribute to the use count.*/
    unsigned nonResourceObjectUseCount(bool excludeChildren = false) const;

    virtual ~ResourceObject() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ResourceObject(const ResourceObject& other) = default;
    ResourceObject(ResourceObject&& other) = default;
    ResourceObject& operator=(const ResourceObject&) = default;
    ResourceObject& operator=(ResourceObject&&) = default;

   protected:
    /** @name Constructors and Destructors */
    //@{

    /// Constructs a new PlanarSurface object in the model.
    ResourceObject(IddObjectType type, const Model& model);

    //@}

    /// @cond

    using ImplType = detail::ResourceObject_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class detail::ResourceObject_Impl;

    explicit ResourceObject(std::shared_ptr<detail::ResourceObject_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.ResourceObject");

    /// @endcond
  };

  /** \relates ResourceObject */
  using OptionalResourceObject = boost::optional<ResourceObject>;

  /** \relates ResourceObject */
  using ResourceObjectVector = std::vector<ResourceObject>;

  /** Returns all \link ResourceObject ResourceObjects \endlink accessible by recursively calling
 *  .resources() starting from object. Will not return object even if object is a ResourceObject.
 *  \relates ModelObject */
  MODEL_API std::vector<ResourceObject> getRecursiveResources(const ModelObject& object);

  /** Returns all objects in getRecursiveResources(object) that can be cast to type T. */
  template <typename T>
  std::vector<T> getSubsetOfRecursiveResources(const ModelObject& object) {
    std::vector<ResourceObject> allResources = getRecursiveResources(object);
    std::vector<T> result;
    for (const ResourceObject& resource : allResources) {
      boost::optional<T> candidate = resource.optionalCast<T>();
      if (candidate) {
        result.push_back(*candidate);
      }
    }
    return result;
  }

  /** Returns all \link ResourceObject ResourceObjects \endlink and their children accessible
 *  by alternately calling ModelObject::resources() and getRecursiveChilden. Each element of the
 *  returned vector is a vector headed by a ResourceObject and followed by that ResourceObject's
 *  children. ComponentCost_LineItem objects are not technically children but may be included
 *  by setting the optional second argument to true.*/
  MODEL_API std::vector<std::vector<ModelObject>> getRecursiveResourceSubTrees(const ModelObject& object, bool includeComponentCostLineItems = false);

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_RESOURCEOBJECT_HPP
