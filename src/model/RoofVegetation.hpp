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

    virtual ~RoofVegetation() = default;
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
