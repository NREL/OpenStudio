/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef UTILITIES_DATA_ATTRIBUTE_HPP
#define UTILITIES_DATA_ATTRIBUTE_HPP

#include "../UtilitiesAPI.hpp"

#include "../core/Enum.hpp"
#include "../core/Logger.hpp"
#include "../core/Path.hpp"
#include "../core/UUID.hpp"
#include "../core/Optional.hpp"

#include <boost/optional.hpp>

#include <variant>

namespace pugi {
  class xml_document;
  class xml_node;
}

namespace openstudio {

class Attribute;

namespace detail{
  class Attribute_Impl;
  class EndUsesAttribute_Impl;
}
  // TODO: do I really need to add 'unsigned int' as a type, or can I store as int instead (we store AttributeValueType too...)
  // Note JM 2018-11-28:
  // we add std::monostate to allow the variant to be empty basically
  typedef std::variant<std::monostate, bool, double, int, unsigned, std::string, std::vector<Attribute> > OSAttributeVariant;

  std::ostream& operator<<(std::ostream& os, const OSAttributeVariant& attribute);


/** \class AttributeValueType
 *  \brief A listing of data types that can be held in an Attribute.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual macro call is:
 *  \code
OPENSTUDIO_ENUM( AttributeValueType,
  ((Boolean)(Boolean)(0))
  ((Double)(Double)(1))
//((Quantity)(Quantity)(2))  // Deprecated
//((Unit)(Unit)(3))          // Deprecated
  ((Integer)(Integer)(4))
  ((Unsigned)(Unsigned)(5))
  ((String)(String)(6))
  ((AttributeVector)(AttributeVector)(7))
);
 *  \endcode */
OPENSTUDIO_ENUM( AttributeValueType,
  ((Boolean)(Boolean)(0))
  ((Double)(Double)(1))
//((Quantity)(Quantity)(2))  // Deprecated
//((Unit)(Unit)(3))          // Deprecated
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
  OSAttributeVariant defaultValue;

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
                       OSAttributeVariant t_defaultValue);

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
                       OSAttributeVariant t_defaultValue);
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

  // AttributeValueType::Boolean
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

  // AttributeValueType::Double
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

  // AttributeValueType::Integer
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

  // AttributeValueType::Unsigned
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

  // AttributeValueType::String
  // Const char overload is needed because otherwise it'll resolve to the bool constructor (implicit conversion of pointer to bool)
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

  // AttributeValueType::AttributeVector
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


  // Destructor
  virtual ~Attribute() {}

  // constructor from xml, throws if required arguments are missing
  explicit Attribute(const pugi::xml_node& element);

  Attribute(const Attribute& other);

  Attribute clone() const;

  // TODO: keep both? Keep public?
  /// static constructor from xml
  static boost::optional<Attribute> loadFromXml(const openstudio::path& path);

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

  /** Returns true if this Attribute value has been set. */
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
    return T(std::move(impl));
  }

  /// cast to optional of type T
  template<typename T>
  boost::optional<T> optionalCast() const{
    boost::optional<T> result;
    std::shared_ptr<typename T::ImplType> impl = this->getImpl<typename T::ImplType>();
    if (impl){
      result = T(std::move(impl));
    }
    return result;
  }

 protected:

  friend std::ostream& operator<<(std::ostream& os, const Attribute& attribute);
  // TODO: remove from public API or make protected/private
  /// write object and all children to xml
  pugi::xml_document toXml() const;

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

} // openstudio

#endif // UTILITIES_DATA_ATTRIBUTE_HPP
