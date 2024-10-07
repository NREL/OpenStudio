/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_MATERIALPROPERTYMOISTUREPENETRATIONDEPTHSETTINGS_HPP
#define MODEL_MATERIALPROPERTYMOISTUREPENETRATIONDEPTHSETTINGS_HPP

#include <vector>
#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  class Material;
  class StandardOpaqueMaterial;
  class MasslessOpaqueMaterial;

  namespace detail {

    class MaterialPropertyMoisturePenetrationDepthSettings_Impl;

  }  // namespace detail

  /** MaterialPropertyMoisturePenetrationDepthSettings is a ModelObject that wraps the OpenStudio IDD object 'OS:MaterialProperty:MoisturePenetrationDepth:Settings'. */
  class MODEL_API MaterialPropertyMoisturePenetrationDepthSettings : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit MaterialPropertyMoisturePenetrationDepthSettings(StandardOpaqueMaterial& material, double waterVaporDiffusionResistanceFactor,
                                                              double moistureEquationCoefficientA, double moistureEquationCoefficientB,
                                                              double moistureEquationCoefficientC, double moistureEquationCoefficientD,
                                                              double coatingLayerThickness, double coatingLayerWaterVaporDiffusionResistanceFactor);

    explicit MaterialPropertyMoisturePenetrationDepthSettings(MasslessOpaqueMaterial& material, double waterVaporDiffusionResistanceFactor,
                                                              double moistureEquationCoefficientA, double moistureEquationCoefficientB,
                                                              double moistureEquationCoefficientC, double moistureEquationCoefficientD,
                                                              double coatingLayerThickness, double coatingLayerWaterVaporDiffusionResistanceFactor);

    virtual ~MaterialPropertyMoisturePenetrationDepthSettings() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    MaterialPropertyMoisturePenetrationDepthSettings(const MaterialPropertyMoisturePenetrationDepthSettings& other) = default;
    MaterialPropertyMoisturePenetrationDepthSettings(MaterialPropertyMoisturePenetrationDepthSettings&& other) = default;
    MaterialPropertyMoisturePenetrationDepthSettings& operator=(const MaterialPropertyMoisturePenetrationDepthSettings&) = default;
    MaterialPropertyMoisturePenetrationDepthSettings& operator=(MaterialPropertyMoisturePenetrationDepthSettings&&) = default;

    //@}

    static IddObjectType iddObjectType();

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
    /// @cond
    using ImplType = detail::MaterialPropertyMoisturePenetrationDepthSettings_Impl;

    explicit MaterialPropertyMoisturePenetrationDepthSettings(std::shared_ptr<detail::MaterialPropertyMoisturePenetrationDepthSettings_Impl> impl);

    friend class detail::MaterialPropertyMoisturePenetrationDepthSettings_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.MaterialPropertyMoisturePenetrationDepthSettings");
  };

  /** \relates MaterialPropertyMoisturePenetrationDepthSettings*/
  using OptionalMaterialPropertyMoisturePenetrationDepthSettings = boost::optional<MaterialPropertyMoisturePenetrationDepthSettings>;

  /** \relates MaterialPropertyMoisturePenetrationDepthSettings*/
  using MaterialPropertyMoisturePenetrationDepthSettingsVector = std::vector<MaterialPropertyMoisturePenetrationDepthSettings>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_MATERIALPROPERTYMOISTUREPENETRATIONDEPTHSETTINGS_HPP
