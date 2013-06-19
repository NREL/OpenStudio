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

#ifndef RULESET_STANDARDSENUMRULESETOPTION_IMPL_HPP
#define RULESET_STANDARDSENUMRULESETOPTION_IMPL_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/StandardsRulesetOption_Impl.hpp>

#include <standardsinterface/EnumAttribute.hxx>

namespace openstudio {
namespace ruleset {

class StandardsEnumRulesetOption;

namespace detail {

  class RULESET_API StandardsEnumRulesetOption_Impl : public StandardsRulesetOption_Impl {

    Q_OBJECT;

   public:
    /** @name Constructors and Destructors */
    //@{

    StandardsEnumRulesetOption_Impl(const standardsinterface::EnumAttribute& enumOption,
                                    const std::string& defaultValue);

    StandardsEnumRulesetOption_Impl(const standardsinterface::EnumAttribute& enumOption,
                                    const std::string& defaultValue,
                                    const std::string& value,
                                    const UUID& uuid,
                                    const UUID& versionUUID);

    virtual RulesetOption clone() const;

    virtual ~StandardsEnumRulesetOption_Impl() {}

    //@}
    /** @name Getters */
    //@{

    virtual std::string name() const;

    virtual std::string factName() const;

    openstudio::standardsinterface::EnumAttribute enumOption() const;

    std::string value() const;

    std::string defaultValue() const;

    std::vector<std::string> validValues() const;

    //@}
    /** @name Setters */
    //@{

    bool setValue(const std::string& newValue);

    //@}
    /** @name Serialization */
    //@{

    virtual std::string xmlElementName() const;

    /** Print definition as CLIPS deftemplate (fact type). */
    virtual std::string printDefinition() const;

    /** Print value as CLIPS fact. */
    virtual std::string printValue() const;

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
    standardsinterface::EnumAttribute m_enumOption;
    std::string m_defaultValue;
    std::string m_value;

    REGISTER_LOGGER("Ruleset.StandardsRulesetOption");
  };

} // detail

} // ruleset
} // openstudio

#endif // RULESET_STANDARDSENUMRULESETOPTION_IMPL_HPP
