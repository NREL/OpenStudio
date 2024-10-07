/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_MATERIALPROPERTYMOISTUREPENETRATIONDEPTHSETTINGS_IMPL_HPP
#define MODEL_MATERIALPROPERTYMOISTUREPENETRATIONDEPTHSETTINGS_IMPL_HPP

#include <vector>
#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class Material;

  namespace detail {

    /** MaterialPropertyMoisturePenetrationDepthSettings_Impl is a ModelObject_Impl that is the implementation class for MaterialPropertyMoisturePenetrationDepthSettings.*/
    class MODEL_API MaterialPropertyMoisturePenetrationDepthSettings_Impl : public ModelObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      MaterialPropertyMoisturePenetrationDepthSettings_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      MaterialPropertyMoisturePenetrationDepthSettings_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                            bool keepHandle);

      MaterialPropertyMoisturePenetrationDepthSettings_Impl(const MaterialPropertyMoisturePenetrationDepthSettings_Impl& other, Model_Impl* model,
                                                            bool keepHandle);

      virtual ~MaterialPropertyMoisturePenetrationDepthSettings_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      std::string materialName() const;

      Material material() const;

      double waterVaporDiffusionResistanceFactor() const;

      double moistureEquationCoefficientA() const;

      double moistureEquationCoefficientB() const;

      double moistureEquationCoefficientC() const;

      double moistureEquationCoefficientD() const;

      std::vector<double> moistureEquationCoefficients() const;

      boost::optional<double> surfaceLayerPenetrationDepth();

      bool isSurfaceLayerPenetrationDepthAutocalculated();

      boost::optional<double> deepLayerPenetrationDepth();

      bool isDeepLayerPenetrationDepthAutocalculated();

      double coatingLayerThickness() const;

      double coatingLayerWaterVaporDiffusionResistanceFactor() const;

      //@}
      /** @name Setters */
      //@{

      bool setWaterVaporDiffusionResistanceFactor(double waterVaporDiffusionResistanceFactor);

      bool setMoistureEquationCoefficientA(double moistureEquationCoefficientA);

      bool setMoistureEquationCoefficientB(double moistureEquationCoefficientB);

      bool setMoistureEquationCoefficientC(double moistureEquationCoefficientC);

      bool setMoistureEquationCoefficientD(double moistureEquationCoefficientD);

      bool setMoistureEquationCoefficients(double moistureEquationCoefficientA, double moistureEquationCoefficientB,
                                           double moistureEquationCoefficientC, double moistureEquationCoefficientD);

      bool setSurfaceLayerPenetrationDepth(double surfaceLayerPenetrationDepth);

      void autocalculateSurfaceLayerPenetrationDepth();

      bool setDeepLayerPenetrationDepth(double deepLayerPenetrationDepth);

      void autocalculateDeepLayerPenetrationDepth();

      bool setCoatingLayerThickness(double coatingLayerThickness);

      bool setCoatingLayerWaterVaporDiffusionResistanceFactor(double coatingLayerWaterVaporDiffusionResistanceFactor);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.MaterialPropertyMoisturePenetrationDepthSettings");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_MATERIALPROPERTYMOISTUREPENETRATIONDEPTHSETTINGS_IMPL_HPP
