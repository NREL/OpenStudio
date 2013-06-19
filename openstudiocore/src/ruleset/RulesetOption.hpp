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

#ifndef RULESET_RULESETOPTION_HPP
#define RULESET_RULESETOPTION_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/RulesetObject.hpp>

namespace openstudio {
namespace ruleset {

namespace detail {
  class RulesetOption_Impl;
} // detail

/** A RulesetOption RulesetObject specifies or modifies how a Ruleset will be applied to a 
 *  model::Model. Most often, it determines which subset of the \link Rule Rules\endlink in a 
 *  Ruleset are implicitly active (separate from the active() boolean). At this time, only 
 *  \link StandardsRuleset StandardsRulesets \endlink utilize \link RulesetOption RulesetOptions
 *  \endlink */
class RULESET_API RulesetOption : public RulesetObject {
 public:

  /// @cond
  typedef detail::RulesetOption_Impl ImplType;
  /// @endcond

  /** @name Constructors and Destructors */
  //@{

  /** Constructor provided for deserialization; not for general use. */
  RulesetOption(const QDomElement& element);

  RulesetOption clone() const;

  //@}
 protected:
  /// @cond
  friend class detail::RulesetOption_Impl;
  friend class RulesetObject;

  /** Protected constructor from impl. */
  RulesetOption(const boost::shared_ptr<detail::RulesetOption_Impl>& impl);

  /// @endcond
 private:

  REGISTER_LOGGER("Ruleset.RulesetOption");
};

/** \relates RulesetOption */
typedef boost::optional<RulesetOption> OptionalRulesetOption;

/** \relates RulesetOption */
typedef std::vector<RulesetOption> RulesetOptionVector;

} // ruleset
} // openstudio

#endif // RULESET_RULESETOPTION_HPP
