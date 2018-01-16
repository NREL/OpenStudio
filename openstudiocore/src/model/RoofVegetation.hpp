/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef MODEL_ROOFVEGETATION_HPP
#define MODEL_ROOFVEGETATION_HPP

#include "ModelAPI.hpp"
#include "OpaqueMaterial.hpp"

namespace openstudio {

class Quantity;
class OSOptionalQuantity;

namespace model {

namespace detail {

  class RoofVegetation_Impl;

} // detail

/** RoofVegetation is a OpaqueMaterial that wraps the OpenStudio IDD object 'OS:Material:RoofVegetation'. */
class MODEL_API RoofVegetation : public OpaqueMaterial {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit RoofVegetation(const Model& model,
                          std::string roughness = "Smooth");

  virtual ~RoofVegetation() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> roughnessValues();

  static std::vector<std::string> moistureDiffusionCalculationMethodValues();

  /** @name Getters */
  //@{

  double heightofPlants() const;

  Quantity getHeightofPlants(bool returnIP=false) const;

  bool isHeightofPlantsDefaulted() const;

  double leafAreaIndex() const;

  Quantity getLeafAreaIndex(bool returnIP=false) const;

  bool isLeafAreaIndexDefaulted() const;

  double leafReflectivity() const;

  Quantity getLeafReflectivity(bool returnIP=false) const;

  bool isLeafReflectivityDefaulted() const;

  double leafEmissivity() const;

  Quantity getLeafEmissivity(bool returnIP=false) const;

  bool isLeafEmissivityDefaulted() const;

  double minimumStomatalResistance() const;

  Quantity getMinimumStomatalResistance(bool returnIP=false) const;

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

  Quantity getThickness(bool returnIP=false) const;

  bool isThicknessDefaulted() const;

  double conductivityofDrySoil() const;

  Quantity getConductivityofDrySoil(bool returnIP=false) const;

  bool isConductivityofDrySoilDefaulted() const;

  double densityofDrySoil() const;

  Quantity getDensityofDrySoil(bool returnIP=false) const;

  bool isDensityofDrySoilDefaulted() const;

  double specificHeatofDrySoil() const;

  Quantity getSpecificHeatofDrySoil(bool returnIP=false) const;

  bool isSpecificHeatofDrySoilDefaulted() const;

  boost::optional<double> thermalAbsorptance() const;

  Quantity getThermalAbsorptance(bool returnIP=false) const;

  bool isThermalAbsorptanceDefaulted() const;

  boost::optional<double> solarAbsorptance() const;

  Quantity getSolarAbsorptance(bool returnIP=false) const;

  bool isSolarAbsorptanceDefaulted() const;

  boost::optional<double> visibleAbsorptance() const;

  Quantity getVisibleAbsorptance(bool returnIP=false) const;

  bool isVisibleAbsorptanceDefaulted() const;

  double saturationVolumetricMoistureContentoftheSoilLayer() const;

  Quantity getSaturationVolumetricMoistureContentoftheSoilLayer(bool returnIP=false) const;

  bool isSaturationVolumetricMoistureContentoftheSoilLayerDefaulted() const;

  double residualVolumetricMoistureContentoftheSoilLayer() const;

  Quantity getResidualVolumetricMoistureContentoftheSoilLayer(bool returnIP=false) const;

  bool isResidualVolumetricMoistureContentoftheSoilLayerDefaulted() const;

  double initialVolumetricMoistureContentoftheSoilLayer() const;

  Quantity getInitialVolumetricMoistureContentoftheSoilLayer(bool returnIP=false) const;

  bool isInitialVolumetricMoistureContentoftheSoilLayerDefaulted() const;

  std::string moistureDiffusionCalculationMethod() const;

  bool isMoistureDiffusionCalculationMethodDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  bool setLeafAreaIndex(double value);

  bool setLeafReflectivity(double value);

  bool setLeafEmissivity(double value);

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

  bool setHeightofPlants(const Quantity& heightofPlants);

  void resetHeightofPlants();

  bool setLeafAreaIndex(const Quantity& leafAreaIndex);

  void resetLeafAreaIndex();

  bool setLeafReflectivity(const Quantity& leafReflectivity);

  void resetLeafReflectivity();

  bool setLeafEmissivity(const Quantity& leafEmissivity);

  void resetLeafEmissivity();

  bool setMinimumStomatalResistance(const Quantity& minimumStomatalResistance);

  void resetMinimumStomatalResistance();

  void resetSoilLayerName();

  void resetRoughness();

  bool setThickness(double thickness);

  bool setThickness(const Quantity& thickness);

  void resetThickness();

  bool setConductivityofDrySoil(double conductivityofDrySoil);

  bool setConductivityofDrySoil(const Quantity& conductivityofDrySoil);

  void resetConductivityofDrySoil();

  bool setDensityofDrySoil(double densityofDrySoil);

  bool setDensityofDrySoil(const Quantity& densityofDrySoil);

  void resetDensityofDrySoil();

  bool setSpecificHeatofDrySoil(double specificHeatofDrySoil);

  bool setSpecificHeatofDrySoil(const Quantity& specificHeatofDrySoil);

  void resetSpecificHeatofDrySoil();

  bool setThermalAbsorptance(double thermalAbsorptance);

  bool setThermalAbsorptance(const Quantity& thermalAbsorptance);

  void resetThermalAbsorptance();

  bool setSolarAbsorptance(double solarAbsorptance);

  bool setSolarAbsorptance(const Quantity& solarAbsorptance);

  void resetSolarAbsorptance();

  bool setVisibleAbsorptance(double visibleAbsorptance);

  bool setVisibleAbsorptance(const Quantity& visibleAbsorptance);

  void resetVisibleAbsorptance();

  bool setSaturationVolumetricMoistureContentoftheSoilLayer(double saturationVolumetricMoistureContentoftheSoilLayer);

  bool setSaturationVolumetricMoistureContentoftheSoilLayer(const Quantity& saturationVolumetricMoistureContentoftheSoilLayer);

  void resetSaturationVolumetricMoistureContentoftheSoilLayer();

  bool setResidualVolumetricMoistureContentoftheSoilLayer(double residualVolumetricMoistureContentoftheSoilLayer);

  bool setResidualVolumetricMoistureContentoftheSoilLayer(const Quantity& residualVolumetricMoistureContentoftheSoilLayer);

  void resetResidualVolumetricMoistureContentoftheSoilLayer();

  bool setInitialVolumetricMoistureContentoftheSoilLayer(double initialVolumetricMoistureContentoftheSoilLayer);

  bool setInitialVolumetricMoistureContentoftheSoilLayer(const Quantity& initialVolumetricMoistureContentoftheSoilLayer);

  void resetInitialVolumetricMoistureContentoftheSoilLayer();

  void resetMoistureDiffusionCalculationMethod();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::RoofVegetation_Impl ImplType;

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
typedef boost::optional<RoofVegetation> OptionalRoofVegetation;

/** \relates RoofVegetation*/
typedef std::vector<RoofVegetation> RoofVegetationVector;

} // model
} // openstudio

#endif // MODEL_ROOFVEGETATION_HPP

