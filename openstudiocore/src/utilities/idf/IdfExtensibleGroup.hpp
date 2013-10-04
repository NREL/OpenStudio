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

#ifndef UTILITIES_IDF_IDFEXTENSIBLEGROUP_HPP
#define UTILITIES_IDF_IDFEXTENSIBLEGROUP_HPP

#include <utilities/UtilitiesAPI.hpp>
#include <utilities/idd/ExtensibleIndex.hpp>
#include <utilities/idf/Handle.hpp>

#include <utilities/core/Logger.hpp>

#include <boost/shared_ptr.hpp>

namespace openstudio {

namespace detail {

  // forward declarations
  class IdfObject_Impl;

} // detail

// forward declarations
class IdfObject;

/** IdfExtensibleGroup wraps a single set of extensible fields in an IdfObject. For instance, 
 *  many geometric objects have extensible groups each of which represents a single vertex
 *  (one three-dimensional point). (Also see the documentation for IddObject, 
 *  IddObjectProperties, and ExtensibleIndex.) */
class UTILITIES_API IdfExtensibleGroup {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~IdfExtensibleGroup() {}

  //@}
  /** @name Getters */
  //@{

  /** Returns this extensible group's fields. Return value will be empty() if this group is 
   *  empty(). */
  std::vector<std::string> fields(bool returnDefault = false) const;

  /** Returns the comments associated with this extensible group's fields. */
  std::vector<std::string> fieldComments(bool returnDefault = false) const;

  /** Returns the comment associated with this extensible group's field fieldIndex. */
  boost::optional<std::string> fieldComment(unsigned fieldIndex, bool returnDefault = false) const;

  /** Returns the string value of this extensible group's field fieldIndex, if that field 
   *  exists (isValid(fieldIndex)). */
  boost::optional<std::string> getString(unsigned fieldIndex, bool returnDefault = false) const;
  
  /** Returns true if the field is empty. */
  bool isEmpty(unsigned fieldIndex) const;

  /** Returns the value, converted to type double, of this extensible group's field fieldIndex,
   *  if possible. */
  boost::optional<double> getDouble(unsigned fieldIndex, bool returnDefault = false) const;

  /** Returns the value, converted to type unsigned, of this extensible group's field fieldIndex,
   *  if possible. */
  boost::optional<unsigned> getUnsigned(unsigned fieldIndex, bool returnDefault = false) const;

  /** Returns the value, converted to type int, of this extensible group's field fieldIndex,
   *  if possible. */
  boost::optional<int> getInt(unsigned fieldIndex, bool returnDefault = false) const;

  //@}
  /** @name Setters */
  //@{

  /** Sets the comment associated with this extensible group's field fieldIndex, if possible. 
   *  Otherwise, returns false. */
  bool setFieldComment(unsigned fieldIndex, const std::string& cmnt);

  /** Set all the fields in this IdfExtensibleGroup, if possible. Returns false if values is wrong 
   *  size or setString fails on any individual fields. */
  bool setFields(const std::vector<std::string>& values);

  /** Set this extensible group's field fieldIndex to value, if possible. Returns false if it 
   *  cannot be set for any reason. */
  bool setString(unsigned fieldIndex, const std::string& value);

  /** Set this extensible group's field fieldIndex to value, if possible. Returns false if it 
   *  cannot be set for any reason. */
  bool setDouble(unsigned fieldIndex, double value);

  /** Set this extensible group's field fieldIndex to value, if possible. Returns false if it 
   *  cannot be set for any reason. */
  bool setUnsigned(unsigned fieldIndex, unsigned value);

  /** Set this extensible group's field fieldIndex to value, if possible. Returns false if it 
   *  cannot be set for any reason. */
  bool setInt(unsigned fieldIndex, int value);

  /** Clones the data in this extensible group, and pushes the new extensible group onto the end
   *  of the enclosing object. The returned IdfExtensibleGroup will be empty() if the operation 
   *  cannot be completed. */
  IdfExtensibleGroup pushClone() const;

  /** Clones the data in this extensible group, and inserts the new extensible group at groupIndex
   *  within the enclosing object. The returned IdfExtensibleGroup will be empty() if the operation 
   *  cannot be completed. */
  IdfExtensibleGroup insertClone(unsigned groupIndex) const;

  //@} 
  /** @name Queries */
  //@{

  /** Returns true if this ExtensibleGroup holds no data. */
  bool empty() const;

  /** Returns the (group) index of this ExtensibleGroup. Indexing starts at 0. Throws if 
   *  ExtensibleGroup is empty(). */
  unsigned groupIndex() const;

  /** Returns the number of fields in this ExtensibleGroup. */
  unsigned numFields() const;

  /** Returns true if fieldIndex is valid for this extensible group. */
  bool isValid(unsigned fieldIndex) const;

  /** Returns the indices of all fields that can point to other objects. */
  std::vector<unsigned> objectListFields() const;

  /** Returns the indices of all fields that are not objectListFields() (and are therefore plain
   *  alpha or numeric data). */
  std::vector<unsigned> dataFields() const;

  /** Returns the indices of all fields in this ExtensibleGroup that are required, as defined 
   *  by the IddObject. */
  std::vector<unsigned> requiredFields() const;

  /** Equality comparitor for \link IdfExtensibleGroup IdfExtensibleGroups\endlink. Tests for 
   *  strict identity (same IdfObject, same groupIndex()). */
  bool operator==(const IdfExtensibleGroup& other) const;

  /** Negation of operator==.*/
  bool operator!=(const IdfExtensibleGroup& other) const;
 
  //@}
  /** @name Type Casting */
  //@{

  /// cast to type T, can throw std::bad_cast
  template<typename T>
  T cast() const{
    return T(this->getImpl<typename T::ImplType>(),m_index);
  }

  /// cast to optional of type T
  template<typename T>
  boost::optional<T> optionalCast() const{
    boost::optional<T> result;
    boost::shared_ptr<typename T::ImplType> impl = this->getImpl<typename T::ImplType>();
    if (impl){
      result = T(impl,m_index);
    }
    return result;
  }

  /** Get the parent object. */
  template<typename T>
  T getObject() const { 
    T result(boost::dynamic_pointer_cast<typename T::ImplType>(m_impl));
    return result; 
  }

  /** Get the parent object. */
  template<typename T>
  boost::optional<T> getOptionalObject() const { 
    boost::optional<T> result;
    boost::shared_ptr<typename T::ImplType> p = boost::dynamic_pointer_cast<typename T::ImplType>(m_impl);
    if (p){
     result = T(p);
    }
    return result; 
  }

  //@}
 protected:
  ///@cond
  typedef detail::IdfObject_Impl ImplType;

  friend class detail::IdfObject_Impl;  

  boost::shared_ptr<detail::IdfObject_Impl> m_impl;
  unsigned m_index; // index corresponding to first field of the extensible group

  // get the impl
  template<typename T>
  boost::shared_ptr<T> getImpl() const
  {  
    return boost::dynamic_pointer_cast<T>(m_impl); 
  }

  /** Private constructor to be used by IdfObject. */
  IdfExtensibleGroup(boost::shared_ptr<detail::IdfObject_Impl> impl,unsigned index);  

  /** Set all the fields in this group, if possible. Returns false if values is wrong size or
   *  setString fails on any individual fields. Emits signals if checkValidity == true. Function
   *  provided so object method implementations can call it with checkValidity == false. */
  bool setFields(const std::vector<std::string>& values,bool checkValidity);

  unsigned mf_toIndex(unsigned fieldIndex) const; 

  /** Return all object indices accessible through this extensible group. */
  std::vector<unsigned> mf_indices() const;

  std::vector<unsigned> mf_subsetAndToFieldIndices(std::vector<unsigned> indices) const;
  ///@endcond 
 private:
  /** Private default constructor. */
  IdfExtensibleGroup();

  /** setString that can delay signal emits. */
  bool setString(unsigned fieldIndex, const std::string& value, bool checkValidity);

  REGISTER_LOGGER("openstudio.IdfExtensibleGroup");
};

/** \relates IdfExtensibleGroup */
typedef std::vector<IdfExtensibleGroup> IdfExtensibleGroupVector;

} // openstudio

#endif // UTILITIES_IDF_IDFEXTENSIBLEGROUP_HPP
