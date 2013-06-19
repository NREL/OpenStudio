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

#include <ruleset/StandardsRuleset.hpp>
#include <ruleset/StandardsRuleset_Impl.hpp>

#include <ruleset/StandardsAttributeCheckRule.hpp>
#include <ruleset/StandardsAttributeCheckRule_Impl.hpp>
#include <ruleset/StandardsRulesetOption_Impl.hpp>
#include <ruleset/StandardsFilterRulesetOption.hpp>
#include <ruleset/StandardsFilterRulesetOption_Impl.hpp>
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
#include <ruleset/StandardsActionGetObjects.hpp>
#include <ruleset/StandardsActionGetObjects_Impl.hpp> // TEMP DEBUG CODE
#include <ruleset/StandardsActionGetAttribute.hpp>

#include <utilities/core/Assert.hpp>
#include <utilities/core/Path.hpp>
#include <utilities/core/PathHelpers.hpp>
#include <utilities/core/Checksum.hpp>
#include <utilities/core/UUID.hpp>
#include <utilities/core/Containers.hpp>

#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include <boost/filesystem/fstream.hpp>

#include <QDomDocument>
#include <QDomElement>

namespace openstudio {
namespace ruleset {

namespace detail {

  StandardsRuleset_Impl::StandardsRuleset_Impl(const std::string& name)
    : Ruleset_Impl(name)
  {}

  StandardsRuleset_Impl::StandardsRuleset_Impl(const std::string& name, const UUID& uuid, const UUID& versionUUID,
                                               const std::vector<StandardsRulesetOption>& options, const std::vector<StandardsRule>& rules)
    : Ruleset_Impl(name, uuid, versionUUID), m_options(options), m_rules(rules)
  {}

 StandardsRuleset_Impl::StandardsRuleset_Impl(const QDomElement& element)
    : Ruleset_Impl(element)
  {
    BOOST_ASSERT(!element.isNull());
    BOOST_ASSERT(element.tagName() == toQString(this->xmlElementName()));
  }

  std::vector<StandardsRulesetOption> StandardsRuleset_Impl::options() const {
    // clone the data
    StandardsRulesetOptionVector result;
    BOOST_FOREACH(const StandardsRulesetOption& option,m_options) {
      result.push_back(option.clone().cast<StandardsRulesetOption>());
    }
    return result;
  }

  std::vector<StandardsRulesetOption> StandardsRuleset_Impl::optionsNoClone() const {
    return m_options;
  }

  std::vector<StandardsRule> StandardsRuleset_Impl::rules(bool includeDisabled) const {
    // do not clone the data
    if (includeDisabled) {
      return m_rules;
    }
    StandardsRuleVector result;
    BOOST_FOREACH(const StandardsRule& rule,m_rules) {
      if (rule.active()) {
        result.push_back(rule);
      }
    }
    return result;
  }

  std::vector<StandardsRule> StandardsRuleset_Impl::impliedRules(bool includeDisabled) const {
    StandardsRuleVector result;

    BOOST_FOREACH(const StandardsRule& rule,rules(includeDisabled)) {
      StandardsRuleVector candidates = rule.impliedRules();
      BOOST_FOREACH(StandardsRule& candidate, candidates) {
        if (!rule.active()) { candidate.disable(); }
        StandardsRuleVector::iterator it = std::find_if(result.begin(),result.end(),
            boost::bind(&StandardsRule::equivalentData,&candidate,_1));
        if (it == result.end()) {
          result.push_back(candidate);
        }
        else if (includeDisabled && rule.active()) {
          it->activate();
        }
      }
    }

    return result;
  }

  bool StandardsRuleset_Impl::add(const StandardsRulesetOption& option) {
    /* Do not need to clone on add
    RulesetOption optionClone = option.clone();
    m_options.push_back(optionClone.cast<StandardsRulesetOption>());
    this->connectChild(optionClone);
    */
    if (!containsEquivalentOptionType(option)) {
      m_options.push_back(option);
      this->connectChild(option);
      this->onChange();
      return true;
    }
    return false;
  }

  void StandardsRuleset_Impl::add(const StandardsRule& rule) {
    // add dependent options
    StandardsFilterRulesetOptionVector optionFilters;
    optionFilters = rule.getFilters<StandardsFilterRulesetOption>();
    BOOST_FOREACH(const StandardsFilterRulesetOption& optionFilter,optionFilters) {
      add(optionFilter.option());
    }
    // add rule
    m_rules.push_back(rule);
    this->connectChild(rule);
    this->onChange();
  }

  bool StandardsRuleset_Impl::remove(const StandardsRulesetOption& option) {
    StandardsRulesetOptionVector::iterator it = std::find(m_options.begin(),m_options.end(),option);
    if (it != m_options.end()) {
      // remove related StandardsFilterRulesetOptions from rules
      StandardsRuleVector myRules = rules(true);
      BOOST_FOREACH(StandardsRule& rule,myRules) {
        StandardsFilterRulesetOptionVector optionFilters = rule.getFilters<StandardsFilterRulesetOption>();
        BOOST_FOREACH(const StandardsFilterRulesetOption& optionFilter,optionFilters) {
          if (optionFilter.option().equivalentOptionType(option)) {
            bool ok = rule.remove(optionFilter);
            BOOST_ASSERT(ok);
          }
        }
      }
      // remove option
      this->disconnectChild(*it);
      it = m_options.erase(it);
      this->onChange();
      return true;
    }
    return false;    
  }

  bool StandardsRuleset_Impl::remove(const StandardsRule& rule) {
    StandardsRuleVector::iterator it = std::find(m_rules.begin(),m_rules.end(),rule);
    if (it != m_rules.end()) {
      this->disconnectChild(*it);
      it = m_rules.erase(it);
      this->onChange();
      return true;
    }
    return false;
  }

  std::string StandardsRuleset_Impl::print(const StandardsRulesetPrintMode& mode) const {
    std::stringstream ss;
    BOOST_FOREACH(const StandardsRulesetOption& option, m_options) {
      ss << option.printDefinition() << std::endl;
    }
    BOOST_FOREACH(const StandardsRule& rule, m_rules) {
      ss << rule.print(mode) << std::endl;
    }
    // print implied rules for engine
    if (mode == StandardsRulesetPrintMode::Engine) {
      StandardsRuleVector impliedRules = this->impliedRules();
      BOOST_FOREACH(StandardsRule& impliedRule, impliedRules) {
        ss << impliedRule.print(mode) << std::endl;
      }
    }
    return ss.str();
  }

  std::string StandardsRuleset_Impl::xmlElementName() const
  {
    return StandardsRuleset::xmlElementName();
  }

  void StandardsRuleset_Impl::writeValues(QDomDocument& doc, QDomElement& element) const
  {
    Ruleset_Impl::writeValues(doc, element);
  }

  bool StandardsRuleset_Impl::save(const openstudio::path& p, bool overwrite) {
    // default extension
    std::string expectedExtension = rulesetFileExtension();

    path wp = completePathToFile(p,path(),expectedExtension,true);
    if (!wp.empty() && (overwrite == false)) {
      LOG(Info,"Ruleset save method failed because instructed not to overwrite path '"
          << toString(p) << "'.");
      return false;
    }

    if (wp.empty()) { wp = p; }

    if (makeParentFolder(wp)) {
      boost::filesystem::ofstream outFile(wp);
      if (outFile) {
        try {
          outFile << print(StandardsRulesetPrintMode::File);
          outFile.close();
          return true;
        }
        catch (...) {
          LOG(Error,"Unable to write Ruleset to path '" << toString(p) << "'.");
          return false;
        }
      }
    }

    LOG(Error,"Unable to write Ruleset to path '" << toString(p)
        << "', because parent directory could not be created.");
    return false;
  }

  bool StandardsRuleset_Impl::equivalentData(const RulesetObject& other) const {
    bool result = true;
    result = result && Ruleset_Impl::equivalentData(other);
    return result;
  }

  unsigned StandardsRuleset_Impl::numOptions() const {
    return m_options.size();
  }

  unsigned StandardsRuleset_Impl::numRules(bool includeDisabled) const {
    return rules(includeDisabled).size();
  }

  bool StandardsRuleset_Impl::contains(const StandardsRulesetOption& option) const {
    return (std::find(m_options.begin(),m_options.end(),option) != m_options.end());
  }

  bool StandardsRuleset_Impl::containsEquivalentData(const StandardsRulesetOption& option) const {
    return (std::find_if(m_options.begin(),m_options.end(),
        boost::bind(&StandardsRulesetOption::equivalentData,&option,_1)) != m_options.end());
  }

  /** Same as containsEquivalentData, except ignores differences in option value or default 
   *  value. */
  bool StandardsRuleset_Impl::containsEquivalentOptionType(
      const StandardsRulesetOption& option) const
  {
    return (std::find_if(m_options.begin(),m_options.end(),
        boost::bind(&StandardsRulesetOption::equivalentOptionType,&option,_1)) != m_options.end());
  }

  bool StandardsRuleset_Impl::contains(const StandardsRule& rule) const {
    return (std::find(m_rules.begin(),m_rules.end(),rule) != m_rules.end());
  }

  bool StandardsRuleset_Impl::containsEquivalentData(const StandardsRule& rule) const {
    return (std::find_if(m_rules.begin(),m_rules.end(),
        boost::bind(&StandardsRule::equivalentData,&rule,_1)) != m_rules.end());
  }

} // detail

/// @cond
std::string StandardsRuleset::xmlElementName()
{
  return "StandardsRuleset";
}
/// @endcond

StandardsRuleset::StandardsRuleset(const std::string& name)
  : Ruleset(boost::shared_ptr<detail::StandardsRuleset_Impl>(new detail::StandardsRuleset_Impl(name)))
{
  BOOST_ASSERT(getImpl<detail::StandardsRuleset_Impl>());
}

StandardsRuleset::StandardsRuleset(const std::string& name, const UUID& uuid, const UUID& versionUUID,
                                   const std::vector<StandardsRulesetOption>& options, const std::vector<StandardsRule>& rules)
  : Ruleset(boost::shared_ptr<detail::StandardsRuleset_Impl>(new detail::StandardsRuleset_Impl(name, uuid, versionUUID, options, rules)))
{
  BOOST_ASSERT(getImpl<detail::StandardsRuleset_Impl>());
}

StandardsRuleset::StandardsRuleset(const QDomElement& element)
  : Ruleset(boost::shared_ptr<detail::StandardsRuleset_Impl>(new detail::StandardsRuleset_Impl(element)))
{
  BOOST_ASSERT(getImpl<detail::StandardsRuleset_Impl>());
}

std::vector<StandardsRulesetOption> StandardsRuleset::options() const {
  return getImpl<detail::StandardsRuleset_Impl>()->options();
}

std::vector<StandardsRulesetOption> StandardsRuleset::optionsNoClone() const {
  return getImpl<detail::StandardsRuleset_Impl>()->optionsNoClone();
}

std::vector<StandardsRule> StandardsRuleset::rules(bool includeDisabled) const {
  return getImpl<detail::StandardsRuleset_Impl>()->rules(includeDisabled);
}

std::vector<StandardsRule> StandardsRuleset::impliedRules(bool includeDisabled) const {
  return getImpl<detail::StandardsRuleset_Impl>()->impliedRules(includeDisabled);
}

bool StandardsRuleset::add(const StandardsRulesetOption& option) {
  return getImpl<detail::StandardsRuleset_Impl>()->add(option);
}

void StandardsRuleset::add(const StandardsRule& rule) {
  getImpl<detail::StandardsRuleset_Impl>()->add(rule);
}

bool StandardsRuleset::remove(const StandardsRulesetOption& option) {
  return getImpl<detail::StandardsRuleset_Impl>()->remove(option);
}

bool StandardsRuleset::remove(const StandardsRule& rule) {
  return getImpl<detail::StandardsRuleset_Impl>()->remove(rule);
}

unsigned StandardsRuleset::numOptions() const {
  return getImpl<detail::StandardsRuleset_Impl>()->numOptions();
}

unsigned StandardsRuleset::numRules(bool includeDisabled) const {
  return getImpl<detail::StandardsRuleset_Impl>()->numRules(includeDisabled);
}

bool StandardsRuleset::contains(const StandardsRulesetOption& option) const {
  return getImpl<detail::StandardsRuleset_Impl>()->contains(option);
}

bool StandardsRuleset::containsEquivalentData(const StandardsRulesetOption& option) const {
  return getImpl<detail::StandardsRuleset_Impl>()->containsEquivalentData(option);
}

bool StandardsRuleset::containsEquivalentOptionType(const StandardsRulesetOption& option) const {
  return getImpl<detail::StandardsRuleset_Impl>()->containsEquivalentOptionType(option);
}

bool StandardsRuleset::contains(const StandardsRule& rule) const {
  return getImpl<detail::StandardsRuleset_Impl>()->contains(rule);
}

bool StandardsRuleset::containsEquivalentData(const StandardsRule& rule) const {
  return getImpl<detail::StandardsRuleset_Impl>()->containsEquivalentData(rule);
}

std::string StandardsRuleset::print(const StandardsRulesetPrintMode& mode) const {
  return getImpl<detail::StandardsRuleset_Impl>()->print(mode);
}

bool StandardsRuleset::save(const openstudio::path& p, bool overwrite) {
  return getImpl<detail::StandardsRuleset_Impl>()->save(p,overwrite);
}

/// @cond
StandardsRuleset::StandardsRuleset(boost::shared_ptr<detail::StandardsRuleset_Impl> impl)
  : Ruleset(impl)
{
  BOOST_ASSERT(getImpl<detail::StandardsRuleset_Impl>());
}
/// @endcond

} // ruleset
} // openstudio
