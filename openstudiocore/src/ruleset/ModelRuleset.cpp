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

#include <ruleset/ModelRuleset.hpp>
#include <ruleset/ModelRuleset_Impl.hpp>
#include <ruleset/ModelRule.hpp>
#include <ruleset/ModelRule_Impl.hpp>

#include <boost/foreach.hpp>
#include <boost/bind.hpp>

#include <QDomDocument>
#include <QDomElement>

namespace openstudio {
namespace ruleset {

namespace detail {

  ModelRuleset_Impl::ModelRuleset_Impl(const std::string& name)
    : Ruleset_Impl(name)
  {}

  ModelRuleset_Impl::ModelRuleset_Impl(const std::string& name, const UUID& uuid, const UUID& versionUUID,
                                       const std::vector<ModelRule>& rules)
    : Ruleset_Impl(name, uuid, versionUUID), m_rules(rules)
  {}

  ModelRuleset_Impl::ModelRuleset_Impl(const QDomElement& element)
    : Ruleset_Impl(element)
  {
    OS_ASSERT(!element.isNull());
    OS_ASSERT(element.tagName() == toQString(this->xmlElementName()));

    {
      QString str;
      QTextStream qts(&str);
      element.save(qts, 2);
    }

    QDomElement rulesElement = element.firstChildElement(QString::fromStdString("Rules"));

    if(!rulesElement.isNull()){

      {
        QString str;
        QTextStream qts(&str);
        rulesElement.save(qts, 2);
      }

      QDomNodeList childNodes = rulesElement.childNodes();
      for (int i = 0; i < childNodes.count(); i++){
        QDomElement childElement = childNodes.at(i).toElement();
        boost::optional<Rule> rule = Rule::factoryFromXml(childElement);
        OS_ASSERT(rule);
        m_rules.push_back(rule->cast<ModelRule>());
      }
    }else{
      LOG(Debug, "rulesElement is null");
    }
  }

  std::vector<ModelRule> ModelRuleset_Impl::rules() const {
    return m_rules;
  }

  void ModelRuleset_Impl::add(const ModelRule& rule) {
    m_rules.push_back(rule);
    this->connectChild(rule);
    this->onChange();
  }

  std::string ModelRuleset_Impl::xmlElementName() const
  {
    return ModelRuleset::xmlElementName();
  }

  void ModelRuleset_Impl::writeValues(QDomDocument& doc, QDomElement& element) const
  {
    Ruleset_Impl::writeValues(doc, element);

    QDomElement childElement;
    QDomText text;

    childElement = doc.createElement(QString::fromStdString("Rules"));
    BOOST_FOREACH(const ModelRule& modelRule, m_rules){
      childElement.appendChild(modelRule.toXml().documentElement());
    }
    element.appendChild(childElement);
  }

  bool ModelRuleset_Impl::equivalentData(const RulesetObject& other) const {
    bool result = true;
    result = result && Ruleset_Impl::equivalentData(other);
    return result;
  }

  bool ModelRuleset_Impl::apply(openstudio::model::Model& model)
  {
    bool result = true;
    BOOST_FOREACH(const ModelRule& rule, m_rules){
      result = result && rule.apply(model);
    }
    return result;
  }

} // detail

std::string ModelRuleset::xmlElementName()
{
  return "ModelRuleset";
}

ModelRuleset::ModelRuleset(const std::string& name)
  : Ruleset(boost::shared_ptr<detail::ModelRuleset_Impl>(new detail::ModelRuleset_Impl(name)))
{
  OS_ASSERT(getImpl<detail::ModelRuleset_Impl>());
}

ModelRuleset::ModelRuleset(const std::string& name, const UUID& uuid, const UUID& versionUUID,
                           const std::vector<ModelRule>& rules)
  : Ruleset(boost::shared_ptr<detail::ModelRuleset_Impl>(new detail::ModelRuleset_Impl(name,uuid,versionUUID,rules)))
{
  OS_ASSERT(getImpl<detail::ModelRuleset_Impl>());
}

ModelRuleset::ModelRuleset(const QDomElement& element)
  : Ruleset(boost::shared_ptr<detail::ModelRuleset_Impl>(new detail::ModelRuleset_Impl(element)))
{
  OS_ASSERT(getImpl<detail::ModelRuleset_Impl>());
}

/// @cond
ModelRuleset::ModelRuleset(boost::shared_ptr<detail::ModelRuleset_Impl> impl)
  : Ruleset(impl)
{
  OS_ASSERT(getImpl<detail::ModelRuleset_Impl>());
}
/// @endcond

std::vector<ModelRule> ModelRuleset::rules() const {
  return getImpl<detail::ModelRuleset_Impl>()->rules();
}

void ModelRuleset::add(const ModelRule& rule) {
  getImpl<detail::ModelRuleset_Impl>()->add(rule);
}

bool ModelRuleset::apply(openstudio::model::Model& model)
{
  return getImpl<detail::ModelRuleset_Impl>()->apply(model);
}

} // ruleset
} // openstudio
