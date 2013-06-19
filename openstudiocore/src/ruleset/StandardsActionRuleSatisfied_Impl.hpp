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

#ifndef RULESET_STANDARDSACTIONRULESATISFIED_IMPL_HPP
#define RULESET_STANDARDSACTIONRULESATISFIED_IMPL_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/StandardsActionRuleResult_Impl.hpp>

class QDomDocument;
class QDomElement;

namespace openstudio {
namespace ruleset {

class StandardsActionRuleSatisfied;

namespace detail {

  class RULESET_API StandardsActionRuleSatisfied_Impl : public StandardsActionRuleResult_Impl {

    Q_OBJECT;

   public:
    /** @name Constructors and Destructors */
    //@{

    StandardsActionRuleSatisfied_Impl(const StandardsFilterObjectAttribute& attributeTest);

    StandardsActionRuleSatisfied_Impl(const StandardsFilterObjectAttribute& attributeTest,
                                      const UUID& uuid,
                                      const UUID& versionUUID);

    StandardsActionRuleSatisfied_Impl(const QDomElement& element);

    /** Creates a clone of this object with disconnected data. Related objects are retained as-is 
     *  (data shared with this object). */
    virtual RulesetObject clone() const;

    /** Creates a clone of this object with deeply disconnected data. Related objects are also 
     *  cloned. */
    virtual RulesetObject deepClone() const;

    //@}
    /** @name Serialization */
    //@{

    virtual std::string print() const;

    virtual std::string print(const StandardsRule& rule) const;

    virtual std::string xmlElementName() const;

    virtual void writeValues(QDomDocument& doc, QDomElement& element) const;

    //@}
    /** @name Queries */
    //@{

    /** Equality as determined by deep comparison of data members. */
    virtual bool equivalentData(const RulesetObject& other) const;

    //@}
   private:
    REGISTER_LOGGER("Ruleset.StandardsActionRuleSatisfied");
  };

} // detail

} // ruleset
} // openstudio

#endif // RULESET_STANDARDSACTIONRULESATISFIED_IMPL_HPP
