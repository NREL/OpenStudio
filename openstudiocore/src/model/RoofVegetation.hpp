/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

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

