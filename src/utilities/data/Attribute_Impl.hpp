/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_DATA_ATTRIBUTE_IMPL_HPP
#define UTILITIES_DATA_ATTRIBUTE_IMPL_HPP

#include "../UtilitiesAPI.hpp"
#include "Attribute.hpp"

#include "../core/Optional.hpp"
#include "../core/String.hpp"
#include "../core/Logger.hpp"
#include "../core/UUID.hpp"

namespace pugi {
class xml_document;
class xml_node;
}  // namespace pugi

namespace Json {
class Value;
}

namespace openstudio {
namespace detail {

  class UTILITIES_API Attribute_Impl : public std::enable_shared_from_this<Attribute_Impl>
  {

   public:
    // Note JM 2018-12-04: As far as a I can tell, the only Ctor actually uses are the simple ones,
    // name, value, optional<string> units

    /// constructors
    Attribute_Impl(const std::string& name, bool value, const boost::optional<std::string>& units);
    Attribute_Impl(const openstudio::UUID& uuid, const openstudio::UUID& versionUUID, const std::string& name,
                   const boost::optional<std::string>& displayName, bool value, const boost::optional<std::string>& units,
                   const std::string& source = std::string());

    Attribute_Impl(const std::string& name, double value, const boost::optional<std::string>& units);
    Attribute_Impl(const openstudio::UUID& uuid, const openstudio::UUID& versionUUID, const std::string& name,
                   const boost::optional<std::string>& displayName, double value, const boost::optional<std::string>& units,
                   const std::string& source = std::string());

    Attribute_Impl(const std::string& name, int value, const boost::optional<std::string>& units);
    Attribute_Impl(const openstudio::UUID& uuid, const openstudio::UUID& versionUUID, const std::string& name,
                   const boost::optional<std::string>& displayName, int value, const boost::optional<std::string>& units,
                   const std::string& source = std::string());

    Attribute_Impl(const std::string& name, unsigned value, const boost::optional<std::string>& units);
    Attribute_Impl(const openstudio::UUID& uuid, const openstudio::UUID& versionUUID, const std::string& name,
                   const boost::optional<std::string>& displayName, unsigned value, const boost::optional<std::string>& units,
                   const std::string& source = std::string());

    Attribute_Impl(const std::string& name, const std::string& value, const boost::optional<std::string>& units);
    Attribute_Impl(const openstudio::UUID& uuid, const openstudio::UUID& versionUUID, const std::string& name,
                   const boost::optional<std::string>& displayName, const std::string& value, const boost::optional<std::string>& units,
                   const std::string& source = std::string());

    Attribute_Impl(const std::string& name, const std::vector<openstudio::Attribute>& value, const boost::optional<std::string>& units);
    Attribute_Impl(const openstudio::UUID& uuid, const openstudio::UUID& versionUUID, const std::string& name,
                   const boost::optional<std::string>& displayName, const std::vector<openstudio::Attribute>& value,
                   const boost::optional<std::string>& units, const std::string& source = std::string());

    // constructor from xml, throws if required arguments are missing
    Attribute_Impl(const pugi::xml_node& element);

    Attribute_Impl(const Attribute_Impl& other);

    // Destructor
    virtual ~Attribute_Impl() = default;

    openstudio::UUID uuid() const;

    openstudio::UUID versionUUID() const;

    /// get the name
    std::string name() const;

    // ETH@20140414 - displayName should return a std::string (with returnName=true behavior),
    // and the return type of setDisplayName should be void.

    /// get the display name. if returnName and the display name is empty, will return
    /// name() instead.
    boost::optional<std::string> displayName(bool returnName = false) const;

    /// set the display name
    bool setDisplayName(const std::string& displayName);

    /// clear the display name
    void clearDisplayName();

    /// get the (optional) data source
    std::string source() const;

    /// set the data source
    void setSource(const std::string& source);

    /// clear the data source
    void clearSource();

    /// get the attribute value type
    AttributeValueType valueType() const;

    // Check if it has a value set
    bool hasValue() const;

    /// get value as a bool
    bool valueAsBoolean() const;

    /// set value. throws if wrong type.
    void setValue(bool value);

    /// get value as int
    int valueAsInteger() const;

    /// set value. throws if wrong type.
    void setValue(int value);

    /// get value as unsigned
    unsigned valueAsUnsigned() const;

    /// set value. throws if wrong type.
    void setValue(unsigned value);

    /// get value as double
    double valueAsDouble() const;

    /// set value. throws if wrong type.
    void setValue(double value);

    /// get value as string
    std::string valueAsString() const;

    /// set value. throws if wrong type.
    void setValue(const char* value);

    /// set value. throws if wrong type.
    void setValue(const std::string& value);

    /// get value as attribute vector
    std::vector<Attribute> valueAsAttributeVector() const;

    /// set value. throws if wrong type.
    void setValue(const std::vector<Attribute>& value);

    /// find child attribute by name
    boost::optional<Attribute> findChildByName(const std::string& name) const;

    /// get the units
    boost::optional<std::string> units() const;

    /// set the units
    bool setUnits(const std::string& units);

    /** If attribute is a simple type (non-vector), prints attribute's value to std::string as
      *  simply as possible. Numeric types print in high precision. Attribute vectors are printed
      *  as XML. */
    std::string toString() const;

    /// write object and all children to xml
    pugi::xml_document toXml() const;

    // short_version: if false, will include the uuid, versionuuid and source (if any)
    Json::Value toJSON(bool short_version) const;

    // Just the value as a Json::Value
    Json::Value valueAsJSON() const;

    /// comparison
    bool operator==(const Attribute& other) const;

   protected:
    friend class openstudio::Attribute;

    /// write values to an xml element
    /// override in derived classes
    virtual void writeValues(pugi::xml_node& element) const;

   private:
    REGISTER_LOGGER("openstudio.Attribute");

    openstudio::UUID m_uuid;
    openstudio::UUID m_versionUUID;
    std::string m_name;
    boost::optional<std::string> m_displayName;
    std::string m_source;
    AttributeValueType m_valueType;
    OSAttributeVariant m_value;
    boost::optional<std::string> m_units;
  };

}  // namespace detail
}  // namespace openstudio

#endif  // UTILITIES_DATA_ATTRIBUTE_IMPL_HPP
