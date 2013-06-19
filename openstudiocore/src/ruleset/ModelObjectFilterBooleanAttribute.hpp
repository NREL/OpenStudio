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

#ifndef RULESET_MODELOBJECTFILTERBOOLEANATTRIBUTE_HPP
#define RULESET_MODELOBJECTFILTERBOOLEANATTRIBUTE_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/ModelObjectFilterAttribute.hpp>

class QDomDocument;
class QDomElement;

namespace openstudio {
namespace ruleset {

namespace detail {
  class ModelObjectFilterBooleanAttribute_Impl;
} // detail

/** ModelObjectFilterBooleanAttribute is a is a ModelObjectFilterAttribute used to select 
 *  model::ModelObjects based on the value of a boolean Attribute. For a ModelObject to match this
 *  filter, it must have an attribute with the correct name, and that attribute must be available 
 *  and be equal to testValue(). */
class RULESET_API ModelObjectFilterBooleanAttribute : public ModelObjectFilterAttribute {
 public:
  /// @cond
  typedef detail::ModelObjectFilterBooleanAttribute_Impl ImplType;
  /// @endcond

  static std::string xmlElementName();

  /** @name Constructors and Destructors */
  //@{

  ModelObjectFilterBooleanAttribute(const std::string& attributeName, 
                                    bool testValue);

  /** Constructor provided for deserialization; not for general use. */
  ModelObjectFilterBooleanAttribute(const std::string& attributeName, 
                                    bool testValue,
                                    const UUID& uuid, 
                                    const UUID& versionUUID);

  /** Constructor provided for deserialization; not for general use. */
  explicit ModelObjectFilterBooleanAttribute(const QDomElement& element);
  
  virtual ~ModelObjectFilterBooleanAttribute() {}

  //@}
  /** @name Getters */
  //@{

  bool testValue() const;

  //@}
 protected:
  /// @cond
  friend class detail::ModelObjectFilterBooleanAttribute_Impl;
  friend class RulesetObject;

  /** Protected constructor from impl. */
  ModelObjectFilterBooleanAttribute(const boost::shared_ptr<detail::ModelObjectFilterBooleanAttribute_Impl>& impl);

  /// @endcond
 private:
  REGISTER_LOGGER("Ruleset.ModelObjectFilterBooleanAttribute");
};

/** \relates ModelObjectFilterBooleanAttribute */
typedef boost::optional<ModelObjectFilterBooleanAttribute> OptionalModelObjectFilterBooleanAttribute;

/** \relates ModelObjectFilterBooleanAttribute */
typedef std::vector<ModelObjectFilterBooleanAttribute> ModelObjectFilterBooleanAttributeVector;

} // ruleset
} // openstudio

#endif // RULESET_MODELOBJECTFILTERBOOLEANATTRIBUTE_HPP
