/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "IddField.hpp"
#include "IddField_Impl.hpp"

#include "IddRegex.hpp"
#include "CommentRegex.hpp"
#include <utilities/idd/IddFactory.hxx>

#include "../units/UnitFactory.hpp"
#include "../units/IddUnitString.hpp"
#include "../units/QuantityConverter.hpp"
#include "../units/SIUnit.hpp"
#include "../units/IPUnit.hpp"
#include "../units/Quantity.hpp"

#include "../core/ASCIIStrings.hpp"
#include "../core/Assert.hpp"
#include "../core/Containers.hpp"

#include <boost/lexical_cast.hpp>

using boost::algorithm::trim;

namespace openstudio {

namespace detail {

  // CONSTRUCTORS

  /// default constructor for serialization
  IddField_Impl::IddField_Impl() {}

  IddField_Impl::IddField_Impl(const std::string& name, const std::string& objectName) : m_name(name), m_objectName(objectName) {}

  // GETTERS

  std::string IddField_Impl::name() const {
    return m_name;
  }

  std::string IddField_Impl::fieldId() const {
    return m_fieldId;
  }

  const IddFieldProperties& IddField_Impl::properties() const {
    return m_properties;
  }

  boost::optional<Unit> IddField_Impl::getUnits(bool returnIP) const {
    OptionalUnit result;

    if (unitsBasedOnOtherField()) {
      return result;
    }

    if (returnIP) {
      if (OptionalString units = properties().ipUnits) {
        IddUnitString iddUnits(*units);
        // let factory determine best system
        result = createUnit(iddUnits.toStandardUnitString());
        if (result && !iddUnits.prettyString().empty() && result->prettyString(false).empty()) {
          result->setPrettyString(iddUnits.prettyString());
        }
      } else {
        // figure out based on SIUnits
        OptionalUnit siUnit = getUnits(false);
        if (siUnit) {
          // if default is Celsius, return Fahrenheit
          if (siUnit->system() == UnitSystem::Celsius) {
            return convert(Quantity(1.0, *siUnit), UnitSystem(UnitSystem::Fahrenheit)).get().units();
          }
          // if default is not SI, return as-is
          if (siUnit->system() != UnitSystem::SI) {
            return siUnit;
          }
          // if dimensionless, return IP dimensionless
          if (siUnit->standardString(false).empty()) {
            return IPUnit();
          }
          UnitSystem sys(UnitSystem::IP);
          // if pretty string contains W or J, use BTU system
          std::string tempString = siUnit->prettyString(false);
          if (!tempString.empty()) {
            Unit tempUnit = parseUnitString(tempString);
            if ((tempUnit.baseUnitExponent("W") != 0) || (tempUnit.baseUnitExponent("J") != 0)) {
              sys = UnitSystem::BTU;
            }
          }
          // otherwise, if standard string contains m and s, use CFM system
          if ((sys == UnitSystem::IP) && (siUnit->baseUnitExponent("s") != 0) && (siUnit->baseUnitExponent("m") != 0)) {
            sys = UnitSystem::CFM;
          }
          return convert(Quantity(1.0, *siUnit), sys).get().units();
        }
      }
    } else {
      if (OptionalString units = properties().units) {
        IddUnitString iddUnits(*units);
        // even here let factory determine best system (SI already preferred)
        result = createUnit(iddUnits.toStandardUnitString());
        if (result && !iddUnits.prettyString().empty() && result->prettyString(false).empty()) {
          result->setPrettyString(iddUnits.prettyString());
        }
      } else if (properties().type == IddFieldType::RealType) {
        // dimensionless by default
        return SIUnit();
      }
    }

    return result;
  }

  bool IddField_Impl::unitsBasedOnOtherField() const {
    if (OptionalString units = properties().units) {
      if (boost::regex_match(*units, boost::regex("BasedOnField A\\d+"))) {
        return true;
      }
    }
    return false;
  }

  OptionalIddKey IddField_Impl::getKey(const std::string& keyName) const {
    OptionalIddKey result;
    for (const IddKey& key : m_keys) {
      if (boost::iequals(key.name(), keyName)) {
        result = key;
        break;
      }
    }
    return result;
  }

  IddKeyVector IddField_Impl::keys() const {
    return m_keys;
  }

  // SETTERS

  void IddField_Impl::setName(const std::string& name) {
    m_name = name;
  }

  void IddField_Impl::incrementFieldId(const boost::regex& fieldType) {
    boost::regex fieldIdRegex("([AN])([0-9]+)");
    boost::smatch m;
    bool ok = boost::regex_match(m_fieldId, m, fieldIdRegex);
    OS_ASSERT(ok);
    std::string letterStr(m[1].first, m[1].second);
    if (boost::regex_match(letterStr, fieldType)) {
      std::string numberStr(m[2].first, m[2].second);
      int n = boost::lexical_cast<int>(numberStr);
      ++n;
      m_fieldId = letterStr + boost::lexical_cast<std::string>(n);
    }
  }

  // QUERIES

  bool IddField_Impl::isNameField() const {
    return (((properties().references.size() > 0) && (properties().objectLists.size() == 0))
            || (boost::iequals("Name", name()) && ((properties().type == IddFieldType::AlphaType) || (properties().type == IddFieldType::NodeType))));
  }

  bool IddField_Impl::isObjectListField() const {
    if ((properties().type == IddFieldType::ObjectListType) && (!properties().objectLists.empty())) {
      return true;
    }
    return false;
  }

  bool IddField_Impl::operator==(const IddField_Impl& other) const {
    if (this == &other) {
      return true;
    }

    if (m_name != other.m_name) {
      return false;
    }
    if (m_fieldId != other.m_fieldId) {
      return false;
    }
    if (m_objectName != other.m_objectName) {
      return false;
    }
    if (m_properties != other.m_properties) {
      return false;
    }
    if (m_keys != other.m_keys) {
      return false;
    }

    return true;
  }

  bool IddField_Impl::operator!=(const IddField_Impl& other) const {
    return !(*this == other);
  }

  // SERIALIZATION

  std::shared_ptr<IddField_Impl> IddField_Impl::load(const std::string& name, const std::string& text, const std::string& objectName) {

    std::shared_ptr<IddField_Impl> result;
    IddField_Impl iddFieldImpl(name, objectName);

    try {
      iddFieldImpl.parse(text);
    } catch (...) {
      return result;
    }

    result = std::shared_ptr<IddField_Impl>(new IddField_Impl(iddFieldImpl));
    return result;
  }

  std::ostream& IddField_Impl::print(std::ostream& os, bool lastField) const {
    std::string separator = (lastField ? std::string(";") : std::string(","));

    os << "  " << m_fieldId << separator;

    if (m_name != m_fieldId) {
      os << " \\field " << m_name;
    }

    os << std::endl;

    m_properties.print(os);

    for (const auto& key : m_keys) {
      key.print(os);
    }

    return os;
  }

  void IddField_Impl::parse(const std::string& text) {
    boost::smatch matches;
    if (boost::regex_search(text, matches, iddRegex::field())) {
      // find and parse the field text
      std::string fieldTypeChar(matches[1].first, matches[1].second);
      std::string fieldTypeNumber(matches[2].first, matches[2].second);
      std::string fieldProperties(matches[3].first, matches[3].second);

      // keep track of field id
      m_fieldId = fieldTypeChar + fieldTypeNumber;

      // check for base content type
      if (boost::iequals(fieldTypeChar, "A")) {
        m_properties.type = IddFieldType(IddFieldType::AlphaType);
      } else if (boost::iequals(fieldTypeChar, "N")) {
        // default numerics to real, can be overwritten later
        m_properties.type = IddFieldType(IddFieldType::RealType);
      } else {
        LOG_AND_THROW("Unknown field type identifier found: '" << fieldTypeChar << "'");
      }

      // parse all the properties
      while (boost::regex_search(fieldProperties, matches, iddRegex::metaDataComment())) {
        std::string thisProperty(matches[1].first, matches[1].second);
        openstudio::ascii_trim(thisProperty);
        parseProperty(thisProperty);

        fieldProperties = std::string(matches[2].first, matches[2].second);
        openstudio::ascii_trim(fieldProperties);
      }

      if (!((boost::regex_match(fieldProperties, commentRegex::whitespaceOnlyBlock()))
            || (boost::regex_match(fieldProperties, iddRegex::commentOnlyLine())))) {
        LOG_AND_THROW("Unable to parse remaining fields: '" << fieldProperties << "'");
      }
    } else {
      LOG_AND_THROW("Field text does not match expected pattern: '" << text << "'");
    }

    if (m_properties.type == IddFieldType::ChoiceType) {
      // if this is a choice, assert we have some keys
      if (m_keys.empty()) {
        LOG(Error, "Field is of type choice but keys are empty: '" << m_name << "'");
      }
    } else {
      // else assert we have no keys
      if (!m_keys.empty()) {
        LOG(Error, "Field is not of type choice but has non-empty keys: '" << m_name << "'");
      }
    }

    if (m_properties.type == IddFieldType::UnknownType) {

      LOG_AND_THROW("Field is of unknown type after parsing: '" << m_name << "'");
    }

    // If the field has a default then it is not required. This overrides the idd text.
    if (m_properties.stringDefault) {
      if (m_properties.required) {
        LOG(Info, "Field '" << m_name << "' of object '" << m_objectName << "' is both required and has default value, setting required = false.");
        m_properties.required = false;
      }
    }
  }

  void IddField_Impl::parseProperty(const std::string& text) {
    // this function is called very often and has been identified as a bottleneck
    // that is why some of the optimizations below have been applied

    if (text.size() < 1) {
      return;
    }

    bool notHandled = true;
    boost::smatch matches;

    std::string lowerText = openstudio::ascii_to_lower_copy(text);

    const char index = lowerText[0];

    //sort inside the case statements based on the probability of that value being in the string.(so we don't run 5 unlikely
    //regex tofind the likely one) Keep the case statements in aphabitical order for ease of maintance, since it doesn't
    //effect the speed
    switch (index) {

      case 'a': {
        if (boost::algorithm::starts_with(lowerText, "autosizable")) {
          m_properties.autosizable = true;
          notHandled = false;
        } else if (boost::algorithm::starts_with(lowerText, "autocalculatable")) {
          m_properties.autocalculatable = true;
          notHandled = false;
        }
        break;
      }
      case 'b': {
        if (boost::algorithm::starts_with(lowerText, "begin-extensible")) {
          m_properties.beginExtensible = true;
          notHandled = false;
        }
        break;
      }

      case 'd': {
        if (boost::algorithm::starts_with(lowerText, "default")) {
          OS_ASSERT(boost::regex_search(text, matches, iddRegex::defaultProperty()));
          std::string stringDefault(matches[1].first, matches[1].second);
          openstudio::ascii_trim(stringDefault);
          m_properties.stringDefault = stringDefault;
          notHandled = false;
          // if we are numeric type and not set to autosize, set the numeric property
          if ((m_properties.type == IddFieldType::RealType) || (m_properties.type == IddFieldType::IntegerType)) {
            if (!boost::regex_match(text, iddRegex::automaticDefault())) {
              m_properties.numericDefault = boost::lexical_cast<double>(stringDefault);
            } else {
              // otherwise this is -9999
              m_properties.numericDefault = -9999;
            }
          }
        } else if (boost::algorithm::starts_with(lowerText, "deprecated")) {
          m_properties.deprecated = true;
          notHandled = false;
        }
        break;
      }
      case 'e': {
        if (boost::algorithm::starts_with(lowerText, "external-list")) {
          OS_ASSERT(boost::regex_search(text, matches, iddRegex::externalListProperty()));
          std::string externalList(matches[1].first, matches[1].second);
          openstudio::ascii_trim(externalList);
          m_properties.externalLists.push_back(externalList);
          notHandled = false;
        }

        break;
      }
      case 'f': {
        if (boost::algorithm::starts_with(lowerText, "field")) {
          OS_ASSERT(boost::regex_search(text, matches, iddRegex::nameProperty()));
          std::string fieldName(matches[1].first, matches[1].second);
          openstudio::ascii_trim(fieldName);
          notHandled = false;
          if (!boost::equals(m_name, fieldName)) {
            LOG_AND_THROW("Field name '" << fieldName << "' does not match expected '" << m_name << "' in object '" << m_objectName << "'");
          }
        }
        break;
      }
      case 'i': {
        if (boost::algorithm::starts_with(lowerText, "ip-units")) {
          OS_ASSERT(boost::regex_search(text, matches, iddRegex::ipUnitsProperty()));
          std::string ipUnits(matches[1].first, matches[1].second);
          openstudio::ascii_trim(ipUnits);
          m_properties.ipUnits = ipUnits;
          notHandled = false;
        }
        break;
      }

      case 'k': {
        if (boost::algorithm::starts_with(lowerText, "key")) {
          OS_ASSERT(boost::regex_search(text, matches, iddRegex::keyProperty()));
          std::string keyText(matches[1].first, matches[1].second);
          notHandled = false;
          boost::smatch keyMatches;
          if (boost::regex_search(keyText, keyMatches, iddRegex::contentAndCommentLine())) {
            std::string keyName(keyMatches[1].first, keyMatches[1].second);
            openstudio::ascii_trim(keyName);

            // construct the key
            OptionalIddKey key = IddKey::load(keyName, keyText);

            // add the key to the keys
            if (key) {
              m_keys.push_back(*key);
            } else {
              LOG_AND_THROW("Key could not be loaded from text '" << keyText << "'.");
            }

          } else {
            LOG_AND_THROW("Key name could not be determined from text '" << keyText << "'.");
          }
        }
        break;
      }
      case 'm': {
        if (boost::algorithm::starts_with(lowerText, "minimum")) {
          if (boost::regex_search(text, matches, iddRegex::minExclusiveProperty())) {
            m_properties.minBoundType = IddFieldProperties::ExclusiveBound;
            std::string minExclusive(matches[1].first, matches[1].second);
            openstudio::ascii_trim(minExclusive);
            m_properties.minBoundValue = boost::lexical_cast<double>(minExclusive);
            m_properties.minBoundText = minExclusive;
            notHandled = false;
          } else if (boost::regex_search(text, matches, iddRegex::minInclusiveProperty())) {
            m_properties.minBoundType = IddFieldProperties::InclusiveBound;
            std::string minInclusive(matches[1].first, matches[1].second);
            openstudio::ascii_trim(minInclusive);
            m_properties.minBoundValue = boost::lexical_cast<double>(minInclusive);
            m_properties.minBoundText = minInclusive;
            notHandled = false;
          }
        } else if (boost::algorithm::starts_with(lowerText, "maximum")) {
          if (boost::regex_search(text, matches, iddRegex::maxExclusiveProperty())) {
            m_properties.maxBoundType = IddFieldProperties::ExclusiveBound;
            std::string maxExclusive(matches[1].first, matches[1].second);
            openstudio::ascii_trim(maxExclusive);
            m_properties.maxBoundValue = boost::lexical_cast<double>(maxExclusive);
            m_properties.maxBoundText = maxExclusive;
            notHandled = false;
          } else if (boost::regex_search(text, matches, iddRegex::maxInclusiveProperty())) {
            m_properties.maxBoundType = IddFieldProperties::InclusiveBound;
            std::string maxInclusive(matches[1].first, matches[1].second);
            openstudio::ascii_trim(maxInclusive);
            m_properties.maxBoundValue = boost::lexical_cast<double>(maxInclusive);
            m_properties.maxBoundText = maxInclusive;
            notHandled = false;
          }
        } else if (boost::algorithm::starts_with(lowerText, "memo")) {
          notHandled = false;
          OS_ASSERT(boost::regex_search(text, matches, iddRegex::memoProperty()));
          std::string memo(matches[1].first, matches[1].second);
          trim(memo);
          if (m_properties.note.empty()) {
            m_properties.note = memo;
          } else {
            m_properties.note += "\n" + memo;
          }
        }
        break;
      }
      case 'n': {
        if (boost::algorithm::starts_with(lowerText, "note")) {
          notHandled = false;
          OS_ASSERT(boost::regex_search(text, matches, iddRegex::noteProperty()));
          std::string note(matches[1].first, matches[1].second);
          trim(note);
          if (m_properties.note.empty()) {
            m_properties.note = note;
          } else {
            m_properties.note += "\n" + note;
          }
        }
        break;
      }
      case 'o': {
        if (boost::algorithm::starts_with(lowerText, "object-list")) {
          OS_ASSERT(boost::regex_search(text, matches, iddRegex::objectListProperty()));
          std::string objectList(matches[1].first, matches[1].second);
          openstudio::ascii_trim(objectList);
          m_properties.objectLists.push_back(objectList);
          notHandled = false;
        }
        break;
      }
      case 'r': {
        if (boost::algorithm::starts_with(lowerText, "required-field")) {
          m_properties.required = true;
          notHandled = false;
        } else if (boost::algorithm::starts_with(lowerText, "reference-class-name")) {
          OS_ASSERT(boost::regex_search(text, matches, iddRegex::referenceClassNameProperty()));
          std::string reference(matches[1].first, matches[1].second);
          openstudio::ascii_trim(reference);
          m_properties.referenceClassNames.push_back(reference);
          notHandled = false;
        } else if (boost::algorithm::starts_with(lowerText, "reference")) {
          OS_ASSERT(boost::regex_search(text, matches, iddRegex::referenceProperty()));
          std::string reference(matches[1].first, matches[1].second);
          openstudio::ascii_trim(reference);
          m_properties.references.push_back(reference);
          notHandled = false;
        } else if (boost::algorithm::starts_with(lowerText, "retaincase")) {
          m_properties.retaincase = true;
          notHandled = false;
        }
        break;
      }

      case 't': {
        if (boost::algorithm::starts_with(lowerText, "type")) {
          OS_ASSERT(boost::regex_search(text, matches, iddRegex::typeProperty()));
          std::string fieldType(matches[1].first, matches[1].second);
          openstudio::ascii_trim(fieldType);
          m_properties.type = IddFieldType(fieldType);
          notHandled = false;
        }
        break;
      }
      case 'u': {
        if (boost::algorithm::starts_with(lowerText, "unitsBasedOnField")) {
          // unhandled
          //I like how we spend time comparing to this, but then don't handle it!
          notHandled = false;
        } else if (boost::algorithm::starts_with(lowerText, "units")) {
          OS_ASSERT(boost::regex_search(text, matches, iddRegex::unitsProperty()));
          std::string units(matches[1].first, matches[1].second);
          openstudio::ascii_trim(units);
          m_properties.units = units;
          notHandled = false;
        }
        break;
      }
    }

    if (notHandled) {
      LOG_AND_THROW("Unknown field property text '" << text << "' detected in field '" << m_name << "'");
    }
  }

}  // namespace detail

// CONSTRUCTORS

IddField::IddField() : m_impl(std::shared_ptr<detail::IddField_Impl>(new detail::IddField_Impl())) {}

// GETTERS

std::string IddField::name() const {
  return m_impl->name();
}

std::string IddField::fieldId() const {
  return m_impl->fieldId();
}

/// get properties
const IddFieldProperties& IddField::properties() const {
  return m_impl->properties();
}

boost::optional<Unit> IddField::getUnits(bool returnIP) const {
  return m_impl->getUnits(returnIP);
}

bool IddField::unitsBasedOnOtherField() const {
  return m_impl->unitsBasedOnOtherField();
}

/// get key by name
OptionalIddKey IddField::getKey(const std::string& keyName) const {
  return m_impl->getKey(keyName);
}

/// get all keys
IddKeyVector IddField::keys() const {
  return m_impl->keys();
}

// SETTERS

/// set name
void IddField::setName(const std::string& name) {
  m_impl->setName(name);
}

void IddField::incrementFieldId(const boost::regex& fieldType) {
  m_impl->incrementFieldId(fieldType);
}

// QUERIES

bool IddField::isNameField() const {
  return m_impl->isNameField();
}

bool IddField::isObjectListField() const {
  return m_impl->isObjectListField();
}

bool IddField::operator==(const IddField& other) const {
  return (*m_impl == *(other.m_impl));
}

bool IddField::operator!=(const IddField& other) const {
  return (*m_impl != *(other.m_impl));
}

// SERIALIZATION

OptionalIddField IddField::load(const std::string& name, const std::string& text, const std::string& objectName) {
  std::shared_ptr<detail::IddField_Impl> p = detail::IddField_Impl::load(name, text, objectName);
  if (p) {
    return IddField(p);
  } else {
    return boost::none;
  }
}

std::ostream& IddField::print(std::ostream& os, bool lastField) const {
  return m_impl->print(os, lastField);
}

IddField::IddField(const std::shared_ptr<detail::IddField_Impl>& impl) : m_impl(impl) {}

bool referencesEqual(const IddField& field1, const IddField& field2) {
  bool result = false;

  StringVector refs1 = field1.properties().references;
  StringVector refs2 = field2.properties().references;

  unsigned n = refs2.size();
  BoolVector found(n, false);

  // to be equal, must be same size
  if (refs1.size() == refs2.size()) {
    unsigned start = 0;  // start index for refs2

    // look for refs1 in turn
    for (const std::string& ref1 : refs1) {
      for (unsigned i = start; i < n; ++i) {
        if (!found[i]) {
          // refs2[i] not found yet--see if there is a match
          if (istringEqual(ref1, refs2[i])) {
            found[i] = true;
            if (i == start) {
              // a match and at where we started, so move start marker up
              ++start;
            }
            break;
          }
        } else if (i == start) {
          // refs[start] already matched, so move start marker up
          ++start;
        }
      }
    }
    // tried to match each ref1 with a refs2. see if we were successful.
    if (found == BoolVector(n, true)) {
      result = true;
    }
  }

  return result;
}

}  // namespace openstudio
