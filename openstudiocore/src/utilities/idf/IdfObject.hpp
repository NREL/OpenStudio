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

#ifndef UTILITIES_IDF_IDFOBJECT_HPP
#define UTILITIES_IDF_IDFOBJECT_HPP

#include "../UtilitiesAPI.hpp"

#include "Handle.hpp"

#include "../core/Logger.hpp"

#include <boost/optional.hpp>

#include <string>
#include <ostream>
#include <vector>
#include <algorithm>

namespace openstudio{
 
// forward declarations
class IddObject;
class IddObjectType;
class IdfExtensibleGroup;
class ValidityReport;
class DataError;
class StrictnessLevel;
class Quantity;
class OSOptionalQuantity;

namespace detail{
  class IdfObject_Impl;
  class WorkspaceObject_Impl;
  class Workspace_Impl;
}

/** IdfObject holds and manipulates data in the EnergyPlus Idd/Idf format. An IdfObject
 *  consists of an IddObject, which defines the IdfObject's data, some number of string fields
 *  (that must conform to the IddObject), and user comments. In text, an IdfObject follows the
 *  syntax of (with method calls replaced with their returned strings):
 *
 *  \code
 *  IdfObject.comment()
 *  IddObject.name(),
 *    IdfObject.getString(0), IdfObject.fieldComment(0)
 *    IdfObject.getString(1), IdfObject.fieldComment(1)
 *    ...
 *    IdfObject.getString(IdfObject.numFields()-1); IdfObject.fieldComment(IdfObject.numFields()-1)
 *  \endcode
 *
 *  IdfObject is shared, that is, on copy construction the original IdfObject and the new one
 *  share their data. If you want an unconnected IdfObject initialized with the same data, use
 *  .clone().
 *
 *  All fields are stored internally as text. Conversions to numeric types may not succeed.
 *
 *  Field indexing follows the C/C++ convention: 0, 1, ...
 *
 *  On its own, IdfObject maintains validity at enums::Strictness level None.
 */
class UTILITIES_API IdfObject {
 public:

  /** @name Constructors */
  //@{

  /** Constructor from type. Equivalent to IdfObject(IddFactory::instance.iddObject(type)),
   *  unless type == IddObjectType::UserCustom, in which case, the constructed object is equivalent
   *  to IdfObject(IddObjectType::Catchall). If fastName is true a UUID is assigned as the name, 
   *  otherwise no name is assigned.*/
  explicit IdfObject(IddObjectType type, bool fastName=false);

  /** Constructor from iddObject. If fastName is true a UUID is assigned as the name, 
   *  otherwise no name is assigned.*/
  explicit IdfObject(const IddObject&, bool fastName=false);
  
  /** Copy constructor. Shares data with other, so changes made by either copy affects the data of
   *  both. */
  IdfObject(const IdfObject& other);

  virtual ~IdfObject() {}

  /** Creates a deep copy of this object. This object and the newly created object do not share
   *  data, and the new object is always unlocked. */
  IdfObject clone(bool keepHandle=false) const;
 
  //@}
  /** @name Getters */
  //@{

  /** Returns this object's handle, which is only valid in the containing Workspace. */
  Handle handle() const;

  /** Get this object's IddObject. */
  IddObject iddObject() const;

  /** Returns the comment block associated with the object. */
  std::string comment() const;

  /** Returns the comment associated with field index, if one exists. Optionally, if returnDefault
   *  is passed in as true, and no field comment exists for index, fieldComment will return a 
   *  comment-ized version of the IddField name, following a commonly used Idf convention. */
  boost::optional<std::string> fieldComment(unsigned index, bool returnDefault=false) const;

  /** Get the name of the object, if it exists. Optionally, if returnDefault is passed in as true, 
   *  name will return the default name, if it exists. */
  boost::optional<std::string> name(bool returnDefault=false) const;

  /** Get a brief description of this object. Always includes iddObject().type(), and includes
   *  name() if available and not empty. */
  std::string briefDescription() const;

  /** Returns true if the field is empty. */
  bool isEmpty(unsigned index) const;

  /** Get the value of field index, if index < numFields(). Optionally, if returnDefault is 
   *  passed in as true, getString will return the default value for non-existent 
   *  (non-extensible) fields and fields with empty data, if a default exists. 
   *  If the default does not exist or is not requested and the field is empty 
   *  then returnUninitializedEmpty is used to determine if an uninitialized result is returned.
   */
  boost::optional<std::string> getString(unsigned index, bool returnDefault=false, bool returnUninitializedEmpty=false ) const;

  /** Get the value of the field at index, converted to double, if possible. Returns an
   *  uninitialized object if the conversion is unsuccessful for any reason. Logs a warning
   *  if the conversion fails, the field is RealType, and the field is not equal to
   *  'autocalculate', 'autosize', or the empty string.
   *
   *  Optionally, if returnDefault is passed in as true, getDouble will return the default
   *  value for non-existent (non-extensible) fields and fields with empty data, if a
   *  real-valued default exists. */
  boost::optional<double> getDouble(unsigned index, bool returnDefault=false) const;

  /** Returns the Quantity at index, if possible. Uses markup in IDD to determine the units. If
   *  returnIP == true, uses the IP unit mark-up in the IDD to perform a conversion. */
  OSOptionalQuantity getQuantity (unsigned index,
                                  bool returnDefault=false,
                                  bool returnIP=false) const;

  /** Get the value of the field at index, converted to unsigned, if possible. Returns an
   *  uninitialized object if the conversion is unsuccessful for any reason. Logs a warning
   *  if the conversion fails, the field is IntegerType, there is a minBound >= 0, and the
   *  field is not equal to 'autocalculate', 'autosize', or the empty string.
   *
   *  Optionally, if returnDefault is passed in as true, getUnsigned will return the default
   *  value for non-existent (non-extensible) fields and fields with empty data, if an
   *  unsigned default exists. */
  boost::optional<unsigned> getUnsigned(unsigned index, bool returnDefault=false) const;

  /** Get value of the field at index, converted to int, if possible. Returns an
   *  uninitialized object if the conversion is unsuccessful for any reason. Logs a warning
   *  if the conversion fails, the field is IntegerType, and the field is not equal to
   *  'autocalculate', 'autosize', or the empty string.
   *
   *  Optionally, if returnDefault is passed in as true, getInt will return the default
   *  value for non-existent (non-extensible) fields and fields with empty data, if an
   *  int default exists. */
  boost::optional<int> getInt(unsigned index, bool returnDefault=false) const;

  /** Get value of the field at index, converted to qurl, if possible. Returns an
   *  uninitialized object if the conversion is unsuccessful for any reason. Logs a warning
   *  if the conversion fails, the field is URLType. Optionally, if returnDefault is passed 
   *  in as true, getURL will return the default value for non-existent (non-extensible) 
   *  fields and fields with empty data, if a url default exists. */
  boost::optional<QUrl> getURL(unsigned index, bool returnDefault=false) const;
  
  /** Returns the IdfExtensibleGroup at groupIndex, if it exists. Otherwise, return value will be
   *  .empty(). */
  IdfExtensibleGroup getExtensibleGroup(unsigned groupIndex) const;

  /** Returns this object's IdfExtensibleGroups. */
  std::vector<IdfExtensibleGroup> extensibleGroups() const;

  //@}
  /** @name Setters */
  //@{

  /** Sets the comment block associated with the object. Will prepend comment characters as
   *  necessary. */
  void setComment(const std::string& msg);

  /** Sets the comment associated with field index, if that field exists. Otherwise returns
   *  false. */
  bool setFieldComment(unsigned index, const std::string& cmnt);

  /** Sets the name field if it exists, returning the actual name string set. Returns false 
   *  otherwise. The return value and newName may differ (by an appended integer) if a
   *  conflict with newName was detected. Name conflicts will not be automatically avoided 
   *  in IDF (text-only) mode. */
  boost::optional<std::string> setName(const std::string& newName);

  /** Sets the name field to a new, unique name, if the name field exists and (is empty or 
   *  overwrite == true). Returns false otherwise. Created name is returned. */
  boost::optional<std::string> createName(bool overwrite=true);

  /** Sets the field at index to value, if possible. Returns false if the value cannot be set for
   *  any reason. (Perhaps index >= numFields().) */
  bool setString(unsigned index, const std::string& value);

  /** Sets the field at index to value, if possible. Returns false if the value cannot be set for
   *  any reason. (Perhaps index >= numFields().) */
  bool setDouble(unsigned index, double value);

  /** Sets the field at index to q, if possible. Returns false if the units of q cannot be converted
   *  to getUnits(index). */
  bool setQuantity (unsigned index, const Quantity q);

  /** Sets the field at index to value, if possible. Returns false if the value cannot be set for
   *  any reason. (Perhaps index >= numFields(), the field is not IntegerType, or the value is out 
   *  of bounds per IddField.properties()). */
  bool setUnsigned(unsigned index, unsigned value);

  /** Sets the field at index to value, if possible. Returns false if the value cannot be set for
   *  any reason. (Perhaps index >= numFields(), the field is not IntegerType, or the value is out 
   *  of bounds per IddField.properties()). */
  bool setInt(unsigned index, int value);

  /** Pushes a new extensible group onto the object, if possible. Return value will be empty() if
   *  the operation cannot be completed for any reason. */
  IdfExtensibleGroup pushExtensibleGroup();

  /** Pushes a new extensible group onto the object, if possible. Return value will be empty() if
   *  the operation cannot be completed for any reason. */
  IdfExtensibleGroup pushExtensibleGroup(const std::vector<std::string>& values);

  /** Inserts a new extensible group at groupIndex, if possible. Return value will be empty() if
   *  the operation cannot be completed for any reason. */
  IdfExtensibleGroup insertExtensibleGroup(unsigned groupIndex);

  /** Inserts a new extensible group at groupIndex, if possible. Return value will be empty() if
   *  the operation cannot be completed for any reason. */
  IdfExtensibleGroup insertExtensibleGroup(unsigned groupIndex, 
                                           const std::vector<std::string>& values);

  /** Pops the final extensible group from the object, if possible. Returns the popped data if 
   *  successful. Otherwise, the returned vector will be empty. */
  std::vector<std::string> popExtensibleGroup();

  /** Erases the extensible group at groupIndex, if possible. Returns the erased data, and 
   *  preserves the relative order of the remaining data if successful. Otherwise, the returned
   *  vector will be empty. */
  std::vector<std::string> eraseExtensibleGroup(unsigned groupIndex);

  /** Pops all \link IdfExtensibleGroup IdfExtensibleGroups\endlink from the object, if possible.
   *  Returns popped data if successful. Otherwise, the returned vector will be empty. */
  std::vector<std::vector<std::string> > clearExtensibleGroups();

  //@}
  /** @name Queries */
  //@{

  /** Returns the current number of fields (including extensible groups) in the object. */
  unsigned numFields() const;

  /** Returns the current number of non-extensible fields in the object. */
  unsigned numNonextensibleFields() const;

  /** Returns the minimum number of fields an object of this type must have to be valid. */
  unsigned minFields() const;

  /** Returns the maximum number of fields an object of this type can have and still be valid. */
  boost::optional<unsigned> maxFields() const;

  /** Returns the number of extensible groups in the object. */
  unsigned numExtensibleGroups() const;

  /** Returns the minimum number of \link IdfExtensibleGroup IdfExtensibleGroups\endlink an 
   *  object of this type must have to be valid. */
  unsigned minExtensibleGroups() const;

  /** Returns the maximum number of \link IdfExtensibleGroup IdfExtensibleGroups\endlink an 
   *  object of this type must have to be valid. */
  boost::optional<unsigned> maxExtensibleGroups() const;

  /** Returns true if index is in objectListFields(). */
  bool isObjectListField(unsigned index) const;

  /** Returns the indices of all fields that can point to other objects. WorkspaceObject manages
   *  all \code \object-list \endcode fields whose reference lists are unambiguously associated with an index-0
   *  name field. */
  std::vector<unsigned> objectListFields() const;

  /** Returns true if index is in dataFields(). */
  bool isDataField(unsigned index) const;

  /** Returns the indices of all fields that are not managedObjectListFields(). */
  std::vector<unsigned> dataFields() const;

  /** Returns the indices of all fields that are required, as defined by the IddObject. */
  std::vector<unsigned> requiredFields() const;

  /** Returns true if this IdfObject is valid, that is, has no errors at or below level. */
  bool isValid(StrictnessLevel level) const;

  /** Returns a ValidityReport for this IdfObject containing all errors at or below level. */
  ValidityReport validityReport(StrictnessLevel level, bool checkNames=true) const;

  /** Checks for equality of data (non-managedObjectList) fields. Prerequisite: iddObject()s 
   *  must be equal. For objects with handle fields, checks for equality of handle OR equality 
   *  of name. */
  bool dataFieldsEqual(const IdfObject& other) const;

  /** Checks for equality of objectListFields(). Prerequisite: iddObject()s must be 
   *  equal. */
  bool objectListFieldsEqual(const IdfObject& other) const;

  /** Returns true if for all managedObjectListFields() that for both objects are non-empty, 
   *  the values are equal (using the same criteria as objectListFieldsEqual). 
   *  Prerequisite: iddObject()s must be equal. */
  bool objectListFieldsNonConflicting(const IdfObject& other) const;

  /** Equality comparator for IdfObjects. Objects must be exactly equal, that is, they must 
   *  share data for the operator to return true. */
  bool operator==(const IdfObject& other) const;

  /** Negation of operator==. */
  bool operator!=(const IdfObject& other) const;

  //@}
  /** @name Serialization */
  //@{

  /** Constructor from text. Parses text and queries the IddFactory for its IddObject. */
  static boost::optional<IdfObject> load(const std::string& text);

  /** Constructor from text and an explicit iddObject. */
  static boost::optional<IdfObject> load(const std::string& text,const IddObject& iddObject);

  /** Returns the width, in characters, of the default amount of space given to field data
   *  during printing. */
  static int printedFieldSpace();

  /** Serialize this object to os as Idf text. */
  std::ostream& print(std::ostream& os) const;

  /** Serialize just the preceding comments and name of this IdfObject in the format used by 
   *  full object print. If hasFields, the name is followed by a ','. Otherwise, the name is 
   *  followed by a ';'. */
  std::ostream& printName(std::ostream& os, bool hasFields=true) const;

  /** Serialize field index in the format used by full object print. If not isLastField, the 
   *  field value is followed by a ','. Otherwise, the object is ended by using a ';'. */
  std::ostream& printField(std::ostream& os, unsigned index, bool isLastField=false) const;

  //@}
  /** @name Type Casting */
  //@{

  // ETH@20101015 Why is this public?!
  //LER@20101028 so that impls can call (someModel.getImpl<some_Impl>())
  //impls do NOT inherit from the object tree, so it's either friend every single
  //model class or make it public.
  // get the impl
  template<typename T>
    std::shared_ptr<T> getImpl() const
  {  return std::dynamic_pointer_cast<T>(m_impl); }

  /// cast to type T, can throw std::bad_cast
  template<typename T>
  T cast() const{
    std::shared_ptr<typename T::ImplType> impl = this->getImpl<typename T::ImplType>();
    if (!impl){
      throw(std::bad_cast());
    }
    return T(impl);
  }

  /// cast to optional of type T
  template<typename T>
  boost::optional<T> optionalCast() const{
    boost::optional<T> result;
    std::shared_ptr<typename T::ImplType> impl = this->getImpl<typename T::ImplType>();
    if (impl){
      result = T(impl);
    }
    return result;
  }

  //@}

 protected:

  typedef detail::IdfObject_Impl ImplType;

  friend struct IdfObjectImplLess;           // lets friend sort by impl
  friend class detail::IdfObject_Impl;       // for IdfObject_Impl::getObject<T>()
  friend class detail::WorkspaceObject_Impl; // for WorkspaceObject_Impl construction
  friend class detail::Workspace_Impl;       // for finding IdfObjects in a workspace
  friend class WorkspaceObject;              // for WorkspaceObject::idfObject()
  friend class Workspace;                    // for toIdfFile completion (constructs IdfObject from impl)

  /** Protected constructor from impl. */
  IdfObject(std::shared_ptr<detail::IdfObject_Impl> impl);

 private:

  // pointer to impl
  std::shared_ptr<detail::IdfObject_Impl> m_impl;

  // configure logging
  REGISTER_LOGGER("utilities.idf.IdfObject");
};

/** Function object for sorting objects by name. \relates IdfObject */
struct UTILITIES_API IdfObjectNameLess {
  bool operator()(const IdfObject& left, const IdfObject& right) const;
};

/** Function object for sorting objects by impl. \relates IdfObject */
struct UTILITIES_API IdfObjectImplLess {
  bool operator()(const IdfObject& left, const IdfObject& right) const;
};

/** Function object for sorting by IddObjectType. \relates IdfObject */
struct UTILITIES_API IdfObjectTypeLess {
  bool operator()(const IdfObject& left, const IdfObject& right) const;
};

/** \relates IdfObject */
typedef boost::optional<IdfObject> OptionalIdfObject;

/** \relates IdfObject */
typedef std::vector<IdfObject> IdfObjectVector;

/** \relates IdfObject */
typedef std::pair<IdfObject,IdfObject> IdfObjectPair;

/** \relates IdfObject */
UTILITIES_API std::ostream& operator<<(std::ostream& os, const IdfObject& IdfObject);

/** Helper function to get the handles of a vector of objects. \relates IdfObject */
template<typename T>
std::vector<Handle> getHandles(const std::vector<T>& objects)
{
  HandleVector result;
  for (const T& object : objects) { result.push_back(object.handle()); }
  return result;
}

/** Returns obj.name().get() if it exists. Otherwise, returns the IddObject.name(), 
 *  which is a good choice for unique objects. */
UTILITIES_API std::string objectName(const IdfObject& obj);

template<typename T>
std::vector<T> sortByObjectName(std::vector<T> objects) {
  std::sort(objects.begin(),
            objects.end(),
            std::bind(&istringLess,std::bind(&objectName,std::placeholders::_1),std::bind(&objectName,std::placeholders::_1)));
  return objects;
}

} // openstudio

#endif //UTILITIES_IDF_IDFOBJECT_HPP
