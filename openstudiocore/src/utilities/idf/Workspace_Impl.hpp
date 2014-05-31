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

#ifndef UTILITIES_IDF_WORKSPACE_IMPL_HPP
#define UTILITIES_IDF_WORKSPACE_IMPL_HPP

#include <utilities/UtilitiesAPI.hpp>

#include <utilities/idf/WorkspaceObject_Impl.hpp>
#include <utilities/idf/WorkspaceObjectOrder.hpp>
#include <utilities/idf/ValidityEnums.hpp>
#include <utilities/idf/ObjectPointer.hpp>

#include <utilities/idd/IddFileAndFactoryWrapper.hpp>

#include <utilities/core/Logger.hpp>

#include <QObject>

#include <string>
#include <ostream>
#include <vector>
#include <set>
#include <map>

namespace openstudio {

// forward declarations
class IdfFile;
class VersionString;
class URLSearchPath;

// private namespace
namespace detail {

  /** Implementation of Workspace. Maintains object handles and relationships. Locks down
   *  relationship fields in its IdfObjects if possible. */
  class UTILITIES_API Workspace_Impl : public QObject,
                                       public std::enable_shared_from_this<Workspace_Impl>
  {
    Q_OBJECT;
   public:

    /** @name Constructors and Destructors */
    //@{

    /** Default constructor. Provides access to entire IddFactory, and does not add any objects. */
    Workspace_Impl(StrictnessLevel level = StrictnessLevel::None,
                   IddFileType iddFileType = IddFileType::OpenStudio);

    /** Construct from IdfFile. Creates independent IdfObjects, and locks down fields that function
     *  as pointers (\object-list type). Inherits IddFile from idfFile. Aims for StrictnessLevel.
     *  Either sets StrictnessLevel lower if can't get to level, or removes all objects depending
     *  on boolean. */
    Workspace_Impl(const IdfFile& idfFile,
                   StrictnessLevel level = StrictnessLevel::None);

    /** Copy constructor makes unconnected copy of all data. Assigning new handles to the new objects
     *  is optional. */
    Workspace_Impl(const Workspace_Impl& other,
                   bool keepHandles=false);

    /** Subset copy constructor makes unconnected copy of all data, only keeping objects in handles. */
    Workspace_Impl(const Workspace_Impl& other,
                   const std::vector<Handle>& hs,
                   bool keepHandles=false,
                   StrictnessLevel level = StrictnessLevel::Draft);

    /** Create a deep copy (clone) of all data in this Workspace and return the result in a new
     *  Workspace object. If keepHandles, then new handles will not be assigned to the cloned objects.
     *  This feature should be used with care, as reuse of unique object identifiers could lead to
     *  changing data in the wrong Workspace. */
    virtual Workspace clone(bool keepHandles=false) const;

    /** Clone just the objects referenced by handles into a new Workspace. All non-object data is
     *  also cloned. If keepHandles, then new handles will not be assigned to the cloned objects.
     *  Virtual implementation, and similar usage to clone. */
    virtual Workspace cloneSubset(const std::vector<Handle>& handles,
                                  bool keepHandles = false,
                                  StrictnessLevel level = StrictnessLevel::Draft) const;

    /** Swaps underlying data between this workspace and other. */
    virtual void swap(Workspace& other);

    virtual ~Workspace_Impl() {}

    //@}
    /** @name Type Casting */
    //@{

    template<typename T>
    T getWorkspace() {
      T result(std::dynamic_pointer_cast<typename T::ImplType>(
                   std::const_pointer_cast<Workspace_Impl>(shared_from_this())));
      return result;
    }

    template<typename T>
    T getWorkspace() const {
      T result(std::dynamic_pointer_cast<typename T::ImplType>(
                   std::const_pointer_cast<Workspace_Impl>(shared_from_this())));
      return result;
    }

    Workspace workspace();

    Workspace workspace() const;

    //@}
    /** @name Getters */
    //@{

    /// Get the StrictnessLevel this Workspace is currently operating under
    StrictnessLevel strictnessLevel() const;

    /** Get the version of this model, as determined by the IDD. */
    VersionString version() const;

    /// get iddFile
    IddFile iddFile() const;

    /** Get the \link IddFile IddFile's\endlink IddFileType. */
    IddFileType iddFileType() const;

    /** Returns the IddObject if .isInIddFile(type). Otherwise return value evaluates to false. */
    boost::optional<IddObject> getIddObject(const IddObjectType& type) const;

    /** True if has an operable name field. */
    boost::optional<std::string> name(const Handle& handle) const;

    /** Get object from its handle. */
    boost::optional<WorkspaceObject> getObject(const Handle& handle) const;

    /** Get all objects in this workspace. The returned objects' data is shared with the workspace.
     *  If sorted, then the objects are returned in the preferred order. */
    std::vector<WorkspaceObject> objects(bool sorted=false) const;

    /** Returns all objects, including the versionObject. Protected in public class. */
    std::vector<WorkspaceObject> allObjects() const;

    /** Get all handles. If sorted, then the handles are returned in the preferred order. */
    std::vector<Handle> handles(bool sorted=false) const;

    /** Get all the objects in the current workspace that can have a URL entry, that is, they have a
     *  URL IddField in their iddObject(). */
    std::vector<WorkspaceObject> objectsWithURLFields() const;

    /** Get the version object in this Workspace, if it exists. */
    boost::optional<WorkspaceObject> versionObject() const;

    /** Get objects from their handles. */
    std::vector<WorkspaceObject> getObjects(const std::vector<Handle>& handles) const;

    /** Returns all objects named name (case insensitive). If exactMatch == false, will return
     *  all objects with name or name plus an integer suffix. */
    std::vector<WorkspaceObject> getObjectsByName(const std::string& name, bool exactMatch=true) const;

    /// get all idf objects by type (e.g. Zone)
    std::vector<WorkspaceObject> getObjectsByType(IddObjectType objectType) const;

    /// get all idf objects by full idd type
    std::vector<WorkspaceObject> getObjectsByType(const IddObject& objectType) const;

    /** Returns the first object found of type objectType and named name (case insensitive,
     *  exact match). */
    boost::optional<WorkspaceObject> getObjectByTypeAndName(IddObjectType objectType,
                                                            const std::string& name) const;

    /** Returns all objects named name or name plus an integer suffix (case insensitive). */
    std::vector<WorkspaceObject> getObjectsByTypeAndName(IddObjectType objectType,
                                                         const std::string& name) const;

    /// get all objects by reference name (e.g. ZoneNames)
    std::vector<WorkspaceObject> getObjectsByReference(const std::string& referenceName) const;

    /// get all objects by reference name (e.g. ZoneNames)
    std::vector<WorkspaceObject> getObjectsByReference(
        const std::vector<std::string>& referenceNames) const;

    /** Returns the first object found that is in at least one of the reference lists in
     *  referenceNames and named name (case insensitive, but exact match). Does not look for
     *  conflicts. */
    boost::optional<WorkspaceObject> getObjectByNameAndReference(
        std::string name,const std::vector<std::string>& referenceNames) const;

    /** Returns true if fast naming is enabled. */
    bool fastNaming() const;

    //@}
    /** @name Setters */
    //@{

    /** Set the strictness level. If moving to a more strict environment, may return false
     *  to indicate that the collection does not satisfy those requirements. */
    virtual bool setStrictnessLevel(StrictnessLevel level);

    // Helper function to start the process of adding an object to the workspace.
    virtual std::shared_ptr<WorkspaceObject_Impl> createObject(const IdfObject& object,
                                                                 bool keepHandle);

    // Helper function to start the process of adding a cloned object to the workspace.
    virtual std::shared_ptr<WorkspaceObject_Impl> createObject(
        const std::shared_ptr<WorkspaceObject_Impl>& originalObjectImplPtr,bool keepHandle);

    virtual std::vector<WorkspaceObject> addObjects(
        std::vector< std::shared_ptr<WorkspaceObject_Impl> >& objectImplPtrs,
        const std::vector<UHPointer>& pointersIntoWorkspace=UHPointerVector(),
        const std::vector<HUPointer>& pointersFromWorkspace=HUPointerVector(),
        bool driverMethod=true,
        bool expectToLosePointers=false);

    /** Adds objectImplPtrs to the Workspace. As clones, the pointer handles may be incorrect. This
     *  is fixed by applying oldNewHandleMap to the pointer data. If this is a wholeCollectionClone,
     *  then the map is applied to the directOrder (if it exists) as well, otherwise, the new
     *  objects' handles are pushed onto the directOrder. */
    virtual std::vector<WorkspaceObject> addClones(
        std::vector< std::shared_ptr<WorkspaceObject_Impl> >& objectImplPtrs,
        const HandleMap& oldNewHandleMap,
        bool collectionClone,
        const std::vector<UHPointer>& pointersIntoWorkspace=UHPointerVector(),
        const std::vector<HUPointer>& pointersFromWorkspace=HUPointerVector(),
        bool driverMethod=true);

    /** Add object to Workspace. */
    virtual boost::optional<WorkspaceObject> addObject(const IdfObject& idfObject);

    /** Equivalent to inserting a Workspace with only idfObject added to it. If successful
     *  (equivalent object found, or object successfully added), new object will be returned.
     *  Otherwise, return value will be false (boost::none). */
    virtual boost::optional<WorkspaceObject> insertObject(const IdfObject& idfObject);

    /** Add objects to Workspace, if possible. Use this function to add multiple objects at once
     *  to avoid losing pointer (name reference) information. If successful, returned vector is of
     *  the same size, and is in the same order, as idfObjects. Otherwise, return value will be
     *  .empty(). */
    virtual std::vector<WorkspaceObject> addObjects(const std::vector<IdfObject>& idfObjects);

    /** Insert idfObjects into Workspace, if possible. Looks for equivalent objects first, then
     *  adds if necessary. If successful, new and equivalent objects will be returned in same order
     *  as idfObjects. Otherwise, return value will be .empty(). Equivalence is determined by
     *  creating a working Workspace with idfObjects as its contents, then looking for equivalence
     *  between the resulting WorkspaceObjects and this Workspace's objects. */
    virtual std::vector<WorkspaceObject> insertObjects(const std::vector<IdfObject>& idfObjects);

    /** Adds objectToAdd and inserts objectsToInsert into Workspace, if possible. If successful,
     *  returned vector is of the same size, and is in the same order, as the concatenation of
     *  objectsToAdd and objectsToInsert. Otherwise, return value will be .empty(). */
    virtual std::vector<WorkspaceObject> addAndInsertObjects(const std::vector<IdfObject>& objectsToAdd,
                                                             const std::vector<IdfObject>& objectsToInsert);

    /** Add objects to this Workspace, if possible. All objects are assumed to be from the same
     *  workspace, possibly this one. All data is cloned. Upon successful completion, the returned
     *  vector is of the same size, and is in the same order, as objects. Pointers are handled as
     *  follows:
     *
     *  \li objects[i] -> objects[j] becomes result[i] -> result[j]
     *  \li objects[i] -> external object becomes NULL or result[i] -> external object depending on
     *      whether objects[0].workspace() == *this.
     *  \li external object -> objects[i] is not in any way duplicated in result */
    virtual std::vector<WorkspaceObject> addObjects(const std::vector<WorkspaceObject>& objects);

    /** Insert objects into this Workspace, if possible. All objects are assumed to be from the same
     *  workspace, possibly this one. Data is only cloned if no equivalent object is located in this
     *  Workspace. Equivalence is defined as dataFieldsEqual and managedObjectListsNonConflicting.
     *  (If objects[0].workspace() == *this, this method call is trivial and objects is returned
     *  directly.) Upon successful completion, the returned vector is of the same size, and is in the
     *  same order, as objects. Pointers are handled as follows:
     *
     *  \li objects[i] -> objects[j] becomes result[i] -> result[j]
     *  \li objects[i] -> external object becomes NULL or result[i] -> external object depending on
     *      whether objects[0].workspace() == *this.
     *  \li external object -> objects[i] only becomes external object -> result[i] if
     *      objects[0].workspace() == *this */
    virtual std::vector<WorkspaceObject> insertObjects(const std::vector<WorkspaceObject>& objects);

    /** Add objectsToAdd and insert objectToInsert into this Workspace. All objects from both vectors
     *  are assumed to be from the same workspace, possibly this one. All relationships between the
     *  objects in both vectors (within and between vectors) are retained. Returns the resulting
     *  objects in the order implied by concatenating objectsToAdd and objectsToInsert. A new object
     *  is always created for each object in objectsToAdd; equivalent objects (dataFieldsEqual and
     *  managedObjectListsNonConflicting) may be returned for objectsToInsert. */
    virtual std::vector<WorkspaceObject> addAndInsertObjects(
        const std::vector<WorkspaceObject>& objectsToAdd,
        const std::vector<WorkspaceObject>& objectsToInsert);

    /** Add objectsToAdd and insert objectsToInsert into this Workspace. This method differs from the
     *  basic addAndInsertObjects for WorkspaceObjects in that the objectsToInsert are grouped.
     *  Equivalent objects are evaluated as a whole for each group (sub-vector) of objectsToInsert.
     *  That is, if no equivalent is found for any one object in a given group, the whole group is
     *  added to the workspace. */
    virtual std::vector<WorkspaceObject> addAndInsertObjects(
        const std::vector<WorkspaceObject>& objectsToAdd,
        const std::vector< std::vector<WorkspaceObject> >& objectsToInsert);

    /** Swap currentObject and newObject. To proceed, the objects must be compatible, that is,
     *  all source fields pointing to currentObject must be reset-able to newObject, and (if
     *  keepTargets) newObject must be able to point to the same target objects as currentObject.
     *  If successful, the return value is true, and the data in currentObject and newObject will
     *  have been swapped. Otherwise, the return value evaluates to false. */
    virtual bool swap(WorkspaceObject& currentObject,IdfObject& newObject,bool keepTargets = false);

    /** Remove object from Workspace with the expectation that it will be destructed.
     *  This function removes only the object specified by handle it is not overridden
     *  to "do the right thing" for ModelObject.
     */
    virtual bool removeObject(const Handle& handle);

    /** Remove objects from Workspace with the expectation that they will be destructed.
     *  This function removes only the objects specified by handles it is not overridden
     *  to "do the right thing" for ModelObjects.
     */
    virtual bool removeObjects(const std::vector<Handle>& handles);

    /** Add forwarded references. Field index of sourceObject is an object list field that also
     *  defines references. Make sure targetHandle is listed under those reference lists. */
    void forwardReferences(const Handle& sourceHandle, unsigned index, const Handle& targetHandle);

    /** Remove forwarded references. Field index of sourceObject is an object list field that also
     *  defines references. That field did point to targetObject. If no other source places
     *  targetObject in those reference lists, remove the association. */
    void removeForwardedReferences(const Handle& sourceHandle,
                                   unsigned index,
                                   const WorkspaceObject& targetObject);

    /** Setting fast naming to true reduces the time taken to create names by using a UUID as the name.
     *   This UUID is not the same as the object's handle.
     */
    void setFastNaming(bool fastNaming);

    /** Resolve name conflicts within other, and between this workspace and other by renaming objects
     *  in other. */
    bool resolvePotentialNameConflicts(Workspace& other);

    //@}
    /** @name Object Order */
    //@{

    /** Access the IdfOrderObject used by the workspace to provide arbitrary order of objects. */
    WorkspaceObjectOrder order();

    /** Access the IdfOrderObject used by the workspace to provide arbitrary order of objects. */
    const WorkspaceObjectOrder order() const;

    /** Sort the objects. */
    std::vector<WorkspaceObject> sort(const std::vector<WorkspaceObject>& objects) const;

    /** Sort the handles. */
    std::vector<Handle> sort(const std::vector<Handle>& handles) const;

    //@}
    /** @name Queries */
    //@{

    /** Return the total number of objects in the workspace. */
    unsigned numObjects() const;

    /** Return the total number of objects (including version objects) in the workspace. */
    unsigned numAllObjects() const;

    /** Return the number of objects of IddObjectType type in the workspace. */
    unsigned numObjectsOfType(IddObjectType type) const;

    /** Return the number of objects by full IddObject type. */
    unsigned numObjectsOfType(const IddObject& objectType) const;

    /** True if handle corresponds to an object in this workspace. */
    bool isMember(const Handle& handle) const;

    /** True if an \\object-list field referencing the given names can point to this object. */
    bool canBeTarget(const Handle& handle, const std::set<std::string>& referenceListNames) const;

    /** True if the IddObject of type is in iddFile(). */
    bool isInIddFile(IddObjectType type) const;

    /** Returns the next name in the pattern 'name << " " << int'. If fillIn, will use unused integer
     *  values smaller than the largest one. */
    std::string nextName(const std::string& name, bool fillIn) const;

    /** Returns the next name in the pattern 'iddObject().name() << " " << int' for iddObjectType.
     *  If fillIn, will use unused integer values smaller than the largest one. */
    std::string nextName(const IddObjectType& iddObjectType, bool fillIn) const;

    /** Returns true if this Workspace is valid at strictnessLevel(). */
    bool isValid() const;

    /** Returns true if this Workspace is valid, that is, has no errors at or below level. */
    bool isValid(StrictnessLevel level) const;

    /** Returns a ValidityReport for this Workspace containing all errors at or below
     *  strictnessLevel(). */
    virtual ValidityReport validityReport() const;

    /** Returns a ValidityReport for this Workspace containing all errors at or below level. */
    virtual ValidityReport validityReport(StrictnessLevel level) const;

    /** Returns an IdfObject based on the Version IddObject appropriate for this Workspace. No
     *  public interface. Used in constructing Workspaces. */
    IdfObject versionObjectToAdd() const;

    //@}
    /** @name Serialization and File Management*/
    //@{

    /** Save Workspace to path. Will construct parent folder, but no further up the chain. Will
     *  only overwrite an existing file if overwrite==true. If no extension is provided will use
     *  .idf or modelFileExtension() depending on the underlying IddFileType. */
    virtual bool save(const openstudio::path& p, bool overwrite=false);

    /** Creates an IdfFile from the collection, naming objects if necessary. To print out IDF text,
     *  use this method, then IdfFile.print(ostream). */
    IdfFile toIdfFile();

    /// Locates and updates urls in the workspace
    std::vector<std::pair<QUrl, openstudio::path> > locateUrls(const std::vector<URLSearchPath> &t_paths, bool t_create_relative_paths,
     const openstudio::path &t_infile, const openstudio::path &t_locationForRemoteUrls = openstudio::path());

    //@}

   signals:

    /// range for progress to take
    void progressRange(int min, int max) const;

    /// report on progress when loading workspace, creating objects, etc
    void progressValue(int value) const;

    /// report caption describing what we are currently doing
    void progressCaption(const QString& caption) const;

    /** Emitted on any change to this Workspace and its contents. */
    void onChange() const;

    /** Send an object being deleted from the workspace. OS_ASSERT(!object.initialized())
     *  should pass, as should OS_ASSERT(object.handle().isNull()). */
    void removeWorkspaceObject(const WorkspaceObject& object, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle) const;

    // DLM: deprecate this version
    void removeWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl>, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle) const;

    /** Sends an object just added to the Workspace. */
    void addWorkspaceObject(const WorkspaceObject& object, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle) const;

    // DLM: deprecate this version
    void addWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl>, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle) const;

   public slots:

    void change();

   protected:

    // helper for non-virtual part of clone implementation
    void createAndAddClonedObjects(const std::shared_ptr<Workspace_Impl>& thisImpl,
                                   std::shared_ptr<Workspace_Impl> cloneImpl,
                                   bool keepHandles) const;

    // helper for non-virtual part of clone subset implementation
    void createAndAddSubsetClonedObjects(const std::shared_ptr<Workspace_Impl>& thisImpl,
                                         std::shared_ptr<Workspace_Impl> cloneImpl,
                                         const std::vector<Handle>& handles,
                                         bool keepHandles) const;

   private:

    // DATA

    StrictnessLevel m_strictnessLevel; // level of validity to be maintained by collection
    std::string m_header;                                // header for the IdfFile
    IddFileAndFactoryWrapper m_iddFileAndFactoryWrapper; // IDD file to be used for validity checking
    bool m_fastNaming;

    typedef std::map<Handle, std::shared_ptr<WorkspaceObject_Impl> > WorkspaceObjectMap;
    WorkspaceObjectMap m_workspaceObjectMap;

    // object for ordering objects in the collection.
    WorkspaceObjectOrder m_workspaceObjectOrder;

    // map of IddObjectType to set of objects identified by UUID
    typedef std::map<IddObjectType, WorkspaceObjectMap > IddObjectTypeMap;
    IddObjectTypeMap m_iddObjectTypeMap;

    // map of reference to set of objects identified by UUID
    typedef std::map<std::string, WorkspaceObjectMap> IdfReferencesMap; // , IstringCompare
    IdfReferencesMap m_idfReferencesMap;

    // data object for undos
    struct SavedWorkspaceObject {
      Handle                   handle;
      std::shared_ptr<WorkspaceObject_Impl>  objectImplPtr;
      OptionalUnsigned         orderIndex;
      SavedWorkspaceObject(const Handle& h, const std::shared_ptr<WorkspaceObject_Impl>& o) : handle(h), objectImplPtr(o) {}
    };
    typedef boost::optional<SavedWorkspaceObject> OptionalSavedWorkspaceObject;
    typedef std::vector<SavedWorkspaceObject> SavedWorkspaceObjectVector;

    // GETTERS

    // Change over from a HandleSet to a std::vector<Handle>.
    std::vector<Handle> handles(const std::set<Handle>& handles, bool sorted=false) const;

    /** Checks if objectName without any suffix integers matches supplied base name. */
    bool baseNamesMatch(const std::string& baseName, const std::string& objectName) const;

    /** Returns optional suffix integer from objectName. */
    boost::optional<int> getNameSuffix(const std::string& objectName) const;

    /** Returns objectName in with any suffix integers removed. */
    std::string getBaseName(const std::string& objectName) const;

    boost::optional<WorkspaceObject> getEquivalentObject(const IdfObject& other) const;

    // SETTERS

    // Replace m_iddFactoryWrapper if workspace remains valid.
    bool setIddFile(const IddFileAndFactoryWrapper& iddFileAndFileWrapper);

      // Helper function to start the process of adding an object to the workspace.
    bool nominallyAddObject(std::shared_ptr<WorkspaceObject_Impl>& ptr);

    void insertIntoObjectMap(const Handle& handle, const std::shared_ptr<WorkspaceObject_Impl>& object);

    void insertIntoIddObjectTypeMap(const std::shared_ptr<WorkspaceObject_Impl>& object);

    void insertIntoIdfReferencesMap(const std::shared_ptr<WorkspaceObject_Impl>& object);

    // note default parameter for toIgnore is empty vector
    bool resolvePotentialNameConflicts(Workspace& other,
                                       const std::vector<unsigned>& toIgnore);

    void mergeIdfObjectAfterPotentialNameConflictResolution(
        IdfObject& mergedObject,const IdfObject& originalObject) const;

    // Adds provided relationships for newly added objects. handles is conversion from unsigned index
    // to (newly created) handle in workspace.
    bool addProvidedRelationships(const std::vector<Handle>& handles,
                                  const std::vector<UHPointer>& pointersIntoWorkspace,
                                  const std::vector<HUPointer>& pointersFromWorkspace);

    std::vector<WorkspaceObject> simpleAddObjects(const std::vector<IdfObject>& objects);

    std::vector<WorkspaceObject> addAndInsertObjects(
      const std::vector<WorkspaceObject>& allObjects,
      const std::vector<unsigned>& foundObjectIndices,
      const std::vector<unsigned>& notFoundObjectIndices,
      std::vector<WorkspaceObject>& equivalentObjects);

    OptionalSavedWorkspaceObject savedWorkspaceObject(const Handle& handle);

    std::vector<WorkspaceObject> nominallyRemoveObject(const Handle& handle);

    std::vector<std::vector<WorkspaceObject> > nominallyRemoveObjects(const std::vector<Handle>& handles);

    void restoreObject(SavedWorkspaceObject& savedObject);

    void restoreObjects(SavedWorkspaceObjectVector& savedObjects);

    void registerRemovalOfObject(std::shared_ptr<WorkspaceObject_Impl> ptr,const std::vector<WorkspaceObject>& sources,const std::vector<Handle>& removedHandles);

    void registerRemovalOfObjects(std::vector<SavedWorkspaceObject>& savedObjects,const std::vector<std::vector<WorkspaceObject> >& sources,const std::vector<Handle>& removedHandles);

    void registerAdditionOfObject(const WorkspaceObject& object);

    // QUERIES

    /** Returns name with the next available integer suffix. */
    std::string constructNextName(const std::string& objectName,
                                  const std::vector<WorkspaceObject>& objectsInTheSeries,
                                  bool fillIn) const;

    std::vector< std::vector<WorkspaceObject> > nameConflicts(
        const std::vector<WorkspaceObject>& candidates) const;

    bool potentialNameConflict(std::string& currentName,const IddObject& iddObject) const;

    // configure logging
    REGISTER_LOGGER("utilities.idf.Workspace");
  };

  typedef std::shared_ptr<Workspace_Impl> Workspace_ImplPtr;

} // detail

} // openstudio

#endif // UTILITIES_IDF_WORKSPACE_IMPL_HPP
