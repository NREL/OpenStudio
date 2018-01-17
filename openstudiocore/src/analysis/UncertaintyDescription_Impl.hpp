/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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
