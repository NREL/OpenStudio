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

#ifndef UTILITIES_IDF_WORKSPACE_HPP
#define UTILITIES_IDF_WORKSPACE_HPP

#include "../UtilitiesAPI.hpp"
#include "ValidityEnums.hpp"
#include "Handle.hpp"

#include "../core/Logger.hpp"
#include "../core/Path.hpp"

#include <string>
#include <ostream>
#include <vector>
#include <set>

namespace openstudio {

// forward declarations
class IddFile;
class IddFileType;
class IddObject;
class IddObjectType;
class IdfFile;
class IdfObject;
class WorkspaceObject;
class WorkspaceObjectOrder;
class URLSearchPath;
class ValidityReport;
class VersionString;

class Quantity;

class ProgressBar;

namespace detail {
  class Workspace_Impl;
  class WorkspaceObject_Impl;
}

/** Workspace holds a collection of interconnected \link WorkspaceObject WorkspaceObjects\endlink.
 *  Similar to IdfFile, Workspace represents data (typically a whole or partial building energy
 *  model) in EnergyPlus Input Data File (IDF) format, and each instance of Workspace is
 *  constructed from a specific IddFile (either explicitly or using the IddFactory plus an
 *  IddFileType.) Workspace is shared so that changes to a copy of a Workspace changes the
 *  original as well.
 *
 *  Workspace contains the following types of \link WorkspaceObject WorkspaceObjects\endlink:
 *    - unique objects, indexed by type, should be present 0-1 times
 *    - objects with no associated reference lists, indexed by type and first field value, present
 *      0-inf times
 *    - objects with one or more associated reference lists, indexed by type, name, and
 *      reference list name, present 0-inf times
 *  Workspaces based on EnergyPlus IDDs are stuck with the above ways of identifying referenced
 *  objects in serialized form. As of Version 0.7.4, OpenStudio IDD serializes a Universal Unique
 *  Identifier (UUID) for each object, and uses this identifier to make inter-object connections.
 *  With this change, it is possible for an IdfObject field to use the OpenStudio-specific
 *  \\object-list AllObjects (see OS:ComponentData for an example).
 *
 *  Although Workspace and IdfFile share many important characteristics, there is no direct
 *  inheritance relationship between the two since their implementations are fundamentally
 *  different. */
class UTILITIES_API Workspace {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Default constructor assumes StrictnessLevel::Draft and IddFileType::OpenStudio. Adds a
   *  versionObject with the version identifier set according to the version registered for
   *  iddFileType in the IddFactory. */
  Workspace();

  /** Construct IddFileType::OpenStudio Workspace at StrictnessLevel level. Adds a versionObject
   *  with the version identifier set according to the version registered for iddFileType in the
   *  IddFactory. If level == StrictnessLevel::Final, Workspace can be invalid
   *  post-construction. */
  Workspace(StrictnessLevel level);

  /** Default constructor. User can specify IddFileType and StrictnessLevel. Adds a versionObject
   *  with the version identifier set according to the version registered for iddFileType in the
   *  IddFactory. If level == StrictnessLevel::Final, Workspace can be invalid
   *  post-construction. */
  Workspace(StrictnessLevel level, IddFileType iddFileType);

  /** Construct from idfFile. The new Workspace inherits idfFile's IddFile. Each IdfObject in
   *  idfFile is used to construct a WorkspaceObject that is held by the new Workspace, and is
   *  equivalent to, but completely separate from, the IdfObject. (All object data is cloned.)
   *
   *  If the Workspace so constructed is not valid at the specified StrictnessLevel, all of the
   *  newly created objects are removed, and the constructor returns an empty Workspace with
   *  StrictnessLevel None. Problems may be diagnosed by calling idfFile.validityReport(level). */
  Workspace(const IdfFile& idfFile,
            StrictnessLevel level = StrictnessLevel::None);

  /** Copy constructor, shares data with other Workspace. */
  Workspace(const Workspace& other);

  /** Create a deep copy (clone) of all data in this Workspace and return the result in a new
   *  Workspace object. Virtual implementation preserves original Workspace type. Example usage
   *  for derived class:
   *
   *  \code
   *  model::Model model;
   *  Workspace clonedModelAsWorkspace = model.clone();
   *  model::Model clonedModel = clonedModelAsWorkspace.cast<model::Model>();
   *  \endcode
   *
   *  If keepHandles, then new handles will not be assigned to the cloned objects. This feature
   *  should be used with care, as reuse of unique object identifiers could lead to changing data
   *  in the wrong Workspace. */
  Workspace clone(bool keepHandles=false) const;

  /** Clone just the objects referenced by handles into a new Workspace. All non-object data is
   *  also cloned. If keepHandles, then new handles will not be assigned to the cloned objects.
   *  Virtual implementation, and similar usage to clone. */
  Workspace cloneSubset(const std::vector<Handle>& handles,
                        bool keepHandles = false,
                        StrictnessLevel level = StrictnessLevel::Draft) const;

  /** Swaps underlying data between this workspace and other. Throws if other and this
   *  are not of the same type (must both be plain Workspaces, model::Models, or 
   *  model::Components). */
  void swap(Workspace& other);

  virtual ~Workspace() {}

  //@}
  /** @name Getters */
  //@{

  /// Returns the strictness level under which this Workspace is currently operating.
  StrictnessLevel strictnessLevel() const;

  /** Returns the version of this model, as determined by the IddFile header. */
  VersionString version() const;

  /** Returns the underlying IddFile (the schema for this Workspace). */
  IddFile iddFile() const;

  /** Returns the underlying IddFile's IddFileType. */
  IddFileType iddFileType() const;

  /** Returns the IddObject corresponding to type if it is available from the underlying IddFile.
   *  Otherwise the return value evaluates to false. */
  boost::optional<IddObject> getIddObject(const IddObjectType& type) const;

  /** Returns the object name associated with handle. */
  boost::optional<std::string> name(const Handle& handle) const;

  /** Get an object from its handle. */
  boost::optional<WorkspaceObject> getObject(Handle handle) const;

  /** Returns all the objects in this Workspace, excluding any version objects. If sorted, the
   *  objects are returned in the preferred order. */
  std::vector<WorkspaceObject> objects(bool sorted=false) const;

  /** Returns the handles of all the objects in this Workspace, excluding those of any version
   *  objects. If sorted, the handles are returned in the preferred order. */
  std::vector<Handle> handles(bool sorted=false) const;

  /** Get all the objects in the current workspace that can have a URL entry, that is, they have a
   *  URL IddField in their iddObject(). */
  std::vector<WorkspaceObject> objectsWithURLFields() const;

  /** Return the version object in this Workspace, if it exists and is unique. */
  boost::optional<WorkspaceObject> versionObject() const;

  /** Get objects from their handles. */
  std::vector<WorkspaceObject> getObjects(const std::vector<Handle>& handles) const;

  /** Returns all objects named name (case insensitive). If exactMatch == false, will return
   *  all objects with name or name plus an integer suffix. */
  std::vector<WorkspaceObject> getObjectsByName(const std::string& name, bool exactMatch=true) const;

  /** Returns all objects of type objectType (e.g. IddObjectType::Zone,
   *  IddObjectType("OS:Construction")). */
  std::vector<WorkspaceObject> getObjectsByType(IddObjectType objectType) const;

  /** Returns all objects with .iddObject() == objectType. */
  std::vector<WorkspaceObject> getObjectsByType(const IddObject& objectType) const;

  /** Returns the first object found of type objectType and named name (case insensitive,
   *  exact match). */
  boost::optional<WorkspaceObject> getObjectByTypeAndName(IddObjectType objectType,
                                                          const std::string& name) const;

  /** Returns all objects named name or name plus an integer suffix (case insensitive). */
  std::vector<WorkspaceObject> getObjectsByTypeAndName(IddObjectType objectType,
                                                       const std::string& name) const;

  /** Returns all objects in the reference list referenceName */
  std::vector<WorkspaceObject> getObjectsByReference(const std::string& referenceName) const;

  /** Returns all objects in at least one of the reference lists in referenceNames. */
  std::vector<WorkspaceObject> getObjectsByReference(
      const std::vector<std::string>& referenceNames) const;

  /** Returns the first object found that is in at least one of the reference lists in
   *  referenceNames and named name (case insensitive, but exact match). Does not look for
   *  conflicts. */
  boost::optional<WorkspaceObject> getObjectByNameAndReference(
      std::string name,const std::vector<std::string>& referenceNames) const;

  /** Returns true if fast naming is enabled. Fast naming creates UUID-based names for new
   *  objects and does not do any name conflict checking. */
  bool fastNaming() const;

  //@}
  /** @name Setters */
  //@{

  /** Set the StrictnessLevel to level. If level > strictnessLevel(), this method may
   *  leave the strictness unchanged and return false to indicate that the collection does not
   *  satisfy those requirements. To diagnose any issues, print the validityReport(level). */
  bool setStrictnessLevel(StrictnessLevel level);

  /** Add a clone of idfObject to Workspace. May rename the new object to avoid name
   *  conflicts with exiting objects. If IdfObject has handle it will be preserved.*/
  boost::optional<WorkspaceObject> addObject(const IdfObject& idfObject);

  /** Insert idfObject into this Workspace. Looks for an equivalent object first, only adding a
   *  clone if necessary. Implementation is equivalent to inserting a Workspace with only
   *  idfObject added to it. If successful (equivalent object found, or object successfully
   *  added), the new object is returned. Otherwise, the return value evaluates to false.
   *  If IdfObjects has handle it will be preserved.*/
  boost::optional<WorkspaceObject> insertObject(const IdfObject& idfObject);

  /** Add clones of idfObjects to Workspace, if possible. Use this function to add multiple
   *  objects at once to avoid losing pointer (name/UUID reference) information. If successful,
   *  returned vector is of the same size, and is in the same order, as idfObjects. Otherwise,
   *  return value will be .empty(). If IdfObjects have handles they will be preserved.*/
  std::vector<WorkspaceObject> addObjects(const std::vector<IdfObject>& idfObjects);

  /** Insert idfObjects into Workspace, if possible. Looks for equivalent objects first, then
   *  adds if necessary. If successful, new and equivalent objects will be returned in same order
   *  as idfObjects. Otherwise, return value will be .empty(). Equivalence is determined by
   *  creating a working Workspace with idfObjects as its contents, then looking for equivalence
   *  between the resulting WorkspaceObjects and this Workspace's objects.
   *  If IdfObjects have handles they will be preserved.*/
  std::vector<WorkspaceObject> insertObjects(const std::vector<IdfObject>& idfObjects);

  /** Adds objectToAdd and inserts objectsToInsert into Workspace, if possible. If successful,
   *  returned vector is of the same size, and is in the same order, as the concatenation of
   *  objectsToAdd and objectsToInsert. Otherwise, return value will be .empty().
   *  If IdfObjects have handles they will be preserved.*/
  std::vector<WorkspaceObject> addAndInsertObjects(const std::vector<IdfObject>& objectsToAdd,
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
  std::vector<WorkspaceObject> addObjects(const std::vector<WorkspaceObject>& objects);

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
  std::vector<WorkspaceObject> insertObjects(const std::vector<WorkspaceObject>& objects);

  /** Add objectsToAdd and insert objectToInsert into this Workspace. All objects from both vectors
   *  are assumed to be from the same workspace, possibly this one. All relationships between the
   *  objects in both vectors (within and between vectors) are retained. Returns the resulting
   *  objects in the order implied by concatenating objectsToAdd and objectsToInsert. A new object
   *  is always created for each object in objectsToAdd; equivalent objects (dataFieldsEqual and
   *  managedObjectListsNonConflicting) may be returned for objectsToInsert. */
  std::vector<WorkspaceObject> addAndInsertObjects(
      const std::vector<WorkspaceObject>& objectsToAdd,
      const std::vector<WorkspaceObject>& objectsToInsert);

  /** Add objectsToAdd and insert objectsToInsert into this Workspace. This method differs from the
   *  basic addAndInsertObjects for WorkspaceObjects in that the objectsToInsert are grouped.
   *  Equivalent objects are evaluated as a whole for each group (sub-vector) of objectsToInsert.
   *  That is, if no equivalent is found for any one object in a given group, the whole group is
   *  added to the workspace. */
  std::vector<WorkspaceObject> addAndInsertObjects(
      const std::vector<WorkspaceObject>& objectsToAdd,
      const std::vector< std::vector<WorkspaceObject> >& objectsToInsert);

  /** Swap currentObject and newObject. To proceed, the objects must be compatible, that is,
   *  all source fields pointing to currentObject must be reset-able to newObject, and
   *  (if keepTargets), newObject must be able to point to the same target objects as
   *  currentObject. If successful, the return value is true, and the data in currentObject and
   *  newObject will have been swapped. Otherwise, the return value evaluates to false. */
  bool swap(WorkspaceObject& currentObject, IdfObject& newObject, bool keepTargets = false);

  /** Remove object from Workspace with the expectation that it will be destructed.
   *  This function removes only the object specified by handle it is not overridden
   *  to "do the right thing" for ModelObject.
   */
  bool removeObject(const Handle& handle);

  /** Remove objects from Workspace with the expectation that they will be destructed.
   *  This function removes only the objects specified by handles it is not overridden
   *  to "do the right thing" for ModelObjects.
   */
  bool removeObjects(const std::vector<Handle>& handles);

  /** Setting fast naming to true reduces the time taken to create and verify the uniqueness
   *  of names by using a UUID as the name. The name UUID is not the same as the object's
   *  handle. */
  void setFastNaming(bool fastNaming);

  //@}
  /** @name Object Order */
  //@{

  /** Access the WorkspaceOrderObject used by the workspace to provide user-specified
   *  ordering of objects. */
  WorkspaceObjectOrder order();

  /** Access the WorkspaceOrderObject used by the workspace to provide user-specified
   *  ordering of objects. */
  const WorkspaceObjectOrder order() const;

  /** Sort objects using order(). */
  std::vector<WorkspaceObject> sort(const std::vector<WorkspaceObject>& objects) const;

  /** Sort handles using order(). */
  std::vector<Handle> sort(const std::vector<Handle>& handles) const;

  //@}
  /** @name Queries */
  //@{

  /** Return the total number of objects in the workspace, ignoring version objects. */
  unsigned numObjects() const;

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

  /** Returns a ValidityReport for this IdfFile containing all errors at or below
   *  strictnessLevel(). */
  ValidityReport validityReport() const;

  /** Returns a ValidityReport for this Workspace containing all errors at or below level. */
  ValidityReport validityReport(StrictnessLevel level) const;

  bool operator==(const Workspace& other) const;

  bool operator!=(const Workspace& other) const;

  //@}
  /** @name Serialization and File Management */
  //@{

  /** Save this Workspace to path p. Will construct the parent folder if necessary and if its
   *  parent folder already exists. Will only overwrite an existing file if overwrite==true. If no
   *  extension is provided will use modelFileExtension() for files using IddFileType::OpenStudio,
   *  and 'idf' otherwise. Returns true if the save operation is successful; false otherwise. */
  bool save(const openstudio::path& p, bool overwrite=false);

  /** Load a Workspace from path using the IddFactory, and choosing iddFileType based on file
   *  extension, if possible. (IddFileType::OpenStudio if extension is modelFileExtension() or
   *  componentFileExtension(), IddFileType::EnergyPlus otherwise.) */
  static boost::optional<Workspace> load(const openstudio::path& p);

  /** Load a Workspace from path using the IddFactory and iddFileType. */
  static boost::optional<Workspace> load(const openstudio::path& p,
                                         const IddFileType& iddFileType);

  /** Load a Workspace from path using iddFile. */
  static boost::optional<Workspace> load(const openstudio::path& p,
                                         const IddFile& iddFile);

  /** Returns an IdfFile equivalent to this Workspace. If the objects have handle fields (as in the
   *  OpenStudio IDD), pointers between objects are serialized as handles, otherwise they are
   *  serialized as names. */
  IdfFile toIdfFile() const;

  /// Find and update all relative (and remote) URLs to well defined locations based on the search paths supplied
  /// \param[in] t_paths Paths to search for relative urls
  /// \param[in] t_create_relative_paths If true, update paths to relative locations on the local filesystem, else, make internal urls
  ///            absolute
  /// \param[in] t_infile Path of this workspace object. Used during the calculation/searching of relative search paths
  /// \returns Vector of source/target pairs. The user will be responsible for ensuring that the results are copied into place
  std::vector<std::pair<QUrl, openstudio::path> > locateUrls(const std::vector<URLSearchPath> &t_paths, bool t_create_relative_paths,
      const openstudio::path &t_infile);

  //@}
  /** @name GUI Helpers */
  //@{

  // connect a progress bar
  bool connectProgressBar(const openstudio::ProgressBar& progressBar) const;

  // disconnect a progress bar
  bool disconnectProgressBar(const openstudio::ProgressBar &progressBar) const;

  //@}
  /** @name Type Casing */
  //@{

  /** Cast to type T. Throws std::bad_cast() if this is not a T. */
  template <typename T>
  T cast() const {
    std::shared_ptr<typename T::ImplType> impl = this->getImpl<typename T::ImplType>();
    if (!impl){
      throw(std::bad_cast());
    }
    return T(impl);
  }

  /** Cast to boost::optional<T>. Returns boost::none on failure of cast. */
  template <typename T>
  boost::optional<T> optionalCast() const {
    boost::optional<T> result;
    std::shared_ptr<typename T::ImplType> impl = this->getImpl<typename T::ImplType>();
    if (impl) {
      result = T(impl);
    }
    return result;
  }

  // get the impl
  template<typename T>
  std::shared_ptr<T> getImpl() const {
    return std::dynamic_pointer_cast<T>(m_impl);
  }

  //@}
 protected:

  typedef detail::Workspace_Impl ImplType;

  friend class WorkspaceObject;
  friend class detail::WorkspaceObject_Impl;
  friend class detail::Workspace_Impl;

  /** Protected constructor from impl. */
  Workspace(std::shared_ptr<detail::Workspace_Impl> impl);

  /** Returns all objects, including the versionObject. Protected in public class. */
  std::vector<WorkspaceObject> allObjects() const;

  virtual void addVersionObject();

 private:

  // configure logging
  REGISTER_LOGGER("utilities.idf.Workspace");

  // pointer to implementation
  std::shared_ptr<detail::Workspace_Impl> m_impl;
};

/** \relates Workspace */
typedef boost::optional<Workspace> OptionalWorkspace;

/** \relates Workspace */
typedef std::vector<Workspace> WorkspaceVector;

/** \relates Workspace */
UTILITIES_API std::ostream& operator<<(std::ostream& os, const Workspace& workspace);

} // openstudio

#endif //UTILITIES_IDF_WORKSPACE_HPP
