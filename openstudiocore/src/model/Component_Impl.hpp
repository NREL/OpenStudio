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

#ifndef MODEL_COMPONENT_IMPL_HPP
#define MODEL_COMPONENT_IMPL_HPP

#include "Model_Impl.hpp"

namespace openstudio {
namespace model {

class ComponentData;

namespace detail {

  /** Implementation of Component. In addition to providing Component functionality, overrides
   *  most derived setters to avoid invalidating the Component. Also does not support SqlFile
   *  attachment. */
  class MODEL_API Component_Impl : public Model_Impl {
    Q_OBJECT;
   public:

    /** @name Constructors and Destructors */
    //@{

    // construct from IdfFile, used for deserialization
    explicit Component_Impl(const IdfFile& idfFile);

    // copy constructor, used for clone
    Component_Impl(const Component_Impl& other, bool keepHandles=false);

    /** Construct from subset of Model. Makes unconnected copy of all data, only keeping
     *  information about objects corresponding to handles. */
    Component_Impl(const Model_Impl& model,
                   const std::vector<Handle>& hs);

    /** Implementation of openstudio::detail::Workspace_Impl::clone for Component_Impl. The returned
     *  value may be cast to type Component. */
    virtual Workspace clone(bool keepHandles=false) const;

    /** Invalid to partially clone a Component. Always throws. In some cases, it may be possible to
     *  clone(), and then remove objects as desired. */
    virtual Workspace cloneSubset(const std::vector<Handle>& handles,
                                  bool keepHandles = false,
                                  StrictnessLevel level = StrictnessLevel::Draft) const;

    // no swap method here because no component-level data

    virtual ~Component_Impl();

    //@}
    /** @name Getters */
    //@{

    /** Returns object that lists meta-data and contents of this Component. */
    ComponentData componentData() const;

    /** Returns the primary ModelObject of this Component. Can be used to access the Component
     *  type by using .optionalCast and/or iddObject().type(). */
    ModelObject primaryObject() const;

    //@}
    /** @name Setters */
    //@{

    /** Override to return false. Components are always at Draft StrictnessLevel. */
    virtual bool setStrictnessLevel(StrictnessLevel level);

    /** Override to return boost::none. */
    virtual boost::optional<WorkspaceObject> addObject(const IdfObject& idfObject);

    /** Override to return boost::none. */
    virtual boost::optional<WorkspaceObject> insertObject(const IdfObject& idfObject);

    /** Override to return empty vector. */
    virtual std::vector<WorkspaceObject> addObjects(const std::vector<IdfObject>& idfObjects);

    /** Override to return empty vector. */
    virtual std::vector<WorkspaceObject> insertObjects(const std::vector<IdfObject>& idfObjects);

    /** Override to return empty vector. */
    virtual std::vector<WorkspaceObject> addAndInsertObjects(const std::vector<IdfObject>& objectsToAdd,
                                                             const std::vector<IdfObject>& objectsToInsert);

    /** Override to return empty vector. */
    virtual std::vector<WorkspaceObject> addObjects(const std::vector<WorkspaceObject>& objects);

    /** Override to return empty vector. */
    virtual std::vector<WorkspaceObject> insertObjects(const std::vector<WorkspaceObject>& objects);

    /** Override to return empty vector. */
    virtual std::vector<WorkspaceObject> addAndInsertObjects(
        const std::vector<WorkspaceObject>& objectsToAdd,
        const std::vector<WorkspaceObject>& objectsToInsert);

    /** Override to return empty vector. */
    virtual std::vector<WorkspaceObject> addAndInsertObjects(
        const std::vector<WorkspaceObject>& objectsToAdd,
        const std::vector< std::vector<WorkspaceObject> >& objectsToInsert);

    /** Override to return false. */
    virtual bool setSqlFile(const openstudio::SqlFile& sqlFile);

    /** Override to return boost::none. */
    virtual boost::optional<ComponentData> insertComponent(const Component& component);

    /** Override to return empty vector. */
    virtual std::vector<openstudio::IdfObject> purgeUnusedResourceObjects();
    virtual std::vector<openstudio::IdfObject> purgeUnusedResourceObjects(IddObjectType iddObjectType);

    //@}
    /** @name Serialization */
    //@{

    /** Save Component to path. Will construct parent folder, but no further up the chain. Will
     *  only overwrite an existing file if overwrite==true. Will set extension to
     *  componentFileExtension(). */
    virtual bool save(const openstudio::path& p, bool overwrite=false);

    //@}
   public slots :

    virtual void obsoleteComponentWatcher(const ComponentWatcher& watcher);

   private:

    REGISTER_LOGGER("openstudio.model.Component");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_COMPONENT_IMPL_HPP
