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

#ifndef RULESET_STANDARDSFILTERRULESETOPTION_HPP
#define RULESET_STANDARDSFILTERRULESETOPTION_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/StandardsFilterClause.hpp>

namespace openstudio {
namespace ruleset {

class StandardsRulesetOption;

namespace detail {
  class StandardsFilterRulesetOption_Impl;
} // detail

/** A StandardsFilterRulesetOption is a StandardsFilterClause that makes a rule applicable based
 *  on the value of a StandardsRulesetOption. For instance, it may be used to specify that a rule
 *  is only applicable if the user has selected prescriptive (as opposed to performance) 
 *  compliance. */
class RULESET_API StandardsFilterRulesetOption : public StandardsFilterClause {
 public:
  /// @cond
  typedef detail::StandardsFilterRulesetOption_Impl ImplType;

  static std::string xmlElementName();
  /// @endcond

  /** @name Constructors and Destructors */
  //@{

  /** Matches option.printValue(). Options are specified by the user when kicking off a run of 
   *  rulesengine::RulesEngine. */
  StandardsFilterRulesetOption(const StandardsRulesetOption& option);

  /** Constructor provided for deserialization; not for general use. */
  StandardsFilterRulesetOption(const StandardsRulesetOption& option, 
                               const UUID& uuid, 
                               const UUID& versionUUID);

  //@}
  /** @name Getters */
  //@{

  StandardsRulesetOption option() const;

  //@}
  /** @name Setters */
  //@{

  void setOption(const StandardsRulesetOption& option);

  //@}
 protected:
  /// @cond
  friend class detail::StandardsFilterRulesetOption_Impl;
  friend class RulesetObject;

  /** Protected constructor from impl. */
  StandardsFilterRulesetOption(boost::shared_ptr<detail::StandardsFilterRulesetOption_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("Ruleset.StandardsFilterRulesetOption");
};

/** \relates StandardsFilterRulesetOption */
typedef boost::optional<StandardsFilterRulesetOption> OptionalStandardsFilterRulesetOption;

/** \relates StandardsFilterRulesetOption */
typedef std::vector<StandardsFilterRulesetOption> StandardsFilterRulesetOptionVector;

} // ruleset
} // openstudio

#endif // RULESET_STANDARDSFILTERRULESETOPTION_HPP
