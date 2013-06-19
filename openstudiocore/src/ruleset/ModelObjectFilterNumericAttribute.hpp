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

#ifndef RULESET_MODELOBJECTFILTERNUMERICATTRIBUTE_HPP
#define RULESET_MODELOBJECTFILTERNUMERICATTRIBUTE_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/ModelObjectFilterAttribute.hpp>
#include <ruleset/RulesetEnums.hpp>

class QDomDocument;
class QDomElement;

namespace openstudio {

  class AttributeValueType;

namespace ruleset {

namespace detail {
  class ModelObjectFilterNumericAttribute_Impl;
} // detail

/** ModelObjectFilterNumericAttribute is a ModelObjectFilterAttribute used to select 
 *  model::ModelObjects based on the value of a numeric Attribute (double, int, or unsigned). For 
 *  a ModelObject to match this filter, it must have an attribute with the correct name, and that 
 *  attribute must be available and satisfy the predicate() applied with testValue(). */
class RULESET_API ModelObjectFilterNumericAttribute : public ModelObjectFilterAttribute {
 public:
  /// @cond
  typedef detail::ModelObjectFilterNumericAttribute_Impl ImplType;
  /// @endcond

  static std::string xmlElementName();

  /** @name Constructors and Destructors */
  //@{

  ModelObjectFilterNumericAttribute(const std::string& attributeName, 
                                    const RulesetNumericalPredicate& predicate, 
                                    unsigned testValue);

  /** Constructor provided for deserialization; not for general use. */
  ModelObjectFilterNumericAttribute(const std::string& attributeName, 
                                    const RulesetNumericalPredicate& predicate, 
                                    unsigned testValue,
                                    const UUID& uuid, 
                                    const UUID& versionUUID);

  ModelObjectFilterNumericAttribute(const std::string& attributeName, 
                                    const RulesetNumericalPredicate& predicate, 
                                    int testValue);

  /** Constructor provided for deserialization; not for general use. */
  ModelObjectFilterNumericAttribute(const std::string& attributeName, 
                                    const RulesetNumericalPredicate& predicate, 
                                    int testValue,
                                    const UUID& uuid, 
                                    const UUID& versionUUID);

  ModelObjectFilterNumericAttribute(const std::string& attributeName, 
                                    const RulesetNumericalPredicate& predicate, 
                                    double testValue);

  /** Constructor provided for deserialization; not for general use. */
  ModelObjectFilterNumericAttribute(const std::string& attributeName, 
                                    const RulesetNumericalPredicate& predicate, 
                                    double testValue,
                                    const UUID& uuid, 
                                    const UUID& versionUUID);

  /** Constructor provided for deserialization; not for general use. */
  explicit ModelObjectFilterNumericAttribute(const QDomElement& element);
  
  virtual ~ModelObjectFilterNumericAttribute() {}

  //@}
  /** @name Getters */
  //@{

  RulesetNumericalPredicate predicate() const;

  openstudio::AttributeValueType attributeValueType() const;
  
  QVariant testValue() const;

  //@}
 protected:
  /// @cond
  friend class detail::ModelObjectFilterNumericAttribute_Impl;
  friend class RulesetObject;

  /** Protected constructor from impl. */
  ModelObjectFilterNumericAttribute(const boost::shared_ptr<detail::ModelObjectFilterNumericAttribute_Impl>& impl);

  /// @endcond
 private:
  REGISTER_LOGGER("Ruleset.ModelObjectFilterNumericAttribute");
};

/** \relates ModelObjectFilterNumericAttribute */
typedef boost::optional<ModelObjectFilterNumericAttribute> OptionalModelObjectFilterNumericAttribute;

/** \relates ModelObjectFilterNumericAttribute */
typedef std::vector<ModelObjectFilterNumericAttribute> ModelObjectFilterNumericAttributeVector;

} // ruleset
} // openstudio

#endif // RULESET_MODELOBJECTFILTERNUMERICATTRIBUTE_HPP
