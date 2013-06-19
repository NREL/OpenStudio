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

#include <ruleset/StandardsFilterObjectNumericalAttribute.hpp>
#include <ruleset/StandardsFilterObjectNumericalAttribute_Impl.hpp>

namespace openstudio {
namespace ruleset {

namespace detail {

  StandardsFilterObjectNumericalAttribute_Impl::StandardsFilterObjectNumericalAttribute_Impl(
      const StandardsFilterAvailableObject& object,
      const RulesetNumericalPredicate& predicate,
      const std::string& unitString)
    : StandardsFilterObjectAttribute_Impl(object), m_predicate(predicate), m_units(unitString)
  {}

  StandardsFilterObjectNumericalAttribute_Impl::StandardsFilterObjectNumericalAttribute_Impl(
      const StandardsFilterAvailableObject& object,
      const RulesetNumericalPredicate& predicate,
      const std::string& unitString,
      const UUID& uuid,
      const UUID& versionUUID)
    : StandardsFilterObjectAttribute_Impl(object,uuid,versionUUID), m_predicate(predicate), m_units(unitString)
  {}

  StandardsFilterObjectNumericalAttribute_Impl::StandardsFilterObjectNumericalAttribute_Impl(
      const QDomElement& element)
    : StandardsFilterObjectAttribute_Impl(element)
  {
    BOOST_ASSERT(!element.isNull());
  }

  RulesetNumericalPredicate StandardsFilterObjectNumericalAttribute_Impl::predicate() const {
    return m_predicate;
  }

  std::string StandardsFilterObjectNumericalAttribute_Impl::units() const {
    return m_units;
  }

  void StandardsFilterObjectNumericalAttribute_Impl::negate() {
    switch (m_predicate.value()) {
      case RulesetNumericalPredicate::Equals :
        setPredicate(RulesetNumericalPredicate::NotEquals);
       break;
      case RulesetNumericalPredicate::NotEquals :
        setPredicate(RulesetNumericalPredicate::Equals);
       break;
      case RulesetNumericalPredicate::GreaterThan :
        setPredicate(RulesetNumericalPredicate::LessThanOrEqualTo);
       break;
      case RulesetNumericalPredicate::GreaterThanOrEqualTo :
        setPredicate(RulesetNumericalPredicate::LessThan);
       break;
      case RulesetNumericalPredicate::LessThan :
        setPredicate(RulesetNumericalPredicate::GreaterThanOrEqualTo);
       break;
      case RulesetNumericalPredicate::LessThanOrEqualTo :
        setPredicate(RulesetNumericalPredicate::GreaterThan);
       break;
      default :
        BOOST_ASSERT(false);
    };
  }

  void StandardsFilterObjectNumericalAttribute_Impl::setPredicate(
      const RulesetNumericalPredicate& predicate)
  {
    m_predicate = predicate;
    this->onChange();
  }

  void StandardsFilterObjectNumericalAttribute_Impl::setUnits(const std::string& unitString) {
    m_units = unitString;
    this->onChange();
  }

  void StandardsFilterObjectNumericalAttribute_Impl::writeValues(QDomDocument& doc, 
                                                                 QDomElement& element) const
  {
    StandardsFilterObjectAttribute_Impl::writeValues(doc, element);
  }

  bool StandardsFilterObjectNumericalAttribute_Impl::equivalentData(
      const RulesetObject& other) const
  {
    // assume called from derived class
    StandardsFilterObjectNumericalAttribute otherObjectNumericalAttribute = other.cast<StandardsFilterObjectNumericalAttribute>();
    if ((m_predicate == otherObjectNumericalAttribute.predicate()) && 
        (m_units == otherObjectNumericalAttribute.units()))
    {
      return StandardsFilterObjectAttribute_Impl::equivalentData(other);
    }
    return false;
  }

  bool StandardsFilterObjectNumericalAttribute_Impl::equivalentAttributeType(
      const StandardsFilterObjectAttribute& other) const
  {
    // predicate and unitString are irrelevant to attribute type
    return StandardsFilterObjectAttribute_Impl::equivalentAttributeType(other);
  }

} // detail

RulesetNumericalPredicate StandardsFilterObjectNumericalAttribute::predicate() const {
  return getImpl<detail::StandardsFilterObjectNumericalAttribute_Impl>()->predicate();
}

std::string StandardsFilterObjectNumericalAttribute::units() const {
  return getImpl<detail::StandardsFilterObjectNumericalAttribute_Impl>()->units();
}

void StandardsFilterObjectNumericalAttribute::setPredicate(const RulesetNumericalPredicate& predicate) {
  getImpl<detail::StandardsFilterObjectNumericalAttribute_Impl>()->setPredicate(predicate);
}

void StandardsFilterObjectNumericalAttribute::setUnits(const std::string& unitString) {
  getImpl<detail::StandardsFilterObjectNumericalAttribute_Impl>()->setUnits(unitString);
}

/// @cond
StandardsFilterObjectNumericalAttribute::StandardsFilterObjectNumericalAttribute(
    boost::shared_ptr<detail::StandardsFilterObjectNumericalAttribute_Impl> impl)
  : StandardsFilterObjectAttribute(impl)
{}
/// @endcond

} // ruleset
} // openstudio
