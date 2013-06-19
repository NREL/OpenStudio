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

#ifndef RULESET_STANDARDSACTIONGETATTRIBUTE_IMPL_HPP
#define RULESET_STANDARDSACTIONGETATTRIBUTE_IMPL_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/StandardsActionClause_Impl.hpp>

#include <ruleset/StandardsFilterAvailableObject.hpp>

#include <standardsinterface/DataDictionary.hpp>

#include <boost/optional.hpp>

class QDomDocument;
class QDomElement;

namespace openstudio {
namespace ruleset {

namespace detail {

  class StandardsActionGetAttribute;

  class RULESET_API StandardsActionGetAttribute_Impl : public StandardsActionClause_Impl {

    Q_OBJECT;

   public:
    /** @name Constructors and Destructors */
    //@{

    StandardsActionGetAttribute_Impl(const StandardsFilterAvailableObject& object,
                                     const standardsinterface::BooleanAttribute& attribute);

    StandardsActionGetAttribute_Impl(const StandardsFilterAvailableObject& object,
                                     const standardsinterface::BooleanAttribute& attribute,
                                     const UUID& uuid,
                                     const UUID& versionUUID);

    StandardsActionGetAttribute_Impl(const StandardsFilterAvailableObject& object,
                                     const standardsinterface::DoubleAttribute& attribute);

    StandardsActionGetAttribute_Impl(const StandardsFilterAvailableObject& object,
                                     const standardsinterface::DoubleAttribute& attribute,
                                     const UUID& uuid,
                                     const UUID& versionUUID);

    StandardsActionGetAttribute_Impl(const StandardsFilterAvailableObject& object,
                                     const standardsinterface::EnumAttribute& attribute);

    StandardsActionGetAttribute_Impl(const StandardsFilterAvailableObject& object,
                                     const standardsinterface::EnumAttribute& attribute,
                                     const UUID& uuid,
                                     const UUID& versionUUID);

    StandardsActionGetAttribute_Impl(const StandardsFilterAvailableObject& object,
                                     const standardsinterface::IntegerAttribute& attribute);

    StandardsActionGetAttribute_Impl(const StandardsFilterAvailableObject& object,
                                     const standardsinterface::IntegerAttribute& attribute,
                                     const UUID& uuid,
                                     const UUID& versionUUID);

    StandardsActionGetAttribute_Impl(const QDomElement& element);

    /** Creates a clone of this object with disconnected data. Related objects are retained as-is 
     *  (data shared with this object). */
    virtual RulesetObject clone() const;

    /** Creates a clone of this object with deeply disconnected data. Related objects are also 
     *  cloned. */
    virtual RulesetObject deepClone() const;

    //@}
    /** @name Getters */
    //@{

    /** \link StandardsFilterClause StandardsFilterClauses \endlink used to construct this StandardsActionClause, and/or
     *  \link StandardsFilterClause StandardsFilterClauses \endlink that define variables used by this StandardsActionClause. */
    virtual std::vector<StandardsFilterClause> referencedFilters() const;

    //@}
    /** @name Setters */
    //@{

    void setObject(const StandardsFilterAvailableObject& object);

    //@}
    /** @name Serialization */
    //@{

    virtual std::string print() const;

    virtual std::string xmlElementName() const;

    virtual void writeValues(QDomDocument& doc, QDomElement& element) const;

    //@}
    /** @name Queries */
    //@{

    /** Equality as determined by deep comparison of data members. */
    virtual bool equivalentData(const RulesetObject& other) const;

    //@}
   protected:
    StandardsFilterAvailableObject m_object;
    standardsinterface::PartsOfSpeech m_attributeType;
    boost::optional<standardsinterface::BooleanAttribute> m_booleanAttribute;
    boost::optional<standardsinterface::DoubleAttribute> m_doubleAttribute;
    boost::optional<standardsinterface::EnumAttribute> m_enumAttribute;
    boost::optional<standardsinterface::IntegerAttribute> m_integerAttribute;

   private:
    REGISTER_LOGGER("Ruleset.StandardsActionGetAttribute");
  };

} // detail

} // ruleset
} // openstudio

#endif // RULESET_STANDARDSACTIONGETATTRIBUTE_IMPL_HPP
