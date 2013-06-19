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

#ifndef RULESET_STANDARDSFILTERAPPLYRULESETRESULT_IMPL_HPP
#define RULESET_STANDARDSFILTERAPPLYRULESETRESULT_IMPL_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/StandardsFilterClause_Impl.hpp>

#include <ruleset/StandardsFilterAvailableModel.hpp>

class QDomDocument;
class QDomElement;

namespace openstudio {
namespace ruleset {

namespace detail {

  class StandardsFilterApplyRulesetResult;

  class RULESET_API StandardsFilterApplyRulesetResult_Impl : public StandardsFilterClause_Impl {

    Q_OBJECT;

   public:
    /** @name Constructors and Destructors */
    //@{

    StandardsFilterApplyRulesetResult_Impl(const StandardsFilterAvailableModel& model);

    StandardsFilterApplyRulesetResult_Impl(const StandardsFilterAvailableModel& model,
                                           const UUID& uuid,
                                           const UUID& versionUUID);

    StandardsFilterApplyRulesetResult_Impl(const QDomElement& element);

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

    StandardsFilterAvailableModel model() const;

    //@}
    /** @name Setters */
    //@{

    void setModel(const StandardsFilterAvailableModel& model);

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
    StandardsFilterAvailableModel m_model;
   private:
    REGISTER_LOGGER("ruleset.StandardsFilterApplyRulesetResult");
  };

} // detail

} // ruleset
} // openstudio

#endif // RULESET_STANDARDSFILTERAPPLYRULESETRESULT_IMPL_HPP
