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

#ifndef UTILITIES_DATA_ATTRIBUTE_HPP
#define UTILITIES_DATA_ATTRIBUTE_HPP

#include "../UtilitiesAPI.hpp"

#include "../core/Enum.hpp"
#include "../core/Logger.hpp"
#include "../core/Path.hpp"
#include "../core/UUID.hpp"
#include "../core/Optional.hpp"

#include <boost/optional.hpp>

#include <QVariant>
#include <QMetaType>

class QDomDocument;
class QDomElement;

namespace openstudio {

class Unit;
class Quantity;
class OSOptionalQuantity;

namespace detail{
  class Attribute_Impl;
  class EndUsesAttribute_Impl;
}

/** \class AttributeValueType
 *  \brief A listing of data types that can be held in an Attribute. 
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual macro 
 *  call is: 
 *  \code
OPENSTUDIO_ENUM( AttributeValueType,
  ((Boolean)(Boolean)(0))
  ((Double)(Double)(1))
  ((Quantity)(Quantity)(2))
  ((Unit)(Unit)(3))
  ((Integer)(Integer)(4))
  ((Unsigned)(Unsigned)(5))
  ((String)(String)(6))
  ((AttributeVector)(AttributeVector)(7))
);
 *  \endcode */
OPENSTUDIO_ENUM( AttributeValueType,
  ((Boolean)(Boolean)(0))
  ((Double)(Double)(1))
  ((Quantity)(Quantity)(2))
  ((Unit)(Unit)(3))
  ((Integer)(Integer)(4))
  ((Unsigned)(Unsigned)(5))
  ((String)(String)(6))
  ((AttributeVector)(AttributeVector)(7))
);

/** AttributeDescription describes what an acceptable Attribute is in a given context. At this 
 *  time, it is primarily used in openstudio::analysis::GenericUncertaintyDescription. Note
 *  that AttributeDescription was designed to provide the information needed by a UI developer 
 *  to properly prompt a user for the data needed to create a new Attribute. */
struct UTILITIES_API AttributeDescription {
  std::string name;
  std::string displayName;
  std::string description;
  std::vector<AttributeValueType> validValueTypes;
  bool required;
  QVariant defaultValue;

  AttributeDescription(const std::string& t_name,
                       const std::string& t_displayName,
                       const std::string& t_description,
                       const AttributeValueType& t_validValueType,
                       bool t_required);

  AttributeDescription(const std::string& t_name,
                       const std::string& t_displayName,
                       const std::string& t_description,
                       const AttributeValueType& t_validValueType,
                       bool t_required,
                       QVariant t_defaultValue);

  AttributeDescription(const std::string& t_name,
                       const std::string& t_displayName,
                       const std::string& t_description,
                       const std::vector<AttributeValueType>& t_validValueTypes,
                       bool t_required);

  AttributeDescription(const std::string& t_name,
                       const std::string& t_displayName,
                       const std::string& t_description,
                       const std::vector<AttributeValueType>& t_validValueTypes,
                       bool t_required,
                       QVariant t_defaultValue);
};

/** \relates AttributeDescription */
typedef boost::optional<AttributeDescription> OptionalAttributeDescription;

/** \relates AttributeDescription */
typedef std::vector<AttributeDescription> AttributeDescriptionVector;

/** An attribute is a name value pair where the value can be a bool, double, int,
 *   string, or vector of other attributes.  The ability for an attribute value
 *   to be a vector of other attributes allows for flexible, hierarchical structures.
 *   Our convention is that attributes which correspond to plain old data (int, string, etc)
 *   are named in lower camel case, this corresponds to a member or variable name.  Attributes
 *   of vector type are named in upper camel case to correspond to a class or struct name.
 */
class UTILITIES_API Attribute {
 public:
  typedef detail::Attribute_Impl ImplType;

  /// constructors
  Attribute(const std::string& name, bool value);
  Attribute(const std::string& name, bool value, const std::string& units);
  Attribute(const std::string& name, bool value, const boost::optional<std::string>& units);
  Attribute(const openstudio::UUID& uuid,
            const openstudio::UUID& versionUUID,
            const std::string& name,
            const boost::optional<std::string>& displayName,
            bool value,
            const boost::optional<std::string>& units,
            const std::string& source = std::string());

  Attribute(const std::string& name, double value);
  Attribute(const std::string& name, double value, const std::string& units);
  Attribute(const std::string& name, double value, const boost::optional<std::string>& units);
  Attribute(const openstudio::UUID& uuid,
            const openstudio::UUID& versionUUID,
            const std::string& name,
            const boost::optional<std::string>& displayName,
            double value,
            const boost::optional<std::string>& units,
            const std::string& source = std::string());

  /** Upon construction, will be of type Quantity or Unit, as appropriate. */
  Attribute(const std::string& name, const OSOptionalQuantity& value);

  Attribute(const std::string& name, const Quantity& value);
  Attribute(const openstudio::UUID& uuid,
            const openstudio::UUID& versionUUID,
            const std::string& name,
            const boost::optional<std::string>& displayName,
            const Quantity& value,
            const std::string& source = std::string());

  Attribute(const std::string& name, const Unit& value);
  Attribute(const openstudio::UUID& uuid,
            const openstudio::UUID& versionUUID,
            const std::string& name,
            const boost::optional<std::string>& displayName,
            const Unit& value,
            const std::string& source = std::string());

  Attribute(const std::string& name, int value);
  Attribute(const std::string& name, int value, const std::string& units);
  Attribute(const std::string& name, int value, const boost::optional<std::string>& units);
  Attribute(const openstudio::UUID& uuid,
            const openstudio::UUID& versionUUID,
            const std::string& name,
            const boost::optional<std::string>& displayName,
            int value,
            const boost::optional<std::string>& units,
            const std::string& source = std::string());

  Attribute(const std::string& name, unsigned value);
  Attribute(const std::string& name, unsigned value, const std::string& units);
  Attribute(const std::string& name, unsigned value, const boost::optional<std::string>& units);
  Attribute(const openstudio::UUID& uuid,
            const openstudio::UUID& versionUUID,
            const std::string& name,
            const boost::optional<std::string>& displayName,
            unsigned value,
            const boost::optional<std::string>& units,
            const std::string& source = std::string());

  Attribute(const std::string& name, const char* value);
  Attribute(const std::string& name, const char* value, const std::string& units);
  Attribute(const std::string& name, const char* value, const boost::optional<std::string>& units);
  Attribute(const openstudio::UUID& uuid,
            const openstudio::UUID& versionUUID,
            const std::string& name,
            const boost::optional<std::string>& displayName,
            const char* value,
            const boost::optional<std::string>& units,
            const std::string& source = std::string());

  Attribute(const std::string& name, const std::string& value);
  Attribute(const std::string& name, const std::string& value, const std::string& units);
  Attribute(const std::string& name, const std::string& value, const boost::optional<std::string>& units);
  Attribute(const openstudio::UUID& uuid,
            const openstudio::UUID& versionUUID,
            const std::string& name,
            const boost::optional<std::string>& displayName,
            const std::string& value,
            const boost::optional<std::string>& units,
            const std::string& source = std::string());

  Attribute(const std::string& name, const std::vector<openstudio::Attribute>& value);
  Attribute(const std::string& name, const std::vector<openstudio::Attribute>& value, const std::string& units);
  Attribute(const std::string& name, const std::vector<openstudio::Attribute>& value, const boost::optional<std::string>& units);
  Attribute(const openstudio::UUID& uuid,
            const openstudio::UUID& versionUUID,
            const std::string& name,
            const boost::optional<std::string>& displayName,
            const std::vector<openstudio::Attribute>& value,
            const boost::optional<std::string>& units,
            const std::string& source = std::string());

  explicit Attribute(const QDomElement& element);

  Attribute(const Attribute& other);

  Attribute clone() const;

  /// static constructor from QVariant
  static boost::optional<Attribute> fromQVariant(const std::string& name, const QVariant& value, const boost::optional<std::string>& units = boost::none);

  /// static constructor from xml
  static boost::optional<Attribute> loadFromXml(const openstudio::path& path);

  /// static constructor from loaded xml
  static boost::optional<Attribute> loadFromXml(const QDomDocument& doc);

  openstudio::UUID uuid() const;

  openstudio::UUID versionUUID() const;

  /// get the name
  std::string name() const;

  /// get the display name. if returnName and the display name is empty, will return 
  /// name() instead.
  boost::optional<std::string> displayName(bool returnName=false) const;

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

  /// get value as Quantity
  Quantity valueAsQuantity() const;

  /// set value. throws if wrong type.
  void setValue(const Quantity& value);

  /// get value as Unit
  Unit valueAsUnit() const;

  /// set value. throws if wrong type.
  void setValue(const Unit& value);

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

  /// get value as qvariant
  QVariant valueAsQVariant() const;

  // set value. throws if wrong type.
  void setValue(const QVariant& value);

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
  QDomDocument toXml() const;

  /// write object and all children out as xml to path.
  bool saveToXml(const openstudio::path& path) const;

  /// comparison
  bool operator==(const Attribute& other) const;

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

 protected:

  friend class detail::Attribute_Impl;
  friend class EndUses;

  // constructor from impl
  Attribute(const std::shared_ptr<detail::Attribute_Impl>& impl);

  /// get the impl
  template<typename T>
  std::shared_ptr<T> getImpl() const
  {
    return std::dynamic_pointer_cast<T>(m_impl);
  }

 private:

  REGISTER_LOGGER("openstudio.Attribute");

  std::shared_ptr<detail::Attribute_Impl> m_impl;

};

/** \relates Attribute */
typedef boost::optional<Attribute> OptionalAttribute;

/** \relates Attribute */
typedef std::vector<Attribute> AttributeVector;

/** Prints Attribute XML to os. \relates Attribute */
UTILITIES_API std::ostream& operator<<(std::ostream& os, const Attribute& attribute);

/** \relates Attribute */
// DLM: can this be a member of Attribute?
UTILITIES_API Attribute createAttributeFromVector(const std::string& name, std::vector<int> value);

/** \relates Attribute */
// DLM: can this be a member of Attribute?
UTILITIES_API Attribute createAttributeFromVector(const std::string& name, std::vector<double> value);

/** \relates Attribute */
// DLM: can this be a member of Attribute?
UTILITIES_API std::vector<int> getIntVectorFromAttribute(const Attribute& attribute);

/** \relates Attribute */
// DLM: can this be a member of Attribute?
UTILITIES_API std::vector<double> getDoubleVectorFromAttribute(const Attribute& attribute);

/** Returns true if candidate and description have matching names and types. \relates Attribute */
// DLM: can this be a member of Attribute?
UTILITIES_API bool isConsistent(const Attribute& candidate,const AttributeDescription& description);

/** Copies description's display name over to attribute. Will return false if
 *  !isConsistent(attribute,description). \relates Attribute */
// DLM: can this be a member of Attribute?
UTILITIES_API bool prepareForDisplay(Attribute& attribute, const AttributeDescription& description);

/** Saves attributes in a flat json file. Discards uuid and version_uuid information.
 *  Recasts Unsigned attributes as Integer attributes, Quantity attributes as Double
 *  attributes with units, Unit attributes as String attributes. \relates Attribute */
UTILITIES_API bool saveJSON(const std::vector<Attribute>& attributes,
                            const openstudio::path& p,
                            bool overwrite=false);

/** Puts attributes out to os in a flat json format. Discards uuid and version_uuid
 *  information. Recasts Unsigned attributes as Integer attributes, Quantity attributes
 *  as Double attributes with units, Unit attributes as String attributes.
 *  \relates Attribute */
UTILITIES_API std::ostream& toJSON(const std::vector<Attribute>& attributes,
                                   std::ostream& os);

/** Returns attributes as a string in a flat json format. Discards uuid and version_uuid
 *  information. Recasts Unsigned attributes as Integer attributes, Quantity attributes
 *  as Double attributes with units, Unit attributes as String attributes.
 *  \relates Attribute */
UTILITIES_API std::string toJSON(const std::vector<Attribute>& attributes);

/** Deserializes the flat attribute json format. \relates Attribute */
UTILITIES_API std::vector<Attribute> toVectorOfAttribute(const openstudio::path& pathToJson);

/** Deserializes the flat attribute json format. \relates Attribute */
UTILITIES_API std::vector<Attribute> toVectorOfAttribute(std::istream& json);

/** Deserializes the flat attribute json format. \relates Attribute */
UTILITIES_API std::vector<Attribute> toVectorOfAttribute(const std::string& json);

namespace detail {
  /** Places attribute's data in a QVariant for JSON serialization. */
  UTILITIES_API QVariant toVariant(const Attribute& attribute);

  /** Deserializes json variant to Attribute. */
  UTILITIES_API Attribute toAttribute(const QVariant& variant, const VersionString& version);

  /** Places a vector of attributes' data in a flat QVariant for JSON serialization. Discards
   *  uuid and version_uuid information. Recasts Unsigned attributes as Integer attributes,
   *  Quantity attributes as Double attributes with units, Unit attributes as String attributes. */
  UTILITIES_API QVariant toVariant(const std::vector<Attribute>& attributes);

  /** Deserializes (flat) json variant to std::vector<Attribute>. */
  UTILITIES_API std::vector<Attribute> toVectorOfAttribute(const QVariant& variant, const VersionString& version);
}

} // openstudio

// declare these types so we can use them as properties
//Q_DECLARE_METATYPE(openstudio::Attribute);
Q_DECLARE_METATYPE(boost::optional<openstudio::Attribute>);
Q_DECLARE_METATYPE(std::vector<openstudio::Attribute>);

#endif // UTILITIES_DATA_ATTRIBUTE_HPP
