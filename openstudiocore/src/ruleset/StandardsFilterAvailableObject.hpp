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

#ifndef RULESET_STANDARDSFILTERAVAILABLEOBJECT_HPP
#define RULESET_STANDARDSFILTERAVAILABLEOBJECT_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/StandardsFilterClause.hpp>

#include <standardsinterface/ObjectType.hxx>

namespace openstudio {
namespace ruleset {

class StandardsFilterAvailableModel;

namespace detail {
  class StandardsFilterAvailableObject_Impl;
} // detail

/** A StandardsFilterAvailableObject is a StandardsFilterClause that matches objects of a given
 *  objectType(). Optionally, the (type of) model the object must come from is also specified. */
class RULESET_API StandardsFilterAvailableObject : public StandardsFilterClause {
 public:
  /// @cond
  typedef detail::StandardsFilterAvailableObject_Impl ImplType;

  static std::string xmlElementName();
  /// @endcond

  /** @name Constructors and Destructors */
  //@{

  StandardsFilterAvailableObject(const StandardsFilterAvailableModel& model,
                                 const standardsinterface::ObjectType& objectType);

  /** Constructor provided for deserialization; not for general use. */
  StandardsFilterAvailableObject(const StandardsFilterAvailableModel& model,
                                 const standardsinterface::ObjectType& objectType,
                                 const UUID& uuid,
                                 const UUID& versionUUID);

  StandardsFilterAvailableObject(const standardsinterface::ObjectType& objectType);

  /** Constructor provided for deserialization; not for general use. */
  StandardsFilterAvailableObject(const standardsinterface::ObjectType& objectType,
                                 const UUID& uuid,
                                 const UUID& versionUUID);

  /** Constructor provided for deserialization; not for general use. */
  StandardsFilterAvailableObject(const QDomElement& element);

  //@}
  /** @name Getters */
  //@{

  /** String representing this filter's modelId. May be internal variable, or variable
   *  inherited from model(). */
  std::string modelId() const;

  boost::optional<StandardsFilterAvailableModel> model() const;

  standardsinterface::ObjectType objectType() const;

  //@}
 protected:
  /// @cond
  friend class detail::StandardsFilterAvailableObject_Impl;
  friend class RulesetObject;

  // Protected setter provided for deepClone.
  void mf_setModel(const boost::optional<StandardsFilterAvailableModel>& model);

  /** Protected constructor from impl. */
  StandardsFilterAvailableObject(boost::shared_ptr<detail::StandardsFilterAvailableObject_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("Ruleset.StandardsFilterAvailableObject");
};

/** \relates StandardsFilterAvailableObject */
typedef boost::optional<StandardsFilterAvailableObject> OptionalStandardsFilterAvailableObject;

/** \relates StandardsFilterAvailableObject */
typedef std::vector<StandardsFilterAvailableObject> StandardsFilterAvailableObjectVector;

} // ruleset
} // openstudio

#endif // RULESET_STANDARDSFILTERAVAILABLEOBJECT_HPP
