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

#ifndef RULESET_STANDARDSFILTEROBJECTENUMATTRIBUTE_IMPL_HPP
#define RULESET_STANDARDSFILTEROBJECTENUMATTRIBUTE_IMPL_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/StandardsFilterObjectAttribute_Impl.hpp>
#include <ruleset/RulesetEnums.hpp>

#include <standardsinterface/DataDictionary.hpp>

class QDomDocument;
class QDomElement;

namespace openstudio {
namespace ruleset {

class StandardsFilterAvailableObject;
class StandardsFilterObjectEnumAttribute;

namespace detail {

  class RULESET_API StandardsFilterObjectEnumAttribute_Impl : public StandardsFilterObjectAttribute_Impl {

    Q_OBJECT;

   public:
    /** @name Constructors and Destructors */
    //@{

    StandardsFilterObjectEnumAttribute_Impl(const StandardsFilterAvailableObject& object,
                                            const standardsinterface::EnumAttribute& attribute,
                                            const RulesetStringPredicate& predicate,
                                            const std::string value);

    StandardsFilterObjectEnumAttribute_Impl(const StandardsFilterAvailableObject& object,
                                            const standardsinterface::EnumAttribute& attribute,
                                            const RulesetStringPredicate& predicate,
                                            const std::string value,
                                            const UUID& uuid,
                                            const UUID& versionUUID);

    StandardsFilterObjectEnumAttribute_Impl(const QDomElement& element);

    /** Creates a clone of this object with disconnected data. Related objects are retained as-is
     *  (data shared with this object). */
    virtual RulesetObject clone() const;

    /** Creates a clone of this object with deeply disconnected data. Related objects are also
     *  cloned. */
    virtual RulesetObject deepClone() const;

    //@}
    /** @name Getters */
    //@{

    virtual std::string factName() const;

    virtual std::string shortDescription() const;

    standardsinterface::EnumAttribute attribute() const;

    RulesetStringPredicate predicate() const;

    std::string value() const;

    //@}
    /** @name Setters */
    //@{

    /** Flip this filter to the opposite form. (True -> false and vice versa, equal -> not equal, 
     *  less than -> greater than or equal to, etc.) */
    virtual void negate();

    void setPredicate(const RulesetStringPredicate& predicate);

    /** Set the object attribute's value. Provided for import of rules from clear text formats.
     *  Virtual implementation. */
    virtual bool setValue(const std::string& value);

    //@}
    /** @name Serialization */
    //@{

    virtual std::string print() const;

    virtual std::string printForMessage() const;

    virtual std::string xmlElementName() const;

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
    standardsinterface::EnumAttribute m_attribute;
    RulesetStringPredicate m_predicate;
    std::string m_value;
   private:
    REGISTER_LOGGER("ruleset.StandardsFilterObjectEnumAttribute");
  };

} // detail

} // ruleset
} // openstudio

#endif // RULESET_STANDARDSFILTEROBJECTENUMATTRIBUTE_IMPL_HPP
