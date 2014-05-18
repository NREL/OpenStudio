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

#include <utilities/idf/WorkspaceObject.hpp>
#include <utilities/idf/WorkspaceObject_Impl.hpp>

#include <utilities/idf/Workspace.hpp>
#include <utilities/idf/Workspace_Impl.hpp>
#include <utilities/idf/WorkspaceObjectDiff.hpp>
#include <utilities/idf/WorkspaceObjectDiff_Impl.hpp>
#include <utilities/idf/WorkspaceExtensibleGroup.hpp>
#include <utilities/idf/IdfExtensibleGroup.hpp>
#include <utilities/idf/ValidityReport.hpp>

#include <utilities/idd/IddObjectProperties.hpp>
#include <utilities/idd/IddFieldProperties.hpp>

#include <utilities/core/Compare.hpp>
#include <utilities/core/Assert.hpp>
#include <utilities/core/Containers.hpp>
#include <utilities/core/StringHelpers.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <boost/regex.hpp>

#include <iostream>
using namespace std;

using openstudio::detail::WorkspaceObject_Impl;
using boost::dynamic_pointer_cast;

namespace openstudio {

namespace detail {

  // CONSTRUCTORS

  WorkspaceObject_Impl::WorkspaceObject_Impl(const IdfObject& idfObject,
                                             Workspace_Impl* workspace,
                                             bool keepHandle)
    : IdfObject_Impl(*(idfObject.getImpl<detail::IdfObject_Impl>()),keepHandle),  // clones idfObject data
      m_initialized(false),
      m_workspace(workspace)
  {
    if (!m_iddObject.objectLists().empty()) {
      // can nominally be source
      m_sourceData = SourceData();
    }
    if (idfObject.name() && idfObject.name(true).get().empty()) {
      // create name if their is a name field and no default value
      createName();
    }
  }

  WorkspaceObject_Impl::WorkspaceObject_Impl(const WorkspaceObject_Impl& other,
                                             Workspace_Impl* workspace,
                                             bool keepHandle) :
    IdfObject_Impl(other, keepHandle),
    m_initialized(false),
    m_workspace(workspace),
    m_sourceData(other.m_sourceData),
    m_targetData(other.m_targetData)
  {}

  WorkspaceObject_Impl::~WorkspaceObject_Impl() {}

  std::vector<IdfObject> WorkspaceObject_Impl::remove()
  {
    std::vector<IdfObject> result;
    result.push_back(this->idfObject());
    bool ok = this->workspace().removeObject(this->handle());
    OS_ASSERT(ok);
    return result;
  }

  void WorkspaceObject_Impl::initializeOnAdd(bool expectToLosePointers) {
    OS_ASSERT(m_workspace);
    bool ptrsAsHandles = iddObject().hasHandleField();
    // loop through object list fields
    UnsignedVector fields = objectListFields();
    BOOST_FOREACH(unsigned index, fields) {
      // determine if field should be managed
      OptionalIddField iddField = iddObject().getField(index);
      OS_ASSERT(iddField);

      // for each one, try to match targetName
      std::string targetName = IdfObject_Impl::getString(index).get();
      if (targetName.empty()) { // set null pointer
        setPointerImpl(index,Handle());
        continue;
      }

      // look for target
      Handle targetHandle;
      if (ptrsAsHandles) {
        targetHandle = toUUID(targetName);
        if (!workspace().isMember(targetHandle)) {
          if (!expectToLosePointers) {
            LOG(Trace,"Field " << index << " of '" << iddObject().name() << "' object points to an object with handle " << toString(targetHandle)
                << ", but there is not object with that handle in the Workspace. Will try to "
                << "interpret as a name.");
          }
          targetHandle = Handle();
        }
      }
      if (targetHandle.isNull()) {
        StringSet intermediate = iddObject().objectLists(index);
        StringVector referenceLists(intermediate.begin(),intermediate.end());
        OptionalWorkspaceObject target = m_workspace->getObjectByNameAndReference(targetName,referenceLists);
        if (target) {
          targetHandle = target->handle();
        }
      }
      setPointerImpl(index,targetHandle);
      if (targetHandle.isNull()) {
        if (!expectToLosePointers) {
          LOG(Warn,briefDescription() << ", points to an object named " << targetName
              << " from field " << index << ", but that object cannot be located.");
        }
      }
    }
  }

  void WorkspaceObject_Impl::initializeOnClone(const HandleMap& oldNewHandleMap) {
    OS_ASSERT(m_workspace);
    if (m_sourceData) {
      SourceData::pointer_set mappedPointers;
      BOOST_FOREACH(const ForwardPointer& fp,m_sourceData->pointers) {
        Handle th = openstudio::applyHandleMap(fp.targetHandle,oldNewHandleMap);
        if (th.isNull() && !fp.targetHandle.isNull() && !oldNewHandleMap.empty()) {
          // if cloned object is also in this workspace, and fp.targetHandle not in
          // the map, may be in the workspace
          OptionalWorkspaceObject target = workspace().getObject(fp.targetHandle);
          if (target) {
            // need to set reverse pointer
            target->getImpl<WorkspaceObject_Impl>()->setReversePointer(handle(),fp.fieldIndex);
            th = fp.targetHandle;
          }
        }
        mappedPointers.insert(ForwardPointer(fp.fieldIndex,th));
        if (!th.isNull()) {
          m_workspace->forwardReferences(m_handle,fp.fieldIndex,th);
        }
      }
      m_sourceData->pointers = mappedPointers;
    }
    if (m_targetData) {
      TargetData::pointer_set mappedPointers;
      BOOST_FOREACH(const ReversePointer& rp,m_targetData->reversePointers) {
        Handle sh = openstudio::applyHandleMap(rp.sourceHandle,oldNewHandleMap);
        if (!sh.isNull()) {
          mappedPointers.insert(ReversePointer(sh,rp.fieldIndex));
        }
      }
      m_targetData->reversePointers = mappedPointers;
    }
  }

  // GETTERS

  Workspace_Impl* WorkspaceObject_Impl::workspaceImpl() const {
    return m_workspace;
  }

  Workspace WorkspaceObject_Impl::workspace() const {
    return Workspace(workspaceImpl()->shared_from_this());
  }

  boost::optional<std::string> WorkspaceObject_Impl::getString(unsigned index,
                                                               bool returnDefault,
                                                               bool returnUninitializedEmpty) const
  {
    if (canBeSource(index) && (index < numFields())) {
      if (!initialized()) { return boost::none; }

      // pointer field, return name of pointed-to object
      OptionalWorkspaceObject oTarget = getTarget(index);
      if (!oTarget) {
        // implicitly or explicitly a null pointer
        if (returnDefault) {
          // get default from idd and return if exists
          OptionalIddField iddField = iddObject().getField(index);
          OS_ASSERT(iddField);
          if (iddField->properties().stringDefault) {
            return iddField->properties().stringDefault;
          }
        }
        if (returnUninitializedEmpty){
          return boost::none;
        }
        return std::string();
      }
      else {
        // return target's name
        return oTarget->name();
      }
    }

    // regular field, let IdfObject_Impl take care of it
    return IdfObject_Impl::getString(index,returnDefault,returnUninitializedEmpty);
  }

  OptionalWorkspaceObject WorkspaceObject_Impl::getTarget(unsigned index) const {
    if (!initialized()) { return boost::none; }

    if (m_sourceData) {
      // find index and return target if handle not null
      SourceData::pointer_set::const_iterator fpIt =
         getConstIteratorAtFieldIndex<SourceData>(m_sourceData->pointers,index);
      if (fpIt != m_sourceData->pointers.end()) {
        Handle th = fpIt->targetHandle;
        if (!th.isNull()) {
          return this->workspace().getObject(th);
        }
      }
    }

    return boost::none;
  }

  std::vector<WorkspaceObject> WorkspaceObject_Impl::targets() const {
    WorkspaceObjectVector result;
    if (!initialized()) { return result; }
    if (m_sourceData) {
      BOOST_FOREACH(const ForwardPointer& ptr,m_sourceData->pointers) {
        if (!ptr.targetHandle.isNull()) {
          OptionalWorkspaceObject owo = this->workspace().getObject(ptr.targetHandle);
          OS_ASSERT(owo);
          result.push_back(*owo);
        }
      }
    }
    return result;
  }

  std::vector<unsigned> WorkspaceObject_Impl::getSourceIndices(const Handle& targetHandle) const {
    UnsignedVector result;
    if (m_sourceData) {
      BOOST_FOREACH(const ForwardPointer& ptr,m_sourceData->pointers) {
        if (ptr.targetHandle == targetHandle) {
          result.push_back(ptr.fieldIndex);
        }
      }
    }
    return result;
  }

  WorkspaceObjectVector WorkspaceObject_Impl::sources() const {
    WorkspaceObjectVector result;
    if (!initialized()) { return result; }
    if (m_targetData) {
      BOOST_FOREACH(const ReversePointer& ptr,m_targetData->reversePointers) {
        OS_ASSERT(!ptr.sourceHandle.isNull());
        OptionalWorkspaceObject owo = this->workspace().getObject(ptr.sourceHandle);
        OS_ASSERT(owo);
        result.push_back(*owo);
      }
    }
    return result;
  }

  WorkspaceObjectVector WorkspaceObject_Impl::getSources(IddObjectType type) const {
    WorkspaceObjectVector result;
    if (!initialized()) { return result; }
    if (m_targetData) {
      BOOST_FOREACH(const ReversePointer& ptr,m_targetData->reversePointers) {
        OS_ASSERT(!ptr.sourceHandle.isNull());
        OptionalWorkspaceObject owo = this->workspace().getObject(ptr.sourceHandle);
        OS_ASSERT(owo);
        if (owo->iddObject().type() == type) { result.push_back(*owo); }
      }
    }
    return result;
  }

  ReversePointerSet WorkspaceObject_Impl::getReversePointers() const {
    ReversePointerSet result;
    if (m_handle.isNull()) { return result; }
    if (m_targetData) { result = m_targetData->reversePointers; }
    return result;
  }

  // SETTERS

  boost::optional<std::string> WorkspaceObject_Impl::setName(const std::string& newName) {
    boost::optional<std::string> result = setName(newName,true);
    if (result) {
      this->emitChangeSignals();
    }
    return result;
  }

  boost::optional<std::string> WorkspaceObject_Impl::setName(const std::string& newName,
                                                             bool checkValidity)
  {
    if (m_handle.isNull()) {
      return false;
    }
    StrictnessLevel level = m_workspace->strictnessLevel();

    OptionalUnsigned index = iddObject().nameFieldIndex();
    if (!index) {
      return false;
    }

    if (checkValidity && (level > StrictnessLevel::None)) {

      // do not set if would violate field NullAndRequired
      if ((level > StrictnessLevel::Draft) && newName.empty() && iddObject().isRequiredField(*index)) {
        return false;
      }

      OptionalString oldName = name();
      OptionalString result = IdfObject_Impl::setName(newName,false);
      if (!result) {
        return result;
      }

      // check collection NameConflict
      if (!uniquelyIdentifiableByName()) {
        result = IdfObject_Impl::setName(workspace().nextName(*result,false));
        OS_ASSERT(result);
      }

      return result;
    }

    return IdfObject_Impl::setName(newName,checkValidity);
  }

  boost::optional<std::string> WorkspaceObject_Impl::createName() {
    return createName(true);
  }

  boost::optional<std::string> WorkspaceObject_Impl::createName(bool overwrite) {
    OptionalString result;
    if (OptionalUnsigned index = iddObject().nameFieldIndex()) {
      OptionalString oName = name();
      if (!oName || oName->empty() || overwrite) {
        Workspace workspace = this->workspace();

        // determine name, then set
        std::string newName;
        if (iddObject().type() != IddObjectType::UserCustom) {
          newName = workspace.nextName(iddObject().type(),true);
        }
        else {
          newName = iddObjectNameToIdfObjectName(iddObject().name());
          newName = workspace.nextName(newName,true);
        }
        result = setName(newName);
      }
    }
    return result;
  }

  bool WorkspaceObject_Impl::setString(unsigned index, const std::string& value) {
    bool result = setString(index,value,true);
    if (result) {
      this->emitChangeSignals();
    }
    return result;
  }

  // Pre-condition:  Object valid at Workspace's strictness level.
  bool WorkspaceObject_Impl::setString(unsigned index, const std::string& value, bool checkValidity)
  {
    if (m_handle.isNull()) { return false; }
    StrictnessLevel level = m_workspace->strictnessLevel();

    if (canBeSource(index)) {
      // pointer field
      OptionalHandle targetHandle = convertToTargetHandle(value,iddObject().objectLists(index),checkValidity);
      if (targetHandle) {
        return setPointer(index,*targetHandle,checkValidity);
      }
      return false;
    }

    // regular field -- name or data
    if ((index == 0) && (iddObject().hasNameField())) {
      return setName(value,checkValidity);
    } // name

    // record diffs at start
    unsigned diffSize = m_diffs.size();

    // field already exists
    if (index < numFields()) {
      // validity checking and reconstitution is limited to field index
      OptionalString oldValue = getString(index);
      bool result = IdfObject_Impl::setString(index,value,false);
      if (!result) {
        return false;
      }
      if (checkValidity) {
        if (!fieldDataIsValid(index,level).empty()) {
          // rollback
          IdfObject_Impl::setString(index,*oldValue,false);

          // remove the diffs
          m_diffs.resize(diffSize);

          return false;
        }
      }
      return true;
    }

    // field does not yet exist. need to check whole object for validity, and pop fields if
    // any operations are unsuccessful or result in an invalid object
    unsigned n = numFields();
    bool result = false;
    if (m_iddObject.isNonextensibleField(index) || m_iddObject.isExtensibleField(index)) {
      result = true;
      while (result && (index >= m_fields.size())) {
        result = pushString(std::string(),false);
      }
      result = true;
      while (result && (index >= m_fields.size())) {
        result = !pushExtensibleGroup(StringVector(),false).empty();
      }
      result = IdfObject_Impl::setString(index,value,false);
      if (checkValidity) {
        result = result && isValid(m_workspace->strictnessLevel());
      }
      if (!result) {
        restoreOriginalNumFields(n);

        // remove the diffs
        m_diffs.resize(diffSize);

        return false;
      }
    }

    return result;
  }

  bool WorkspaceObject_Impl::setPointer(unsigned index, const Handle& targetHandle) {
    bool result = setPointer(index, targetHandle, true);
    if (result) {
      this->emitChangeSignals();
    }
    return result;
  }

  bool WorkspaceObject_Impl::setPointer(unsigned index,
                                        const Handle& targetHandle,
                                        bool checkValidity)
  {
    if (m_handle.isNull()) {
      return false;
    }

    // essential hurdles
    if (canBeSource(index) && (targetHandle.isNull() || m_workspace->isMember(targetHandle))) {

      StrictnessLevel level = m_workspace->strictnessLevel();

      // field NullAndRequired
      if (checkValidity &&
          (level > StrictnessLevel::Draft) &&
          targetHandle.isNull() &&
          iddObject().isRequiredField(index))
      {
        return false;
      }

      // field PointerType
      if (checkValidity && (level > StrictnessLevel::None) && (!targetHandle.isNull()) &&
          (!m_workspace->canBeTarget(targetHandle,iddObject().objectLists(index))))
      {
        return false;
      }

      // record diffs at start
      unsigned diffSize = m_diffs.size();
      bool checkValid = false; // check validity at object level?
      if (checkValidity && (level > StrictnessLevel::None) &&
          (m_workspace->iddFileType() == IddFileType::OpenStudio))
      {
        // there may be model-level checks on this field
        checkValid = true;
      }
      OptionalUnsigned n;
      if (index >= numFields()) {
        if (checkValidity) {
          checkValid = true;
        }
        n = numFields();
        if (index-1 >= numFields()) {
          // call set string to add all fields up to this one
          setString(index-1,std::string(),false);
        }
      }

      Handle oldHandle = setPointerImpl(index, targetHandle);

      OptionalString oldValue, newValue;
      if (iddObject().hasHandleField()) {
        oldValue = toString(oldHandle);
        newValue = toString(targetHandle);
      }
      else {
        oldValue = m_workspace->name(oldHandle);
        newValue = m_workspace->name(targetHandle);
      }

      m_diffs.push_back(WorkspaceObjectDiff(index, oldValue, newValue, oldHandle, targetHandle));

      if (checkValid && !isValid(level,false)) {
        if (n) {
          restoreOriginalNumFields(*n);
        }
        else {
          setPointerImpl(index,oldHandle);
        }

        // remove the diffs
        m_diffs.resize(diffSize);

        return false;
      }

      return true;
    }

    return false;
  }

  bool WorkspaceObject_Impl::pushString() {
    bool result = pushString(std::string(),true);
    if (result) {
      this->emitChangeSignals();
    }
    return result;
  }

  bool WorkspaceObject_Impl::pushString(bool checkValidity) {
    return pushString(std::string(),checkValidity);
  }

  bool WorkspaceObject_Impl::pushString(const std::string& value) {
    bool result = pushString(value,true);
    if (result) {
      this->emitChangeSignals();
    }
    return result;
  }

  bool WorkspaceObject_Impl::pushString(const std::string& value, bool checkValidity) {
    if (m_handle.isNull()) {
      return false;
    }

    unsigned index = numFields();

    // pointer field?
    if (canBeSource(index)) {
      OptionalHandle targetHandle = convertToTargetHandle(value,iddObject().objectLists(index),checkValidity);
      if (targetHandle) {
        return pushPointer(*targetHandle,checkValidity);
      }
      return false;
    }

    unsigned diffSize = m_diffs.size();

    // regular field
    bool result = IdfObject_Impl::pushString(value,checkValidity); // nominally add
    if (result) {
      // let WorkspaceObject_Impl::setString check validity
      result = setString(index,value,checkValidity);
    }

    if (!result) {
      restoreOriginalNumFields(index);

      // remove diffs
      m_diffs.resize(diffSize);
    }

    return result;
  }

  bool WorkspaceObject_Impl::pushPointer(const Handle& targetHandle) {
    bool result = pushPointer(targetHandle, true);
    if (result) {
      this->emitChangeSignals();
    }
    return result;
  }

  bool WorkspaceObject_Impl::pushPointer(const Handle& targetHandle, bool checkValidity) {
    if (m_handle.isNull()) {
      return false;
    }

    unsigned index = numFields();

    StrictnessLevel level = m_workspace->strictnessLevel();
    // essential hurdles
    if (canBeSource(index) && (targetHandle.isNull() || m_workspace->name(targetHandle))) {
      // field NullAndRequired (applicable if extensible group of size 1)
      if (checkValidity && (level > StrictnessLevel::Draft) && targetHandle.isNull() && iddObject().isRequiredField(index)) {
        return false;
      }
      // field PointerType
      if (checkValidity && (level > StrictnessLevel::None) && (!targetHandle.isNull()) &&
          (!m_workspace->canBeTarget(targetHandle,iddObject().objectLists(index))))
      {
        return false;
      }
      bool result = IdfObject_Impl::pushString(checkValidity);
      if (result) {
        result = setPointer(index,targetHandle,checkValidity);
      }
      return result;
    }
    return false;
  }

  std::vector<std::string> WorkspaceObject_Impl::popExtensibleGroup() {
    StringVector result = popExtensibleGroup(true);
    if (!result.empty()) {
      this->emitChangeSignals();
    }
    return result;
  }

  std::vector<std::string> WorkspaceObject_Impl::popExtensibleGroup(bool checkValidity) {
    StringVector result;
    if (!initialized()) {
      UnsignedVector olFields = iddObject().objectListFields();
      if (!olFields.empty() && iddObject().isExtensibleField(olFields.back())) {
        return result;
      }
    }

    // check minFields in Final Strictness
    if (m_workspace->strictnessLevel() == StrictnessLevel::Final) {
      unsigned groupSize = m_iddObject.properties().numExtensible;
      unsigned numAfterPop = numFields() - groupSize;
      if (numAfterPop < m_iddObject.properties().minFields) {
        return result;
      }
    }

    // erase fields, handling pointers as necessary
    result = IdfObject_Impl::popExtensibleGroup(checkValidity);
    if (m_sourceData && !result.empty()) {
      unsigned n = numFields();
      UnsignedVector outOfRangePtrs;
      BOOST_FOREACH(const ForwardPointer& ptr,m_sourceData->pointers) {
        if (ptr.fieldIndex >= n) {
          outOfRangePtrs.push_back(ptr.fieldIndex);
        }
      }
      BOOST_FOREACH(unsigned index,outOfRangePtrs) {
        nullifyPointer(index);
        SourceData::pointer_set::iterator fpIt =
            getIteratorAtFieldIndex<SourceData>(m_sourceData->pointers,index);
        OS_ASSERT(fpIt != m_sourceData->pointers.end());
        m_sourceData->pointers.erase(fpIt);
      }
    }

    return result;
  }

  // QUERIES

  bool WorkspaceObject_Impl::initialized() const {
    return m_initialized && (!m_handle.isNull());
  }

  unsigned WorkspaceObject_Impl::numSources() const {
    if (m_handle.isNull()) { return false; }
    if (m_targetData) { return m_targetData->reversePointers.size(); }
    return 0;
  }

  bool WorkspaceObject_Impl::isSource() const {
    if (!initialized()){ return false; }
    if (m_sourceData) {
      BOOST_FOREACH(const ForwardPointer& fPtr,m_sourceData->pointers) {
        if (!fPtr.targetHandle.isNull()) { return true; }
      }
    }
    return false;
  }

  bool WorkspaceObject_Impl::isSource(unsigned index) const {
    if (m_handle.isNull()) { return false; }
    if (m_sourceData) {
      SourceData::pointer_set::const_iterator it =
        getConstIteratorAtFieldIndex<SourceData>(m_sourceData->pointers,index);
      if ((it != m_sourceData->pointers.end()) && (!it->targetHandle.isNull())) {
        return true;
      }
    }
    return false;
  }

  bool WorkspaceObject_Impl::canBeSource() const {
    if (m_handle.isNull()) {
      return false;
    }
    return m_sourceData;
  }

  bool WorkspaceObject_Impl::canBeSource(unsigned index,const StringVector& refLists) const {
    if (m_handle.isNull()) {
      return false;
    }
    OptionalIddField iddField = iddObject().getField(index);
    if (iddField) {
      if (refLists.empty()) {
        return iddField->isObjectListField();
      }
      else {
        StringVector objLists = iddField->properties().objectLists;
        StringVector intersection = intersectReferenceLists(refLists,objLists);
        return !intersection.empty();
      }
    }
    return false;
  }

  bool WorkspaceObject_Impl::isTarget() const {
    if (m_handle.isNull()) { return false; }
    return m_targetData;
  }

  std::vector<std::string> WorkspaceObject_Impl::canBeTarget() const {
    StringVector result;
    if (!m_handle.isNull() && name()) { result = iddObject().references(); }
    return result;
  }

  bool WorkspaceObject_Impl::objectListFieldsEqual(const WorkspaceObject& other) const {
    // Looks for object equality of targets if same workspace(), otherwise, looks for data
    // field equality of targets.
    if (iddObject() != other.iddObject()) { return false; }
    UnsignedVector myFields = objectListFields();
    UnsignedVector otherFields = other.objectListFields();
    if (myFields != otherFields) { return false; }

    // iddObject() same, field indices same--compare data
    BOOST_FOREACH(unsigned i,myFields) {
      OptionalWorkspaceObject oMyTarget = getTarget(i);
      OptionalWorkspaceObject oOtherTarget = other.getTarget(i);
      if (oMyTarget || oOtherTarget) {
        // at least one is non-null
        if (!(oMyTarget && oOtherTarget)) { return false; }
        WorkspaceObject myTarget = *oMyTarget;
        WorkspaceObject otherTarget = *oOtherTarget;
        if (workspace() == other.workspace()) {
          if (myTarget != otherTarget) { return false; }
        }
        else {
          if (!myTarget.dataFieldsEqual(otherTarget)) { return false; }
        }
      }
    }

    return true;
  }

  bool WorkspaceObject_Impl::objectListFieldsNonConflicting(const WorkspaceObject& other) const {
    // Looks for object equality of targets if same workspace(), otherwise, looks for data
    // field equality of targets.
    if (iddObject() != other.iddObject()) { return false; }
    UnsignedVector myFields = objectListFields();
    UnsignedVector otherFields = other.objectListFields();
    // require same managedObjectLists fields to at least exist
    if (myFields != otherFields) { return false; }

    // iddObject() same--compare data
    BOOST_FOREACH(unsigned i,myFields) {
      OptionalWorkspaceObject oMyTarget = getTarget(i);
      // always ok if I am not pointing to anyone
      if (!oMyTarget) { continue; }

      OptionalWorkspaceObject oOtherTarget = other.getTarget(i);
      // ok if other is not pointing to anyone
      if (!oOtherTarget) { continue; }

      WorkspaceObject myTarget = *oMyTarget;
      WorkspaceObject otherTarget = *oOtherTarget;
      if (workspace() == other.workspace()) {
        if (myTarget != otherTarget) { return false; }
      }
      else {
        if (!myTarget.dataFieldsEqual(otherTarget)) { return false; }
      }
    }

    return true;
  }

  // SERIALIZATION

  IdfObject_ImplPtr WorkspaceObject_Impl::idfObjectImplPtr() {
    if (!initialized()) {
      LOG_AND_THROW("Attempt to write a disconnected WorkspaceObject out to Idf.");
    }

    // construct new IdfObject from WorkspaceObject's data
    IdfObject_ImplPtr result(new IdfObject_Impl(m_handle,
                                                m_comment,
                                                m_iddObject,
                                                m_fields,
                                                m_fieldComments));
    // add name references based on WorkspaceObject's pointer data
    if (m_sourceData) {
      bool serializeHandle = m_iddObject.hasHandleField();
      BOOST_FOREACH(const ForwardPointer& ptr,m_sourceData->pointers) {
        if (!ptr.targetHandle.isNull()) {
          if (serializeHandle) {
            result->setString(ptr.fieldIndex,toString(ptr.targetHandle));
          }
          else {
            OptionalString targetName = m_workspace->name(ptr.targetHandle);
            OS_ASSERT(targetName);
            if (targetName->empty()) {
              // give target a name
              OptionalWorkspaceObject target = m_workspace->getObject(ptr.targetHandle);
              OS_ASSERT(target);
              target->createName(false);
              targetName = target->name();
              OS_ASSERT(targetName);
            }
            result->setString(ptr.fieldIndex,*targetName);
          }
        }
      }
    }
    return result;
  }

  IdfObject_ImplPtr WorkspaceObject_Impl::idfObjectImplPtr() const {
    if (!initialized()) {
      LOG_AND_THROW("Attempt to write a disconnected WorkspaceObject out to Idf.");
    }

    // construct new IdfObject from WorkspaceObject's data
    IdfObject_ImplPtr result(new IdfObject_Impl(m_handle,
                                                m_comment,
                                                m_iddObject,
                                                m_fields,
                                                m_fieldComments));
    // add name references based on WorkspaceObject's pointer data
    if (m_sourceData) {
      bool serializeHandle = m_iddObject.hasHandleField();
      BOOST_FOREACH(const ForwardPointer& ptr,m_sourceData->pointers) {
        if (!ptr.targetHandle.isNull()) {
          if (serializeHandle) {
            result->setString(ptr.fieldIndex,toString(ptr.targetHandle));
          }
          else {
            OptionalString targetName = m_workspace->name(ptr.targetHandle);
            OS_ASSERT(targetName);
            result->setString(ptr.fieldIndex,*targetName);
          }
        }
      }
    }
    return result;
  }

  /** Returns equivalent IdfObject, naming targets if necessary. All data is cloned. */
  IdfObject WorkspaceObject_Impl::idfObject()
  {
    return getObject<WorkspaceObject>().idfObject();
  }

  /** Returns equivalent IdfObject, leaving unnamed target objects unnamed. All data is cloned. */
  IdfObject WorkspaceObject_Impl::idfObject() const
  {
    return getObject<WorkspaceObject>().idfObject();
  }

  void WorkspaceObject_Impl::emitChangeSignals()
  {
    if (m_diffs.empty()){
      return;
    }

    bool nameChange = false;
    bool dataChange = false;

    BOOST_FOREACH(const IdfObjectDiff& diff, m_diffs){

      if (diff.isNull()){
        continue;
      }

      boost::optional<unsigned> index = diff.index();
      if (index){

        OptionalIddField oIddField = iddObject().getField(*index);

        if (oIddField && oIddField->isObjectListField() && diff.optionalCast<WorkspaceObjectDiff>()) {

          WorkspaceObjectDiff workspaceObjectDiff = diff.cast<WorkspaceObjectDiff>();

          Handle newHandle;
          if (workspaceObjectDiff.newHandle()){
            newHandle = workspaceObjectDiff.newHandle().get();
          }

          Handle oldHandle;
          if (workspaceObjectDiff.oldHandle()){
            oldHandle = workspaceObjectDiff.oldHandle().get();
          }

          emit onRelationshipChange(*index, newHandle, oldHandle);

        } else if (oIddField && oIddField->isNameField()) {
          nameChange = true;
        } else {
          dataChange = true;
        }
      }
    }

    if (nameChange){
      emit onNameChange();
    }

    if (dataChange){
      emit onDataChange();
    }

    emit onChange();

    m_diffs.clear();
  }

  // PROTECTED

  void WorkspaceObject_Impl::setInitialized() {
    m_initialized = true;
  }

  void WorkspaceObject_Impl::disconnect() {
    emit onRemoveFromWorkspace(m_handle);
    m_handle = Handle();
    m_workspace = 0;
  }

  // Pre-condition:  field index is a pointer, and its targetHandle is either null or valid in
  //                 m_workspace.
  // Post-condition: field index is a pointer with a null targetHandle.
  void WorkspaceObject_Impl::nullifyPointer(unsigned index) {
    OS_ASSERT(!m_handle.isNull());
    // reverse pointer
    OptionalWorkspaceObject oTarget = getTarget(index);
    if (oTarget) {
      WorkspaceObject target = *oTarget;
      target.getImpl<WorkspaceObject_Impl>()->nullifyReversePointer(m_handle,index);
      // remove forwarded reference if no other source sets the same
      m_workspace->removeForwardedReferences(handle(),index,target);
    }

    // forward pointer
    SourceData::pointer_set::iterator fpIt =
         getIteratorAtFieldIndex<SourceData>(m_sourceData->pointers,index);
    OS_ASSERT(fpIt != m_sourceData->pointers.end());
    m_sourceData->pointers.erase(fpIt);
    std::pair<SourceData::pointer_set::iterator,bool> insertResult;
    insertResult = m_sourceData->pointers.insert(ForwardPointer(index,Handle()));
    OS_ASSERT(insertResult.second);
  }

  // Pre-condition:  Object sourceHandle points to this object from field index.
  // Post-condition: That information is removed from this object's m_targetData (in preparation for
  //                 a change to the source pointer).
  void WorkspaceObject_Impl::nullifyReversePointer(const Handle& sourceHandle,unsigned index) {
    OS_ASSERT(!m_handle.isNull());
    OS_ASSERT(m_targetData);
    TargetData::pointer_set::iterator it =
        m_targetData->reversePointers.find(ReversePointer(sourceHandle,index));
    OS_ASSERT(it != m_targetData->reversePointers.end());
    m_targetData->reversePointers.erase(it);
  }

  // Pre-condition:  ReversePointer(sourceHandle,index) is not in m_targetData.
  // Post-condition: m_targetData indicates that object sourceHandle points to this object from
  //                 field index.
  void WorkspaceObject_Impl::setReversePointer(const Handle& sourceHandle, unsigned index) {
    OS_ASSERT(!m_handle.isNull());
    if (!m_targetData) { m_targetData = TargetData(); }
    // automatically maintains uniqueness
    std::pair<TargetData::pointer_set::iterator,bool> insertResult;
    insertResult = m_targetData->reversePointers.insert(ReversePointer(sourceHandle,index));
    OS_ASSERT(insertResult.second);
  }

  void WorkspaceObject_Impl::restorePointers() {
    OS_ASSERT(!m_handle.isNull());
    if (m_sourceData) {
      BOOST_FOREACH(const ForwardPointer& ptr,m_sourceData->pointers) {
        if (!ptr.targetHandle.isNull()) {
          OptionalWorkspaceObject target = m_workspace->getObject(ptr.targetHandle);
          if (target) {
            WorkspaceObjectVector sources = target->getSources(iddObject().type());
            HandleVector h = getHandles<WorkspaceObject>(sources);
            if (std::find(h.begin(),h.end(),m_handle) == h.end()) {
              target->getImpl<WorkspaceObject_Impl>()->setReversePointer(m_handle,ptr.fieldIndex);
            }
          }
        }
      }
    }
    if (m_targetData) {
      BOOST_FOREACH(const ReversePointer& ptr,m_targetData->reversePointers) {
        OptionalWorkspaceObject source = m_workspace->getObject(ptr.sourceHandle);
        if (source) {
          OptionalWorkspaceObject oTarget = source->getTarget(ptr.fieldIndex);
          OptionalHandle h;
          if (oTarget) { h = oTarget->handle(); }
          if ((!h) || (*h != m_handle)) {
            source->getImpl<WorkspaceObject_Impl>()->setPointer(ptr.fieldIndex,m_handle,false);
          }
        }
      }
    }
  }

  // PRIVATE

  // SETTERS

  // Pre-condition:  targetHandle is null or in m_workspace. index is an object-list field.
  // Post-condition: Field index points to object targetHandle.
  Handle WorkspaceObject_Impl::setPointerImpl(unsigned index, const Handle& targetHandle) {
    OS_ASSERT(!m_handle.isNull());
    Handle result;
    // check current status
    SourceData::pointer_set::iterator fpIt =
         getIteratorAtFieldIndex<SourceData>(m_sourceData->pointers,index);
    if (fpIt == m_sourceData->pointers.end()) {
      /*bool result =*/ IdfObject_Impl::setString(index,std::string(),false);
    }
    else { // entry exists
      if (!fpIt->targetHandle.isNull()) {
        result = fpIt->targetHandle;
        if (fpIt->targetHandle == targetHandle) { return result; } // nothing to do
        nullifyPointer(index); // takes care of reverse pointer
      }
    }
    // add pointer
    fpIt = getIteratorAtFieldIndex<SourceData>(m_sourceData->pointers,index);
    if (fpIt != m_sourceData->pointers.end()) {
      m_sourceData->pointers.erase(fpIt);
    }
    std::pair<SourceData::pointer_set::iterator,bool> insertResult;
    insertResult = m_sourceData->pointers.insert(ForwardPointer(index,targetHandle));
    OS_ASSERT(insertResult.second);

    // add reverse pointer
    if (!targetHandle.isNull()) {
      OptionalWorkspaceObject target = m_workspace->getObject(targetHandle);
      OS_ASSERT(target);
      target->getImpl<WorkspaceObject_Impl>()->setReversePointer(m_handle,index);
      // forward references if is object-list and defines references simultaneously
      m_workspace->forwardReferences(m_handle,index,targetHandle);
    }

    return result;
  }

  boost::optional<Handle> WorkspaceObject_Impl::convertToTargetHandle(
      const std::string& name,const std::set<std::string>& referenceLists,bool checkValidity) const
  {
    OS_ASSERT(!m_handle.isNull());

    if (name.empty()) { return Handle(); } // null pointer

    WorkspaceObjectVector candidates = m_workspace->getObjectsByName(name);

    if (candidates.size() == 0) { return boost::none; }
    if (candidates.size() == 1) { return candidates[0].handle(); }

    // multiple. choose first in appropriate reference list.
    BOOST_FOREACH(const WorkspaceObject& candidate,candidates) {
      if (m_workspace->canBeTarget(candidate.handle(),referenceLists)) {
        return candidate.handle();
      }
    }
    // none found. connect to first candidate if no strictness.
    if ((!checkValidity) || (m_workspace->strictnessLevel() < StrictnessLevel::Draft)) {
      return candidates[0].handle();
    }
    return boost::none;
  }

  void WorkspaceObject_Impl::restoreOriginalNumFields(unsigned n) {
    bool popResult = true;
    while (popResult && (numFields() > n)) {
      popResult = !popExtensibleGroup().empty();
    }
    popResult = true;
    while (popResult && (numFields() > n)) {
      popResult = popField();
    }
  }

  bool WorkspaceObject_Impl::popField() {
    if (m_handle.isNull()) { return false; }

    unsigned index = numFields()-1;
    // last field must be nonextensible, and final size must satisfy minimum number of fields
    if ((index >= minFields()) && (numExtensibleGroups() == 0)) {
      // delete field
      m_diffs.push_back(IdfObjectDiff(index, m_fields[index], boost::none));
      m_fields.pop_back();
      if (m_fieldComments.size() > m_fields.size()) {
        m_fieldComments.resize(m_fields.size());
      }
    } else {
      return false;
    }

    if (canBeSource(index)) {
      // pointer field
      OS_ASSERT(m_sourceData);
      // nullify the pointer
      nullifyPointer(index);
      // get the pointer
      SourceData::pointer_set::iterator fpIt =
        getIteratorAtFieldIndex<SourceData>(m_sourceData->pointers,index);
      OS_ASSERT(fpIt != m_sourceData->pointers.end());
      // erase the pointer
      m_sourceData->pointers.erase(fpIt);
    }

    return true;
  }

  // QUERY HELPERS

  void WorkspaceObject_Impl::populateValidityReport(ValidityReport& report, bool checkNames) const
  {
    // StrictnessLevel::None

    // DataErrorType::NotInitialized
    // object-level
    if (!initialized()) {
      report.insertError(DataError(getObject<WorkspaceObject>(),DataErrorType(DataErrorType::NotInitialized)));
      return;
    }

    // NOW INHERIT FROM IDFOBJECT
    IdfObject_Impl::populateValidityReport(report,checkNames);

    // StrictnessLevel::Draft
    if (report.level() > StrictnessLevel::None) {
      // DataErrorType::NameConflict
      // object-level
      if (checkNames && !uniquelyIdentifiableByName()) {
        report.insertError(DataError(getObject<WorkspaceObject>(),DataErrorType(DataErrorType::NameConflict)));
      }
    } // StrictnessLevel::Draft
  }

  bool WorkspaceObject_Impl::uniquelyIdentifiableByName() const {
    OptionalString oName = name();
    if (!oName) {
      return true;
    }
    WorkspaceObjectVector candidates = m_workspace->getObjectsByReference(iddObject().references());
    BOOST_FOREACH(const WorkspaceObject& candidate,candidates) {
      OptionalString candidateName = candidate.name();
      OS_ASSERT(candidateName);
      if ((istringEqual(*oName,*candidateName) &&
          (!initialized() || (getObject<WorkspaceObject>() != candidate))))
      {
        return false;
      }
    }
    return true;
  }

  bool WorkspaceObject_Impl::fieldDataIsCorrectType(unsigned index) const {
    OptionalIddField oIddField = iddObject().getField(index);
    if (!oIddField) { return true; }

    IddField iddField = *oIddField;
    IddFieldType fieldType = iddField.properties().type;
    OS_ASSERT(m_fields.size() > index);

    bool result = true;
    if ((fieldType == IddFieldType::ObjectListType) && m_sourceData) {
      // automatically excludes unsupported reference lists by going through m_sourceData
      SourceData::pointer_set::const_iterator it = getConstIteratorAtFieldIndex<SourceData>(m_sourceData.get().pointers,index);
      if (it != m_sourceData.get().pointers.end()) {
        ForwardPointer ptr = *it;
        if (!ptr.targetHandle.isNull()) {
          result = m_workspace->canBeTarget(ptr.targetHandle,iddObject().objectLists(index));
        }
      }
    }
    else {
      result = IdfObject_Impl::fieldDataIsCorrectType(index);
    }

    return result;
  }

  bool WorkspaceObject_Impl::fieldIsNonnullIfRequired(unsigned index) const {
    bool result = true;

    OptionalIddField oIddField = iddObject().getField(index);
    if (!oIddField) { return result; }

    IddField field = *oIddField;
    if (m_sourceData) {
      SourceData::pointer_set::const_iterator it = getConstIteratorAtFieldIndex<SourceData>(m_sourceData.get().pointers,index);
      if (it != m_sourceData.get().pointers.end()) {
        if (it->targetHandle.isNull() && field.properties().required) {
          result = false;
        }
        return result;
      }
    }
    // not managed by WorkspaceObject_Impl, defer to IdfObject_Impl
    result = IdfObject_Impl::fieldIsNonnullIfRequired(index);
    return result;
  }

  struct WorkspaceObjectMetaTypeInitializer
  {
    WorkspaceObjectMetaTypeInitializer()
    {
      qRegisterMetaType<boost::shared_ptr<openstudio::detail::WorkspaceObject_Impl> >();
    }
  };

  static WorkspaceObjectMetaTypeInitializer __workspaceObjectMetaTypeInitializer__;

} // detail

std::vector<IdfObject> WorkspaceObject::remove()
{
  return getImpl<detail::WorkspaceObject_Impl>()->remove();
}

// GETTERS

Workspace WorkspaceObject::workspace() const
{
  return getImpl<detail::WorkspaceObject_Impl>()->workspace();
}

OptionalWorkspaceObject WorkspaceObject::getTarget(unsigned index) const {
  return getImpl<WorkspaceObject_Impl>()->getTarget(index);
}

WorkspaceObjectVector WorkspaceObject::targets() const {
  return getImpl<WorkspaceObject_Impl>()->targets();
}

std::vector<unsigned> WorkspaceObject::getSourceIndices(const Handle& targetHandle) const {
  return getImpl<WorkspaceObject_Impl>()->getSourceIndices(targetHandle);
}

std::vector<WorkspaceObject> WorkspaceObject::sources() const {
  return getImpl<WorkspaceObject_Impl>()->sources();
}

std::vector<WorkspaceObject> WorkspaceObject::getSources(IddObjectType objectType) const {
  return getImpl<WorkspaceObject_Impl>()->getSources(objectType);
}

// SETTERS

bool WorkspaceObject::setPointer(unsigned index, const Handle& targetHandle) {
  return getImpl<WorkspaceObject_Impl>()->setPointer(index,targetHandle);
}

// QUERIES

bool WorkspaceObject::initialized() const {
  return getImpl<WorkspaceObject_Impl>()->initialized();
}

unsigned WorkspaceObject::numSources() const {
  return getImpl<WorkspaceObject_Impl>()->numSources();
}

bool WorkspaceObject::isSource() const {
  return getImpl<WorkspaceObject_Impl>()->isSource();
}

bool WorkspaceObject::canBeSource() const {
  return getImpl<WorkspaceObject_Impl>()->canBeSource();
}

bool WorkspaceObject::canBeSource(unsigned index,const StringVector& refLists) const {
  return getImpl<WorkspaceObject_Impl>()->canBeSource(index,refLists);
}

bool WorkspaceObject::isTarget() const {
  return getImpl<WorkspaceObject_Impl>()->isTarget();
}

std::vector<std::string> WorkspaceObject::canBeTarget() const {
  return getImpl<WorkspaceObject_Impl>()->canBeTarget();
}

bool WorkspaceObject::objectListFieldsEqual(const WorkspaceObject& other) const {
  return getImpl<WorkspaceObject_Impl>()->objectListFieldsEqual(other);
}

bool WorkspaceObject::objectListFieldsNonConflicting(const WorkspaceObject& other) const {
  return getImpl<WorkspaceObject_Impl>()->objectListFieldsNonConflicting(other);
}

// SERIALIZATION

IdfObject WorkspaceObject::idfObject() {
  IdfObject object(getImpl<WorkspaceObject_Impl>()->idfObjectImplPtr());
  return object;
}

IdfObject WorkspaceObject::idfObject() const {
  IdfObject object(getImpl<detail::WorkspaceObject_Impl>()->idfObjectImplPtr());
  return object;
}

// PROTECTED

WorkspaceObject::WorkspaceObject(boost::shared_ptr<detail::WorkspaceObject_Impl> impl)
  : IdfObject(impl)
{}

std::ostream& operator<<(std::ostream& os, const WorkspaceObject& workspaceObject)
{
  os << workspaceObject.idfObject();
  return os;
}

} // openstudio
