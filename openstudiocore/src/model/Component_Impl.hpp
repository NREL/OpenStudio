/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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
    virtual Workspace clone(bool keepHandles=false) const override;

    /** Invalid to partially clone a Component. Always throws. In some cases, it may be possible to
     *  clone(), and then remove objects as desired. */
    virtual Workspace cloneSubset(const std::vector<Handle>& handles,
                                  bool keepHandles = false,
                                  StrictnessLevel level = StrictnessLevel::Draft) const override;

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
    virtual bool setStrictnessLevel(StrictnessLevel level) override;

    /** Override to return boost::none. */
    virtual boost::optional<WorkspaceObject> addObject(const IdfObject& idfObject) override;

    /** Override to return boost::none. */
    virtual boost::optional<WorkspaceObject> insertObject(const IdfObject& idfObject) override;

    /** Override to return empty vector. */
    virtual std::vector<WorkspaceObject> addObjects(const std::vector<IdfObject>& idfObjects, bool checkNames) override;

    /** Override to return empty vector. */
    virtual std::vector<WorkspaceObject> insertObjects(const std::vector<IdfObject>& idfObjects) override;

    /** Override to return empty vector. */
    virtual std::vector<WorkspaceObject> addAndInsertObjects(const std::vector<IdfObject>& objectsToAdd,
                                                             const std::vector<IdfObject>& objectsToInsert) override;

    /** Override to return empty vector. */
    virtual std::vector<WorkspaceObject> addObjects(const std::vector<WorkspaceObject>& objects, bool checkNames) override;

    /** Override to return empty vector. */
    virtual std::vector<WorkspaceObject> insertObjects(const std::vector<WorkspaceObject>& objects) override;

    /** Override to return empty vector. */
    virtual std::vector<WorkspaceObject> addAndInsertObjects(
        const std::vector<WorkspaceObject>& objectsToAdd,
        const std::vector<WorkspaceObject>& objectsToInsert) override;

    /** Override to return empty vector. */
    virtual std::vector<WorkspaceObject> addAndInsertObjects(
        const std::vector<WorkspaceObject>& objectsToAdd,
        const std::vector< std::vector<WorkspaceObject> >& objectsToInsert) override;

    /** Override to return false. */
    virtual bool setSqlFile(const openstudio::SqlFile& sqlFile) override;

    /** Override to return boost::none. */
    virtual boost::optional<ComponentData> insertComponent(const Component& component) override;

    /** Override to return empty vector. */
    virtual std::vector<openstudio::IdfObject> purgeUnusedResourceObjects() override;
    virtual std::vector<openstudio::IdfObject> purgeUnusedResourceObjects(IddObjectType iddObjectType) override;

    //@}
    /** @name Serialization */
    //@{

    /** Save Component to path. Will construct parent folder, but no further up the chain. Will
     *  only overwrite an existing file if overwrite==true. Will set extension to
     *  componentFileExtension(). */
    virtual bool save(const openstudio::path& p, bool overwrite=false) override;

    //@}

    virtual void obsoleteComponentWatcher(const ComponentWatcher& watcher) override;

   private:

    REGISTER_LOGGER("openstudio.model.Component");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_COMPONENT_IMPL_HPP
