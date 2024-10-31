/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

    virtual ~AdditionalProperties() override = default;
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
