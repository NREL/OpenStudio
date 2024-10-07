/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COMPONENT_HPP
#define MODEL_COMPONENT_HPP

#include "Model.hpp"

namespace openstudio {

namespace detail {
  class Workspace_Impl;
}

namespace model {

  class ComponentData;

  namespace detail {
    class Component_Impl;
    class ModelObject_Impl;
    class ParentObject_Impl;
  }  // namespace detail

  /** Component is a Model that contains exactly one ComponentData object. All objects other than
 *  the ComponentData object and the version object are the Component contents. There is a
 *  primary ModelObject that provides the Component's type. There are no public constructors,
 *  as Components are to be constructed through the ModelObject method \link
 *  ModelObject::createComponent createComponent\endlink. Similarly, Model-level object addition
 *  and removal, and SqlFile functionality, has been disabled. */
  class MODEL_API Component : public Model
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Construct from an IdfFile. Must contain exactly one ComponentData object, and contents
   *  list in ComponentData must exactly match the contents of idfFile. */
    explicit Component(const openstudio::IdfFile& idfFile);

    virtual ~Component() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    Component(const Component& other) = default;
    Component(Component&& other) = default;
    Component& operator=(const Component&) = default;
    Component& operator=(Component&&) = default;

    //@}
    /** @name Getters */
    //@{

    /** Returns the object that contains the contents list, unique identifiers, and timestamps
   *  for this Component. */
    ComponentData componentData() const;

    /** Returns the primary ModelObject of this Component. Can be used to access the Component
   *  type by using .optionalCast or iddObjectType(). */
    ModelObject primaryObject() const;

    //@}
    /** @name Serialization */
    //@{

    /** Load Component from path p, if possible. */
    static boost::optional<Component> load(const path& p);

    /** Save Component to path p. Will construct the parent folder if its parent folder exists.
   *  An existing file will only be overwritten if if overwrite==true. If no extension is provided,
   *  componentFileExtension() will be used. */
    virtual bool save(const openstudio::path& p, bool overwrite = false);

    //@}
   protected:
    /// @cond
    using ImplType = detail::Component_Impl;

    friend class openstudio::Workspace;
    friend class openstudio::detail::Workspace_Impl;
    friend class detail::Component_Impl;
    friend class detail::ModelObject_Impl;
    friend class detail::ParentObject_Impl;

    /** Protected constructor from impl. */
    Component(std::shared_ptr<detail::Component_Impl> impl);

    /// @endcond

    /** Construct from a vector of ModelObjects, all in the same .model(). The primary object
   *  must be listed first. Throws if contents.empty() or if more than one .model() is found.
   *  StrictnessLevel is always set to Draft. Not for general use; used by
   *  ModelObject::createComponent. */
    Component(const std::vector<ModelObject>& contents);

   private:
    REGISTER_LOGGER("openstudio.model.Component");
  };

  /** \relates Component */
  using OptionalComponent = boost::optional<Component>;

  /** \relates Component */
  using ComponentVector = std::vector<Component>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COMPONENT_HPP
