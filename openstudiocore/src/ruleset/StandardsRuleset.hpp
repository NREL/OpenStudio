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

#ifndef RULESET_STANDARDSRULESET_HPP
#define RULESET_STANDARDSRULESET_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/Ruleset.hpp>
#include <ruleset/StandardsRulesetEnums.hpp>

#include <utilities/core/Path.hpp>

namespace openstudio {

namespace standardsinterface {
  class BooleanAttribute;
  class EnumAttribute;
}

namespace ruleset {

class StandardsRule;
class StandardsRulesetOption;

namespace detail {
  class StandardsRuleset_Impl;
} // detail

/** A StandardsRuleset Ruleset consists of a number of \link StandardsRulesetOption 
 *  StandardsRulesetOptions \endlink and \link StandardsRule StandardsRules \endlink. It is an 
 *  electronic representation of a building energy standard, built up from grammatical clauses
 *  containing DataDictionary (see the standardsinterface documentation) terms, and acting on
 *  the abstract notion of a building model and building model objects. */
class RULESET_API StandardsRuleset : public Ruleset {
 public:
  /// @cond
  typedef detail::StandardsRuleset_Impl ImplType;

  static std::string xmlElementName();
  /// @endcond

  /** @name Constructors and Destructors */
  //@{

  /** Default constructor. Creates empty ruleset named name. */
  StandardsRuleset(const std::string& name);

  /** Constructor provided for deserialization; not for general use. */
  StandardsRuleset(const std::string& name, 
                   const UUID& uuid, 
                   const UUID& versionUUID,
                   const std::vector<StandardsRulesetOption>& options, 
                   const std::vector<StandardsRule>& rules);

  /** Constructor provided for deserialization; not for general use. */
  StandardsRuleset(const QDomElement& element);
  
  virtual ~StandardsRuleset() {}

  //@}
  /** @name Getters */
  //@{

  /** Returns cloned copies of the options in this rulest. The values of these options can be 
   *  changed without affecting the underlying state of the ruleset. */
  std::vector<StandardsRulesetOption> options() const;

  /** Retuns the options in this ruleset. The returned objects share their data with the option
   *  instances in this ruleset. */
  std::vector<StandardsRulesetOption> optionsNoClone() const;

  /** Get the \link StandardsRule StandardsRules \endlink in this Ruleset. If includedDisabled,
   *  all rules are returned. Otherwise, only active() rules are returned. Returned objects share
   *  data with the ruleset. */
  std::vector<StandardsRule> rules(bool includeDisabled=true) const;

  /** Return the \link StandardsRule StandardsRules \endlink implied by this Ruleset. Implied 
   *  rules are needed by the rulesengine, but do not have to be written by the user; they can
   *  be deduced from the main rules. For instance, if a ruleset filters by available model type
   *  and object type, one of the implied rules will tell the rulesengine to get all such 
   *  objects. */
  std::vector<StandardsRule> impliedRules(bool includeDisabled=false) const;

  //@}
  /** @name Setters */
  //@{

  /** Add option to Ruleset. Returns false if containsEquivalentOptionType(option). */
  bool add(const StandardsRulesetOption& option);

  /** Add rule to Ruleset. Adds \link StandardsRulesetOption StandardsRulesetOptions 
   *  \endlink as needed for any \link StandardsRulesetOption StandardsFilterRulesetOptions 
   *  \endlink. For performance reasons, does not check if containsEquivalentData(rule). 
   *  Please perform this check yourself if duplication is a concern. */
  void add(const StandardsRule& rule);

  /** Remove option from Ruleset. Returns false if (!contains(option)). Removes  
   *  \link StandardsRulesetOption StandardsFilterRulesetOptions \endlink with 
   *  option.equivalentOptionType(optionFilter.option()) from all rules as needed to 
   *  keep the Ruleset consistent. */
  bool remove(const StandardsRulesetOption& option);

  /** Remove rule from Ruleset. Returns false if (!contains(rule)). */
  bool remove(const StandardsRule& rule);

  //@}
  /** @name Queries */
  //@{

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
  /** @name Serialization */
  //@{

  /** Print the rulest to std::string. Printed in CLIPS syntax. If mode == 
   *  StandardsRulesetPrintMode, prints implied, as well as explicitly written objects. Otherwise, 
   *  still prints options and rules as CLIPS constructs, but leaves out implied objects. */
  std::string print(const StandardsRulesetPrintMode& mode) const;

  /** Save print(StandardsRulesetPrintModel::File) to file (preferred extenstion .osr). Returns 
   *  false on fail. No load functionality is provided at this time. To save a StandardsRuleset,
   *  please see project::StandardsRulesetRecord. */
  bool save(const openstudio::path& p, bool overwrite=false);

  //@}

 protected:
  /// @cond
  friend class detail::StandardsRuleset_Impl;
  friend class RulesetObject;

  /** Protected constructor from impl. */
  StandardsRuleset(boost::shared_ptr<detail::StandardsRuleset_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("Ruleset.StandardsRuleset");
};

} // ruleset
} // openstudio

#endif // RULESET_STANDARDSRULESET_HPP
