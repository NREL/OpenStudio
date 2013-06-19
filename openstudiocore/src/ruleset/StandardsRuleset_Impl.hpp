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

#ifndef RULESET_STANDARDSRULESET_IMPL_HPP
#define RULESET_STANDARDSRULESET_IMPL_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/Ruleset_Impl.hpp>
#include <ruleset/StandardsRule.hpp>
#include <ruleset/StandardsRulesetOption.hpp>

#include <utilities/core/Path.hpp>

namespace openstudio {
namespace ruleset {

class StandardsRuleset;

namespace detail {

  class RULESET_API StandardsRuleset_Impl : public Ruleset_Impl {

    Q_OBJECT;

   public:
    /** @name Constructors and Destructors */
    //@{

    /** Default constructor. Creates empty ruleset. */
    StandardsRuleset_Impl(const std::string& name);

    StandardsRuleset_Impl(const std::string& name, const UUID& uuid, const UUID& versionUUID,
                          const std::vector<StandardsRulesetOption>& options, const std::vector<StandardsRule>& rules);

    StandardsRuleset_Impl(const QDomElement& element);
    
    virtual ~StandardsRuleset_Impl() {}

    //@}
    /** @name Getters */
    //@{

    std::vector<StandardsRulesetOption> options() const;

    std::vector<StandardsRulesetOption> optionsNoClone() const;

    std::vector<StandardsRule> rules(bool includeDisabled=true) const;

    std::vector<StandardsRule> impliedRules(bool includeDisabled=false) const;

    //@}
    /** @name Setters */
    //@{

    /** Add option to Ruleset. Returns false if containsEquivalentOptionType(option). */
    bool add(const StandardsRulesetOption& option);

    /** Add rule to Ruleset. Adds \link StandardsRulesetOption StandardsRulesetOptions 
     *  \endlink as needed for any \link StandardsRulesetOption StandardsFilterRulesetOptions 
     *  \endlink to be valid. For performance reasons, does not check if 
     *  containsEquivalentData(rule). Please perform this check yourself if duplication is 
     *  a concern. */
    void add(const StandardsRule& rule);

    /** Remove option from Ruleset. Returns false if (!contains(option)). Removes  
     *  \link StandardsRulesetOption StandardsFilterRulesetOptions \endlink with 
     *  option.equivalentOptionType(optionFilter.option()) from all rules as needed to 
     *  keep the Ruleset consistent. */
    bool remove(const StandardsRulesetOption& option);

    /** Remove rule from Ruleset. Returns false if (!contains(rule)). */
    bool remove(const StandardsRule& rule);

    //@}
    /** @name Serialization */
    //@{

    std::string print(const StandardsRulesetPrintMode& mode) const;

    /** Save this Ruleset to file (preferred extenstion .osr). May fail. */
    bool save(const openstudio::path& p, bool overwrite=false);

    virtual std::string xmlElementName() const;

    virtual void writeValues(QDomDocument& doc, QDomElement& element) const;

    //@}
    /** @name Queries */
    //@{

    /** Equality as determined by deep comparison of data members. */
    virtual bool equivalentData(const RulesetObject& other) const;

    unsigned numOptions() const;

    unsigned numRules(bool includeDisabled=true) const;

    bool contains(const StandardsRulesetOption& option) const;

    bool containsEquivalentData(const StandardsRulesetOption& option) const;

    /** Same as containsEquivalentData, except ignores differences in option value or default 
     *  value. */
    bool containsEquivalentOptionType(const StandardsRulesetOption& option) const;

    bool contains(const StandardsRule& rule) const;

    bool containsEquivalentData(const StandardsRule& rule) const;

    //@}
   protected:
    friend class openstudio::ruleset::Ruleset;

   private:
    std::vector<StandardsRulesetOption> m_options;
    std::vector<StandardsRule> m_rules;

    REGISTER_LOGGER("Ruleset.StandardsRuleset");
  };

} // detail

} // ruleset
} // openstudio

#endif // RULESET_STANDARDSRULESET_IMPL_HPP
