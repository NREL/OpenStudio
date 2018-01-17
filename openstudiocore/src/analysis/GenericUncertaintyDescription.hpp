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

#ifndef ANALYSIS_GENERICUNCERTAINTYDESCRIPTION_HPP
#define ANALYSIS_GENERICUNCERTAINTYDESCRIPTION_HPP

#include "AnalysisAPI.hpp"
#include "UncertaintyDescription.hpp"

namespace openstudio {

class Attribute;
struct AttributeDescription;

namespace analysis {

/** GenericUncertaintyDescription is an UncertaintyDescription interface for use by
 *  APIs and for serialization/deserialization. It works equally well for all 
 *  \link UncertaintyDescriptionType UncertaintyDescriptionTypes \endlink.  */
class ANALYSIS_API GenericUncertaintyDescription : public UncertaintyDescription {
 public:
  /** @name Constructors and Destructors */
  //@{

  GenericUncertaintyDescription(const UncertaintyDescriptionType& type);

  /** Constructor provided for deserialization; not for general use. */
  GenericUncertaintyDescription(const UncertaintyDescriptionType& type,
                                const std::vector<Attribute>& attributes);

  virtual ~GenericUncertaintyDescription() {}

  //@}

  /** Returns the interface UncertaintyDescriptionType, that is,
   *  UncertaintyDescriptionType::Generic */
  static UncertaintyDescriptionType type();

  /** @name Getters and Queries */
  //@{

  /** Returns the UncertaintyDescriptionType that was used to construct this object, that
   *  is, the underlying, actual, type. */
  UncertaintyDescriptionType actualType() const;

  /** Returns all options that are set. */
  std::vector<Attribute> attributes() const;

  /** Describes all options for this UncertaintyDescription::type(), including whether they 
   *  are required. */
  std::vector<AttributeDescription> attributeDescriptions() const;

  /** Returns Attribute attributeName by value (returned object does not share data with 
   *  private member). Throws if not isSet(attributeName). */
  Attribute getAttribute(const std::string& attributeName) const;

  bool isSet(const std::string& attributeName) const;

  /** Returns true if all required attributes are set. */
  bool isComplete() const;

  //@}
  /** @name Setters */
  //@{

  /** Sets candidate Attribute if possible. Must match one of the attributeDescriptions(). 
   *  \post candidate does not share data with any member variables */
  bool setAttribute(const Attribute& candidate);

  /** Clears Attribute attributeName if that Attribute is currently set and is not required. */
  bool clearAttribute(const std::string& attributeName);

  //@}
 protected:
  explicit GenericUncertaintyDescription(std::shared_ptr<detail::UncertaintyDescription_Impl> impl);

  friend class UncertaintyDescription;
 private:
  REGISTER_LOGGER("openstudio.analysis.GenericUncertaintyDescription");
};

/** \relates GenericUncertaintyDescription */
typedef std::vector<GenericUncertaintyDescription> GenericUncertaintyDescriptionVector;

} // analysis
} // openstudio

#endif // ANALYSIS_GENERICUNCERTAINTYDESCRIPTION_HPP
