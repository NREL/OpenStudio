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

#include <ruleset/StandardsRule.hpp>
#include <ruleset/StandardsRule_Impl.hpp>

#include <ruleset/StandardsEnumRulesetOption.hpp>

#include <ruleset/FilterClause_Impl.hpp>
#include <ruleset/StandardsFilterAvailableModel.hpp>
#include <ruleset/StandardsFilterAvailableModel_Impl.hpp>
#include <ruleset/StandardsFilterAvailableObject.hpp>
#include <ruleset/StandardsFilterAvailableObject_Impl.hpp>
#include <ruleset/StandardsFilterObjectAttribute.hpp>
#include <ruleset/StandardsFilterObjectAttribute_Impl.hpp>
#include <ruleset/StandardsFilterObjectBooleanAttribute.hpp>
#include <ruleset/StandardsFilterObjectBooleanAttribute_Impl.hpp>
#include <ruleset/StandardsFilterObjectDoubleAttribute.hpp>
#include <ruleset/StandardsFilterObjectDoubleAttribute_Impl.hpp>
#include <ruleset/StandardsFilterObjectEnumAttribute.hpp>
#include <ruleset/StandardsFilterObjectEnumAttribute_Impl.hpp>
#include <ruleset/StandardsFilterObjectIntegerAttribute.hpp>
#include <ruleset/StandardsFilterObjectIntegerAttribute_Impl.hpp>
#include <ruleset/StandardsFilterRulesetOption.hpp>
#include <ruleset/StandardsFilterRulesetOption_Impl.hpp>
#include <ruleset/StandardsActionGetObjects.hpp>
#include <ruleset/StandardsActionGetAttribute.hpp>

#include <ruleset/ActionClause_Impl.hpp>
#include <ruleset/StandardsActionRuleSatisfied.hpp>
#include <ruleset/StandardsActionRuleSatisfied_Impl.hpp>

#include <standardsinterface/DataDictionary.hpp>

#include <utilities/core/Assert.hpp>
#include <utilities/core/StringHelpers.hpp>
#include <utilities/core/Checksum.hpp>
#include <utilities/core/Containers.hpp>

#include <boost/foreach.hpp>
#include <boost/bind.hpp>

#include <QDomDocument>
#include <QDomElement>

namespace openstudio {
namespace ruleset {

namespace detail {

  StandardsRule_Impl::StandardsRule_Impl(const std::string& name)
    : Rule_Impl(name)
  {
    BOOST_ASSERT(m_filters.empty());
    BOOST_ASSERT(m_actions.empty());
  }

  StandardsRule_Impl::StandardsRule_Impl(const std::string& name, bool active, const UUID& uuid, const UUID& versionUUID,
                                         const std::vector<FilterClause>& filters, const std::vector<ActionClause>& actions)
    : Rule_Impl(name, active, uuid, versionUUID, filters, actions)
  {}

  StandardsRule_Impl::StandardsRule_Impl(const QDomElement& element)
    : Rule_Impl(element)
  {
    BOOST_ASSERT(!element.isNull());
    BOOST_ASSERT(element.tagName() == toQString(this->xmlElementName()));
  }

  bool StandardsRule_Impl::add(const FilterClause& filter) {

    boost::optional<StandardsFilterClause> standardsFilter = filter.optionalCast<StandardsFilterClause>();
    BOOST_ASSERT(standardsFilter);

    if (contains(filter)) { return false; }

    m_filters.push_back(filter);
    this->connectChild(filter);
    this->onChange();

    StandardsFilterClauseVector refs = standardsFilter->referencedFilters();
    BOOST_FOREACH(const FilterClause& ref,refs) {
      StandardsRule_Impl::add(ref);
      m_filterReferencedByMap[ref].insert(filter);
    }

    return true;
  }

  bool StandardsRule_Impl::add(const std::vector<FilterClause>& filters) {
    StandardsFilterClauseVector stdsFilters = castVector<StandardsFilterClause>(filters);
    BOOST_ASSERT(stdsFilters.size() == filters.size());
    bool result(true);
    BOOST_FOREACH(const StandardsFilterClause& filter,stdsFilters) {
      this->add(filter);
      result = result && contains(filter);
    }
    return result;
  }

  bool StandardsRule_Impl::add(const ActionClause& action) {

    boost::optional<StandardsActionClause> standardsAction = action.optionalCast<StandardsActionClause>();
    BOOST_ASSERT(standardsAction);

    if (contains(action)) { return false; }

    m_actions.push_back(action);
    this->connectChild(action);
    this->onChange();

    StandardsFilterClauseVector refs = standardsAction->referencedFilters();
    BOOST_FOREACH(const FilterClause& filter,refs) {
      StandardsRule_Impl::add(filter);
      m_filterReferencedByMap[filter].insert(action);
    }

    return true;
  }

  bool StandardsRule_Impl::remove(const FilterClause& filter) {

    boost::optional<StandardsFilterClause> standardsFilter = filter.optionalCast<StandardsFilterClause>();
    BOOST_ASSERT(standardsFilter);

    FilterClauseVector::iterator it = std::find(m_filters.begin(),m_filters.end(),filter);
    if (it != m_filters.end()) {
      // delete Clauses that reference this filter
      FilterToSetOfClausesMap::iterator mapIt = m_filterReferencedByMap.find(filter);
      if (mapIt != m_filterReferencedByMap.end()) {
        std::set<Clause> clausesToRemove = mapIt->second;
        BOOST_FOREACH(const Clause& clause, clausesToRemove) {
          if (clause.optionalCast<FilterClause>()) {
            StandardsRule_Impl::remove(clause.cast<FilterClause>());
          }
          else {
            StandardsRule_Impl::remove(clause.cast<ActionClause>());
          }
        }
        // refresh iterator and delete map entry
        mapIt = m_filterReferencedByMap.find(filter);
        BOOST_ASSERT(mapIt != m_filterReferencedByMap.end());
        m_filterReferencedByMap.erase(mapIt);
      }
      // delete from m_filters
      this->disconnectChild(*it);
      it = m_filters.erase(it);
      this->onChange();
      return true;
    }
    return false;
  }

  bool StandardsRule_Impl::remove(const ActionClause& action) {

    boost::optional<StandardsActionClause> standardsAction = action.optionalCast<StandardsActionClause>();
    BOOST_ASSERT(standardsAction);

    ActionClauseVector::iterator it = std::find(m_actions.begin(),m_actions.end(),action);
    if (it != m_actions.end()) {
      // delete from m_filterReferencedByMap
      StandardsFilterClauseVector refs = standardsAction->referencedFilters();
      BOOST_FOREACH(const FilterClause& ref,refs) {
        FilterToSetOfClausesMap::iterator mapIt = m_filterReferencedByMap.find(ref);
        BOOST_ASSERT(mapIt != m_filterReferencedByMap.end());
        unsigned n = mapIt->second.erase(action);
        BOOST_ASSERT(n == 1);
      }
      // delete from m_actions
      this->disconnectChild(*it);
      it = m_actions.erase(it);
      this->onChange();
      return true;
    }
    return false;
  }

  std::vector<StandardsRule> StandardsRule_Impl::impliedRules() const {
    StandardsRuleVector result;

    StandardsRule thisObject = getObject<StandardsRule>();

    // filters that make up implied rules
    StandardsFilterRulesetOptionVector options = thisObject.getFilters<StandardsFilterRulesetOption>();
    StandardsFilterAvailableObjectVector objects = thisObject.getFilters<StandardsFilterAvailableObject>();
    StandardsFilterObjectAttributeVector attributes = thisObject.getFilters<StandardsFilterObjectAttribute>();

    // get object rules
    BOOST_FOREACH(const StandardsFilterAvailableObject& object,objects) {
      std::stringstream ss;
      ss << "GetObjects_" << object.objectType().valueName() << "_" << toString(createUUID());
      StandardsRule candidate(ss.str());
      StandardsActionGetObjects getObjects(object);
      candidate.add(castVector<FilterClause>(options));
      candidate.add(getObjects);

      // see if already exists in result
      StandardsRuleVector::iterator it = std::find_if(result.begin(),result.end(),
          boost::bind(&StandardsRule::equivalentData,&candidate,_1));
      if (it == result.end()) {
        result.push_back(candidate);
      }
    } // FOREACH object

    // get attribute rules
    BOOST_FOREACH(const StandardsFilterObjectAttribute& attribute,attributes) {
      StandardsRule candidate = mf_basicGetAttributeImpliedRule(attribute);
      candidate.add(castVector<FilterClause>(options));

      // see if already exists in result
      StandardsRuleVector::iterator it = std::find_if(result.begin(),result.end(),
          boost::bind(&StandardsRule::equivalentData,&candidate,_1));
      if (it == result.end()) {
        result.push_back(candidate);
      }
    } // FOREACH attribute
    
    return result;
  }

  std::string StandardsRule_Impl::print(const StandardsRulesetPrintMode& mode) const {

    std::stringstream ss;
    if (!active()) {
      ss << ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;" << std::endl;
      ss << "; DISABLED RULE. WILL NOT BE LOADED INTO RULESENGINE. ;;;;;;;;;;;;;;;;;;;;;;;;;;" << std::endl;
      ss << ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;" << std::endl;
      ss << std::endl;
    }

    if (mode == StandardsRulesetPrintMode::File) {
      ss << "(defrule " << name() << std::endl;
    }
    else if (mode == StandardsRulesetPrintMode::Engine) {
      ss << "(defrule " << toUpperCamelCase(name()) << std::endl;
    }
    else { BOOST_ASSERT(false); }
    ss << "    (phase Query)" << std::endl; // Ruleset defines Query phase.
    BOOST_FOREACH(const FilterClause& filter,filters()) {
      ss << filter.cast<StandardsFilterClause>().print();
    }
    // print implied filters for engine
    if (mode == StandardsRulesetPrintMode::Engine) {
      BOOST_FOREACH(const FilterClause& filter,filters()) {
        BOOST_FOREACH(const StandardsFilterClause& impliedFilter,filter.cast<StandardsFilterClause>().impliedFilters()) {
          ss << impliedFilter.print();
        }
      }
      BOOST_FOREACH(const ActionClause& action,actions()) {
        BOOST_FOREACH(const StandardsFilterClause& impliedFilter,action.cast<StandardsActionClause>().impliedFilters()) {
          ss << impliedFilter.print();
        }
      }
    }
    ss << "  =>" << std::endl;
    BOOST_FOREACH(const ActionClause& action,actions()) {
      ss << action.cast<StandardsActionClause>().print(getObject<StandardsRule>());
    }
    ss << ")" << std::endl;

    if (!active()) {
      ss << ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;" << std::endl;
      ss << "; END DISABLED RULE. ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;" << std::endl;
      ss << ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;" << std::endl;
      ss << std::endl;
    }

    return ss.str();
  }

  std::string StandardsRule_Impl::xmlElementName() const
  {
    return StandardsRule::xmlElementName();
  }

  void StandardsRule_Impl::writeValues(QDomDocument& doc, QDomElement& element) const
  {
    Rule_Impl::writeValues(doc, element);
  }

  bool StandardsRule_Impl::equivalentData(const RulesetObject& other) const {
    if (other.optionalCast<StandardsRule>()) {
      return Rule_Impl::equivalentData(other);
    }
    return false;
  }

  bool StandardsRule_Impl::containsEquivalentAttributeType(
      const StandardsFilterObjectAttribute& attributeFilter) const
  {
    StandardsFilterObjectAttributeVector myAttributeFilters;
    myAttributeFilters = getObject<StandardsRule>().getFilters<StandardsFilterObjectAttribute>();
    return (std::find_if(myAttributeFilters.begin(),myAttributeFilters.end(),
        boost::bind(&StandardsFilterObjectAttribute::equivalentAttributeType,&attributeFilter,_1)) 
        != myAttributeFilters.end());
  }

  bool StandardsRule_Impl::containsEquivalentAttributeType(
      const standardsinterface::ObjectType& objectType,
      const standardsinterface::BooleanAttribute& attribute) const 
  {
    OptionalStandardsFilterAvailableObject oObjectFilter = mf_getLikelyMatchObjectFilter(objectType);
    if (oObjectFilter) {
      StandardsFilterObjectBooleanAttribute testAttributeFilter(*oObjectFilter,attribute,true);
      return containsEquivalentAttributeType(testAttributeFilter);
    }
    return false;
  }

  bool StandardsRule_Impl::containsEquivalentAttributeType(
      const standardsinterface::ObjectType& objectType,
      const standardsinterface::DoubleAttribute& attribute) const
  {
    OptionalStandardsFilterAvailableObject oObjectFilter = mf_getLikelyMatchObjectFilter(objectType);
    if (oObjectFilter) {
      StandardsFilterObjectDoubleAttribute testAttributeFilter(*oObjectFilter,attribute,RulesetNumericalPredicate::Equals,0.0,"");
      return containsEquivalentAttributeType(testAttributeFilter);
    }
    return false;
  }

  bool StandardsRule_Impl::containsEquivalentAttributeType(
      const standardsinterface::ObjectType& objectType,
      const standardsinterface::EnumAttribute& attribute) const
  {
    OptionalStandardsFilterAvailableObject oObjectFilter = mf_getLikelyMatchObjectFilter(objectType);
    if (oObjectFilter) {
      StringVector enumAttributeValues = standardsinterface::DataDictionary::enumValues(attribute);
      if (!enumAttributeValues.empty()) {
        StandardsFilterObjectEnumAttribute testAttributeFilter(*oObjectFilter,attribute,RulesetStringPredicate::Equals,enumAttributeValues[0]);
        return containsEquivalentAttributeType(testAttributeFilter);
      }
    }
    return false;
  }

  bool StandardsRule_Impl::containsEquivalentAttributeType(
      const standardsinterface::ObjectType& objectType,
      const standardsinterface::IntegerAttribute& attribute) const
  {
    OptionalStandardsFilterAvailableObject oObjectFilter = mf_getLikelyMatchObjectFilter(objectType);
    if (oObjectFilter) {
      StandardsFilterObjectIntegerAttribute testAttributeFilter(*oObjectFilter,attribute,RulesetNumericalPredicate::Equals,0,"");
      return containsEquivalentAttributeType(testAttributeFilter);
    }
    return false;
  }

  boost::optional<StandardsFilterAvailableObject> StandardsRule_Impl::mf_getLikelyMatchObjectFilter(
      const standardsinterface::ObjectType& objectType) const
  {
    OptionalStandardsFilterAvailableObject result;
    StandardsFilterAvailableObjectVector myObjectFilters = getObject<StandardsRule>().getFilters<StandardsFilterAvailableObject>();
    BOOST_FOREACH(const StandardsFilterAvailableObject& filter,myObjectFilters) {
      if (filter.objectType() == objectType) {
        if (filter.model()) {
          result = StandardsFilterAvailableObject(*(filter.model()),objectType);
        }
        else {
          result = StandardsFilterAvailableObject(objectType);
        }
        break;
      }
    }
    return result;
  }

  StandardsRule StandardsRule_Impl::mf_basicGetAttributeImpliedRule(
      const StandardsFilterObjectAttribute& attribute) const
  {
    std::stringstream ss;
    ss << "GetAttribute_";
    OptionalStandardsActionGetAttribute oAction;

    if (attribute.optionalCast<StandardsFilterObjectBooleanAttribute>()) {
      StandardsFilterObjectBooleanAttribute ba = attribute.cast<StandardsFilterObjectBooleanAttribute>();
      StandardsActionGetAttribute getAttribute(ba.object(),ba.attribute());
      oAction = getAttribute;
      ss << ba.attribute().valueName() << "_FromObjects_"
         << ba.object().objectType().valueName() << "_" << checksum(toString(createUUID()));
    }
    else if (attribute.optionalCast<StandardsFilterObjectDoubleAttribute>()) {
      StandardsFilterObjectDoubleAttribute da = attribute.cast<StandardsFilterObjectDoubleAttribute>();
      StandardsActionGetAttribute getAttribute(da.object(),da.attribute());
      oAction = getAttribute;
      ss << da.attribute().valueName() << "_FromObjects_"
         << da.object().objectType().valueName() << "_" << checksum(toString(createUUID()));
    }
    else if (attribute.optionalCast<StandardsFilterObjectEnumAttribute>()) {
      StandardsFilterObjectEnumAttribute ea = attribute.cast<StandardsFilterObjectEnumAttribute>();
      StandardsActionGetAttribute getAttribute(ea.object(),ea.attribute());
      oAction = getAttribute;
      ss << ea.attribute().valueName() << "_FromObjects_"
         << ea.object().objectType().valueName() << "_" << checksum(toString(createUUID()));
    }
    else if (attribute.optionalCast<StandardsFilterObjectIntegerAttribute>()) {
      StandardsFilterObjectIntegerAttribute ia = attribute.cast<StandardsFilterObjectIntegerAttribute>();
      StandardsActionGetAttribute getAttribute(ia.object(),ia.attribute());
      oAction = getAttribute;
      ss << ia.attribute().valueName() << "_FromObjects_"
         << ia.object().objectType().valueName() << "_" << checksum(toString(createUUID()));
    }
    BOOST_ASSERT(oAction);

    StandardsRule result(ss.str());
    result.add(*oAction);

    return result;
  }

} // detail

/// @cond
std::string StandardsRule::xmlElementName()
{
  return "StandardsRule";
}
/// @endcond

StandardsRule::StandardsRule(const std::string& name)
  : Rule(boost::shared_ptr<detail::StandardsRule_Impl>(new detail::StandardsRule_Impl(name)))
{
  BOOST_ASSERT(getImpl<detail::StandardsRule_Impl>());
}

StandardsRule::StandardsRule(const std::string& name, bool active, const UUID& uuid, const UUID& versionUUID,
                             const std::vector<FilterClause>& filters, const std::vector<ActionClause>& actions)
  : Rule(boost::shared_ptr<detail::StandardsRule_Impl>(new detail::StandardsRule_Impl(name, active, uuid, versionUUID, filters, actions)))
{
  BOOST_ASSERT(getImpl<detail::StandardsRule_Impl>());
}

StandardsRule::StandardsRule(const QDomElement& element)
  : Rule(boost::shared_ptr<detail::StandardsRule_Impl>(new detail::StandardsRule_Impl(element)))
{
  BOOST_ASSERT(getImpl<detail::StandardsRule_Impl>());
}

std::vector<StandardsRule> StandardsRule::impliedRules() const {
  return getImpl<detail::StandardsRule_Impl>()->impliedRules();
}

std::string StandardsRule::print(const StandardsRulesetPrintMode& mode) const {
  return getImpl<detail::StandardsRule_Impl>()->print(mode);
}

bool StandardsRule::containsEquivalentAttributeType(
    const StandardsFilterObjectAttribute& attributeFilter) const 
{
  return getImpl<detail::StandardsRule_Impl>()->containsEquivalentAttributeType(attributeFilter);
}

bool StandardsRule::containsEquivalentAttributeType(
    const standardsinterface::ObjectType& objectType,
    const standardsinterface::BooleanAttribute& attribute) const
{
  return getImpl<detail::StandardsRule_Impl>()->containsEquivalentAttributeType(objectType,attribute);
}

bool StandardsRule::containsEquivalentAttributeType(
    const standardsinterface::ObjectType& objectType,
    const standardsinterface::DoubleAttribute& attribute) const 
{
  return getImpl<detail::StandardsRule_Impl>()->containsEquivalentAttributeType(objectType,attribute);
}

bool StandardsRule::containsEquivalentAttributeType(
    const standardsinterface::ObjectType& objectType,
    const standardsinterface::EnumAttribute& attribute) const 
{
  return getImpl<detail::StandardsRule_Impl>()->containsEquivalentAttributeType(objectType,attribute);
}

bool StandardsRule::containsEquivalentAttributeType(
    const standardsinterface::ObjectType& objectType,
    const standardsinterface::IntegerAttribute& attribute) const 
{
  return getImpl<detail::StandardsRule_Impl>()->containsEquivalentAttributeType(objectType,attribute);
}

/// @cond
StandardsRule::StandardsRule(boost::shared_ptr<detail::StandardsRule_Impl> impl)
  : Rule(impl)
{
  BOOST_ASSERT(getImpl<detail::StandardsRule_Impl>());
}
/// @endcond

} // ruleset
} // openstudio

