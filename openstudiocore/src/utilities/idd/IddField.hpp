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

#ifndef UTILITIES_IDD_IDDFIELD_HPP
#define UTILITIES_IDD_IDDFIELD_HPP

#include "../UtilitiesAPI.hpp"
#include "IddFieldProperties.hpp"

#include "../core/Logger.hpp"

#include <string>
#include <ostream>
#include <vector>

#include <boost/optional.hpp>

namespace openstudio {

class Unit;
class IddKey;

// forward declarations
namespace detail {
  class IddField_Impl; 
}

/** IddField represents a field in an IddObject, that is, the schema for a single piece of 
 *  data (alpha or numeric) in an IDF. */
class UTILITIES_API IddField {
 public:
  /** @name Constructors */
  //@{

  /** Default constructor. */
  IddField();

  //@}
  /** @name Getters */
  //@{

  /** Returns this field's name. */
  std::string name() const;

  /** Returns this field's id in its parent IddObject, e.g. A1, A2, N1, N2. */
  std::string fieldId() const;

  /** Returns the properties of this field, that is, a list of IDD-markup information such as
   *  data type, default value, units, and numeric bounds. */
  const IddFieldProperties& properties() const;

  /** Returns the SI or IP units associated with this field, if they exist. All IDF data is stored
   *  in  SI. The return value with returnIP == true can be used to convert underlying data to 
   *  the default IP units. If there are no particular IP units specified in the IDD, this 
   *  method attempts to choose appropriate units to which one can convert. */
  boost::optional<Unit> getUnits(bool returnIP = false) const;

  /** Returns true if the units markup for this field specifies that the units are dependent on 
   *  the value of another field. Such units are not handled at the IdfFile/Workspace level. They
   *  should be explicitly handled by individual ModelObject classes. */
  bool unitsBasedOnOtherField() const;

  /** Get the IddKey object by keyName (case insensitive). */
  boost::optional<IddKey> getKey(const std::string& keyName) const;

  /** Get all of the IddKeys for this field. Only expected to be non-empty if this is a choice 
   *  field (properties().type() == IddFieldType::ChoiceType). */
  std::vector<IddKey> keys() const;

  //@}
  /** @name Setters 
   *
   *  Most users should not use any of the IDD setters. */
  //@{

  /** Set the field name. Not for general use. */
  void setName(const std::string& name);

  /** Increments the field id number, that is, A1 becomes A2, etc. Added to support
   *  IddObject::insertHandleField(). Not for general use. */
  void incrementFieldId(const boost::regex& fieldType = boost::regex("A"));

  //@}
  /** @name Queries */
  //@{

  /** Returns true if this field defines a reference name in IDF, or can be otherwise distinguised
   *  as an IdfObject name. Returns true if the field has a \\reference property and does not have 
   *  an \\object-list property (if it has both, it is an example of a forwarded reference), or if 
   *  the field is a plain AlphaType field named 'Name'. */
  bool isNameField() const;

  /** Returns true if this field is of object-list type and calls out one or more object-lists. */
  bool isObjectListField() const;

  /** Equality operator. Returns true if this IddField and other share an impl or if all 
   *  underlying data is exactly equal. */
  bool operator==(const IddField& other) const;

  /** Inequality operator. Negation of operator==. */
  bool operator!=(const IddField& other) const;

  //@}
  /** @name Serialization */
  //@{

  /** Load the IddField, that is, parse its text. Whitespace is not preserved. The string 
   *  following the \\field slash code, or, if that is absent, the 'A' or 'N' identifier, 
   *  should be sent in as name. objectName is the IddObject.name() to which this field 
   *  belongs. */
  static boost::optional<IddField> load(const std::string& name, 
                                        const std::string& text, 
                                        const std::string& objectName);

  /** Print the IddField to an output stream. Field slash codes are indented to produce pretty 
   *  output. If lastField, then the field id will be followed by a semi-colon; otherwise, a 
   *  comma will be used (consistent with IDD formatting). */
  std::ostream& print(std::ostream& os, bool lastField) const;

  //@}
 private:
  ///@cond
  // pointer to impl
  std::shared_ptr<detail::IddField_Impl> m_impl;

  // construct from impl
  IddField(const std::shared_ptr<detail::IddField_Impl>& impl);
  ///@endcond

  // configure logging
  REGISTER_LOGGER("utilities.idd.IddField");
};

/** \relates IddField */
typedef std::vector<IddField> IddFieldVector;

/** \relates IddField */
typedef boost::optional<IddField> OptionalIddField;

/** Returns true if the reference lists of field1 and field2 match exactly (case insensitive). 
 *  \relates IddField */
UTILITIES_API bool referencesEqual(const IddField& field1, const IddField& field2);

} // openstudio

#endif //UTILITIES_IDD_IDDFIELD_HPP
