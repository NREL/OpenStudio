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

#ifndef RULESET_MODELOBJECTFILTERATTRIBUTE_IMPL_HPP
#define RULESET_MODELOBJECTFILTERATTRIBUTE_IMPL_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/ModelObjectFilterClause_Impl.hpp>

#include <QDomDocument>
#include <QDomElement>

namespace openstudio {
namespace ruleset {

class ModelObjectFilterAttribute;

namespace detail {

  class RULESET_API ModelObjectFilterAttribute_Impl : public ModelObjectFilterClause_Impl {

    Q_OBJECT;

   public:
    /** @name Constructors and Destructors */
    //@{

     ModelObjectFilterAttribute_Impl(const std::string& attributeName);

     ModelObjectFilterAttribute_Impl(const std::string& attributeName, const UUID& uuid, const UUID& versionUUID);

     ModelObjectFilterAttribute_Impl(const QDomElement& element);
     
     virtual ~ModelObjectFilterAttribute_Impl() {}

    //@}
    /** @name Getters */
    //@{

     std::string attributeName() const;

    //@}
    /** @name Setters */
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

    //@}

   private:
    REGISTER_LOGGER("Ruleset.ModelObjectFilterAttribute");

    std::string m_attributeName;
  };

} // detail

} // ruleset
} // openstudio

#endif // RULESET_MODELOBJECTFILTERATTRIBUTE_IMPL_HPP
