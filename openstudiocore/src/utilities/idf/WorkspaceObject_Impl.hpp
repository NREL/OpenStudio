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

#ifndef UTILITIES_IDF_WORKSPACEOBJECT_IMPL_HPP
#define UTILITIES_IDF_WORKSPACEOBJECT_IMPL_HPP

#include <utilities/UtilitiesAPI.hpp>

#include <utilities/idf/IdfObject_Impl.hpp>
#include <utilities/idf/ObjectPointer.hpp>

#include <QObject>

namespace openstudio {

// forward declarations
class WorkspaceObject;
class Workspace;

namespace detail {

  class Workspace_Impl; // forward declaration

  struct UTILITIES_API ForwardPointer {
    unsigned fieldIndex;
    Handle   targetHandle;

    /// \todo Default constructor needed to iterate over Source Map, but setting fieldIndex to 0
    /// seems sub-optimal.
    ForwardPointer() : fieldIndex(0) {}
    ForwardPointer(unsigned i,const Handle& h) : fieldIndex(i), targetHandle(h) {}
  };
  typedef std::set<ForwardPointer,FieldIndexLess<ForwardPointer> > ForwardPointerSet;

  struct UTILITIES_API SourceData {
    typedef ForwardPointer    pointer_type;
    typedef ForwardPointerSet pointer_set;

    pointer_set     pointers;

    SourceData() {}
  };
  typedef boost::optional<SourceData> OptionalSourceData;

  struct UTILITIES_API ReversePointer {
    Handle   sourceHandle;
    unsigned fieldIndex;

    ReversePointer() : fieldIndex(0) {}
    ReversePointer(const Handle& h, unsigned i) : sourceHandle(h), fieldIndex(i) {}
  };
  struct UTILITIES_API ReversePointerLess {
    bool operator()(const ReversePointer& left, const ReversePointer& right) const {
      if (left.sourceHandle == right.sourceHandle) {
        return (left.fieldIndex < right.fieldIndex);
      }
      else {
        return (left.sourceHandle < right.sourceHandle);
      }
    }
  };
  typedef std::set<ReversePointer,ReversePointerLess > ReversePointerSet;

  struct UTILITIES_API TargetData {
    typedef ReversePointer    pointer_type;
    typedef ReversePointerSet pointer_set;

    pointer_set reversePointers;
  };
  typedef boost::optional<TargetData> OptionalTargetData;

  template<class T>
  typename T::pointer_set::iterator getIteratorAtFieldIndex(
                                                            typename T::pointer_set& pointerSet,
                                                            unsigned fieldIndex)
  {
    return std::find_if(pointerSet.begin(),pointerSet.end(),
                        std::bind(fieldIndexEqualTo<typename T::pointer_type>,std::placeholders::_1,fieldIndex));
  }

  template<class T>
  typename T::pointer_set::const_iterator getConstIteratorAtFieldIndex(
                                                                       const typename T::pointer_set& pointerSet,
                                                                       unsigned fieldIndex)
  {
    return std::find_if(pointerSet.begin(),pointerSet.end(),
                        std::bind(fieldIndexEqualTo<typename T::pointer_type>,std::placeholders::_1,fieldIndex));
  }

  class UTILITIES_API WorkspaceObject_Impl : public IdfObject_Impl {
    Q_OBJECT;
   public:

    /** @name Constructors */
    //@{

    /** Construct from IdfObject_Impl (via idfObject). */
    WorkspaceObject_Impl(const IdfObject& idfObject, Workspace_Impl* workspace, bool keepHandle=false);

    /** Copy constructor. Assigns new handle, keeps same workspace pointer. Workspace should know
     *  whether copy is to live in original workspace, or in cloned workspace. If the latter,
     *  Workspace will need to update the workspace pointer, and any source and target pointer
     *  handles. */
    WorkspaceObject_Impl(const WorkspaceObject_Impl& other, Workspace_Impl* workspace, bool keepHandle=false);

    /** Complete construction process by pointing to workspace and replacing name pointers. */
    virtual void initializeOnAdd(bool expectToLosePointers = false);

    /** Complete copy construction process by updating pointer handles. */
    virtual void initializeOnClone(const HandleMap& oldNewHandleMap);

    virtual ~WorkspaceObject_Impl();

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
    virtual boost::optional<std::string> getString(unsigned index, bool returnDefault=false,bool returnUninitializedEmpty=false) const;

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
    virtual boost::optional<std::string> setName(const std::string& newName);
    virtual boost::optional<std::string> setName(const std::string& newName, bool checkValidity);

    /** Sets the name field to a new, unique name, if the name field exists and (is empty or
     *  overwrite == true). Returns false otherwise. Created name is returned. */
    virtual boost::optional<std::string> createName();
    virtual boost::optional<std::string> createName(bool overwrite);

    /** Sets the field at index to value, if possible. Returns false if the value cannot be set for any
     *  reason. (Perhaps index >= numFields(), or setting field index to value would invalidate the
     *   object.) */
    virtual bool setString(unsigned index, const std::string& value);
    virtual bool setString(unsigned index, const std::string& value, bool checkValidity);

    /** Sets the pointer at field index to targetHandle, if possible. The field must be of \object-list
     *  type, and targetHandle must be valid (null or in the containing Workspace, and if the strictness
     *  is greater than enums::None, must correspond to an object of a proper type). */
    virtual bool setPointer(unsigned index, const Handle& targetHandle);
    virtual bool setPointer(unsigned index, const Handle& targetHandle, bool checkValidity);

    /** Pushes a new field onto the object, if possible. Returns false if the operation cannot be completed
     *  for any reason. */
    virtual bool pushString();
    virtual bool pushString(bool checkValidity);
    virtual bool pushString(const std::string& value);
    virtual bool pushString(const std::string& value, bool checkValidity);

    /** Pushes a new pointer field onto the object, pointing to targetHandle, if possible. Returns
     *  false if operation cannot be completed for any reason. */
    bool pushPointer(const Handle& targetHandle);
    bool pushPointer(const Handle& targetHandle, bool checkValidity);

    /** Pops the final extensible group from the object, if possible. Returns the popped data if
     *  successful. Otherwise, the returned vector will be empty. */
    virtual std::vector<std::string> popExtensibleGroup();
    virtual std::vector<std::string> popExtensibleGroup(bool checkValidity);

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
    bool canBeSource(unsigned index,
                     const std::vector<std::string>& refLists=std::vector<std::string>()) const;

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
    virtual void emitChangeSignals();

    //@}

   signals:

    /** Emitted when a pointer field is changed. */
    void onRelationshipChange(int index, Handle newHandle, Handle oldHandle) const;

    /** Emitted when this object is disconnected from the workspace.  Do not
     *  access any methods of this object as it is invalid. */
    void onRemoveFromWorkspace(Handle handle) const;

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

    virtual void populateValidityReport(ValidityReport& report,bool checkNames) const;

    /** Returns true if the object is identifiable (within its collection) by IddObjectType and
     *  name. Also, if this object is in a reference list, its name must be unique within (single)
     *  overlapping references. */
    virtual bool uniquelyIdentifiableByName() const;

    /** Checks ObjectList fields, and calls IdfObject_Impl version. */
    virtual bool fieldDataIsCorrectType(unsigned index) const;

    virtual bool fieldIsNonnullIfRequired(unsigned index) const;

   private:

    bool                m_initialized;
    Workspace_Impl*     m_workspace;
    OptionalSourceData  m_sourceData;
    OptionalTargetData  m_targetData;

    // SETTER HELPERS

    /** Sets pointer at field index to targetHandle, and returns old target. */
    Handle setPointerImpl(unsigned index, const Handle& targetHandle);

    boost::optional<Handle> convertToTargetHandle(const std::string& name,
                                                     const std::set<std::string>& referenceLists,
                                                     bool checkValidity) const;

    void restoreOriginalNumFields(unsigned n);

    bool popField();

    // configure logging
    REGISTER_LOGGER("utilities.idf.WorkspaceObject");
  };

  /** \relates WorkspaceObject_Impl */
  typedef std::shared_ptr<WorkspaceObject_Impl> WorkspaceObject_ImplPtr;

  /** \relates WorkspaceObject_Impl */
  typedef std::vector<WorkspaceObject_ImplPtr> WorkspaceObject_ImplPtrVector;

} // detail

} // openstudio

Q_DECLARE_METATYPE(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl>)

#endif // UTILITIES_IDF_WORKSPACEOBJECT_IMPL_HPP
