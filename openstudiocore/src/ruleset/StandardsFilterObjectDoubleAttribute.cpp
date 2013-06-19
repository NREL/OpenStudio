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

#include <ruleset/StandardsFilterObjectDoubleAttribute.hpp>
#include <ruleset/StandardsFilterObjectDoubleAttribute_Impl.hpp>

#include <ruleset/StandardsFilterAvailableObject_Impl.hpp>

#include <QDomDocument>
#include <QDomElement>

#include <boost/lexical_cast.hpp>

#include <iomanip>

namespace openstudio {
namespace ruleset {

namespace detail {

  StandardsFilterObjectDoubleAttribute_Impl::StandardsFilterObjectDoubleAttribute_Impl(
      const StandardsFilterAvailableObject& object,
      const standardsinterface::DoubleAttribute& attribute,
      const RulesetNumericalPredicate& predicate,
      double value,
      const std::string& unitString)
    : StandardsFilterObjectNumericalAttribute_Impl(object,predicate,unitString),
      m_attribute(attribute),
      m_value(value)
  {}

  StandardsFilterObjectDoubleAttribute_Impl::StandardsFilterObjectDoubleAttribute_Impl(
      const StandardsFilterAvailableObject& object,
      const standardsinterface::DoubleAttribute& attribute,
      const RulesetNumericalPredicate& predicate,
      double value,
      const std::string& unitString,
      const UUID& uuid,
      const UUID& versionUUID)
    : StandardsFilterObjectNumericalAttribute_Impl(object, predicate, unitString, uuid, versionUUID),
      m_attribute(attribute),
      m_value(value)
  {}

  StandardsFilterObjectDoubleAttribute_Impl::StandardsFilterObjectDoubleAttribute_Impl(const QDomElement& element)
    : StandardsFilterObjectNumericalAttribute_Impl(element)
  {
    BOOST_ASSERT(!element.isNull());
    BOOST_ASSERT(element.tagName() == toQString(this->xmlElementName()));
  }

  RulesetObject StandardsFilterObjectDoubleAttribute_Impl::clone() const {
    boost::shared_ptr<StandardsFilterObjectDoubleAttribute_Impl> impl(new StandardsFilterObjectDoubleAttribute_Impl(*this));
    return StandardsFilterObjectDoubleAttribute(impl).cast<RulesetObject>();
  }

  RulesetObject StandardsFilterObjectDoubleAttribute_Impl::deepClone() const {
    StandardsFilterObjectDoubleAttribute result = clone().cast<StandardsFilterObjectDoubleAttribute>();
    result.mf_setObject(object().deepClone().cast<StandardsFilterAvailableObject>());
    return result.cast<RulesetObject>();
  }

  std::string StandardsFilterObjectDoubleAttribute_Impl::factName() const {
    return ("doubleAttribute" + this->printUUID());
  }

  std::string StandardsFilterObjectDoubleAttribute_Impl::shortDescription() const {
    return attribute().valueName();
  }

  standardsinterface::DoubleAttribute StandardsFilterObjectDoubleAttribute_Impl::attribute() const {
    return m_attribute;
  }

  double StandardsFilterObjectDoubleAttribute_Impl::value() const {
    return m_value;
  }

  void StandardsFilterObjectDoubleAttribute_Impl::setAttribute(
      const standardsinterface::DoubleAttribute& attribute)
  {
    m_attribute = attribute;
    this->onChange();
  }

  bool StandardsFilterObjectDoubleAttribute_Impl::setValue(const std::string& value) {
    try {
      double dval = boost::lexical_cast<double>(value);
      return setValue(dval);
    }
    catch (...) { return false; }
  }

  bool StandardsFilterObjectDoubleAttribute_Impl::setValue(double value) {
    m_value = value;
    this->onChange();
    return true;
  }

  std::string StandardsFilterObjectDoubleAttribute_Impl::print() const {
    std::stringstream ss;
    ss << "    ?" << factName() << " <- (DoubleAttribute ";
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
    };
    ss << std::endl;
    return ss.str();
  }

  std::string StandardsFilterObjectDoubleAttribute_Impl::printForMessage() const {
    std::stringstream ss;
    ss << "Double Attribute '" << m_attribute.valueName() << "' ";
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

  std::string StandardsFilterObjectDoubleAttribute_Impl::xmlElementName() const
  {
    return StandardsFilterObjectDoubleAttribute::xmlElementName();
  }

  void StandardsFilterObjectDoubleAttribute_Impl::writeValues(QDomDocument& doc, QDomElement& element) const
  {
    StandardsFilterObjectNumericalAttribute_Impl::writeValues(doc, element);
  }

  bool StandardsFilterObjectDoubleAttribute_Impl::equivalentData(const RulesetObject& other) const {
    OptionalStandardsFilterObjectDoubleAttribute oDoubleAttribute = other.optionalCast<StandardsFilterObjectDoubleAttribute>();
    if (oDoubleAttribute) {
      if ((m_attribute == oDoubleAttribute->attribute()) &&
          (m_value == oDoubleAttribute->value()))
      {
        return StandardsFilterObjectNumericalAttribute_Impl::equivalentData(other);
      }
    }
    return false;
  }

  bool StandardsFilterObjectDoubleAttribute_Impl::equivalentAttributeType(
      const StandardsFilterObjectAttribute& other) const 
  {
    OptionalStandardsFilterObjectDoubleAttribute oDoubleAttribute = other.optionalCast<StandardsFilterObjectDoubleAttribute>();
    if (oDoubleAttribute) {
      if (m_attribute == oDoubleAttribute->attribute()) {
        return StandardsFilterObjectNumericalAttribute_Impl::equivalentAttributeType(other);
      }
    }
    return false;
  }

} // detail

/// @cond
std::string StandardsFilterObjectDoubleAttribute::xmlElementName()
{
  return "StandardsFilterObjectDoubleAttribute";
}
/// @endcond

StandardsFilterObjectDoubleAttribute::StandardsFilterObjectDoubleAttribute(
    const StandardsFilterAvailableObject& object,
    const standardsinterface::DoubleAttribute& attribute,
    const RulesetNumericalPredicate& predicate,
    double value,
    const std::string& unitString)
  : StandardsFilterObjectNumericalAttribute(boost::shared_ptr<detail::StandardsFilterObjectDoubleAttribute_Impl>(
        new detail::StandardsFilterObjectDoubleAttribute_Impl(object,attribute,predicate,value,unitString)))
{}

StandardsFilterObjectDoubleAttribute::StandardsFilterObjectDoubleAttribute(
    const StandardsFilterAvailableObject& object,
    const standardsinterface::DoubleAttribute& attribute,
    const RulesetNumericalPredicate& predicate,
    double value,
    const std::string& unitString,
    const UUID& uuid,
    const UUID& versionUUID)
  : StandardsFilterObjectNumericalAttribute(boost::shared_ptr<detail::StandardsFilterObjectDoubleAttribute_Impl>(
        new detail::StandardsFilterObjectDoubleAttribute_Impl(object, attribute, predicate, value, unitString, uuid, versionUUID)))
{}

standardsinterface::DoubleAttribute StandardsFilterObjectDoubleAttribute::attribute() const {
  return getImpl<detail::StandardsFilterObjectDoubleAttribute_Impl>()->attribute();
}

double StandardsFilterObjectDoubleAttribute::value() const {
  return getImpl<detail::StandardsFilterObjectDoubleAttribute_Impl>()->value();
}

void StandardsFilterObjectDoubleAttribute::setAttribute(const standardsinterface::DoubleAttribute& attribute)
{
  getImpl<detail::StandardsFilterObjectDoubleAttribute_Impl>()->setAttribute(attribute);
}

bool StandardsFilterObjectDoubleAttribute::setValue(double value) {
  return getImpl<detail::StandardsFilterObjectDoubleAttribute_Impl>()->setValue(value);
}

/// @cond
StandardsFilterObjectDoubleAttribute::StandardsFilterObjectDoubleAttribute(
    boost::shared_ptr<detail::StandardsFilterObjectDoubleAttribute_Impl> impl)
  : StandardsFilterObjectNumericalAttribute(impl)
{}
/// @endcond

} // ruleset
} // openstudio
