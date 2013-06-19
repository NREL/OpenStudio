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

#ifndef RULESET_MODELOBJECTACTIONSETATTRIBUTE_IMPL_HPP
#define RULESET_MODELOBJECTACTIONSETATTRIBUTE_IMPL_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/ModelObjectActionClause_Impl.hpp>

#include <utilities/data/Attribute.hpp>

#include <boost/optional.hpp>

class QDomDocument;
class QDomElement;

namespace openstudio {

  class AttributeValueType;

namespace ruleset {

  class ModelObjectActionSetAttribute;

namespace detail {


  class RULESET_API ModelObjectActionSetAttribute_Impl : public ModelObjectActionClause_Impl {

    Q_OBJECT;

   public:
    /** @name Constructors and Destructors */
    //@{

    ModelObjectActionSetAttribute_Impl(const std::string& attributeName, bool value);

    ModelObjectActionSetAttribute_Impl(const std::string& attributeName, bool value,
                                       const UUID& uuid, const UUID& versionUUID);

    ModelObjectActionSetAttribute_Impl(const std::string& attributeName, unsigned value);

    ModelObjectActionSetAttribute_Impl(const std::string& attributeName, unsigned value,
                                       const UUID& uuid, const UUID& versionUUID);

    ModelObjectActionSetAttribute_Impl(const std::string& attributeName, int value);

    ModelObjectActionSetAttribute_Impl(const std::string& attributeName, int value,
                                       const UUID& uuid, const UUID& versionUUID);

    ModelObjectActionSetAttribute_Impl(const std::string& attributeName, double value);

    ModelObjectActionSetAttribute_Impl(const std::string& attributeName, double value,
                                       const UUID& uuid, const UUID& versionUUID);

    ModelObjectActionSetAttribute_Impl(const std::string& attributeName, const std::string& value);

    ModelObjectActionSetAttribute_Impl(const std::string& attributeName, const std::string& value,
                                       const UUID& uuid, const UUID& versionUUID);

    ModelObjectActionSetAttribute_Impl(const std::string& attributeName, const char* value);

    ModelObjectActionSetAttribute_Impl(const std::string& attributeName, const char* value,
                                       const UUID& uuid, const UUID& versionUUID);

    ModelObjectActionSetAttribute_Impl(const QDomElement& element);
    
    virtual ~ModelObjectActionSetAttribute_Impl() {}

    //@}
    /** @name Getters */
    //@{

    std::string attributeName() const;

    openstudio::AttributeValueType attributeValueType() const;

    QVariant attributeValue() const;

    //@}
    /** @name Serialization */
    //@{

    virtual std::string xmlElementName() const;

    virtual void writeValues(QDomDocument& doc, QDomElement& element) const;

    //@}
    /** @name Queries */
    //@{

    /** Equality as determined by deep comparison of data members. */
    virtual bool equivalentData(const RulesetObject& other) const;

    //@}
    /** @name Actions */
    //@{

    virtual bool apply(openstudio::model::ModelObject& modelObject) const;

    //@}

   protected:

    friend class openstudio::ruleset::ModelObjectActionSetAttribute;

    ModelObjectActionSetAttribute_Impl(const std::string& attributeName, const openstudio::AttributeValueType& attributeValueType, const QVariant& value);

    ModelObjectActionSetAttribute_Impl(const std::string& attributeName, const openstudio::AttributeValueType& attributeValueType, const QVariant& value,
                                       const UUID& uuid, const UUID& versionUUID);

   private:
    REGISTER_LOGGER("Ruleset.ModelObjectActionSetAttribute");

    std::string m_attributeName;
    openstudio::AttributeValueType m_attributeValueType;
    QVariant m_value;
  };

} // detail

} // ruleset
} // openstudio

#endif // RULESET_MODELOBJECTACTIONSETATTRIBUTE_IMPL_HPP
