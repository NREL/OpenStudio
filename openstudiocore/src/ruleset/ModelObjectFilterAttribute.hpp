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

#ifndef RULESET_MODELOBJECTFILTERATTRIBUTE_HPP
#define RULESET_MODELOBJECTFILTERATTRIBUTE_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/ModelObjectFilterClause.hpp>

namespace openstudio {
namespace ruleset {

namespace detail {
  class ModelObjectFilterAttribute_Impl;
} // detail

/** ModelObjectFilterAttribute is a ModelObjectFilterClause that matches a model::ModelObject based
 *  on some conditions placed on the Attribute returned by 
 *  model::ModelObject::getAttribute(attributeName()). This class is essentially abstract, as it
 *  has no public constructors. */
class RULESET_API ModelObjectFilterAttribute : public ModelObjectFilterClause {
 public:
  /// @cond
  typedef detail::ModelObjectFilterAttribute_Impl ImplType;
  /// @endcond
  
  /** @name Constructors and Destructors */
  //@{
  
  virtual ~ModelObjectFilterAttribute() {}
  
  //@}
  /** @name Getters */
  //@{

  std::string attributeName() const;

  //@}
 protected:
  /// @cond
  friend class detail::ModelObjectFilterAttribute_Impl;
  friend class RulesetObject;

  /** Protected constructor from impl. */
  ModelObjectFilterAttribute(boost::shared_ptr<detail::ModelObjectFilterAttribute_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("Ruleset.ModelObjectFilterAttribute");
};

/** \relates ModelObjectFilterAttribute */
typedef boost::optional<ModelObjectFilterAttribute> OptionalModelObjectFilterAttribute;

/** \relates ModelObjectFilterAttribute */
typedef std::vector<ModelObjectFilterAttribute> ModelObjectFilterAttributeVector;

} // ruleset
} // openstudio

#endif // RULESET_MODELOBJECTFILTERATTRIBUTE_HPP
