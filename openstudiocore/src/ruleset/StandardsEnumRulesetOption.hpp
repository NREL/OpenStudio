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

#ifndef RULESET_STANDARDSENUMRULESETOPTION_HPP
#define RULESET_STANDARDSENUMRULESETOPTION_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/StandardsRulesetOption.hpp>

#include <standardsinterface/EnumAttribute.hxx>

namespace openstudio {
namespace ruleset {

namespace detail {
  class StandardsEnumRulesetOption_Impl;
} // detail

/** A StandardsEnumRulesetOption is a StandardsRulesetOption whose validValues() are specified by
 *  a standardsinterface::EnumAttribute. The option may have a defaultValue(), and a value(). The
 *  enumOption() and defaultValue() are used by StandardsRuleset to define the option; The 
 *  enumOption() and value() are used by \link StandardsFilterRulesetOption 
 *  StandardsFilterRulesetOptions \endlink, and also by users controlling the application of a 
 *  ruleset to a model using the rulesengine::RulesEngine. */
class RULESET_API StandardsEnumRulesetOption : public StandardsRulesetOption {
 public:
  /// @cond
  typedef detail::StandardsEnumRulesetOption_Impl ImplType;

  static std::string xmlElementName();
  /// @endcond

  /** @name Constructors and Destructors */
  //@{

  StandardsEnumRulesetOption(const standardsinterface::EnumAttribute& enumOption,
                             const std::string& defaultValue=std::string());

  /** Constructor provided for deserialization; not for general use. */
  StandardsEnumRulesetOption(const standardsinterface::EnumAttribute& enumOption,
                             const std::string& defaultValue,
                             const std::string& value,
                             const UUID& uuid,
                             const UUID& versionUUID);
  //@}
  /** @name Getters */
  //@{

  openstudio::standardsinterface::EnumAttribute enumOption() const;

  std::string value() const;

  std::string defaultValue() const;

  std::vector<std::string> validValues() const;

  //@}
  /** @name Setters */
  //@{

  bool setValue(const std::string& newValue);

  //@}
 protected:
  /// @cond
  friend class detail::StandardsEnumRulesetOption_Impl;
  friend class RulesetObject;

  /** Protected constructor from impl. */
  StandardsEnumRulesetOption(const boost::shared_ptr<detail::StandardsEnumRulesetOption_Impl>& impl);

  /// @endcond
 private:
  REGISTER_LOGGER("Ruleset.StandardsEnumRulesetOption");
};

/** \relates StandardsEnumRulesetOption */
typedef boost::optional<StandardsEnumRulesetOption> OptionalStandardsEnumRulesetOption;

/** \relates StandardsEnumRulesetOption */
typedef std::vector<StandardsEnumRulesetOption> StandardsEnumRulesetOptionVector;

} // ruleset
} // openstudio

#endif // RULESET_STANDARDSENUMRULESETOPTION_HPP
