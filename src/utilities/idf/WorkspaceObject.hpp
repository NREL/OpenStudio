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

#ifndef UTILITIES_IDF_WORKSPACEOBJECT_HPP
#define UTILITIES_IDF_WORKSPACEOBJECT_HPP

#include "../UtilitiesAPI.hpp"
#include "IdfObject.hpp"

namespace openstudio {

class Workspace;
class WorkspaceExtensibleGroup;

// forward declarations
namespace detail {
  class IdfObject_Impl;
  class WorkspaceObject_Impl;
  class Workspace_Impl;
}  // namespace detail

/** WorkspaceObject holds and manipulates data objects in Energyplus IDF format in the context of
 *  a Workspace. Over and above IdfObject, WorkspaceObject maintains ObjectListType fields as
 *  pointers (possibly null) to other WorkspaceObjects in the same Workspace, and only commits
 *  changes that maintain the validity of its Workspace at the current StrictnessLevel (typically
 *  Draft, moving to Final right before simulation). */
class UTILITIES_API WorkspaceObject : public IdfObject
{
 public:
  /** @name Constructors and Destructors */
  //@{

  // TODO: virtual dtor, but no copy ctor / assignment nor move ones... The move ones will **not** be declared/defaulted
  virtual ~WorkspaceObject() = default;

  WorkspaceObject(const WorkspaceObject& other) = default;
  WorkspaceObject(WorkspaceObject&& other) = default;
  WorkspaceObject& operator=(const WorkspaceObject&) = default;
  WorkspaceObject& operator=(WorkspaceObject&&) = default;

  /** Removes the object from the Workspace. Returns an IdfObjectVector of all removed object(s). */
  // TODO: Replace return type with undo struct including connections.
  std::vector<openstudio::IdfObject> remove();

  //@}
  /** @name Getters */
  //@{

  /** Return the Workspace that contains this WorkspaceObject. */
  Workspace workspace() const;

  /** Returns the object pointed to by field index, if it exists. */
  boost::optional<WorkspaceObject> getTarget(unsigned index) const;

  /** Returns all objects pointed to by this object. */
  std::vector<WorkspaceObject> targets() const;

  /** Returns the index of the field in this object that points to targetHandle, if possible. */
  std::vector<unsigned> getSourceIndices(const Handle& targetHandle) const;

  /** Returns all objects that point to this object, filtering for duplicate objects. */
  std::vector<WorkspaceObject> sources() const;

  /** Returns all objects of type that point to this object, filtering for duplicate objects. */
  std::vector<WorkspaceObject> getSources(IddObjectType type) const;

  /** Like getString except for reference fields getString will return the
   *  name of the referenced object. This method, getField, will always return the string value
   *  of the field.
   */
  boost::optional<std::string> getField(unsigned index) const;

  //@}
  /** @name Setters */
  //@{

  /** Sets the pointer at field index to targetHandle, if possible. The field must be of object-list
   *  type, and targetHandle must be valid (null or in the containing Workspace, and if the strictness
   *  is greater than enums::None, must correspond to an object of a proper type). */
  bool setPointer(unsigned index, const Handle& targetHandle);

  //@}
  /** @name Queries */
  //@{

  bool operator<(const WorkspaceObject& right) const;

  bool initialized() const;

  /** Returns the number of objects that point to this object. */
  unsigned numSources() const;

  /** Returns true if this object points to another object. To qualify, there must be at least one
   *  pointer field holding a non-null pointer. */
  bool isSource() const;

  /** Returns true if this object can point to another object. To qualify, there must be at least
   *  one object-list field. */
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

  /** Returns an equivalent IdfObject, naming target objects if necessary. All data is cloned.
   *  If the objects have handle fields (as in the OpenStudio IDD), pointers to other objects
   *  are printed as handles; otherwise pointers are printed as names. */
  //DLM@20101010: don't like const overload, just make sure everything is named all the time
  IdfObject idfObject();

  /** Returns an equivalent IdfObject, leaving unnamed target objects unnamed. All data is cloned.
   *  If the objects have handle fields (as in the OpenStudio IDD), pointers to other objects
   *  are printed as handles; otherwise pointers are printed as names. */
  //DLM@20101010: don't like const overload, just make sure everything is named all the time
  IdfObject idfObject() const;

  //@}

 protected:
  using ImplType = detail::WorkspaceObject_Impl;

  friend class Workspace;
  friend class detail::Workspace_Impl;
  friend class IdfObject;
  friend class detail::IdfObject_Impl;
  friend class detail::WorkspaceObject_Impl;
  friend class WorkspaceExtensibleGroup;

  WorkspaceObject(std::shared_ptr<detail::WorkspaceObject_Impl> impl);

 private:
  std::vector<WorkspaceObject> objects(const std::vector<Handle>& handles, bool sorted) const;

  REGISTER_LOGGER("utilities.idf.WorkspaceObject");
};

/** \relates WorkspaceObject */
using OptionalWorkspaceObject = boost::optional<WorkspaceObject>;

/** \relates WorkspaceObject */
using WorkspaceObjectVector = std::vector<WorkspaceObject>;

/** \relates WorkspaceObject
 *  Outputs WorkspaceObject as (IDF) text. */
UTILITIES_API std::ostream& operator<<(std::ostream& os, const WorkspaceObject& workspaceObject);

}  // namespace openstudio

#endif  // UTILITIES_IDF_WORKSPACEOBJECT_HPP
