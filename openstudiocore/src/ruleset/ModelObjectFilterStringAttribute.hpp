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

#ifndef RULESET_MODELOBJECTFILTERSTRINGATTRIBUTE_HPP
#define RULESET_MODELOBJECTFILTERSTRINGATTRIBUTE_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/ModelObjectFilterAttribute.hpp>
#include <ruleset/RulesetEnums.hpp>

class QDomDocument;
class QDomElement;

namespace openstudio {
namespace ruleset {

namespace detail {
  class ModelObjectFilterStringAttribute_Impl;
} // detail

/** ModelObjectFilterStringAttribute is a ModelObjectFilterAttribute used to select 
 *  model::ModelObjects based on the value of a string Attribute. For a ModelObject to match this
 *  filter, it must have an attribute with the correct name, and that attribute must be available 
 *  and satisfy the predicate() applied with testValue(). */
class RULESET_API ModelObjectFilterStringAttribute : public ModelObjectFilterAttribute {
 public:

  /// @cond
  typedef detail::ModelObjectFilterStringAttribute_Impl ImplType;
  /// @endcond

  static std::string xmlElementName();

  /** @name Constructors and Destructors */
  //@{

  ModelObjectFilterStringAttribute(const std::string& attributeName, 
                                   const RulesetStringPredicate& predicate, 
                                   const std::string& testValue);

  /** Constructor provided for deserialization; not for general use. */
  ModelObjectFilterStringAttribute(const std::string& attributeName, 
                                   const RulesetStringPredicate& predicate, 
                                   const std::string& testValue,
                                   const UUID& uuid, 
                                   const UUID& versionUUID);

  /** Constructor provided for deserialization; not for general use. */
  explicit ModelObjectFilterStringAttribute(const QDomElement& element);
  
  virtual ~ModelObjectFilterStringAttribute() {}

  //@}
  /** @name Getters */
  //@{

   RulesetStringPredicate predicate() const;

   std::string testValue() const;

  //@}
 protected:
  /// @cond
  friend class detail::ModelObjectFilterStringAttribute_Impl;
  friend class RulesetObject;

  /** Protected constructor from impl. */
  ModelObjectFilterStringAttribute(const boost::shared_ptr<detail::ModelObjectFilterStringAttribute_Impl>& impl);

  /// @endcond
 private:
  REGISTER_LOGGER("Ruleset.ModelObjectFilterStringAttribute");
};

/** \relates ModelObjectFilterStringAttribute */
typedef boost::optional<ModelObjectFilterStringAttribute> OptionalModelObjectFilterStringAttribute;

/** \relates ModelObjectFilterStringAttribute */
typedef std::vector<ModelObjectFilterStringAttribute> ModelObjectFilterStringAttributeVector;

} // ruleset
} // openstudio

#endif // RULESET_MODELOBJECTFILTERSTRINGATTRIBUTE_HPP
