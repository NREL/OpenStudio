/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef MODEL_ADDITIONALPROPERTIES_HPP
#define MODEL_ADDITIONALPROPERTIES_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  class ModelObject;

  namespace detail {
    class ModelObject_Impl;
    class AdditionalProperties_Impl;

  }  // namespace detail

  class MODEL_API AdditionalProperties : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~AdditionalProperties() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AdditionalProperties(const AdditionalProperties& other) = default;
    AdditionalProperties(AdditionalProperties&& other) = default;
    AdditionalProperties& operator=(const AdditionalProperties&) = default;
    AdditionalProperties& operator=(AdditionalProperties&&) = default;

    //@}

    /** @name Static Methods */
    //@{

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    /// Returns the ModelObject this AdditionalProperties object refers to.
    /// This method will throw if the ModelObject cannot be found.
    ModelObject modelObject() const;

    /// Returns the set of feature name strings.
    std::vector<std::string> featureNames() const;

    /// Returns true if a feature exists
    bool hasFeature(const std::string& name) const;

    /// Returns optional feature data type as String, Double, Boolean, or Integer.
    boost::optional<std::string> getFeatureDataType(const std::string& name) const;

    /// Returns optional feature value as String.
    boost::optional<std::string> getFeatureAsString(const std::string& name) const;

    /// Returns optional feature value as Double.
    boost::optional<double> getFeatureAsDouble(const std::string& name) const;

    /// Returns optional feature value as Integer.
    boost::optional<int> getFeatureAsInteger(const std::string& name) const;

    /// Returns optional feature value as Boolean.
    boost::optional<bool> getFeatureAsBoolean(const std::string& name) const;

    /// Returns set of AdditionalProperties feature names already in the model.
    std::vector<std::string> suggestedFeatureNames() const;

    //@}
    /** @name Setters */
    //@{

    bool setFeature(const std::string& name, const std::string& value);

    bool setFeature(const std::string& name, const char* value);

    bool setFeature(const std::string& name, double value);

    bool setFeature(const std::string& name, int value);

    bool setFeature(const std::string& name, bool value);

    bool resetFeature(const std::string& name);

    //@}

    /// Merge another AdditionalProperties object with this one
    /// If overwrite is true, the keys from other object overwrite keys from this
    void merge(const AdditionalProperties& other, bool overwrite = false);

   protected:
    /// @cond

    using ImplType = detail::AdditionalProperties_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    friend class detail::AdditionalProperties_Impl;
    friend class detail::ModelObject_Impl;

    /** Create new object as child of model object. */
    explicit AdditionalProperties(const ModelObject& modelObject);

    explicit AdditionalProperties(std::shared_ptr<detail::AdditionalProperties_Impl> impl);

    /// @endcond

   private:
    REGISTER_LOGGER("openstudio.model.AdditionalProperties")
  };

  /** \relates AdditionalProperties */
  using OptionalAdditionalProperties = boost::optional<AdditionalProperties>;

  /** \relates AdditionalProperties */
  using AdditionalPropertiesVector = std::vector<AdditionalProperties>;

}  // namespace model

}  // namespace openstudio

#endif  //MODEL_ADDITIONALPROPERTIES_HPP
