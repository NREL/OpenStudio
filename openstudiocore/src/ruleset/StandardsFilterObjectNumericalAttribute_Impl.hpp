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

#ifndef RULESET_STANDARDSFILTEROBJECTNUMERICALATTRIBUTE_IMPL_HPP
#define RULESET_STANDARDSFILTEROBJECTNUMERICALATTRIBUTE_IMPL_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/StandardsFilterObjectAttribute_Impl.hpp>

#include <ruleset/StandardsRulesetEnums.hpp>

namespace openstudio {
namespace ruleset {

class StandardsFilterObjectNumericalAttribute;

namespace detail {

  class RULESET_API StandardsFilterObjectNumericalAttribute_Impl : public StandardsFilterObjectAttribute_Impl {
    Q_OBJECT;
   public:
    /** @name Constructors and Destructors */
    //@{

    StandardsFilterObjectNumericalAttribute_Impl(const StandardsFilterAvailableObject& object,
                                                 const RulesetNumericalPredicate& predicate,
                                                 const std::string& unitString);

    StandardsFilterObjectNumericalAttribute_Impl(const StandardsFilterAvailableObject& object,
                                                 const RulesetNumericalPredicate& predicate,
                                                 const std::string& unitString,
                                                 const UUID& uuid,
                                                 const UUID& versionUUID);

    StandardsFilterObjectNumericalAttribute_Impl(const QDomElement& element);

    virtual ~StandardsFilterObjectNumericalAttribute_Impl() {}

    //@}
    /** @name Getters */
    //@{

    RulesetNumericalPredicate predicate() const;

    std::string units() const;

    //@}
    /** @name Setters */
    //@{

    /** Flip this filter to the opposite form. (True -> false and vice versa, equal -> not equal, 
     *  less than -> greater than or equal to, etc.) */
    virtual void negate();

    void setPredicate(const RulesetNumericalPredicate& predicate);

    void setUnits(const std::string& unitString);

    //@}
    /** @name Serialization */
    //@{

    virtual void writeValues(QDomDocument& doc, QDomElement& element) const;

    //@}
    /** @name Queries */
    //@{

    /** Equality as determined by deep comparison of data members. */
    virtual bool equivalentData(const RulesetObject& other) const;

    /** Same as equivalentData, except that this method ignores differences in attribute value(s). */
    virtual bool equivalentAttributeType(const StandardsFilterObjectAttribute& other) const;

    //@}
   protected:
    RulesetNumericalPredicate m_predicate;
    std::string m_units;
   private:
    REGISTER_LOGGER("ruleset.StandardsFilterObjectNumericalAttribute");
  };

} // detail

} // ruleset
} // openstudio

#endif // RULESET_STANDARDSFILTEROBJECTNUMERICALATTRIBUTE_IMPL_HPP
