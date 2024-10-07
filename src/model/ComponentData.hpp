/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COMPONENTDATA_HPP
#define MODEL_COMPONENTDATA_HPP

#include "ModelAPI.hpp"
#include "ResourceObject.hpp"

#include "../utilities/core/UUID.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class ComponentData_Impl;
  }  // namespace detail

  /** ComponentData is a ModelObject that wraps the OpenStudio IDD object 'OS:ComponentData'. It is
 *  used in \link Component Components\endlink to provide a contents list and data (unique
 *  identifiers and timestamps) that link the Component back to the Building Component Library
 *  (BCL). It is used in \link Model Models \endlink to indicate which objects are associated
 *  with which Components. */
  class MODEL_API ComponentData : public ResourceObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~ComponentData() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ComponentData(const ComponentData& other) = default;
    ComponentData(ComponentData&& other) = default;
    ComponentData& operator=(const ComponentData&) = default;
    ComponentData& operator=(ComponentData&&) = default;

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
    using ImplType = detail::ComponentData_Impl;

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
  using OptionalComponentData = boost::optional<ComponentData>;

  /** \relates ComponentData */
  using ComponentDataVector = std::vector<ComponentData>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COMPONENTDATA_HPP
