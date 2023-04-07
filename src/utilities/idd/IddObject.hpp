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

#ifndef UTILITIES_IDD_IDDOBJECT_HPP
#define UTILITIES_IDD_IDDOBJECT_HPP

#include "../UtilitiesAPI.hpp"
#include "IddObjectProperties.hpp"
#include "IddField.hpp"

#include "../core/Logger.hpp"

#include <ostream>
#include <vector>

#include <boost/optional.hpp>

namespace openstudio {

// forward declarations
class ExtensibleIndex;
struct IddObjectType;

namespace detail {
  class IddObject_Impl;
}  // namespace detail

/** IddObject represents an object in the Idd.  IddObject is a shared object. */
class UTILITIES_API IddObject
{
 public:
  /** @name Constructors */
  //@{

  /** Default constructor returns Catchall object. This object is provided to help users
   *  correct typos in their object names. */
  IddObject();

  /** Copy constructor returns an IddObject that shares its data with other. */
  IddObject(const IddObject& other);

  //@}
  /** @name Getters */
  //@{

  /** Get this IddObject's name. */
  std::string name() const;

  /** Get object type, as specified by the OPENSTUDIO_ENUM IddObjectType. Similar information to
   *  name() for \link IddObject IddObjects \endlink stored by the \link IddFactorySingleton
   *  IddFactory \endlink, that is type() == IddObjectType(name()) and
   *  type().valueDescription() == name(). */
  IddObjectType type() const;

  /** Get the name of the IDD group to which this object belongs. */
  std::string group() const;

  /** Get the properties of this object. */
  const IddObjectProperties& properties() const;

  /** Get all non-extensible \link IddField IddFields\endlink. */
  const std::vector<IddField>& nonextensibleFields() const;

  /** Get this object's extensible group, that is, the vector of extensible \link IddField
   *  IddFields \endlink that may be repeated indefinitely (except as capped by
   *  properties().maxFields) at the end of an IdfObject following this schema. */
  const std::vector<IddField>& extensibleGroup() const;

  /** Get the IddField at index, if it exists. Returns fields in extensible groups using the
   *  assumption that extensible groups repeat indefinitely. */
  boost::optional<IddField> getField(unsigned index) const;

  /** Get the IddField in this object named fieldName. Extensible fields are included, but they
   *  are not named uniquely. */
  boost::optional<IddField> getField(const std::string& fieldName) const;

  /** Get the index of the IddField named fieldName. */
  boost::optional<int> getFieldIndex(const std::string& fieldName) const;

  //@}
  /** @name Setters
   *
   *  Most users should not use any of the IDD setters. */
  //@{

  /** If not already present, inserts a field of type handle at the top of the object. Not for
   *  general use. */
  void insertHandleField();

  //@}
  /** @name Queries */
  //@{

  /** Returns the number of non-extensible fields in this object. */
  unsigned numFields() const;

  /** Returns the initial number of fields with which to populate an IdfObject constructed from
   *  IddObject or IddObjectType. Accounts for properties().minFields,
   *  properties().numExtensibleGroupsRequired, and IddField::properties().required. */
  unsigned numFieldsInDefaultObject() const;

  /** Returns true if name contains some form of "Version". */
  bool isVersionObject() const;

  /** Returns true if index, as used in an IdfObject following this schema, corresponds to a
   *  non-extensible field. */
  bool isNonextensibleField(unsigned index) const;

  /** Returns true if index, as used in an IdfObject following this schema, corresponds to a
   *  field in an extensible group. */
  bool isExtensibleField(unsigned index) const;

  /** Returns true if first IddField is of type handle. This should be true for every OpenStudio
   *  IddObject and false for every EnergyPlus IddObject. */
  bool hasHandleField() const;

  /** Returns true if the first IddField after the handle field, if present, is a name IddField. */
  bool hasNameField() const;

  /** Returns the index containing the name field, if hasNameField(). Otherwise, returns
   *  boost::none. */
  boost::optional<unsigned> nameFieldIndex() const;

  /** Returns true if index, as used in an IdfObject following this schema, corresponds to a
   *  required field. */
  bool isRequiredField(unsigned index) const;

  /** Returns true if this object has any url fields. */
  bool hasURL() const;

  /** Returns the indices of all fields of url type. */
  std::vector<unsigned> urlFields() const;

  /** Returns the ExtensibleIndex(groupIndex,fieldIndex) that corresponds to field index. Throws if
   *  !isExtensibleField(index). */
  ExtensibleIndex extensibleIndex(unsigned index) const;

  /** Returns the overall IddObject index equivalent to extensibleIndex. Throws if IddObject
   *  is not extensible or if extensibleIndex.field >= properties().numExtensible. */
  unsigned index(ExtensibleIndex extensibleIndex) const;

  /** Returns the reference lists to which this object belongs. This method only returns reference
   *  lists explicitly attached the name field. (It does not include 'AllObjects', for instance.) */
  std::vector<std::string> references() const;

  /** Returns the union of all the object lists to which fields in this object can refer. */
  std::set<std::string> objectLists() const;

  /** Returns all the object lists to which IddField index refers. */
  std::set<std::string> objectLists(unsigned index) const;

  /** Returns the indices of the \link IddField IddFields \endlink in this object of object-list
   *  type. Includes indices in the first extensible group. */
  std::vector<unsigned> objectListFields() const;

  /** Returns true if all underlying data is equal (either trivially or by exhaustive
   *  comparison). */
  bool operator==(const IddObject& other) const;

  /** The negation of operator==. */
  bool operator!=(const IddObject& other) const;

  //@}
  /** @name Serialization */
  //@{

  /** Load from name, group, type, and text. The first string in the IddObject text, appearing
   *  immediately before the first comma, is the IddObject name. The group string comes from the
   *  IddFile. If not constructed by the IddFactory, or the IddObject does not otherwise come from
   *  the EnergyPlus or OpenStudio IDD files, type should be equal to UserCustom. The constructor
   *  also needs the full text of the object for parsing. */
  static boost::optional<IddObject> load(const std::string& name, const std::string& group, const std::string& text, IddObjectType type);

  /** \overload Sets type to IddObjectType::UserCustom. */
  static boost::optional<IddObject> load(const std::string& name, const std::string& group, const std::string& text);

  /** Print this object to os, in standard IDD format. */
  std::ostream& print(std::ostream& os) const;

  //@}
 private:
  ///@cond
  // pointer to impl
  std::shared_ptr<detail::IddObject_Impl> m_impl;

  // construct from impl
  IddObject(const std::shared_ptr<detail::IddObject_Impl>& impl);
  ///@endcond

  // configure logging
  REGISTER_LOGGER("utilities.idd.IddObject");
};

/** \relates IddObject */
using OptionalIddObject = boost::optional<IddObject>;

/** \relates IddObject */
using IddObjectVector = std::vector<IddObject>;

/** \relates IddObject */
UTILITIES_API std::ostream& operator<<(std::ostream& os, const IddObject& iddObject);

/** \relates IddObject */
UTILITIES_API std::vector<IddObjectType> getIddObjectTypeVector(const std::vector<IddObject>& objects);

/** \relates IddObject */
UTILITIES_API std::set<IddObjectType> getIddObjectTypeSet(const std::vector<IddObject>& objects);

/** Converts the vector of \link IddKey IddKeys\endlink associated with object.getField(index)
 *  to a vector of std::string. \relates IddObject */
UTILITIES_API std::vector<std::string> getIddKeyNames(const IddObject& object, unsigned index);

}  // namespace openstudio

#endif  //UTILITIES_IDD_IDDOBJECT_HPP
