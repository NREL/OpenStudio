/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ROOFVEGETATION_HPP
#define MODEL_ROOFVEGETATION_HPP

#include "ModelAPI.hpp"
#include "OpaqueMaterial.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class RoofVegetation_Impl;

  }  // namespace detail

  /** RoofVegetation is a OpaqueMaterial that wraps the OpenStudio IDD object 'OS:Material:RoofVegetation'. */
  class MODEL_API RoofVegetation : public OpaqueMaterial
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit RoofVegetation(const Model& model, const std::string& roughness = "Smooth");

    virtual ~RoofVegetation() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    RoofVegetation(const RoofVegetation& other) = default;
    RoofVegetation(RoofVegetation&& other) = default;
    RoofVegetation& operator=(const RoofVegetation&) = default;
    RoofVegetation& operator=(RoofVegetation&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> roughnessValues();

    static std::vector<std::string> moistureDiffusionCalculationMethodValues();

    /** @name Getters */
    //@{

    double heightofPlants() const;

    bool isHeightofPlantsDefaulted() const;

    double leafAreaIndex() const;

    bool isLeafAreaIndexDefaulted() const;

    double leafReflectivity() const;

    bool isLeafReflectivityDefaulted() const;

    double leafEmissivity() const;

    bool isLeafEmissivityDefaulted() const;

    double minimumStomatalResistance() const;

    bool isMinimumStomatalResistanceDefaulted() const;

    std::string soilLayerName() const;

    bool isSoilLayerNameDefaulted() const;

    std::string roughness() const;

    /** Density of dry soil (kg/m^3). */
    double density() const;

    /** Specific heat of dry soil (J/kg*K). */
    double specificHeat() const;

    /** Maximum moisture content of the soil layer by volume fraction. */
    double saturationVolumetricMoistureContent() const;

    double residualVolumetricMoistureContent() const;

    double initialVolumetricMoistureContent() const;

    bool isRoughnessDefaulted() const;

    double thickness() const;

    bool isThicknessDefaulted() const;

    double conductivityofDrySoil() const;

    bool isConductivityofDrySoilDefaulted() const;

    double densityofDrySoil() const;

    bool isDensityofDrySoilDefaulted() const;

    double specificHeatofDrySoil() const;

    bool isSpecificHeatofDrySoilDefaulted() const;

    boost::optional<double> thermalAbsorptance() const;

    bool isThermalAbsorptanceDefaulted() const;

    boost::optional<double> solarAbsorptance() const;

    bool isSolarAbsorptanceDefaulted() const;

    boost::optional<double> visibleAbsorptance() const;

    bool isVisibleAbsorptanceDefaulted() const;

    double saturationVolumetricMoistureContentoftheSoilLayer() const;

    bool isSaturationVolumetricMoistureContentoftheSoilLayerDefaulted() const;

    double residualVolumetricMoistureContentoftheSoilLayer() const;

    bool isResidualVolumetricMoistureContentoftheSoilLayerDefaulted() const;

    double initialVolumetricMoistureContentoftheSoilLayer() const;

    bool isInitialVolumetricMoistureContentoftheSoilLayerDefaulted() const;

    std::string moistureDiffusionCalculationMethod() const;

    bool isMoistureDiffusionCalculationMethodDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setLeafAreaIndex(double leafAreaIndex);

    bool setLeafReflectivity(double leafReflectivity);

    bool setLeafEmissivity(double leafEmissivity);

    bool setMinimumStomatalResistance(double value);

    bool setSoilLayerName(const std::string& name);

    bool setRoughness(const std::string& value);

    /** Set density of dry soil (kg/m^3). */
    bool setDensity(double value);

    /** Set specific heat of dry soil (J/kg*K). */
    bool setSpecificHeat(double value);

    /** Set the maximum moisture content of the soil layer by volume fraction. */
    bool setSaturationVolumetricMoistureContent(double value);

    bool setResidualVolumetricMoistureContent(double value);

    bool setInitialVolumetricMoistureConent(double value);

    bool setMoistureDiffusionCalculationMethod(const std::string& value);

    bool setHeightofPlants(double heightofPlants);

    void resetHeightofPlants();

    void resetLeafAreaIndex();

    void resetLeafReflectivity();

    void resetLeafEmissivity();

    void resetMinimumStomatalResistance();

    void resetSoilLayerName();

    void resetRoughness();

    bool setThickness(double thickness);

    void resetThickness();

    bool setConductivityofDrySoil(double conductivityofDrySoil);

    void resetConductivityofDrySoil();

    bool setDensityofDrySoil(double densityofDrySoil);

    void resetDensityofDrySoil();

    bool setSpecificHeatofDrySoil(double specificHeatofDrySoil);

    void resetSpecificHeatofDrySoil();

    bool setThermalAbsorptance(double thermalAbsorptance);

    void resetThermalAbsorptance();

    bool setSolarAbsorptance(double solarAbsorptance);

    void resetSolarAbsorptance();

    bool setVisibleAbsorptance(double visibleAbsorptance);

    void resetVisibleAbsorptance();

    bool setSaturationVolumetricMoistureContentoftheSoilLayer(double saturationVolumetricMoistureContentoftheSoilLayer);

    void resetSaturationVolumetricMoistureContentoftheSoilLayer();

    bool setResidualVolumetricMoistureContentoftheSoilLayer(double residualVolumetricMoistureContentoftheSoilLayer);

    void resetResidualVolumetricMoistureContentoftheSoilLayer();

    bool setInitialVolumetricMoistureContentoftheSoilLayer(double initialVolumetricMoistureContentoftheSoilLayer);

    void resetInitialVolumetricMoistureContentoftheSoilLayer();

    void resetMoistureDiffusionCalculationMethod();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::RoofVegetation_Impl;

    explicit RoofVegetation(std::shared_ptr<detail::RoofVegetation_Impl> impl);

    friend class detail::RoofVegetation_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.RoofVegetation");
  };

  /** \relates RoofVegetation*/
  using OptionalRoofVegetation = boost::optional<RoofVegetation>;

  /** \relates RoofVegetation*/
  using RoofVegetationVector = std::vector<RoofVegetation>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ROOFVEGETATION_HPP
