/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_IDF_IDFEXTENSIBLEGROUP_HPP
#define UTILITIES_IDF_IDFEXTENSIBLEGROUP_HPP

#include "../UtilitiesAPI.hpp"
#include "../idd/ExtensibleIndex.hpp"
#include "Handle.hpp"

#include "../core/Logger.hpp"

namespace openstudio {

namespace detail {

  // forward declarations
  class IdfObject_Impl;

}  // namespace detail

// forward declarations
class IdfObject;

/** IdfExtensibleGroup wraps a single set of extensible fields in an IdfObject. For instance,
 *  many geometric objects have extensible groups each of which represents a single vertex
 *  (one three-dimensional point). (Also see the documentation for IddObject,
 *  IddObjectProperties, and ExtensibleIndex.) */
class UTILITIES_API IdfExtensibleGroup
{
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~IdfExtensibleGroup() = default;

  //@}
  /** @name Getters */
  //@{

  /** Returns this extensible group's fields. Return value will be empty() if this group is empty(). */
  std::vector<std::string> fields(bool returnDefault = false) const;

  /** Returns this extensible group's fields. Return value will be empty() if this group is empty().
    * Unlike fields(), in the case it's a WorkspaceObject_Impl, it uses handles as string and not the name of the target object */
  std::vector<std::string> fieldsWithHandles(bool returnDefault = false) const;

  /** Returns the comments associated with this extensible group's fields. */
  std::vector<std::string> fieldComments(bool returnDefault = false) const;

  /** Returns the comment associated with this extensible group's field fieldIndex. */
  boost::optional<std::string> fieldComment(unsigned fieldIndex, bool returnDefault = false) const;

  /** Returns the string value of this extensible group's field fieldIndex, if that field
   *  exists (isValid(fieldIndex)). */
  boost::optional<std::string> getString(unsigned fieldIndex, bool returnDefault = false) const;

  /** Like getString except for reference fields getString will return the name of the referenced object.
   * This method, getField, will always return the string value  of the field.   */
  boost::optional<std::string> getField(unsigned index, bool returnDefault = false) const;

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

  /** setString that can delay signal emits. */
  bool setString(unsigned fieldIndex, const std::string& value, bool checkValidity);

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

  /** Equality comparator for \link IdfExtensibleGroup IdfExtensibleGroups\endlink. Tests for
   *  strict identity (same IdfObject, same groupIndex()). */
  bool operator==(const IdfExtensibleGroup& other) const;

  /** Negation of operator==.*/
  bool operator!=(const IdfExtensibleGroup& other) const;

  //@}
  /** @name Type Casting */
  //@{

  /// cast to type T, can throw std::bad_cast
  template <typename T>
  T cast() const {
    return T(this->getImpl<typename T::ImplType>(), m_index);
  }

  /// cast to optional of type T
  template <typename T>
  boost::optional<T> optionalCast() const {
    boost::optional<T> result;
    std::shared_ptr<typename T::ImplType> impl = this->getImpl<typename T::ImplType>();
    if (impl) {
      result = T(std::move(impl), m_index);
    }
    return result;
  }

  /** Get the parent object. */
  template <typename T>
  T getObject() const {
    T result(std::move(std::dynamic_pointer_cast<typename T::ImplType>(m_impl)));
    return result;
  }

  /** Get the parent object. */
  template <typename T>
  boost::optional<T> getOptionalObject() const {
    boost::optional<T> result;
    std::shared_ptr<typename T::ImplType> p = std::dynamic_pointer_cast<typename T::ImplType>(m_impl);
    if (p) {
      result = T(std::move(p));
    }
    return result;
  }

  //@}
 protected:
  ///@cond
  using ImplType = detail::IdfObject_Impl;

  friend class detail::IdfObject_Impl;

  std::shared_ptr<detail::IdfObject_Impl> m_impl;
  unsigned m_index;  // index corresponding to first field of the extensible group

  // get the impl
  template <typename T>
  std::shared_ptr<T> getImpl() const {
    return std::dynamic_pointer_cast<T>(m_impl);
  }

  /** Private constructor to be used by IdfObject. */
  IdfExtensibleGroup(std::shared_ptr<detail::IdfObject_Impl> impl, unsigned index);

  /** Set all the fields in this group, if possible. Returns false if values is wrong size or
   *  setString fails on any individual fields. Emits signals if checkValidity == true. Function
   *  provided so object method implementations can call it with checkValidity == false. */
  bool setFields(const std::vector<std::string>& values, bool checkValidity);

  unsigned mf_toIndex(unsigned fieldIndex) const;

  /** Return all object indices accessible through this extensible group. */
  std::vector<unsigned> mf_indices() const;

  std::vector<unsigned> mf_subsetAndToFieldIndices(std::vector<unsigned> indices) const;
  ///@endcond
 private:
  /** Private default constructor. */
  IdfExtensibleGroup();

  REGISTER_LOGGER("openstudio.IdfExtensibleGroup");
};

/** \relates IdfExtensibleGroup */
using IdfExtensibleGroupVector = std::vector<IdfExtensibleGroup>;

}  // namespace openstudio

#endif  // UTILITIES_IDF_IDFEXTENSIBLEGROUP_HPP
