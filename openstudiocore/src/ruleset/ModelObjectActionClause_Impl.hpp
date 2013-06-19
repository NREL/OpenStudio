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

#ifndef RULESET_MODELOBJECTACTIONCLAUSE_IMPL_HPP
#define RULESET_MODELOBJECTACTIONCLAUSE_IMPL_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/ActionClause_Impl.hpp>

class QDomDocument;
class QDomElement;

namespace openstudio {
namespace model {
  class ModelObject;
}

namespace ruleset {

class ModelObjectActionClause;

namespace detail {

  class RULESET_API ModelObjectActionClause_Impl : public ActionClause_Impl {

    Q_OBJECT;

   public:
    /** @name Constructors and Destructors */
    //@{

    ModelObjectActionClause_Impl();

    ModelObjectActionClause_Impl(const UUID& uuid, const UUID& versionUUID);

    ModelObjectActionClause_Impl(const QDomElement& element);
    
    virtual ~ModelObjectActionClause_Impl() {}

    //@}
    /** @name Getters */
    //@{

    //@}
    /** @name Serialization */
    //@{

    virtual void writeValues(QDomDocument& doc, QDomElement& element) const;

    //@}
    /** @name Queries */
    //@{

    /** Equality as determined by deep comparison of data members. */
    virtual bool equivalentData(const RulesetObject& other) const;

    //@}
    /** @name Actions */
    //@{

    virtual bool apply(openstudio::model::ModelObject& modelObject) const = 0;

    //@}
   private:
    REGISTER_LOGGER("Ruleset.ModelObjectActionClause");
  };

} // detail

} // ruleset
} // openstudio

#endif // RULESET_MODELOBJECTACTIONCLAUSE_IMPL_HPP
