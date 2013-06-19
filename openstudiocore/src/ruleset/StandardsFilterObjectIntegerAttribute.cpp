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

#include <ruleset/StandardsFilterObjectIntegerAttribute.hpp>
#include <ruleset/StandardsFilterObjectIntegerAttribute_Impl.hpp>

#include <ruleset/StandardsFilterAvailableObject_Impl.hpp>

#include <QDomDocument>
#include <QDomElement>

#include <boost/lexical_cast.hpp>

#include <iomanip>

namespace openstudio {
namespace ruleset {

namespace detail {

  StandardsFilterObjectIntegerAttribute_Impl::StandardsFilterObjectIntegerAttribute_Impl(
      const StandardsFilterAvailableObject& object,
      const standardsinterface::IntegerAttribute& attribute,
      const RulesetNumericalPredicate& predicate,
      int value,
      const std::string& unitString)
    : StandardsFilterObjectNumericalAttribute_Impl(object,predicate,unitString),
      m_attribute(attribute),
      m_value(value)
  {}

  StandardsFilterObjectIntegerAttribute_Impl::StandardsFilterObjectIntegerAttribute_Impl(
      const StandardsFilterAvailableObject& object,
      const standardsinterface::IntegerAttribute& attribute,
      const RulesetNumericalPredicate& predicate,
      int value,
      const std::string& unitString,
      const UUID& uuid,
      const UUID& versionUUID)
    : StandardsFilterObjectNumericalAttribute_Impl(object, predicate, unitString, uuid, versionUUID),
      m_attribute(attribute),
      m_value(value)
  {}

  StandardsFilterObjectIntegerAttribute_Impl::StandardsFilterObjectIntegerAttribute_Impl(const QDomElement& element)
    : StandardsFilterObjectNumericalAttribute_Impl(element)
  {
    BOOST_ASSERT(!element.isNull());
    BOOST_ASSERT(element.tagName() == toQString(this->xmlElementName()));
  }

  RulesetObject StandardsFilterObjectIntegerAttribute_Impl::clone() const {
    boost::shared_ptr<StandardsFilterObjectIntegerAttribute_Impl> impl(new StandardsFilterObjectIntegerAttribute_Impl(*this));
    return StandardsFilterObjectIntegerAttribute(impl).cast<RulesetObject>();
  }

  RulesetObject StandardsFilterObjectIntegerAttribute_Impl::deepClone() const {
    StandardsFilterObjectIntegerAttribute result = clone().cast<StandardsFilterObjectIntegerAttribute>();
    result.mf_setObject(object().deepClone().cast<StandardsFilterAvailableObject>());
    return result.cast<RulesetObject>();
  }

  std::string StandardsFilterObjectIntegerAttribute_Impl::factName() const {
    return ("integerAttribute" + this->printUUID());
  }

  std::string StandardsFilterObjectIntegerAttribute_Impl::shortDescription() const {
    return attribute().valueName();
  }

  standardsinterface::IntegerAttribute StandardsFilterObjectIntegerAttribute_Impl::attribute() const {
    return m_attribute;
  }

  int StandardsFilterObjectIntegerAttribute_Impl::value() const {
    return m_value;
  }

  void StandardsFilterObjectIntegerAttribute_Impl::setAttribute(
      const standardsinterface::IntegerAttribute& attribute)
  {
    m_attribute = attribute;
    this->onChange();
  }

  bool StandardsFilterObjectIntegerAttribute_Impl::setValue(const std::string& value) {
    try {
      int ival = boost::lexical_cast<int>(value);
      return setValue(ival);
    }
    catch (...) { return false; }
  }

  bool StandardsFilterObjectIntegerAttribute_Impl::setValue(int value) {
    m_value = value;
    this->onChange();
    return true;
  }

  std::string StandardsFilterObjectIntegerAttribute_Impl::print() const {
    std::stringstream ss;
    ss << "    ?" << factName() << " <- (IntegerAttribute ";
    unsigned indentSize = ss.str().length();
    ss << "(modelId " << m_object.modelId() << ")" << std::endl;
    ss << std::setw(indentSize) << " " << "(objectId ?" << m_object.factName() << "_ObjectId)" << std::endl;
    ss << std::setw(indentSize) << " " << "(name ?" << factName() << "_Name&\"" << m_attribute.valueName() << "\")" << std::endl;
    ss << std::setw(indentSize) << " " << "(value ?" << factName() << "_Value))" << std::endl;
    ss << "    (test ";
    switch (m_predicate.value()) {
      case (RulesetNumericalPredicate::Equals) :
        ss << "(== ?" << factName() << "_Value " << value() << "))";
       break;
      case (RulesetNumericalPredicate::GreaterThan) :
        ss << "(> ?" << factName() << "_Value " << value() << "))";
       break;
      case (RulesetNumericalPredicate::GreaterThanOrEqualTo) :
        ss << "(>= ?" << factName() << "_Value " << value() << "))";
       break;
      case (RulesetNumericalPredicate::LessThan) :
        ss << "(< ?" << factName() << "_Value " << value() << "))";
       break;
      case (RulesetNumericalPredicate::LessThanOrEqualTo) :
        ss << "(<= ?" << factName() << "_Value " << value() << "))";
       break;
      default :
       BOOST_ASSERT(false);
    }
    ss << std::endl;
    return ss.str();
  }

  std::string StandardsFilterObjectIntegerAttribute_Impl::printForMessage() const {
    std::stringstream ss;
    ss << "Integer Attribute '" << m_attribute.valueName() << "' ";
    switch (m_predicate.value()) {
      case (RulesetNumericalPredicate::Equals) :
        ss << "==";
       break;
      case (RulesetNumericalPredicate::GreaterThan) :
        ss << ">";
       break;
      case (RulesetNumericalPredicate::GreaterThanOrEqualTo) :
        ss << ">=";
       break;
      case (RulesetNumericalPredicate::LessThan) :
        ss << "<";
       break;
      case (RulesetNumericalPredicate::LessThanOrEqualTo) :
        ss << "<=";
       break;
      default :
       BOOST_ASSERT(false);
    };
    ss << " " << value() << " " << units();
    return ss.str();
  }

  std::string StandardsFilterObjectIntegerAttribute_Impl::xmlElementName() const
  {
    return StandardsFilterObjectIntegerAttribute::xmlElementName();
  }

  void StandardsFilterObjectIntegerAttribute_Impl::writeValues(QDomDocument& doc, QDomElement& element) const
  {
    StandardsFilterObjectNumericalAttribute_Impl::writeValues(doc, element);
  }

  bool StandardsFilterObjectIntegerAttribute_Impl::equivalentData(const RulesetObject& other) const {
    OptionalStandardsFilterObjectIntegerAttribute oIntegerAttribute = other.optionalCast<StandardsFilterObjectIntegerAttribute>();
    if (oIntegerAttribute) {
      if ((m_attribute == oIntegerAttribute->attribute()) &&
          (m_value == oIntegerAttribute->value()))
      {
        return StandardsFilterObjectNumericalAttribute_Impl::equivalentData(other);
      }
    }
    return false;
  }

  bool StandardsFilterObjectIntegerAttribute_Impl::equivalentAttributeType(
      const StandardsFilterObjectAttribute& other) const 
  {
    OptionalStandardsFilterObjectIntegerAttribute oIntegerAttribute = other.optionalCast<StandardsFilterObjectIntegerAttribute>();
    if (oIntegerAttribute) {
      if (m_attribute == oIntegerAttribute->attribute()) {
        return StandardsFilterObjectNumericalAttribute_Impl::equivalentAttributeType(other);
      }
    }
    return false;
  }

} // detail

/// @cond
std::string StandardsFilterObjectIntegerAttribute::xmlElementName()
{
  return "StandardsFilterObjectIntegerAttribute";
}
/// @endcond

StandardsFilterObjectIntegerAttribute::StandardsFilterObjectIntegerAttribute(
    const StandardsFilterAvailableObject& object,
    const standardsinterface::IntegerAttribute& attribute,
    const RulesetNumericalPredicate& predicate,
    int value,
    const std::string& unitString)
  : StandardsFilterObjectNumericalAttribute(boost::shared_ptr<detail::StandardsFilterObjectIntegerAttribute_Impl>(
        new detail::StandardsFilterObjectIntegerAttribute_Impl(object,attribute,predicate,value,unitString)))
{}

StandardsFilterObjectIntegerAttribute::StandardsFilterObjectIntegerAttribute(
    const StandardsFilterAvailableObject& object,
    const standardsinterface::IntegerAttribute& attribute,
    const RulesetNumericalPredicate& predicate,
    int value,
    const std::string& unitString,
    const UUID& uuid,
    const UUID& versionUUID)
  : StandardsFilterObjectNumericalAttribute(boost::shared_ptr<detail::StandardsFilterObjectIntegerAttribute_Impl>(
        new detail::StandardsFilterObjectIntegerAttribute_Impl(object, attribute, predicate, value, unitString, uuid, versionUUID)))
{}

standardsinterface::IntegerAttribute StandardsFilterObjectIntegerAttribute::attribute() const {
  return getImpl<detail::StandardsFilterObjectIntegerAttribute_Impl>()->attribute();
}

int StandardsFilterObjectIntegerAttribute::value() const {
  return getImpl<detail::StandardsFilterObjectIntegerAttribute_Impl>()->value();
}

void StandardsFilterObjectIntegerAttribute::setAttribute(const standardsinterface::IntegerAttribute& attribute)
{
  getImpl<detail::StandardsFilterObjectIntegerAttribute_Impl>()->setAttribute(attribute);
}

bool StandardsFilterObjectIntegerAttribute::setValue(int value) {
  return getImpl<detail::StandardsFilterObjectIntegerAttribute_Impl>()->setValue(value);
}

/// @cond
StandardsFilterObjectIntegerAttribute::StandardsFilterObjectIntegerAttribute(
    boost::shared_ptr<detail::StandardsFilterObjectIntegerAttribute_Impl> impl)
  : StandardsFilterObjectNumericalAttribute(impl)
{}
/// @endcond

} // ruleset
} // openstudio
