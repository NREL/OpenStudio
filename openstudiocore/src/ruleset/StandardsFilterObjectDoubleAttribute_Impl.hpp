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

#ifndef RULESET_STANDARDSFILTEROBJECTDOUBLEATTRIBUTE_IMPL_HPP
#define RULESET_STANDARDSFILTEROBJECTDOUBLEATTRIBUTE_IMPL_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/StandardsFilterObjectNumericalAttribute_Impl.hpp>

#include <standardsinterface/DataDictionary.hpp>

class QDomDocument;
class QDomElement;

namespace openstudio {
namespace ruleset {

class StandardsFilterAvailableObject;
class StandardsFilterObjectDoubleAttribute;

namespace detail {

  class RULESET_API StandardsFilterObjectDoubleAttribute_Impl : public StandardsFilterObjectNumericalAttribute_Impl {
    Q_OBJECT;
   public:
    /** @name Constructors and Destructors */
    //@{

    StandardsFilterObjectDoubleAttribute_Impl(const StandardsFilterAvailableObject& object,
                                              const standardsinterface::DoubleAttribute& attribute,
                                              const RulesetNumericalPredicate& predicate,
                                              double value,
                                              const std::string& unitString);

    StandardsFilterObjectDoubleAttribute_Impl(const StandardsFilterAvailableObject& object,
                                              const standardsinterface::DoubleAttribute& attribute,
                                              const RulesetNumericalPredicate& predicate,
                                              double value,
                                              const std::string& unitString,
                                              const UUID& uuid,
                                              const UUID& versionUUID);

    StandardsFilterObjectDoubleAttribute_Impl(const QDomElement& element);

    virtual ~StandardsFilterObjectDoubleAttribute_Impl() {}

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

    standardsinterface::DoubleAttribute attribute() const;

    double value() const;

    //@}
    /** @name Setters */
    //@{

    void setAttribute(const standardsinterface::DoubleAttribute& attribute);

    /** Set the object attribute's value. Provided for import of rules from clear text formats. 
     *  Virtual implementation. */
    virtual bool setValue(const std::string& value);

    bool setValue(double value);

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
     standardsinterface::DoubleAttribute m_attribute;
     double m_value;
   private:
    REGISTER_LOGGER("ruleset.StandardsFilterObjectDoubleAttribute");
  };

} // detail

} // ruleset
} // openstudio

#endif // RULESET_STANDARDSFILTEROBJECTDOUBLEATTRIBUTE_IMPL_HPP
