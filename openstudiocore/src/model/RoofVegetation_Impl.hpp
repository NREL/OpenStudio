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

#ifndef MODEL_ROOFVEGETATION_IMPL_HPP
#define MODEL_ROOFVEGETATION_IMPL_HPP

#include "ModelAPI.hpp"
#include "OpaqueMaterial_Impl.hpp"

#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

namespace openstudio {
namespace model {

namespace detail {

  /** RoofVegetation_Impl is a OpaqueMaterial_Impl that is the implementation class for RoofVegetation.*/
  class MODEL_API RoofVegetation_Impl : public OpaqueMaterial_Impl {
    Q_OBJECT;

    Q_PROPERTY(double heightofPlants READ heightofPlants WRITE setHeightofPlants RESET resetHeightofPlants);
    Q_PROPERTY(openstudio::Quantity heightofPlants_SI READ heightofPlants_SI WRITE setHeightofPlants RESET resetHeightofPlants);
    Q_PROPERTY(openstudio::Quantity heightofPlants_IP READ heightofPlants_IP WRITE setHeightofPlants RESET resetHeightofPlants);
    Q_PROPERTY(bool isHeightofPlantsDefaulted READ isHeightofPlantsDefaulted);

    Q_PROPERTY(double leafAreaIndex READ leafAreaIndex WRITE setLeafAreaIndex RESET resetLeafAreaIndex);
    Q_PROPERTY(openstudio::Quantity leafAreaIndex_SI READ leafAreaIndex_SI WRITE setLeafAreaIndex RESET resetLeafAreaIndex);
    Q_PROPERTY(openstudio::Quantity leafAreaIndex_IP READ leafAreaIndex_IP WRITE setLeafAreaIndex RESET resetLeafAreaIndex);
    Q_PROPERTY(bool isLeafAreaIndexDefaulted READ isLeafAreaIndexDefaulted);

    Q_PROPERTY(double leafReflectivity READ leafReflectivity WRITE setLeafReflectivity RESET resetLeafReflectivity);
    Q_PROPERTY(openstudio::Quantity leafReflectivity_SI READ leafReflectivity_SI WRITE setLeafReflectivity RESET resetLeafReflectivity);
    Q_PROPERTY(openstudio::Quantity leafReflectivity_IP READ leafReflectivity_IP WRITE setLeafReflectivity RESET resetLeafReflectivity);
    Q_PROPERTY(bool isLeafReflectivityDefaulted READ isLeafReflectivityDefaulted);

    Q_PROPERTY(double leafEmissivity READ leafEmissivity WRITE setLeafEmissivity RESET resetLeafEmissivity);
    Q_PROPERTY(openstudio::Quantity leafEmissivity_SI READ leafEmissivity_SI WRITE setLeafEmissivity RESET resetLeafEmissivity);
    Q_PROPERTY(openstudio::Quantity leafEmissivity_IP READ leafEmissivity_IP WRITE setLeafEmissivity RESET resetLeafEmissivity);
    Q_PROPERTY(bool isLeafEmissivityDefaulted READ isLeafEmissivityDefaulted);

    Q_PROPERTY(double minimumStomatalResistance READ minimumStomatalResistance WRITE setMinimumStomatalResistance RESET resetMinimumStomatalResistance);
    Q_PROPERTY(openstudio::Quantity minimumStomatalResistance_SI READ minimumStomatalResistance_SI WRITE setMinimumStomatalResistance RESET resetMinimumStomatalResistance);
    Q_PROPERTY(openstudio::Quantity minimumStomatalResistance_IP READ minimumStomatalResistance_IP WRITE setMinimumStomatalResistance RESET resetMinimumStomatalResistance);
    Q_PROPERTY(bool isMinimumStomatalResistanceDefaulted READ isMinimumStomatalResistanceDefaulted);

    Q_PROPERTY(std::string soilLayerName READ soilLayerName WRITE setSoilLayerName RESET resetSoilLayerName);
    Q_PROPERTY(bool isSoilLayerNameDefaulted READ isSoilLayerNameDefaulted);

    Q_PROPERTY(std::string roughness READ roughness WRITE setRoughness RESET resetRoughness);
    Q_PROPERTY(bool isRoughnessDefaulted READ isRoughnessDefaulted);
    Q_PROPERTY(std::vector<std::string> roughnessValues READ roughnessValues);

    Q_PROPERTY(double thickness READ thickness WRITE setThickness RESET resetThickness);
    Q_PROPERTY(openstudio::Quantity thickness_SI READ thickness_SI WRITE setThickness RESET resetThickness);
    Q_PROPERTY(openstudio::Quantity thickness_IP READ thickness_IP WRITE setThickness RESET resetThickness);
    Q_PROPERTY(bool isThicknessDefaulted READ isThicknessDefaulted);

    Q_PROPERTY(double conductivityofDrySoil READ conductivityofDrySoil WRITE setConductivityofDrySoil RESET resetConductivityofDrySoil);
    Q_PROPERTY(openstudio::Quantity conductivityofDrySoil_SI READ conductivityofDrySoil_SI WRITE setConductivityofDrySoil RESET resetConductivityofDrySoil);
    Q_PROPERTY(openstudio::Quantity conductivityofDrySoil_IP READ conductivityofDrySoil_IP WRITE setConductivityofDrySoil RESET resetConductivityofDrySoil);
    Q_PROPERTY(bool isConductivityofDrySoilDefaulted READ isConductivityofDrySoilDefaulted);

    Q_PROPERTY(double densityofDrySoil READ densityofDrySoil WRITE setDensityofDrySoil RESET resetDensityofDrySoil);
    Q_PROPERTY(openstudio::Quantity densityofDrySoil_SI READ densityofDrySoil_SI WRITE setDensityofDrySoil RESET resetDensityofDrySoil);
    Q_PROPERTY(openstudio::Quantity densityofDrySoil_IP READ densityofDrySoil_IP WRITE setDensityofDrySoil RESET resetDensityofDrySoil);
    Q_PROPERTY(bool isDensityofDrySoilDefaulted READ isDensityofDrySoilDefaulted);

    Q_PROPERTY(double specificHeatofDrySoil READ specificHeatofDrySoil WRITE setSpecificHeatofDrySoil RESET resetSpecificHeatofDrySoil);
    Q_PROPERTY(openstudio::Quantity specificHeatofDrySoil_SI READ specificHeatofDrySoil_SI WRITE setSpecificHeatofDrySoil RESET resetSpecificHeatofDrySoil);
    Q_PROPERTY(openstudio::Quantity specificHeatofDrySoil_IP READ specificHeatofDrySoil_IP WRITE setSpecificHeatofDrySoil RESET resetSpecificHeatofDrySoil);
    Q_PROPERTY(bool isSpecificHeatofDrySoilDefaulted READ isSpecificHeatofDrySoilDefaulted);

    Q_PROPERTY(double thermalAbsorptance READ thermalAbsorptance WRITE setThermalAbsorptance RESET resetThermalAbsorptance);
    Q_PROPERTY(openstudio::Quantity thermalAbsorptance_SI READ thermalAbsorptance_SI WRITE setThermalAbsorptance RESET resetThermalAbsorptance);
    Q_PROPERTY(openstudio::Quantity thermalAbsorptance_IP READ thermalAbsorptance_IP WRITE setThermalAbsorptance RESET resetThermalAbsorptance);
    Q_PROPERTY(bool isThermalAbsorptanceDefaulted READ isThermalAbsorptanceDefaulted);

    Q_PROPERTY(double solarAbsorptance READ solarAbsorptance WRITE setSolarAbsorptance RESET resetSolarAbsorptance);
    Q_PROPERTY(openstudio::Quantity solarAbsorptance_SI READ solarAbsorptance_SI WRITE setSolarAbsorptance RESET resetSolarAbsorptance);
    Q_PROPERTY(openstudio::Quantity solarAbsorptance_IP READ solarAbsorptance_IP WRITE setSolarAbsorptance RESET resetSolarAbsorptance);
    Q_PROPERTY(bool isSolarAbsorptanceDefaulted READ isSolarAbsorptanceDefaulted);

    Q_PROPERTY(double visibleAbsorptance READ visibleAbsorptance WRITE setVisibleAbsorptance RESET resetVisibleAbsorptance);
    Q_PROPERTY(openstudio::Quantity visibleAbsorptance_SI READ visibleAbsorptance_SI WRITE setVisibleAbsorptance RESET resetVisibleAbsorptance);
    Q_PROPERTY(openstudio::Quantity visibleAbsorptance_IP READ visibleAbsorptance_IP WRITE setVisibleAbsorptance RESET resetVisibleAbsorptance);
    Q_PROPERTY(bool isVisibleAbsorptanceDefaulted READ isVisibleAbsorptanceDefaulted);

    Q_PROPERTY(double saturationVolumetricMoistureContentoftheSoilLayer READ saturationVolumetricMoistureContentoftheSoilLayer WRITE setSaturationVolumetricMoistureContentoftheSoilLayer RESET resetSaturationVolumetricMoistureContentoftheSoilLayer);
    Q_PROPERTY(openstudio::Quantity saturationVolumetricMoistureContentoftheSoilLayer_SI READ saturationVolumetricMoistureContentoftheSoilLayer_SI WRITE setSaturationVolumetricMoistureContentoftheSoilLayer RESET resetSaturationVolumetricMoistureContentoftheSoilLayer);
    Q_PROPERTY(openstudio::Quantity saturationVolumetricMoistureContentoftheSoilLayer_IP READ saturationVolumetricMoistureContentoftheSoilLayer_IP WRITE setSaturationVolumetricMoistureContentoftheSoilLayer RESET resetSaturationVolumetricMoistureContentoftheSoilLayer);
    Q_PROPERTY(bool isSaturationVolumetricMoistureContentoftheSoilLayerDefaulted READ isSaturationVolumetricMoistureContentoftheSoilLayerDefaulted);

    Q_PROPERTY(double residualVolumetricMoistureContentoftheSoilLayer READ residualVolumetricMoistureContentoftheSoilLayer WRITE setResidualVolumetricMoistureContentoftheSoilLayer RESET resetResidualVolumetricMoistureContentoftheSoilLayer);
    Q_PROPERTY(openstudio::Quantity residualVolumetricMoistureContentoftheSoilLayer_SI READ residualVolumetricMoistureContentoftheSoilLayer_SI WRITE setResidualVolumetricMoistureContentoftheSoilLayer RESET resetResidualVolumetricMoistureContentoftheSoilLayer);
    Q_PROPERTY(openstudio::Quantity residualVolumetricMoistureContentoftheSoilLayer_IP READ residualVolumetricMoistureContentoftheSoilLayer_IP WRITE setResidualVolumetricMoistureContentoftheSoilLayer RESET resetResidualVolumetricMoistureContentoftheSoilLayer);
    Q_PROPERTY(bool isResidualVolumetricMoistureContentoftheSoilLayerDefaulted READ isResidualVolumetricMoistureContentoftheSoilLayerDefaulted);

    Q_PROPERTY(double initialVolumetricMoistureContentoftheSoilLayer READ initialVolumetricMoistureContentoftheSoilLayer WRITE setInitialVolumetricMoistureContentoftheSoilLayer RESET resetInitialVolumetricMoistureContentoftheSoilLayer);
    Q_PROPERTY(openstudio::Quantity initialVolumetricMoistureContentoftheSoilLayer_SI READ initialVolumetricMoistureContentoftheSoilLayer_SI WRITE setInitialVolumetricMoistureContentoftheSoilLayer RESET resetInitialVolumetricMoistureContentoftheSoilLayer);
    Q_PROPERTY(openstudio::Quantity initialVolumetricMoistureContentoftheSoilLayer_IP READ initialVolumetricMoistureContentoftheSoilLayer_IP WRITE setInitialVolumetricMoistureContentoftheSoilLayer RESET resetInitialVolumetricMoistureContentoftheSoilLayer);
    Q_PROPERTY(bool isInitialVolumetricMoistureContentoftheSoilLayerDefaulted READ isInitialVolumetricMoistureContentoftheSoilLayerDefaulted);

    Q_PROPERTY(std::string moistureDiffusionCalculationMethod READ moistureDiffusionCalculationMethod WRITE setMoistureDiffusionCalculationMethod RESET resetMoistureDiffusionCalculationMethod);
    Q_PROPERTY(bool isMoistureDiffusionCalculationMethodDefaulted READ isMoistureDiffusionCalculationMethodDefaulted);
    Q_PROPERTY(std::vector<std::string> moistureDiffusionCalculationMethodValues READ moistureDiffusionCalculationMethodValues);

    // TODO: Add relationships for objects related to this one, but not pointed to by the underlying data.
    //       Such relationships can be generated by the GenerateRelationships.rb script.
   public:
    /** @name Constructors and Destructors */
    //@{

    RoofVegetation_Impl(const IdfObject& idfObject,
                        Model_Impl* model,
                        bool keepHandle);

    RoofVegetation_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                        Model_Impl* model,
                        bool keepHandle);

    RoofVegetation_Impl(const RoofVegetation_Impl& other,
                        Model_Impl* model,
                        bool keepHandle);

    virtual ~RoofVegetation_Impl() {}

    //@}

    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
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

    /** Get the thickness of the material. */
    virtual double thickness() const;

    /** The conductivitiy of the material in W/m*K. */
    virtual double thermalConductivity() const;

    /** The conductance of the material in W/m^2*K. */
    virtual double thermalConductance() const;

    /** The resistivity of the material in m*K/W. */
    virtual double thermalResistivity() const;

    /** The resistance of the material in m^2*K/W. */
    double thermalResistance() const;

    /** Density of dry soil (kg/m^3). */
    double density() const;

    /** Specific heat of dry soil (J/kg*K). */
    double specificHeat() const;

    virtual double thermalTransmittance() const;

    virtual double thermalAbsorptance() const;

    virtual boost::optional<double> thermalReflectance() const;

    virtual double solarTransmittance() const;

    virtual double solarAbsorptance() const;

    virtual boost::optional<double> solarReflectance() const;

    virtual double visibleTransmittance() const;

    virtual double visibleAbsorptance() const;

    virtual boost::optional<double> visibleReflectance() const;

    /** Maximum moisture content of the soil layer by volume fraction. */
    double saturationVolumetricMoistureContent() const;

    double residualVolumetricMoistureContent() const;

    double initialVolumetricMoistureContent() const;

    bool isRoughnessDefaulted() const;

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

    Quantity getThermalAbsorptance(bool returnIP=false) const;

    bool isThermalAbsorptanceDefaulted() const;

    Quantity getSolarAbsorptance(bool returnIP=false) const;

    bool isSolarAbsorptanceDefaulted() const;

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

    bool setHeightOfPlants(double value);

    bool setLeafAreaIndex(double value);

    bool setLeafReflectivity(double value);

    bool setLeafEmissivity(double value);

    bool setMinimumStomatalResistance(double value);

    bool setSoilLayerName(const std::string& name);

    bool setRoughness(const std::string& value);

    /** Set thickness to value (m). */
    virtual bool setThickness(double value);

    /** Sets the conductivity of the material in W/m*K, if possible. */
    virtual bool setThermalConductivity(double value);

    /** Sets the conductance of the material in W/m^2*K, if possible. */
    virtual bool setThermalConductance(double value);

    /** Sets the resistivity of the material in m*K/W, if possible. */
    virtual bool setThermalResistivity(double value);

    /** Sets the resistance of the material in m^2*K/W, if possible. */
    virtual bool setThermalResistance(double value);

    /** Set density of dry soil (kg/m^3). */
    bool setDensity(double value);

    /** Set specific heat of dry soil (J/kg*K). */
    bool setSpecificHeat(double value);

    virtual bool setThermalTransmittance(double value);

    virtual bool setThermalAbsorptance(boost::optional<double> value);

    virtual bool setThermalReflectance(boost::optional<double> value);

    virtual bool setSolarTransmittance(double value);

    virtual bool setSolarAbsorptance(boost::optional<double> value);

    virtual bool setSolarReflectance(boost::optional<double> value);

    virtual bool setVisibleTransmittance(double value);

    virtual bool setVisibleAbsorptance(boost::optional<double> value);

    virtual bool setVisibleReflectance(boost::optional<double> value);

    /** Set the maximum moisture content of the soil layer by volume fraction. */
    bool setSaturationVolumetricMoistureContent(double value);

    bool setResidualVolumetricMoistureContent(double value);

    bool setInitialVolumetricMoistureContent(double value);

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
    /** @name Queries */
    //@{

    /** Energy it takes to raise the temperature of this material 1 K (J/m^2*K). Equal to 
     *  specificHeat() * density() * thickness(). */
    virtual boost::optional<double> heatCapacity() const;

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.RoofVegetation");

    openstudio::Quantity heightofPlants_SI() const;
    openstudio::Quantity heightofPlants_IP() const;
    openstudio::Quantity leafAreaIndex_SI() const;
    openstudio::Quantity leafAreaIndex_IP() const;
    openstudio::Quantity leafReflectivity_SI() const;
    openstudio::Quantity leafReflectivity_IP() const;
    openstudio::Quantity leafEmissivity_SI() const;
    openstudio::Quantity leafEmissivity_IP() const;
    openstudio::Quantity minimumStomatalResistance_SI() const;
    openstudio::Quantity minimumStomatalResistance_IP() const;
    std::vector<std::string> roughnessValues() const;
    openstudio::Quantity thickness_SI() const;
    openstudio::Quantity thickness_IP() const;
    openstudio::Quantity conductivityofDrySoil_SI() const;
    openstudio::Quantity conductivityofDrySoil_IP() const;
    openstudio::Quantity densityofDrySoil_SI() const;
    openstudio::Quantity densityofDrySoil_IP() const;
    openstudio::Quantity specificHeatofDrySoil_SI() const;
    openstudio::Quantity specificHeatofDrySoil_IP() const;
    openstudio::Quantity thermalAbsorptance_SI() const;
    openstudio::Quantity thermalAbsorptance_IP() const;
    openstudio::Quantity solarAbsorptance_SI() const;
    openstudio::Quantity solarAbsorptance_IP() const;
    openstudio::Quantity visibleAbsorptance_SI() const;
    openstudio::Quantity visibleAbsorptance_IP() const;
    openstudio::Quantity saturationVolumetricMoistureContentoftheSoilLayer_SI() const;
    openstudio::Quantity saturationVolumetricMoistureContentoftheSoilLayer_IP() const;
    openstudio::Quantity residualVolumetricMoistureContentoftheSoilLayer_SI() const;
    openstudio::Quantity residualVolumetricMoistureContentoftheSoilLayer_IP() const;
    openstudio::Quantity initialVolumetricMoistureContentoftheSoilLayer_SI() const;
    openstudio::Quantity initialVolumetricMoistureContentoftheSoilLayer_IP() const;
    std::vector<std::string> moistureDiffusionCalculationMethodValues() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_ROOFVEGETATION_IMPL_HPP

