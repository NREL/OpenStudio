/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

    virtual ~Component() = default;
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
