/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ADDITIONALPROPERTIES_IMPL_HPP
#define MODEL_ADDITIONALPROPERTIES_IMPL_HPP

#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class AdditionalProperties;
  class ModelObject;

  namespace detail {

    class MODEL_API AdditionalProperties_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AdditionalProperties_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AdditionalProperties_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AdditionalProperties_Impl(const AdditionalProperties_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AdditionalProperties_Impl() override = default;

      //@}

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      /// return the parent object in the hierarchy
      virtual boost::optional<ParentObject> parent() const override;

      /// set the parent, child may have to call non-const methods on the parent
      virtual bool setParent(ParentObject& newParent) override;

      /** Get the resources directly used by this ModelObject. */
      virtual std::vector<ResourceObject> resources() const override;

      /** @name Getters */
      //@{

      ModelObject modelObject() const;

      std::vector<std::string> featureNames() const;

      bool hasFeature(const std::string& name) const;

      boost::optional<std::string> getFeatureDataType(const std::string& name) const;

      boost::optional<std::string> getFeatureAsString(const std::string& name) const;

      boost::optional<double> getFeatureAsDouble(const std::string& name) const;

      boost::optional<int> getFeatureAsInteger(const std::string& name) const;

      boost::optional<bool> getFeatureAsBoolean(const std::string& name) const;

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

      void merge(const AdditionalProperties& other, bool overwrite = false);

     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AdditionalProperties")

      boost::optional<ModelExtensibleGroup> getFeatureGroupByName(const std::string& name) const;

      bool setFeatureGroupDataTypeAndValue(const std::string& name, const std::string& dataType, const std::string& value);

      boost::optional<std::string> getFeatureStringAndCheckForType(const std::string& name, const std::string& expectedDataType) const;
    };

  }  // namespace detail
}  // namespace model
}  // namespace openstudio

#endif  //MODEL_ADDITIONALPROPERTIES_IMPL_HPP
