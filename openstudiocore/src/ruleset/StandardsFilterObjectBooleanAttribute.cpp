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

#include <ruleset/StandardsFilterObjectBooleanAttribute.hpp>
#include <ruleset/StandardsFilterObjectBooleanAttribute_Impl.hpp>

#include <ruleset/StandardsFilterAvailableObject_Impl.hpp>

#include <utilities/core/Compare.hpp>

#include <QDomDocument>
#include <QDomElement>

#include <iomanip>

namespace openstudio {
namespace ruleset {

namespace detail {

  StandardsFilterObjectBooleanAttribute_Impl::StandardsFilterObjectBooleanAttribute_Impl(
      const StandardsFilterAvailableObject& object,
      const standardsinterface::BooleanAttribute& attribute,
      bool value)
    : StandardsFilterObjectAttribute_Impl(object),
      m_attribute(attribute),
      m_value(value)
  {}

  StandardsFilterObjectBooleanAttribute_Impl::StandardsFilterObjectBooleanAttribute_Impl(
      const StandardsFilterAvailableObject& object,
      const standardsinterface::BooleanAttribute& attribute,
      bool value,
      const UUID& uuid,
      const UUID& versionUUID)
    : StandardsFilterObjectAttribute_Impl(object, uuid, versionUUID),
      m_attribute(attribute),
      m_value(value)
  {}

  StandardsFilterObjectBooleanAttribute_Impl::StandardsFilterObjectBooleanAttribute_Impl(const QDomElement& element)
    : StandardsFilterObjectAttribute_Impl(element)
  {
    BOOST_ASSERT(!element.isNull());
    BOOST_ASSERT(element.tagName() == toQString(this->xmlElementName()));
  }

  RulesetObject StandardsFilterObjectBooleanAttribute_Impl::clone() const {
    boost::shared_ptr<StandardsFilterObjectBooleanAttribute_Impl> impl(new StandardsFilterObjectBooleanAttribute_Impl(*this));
    return StandardsFilterObjectBooleanAttribute(impl).cast<RulesetObject>();
  }

  RulesetObject StandardsFilterObjectBooleanAttribute_Impl::deepClone() const {
    StandardsFilterObjectBooleanAttribute result = clone().cast<StandardsFilterObjectBooleanAttribute>();
    result.mf_setObject(object().deepClone().cast<StandardsFilterAvailableObject>());
    return result.cast<RulesetObject>();
  }

  std::string StandardsFilterObjectBooleanAttribute_Impl::factName() const {
    return ("booleanAttribute" + this->printUUID());
  }

  std::string StandardsFilterObjectBooleanAttribute_Impl::shortDescription() const {
    std::stringstream ss;
    if (!value()) { ss << "not "; }
    ss << attribute().valueName();
    return ss.str();
  }

  standardsinterface::BooleanAttribute StandardsFilterObjectBooleanAttribute_Impl::attribute() const {
    return m_attribute;
  }

  bool StandardsFilterObjectBooleanAttribute_Impl::value() const {
    return m_value;
  }

  void StandardsFilterObjectBooleanAttribute_Impl::negate() {
    setValue(!value());
  }

  bool StandardsFilterObjectBooleanAttribute_Impl::setValue(const std::string& value) {
    std::string wv(value);
    boost::trim(wv);
    if (istringEqual(wv,"true") || istringEqual(wv,"t") || 
        istringEqual(wv,"yes") || istringEqual(wv,"y")) 
    {
      return setValue(true);
    }
    else if (istringEqual(wv,"false") || istringEqual(wv,"f") || 
             istringEqual(wv,"no") || istringEqual(wv,"n")) 
    {
      return setValue(false);
    }
    
    return false;
  }

  void StandardsFilterObjectBooleanAttribute_Impl::setAttribute(
    const standardsinterface::BooleanAttribute& attribute)
  {
    m_attribute = attribute;
    this->onChange();
  }

  bool StandardsFilterObjectBooleanAttribute_Impl::setValue(bool value) {
    m_value = value;
    this->onChange();
    return true;
  }

  std::string StandardsFilterObjectBooleanAttribute_Impl::print() const {
    std::stringstream ss;
    ss << "    ?" << factName() << " <- (BooleanAttribute ";
    unsigned indentSize = ss.str().length();
    ss << "(modelId " << m_object.modelId() << ")" << std::endl;
    ss << std::setw(indentSize) << " " << "(objectId ?" << m_object.factName() << "_ObjectId)" << std::endl;
    ss << std::setw(indentSize) << " " << "(name ?" << factName() << "_Name&\"" << m_attribute.valueName() << "\")" << std::endl;
    ss << std::setw(indentSize) << " " << "(value ?" << factName() << "_Value))" << std::endl;
    ss << "    (test (== ?" << factName() << "_Value ";
    if (value()) { ss << "TRUE))"; }
    else { ss << "FALSE))"; }
    ss << std::endl;
    return ss.str();
  }

  std::string StandardsFilterObjectBooleanAttribute_Impl::printForMessage() const {
    std::stringstream ss;
    ss << "Boolean Attribute '" << m_attribute.valueName() << "' == ";
    if (value()) { ss << "TRUE"; }
    else { ss << "FALSE"; }
    return ss.str();
  }

  std::string StandardsFilterObjectBooleanAttribute_Impl::xmlElementName() const
  {
    return StandardsFilterObjectBooleanAttribute::xmlElementName();
  }

  void StandardsFilterObjectBooleanAttribute_Impl::writeValues(QDomDocument& doc, QDomElement& element) const
  {
    StandardsFilterObjectAttribute_Impl::writeValues(doc, element);
  }

  bool StandardsFilterObjectBooleanAttribute_Impl::equivalentData(const RulesetObject& other) const {
    OptionalStandardsFilterObjectBooleanAttribute oBooleanAttribute = other.optionalCast<StandardsFilterObjectBooleanAttribute>();
    if (oBooleanAttribute) {
      if ((m_attribute == oBooleanAttribute->attribute()) &&
          (m_value == oBooleanAttribute->value()))
      {
        return StandardsFilterObjectAttribute_Impl::equivalentData(other);
      }
    }
    return false;
  }

  bool StandardsFilterObjectBooleanAttribute_Impl::equivalentAttributeType(
      const StandardsFilterObjectAttribute& other) const 
  {
    OptionalStandardsFilterObjectBooleanAttribute oBooleanAttribute = other.optionalCast<StandardsFilterObjectBooleanAttribute>();
    if (oBooleanAttribute) {
      if (m_attribute == oBooleanAttribute->attribute()) {
        return StandardsFilterObjectAttribute_Impl::equivalentAttributeType(other);
      }
    }
    return false;
  }


} // detail

/// @cond
std::string StandardsFilterObjectBooleanAttribute::xmlElementName()
{
  return "StandardsFilterObjectBooleanAttribute";
}
/// @endcond

StandardsFilterObjectBooleanAttribute::StandardsFilterObjectBooleanAttribute(
    const StandardsFilterAvailableObject& object,
    const standardsinterface::BooleanAttribute& attribute,
    bool value)
  : StandardsFilterObjectAttribute(boost::shared_ptr<detail::StandardsFilterObjectBooleanAttribute_Impl>(new
                              detail::StandardsFilterObjectBooleanAttribute_Impl(object,attribute,value)))
{}

StandardsFilterObjectBooleanAttribute::StandardsFilterObjectBooleanAttribute(
    const StandardsFilterAvailableObject& object,
    const standardsinterface::BooleanAttribute& attribute,
    bool value,
    const UUID& uuid,
    const UUID& versionUUID)
  : StandardsFilterObjectAttribute(boost::shared_ptr<detail::StandardsFilterObjectBooleanAttribute_Impl>(new
                              detail::StandardsFilterObjectBooleanAttribute_Impl(object,attribute,value, uuid, versionUUID)))
{}

standardsinterface::BooleanAttribute StandardsFilterObjectBooleanAttribute::attribute() const {
  return getImpl<detail::StandardsFilterObjectBooleanAttribute_Impl>()->attribute();
}

bool StandardsFilterObjectBooleanAttribute::value() const {
  return getImpl<detail::StandardsFilterObjectBooleanAttribute_Impl>()->value();
}

void StandardsFilterObjectBooleanAttribute::setAttribute(
    const standardsinterface::BooleanAttribute& attribute)
{
  getImpl<detail::StandardsFilterObjectBooleanAttribute_Impl>()->setAttribute(attribute);
}

bool StandardsFilterObjectBooleanAttribute::setValue(bool value) {
  return getImpl<detail::StandardsFilterObjectBooleanAttribute_Impl>()->setValue(value);
}

/// @cond
StandardsFilterObjectBooleanAttribute::StandardsFilterObjectBooleanAttribute(
    boost::shared_ptr<detail::StandardsFilterObjectBooleanAttribute_Impl> impl)
  : StandardsFilterObjectAttribute(impl)
{}
/// @endcond

} // ruleset
} // openstudio
