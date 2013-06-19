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

#ifndef RULESET_STANDARDSACTIONGETOBJECTS_HPP
#define RULESET_STANDARDSACTIONGETOBJECTS_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/StandardsActionClause.hpp>

#include <standardsinterface/ObjectType.hxx>

namespace openstudio {
namespace ruleset {

class StandardsFilterAvailableModel;
class StandardsFilterAvailableObject;

namespace detail {
  class StandardsActionGetObjects_Impl;

  /** This is a back-end StandardsActionClause added as part of back-end Rules created
   *  by Ruleset in response to Rules with StandardsFilterAvailableObject. */
  class RULESET_API StandardsActionGetObjects : public StandardsActionClause {
   public:

     typedef detail::StandardsActionGetObjects_Impl ImplType;

     static std::string xmlElementName();

    /** @name Constructors and Destructors */
    //@{

    StandardsActionGetObjects(const StandardsFilterAvailableObject& object);

    /** Constructor provided for deserialization; not for general use. */
    StandardsActionGetObjects(const boost::optional<StandardsFilterAvailableModel>& model,
                              const standardsinterface::ObjectType& objectType,
                              const UUID& uuid,
                              const UUID& versionUUID);

    //@}
    /** @name Getters */
    //@{

    standardsinterface::ObjectType objectType() const;

    //@}
   protected:

    friend class detail::StandardsActionGetObjects_Impl;
    friend class RulesetObject;

    // Protected setter provided for deepClone.
    void mf_setModel(const boost::optional<StandardsFilterAvailableModel>& model);

    /** Protected constructor from impl. */
    StandardsActionGetObjects(boost::shared_ptr<detail::StandardsActionGetObjects_Impl> impl);

   private:
    REGISTER_LOGGER("Ruleset.StandardsActionGetObjects");
  };

  /** \relates StandardsActionGetObjects */
  typedef boost::optional<StandardsActionGetObjects> OptionalStandardsActionGetObjects;

  /** \relates StandardsActionGetObjects */
  typedef std::vector<StandardsActionGetObjects> StandardsActionGetObjectsVector;

} // detail

} // ruleset
} // openstudio

#endif // RULESET_STANDARDSACTIONGETOBJECTS_HPP
