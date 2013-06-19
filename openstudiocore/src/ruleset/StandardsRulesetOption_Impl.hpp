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

#ifndef RULESET_STANDARDSRULESETOPTION_IMPL_HPP
#define RULESET_STANDARDSRULESETOPTION_IMPL_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/RulesetOption_Impl.hpp>

namespace openstudio {
namespace ruleset {

class StandardsRulesetOption;

namespace detail {

  class RULESET_API StandardsRulesetOption_Impl : public RulesetOption_Impl {

    Q_OBJECT;

   public:
    /** @name Constructors and Destructors */
    //@{

    StandardsRulesetOption_Impl();

    StandardsRulesetOption_Impl(const UUID& uuid, const UUID& versionUUID);

    virtual ~StandardsRulesetOption_Impl() {}

    //@}
    /** @name Getters */
    //@{

    virtual std::string name() const=0;

    virtual std::string factName() const=0;

    //@}
    /** @name Serialization */
    //@{

    /** Print definition as CLIPS deftemplate (fact type). */
    virtual std::string printDefinition() const = 0;

    /** Print value as CLIPS fact. */
    virtual std::string printValue() const = 0;

    //@}
    /** @name Queries */
    //@{

    /** Equality as determined by deep comparison of data members. */
    virtual bool equivalentData(const RulesetObject& other) const;

    /** Same as equivalentData, except that this method ignores differences in values and default 
     *  values. */
    virtual bool equivalentOptionType(const StandardsRulesetOption& other) const;

    //@}
   private:
    REGISTER_LOGGER("Ruleset.StandardsRulesetOption");
  };

} // detail

} // ruleset
} // openstudio

#endif // RULESET_STANDARDSRULESETOPTION_IMPL_HPP
