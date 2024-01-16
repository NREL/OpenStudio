/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "OSArgument.hpp"

#include "../utilities/idf/WorkspaceObject.hpp"

#include "../utilities/idd/IddObject.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Containers.hpp"
#include "../utilities/core/Compare.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/functional/value_factory.hpp>
#include <json/json.h>

#include <json/value.h>
#include <limits>
#include <sstream>

namespace openstudio {
namespace measure {

  OSArgument OSArgument::clone() const {
    OSArgument result(*this);
    result.m_uuid = createUUID();
    result.m_versionUUID = createUUID();
    return result;
  }

  OSArgument OSArgument::makeBoolArgument(const std::string& name, bool required, bool modelDependent) {
    OSArgument result(name, OSArgumentType::Boolean, required, modelDependent);
    result.setDomainType(OSDomainType::Enumeration);
    result.m_choices.push_back("true");
    result.m_choices.push_back("false");
    return result;
  }

  OSArgument OSArgument::makeDoubleArgument(const std::string& name, bool required, bool modelDependent) {
    OSArgument result(name, OSArgumentType::Double, required, modelDependent);
    result.setDomainType(OSDomainType::Interval);
    return result;
  }

  OSArgument OSArgument::makeIntegerArgument(const std::string& name, bool required, bool modelDependent) {
    OSArgument result(name, OSArgumentType::Integer, required, modelDependent);
    result.setDomainType(OSDomainType::Interval);
    return result;
  }

  OSArgument OSArgument::makeStringArgument(const std::string& name, bool required, bool modelDependent) {
    OSArgument result(name, OSArgumentType::String, required, modelDependent);
    result.setDomainType(OSDomainType::Enumeration);
    return result;
  }

  OSArgument OSArgument::makeChoiceArgument(const std::string& name, const std::vector<std::string>& choices, bool required, bool modelDependent) {
    return makeChoiceArgument(name, choices, choices, required, modelDependent);
  }

  OSArgument OSArgument::makeChoiceArgument(const std::string& name, const std::vector<std::string>& choices,
                                            const std::vector<std::string>& displayNames, bool required, bool modelDependent) {
    OSArgument result(name, OSArgumentType::Choice, required, modelDependent);
    result.setDomainType(OSDomainType::Enumeration);
    result.m_choices = choices;
    result.m_choiceDisplayNames = displayNames;
    return result;
  }

  OSArgument OSArgument::makePathArgument(const std::string& name, bool isRead, const std::string& extension, bool required, bool modelDependent) {
    OSArgument result(name, OSArgumentType::Path, required, modelDependent);
    result.setDomainType(OSDomainType::Enumeration);
    result.m_isRead = isRead;
    result.m_extension = extension;
    return result;
  }

  OSArgument OSArgument::makeSeparatorArgument(const std::string& name, bool modelDependent) {
    OSArgument result(name, OSArgumentType::Separator, false, modelDependent);
    return result;
  }

  //openstudio::UUID OSArgument::uuid() const {
  //  return m_uuid;
  //}

  //openstudio::UUID OSArgument::versionUUID() const {
  //  return m_versionUUID;
  //}

  std::string OSArgument::name() const {
    return m_name;
  }

  std::string OSArgument::displayName() const {
    return m_displayName;
  }

  boost::optional<std::string> OSArgument::description() const {
    return m_description;
  }

  OSArgumentType OSArgument::type() const {
    return m_type;
  }

  boost::optional<std::string> OSArgument::units() const {
    return m_units;
  }

  bool OSArgument::required() const {
    return m_required;
  }

  bool OSArgument::modelDependent() const {
    return m_modelDependent;
  }

  bool OSArgument::hasValue() const {
    // If index is 0, then its std::monostate (=empty)
    return (m_value.index() != 0);
  }

  bool OSArgument::valueAsBool() const {
    if (!hasValue()) {
      LOG_AND_THROW("This argument does not have a value set.")
    }
    if (type() != OSArgumentType::Boolean) {
      LOG_AND_THROW("This argument is of type " << type().valueName() << ", not of type Bool.");
    }

    // Note JM 2019-05-17: This is functionally equivalent to `std::get<bool>(m_value)` except it doesn't risk throwing
    // std::bad_variant_access which isn't available on mac prior to 10.14
    // No need to check if get_if succeeds because we checked the type above
    return *(std::get_if<bool>(&m_value));
  }

  double OSArgument::valueAsDouble() const {
    if (!hasValue()) {
      LOG_AND_THROW("This argument does not have a value set.")
    }
    if ((type() != OSArgumentType::Double) && (type() != OSArgumentType::Integer)) {
      LOG_AND_THROW("This argument is of type " << type().valueName() << ", not of type Double.");
    }

    double result;
    if (type() == OSArgumentType::Double) {
      result = *(std::get_if<double>(&m_value));
    } else {
      result = *(std::get_if<int>(&m_value));
      LOG(Warn, "This argument is of type 'Integer' but returning as a Double as requested. You should consider using valueAsInteger instead");
    }

    return result;
  }

  int OSArgument::valueAsInteger() const {
    if (!hasValue()) {
      LOG_AND_THROW("This argument does not have a value set.")
    }
    if (type() != OSArgumentType::Integer) {
      LOG_AND_THROW("This argument is of type " << type().valueName() << ", not of type Integer.");
    }

    return *(std::get_if<int>(&m_value));
  }

  std::string OSArgument::valueAsString() const {
    if (!hasValue()) {
      LOG_AND_THROW("Argument " << name() << " has no value.");
    }

    return printOSArgumentVariant(m_value);
  }

  openstudio::path OSArgument::valueAsPath() const {
    if (!hasValue()) {
      LOG_AND_THROW("This argument does not have a value set.")
    }
    if (type() != OSArgumentType::Path) {
      LOG_AND_THROW("This argument is of type " << type().valueName() << ", not of type Path.");
    }

    return *(std::get_if<openstudio::path>(&m_value));
  }

  bool OSArgument::hasDefaultValue() const {
    return (m_defaultValue.index() != 0);
  }

  bool OSArgument::defaultValueAsBool() const {
    if (!hasDefaultValue()) {
      LOG_AND_THROW("This argument does not have a default value set.")
    }
    if (type() != OSArgumentType::Boolean) {
      LOG_AND_THROW("This argument is of type " << type().valueName() << ", not of type Bool.");
    }

    return *(std::get_if<bool>(&m_defaultValue));
  }

  double OSArgument::defaultValueAsDouble() const {
    if (!hasDefaultValue()) {
      LOG_AND_THROW("This argument does not have a default value set.")
    }
    if (type() != OSArgumentType::Double) {
      LOG_AND_THROW("This argument is of type " << type().valueName() << ", not of type Double.");
    }

    return *(std::get_if<double>(&m_defaultValue));
  }

  int OSArgument::defaultValueAsInteger() const {
    if (!hasDefaultValue()) {
      LOG_AND_THROW("This argument does not have a default value set.")
    }
    if (type() != OSArgumentType::Integer) {
      LOG_AND_THROW("This argument is of type " << type().valueName() << ", not of type Integer.");
    }

    return *(std::get_if<int>(&m_defaultValue));
  }

  std::string OSArgument::defaultValueAsString() const {
    if (!hasDefaultValue()) {
      LOG_AND_THROW("Argument " << name() << " does not have a default value.");
    }

    return printOSArgumentVariant(m_defaultValue);
  }

  openstudio::path OSArgument::defaultValueAsPath() const {
    if (!hasDefaultValue()) {
      LOG_AND_THROW("This argument does not have a default value set.")
    }
    if (type() != OSArgumentType::Path) {
      LOG_AND_THROW("This argument is of type " << type().valueName() << ", not of type Path.");
    }

    return *(std::get_if<openstudio::path>(&m_defaultValue));
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
    if (type() != OSArgumentType::Boolean) {
      LOG_AND_THROW("This argument is of type " << type().valueName() << ", not of type Bool.");
    }

    std::vector<bool> result;

    for (const OSArgumentVariant& value : m_domain) {
      result.push_back(*(std::get_if<bool>(&value)));
    }
    return result;
  }

  std::vector<double> OSArgument::domainAsDouble() const {
    if (!hasDomain()) {
      LOG_AND_THROW("No domain set for OSArgument '" << name() << "'.");
    }
    if ((type() != OSArgumentType::Double) && (type() != OSArgumentType::Integer)) {
      LOG_AND_THROW("This argument is of type " << type().valueName() << ", not of type Double.");
    }

    std::vector<double> result;

    if (type() == OSArgumentType::Double) {
      for (const OSArgumentVariant& value : m_domain) {
        result.push_back(*(std::get_if<double>(&value)));
      }
    } else {
      // It's an int
      LOG(Warn, "This argument is of type 'Integer' but returning Domain as a Double as requested. "
                "You should consider using domainAsInteger instead");
      for (const OSArgumentVariant& value : m_domain) {
        result.push_back(*(std::get_if<int>(&value)));
      }
    }
    return result;
  }

  std::vector<int> OSArgument::domainAsInteger() const {
    if (!hasDomain()) {
      LOG_AND_THROW("No domain set for OSArgument '" << name() << "'.");
    }
    if (type() != OSArgumentType::Integer) {
      LOG_AND_THROW("This argument is of type " << type().valueName() << ", not of type Integer.");
    }

    std::vector<int> result;

    for (const OSArgumentVariant& value : m_domain) {
      result.push_back(*(std::get_if<int>(&value)));
    }
    return result;
  }

  std::vector<openstudio::path> OSArgument::domainAsPath() const {
    if (!hasDomain()) {
      LOG_AND_THROW("No domain set for OSArgument '" << name() << "'.");
    }
    if (type() != OSArgumentType::Path) {
      LOG_AND_THROW("This argument is of type " << type().valueName() << ", not of type Path.");
    }

    std::vector<openstudio::path> result;

    for (const OSArgumentVariant& value : m_domain) {
      result.push_back(*(std::get_if<openstudio::path>(&value)));
    }
    return result;
  }

  std::vector<std::string> OSArgument::domainAsString() const {
    if (!hasDomain()) {
      LOG_AND_THROW("No domain set for OSArgument '" << name() << "'.");
    }

    // TODO: add check for arg type?
    std::vector<std::string> result;

    for (const OSArgumentVariant& value : m_domain) {
      result.push_back(printOSArgumentVariant(value));
    }
    return result;
  }

  std::vector<std::string> OSArgument::choiceValues() const {
    return m_choices;
  }

  std::vector<std::string> OSArgument::choiceValueDisplayNames() const {
    return m_choiceDisplayNames;
  }

  std::string OSArgument::valueDisplayName() const {
    std::string valueString = valueAsString();
    int index = int(std::find(m_choices.begin(), m_choices.end(), valueString) - m_choices.begin());
    if (index < int(m_choiceDisplayNames.size())) {
      return m_choiceDisplayNames[index];
    }
    return valueString;
  }

  std::string OSArgument::defaultValueDisplayName() const {
    std::string defaultValueString = defaultValueAsString();
    int index = int(std::find(m_choices.begin(), m_choices.end(), defaultValueString) - m_choices.begin());
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

  void OSArgument::setDescription(const std::string& description) {
    m_description = description;
    onChange();
  }

  void OSArgument::setUnits(const std::string& units) {
    m_units = units;
    onChange();
  }

  bool OSArgument::setValue(bool value) {
    bool result = false;
    if (m_type == OSArgumentType::Boolean) {
      m_value = value;
      OS_ASSERT(hasValue());
      onChange();
      result = true;
    }
    return result;
  }

  bool OSArgument::setValue(double value) {
    bool result = false;
    if (m_type == OSArgumentType::Double) {
      m_value = value;
      OS_ASSERT(hasValue());
      onChange();
      result = true;

    } else if (m_type == OSArgumentType::Integer) {
      // Let a double be assigned to an int, only if said double is really an integer expressed as a double
      int test = (int)floor(value);
      if (test == value) {
        result = setValue(test);
      } else {
        result = false;
      }
    }

    return result;
  }

  bool OSArgument::setValue(int value) {
    bool result = false;
    if (m_type == OSArgumentType::Integer) {
      m_value = value;
      result = true;

    } else if (m_type == OSArgumentType::Double) {
      // If we pass an int, but our type is double, we let it happen
      m_value.emplace<double>(value);
      result = true;
    }

    if (result) {
      OS_ASSERT(hasValue());
      onChange();
    }

    return result;
  }

  bool OSArgument::setValue(const openstudio::path& value) {
    bool result = false;
    if (m_type == OSArgumentType::Path) {
      m_value = value;
      OS_ASSERT(hasValue());
      onChange();
      result = true;
    }
    return result;
  }

  bool OSArgument::setValue(const char* value) {
    return setValue(std::string(value));
  }

  bool OSArgument::setValue(const std::string& value) {
    bool result = setStringInternal(m_value, value);
    if (result) {
      OS_ASSERT(hasValue());
      onChange();
    }
    return result;
  }

  void OSArgument::clearValue() {
    m_value = OSArgumentVariant();
    OS_ASSERT(!hasValue());
    onChange();
  }

  bool OSArgument::setDefaultValue(bool defaultValue) {
    bool result = false;
    if (m_type == OSArgumentType::Boolean) {
      m_defaultValue = defaultValue;
      OS_ASSERT(hasDefaultValue());
      onChange();
      result = true;
    }
    return result;
  }

  bool OSArgument::setDefaultValue(double defaultValue) {
    bool result = false;
    if (m_type == OSArgumentType::Double) {
      m_defaultValue = defaultValue;
      OS_ASSERT(hasDefaultValue());
      onChange();
      result = true;
    } else if (m_type == OSArgumentType::Integer) {
      // Let a double be assigned to an int, only if said double is really an integer expressed as a double
      int test = (int)floor(defaultValue);
      if (test == defaultValue) {
        result = setDefaultValue(test);
      } else {
        result = false;
      }
    }
    return result;
  }

  bool OSArgument::setDefaultValue(int defaultValue) {
    bool result = false;
    if (m_type == OSArgumentType::Integer) {
      m_defaultValue = defaultValue;
      result = true;

    } else if (m_type == OSArgumentType::Double) {
      // If we pass an int, but our type is double, we let it happen
      m_defaultValue.emplace<double>(defaultValue);
      result = true;
    }

    if (result) {
      OS_ASSERT(hasDefaultValue());
      onChange();
    }

    return result;
  }

  bool OSArgument::setDefaultValue(const openstudio::path& defaultValue) {
    bool result = false;
    if (m_type == OSArgumentType::Path) {
      m_defaultValue = defaultValue;
      OS_ASSERT(hasDefaultValue());
      onChange();
      result = true;
    }
    return result;
  }

  bool OSArgument::setDefaultValue(const char* defaultValue) {
    return setDefaultValue(std::string(defaultValue));
  }

  bool OSArgument::setDefaultValue(const std::string& defaultValue) {
    bool result = setStringInternal(m_defaultValue, defaultValue);
    if (result) {
      OS_ASSERT(hasDefaultValue());
      onChange();
    }
    return result;
  }

  bool OSArgument::setDomainType(const OSDomainType& domainType) {
    if (domainType != this->domainType()) {
      // check whether domainType makes sense for type()
      switch (domainType.value()) {
        case OSDomainType::Interval:
          switch (m_type.value()) {
            case OSArgumentType::Boolean:
            case OSArgumentType::String:
            case OSArgumentType::Choice:
            case OSArgumentType::Path:
              LOG(Info, "OSDomainType " << domainType.valueName() << " does not make sense for "
                                        << "arguments of type " << m_type.valueName() << ", such as " << name() << ".");
              return false;
            default:
              break;
          }
          break;
        case OSDomainType::Enumeration:
          break;
        default:
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
        m_domain.push_back(OSArgumentVariant(value));
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
          m_domain.push_back(OSArgumentVariant(value));
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
          m_domain.push_back(OSArgumentVariant(value));
        }
        onChange();
        result = true;
      }
    } else if (m_type == OSArgumentType::Double) {
      if ((m_domainType != OSDomainType::Interval) || (domain.size() == 2u)) {
        // could check for uniqueness, min < max, but pass on that for now
        m_domain.clear();
        for (int value : domain) {
          m_domain.push_back(OSArgumentVariant(double(value)));
        }
        onChange();
        result = true;
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
        m_domain.push_back(OSArgumentVariant(value));
      }
      onChange();
      result = true;
    }
    return result;
  }

  bool OSArgument::setDomain(const std::vector<std::string>& domain) {
    bool result(false);
    if ((m_domainType != OSDomainType::Interval) || (domain.size() == 2u)) {
      // Store the original, in case we fail to set one element
      std::vector<OSArgumentVariant> originalDomain = m_domain;
      m_domain.clear();
      for (const std::string& value : domain) {
        OSArgumentVariant newValue;
        result = setStringInternal(newValue, value);
        if (!result) {
          // Restore the original domain
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

  void OSArgument::clearDomain() {
    m_domain.clear();
  }

  bool OSArgument::setMinValue(double minValue) {
    if (m_type == OSArgumentType::Integer) {
      int test = (int)floor(minValue);
      if (test == minValue) {
        // If int expressed as double (eg: 1.0 when type = int)
        // Then we call the int overload instead
        return setMinValue(test);
      }
      return false;
    } else if (m_type != OSArgumentType::Double) {
      return false;
    }

    double maxValue = std::numeric_limits<double>::max();
    // Try to fetch the existing maxValue
    if (hasDomain() && (m_domainType == OSDomainType::Interval)) {
      std::vector<double> domain = domainAsDouble();
      if (domain.size() == 2) {
        maxValue = domain[1];
      }
    }

    m_domainType = OSDomainType::Interval;
    m_domain.clear();
    m_domain.push_back(OSArgumentVariant(minValue));
    m_domain.push_back(OSArgumentVariant(maxValue));

    onChange();

    return true;
  }

  bool OSArgument::setMinValue(int minValue) {
    if (m_type == OSArgumentType::Double) {
      auto test = (double)minValue;
      return setMinValue(test);
    } else if (m_type != OSArgumentType::Integer) {
      return false;
    }

    int maxValue = std::numeric_limits<int>::max();
    if (hasDomain() && (m_domainType == OSDomainType::Interval)) {
      std::vector<int> domain = domainAsInteger();
      if (domain.size() == 2) {
        maxValue = domain[1];
      }
    }

    m_domainType = OSDomainType::Interval;
    m_domain.clear();
    m_domain.push_back(OSArgumentVariant(minValue));
    m_domain.push_back(OSArgumentVariant(maxValue));

    onChange();

    return true;
  }

  bool OSArgument::setMaxValue(double maxValue) {
    if (m_type == OSArgumentType::Integer) {
      int test = (int)floor(maxValue);
      if (test == maxValue) {
        return setMaxValue(test);
      }
      return false;
    } else if (m_type != OSArgumentType::Double) {
      return false;
    }

    double minValue = std::numeric_limits<double>::lowest();
    if (hasDomain() && (m_domainType == OSDomainType::Interval)) {
      std::vector<double> domain = domainAsDouble();
      if (domain.size() == 2) {
        minValue = domain[0];
      }
    }

    m_domainType = OSDomainType::Interval;
    m_domain.clear();
    m_domain.push_back(OSArgumentVariant(minValue));
    m_domain.push_back(OSArgumentVariant(maxValue));

    onChange();

    return true;
  }

  bool OSArgument::setMaxValue(int maxValue) {
    if (m_type == OSArgumentType::Double) {
      auto test = (double)maxValue;
      return setMaxValue(test);
    } else if (m_type != OSArgumentType::Integer) {
      return false;
    }

    int minValue = std::numeric_limits<int>::lowest();
    if (hasDomain() && (m_domainType == OSDomainType::Interval)) {
      std::vector<int> domain = domainAsInteger();
      if (domain.size() == 2) {
        minValue = domain[0];
      }
    }

    m_domainType = OSDomainType::Interval;
    m_domain.clear();
    m_domain.push_back(OSArgumentVariant(minValue));
    m_domain.push_back(OSArgumentVariant(maxValue));

    onChange();

    return true;
  }

  bool OSArgument::setStringInternal(OSArgumentVariant& variant, const std::string& value) {
    bool result = false;

    if (m_type == OSArgumentType::Boolean) {
      if (openstudio::istringEqual(value, "true")) {
        variant = true;
        result = true;
      } else if (openstudio::istringEqual(value, "false")) {
        variant = false;
        result = true;
      } else {
        LOG(Debug, "Unknown value '" << value << "' for argument of type Bool.");
      }
    } else if (m_type == OSArgumentType::Double) {
      try {
        auto const double_val = std::stod(value, nullptr);
        variant = double_val;
        result = true;
      } catch (std::exception&) {
        LOG(Debug, "Unable to convert value '" << value << "' to argument of type Double.");
      }
    } else if (m_type == OSArgumentType::Integer) {
      try {
        auto const int_val = std::stoi(value, nullptr);
        variant = int_val;
        result = true;
      } catch (std::exception&) {
        LOG(Debug, "Unable to convert value '" << value << "' to argument of type Integer.");
      }
    } else if (m_type == OSArgumentType::String) {
      variant = value;
      result = true;
    } else if (m_type == OSArgumentType::Choice) {
      if (std::find(m_choices.begin(), m_choices.end(), value) != m_choices.end()) {
        variant = value;
        result = true;
      } else {
        // can also set using display name
        auto it = std::find(m_choiceDisplayNames.begin(), m_choiceDisplayNames.end(), value);
        if (it != m_choiceDisplayNames.end()) {
          int index = int(it - m_choiceDisplayNames.begin());
          if (index < int(m_choices.size())) {
            variant = m_choices[index];
            result = true;
          }
        } else {
          LOG(Debug, "Unable to find choice '" << value << "' in either choice list or choice display names list for argument of type Choice.");
        }
      }
    } else if (m_type == OSArgumentType::Path) {
      openstudio::path temp = toPath(value);
      variant = temp;
      result = true;
    }

    // Don't really need to handle the other case, as it can only be a separator

    return result;
  }

  std::string OSArgument::print() const {
    std::stringstream ss;

    // name
    ss << name();
    if (!displayName().empty()) {
      ss << " (" << displayName() << ")";
    }
    ss << '\n';

    // type and required
    ss << type().valueName() << ", ";
    if (required()) {
      ss << "Required";
    } else {
      ss << "Optional";
    }
    ss << '\n';

    // value
    ss << "Value: ";
    if (hasValue()) {
      ss << printValue(false) << " ";
    }
    if (hasDefaultValue()) {
      ss << "(" << printDefaultValue() << ")";
    }
    ss << '\n';

    if (m_type.value() == OSArgumentType::Choice) {
      ss << "Choices:" << '\n';
      int dnn = m_choiceDisplayNames.size();
      for (int i = 0, n = m_choices.size(); i < n; ++i) {
        ss << "  " << m_choices[i];
        if ((i < dnn) && (!m_choiceDisplayNames[i].empty())) {
          ss << " (" << m_choiceDisplayNames[i] << ")";
        }
        ss << '\n';
      }
    }

    if (hasDomain()) {
      ss << m_domainType.valueName() << " Domain: ";
      if (m_domainType == OSDomainType::Interval) {
        OS_ASSERT(m_domain.size() == 2u);
        ss << "[" << printOSArgumentVariant(m_domain[0]) << ", " << printOSArgumentVariant(m_domain[1]) << "]" << '\n';
      } else {
        ss << '\n';
        for (const OSArgumentVariant& value : m_domain) {
          ss << "  " << printOSArgumentVariant(value) << '\n';
        }
      }
    }

    return ss.str();
  }

  std::string OSArgument::printValue(bool printDefault) const {
    std::string result;

    OSArgumentVariant toPrint;
    if (hasValue()) {
      toPrint = m_value;
    } else if (printDefault && hasDefaultValue()) {
      toPrint = m_defaultValue;
    }

    // If not std::monostate (= empty)
    if (toPrint.index() != 0) {
      result = printOSArgumentVariant(toPrint);
    }

    return result;
  }

  std::string OSArgument::printDefaultValue() const {
    std::string result;

    if (hasDefaultValue()) {
      result = printOSArgumentVariant(m_defaultValue);
    }

    return result;
  }

  // ostream operators for OSArgument
  std::ostream& operator<<(std::ostream& os, const OSArgument& arg) {
    os << arg.print();
    return os;
  }

  std::string OSArgument::printOSArgumentVariant(const OSArgumentVariant& toPrint) const {
    OS_ASSERT(toPrint.index() != 0);
    std::stringstream ss;

    // We use std::visit, filtering out the case where it's monostate
    // Aside from monostate, every possible type is streamable
    //std::visit(
    //[&ss](const auto& val){
    ////Needed to properly compare the types
    //using T = std::remove_cv_t<std::remove_reference_t<decltype(val)>>;
    //if constexpr (!std::is_same_v<T, std::monostate>) {
    //ss << val;
    //}
    //},
    //arg);

    // Note JM 2019-05-17: std::visit is problematic on mac below 10.14, because it might throw std::bad_variant_access
    // So we don't use it here. Same with std::get, so we use get_if instead
    if (const auto* p = std::get_if<bool>(&toPrint)) {
      ss << std::boolalpha << *p;
    } else if (const auto* p = std::get_if<double>(&toPrint)) {
      ss << *p;
    } else if (const auto* p = std::get_if<int>(&toPrint)) {
      ss << *p;
    } else if (const auto* p = std::get_if<std::string>(&toPrint)) {
      ss << *p;
    } else if (const auto* p = std::get_if<openstudio::path>(&toPrint)) {
      ss << *p;
    }

    return ss.str();
  }

  OSArgument::OSArgument() : m_uuid(createUUID()), m_versionUUID(createUUID()) {}

  OSArgument::OSArgument(const std::string& name, const OSArgumentType& type, bool required, bool modelDependent)
    : m_uuid(createUUID()),
      m_versionUUID(createUUID()),
      m_name(name),
      m_displayName(name),
      m_type(type),
      m_required(required),
      m_modelDependent(modelDependent),
      m_isRead(false) {}

  void OSArgument::onChange() {
    m_versionUUID = createUUID();
  }

  OSArgument makeChoiceArgumentOfWorkspaceObjects(const std::string& name, const IddObjectType& iddObjectType, const Workspace& workspace,
                                                  bool required) {
    std::vector<std::pair<std::string, std::string>> intermediate;

    std::vector<WorkspaceObject> objects = workspace.getObjectsByType(iddObjectType);
    for (const WorkspaceObject& object : objects) {
      std::string objectName;
      if (object.name()) {
        objectName = object.name().get();
      } else {
        objectName = object.iddObject().type().valueName();
      }
      intermediate.push_back(std::pair<std::string, std::string>(toString(object.handle()), objectName));
    }

    std::sort(intermediate.begin(), intermediate.end(), SecondOfPairLess<std::pair<std::string, std::string>>());

    int n = intermediate.size();
    StringVector choices(n);
    StringVector displayNames(n);
    for (int i = 0; i < n; ++i) {
      choices[i] = intermediate[i].first;
      displayNames[i] = intermediate[i].second;
    }

    return OSArgument::makeChoiceArgument(name, choices, displayNames, required);
  }

  OSArgument makeChoiceArgumentOfWorkspaceObjects(const std::string& name, const std::string& referenceName, const Workspace& workspace,
                                                  bool required) {
    std::vector<std::pair<std::string, std::string>> intermediate;

    std::vector<WorkspaceObject> objects = workspace.getObjectsByReference(referenceName);
    for (const WorkspaceObject& object : objects) {
      intermediate.push_back(std::pair<std::string, std::string>(toString(object.handle()), object.name().get()));
    }

    std::sort(intermediate.begin(), intermediate.end(), SecondOfPairLess<std::pair<std::string, std::string>>());

    int n = intermediate.size();
    StringVector choices(n);
    StringVector displayNames(n);
    for (int i = 0; i < n; ++i) {
      choices[i] = intermediate[i].first;
      displayNames[i] = intermediate[i].second;
    }

    return OSArgument::makeChoiceArgument(name, choices, displayNames, required);
  }

  std::map<std::string, OSArgument> convertOSArgumentVectorToMap(const std::vector<OSArgument>& arguments) {
    std::map<std::string, OSArgument> argMap;
    for (const OSArgument& arg : arguments) {
      argMap.insert(std::make_pair(arg.name(), arg.clone()));
    }
    return argMap;
  }

  Json::Value OSArgument::toJSON() const {
    Json::Value root;
    root["name"] = m_name;
    // root["uid"] = openstudio::removeBraces(m_uuid);
    // root["versionuuid"] = openstudio::removeBraces(m_versionUUID);
    root["display_name"] = m_displayName;
    if (m_description) {
      root["description"] = *m_description;
    }
    root["type"] = m_type.valueName();
    root["required"] = m_required;
    root["model_dependent"] = m_modelDependent;

    if (m_type == OSArgumentType::Boolean) {
      if (hasDefaultValue()) {
        root["default_value"] = defaultValueAsBool();
      }

    } else if (m_type == OSArgumentType::Double) {
      if (m_units) {
        root["units"] = *m_units;
      }
      if (hasDefaultValue()) {
        root["default_value"] = defaultValueAsDouble();
      }
      if (hasDomain()) {
        auto domain = domainAsDouble();
        if (domain.size() != 2) {
          LOG_AND_THROW("Domain for a double should be of size 2!");
        }
        auto& minValue = domain.front();
        auto& maxValue = domain.back();
        if (minValue != std::numeric_limits<double>::lowest()) {
          root["min_value"] = minValue;
        }
        if (maxValue != std::numeric_limits<double>::max()) {
          root["max_value"] = maxValue;
        }
      }

    } else if (m_type == OSArgumentType::Quantity) {
      // Note: This is heavily deprecated, and the std::variant can't contain it
      if (m_units) {
        root["units"] = *m_units;
      }
      if (hasDefaultValue()) {
        root["default_value"] = defaultValueAsDouble();
      }

    } else if (m_type == OSArgumentType::Integer) {
      if (m_units) {
        root["units"] = *m_units;
      }
      if (hasDefaultValue()) {
        root["default_value"] = defaultValueAsInteger();
      }
      if (hasDomain()) {
        auto domain = domainAsInteger();
        if (domain.size() != 2) {
          LOG_AND_THROW("Domain for an integer should be of size 2!");
        }
        auto& minValue = domain.front();
        auto& maxValue = domain.back();
        if (minValue != std::numeric_limits<int>::lowest()) {
          root["min_value"] = minValue;
        }
        if (maxValue != std::numeric_limits<int>::max()) {
          root["max_value"] = maxValue;
        }
      }
    } else if (m_type == OSArgumentType::String) {
      if (hasDefaultValue()) {
        root["default_value"] = defaultValueAsString();
      }
    } else if (m_type == OSArgumentType::Choice) {
      if (hasDefaultValue()) {
        root["default_value"] = defaultValueAsString();
      }
      auto& choiceValues = root["choice_values"];
      choiceValues = Json::arrayValue;  // Without this line, it's "null" (/nil in Ruby) if m_choices is empty, while old CLI had it as `[]`
      for (const auto& choice : m_choices) {
        choiceValues.append(choice);
      }
      auto& choiceDisplayNames = root["choice_display_names"];
      choiceDisplayNames = Json::arrayValue;
      for (const auto& choice : m_choiceDisplayNames) {
        choiceDisplayNames.append(choice);
      }

    } else if (m_type == OSArgumentType::Path) {
      if (hasDefaultValue()) {
        root["default_value"] = openstudio::toString(defaultValueAsPath());
      }

    } else if (m_type == OSArgumentType::Separator) {
      // No-op
    } else {
      LOG_AND_THROW("Unhandled type");
    }

    return root;
  }

  std::string OSArgument::toJSONString() const {
    return toJSON().toStyledString();
  }

}  // namespace measure
}  // namespace openstudio
