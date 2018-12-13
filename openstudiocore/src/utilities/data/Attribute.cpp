/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "Attribute.hpp"
#include "Attribute_Impl.hpp"

#include "../core/Assert.hpp"
#include "../core/Containers.hpp"
#include "../core/FilesystemHelpers.hpp"

#include "../units/QuantityFactory.hpp"

#include <boost/lexical_cast.hpp>

#include <QDomElement>
#include <QTextStream>

namespace openstudio {
namespace detail {


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
                                 const boost::optional<std::string>& units,
                                 const std::string& source)
    : m_uuid(uuid),
      m_versionUUID(versionUUID),
      m_name(name),
      m_displayName(displayName),
      m_source(source),
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
                                 const boost::optional<std::string>& units,
                                 const std::string& source)
    : m_uuid(uuid),
      m_versionUUID(versionUUID),
      m_name(name),
      m_displayName(displayName),
      m_source(source),
      m_valueType(AttributeValueType::Double),
      m_value(value),
      m_units(units)
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
                                 const boost::optional<std::string>& units,
                                 const std::string& source)
    : m_uuid(uuid),
      m_versionUUID(versionUUID),
      m_name(name),
      m_displayName(displayName),
      m_source(source),
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
                                 const boost::optional<std::string>& units,
                                 const std::string& source)
    : m_uuid(uuid),
      m_versionUUID(versionUUID),
      m_name(name),
      m_displayName(displayName),
      m_source(source),
      m_valueType(AttributeValueType::Unsigned),
      m_value(value),
      m_units(units)
  {
  }

  Attribute_Impl::Attribute_Impl(const std::string& name,
                                 const std::string& value,
                                 const boost::optional<std::string>& units)
    : m_uuid(createUUID()),
      m_versionUUID(createUUID()),
      m_name(name),
      m_valueType(AttributeValueType::String),
      m_value(std::string(value)),
      m_units(units)
  {
  }

  Attribute_Impl::Attribute_Impl(const openstudio::UUID& uuid,
                                 const openstudio::UUID& versionUUID,
                                 const std::string& name,
                                 const boost::optional<std::string>& displayName,
                                 const std::string& value,
                                 const boost::optional<std::string>& units,
                                 const std::string& source)
    : m_uuid(uuid),
      m_versionUUID(versionUUID),
      m_name(name),
      m_displayName(displayName),
      m_source(source),
      m_valueType(AttributeValueType::String),
      m_value(value),
      m_units(units)
  {
  }

  Attribute_Impl::Attribute_Impl(const std::string& name,
                                 const std::vector<openstudio::Attribute>& value,
                                 const boost::optional<std::string>& units)
    : m_uuid(createUUID()),
      m_versionUUID(createUUID()),
      m_name(name),
      m_valueType(AttributeValueType::AttributeVector),
      m_value(value),
      m_units(units)
  {
  }

  Attribute_Impl::Attribute_Impl(const openstudio::UUID& uuid,
                                 const openstudio::UUID& versionUUID,
                                 const std::string& name,
                                 const boost::optional<std::string>& displayName,
                                 const std::vector<openstudio::Attribute>& value,
                                 const boost::optional<std::string>& units,
                                 const std::string& source)
    : m_uuid(uuid),
      m_versionUUID(versionUUID),
      m_name(name),
      m_displayName(displayName),
      m_source(source),
      m_valueType(AttributeValueType::AttributeVector),
      m_value(value),
      m_units(units)
  {
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
    QDomElement sourceElement = element.firstChildElement(QString::fromStdString("Source"));
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

    if (!sourceElement.isNull()) {
      m_source = sourceElement.firstChild().nodeValue().toStdString();
    }

    m_valueType = AttributeValueType(valueTypeElement.firstChild().nodeValue().toStdString());

    if (!unitsElement.isNull()) {
      m_units = unitsElement.firstChild().nodeValue().toStdString();
    }

    OptionalUnitSystem sys;
    if (!unitSystemElement.isNull()) {
      sys = UnitSystem(unitSystemElement.firstChild().nodeValue().toStdString());
    }

    // TODO: handle units?

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
    case AttributeValueType::String:
      m_value = valueElement.firstChild().nodeValue().toStdString();
      break;
    case AttributeValueType::AttributeVector:
      for (int i = 0; i < childNodes.count(); i++){
        QDomElement childElement = childNodes.at(i).toElement();
        children.push_back(Attribute(childElement));
      }
      m_value = children;
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
      m_source(other.source()),
      m_valueType(other.valueType()),
      m_value(other.m_value),
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

  boost::optional<std::string> Attribute_Impl::displayName(bool returnName) const
  {
    OptionalString result = m_displayName;
    if (!result && returnName) {
      result = name();
    }
    return result;
  }

  bool Attribute_Impl::setDisplayName(const std::string& displayName)
  {
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

  AttributeValueType Attribute_Impl::valueType() const
  {
    return m_valueType;
  }

  bool Attribute_Impl::hasValue() const {
    return (m_value.index() != 0);
  }

  bool Attribute_Impl::valueAsBoolean() const
  {
    if(!hasValue() || m_valueType != AttributeValueType::Boolean){
      LOG_AND_THROW("Cannot convert attribute '" << name() << "' of type "
                    << valueType().valueDescription() << " to Boolean.");
    }
    return std::get<bool>(m_value);
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
    if(!hasValue() || m_valueType != AttributeValueType::Integer){
      LOG_AND_THROW("Cannot convert attribute '" << name() << "' of type "
                    << valueType().valueDescription() << " to Integer.");
    }
    return std::get<int>(m_value);
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
    if(!hasValue() || m_valueType != AttributeValueType::Unsigned){
      LOG_AND_THROW("Cannot convert attribute '" << name() << "' of type "
                    << valueType().valueDescription() << " to Unsigned.");
    }
    return std::get<unsigned>(m_value);
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
    if(!hasValue() || m_valueType != AttributeValueType::Double){
      LOG_AND_THROW("Cannot convert attribute '" << name() << "' of type "
                    << valueType().valueDescription() << " to Double.");
    }
    return std::get<double>(m_value);
  }

  void Attribute_Impl::setValue(double value) {
    if (m_valueType != AttributeValueType::Double) {
      LOG_AND_THROW("Attribute is '" << name() << "' is of type "
                    << valueType().valueDescription() << ", not Double.");
    }
    m_value = value;
    m_versionUUID = createUUID();
  }

  std::string Attribute_Impl::valueAsString() const
  {
    if(!hasValue() || m_valueType != AttributeValueType::String){
      LOG_AND_THROW("Cannot convert attribute '" << name() << "' of type "
                    << valueType().valueDescription() << " to String.");
    }
    return std::get<std::string>(m_value);
  }

  void Attribute_Impl::setValue(const char* value) {
    if (m_valueType != AttributeValueType::String) {
      LOG_AND_THROW("Attribute is '" << name() << "' is of type "
                    << valueType().valueDescription() << ", not String.");
    }
    m_value = std::string(value);
    m_versionUUID = createUUID();
  }

  void Attribute_Impl::setValue(const std::string& value) {
    if (m_valueType != AttributeValueType::String) {
      LOG_AND_THROW("Attribute is '" << name() << "' is of type "
                    << valueType().valueDescription() << ", not String.");
    }
    m_value = value;
    m_versionUUID = createUUID();
  }

  std::vector<Attribute> Attribute_Impl::valueAsAttributeVector() const
  {
    if(!hasValue() || m_valueType != AttributeValueType::AttributeVector){
      LOG_AND_THROW("Cannot convert attribute '" << name() << "' of type "
                    << valueType().valueDescription() << " to AttributeVector.");
    }
    return std::get<std::vector<Attribute> >(m_value);
  }

  void Attribute_Impl::setValue(const std::vector<Attribute>& value) {
    if (m_valueType != AttributeValueType::AttributeVector) {
      LOG_AND_THROW("Attribute is '" << name() << "' is of type "
                    << valueType().valueDescription() << ", not AttributeVector.");
    }
    m_value = value;
    m_versionUUID = createUUID();
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
    if (hasValue() && m_valueType == AttributeValueType::AttributeVector){
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
    std::stringstream ss;

    // We use std::visit, filtering out the case where it's monostate
    // Aside from monostate, every possible type is streamable
    // expect AttributeVector that needs a special treatment
    std::visit(
        [this, &ss](const auto& val) {
          // Needed to properly compare the types
          using T = std::remove_cv_t<std::remove_reference_t<decltype(val)>>;

          // If it's a vector of attributes
          if constexpr (std::is_same_v<T, openstudio::Attribute>) {
            // TODO: call toXml()
            // This will change anyways once QXML is removed
            QString str;
            QTextStream qts(&str);
            this->toXml().save(qts, 2);
            ss << str.toStdString();

          // Otherwise, if anything else but monostate
          } else if constexpr (!std::is_same_v<T, std::monostate>) {
            ss << std::boolalpha << val;
          }
        },
        this->m_value);

    return ss.str();

    // TODO: remove
/*
 *    QString str;
 *    QTextStream qts(&str);
 *
 *    switch(this->valueType().value()) {
 *      case AttributeValueType::Boolean:
 *        if (this->valueAsBoolean()) {
 *          result = "true";
 *        }
 *        else {
 *          result = "false";
 *        }
 *        break;
 *      case AttributeValueType::Integer:
 *        result = boost::lexical_cast<std::string>(this->valueAsInteger());
 *        break;
 *      case AttributeValueType::Unsigned:
 *        result = boost::lexical_cast<std::string>(this->valueAsUnsigned());
 *        break;
 *      case AttributeValueType::Double:
 *        result = openstudio::toString(this->valueAsDouble());
 *        break;
 *      case AttributeValueType::Quantity :
 *        ss << this->valueAsQuantity();
 *        result = ss.str();
 *        break;
 *      case AttributeValueType::Unit :
 *        ss << this->valueAsUnit();
 *        result = ss.str();
 *        break;
 *      case AttributeValueType::String :
 *        result = this->valueAsString();
 *        break;
 *      case AttributeValueType::AttributeVector:
 *        this->toXml().save(qts, 2);
 *        result = str.toStdString();
 *        break;
 *      default:
 *        OS_ASSERT(false);
 *        break;
 *    }
 *    return result;
 */
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

    AttributeValueType thisValueType = this->valueType();
    AttributeValueType otherValueType = other.valueType();

    // std::variant operator== will start by comparing indexes (=same type),
    // and if that works, compares std::get<i>(v) == std::get<i>(w), but that must be returning a type converting to bool
    // Which is true except for AttributeVector, and probably undefined for std::monostate

    // No need to proceed with value comparison if types aren't the same,
    // and they must either have both a value or none should have one
    if ((thisValueType == otherValueType) && (this->hasValue() == other.hasValue()) ) {

      // If there's a value to compare, let's do this
      if (this->hasValue()) {
        if (thisValueType == AttributeValueType::AttributeVector) {
            std::vector<Attribute> thisAttributes = this->valueAsAttributeVector();
            std::vector<Attribute> otherAttributes = other.valueAsAttributeVector();
            if (thisAttributes.size() == otherAttributes.size()) {
              result = true;
              for (unsigned i = 0; i < thisAttributes.size(); ++i) {
                result = result && (thisAttributes[i] == otherAttributes[i]);
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

  void Attribute_Impl::writeValues(QDomDocument& doc, QDomElement& element) const
  {
    QDomElement childElement;
    QDomText text;

    childElement = doc.createElement(QString::fromStdString("UUID"));
    text = doc.createTextNode(openstudio::toQString(m_uuid));
    childElement.appendChild(text);
    element.appendChild(childElement);

    childElement = doc.createElement(QString::fromStdString("VersionUUID"));
    text = doc.createTextNode(openstudio::toQString(m_versionUUID));
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

    if (!m_source.empty()) {
      childElement = doc.createElement(QString::fromStdString("Source"));
      text = doc.createTextNode(toQString(m_source));
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
    case AttributeValueType::String:
      text = doc.createTextNode(QString::fromStdString(this->valueAsString()));
      childElement.appendChild(text);
      break;
    case AttributeValueType::AttributeVector:
      for (const Attribute& attribute : this->valueAsAttributeVector()){
        childElement.appendChild(attribute.toXml().documentElement());
      }
      break;
        default:
      OS_ASSERT(false);
      break;
    }
    element.appendChild(childElement);

/*
 *    if ((m_valueType.value() == AttributeValueType::Quantity) ||
 *        (m_valueType.value() == AttributeValueType::Unit))
 *    {
 *      OptionalUnit units;
 *      if (m_valueType.value() == AttributeValueType::Quantity) {
 *        units = this->valueAsQuantity().units();
 *      }
 *      else {
 *        units = this->valueAsUnit();
 *      }
 *
 *      childElement = doc.createElement(QString::fromStdString("Units"));
 *      text = doc.createTextNode(QString::fromStdString(units->standardString()));
 *      childElement.appendChild(text);
 *      element.appendChild(childElement);
 *
 *      childElement = doc.createElement(QString::fromStdString("UnitSystem"));
 *      text = doc.createTextNode(QString::fromStdString(units->system().valueName()));
 *      childElement.appendChild(text);
 *      element.appendChild(childElement);
 *    }else
 */
    if(m_units) {
      childElement = doc.createElement(QString::fromStdString("Units"));
      text = doc.createTextNode(QString::fromStdString(*m_units));
      childElement.appendChild(text);
      element.appendChild(childElement);
    }
  }

} // detail

// For debug purposes only
std::ostream& operator<<(std::ostream& os, const OSAttributeVariant& attributeVariant) {

    // We use std::visit, filtering out the case where it's monostate
    // Aside from monostate, every possible type is streamable
    // expect AttributeVector that needs a special treatment
    std::visit(
        [&os](const auto& val) {
          // Needed to properly compare the types
          using T = std::remove_cv_t<std::remove_reference_t<decltype(val)>>;

          // If it's a vector of attributes
          if constexpr (std::is_same_v<T, openstudio::Attribute>) {
            // Will end up calling toString, which calls toXml
            os << val;
          // Otherwise, if anything else but monostate
          } else if constexpr (!std::is_same_v<T, std::monostate>) {
            os << std::boolalpha << val;
          }
        },
        attributeVariant);

  return os;
}



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
                                           OSAttributeVariant t_defaultValue)
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
                                           OSAttributeVariant t_defaultValue)
  : name(t_name),
    displayName(t_displayName),
    description(t_description),
    validValueTypes(t_validValueTypes),
    required(t_required),
    defaultValue(t_defaultValue)
{}

Attribute::Attribute(const std::string& name, bool value)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(name, value, boost::none)))
{
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const std::string& name, bool value, const std::string& units)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(name, value, units)))
{
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const std::string& name, bool value, const boost::optional<std::string>& units)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(name, value, units)))
{
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const openstudio::UUID& uuid,
                     const openstudio::UUID& versionUUID,
                     const std::string& name,
                     const boost::optional<std::string>& displayName,
                     bool value,
                     const boost::optional<std::string>& units,
                     const std::string& source)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(uuid,
                                        versionUUID,
                                        name,
                                        displayName,
                                        value,
                                        units,
                                        source)))
{
  OS_ASSERT(m_impl);
}

Attribute::Attribute(const std::string& name, double value)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(name, value, boost::none)))
{
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const std::string& name, double value, const std::string& units)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(name, value, units)))
{
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const std::string& name, double value, const boost::optional<std::string>& units)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(name, value, units)))
{
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const openstudio::UUID& uuid,
                     const openstudio::UUID& versionUUID,
                     const std::string& name,
                     const boost::optional<std::string>& displayName,
                     double value,
                     const boost::optional<std::string>& units,
                     const std::string& source)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(uuid,
                                        versionUUID,
                                        name,
                                        displayName,
                                        value,
                                        units,
                                        source)))
{
  OS_ASSERT(m_impl);
}

Attribute::Attribute(const std::string& name, int value)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(name, value, boost::none)))
{
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const std::string& name, int value, const std::string& units)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(name, value, units)))
{
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const std::string& name, int value, const boost::optional<std::string>& units)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(name, value, units)))
{
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const openstudio::UUID& uuid,
                     const openstudio::UUID& versionUUID,
                     const std::string& name,
                     const boost::optional<std::string>& displayName,
                     int value,
                     const boost::optional<std::string>& units,
                     const std::string& source)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(uuid,
                                        versionUUID,
                                        name,
                                        displayName,
                                        value,
                                        units,
                                        source)))
{
  OS_ASSERT(m_impl);
}

Attribute::Attribute(const std::string& name, unsigned value)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(name, value, boost::none)))
{
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const std::string& name, unsigned value, const std::string& units)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(name, value, units)))
{
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const std::string& name, unsigned value, const boost::optional<std::string>& units)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(name, value, units)))
{
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const openstudio::UUID& uuid,
                     const openstudio::UUID& versionUUID,
                     const std::string& name,
                     const boost::optional<std::string>& displayName,
                     unsigned value,
                     const boost::optional<std::string>& units,
                     const std::string& source)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(uuid,
                                        versionUUID,
                                        name,
                                        displayName,
                                        value,
                                        units,
                                        source)))
{
  OS_ASSERT(m_impl);
}


Attribute::Attribute(const std::string& name, const char* value)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(name, std::string(value), boost::none)))
{
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const std::string& name, const char* value, const std::string& units)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(name, std::string(value), units)))
{
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const std::string& name, const char* value, const boost::optional<std::string>& units)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(name, std::string(value), units)))
{
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const openstudio::UUID& uuid,
                     const openstudio::UUID& versionUUID,
                     const std::string& name,
                     const boost::optional<std::string>& displayName,
                     const char* value,
                     const boost::optional<std::string>& units,
                     const std::string& source)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(uuid,
                                        versionUUID,
                                        name,
                                        displayName,
                                        std::string(value),
                                        units,
                                        source)))
{
  OS_ASSERT(m_impl);
}

Attribute::Attribute(const std::string& name, const std::string& value)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(name, value, boost::none)))
{
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const std::string& name, const std::string& value, const std::string& units)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(name, value, units)))
{
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const std::string& name, const std::string& value, const boost::optional<std::string>& units)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(name, value, units)))
{
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const openstudio::UUID& uuid,
                     const openstudio::UUID& versionUUID,
                     const std::string& name,
                     const boost::optional<std::string>& displayName,
                     const std::string& value,
                     const boost::optional<std::string>& units,
                     const std::string& source)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(uuid,
                                        versionUUID,
                                        name,
                                        displayName,
                                        value,
                                        units,
                                        source)))
{
  OS_ASSERT(m_impl);
}

Attribute::Attribute(const std::string& name, const std::vector<openstudio::Attribute>& value)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(name, value, boost::none)))
{
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const std::string& name, const std::vector<openstudio::Attribute>& value, const std::string& units)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(name, value, units)))
{
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const std::string& name, const std::vector<openstudio::Attribute>& value, const boost::optional<std::string>& units)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(new detail::Attribute_Impl(name, value, units)))
{
  OS_ASSERT(m_impl);
}
Attribute::Attribute(const openstudio::UUID& uuid,
                     const openstudio::UUID& versionUUID,
                     const std::string& name,
                     const boost::optional<std::string>& displayName,
                     const std::vector<openstudio::Attribute>& value,
                     const boost::optional<std::string>& units,
                     const std::string& source)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(
             new detail::Attribute_Impl(uuid,
                                        versionUUID,
                                        name,
                                        displayName,
                                        value,
                                        units,
                                        source)))
{
  OS_ASSERT(m_impl);
}

Attribute::Attribute(const QDomElement& element)
  : m_impl(std::shared_ptr<detail::Attribute_Impl>(new detail::Attribute_Impl(element)))
{
  OS_ASSERT(m_impl);
}

Attribute::Attribute(const Attribute& other)
  : m_impl(other.m_impl)
{
  OS_ASSERT(m_impl);
}

Attribute Attribute::clone() const {
  Attribute result(std::shared_ptr<detail::Attribute_Impl>(new detail::Attribute_Impl(*m_impl)));
  return result;
}

Attribute::Attribute(const std::shared_ptr<detail::Attribute_Impl>& impl)
  : m_impl(impl)
{
  OS_ASSERT(m_impl);
}

boost::optional<Attribute> Attribute::loadFromXml(const openstudio::path& path)
{
  boost::optional<Attribute> result;

  if (openstudio::filesystem::exists(path)){
    try{

      openstudio::filesystem::ifstream file(path, std::ios_base::binary);
      QDomDocument qDomDocument;
      qDomDocument.setContent(openstudio::filesystem::read_as_QByteArray(file));
      file.close();

      result = Attribute(qDomDocument.documentElement());
    }catch(...){
    }
  }

  return result;
}

boost::optional<Attribute> Attribute::loadFromXml(const QDomDocument& doc) {
  boost::optional<Attribute> result;
  try {
    result = Attribute(doc.documentElement());
  }
  catch (...) {
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

boost::optional<std::string> Attribute::displayName(bool returnName) const
{
  return m_impl->displayName(returnName);
}

bool Attribute::setDisplayName(const std::string& displayName)
{
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

AttributeValueType Attribute::valueType() const
{
  return m_impl->valueType();
}

bool Attribute::hasValue() const {
  return m_impl->hasValue();
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
    openstudio::filesystem::ofstream file(path);
    openstudio::filesystem::write(file, this->toXml().toString(2));
    file.close();
    result = true;
  }
  catch( const std::exception &e) {
    LOG_FREE(Error, "openstudio.Attribute", "Error saving to XML: " << e.what());
  }

  return result;
}

bool Attribute::operator==(const Attribute& other) const
{
  return m_impl->operator==(other);
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
  for (int v : value) {
    Attribute attribute(std::string(),v);
    valueAsAttributes.push_back(attribute);
  }
  return Attribute(name,valueAsAttributes);
}

Attribute createAttributeFromVector(const std::string& name, std::vector<double> value) {
  AttributeVector valueAsAttributes;
  for (double v : value) {
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
  for (const Attribute& a : attributes) {
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
  for (const Attribute& a : attributes) {
    result.push_back(a.valueAsDouble());
  }
  return result;
}

} // openstudio

