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

#ifndef RULESET_STANDARDSRULESETOPTION_HPP
#define RULESET_STANDARDSRULESETOPTION_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/RulesetOption.hpp>

namespace openstudio {
namespace ruleset {

namespace detail {
  class StandardsRulesetOption_Impl;
} // detail

/** A StandardsRulesetOption is a RulesetOption applicable to \link StandardsRuleset 
 *  StandardsRulesets \endlink. The class serves the dual purpose of defining an option for 
 *  a StandardsRuleset, and also holding its current value, which is used in \link 
 *  StandardsFilterRulesetOption StandardsFilterRulesetOptions \endlink and by users of 
 *  rulesengine::RulesEngine. In the latter case, the user selected options determine what
 *  sort of standards analysis is run on their particular model. */
class RULESET_API StandardsRulesetOption : public RulesetOption {
 public:
  /// @cond
  typedef detail::StandardsRulesetOption_Impl ImplType;
  /// @endcond

  /** @name Constructors and Destructors */
  //@{

  /** Constructor provided for deserialization; not for general use. */
  StandardsRulesetOption(const QDomElement& element);

  //@}
  /** @name Getters */
  //@{

  std::string name() const;

  std::string factName() const;

  //@}
  /** @name Serialization */
  //@{

  /** Print definition as CLIPS deftemplate (fact type) to stream. */
  std::string printDefinition() const;

  /** Print value as CLIPS fact to stream. */
  std::string printValue() const;

  //@}
  /** @name Queries */
  //@{

  /** Same as equivalentData, except that this method ignores differences in values and default 
   *  values. */
  bool equivalentOptionType(const StandardsRulesetOption& other) const;

  //@}
 protected:
  /// @cond
  friend class detail::StandardsRulesetOption_Impl;
  friend class RulesetObject;

  /** Protected constructor from impl. */
  StandardsRulesetOption(const boost::shared_ptr<detail::StandardsRulesetOption_Impl>& impl);

  /// @endcond
 private:

  REGISTER_LOGGER("Ruleset.StandardsRulesetOption");
};

/** \relates StandardsRulesetOption */
RULESET_API std::ostream& operator<<(std::ostream& os, const StandardsRulesetOption& option);

/** \relates StandardsRulesetOption */
typedef boost::optional<StandardsRulesetOption> OptionalStandardsRulesetOption;

/** \relates StandardsRulesetOption */
typedef std::vector<StandardsRulesetOption> StandardsRulesetOptionVector;

} // ruleset
} // openstudio

#endif // RULESET_STANDARDSRULESETOPTION_HPP
