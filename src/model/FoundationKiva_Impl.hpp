/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_FOUNDATIONKIVA_IMPL_HPP
#define MODEL_FOUNDATIONKIVA_IMPL_HPP

#include <vector>
#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class Material;
  class ConstructionBase;
  class Surface;
  class CustomBlock;

  namespace detail {

    /** FoundationKiva_Impl is a ModelObject_Impl that is the implementation class for FoundationKiva.*/
    class MODEL_API FoundationKiva_Impl : public ModelObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      FoundationKiva_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      FoundationKiva_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      FoundationKiva_Impl(const FoundationKiva_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~FoundationKiva_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //extensible fields

      bool addCustomBlock(const CustomBlock& customBlock);

      bool addCustomBlock(const Material& material, double depth, double xPosition, double zPosition);

      bool removeCustomBlock(unsigned groupIndex);

      void removeAllCustomBlocks();

      std::vector<CustomBlock> customBlocks() const;

      bool addCustomBlocks(const std::vector<CustomBlock>& customBlocks);

      //@}
      /** @name Getters */
      //@{

      boost::optional<double> initialIndoorAirTemperature();

      boost::optional<Material> interiorHorizontalInsulationMaterial() const;

      double interiorHorizontalInsulationDepth() const;

      bool isInteriorHorizontalInsulationDepthDefaulted() const;

      boost::optional<double> interiorHorizontalInsulationWidth();

      boost::optional<Material> interiorVerticalInsulationMaterial() const;

      boost::optional<double> interiorVerticalInsulationDepth();

      boost::optional<Material> exteriorHorizontalInsulationMaterial() const;

      boost::optional<double> exteriorHorizontalInsulationDepth();

      double exteriorHorizontalInsulationWidth() const;

      bool isExteriorHorizontalInsulationWidthDefaulted() const;

      boost::optional<Material> exteriorVerticalInsulationMaterial() const;

      boost::optional<double> exteriorVerticalInsulationDepth();

      double wallHeightAboveGrade() const;

      bool isWallHeightAboveGradeDefaulted() const;

      double wallDepthBelowSlab() const;

      bool isWallDepthBelowSlabDefaulted() const;

      boost::optional<ConstructionBase> footingWallConstruction() const;

      boost::optional<Material> footingMaterial() const;

      double footingDepth() const;

      bool isFootingDepthDefaulted() const;

      std::vector<Surface> surfaces() const;

      unsigned int numberofCustomBlocks() const;

      //@}
      /** @name Setters */
      //@{

      bool setInitialIndoorAirTemperature(double initialIndoorAirTemperature);

      bool setInteriorHorizontalInsulationMaterial(const Material& material);

      void resetInteriorHorizontalInsulationMaterial();

      bool setInteriorHorizontalInsulationDepth(double interiorHorizontalInsulationDepth);

      void resetInteriorHorizontalInsulationDepth();

      bool setInteriorHorizontalInsulationWidth(double interiorHorizontalInsulationWidth);

      bool setInteriorVerticalInsulationMaterial(const Material& material);

      void resetInteriorVerticalInsulationMaterial();

      bool setInteriorVerticalInsulationDepth(double interiorVerticalInsulationDepth);

      bool setExteriorHorizontalInsulationMaterial(const Material& material);

      void resetExteriorHorizontalInsulationMaterial();

      bool setExteriorHorizontalInsulationDepth(double exteriorHorizontalInsulationDepth);

      bool setExteriorHorizontalInsulationWidth(double exteriorHorizontalInsulationWidth);

      void resetExteriorHorizontalInsulationWidth();

      bool setExteriorVerticalInsulationMaterial(const Material& material);

      void resetExteriorVerticalInsulationMaterial();

      bool setExteriorVerticalInsulationDepth(double exteriorVerticalInsulationDepth);

      bool setWallHeightAboveGrade(double wallHeightAboveGrade);

      void resetWallHeightAboveGrade();

      bool setWallDepthBelowSlab(double wallDepthBelowSlab);

      void resetWallDepthBelowSlab();

      bool setFootingWallConstruction(const ConstructionBase& construction);

      void resetFootingWallConstruction();

      bool setFootingMaterial(const Material& material);

      void resetFootingMaterial();

      bool setFootingDepth(double footingDepth);

      void resetFootingDepth();

      //@}
      /** @name Other */
      //@{

      virtual ModelObject clone(Model model) const override;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.FoundationKiva");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_FOUNDATIONKIVA_IMPL_HPP
