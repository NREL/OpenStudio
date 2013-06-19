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

#ifndef RULESET_MODELOBJECTACTIONSETATTRIBUTE_HPP
#define RULESET_MODELOBJECTACTIONSETATTRIBUTE_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/ModelObjectActionClause.hpp>

#include <QVariant>

class QDomDocument;
class QDomElement;

namespace openstudio {

class AttributeValueType;

namespace ruleset {

namespace detail {
  class ModelObjectActionSetAttribute_Impl;
}

/** ModelObjectActionSetAttribute is a ModelObjectActionClause that sets attributeName() to 
 *  attributeValue() for the given model::ModelObject. */
class RULESET_API ModelObjectActionSetAttribute : public ModelObjectActionClause {
 public:
  /// @cond
  typedef detail::ModelObjectActionSetAttribute_Impl ImplType;
  /// @endcond

  static std::string xmlElementName();

  /** @name Constructors and Destructors */
  //@{

  ModelObjectActionSetAttribute(const std::string& attributeName, 
                                bool value);

  /** Constructor provided for deserialization; not for general use. */
  ModelObjectActionSetAttribute(const std::string& attributeName, 
                                bool value,
                                const UUID& uuid, 
                                const UUID& versionUUID);

  ModelObjectActionSetAttribute(const std::string& attributeName, 
                                unsigned value);

  /** Constructor provided for deserialization; not for general use. */
  ModelObjectActionSetAttribute(const std::string& attributeName, 
                                unsigned value,
                                const UUID& uuid, 
                                const UUID& versionUUID);

  ModelObjectActionSetAttribute(const std::string& attributeName, 
                                int value);

  /** Constructor provided for deserialization; not for general use. */
  ModelObjectActionSetAttribute(const std::string& attributeName, 
                                int value,
                                const UUID& uuid, 
                                const UUID& versionUUID);

  ModelObjectActionSetAttribute(const std::string& attributeName, 
                                double value);

  /** Constructor provided for deserialization; not for general use. */
  ModelObjectActionSetAttribute(const std::string& attributeName, 
                                double value,
                                const UUID& uuid, 
                                const UUID& versionUUID);

  ModelObjectActionSetAttribute(const std::string& attributeName, 
                                const std::string& value);

  /** Constructor provided for deserialization; not for general use. */
  ModelObjectActionSetAttribute(const std::string& attributeName, 
                                const std::string& value,
                                const UUID& uuid, 
                                const UUID& versionUUID);

  ModelObjectActionSetAttribute(const std::string& attributeName, 
                                const char* value);

  /** Constructor provided for deserialization; not for general use. */
  ModelObjectActionSetAttribute(const std::string& attributeName, 
                                const char* value,
                                const UUID& uuid, 
                                const UUID& versionUUID);

  ModelObjectActionSetAttribute(const std::string& attributeName, 
                                const openstudio::AttributeValueType& attributeValueType, 
                                const QVariant& value);

  /** Constructor provided for deserialization; not for general use. */
  ModelObjectActionSetAttribute(const std::string& attributeName, 
                                const openstudio::AttributeValueType& attributeValueType, 
                                const QVariant& value,
                                const UUID& uuid, 
                                const UUID& versionUUID);

  /** Constructor provided for deserialization; not for general use. */
  explicit ModelObjectActionSetAttribute(const QDomElement& element);
  
  virtual ~ModelObjectActionSetAttribute() {}

  //@}
  /** @name Getters */
  //@{

  std::string attributeName() const;

  openstudio::AttributeValueType attributeValueType() const;

  QVariant attributeValue() const;

  //@}
 protected:
  /// @cond
  friend class detail::ModelObjectActionSetAttribute_Impl;
  friend class RulesetObject;

  /** Protected constructor from impl. */
  ModelObjectActionSetAttribute(const boost::shared_ptr<detail::ModelObjectActionSetAttribute_Impl>& impl);

  /// @endcond
 private:
  REGISTER_LOGGER("Ruleset.ModelObjectActionSetAttribute");
};

/** \relates ModelObjectActionSetAttribute */
typedef boost::optional<ModelObjectActionSetAttribute> OptionalModelObjectActionSetAttribute;

/** \relates ModelObjectActionSetAttribute */
typedef std::vector<ModelObjectActionSetAttribute> ModelObjectActionSetAttributeVector;

} // ruleset
} // openstudio

#endif // RULESET_MODELOBJECTACTIONSETATTRIBUTE_HPP
