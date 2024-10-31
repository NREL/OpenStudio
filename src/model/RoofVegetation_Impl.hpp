/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ROOFVEGETATION_IMPL_HPP
#define MODEL_ROOFVEGETATION_IMPL_HPP

#include "ModelAPI.hpp"
#include "OpaqueMaterial_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** RoofVegetation_Impl is a OpaqueMaterial_Impl that is the implementation class for RoofVegetation.*/
    class MODEL_API RoofVegetation_Impl : public OpaqueMaterial_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      RoofVegetation_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      RoofVegetation_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      RoofVegetation_Impl(const RoofVegetation_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~RoofVegetation_Impl() override = default;

      //@}

      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
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

      /** Get the thickness of the material. */
      virtual double thickness() const override;

      /** The conductivity of the material in W/m*K. */
      virtual double thermalConductivity() const override;

      /** The conductance of the material in W/m^2*K. */
      virtual double thermalConductance() const override;

      /** The resistivity of the material in m*K/W. */
      virtual double thermalResistivity() const override;

      /** The resistance of the material in m^2*K/W. */
      double thermalResistance() const override;

      /** Density of dry soil (kg/m^3). */
      double density() const;

      /** Specific heat of dry soil (J/kg*K). */
      double specificHeat() const;

      virtual double thermalTransmittance() const;

      virtual double thermalAbsorptance() const override;

      virtual boost::optional<double> thermalReflectance() const override;

      virtual double solarTransmittance() const;

      virtual double solarAbsorptance() const override;

      virtual boost::optional<double> solarReflectance() const override;

      virtual double visibleTransmittance() const;

      virtual double visibleAbsorptance() const override;

      virtual boost::optional<double> visibleReflectance() const override;

      /** Maximum moisture content of the soil layer by volume fraction. */
      double saturationVolumetricMoistureContent() const;

      double residualVolumetricMoistureContent() const;

      double initialVolumetricMoistureContent() const;

      bool isRoughnessDefaulted() const;

      bool isThicknessDefaulted() const;

      double conductivityofDrySoil() const;

      bool isConductivityofDrySoilDefaulted() const;

      double densityofDrySoil() const;

      bool isDensityofDrySoilDefaulted() const;

      double specificHeatofDrySoil() const;

      bool isSpecificHeatofDrySoilDefaulted() const;

      bool isThermalAbsorptanceDefaulted() const;

      bool isSolarAbsorptanceDefaulted() const;

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

      bool setHeightOfPlants(double value);

      bool setLeafAreaIndex(double value);

      bool setLeafReflectivity(double value);

      bool setLeafEmissivity(double value);

      bool setMinimumStomatalResistance(double value);

      bool setSoilLayerName(const std::string& name);

      bool setRoughness(const std::string& value);

      /** Set thickness to value (m). */
      virtual bool setThickness(double value) override;

      /** Sets the conductivity of the material in W/m*K, if possible. */
      virtual bool setThermalConductivity(double value) override;

      /** Sets the conductance of the material in W/m^2*K, if possible. */
      virtual bool setThermalConductance(double value) override;

      /** Sets the resistivity of the material in m*K/W, if possible. */
      virtual bool setThermalResistivity(double value) override;

      /** Sets the resistance of the material in m^2*K/W, if possible. */
      virtual bool setThermalResistance(double value) override;

      /** Set density of dry soil (kg/m^3). */
      bool setDensity(double value);

      /** Set specific heat of dry soil (J/kg*K). */
      bool setSpecificHeat(double value);

      virtual bool setThermalTransmittance(double value);

      virtual bool setThermalAbsorptance(boost::optional<double> value);

      virtual bool setThermalReflectance(boost::optional<double> value) override;

      virtual bool setSolarTransmittance(double value);

      virtual bool setSolarAbsorptance(boost::optional<double> value) override;

      virtual bool setSolarReflectance(boost::optional<double> value) override;

      virtual bool setVisibleTransmittance(double value);

      virtual bool setVisibleAbsorptance(boost::optional<double> value) override;

      virtual bool setVisibleReflectance(boost::optional<double> value) override;

      /** Set the maximum moisture content of the soil layer by volume fraction. */
      bool setSaturationVolumetricMoistureContent(double value);

      bool setResidualVolumetricMoistureContent(double value);

      bool setInitialVolumetricMoistureContent(double value);

      bool setMoistureDiffusionCalculationMethod(const std::string& value);

      bool setHeightofPlants(double heightofPlants);

      void resetHeightofPlants();

      void resetLeafAreaIndex();

      void resetLeafReflectivity();

      void resetLeafEmissivity();

      void resetMinimumStomatalResistance();

      void resetSoilLayerName();

      void resetRoughness();

      void resetThickness();

      bool setConductivityofDrySoil(double conductivityofDrySoil);

      void resetConductivityofDrySoil();

      bool setDensityofDrySoil(double densityofDrySoil);

      void resetDensityofDrySoil();

      bool setSpecificHeatofDrySoil(double specificHeatofDrySoil);

      void resetSpecificHeatofDrySoil();

      bool setThermalAbsorptance(double thermalAbsorptance) override;

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
      /** @name Queries */
      //@{

      /** Energy it takes to raise the temperature of this material 1 K (J/m^2*K). Equal to
     *  specificHeat() * density() * thickness(). */
      virtual boost::optional<double> heatCapacity() const override;

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.RoofVegetation");

      std::vector<std::string> roughnessValues() const;
      std::vector<std::string> moistureDiffusionCalculationMethodValues() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ROOFVEGETATION_IMPL_HPP
