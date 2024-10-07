/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_STANDARDSINFORMATIONMATERIAL_IMPL_HPP
#define MODEL_STANDARDSINFORMATIONMATERIAL_IMPL_HPP

#include "ModelObject_Impl.hpp"

namespace Json {
class Value;
}

namespace openstudio {
namespace model {

  class Material;

  namespace detail {

    /** StandardsInformationMaterial_Impl is a ModelObject_Impl that is the implementation class for StandardsInformationMaterial.*/
    class MODEL_API StandardsInformationMaterial_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      StandardsInformationMaterial_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      StandardsInformationMaterial_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      StandardsInformationMaterial_Impl(const StandardsInformationMaterial_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~StandardsInformationMaterial_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual boost::optional<ParentObject> parent() const override;

      virtual bool setParent(ParentObject& newParent) override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      Material material() const;

      boost::optional<std::string> materialStandard() const;
      std::vector<std::string> suggestedMaterialStandards() const;

      boost::optional<std::string> materialStandardSource() const;
      std::vector<std::string> suggestedMaterialStandardSources() const;

      boost::optional<std::string> standardsCategory() const;
      std::vector<std::string> suggestedStandardsCategories() const;
      bool isCompositeMaterial() const;

      boost::optional<std::string> standardsIdentifier() const;
      std::vector<std::string> suggestedStandardsIdentifiers() const;

      boost::optional<std::string> compositeFramingMaterial() const;
      std::vector<std::string> suggestedCompositeFramingMaterials() const;

      boost::optional<std::string> compositeFramingConfiguration() const;
      std::vector<std::string> suggestedCompositeFramingConfigurations() const;

      boost::optional<std::string> compositeFramingDepth() const;
      std::vector<std::string> suggestedCompositeFramingDepths() const;

      boost::optional<std::string> compositeFramingSize() const;
      std::vector<std::string> suggestedCompositeFramingSizes() const;

      boost::optional<std::string> compositeCavityInsulation() const;
      std::vector<std::string> suggestedCompositeCavityInsulations() const;

      //@}
      /** @name Setters */
      //@{

      bool setMaterialStandard(const std::string& materialStandard);
      void resetMaterialStandard();

      bool setMaterialStandardSource(const std::string& materialStandardSource);
      void resetMaterialStandardSource();

      bool setStandardsCategory(const std::string& standardsCategory);
      void resetStandardsCategory();

      bool setStandardsIdentifier(const std::string& standardsIdentifier);
      void resetStandardsIdentifier();

      bool setCompositeFramingMaterial(const std::string& compositeFramingMaterial);
      void resetCompositeFramingMaterial();

      bool setCompositeFramingConfiguration(const std::string& compositeFramingConfiguration);
      void resetCompositeFramingConfiguration();

      bool setCompositeFramingDepth(const std::string& compositeFramingDepth);
      void resetCompositeFramingDepth();

      bool setCompositeFramingSize(const std::string& compositeFramingSize);
      void resetCompositeFramingSize();

      bool setCompositeCavityInsulation(const std::string& compositeCavityInsulation);
      void resetCompositeCavityInsulation();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.StandardsInformationMaterial");

      // Relies on utilties/StandardsJSON since it's used in several places
      // Here we store a Json::Value rather than the StandardsJSON because we only care about the "materials" primaryKey
      // Not the whole StandardsJSON
      static const Json::Value& getStandardsJSON();
      static Json::Value parseStandardsJSON();
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_STANDARDSINFORMATIONMATERIAL_IMPL_HPP
