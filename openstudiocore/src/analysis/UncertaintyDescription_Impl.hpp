/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#ifndef ANALYSIS_UNCERTAINTYDESCRIPTION_IMPL_HPP
#define ANALYSIS_UNCERTAINTYDESCRIPTION_IMPL_HPP

#include "AnalysisAPI.hpp"
#include "AnalysisEnums.hpp"

#include "../utilities/data/Attribute.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  class ANALYSIS_API UncertaintyDescription_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Populates attribute descriptions and sets default attribute values based on type. */
    UncertaintyDescription_Impl(const UncertaintyDescriptionType& type);

    /** Constructor provided for deserialization; not for general use. */
    UncertaintyDescription_Impl(const UncertaintyDescriptionType& type,
                                const std::vector<Attribute>& attributes);

    /** Provides deep copy of attributes. */
    UncertaintyDescription_Impl(const UncertaintyDescription_Impl& other);

    //@}
    /** @name Getters and Queries */
    //@{

    UncertaintyDescriptionType type() const;

    std::vector<Attribute> attributes(bool clones) const;

    Attribute getAttribute(const std::string& attributeName, bool clone) const;

    bool isSet(const std::string& attributeName) const;

    std::vector<AttributeDescription> attributeDescriptions() const;

    AttributeDescription getAttributeDescription(const std::string& attributeName) const;

    bool hasDescription(const std::string& attributeName) const;

    /** Returns true if all required attributes are set. */
    bool isComplete() const;

    //@}
    /** @name Setters */
    //@{

    bool setAttribute(const Attribute& candidate, bool check);

    /** Clears Attribute attributeName if that Attribute is currently set but not required. */
    bool clearAttribute(const std::string& attributeName, bool check);

    /** Sets the default attributes based on type(). */
    void restoreDefaults();

    //@}
   private:
    UncertaintyDescriptionType m_type;
    std::vector<Attribute> m_attributes;
    std::vector<AttributeDescription> m_attributeDescriptions;

    REGISTER_LOGGER("openstudio.analysis.UncertaintyDescription");

    /** Populates the attribute descriptions based on type. */
    void populateAttributeDescriptions();
  };

} // detail

} // analysis
} // openstudio

#endif // ANALYSIS_UNCERTAINTYDESCRIPTION_IMPL_HPP
