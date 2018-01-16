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

#include "Component.hpp"
#include "Component_Impl.hpp"

#include "Version.hpp"
#include "Version_Impl.hpp"
#include "ComponentData.hpp"
#include "ComponentData_Impl.hpp"

#include "../utilities/idf/IdfFile.hpp"
#include "../utilities/idf/WorkspaceObject.hpp"

#include <utilities/idd/OS_ComponentData_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>


#include "../utilities/core/Assert.hpp"
#include "../utilities/core/PathHelpers.hpp"

namespace openstudio {
namespace model {

namespace detail {

  Component_Impl::Component_Impl(const IdfFile& idfFile)
    : Model_Impl(idfFile)
  {}

  Component_Impl::Component_Impl(const Component_Impl& other, bool keepHandles)
    : Model_Impl(other,keepHandles)
  {}

  Component_Impl::Component_Impl(const Model_Impl& model,
                                 const std::vector<Handle>& hs)
    : Model_Impl(model,hs,false,StrictnessLevel::Draft)
  {}

  Component_Impl::~Component_Impl() {
    LOG(Trace,"Component_Impl destructor.");
  }

  Workspace Component_Impl::clone(bool keepHandles) const {
    // copy everything but objects
    std::shared_ptr<Component_Impl> cloneImpl(new Component_Impl(*this,keepHandles));
    // clone objects
    createAndAddClonedObjects(getWorkspace<Component>().getImpl<Component_Impl>(),cloneImpl,keepHandles);
    // wrap impl and return
    Component result(cloneImpl);
    return result.cast<Workspace>();
  }

  Workspace Component_Impl::cloneSubset(const std::vector<Handle>& handles,
                                        bool keepHandles,
                                        StrictnessLevel level) const
  {
    if (!handles.empty()) {
      LOG_AND_THROW("Components::cloneSubset is not supported. A workaround may be to clone() and "
                    << "then remove the unwanted objects.");
    }
    // create Model with same characteristics as this Component
    return Model_Impl::cloneSubset(handles,keepHandles,level);
  }

  ComponentData Component_Impl::componentData() const {
    ComponentDataVector candidates = model().getConcreteModelObjects<ComponentData>();
    OS_ASSERT(candidates.size() == 1);
    return candidates[0];
  }

  ModelObject Component_Impl::primaryObject() const {
    ComponentData metaDataAndContents = componentData();
    OS_ASSERT(metaDataAndContents.numComponentObjects() > 0);
    return metaDataAndContents.primaryComponentObject();
  }

  // SETTERS (override to do nothing)

  bool Component_Impl::setStrictnessLevel(StrictnessLevel level) {
    return false;
  }

  boost::optional<WorkspaceObject> Component_Impl::addObject(const IdfObject& idfObject) {
    return boost::none;
  }

  boost::optional<WorkspaceObject> Component_Impl::insertObject(const IdfObject& idfObject)
  {
    return boost::none;
  }

  std::vector<WorkspaceObject> Component_Impl::addObjects(const IdfObjectVector& idfObjects, bool checkNames)
  {
    return WorkspaceObjectVector();
  }

  std::vector<WorkspaceObject> Component_Impl::insertObjects(const IdfObjectVector& idfObjects)
  {
    return WorkspaceObjectVector();
  }

  std::vector<WorkspaceObject> Component_Impl::addAndInsertObjects(
      const std::vector<IdfObject>& objectsToAdd,const std::vector<IdfObject>& objectsToInsert)
  {
    return WorkspaceObjectVector();
  }

  std::vector<WorkspaceObject> Component_Impl::addObjects(
      const std::vector<WorkspaceObject>& objects, bool checkNames)
  {
    return WorkspaceObjectVector();
  }

  std::vector<WorkspaceObject> Component_Impl::insertObjects(
      const std::vector<WorkspaceObject>& objects)
  {
    return WorkspaceObjectVector();
  }

  std::vector<WorkspaceObject> Component_Impl::addAndInsertObjects(
        const std::vector<WorkspaceObject>& objectsToAdd,
        const std::vector<WorkspaceObject>& objectsToInsert)
  {
    return WorkspaceObjectVector();
  }

  std::vector<WorkspaceObject> Component_Impl::addAndInsertObjects(
        const std::vector<WorkspaceObject>& objectsToAdd,
        const std::vector< std::vector<WorkspaceObject> >& objectsToInsert)
  {
    return WorkspaceObjectVector();
  }

  bool Component_Impl::setSqlFile(const openstudio::SqlFile& sqlFile) {
    return false;
  }

  boost::optional<ComponentData> Component_Impl::insertComponent(const Component& component) {
    return boost::none;
  }

  std::vector<openstudio::IdfObject> Component_Impl::purgeUnusedResourceObjects() {
    return IdfObjectVector();
  }

  std::vector<openstudio::IdfObject> Component_Impl::purgeUnusedResourceObjects(IddObjectType iddObjectType) {
    return IdfObjectVector();
  }

  void Component_Impl::obsoleteComponentWatcher(const ComponentWatcher& watcher) {
    Model_Impl::obsoleteComponentWatcher(watcher);
    LOG_AND_THROW("The ComponentData object or primaryComponentObject has been removed. "
      << "This Component is no longer valid. (The ComponentData object may have been removed "
      << "because a relationship change called into question whether the Component was still "
      << "valid.)");
  }

  bool Component_Impl::save(const openstudio::path& p, bool overwrite) {
    return openstudio::detail::Workspace_Impl::save(
        setFileExtension(p,componentFileExtension(),true,true),overwrite);
  }

} // detail

Component::Component(const openstudio::IdfFile& idfFile)
  : Model(std::shared_ptr<detail::Component_Impl>(new detail::Component_Impl(idfFile)))
{
  // construct WorkspaceObject_ImplPtrs
  openstudio::detail::WorkspaceObject_ImplPtrVector objectImplPtrs;
  if (OptionalIdfObject vo = idfFile.versionObject()) {
    objectImplPtrs.push_back(getImpl<detail::Model_Impl>()->createObject(*vo,true));
  }
  for (const IdfObject& idfObject : idfFile.objects()) {
    objectImplPtrs.push_back(getImpl<detail::Component_Impl>()->createObject(idfObject,true));
  }
  // add Object_ImplPtrs to Workspace_Impl
  getImpl<openstudio::detail::Workspace_Impl>()->addObjects(objectImplPtrs);

  // check that this really is a component
  // 1 Version object
  boost::optional<Version> versionObject = getOptionalUniqueModelObject<Version>();
  if (!versionObject) {
    LOG_AND_THROW("Cannot construct Component because it does not contain a version object.")
  }

  // 1 ComponentData object
  ComponentDataVector componentDataObjects = getConcreteModelObjects<ComponentData>();
  if (componentDataObjects.size() != 1) {
    LOG_AND_THROW("Cannot construct Component from IdfFile because the file contains "
      << componentDataObjects.size() << " ComponentData objects.");
  }
  OS_ASSERT(componentDataObjects.size() == 1);
  ComponentData componentData = componentDataObjects[0];

  // All other objects are Component contents
  if (componentData.numComponentObjects() == 0) {
    LOG_AND_THROW("Cannot construct Component from IdfFile because there are no objects listed "
      << "in the contents.");
  }
  int nNonTrivialObjects = numObjects() - getObjectsByType(IddObjectType::CommentOnly).size();
  if (nNonTrivialObjects != static_cast<int>(componentData.numComponentObjects()) + 1) {
    LOG_AND_THROW("Cannot construct Component from IdfFile because the file contains " << nNonTrivialObjects
                  << " non-trivial objects, not " << componentData.numComponentObjects() + 1u
                  << ", as expected. The file: " << std::endl << idfFile);
  }
  try {
    ModelObjectVector componentContents = componentData.componentObjects();
  }
  catch (...) {
    LOG_AND_THROW("Cannot construct Component from IdfFile because at least one of the "
      << "comprising objects cannot be located.");
  }

  getImpl<detail::Model_Impl>()->createComponentWatchers();
}

Component::~Component() {
  LOG(Trace,"Component destructor.");
}

boost::optional<Component> Component::load(const path& p) {
  OptionalComponent result;
  OptionalIdfFile oIdfFile = IdfFile::load(p,IddFileType::OpenStudio);
  if (oIdfFile) {
    try {
      result = Component(*oIdfFile);
    }
    catch (...) {}
  }
  return result;
}

ComponentData Component::componentData() const {
  return getImpl<detail::Component_Impl>()->componentData();
}

ModelObject Component::primaryObject() const {
  return getImpl<detail::Component_Impl>()->primaryObject();
}

bool Component::save(const openstudio::path& p, bool overwrite) {
  return getImpl<detail::Component_Impl>()->save(p,overwrite);
}

/// @cond
Component::Component(std::shared_ptr<detail::Component_Impl> impl)
  : Model(std::move(impl))
{}
/// @endcond

Component::Component(const std::vector<ModelObject>& contents)
  : Model(std::shared_ptr<detail::Component_Impl>(new detail::Component_Impl(
      *(contents[0].model().getImpl<detail::Model_Impl>()),getHandles<ModelObject>(contents))))
{
  // create Version object
  this->addVersionObject();

  // create ComponentData object
  openstudio::detail::WorkspaceObject_ImplPtrVector objectImplPtrs;
  objectImplPtrs.push_back(getImpl<detail::Component_Impl>()->createObject(IdfObject(ComponentData::iddObjectType()),false));
  WorkspaceObjectVector objects = getImpl<openstudio::detail::Workspace_Impl>()->addObjects(objectImplPtrs);
  OS_ASSERT(objects.size() == static_cast<unsigned>(1));

  ComponentDataVector cdTemp = getConcreteModelObjects<ComponentData>();
  OS_ASSERT(cdTemp.size() == 1);
  ComponentData componentData = cdTemp[0];
  componentData.setString(OS_ComponentDataFields::UUID,toString(createUUID()));
  componentData.setInt(OS_ComponentDataFields::CreationTimestamp,time(nullptr));
  componentData.createVersionUUID();

  // clone objects
  openstudio::detail::WorkspaceObject_ImplPtrVector newObjectImplPtrs;
  HandleMap oldNewHandleMap;
  Model model = contents[0].model();
  for (const ModelObject& mo : contents) {
    OS_ASSERT(mo.model() == model);
    newObjectImplPtrs.push_back(getImpl<ImplType>()->createObject(
        mo.getImpl<detail::ModelObject_Impl>(),false) );
    oldNewHandleMap.insert(HandleMap::value_type(mo.handle(),newObjectImplPtrs.back()->handle()));
  }
  WorkspaceObjectVector newObjects = getImpl<detail::Model_Impl>()->addClones(newObjectImplPtrs,
                                                                              oldNewHandleMap,
                                                                              true);
  OS_ASSERT(newObjects.size() == newObjectImplPtrs.size());

  // populate ComponentData
  componentData.clearExtensibleGroups();
  for (WorkspaceObject& newObject : newObjects) {
    ModelObject mo = newObject.cast<ModelObject>();
    bool ok = componentData.getImpl<detail::ComponentData_Impl>()->registerObject(mo);
    OS_ASSERT(ok);
  }

  getImpl<detail::Model_Impl>()->createComponentWatchers();
}

} // model
} // openstudio
