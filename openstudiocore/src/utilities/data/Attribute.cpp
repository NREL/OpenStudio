/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <utilities/data/Attribute.hpp>
#include <utilities/data/Attribute_Impl.hpp>

#include <utilities/core/Assert.hpp>
#include <utilities/core/Containers.hpp>
#include <utilities/core/Json.hpp>

#include <utilities/units/UnitFactory.hpp>
#include <utilities/units/Quantity.hpp>
#include <utilities/units/QuantityFactory.hpp>
#include <utilities/units/OSOptionalQuantity.hpp>

#include <boost/foreach.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>

#include <sstream>
#include <limits>

#include <QDomElement>
#include <QDomDocument>
#include <QFile>

namespace openstudio {
namespace detail{

  //int __attribute_type__ = qRegisterMetaType<openstudio::Attribute>("openstudio::Attribute");
  int __attribute_optional_type__ = qRegisterMetaType<boost::optional<openstudio::Attribute> >("boost::optional<openstudio::Attribute>");
  int __attribute_vector_type__ = qRegisterMetaType<std::vector<openstudio::Attribute> >("std::vector<openstudio::Attribute>");

  // constructors
  Attribute_Impl::Attribute_Impl(const std::string& name,
                                 bool value,
                                 const boost::optional<std::string>& units)
    : m_uuid(createUUID()),
      m_versionUUID(createUUID()),
      m_name(name),
      m_valueType(AttributeValueType::Boolean),
      m_value(value),
      m_units(units)
  {
  }

  Attribute_Impl::Attribute_Impl(const openstudio::UUID& uuid,
                                 const openstudio::UUID& versionUUID,
                                 const std::string& name,
                                 const boost::optional<std::string>& displayName,
                                 bool value,
                                 const boost::optional<std::string>& units)
    : m_uuid(uuid),
      m_versionUUID(versionUUID),
      m_name(name),
      m_displayName(displayName),
      m_valueType(AttributeValueType::Boolean),
      m_value(value),
      m_units(units)
  {
  }

  Attribute_Impl::Attribute_Impl(const std::string& name,
                                 double value,
                                 const boost::optional<std::string>& units)
    : m_uuid(createUUID()),
      m_versionUUID(createUUID()),
      m_name(name),
      m_valueType(AttributeValueType::Double),
      m_value(value),
      m_units(units)
  {
  }

  Attribute_Impl::Attribute_Impl(const openstudio::UUID& uuid,
                                 const openstudio::UUID& versionUUID,
                                 const std::string& name,
                                 const boost::optional<std::string>& displayName,
                                 double value,
                                 const boost::optional<std::string>& units)
    : m_uuid(uuid),
      m_versionUUID(versionUUID),
      m_name(name),
      m_displayName(displayName),
      m_valueType(AttributeValueType::Double),
      m_value(value),
      m_units(units)
  {
  }

  Attribute_Impl::Attribute_Impl(const std::string& name, const OSOptionalQuantity& value)
    : m_uuid(createUUID()),
      m_versionUUID(createUUID()),
      m_name(name)
  {
    if (value.isSet()) {
      m_valueType = AttributeValueType::Quantity;
      m_value = QVariant::fromValue(value.get());
    }
    else {
      m_valueType = AttributeValueType::Unit;
      m_value = QVariant::fromValue(value);
    }
  }

  Attribute_Impl::Attribute_Impl(const std::string& name, const Quantity& value)
    : m_uuid(createUUID()),
      m_versionUUID(createUUID()),
      m_name(name),
      m_valueType(AttributeValueType::Quantity),
      m_value(QVariant::fromValue(value))
  {
  }

  Attribute_Impl::Attribute_Impl(const openstudio::UUID& uuid,
                                 const openstudio::UUID& versionUUID,
                                 const std::string& name,
                                 const boost::optional<std::string>& displayName,
                                 const Quantity& value)
    : m_uuid(uuid),
      m_versionUUID(versionUUID),
      m_name(name),
      m_displayName(displayName),
      m_valueType(AttributeValueType::Quantity),
      m_value(QVariant::fromValue(value))
  {
  }

  Attribute_Impl::Attribute_Impl(const std::string& name, const Unit& value)
    : m_uuid(createUUID()),
      m_versionUUID(createUUID()),
      m_name(name),
      m_valueType(AttributeValueType::Unit),
      m_value(QVariant::fromValue(OSOptionalQuantity(value)))
  {
  }

  Attribute_Impl::Attribute_Impl(const openstudio::UUID& uuid,
                                 const openstudio::UUID& versionUUID,
                                 const std::string& name,
                                 const boost::optional<std::string>& displayName,
                                 const Unit& value)
    : m_uuid(uuid),
      m_versionUUID(versionUUID),
      m_name(name),
      m_displayName(displayName),
      m_valueType(AttributeValueType::Unit),
      m_value(QVariant::fromValue(OSOptionalQuantity(value)))
  {
  }

  Attribute_Impl::Attribute_Impl(const std::string& name,
                                 int value,
                                 const boost::optional<std::string>& units)
    : m_uuid(createUUID()),
      m_versionUUID(createUUID()),
      m_name(name),
      m_valueType(AttributeValueType::Integer),
      m_value(value),
      m_units(units)
  {
  }

  Attribute_Impl::Attribute_Impl(const openstudio::UUID& uuid,
                                 const openstudio::UUID& versionUUID,
                                 const std::string& name,
                                 const boost::optional<std::string>& displayName,
                                 int value,
                                 const boost::optional<std::string>& units)
    : m_uuid(uuid),
      m_versionUUID(versionUUID),
      m_name(name),
      m_displayName(displayName),
      m_valueType(AttributeValueType::Integer),
      m_value(value),
      m_units(units)
  {
  }

  Attribute_Impl::Attribute_Impl(const std::string& name,
                                 unsigned value,
                                 const boost::optional<std::string>& units)
    : m_uuid(createUUID()),
      m_versionUUID(createUUID()),
      m_name(name),
      m_valueType(AttributeValueType::Unsigned),
      m_value(value),
      m_units(units)
  {
  }

  Attribute_Impl::Attribute_Impl(const openstudio::UUID& uuid,
                                 const openstudio::UUID& versionUUID,
                                 const std::string& name,
                                 const boost::optional<std::string>& displayName,
                                 unsigned value,
                                 const boost::optional<std::string>& units)
    : m_uuid(uuid),
      m_versionUUID(versionUUID),
      m_name(name),
      m_displayName(displayName),
      m_valueType(AttributeValueType::Unsigned),
      m_value(value),
      m_units(units)
  {
  }

  Attribute_Impl::Attribute_Impl(const std::string& name,
                                 const char* value,
                                 const boost::optional<std::string>& units)
    : m_uuid(createUUID()),
      m_versionUUID(createUUID()),
      m_name(name),
      m_valueType(AttributeValueType::String),
      m_value(),
      m_units(units)
  {
    m_value.setValue(std::string(value));
  }

  Attribute_Impl::Attribute_Impl(const openstudio::UUID& uuid,
                                 const openstudio::UUID& versionUUID,
                                 const std::string& name,
                                 const boost::optional<std::string>& displayName,
                                 const char* value,
                                 const boost::optional<std::string>& units)
    : m_uuid(uuid),
      m_versionUUID(versionUUID),
      m_name(name),
      m_displayName(displayName),
      m_valueType(AttributeValueType::String),
      m_value(),
      m_units(units)
  {
    m_value.setValue(std::string(value));
  }

  Attribute_Impl::Attribute_Impl(const std::string& name,
                                 const std::string& value,
                                 const boost::optional<std::string>& units)
    : m_uuid(createUUID()),
      m_versionUUID(createUUID()),
      m_name(name),
      m_valueType(AttributeValueType::String),
      m_value(),
      m_units(units)
  {
    m_value.setValue(value);
  }

  Attribute_Impl::Attribute_Impl(const openstudio::UUID& uuid,
                                 const openstudio::UUID& versionUUID,
                                 const std::string& name,
                                 const boost::optional<std::string>& displayName,
                                 const std::string& value,
                                 const boost::optional<std::string>& units)
    : m_uuid(uuid),
      m_versionUUID(versionUUID),
      m_name(name),
      m_displayName(displayName),
      m_valueType(AttributeValueType::String),
      m_value(),
      m_units(units)
  {
    m_value.setValue(value);
  }

  Attribute_Impl::Attribute_Impl(const std::string& name,
                                 const std::vector<openstudio::Attribute>& value,
                                 const boost::optional<std::string>& units)
    : m_uuid(createUUID()),
      m_versionUUID(createUUID()),
      m_name(name),
      m_valueType(AttributeValueType::AttributeVector),
      m_value(),
      m_units(units)
  {
    m_value.setValue(value);
  }

  Attribute_Impl::Attribute_Impl(const openstudio::UUID& uuid,
                                 const openstudio::UUID& versionUUID,
                                 const std::string& name,
                                 const boost::optional<std::string>& displayName,
                                 const std::vector<openstudio::Attribute>& value,
                                 const boost::optional<std::string>& units)
    : m_uuid(uuid),
      m_versionUUID(versionUUID),
      m_name(name),
      m_displayName(displayName),
      m_valueType(AttributeValueType::AttributeVector),
      m_value(),
      m_units(units)
  {
    m_value.setValue(value);
  }

  Attribute_Impl::Attribute_Impl(const QDomElement& element)
  {
    if(element.isNull()){
      LOG_AND_THROW("Cannot construct Attribute from null QDomElement");
    }

    QDomElement uuidElement = element.firstChildElement(QString::fromStdString("UUID"));
    QDomElement versionUUIDElement = element.firstChildElement(QString::fromStdString("VersionUUID"));
    QDomElement nameElement = element.firstChildElement(QString::fromStdString("Name"));
    QDomElement displayNameElement = element.firstChildElement(QString::fromStdString("DisplayName"));
    QDomElement valueTypeElement = element.firstChildElement(QString::fromStdString("ValueType"));
    QDomElement valueElement = element.firstChildElement(QString::fromStdString("Value"));
    QDomElement unitsElement = element.firstChildElement(QString::fromStdString("Units"));
    QDomElement unitSystemElement = element.firstChildElement(QString::fromStdString("UnitSystem"));

    if(uuidElement.isNull()){
      LOG_AND_THROW("uuidElement is null");
    }
    if(versionUUIDElement.isNull()){
      LOG_AND_THROW("versionUUIDElement is null");
    }
    if(nameElement.isNull()){
      LOG_AND_THROW("nameElement is null");
    }
    if(valueTypeElement.isNull()){
      LOG_AND_THROW("valueTypeElement is null");
    }
    if(valueElement.isNull()){
      LOG_AND_THROW("valueElement is null");
    }

    m_uuid = toUUID(uuidElement.firstChild().nodeValue().toStdString());

    m_versionUUID = toUUID(versionUUIDElement.firstChild().nodeValue().toStdString());

    m_name = nameElement.firstChild().nodeValue().toStdString();

    if (!displayNameElement.isNull()){
      m_displayName = displayNameElement.firstChild().nodeValue().toStdString();
    }

    m_valueType = AttributeValueType(valueTypeElement.firstChild().nodeValue().toStdString());

    if (!unitsElement.isNull()) {
      m_units = unitsElement.firstChild().nodeValue().toStdString();
    }

    OptionalUnitSystem sys;
    if (!unitSystemElement.isNull()) {
      sys = UnitSystem(unitSystemElement.firstChild().nodeValue().toStdString());
    }

    std::vector<Attribute> children;
    QDomNodeList childNodes = valueElement.childNodes();

    switch(m_valueType.value()){
    case AttributeValueType::Boolean:
      if (valueElement.firstChild().nodeValue() == "true"){
        m_value = true;
      }else{
        m_value = false;
      }
      break;
        case AttributeValueType::Integer:
      m_value = valueElement.firstChild().nodeValue().toInt();
      break;
        case AttributeValueType::Unsigned:
      m_value = valueElement.firstChild().nodeValue().toUInt();
      break;
        case AttributeValueType::Double:
      m_value = valueElement.firstChild().nodeValue().toDouble();
      break;
        case AttributeValueType::Quantity :
      OS_ASSERT(m_units);
      OS_ASSERT(sys);
      m_value = QVariant::fromValue(Quantity(valueElement.firstChild().nodeValue().toDouble(),createUnit(*m_units,*sys).get()));
      m_units.reset();
      break;
        case AttributeValueType::Unit :
      OS_ASSERT(m_units);
      OS_ASSERT(sys);
      m_value = QVariant::fromValue(OSOptionalQuantity(createUnit(*m_units,*sys).get()));
      m_units.reset();
      break;
        case AttributeValueType::String:
      m_value.setValue(valueElement.firstChild().nodeValue().toStdString());
      break;
        case AttributeValueType::AttributeVector:
      for (int i = 0; i < childNodes.count(); i++){
        QDomElement childElement = childNodes.at(i).toElement();
        children.push_back(Attribute(childElement));
      }
      m_value.setValue(children);
      break;
        default:
      OS_ASSERT(false);
      break;
    }
  }

  Attribute_Impl::Attribute_Impl(const Attribute_Impl& other)
    : m_uuid(createUUID()),
      m_versionUUID(createUUID()),
      m_name(other.name()),
      m_displayName(other.displayName()),
      m_valueType(other.valueType()),
      m_value(other.valueAsQVariant()),
      m_units(other.units())
  {}

  openstudio::UUID Attribute_Impl::uuid() const {
    return m_uuid;
  }

  openstudio::UUID Attribute_Impl::versionUUID() const {
    return m_versionUUID;
  }

  std::string Attribute_Impl::name() const
  {
    return m_name;
  }

  boost::optional<std::string> Attribute_Impl::displayName() const
  {
    return m_displayName;
  }

  bool Attribute_Impl::setDisplayName(const std::string& displayName)
  {
    m_displayName = displayName;
    m_versionUUID = createUUID();
    return true;
  }

  AttributeValueType Attribute_Impl::valueType() const
  {
    return m_valueType;
  }

  bool Attribute_Impl::valueAsBoolean() const
  {
    if(!m_value.isValid() || m_value.isNull() || m_valueType != AttributeValueType::Boolean){
      LOG_AND_THROW("Cannot convert attribute '" << name() << "' of type " 
                    << valueType().valueDescription() << " to Boolean.");
    }
    return m_value.value<bool>();
  }

  void Attribute_Impl::setValue(bool value) {
    if (m_valueType != AttributeValueType::Boolean) {
      LOG_AND_THROW("Attribute is '" << name() << "' is of type " 
                    << valueType().valueDescription() << ", not Boolean.");
    }
    m_value = value;
    m_versionUUID = createUUID();
  }

  int Attribute_Impl::valueAsInteger() const
  {
    if(!m_value.isValid() || m_value.isNull() || m_valueType != AttributeValueType::Integer){
      LOG_AND_THROW("Cannot convert attribute '" << name() << "' of type " 
                    << valueType().valueDescription() << " to Integer.");
    }
    return m_value.value<int>();
  }

  void Attribute_Impl::setValue(int value) {
    if (m_valueType != AttributeValueType::Integer) {
      LOG_AND_THROW("Attribute is '" << name() << "' is of type " 
                    << valueType().valueDescription() << ", not Integer.");
    }
    m_value = value;
    m_versionUUID = createUUID();
  }

  unsigned Attribute_Impl::valueAsUnsigned() const
  {
    if(!m_value.isValid() || m_value.isNull() || m_valueType != AttributeValueType::Unsigned){
      LOG_AND_THROW("Cannot convert attribute '" << name() << "' of type " 
                    << valueType().valueDescription() << " to Unsigned.");
    }
    return m_value.value<unsigned>();
  }

  void Attribute_Impl::setValue(unsigned value) {
    if (m_valueType != AttributeValueType::Unsigned) {
      LOG_AND_THROW("Attribute is '" << name() << "' is of type " 
                    << valueType().valueDescription() << ", not Unsigned.");
    }
    m_value = value;
    m_versionUUID = createUUID();
  }

  double Attribute_Impl::valueAsDouble() const
  {
    if(!m_value.isValid() || m_value.isNull() || m_valueType != AttributeValueType::Double){
      LOG_AND_THROW("Cannot convert attribute '" << name() << "' of type " 
                    << valueType().valueDescription() << " to Double.");
    }
    return m_value.value<double>();
  }

  void Attribute_Impl::setValue(double value) {
    if (m_valueType != AttributeValueType::Double) {
      LOG_AND_THROW("Attribute is '" << name() << "' is of type " 
                    << valueType().valueDescription() << ", not Double.");
    }
    m_value = value;
    m_versionUUID = createUUID();
  }

  Quantity Attribute_Impl::valueAsQuantity() const
  {
    if(!m_value.isValid() || m_value.isNull() || m_valueType != AttributeValueType::Quantity){
      LOG_AND_THROW("Cannot convert attribute '" << name() << "' of type " 
                    << valueType().valueDescription() << " to Quantity.");
    }
    return m_value.value<openstudio::Quantity>();
  }

  void Attribute_Impl::setValue(const Quantity& value) {
    if (m_valueType != AttributeValueType::Quantity) {
      LOG_AND_THROW("Attribute is '" << name() << "' is of type " 
                    << valueType().valueDescription() << ", not Quantity.");
    }
    m_value.setValue(value);
    m_versionUUID = createUUID();
  }

  Unit Attribute_Impl::valueAsUnit() const
  {
    if(!m_value.isValid() || m_value.isNull() || m_valueType != AttributeValueType::Unit){
      LOG_AND_THROW("Cannot convert attribute '" << name() << "' of type " 
                    << valueType().valueDescription() << " to Unit.");
    }
    return m_value.value<openstudio::OSOptionalQuantity>().units();
  }

  void Attribute_Impl::setValue(const Unit& value) {
    if (m_valueType != AttributeValueType::Unit) {
      LOG_AND_THROW("Attribute is '" << name() << "' is of type " 
                    << valueType().valueDescription() << ", not Unit.");
    }
    m_value.setValue(OSOptionalQuantity(value));
    m_versionUUID = createUUID();
  }

  std::string Attribute_Impl::valueAsString() const
  {
    if(!m_value.isValid() || m_value.isNull() || m_valueType != AttributeValueType::String){
      LOG_AND_THROW("Cannot convert attribute '" << name() << "' of type " 
                    << valueType().valueDescription() << " to String.");
    }
    return m_value.value<std::string>();
  }

  void Attribute_Impl::setValue(const char* value) {
    if (m_valueType != AttributeValueType::String) {
      LOG_AND_THROW("Attribute is '" << name() << "' is of type " 
                    << valueType().valueDescription() << ", not String.");
    }
    m_value.setValue(std::string(value));
    m_versionUUID = createUUID();
  }

  void Attribute_Impl::setValue(const std::string& value) {
    if (m_valueType != AttributeValueType::String) {
      LOG_AND_THROW("Attribute is '" << name() << "' is of type " 
                    << valueType().valueDescription() << ", not String.");
    }
    m_value.setValue(value);
    m_versionUUID = createUUID();
  }

  std::vector<Attribute> Attribute_Impl::valueAsAttributeVector() const
  {
    if(!m_value.isValid() || m_value.isNull() || m_valueType != AttributeValueType::AttributeVector){
      LOG_AND_THROW("Cannot convert attribute '" << name() << "' of type " 
                    << valueType().valueDescription() << " to AttributeVector.");
    }
    return m_value.value<std::vector<Attribute> >();
  }

  void Attribute_Impl::setValue(const std::vector<Attribute>& value) {
    if (m_valueType != AttributeValueType::AttributeVector) {
      LOG_AND_THROW("Attribute is '" << name() << "' is of type " 
                    << valueType().valueDescription() << ", not AttributeVector.");
    }
    m_value.setValue(value);
    m_versionUUID = createUUID();
  }

  QVariant Attribute_Impl::valueAsQVariant() const
  {
    return m_value;
  }

  struct FindChildByName {
    FindChildByName(const std::string& name)
      : m_name(name)
    {}

    bool operator()(const Attribute& attribute)
    {
      return m_name == attribute.name();
    }

    std::string m_name;
  };

  boost::optional<Attribute> Attribute_Impl::findChildByName(const std::string& name) const
  {
    boost::optional<Attribute> result;
    if (m_value.isValid() && !m_value.isNull() && m_valueType == AttributeValueType::AttributeVector){
      std::vector<Attribute> children = this->valueAsAttributeVector();
      std::vector<Attribute>::const_iterator it = std::find_if(children.begin(), children.end(), FindChildByName(name));
      if (it != children.end()){
        result = *it;
      }
    }
    return result;
  }

  boost::optional<std::string> Attribute_Impl::units() const
  {
    return m_units;
  }

  bool Attribute_Impl::setUnits(const std::string& units)
  {
    m_units = units;
    m_versionUUID = createUUID();
    return true;
  }

  std::string Attribute_Impl::toString() const
  {
    std::string result;
    std::stringstream ss;
    QString str;
    QTextStream qts(&str);

    switch(this->valueType().value()) {
      case AttributeValueType::Boolean:
        if (this->valueAsBoolean()) {
          result = "true";
        }
        else {
          result = "false";
        }
        break;
      case AttributeValueType::Integer:
        result = boost::lexical_cast<std::string>(this->valueAsInteger());
        break;
      case AttributeValueType::Unsigned:
        result = boost::lexical_cast<std::string>(this->valueAsUnsigned());
        break;
      case AttributeValueType::Double:
        result = openstudio::toString(this->valueAsDouble());
        break;
      case AttributeValueType::Quantity :
        ss << this->valueAsQuantity();
        result = ss.str();
        break;
      case AttributeValueType::Unit :
        ss << this->valueAsUnit();
        result = ss.str();
        break;
      case AttributeValueType::String :
        result = this->valueAsString();
        break;
      case AttributeValueType::AttributeVector:
        this->toXml().save(qts, 2);
        result = str.toStdString();
        break;
      default:
        OS_ASSERT(false);
        break;
    }
    return result;  
  }

  QDomDocument Attribute_Impl::toXml() const
  {
    QDomDocument doc;
    QDomElement element = doc.createElement(QString("Attribute"));
    OS_ASSERT(!element.isNull());

    this->writeValues(doc, element);
    doc.appendChild(element);

    return doc;
  }

  bool Attribute_Impl::operator==(const Attribute& other) const
  {
    bool result = false;

    QVariant otherValue = other.valueAsQVariant();

    if (this->valueType() == AttributeValueType::Quantity) {
      if (otherValue.canConvert<openstudio::Quantity>()) {
        result = (this->valueAsQuantity() == otherValue.value<openstudio::Quantity>());
      }
    }else if (this->valueType() == AttributeValueType::Unit) {
      if (otherValue.canConvert<openstudio::OSOptionalQuantity>()) {
        result = (this->valueAsUnit() == otherValue.value<openstudio::OSOptionalQuantity>().units());
      }
    }else if (this->valueType() == AttributeValueType::String){
      if (otherValue.canConvert<std::string>()){
        result = (this->valueAsString() == otherValue.value<std::string>());
      }
    }else if (this->valueType() == AttributeValueType::AttributeVector){
      if (otherValue.canConvert<std::vector<Attribute> >()){

        std::vector<Attribute> thisAttributes = this->valueAsAttributeVector();
        std::vector<Attribute> otherAttributes = other.valueAsAttributeVector();

        if (thisAttributes.size() == otherAttributes.size()){
          result = true;
          for (unsigned i = 0; i < thisAttributes.size(); ++i){
            result = result && (thisAttributes[i] == otherAttributes[i]);
            if (!result){
              break;
            }
          }
        }

      }
    }else{
      result = (m_value == otherValue);
    }

    if (m_units){
      result = result && other.units() && (*m_units == other.units().get());
    }else{
      result = result && !other.units();
    }

    return result;
  }

  void Attribute_Impl::writeValues(QDomDocument& doc, QDomElement& element) const
  {
    std::stringstream ss;
    QDomElement childElement;
    QDomText text;

    childElement = doc.createElement(QString::fromStdString("UUID"));
    text = doc.createTextNode(m_uuid.toString());
    childElement.appendChild(text);
    element.appendChild(childElement);

    childElement = doc.createElement(QString::fromStdString("VersionUUID"));
    text = doc.createTextNode(m_versionUUID.toString());
    childElement.appendChild(text);
    element.appendChild(childElement);

    childElement = doc.createElement(QString::fromStdString("Name"));
    text = doc.createTextNode(toQString(m_name));
    childElement.appendChild(text);
    element.appendChild(childElement);

    if (m_displayName){
      childElement = doc.createElement(QString::fromStdString("DisplayName"));
      text = doc.createTextNode(toQString(*m_displayName));
      childElement.appendChild(text);
      element.appendChild(childElement);
    }

    childElement = doc.createElement(QString::fromStdString("ValueType"));
    text = doc.createTextNode(toQString(m_valueType.valueName()));
    childElement.appendChild(text);
    element.appendChild(childElement);

    std::string temp;
    childElement = doc.createElement(QString::fromStdString("Value"));
    switch(m_valueType.value()){
    case AttributeValueType::Boolean:
      text = doc.createTextNode(this->valueAsBoolean() ? QString::fromStdString("true") : QString::fromStdString("false"));
      childElement.appendChild(text);
      break;
    case AttributeValueType::Integer:
      text = doc.createTextNode(QString::number(this->valueAsInteger()));
      childElement.appendChild(text);
      break;
    case AttributeValueType::Unsigned:
      text = doc.createTextNode(QString::number(this->valueAsUnsigned()));
      childElement.appendChild(text);
      break;
    case AttributeValueType::Double:
      temp = boost::regex_replace(openstudio::toString(QString::number(this->valueAsDouble(),
                                                                       'G',
                                                                       std::numeric_limits<double>::digits10)),
                                  boost::regex("\\+"),"");
      text = doc.createTextNode(toQString(temp));
      childElement.appendChild(text);
      break;
    case AttributeValueType::Quantity:
      temp = boost::regex_replace(openstudio::toString(QString::number(this->valueAsQuantity().value(),
                                                                       'G',
                                                                       std::numeric_limits<double>::digits10)),
                                  boost::regex("\\+"),"");
      text = doc.createTextNode(toQString(temp));
      childElement.appendChild(text);
      break;
    case AttributeValueType::Unit:
      text = doc.createTextNode(QString());
      childElement.appendChild(text);
      break;
    case AttributeValueType::String:
      text = doc.createTextNode(QString::fromStdString(this->valueAsString()));
      childElement.appendChild(text);
      break;
    case AttributeValueType::AttributeVector:
      BOOST_FOREACH(const Attribute& Attribute, this->valueAsAttributeVector()){
        childElement.appendChild(Attribute.toXml().documentElement());
      }
      break;
        default:
      OS_ASSERT(false);
      break;
    }
    element.appendChild(childElement);

    if ((m_valueType.value() == AttributeValueType::Quantity) ||
        (m_valueType.value() == AttributeValueType::Unit))
    {
      OptionalUnit units;
      if (m_valueType.value() == AttributeValueType::Quantity) {
        units = this->valueAsQuantity().units();
      }
      else {
        units = this->valueAsUnit();
      }

      childElement = doc.createElement(QString::fromStdString("Units"));
      text = doc.createTextNode(QString::fromStdString(units->standardString()));
      childElement.appendChild(text);
      element.appendChild(childElement);

      childElement = doc.createElement(QString::fromStdString("UnitSystem"));
      text = doc.createTextNode(QString::fromStdString(units->system().valueName()));
      childElement.appendChild(text);
      element.appendChild(childElement);
    }else if(m_units){
      childElement = doc.createElement(QString::fromStdString("Units"));
      text = doc.createTextNode(QString::fromStdString(*m_units));
      childElement.appendChild(text);
      element.appendChild(childElement);
    }
  }

  void Attribute_Impl::setValue(const QVariant& value, bool check)
  {
    if (check) {
      if (OptionalAttribute validator = Attribute::fromQVariant(name(),value)) {
        if (validator->valueType() != valueType()) {
          LOG_AND_THROW("QVariant value of type " << value.typeName() 
              << " does not convert to AttributeValueType " << valueType().valueDescription() 
              << ".");
        }
      }
      else {
        LOG_AND_THROW("Unable to construct Attribute using QVariant value of type " 
            << value.typeName() << ".");
      }
    }
    m_value = value;
  }

} // detail

AttributeDescription::AttributeDescription(const std::string& t_name,
                                           const std::string& t_displayName,
                                           const std::string& t_description,
                                           const AttributeValueType& t_validValueType,
                                           bool t_required)
  : name(t_name), 
    displayName(t_displayName),
    description(t_description),
    validValueTypes(1u,t_validValueType),
    required(t_required)
{}

AttributeDescription::AttributeDescription(const std::string& t_name,
                                           const std::string& t_displayName,
                                           const std::string& t_description,
                                           const AttributeValueType& t_validValueType,
                                           bool t_required,
                                           QVariant t_defaultValue)
  : name(t_name), 
    displayName(t_displayName),
    description(t_description),
    validValueTypes(1u,t_validValueType),
    required(t_required),
    defaultValue(t_defaultValue)
{}

AttributeDescription::AttributeDescription(const std::string& t_name,
                                           const std::string& t_displayName,
                                           const std::string& t_description,
                                           const std::vector<AttributeValueType>& t_validValueTypes,
                                           bool t_required)
  : name(t_name), 
    displayName(t_displayName),
    description(t_description),
    validValueTypes(t_validValueTypes),
    required(t_required)
{}

AttributeDescription::AttributeDescription(const std::string& t_name,
                                           const std::string& t_displayName,
                                           const std::string& t_description,
                                           const std::vector<AttributeValueType>& t_validValueTypes,
                                           bool t_required,
                                           QVariant t_defaultValue)
  : name(t_name), 
    displayName(t_displayName),
    description(t_description),
    validValueTypes(t_validValueTypes),
    required(t_required),
    defaultValue(t_defaultValue)
{}

Attribute::Attribute(const std::string& name, bool value)
  : m_impl(boost::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(name, value, boost::none)))
{
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const std::string& name, bool value, const std::string& units)
  : m_impl(boost::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(name, value, units)))
{
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const std::string& name, bool value, const boost::optional<std::string>& units)
  : m_impl(boost::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(name, value, units)))
{
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const openstudio::UUID& uuid,
                     const openstudio::UUID& versionUUID,
                     const std::string& name,
                     const boost::optional<std::string>& displayName,
                     bool value,
                     const boost::optional<std::string>& units)
  : m_impl(boost::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(uuid, versionUUID, name, displayName, value, units)))
{
  OS_ASSERT(m_impl);
}

Attribute::Attribute(const std::string& name, double value)
  : m_impl(boost::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(name, value, boost::none)))
{
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const std::string& name, double value, const std::string& units)
  : m_impl(boost::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(name, value, units)))
{
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const std::string& name, double value, const boost::optional<std::string>& units)
  : m_impl(boost::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(name, value, units)))
{
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const openstudio::UUID& uuid,
                     const openstudio::UUID& versionUUID,
                     const std::string& name,
                     const boost::optional<std::string>& displayName,
                     double value,
                     const boost::optional<std::string>& units)
  : m_impl(boost::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(uuid, versionUUID, name, displayName, value, units)))
{
  OS_ASSERT(m_impl);
}

Attribute::Attribute(const std::string& name, const OSOptionalQuantity& value)
  : m_impl(boost::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(name,value)))
{
  OS_ASSERT(m_impl);
}

Attribute::Attribute(const std::string& name, const Quantity& value)
  : m_impl(boost::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(name, value)))
{
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const openstudio::UUID& uuid,
                     const openstudio::UUID& versionUUID,
                     const std::string& name,
                     const boost::optional<std::string>& displayName,
                     const Quantity& value)
  : m_impl(boost::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(uuid, versionUUID, name, displayName, value)))
{
  OS_ASSERT(m_impl);
}

Attribute::Attribute(const std::string& name, const Unit& value)
  : m_impl(boost::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(name,value)))
{
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const openstudio::UUID& uuid,
                     const openstudio::UUID& versionUUID,
                     const std::string& name,
                     const boost::optional<std::string>& displayName,
                     const Unit& value)
  : m_impl(boost::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(uuid, versionUUID, name, displayName, value)))
{
  OS_ASSERT(m_impl);
}

Attribute::Attribute(const std::string& name, int value)
  : m_impl(boost::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(name, value, boost::none)))
{
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const std::string& name, int value, const std::string& units)
  : m_impl(boost::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(name, value, units)))
{
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const std::string& name, int value, const boost::optional<std::string>& units)
  : m_impl(boost::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(name, value, units)))
{
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const openstudio::UUID& uuid,
                     const openstudio::UUID& versionUUID,
                     const std::string& name,
                     const boost::optional<std::string>& displayName,
                     int value,
                     const boost::optional<std::string>& units)
  : m_impl(boost::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(uuid, versionUUID, name, displayName, value, units)))
{
  OS_ASSERT(m_impl);
}

Attribute::Attribute(const std::string& name, unsigned value)
  : m_impl(boost::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(name, value, boost::none)))
{
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const std::string& name, unsigned value, const std::string& units)
  : m_impl(boost::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(name, value, units)))
{
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const std::string& name, unsigned value, const boost::optional<std::string>& units)
  : m_impl(boost::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(name, value, units)))
{
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const openstudio::UUID& uuid,
                     const openstudio::UUID& versionUUID,
                     const std::string& name,
                     const boost::optional<std::string>& displayName,
                     unsigned value,
                     const boost::optional<std::string>& units)
  : m_impl(boost::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(uuid, versionUUID, name, displayName, value, units)))
{
  OS_ASSERT(m_impl);
}

Attribute::Attribute(const std::string& name, const char* value)
  : m_impl(boost::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(name, value, boost::none)))
{
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const std::string& name, const char* value, const std::string& units)
  : m_impl(boost::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(name, value, units)))
{
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const std::string& name, const char* value, const boost::optional<std::string>& units)
  : m_impl(boost::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(name, value, units)))
{
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const openstudio::UUID& uuid,
                     const openstudio::UUID& versionUUID,
                     const std::string& name,
                     const boost::optional<std::string>& displayName,
                     const char* value,
                     const boost::optional<std::string>& units)
  : m_impl(boost::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(uuid, versionUUID, name, displayName, value, units)))
{
  OS_ASSERT(m_impl);
}

Attribute::Attribute(const std::string& name, const std::string& value)
  : m_impl(boost::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(name, value, boost::none)))
{
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const std::string& name, const std::string& value, const std::string& units)
  : m_impl(boost::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(name, value, units)))
{
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const std::string& name, const std::string& value, const boost::optional<std::string>& units)
  : m_impl(boost::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(name, value, units)))
{
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const openstudio::UUID& uuid,
                     const openstudio::UUID& versionUUID,
                     const std::string& name,
                     const boost::optional<std::string>& displayName,
                     const std::string& value,
                     const boost::optional<std::string>& units)
  : m_impl(boost::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(uuid, versionUUID, name, displayName, value, units)))
{
  OS_ASSERT(m_impl);
}

Attribute::Attribute(const std::string& name, const std::vector<openstudio::Attribute>& value)
  : m_impl(boost::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(name, value, boost::none)))
{
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const std::string& name, const std::vector<openstudio::Attribute>& value, const std::string& units)
  : m_impl(boost::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(name, value, units)))
{
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const std::string& name, const std::vector<openstudio::Attribute>& value, const boost::optional<std::string>& units)
  : m_impl(boost::shared_ptr<detail::Attribute_Impl>(new detail::Attribute_Impl(name, value, units)))
{
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const openstudio::UUID& uuid,
                     const openstudio::UUID& versionUUID,
                     const std::string& name,
                     const boost::optional<std::string>& displayName,
                     const std::vector<openstudio::Attribute>& value,
                     const boost::optional<std::string>& units)
  : m_impl(boost::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(uuid, versionUUID, name, displayName, value, units)))
{
  OS_ASSERT(m_impl);
}

Attribute::Attribute(const QDomElement& element)
  : m_impl(boost::shared_ptr<detail::Attribute_Impl>(new detail::Attribute_Impl(element)))
{
  OS_ASSERT(m_impl);
}

Attribute::Attribute(const Attribute& other)
  : m_impl(other.m_impl)
{
  OS_ASSERT(m_impl);
}

Attribute Attribute::clone() const {
  Attribute result(boost::shared_ptr<detail::Attribute_Impl>(new detail::Attribute_Impl(*m_impl)));
  return result;
}

Attribute::Attribute(const boost::shared_ptr<detail::Attribute_Impl>& impl)
  : m_impl(impl)
{
  OS_ASSERT(m_impl);
}

boost::optional<Attribute> Attribute::fromQVariant(const std::string& name, const QVariant& value, const boost::optional<std::string>& units)
{

  boost::optional<Attribute> result;
  std::string typeName = value.typeName();

  switch (value.type()) {
  case QVariant::Bool :
    result = Attribute(name, value.toBool(), units);
    break;
  case QVariant::Int :
    result = Attribute(name, value.toInt(), units);
    break;
  case QVariant::UInt :
    result = Attribute(name, value.toUInt(), units);
    break;
  case QVariant::Double :
    result = Attribute(name, value.toDouble(), units);
    break;
  case QVariant::UserType :

    if (typeName == "std::string") {
      result = Attribute(name, value.value<std::string>(), units);
    }else if (typeName == "openstudio::Attribute") {
      LOG(Error, "openstudio::Attribute is not yet registered with QMetaType");
      //result = value.value<openstudio::Attribute>();
    }else if (typeName == "boost::optional<int>") {
      boost::optional<int> test = value.value<boost::optional<int> >();
      if (test){
        result = Attribute(name, *test, units);
      }
    }else if (typeName == "boost::optional<unsigned>") {
      boost::optional<unsigned> test = value.value<boost::optional<unsigned> >();
      if (test){
        result = Attribute(name, *test, units);
      }
    }else if (typeName == "boost::optional<double>") {
      boost::optional<double> test = value.value<boost::optional<double> >();
      if (test){
        result = Attribute(name, *test, units);
      }
    }else if (typeName == "openstudio::Quantity") {
      result = Attribute(name,value.value<openstudio::Quantity>());
    }else if (typeName == "openstudio::OSOptionalQuantity") {
      result = Attribute(name,value.value<openstudio::OSOptionalQuantity>());
    }else if (typeName == "boost::optional<std::string>") {
      boost::optional<std::string> test = value.value<boost::optional<std::string> >();
      if (test){
        result = Attribute(name, *test, units);
      }
    }else if (typeName == "boost::optional<openstudio::Attribute>") {
      boost::optional<openstudio::Attribute> test = value.value<boost::optional<openstudio::Attribute> >();
      if (test){
        result = *test;
      }
    }else if (typeName == "std::vector<openstudio::Attribute>") {
      result = Attribute(name,value.value<std::vector<openstudio::Attribute> >(),units);      
    }
    break;
    default:
    ;
  };

  return result;
}

boost::optional<Attribute> Attribute::loadFromXml(const openstudio::path& path)
{
  boost::optional<Attribute> result;

  if (boost::filesystem::exists(path)){
    try{

      QFile file(toQString(path));
      file.open(QFile::ReadOnly);
      QDomDocument qDomDocument;
      qDomDocument.setContent(&file);
      file.close();

      result = Attribute(qDomDocument.documentElement());
    }catch(...){
    }
  }

  return result;
}

openstudio::UUID Attribute::uuid() const
{
  return m_impl->uuid();
}

openstudio::UUID Attribute::versionUUID() const
{
  return m_impl->versionUUID();
}

std::string Attribute::name() const
{
  return m_impl->name();
}

boost::optional<std::string> Attribute::displayName() const
{
  return m_impl->displayName();
}

bool Attribute::setDisplayName(const std::string& displayName)
{
  return m_impl->setDisplayName(displayName);
}

AttributeValueType Attribute::valueType() const
{
  return m_impl->valueType();
}

bool Attribute::valueAsBoolean() const
{
  return m_impl->valueAsBoolean();
}

void Attribute::setValue(bool value) {
  m_impl->setValue(value);
}

int Attribute::valueAsInteger() const
{
  return m_impl->valueAsInteger();
}

void Attribute::setValue(int value) {
  m_impl->setValue(value);
}

unsigned Attribute::valueAsUnsigned() const
{
  return m_impl->valueAsUnsigned();
}

void Attribute::setValue(unsigned value) {
  m_impl->setValue(value);
}

double Attribute::valueAsDouble() const
{
  return m_impl->valueAsDouble();
}

void Attribute::setValue(double value) {
  m_impl->setValue(value);
}

Quantity Attribute::valueAsQuantity() const {
  return m_impl->valueAsQuantity();
}

void Attribute::setValue(const Quantity& value) {
  m_impl->setValue(value);
}

Unit Attribute::valueAsUnit() const {
  return m_impl->valueAsUnit();
}

void Attribute::setValue(const Unit& value) {
  m_impl->setValue(value);
}

std::string Attribute::valueAsString() const
{
  return m_impl->valueAsString();
}

void Attribute::setValue(const char* value)  {
  m_impl->setValue(value);
}

void Attribute::setValue(const std::string& value) {
  m_impl->setValue(value);
}

std::vector<Attribute> Attribute::valueAsAttributeVector() const
{
  return m_impl->valueAsAttributeVector();
}

void Attribute::setValue(const std::vector<Attribute>& value) {
  m_impl->setValue(value);
}

QVariant Attribute::valueAsQVariant() const
{
  return m_impl->valueAsQVariant();
}

boost::optional<Attribute> Attribute::findChildByName(const std::string& name) const
{
  return m_impl->findChildByName(name);
}

boost::optional<std::string> Attribute::units() const
{
  return m_impl->units();
}

bool Attribute::setUnits(const std::string& units)
{
  return m_impl->setUnits(units);
}

std::string Attribute::toString() const
{
  return m_impl->toString();
}

QDomDocument Attribute::toXml() const
{
  return m_impl->toXml();
}

bool Attribute::saveToXml(const openstudio::path& path) const
{
  bool result = false;

  try {
    QFile file(toQString(path));
    file.open(QFile::WriteOnly);
    QTextStream out(&file);
    this->toXml().save(out, 2);
    file.close();
    result = true;
  }
  catch(...) {}

  return result;
}

bool Attribute::operator==(const Attribute& other) const
{
  return m_impl->operator==(other);
}

void Attribute::setValue(const QVariant& value)
{
  m_impl->setValue(value,true);
}

std::ostream& operator<<(std::ostream& os, const Attribute& attribute)
{
  QString str;
  QTextStream qts(&str);
  attribute.toXml().save(qts, 2);
  os << str.toStdString();
  return os;
}

Attribute createAttributeFromVector(const std::string& name, std::vector<int> value) {
  AttributeVector valueAsAttributes;
  BOOST_FOREACH(int v,value) {
    Attribute attribute(std::string(),v);
    valueAsAttributes.push_back(attribute);
  }
  return Attribute(name,valueAsAttributes);
}

Attribute createAttributeFromVector(const std::string& name, std::vector<double> value) {
  AttributeVector valueAsAttributes;
  BOOST_FOREACH(double v,value) {
    Attribute attribute(std::string(),v);
    valueAsAttributes.push_back(attribute);
  }
  return Attribute(name,valueAsAttributes);
}

std::vector<int> getIntVectorFromAttribute(const Attribute& attribute) {
  IntVector result;
  if (attribute.valueType() != AttributeValueType::AttributeVector) {
    LOG_FREE_AND_THROW("openstudio.Attribute","Attribute must be of type AttributeVector for this function to be applicable.");
  }
  AttributeVector attributes = attribute.valueAsAttributeVector();
  BOOST_FOREACH(const Attribute& a, attributes) {
    result.push_back(a.valueAsInteger());
  }
  return result;
}

std::vector<double> getDoubleVectorFromAttribute(const Attribute& attribute) {
  DoubleVector result;
  if (attribute.valueType() != AttributeValueType::AttributeVector) {
    LOG_FREE_AND_THROW("openstudio.Attribute","Attribute must be of type AttributeVector for this function to be applicable.");
  }
  AttributeVector attributes = attribute.valueAsAttributeVector();
  BOOST_FOREACH(const Attribute& a, attributes) {
    result.push_back(a.valueAsDouble());
  }
  return result;
}

bool isConsistent(const Attribute& candidate,const AttributeDescription& description) {
  return ((candidate.name() == description.name) && 
          (std::find(description.validValueTypes.begin(),
                     description.validValueTypes.end(),
                     candidate.valueType()) != description.validValueTypes.end()));
}

bool prepareForDisplay(Attribute& attribute, const AttributeDescription& description) {
  if (isConsistent(attribute,description)) {
    attribute.setDisplayName(description.displayName);
    return true;
  }
  return false;
}

namespace detail {

  QVariant toVariant(const Attribute& attribute) {
    QVariantMap attributeData;

    attributeData["uuid"] = toQString(removeBraces(attribute.uuid()));
    attributeData["version_uuid"] = toQString(removeBraces(attribute.versionUUID()));
    attributeData["name"] = toQString(attribute.name());
    if (attribute.displayName()) {
      attributeData["display_name"] = toQString(attribute.displayName().get());
    }
    AttributeValueType valueType = attribute.valueType();
    attributeData["value_type"] = toQString(valueType.valueName());
    if (valueType == AttributeValueType::Quantity) {
      Quantity value = attribute.valueAsQuantity();
      attributeData["value"] = QVariant(value.value());
      attributeData["units"] = toQString(value.units().standardString());
    }
    else if (valueType == AttributeValueType::Unit) {
      attributeData["units"] = toQString(attribute.valueAsUnit().standardString());
    }
    else if (valueType == AttributeValueType::AttributeVector) {
      QVariantList childAttributesList;
      Q_FOREACH(const Attribute& child,attribute.valueAsAttributeVector()) {
        childAttributesList.push_back(toVariant(child));
      }
      attributeData["value"] = QVariant(childAttributesList);
      if (attribute.units()) {
        attributeData["units"] = toQString(attribute.units().get());
      }
    }
    else {
      // use QVariant directly
      QVariant val = attribute.valueAsQVariant();
      if (std::string(val.typeName()) == std::string("std::string")) {
        // in some cases a std::string gets shoved in
        // convert it to QString
        val = toQString(val.value<std::string>());
      }
      attributeData["value"] = val;
      if (attribute.units()) {
        attributeData["units"] = toQString(attribute.units().get());
      }
    }

    return QVariant(attributeData);
  }

  Attribute toAttribute(const QVariant& variant, const VersionString& version) {
    QVariantMap map = variant.toMap();

    openstudio::UUID uuid = toUUID(map["uuid"].toString().toStdString());
    openstudio::UUID versionUUID = toUUID(map["version_uuid"].toString().toStdString());
    std::string name = map["name"].toString().toStdString();
    OptionalString displayName;
    if (map.contains("display_name")) {
      displayName = map["display_name"].toString().toStdString();
    }

    OptionalUnit ou;
    OptionalQuantity oq;
    AttributeVector children;
    AttributeValueType valueType(map["value_type"].toString().toStdString());
    switch (valueType.value()) {
      case AttributeValueType::Boolean :
        return Attribute(uuid,
                         versionUUID,
                         name,
                         displayName,
                         map["value"].toBool(),
                         map.contains("units") ? map["units"].toString().toStdString() : OptionalString());
      case AttributeValueType::Double :
        return Attribute(uuid,
                         versionUUID,
                         name,
                         displayName,
                         map["value"].toDouble(),
                         map.contains("units") ? map["units"].toString().toStdString() : OptionalString());
      case AttributeValueType::Quantity :
        oq = createQuantity(map["value"].toDouble(),map["units"].toString().toStdString());
        OS_ASSERT(oq);
        return Attribute(uuid,
                         versionUUID,
                         name,
                         displayName,
                         oq.get());
      case AttributeValueType::Unit :
        ou = createUnit(map["units"].toString().toStdString());
        OS_ASSERT(ou);
        return Attribute(uuid,
                         versionUUID,
                         name,
                         displayName,
                         ou.get());
      case AttributeValueType::Integer :
        return Attribute(uuid,
                         versionUUID,
                         name,
                         displayName,
                         map["value"].toInt(),
                         map.contains("units") ? map["units"].toString().toStdString() : OptionalString());
      case AttributeValueType::Unsigned :
        return Attribute(uuid,
                         versionUUID,
                         name,
                         displayName,
                         map["value"].toUInt(),
                         map.contains("units") ? map["units"].toString().toStdString() :  OptionalString());
      case AttributeValueType::String :
        return Attribute(uuid,
                         versionUUID,
                         name,
                         displayName,
                         map["value"].toString().toStdString(),
                         map.contains("units") ? map["units"].toString().toStdString() :  OptionalString());
      case AttributeValueType::AttributeVector :
        children = deserializeUnorderedVector<Attribute>(
              map["value"].toList(),
              boost::function<Attribute (const QVariant&)>(boost::bind(openstudio::detail::toAttribute,_1,version)));
        return Attribute(uuid,
                         versionUUID,
                         name,
                         displayName,
                         children,
                         map.contains("units") ? map["units"].toString().toStdString() :  OptionalString());
      default :
        LOG_FREE_AND_THROW("openstudio.Attribute","Unknown AttributeValueType " << valueType.valueName() << ".");
    }
  }

} // detail

} // openstudio

