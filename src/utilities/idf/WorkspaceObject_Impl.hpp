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

#ifndef UTILITIES_IDF_WORKSPACEOBJECT_IMPL_HPP
#define UTILITIES_IDF_WORKSPACEOBJECT_IMPL_HPP

#include <utilities/UtilitiesAPI.hpp>
#include <nano/nano_signal_slot.hpp>  // Signal-Slot replacement

#include <utilities/idf/IdfObject_Impl.hpp>
#include <utilities/idf/ObjectPointer.hpp>

namespace openstudio {

// forward declarations
class WorkspaceObject;
class Workspace;

namespace detail {

  class Workspace_Impl;  // forward declaration

  struct UTILITIES_API ForwardPointer
  {
    unsigned fieldIndex;
    Handle targetHandle;

    /// \todo Default constructor needed to iterate over Source Map, but setting fieldIndex to 0
    /// seems sub-optimal.
    ForwardPointer() : fieldIndex(0) {}
    ForwardPointer(unsigned i, const Handle& h) : fieldIndex(i), targetHandle(h) {}
  };
  using ForwardPointerSet = std::set<ForwardPointer, FieldIndexLess<ForwardPointer>>;

  struct UTILITIES_API SourceData
  {
    using pointer_type = ForwardPointer;
    using pointer_set = ForwardPointerSet;

    pointer_set pointers;

    SourceData() = default;
  };
  using OptionalSourceData = boost::optional<SourceData>;

  struct UTILITIES_API ReversePointer
  {
    Handle sourceHandle;
    unsigned fieldIndex;

    ReversePointer() : fieldIndex(0) {}
    ReversePointer(const Handle& h, unsigned i) : sourceHandle(h), fieldIndex(i) {}
  };
  struct UTILITIES_API ReversePointerLess
  {
    bool operator()(const ReversePointer& left, const ReversePointer& right) const {
      if (left.sourceHandle == right.sourceHandle) {
        return (left.fieldIndex < right.fieldIndex);
      } else {
        return (left.sourceHandle < right.sourceHandle);
      }
    }
  };
  using ReversePointerSet = std::set<ReversePointer, ReversePointerLess>;

  struct UTILITIES_API TargetData
  {
    using pointer_type = ReversePointer;
    using pointer_set = ReversePointerSet;

    pointer_set reversePointers;
  };
  using OptionalTargetData = boost::optional<TargetData>;

  template <class T>
  typename T::pointer_set::iterator getIteratorAtFieldIndex(typename T::pointer_set& pointerSet, unsigned fieldIndex) {
    return std::find_if(pointerSet.begin(), pointerSet.end(),
                        [fieldIndex](const auto& ptr_type) { return fieldIndexEqualTo<typename T::pointer_type>(ptr_type, fieldIndex); });
  }

  template <class T>
  typename T::pointer_set::const_iterator getConstIteratorAtFieldIndex(const typename T::pointer_set& pointerSet, unsigned fieldIndex) {
    return std::find_if(pointerSet.begin(), pointerSet.end(),
                        [fieldIndex](const auto& ptr_type) { return fieldIndexEqualTo<typename T::pointer_type>(ptr_type, fieldIndex); });
  }

  class UTILITIES_API WorkspaceObject_Impl : public IdfObject_Impl
  {
   public:
    /** @name Constructors */
    //@{

    /** Construct from IdfObject_Impl (via idfObject). */
    WorkspaceObject_Impl(const IdfObject& idfObject, Workspace_Impl* workspace, bool keepHandle = false);

    /** Copy constructor. Assigns new handle, keeps same workspace pointer. Workspace should know
     *  whether copy is to live in original workspace, or in cloned workspace. If the latter,
     *  Workspace will need to update the workspace pointer, and any source and target pointer
     *  handles. */
    WorkspaceObject_Impl(const WorkspaceObject_Impl& other, Workspace_Impl* workspace, bool keepHandle = false);

    /** Complete construction process by pointing to workspace and replacing name pointers. */
    virtual void initializeOnAdd(bool expectToLosePointers = false);

    /** Complete copy construction process by updating pointer handles. */
    virtual void initializeOnClone(const HandleMap& oldNewHandleMap);

    virtual ~WorkspaceObject_Impl() = default;

    /// remove the object from the workspace
    /// return std::vector<IdfObject> containing any removed object(s)
    // TODO: replace with undo struct including connections
    virtual std::vector<IdfObject> remove();

    //@}
    /** @name Getters */
    //@{

    /** Returns pointer to this object's workspace. */
    Workspace_Impl* workspaceImpl() const;

    /** Returns this object's workspace. */
    Workspace workspace() const;

    /** Get the value of field index, if index < numFields(). Optionally, if returnDefault is
     *  passed in as true, getString will return the default value for non-existent
     *  (non-extensible) fields and fields with empty data, if a default exists. */
    virtual boost::optional<std::string> getString(unsigned index, bool returnDefault = false, bool returnUninitializedEmpty = false) const override;

    boost::optional<std::string> getField(unsigned index) const;

    /** Returns the object pointed to by the field at index, if it exists. */
    boost::optional<WorkspaceObject> getTarget(unsigned index) const;

    /** Returns the objects pointed to by this object. */
    std::vector<WorkspaceObject> targets() const;

    /** Returns the index of the field in this object that points to targetHandle, if possible. */
    std::vector<unsigned> getSourceIndices(const Handle& targetHandle) const;

    /** Returns the objects that point to this object. */
    std::vector<WorkspaceObject> sources() const;

    /** Returns the objects of type that point to this object. */
    std::vector<WorkspaceObject> getSources(IddObjectType type) const;

    /** Provided for Workspace_Impl to get easy access to targetData. */
    ReversePointerSet getReversePointers() const;

    //@}
    /** @name Setters */
    //@{

    /** Sets the name field if it exists, returning the actual name string set. Returns false
     *  otherwise. The return value and newName may differ (by an appended integer) if a
     *  conflict with newName was detected. Name conflicts will not be automatically avoided
     *  in IDF (text-only) mode. */
    virtual boost::optional<std::string> setName(const std::string& newName) override;
    virtual boost::optional<std::string> setName(const std::string& newName, bool checkValidity) override;

    /** Sets the name field to a new, unique name, if the name field exists and (is empty or
     *  overwrite == true). Returns false otherwise. Created name is returned. */
    virtual boost::optional<std::string> createName() override;
    virtual boost::optional<std::string> createName(bool overwrite) override;

    /** Sets the field at index to value, if possible. Returns false if the value cannot be set for any
     *  reason. (Perhaps index >= numFields(), or setting field index to value would invalidate the
     *   object.) */
    virtual bool setString(unsigned index, const std::string& value) override;
    virtual bool setString(unsigned index, const std::string& value, bool checkValidity) override;

    /** Sets the pointer at field index to targetHandle, if possible. The field must be of \object-list
     *  type, and targetHandle must be valid (null or in the containing Workspace, and if the strictness
     *  is greater than enums::None, must correspond to an object of a proper type). */
    virtual bool setPointer(unsigned index, const Handle& targetHandle);
    virtual bool setPointer(unsigned index, const Handle& targetHandle, bool checkValidity);

    /** Pushes a new field onto the object, if possible. Returns false if the operation cannot be completed
     *  for any reason. */
    virtual bool pushString() override;
    virtual bool pushString(bool checkValidity) override;
    virtual bool pushString(const std::string& value) override;
    virtual bool pushString(const std::string& value, bool checkValidity) override;

    /** Pushes a new pointer field onto the object, pointing to targetHandle, if possible. Returns
     *  false if operation cannot be completed for any reason. */
    bool pushPointer(const Handle& targetHandle);
    bool pushPointer(const Handle& targetHandle, bool checkValidity);

    /** Pops the final extensible group from the object, if possible. Returns the popped data if
     *  successful. Otherwise, the returned vector will be empty. */
    virtual std::vector<std::string> popExtensibleGroup() override;
    virtual std::vector<std::string> popExtensibleGroup(bool checkValidity) override;

    //@}
    /** @name Queries */
    //@{

    /** Returns true if object is connected to a workspace and initialized. */
    bool initialized() const;

    /** Returns the number of objects that point to this object. */
    unsigned numSources() const;

    /** Returns true if this object points to another object. To qualify, there must be at least one
     *  pointer field holding a non-null pointer. */
    bool isSource() const;

    bool isSource(unsigned index) const;

    /** Returns true if this object can point to another object. To qualify, there must be at least
     *  one \object-list field. */
    bool canBeSource() const;

    /** Returns true if field index can be a source field. If no refLists are specified, then
     *  this call is equivalent to iddObject().getField(index)->isObjectListField(). If a
     *  non-empty vector of refLists is specified, then true is only returned if there is a match
     *  between field index's object lists and the referencesLists argument. */
    bool canBeSource(unsigned index, const std::vector<std::string>& refLists = std::vector<std::string>()) const;

    /** Returns true if another object points to this object. */
    bool isTarget() const;

    /** Returns the reference lists of which this object is a member, if this object has a name. */
    std::vector<std::string> canBeTarget() const;

    /** Checks for equality of objectListFields(). Prerequisite: iddObject()s must be
     *  equal. Looks for object equality of targets if same workspace(), otherwise, looks for data
     *  field equality of targets. */
    bool objectListFieldsEqual(const WorkspaceObject& other) const;

    /** Returns true if for all objectListFields() that for both objects are non-empty,
     *  the values are equal (using the same criteria as objectListFieldsEqual).
     *  Prerequisite: iddObject()s must be equal. */
    bool objectListFieldsNonConflicting(const WorkspaceObject& other) const;

    //@}
    /** @name Serialization */
    //@{

    /** Returns equivalent IdfObject, naming targets if necessary. All data is cloned. */
    IdfObject_ImplPtr idfObjectImplPtr();

    /** Returns equivalent IdfObject, leaving unnamed target objects unnamed. All data is cloned. */
    IdfObject_ImplPtr idfObjectImplPtr() const;

    /** Returns equivalent IdfObject, naming targets if necessary. All data is cloned. */
    IdfObject idfObject();

    /** Returns equivalent IdfObject, leaving unnamed target objects unnamed. All data is cloned. */
    IdfObject idfObject() const;

    //@}
    /** @name Signal Helpers */
    //@{

    /** Emits signals after batch update and error checking is complete, clears the diffs */
    virtual void emitChangeSignals() override;

    //@}

    //@}
    /** @name Nano Signal */
    //@{

    /** Emitted when a pointer field is changed. */
    Nano::Signal<void(int, Handle, Handle)> onRelationshipChange;

    /** Emitted when this object is disconnected from the workspace.  Do not
     *  access any methods of this object as it is invalid. */
    Nano::Signal<void(const Handle&)> onRemoveFromWorkspace;
    // Nano::Signal<void(Handle&)> onRemoveFromWorkspaceRef;

    Nano::Signal<void(const openstudio::Handle&)> workspaceObjectRemovedSignal;

   protected:
    friend class Workspace_Impl;

    /** Denotes that this object has been initialized by Workspace_Impl. */
    void setInitialized();

    /** Disconnects this object from its workspace. Nullifies m_workspace and m_handle. */
    void disconnect();

    /** Mechanics only exposed to Workspace_Impl for use in object removal. */
    void nullifyPointer(unsigned index);

    void nullifyReversePointer(const Handle& sourceHandle, unsigned index);

    void setReversePointer(const Handle& sourceHandle, unsigned index);

    /** Called when restoring object because could not remove and retain validity. Double-checks
     *  that companion pointers are in place. May not be able to fix all if multiple objects are
     *  being restored. Does not throw or log because trusts Workspace to restore all relevant
     *  objects. */
    void restorePointers();

    // QUERY HELPERS

    virtual void populateValidityReport(ValidityReport& report, bool checkNames) const override;

    /** Returns true if the object is identifiable (within its collection) by IddObjectType and
     *  name. Also, if this object is in a reference list, its name must be unique within (single)
     *  overlapping references. */
    virtual bool uniquelyIdentifiableByName() const;

    /** Checks ObjectList fields, and calls IdfObject_Impl version. */
    virtual bool fieldDataIsCorrectType(unsigned index) const override;

    virtual bool fieldIsNonnullIfRequired(unsigned index) const override;

   private:
    bool m_initialized;
    Workspace_Impl* m_workspace;
    OptionalSourceData m_sourceData;
    OptionalTargetData m_targetData;

    // SETTER HELPERS

    /** Sets pointer at field index to targetHandle, and returns old target. */
    Handle setPointerImpl(unsigned index, const Handle& targetHandle);

    boost::optional<Handle> convertToTargetHandle(const std::string& name, const std::set<std::string>& referenceLists, bool checkValidity) const;

    void restoreOriginalNumFields(unsigned n);

    bool popField();

    // configure logging
    REGISTER_LOGGER("utilities.idf.WorkspaceObject");
  };

  /** \relates WorkspaceObject_Impl */
  using WorkspaceObject_ImplPtr = std::shared_ptr<WorkspaceObject_Impl>;

  /** \relates WorkspaceObject_Impl */
  using WorkspaceObject_ImplPtrVector = std::vector<WorkspaceObject_ImplPtr>;

}  // namespace detail

}  // namespace openstudio

#endif  // UTILITIES_IDF_WORKSPACEOBJECT_IMPL_HPP
