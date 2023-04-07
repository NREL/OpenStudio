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

#ifndef UTILITIES_IDD_IDDOBJECT_IMPL_HPP
#define UTILITIES_IDD_IDDOBJECT_IMPL_HPP

#include "../UtilitiesAPI.hpp"
#include "IddObjectProperties.hpp"
#include "IddFieldProperties.hpp"
#include "IddField.hpp"

#include "../core/Logger.hpp"
#include "../core/Containers.hpp"

#include "IddEnums.hpp"

#include <string>
#include <ostream>
#include <vector>

namespace openstudio {

// forward declarations
class ExtensibleIndex;

namespace detail {

  /** Implementation of IddObject */
  class UTILITIES_API IddObject_Impl
  {
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
    bool hasHandleField() const;

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
    static std::shared_ptr<IddObject_Impl> load(const std::string& name, const std::string& group, const std::string& text, IddObjectType type);

    // print
    std::ostream& print(std::ostream& os) const;

    //@}

   private:
    std::string m_name;
    std::string m_group;  // group name
    IddObjectType m_type;
    IddObjectProperties m_properties;
    IddFieldVector m_fields;            // vector of non-extensible fields
    IddFieldVector m_extensibleFields;  // vector of extensible fields, forms single
                                        // extensible field group
    std::vector<unsigned> m_urlIdx;
    // .first = hasNameField(); .second = nameFieldIndex
    mutable boost::optional<std::pair<bool, unsigned>> m_nameFieldCache;

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

}  // namespace detail
}  // namespace openstudio

#endif  // UTILITIES_IDD_IDDOBJECT_IMPL_HPP
