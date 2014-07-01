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

#ifndef UTILITIES_IDF_IDFOBJECT_IMPL_HPP
#define UTILITIES_IDF_IDFOBJECT_IMPL_HPP

#include <utilities/UtilitiesAPI.hpp>
#include <utilities/idf/Handle.hpp>
#include <utilities/idf/IdfObjectDiff.hpp>
#include <utilities/idd/IddObject.hpp>

#include <utilities/core/Logger.hpp>
#include <utilities/core/Containers.hpp>

#include <boost/optional.hpp>

#include <QObject>
#include <QUrl>

#include <string>
#include <ostream>
#include <vector>

namespace openstudio { 

// forward declarations
class IdfObject;
class IdfExtensibleGroup;
struct IdfObjectImplLess;
class StrictnessLevel;
class ValidityReport;
class DataError;
class Quantity;
class OSOptionalQuantity;
  
// private namespace
namespace detail { 

  /** Implementation of IdfObject. */
  class UTILITIES_API IdfObject_Impl : public QObject, public std::enable_shared_from_this<IdfObject_Impl> {
    Q_OBJECT;
   public:

    /** @name Constructors */
    //@{

    /** Copy constructor, used for cloning. */
    IdfObject_Impl(const IdfObject_Impl& other, bool keepHandle=false);

    /** Constructor from type. Equivalent to IdfObject(IddFactory::instance.iddObject(type)). */
    explicit IdfObject_Impl(IddObjectType type, bool fastName=false);

    /** Constructor from iddObject. */
    explicit IdfObject_Impl(const IddObject& iddObject, bool fastName=false);

    /** Constructor from underlying data. Used by WorkspaceObject_Impl. */
    IdfObject_Impl(const Handle& handle,
                   const std::string& comment,
                   const IddObject& iddObject,
                   const StringVector& fields,
                   const StringVector& fieldComments);

    virtual ~IdfObject_Impl() {}

    //@}
    /** @name Getters */
    //@{
      
    /** Returns the handle associated with the object. */
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
    virtual boost::optional<std::string> getString(unsigned index,
                                                   bool returnDefault=false,
                                                   bool returnUninitializedEmpty=false) const;

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
    OSOptionalQuantity getQuantity (unsigned index, bool returnDefault=false, bool returnIP=false) const;

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
    void setComment(const std::string& msg, bool checkValidity);

    /** Sets the comment associated with field index, if that field exists. Otherwise returns
     *  false. */
    bool setFieldComment(unsigned index, const std::string& cmnt);
    bool setFieldComment(unsigned index, const std::string& cmnt, bool checkValidity);

    /** Sets the name field if it exists, returning the actual name string set. Returns false 
     *  otherwise. The return value and newName may differ (by an appended integer) if a
     *  conflict with newName was detected. Name conflicts will not be automatically avoided 
     *  in IDF (text-only) mode. */
    virtual boost::optional<std::string> setName(const std::string& newName);
    virtual boost::optional<std::string> setName(const std::string& newName, bool checkValidity);

    /** Sets the name field to a new, unique name, if the name field exists and (is empty or 
     *  overwrite == true). Returns false otherwise. Created name is returned. */
    virtual boost::optional<std::string> createName();
    virtual boost::optional<std::string> createName(bool overwrite);

    /** Sets the field at index to value, if possible. Returns false if the value cannot be set for
     *  any reason. (Perhaps index >= numFields().) */
    virtual bool setString(unsigned index, const std::string& value);
    virtual bool setString(unsigned index, const std::string& value, bool checkValidity);

    /** Sets the field at index to value, if possible. Returns false if the value cannot be set for
     *  any reason. (Perhaps index >= numFields(), the field is not RealType, or the value is out of 
     *  bounds per IddField.properties()). */
    bool setDouble(unsigned index, double value);
    bool setDouble(unsigned index, double value, bool checkValidity);

    /** Sets the field at index to q, if possible. Returns false if the units of q cannot be converted
     *  to getUnits(index). */
    bool setQuantity (unsigned index, const Quantity q);
    bool setQuantity (unsigned index, const Quantity q, bool checkValidity);

    /** Sets the field at index to value, if possible. Returns false if the value cannot be set for
     *  any reasons. (Perhaps index >= numFields(), the field is not IntegerType, or the value is out 
     *  of bounds per IddField.properties()). */
    bool setUnsigned(unsigned index, unsigned value);
    bool setUnsigned(unsigned index, unsigned value, bool checkValidity);

    /** Sets the field at index to value, if possible. Returns false if the value cannot be set for
     *  any reasons. (Perhaps index >= numFields(), the field is not IntegerType, or the value is out 
     *  of bounds per IddField.properties()). */
    bool setInt(unsigned index, int value);
    bool setInt(unsigned index, int value, bool checkValidity);

    /** Pushes a new field onto the object, if possible. Returns false if the operation cannot
     *  be completed for any reason. */
    virtual bool pushString();
    virtual bool pushString(bool checkValidity);
    virtual bool pushString(const std::string& value);
    virtual bool pushString(const std::string& value, bool checkValidity);

    /** Pushes a new extensible group onto the object, if possible. Return value will be empty() if
     *  the operation cannot be completed for any reason. */
    IdfExtensibleGroup pushExtensibleGroup(const std::vector<std::string>& values);
    IdfExtensibleGroup pushExtensibleGroup(const std::vector<std::string>& values, bool checkValidity);

    /** Inserts a new extensible group at groupIndex, if possible. Return value will be empty() if
     *  the operation cannot be completed for any reason. */
    IdfExtensibleGroup insertExtensibleGroup(unsigned groupIndex, const std::vector<std::string>& values);
    IdfExtensibleGroup insertExtensibleGroup(unsigned groupIndex, const std::vector<std::string>& values, bool checkValidity);

    /** Pops the final extensible group from the object, if possible. Returns the popped data if 
     *  successful. Otherwise, the returned vector will be empty. Internally IdfObject might call 
     *  this function to rewind an operation that has gone bad half way through a calculation. */
    virtual std::vector<std::string> popExtensibleGroup();
    virtual std::vector<std::string> popExtensibleGroup(bool checkValidity);

    /** Erases the extensible group at groupIndex, if possible. Returns the erased data, and 
     *  preserves the relative order of the remaining data if successful. Otherwise, the returned
     *  vector will be empty. */
    std::vector<std::string> eraseExtensibleGroup(unsigned groupIndex);
    std::vector<std::string> eraseExtensibleGroup(unsigned groupIndex, bool checkValidity);

    /** Pops all \link IdfExtensibleGroup IdfExtensibleGroups\endlink from the object, if possible.
     *  Returns popped data if successful. Otherwise, the returned vector will be empty. */
    std::vector<std::vector<std::string> > clearExtensibleGroups();
    std::vector<std::vector<std::string> > clearExtensibleGroups(bool checkValidity);

    //@}
    /** @name Queries */
    //@{

    /** Returns the current number of fields in the object. */
    unsigned numFields() const;

    /** Returns the current number of non-extensible fields in the object. */
    unsigned numNonextensibleFields() const;

    /** Returns the minimum number of fields an object of this type must have to be valid. */
    unsigned minFields() const;

    /** Returns the maximum number of fields an object of this type can have and still be valid. */
    boost::optional<unsigned> maxFields() const;

    /** Returns the number of extensible groups in the object. */
    unsigned numExtensibleGroups() const;

    /** Returns the minimum number of \link ExtensibleGroup ExtensibleGroups\endlink an object of
     *  this type must have to be valid. */
    unsigned minExtensibleGroups() const;

    /** Returns the maximum number of \link ExtensibleGroup ExtensibleGroups\endlink an object of
     *  this type must have to be valid. */
    boost::optional<unsigned> maxExtensibleGroups() const;

    /** Returns true if index is in objectListFields(). */
    bool isObjectListField(unsigned index) const;

    /** Returns the indices of all fields that can point to other objects. WorkspaceObject manages
     *  all \object-list fields whose reference lists are unambiguously associated with an index-0 
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

    bool isValid(StrictnessLevel level, bool checkNames) const;

    /** Returns a ValidityReport for this IdfObject containing all errors at or below level. */
    ValidityReport validityReport(StrictnessLevel level,bool checkNames) const;

    /** Checks for equality of data (non-managedObjectList) fields. Prerequisite: iddObject()s 
     *  must be equal. For objects with handle fields, checks for equality of handle OR equality 
     *  of name. */
    bool dataFieldsEqual(const IdfObject& other) const;

    /** Checks for equality of objectListFields(). Prerequisite: iddObject()s must be 
     *  equal. */
    bool objectListFieldsEqual(const IdfObject& other) const;

    /** Returns true if for all objectListFields() that for both objects are non-empty, 
     *  the values are equal (using the same criteria as objectListFieldsEqual). 
     *  Prerequisite: iddObject()s must be equal. */
    bool objectListFieldsNonConflicting(const IdfObject& other) const;

    //@}
    /** @name Serialization */
    //@{

    /** Constructor from text. Parses text and queries the IddFactory for its IddObject. May create
     *  an invalid object. (May even be invalid at enums::Strictness level None.) */
    static std::shared_ptr<IdfObject_Impl> load(const std::string& text);

    /** Constructor from text and an explicit iddObject. May create an invalid object. (May even
     *  be invalid at enums::Strictness level None.) */
    static std::shared_ptr<IdfObject_Impl> load(const std::string& text,const IddObject& iddObject);

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

    /** Get an object that wraps this impl. */
    template<typename T>
    T getObject() const { 
      T result(std::dynamic_pointer_cast<typename T::ImplType>(
                 std::const_pointer_cast<IdfObject_Impl>(shared_from_this())));
      return result; 
    }

    //@}
    /** @name Signal Helpers */
    //@{

    /** Emits signals after batch update and error checking is complete, clears the diffs */
    virtual void emitChangeSignals();

    //@}

   signals:

    /** Emitted on any change--any field, any comment. */
    void onChange();

    /** Emitted if name field changed. */
    void onNameChange();

    /** Emitted when any dataFields() are changed (WorkspaceObject and higher), or when any field 
     *  data is changed in Idf mode. */
    void onDataChange();

   protected:

    friend class openstudio::IdfObject;

    // handle
    Handle m_handle;

    // user comment
    std::string m_comment;

    // idd object definition
    IddObject m_iddObject;

    // idf fields
    std::vector<std::string> m_fields;
    std::vector<std::string> m_fieldComments; // only populated if encounter non-empty, non-default comment

    // idf differences
    std::vector<IdfObjectDiff> m_diffs;

    // GETTER HELPERS

    std::vector<std::string> fields() const;

    std::vector<std::string> fieldComments() const;

    virtual OSOptionalQuantity getQuantityFromDouble(unsigned index, boost::optional<double> value, bool returnIP) const;
    
    virtual boost::optional<double> getDoubleFromQuantity(unsigned index, Quantity q) const;

    // QUERY HELPERS

    virtual void populateValidityReport(ValidityReport& report, bool checkNames) const;

    virtual std::vector<DataError> fieldDataIsValid(unsigned index, const StrictnessLevel& level) const;

    /** Checks Integer, Real and Choice fields. */
    virtual bool fieldDataIsCorrectType(unsigned index) const;

    virtual bool fieldIsNonnullIfRequired(unsigned index) const;
    
   private:

    IdfObject_Impl(){}

    // CONSTRUCTION HELPERS

    /** Minimal constructor from iddObject for use by IdfObject_Impl::load. */
    IdfObject_Impl(const IddObject& iddObject, bool fastName, bool minimal);

    // Resizes m_fields to m_iddObject.numFieldsInDefaultObject(). Called to make sure that 
    // IdfObject satisfies Strictness::None.
    void resizeToMinFields();

    /* Parse IdfObject text. If getIddFromFactory, will first search for the IddObject using the 
     * IddFactory, otherwise, assumes that m_iddObject was provided and is correct. (Will log 
     * warning if the names do not match.) */
    void parse(const std::string& text, bool getIddFromFactory);

    // parse fields
    void parseFields(const std::string& text);

    // GETTER AND SETTER HELPERS

    /** Set this object's IddObject to iddObject. */
    bool setIddObject(const IddObject& iddObject);

    // remove any indices that are outside m_fields' range
    UnsignedVector trimFieldIndices(const UnsignedVector& indices) const;

    // repeat indices as many times as necessary to fill out extensible groups in m_fields
    UnsignedVector repeatExtensibleIndices(const UnsignedVector& indices) const;

    // QUERY HELPERS

    bool fieldDataIsWithinBounds(unsigned index) const;

    /** Check fieldValue against bounds in iddField. */
    bool withinBounds(double fieldValue,const IddField& iddField) const;

    // configure logging
    REGISTER_LOGGER("utilities.idf.IdfObject");
  };

  typedef std::shared_ptr<IdfObject_Impl> IdfObject_ImplPtr;

  typedef std::vector<IdfObject_ImplPtr> IdfObject_ImplPtrVector;

} // detail

} // openstudio

#endif // UTILITIES_IDF_IDFOBJECT_IMPL_HPP
