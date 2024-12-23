/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "Attribute.hpp"
#include "Attribute_Impl.hpp"

#include "../core/Assert.hpp"
#include "../core/Containers.hpp"
#include "../core/FilesystemHelpers.hpp"
#include "../core/StringHelpers.hpp"

#include <boost/lexical_cast.hpp>
#include <pugixml.hpp>
#include <json/json.h>

namespace openstudio {
namespace detail {

  // constructors
  Attribute_Impl::Attribute_Impl(const std::string& name, bool value, const boost::optional<std::string>& units)
    : m_uuid(createUUID()), m_versionUUID(createUUID()), m_name(name), m_valueType(AttributeValueType::Boolean), m_value(value), m_units(units) {}

  Attribute_Impl::Attribute_Impl(const openstudio::UUID& uuid, const openstudio::UUID& versionUUID, const std::string& name,
                                 const boost::optional<std::string>& displayName, bool value, const boost::optional<std::string>& units,
                                 const std::string& source)
    : m_uuid(uuid),
      m_versionUUID(versionUUID),
      m_name(name),
      m_displayName(displayName),
      m_source(source),
      m_valueType(AttributeValueType::Boolean),
      m_value(value),
      m_units(units) {}

  Attribute_Impl::Attribute_Impl(const std::string& name, double value, const boost::optional<std::string>& units)
    : m_uuid(createUUID()), m_versionUUID(createUUID()), m_name(name), m_valueType(AttributeValueType::Double), m_value(value), m_units(units) {}

  Attribute_Impl::Attribute_Impl(const openstudio::UUID& uuid, const openstudio::UUID& versionUUID, const std::string& name,
                                 const boost::optional<std::string>& displayName, double value, const boost::optional<std::string>& units,
                                 const std::string& source)
    : m_uuid(uuid),
      m_versionUUID(versionUUID),
      m_name(name),
      m_displayName(displayName),
      m_source(source),
      m_valueType(AttributeValueType::Double),
      m_value(value),
      m_units(units) {}

  Attribute_Impl::Attribute_Impl(const std::string& name, int value, const boost::optional<std::string>& units)
    : m_uuid(createUUID()), m_versionUUID(createUUID()), m_name(name), m_valueType(AttributeValueType::Integer), m_value(value), m_units(units) {}

  Attribute_Impl::Attribute_Impl(const openstudio::UUID& uuid, const openstudio::UUID& versionUUID, const std::string& name,
                                 const boost::optional<std::string>& displayName, int value, const boost::optional<std::string>& units,
                                 const std::string& source)
    : m_uuid(uuid),
      m_versionUUID(versionUUID),
      m_name(name),
      m_displayName(displayName),
      m_source(source),
      m_valueType(AttributeValueType::Integer),
      m_value(value),
      m_units(units) {}

  Attribute_Impl::Attribute_Impl(const std::string& name, unsigned value, const boost::optional<std::string>& units)
    : m_uuid(createUUID()), m_versionUUID(createUUID()), m_name(name), m_valueType(AttributeValueType::Unsigned), m_value(value), m_units(units) {}

  Attribute_Impl::Attribute_Impl(const openstudio::UUID& uuid, const openstudio::UUID& versionUUID, const std::string& name,
                                 const boost::optional<std::string>& displayName, unsigned value, const boost::optional<std::string>& units,
                                 const std::string& source)
    : m_uuid(uuid),
      m_versionUUID(versionUUID),
      m_name(name),
      m_displayName(displayName),
      m_source(source),
      m_valueType(AttributeValueType::Unsigned),
      m_value(value),
      m_units(units) {}

  Attribute_Impl::Attribute_Impl(const std::string& name, const std::string& value, const boost::optional<std::string>& units)
    : m_uuid(createUUID()),
      m_versionUUID(createUUID()),
      m_name(name),
      m_valueType(AttributeValueType::String),
      m_value(std::string(value)),
      m_units(units) {}

  Attribute_Impl::Attribute_Impl(const openstudio::UUID& uuid, const openstudio::UUID& versionUUID, const std::string& name,
                                 const boost::optional<std::string>& displayName, const std::string& value, const boost::optional<std::string>& units,
                                 const std::string& source)
    : m_uuid(uuid),
      m_versionUUID(versionUUID),
      m_name(name),
      m_displayName(displayName),
      m_source(source),
      m_valueType(AttributeValueType::String),
      m_value(value),
      m_units(units) {}

  Attribute_Impl::Attribute_Impl(const std::string& name, const std::vector<openstudio::Attribute>& value, const boost::optional<std::string>& units)
    : m_uuid(createUUID()),
      m_versionUUID(createUUID()),
      m_name(name),
      m_valueType(AttributeValueType::AttributeVector),
      m_value(value),
      m_units(units) {}

  Attribute_Impl::Attribute_Impl(const openstudio::UUID& uuid, const openstudio::UUID& versionUUID, const std::string& name,
                                 const boost::optional<std::string>& displayName, const std::vector<openstudio::Attribute>& value,
                                 const boost::optional<std::string>& units, const std::string& source)
    : m_uuid(uuid),
      m_versionUUID(versionUUID),
      m_name(name),
      m_displayName(displayName),
      m_source(source),
      m_valueType(AttributeValueType::AttributeVector),
      m_value(value),
      m_units(units) {}

  Attribute_Impl::Attribute_Impl(const pugi::xml_node& element) {  // NOLINT(misc-no-recursion)
    if (!element) {
      LOG_AND_THROW("Cannot construct Attribute from null pugi::xml_node element");
    }

    pugi::xml_node uuidElement = element.child("UUID");
    pugi::xml_node versionUUIDElement = element.child("VersionUUID");
    pugi::xml_node nameElement = element.child("Name");
    pugi::xml_node displayNameElement = element.child("DisplayName");
    pugi::xml_node sourceElement = element.child("Source");
    pugi::xml_node valueTypeElement = element.child("ValueType");
    pugi::xml_node valueElement = element.child("Value");
    pugi::xml_node unitsElement = element.child("Units");
    pugi::xml_node unitSystemElement = element.child("UnitSystem");

    if (!uuidElement) {
      LOG_AND_THROW("UUID is required and cannot be null.");
    }
    if (!versionUUIDElement) {
      LOG_AND_THROW("VersionUUID is required and cannot be null.");
    }
    if (!nameElement) {
      LOG_AND_THROW("Name is required and cannot be null.");
    }
    if (!valueTypeElement) {
      LOG_AND_THROW("ValueType is required and cannot be null.");
    }
    if (!valueElement) {
      LOG_AND_THROW("Value is required and cannot be null.");
    }

    std::string uuid = uuidElement.text().as_string();
    m_uuid = toUUID(uuid);

    std::string vuuid = versionUUIDElement.text().as_string();
    m_versionUUID = toUUID(vuuid);

    m_name = nameElement.text().as_string();

    if (displayNameElement) {
      m_displayName = displayNameElement.text().as_string();
    }

    if (sourceElement) {
      m_source = sourceElement.text().as_string();
    }

    std::string valueTypeString = valueTypeElement.text().as_string();
    // Deprecated!
    if (valueTypeString == "Unit") {
      LOG(Warn, "AttributeValueType::Unit is deprecated and is mapped to String.");
      valueTypeString = "String";
    } else if (valueTypeString == "Quantity") {
      // Deprecated!
      LOG(Warn, "AttributeValueType::Quantity is deprecated and is mapped to Double.");
      valueTypeString = "Double";
      // Could check if units is also there, but we don't really care
    }

    try {
      m_valueType = AttributeValueType(valueTypeString);
    } catch (const std::exception&) {
      LOG_AND_THROW("Couldn't create an AttributeValueType with value: [" << valueTypeString << "].");
    }

    if (unitsElement) {
      m_units = unitsElement.text().as_string();
    }

    // TODO: handle unitsSystem conversion? I think it's redundant with the information contained inside "Units" and we're no longer
    // creating quantities anyways
    // OptionalUnitSystem sys;
    if (unitSystemElement) {
      LOG(Warn, "UnitSystem is deprecated and isn't used, units are stored as string within the 'Units' tag instead");
      // sys = UnitSystem(unitSystemElement.text().as_string());
    }

    // Note JM 2018-12-17: We do not use pugixml's text().as_double (as_int, as_uint)
    // because it's too permissive and will return 0 even if the string isn't actually representing a number
    // So we use boost::lexical_cast instead

    if (m_valueType.value() == AttributeValueType::Boolean) {
      m_value = valueElement.text().as_bool();
    } else if (m_valueType.value() == AttributeValueType::Integer) {
      try {
        m_value = boost::lexical_cast<int>(valueElement.text().as_string());
      } catch (const boost::bad_lexical_cast&) {
        LOG_AND_THROW("Please double check your XML, you have an Attribute with a ValueType of 'Integer' "
                      << "but the Value isn't an int: [" << valueElement.text().as_string() << "].");
      }
    } else if (m_valueType.value() == AttributeValueType::Unsigned) {
      try {
        m_value = boost::lexical_cast<unsigned>(valueElement.text().as_string());
      } catch (const boost::bad_lexical_cast&) {
        LOG_AND_THROW("Please double check your XML, you have an Attribute with a ValueType of 'Unsigned' "
                      << "but the Value isn't an unsigned int: [" << valueElement.text().as_string() << "].");
      }
    } else if (m_valueType.value() == AttributeValueType::Double) {
      try {
        m_value = boost::lexical_cast<double>(valueElement.text().as_string());
      } catch (const boost::bad_lexical_cast&) {
        LOG_AND_THROW("Please double check your XML, you have an Attribute with a ValueType of 'Double' " << "but the Value isn't a double: ["
                                                                                                          << valueElement.text().as_string() << "].");
      }
    } else if (m_valueType.value() == AttributeValueType::String) {
      // Note JM 2018-12-14: Carefull not to end up with const char,
      // since that'll emplace in bool by default
      m_value.emplace<std::string>(valueElement.text().as_string());
    } else if (m_valueType.value() == AttributeValueType::AttributeVector) {
      std::vector<Attribute> children;

      for (const pugi::xml_node& childElement : valueElement.children()) {
        children.emplace_back(childElement);
      }
      m_value = children;

    } else {
      // Should never happen
      OS_ASSERT(false);
    }
  }

  Attribute_Impl::Attribute_Impl(const Attribute_Impl& other)
    : m_uuid(createUUID()),
      m_versionUUID(createUUID()),
      m_name(other.name()),
      m_displayName(other.displayName()),
      m_source(other.source()),
      m_valueType(other.valueType()),
      m_value(other.m_value),
      m_units(other.units()) {}

  openstudio::UUID Attribute_Impl::uuid() const {
    return m_uuid;
  }

  openstudio::UUID Attribute_Impl::versionUUID() const {
    return m_versionUUID;
  }

  std::string Attribute_Impl::name() const {
    return m_name;
  }

  boost::optional<std::string> Attribute_Impl::displayName(bool returnName) const {
    OptionalString result = m_displayName;
    if (!result && returnName) {
      result = name();
    }
    return result;
  }

  bool Attribute_Impl::setDisplayName(const std::string& displayName) {
    m_displayName = displayName;
    m_versionUUID = createUUID();
    return true;
  }

  void Attribute_Impl::clearDisplayName() {
    if (m_displayName) {
      m_displayName.reset();
      m_versionUUID = createUUID();
    }
  }

  std::string Attribute_Impl::source() const {
    return m_source;
  }

  void Attribute_Impl::setSource(const std::string& source) {
    m_source = source;
    m_versionUUID = createUUID();
  }

  void Attribute_Impl::clearSource() {
    if (!m_source.empty()) {
      m_source.clear();
      m_versionUUID = createUUID();
    }
  }

  AttributeValueType Attribute_Impl::valueType() const {
    return m_valueType;
  }

  bool Attribute_Impl::hasValue() const {
    return (m_value.index() != 0);
  }

  bool Attribute_Impl::valueAsBoolean() const {
    if (!hasValue() || m_valueType != AttributeValueType::Boolean) {
      LOG_AND_THROW("Cannot convert attribute '" << name() << "' of type " << valueType().valueDescription() << " to Boolean.");
    }
    // Note JM 2019-05-17: This is functionally equivalent to `std::get<bool>(m_value)` except it doesn't risk throwing
    // std::bad_variant_access which isn't available on mac prior to 10.14
    // No need to check if get_if succeeds because we checked the type above
    return *(std::get_if<bool>(&m_value));
  }

  void Attribute_Impl::setValue(bool value) {
    if (m_valueType != AttributeValueType::Boolean) {
      LOG_AND_THROW("Attribute is '" << name() << "' is of type " << valueType().valueDescription() << ", not Boolean.");
    }
    m_value = value;
    m_versionUUID = createUUID();
  }

  int Attribute_Impl::valueAsInteger() const {
    if (!hasValue() || m_valueType != AttributeValueType::Integer) {
      LOG_AND_THROW("Cannot convert attribute '" << name() << "' of type " << valueType().valueDescription() << " to Integer.");
    }
    return *(std::get_if<int>(&m_value));
  }

  void Attribute_Impl::setValue(int value) {
    if (m_valueType != AttributeValueType::Integer) {
      LOG_AND_THROW("Attribute is '" << name() << "' is of type " << valueType().valueDescription() << ", not Integer.");
    }
    m_value = value;
    m_versionUUID = createUUID();
  }

  unsigned Attribute_Impl::valueAsUnsigned() const {
    if (!hasValue() || m_valueType != AttributeValueType::Unsigned) {
      LOG_AND_THROW("Cannot convert attribute '" << name() << "' of type " << valueType().valueDescription() << " to Unsigned.");
    }
    return *(std::get_if<unsigned>(&m_value));
  }

  void Attribute_Impl::setValue(unsigned value) {
    if (m_valueType != AttributeValueType::Unsigned) {
      LOG_AND_THROW("Attribute is '" << name() << "' is of type " << valueType().valueDescription() << ", not Unsigned.");
    }
    m_value = value;
    m_versionUUID = createUUID();
  }

  double Attribute_Impl::valueAsDouble() const {
    if (!hasValue() || m_valueType != AttributeValueType::Double) {
      LOG_AND_THROW("Cannot convert attribute '" << name() << "' of type " << valueType().valueDescription() << " to Double.");
    }
    return *(std::get_if<double>(&m_value));
  }

  void Attribute_Impl::setValue(double value) {
    if (m_valueType != AttributeValueType::Double) {
      LOG_AND_THROW("Attribute is '" << name() << "' is of type " << valueType().valueDescription() << ", not Double.");
    }
    m_value = value;
    m_versionUUID = createUUID();
  }

  std::string Attribute_Impl::valueAsString() const {
    if (!hasValue() || m_valueType != AttributeValueType::String) {
      LOG_AND_THROW("Cannot convert attribute '" << name() << "' of type " << valueType().valueDescription() << " to String.");
    }
    return *(std::get_if<std::string>(&m_value));
  }

  void Attribute_Impl::setValue(const char* value) {
    if (m_valueType != AttributeValueType::String) {
      LOG_AND_THROW("Attribute is '" << name() << "' is of type " << valueType().valueDescription() << ", not String.");
    }
    m_value = std::string(value);
    m_versionUUID = createUUID();
  }

  void Attribute_Impl::setValue(const std::string& value) {
    if (m_valueType != AttributeValueType::String) {
      LOG_AND_THROW("Attribute is '" << name() << "' is of type " << valueType().valueDescription() << ", not String.");
    }
    m_value = value;
    m_versionUUID = createUUID();
  }

  std::vector<Attribute> Attribute_Impl::valueAsAttributeVector() const {
    if (!hasValue() || m_valueType != AttributeValueType::AttributeVector) {
      LOG_AND_THROW("Cannot convert attribute '" << name() << "' of type " << valueType().valueDescription() << " to AttributeVector.");
    }
    return *(std::get_if<std::vector<Attribute>>(&m_value));
  }

  void Attribute_Impl::setValue(const std::vector<Attribute>& value) {
    if (m_valueType != AttributeValueType::AttributeVector) {
      LOG_AND_THROW("Attribute is '" << name() << "' is of type " << valueType().valueDescription() << ", not AttributeVector.");
    }
    m_value = value;
    m_versionUUID = createUUID();
  }

  struct FindChildByName
  {
    FindChildByName(const std::string& name) : m_name(name) {}

    bool operator()(const Attribute& attribute) const {
      return m_name == attribute.name();
    }

    std::string m_name;
  };

  boost::optional<Attribute> Attribute_Impl::findChildByName(const std::string& name) const {
    boost::optional<Attribute> result;
    if (hasValue() && m_valueType == AttributeValueType::AttributeVector) {
      std::vector<Attribute> children = this->valueAsAttributeVector();
      auto it = std::find_if(children.begin(), children.end(), FindChildByName(name));
      if (it != children.end()) {
        result = *it;
      }
    }
    return result;
  }

  boost::optional<std::string> Attribute_Impl::units() const {
    return m_units;
  }

  bool Attribute_Impl::setUnits(const std::string& units) {
    m_units = units;
    m_versionUUID = createUUID();
    return true;
  }

  std::string Attribute_Impl::toString() const {
    std::stringstream ss;

    // We use std::visit, filtering out the case where it's monostate
    // Aside from monostate, every possible type is streamable
    // expect AttributeVector that needs a special treatment
    //std::visit(
    //[this, &ss](const auto& val) {
    //// Needed to properly compare the types
    //using T = std::remove_cv_t<std::remove_reference_t<decltype(val)>>;

    //// If it's a vector of attributes
    //if constexpr (std::is_same_v<T, openstudio::Attribute>) {
    //// Call toXml() and save to the stringstream
    //this->toXml().save(ss, "  ");

    //// Otherwise, if anything else but monostate, it's a simple type
    //// and we print the value as is
    //} else if constexpr (!std::is_same_v<T, std::monostate>) {
    //ss << std::boolalpha << val;
    //}
    //},
    //this->m_value);

    // Note JM 2019-05-17: std::visit is problematic on mac below 10.14, because it might throw std::bad_variant_access
    // So we don't use it here. Same with std::get, so we use get_if instead
    if (const auto* p = std::get_if<bool>(&m_value)) {
      ss << std::boolalpha << *p;
    } else if (const auto* p = std::get_if<double>(&m_value)) {
      ss << *p;
    } else if (const auto* p = std::get_if<int>(&m_value)) {
      ss << *p;
    } else if (const auto* p = std::get_if<unsigned>(&m_value)) {
      ss << *p;
    } else if (const auto* p = std::get_if<std::string>(&m_value)) {
      ss << *p;
    } else if (std::get_if<std::vector<Attribute>>(&m_value)) {
      // Call toXml() and save to the stringstream
      this->toXml().save(ss, "  ");
    }

    return ss.str();
  }

  pugi::xml_document Attribute_Impl::toXml() const {  // NOLINT(misc-no-recursion)
    pugi::xml_document doc;
    pugi::xml_node element = doc.append_child("Attribute");
    this->writeValues(element);
    return doc;
  }

  bool Attribute_Impl::operator==(const Attribute& other) const {  // NOLINT(misc-no-recursion)
    bool result = false;

    AttributeValueType thisValueType = this->valueType();
    AttributeValueType otherValueType = other.valueType();

    // std::variant operator== will start by comparing indexes (=same type),
    // and if that works, compares std::get<i>(v) == std::get<i>(w), but that must be returning a type converting to bool
    // Which is true except for AttributeVector, and probably undefined for std::monostate

    // No need to proceed with value comparison if types aren't the same,
    // and they must either have both a value or none should have one
    if ((thisValueType == otherValueType) && (this->hasValue() == other.hasValue())) {

      // If there's a value to compare, let's do this
      if (this->hasValue()) {
        if (thisValueType == AttributeValueType::AttributeVector) {
          std::vector<Attribute> thisAttributes = this->valueAsAttributeVector();
          std::vector<Attribute> otherAttributes = other.valueAsAttributeVector();
          if (thisAttributes.size() == otherAttributes.size()) {
            result = true;
            for (unsigned i = 0; i < thisAttributes.size(); ++i) {
              if (!(result && (thisAttributes[i] == otherAttributes[i]))) {
                result = false;
              }
              if (!result) {
                break;
              }
            }
          }

        } else {
          OSAttributeVariant otherValue = other.getImpl<Attribute_Impl>()->m_value;
          result = (m_value == otherValue);
        }
      }

      // Now we check the units
      if (m_units) {
        result = result && other.units() && (*m_units == other.units().get());
      } else {
        result = result && !other.units();
      }
    }

    return result;
  }

  void Attribute_Impl::writeValues(pugi::xml_node& element) const {  // NOLINT(misc-no-recursion)
    pugi::xml_node subElement = element.append_child("UUID");
    subElement.text().set(openstudio::toString(m_uuid).c_str());

    subElement = element.append_child("VersionUUID");
    subElement.text().set(openstudio::toString(m_versionUUID).c_str());

    subElement = element.append_child("Name");
    subElement.text().set(m_name.c_str());

    if (m_displayName) {
      subElement = element.append_child("DisplayName");
      subElement.text().set(m_displayName.get().c_str());
    }

    if (!m_source.empty()) {
      subElement = element.append_child("Source");
      subElement.text().set(m_source.c_str());
    }

    subElement = element.append_child("ValueType");
    subElement.text().set(m_valueType.valueName().c_str());

    subElement = element.append_child("Value");
    switch (m_valueType.value()) {
      case AttributeValueType::Boolean:
        subElement.text().set(this->valueAsBoolean());
        break;
      case AttributeValueType::Integer:
        subElement.text().set(this->valueAsInteger());
        break;
      case AttributeValueType::Unsigned:
        subElement.text().set(this->valueAsUnsigned());
        break;
      case AttributeValueType::Double:
        subElement.text().set(this->valueAsDouble());
        //temp = boost::regex_replace(openstudio::toString(QString::number(this->valueAsDouble(),
        //'G',
        //std::numeric_limits<double>::digits10)),
        //boost::regex("\\+"),"");
        break;
      case AttributeValueType::String:
        subElement.text().set(this->valueAsString().c_str());
        break;
      case AttributeValueType::AttributeVector:
        for (const Attribute& attribute : this->valueAsAttributeVector()) {
          // pugi::xml_node subSubElement = subElement.append_child("Attribute");
          pugi::xml_document tempdoc = attribute.toXml();
          // Cannot append_move, since this is limited to within the same tree (allow_insert_child wild return false)
          subElement.append_copy(tempdoc.document_element());
        }
        break;
      default:
        OS_ASSERT(false);
        break;
    }

    if (m_units) {
      subElement = element.append_child("Units");
      subElement.text().set(m_units.get().c_str());
    }
  }

  Json::Value Attribute_Impl::toJSON(bool short_version) const {  // NOLINT(misc-no-recursion)

    Json::Value root;
    root["name"] = m_name;
    if (auto displayName_ = displayName(false)) {
      root["display_name"] = std::move(*displayName_);
    }
    // root["display_name"] = displayName(true).get();

    if (!short_version) {
      root["uuid"] = openstudio::removeBraces(m_uuid);
      root["versionuuid"] = openstudio::removeBraces(m_versionUUID);

      if (!m_source.empty()) {
        root["source"] = m_source;
      }
    }

    // TODO: do we really need the lowercase?
    // root["datatype"] = ascii_to_lower_copy(m_valueType.valueName());

    if (m_valueType == AttributeValueType::Boolean) {
      root["value"] = valueAsBoolean();
      root["datatype"] = "boolean";
    } else if (m_valueType == AttributeValueType::Double) {
      root["value"] = valueAsDouble();
      root["datatype"] = "float";
    } else if (m_valueType == AttributeValueType::Integer) {
      root["value"] = valueAsInteger();
      root["datatype"] = "integer";
    } else if (m_valueType == AttributeValueType::Unsigned) {
      root["value"] = valueAsUnsigned();
      root["datatype"] = "unsigned";
    } else if (m_valueType == AttributeValueType::String) {
      root["value"] = valueAsString();
      root["datatype"] = "string";
    } else if (m_valueType == AttributeValueType::AttributeVector) {
      auto& subElement = root["value"];
      root["datatype"] = "attributevector";
      for (const Attribute& attribute : this->valueAsAttributeVector()) {
        subElement.append(attribute.toJSON());
      }
    } else {
      OS_ASSERT(false);
    }

    if (m_units) {
      root["units"] = *m_units;
    }
    return root;
  }

  // helper constant for the visitor below so we static assert we didn't miss a type
  template <class>
  inline constexpr bool always_false_v = false;

#if 0
  // helper type for visitor
  template <class... Ts>
  struct overloaded : Ts...
  {
    using Ts::operator()...;
  };
  // explicit deduction guide (not needed as of C++20)
  template <class... Ts>
  overloaded(Ts...) -> overloaded<Ts...>;

  Json::Value Attribute_Impl::valueAsJSON() const {
    return std::visit(overloaded{//
                                 [](std::monostate) { return Json::Value{Json::nullValue}; },
                                 [](const std::vector<Attribute>& arg) {
                                   Json::Value subElement(Json::arrayValue);
                                   for (const Attribute& attribute : arg) {
                                     subElement.append(attribute.toJSON());
                                   }
                                   return subElement;
                                 },
                                 [](auto arg) { return Json::Value(arg); }},
                      m_value);
  }
#endif

  Json::Value Attribute_Impl::valueAsJSON() const {  // NOLINT(misc-no-recursion)
    return std::visit(
      [](auto&& arg) -> Json::Value {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::monostate>) {
          return Json::nullValue;
        } else if constexpr (std::is_same_v<T, bool>) {  // NOLINT(bugprone-branch-clone)
          return arg;
        } else if constexpr (std::is_same_v<T, double>) {
          return arg;
        } else if constexpr (std::is_same_v<T, int>) {
          return arg;
          // cppcheck-suppress identicalConditionAfterEarlyExit
          // cppcheck-suppress multiCondition
        } else if constexpr (std::is_same_v<T, unsigned int>) {
          return arg;
        } else if constexpr (std::is_same_v<T, std::string>) {
          return arg;
        } else if constexpr (std::is_same_v<T, std::vector<Attribute>>) {
          Json::Value subElement(Json::arrayValue);
          for (const Attribute& attribute : arg) {
            subElement.append(attribute.valueAsJSON());
          }
          return subElement;
        } else {
          static_assert(always_false_v<T>, "non-exhaustive visitor!");
        }
      },
      m_value);
  }

}  // namespace detail

// For debug purposes only
std::ostream& operator<<(std::ostream& os, const OSAttributeVariant& attributeVariant) {

  // We use std::visit, filtering out the case where it's monostate
  // Aside from monostate, every possible type is streamable
  // expect AttributeVector that needs a special treatment
  //std::visit(
  //[&os](const auto& val) {
  //// Needed to properly compare the types
  //using T = std::remove_cv_t<std::remove_reference_t<decltype(val)> >;

  //// If it's a vector of attributes
  //if constexpr (std::is_same_v<T, std::vector<openstudio::Attribute>>) {
  //// Will end up calling toXml
  //os << val;
  //// Otherwise, if anything else but monostate
  //} else if constexpr (!std::is_same_v<T, std::monostate>) {
  //os << std::boolalpha << val;
  //}
  //},
  //attributeVariant);

  // Note JM 2019-05-17: std::visit is problematic on mac below 10.14, because it might throw std::bad_variant_access
  // So we don't use it here. Same with std::get, so we use get_if instead
  if (const auto* p = std::get_if<bool>(&attributeVariant)) {
    os << std::boolalpha << *p;
  } else if (const auto* p = std::get_if<double>(&attributeVariant)) {
    os << *p;
  } else if (const auto* p = std::get_if<int>(&attributeVariant)) {
    os << *p;
  } else if (const auto* p = std::get_if<unsigned>(&attributeVariant)) {
    os << *p;
  } else if (const auto* p = std::get_if<std::string>(&attributeVariant)) {
    os << *p;
  } else if (const auto* p = std::get_if<std::vector<Attribute>>(&attributeVariant)) {
    // Will end up calling toXml
    for (const Attribute& attr : *p) {
      os << attr << '\n';
    }
  }

  return os;
}

AttributeDescription::AttributeDescription(const std::string& t_name, const std::string& t_displayName, const std::string& t_description,
                                           const AttributeValueType& t_validValueType, bool t_required)
  : name(t_name), displayName(t_displayName), description(t_description), validValueTypes(1u, t_validValueType), required(t_required) {}

AttributeDescription::AttributeDescription(const std::string& t_name, const std::string& t_displayName, const std::string& t_description,
                                           const AttributeValueType& t_validValueType, bool t_required, OSAttributeVariant t_defaultValue)
  : name(t_name),
    displayName(t_displayName),
    description(t_description),
    validValueTypes(1u, t_validValueType),
    required(t_required),
    defaultValue(t_defaultValue) {}

AttributeDescription::AttributeDescription(const std::string& t_name, const std::string& t_displayName, const std::string& t_description,
                                           const std::vector<AttributeValueType>& t_validValueTypes, bool t_required)
  : name(t_name), displayName(t_displayName), description(t_description), validValueTypes(t_validValueTypes), required(t_required) {}

AttributeDescription::AttributeDescription(const std::string& t_name, const std::string& t_displayName, const std::string& t_description,
                                           const std::vector<AttributeValueType>& t_validValueTypes, bool t_required,
                                           OSAttributeVariant t_defaultValue)
  : name(t_name),
    displayName(t_displayName),
    description(t_description),
    validValueTypes(t_validValueTypes),
    required(t_required),
    defaultValue(t_defaultValue) {}

Attribute::Attribute(const std::string& name, bool value)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(new detail::Attribute_Impl(name, value, boost::none))) {
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const std::string& name, bool value, const std::string& units)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(new detail::Attribute_Impl(name, value, units))) {
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const std::string& name, bool value, const boost::optional<std::string>& units)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(new detail::Attribute_Impl(name, value, units))) {
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const openstudio::UUID& uuid, const openstudio::UUID& versionUUID, const std::string& name,
                     const boost::optional<std::string>& displayName, bool value, const boost::optional<std::string>& units,
                     const std::string& source)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(new detail::Attribute_Impl(uuid, versionUUID, name, displayName, value, units, source))) {
  OS_ASSERT(m_impl);
}

Attribute::Attribute(const std::string& name, double value)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(new detail::Attribute_Impl(name, value, boost::none))) {
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const std::string& name, double value, const std::string& units)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(new detail::Attribute_Impl(name, value, units))) {
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const std::string& name, double value, const boost::optional<std::string>& units)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(new detail::Attribute_Impl(name, value, units))) {
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const openstudio::UUID& uuid, const openstudio::UUID& versionUUID, const std::string& name,
                     const boost::optional<std::string>& displayName, double value, const boost::optional<std::string>& units,
                     const std::string& source)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(new detail::Attribute_Impl(uuid, versionUUID, name, displayName, value, units, source))) {
  OS_ASSERT(m_impl);
}

Attribute::Attribute(const std::string& name, int value)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(new detail::Attribute_Impl(name, value, boost::none))) {
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const std::string& name, int value, const std::string& units)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(new detail::Attribute_Impl(name, value, units))) {
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const std::string& name, int value, const boost::optional<std::string>& units)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(new detail::Attribute_Impl(name, value, units))) {
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const openstudio::UUID& uuid, const openstudio::UUID& versionUUID, const std::string& name,
                     const boost::optional<std::string>& displayName, int value, const boost::optional<std::string>& units, const std::string& source)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(new detail::Attribute_Impl(uuid, versionUUID, name, displayName, value, units, source))) {
  OS_ASSERT(m_impl);
}

Attribute::Attribute(const std::string& name, unsigned value)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(new detail::Attribute_Impl(name, value, boost::none))) {
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const std::string& name, unsigned value, const std::string& units)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(new detail::Attribute_Impl(name, value, units))) {
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const std::string& name, unsigned value, const boost::optional<std::string>& units)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(new detail::Attribute_Impl(name, value, units))) {
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const openstudio::UUID& uuid, const openstudio::UUID& versionUUID, const std::string& name,
                     const boost::optional<std::string>& displayName, unsigned value, const boost::optional<std::string>& units,
                     const std::string& source)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(new detail::Attribute_Impl(uuid, versionUUID, name, displayName, value, units, source))) {
  OS_ASSERT(m_impl);
}

Attribute::Attribute(const std::string& name, const char* value)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(new detail::Attribute_Impl(name, std::string(value), boost::none))) {
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const std::string& name, const char* value, const std::string& units)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(new detail::Attribute_Impl(name, std::string(value), units))) {
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const std::string& name, const char* value, const boost::optional<std::string>& units)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(new detail::Attribute_Impl(name, std::string(value), units))) {
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const openstudio::UUID& uuid, const openstudio::UUID& versionUUID, const std::string& name,
                     const boost::optional<std::string>& displayName, const char* value, const boost::optional<std::string>& units,
                     const std::string& source)
  : m_impl(
      std::shared_ptr<detail::Attribute_Impl>(new detail::Attribute_Impl(uuid, versionUUID, name, displayName, std::string(value), units, source))) {
  OS_ASSERT(m_impl);
}

Attribute::Attribute(const std::string& name, const std::string& value)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(new detail::Attribute_Impl(name, value, boost::none))) {
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const std::string& name, const std::string& value, const std::string& units)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(new detail::Attribute_Impl(name, value, units))) {
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const std::string& name, const std::string& value, const boost::optional<std::string>& units)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(new detail::Attribute_Impl(name, value, units))) {
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const openstudio::UUID& uuid, const openstudio::UUID& versionUUID, const std::string& name,
                     const boost::optional<std::string>& displayName, const std::string& value, const boost::optional<std::string>& units,
                     const std::string& source)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(new detail::Attribute_Impl(uuid, versionUUID, name, displayName, value, units, source))) {
  OS_ASSERT(m_impl);
}

Attribute::Attribute(const std::string& name, const std::vector<openstudio::Attribute>& value)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(new detail::Attribute_Impl(name, value, boost::none))) {
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const std::string& name, const std::vector<openstudio::Attribute>& value, const std::string& units)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(new detail::Attribute_Impl(name, value, units))) {
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const std::string& name, const std::vector<openstudio::Attribute>& value, const boost::optional<std::string>& units)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(new detail::Attribute_Impl(name, value, units))) {
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const openstudio::UUID& uuid, const openstudio::UUID& versionUUID, const std::string& name,
                     const boost::optional<std::string>& displayName, const std::vector<openstudio::Attribute>& value,
                     const boost::optional<std::string>& units, const std::string& source)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(new detail::Attribute_Impl(uuid, versionUUID, name, displayName, value, units, source))) {
  OS_ASSERT(m_impl);
}

Attribute::Attribute(const pugi::xml_node& element) : m_impl(std::shared_ptr<detail::Attribute_Impl>(new detail::Attribute_Impl(element))) {
  OS_ASSERT(m_impl);
}

Attribute::Attribute(const Attribute& other) : m_impl(other.m_impl) {
  OS_ASSERT(m_impl);
}

Attribute Attribute::clone() const {
  Attribute result(std::shared_ptr<detail::Attribute_Impl>(new detail::Attribute_Impl(*m_impl)));
  return result;
}

Attribute::Attribute(const std::shared_ptr<detail::Attribute_Impl>& impl) : m_impl(impl) {
  OS_ASSERT(m_impl);
}

boost::optional<Attribute> Attribute::loadFromXml(const openstudio::path& xmlPath) {
  boost::optional<Attribute> result;

  if (!openstudio::filesystem::exists(xmlPath)) {
    LOG(Error, "'" << xmlPath << "' does not exist.");
    return result;
  }

  if (!openstudio::filesystem::is_regular_file(xmlPath)) {
    LOG(Error, "'" << xmlPath << "' cannot be opened for reading Attribute XML data.");
    return result;
  }

  pugi::xml_document attributeXML;

  openstudio::filesystem::ifstream file(xmlPath, std::ios_base::binary);
  if (file.is_open()) {
    auto success = attributeXML.load(file);
    if (!success) {
      LOG(Error, "'" << xmlPath << "' could not be read as XML data");
      return result;
    }
    file.close();
  } else {
    file.close();
    LOG(Error, "'" << xmlPath << "' could not be opened");
    return result;
  }

  try {
    result = Attribute(attributeXML.child("Attribute"));
  } catch (const std::exception&) {
    // Output any other handled error's message to the user
    // LOG(Error, "Cannot create attribute from XML: " << e.what());
  }

  return result;
}

openstudio::UUID Attribute::uuid() const {
  return m_impl->uuid();
}

openstudio::UUID Attribute::versionUUID() const {
  return m_impl->versionUUID();
}

std::string Attribute::name() const {
  return m_impl->name();
}

boost::optional<std::string> Attribute::displayName(bool returnName) const {
  return m_impl->displayName(returnName);
}

bool Attribute::setDisplayName(const std::string& displayName) {
  return m_impl->setDisplayName(displayName);
}

void Attribute::clearDisplayName() {
  m_impl->clearDisplayName();
}

std::string Attribute::source() const {
  return m_impl->source();
}

void Attribute::setSource(const std::string& source) {
  m_impl->setSource(source);
}

void Attribute::clearSource() {
  m_impl->clearSource();
}

AttributeValueType Attribute::valueType() const {
  return m_impl->valueType();
}

bool Attribute::hasValue() const {
  return m_impl->hasValue();
}

bool Attribute::valueAsBoolean() const {
  return m_impl->valueAsBoolean();
}

void Attribute::setValue(bool value) {
  m_impl->setValue(value);
}

int Attribute::valueAsInteger() const {
  return m_impl->valueAsInteger();
}

void Attribute::setValue(int value) {
  m_impl->setValue(value);
}

unsigned Attribute::valueAsUnsigned() const {
  return m_impl->valueAsUnsigned();
}

void Attribute::setValue(unsigned value) {
  m_impl->setValue(value);
}

double Attribute::valueAsDouble() const {
  return m_impl->valueAsDouble();
}

void Attribute::setValue(double value) {
  m_impl->setValue(value);
}

std::string Attribute::valueAsString() const {
  return m_impl->valueAsString();
}

void Attribute::setValue(const char* value) {
  m_impl->setValue(value);
}

void Attribute::setValue(const std::string& value) {
  m_impl->setValue(value);
}

std::vector<Attribute> Attribute::valueAsAttributeVector() const {
  return m_impl->valueAsAttributeVector();
}

void Attribute::setValue(const std::vector<Attribute>& value) {
  m_impl->setValue(value);
}

boost::optional<Attribute> Attribute::findChildByName(const std::string& name) const {
  return m_impl->findChildByName(name);
}

boost::optional<std::string> Attribute::units() const {
  return m_impl->units();
}

bool Attribute::setUnits(const std::string& units) {
  return m_impl->setUnits(units);
}

std::string Attribute::toString() const {
  return m_impl->toString();
}

// Protected
pugi::xml_document Attribute::toXml() const {  // NOLINT(misc-no-recursion)
  return m_impl->toXml();
}

Json::Value Attribute::toJSON(bool short_version) const {  // NOLINT(misc-no-recursion)
  return m_impl->toJSON(short_version);
}

std::string Attribute::toJSONString() const {
  return toJSON().toStyledString();
}

Json::Value Attribute::valueAsJSON() const {
  return m_impl->valueAsJSON();
}

bool Attribute::saveToXml(const openstudio::path& path) const {
  bool result = false;

  try {
    openstudio::filesystem::ofstream file(path);
    this->toXml().save(file, "  ");
    file.close();
    result = true;
  } catch (const std::exception& e) {
    LOG_FREE(Error, "openstudio.Attribute", "Error saving to XML: " << e.what());
  }

  return result;
}

bool Attribute::operator==(const Attribute& other) const {  // NOLINT(misc-no-recursion)
  return m_impl->operator==(other);
}

std::ostream& operator<<(std::ostream& os, const Attribute& attribute) {
  pugi::xml_document doc = attribute.toXml();
  doc.save(os, "  ");
  return os;
}

Attribute createAttributeFromVector(const std::string& name, const std::vector<int>& value) {
  AttributeVector valueAsAttributes;
  valueAsAttributes.reserve(value.size());
  for (int v : value) {
    valueAsAttributes.emplace_back(std::string(), v);
  }
  return Attribute(name, valueAsAttributes);
}

Attribute createAttributeFromVector(const std::string& name, const std::vector<double>& value) {
  AttributeVector valueAsAttributes;
  valueAsAttributes.reserve(value.size());
  for (double v : value) {
    valueAsAttributes.emplace_back(std::string(), v);
  }
  return Attribute(name, valueAsAttributes);
}

std::vector<int> getIntVectorFromAttribute(const Attribute& attribute) {
  IntVector result;
  if (attribute.valueType() != AttributeValueType::AttributeVector) {
    LOG_FREE_AND_THROW("openstudio.Attribute", "Attribute must be of type AttributeVector for this function to be applicable.");
  }
  AttributeVector attributes = attribute.valueAsAttributeVector();
  for (const Attribute& a : attributes) {
    result.push_back(a.valueAsInteger());
  }
  return result;
}

std::vector<double> getDoubleVectorFromAttribute(const Attribute& attribute) {
  DoubleVector result;
  if (attribute.valueType() != AttributeValueType::AttributeVector) {
    LOG_FREE_AND_THROW("openstudio.Attribute", "Attribute must be of type AttributeVector for this function to be applicable.");
  }
  AttributeVector attributes = attribute.valueAsAttributeVector();
  for (const Attribute& a : attributes) {
    result.push_back(a.valueAsDouble());
  }
  return result;
}

}  // namespace openstudio
