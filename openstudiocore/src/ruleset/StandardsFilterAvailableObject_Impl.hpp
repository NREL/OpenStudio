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

#ifndef RULESET_STANDARDSFILTERAVAILABLEOBJECT_IMPL_HPP
#define RULESET_STANDARDSFILTERAVAILABLEOBJECT_IMPL_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/StandardsFilterClause_Impl.hpp>

#include <ruleset/StandardsFilterAvailableModel.hpp>

#include <standardsinterface/ObjectType.hxx>

class QDomDocument;
class QDomElement;

namespace openstudio {
namespace ruleset {

class StandardsFilterAvailableObject;

namespace detail {

  class RULESET_API StandardsFilterAvailableObject_Impl : public StandardsFilterClause_Impl {

    Q_OBJECT;

   public:
    /** @name Constructors and Destructors */
    //@{

    StandardsFilterAvailableObject_Impl(const StandardsFilterAvailableModel& model,
                                        const standardsinterface::ObjectType& objectType);

    StandardsFilterAvailableObject_Impl(const StandardsFilterAvailableModel& model,
                                        const standardsinterface::ObjectType& objectType,
                                        const UUID& uuid,
                                        const UUID& versionUUID);

    StandardsFilterAvailableObject_Impl(const standardsinterface::ObjectType& objectType);

    StandardsFilterAvailableObject_Impl(const standardsinterface::ObjectType& objectType,
                                        const UUID& uuid,
                                        const UUID& versionUUID);

    StandardsFilterAvailableObject_Impl(const QDomElement& element);

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

    /** \link StandardsFilterClause StandardsFilterClauses \endlink used to construct this ActionClause, and/or
     *  \link StandardsFilterClause StandardsFilterClauses \endlink that define variables used by this ActionClause. */
    virtual std::vector<StandardsFilterClause> referencedFilters() const;

    /** String representing this filter's modelId. May be internal variable, or variable
     *  inherited from model(). */
    std::string modelId() const;

    boost::optional<StandardsFilterAvailableModel> model() const;

    standardsinterface::ObjectType objectType() const;

    //@}
    /** @name Setters */
    //@{

    /** Any referenced filters whose UUIDs match preferredInstances[i] are to be removed and 
     *  replaced with preferredInstances[i]. This method assists in reconstruction of \link 
     *  StandardsRule StandardsRules \endlink stored in a project::ProjectDatabase. */
    virtual void replaceReferencedFilters(std::vector<StandardsFilterClause>& preferredInstances);

    void setModel(const boost::optional<StandardsFilterAvailableModel>& model);

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
    boost::optional<StandardsFilterAvailableModel> m_model;
    standardsinterface::ObjectType m_objectType;
   private:
    REGISTER_LOGGER("ruleset.StandardsFilterAvailableObject");
  };

} // detail

} // ruleset
} // openstudio

#endif // RULESET_STANDARDSFILTERAVAILABLEOBJECT_IMPL_HPP
