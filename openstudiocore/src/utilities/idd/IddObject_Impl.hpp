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

#ifndef UTILITIES_IDD_IDDOBJECT_IMPL_HPP
#define UTILITIES_IDD_IDDOBJECT_IMPL_HPP

#include "../UtilitiesAPI.hpp"
#include "IddObjectProperties.hpp"
#include "IddFieldProperties.hpp"
#include "IddField.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../core/Logger.hpp"
#include "../core/Containers.hpp"

#include <string>
#include <ostream>
#include <vector>

namespace openstudio {

// forward declarations
class ExtensibleIndex;

namespace detail {

  /** Implementation of IddObject */
  class UTILITIES_API IddObject_Impl {
   public:

    /** @name Constructors */
    //@{

    /** Default constructor returns Catchall object. */
    IddObject_Impl();

    //@}
    /** @name Getters */
    //@{

    /** Get this IddObject's name. */
    std::string name() const;

    /** Get the type of this object, as specified by the OPENSTUDIO_ENUM IddObjectType. type()
     *  is essentially an enum-encoding of name(). type() should be equivalent to
     *  IddObjectType(name()). */
    IddObjectType type() const;

    /** Get the name of the Idd group to which this object belongs. */
    std::string group() const;

    /** Get the properties of this object. */
    const IddObjectProperties& properties() const;

    /** Get all non-extensible \link IddField IddFields\endlink. */
    const IddFieldVector& nonextensibleFields() const;

    /** Get this object's extensible group, that is, the vector of extensible \link IddFields
     *  IddField\endlink. */
    const IddFieldVector& extensibleGroup() const;

    /** Get the IddField at index, if it exists. Returns fields in extensible groups using the
     *  assumption that extensible groups repeat indefinitely. */
    OptionalIddField getField(unsigned index) const;

    /** Get the field named fieldName. Extensible fields are included, but they are not named
     *  uniquely. */
    OptionalIddField getField(const std::string& fieldName) const;

    /** Get the index of the IddField named fieldName. */
    boost::optional<int> getFieldIndex(const std::string& fieldName) const;

    //@}
    /** @name Setters */
    //@{

    /** If not already present, inserts a field of type handle at the top of the object. */
    void insertHandleField();

    //@}
    /** @name Queries */
    //@{

    /** Get number of non-extensible fields. */
    unsigned numFields() const;

    /** The original number of fields included in an object constructed by IddObjectType. Accounts 
     *  for properties().minFields, properties().numExtensibleGroupsRequired, and 
     *  IddField::properties().required. */
    unsigned numFieldsInDefaultObject() const;

    /** Returns true if name contains some form of "Version". */
    bool isVersionObject() const;

    /** Returns true if index in IdfObject is not part of an extensible group. */
    bool isNonextensibleField(unsigned index) const;

    /** Returns true if index in IdfObject would be part of extensible group. */
    bool isExtensibleField(unsigned index) const;

    /** Returns true if first IddField is of type handle. */
    bool hasHandleField()  const;

    /** Returns true if first IddField (after the handle field, if present) is a name. */
    bool hasNameField() const;

    /** Returns the index containing the name field, if hasNameField(). Otherwise, returns
     *  boost::none. */
    boost::optional<unsigned> nameFieldIndex() const;

    /** Returns true if index in IdfObject corresponds to a required field. */
    bool isRequiredField(unsigned index) const;

    /** Get this IddObject's url flag. True if this object has a url in it */
    bool hasURL() const;

    /** Returns ExtensibleIndex(groupIndex,fieldIndex) that corresponds to field index. Throws if
     *  !isExtensibleField(index). */
    ExtensibleIndex extensibleIndex(unsigned index) const;

    /** Returns the overall IddObject index equivalent to extensibleIndex. Throws if IddObject 
     *  is not extensible or if extensibleIndex.field >= properties().numExtensible. */
    unsigned index(ExtensibleIndex extensibleIndex) const;

    /** Get the reference lists to which this object belongs. This method only returns supported 
     *  reference lists attached to an index 0 name field. */
    std::vector<std::string> references() const;

    /// get all the object lists that fields in this object refer to
    StringSet objectLists() const;

    /// get all the object lists that field index refers to
    StringSet objectLists(unsigned index) const;

    /// get the indices of the fields of \object-list type. includes indices in the first
    /// extensible group
    std::vector<unsigned> objectListFields() const;
    
    /// get the indices of the fields of \type url
    std::vector<unsigned> urlFields() const;

    /// equality operator
    bool operator==(const IddObject_Impl& other) const;

    //@}
    /** @name Serialization */
    //@{

    /** Load from name, group, type, and text. */
    static std::shared_ptr<IddObject_Impl> load(const std::string& name, 
                                                  const std::string& group,
                                                  const std::string& text, 
                                                  IddObjectType type);

    // print
    std::ostream& print(std::ostream& os) const;

    //@}

   private:

    std::string m_name;   
    std::string m_group;               // group name
    IddObjectType m_type;
    IddObjectProperties m_properties;
    IddFieldVector m_fields;           // vector of non-extensible fields
    IddFieldVector m_extensibleFields; // vector of extensible fields, forms single
                                       // extensible field group
    std::vector<unsigned> m_urlIdx;
    // .first = hasNameField(); .second = nameFieldIndex
    mutable boost::optional< std::pair<bool,unsigned> > m_nameFieldCache;

    // partial constructor used by load
    IddObject_Impl(const std::string& name, const std::string& group, IddObjectType type);

    // parse
    void parse(const std::string& text);

    void parseObject(const std::string& text);
    void parseProperty(const std::string& text);
    void parseFields(const std::string& text);
    void makeExtensible();

    // configure logging
    REGISTER_LOGGER("utilities.idd.IddObject");
  };
  
} // detail
} // openstudio

#endif // UTILITIES_IDD_IDDOBJECT_IMPL_HPP
