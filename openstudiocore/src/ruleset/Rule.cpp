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

#include <ruleset/Rule.hpp>
#include <ruleset/Rule_Impl.hpp>
#include <ruleset/ModelRule.hpp>
#include <ruleset/FilterClause.hpp>
#include <ruleset/FilterClause_Impl.hpp>
#include <ruleset/ActionClause.hpp>
#include <ruleset/ActionClause_Impl.hpp>

#include <utilities/core/Assert.hpp>

#include <boost/foreach.hpp>
#include <boost/bind.hpp>

#include <QDomDocument>
#include <QDomElement>

namespace openstudio {
namespace ruleset {

namespace detail {

  Rule_Impl::Rule_Impl(const std::string& name)
    : RulesetObject_Impl(), m_name(name), m_active(true)
  {
    OS_ASSERT(m_filters.empty());
    OS_ASSERT(m_actions.empty());
  }

  Rule_Impl::Rule_Impl(const std::string& name, bool active, const UUID& uuid, const UUID& versionUUID,
                       const std::vector<FilterClause>& filters, const std::vector<ActionClause>& actions)
    : RulesetObject_Impl(uuid, versionUUID), m_filters(filters), m_actions(actions), m_name(name), m_active(active)
  {}

  Rule_Impl::Rule_Impl(const QDomElement& element)
    : RulesetObject_Impl(element)
  {
    OS_ASSERT(!element.isNull());

    // name
    QDomElement nameElement = element.firstChildElement(QString::fromStdString("Name"));
    OS_ASSERT(!nameElement.isNull());
    m_name = nameElement.firstChild().nodeValue().toStdString();

    // active
    QDomElement activeElement = element.firstChildElement(QString::fromStdString("Active"));
    OS_ASSERT(!activeElement.isNull());
    std::string boolValue = activeElement.firstChild().nodeValue().toStdString();
    if (boolValue == "true") { m_active = true; }
    else if (boolValue == "false") { m_active = false; }
    else { OS_ASSERT(false); }

    QDomElement filtersElement = element.firstChildElement(QString::fromStdString("Filters"));
    QDomElement actionsElement = element.firstChildElement(QString::fromStdString("Actions"));

    if(!filtersElement.isNull()){
      QDomNodeList childNodes = filtersElement.childNodes();
      for (int i = 0; i < childNodes.count(); i++){
        QDomElement childElement = childNodes.at(i).toElement();
        boost::optional<Clause> clause = Clause::factoryFromXml(childElement);
        OS_ASSERT(clause);
        m_filters.push_back(clause->cast<FilterClause>());
      }
    }

    if(!actionsElement.isNull()){
      QDomNodeList childNodes = actionsElement.childNodes();
      for (int i = 0; i < childNodes.count(); i++){
        QDomElement childElement = childNodes.at(i).toElement();
        boost::optional<Clause> clause = Clause::factoryFromXml(childElement);
        OS_ASSERT(clause);
        m_actions.push_back(clause->cast<ActionClause>());
      }
    }

  }

  Rule Rule_Impl::rule() const {
    return this->getObject<Rule>();
  }

  std::string Rule_Impl::name() const {
    return m_name;
  }

  bool Rule_Impl::active() const { 
    return m_active;
  }

  std::vector<FilterClause> Rule_Impl::filters() const {
    return m_filters;
  }

  std::vector<ActionClause> Rule_Impl::actions() const {
    return m_actions;
  }

  void Rule_Impl::setName(const std::string& name) {
    m_name = name;
    this->onChange();
  }

  void Rule_Impl::activate() {
    if (!m_active) {
      m_active = true;
      this->onChange();
    }
  }

  void Rule_Impl::disable() {
    if (m_active) {
      m_active = false;
      this->onChange();
    }
  }

  bool Rule_Impl::add(const FilterClause& filter) {
    if (contains(filter)) { return false; }

    m_filters.push_back(filter);
    this->connectChild(filter);
    this->onChange();
    return true;
  }

  bool Rule_Impl::add(const std::vector<FilterClause>& filters) {
    bool result(true);
    BOOST_FOREACH(const FilterClause& filter,filters) {
      result = result && this->add(filter);
    }
    return result;
  }

  bool Rule_Impl::add(const ActionClause& action) {
    if (contains(action)) { return false; }

    m_actions.push_back(action);
    this->connectChild(action);
    this->onChange();
    return true;
  }

  bool Rule_Impl::remove(const FilterClause& filter) {
    FilterClauseVector::iterator it = std::find(m_filters.begin(),m_filters.end(),filter);
    if (it != m_filters.end()) {
      // delete from m_filters
      this->disconnectChild(*it);
      it = m_filters.erase(it);
      this->onChange();
      return true;
    }
    return false;
  }

  bool Rule_Impl::remove(const ActionClause& action) {
    ActionClauseVector::iterator it = std::find(m_actions.begin(),m_actions.end(),action);
    if (it != m_actions.end()) {
      // delete from m_actions
      this->disconnectChild(*it);
      it = m_actions.erase(it);
      this->onChange();
      return true;
    }
    return false;
  }

  unsigned Rule_Impl::clearFilters() {
    FilterClauseVector filters = this->filters();
    BOOST_FOREACH(const FilterClause& filter,filters) {
      remove(filter);
    }
    return filters.size();
  }

  unsigned Rule_Impl::clearActions() {
    ActionClauseVector actions = this->actions();
    BOOST_FOREACH(const ActionClause& action,actions) {
      remove(action);
    }
    return actions.size();
  }

  unsigned Rule_Impl::numFilters() const {
    return m_filters.size();
  }

  unsigned Rule_Impl::numActions() const {
    return m_actions.size();
  }

  bool Rule_Impl::contains(const FilterClause& filter) const {
    return (std::find(m_filters.begin(),m_filters.end(),filter) != m_filters.end());
  }

  bool Rule_Impl::containsEquivalentData(const FilterClause& filter) const {
    return (std::find_if(m_filters.begin(),m_filters.end(),
        boost::bind(&FilterClause::equivalentData,&filter,_1)) != m_filters.end());
  }

  bool Rule_Impl::contains(const ActionClause& action) const {
    return (std::find(m_actions.begin(),m_actions.end(),action) != m_actions.end());
  }

  bool Rule_Impl::containsEquivalentData(const ActionClause& action) const {
    return (std::find_if(m_actions.begin(),m_actions.end(),
        boost::bind(&ActionClause::equivalentData,&action,_1)) != m_actions.end());
  }

  void Rule_Impl::writeValues(QDomDocument& doc, QDomElement& element) const
  {
    RulesetObject_Impl::writeValues(doc, element);

    QDomElement childElement;
    QDomText text;

    childElement = doc.createElement(QString::fromStdString("Name"));
    text = doc.createTextNode(toQString(m_name));
    childElement.appendChild(text);
    element.appendChild(childElement);

    childElement = doc.createElement(QString::fromStdString("Active"));
    if (m_active) {
      text = doc.createTextNode(toQString(std::string("true")));
    }
    else {
      text = doc.createTextNode(toQString(std::string("false")));
    }
    childElement.appendChild(text);
    element.appendChild(childElement);

    childElement = doc.createElement(QString::fromStdString("Filters"));
    BOOST_FOREACH(const FilterClause& filter, m_filters){
      childElement.appendChild(filter.toXml().documentElement());
    }
    element.appendChild(childElement);

    childElement = doc.createElement(QString::fromStdString("Actions"));
    BOOST_FOREACH(const ActionClause& action, m_actions){
      childElement.appendChild(action.toXml().documentElement());
    }
    element.appendChild(childElement);
  }

  bool Rule_Impl::equivalentData(const RulesetObject& other) const {

    bool result = true;

    result = result && RulesetObject_Impl::equivalentData(other);

    boost::optional<Rule> rule = other.optionalCast<Rule>();
    if (!rule){
      return false;
    }

    if ((numFilters() == rule->numFilters()) && (numActions() == rule->numActions()))
    {
      FilterClauseVector otherFilters = rule->filters();
      BOOST_FOREACH(const FilterClause& filter,m_filters) {
        FilterClauseVector::const_iterator it = std::find_if(otherFilters.begin(),
            otherFilters.end(),boost::bind(&FilterClause::equivalentData,&filter,_1));
        if (it == otherFilters.end()) { 
          return false; 
        }
      }

      ActionClauseVector otherActions = rule->actions();
      BOOST_FOREACH(const ActionClause& action,m_actions) {
        ActionClauseVector::const_iterator it = std::find_if(otherActions.begin(),
            otherActions.end(),boost::bind(&ActionClause::equivalentData,&action,_1));
        if (it == otherActions.end()) { 
          return false; 
        }
      }

      return result;
    }
    return false;
  }

} // detail

std::string Rule::name() const {
  return this->getImpl<detail::Rule_Impl>()->name();
}

bool Rule::active() const {
  return getImpl<detail::Rule_Impl>()->active();
}

boost::optional<Rule> Rule::factoryFromXml(const QDomElement& element)
{
  boost::optional<Rule> result;

  std::string xmlElementName = toString(element.tagName());
  if (xmlElementName == ModelRule::xmlElementName()){
    result = ModelRule(element);
  }

  return result;
}

std::vector<FilterClause> Rule::filters() const {
  return this->getImpl<detail::Rule_Impl>()->filters();
}

std::vector<ActionClause> Rule::actions() const {
  return this->getImpl<detail::Rule_Impl>()->actions();
}

void Rule::setName(const std::string& name) {
  this->getImpl<detail::Rule_Impl>()->setName(name);
}

void Rule::activate() {
  getImpl<detail::Rule_Impl>()->activate();
}

void Rule::disable() {
  getImpl<detail::Rule_Impl>()->disable();
}

bool Rule::add(const FilterClause& filter) {
  return this->getImpl<detail::Rule_Impl>()->add(filter);
}

bool Rule::add(const std::vector<FilterClause>& filters) {
  return this->getImpl<detail::Rule_Impl>()->add(filters);
}

bool Rule::add(const ActionClause& action) {
  return this->getImpl<detail::Rule_Impl>()->add(action);
}

bool Rule::remove(const FilterClause& filter) {
  return this->getImpl<detail::Rule_Impl>()->remove(filter);
}

bool Rule::remove(const ActionClause& action) {
  return this->getImpl<detail::Rule_Impl>()->remove(action);
}

unsigned Rule::clearFilters() {
  return this->getImpl<detail::Rule_Impl>()->clearFilters();
}

unsigned Rule::clearActions() {
  return this->getImpl<detail::Rule_Impl>()->clearActions();
}

unsigned Rule::numFilters() const {
  return this->getImpl<detail::Rule_Impl>()->numFilters();
}

unsigned Rule::numActions() const {
  return this->getImpl<detail::Rule_Impl>()->numActions();
}

bool Rule::contains(const FilterClause& filter) const {
  return this->getImpl<detail::Rule_Impl>()->contains(filter);
}

bool Rule::containsEquivalentData(const FilterClause& filter) const {
  return getImpl<detail::Rule_Impl>()->containsEquivalentData(filter);
}

bool Rule::contains(const ActionClause& action) const {
  return this->getImpl<detail::Rule_Impl>()->contains(action);
}

bool Rule::containsEquivalentData(const ActionClause& action) const {
  return getImpl<detail::Rule_Impl>()->containsEquivalentData(action);
}

/// @cond
Rule::Rule(const boost::shared_ptr<detail::Rule_Impl>& impl)
  : RulesetObject(impl)
{
  OS_ASSERT(getImpl<detail::Rule_Impl>());
}
/// @endcond

} // ruleset
} // openstudio
