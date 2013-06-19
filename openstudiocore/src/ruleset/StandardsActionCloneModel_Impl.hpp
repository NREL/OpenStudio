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

#ifndef RULESET_STANDARDSACTIONCLONEMODEL_IMPL_HPP
#define RULESET_STANDARDSACTIONCLONEMODEL_IMPL_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/StandardsActionClause_Impl.hpp>

#include <ruleset/StandardsFilterAvailableModel.hpp>
#include <ruleset/StandardsRulesetEnums.hpp>

class QDomDocument;
class QDomElement;

namespace openstudio {
namespace ruleset {

class StandardsActionCloneModel;

namespace detail {

  class RULESET_API StandardsActionCloneModel_Impl : public StandardsActionClause_Impl {

    Q_OBJECT;

   public:
    /** @name Constructors and Destructors */
    //@{

    /** Will create a new model labeled as type modelTypeOfClone. To work properly, a rule that
     *  uses this clause must include a single StandardsFilterAvailableModel clause. */
    StandardsActionCloneModel_Impl(const StandardsFilterAvailableModel& modelToClone,
                                   const StandardsRulesetModelType& modelTypeOfClone);

    StandardsActionCloneModel_Impl(const StandardsFilterAvailableModel& modelToClone,
                                   const StandardsRulesetModelType& modelTypeOfClone,
                                   const UUID& uuid,
                                   const UUID& versionUUID);

    StandardsActionCloneModel_Impl(const QDomElement& element);

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

    StandardsFilterAvailableModel modelToClone() const;

    StandardsRulesetModelType modelTypeOfClone() const;

    //@}
    /** @name Setters */
    //@{

    /** Any referenced filters whose UUIDs match preferredInstances[i] are to be removed and 
     *  replaced with preferredInstances[i]. This method assists in reconstruction of \link 
     *  StandardsRule StandardsRules \endlink stored in a project::ProjectDatabase. */
    virtual void replaceReferencedFilters(const std::vector<StandardsFilterClause>& preferredInstances);


    void setModelToClone(const StandardsFilterAvailableModel& filter);

    void setModelTypeOfClone(const StandardsRulesetModelType& value);

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
    StandardsFilterAvailableModel m_modelToClone;
    StandardsRulesetModelType m_modelTypeOfClone;
   private:
    REGISTER_LOGGER("Ruleset.StandardsActionCloneModel");
  };

} // detail

} // ruleset
} // openstudio

#endif // RULESET_STANDARDSACTIONCLONEMODEL_IMPL_HPP
