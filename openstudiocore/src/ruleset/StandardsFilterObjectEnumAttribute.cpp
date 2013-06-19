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

#include <ruleset/StandardsFilterObjectEnumAttribute.hpp>
#include <ruleset/StandardsFilterObjectEnumAttribute_Impl.hpp>

#include <ruleset/StandardsFilterAvailableObject_Impl.hpp>

#include <standardsinterface/DataDictionary.hpp>

#include <utilities/core/Containers.hpp>

#include <boost/foreach.hpp>

#include <QDomDocument>
#include <QDomElement>

#include <iomanip>

namespace openstudio {
namespace ruleset {

namespace detail {

  StandardsFilterObjectEnumAttribute_Impl::StandardsFilterObjectEnumAttribute_Impl(
      const StandardsFilterAvailableObject& object,
      const standardsinterface::EnumAttribute& attribute,
      const RulesetStringPredicate& predicate,
      const std::string value)
    : StandardsFilterObjectAttribute_Impl(object),
      m_attribute(attribute),
      m_predicate(predicate),
      m_value(value)
  {
    if (!value.empty()) {
      std::string wv = standardsinterface::DataDictionary::standardEnumValue(m_attribute,value);
      if (!wv.empty()) { 
        m_value = wv; 
      }
      else {
        LOG(Warn,"Unable to standardize value '" << m_value << "' for EnumAttribute '" 
            << m_attribute.valueName() << "'.");
      }
    }
  }

  StandardsFilterObjectEnumAttribute_Impl::StandardsFilterObjectEnumAttribute_Impl(
      const StandardsFilterAvailableObject& object,
      const standardsinterface::EnumAttribute& attribute,
      const RulesetStringPredicate& predicate,
      const std::string value,
      const UUID& uuid,
      const UUID& versionUUID)
    : StandardsFilterObjectAttribute_Impl(object, uuid, versionUUID),
      m_attribute(attribute),
      m_predicate(predicate),
      m_value(value)
  {}

  StandardsFilterObjectEnumAttribute_Impl::StandardsFilterObjectEnumAttribute_Impl(const QDomElement& element)
    : StandardsFilterObjectAttribute_Impl(element)
  {
    BOOST_ASSERT(!element.isNull());
    BOOST_ASSERT(element.tagName() == toQString(this->xmlElementName()));
  }

  RulesetObject StandardsFilterObjectEnumAttribute_Impl::clone() const {
    boost::shared_ptr<StandardsFilterObjectEnumAttribute_Impl> impl(new StandardsFilterObjectEnumAttribute_Impl(*this));
    return StandardsFilterObjectEnumAttribute(impl).cast<RulesetObject>();
  }

  RulesetObject StandardsFilterObjectEnumAttribute_Impl::deepClone() const {
    StandardsFilterObjectEnumAttribute result = clone().cast<StandardsFilterObjectEnumAttribute>();
    result.mf_setObject(object().deepClone().cast<StandardsFilterAvailableObject>());
    return result.cast<RulesetObject>();
  }

  std::string StandardsFilterObjectEnumAttribute_Impl::factName() const {
    return ("enumAttribute" + this->printUUID());
  }

  std::string StandardsFilterObjectEnumAttribute_Impl::shortDescription() const {
    std::stringstream ss;
    if (value().size() < 5u) {
      ss << attribute().valueName() << " " << value();
    }
    else {
      ss << value();
    }
    return ss.str();
  }

  standardsinterface::EnumAttribute StandardsFilterObjectEnumAttribute_Impl::attribute() const {
    return m_attribute;
  }

  RulesetStringPredicate StandardsFilterObjectEnumAttribute_Impl::predicate() const {
    return m_predicate;
  }

  std::string StandardsFilterObjectEnumAttribute_Impl::value() const {
    return m_value;
  }

  void StandardsFilterObjectEnumAttribute_Impl::negate() {
    switch (m_predicate.value()) {
      case RulesetStringPredicate::Equals :
        setPredicate(RulesetStringPredicate::NotEquals);
       break;
      case RulesetStringPredicate::NotEquals :
        setPredicate(RulesetStringPredicate::Equals);
       break;
      case RulesetStringPredicate::IEquals :
        setPredicate(RulesetStringPredicate::NotIEquals);
       break;
      case RulesetStringPredicate::NotIEquals :
        setPredicate(RulesetStringPredicate::IEquals);
       break;
      case RulesetStringPredicate::Match :
        setPredicate(RulesetStringPredicate::NotMatch);
       break;
      case RulesetStringPredicate::NotMatch :
        setPredicate(RulesetStringPredicate::Match);
       break;
      default :
       BOOST_ASSERT(false);
    };
  }

  void StandardsFilterObjectEnumAttribute_Impl::setPredicate(
      const RulesetStringPredicate& predicate) 
  {
    m_predicate = predicate;
  }

  bool StandardsFilterObjectEnumAttribute_Impl::setValue(const std::string& value) {
    std::string wv = standardsinterface::DataDictionary::standardEnumValue(m_attribute,value);
    if (!wv.empty()) {
      m_value = wv;
      this->onChange();
      return true;
    }
    return false;
  }

  std::string StandardsFilterObjectEnumAttribute_Impl::print() const {
    std::stringstream ss;
    ss << "    ?" << factName() << " <- (EnumAttribute ";
    unsigned indentSize = ss.str().length();
    ss << "(modelId " << m_object.modelId() << ")" << std::endl;
    ss << std::setw(indentSize) << " " << "(objectId ?" << m_object.factName() << "_ObjectId)" << std::endl;
    ss << std::setw(indentSize) << " " << "(name ?" << factName() << "_Name&\"" << m_attribute.valueName() << "\")" << std::endl;
    ss << std::setw(indentSize) << " " << "(value ?" << factName() << "_Value" << std::endl;
    switch (m_predicate.value()) {
      case RulesetStringPredicate::Equals :
      case RulesetStringPredicate::IEquals :
      case RulesetStringPredicate::Match :
        ss << "&";
       break;
      case RulesetStringPredicate::NotEquals :
      case RulesetStringPredicate::NotIEquals :
      case RulesetStringPredicate::NotMatch :
        ss << "&~"; 
       break;
      default:
       BOOST_ASSERT(false);
    };
    ss << "\"" << m_value << "\"";
    ss << "))" << std::endl;
    return ss.str();
  }


  std::string StandardsFilterObjectEnumAttribute_Impl::printForMessage() const {
    std::stringstream ss;
    ss << "Enum Attribute '" << m_attribute.valueName() << "' in {";
    ss << "'" << m_value << "'";
    ss << "}";
    return ss.str();
  }

  std::string StandardsFilterObjectEnumAttribute_Impl::xmlElementName() const
  {
    return StandardsFilterObjectEnumAttribute::xmlElementName();
  }

  void StandardsFilterObjectEnumAttribute_Impl::writeValues(QDomDocument& doc, QDomElement& element) const
  {
    StandardsFilterObjectAttribute_Impl::writeValues(doc, element);
  }

  bool StandardsFilterObjectEnumAttribute_Impl::equivalentData(const RulesetObject& other) const {
    OptionalStandardsFilterObjectEnumAttribute oEnumAttribute = other.optionalCast<StandardsFilterObjectEnumAttribute>();
    if (oEnumAttribute) {
      if ((m_attribute == oEnumAttribute->attribute()) &&
          (m_value == oEnumAttribute->value()))
      {
        return StandardsFilterObjectAttribute_Impl::equivalentData(other);
      }
    }
    return false;
  }

  bool StandardsFilterObjectEnumAttribute_Impl::equivalentAttributeType(
      const StandardsFilterObjectAttribute& other) const 
  {
    OptionalStandardsFilterObjectEnumAttribute oEnumAttribute = other.optionalCast<StandardsFilterObjectEnumAttribute>();
    if (oEnumAttribute) {
      if (m_attribute == oEnumAttribute->attribute()) {
        return StandardsFilterObjectAttribute_Impl::equivalentAttributeType(other);
      }
    }
    return false;
  }

} // detail

/// @cond
std::string StandardsFilterObjectEnumAttribute::xmlElementName()
{
  return "StandardsFilterObjectEnumAttribute";
}
/// @endcond

StandardsFilterObjectEnumAttribute::StandardsFilterObjectEnumAttribute(
    const StandardsFilterAvailableObject& object,
    const standardsinterface::EnumAttribute& attribute,
    const RulesetStringPredicate& predicate,
    const std::string value)
  : StandardsFilterObjectAttribute(boost::shared_ptr<detail::StandardsFilterObjectEnumAttribute_Impl>(
                              new detail::StandardsFilterObjectEnumAttribute_Impl(object,attribute,predicate,value)))
{}

StandardsFilterObjectEnumAttribute::StandardsFilterObjectEnumAttribute(
    const StandardsFilterAvailableObject& object,
    const standardsinterface::EnumAttribute& attribute,
    const RulesetStringPredicate& predicate,
    const std::string value,
    const UUID& uuid,
    const UUID& versionUUID)
  : StandardsFilterObjectAttribute(boost::shared_ptr<detail::StandardsFilterObjectEnumAttribute_Impl>(
                              new detail::StandardsFilterObjectEnumAttribute_Impl(object,attribute,predicate,value,uuid,versionUUID)))
{}

standardsinterface::EnumAttribute StandardsFilterObjectEnumAttribute::attribute() const {
  return getImpl<detail::StandardsFilterObjectEnumAttribute_Impl>()->attribute();
}

RulesetStringPredicate StandardsFilterObjectEnumAttribute::predicate() const {
  return getImpl<detail::StandardsFilterObjectEnumAttribute_Impl>()->predicate();
}

std::string StandardsFilterObjectEnumAttribute::value() const {
  return getImpl<detail::StandardsFilterObjectEnumAttribute_Impl>()->value();
}

void StandardsFilterObjectEnumAttribute::setPredicate(const RulesetStringPredicate& predicate) {
  getImpl<detail::StandardsFilterObjectEnumAttribute_Impl>()->setPredicate(predicate);
}

/// @cond
StandardsFilterObjectEnumAttribute::StandardsFilterObjectEnumAttribute(
    boost::shared_ptr<detail::StandardsFilterObjectEnumAttribute_Impl> impl)
  : StandardsFilterObjectAttribute(impl)
{}
/// @endcond

} // ruleset
} // openstudio
