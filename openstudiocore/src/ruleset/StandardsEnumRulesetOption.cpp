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

#include <ruleset/StandardsEnumRulesetOption.hpp>
#include <ruleset/StandardsEnumRulesetOption_Impl.hpp>

#include <standardsinterface/DataDictionary.hpp>

#include <boost/foreach.hpp>

namespace openstudio {
namespace ruleset {

namespace detail {

  StandardsEnumRulesetOption_Impl::StandardsEnumRulesetOption_Impl(
      const standardsinterface::EnumAttribute& enumOption,const std::string& defaultValue)
    : StandardsRulesetOption_Impl(),
      m_enumOption(enumOption),
      m_defaultValue(standardsinterface::DataDictionary::standardEnumValue(enumOption,defaultValue)),
      m_value(m_defaultValue)
  {
    if (!defaultValue.empty() && m_defaultValue.empty()) {
      // defaultValue was not valid value for enumOption
      LOG(Error,defaultValue << " is not a valid default value for the EnumAttribute "
          << enumOption.valueName() << ". No default value will be assigned.");
    }
  }

  StandardsEnumRulesetOption_Impl::StandardsEnumRulesetOption_Impl(
      const standardsinterface::EnumAttribute& enumOption,
      const std::string& defaultValue,
      const std::string& value,
      const UUID& uuid, 
      const UUID& versionUUID)
    : StandardsRulesetOption_Impl(uuid, versionUUID),
      m_enumOption(enumOption),
      m_defaultValue(standardsinterface::DataDictionary::standardEnumValue(enumOption,defaultValue)),
      m_value(value)
  {
    if (!defaultValue.empty() && m_defaultValue.empty()) {
      // defaultValue was not valid value for enumOption
      LOG(Error,defaultValue << " is not a valid default value for the EnumAttribute "
          << enumOption.valueName() << ". No default value will be assigned.");
    }
  }

  RulesetOption StandardsEnumRulesetOption_Impl::clone() const {
     return StandardsEnumRulesetOption(m_enumOption, m_defaultValue, m_value, createUUID(), createUUID());
  }

  std::string StandardsEnumRulesetOption_Impl::name() const {
    return m_enumOption.valueName();
  }

  std::string StandardsEnumRulesetOption_Impl::factName() const {
    std::stringstream ss;
    ss << "RulesetOption_Enum_" << name();
    return ss.str();
  }

  openstudio::standardsinterface::EnumAttribute StandardsEnumRulesetOption_Impl::enumOption() const{
    return m_enumOption;
  }

  std::string StandardsEnumRulesetOption_Impl::value() const { return m_value; }

  std::string StandardsEnumRulesetOption_Impl::defaultValue() const { return m_defaultValue; }

  std::vector<std::string> StandardsEnumRulesetOption_Impl::validValues() const {
    return standardsinterface::DataDictionary::enumValues(m_enumOption);
  }

  bool StandardsEnumRulesetOption_Impl::setValue(const std::string& newValue) {
    if (newValue.empty()) {
      m_value = newValue;
      this->onChange();
      return true;
    }

    std::string standardString;
    standardString = standardsinterface::DataDictionary::standardEnumValue(m_enumOption,newValue);
    if (standardString.empty()) {
      return false;
    }

    m_value = standardString;
    this->onChange();
    return true;
  }

  std::string StandardsEnumRulesetOption_Impl::xmlElementName() const
  {
    return StandardsEnumRulesetOption::xmlElementName();
  }

  std::string StandardsEnumRulesetOption_Impl::printDefinition() const {
    std::stringstream ss;

    // define fact type
    ss << "(deftemplate " << factName() << std::endl;
    ss << "    (slot value (type STRING)" << std::endl;
    ss << "                (allowed-strings";
    BOOST_FOREACH(const std::string& validValue, validValues()) {
      ss << " \"" << validValue << "\"";
    }
    ss << ")";
    if (!m_defaultValue.empty()) {
      ss << std::endl << "                (default \"" << m_defaultValue << "\")";
    }
    ss << ")" << std::endl << ")" << std::endl;

    return ss.str();
  }

  std::string StandardsEnumRulesetOption_Impl::printValue() const {
    std::stringstream ss;
    ss << "(" << factName() << " (value \"" << m_value << "\"))" << std::endl;
    return ss.str();
  }

  bool StandardsEnumRulesetOption_Impl::equivalentData(const RulesetObject& other) const {
    OptionalStandardsEnumRulesetOption oEnumOption = other.optionalCast<StandardsEnumRulesetOption>();
    if (oEnumOption) {
      if ((m_enumOption == oEnumOption->getImpl<StandardsEnumRulesetOption_Impl>()->m_enumOption) &&
          (defaultValue() == oEnumOption->defaultValue()) &&
          (value() == oEnumOption->value())) 
      {
        return StandardsRulesetOption_Impl::equivalentData(other);
      }
    }
    return false;
  }

  bool StandardsEnumRulesetOption_Impl::equivalentOptionType(const StandardsRulesetOption& other) const {
    OptionalStandardsEnumRulesetOption oEnumOption = other.optionalCast<StandardsEnumRulesetOption>();
    if (oEnumOption) {
      if (m_enumOption == oEnumOption->getImpl<StandardsEnumRulesetOption_Impl>()->m_enumOption) {
        return StandardsRulesetOption_Impl::equivalentData(other);
      }
    }
    return false;
  }

} // detail

/// @cond
std::string StandardsEnumRulesetOption::xmlElementName()
{
  return "StandardsEnumRulesetOption";
}
/// @endcond

StandardsEnumRulesetOption::StandardsEnumRulesetOption(const standardsinterface::EnumAttribute& enumOption,
                                                       const std::string& defaultValue)
  : StandardsRulesetOption(boost::shared_ptr<detail::StandardsEnumRulesetOption_Impl>(new detail::StandardsEnumRulesetOption_Impl(enumOption,defaultValue)))
{
  BOOST_ASSERT(getImpl<detail::StandardsEnumRulesetOption_Impl>());
}

StandardsEnumRulesetOption::StandardsEnumRulesetOption(const standardsinterface::EnumAttribute& enumOption,
                                                       const std::string& defaultValue,
                                                       const std::string& value,
                                                       const UUID& uuid,
                                                       const UUID& versionUUID)
  : StandardsRulesetOption(boost::shared_ptr<detail::StandardsEnumRulesetOption_Impl>(new detail::StandardsEnumRulesetOption_Impl(enumOption,defaultValue,value,uuid,versionUUID)))
{
  BOOST_ASSERT(getImpl<detail::StandardsEnumRulesetOption_Impl>());
}

openstudio::standardsinterface::EnumAttribute StandardsEnumRulesetOption::enumOption() const{
  return getImpl<detail::StandardsEnumRulesetOption_Impl>()->enumOption();
}

std::string StandardsEnumRulesetOption::value() const {
  return getImpl<detail::StandardsEnumRulesetOption_Impl>()->value();
}

std::string StandardsEnumRulesetOption::defaultValue() const {
  return getImpl<detail::StandardsEnumRulesetOption_Impl>()->defaultValue();
}

std::vector<std::string> StandardsEnumRulesetOption::validValues() const {
  return getImpl<detail::StandardsEnumRulesetOption_Impl>()->validValues();
}

bool StandardsEnumRulesetOption::setValue(const std::string& newValue) {
  return getImpl<detail::StandardsEnumRulesetOption_Impl>()->setValue(newValue);
}

/// @cond
StandardsEnumRulesetOption::StandardsEnumRulesetOption(const boost::shared_ptr<detail::StandardsEnumRulesetOption_Impl>& impl)
  : StandardsRulesetOption(impl)
{}
/// @endcond

} // ruleset
} // openstudio
