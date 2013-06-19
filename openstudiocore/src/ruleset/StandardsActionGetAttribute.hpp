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

#ifndef RULESET_STANDARDSACTIONGETATTRIBUTE_HPP
#define RULESET_STANDARDSACTIONGETATTRIBUTE_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/StandardsActionClause.hpp>

#include <standardsinterface/BooleanAttribute.hxx>
#include <standardsinterface/DoubleAttribute.hxx>
#include <standardsinterface/EnumAttribute.hxx>
#include <standardsinterface/IntegerAttribute.hxx>

namespace openstudio {
namespace ruleset {

class StandardsFilterAvailableObject;

namespace detail {
  class StandardsActionGetAttribute_Impl;

  /** This is a back-end ActionClause added as part of back-end \link Rule Rules\endlink created
   *  by Ruleset in response to \link Rule Rules\endlink with \link StandardsFilterObjectAttribute
   *  FilterObjectAttributes\endlink. This clause kicks off the process of checking and retrieving
   *  an attribute through an AgnosticInterface. */
  class RULESET_API StandardsActionGetAttribute : public StandardsActionClause {
   public:

     typedef detail::StandardsActionGetAttribute_Impl ImplType;

     static std::string xmlElementName();

    /** @name Constructors and Destructors */
    //@{

    StandardsActionGetAttribute(const StandardsFilterAvailableObject& object,
                                const standardsinterface::BooleanAttribute& attribute);

    /** Constructor provided for deserialization; not for general use. */
    StandardsActionGetAttribute(const StandardsFilterAvailableObject& object,
                                const standardsinterface::BooleanAttribute& attribute,
                                const UUID& uuid,
                                const UUID& versionUUID);

    StandardsActionGetAttribute(const StandardsFilterAvailableObject& object,
                                const standardsinterface::DoubleAttribute& attribute);

    /** Constructor provided for deserialization; not for general use. */
    StandardsActionGetAttribute(const StandardsFilterAvailableObject& object,
                                const standardsinterface::DoubleAttribute& attribute,
                                const UUID& uuid,
                                const UUID& versionUUID);

    StandardsActionGetAttribute(const StandardsFilterAvailableObject& object,
                                const standardsinterface::EnumAttribute& attribute);

    /** Constructor provided for deserialization; not for general use. */
    StandardsActionGetAttribute(const StandardsFilterAvailableObject& object,
                                const standardsinterface::EnumAttribute& attribute,
                                const UUID& uuid,
                                const UUID& versionUUID);

    StandardsActionGetAttribute(const StandardsFilterAvailableObject& object,
                                const standardsinterface::IntegerAttribute& attribute);

    /** Constructor provided for deserialization; not for general use. */
    StandardsActionGetAttribute(const StandardsFilterAvailableObject& object,
                                const standardsinterface::IntegerAttribute& attribute,
                                const UUID& uuid,
                                const UUID& versionUUID);

    //@}
   protected:

    friend class detail::StandardsActionGetAttribute_Impl;
    friend class RulesetObject;

    // protected object setter used by deepClone.
    void mf_setObject(const StandardsFilterAvailableObject& object);

    /** Protected constructor from impl. */
    StandardsActionGetAttribute(boost::shared_ptr<detail::StandardsActionGetAttribute_Impl> impl);

   private:
    REGISTER_LOGGER("Ruleset.StandardsActionGetAttribute");
  };

  /** \relates StandardsActionGetAttribute */
  typedef boost::optional<StandardsActionGetAttribute> OptionalStandardsActionGetAttribute;

  /** \relates StandardsActionGetAttribute */
  typedef std::vector<StandardsActionGetAttribute> StandardsActionGetAttributeVector;

} // detail

} // ruleset
} // openstudio

#endif // RULESET_STANDARDSACTIONGETATTRIBUTE_HPP
