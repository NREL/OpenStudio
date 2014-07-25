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
} // detail

/** Component is a Model that contains exactly one ComponentData object. All objects other than
 *  the ComponentData object and the version object are the Component contents. There is a
 *  primary ModelObject that provides the Component's type. There are no public constructors,
 *  as Components are to be constructed through the ModelObject method \link
 *  ModelObject::createComponent createComponent\endlink. Similarly, Model-level object addition
 *  and removal, and SqlFile functionality, has been disabled. */
class MODEL_API Component : public Model {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Construct from an IdfFile. Must contain exactly one ComponentData object, and contents
   *  list in ComponentData must exactly match the contents of idfFile. */
  explicit Component(const openstudio::IdfFile& idfFile);

  virtual ~Component();

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
  virtual bool save(const openstudio::path& p, bool overwrite=false);

  //@}
 protected:
  /// @cond
  typedef detail::Component_Impl ImplType;

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
typedef boost::optional<Component> OptionalComponent;

/** \relates Component */
typedef std::vector<Component> ComponentVector;

} // model
} // openstudio

#endif // MODEL_COMPONENT_HPP
