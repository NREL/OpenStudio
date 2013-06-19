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

#include <ruleset/StandardsAttributeCheckRule.hpp>
#include <ruleset/StandardsAttributeCheckRule_Impl.hpp>

#include <ruleset/StandardsRulesetEnums.hpp>
#include <ruleset/StandardsEnumRulesetOption.hpp>

#include <ruleset/FilterClause_Impl.hpp>
#include <ruleset/StandardsFilterRulesetOption.hpp>
#include <ruleset/StandardsFilterRulesetOption_Impl.hpp>
#include <ruleset/StandardsFilterAvailableModel.hpp>
#include <ruleset/StandardsFilterAvailableModel_Impl.hpp>
#include <ruleset/StandardsFilterAvailableObject.hpp>
#include <ruleset/StandardsFilterAvailableObject_Impl.hpp>
#include <ruleset/StandardsFilterRulesetOption.hpp>
#include <ruleset/StandardsFilterRulesetOption_Impl.hpp>
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
#include <ruleset/StandardsActionRuleResult.hpp>
#include <ruleset/StandardsActionRuleResult_Impl.hpp>
#include <ruleset/StandardsActionRuleSatisfied.hpp>
#include <ruleset/StandardsActionRuleSatisfied_Impl.hpp>
#include <ruleset/StandardsActionRuleViolated.hpp>
#include <ruleset/StandardsActionRuleViolated_Impl.hpp>
#include <ruleset/StandardsActionGetObjects.hpp>
#include <ruleset/StandardsActionGetAttribute.hpp>

#include <utilities/document/Table.hpp>

#include <utilities/core/Assert.hpp>
#include <utilities/core/Checksum.hpp>
#include <utilities/core/Containers.hpp>

#include <boost/foreach.hpp>
#include <boost/bind.hpp>

namespace openstudio {
namespace ruleset {

namespace detail {

  StandardsAttributeCheckRule_Impl::StandardsAttributeCheckRule_Impl(
      const std::vector<StandardsFilterRulesetOption>& optionFilters,
      const std::vector<StandardsFilterObjectAttribute>& qualifyingFilters,
      const StandardsActionRuleResult& ruleResultAction)
    : StandardsRule_Impl(std::string())
  {
    // option filters
    FilterClauseVector plainFilters = castVector<FilterClause>(optionFilters);
    bool ok = StandardsRule_Impl::add(plainFilters);
    BOOST_ASSERT(ok);

    // THE attribute check action
    ok = add(ruleResultAction);
    BOOST_ASSERT(ok);

    // filters on unique objects
    plainFilters = castVector<FilterClause>(qualifyingFilters);
    ok = StandardsRule_Impl::add(plainFilters);
    if (!ok) {
      LOG_AND_THROW("Unable to construct StandardsAttributeCheckRule, because at least one "
            << "qualifyingFilter refers to an invalid .object().objectType(), or has an "
            << "inconsistent .object().model().");
    }

    // give it all a nice name
    createDescriptiveName();
  }

  StandardsAttributeCheckRule_Impl::StandardsAttributeCheckRule_Impl(
      const std::string& name, 
      bool active, 
      const UUID& uuid, 
      const UUID& versionUUID,
      const std::vector<FilterClause>& filters, 
      const std::vector<ActionClause>& actions)
    : StandardsRule_Impl(name,active,uuid,versionUUID,filters,actions)
  {}

  StandardsAttributeCheckRule_Impl::StandardsAttributeCheckRule_Impl(const QDomElement& element)
    : StandardsRule_Impl(element)
  {
    BOOST_ASSERT(!element.isNull());
    BOOST_ASSERT(element.tagName() == toQString(this->xmlElementName()));
  }

  StandardsAttributeCheckRule StandardsAttributeCheckRule_Impl::createCompanionRule() const {
    StandardsFilterRulesetOptionVector optionFilters = getObject<StandardsAttributeCheckRule>().getFilters<StandardsFilterRulesetOption>();
    StandardsActionRuleResult currentAction = ruleResultAction();
    StandardsActionRuleResult newAction(currentAction);
    StandardsFilterObjectAttribute newAttributeTest = currentAction.attributeTest().clone().cast<StandardsFilterObjectAttribute>();
    newAttributeTest.negate();
    if (currentAction.optionalCast<StandardsActionRuleSatisfied>()) {
      newAction = StandardsActionRuleViolated(newAttributeTest);
    }
    else {
      newAction = StandardsActionRuleSatisfied(newAttributeTest);
    }
    StandardsAttributeCheckRule companionRule(optionFilters,qualifyingFilters(),newAction);
    companionRule.setName(companionRule.name() + " Companion");
    return companionRule;
  }

  std::vector<StandardsRule> StandardsAttributeCheckRule_Impl::impliedRules() const {
    StandardsRuleVector result;

    StandardsAttributeCheckRule thisObject = getObject<StandardsAttributeCheckRule>();

    // filters that make up implied rules
    StandardsFilterRulesetOptionVector options = thisObject.getFilters<StandardsFilterRulesetOption>();
    StandardsFilterAvailableObjectVector objects = thisObject.getFilters<StandardsFilterAvailableObject>();
    StandardsFilterObjectAttributeVector qualifyingAttributes = thisObject.qualifyingFilters();
    StandardsFilterObjectAttribute starAttribute = thisObject.ruleResultAction().attributeTest();

    // TODO -- refactor this code out. perhaps private member of StandardsRule
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
    StandardsFilterObjectAttributeVector previousAttributeFilters;
    BOOST_FOREACH(const StandardsFilterObjectAttribute& attribute, qualifyingAttributes) {
      StandardsRule candidate = mf_basicGetAttributeImpliedRule(attribute);
      candidate.add(castVector<FilterClause>(options));
      candidate.add(castVector<FilterClause>(previousAttributeFilters));
      previousAttributeFilters.push_back(attribute);

      result.push_back(candidate);
    } // FOREACH attribute

    StandardsRule candidate = mf_basicGetAttributeImpliedRule(starAttribute);
    candidate.add(castVector<FilterClause>(options));
    candidate.add(castVector<FilterClause>(qualifyingAttributes));
    result.push_back(candidate);

    StandardsAttributeCheckRule companion = createCompanionRule();
    result.push_back(companion);
    
    return result;
  }

  standardsinterface::ObjectType StandardsAttributeCheckRule_Impl::objectType() const {
    return ruleResultAction().attributeTest().object().objectType();
  }

  std::vector<StandardsFilterObjectAttribute> StandardsAttributeCheckRule_Impl::qualifyingFilters() const 
  {
    // get all attribute filters
    StandardsFilterObjectAttributeVector result;
    FilterClauseVector plainFilters = filters();
    result = subsetCastVector<StandardsFilterObjectAttribute>(plainFilters);

    // remove bottom attribute filter
    StandardsFilterObjectAttribute myStarAttributeFilter = ruleResultAction().attributeTest();
    StandardsFilterObjectAttributeVector::iterator it = std::find(result.begin(),result.end(),myStarAttributeFilter);
    BOOST_ASSERT(it != result.end());
    result.erase(it);

    // initialize result
    std::sort(result.begin(),result.end(),boost::bind(objectTypeLess,_1,_2,mf_objectTypes()));

    return result;
  }

  StandardsActionRuleResult StandardsAttributeCheckRule_Impl::ruleResultAction() const {
    ActionClauseVector actions = this->actions();
    BOOST_ASSERT(actions.size() == 1u);
    return actions[0].cast<StandardsActionRuleResult>();
  }

  bool StandardsAttributeCheckRule_Impl::add(const FilterClause& filter) {
    OptionalStandardsFilterRulesetOption oOptionFilter = filter.optionalCast<StandardsFilterRulesetOption>();
    if (oOptionFilter) { 
      return StandardsRule_Impl::add(filter);
    }
    OptionalStandardsFilterObjectAttribute oAttributeFilter = filter.optionalCast<StandardsFilterObjectAttribute>();
    if (oAttributeFilter) {
      const standardsinterface::ObjectTypeVector& ots = mf_objectTypes();
      if (std::find(ots.begin(),ots.end(),oAttributeFilter->object().objectType()) != ots.end()) 
      {
        return StandardsRule_Impl::add(filter);
      }
    }
    return false;
  }

  bool StandardsAttributeCheckRule_Impl::add(const ActionClause& action) {
    if ((actions().size() == 0u) && (action.optionalCast<StandardsActionRuleResult>())) {
      return StandardsRule_Impl::add(action);
    }
    return false;      
  }

  bool StandardsAttributeCheckRule_Impl::remove(const FilterClause& filter) {
    OptionalStandardsFilterRulesetOption oOptionFilter = filter.optionalCast<StandardsFilterRulesetOption>();
    if (oOptionFilter) { 
      return StandardsRule_Impl::remove(filter);
    }
    OptionalStandardsFilterObjectAttribute oAttributeFilter = filter.optionalCast<StandardsFilterObjectAttribute>();
    if (oAttributeFilter && ((*oAttributeFilter) != ruleResultAction().attributeTest())) {
      return StandardsRule_Impl::remove(filter);
    }
    return false;
  }

  bool StandardsAttributeCheckRule_Impl::remove(const ActionClause& action) {
    return false;
  }

  unsigned StandardsAttributeCheckRule_Impl::clearFilters() {
    return 0u;
  }

  unsigned StandardsAttributeCheckRule_Impl::clearActions() {
    return 0u;
  }

  void StandardsAttributeCheckRule_Impl::createDescriptiveName() {
    std::stringstream ss;
    standardsinterface::ObjectType primaryObjectType = objectType();
    StandardsFilterObjectAttributeVector qFilters = qualifyingFilters();
    StandardsFilterObjectAttribute starFilter = ruleResultAction().attributeTest();
    ss << "Test ";
    bool containsOtherObjectTypes(false);
    BOOST_FOREACH(const StandardsFilterObjectAttribute& qFilter,qFilters) {
      if (qFilter.object().objectType() == primaryObjectType) {
        ss << qFilter.shortDescription() << " ";
      }
      else { containsOtherObjectTypes = true; }
    }
    ss << "'" << primaryObjectType.valueName() << "' " << starFilter.shortDescription();
    if (containsOtherObjectTypes) {
      standardsinterface::OptionalObjectType currentObjectType;
      std::string sep;
      BOOST_FOREACH(const StandardsFilterObjectAttribute& qFilter,qFilters) {
        if (qFilter.object().objectType() == primaryObjectType) {
          break; 
        }
        if (!currentObjectType || (qFilter.object().objectType() != *currentObjectType)) {
          currentObjectType = qFilter.object().objectType();
          ss << " for '" << currentObjectType->valueName() << "'";
          sep = " with ";
        }
        ss << sep << qFilter.shortDescription();
        sep = " and ";
      }
    }
    setName(ss.str());
  }

  std::string StandardsAttributeCheckRule_Impl::xmlElementName() const {
    return StandardsAttributeCheckRule::xmlElementName();
  }

  void StandardsAttributeCheckRule_Impl::writeValues(QDomDocument& doc, QDomElement& element) const {
    StandardsRule_Impl::writeValues(doc, element);
  }

  bool StandardsAttributeCheckRule_Impl::equivalentData(const RulesetObject& other) const {
    if (other.optionalCast<StandardsAttributeCheckRule>()) {
      return StandardsRule_Impl::equivalentData(other);
    }
    return false;
  }

  bool StandardsAttributeCheckRule_Impl::mf_consistentModel(const StandardsFilterObjectAttribute& attributeFilter) const {
    OptionalStandardsFilterAvailableModel oThisModel = ruleResultAction().attributeTest().object().model();
    OptionalStandardsFilterAvailableModel oThatModel = attributeFilter.object().model();
    if (oThisModel) {
      if (!oThatModel) { return false; }
      return oThisModel->equivalentData(*oThatModel);
    }
    else {
      if (oThatModel) { return false; }
    }
    return true;
  }

  const std::vector<standardsinterface::ObjectType>& StandardsAttributeCheckRule_Impl::mf_objectTypes() const {
    if (m_objectTypes.empty()) {
      m_objectTypes = standardsinterface::DataDictionary::objectTypesUniqueWithinOneModel();
      m_objectTypes.push_back(objectType());
    }
    return m_objectTypes;
  }

} // detail

/// @cond
std::string StandardsAttributeCheckRule::xmlElementName() {
  return "StandardsAttributeCheckRule";
}
/// @endcond

StandardsAttributeCheckRule::StandardsAttributeCheckRule(
    const std::vector<StandardsFilterRulesetOption>& optionFilters,
    const std::vector<StandardsFilterObjectAttribute>& qualifyingFilters,
    const StandardsActionRuleResult& ruleResultAction)
  : StandardsRule(boost::shared_ptr<detail::StandardsAttributeCheckRule_Impl>(
        new detail::StandardsAttributeCheckRule_Impl(optionFilters,
                                                     qualifyingFilters,
                                                     ruleResultAction)))
{
  BOOST_ASSERT(getImpl<detail::StandardsAttributeCheckRule_Impl>());
}

StandardsAttributeCheckRule::StandardsAttributeCheckRule(
    const std::string& name, 
    bool active, 
    const UUID& uuid, 
    const UUID& versionUUID,
    const std::vector<FilterClause>& filters, 
    const std::vector<ActionClause>& actions)
  : StandardsRule(boost::shared_ptr<detail::StandardsAttributeCheckRule_Impl>(
        new detail::StandardsAttributeCheckRule_Impl(name,
                                                     active,
                                                     uuid,
                                                     versionUUID,
                                                     filters,
                                                     actions)))
{
  BOOST_ASSERT(getImpl<detail::StandardsAttributeCheckRule_Impl>());
}

StandardsAttributeCheckRule::StandardsAttributeCheckRule(const QDomElement& element)
  : StandardsRule(boost::shared_ptr<detail::StandardsAttributeCheckRule_Impl>(
        new detail::StandardsAttributeCheckRule_Impl(element)))
{
  BOOST_ASSERT(getImpl<detail::StandardsAttributeCheckRule_Impl>());
}

StandardsAttributeCheckRule StandardsAttributeCheckRule::createCompanionRule() const {
  return getImpl<detail::StandardsAttributeCheckRule_Impl>()->createCompanionRule();
}

standardsinterface::ObjectType StandardsAttributeCheckRule::objectType() const {
  return getImpl<detail::StandardsAttributeCheckRule_Impl>()->objectType();
}

std::vector<StandardsFilterObjectAttribute> StandardsAttributeCheckRule::qualifyingFilters() const {
  return getImpl<detail::StandardsAttributeCheckRule_Impl>()->qualifyingFilters();
}

StandardsActionRuleResult StandardsAttributeCheckRule::ruleResultAction() const {
  return getImpl<detail::StandardsAttributeCheckRule_Impl>()->ruleResultAction();
}

void StandardsAttributeCheckRule::createDescriptiveName() {
  getImpl<detail::StandardsAttributeCheckRule_Impl>()->createDescriptiveName();
}

/// @cond
StandardsAttributeCheckRule::StandardsAttributeCheckRule(
    boost::shared_ptr<detail::StandardsAttributeCheckRule_Impl> impl)
  : StandardsRule(impl)
{}
/// @endcond

std::vector<StandardsAttributeCheckRule> loadStandardsAttributeCheckRulesFromFile(
    const openstudio::path& p, bool writeSI)
{
  StandardsAttributeCheckRuleVector result;
  StandardsFilterRulesetOptionVector optionFilters;
  std::vector<StandardsFilterObjectAttributeVector> qualifyingFilters;
  StandardsFilterAvailableModel modelFilter(StandardsRulesetModelType::Original);

  // open p as csv table
  Table table = Table::load(p);
  unsigned nPreamble(0);
  std::string text, ruleTypeDescriptor;
  for (unsigned i = 0, n = table.nRows(); i < n; ++i) {
    ++nPreamble;
    text = print(table.element(i,0));

    // if blank line, end preamble
    if (text.empty()) { break; }

    boost::trim(text);
    // create ruleset option filters if possible
    if (standardsinterface::DataDictionary::isEnumAttribute(text)) {
      standardsinterface::EnumAttribute ea(text);
      text = print(table.element(i,1));
      boost::trim(text);
      std::string value = standardsinterface::DataDictionary::standardEnumValue(ea,text);
      if (value.empty()) {
        LOG_FREE(Warn,"Ruleset.StandardsRule","Unexpected format in '" << toString(p) 
            << "'. Expected ruleset option filters specified one per line, with the name of the "
            << "option in the first column, and the value in the second. However, in row " << i 
            << ", found a ruleset option name in column 1, and and empty cell in column 2.");
        continue;
      }
      StandardsEnumRulesetOption option(ea);
      option.setValue(value);
      StandardsFilterRulesetOption optionFilter(option);
      optionFilters.push_back(optionFilter);
    }
    else {
      // otherwise, get rule type descriptor
      if (!ruleTypeDescriptor.empty()) {
        LOG_FREE(Warn,"Ruleset.StandardsRule","Unexpected format in '" << toString(p) 
            << "'. Expected ruleset option filters (one per line), followed by a single rule "
            << "type descriptor, followed by a blank line.");
      }
      ruleTypeDescriptor = text;
    }
  }

  // header is nPreamble + 4 (objectType, attribute name, [predicate], [units])
  table.setNHead(nPreamble + 4);

  bool tableModified(false);
  for (unsigned j = 0, m = table.nCols(); j < m; ++j) {
    // identify columns with units and convert to SI
    text = print(table.element(nPreamble + 3,j));
    if (!text.empty()) {
      bool ok = table.convertUnits(j,Table::HEAD,nPreamble+3,UnitSystem(UnitSystem::SI));
      if (ok) { 
        if (text != print(table.element(nPreamble+3,j))) { tableModified = true; }
      }
    }
  }

  // if changes and requested, print converted table
  if (tableModified && writeSI) {
    std::stringstream ss;
    ss << toString(p.stem()) << "_SI.csv";
    openstudio::path siPath = p.parent_path() / toPath(ss.str());
    table.save(siPath,true);
  }

  bool isActionColumn(false);
  unsigned ruleIndex(0);
  StandardsFilterAvailableObjectVector objectFilters;
  for (unsigned j = 0, m = table.nCols(); j < m; ++j) {
    // determine object
    text = print(table.element(nPreamble,j));
    boost::trim(text);
    bool ok = standardsinterface::DataDictionary::isObjectType(text);
    if (text.empty() || !ok) {
      if (j == 0) {
        LOG_FREE(Warn,"Ruleset.StandardsRule","Unexpected format in '" << toString(p) 
            << "'. Expected preamble, followed by one blank line, followed by a row of "
            << "ObjectType names. No ObjectType name found in the first column of the expected "
            << "ObjectType name row. Returning empty vector of StandardsRules.");
        return result;
      }
      if (!text.empty() && !ok) {
        LOG_FREE(Warn,"Ruleset.StandardsRule","Unexpected format in '" << toString(p) 
            << "'. Could not convert text in cell (" << nPreamble << "," << j 
            << ") to standardsinterface::ObjectType. Ignoring column " << j << ".");
      }
      else {
        // text empty
        isActionColumn = true;
      }
      continue;
    }
    standardsinterface::ObjectType ot(text);
    StandardsFilterAvailableObject objectFilter(modelFilter,ot);
    StandardsFilterAvailableObjectVector::const_iterator it = std::find_if(
        objectFilters.begin(),
        objectFilters.end(),
        boost::bind(&FilterClause::equivalentData,&objectFilter,_1));
    if (it == objectFilters.end()) {
      objectFilters.push_back(objectFilter);
      it = objectFilters.end();
      --it;
    }
    objectFilter = *it;
    BOOST_ASSERT(objectFilter.objectType() == ot);

    // determine attribute
    text = print(table.element(nPreamble + 1,j));
    boost::trim(text);
    boost::optional<StandardsFilterObjectAttribute> oAttributeFilter;
    if (standardsinterface::DataDictionary::isBooleanAttribute(text)) {
      standardsinterface::BooleanAttribute ba(text);
      StandardsFilterObjectBooleanAttribute baf(objectFilter,ba,true);
      oAttributeFilter = baf;
    }
    else if (standardsinterface::DataDictionary::isDoubleAttribute(text)) {
      standardsinterface::DoubleAttribute da(text);
      // get predicate
      text = print(table.element(nPreamble + 2,j));
      boost::trim(text);
      RulesetNumericalPredicate predicate;
      try {
        predicate = RulesetNumericalPredicate(text);
      }
      catch (...) {
        LOG_FREE(Warn,"Ruleset.StandardsRule","Unexpected format in '" << toString(p) 
          << "'. Could not convert text in cell (" << nPreamble + 2 << "," << j 
          << ") to a RulesetNumericalPredicate (required for double and integer attributes). "
          << "Ignoring column " << j << ".");
        continue;
      }
      // get units
      std::string unitString = table.units(j,Table::HEAD,nPreamble+3);
      StandardsFilterObjectDoubleAttribute daf(objectFilter,da,predicate,0.0,unitString);
      oAttributeFilter = daf;
    }
    else if (standardsinterface::DataDictionary::isEnumAttribute(text)) {
      standardsinterface::EnumAttribute ea(text);
      StandardsFilterObjectEnumAttribute eaf(objectFilter,ea,RulesetStringPredicate::Equals,std::string());
      oAttributeFilter = eaf;
    }
    else if (standardsinterface::DataDictionary::isIntegerAttribute(text)) {
      standardsinterface::IntegerAttribute ia(text);
      // get predicate
      text = print(table.element(nPreamble + 2,j));
      boost::trim(text);
      RulesetNumericalPredicate predicate;
      try {
        predicate = RulesetNumericalPredicate(text);
      }
      catch (...) {
        LOG_FREE(Warn,"Ruleset.StandardsRule","Unexpected format in '" << toString(p) 
          << "'. Could not convert text in cell (" << nPreamble + 2 << "," << j 
          << ") to a RulesetNumericalPredicate (required for double and integer attributes). "
          << "Ignoring column " << j << ".");
        continue;
      }
      // get units
      std::string unitString = table.units(j,Table::HEAD,nPreamble+3);
      StandardsFilterObjectIntegerAttribute iaf(objectFilter,ia,predicate,0,unitString);
      oAttributeFilter = iaf;
    }
    else {
      LOG_FREE(Warn,"Ruleset.StandardsRule","Unexpected format in '" << toString(p) 
          << "'. Could not convert text in cell (" << nPreamble + 1 << "," << j 
          << ") to a standardsinterface attribute. Ignoring column " << j << ".");
      continue;
    }

    BOOST_ASSERT(oAttributeFilter);
    BOOST_ASSERT(oAttributeFilter->object() == objectFilter);

    // loop through rows, creating filters and rules
    unsigned qualifyingFiltersRowIndex = 0;
    for (unsigned i = table.nHead(), n = table.nRows(); i < n; ++i) {
      if (j == 0) {
        // create qualifyingFilters entry
        qualifyingFilters.push_back(StandardsFilterObjectAttributeVector());
      }

      // create attribute filter
      StandardsFilterObjectAttribute attributeFilter = oAttributeFilter->clone().cast<StandardsFilterObjectAttribute>();
      text = print(table.element(i,j));
      boost::trim(text);
      bool filterIsNull(true);
      if (!text.empty()) {
        // set filter value
        bool ok = attributeFilter.setValue(text);
        if (ok) { filterIsNull = false; }
        else {
          LOG_FREE(Warn,"Ruleset.StandardsRule","Unexpected format in '" << toString(p) 
            << "'. Could not convert text in cell (" << i << "," << j 
            << ") to a standardsinterface attribute value. Ignoring this cell.");
        }
      }

      if (!isActionColumn) {
        // add attribute filter to rule
        if (!filterIsNull) {
          qualifyingFilters[qualifyingFiltersRowIndex].push_back(attributeFilter);
        }
      }
      else {
        // create rule satisfied action
        StandardsActionRuleSatisfied ruleSatisfiedAction(attributeFilter);

        // create new rule
        result.push_back(StandardsAttributeCheckRule(optionFilters,
                                                     qualifyingFilters[qualifyingFiltersRowIndex],
                                                     ruleSatisfiedAction));
        ++ruleIndex;
      }

      ++qualifyingFiltersRowIndex;
    }
  }

  return result;
}

} // ruleset
} // openstudio
