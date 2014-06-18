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

#include "OSArgument.hpp"

#include "../utilities/idf/WorkspaceObject.hpp"

#include "../utilities/idd/IddObject.hpp"

#include "../utilities/units/QuantityFactory.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Containers.hpp"
#include "../utilities/core/Compare.hpp"
#include "../utilities/core/Json.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/functional/value_factory.hpp>

#include <sstream>

namespace openstudio {
namespace ruleset {

OSArgument::OSArgument(const UUID& uuid,
                       const UUID& versionUUID,
                       const std::string& name,
                       const std::string& displayName,
                       const OSArgumentType& type,
                       bool required,
                       const boost::optional<std::string>& value,
                       const boost::optional<std::string>& defaultValue,
                       const OSDomainType& domainType,
                       std::vector<std::string>& domain,
                       const std::vector<std::string>& choices,
                       const std::vector<std::string>& choiceDisplayNames,
                       bool isRead,
                       const std::string& extension)
  : m_uuid(uuid),
    m_name(name),
    m_displayName(displayName),
    m_type(type),
    m_required(required),
    m_domainType(domainType),
    m_choices(choices),
    m_choiceDisplayNames(choiceDisplayNames),
    m_isRead(isRead),
    m_extension(extension)
{
  bool result;
  if (value) {
    result = setValue(value.get());
    OS_ASSERT(result);
  }
  if (defaultValue) {
    result = setDefaultValue(defaultValue.get());
    OS_ASSERT(result);
  }
  if (!domain.empty()) {
    result = setDomain(domain);
    OS_ASSERT(result);
  }
  m_versionUUID = versionUUID;
}

OSArgument::OSArgument(const UUID& uuid,
                       const UUID& versionUUID,
                       const std::string& name,
                       const std::string& displayName,
                       const OSArgumentType& type,
                       bool required,
                       const QVariant& value,
                       const QVariant& defaultValue,
                       const OSDomainType& domainType,
                       std::vector<QVariant>& domain,
                       const std::vector<std::string>& choices,
                       const std::vector<std::string>& choiceDisplayNames,
                       bool isRead,
                       const std::string& extension)
  : m_uuid(uuid),
    m_versionUUID(versionUUID),
    m_name(name),
    m_displayName(displayName),
    m_type(type),
    m_required(required),
    m_value(value),
    m_defaultValue(defaultValue),
    m_domainType(domainType),
    m_domain(domain),
    m_choices(choices),
    m_choiceDisplayNames(choiceDisplayNames),
    m_isRead(isRead),
    m_extension(extension)
{}

OSArgument OSArgument::clone() const {
  OSArgument result(*this);
  result.m_uuid = createUUID();
  result.m_versionUUID = createUUID();
  return result;
}

OSArgument OSArgument::makeBoolArgument(const std::string& name,bool required)
{
  OSArgument result(name, OSArgumentType::Boolean, required);
  result.setDomainType(OSDomainType::Enumeration);
  result.m_choices.push_back("true");
  result.m_choices.push_back("false");
  return result;
}

OSArgument OSArgument::makeDoubleArgument(const std::string& name,bool required)
{
  OSArgument result(name, OSArgumentType::Double, required);
  result.setDomainType(OSDomainType::Interval);
  return result;
}

OSArgument OSArgument::makeQuantityArgument(const std::string& name,bool required)
{
  OSArgument result(name, OSArgumentType::Quantity, required);
  result.setDomainType(OSDomainType::Interval);
  return result;
}

OSArgument OSArgument::makeIntegerArgument(const std::string& name,bool required)
{
  OSArgument result(name, OSArgumentType::Integer, required);
  result.setDomainType(OSDomainType::Interval);
  return result;
}

OSArgument OSArgument::makeStringArgument(const std::string& name,bool required)
{
  OSArgument result(name, OSArgumentType::String, required);
  result.setDomainType(OSDomainType::Enumeration);
  return result;
}

OSArgument OSArgument::makeChoiceArgument(const std::string& name,
                                          const std::vector<std::string>& choices,
                                          bool required)
{
  return makeChoiceArgument(name,choices,choices,required);
}

OSArgument OSArgument::makeChoiceArgument(const std::string& name,
                                          const std::vector<std::string>& choices,
                                          const std::vector<std::string>& displayNames,
                                          bool required)
{
  OSArgument result(name, OSArgumentType::Choice, required);
  result.setDomainType(OSDomainType::Enumeration);
  result.m_choices = choices;
  result.m_choiceDisplayNames = displayNames;
  return result;
}

OSArgument OSArgument::makePathArgument(const std::string& name,
                                        bool isRead,
                                        const std::string& extension,
                                        bool required)
{
  OSArgument result(name, OSArgumentType::Path, required);
  result.setDomainType(OSDomainType::Enumeration);
  result.m_isRead = isRead;
  result.m_extension = extension;
  return result;
}

openstudio::UUID OSArgument::uuid() const {
  return m_uuid;
}

openstudio::UUID OSArgument::versionUUID() const {
  return m_versionUUID;
}

std::string OSArgument::name() const {
  return m_name;
}

std::string OSArgument::displayName() const {
  return m_displayName;
}

OSArgumentType OSArgument::type() const {
  return m_type;
}

bool OSArgument::required() const {
  return m_required;
}

bool OSArgument::hasValue() const {
  return (!m_value.isNull());
}

bool OSArgument::valueAsBool() const
{
  if (!hasValue()) {
    LOG_AND_THROW("This argument does not have a value set.")
  }
  if (type() != OSArgumentType::Boolean) {
    LOG_AND_THROW("This argument is of type " << type().valueName() << ", not of type Bool.");
  }
  if ("true" == m_value.toString()){
    return true;
  }
  return false;
}

double OSArgument::valueAsDouble() const
{
  if (!hasValue()) {
    LOG_AND_THROW("This argument does not have a value set.")
  }
  if (type() != OSArgumentType::Double) {
    LOG_AND_THROW("This argument is of type " << type().valueName() << ", not of type Double.");
  }
  return m_value.toDouble();
}

Quantity OSArgument::valueAsQuantity() const
{
  if (!hasValue()) {
    LOG_AND_THROW("This argument does not have a value set.")
  }
  if (type() != OSArgumentType::Quantity) {
    LOG_AND_THROW("This argument is of type " << type().valueName() << ", not of type Quantity.");
  }
  return m_value.value<openstudio::Quantity>();
}

int OSArgument::valueAsInteger() const
{
  if (!hasValue()) {
    LOG_AND_THROW("This argument does not have a value set.")
  }
  if (type() != OSArgumentType::Integer) {
    LOG_AND_THROW("This argument is of type " << type().valueName() << ", not of type Integer.");
  }
  return m_value.toInt();
}

std::string OSArgument::valueAsString() const
{
  if (!hasValue()) {
    LOG_AND_THROW("Argument " << name() << " has no value.");
  }
  return printQVariant(m_value);
}

openstudio::path OSArgument::valueAsPath() const
{
  if (!hasValue()) {
    LOG_AND_THROW("This argument does not have a value set.")
  }
  if (type() != OSArgumentType::Path) {
    LOG_AND_THROW("This argument is of type " << type().valueName() << ", not of type Path.");
  }
  return toPath(m_value.toString());
}

QVariant OSArgument::valueAsQVariant() const {
  return m_value;
}

bool OSArgument::hasDefaultValue() const {
  return (!m_defaultValue.isNull());
}

bool OSArgument::defaultValueAsBool() const
{
  if (!hasDefaultValue()) {
    LOG_AND_THROW("This argument does not have a default value set.")
  }
  if (type() != OSArgumentType::Boolean) {
    LOG_AND_THROW("This argument is of type " << type().valueName() << ", not of type Bool.");
  }
  if ("true" == m_defaultValue.toString()){
    return true;
  }
  return false;
}

double OSArgument::defaultValueAsDouble() const
{
  if (!hasDefaultValue()) {
    LOG_AND_THROW("This argument does not have a default value set.")
  }
  if (type() != OSArgumentType::Double) {
    LOG_AND_THROW("This argument is of type " << type().valueName() << ", not of type Double.");
  }
  return m_defaultValue.toDouble();
}

Quantity OSArgument::defaultValueAsQuantity() const
{
  if (!hasDefaultValue()) {
    LOG_AND_THROW("This argument does not have a default value set.")
  }
  if (type() != OSArgumentType::Quantity) {
    LOG_AND_THROW("This argument is of type " << type().valueName() << ", not of type Quantity.");
  }
  return m_defaultValue.value<openstudio::Quantity>();
}

int OSArgument::defaultValueAsInteger() const
{
  if (!hasDefaultValue()) {
    LOG_AND_THROW("This argument does not have a default value set.")
  }
  if (type() != OSArgumentType::Integer) {
    LOG_AND_THROW("This argument is of type " << type().valueName() << ", not of type Integer.");
  }
  return m_defaultValue.toInt();
}

std::string OSArgument::defaultValueAsString() const
{
  if (!hasDefaultValue()) {
    LOG_AND_THROW("Argument " << name() << " does not have a default value.");
  }
  return printQVariant(m_defaultValue);
}

openstudio::path OSArgument::defaultValueAsPath() const
{
  if (!hasDefaultValue()) {
    LOG_AND_THROW("This argument does not have a default value set.")
  }
  if (type() != OSArgumentType::Path) {
    LOG_AND_THROW("This argument is of type " << type().valueName() << ", not of type Path.");
  }
  return toPath(m_defaultValue.toString());
}

QVariant OSArgument::defaultValueAsQVariant() const {
  return m_defaultValue;
}

bool OSArgument::hasDomain() const {
  return !m_domain.empty();
}

OSDomainType OSArgument::domainType() const {
  return m_domainType;
}

std::vector<bool> OSArgument::domainAsBool() const {
  if (!hasDomain()) {
    LOG_AND_THROW("No domain set for OSArgument '" << name() << "'.");
  }
  BoolVector result;
  for (const QVariant& value : m_domain) {
    if ("true" == value.toString()) {
      result.push_back(true);
    }
    result.push_back(false);
  }
  return result;
}

std::vector<double> OSArgument::domainAsDouble() const {
  if (!hasDomain()) {
    LOG_AND_THROW("No domain set for OSArgument '" << name() << "'.");
  }
  DoubleVector result;
  for (const QVariant& value : m_domain) {
    result.push_back(value.toDouble());
  }
  return result;
}

std::vector<Quantity> OSArgument::domainAsQuantity() const {
  if (!hasDomain()) {
    LOG_AND_THROW("No domain set for OSArgument '" << name() << "'.");
  }
  QuantityVector result;
  for (const QVariant& value : m_domain) {
    result.push_back(value.value<openstudio::Quantity>());
  }
  return result;
}

std::vector<int> OSArgument::domainAsInteger() const {
  if (!hasDomain()) {
    LOG_AND_THROW("No domain set for OSArgument '" << name() << "'.");
  }
  IntVector result;
  for (const QVariant& value : m_domain) {
    result.push_back(value.toInt());
  }
  return result;
}

std::vector<std::string> OSArgument::domainAsString() const {
  if (!hasDomain()) {
    LOG_AND_THROW("No domain set for OSArgument '" << name() << "'.");
  }
  StringVector result;
  for (const QVariant& value : m_domain) {
    result.push_back(printQVariant(value));
  }
  return result;
}

std::vector<openstudio::path> OSArgument::domainAsPath() const {
  if (!hasDomain()) {
    LOG_AND_THROW("No domain set for OSArgument '" << name() << "'.");
  }
  std::vector<openstudio::path> result;
  for (const QVariant& value : m_domain) {
    result.push_back(toPath(value.toString()));
  }
  return result;
}

std::vector<QVariant> OSArgument::domainAsQVariant() const {
  if (!hasDomain()) {
    LOG_AND_THROW("No domain set for OSArgument '" << name() << "'.");
  }
  return m_domain;
}

std::vector<std::string> OSArgument::choiceValues() const {
  return m_choices;
}

std::vector<std::string> OSArgument::choiceValueDisplayNames() const {
  return m_choiceDisplayNames;
}

std::string OSArgument::valueDisplayName() const {
  std::string valueString = valueAsString();
  int index = int(std::find(m_choices.begin(),m_choices.end(),valueString) -
                  m_choices.begin());
  if (index < int(m_choiceDisplayNames.size())) {
    return m_choiceDisplayNames[index];
  }
  return valueString;
}

std::string OSArgument::defaultValueDisplayName() const {
  std::string defaultValueString = defaultValueAsString();
  int index = int(std::find(m_choices.begin(),m_choices.end(),defaultValueString) -
                  m_choices.begin());
  if (index < int(m_choiceDisplayNames.size())) {
    return m_choiceDisplayNames[index];
  }
  return defaultValueString;
}


bool OSArgument::isRead() const {
  return m_isRead;
}

std::string OSArgument::extension() const {
  return m_extension;
}

void OSArgument::setDisplayName(const std::string& displayName) {
  m_displayName = displayName;
  onChange();
}

bool OSArgument::setValue(bool value) {
  bool result = false;
  if (m_type == OSArgumentType::Boolean) {
    if (value) {
      m_value.setValue(QString("true"));
    }
    else {
      m_value.setValue(QString("false"));
    }
    OS_ASSERT(hasValue());
    onChange();
    result = true;
  }
  return result;
}

bool OSArgument::setValue(double value) {
  bool result = false;
  if (m_type == OSArgumentType::Double) {
    m_value.setValue(value);
    OS_ASSERT(hasValue());
    onChange();
    result = true;
  }
  return result;
}

bool OSArgument::setValue(const Quantity& value) {
  bool result = false;
  if (m_type == OSArgumentType::Quantity) {
    m_value = QVariant::fromValue<openstudio::Quantity>(value);
    OS_ASSERT(hasValue());
    onChange();
    result = true;
  }
  return result;
}

bool OSArgument::setValue(int value) {
  bool result = false;
  if (m_type == OSArgumentType::Integer) {
    m_value.setValue(value);
    OS_ASSERT(hasValue());
    onChange();
    result = true;
  }
  else if (m_type == OSArgumentType::Double) {
    result = setValue(double(value));
  }
  return result;
}

bool OSArgument::setValue(const std::string& value) {
  bool result = setStringInternal(m_value, value);
  if (result) {
    OS_ASSERT(hasValue());
    onChange();
  }
  return result;
}

bool OSArgument::setValue(const char* value) {
  return setValue(std::string(value));
}

bool OSArgument::setValue(const openstudio::path& value) {
  bool result = false;
  if (m_type == OSArgumentType::Path) {
    m_value.setValue(toQString(value));
    OS_ASSERT(hasValue());
    onChange();
    result = true;
  }
  return result;
}

void OSArgument::clearValue() {
  m_value = QVariant();
  OS_ASSERT(!hasValue());
  onChange();
}

bool OSArgument::setDefaultValue(bool defaultValue) {
  bool result = false;
  if (m_type == OSArgumentType::Boolean) {
    if (defaultValue) {
      m_defaultValue.setValue(QString("true"));
    }
    else {
      m_defaultValue.setValue(QString("false"));
    }
    OS_ASSERT(hasDefaultValue());
    onChange();
    result = true;
  }
  return result;
}

bool OSArgument::setDefaultValue(double defaultValue) {
  bool result = false;
  if (m_type == OSArgumentType::Double){
    m_defaultValue.setValue(defaultValue);
    OS_ASSERT(hasDefaultValue());
    onChange();
    result = true;
  }
  return result;
}

bool OSArgument::setDefaultValue(const Quantity& value) {
  bool result = false;
  if (m_type == OSArgumentType::Quantity) {
    m_defaultValue = QVariant::fromValue<openstudio::Quantity>(value);
    OS_ASSERT(hasDefaultValue());
    onChange();
    result = true;
  }
  return result;
}

bool OSArgument::setDefaultValue(int defaultValue)
{
  bool result = false;
  if (m_type == OSArgumentType::Integer){
    m_defaultValue.setValue(defaultValue);
    OS_ASSERT(hasDefaultValue());
    onChange();
    result = true;
  }
  else if (m_type == OSArgumentType::Double) {
    result = setDefaultValue(double(defaultValue));
  }
  return result;
}

bool OSArgument::setDefaultValue(const std::string& defaultValue) {
  bool result = setStringInternal(m_defaultValue, defaultValue);
  if (result) {
    OS_ASSERT(hasDefaultValue());
    onChange();
  }
  return result;
}

bool OSArgument::setDefaultValue(const char* defaultValue) {
  return setDefaultValue(std::string(defaultValue));
}

bool OSArgument::setDefaultValue(const openstudio::path& defaultValue) {
  bool result = false;
  if (m_type == OSArgumentType::Path){
    m_defaultValue.setValue(toQString(defaultValue));
    OS_ASSERT(hasDefaultValue());
    onChange();
    result = true;
  }
  return result;
}

bool OSArgument::setDomainType(const OSDomainType& domainType) {
  if (domainType != this->domainType()) {
    // check whether domainType makes sense for type()
    switch (domainType.value()) {
      case OSDomainType::Interval :
        switch (m_type.value()) {
          case OSArgumentType::Boolean :
          case OSArgumentType::String :
          case OSArgumentType::Choice :
          case OSArgumentType::Path :
            LOG(Info,"OSDomainType " << domainType.valueName() << " does not make sense for "
                << "arguments of type " << m_type.valueName() << ", such as " << name() << ".");
            return false;
          default:
           break;
        }
        break;
      case OSDomainType::Enumeration :
        break;
      default :
        OS_ASSERT(false);
    }

    // if so, change and clear current domain
    m_domainType = domainType;
    m_domain.clear();
    onChange();
  }
  return true;
}

bool OSArgument::setDomain(const std::vector<bool>& domain) {
  bool result(false);
  if (m_type == OSArgumentType::Boolean) {
    OS_ASSERT(m_domainType == OSDomainType::Enumeration);
    // could check for uniqueness, but pass on that for now
    m_domain.clear();
    for (bool value : domain) {
      if (value) {
        m_domain.push_back(QVariant(QString("true")));
      }
      else {
        m_domain.push_back(QVariant(QString("false")));
      }
    }
    onChange();
    result = true;
  }
  return result;
}

bool OSArgument::setDomain(const std::vector<double>& domain) {
  bool result(false);
  if (m_type == OSArgumentType::Double) {
    if ((m_domainType != OSDomainType::Interval) || (domain.size() == 2u)) {
      // could check for uniqueness, min < max, but pass on that for now
      m_domain.clear();
      for (double value : domain) {
        m_domain.push_back(QVariant(value));
      }
      onChange();
      result = true;
    }
  }
  return result;
}

bool OSArgument::setDomain(const std::vector<Quantity>& domain) {
  bool result(false);
  if (m_type == OSArgumentType::Quantity) {
    if ((m_domainType != OSDomainType::Interval) || (domain.size() == 2u)) {
      // could check for uniqueness, min < max, but pass on that for now
      m_domain.clear();
      for (const Quantity& value : domain) {
        m_domain.push_back(QVariant::fromValue<openstudio::Quantity>(value));
      }
      onChange();
      result = true;
    }
  }
  return result;
}

bool OSArgument::setDomain(const std::vector<int>& domain) {
  bool result(false);
  if (m_type == OSArgumentType::Integer) {
    if ((m_domainType != OSDomainType::Interval) || (domain.size() == 2u)) {
      // could check for uniqueness, min < max, but pass on that for now
      m_domain.clear();
      for (int value : domain) {
        m_domain.push_back(QVariant(value));
      }
      onChange();
      result = true;
    }
  }
  return result;
}

bool OSArgument::setDomain(const std::vector<std::string>& domain) {
  bool result(false);
  if ((m_domainType != OSDomainType::Interval) || (domain.size() == 2u)) {
    std::vector<QVariant> originalDomain = m_domain;
    m_domain.clear();
    for (const std::string& value : domain) {
      QVariant newValue;
      result = setStringInternal(newValue,value);
      if (!result) {
        m_domain = originalDomain;
        break;
      }
      m_domain.push_back(newValue);
    }
    if (result) {
      onChange();
    }
  }
  return result;
}

bool OSArgument::setDomain(const std::vector<openstudio::path>& domain) {
  bool result(false);
  if (m_type == OSArgumentType::Path) {
    OS_ASSERT(m_domainType == OSDomainType::Enumeration);
    // could check for uniqueness, but pass on that for now
    m_domain.clear();
    for (const openstudio::path& value : domain) {
      m_domain.push_back(QVariant(toQString(value)));
    }
    onChange();
    result = true;
  }
  return result;
}

void OSArgument::clearDomain() {
  m_domain.clear();
}

std::string OSArgument::print() const {
  std::stringstream ss;

  // name
  ss << name();
  if (!displayName().empty()) {
    ss << " (" << displayName() << ")";
  }
  ss << std::endl;

  // type and required
  ss << type().valueName() << ", ";
  if (required()) {
    ss << "Required";
  }
  else {
    ss << "Optional";
  }
  ss << std::endl;

  // value
  ss << "Value: ";
  if (hasValue()) {
    ss << printValue(false) << " ";
  }
  if (hasDefaultValue()) {
    ss << "(" << printDefaultValue() << ")";
  }
  ss << std::endl;

  if (m_type.value() == OSArgumentType::Choice) {
    ss << "Choices:" << std::endl;
    int dnn = m_choiceDisplayNames.size();
    for (int i = 0, n = m_choices.size(); i < n; ++i) {
      ss << "  " << m_choices[i];
      if ((i < dnn) && (!m_choiceDisplayNames[i].empty())) {
        ss << " (" << m_choiceDisplayNames[i] << ")";
      }
      ss << std::endl;
    }
  }

  if (hasDomain()) {
    ss << m_domainType.valueName() << " Domain: ";
    if (m_domainType == OSDomainType::Interval) {
      OS_ASSERT(m_domain.size() == 2u);
      ss << "[" << printQVariant(m_domain[0]) << ", " << printQVariant(m_domain[1]) << "]" << std::endl;
    }
    else {
      ss << std::endl;
      for (const QVariant& value : m_domain) {
        ss << "  " << printQVariant(value) << std::endl;
      }
    }
  }

  return ss.str();

}

std::string OSArgument::printValue(bool printDefault) const {
  std::string result;

  QVariant toPrint;
  if (hasValue()) {
    toPrint = m_value;
  }
  else if (printDefault && hasDefaultValue()) {
    toPrint = m_defaultValue;
  }

  if (!toPrint.isNull()) {
    result = printQVariant(toPrint);
  }

  return result;
}

std::string OSArgument::printDefaultValue() const {
  std::string result;

  if (hasDefaultValue()) {
    result = printQVariant(m_defaultValue);
  }

  return result;
}

OSArgument::OSArgument()
  : m_uuid(createUUID()), m_versionUUID(createUUID())
{}

OSArgument::OSArgument(const std::string& name,
                       const OSArgumentType& type,
                       bool required)
  : m_uuid(createUUID()),
    m_versionUUID(createUUID()),
    m_name(name),
    m_displayName(name),
    m_type(type),
    m_required(required),
    m_isRead(false)
{}

bool OSArgument::setStringInternal(QVariant& variant, const std::string& value) {
  bool result = false;
  if (m_type == OSArgumentType::Boolean) {
    if (value == "true") {
      variant.setValue(QString("true"));
      result = true;
    }
    else if (value == "false") {
      variant.setValue(QString("false"));
      result = true;
    }
  }
  else if (m_type == OSArgumentType::Double) {
    bool test;
    double temp = toQString(value).toDouble(&test);
    if (test){
      variant.setValue(temp);
      result = true;
    }
  }
  else if (m_type == OSArgumentType::Quantity) {
    OptionalQuantity oq = createQuantity(value);
    if (oq) {
      variant = QVariant::fromValue<openstudio::Quantity>(*oq);
      result = true;
    }
  }
  else if (m_type == OSArgumentType::Integer) {
    bool test;
    int temp = toQString(value).toInt(&test);
    if (test){
      variant.setValue(temp);
      result = true;
    }
  }
  else if (m_type == OSArgumentType::String) {
    variant.setValue(toQString(value));
    result = true;
  }
  else if (m_type == OSArgumentType::Choice) {
    if (std::find(m_choices.begin(), m_choices.end(), value) != m_choices.end()){
      variant.setValue(toQString(value));
      result = true;
    }
    else {
      // can also set using display name
      StringVector::const_iterator it = std::find(m_choiceDisplayNames.begin(), m_choiceDisplayNames.end(), value);
      if (it != m_choiceDisplayNames.end()) {
        int index = int(it - m_choiceDisplayNames.begin());
        if (index < int(m_choices.size())) {
          variant.setValue(toQString(m_choices[index]));
          result = true;
        }
      }
    }
  }
  else if (m_type == OSArgumentType::Path) {
    QString temp = toQString(toPath(value));
    if (!temp.isEmpty()){
      variant.setValue(temp);
      result = true;
    }
  }
  return result;
}

std::string OSArgument::printQVariant(const QVariant& toPrint) const {
  OS_ASSERT(!toPrint.isNull());
  std::string result;

  std::stringstream ss;
  switch (m_type.value()) {
    case OSArgumentType::Double:
      result = toString(toPrint.toDouble());
      break;
    case OSArgumentType::Quantity:
      ss << toPrint.value<openstudio::Quantity>();
      result = ss.str();
      break;
    case OSArgumentType::Integer:
      result = boost::lexical_cast<std::string>(toPrint.toInt());
      break;
    case OSArgumentType::Boolean:
    case OSArgumentType::String:
    case OSArgumentType::Choice:
    case OSArgumentType::Path:
      result = toPrint.toString().toStdString();
      break;
    default:
      OS_ASSERT(false);
  }

  return result;
}

void OSArgument::onChange() {
  m_versionUUID = createUUID();
}

std::ostream& operator<<(std::ostream& os, const OSArgument& arg) {
  os << arg.print();
  return os;
}

OSArgument makeChoiceArgumentOfWorkspaceObjects(const std::string& name,
                                                const IddObjectType& iddObjectType,
                                                const Workspace& workspace,
                                                bool required)
{
  std::vector< std::pair<std::string, std::string> > intermediate;

  std::vector<WorkspaceObject> objects = workspace.getObjectsByType(iddObjectType);
  for (const WorkspaceObject& object : objects){
    std::string objectName;
    if (object.name()) {
      objectName = object.name().get();
    }
    else {
      objectName = object.iddObject().type().valueName();
    }
    intermediate.push_back(std::pair<std::string,std::string>(toString(object.handle()),
                                                              objectName));
  }

  std::sort(intermediate.begin(),
            intermediate.end(),
            SecondOfPairLess< std::pair<std::string,std::string> >());

  int n = intermediate.size();
  StringVector choices(n), displayNames(n);
  for (int i = 0; i < n; ++i) {
    choices[i] = intermediate[i].first;
    displayNames[i] = intermediate[i].second;
  }

  return OSArgument::makeChoiceArgument(name,
                                        choices,
                                        displayNames,
                                        required);
}

OSArgument makeChoiceArgumentOfWorkspaceObjects(const std::string& name,
                                                const std::string& referenceName,
                                                const Workspace& workspace,
                                                bool required)
{
  std::vector< std::pair<std::string, std::string> > intermediate;

  std::vector<WorkspaceObject> objects = workspace.getObjectsByReference(referenceName);
  for (const WorkspaceObject& object : objects){
    intermediate.push_back(std::pair<std::string,std::string>(toString(object.handle()),
                                                              object.name().get()));
  }

  std::sort(intermediate.begin(),
            intermediate.end(),
            SecondOfPairLess< std::pair<std::string,std::string> >());

  int n = intermediate.size();
  StringVector choices(n), displayNames(n);
  for (int i = 0; i < n; ++i) {
    choices[i] = intermediate[i].first;
    displayNames[i] = intermediate[i].second;
  }

  return OSArgument::makeChoiceArgument(name,
                                        choices,
                                        displayNames,
                                        required);
}

std::map<std::string,OSArgument> convertOSArgumentVectorToMap(const std::vector<OSArgument>& arguments)
{
  std::map<std::string, OSArgument> argMap;
  for (const OSArgument& arg : arguments) {
    argMap.insert(std::make_pair(arg.name(), arg.clone()));
  }
  return argMap;
}

namespace detail {

  QVariant toVariant(const OSArgument& argument) {
    QVariantMap argumentData;

    argumentData["uuid"] = toQString(removeBraces(argument.uuid()));
    argumentData["version_uuid"] = toQString(removeBraces(argument.versionUUID()));
    argumentData["name"] = toQString(argument.name());
    if (!argument.displayName().empty()) {
      argumentData["display_name"] = toQString(argument.displayName());
    }
    OSArgumentType type = argument.type();
    argumentData["type"] = toQString(type.valueName());
    argumentData["required"] = argument.required();
    if (argument.hasValue()) {
      if (type == OSArgumentType::Quantity) {
        Quantity value = argument.valueAsQuantity();
        argumentData["value"] = value.value();
        argumentData["value_units"] = toQString(value.units().standardString());
      }
      else {
        // use QVariant directly
        argumentData["value"] = argument.valueAsQVariant();
      }
    }
    if (argument.hasDefaultValue()) {
      if (type == OSArgumentType::Quantity) {
        Quantity defaultValue = argument.defaultValueAsQuantity();
        argumentData["default_value"] = defaultValue.value();
        argumentData["default_value_units"] = toQString(defaultValue.units().standardString());
      }
      else {
        // use QVariant directly
        argumentData["default_value"] = argument.defaultValueAsQVariant();
      }
    }
    argumentData["domain_type"] = toQString(argument.domainType().valueName());
    if (argument.hasDomain()) {
      QVariantList domainList;
      int index(0);
      for (const QVariant& dval : argument.domainAsQVariant()) {
        QVariantMap domainValueMap;
        domainValueMap["domain_value_index"] = index;
        if (type == OSArgumentType::Quantity) {
          Quantity q = dval.value<openstudio::Quantity>();
          domainValueMap["value"] = q.value();
          domainValueMap["units"] = toQString(q.units().standardString());
        }
        else {
          domainValueMap["value"] = dval;
        }
        domainList.push_back(domainValueMap);
        ++index;
      }
      argumentData["domain"] = domainList;
    }
    if (type == OSArgumentType::Choice) {
      QVariantList choicesList;
      StringVector displayNames = argument.choiceValueDisplayNames();
      int index(0), displayNamesN(displayNames.size());
      for (const std::string& choice : argument.choiceValues()) {
        QVariantMap choiceMap;
        choiceMap["choice_index"] = index;
        choiceMap["value"] = toQString(choice);
        if (index < displayNamesN) {
          choiceMap["display_name"] = toQString(displayNames[index]);
        }
        choicesList.push_back(choiceMap);
        ++index;
      }
      argumentData["choices"] = QVariant(choicesList);
    }
    if (type == OSArgumentType::Path) {
      argumentData["is_read"] = argument.isRead();
      argumentData["extension"] = toQString(argument.extension());
    }

    return QVariant(argumentData);
  }

  OSArgument toOSArgument(const QVariant& variant, const VersionString& version) {
    QVariantMap map = variant.toMap();

    OSArgumentType type(map["type"].toString().toStdString());

    QVariant value, defaultValue;
    OS_ASSERT(value.isNull() && defaultValue.isNull());
    if (map.contains("value")) {
      if (type == OSArgumentType::Quantity) {
        value = toQuantityQVariant(map,"value","value_units");
      }
      else {
        value = map["value"];
      }
    }
    if (map.contains("default_value")) {
      if (type == OSArgumentType::Quantity) {
        defaultValue = toQuantityQVariant(map,"default_value","default_value_units");
      }
      else {
        defaultValue = map["default_value"];
      }
    }

    std::vector<QVariant> domain;
    if (map.contains("domain")) {
      if (type == OSArgumentType::Quantity) {
        domain = deserializeOrderedVector(
              map["domain"].toList(),
              "domain_value_index",
              std::function<QVariant (QVariant*)>(std::bind(
                                                            toQuantityQVariant,
                                                            std::bind(&QVariant::toMap,std::placeholders::_1),
                                                            "value",
                                                            "units")));
      }
      else {
        domain = deserializeOrderedVector(
              map["domain"].toList(),
              "value",
              "domain_value_index",
              std::function<QVariant (const QVariant&)>(std::bind(boost::value_factory<QVariant>(),std::placeholders::_1)));
      }
    }

    StringVector choices, choiceDisplayNames;
    if (map.contains("choices")) {
      QVariantList choicesList = map["choices"].toList();
      choices = deserializeOrderedVector(
            choicesList,
            "value",
            "choice_index",
            std::function<std::string (QVariant*)>(std::bind(&QString::toStdString,
                                                                 std::bind(&QVariant::toString,std::placeholders::_1))));
      if (!choicesList.empty() && choicesList[0].toMap().contains("display_name")) {
        try {
          choiceDisplayNames = deserializeOrderedVector(
                choicesList,
                "display_name",
                "choice_index",
                std::function<std::string (QVariant*)>(std::bind(&QString::toStdString,
                                                                     std::bind(&QVariant::toString,std::placeholders::_1))));
        }
        catch (...) {
          LOG_FREE(Warn,"openstudio.ruleset.OSArgument","Unable to deserialize partial list of choice display names.");
        }
      }
    }

    return OSArgument(toUUID(map["uuid"].toString().toStdString()),
                      toUUID(map["version_uuid"].toString().toStdString()),
                      map["name"].toString().toStdString(),
                      map.contains("display_name") ? map["display_name"].toString().toStdString() : std::string(),
                      type,
                      map["required"].toBool(),
                      value,
                      defaultValue,
                      OSDomainType(map["domain_type"].toString().toStdString()),
                      domain,
                      choices,
                      choiceDisplayNames,
                      map.contains("is_read") ? map["is_read"].toBool() : false,
                      map.contains("extension") ? map["extension"].toString().toStdString() : std::string());
  }

  QVariant toQuantityQVariant(const QVariantMap& map,
                              const std::string& valueKey,
                              const std::string& unitsKey)
  {
    Quantity q = createQuantity(map[toQString(valueKey)].toDouble(),map[toQString(unitsKey)].toString().toStdString()).get();
    return QVariant::fromValue<openstudio::Quantity>(q);
  }

} // detail

} // ruleset
} // openstudio
