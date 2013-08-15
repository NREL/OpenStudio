/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <utilities/idf/Workspace.hpp>
#include <utilities/idf/Workspace_Impl.hpp>

#include <utilities/idf/WorkspaceObject_Impl.hpp>
#include <utilities/idf/IdfFile.hpp>
#include <utilities/idf/URLSearchPath.hpp>
#include <utilities/idf/ValidityReport.hpp>

#include <utilities/idd/IddFactory.hxx>

#include <utilities/plot/ProgressBar.hpp>

#include <utilities/core/Assert.hpp>
#include <utilities/core/Containers.hpp>
#include <utilities/core/URLHelpers.hpp>
#include <utilities/core/Compare.hpp>
#include <utilities/core/StringHelpers.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

#include <sstream>
#include <iostream>
#include <deque>
#include <map>
#include <list>

using namespace std;
using openstudio::istringEqual; // used for all name comparisons

namespace openstudio {

namespace detail {

  // CONSTRUCTORS

  Workspace_Impl::Workspace_Impl(StrictnessLevel level,IddFileType iddFileType) :
      m_strictnessLevel(level),
      m_iddFileAndFactoryWrapper(iddFileType),
      m_fastNaming(false),
      m_workspaceObjectOrder(boost::shared_ptr<WorkspaceObjectOrder_Impl>(new
          WorkspaceObjectOrder_Impl(HandleVector(),boost::bind(&Workspace_Impl::getObject,this,_1))))
  {}

  Workspace_Impl::Workspace_Impl(const IdfFile& idfFile,
                                 StrictnessLevel level) :
      m_strictnessLevel(level),
      m_header(idfFile.header()),
      m_iddFileAndFactoryWrapper(idfFile.iddFileAndFactoryWrapper()),
      m_fastNaming(false),
      m_workspaceObjectOrder(boost::shared_ptr<WorkspaceObjectOrder_Impl>(new
          WorkspaceObjectOrder_Impl(HandleVector(),boost::bind(&Workspace_Impl::getObject,this,_1))))
  {}

  Workspace_Impl::Workspace_Impl(const Workspace_Impl& other,bool keepHandles) :
    m_strictnessLevel(other.m_strictnessLevel),
    m_header(other.m_header),
    m_iddFileAndFactoryWrapper(other.m_iddFileAndFactoryWrapper),
    m_fastNaming(other.fastNaming()),
    m_workspaceObjectOrder(boost::shared_ptr<WorkspaceObjectOrder_Impl>(new
          WorkspaceObjectOrder_Impl(boost::bind(&Workspace_Impl::getObject,this,_1))))
  {
    // m_workspaceObjectOrder
    OptionalIddObjectTypeVector iddOrderVector = other.order().iddOrder();
    if (iddOrderVector) {
      m_workspaceObjectOrder.setIddOrder(*iddOrderVector);
    }
    OptionalHandleVector directOrderVector = other.order().directOrder();
    if (directOrderVector) {
      m_workspaceObjectOrder.setDirectOrder(*directOrderVector);
    }
  }

  Workspace_Impl::Workspace_Impl(const Workspace_Impl& other,
                                 const std::vector<Handle>& hs,
                                 bool keepHandles,
                                 StrictnessLevel level) :
      m_strictnessLevel(level),
      m_header(), // subset of original data--discard header
      m_iddFileAndFactoryWrapper(other.m_iddFileAndFactoryWrapper),
      m_fastNaming(other.fastNaming()),
      m_workspaceObjectOrder(boost::shared_ptr<WorkspaceObjectOrder_Impl>(new
          WorkspaceObjectOrder_Impl(hs,boost::bind(&Workspace_Impl::getObject,this,_1))))
  {
    // m_workspaceObjectOrder
    OptionalIddObjectTypeVector iddOrderVector = other.order().iddOrder();
    if (iddOrderVector) {
      m_workspaceObjectOrder.setIddOrder(*iddOrderVector);
    }
    OptionalHandleVector directOrderVector = other.order().directOrder();
    if (directOrderVector) {
      // discard unused handles
      HandleVector subsetOrder;
      BOOST_FOREACH(const Handle& h, *directOrderVector) {
        if (std::find(hs.begin(),hs.end(),h) != hs.end()) { subsetOrder.push_back(h); }
      }
      m_workspaceObjectOrder.setDirectOrder(subsetOrder);
    }
  }

  Workspace Workspace_Impl::clone(bool keepHandles) const {
    // copy everything but objects
    boost::shared_ptr<Workspace_Impl> cloneImpl(new Workspace_Impl(*this,keepHandles));
    // clone objects
    createAndAddClonedObjects(workspace().getImpl<Workspace_Impl>(),cloneImpl,keepHandles);
    // wrap impl and return
    Workspace result(cloneImpl);
    return result;
  }

  Workspace Workspace_Impl::cloneSubset(const std::vector<Handle>& handles,
                                        bool keepHandles,
                                        StrictnessLevel level) const
  {
    // copy everything but objects
    boost::shared_ptr<Workspace_Impl> cloneImpl(new Workspace_Impl(*this,handles,keepHandles,level));
    // clone objects
    createAndAddSubsetClonedObjects(workspace().getImpl<Workspace_Impl>(),cloneImpl,handles,keepHandles);
    // wrap impl and return
    Workspace result(cloneImpl);
    return result;
  }

  void Workspace_Impl::swap(Workspace& other) {
    boost::shared_ptr<Workspace_Impl> otherImpl = other.getImpl<Workspace_Impl>();

    StrictnessLevel tsl = m_strictnessLevel;
    m_strictnessLevel = otherImpl->m_strictnessLevel;
    otherImpl->m_strictnessLevel = tsl;

    std::string th = m_header;
    m_header = otherImpl->m_header;
    otherImpl->m_header = th;

    IddFileAndFactoryWrapper tifafw = m_iddFileAndFactoryWrapper;
    m_iddFileAndFactoryWrapper = otherImpl->m_iddFileAndFactoryWrapper;
    otherImpl->m_iddFileAndFactoryWrapper = tifafw;

    bool tfn = m_fastNaming;
    m_fastNaming = otherImpl->m_fastNaming;
    otherImpl->m_fastNaming = tfn;

    WorkspaceObjectMap twop = m_workspaceObjectMap;
    m_workspaceObjectMap = otherImpl->m_workspaceObjectMap;
    otherImpl->m_workspaceObjectMap = twop;

    WorkspaceObjectOrder twoo = m_workspaceObjectOrder;
    m_workspaceObjectOrder = otherImpl->m_workspaceObjectOrder;
    otherImpl->m_workspaceObjectOrder = twoo;

    IddObjectTypeMap tiotm = m_iddObjectTypeMap;
    m_iddObjectTypeMap = otherImpl->m_iddObjectTypeMap;
    otherImpl->m_iddObjectTypeMap = tiotm;

    IdfReferencesMap tirm = m_idfReferencesMap;
    m_idfReferencesMap = otherImpl->m_idfReferencesMap;
    otherImpl->m_idfReferencesMap = tirm;
  }

  // GETTERS

  Workspace Workspace_Impl::workspace() {
    return getWorkspace<Workspace>();
  }

  Workspace Workspace_Impl::workspace() const {
    return getWorkspace<Workspace>();
  }

  StrictnessLevel Workspace_Impl::strictnessLevel() const {
    return m_strictnessLevel;
  }

  VersionString Workspace_Impl::version() const {
    return VersionString(m_iddFileAndFactoryWrapper.version());
  }

  IddFile Workspace_Impl::iddFile() const {
    return m_iddFileAndFactoryWrapper.iddFile();
  }

  IddFileType Workspace_Impl::iddFileType() const {
    return m_iddFileAndFactoryWrapper.iddFileType();
  }

  boost::optional<IddObject> Workspace_Impl::getIddObject(const IddObjectType& type) const {
    return m_iddFileAndFactoryWrapper.getObject(type);
  }

  boost::optional<std::string> Workspace_Impl::name(const Handle& handle) const {
    OptionalWorkspaceObject object = getObject(handle);
    if (object) {
      return object->name();
    }
    return boost::none;
  }

  boost::optional<WorkspaceObject> Workspace_Impl::getObject(const Handle& handle) const {
    OptionalWorkspaceObject result;
    WorkspaceObjectMap::const_iterator womIt = m_workspaceObjectMap.find(handle);
    if (womIt != m_workspaceObjectMap.end()) { result = WorkspaceObject(womIt->second); }
    return result;
  }

  std::vector<WorkspaceObject> Workspace_Impl::objects(bool sorted) const {
    OptionalIddObject versionIdd = m_iddFileAndFactoryWrapper.versionObject();

    if (sorted) {
      OptionalHandleVector directOrder = order().directOrder();
      if (directOrder && (directOrder->size() == numObjects())) {
        WorkspaceObjectVector result;
        HandleSet setToCheckUniqueness;
        BOOST_FOREACH(const Handle& h, *directOrder) {
          OptionalWorkspaceObject owo = getObject(h);
          std::pair<HandleSet::iterator,bool> insertResult = setToCheckUniqueness.insert(h);
          if (owo &&
              insertResult.second &&
              (!versionIdd || (owo->iddObject() != versionIdd.get())))
          {
            result.push_back(*owo);
          }
          else {
            return sort(objects(false));
          }
        }
        return result;
      }
      return sort(objects(false));
    }

    WorkspaceObjectVector result;
    BOOST_FOREACH(const WorkspaceObjectMap::value_type& p, m_workspaceObjectMap) {
      WorkspaceObject obj = WorkspaceObject(p.second);
      if (!versionIdd || (obj.iddObject() != versionIdd.get())) {
        result.push_back(obj);
      }
    }
    return result;
  }

  std::vector<Handle> Workspace_Impl::handles(bool sorted) const {
    if (sorted) {
      OptionalHandleVector directOrder = order().directOrder();
      if (directOrder && (directOrder->size() == numObjects())) {
        HandleSet setToCheckUniqueness;
        BOOST_FOREACH(const Handle& h, *directOrder) {
          std::pair<HandleSet::iterator,bool> insertResult = setToCheckUniqueness.insert(h);
          if (!insertResult.second) {
            return sort(handles(false));
          }
        }
        return *directOrder;
      }
      return sort(handles(false));
    }

    HandleVector result;
    OptionalIddObject versionIdd = m_iddFileAndFactoryWrapper.versionObject();
    BOOST_FOREACH(const WorkspaceObjectMap::value_type& p, m_workspaceObjectMap) {
      if (!versionIdd || (p.second->iddObject() != versionIdd.get())) {
        result.push_back(p.first);
      }
    }
    return result;
  }

  std::vector<WorkspaceObject> Workspace_Impl::objectsWithURLFields() const {
    WorkspaceObjectVector result;
    BOOST_FOREACH(const WorkspaceObjectMap::value_type& p,m_workspaceObjectMap) {
      if( p.second->iddObject().hasURL()) {
         result.push_back(WorkspaceObject(p.second));
      }
    }
    return result;
  }

  boost::optional<WorkspaceObject> Workspace_Impl::versionObject() const {
    OptionalWorkspaceObject result;
    WorkspaceObjectVector candidates;
    if (iddFileType() == IddFileType::EnergyPlus) {
      candidates = getObjectsByType(IddObjectType(IddObjectType::Version));
    }
    else if (iddFileType() == IddFileType::OpenStudio) {
      candidates = getObjectsByType(IddObjectType(IddObjectType::OS_Version));
    }
    else if (OptionalIddObject versionIdd = m_iddFileAndFactoryWrapper.versionObject()){
      candidates = getObjectsByType(*versionIdd);
    }
    if (candidates.size() == 1u) {
      result = candidates[0];
    }
    return result;
  }

  std::vector<WorkspaceObject> Workspace_Impl::getObjects(const std::vector<Handle>& handles) const
  {
    WorkspaceObjectVector result;
    result.reserve(handles.size());

    bool lossy = false;

    // return in same order
    BOOST_FOREACH(const Handle& handle, handles) {
      OptionalWorkspaceObject candidate = getObject(handle);
      if (candidate) { result.push_back(*candidate); }
      else { lossy = true; }
    }

    if (lossy) { LOG(Warn,"Invalid handle. Cannot return all requested objects."); }
    return result;
  }

  std::vector<WorkspaceObject> Workspace_Impl::getObjectsByName(const std::string& name,
                                                                bool exactMatch) const
  {
    WorkspaceObjectVector result;
    if (exactMatch) {
      BOOST_FOREACH(const WorkspaceObjectMap::value_type& p,m_workspaceObjectMap) {
        if (OptionalString candidate = p.second->name()) {
          if (istringEqual(*candidate,name)) {
            result.push_back(WorkspaceObject(p.second));
          }
        }
      }
    }
    else {
      std::string baseName = getBaseName(name);
      boost::regex baseNameRegex = getBaseNameRegex(baseName);
      BOOST_FOREACH(const WorkspaceObjectMap::value_type& p,m_workspaceObjectMap) {
        if (OptionalString candidate = p.second->name()) {
          std::string lcCandidate = candidate.get();
          boost::to_lower(lcCandidate);
          if (boost::regex_match(lcCandidate,baseNameRegex)) {
            result.push_back(WorkspaceObject(p.second));
          }
        }
      }
    }
    return result;
  }

  std::vector<WorkspaceObject> Workspace_Impl::getObjectsByType(IddObjectType objectType) const {
    IddObjectTypeMap::const_iterator loc = m_iddObjectTypeMap.find(objectType);
    if (loc == m_iddObjectTypeMap.end()) { return WorkspaceObjectVector(); }
    return getObjects(handles(loc->second)); // convert set to vector, then getObjects
  }

  std::vector<WorkspaceObject> Workspace_Impl::getObjectsByType(const IddObject& objectType) const {
    WorkspaceObjectVector result;
    BOOST_FOREACH(const WorkspaceObject& object,objects()) {
      if (object.iddObject() == objectType) {
        result.push_back(object);
      }
    }
    return result;
  }

  boost::optional<WorkspaceObject> Workspace_Impl::getObjectByTypeAndName(
      IddObjectType objectType,const std::string& name) const
  {
    OptionalWorkspaceObject result;
    BOOST_FOREACH(const WorkspaceObject& object,getObjectsByType(objectType)) {
      OptionalString candidate = object.name();
      if (candidate && istringEqual(*candidate,name)) {
        result = object;
        break;
      }
    }
    return result;
  }

  std::vector<WorkspaceObject> Workspace_Impl::getObjectsByTypeAndName(
      IddObjectType objectType,
      const std::string& name) const
  {
    WorkspaceObjectVector result;
    std::string baseName = getBaseName(name);
    boost::regex baseNameRegex = getBaseNameRegex(baseName);
    BOOST_FOREACH(const WorkspaceObject& object,getObjectsByType(objectType)) {
      if (OptionalString candidate = object.name()) {
        std::string lcCandidate = candidate.get();
        boost::to_lower(lcCandidate);
        if (boost::regex_match(lcCandidate,baseNameRegex)) {
          result.push_back(object);
        }
      }
    }
    return result;
  }

  std::vector<WorkspaceObject> Workspace_Impl::getObjectsByReference(
      const std::string& referenceName) const
  {
    IdfReferencesMap::const_iterator loc = m_idfReferencesMap.find(referenceName);
    if (loc == m_idfReferencesMap.end()) {
      return WorkspaceObjectVector();
    }
    return getObjects(handles(loc->second)); // convert set to vector, then getObjects
  }

  std::vector<WorkspaceObject> Workspace_Impl::getObjectsByReference(
      const std::vector<std::string>& referenceNames) const
  {
    HandleSet hs;
    BOOST_FOREACH(const std::string& referenceName,referenceNames) {
      IdfReferencesMap::const_iterator loc = m_idfReferencesMap.find(referenceName);
      if (loc != m_idfReferencesMap.end()) {
        BOOST_FOREACH(const Handle& h,loc->second) {
          hs.insert(h);
        }
      }
    }
    return getObjects(handles(hs)); // convert set to vector, then getObjects
  }

  boost::optional<WorkspaceObject> Workspace_Impl::getObjectByNameAndReference(
      std::string name,
      const std::vector<std::string>& referenceNames) const
  {
    OptionalWorkspaceObject result;
    boost::to_lower(name);
    BOOST_FOREACH(const WorkspaceObject& object,getObjectsByReference(referenceNames)) {
      std::string candidateName = object.name().get();
      boost::to_lower(candidateName);
      if (candidateName == name) {
        result = object;
        break;
      }
    }
    return result;
  }

  bool Workspace_Impl::fastNaming() const
  {
    return m_fastNaming;
  }

  // SETTERS

  bool Workspace_Impl::setStrictnessLevel(StrictnessLevel level) {
    if (isValid(level)) {
      m_strictnessLevel = level;
      return true;
    }
    return false;
  }

  // Helper function to start the process of adding an object to the workspace.
  boost::shared_ptr<WorkspaceObject_Impl> Workspace_Impl::createObject(const IdfObject& object,
                                                                       bool keepHandle)
  {
    return WorkspaceObject_ImplPtr(new WorkspaceObject_Impl(object,this,keepHandle));
  }

  // Helper function to start the process of adding a cloned object to the workspace.
  WorkspaceObject_ImplPtr Workspace_Impl::createObject(
        const boost::shared_ptr<WorkspaceObject_Impl>& originalObjectImplPtr,
        bool keepHandle)
  {
    OS_ASSERT(originalObjectImplPtr);
    return WorkspaceObject_ImplPtr(new WorkspaceObject_Impl(*originalObjectImplPtr,
                                                            this,
                                                            keepHandle));
  }

  std::vector<WorkspaceObject> Workspace_Impl::addObjects(
      std::vector<boost::shared_ptr<WorkspaceObject_Impl> >& objectImplPtrs,
      const std::vector<UHPointer>& pointersIntoWorkspace,
      const std::vector<HUPointer>& pointersFromWorkspace,
      bool driverMethod,
      bool expectToLosePointers)
  {
    HandleVector newHandles;
    WorkspaceObjectVector newObjects;

    int i = 0;
    int N = objectImplPtrs.size();
    emit progressRange(0, 3*N);
    emit progressValue(0);
    emit progressCaption("Adding Objects");

    // step 1: add to maps
    bool ok = true;
    BOOST_FOREACH(WorkspaceObject_ImplPtr& ptr,objectImplPtrs) {
      ok = ok && nominallyAddObject(ptr); // will fail if ptr already in map
      if (ok) {
        newHandles.push_back(ptr->handle());
        LOG(Trace,"Adding object with handle " << newHandles.back());
      }
      else {
        LOG(Error,"Tried to add two objects with the same handle: " << ptr->handle());
      }
      emit progressValue(++i);
    }

    // step 2: replace string pointers
    if (ok){
      BOOST_FOREACH(WorkspaceObject_ImplPtr& ptr,objectImplPtrs) {
        ptr->initializeOnAdd(expectToLosePointers);
        emit progressValue(++i);
      }
    }

    // step 3: handle provided relationships
    if (ok && (!pointersIntoWorkspace.empty() || !pointersFromWorkspace.empty())) {
      ok = ok && addProvidedRelationships(newHandles,pointersIntoWorkspace,pointersFromWorkspace);
    }

    // step 4: register initialization
    if (ok){
      BOOST_FOREACH(WorkspaceObject_ImplPtr& ptr, objectImplPtrs) {
        ptr->setInitialized();
        newObjects.push_back(WorkspaceObject(ptr));
        emit progressValue(++i);
      }
    }

    // step 5: check validity
    if (ok && driverMethod) {
      StrictnessLevel level = strictnessLevel();
      if ((objectImplPtrs.size() == numAllObjects()) || (level == StrictnessLevel::Final)) {
        // check whole workspace
        ok = isValid();
      }
      else {
        // check individual objects
        BOOST_FOREACH(const WorkspaceObject& newObject, newObjects) {
          ok = ok && newObject.isValid(level);
          if (!ok) {
            break;
          }
        }
      }
    }

    // step 6: rollback if necessary
    if (!ok) {
      LOG(Info,"Unable to add objects to Workspace. The validity report is: " <<
          std::endl << validityReport());
      nominallyRemoveObjects(newHandles); // no validity check
      BOOST_FOREACH(WorkspaceObject_ImplPtr& ptr, objectImplPtrs) {
        ptr->disconnect();
      }
      newObjects.clear();
      return newObjects;
    }

    // step 7: emit signals for successful completion
    if (driverMethod) {
      BOOST_FOREACH(const WorkspaceObject& newObject, newObjects) {
        registerAdditionOfObject(newObject);
      }
    }

    return newObjects;
  }

  std::vector<WorkspaceObject> Workspace_Impl::addClones(
      std::vector< boost::shared_ptr<WorkspaceObject_Impl> >& objectImplPtrs,
      const HandleMap& oldNewHandleMap,
      bool collectionClone,
      const std::vector<UHPointer>& pointersIntoWorkspace,
      const std::vector<HUPointer>& pointersFromWorkspace,
      bool driverMethod)
  {
    int i = 0;
    int N = objectImplPtrs.size();
    if (oldNewHandleMap.empty()) {
      emit progressRange(0, 2*N);
    }
    else {
      emit progressRange(0, 3*N);
    }
    emit progressValue(0);
    emit progressCaption("Cloning Objects");

    // step 1: add objects to maps
    HandleVector newHandles;
    BOOST_FOREACH(const WorkspaceObject_ImplPtr& ptr, objectImplPtrs) {
      newHandles.push_back(ptr->handle());
      m_workspaceObjectMap.insert(WorkspaceObjectMap::value_type(newHandles.back(),ptr));
      insertIntoIddObjectTypeMap(ptr);
      insertIntoIdfReferencesMap(ptr);
      emit progressValue(++i);
    }

    // step 2: apply handle map to pointers
    if (!oldNewHandleMap.empty()) {
      BOOST_FOREACH(const WorkspaceObject_ImplPtr& ptr, objectImplPtrs) {
        ptr->initializeOnClone(oldNewHandleMap);
        emit progressValue(++i);
      }
    }

    // step 3: apply handle map to orderer
    if (!oldNewHandleMap.empty() && m_workspaceObjectOrder.isDirectOrder()) {
      if (collectionClone) {
        // objects in order, just under wrong handles
        HandleVector directOrderVector = order().directOrder().get();
        HandleVector mappedOrder = applyHandleMap(directOrderVector,oldNewHandleMap);
        m_workspaceObjectOrder.setDirectOrder(mappedOrder);
      }
      else {
        // new objects not yet in order
        BOOST_FOREACH(const WorkspaceObject_ImplPtr& ptr, objectImplPtrs) {
          m_workspaceObjectOrder.push_back(ptr->handle());
        }
      }
    }

    // step 4: handle provided relationships
    bool ok = true;
    if (!pointersIntoWorkspace.empty() || !pointersFromWorkspace.empty()) {
      ok = ok && addProvidedRelationships(newHandles,pointersIntoWorkspace,pointersFromWorkspace);
    }

    // step 5: register initialization
    WorkspaceObjectVector newObjects;
    BOOST_FOREACH(WorkspaceObject_ImplPtr& ptr, objectImplPtrs) {
      ptr->setInitialized();
      newObjects.push_back(WorkspaceObject(ptr));
      emit progressValue(++i);
    }

    // step 6: check validity
    StrictnessLevel level = strictnessLevel();
    if (ok && driverMethod && (!collectionClone || (level == StrictnessLevel::Final))) {
      if (objectImplPtrs.size() == numObjects()) {
        // check whole workspace
        ok = isValid();
      }
      else {
        // check individual objects
        BOOST_FOREACH(const WorkspaceObject& newObject, newObjects) {
          ok = ok && newObject.isValid(level);
          if ((level > StrictnessLevel::Draft) &&
              (newObject.iddObject().properties().unique))
          {
            ok = ok && (numObjectsOfType(newObject.iddObject().type()) == 1u);
          }
          if (!ok) {
            break;
          }
        }
      }
    }

    // step 7: rollback if necessary
    if (!ok) {
      LOG(Info,"Unable to add cloned objects to Workspace. The validity report is: " <<
          std::endl << validityReport());
      nominallyRemoveObjects(newHandles); // no validity check
      BOOST_FOREACH(WorkspaceObject_ImplPtr& ptr, objectImplPtrs) {
        ptr->disconnect();
      }
      newObjects.clear();
      return newObjects;
    }

    // step 8: emit signals for successful completion
    if (driverMethod) {
      BOOST_FOREACH(const WorkspaceObject& newObject, newObjects) {
        registerAdditionOfObject(newObject);
      }
    }

    return newObjects;
  }

  boost::optional<WorkspaceObject> Workspace_Impl::addObject(const IdfObject& idfObject) {
    WorkspaceObject_ImplPtr objectImplPtr;

    // handle potential name conflicts up front
    IdfObject workingIdfObject = idfObject; // shallow copy
    if (idfObject.iddObject().hasNameField()) {
      OptionalString currentName = idfObject.name();
      if (currentName && !currentName->empty()) {
        bool potentialConflict = potentialNameConflict(idfObject.name().get(),idfObject.iddObject());
        if (potentialConflict) {
          LOG(Info,"Resetting name of " << idfObject.briefDescription()
              << " to avoid name conflict upon adding it to the Workspace.");
          workingIdfObject = idfObject.clone(); // deep copy
          // clear name of single object--WorkspaceObject_Impl constructor will make new one
          bool ok = workingIdfObject.setName("");
          OS_ASSERT(ok);
        }
      }
    }
    // create new object
    objectImplPtr = this->createObject(workingIdfObject,false);

    // add to workspace
    WorkspaceObject_ImplPtrVector objectImplPtrs(1u,objectImplPtr);
    WorkspaceObjectVector objects = addObjects(objectImplPtrs);

    // assess result and return
    if (objects.size() != 1) {
      OS_ASSERT(objects.size() == 0);
      return boost::none;
    }
    return objects[0];
  }

  boost::optional<WorkspaceObject> Workspace_Impl::insertObject(const IdfObject& idfObject) {
    OptionalWorkspaceObject result = getEquivalentObject(idfObject);
    if (!result) {
      result = addObject(idfObject);
    }
    return result;
  }

  std::vector<WorkspaceObject> Workspace_Impl::addObjects(const std::vector<IdfObject>& idfObjects) {
    WorkspaceObjectVector result;
    WorkspaceObject_ImplPtrVector newObjects;

    if (idfObjects.empty()) {
      return result;
    }

    bool keepHandles = idfObjects[0].iddObject().hasHandleField();

    bool checkedForNameConflicts(false);
    if (numObjects() > 0) {
      // handle potential name conflicts
      Workspace working = this->cloneSubset(HandleVector()); // empty clone
      working.order().setDirectOrder(HandleVector()); // maintain order in vector
      // call method like this one, but simpler, which expects to lose outward pointers
      WorkspaceObjectVector wsObjects = working.getImpl<detail::Workspace_Impl>()->simpleAddObjects(idfObjects);
      if (wsObjects.size() != idfObjects.size()) {
        LOG(Error,"Unable to add idfObjects to their own Workspace as an intermediate step.");
        return WorkspaceObjectVector();
      }
      bool changes = resolvePotentialNameConflicts(working);
      checkedForNameConflicts = true;
      if (changes) {
        // merge data and create objects
        IdfObjectVector::const_iterator it(idfObjects.begin()), itEnd(idfObjects.end());
        BOOST_FOREACH(const WorkspaceObject& object,wsObjects) {
          OS_ASSERT(it != itEnd);
          IdfObject mergedObject = object.idfObject();
          mergeIdfObjectAfterPotentialNameConflictResolution(mergedObject,*it);
          newObjects.push_back(this->createObject(mergedObject,keepHandles));
          ++it;
        }
        // add objects
        result = addObjects(newObjects);
        return result;
      }
    }

    // no name conflicts---directly create and add objects
    OS_ASSERT(newObjects.empty());
    IdfObjectVector::const_iterator it(idfObjects.begin()), itEnd(idfObjects.end());
    for (; it != itEnd; ++it) {
      newObjects.push_back(this->createObject(*it,keepHandles));
    }
    result = addObjects(newObjects);
    if (!checkedForNameConflicts) {
      Workspace thisWorkspace = workspace();
      resolvePotentialNameConflicts(thisWorkspace);
    }

    return result;
  }

  std::vector<WorkspaceObject> Workspace_Impl::insertObjects(const IdfObjectVector& idfObjects) {
    return addAndInsertObjects(IdfObjectVector(),idfObjects);
  }

  std::vector<WorkspaceObject> Workspace_Impl::addAndInsertObjects(
      const std::vector<IdfObject>& objectsToAdd, const std::vector<IdfObject>& objectsToInsert)
  {
    // merge objects into one vector
    unsigned nAdd = objectsToAdd.size();
    unsigned nInsert = objectsToInsert.size();
    unsigned n = nAdd + nInsert;
    IdfObjectVector idfObjects = objectsToAdd;
    idfObjects.insert(idfObjects.end(),objectsToInsert.begin(),objectsToInsert.end());
    OS_ASSERT(idfObjects.size() == n);

    if (idfObjects.empty()) {
      return WorkspaceObjectVector();
    }

    bool keepHandles = idfObjects[0].iddObject().hasHandleField();

    // find equivalent objects for objectsToInsert
    UnsignedVector foundObjectIndices;
    UnsignedVector notFoundObjectIndices;
    WorkspaceObjectVector equivalentObjects;
    unsigned i = nAdd;
    for (unsigned i = nAdd, n = idfObjects.size(); i < n; ++i) {
      OptionalWorkspaceObject oEquivalent = getEquivalentObject(idfObjects[i]);
      if (oEquivalent) {
        foundObjectIndices.push_back(i);
        equivalentObjects.push_back(*oEquivalent);
      }
      else {
        notFoundObjectIndices.push_back(i);
      }
    }
    OS_ASSERT(foundObjectIndices.size() + notFoundObjectIndices.size() == nInsert);

    // handle potential name conflicts. only objects to be added need attention.
    Workspace working = this->cloneSubset(HandleVector()); // empty clone
    working.order().setDirectOrder(HandleVector()); // maintain order in vector
    WorkspaceObjectVector wsObjects = working.addObjects(idfObjects);
    if (wsObjects.size() != idfObjects.size()) {
      LOG(Error,"Unable to add idfObjects to their own Workspace as an intermediate step.");
      return WorkspaceObjectVector();
    }
    bool changes = resolvePotentialNameConflicts(working,foundObjectIndices);

    // create objects while handling the mess we have made above
    WorkspaceObject_ImplPtrVector newObjects; // objects to be added
    HUPointerVector pointersFromWorkspace;    // workspace object -> object to be added
    HHPointerVector pointersToRollBackOnFail; // workspace object -> workspace object pointers
                                              // directly added by this method
    i = 0; // step through all of the objects
    WorkspaceObjectVector::iterator equivIt = equivalentObjects.begin();
    UnsignedVector::const_iterator foundIndicesBegin = foundObjectIndices.begin();
    UnsignedVector::const_iterator foundIndicesEnd = foundObjectIndices.end();
    // loop through Workspace versions of idfObjects
    BOOST_FOREACH(const WorkspaceObject& object,wsObjects) {
      if (std::find(foundIndicesBegin,foundIndicesEnd,i) == foundIndicesEnd) {
        // no equivalent object in workspace -- create new object to add
        if (changes) {
          IdfObject mergedObject = object.idfObject();
          mergeIdfObjectAfterPotentialNameConflictResolution(mergedObject,idfObjects[i]);
          newObjects.push_back(this->createObject(mergedObject,keepHandles));
        }
        else {
          newObjects.push_back(this->createObject(idfObjects[i],keepHandles));
        }
      }
      else {
        // there is an equivalent object in workspace
        OS_ASSERT(equivIt->iddObject() == object.iddObject());
        // preserve idfObject[i] -> idfObject[j]
        WorkspaceObjectVector targets = object.targets();
        BOOST_FOREACH(const WorkspaceObject& target,targets) {
          OptionalUnsigned targetIndexInWorking = working.order().indexInOrder(target.handle());
          OS_ASSERT(targetIndexInWorking);
          UnsignedVector fieldIndicesObjectToTarget = object.getSourceIndices(target.handle());
          OS_ASSERT(!fieldIndicesObjectToTarget.empty());
          UnsignedVector::const_iterator targetInFoundObjectsIt = std::find(
              foundIndicesBegin,foundIndicesEnd,*targetIndexInWorking);
          if (targetInFoundObjectsIt == foundIndicesEnd) {
            // target has no equivalent in this Workspace
            // get index target will have in newObjects vector
            unsigned newObjectsIndexOfTarget(0);
            if (*targetIndexInWorking < nAdd) {
              newObjectsIndexOfTarget = *targetIndexInWorking;
            }
            else {
              UnsignedVector::const_iterator targetInNotFoundObjectsIt =
                  std::find(notFoundObjectIndices.begin(),
                  notFoundObjectIndices.end(),
                  *targetIndexInWorking);
              OS_ASSERT(targetInNotFoundObjectsIt != notFoundObjectIndices.end());
              newObjectsIndexOfTarget = nAdd +
                  static_cast<unsigned>(targetInNotFoundObjectsIt - notFoundObjectIndices.begin());
            }
            BOOST_FOREACH(unsigned index,fieldIndicesObjectToTarget) {
              pointersFromWorkspace.push_back(HUPointer(equivIt->handle(),index,newObjectsIndexOfTarget));
            }
          }
          else {
            // target has equivalent in this Workspace. make sure pointer is there or add it.
            unsigned targetIndexInEquivalentObjects = static_cast<unsigned>(
                targetInFoundObjectsIt - foundIndicesBegin);
            WorkspaceObject equivalentTarget = equivalentObjects[targetIndexInEquivalentObjects];
            UnsignedVector fieldIndicesFoundObjectToFoundTarget = equivIt->getSourceIndices(equivalentTarget.handle());
            BOOST_FOREACH(unsigned index,fieldIndicesObjectToTarget) {
              if (std::find(fieldIndicesFoundObjectToFoundTarget.begin(),
                            fieldIndicesFoundObjectToFoundTarget.end(),
                            index) == fieldIndicesFoundObjectToFoundTarget.end())
              {
                OS_ASSERT(!(equivIt->getTarget(index)));
                bool ok = equivIt->setPointer(index,equivalentTarget.handle());
                if (ok) {
                  pointersToRollBackOnFail.push_back(HHPointer(equivIt->handle(),index,equivalentTarget.handle()));
                }
                else {
                  LOG(Warn,"Unable to move pointer from " << object.briefDescription() << " to "
                      << target.briefDescription() << " from field " << index
                      << " over to equivalent objects " << equivIt->briefDescription() << " and "
                      << equivalentTarget.briefDescription() << ", respectively, already in Workspace.");
                }
              }
            }
          } // if (target does not have an equivalent) else
        } // for each target of an object that has an equivalent
        ++equivIt;
      } // if (object does not have an equivalent) else
      ++i;
    } // for each object

    // add objects
    WorkspaceObjectVector addedObjects = addObjects(newObjects,
                                                    UHPointerVector(),
                                                    pointersFromWorkspace);
    if (addedObjects.size() != newObjects.size()) {
      BOOST_FOREACH(const HHPointer& ptr,pointersToRollBackOnFail) {
        OptionalWorkspaceObject oSource = getObject(ptr.source);
        OS_ASSERT(oSource);
        oSource->setPointer(ptr.fieldIndex,Handle());
      }
      return WorkspaceObjectVector();
    }

    // successful, make result vector
    WorkspaceObjectVector result;
    WorkspaceObjectVector::const_iterator addedIt = addedObjects.begin();
    equivIt = equivalentObjects.begin();
    OS_ASSERT(addedObjects.size() + equivalentObjects.size() == n);
    for (unsigned i = 0; i < n; ++i) {
      if (std::find(foundIndicesBegin,foundIndicesEnd,i) == foundIndicesEnd) {
        // no equivalent--added
        OS_ASSERT(addedIt != addedObjects.end());
        result.push_back(*addedIt);
        ++addedIt;
      }
      else {
        // equivalent
        OS_ASSERT(equivIt != equivalentObjects.end());
        result.push_back(*equivIt);
        ++equivIt;
      }
    }
    OS_ASSERT(addedIt == addedObjects.end());
    OS_ASSERT(equivIt == equivalentObjects.end());

    return result;
  }

  std::vector<WorkspaceObject> Workspace_Impl::addObjects(
    const std::vector<WorkspaceObject>& objects)
  {
    WorkspaceObjectVector result;
    WorkspaceObject_ImplPtrVector newObjects;
    HandleMap oldNewHandleMap;
    UHPointerVector pointersIntoWorkspace;

    if (objects.empty()) {
      return result;
    }

    if (numObjects() > 0) {

      // handle potential name conflicts
      Workspace working = this->cloneSubset(HandleVector()); // empty clone
      working.order().setDirectOrder(HandleVector()); // maintain order in vector
      WorkspaceObjectVector wsObjects = working.addObjects(objects); // recursive call
      if (wsObjects.size() != objects.size()) {
        LOG(Error,"Unable to add objects to their own Workspace as an intermediate step.");
        return result;
      }
      bool changes = resolvePotentialNameConflicts(working);

      // create new objects, and keep track of outward pointers if
      // objects[0].workspace() == workspace()
      bool sameWorkspace = false;
      if (objects[0].workspace() == this->workspace()) {
        sameWorkspace = true;
        OS_ASSERT(changes);
      }
      unsigned i(0);
      BOOST_FOREACH(const WorkspaceObject& wsObject,wsObjects) {
        newObjects.push_back(this->createObject(wsObject.getImpl<WorkspaceObject_Impl>(),false));
        oldNewHandleMap.insert(HandleMap::value_type(wsObject.handle(),newObjects.back()->handle()));
        if (sameWorkspace) {
          WorkspaceObjectVector objectTargets = objects[i].targets();
          BOOST_FOREACH(const WorkspaceObject target,objectTargets) {
            if (std::find(objects.begin(),objects.end(),target) == objects.end()) {
              UnsignedVector fieldIndices = objects[i].getSourceIndices(target.handle());
              BOOST_FOREACH(unsigned index,fieldIndices) {
                pointersIntoWorkspace.push_back(UHPointer(i,index,target.handle()));
              }
            }
          }
        }
        ++i;
      }

      result = addClones(newObjects,oldNewHandleMap,false,pointersIntoWorkspace);
      return result;

    }

    // blank Workspace---directly create and add objects
    OS_ASSERT(newObjects.empty());
    OS_ASSERT(oldNewHandleMap.empty());
    unsigned i(0);
    BOOST_FOREACH(const WorkspaceObject& object, objects) {
      newObjects.push_back(this->createObject(object.getImpl<WorkspaceObject_Impl>(),false));
      oldNewHandleMap.insert(HandleMap::value_type(object.handle(),newObjects.back()->handle()));
      ++i;
    }
    result = addClones(newObjects,oldNewHandleMap,false);


    return result;
  }

  std::vector<WorkspaceObject> Workspace_Impl::insertObjects(
      const std::vector<WorkspaceObject>& objects)
  {
    return addAndInsertObjects(WorkspaceObjectVector(),objects);
  }

  std::vector<WorkspaceObject> Workspace_Impl::addAndInsertObjects(
        const std::vector<WorkspaceObject>& objectsToAdd,
        const std::vector<WorkspaceObject>& objectsToInsert)
  {
    // merge objects into one vector
    unsigned nAdd = objectsToAdd.size();
    unsigned nInsert = objectsToInsert.size();
    unsigned n = nAdd + nInsert;
    WorkspaceObjectVector allObjects = objectsToAdd;
    allObjects.insert(allObjects.end(),objectsToInsert.begin(),objectsToInsert.end());
    OS_ASSERT(allObjects.size() == n);

    if (allObjects.empty()) {
      return WorkspaceObjectVector();
    }

    // find equivalent objects for objectsToInsert
    UnsignedVector foundObjectIndices;
    UnsignedVector notFoundObjectIndices;
    WorkspaceObjectVector equivalentObjects;
    for (unsigned i = nAdd, n = allObjects.size(); i < n; ++i) {
      OptionalWorkspaceObject oEquivalent = getEquivalentObject(allObjects[i]);
      if (oEquivalent) {
        foundObjectIndices.push_back(i);
        equivalentObjects.push_back(*oEquivalent);
      }
      else {
        notFoundObjectIndices.push_back(i);
      }
    }
    OS_ASSERT(foundObjectIndices.size() + notFoundObjectIndices.size() == nInsert);

    return addAndInsertObjects(allObjects,
                                  foundObjectIndices,
                                  notFoundObjectIndices,
                                  equivalentObjects);
  }

  std::vector<WorkspaceObject> Workspace_Impl::addAndInsertObjects(
      const std::vector<WorkspaceObject>& objectsToAdd,
      const std::vector< std::vector<WorkspaceObject> >& objectsToInsert)
  {
    UnsignedVector foundObjectIndices;
    UnsignedVector notFoundObjectIndices;
    WorkspaceObjectVector equivalentObjects;

    // merge objects into one vector and find equivalent objects for objectsToInsert
    unsigned nAdd = objectsToAdd.size();
    WorkspaceObjectVector allObjects = objectsToAdd;
    unsigned i(nAdd);
    unsigned nInsert(0);
    BOOST_FOREACH(const std::vector<WorkspaceObject>& group,objectsToInsert) {
      UnsignedVector groupIndices;
      WorkspaceObjectVector tempEquivalents;
      BOOST_FOREACH(const WorkspaceObject& object,group) {
        groupIndices.push_back(i);
        allObjects.push_back(object);
        OptionalWorkspaceObject oEquivalent = getEquivalentObject(allObjects[i]);
        if (oEquivalent) {
          tempEquivalents.push_back(*oEquivalent);
        }
        ++i;
      }
      nInsert += groupIndices.size();
      if (tempEquivalents.size() == groupIndices.size()) {
        // all have equivalents
        foundObjectIndices.insert(foundObjectIndices.end(),groupIndices.begin(),groupIndices.end());
        equivalentObjects.insert(equivalentObjects.end(),tempEquivalents.begin(),tempEquivalents.end());
      }
      else {
        notFoundObjectIndices.insert(notFoundObjectIndices.end(),groupIndices.begin(),groupIndices.end());
      }
    }

    OS_ASSERT(allObjects.size() == nAdd + nInsert);
    OS_ASSERT(foundObjectIndices.size() + notFoundObjectIndices.size() == nInsert);

    if (allObjects.empty()) {
      return WorkspaceObjectVector();
    }

    return addAndInsertObjects(allObjects,
                                  foundObjectIndices,
                                  notFoundObjectIndices,
                                  equivalentObjects);
  }

  std::vector<WorkspaceObject> Workspace_Impl::addAndInsertObjects(
      const std::vector<WorkspaceObject>& allObjects,
      const std::vector<unsigned>& foundObjectIndices,
      const std::vector<unsigned>& notFoundObjectIndices,
      std::vector<WorkspaceObject>& equivalentObjects)
  {
    WorkspaceObjectVector result;
    WorkspaceObject_ImplPtrVector newObjects;
    HandleMap oldNewHandleMap;
    UHPointerVector pointersIntoWorkspace;
    HUPointerVector pointersFromWorkspace;

    unsigned n = allObjects.size();
    unsigned nInsert = foundObjectIndices.size() + notFoundObjectIndices.size();
    unsigned nAdd = n - nInsert;

    // handle potential name conflicts. only objects to be added need attention.
    Workspace working = this->cloneSubset(HandleVector()); // empty clone
    working.order().setDirectOrder(HandleVector()); // maintain order in vector
    WorkspaceObjectVector wsObjects = working.addObjects(allObjects);
    if (wsObjects.size() != allObjects.size()) {
      LOG(Error,"Unable to add objectsToAdd and objectsToInsert to their own Workspace as an "
          << "intermediate step.");
      return result;
    }
    resolvePotentialNameConflicts(working,foundObjectIndices);

    // create objects while handling the mess we have made above
    HHPointerVector pointersToRollBackOnFail; // workspace object -> workspace object pointers
                                              // directly added by this method
    unsigned i = 0; // step through all of the objects
    unsigned j = 0; // index for new objects
    WorkspaceObjectVector::iterator equivIt = equivalentObjects.begin();
    UnsignedVector::const_iterator foundIndicesBegin = foundObjectIndices.begin();
    UnsignedVector::const_iterator foundIndicesEnd = foundObjectIndices.end();
    // keep track of outward pointers if objects[0].workspace() == workspace()
    bool sameWorkspace = false;
    if (allObjects[0].workspace() == this->workspace()) {
      sameWorkspace = true;
    }
    // loop through working versions of allObjects
    BOOST_FOREACH(const WorkspaceObject& object,wsObjects) {
      if (std::find(foundIndicesBegin,foundIndicesEnd,i) == foundIndicesEnd) {
        // no equivalent object in workspace, create new object
        newObjects.push_back(this->createObject(object.getImpl<detail::WorkspaceObject_Impl>(),false));
        oldNewHandleMap.insert(HandleMap::value_type(object.handle(),newObjects.back()->handle()));
        WorkspaceObjectVector objectTargets = object.targets();
        // preserve pointers to equivalent objects
        BOOST_FOREACH(const WorkspaceObject target,objectTargets) {
          OptionalUnsigned targetIndexInWorking = working.order().indexInOrder(target.handle());
          OS_ASSERT(targetIndexInWorking);
          UnsignedVector::const_iterator targetInFoundObjectsIt = std::find(
              foundIndicesBegin,foundIndicesEnd,*targetIndexInWorking);
          if (targetInFoundObjectsIt != foundIndicesEnd) {
            // set pointer(s) to equivalent object
            UnsignedVector fieldIndicesObjectToTarget = object.getSourceIndices(target.handle());
            OS_ASSERT(!fieldIndicesObjectToTarget.empty());
            unsigned targetIndexInEquivalentObjects = static_cast<unsigned>(
                targetInFoundObjectsIt - foundIndicesBegin);
            WorkspaceObject equivalentTarget = equivalentObjects[targetIndexInEquivalentObjects];
            BOOST_FOREACH(unsigned index,fieldIndicesObjectToTarget) {
              pointersIntoWorkspace.push_back(UHPointer(j,index,equivalentTarget.handle()));
            }
          }
        }
        if (sameWorkspace) {
          // preserve external targets too
          objectTargets = allObjects[i].targets();
          BOOST_FOREACH(const WorkspaceObject target,objectTargets) {
            if (std::find(allObjects.begin(),allObjects.end(),target) == allObjects.end()) {
              UnsignedVector fieldIndices = allObjects[i].getSourceIndices(target.handle());
              BOOST_FOREACH(unsigned index,fieldIndices) {
                pointersIntoWorkspace.push_back(UHPointer(j,index,target.handle()));
              }
            }
          }
        }
        ++j;
      }
      else {
        // there is an equivalent object in workspace
        OS_ASSERT(equivIt->iddObject() == object.iddObject());
        // preserve idfObject[i] -> idfObject[j]
        WorkspaceObjectVector targets = object.targets();
        BOOST_FOREACH(const WorkspaceObject& target,targets) {
          OptionalUnsigned targetIndexInWorking = working.order().indexInOrder(target.handle());
          OS_ASSERT(targetIndexInWorking);
          UnsignedVector fieldIndicesObjectToTarget = object.getSourceIndices(target.handle());
          OS_ASSERT(!fieldIndicesObjectToTarget.empty());
          UnsignedVector::const_iterator targetInFoundObjectsIt = std::find(
              foundIndicesBegin,foundIndicesEnd,*targetIndexInWorking);
          if (targetInFoundObjectsIt == foundIndicesEnd) {
            // target has no equivalent in this Workspace
            // get index target will have in newObjects vector
            unsigned newObjectsIndexOfTarget(0);
            if (*targetIndexInWorking < nAdd) {
              newObjectsIndexOfTarget = *targetIndexInWorking;
            }
            else {
              UnsignedVector::const_iterator targetInNotFoundObjectsIt =
                  std::find(notFoundObjectIndices.begin(),
                  notFoundObjectIndices.end(),
                  *targetIndexInWorking);
              OS_ASSERT(targetInNotFoundObjectsIt != notFoundObjectIndices.end());
              newObjectsIndexOfTarget = nAdd +
                  static_cast<unsigned>(targetInNotFoundObjectsIt - notFoundObjectIndices.begin());
            }
            BOOST_FOREACH(unsigned index,fieldIndicesObjectToTarget) {
              pointersFromWorkspace.push_back(HUPointer(equivIt->handle(),index,newObjectsIndexOfTarget));
            }
          }
          else {
            // target has equivalent in this Workspace. make sure pointer is there or add it.
            unsigned targetIndexInEquivalentObjects = static_cast<unsigned>(
                targetInFoundObjectsIt - foundIndicesBegin);
            WorkspaceObject equivalentTarget = equivalentObjects[targetIndexInEquivalentObjects];
            UnsignedVector fieldIndicesFoundObjectToFoundTarget = equivIt->getSourceIndices(equivalentTarget.handle());
            BOOST_FOREACH(unsigned index,fieldIndicesObjectToTarget) {
              if (std::find(fieldIndicesFoundObjectToFoundTarget.begin(),
                            fieldIndicesFoundObjectToFoundTarget.end(),
                            index) == fieldIndicesFoundObjectToFoundTarget.end())
              {
                OS_ASSERT(!(equivIt->getTarget(index)));
                bool ok = equivIt->setPointer(index,equivalentTarget.handle());
                if (ok) {
                  pointersToRollBackOnFail.push_back(HHPointer(equivIt->handle(),index,equivalentTarget.handle()));
                }
                else {
                  LOG(Warn,"Unable to move pointer from " << object.briefDescription() << " to "
                      << target.briefDescription() << " from field " << index
                      << " over to equivalent objects " << equivIt->briefDescription() << " and "
                      << equivalentTarget.briefDescription() << ", respectively, already in Workspace.");
                }
              }
            }
          } // if (target does not have an equivalent) else
        } // for each target of an object that has an equivalent
        ++equivIt;
      } // if (object does not have an equivalent) else
      ++i;
    } // for each object

    // add objects
    WorkspaceObjectVector addedObjects = addClones(newObjects,
                                                   oldNewHandleMap,
                                                   false,
                                                   pointersIntoWorkspace,
                                                   pointersFromWorkspace);
    if (addedObjects.size() != newObjects.size()) {
      BOOST_FOREACH(const HHPointer& ptr,pointersToRollBackOnFail) {
        OptionalWorkspaceObject oSource = getObject(ptr.source);
        OS_ASSERT(oSource);
        oSource->setPointer(ptr.fieldIndex,Handle());
      }
      return result;
    }

    // successful, make result vector
    WorkspaceObjectVector::const_iterator addedIt = addedObjects.begin();
    equivIt = equivalentObjects.begin();
    OS_ASSERT(addedObjects.size() + equivalentObjects.size() == unsigned(n));
    for (unsigned i = 0; i < n; ++i) {
      if (std::find(foundIndicesBegin,foundIndicesEnd,i) == foundIndicesEnd) {
        OS_ASSERT(addedIt != addedObjects.end());
        result.push_back(*addedIt);
        ++addedIt;
      }
      else {
        // equivalent
        OS_ASSERT(equivIt != equivalentObjects.end());
        result.push_back(*equivIt);
        ++equivIt;
      }
    }
    OS_ASSERT(addedIt == addedObjects.end());
    OS_ASSERT(equivIt == equivalentObjects.end());

    return result;
  }

  bool Workspace_Impl::swap(WorkspaceObject& currentObject,IdfObject& newObject,bool keepTargets)
  {
    // make sure currentObject is in workspace
    if (!isMember(currentObject.handle())) {
      LOG(Info,"Unable to swap objects because WorkspaceObject is not in this Workspace.");
      return false;
    }

    std::stringstream ss; // placeholder for information common to all log messages
    ss << "The WorkspaceObject is of type " << currentObject.iddObject().name()
       << ", and the IdfObject is of type " << newObject.iddObject().name() << ".";

    StringVector curRefs = currentObject.iddObject().references();
    StringVector newRefs = newObject.iddObject().references();
    if (currentObject.iddObject().type() != newObject.iddObject().type()) {
      // make sure there is some overlap in references
      StringVector intersection = intersectReferenceLists(curRefs,newRefs);
      if (intersection.empty()) {
        LOG(Info,"Unable to swap objects because the two objects' reference lists have an empty "
            << "intersection. " << ss.str());
        return false;
      }
    }

    // place newObject in its own workspace
    Workspace working = this->cloneSubset(HandleVector());
    OptionalWorkspaceObject newObjectInWS = working.addObject(newObject);
    if (!newObjectInWS) { return false; }

    // rename newObject if necessary
    OptionalString newObjectName = newObjectInWS->name();
    OptionalString currentObjectName = currentObject.name();
    if (newObjectName) {
      // placing in workspace will provide name
      OS_ASSERT(!(newObjectName->empty()));
      // okay if name same as originalObject
      if (!currentObjectName || (!istringEqual(*newObjectName,*currentObjectName))) {
        if (potentialNameConflict(*newObjectName,newObjectInWS->iddObject())) {
          OptionalString ok = newObjectInWS->setName(nextName(newObjectInWS->iddObject().type(),false));
          OS_ASSERT(ok);
          LOG(Info,"Renaming " << newObject.briefDescription() << " during swap operation to "
              << "avoid name conflict.");
        }
      }
    }

    // check each source to see if pointer can be repointed
    // and accumulate information for doing the swap
    HUPointerVector sources; // from workspace
    BOOST_FOREACH(const WorkspaceObject& source,currentObject.sources()) {
      UnsignedVector indices = source.getSourceIndices(currentObject.handle());
      OS_ASSERT(!indices.empty());
      BOOST_FOREACH(unsigned index,indices) {
        if (!source.canBeSource(index,newRefs)) {
          LOG(Info,"Unable to swap objects because one of the WorkspaceObject's sources, which is "
              << "of type " << source.iddObject().name() <<", cannot point to the IdfObject from "
              << "field " << index << ". " << ss.str());
          return false;
        }
        sources.push_back(HUPointer(source.handle(),index,0));
      }
    }

    // construct targets for newObject
    UHPointerVector targets; // into workspace
    UnsignedSet spokenFor; // maintain uniqueness of newObject pointers
    if (keepTargets) {
      // first try to pull currentObject targets over
      BOOST_FOREACH(unsigned index,currentObject.objectListFields()) {
        OptionalWorkspaceObject owo = currentObject.getTarget(index);
        if (owo) {
          // newObject must be able to point to the same target
          WorkspaceObject target = *owo;
          OptionalString targetName = target.name();
          OS_ASSERT(targetName);
          // loop through candidate fields
          bool found = false;
          BOOST_FOREACH(unsigned i,newObject.objectListFields()) {
            if (spokenFor.find(i) != spokenFor.end()) { continue; }
            // data null or equal to *targetName?
            OptionalString newField = newObject.getString(i);
            OS_ASSERT(newField);
            if (newField->empty() || istringEqual(*newField,*targetName)) {
              // object lists and target references have non-empty intersection?
              OptionalIddField newOLIdd = newObject.iddObject().getField(i);
              OS_ASSERT(newOLIdd);
              StringVector tRefs = target.iddObject().references();
              StringVector newObjLists = newOLIdd->properties().objectLists;
              StringVector intersection = intersectReferenceLists(tRefs,newObjLists);
              if (!intersection.empty()) {
                // save target data
                targets.push_back(UHPointer(0,i,target.handle()));
                found = true;
                spokenFor.insert(i);
                break;
              }
            }
          } // for each newObject.objectListFields()
          if (!found) {
            LOG(Info,"Unable to swap objects because the IdfObject cannot be made to point to one of "
                << "the WorkspaceObject's targets, which is of type " << target.iddObject().name()
                << " and named '" << *targetName << "'. " << ss.str() << " The IdfObject in question "
                << "is:" << std::endl << newObject);
            return false;
          }
        } // if non-null target
      } // for each currentObject.objectListFields()
    } // if keepTargets

    BOOST_FOREACH(unsigned i,newObject.objectListFields()) {
      // if not spoken for ...
      if (std::find(spokenFor.begin(),spokenFor.end(),i) == spokenFor.end()) {
        // ... try to find target in workspace
        OptionalString targetName = newObject.getString(i);
        OS_ASSERT(targetName);
        OptionalIddField iddField = newObject.iddObject().getField(i);
        OS_ASSERT(iddField);
        OptionalWorkspaceObject oTarget = getObjectByNameAndReference(
            *targetName,iddField->properties().objectLists);
        if (oTarget) {
          targets.push_back(UHPointer(0,i,oTarget->handle()));
        }
      }
    }

    // compatible--try to add newObject
    WorkspaceObject_ImplPtrVector newObjectPtrs;
    OS_ASSERT(working.numObjects() == 1);
    newObjectPtrs.push_back(this->createObject(working.objects()[0].getImpl<WorkspaceObject_Impl>(),false));
    WorkspaceObjectVector addResult = Workspace_Impl::addObjects(newObjectPtrs,targets,sources,false);
    if (!addResult.empty()) {
      OS_ASSERT(addResult.size() == 1);
      // sucessful add, prepare to swap data by preserving current object
      IdfObject idfCurrentObject = currentObject.idfObject();
      // preserve order if directOrder
      if (order().isDirectOrder()) {
        OptionalUnsigned position = order().indexInOrder(currentObject.handle());
        if (position) {
          order().move(addResult[0].handle(),*position);
        }
      }
      // try to remove old workspace object
      bool ok = removeObject(currentObject.handle()); // checks validity and emits remove signal
      if (ok) {
        currentObject = addResult[0];
        newObject = idfCurrentObject;
        // recover original name if new and current names were same
        if (newObjectName && currentObjectName && (istringEqual(*newObjectName,*currentObjectName))) {
          LOG(Info,"Restoring original name, '" << *newObjectName << "' of "
              << currentObject.briefDescription() << " after a successful swap with "
              << newObject.briefDescription() << ".");
          ok = currentObject.getImpl<WorkspaceObject_Impl>()->setName(*newObjectName,false);
          OS_ASSERT(ok);
        }
        // handle add signals
        registerAdditionOfObject(addResult[0]);
        return true;
      }
      LOG(Info,"Unable to swap objects because the removal of the WorkspaceObject, " << std::endl
          << idfCurrentObject << std::endl << ", was unsuccessful.");
      // remove new object
      nominallyRemoveObject(addResult[0].handle());
    }
    else {
      LOG(Info,"Unable to swap objects because the addition of the IdfObject, " << std::endl
          << newObject << std::endl << ", was unsuccessful. In addition to the object itself, "
          << "there were " << sources.size() << " pointers to the object, and " << targets.size()
          << "pointers from the object to set.");
    }

    // reset source pointers
    BOOST_FOREACH(HUPointer sourcePtr,sources) {
      OptionalWorkspaceObject source = getObject(sourcePtr.source);
      OS_ASSERT(source);
      source->getImpl<WorkspaceObject_Impl>()->setPointer(sourcePtr.fieldIndex,
                                                          currentObject.handle(),
                                                          false);
    }

    return false;
  }

  bool Workspace_Impl::removeObject(const Handle& handle) {

    OptionalSavedWorkspaceObject objectData = savedWorkspaceObject(handle);
    if (!objectData) {
      return true;
    } // trivially satisfied

    emit removeWorkspaceObject(WorkspaceObject(objectData->objectImplPtr), objectData->objectImplPtr->iddObject().type(), objectData->handle);
    emit removeWorkspaceObject(objectData->objectImplPtr, objectData->objectImplPtr->iddObject().type(), objectData->handle);

    // actual work of removing from maps--is always successful
    WorkspaceObjectVector sources = nominallyRemoveObject(handle);


    // can only be invalid if removal results in null and required
    if ((m_strictnessLevel < StrictnessLevel::Final) || isValid()) {
      std::vector<Handle> removedHandles(1, handle);
      registerRemovalOfObject(objectData->objectImplPtr,sources,removedHandles);
      emit onChange();
      return true;
    }
    else {
      restoreObject(*objectData);
      return false;
    }

  }

  bool Workspace_Impl::removeObjects(const std::vector<Handle>& handles) {

    if (handles.empty()) { return true; }

    SavedWorkspaceObjectVector objectData;
    BOOST_FOREACH(const Handle& handle,handles) {
      OptionalSavedWorkspaceObject candidate = savedWorkspaceObject(handle);
      if (candidate) {
        objectData.push_back(*candidate);
      }
    }

    BOOST_FOREACH(SavedWorkspaceObject savedObject, objectData) {
      emit removeWorkspaceObject(WorkspaceObject(savedObject.objectImplPtr), savedObject.objectImplPtr->iddObject().type(), savedObject.handle);
      emit removeWorkspaceObject(savedObject.objectImplPtr, savedObject.objectImplPtr->iddObject().type(), savedObject.handle);
    }

    // actual work of removing from maps--is always successful
    std::vector<WorkspaceObjectVector> sources = nominallyRemoveObjects(handles);

    if ((m_strictnessLevel < StrictnessLevel::Final) || isValid()) {
      registerRemovalOfObjects(objectData,sources,handles);
      emit onChange();
      return true;
    }
    else {
      restoreObjects(objectData);
      return false;
    }
  }

  void Workspace_Impl::forwardReferences(const Handle& sourceHandle,
                                         unsigned index,
                                         const Handle& targetHandle) {

    // get source object
    OptionalWorkspaceObject owo = getObject(sourceHandle);
    OS_ASSERT(owo);
    WorkspaceObject sourceObject = *owo;

    // get reference lists and add targetHandle to them (ok if insert fails)
    OptionalIddField iddField = sourceObject.iddObject().getField(index);
    OS_ASSERT(iddField);
    BOOST_FOREACH(const std::string& referenceName,iddField->properties().references) {
      m_idfReferencesMap[referenceName].insert(targetHandle);
    }
  }

  void Workspace_Impl::removeForwardedReferences(
      const Handle& sourceHandle,
      unsigned index,
      const WorkspaceObject& targetObject)
  {
    // get source object
    OptionalWorkspaceObject owo = getObject(sourceHandle);
    OS_ASSERT(owo);
    WorkspaceObject srcObj = *owo;

    // only do something if references were forwarded
    OptionalIddField iddField = srcObj.iddObject().getField(index);
    if (!iddField->properties().references.empty()) {
      // get other source objects
      WorkspaceObjectVector sourceObjects = targetObject.sources();
      std::vector<UnsignedVector> sourceIndices;
      // get source index for each source object
      BOOST_FOREACH(const WorkspaceObject& sourceObject,sourceObjects) {
        OS_ASSERT(sourceObject.handle() != sourceHandle); // pointer already nullified
        UnsignedVector temp = sourceObject.getSourceIndices(targetObject.handle());
        OS_ASSERT(!temp.empty());
        sourceIndices.push_back(temp);
      }
      // loop through reference names
      BOOST_FOREACH(const std::string& referenceName,iddField->properties().references) {
        // see if name is forwarded by any other source objects
        bool found = false;
        std::vector<UnsignedVector>::const_iterator indIt = sourceIndices.begin();
        for (WorkspaceObjectVector::const_iterator objIt = sourceObjects.begin(),
             objItEnd = sourceObjects.end(); objIt != objItEnd; ++objIt, ++indIt) {
          BOOST_FOREACH(unsigned index,*indIt) {
            OptionalIddField sourceIddField = (*objIt).iddObject().getField(index);
            OS_ASSERT(sourceIddField);
            StringVector sourceFieldRefs = sourceIddField->properties().references;
            if (std::find_if(sourceFieldRefs.begin(),sourceFieldRefs.end(),
                boost::bind(istringEqual,_1,referenceName)) != sourceFieldRefs.end()) {
              found = true;
              break;
            }
          }
        }
        // if not, erase the reference
        if (!found) {
          HandleSet::iterator it = m_idfReferencesMap[referenceName].find(targetObject.handle());
          OS_ASSERT(it != m_idfReferencesMap[referenceName].end());
          m_idfReferencesMap[referenceName].erase(it);
        }
      }
    }
  }

  void Workspace_Impl::setFastNaming(bool fastNaming)
  {
    m_fastNaming = fastNaming;
  }

  // OBJECT ORDER

  WorkspaceObjectOrder Workspace_Impl::order() {
    return m_workspaceObjectOrder;
  }

  const WorkspaceObjectOrder Workspace_Impl::order() const {
    return m_workspaceObjectOrder;
  }

  std::vector<WorkspaceObject> Workspace_Impl::sort(
      const std::vector<WorkspaceObject>& objects) const
  {
    return m_workspaceObjectOrder.sort(objects);
  }

  std::vector<Handle> Workspace_Impl::sort(const std::vector<Handle>& handles) const {
    return m_workspaceObjectOrder.sort(handles);
  }

  // QUERIES

  unsigned Workspace_Impl::numObjects() const {
    return objects().size();
  }

  unsigned Workspace_Impl::numAllObjects() const {
    return m_workspaceObjectMap.size();
  }

  unsigned Workspace_Impl::numObjectsOfType(IddObjectType type) const {
    IddObjectTypeMap::const_iterator iotmLoc = m_iddObjectTypeMap.find(type);
    if (iotmLoc == m_iddObjectTypeMap.end()) { return 0; }
    return iotmLoc->second.size();
  }

  unsigned Workspace_Impl::numObjectsOfType(const IddObject& objectType) const {
    return getObjectsByType(objectType).size();
  }

  bool Workspace_Impl::isMember(const Handle& handle) const {
    WorkspaceObjectMap::const_iterator womIt = m_workspaceObjectMap.find(handle);
    return (womIt != m_workspaceObjectMap.end());
  }

  bool Workspace_Impl::canBeTarget(const Handle& handle,
                                   const std::set<std::string>& referenceListNames) const
  {
    BOOST_FOREACH(const std::string& referenceName,referenceListNames) {
      if (istringEqual(referenceName,"AllObjects")) {
        return true;
      }
      IdfReferencesMap::const_iterator irmLoc = m_idfReferencesMap.find(referenceName);
      if (irmLoc != m_idfReferencesMap.end()) {
        HandleSet::const_iterator it = irmLoc->second.find(handle);
        if (it != irmLoc->second.end()) {
          return true;
        }
      }
    }
    return false;
  }

  bool Workspace_Impl::isInIddFile(IddObjectType type) const {
    return m_iddFileAndFactoryWrapper.isInFile(type);
  }

  std::string Workspace_Impl::nextName(const std::string& name, bool fillIn) const {
    if (m_fastNaming){
      return toString(createUUID());
    }

    WorkspaceObjectVector objectsInSeries = getObjectsByName(name,false);
    return constructNextName(name,objectsInSeries,fillIn);
  }

  std::string Workspace_Impl::nextName(const IddObjectType& iddObjectType, bool fillIn) const {
    if (m_fastNaming){
      return toString(createUUID());
    }

    OptionalIddObject iddObject = getIddObject(iddObjectType);
    if (!iddObject) {
      return std::string();
    }
    std::string name = iddObjectNameToIdfObjectName(iddObject->name());
    WorkspaceObjectVector objectsInSeries = getObjectsByTypeAndName(iddObjectType,name);
    return constructNextName(name,objectsInSeries,fillIn);
  }

  bool Workspace_Impl::isValid() const {
    return isValid(strictnessLevel());
  }

  bool Workspace_Impl::isValid(StrictnessLevel level) const {
    ValidityReport report = validityReport(level);
    return (report.numErrors() == 0);
  }

  ValidityReport Workspace_Impl::validityReport() const {
    return validityReport(strictnessLevel());
  }

  ValidityReport Workspace_Impl::validityReport(StrictnessLevel level) const
  {
    ValidityReport report(level);

    int i = 0;
    emit progressRange(0, static_cast<int>(numObjects()));
    emit progressValue(i);
    emit progressCaption("Checking Validity");

    // StrictnessLevel::None
    // DataErrorType::NoIdd
    // \todo Only way there can be no IddFile is if IddFileType is set to UserCustom

    // Accumulate information about names for later name checking
    map<string,pair<bool,boost::shared_ptr<WorkspaceObject_Impl> > > mapOfNames;
    map<string,list <boost::shared_ptr<WorkspaceObject_Impl> > > objectsRepeatNames;

    // by-object items
    BOOST_FOREACH(const WorkspaceObjectMap::value_type& p, m_workspaceObjectMap)
    {

      //find all objects with the same name

      OptionalString oName = p.second->name();
      if(oName)
      {
        map<string,pair<bool,boost::shared_ptr<WorkspaceObject_Impl> > >::iterator itr = mapOfNames.find(*oName);
        if(itr!=mapOfNames.end())
        {

          if( !itr->second.first )
          {
            itr->second.first=true;
            list<boost::shared_ptr<WorkspaceObject_Impl> > l;
            l.push_front(itr->second.second);
            l.push_front(p.second);
            objectsRepeatNames[itr->first] = l;
          }
          else
          {

            map<string,list <boost::shared_ptr<WorkspaceObject_Impl> > >::iterator j= objectsRepeatNames.find(itr->first);
            OS_ASSERT(j!=objectsRepeatNames.end());
            j->second.push_front( p.second );
          }
        }
        else
        {
          mapOfNames[*oName] = pair<bool,boost::shared_ptr<WorkspaceObject_Impl> >(false,p.second);
        }
      }


      // object-level report
      ValidityReport objectReport = p.second->validityReport(level,false);
      OptionalDataError oError = objectReport.nextError();
      while (oError) {
        report.insertError(*oError);
        oError = objectReport.nextError();
      }

      // StrictnessLevel::Draft
      if (level > StrictnessLevel::None) {
        // DataErrorType::NoIdd
        // object-level
        if (iddFileType() == IddFileType::UserCustom) {
          if (!m_iddFileAndFactoryWrapper.isInFile(p.second->iddObject().name())) {
            report.insertError(DataError(WorkspaceObject(p.second),DataErrorType(DataErrorType::NoIdd)));
          }
        }
        else {
          if (!m_iddFileAndFactoryWrapper.isInFile(p.second->iddObject().type())) {
            report.insertError(DataError(WorkspaceObject(p.second),DataErrorType(DataErrorType::NoIdd)));
          }
        }
      } // StrictnessLevel::Draft

      emit progressValue(++i);
    }

    // StrictnessLevel::Draft
    if (level > StrictnessLevel::None) {
    // Check Name Conflicts
    //worst case, EVERY name is the same, EVERY object has a conflicting reference list.... this in O(n^3)
    //however, that is so unlikely its not even funny
    for( map<string,list <boost::shared_ptr<WorkspaceObject_Impl> > >::iterator itr = objectsRepeatNames.begin(),iend=objectsRepeatNames.end();itr!=iend;++itr)
    {
      boost::shared_ptr<WorkspaceObject_Impl> obj;
      list<StringVector> checkList;
      for(list <boost::shared_ptr<WorkspaceObject_Impl> >::iterator j=itr->second.begin(),jend=itr->second.end();j!=jend;++j)
      {
        StringVector refs = (*j)->iddObject().references();
        checkList.push_front(refs);
        obj=*j;
      }

      for(list<StringVector>::iterator y=checkList.begin(),yend=checkList.end();y!=yend;++y)
      {
        list<StringVector>::iterator z = y;
        list<StringVector>::iterator zend = yend;
        ++z;
        for(;z!=zend;++z)
        {
          StringVector intersection = intersectReferenceLists(*y,*z);
          if (!intersection.empty()) {
            //obj might NOT be the object that caused the collision, but it WILL have the same name
            report.insertError(DataError( obj->getObject<WorkspaceObject>(),DataErrorType(DataErrorType::NameConflict)));
          }
        }
      }
    } // for objectsRepeatedNames
    } // StrictnessLevel ::Draft

    // StrictnessLevel::Final
    if (level > StrictnessLevel::Draft) {
      // DataErrorType::NullAndRequired
      // collection-level: required object missing
      IddObjectVector requiredObjects = m_iddFileAndFactoryWrapper.requiredObjects();
      BOOST_FOREACH(const IddObject& iddObject, requiredObjects){
        if (numObjectsOfType(iddObject.type()) < 1) {
          report.insertError(DataError(DataErrorType(DataErrorType::NullAndRequired),iddObject.type()));
        }
      }

      // DataErrorType::Duplicate
      // collection-level: unique object duplicated
      IddObjectVector uniqueObjects = m_iddFileAndFactoryWrapper.uniqueObjects();
      BOOST_FOREACH(const IddObject& iddObject, uniqueObjects){
        if (numObjectsOfType(iddObject.type()) > 1) {
          report.insertError(DataError(DataErrorType(DataErrorType::Duplicate),iddObject.type()));
        }
      }

    } // StrictnessLevel::Final

    return report;
  }

  IdfObject Workspace_Impl::versionObjectToAdd() const {
    OptionalIddObject versionIdd = m_iddFileAndFactoryWrapper.versionObject();
    if (!versionIdd) {
      LOG_AND_THROW("Unable to determine a Version Idd object.");
    }
    IdfObject result(*versionIdd);
    // look for field named "Version Identifier"
    OptionalInt index = result.iddObject().getFieldIndex("Version Identifier");
    if (!index) {
      LOG(Warn,"No 'Version Identifier' field found in the Version IddObject.");
    }
    else {
      result.setString(*index,version().str());
    }
    return result;
  }

  // SERIALIZATION

  bool Workspace_Impl::save(const openstudio::path& p, bool overwrite) {
    return toIdfFile().save(p,overwrite);
  }

  IdfFile Workspace_Impl::toIdfFile() {

    IdfFile result;
    if (OptionalIdfObject vo = result.versionObject()) {
      result.removeObject(*vo);
    }

    // header
    result.setHeader(m_header);

    // specify IddFile
    result.setIddFileAndFactoryWrapper(m_iddFileAndFactoryWrapper);

    // add version object
    if (OptionalWorkspaceObject vo = versionObject()) {
      result.addObject(vo->idfObject());
    }

    // add objects and replace handle pointers with names
    WorkspaceObjectVector objs = objects(true); // sorted objects
    BOOST_FOREACH(WorkspaceObject& obj,objs) {
      result.addObject(obj.idfObject());
    }

    return result;
  }

  // PRIVATE

  // GETTER HELPERS

  HandleVector Workspace_Impl::handles(const std::set<Handle>& handles, bool sorted) const {
    HandleVector result(handles.begin(), handles.end());
    if (sorted) { result = sort(result); }
    return result;
  }

  std::string Workspace_Impl::getBaseName(const std::string& objectName) const {
    //boost::regex nameWithSuffix("([\\w :]*) \\d+");
    // DLM: Making more accepting of different characters in object names, e.g. '(', ')', etc
    boost::regex nameWithSuffix("(.*) \\d+$");
    boost::smatch m;
    if (boost::regex_match(objectName,m,nameWithSuffix)) {
      std::string result(m[1].first,m[1].second);
      return result;
    }
    return objectName;
  }

  boost::regex Workspace_Impl::getBaseNameRegex(std::string baseName) const {
    std::stringstream ss;
    boost::to_lower(baseName);
    //ss << baseName << "( \\d+)?";
    // DLM: Making more accepting of different characters in object names, e.g. '(', ')', etc
    const boost::regex esc("[\\^\\.\\$\\|\\(\\)\\[\\]\\*\\+\\?\\/\\\\]");
    const std::string rep("\\\\\\1&");
    std::string escapedBaseName = regex_replace(baseName, esc, rep, boost::match_default | boost::format_sed);
    ss << escapedBaseName << "( \\d+)?$";
    return boost::regex(ss.str());
  }

  boost::optional<WorkspaceObject> Workspace_Impl::getEquivalentObject(
      const IdfObject& other) const
  {
    // never overwrite existing version object
    if (other.iddObject().isVersionObject()) {
      return versionObject();
    }

    // get candidates by type and name
    OptionalString oName = other.name();
    WorkspaceObjectVector candidates;
    if (oName && !oName->empty()) {
      OptionalWorkspaceObject owo = getObjectByTypeAndName(other.iddObject().type(),*oName);
      if (owo) { candidates.push_back(*owo); }
    }
    else {
      candidates = getObjectsByType(other.iddObject().type());
    }

    // test for equivalency
    OptionalWorkspaceObject result;
    OptionalWorkspaceObject wsOther = other.optionalCast<WorkspaceObject>();
    BOOST_FOREACH(const WorkspaceObject& candidate,candidates) {
      if (wsOther) {
        if (candidate.dataFieldsEqual(*wsOther) &&
            candidate.objectListFieldsNonConflicting(*wsOther))
        {
          result = candidate;
          break;
        }
      }
      else if (other.dataFieldsEqual(candidate) &&
               other.objectListFieldsNonConflicting(candidate))
      {
        result = candidate;
        break;
      }
    }

    return result;
  }

  // SETTER HELPERS

  bool Workspace_Impl::setIddFile(const IddFileAndFactoryWrapper& iddFileAndFactoryWrapper) {
    IddFileAndFactoryWrapper temp = m_iddFileAndFactoryWrapper;
    m_iddFileAndFactoryWrapper = iddFileAndFactoryWrapper;
    if (isValid()) { return true; }
    else {
      LOG(Warn,"Unable to set IddFile to IddFileType " << iddFileAndFactoryWrapper.iddFileType()
          << ". Resulting Workspace is not valid:" << std::endl
          << validityReport(m_strictnessLevel));
      m_iddFileAndFactoryWrapper = temp;
      return false;
    }
  }

  bool Workspace_Impl::nominallyAddObject(boost::shared_ptr<WorkspaceObject_Impl>& ptr) {

    Handle h = ptr->handle();
    if (h.isNull()) { return false; }

    // WorkspaceObjectMap
    std::pair<WorkspaceObjectMap::iterator,bool> insertOK;
    insertOK = m_workspaceObjectMap.insert(WorkspaceObjectMap::value_type(h,ptr));
    if (!insertOK.second) { return false; }

    // WorkspaceObjectOrder--push_back if ordered directly
    if (m_workspaceObjectOrder.isDirectOrder()) {
      m_workspaceObjectOrder.push_back(h);
    }

    // IddObjectTypeMap
    insertIntoIddObjectTypeMap(ptr);

    // IdfReferencesMap
    insertIntoIdfReferencesMap(ptr);

    return true;
  }

  void Workspace_Impl::insertIntoObjectMap(
      const Handle& handle, const boost::shared_ptr<WorkspaceObject_Impl>& objectImplPtr)
  {
    m_workspaceObjectMap[handle] = objectImplPtr;
  }

  void Workspace_Impl::insertIntoIddObjectTypeMap(
      const boost::shared_ptr<WorkspaceObject_Impl>& objectImplPtr)
  {
    m_iddObjectTypeMap[objectImplPtr->iddObject().type()].insert(objectImplPtr->handle());
  }

  void Workspace_Impl::insertIntoIdfReferencesMap(
      const boost::shared_ptr<WorkspaceObject_Impl>& objectImplPtr)
  {
    StringVector references = objectImplPtr->iddObject().references();
    BOOST_FOREACH(const std::string& referenceName, references) {
      m_idfReferencesMap[referenceName].insert(objectImplPtr->handle());
    }
  }
  bool Workspace_Impl::resolvePotentialNameConflicts(Workspace& other) {
    return resolvePotentialNameConflicts(other, std::vector<unsigned>());
  }

  bool Workspace_Impl::resolvePotentialNameConflicts(Workspace& other,
                                                     const std::vector<unsigned>& toIgnore)
  {
    bool changeMade = false;

    WorkspaceObjectVector objectsToRename, objectsToExamine;
    std::map<std::string,WorkspaceObjectVector> nameObjectMap;
    if (!toIgnore.empty()) {
      objectsToExamine = other.objects(true);
    }
    else {
      objectsToExamine = other.objects(false);
    }
    unsigned i = 0;
    bool sameWorkspace(workspace() == other);
    BOOST_FOREACH(const WorkspaceObject& object,objectsToExamine) {
      if (object.iddObject().hasNameField() &&
          (std::find(toIgnore.begin(),toIgnore.end(),i) == toIgnore.end()))
      {
        OptionalString currentName = object.name();
        if (currentName && !currentName->empty()) {
          if (!sameWorkspace && potentialNameConflict(*currentName,object.iddObject())) {
            objectsToRename.push_back(object);
          }
          else {
            std::string lName = *currentName;
            boost::to_lower(lName);
            std::map<std::string,WorkspaceObjectVector>::iterator it = nameObjectMap.find(lName);
            if (it == nameObjectMap.end()) {
              nameObjectMap[lName] = WorkspaceObjectVector(1u,object);
            }
            else {
              BOOST_FOREACH(const WorkspaceObject& other,it->second) {
                if (!intersectReferenceLists(object.iddObject().references(),
                                             other.iddObject().references()).empty())
                {
                  objectsToRename.push_back(object);
                  break;
                }
              }
              it->second.push_back(object);
            }
          }
        }
      }
      ++i;
    }

    OptionalString newName;
    std::string originalDescription;
    BOOST_FOREACH(WorkspaceObject& object,objectsToRename) {
      originalDescription = object.briefDescription();
      newName = nextName(object.name().get(),false);
      newName = object.setName(*newName);
      OS_ASSERT(newName);
      LOG(Info,"Renamed " << originalDescription << " to '" << *newName
          << "' to avoid a name conflict upon WorkspaceObject addition.");
      changeMade = true;
    }

    return changeMade;
  }

  void Workspace_Impl::mergeIdfObjectAfterPotentialNameConflictResolution(
      IdfObject& mergedObject,const IdfObject& originalObject) const
  {
    OS_ASSERT(mergedObject.iddObject() == originalObject.iddObject());
    OS_ASSERT(mergedObject.numFields() == originalObject.numFields());
    for (unsigned i = 0, n = mergedObject.numFields(); i < n; ++i) {
      if (mergedObject.getString(i).get().empty()) {
        mergedObject.setString(i,originalObject.getString(i).get());
      }
    }
  }

  bool Workspace_Impl::addProvidedRelationships(
      const std::vector<Handle>& handles,
      const std::vector<UHPointer>& pointersIntoWorkspace,
      const std::vector<HUPointer>& pointersFromWorkspace) {

    bool success = true;

    BOOST_FOREACH(const UHPointer& intoPtr,pointersIntoWorkspace) {
      if (!success) { break; }
      OS_ASSERT(intoPtr.source < handles.size());
      // get source object
      OptionalWorkspaceObject oSource = getObject(handles[intoPtr.source]);
      if (!oSource) { success = false; }
      else { success = oSource->getImpl<WorkspaceObject_Impl>()->setPointer(
          intoPtr.fieldIndex,intoPtr.target,false); }
    }

    BOOST_FOREACH(const HUPointer& fromPtr,pointersFromWorkspace) {
      if (!success) { break; }
      // get target handle
      OS_ASSERT(fromPtr.target < handles.size());
      Handle targetHandle = handles[fromPtr.target];
      // get source object
      OptionalWorkspaceObject oSource = getObject(fromPtr.source);
      if (!oSource) { success = false; }
      else { success = oSource->getImpl<WorkspaceObject_Impl>()->setPointer(
          fromPtr.fieldIndex,targetHandle,false); }
    }

    return success;
  }

  std::vector<WorkspaceObject> Workspace_Impl::simpleAddObjects(const std::vector<IdfObject>& idfObjects) {
    WorkspaceObjectVector result;
    WorkspaceObject_ImplPtrVector newObjects;

    if (idfObjects.empty()) {
      return result;
    }

    bool keepHandles = idfObjects[0].iddObject().hasHandleField();

    IdfObjectVector::const_iterator it(idfObjects.begin()), itEnd(idfObjects.end());
    for (; it != itEnd; ++it) {
      newObjects.push_back(this->createObject(*it,keepHandles));
    }
    result = addObjects(newObjects,UHPointerVector(),HUPointerVector(),true,true);
    Workspace thisWorkspace = workspace();
    resolvePotentialNameConflicts(thisWorkspace);

    return result;
  }

  Workspace_Impl::OptionalSavedWorkspaceObject Workspace_Impl::savedWorkspaceObject(
      const Handle& handle)
  {
    OptionalWorkspaceObject oObject = getObject(handle);
    if (!oObject) { return boost::none; }

    // required info
    SavedWorkspaceObject result(handle,oObject->getImpl<WorkspaceObject_Impl>());

    // direct order
    result.orderIndex = m_workspaceObjectOrder.indexInOrder(handle);

    return result;
  }

  std::vector<WorkspaceObject> Workspace_Impl::nominallyRemoveObject(const Handle& handle) {
    // get object. should exist since call came from within Workspace.
    OptionalWorkspaceObject oObject = getObject(handle);
    if (!oObject) {
      OS_ASSERT(oObject);
    }
    WorkspaceObject_ImplPtr objectImplPtr = oObject->getImpl<WorkspaceObject_Impl>();

    // unlink from other objects
    // handle -> others
    UnsignedVector ptrFields = objectImplPtr->objectListFields();
    BOOST_FOREACH(unsigned index,ptrFields) {
      objectImplPtr->setPointer(index,Handle(),false);
    }
    // others -> handle
    WorkspaceObjectVector sources;
    ReversePointerSet pointers = objectImplPtr->getReversePointers();
    BOOST_FOREACH(const ReversePointer& ptr,pointers) {
      OptionalWorkspaceObject source = getObject(ptr.sourceHandle);
      // OS_ASSERT(sourceImplPtr);
      if (source) {
        source->getImpl<WorkspaceObject_Impl>()->setPointer(ptr.fieldIndex,Handle(),false);
        sources.push_back(*source);
      }
      else {
        OptionalString objectName = objectImplPtr->name();
        if (objectName) {
          LOG(Debug,"Object of type '" << objectImplPtr->iddObject().name() << "' named '"
              << *objectName << "' has a reverse pointer that cannot be followed.");
        }
        else {
          LOG(Debug,"Object of type '" << objectImplPtr->iddObject().name()
              << "' has a reverse pointer that cannot be followed.");
        }
      }
    }

    // IdfReferencesMap
    StringVector references = objectImplPtr->iddObject().references();
    BOOST_FOREACH(const std::string& reference,references) {
      IdfReferencesMap::iterator irmLoc = m_idfReferencesMap.find(reference);
      OS_ASSERT(irmLoc != m_idfReferencesMap.end());
      HandleSet::iterator loc = irmLoc->second.find(handle);
      OS_ASSERT(loc != irmLoc->second.end());
      irmLoc->second.erase(loc);
      // erase entry if set is emtpy
      if (irmLoc->second.empty()) { m_idfReferencesMap.erase(irmLoc); }
    }

    // IddObjectTypeMap
    IddObjectTypeMap::iterator iotmLoc = m_iddObjectTypeMap.find(objectImplPtr->iddObject().type());
    OS_ASSERT(iotmLoc != m_iddObjectTypeMap.end());
    HandleSet::iterator loc = iotmLoc->second.find(handle);
    OS_ASSERT(loc != iotmLoc->second.end());
    iotmLoc->second.erase(loc);
    // erase entry if set is empty
    if (iotmLoc->second.empty()) { m_iddObjectTypeMap.erase(iotmLoc); }

    // WorkspaceObjectOrder
    if (m_workspaceObjectOrder.isDirectOrder()) {
      m_workspaceObjectOrder.erase(handle);
    }

    // WorkspaceObjectMap
    WorkspaceObjectMap::iterator womIt = m_workspaceObjectMap.find(handle);
    m_workspaceObjectMap.erase(womIt);

    return sources;
  }

  std::vector<std::vector<WorkspaceObject> > Workspace_Impl::nominallyRemoveObjects(const std::vector<Handle>& handles) {
    std::vector<std::vector<WorkspaceObject> > sources;
    BOOST_FOREACH(const Handle& handle,handles) {
      sources.push_back(nominallyRemoveObject(handle));
    }
    return sources;
  }

  void Workspace_Impl::registerRemovalOfObject(boost::shared_ptr<WorkspaceObject_Impl> ptr,
                                               const std::vector<WorkspaceObject>& sources,
                                               const std::vector<Handle>& removedHandles)
  {
    //DLM@20110810: moved remove emits to occur before object is removed from workspace
    //ptr->emitChangeSignals(); // do not emit signals for changes that occurred during removal
    BOOST_FOREACH(const WorkspaceObject& source,sources) {
      // do not emit signals if this source is also being removed
      if (std::find(removedHandles.begin(), removedHandles.end(), source.handle()) == removedHandles.end()){
        source.getImpl<detail::WorkspaceObject_Impl>()->emitChangeSignals();
      }
    }
    ptr->disconnect();
    disconnect(ptr.get(),SIGNAL(onChange()),this,SLOT(change()));
  }

  void Workspace_Impl::registerRemovalOfObjects(std::vector<SavedWorkspaceObject>& savedObjects,
                                                const std::vector<std::vector<WorkspaceObject> >& sources,
                                                const std::vector<Handle>& removedHandles)
  {
    for (int i = 0, n = savedObjects.size(); i < n; ++i) {
      registerRemovalOfObject(savedObjects[i].objectImplPtr,sources[i],removedHandles);
    }
  }

  void Workspace_Impl::registerAdditionOfObject(const WorkspaceObject& object) {
    connect(object.getImpl<WorkspaceObject_Impl>().get(),SIGNAL(onChange()),this,SLOT(change()));
    emit addWorkspaceObject(object, object.iddObject().type(), object.handle());
    emit addWorkspaceObject(object.getImpl<WorkspaceObject_Impl>(), object.iddObject().type(), object.handle());
    emit onChange();
  }

  void Workspace_Impl::restoreObject(SavedWorkspaceObject& savedObject) {
    // WorkspaceObjectMap
    m_workspaceObjectMap.insert(WorkspaceObjectMap::value_type(savedObject.handle,savedObject.objectImplPtr));

    // WorkspaceObjectOrder
    if (savedObject.orderIndex) {
      m_workspaceObjectOrder.insert(savedObject.handle,*(savedObject.orderIndex));
    }

    // IddObjectTypeMap
    insertIntoIddObjectTypeMap(savedObject.objectImplPtr);

    // IdfReferencesMap
    insertIntoIdfReferencesMap(savedObject.objectImplPtr);

    // Fix Pointers
    savedObject.objectImplPtr->restorePointers();

    // Connect signals
    WorkspaceObject workspaceObject(savedObject.objectImplPtr);

    // emit signals
    registerAdditionOfObject(workspaceObject);
  }

  void Workspace_Impl::restoreObjects(SavedWorkspaceObjectVector& savedObjects) {
    BOOST_FOREACH(SavedWorkspaceObject& object, savedObjects) {
      restoreObject(object);
    }
  }

  std::vector<std::pair<QUrl, openstudio::path> > Workspace_Impl::locateUrls(
      const std::vector<URLSearchPath> &t_paths,
      bool t_create_relative_paths,
      const openstudio::path &t_infile,
      const openstudio::path &t_locationForRemoteUrls )
  {
    std::vector<std::pair<QUrl, openstudio::path> > retval;

    std::vector<WorkspaceObject> urlobjects = objectsWithURLFields();

    std::vector<std::pair<boost::optional<IddObjectType>, QUrl> > search_paths;

    for (std::vector<URLSearchPath>::const_iterator itr = t_paths.begin();
         itr != t_paths.end();
         ++itr)
    {
      QUrl searchurl = itr->getUrl();
      URLSearchPath::Relative relativity = itr->getRelativity();

      QUrl updatedsearchurl = searchurl;

      //search for files here...
      /// \todo support remote urls
      if (searchurl.scheme() == "file")
      {
        QString searchpathstr = searchurl.toLocalFile();
        openstudio::path searchpath = toPath(searchpathstr);
        openstudio::path completesearchpath = searchpath;

        if (!searchpath.is_complete())
        {
          if (relativity == URLSearchPath::ToInputFile)
          {
            completesearchpath = boost::filesystem::complete(searchpath, t_infile.parent_path());
          } else {
            // relative to where app was started from
            completesearchpath = boost::filesystem::complete(searchpath);
          }
        }

        updatedsearchurl = QUrl::fromLocalFile(toQString(completesearchpath));
      }

      QUrl searchpath;
      search_paths.push_back(std::make_pair(itr->getIddObjectType(), updatedsearchurl));
    }

    if (urlobjects.empty())
    {
      LOG(Debug, "No objects containing URL fields were found in file");
    }

    for (std::vector<WorkspaceObject>::iterator itr = urlobjects.begin();
        itr != urlobjects.end();
        ++itr)
    {
      const unsigned int numfields = itr->numFields();

      for (unsigned int i = 0; i < numfields; ++i)
      {
        boost::optional<QUrl> url = itr->getURL(i);

        if (url)
        {
          QUrl sourceurl;

          LOG(Debug, "Url found in file: " << toString(url->toString()));

          std::vector<QUrl> thisobjectssearchpaths;

          for (std::vector<std::pair<boost::optional<IddObjectType>, QUrl> >::const_iterator searchitr = search_paths.begin();
               searchitr != search_paths.end();
               ++searchitr)
          {
            if (!searchitr->first || *(searchitr->first) == itr->iddObject().type())
            {
              //This search criteria applies to this object
              thisobjectssearchpaths.push_back(searchitr->second);
            }
          }


          if (url->isRelative() || url->scheme().size() == 1 /*a single char scheme is probably a drive letter*/
                || url->scheme() == "file")
          {
            LOG(Debug, "Url is relative or 'file'");

            openstudio::path origpath;
            if (url->scheme() == "file")
            {
              origpath = toPath(url->toLocalFile());
            } else {
              origpath = toPath(url->toString());
            }

            if (!origpath.is_complete())
            {
              QUrl tempUrl = completeURL(QUrl::fromLocalFile(toQString(origpath)), thisobjectssearchpaths, false);
              if (!tempUrl.isEmpty())
              {
                sourceurl = tempUrl;
              }
            } else {
              // the url is a complete path, no reason to apply searches to it
              // in fact, we'll even trust that it's there
              sourceurl = QUrl::fromLocalFile(toQString(origpath));
            }
          } else {
            // Not a local file or relative, trust it:
            sourceurl = *url;
          }

          LOG(Debug, "Source URL: " << toString(sourceurl.toString()) << " for original URL: " << toString(url->toString()));


          if (!sourceurl.isEmpty())
          {
            // We've managed to complete the path

            QUrl destinationURL;

            openstudio::path relativepath = toPath(sourceurl.toLocalFile());
            openstudio::path filename = toPath(relativepath.filename());

            LOG(Debug, "Extracted filename: " << toString(filename));

            if (t_create_relative_paths)
            {
              destinationURL = QUrl::fromLocalFile(toQString(filename));
            } else {
              if (sourceurl.scheme() != "file")
              {
                destinationURL = QUrl::fromLocalFile(toQString(t_locationForRemoteUrls / filename));
              } else {
                destinationURL = sourceurl;
              }
            }

            if (destinationURL != *url)
            {
              itr->setString(i, toString(destinationURL.toString()));
              LOG(Debug, "File updated, url: " << toString(url->toLocalFile()) << " to: " << toString(destinationURL.toLocalFile()) << " with source of: " << toString(sourceurl.toString()));
              retval.push_back(std::make_pair(sourceurl, toPath(destinationURL.toLocalFile())));
            }
          } else {
            LOG(Error, "URL Found in IDF: " << toString(url->toString()) << " but no source could be located.");
            if (!t_paths.empty()) {
              LOG(Debug,"Search paths: ");
              BOOST_FOREACH(const URLSearchPath& searchPath, t_paths) {
                LOG(Debug,"  " << toString(searchPath.getUrl()));
              }
            }
          }
        }
      }
    }

    return retval;

  }

  // QUERIES

  std::string Workspace_Impl::constructNextName(const std::string& objectName,
                                                const std::vector<WorkspaceObject>& objectsInTheSeries,
                                                bool fillIn) const
  {
    IntSet takenValues;
    std::string baseName = getBaseName(objectName);
    boost::regex nameRegex = getBaseNameRegex(baseName);
    boost::smatch m;
    BOOST_FOREACH(const WorkspaceObject& object,objectsInTheSeries) {
      std::string name = object.name().get();
      boost::to_lower(name);
      if (boost::regex_match(name,m,nameRegex)) {
        std::string intstr(m[1].first,m[1].second);
        boost::trim(intstr);
        if (!intstr.empty()) {
          takenValues.insert(boost::lexical_cast<int>(intstr));
        }
      }
    }

    int suffix(1);
    if (fillIn) {
      BOOST_FOREACH(int usedSuffix,takenValues) {
        if (usedSuffix == suffix) {
          ++suffix;
        }
        else {
          break;
        }
      }
    }
    else {
      if (!takenValues.empty()) {
        suffix = *(takenValues.rbegin()) + 1;
      }
    }

    std::stringstream ss;
    ss << baseName << " " << suffix;
    return ss.str();
  }

  std::vector< std::vector<WorkspaceObject> > Workspace_Impl::nameConflicts(
      const std::vector<WorkspaceObject>& candidates) const
  {
    std::vector<WorkspaceObjectVector> result;
    IStringSet examinedNames;

    for (WorkspaceObjectVector::const_iterator it = candidates.begin(),
         itEnd = candidates.end(); it != itEnd; ++it)
    {
      WorkspaceObject candidate = *it;
      OptionalString candidateName = candidate.name();
      OS_ASSERT(candidateName);
      std::pair<IStringSet::iterator,bool> isNewName = examinedNames.insert(*candidateName);
      if (!isNewName.second) {
        // conflict found -- add candidate to result
        bool added = false;
        BOOST_FOREACH(WorkspaceObjectVector& conflictGroup,result) {
          if (boost::iequals(*candidateName,conflictGroup[0].name().get())) {
            conflictGroup.push_back(candidate);
            added = true;
            break;
          }
        }
        if (!added) {
          // create new group
          WorkspaceObjectVector newConflictGroup;
          // get first instance of name
          for (WorkspaceObjectVector::const_iterator jt = candidates.begin();
               jt != it; ++jt)
          {
            if (boost::iequals(*candidateName,jt->name().get())) {
              newConflictGroup.push_back(*jt);
              added = true;
              break;
            }
          }
          OS_ASSERT(added);
          newConflictGroup.push_back(candidate);
          result.push_back(newConflictGroup);
        }
      }
    }

    return result;
  }

  bool Workspace_Impl::potentialNameConflict(std::string& currentName,
                                             const IddObject& iddObject) const
  {
    bool result = false;
    WorkspaceObjectVector candidates = getObjectsByName(currentName);
    if (candidates.size() == 0) { return result; }

    // potential for conflicts. take set intersection of reference lists
    StringVector iddObjectReferences = iddObject.references();
    BOOST_FOREACH(const WorkspaceObject& candidate,candidates) {
      if (candidate.iddObject() == iddObject) {
        return true;
      }
      StringVector candidateReferences = candidate.iddObject().references();
      StringVector intersection = intersectReferenceLists(iddObjectReferences,candidateReferences);
      if (!intersection.empty()) {
        return true;
      }
    }
    return result;
  }

  void Workspace_Impl::change() {
    emit onChange();
  }

  void Workspace_Impl::createAndAddClonedObjects(
      const boost::shared_ptr<detail::Workspace_Impl>& thisImpl,
      boost::shared_ptr<detail::Workspace_Impl> cloneImpl,
      bool keepHandles) const
  {
    detail::WorkspaceObject_ImplPtrVector newObjectImplPtrs;
    HandleMap oldNewHandleMap;
    BOOST_FOREACH(const WorkspaceObject& object,allObjects()) {
      newObjectImplPtrs.push_back(cloneImpl->createObject(
          object.getImpl<detail::WorkspaceObject_Impl>(),keepHandles));
      Handle h = newObjectImplPtrs.back()->handle();
      if (!keepHandles) { oldNewHandleMap.insert(HandleMap::value_type(object.handle(),h)); }
    }
    // add Object_ImplPtrs to clone's Workspace_Impl
    cloneImpl->addClones(newObjectImplPtrs,oldNewHandleMap,true);
  }

  void Workspace_Impl::createAndAddSubsetClonedObjects(
      const boost::shared_ptr<detail::Workspace_Impl>& thisImpl,
      boost::shared_ptr<detail::Workspace_Impl> cloneImpl,
      const std::vector<Handle>& handles,
      bool keepHandles) const
  {
    // make sure versionObject comes along for the ride
    HandleVector wHandles = handles;
    if (OptionalWorkspaceObject vo = versionObject()) {
      if (std::find(wHandles.begin(),wHandles.end(),vo->handle()) == wHandles.end()) {
        wHandles.insert(wHandles.begin(),vo->handle());
      }
    }

    // construct clone's WorkspaceObject_ImplPtrs
    detail::WorkspaceObject_ImplPtrVector newObjectImplPtrs;
    HandleMap oldNewHandleMap;
    BOOST_FOREACH(const Handle& h,wHandles) {
      OptionalWorkspaceObject object = thisImpl->getObject(h);
      if (object) {
        newObjectImplPtrs.push_back(cloneImpl->createObject(
            object->getImpl<detail::WorkspaceObject_Impl>(),keepHandles));
        Handle newH = newObjectImplPtrs.back()->handle();
        if (!keepHandles) { oldNewHandleMap.insert(HandleMap::value_type(h,newH)); }
      }
    }

    // add Object_ImplPtrs to clone's Workspace_Impl
    cloneImpl->addClones(newObjectImplPtrs,oldNewHandleMap,true);
  }

  std::vector<WorkspaceObject> Workspace_Impl::allObjects() const {
    WorkspaceObjectVector result;
    BOOST_FOREACH(const WorkspaceObjectMap::value_type& p, m_workspaceObjectMap) {
      result.push_back(WorkspaceObject(p.second));
    }
    return result;
  }

} // detail

// CONSTRUCTORS

Workspace::Workspace() :
    m_impl(new detail::Workspace_Impl(StrictnessLevel(StrictnessLevel::Draft),
                                      IddFileType(IddFileType::OpenStudio)))
{
  addVersionObject();
}

Workspace::Workspace(StrictnessLevel level) :
    m_impl(new detail::Workspace_Impl(level,IddFileType(IddFileType::OpenStudio)))
{
  addVersionObject();
}

Workspace::Workspace(StrictnessLevel level, IddFileType iddFileType) :
    m_impl(new detail::Workspace_Impl(level,iddFileType))
{
  addVersionObject();
}

Workspace::Workspace(const IdfFile& idfFile, StrictnessLevel level) :
    m_impl(new detail::Workspace_Impl(idfFile,level))
{
  // construct WorkspaceObject_ImplPtrs
  openstudio::detail::WorkspaceObject_ImplPtrVector objectImplPtrs;
  if (OptionalIdfObject vo = idfFile.versionObject()) {
    objectImplPtrs.push_back(m_impl->createObject(*vo,true));
  }
  BOOST_FOREACH(const IdfObject& idfObject,idfFile.objects()) {
    objectImplPtrs.push_back(m_impl->createObject(idfObject,true));
  }
  // add Object_ImplPtrs to Workspace_Impl
  m_impl->addObjects(objectImplPtrs);
  Workspace copyOfThis(m_impl);
  m_impl->resolvePotentialNameConflicts(copyOfThis);
}

Workspace::Workspace(const Workspace& other)
  : m_impl(other.m_impl)
{}

Workspace Workspace::clone(bool keepHandles) const {
  Workspace result = m_impl->clone(keepHandles);
  return result;
}

Workspace Workspace::cloneSubset(const std::vector<Handle>& handles,
                                 bool keepHandles,
                                 StrictnessLevel level) const
{
  Workspace result = m_impl->cloneSubset(handles,keepHandles,level);
  result.addVersionObject();
  return result;
}

void Workspace::swap(Workspace& other) {
  if (typeid(*(m_impl.get())) != typeid(*(other.m_impl.get()))) {
    LOG_AND_THROW("Workspaces can only be swapped if they are both of the same type "
                  << "(both Workspaces, both Models, or both Components).");
  }
  m_impl->swap(other);
}

// GETTERS

StrictnessLevel Workspace::strictnessLevel() const {
  return m_impl->strictnessLevel();
}

VersionString Workspace::version() const {
  return m_impl->version();
}

IddFile Workspace::iddFile() const {
  return m_impl->iddFile();
}

IddFileType Workspace::iddFileType() const {
  return m_impl->iddFileType();
}

boost::optional<IddObject> Workspace::getIddObject(const IddObjectType& type) const {
  return m_impl->getIddObject(type);
}

boost::optional<std::string> Workspace::name(const Handle& handle) const {
  return m_impl->name(handle);
}

boost::optional<WorkspaceObject> Workspace::getObject(Handle handle) const {
  boost::optional<WorkspaceObject> result = m_impl->getObject(handle);
  if (result) { OS_ASSERT(result->initialized()); }
  return result;
}

std::vector<WorkspaceObject> Workspace::objects(bool sorted) const {
  return m_impl->objects(sorted);
}

std::vector<Handle> Workspace::handles(bool sorted) const {
  return m_impl->handles(sorted);
}

std::vector<WorkspaceObject> Workspace::objectsWithURLFields() const
{
  return m_impl->objectsWithURLFields();
}

boost::optional<WorkspaceObject> Workspace::versionObject() const {
  return m_impl->versionObject();
}

std::vector<WorkspaceObject> Workspace::getObjects(const std::vector<Handle>& handles) const {
  return m_impl->getObjects(handles);
}

std::vector<WorkspaceObject> Workspace::getObjectsByName(const std::string& name,
                                                         bool exactMatch) const
{
  return m_impl->getObjectsByName(name,exactMatch);
}

std::vector<WorkspaceObject> Workspace::getObjectsByType(IddObjectType objectType) const {
  return m_impl->getObjectsByType(objectType);
}

std::vector<WorkspaceObject> Workspace::getObjectsByType(const IddObject& objectType) const {
  return m_impl->getObjectsByType(objectType);
}

boost::optional<WorkspaceObject> Workspace::getObjectByTypeAndName(IddObjectType objectType,
                                                                   const std::string& name) const
{
  return m_impl->getObjectByTypeAndName(objectType,name);
}

std::vector<WorkspaceObject> Workspace::getObjectsByTypeAndName(IddObjectType objectType,
                                                                const std::string& name) const
{
  return m_impl->getObjectsByTypeAndName(objectType,name);
}

std::vector<WorkspaceObject> Workspace::getObjectsByReference(
    const std::string& referenceName) const
{
  return m_impl->getObjectsByReference(referenceName);
}

std::vector<WorkspaceObject> Workspace::getObjectsByReference(
    const std::vector<std::string>& referenceNames) const
{
  return m_impl->getObjectsByReference(referenceNames);
}

boost::optional<WorkspaceObject> Workspace::getObjectByNameAndReference(
    std::string name,const StringVector& referenceNames) const
{
  return m_impl->getObjectByNameAndReference(name,referenceNames);
}

bool Workspace::fastNaming() const
{
  return m_impl->fastNaming();
}

// SETTERS

bool Workspace::setStrictnessLevel(StrictnessLevel level) {
  return m_impl->setStrictnessLevel(level);
}

boost::optional<WorkspaceObject> Workspace::addObject(const IdfObject& idfObject) {
  return m_impl->addObject(idfObject);
}

boost::optional<WorkspaceObject> Workspace::insertObject(const IdfObject& idfObject) {
  return m_impl->insertObject(idfObject);
}

std::vector<WorkspaceObject> Workspace::addObjects(const std::vector<IdfObject>& idfObjects) {
  return m_impl->addObjects(idfObjects);
}

std::vector <WorkspaceObject> Workspace::insertObjects(const std::vector<IdfObject>& idfObjects) {
  return m_impl->insertObjects(idfObjects);
}

std::vector<WorkspaceObject> Workspace::addAndInsertObjects(
    const std::vector<IdfObject>& objectsToAdd,const std::vector<IdfObject>& objectsToInsert)
{
  return m_impl->addAndInsertObjects(objectsToAdd,objectsToInsert);
}

std::vector<WorkspaceObject> Workspace::addObjects(const std::vector<WorkspaceObject>& objects) {
  return m_impl->addObjects(objects);
}

std::vector<WorkspaceObject> Workspace::insertObjects(const std::vector<WorkspaceObject>& objects) {
  return m_impl->insertObjects(objects);
}

std::vector<WorkspaceObject> Workspace::addAndInsertObjects(
    const std::vector<WorkspaceObject>& objectsToAdd,
    const std::vector<WorkspaceObject>& objectsToInsert)
{
  return m_impl->addAndInsertObjects(objectsToAdd,objectsToInsert);
}

std::vector<WorkspaceObject> Workspace::addAndInsertObjects(
        const std::vector<WorkspaceObject>& objectsToAdd,
        const std::vector< std::vector<WorkspaceObject> >& objectsToInsert)
{
  return m_impl->addAndInsertObjects(objectsToAdd,objectsToInsert);
}

bool Workspace::swap(WorkspaceObject& currentObject,IdfObject& newObject,bool keepTargets) {
  return m_impl->swap(currentObject,newObject,keepTargets);
}

bool Workspace::removeObject(const Handle& handle) {
  return m_impl->removeObject(handle);
}

bool Workspace::removeObjects(const std::vector<Handle>& handles) {
  return m_impl->removeObjects(handles);
}

void Workspace::setFastNaming(bool fastNaming)
{
  m_impl->setFastNaming(fastNaming);
}

// ORDER

WorkspaceObjectOrder Workspace::order() {
  return m_impl->order();
}

const WorkspaceObjectOrder Workspace::order() const {
  return m_impl->order();
}

std::vector<WorkspaceObject> Workspace::sort(const std::vector<WorkspaceObject>& objects) const {
  return m_impl->sort(objects);
}

std::vector<Handle> Workspace::sort(const std::vector<Handle>& handles) const {
  return m_impl->sort(handles);
}

// QUERIES

unsigned Workspace::numObjects() const {
  return m_impl->numObjects();
}

unsigned Workspace::numObjectsOfType(IddObjectType type) const {
  return m_impl->numObjectsOfType(type);
}

unsigned Workspace::numObjectsOfType(const IddObject& objectType) const {
  return m_impl->numObjectsOfType(objectType);
}

bool Workspace::isMember(const Handle& handle) const {
  return m_impl->isMember(handle);
}

bool Workspace::canBeTarget(const Handle& handle, const StringSet& referenceListNames) const {
  return m_impl->canBeTarget(handle,referenceListNames);
}

bool Workspace::isInIddFile(IddObjectType type) const {
  return m_impl->isInIddFile(type);
}

std::string Workspace::nextName(const std::string& name, bool fillIn) const {
  return m_impl->nextName(name,fillIn);
}

std::string Workspace::nextName(const IddObjectType& iddObjectType, bool fillIn) const {
  return m_impl->nextName(iddObjectType,fillIn);
}

bool Workspace::isValid() const {
  return m_impl->isValid();
}

bool Workspace::isValid(StrictnessLevel level) const {
  LOG(Debug,"Workspace::isValid called.");
  return m_impl->isValid(level);
}

ValidityReport Workspace::validityReport() const {
  return m_impl->validityReport();
}

ValidityReport Workspace::validityReport(StrictnessLevel level) const {
  return m_impl->validityReport(level);
}

bool Workspace::operator==(const Workspace& other) const {
  return (m_impl == other.m_impl);
}

bool Workspace::operator!=(const Workspace& other) const {
  return !(operator==(other));
}

// connect a progress bar
bool Workspace::connectProgressBar(const openstudio::ProgressBar& progressBar) const
{
  bool result = true;
  result = result && progressBar.connect(m_impl.get(), SIGNAL(progressRange(int, int)), SLOT(setRange(int, int)));
  result = result && progressBar.connect(m_impl.get(), SIGNAL(progressValue(int)), SLOT(setValue(int)));
  result = result && progressBar.connect(m_impl.get(), SIGNAL(progressCaption(const QString&)), SLOT(setWindowTitle(const QString&)));
  return result;
}

// disconnect a progress bar
bool Workspace::disconnectProgressBar(const openstudio::ProgressBar& progressBar) const
{
  return m_impl.get()->disconnect(&progressBar);
}

// SERIALIZATION

bool Workspace::save(const openstudio::path& p, bool overwrite) {
  return m_impl->save(p,overwrite);
}

boost::optional<Workspace> Workspace::load(const openstudio::path& p) {
  OptionalIdfFile oIdfFile = IdfFile::load(p);
  if (oIdfFile) {
    return Workspace(*oIdfFile);
  }
  return boost::none;
}

boost::optional<Workspace> Workspace::load(const openstudio::path& p,
                                           const IddFileType& iddFileType)
{
  OptionalIdfFile oIdfFile = IdfFile::load(p,iddFileType);
  if (oIdfFile) {
    return Workspace(*oIdfFile);
  }
  return boost::none;
}

boost::optional<Workspace> Workspace::load(const openstudio::path& p,
                                           const IddFile& iddFile)
{
  OptionalIdfFile oIdfFile = IdfFile::load(p,iddFile);
  if (oIdfFile) {
    return Workspace(*oIdfFile);
  }
  return boost::none;
}

IdfFile Workspace::toIdfFile() const {
  return m_impl->toIdfFile();
}

std::vector<std::pair<QUrl, openstudio::path> > Workspace::locateUrls(
    const std::vector<URLSearchPath> &t_paths, bool t_create_relative_paths,
    const openstudio::path &t_infile)
{
  return m_impl->locateUrls(t_paths, t_create_relative_paths, t_infile);
}

// PROTECTED

Workspace::Workspace(boost::shared_ptr<detail::Workspace_Impl> impl)
  : m_impl(impl)
{}

std::vector<WorkspaceObject> Workspace::allObjects() const {
  return m_impl->allObjects();
}

void Workspace::addVersionObject() {
  if (!versionObject()) {
    IdfObject idfVersionObject = m_impl->versionObjectToAdd();
    openstudio::detail::WorkspaceObject_ImplPtrVector objectImplPtrs;
    objectImplPtrs.push_back(m_impl->createObject(idfVersionObject,false));
    WorkspaceObjectVector objects = m_impl->addObjects(objectImplPtrs);
    if (objects.size() != 1u) {
      LOG_AND_THROW("Unable to add version object to Workspace.")
    }
  }
}

std::ostream& operator<<(std::ostream& os, const Workspace& workspace)
{
  os << workspace.toIdfFile();
  return os;
}

} // openstudio
