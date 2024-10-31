/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "RoofVegetation.hpp"
#include "RoofVegetation_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Material_RoofVegetation_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    RoofVegetation_Impl::RoofVegetation_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : OpaqueMaterial_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == RoofVegetation::iddObjectType());
    }

    RoofVegetation_Impl::RoofVegetation_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : OpaqueMaterial_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == RoofVegetation::iddObjectType());
    }

    RoofVegetation_Impl::RoofVegetation_Impl(const RoofVegetation_Impl& other, Model_Impl* model, bool keepHandle)
      : OpaqueMaterial_Impl(other, model, keepHandle) {}

    double RoofVegetation_Impl::leafAreaIndex() const {
      OptionalDouble od = getDouble(OS_Material_RoofVegetationFields::LeafAreaIndex, true);
      if (!od) {
        LOG_AND_THROW("Leaf area index is not yet set for " << briefDescription() << ".");
      }
      return *od;
    }

    double RoofVegetation_Impl::leafReflectivity() const {
      OptionalDouble od = getDouble(OS_Material_RoofVegetationFields::LeafReflectivity, true);
      if (!od) {
        LOG_AND_THROW("Leaf reflectivity is not yet set for " << briefDescription() << ".");
      }
      return *od;
    }

    double RoofVegetation_Impl::leafEmissivity() const {
      OptionalDouble od = getDouble(OS_Material_RoofVegetationFields::LeafEmissivity, true);
      if (!od) {
        LOG_AND_THROW("Leaf emissivity is not yet set for " << briefDescription() << ".");
      }
      return *od;
    }

    double RoofVegetation_Impl::minimumStomatalResistance() const {
      OptionalDouble od = getDouble(OS_Material_RoofVegetationFields::MinimumStomatalResistance, true);
      if (!od) {
        LOG_AND_THROW("Minimum stomatal resistance is not yet set for " << briefDescription() << ".");
      }
      return *od;
    }

    std::string RoofVegetation_Impl::soilLayerName() const {
      OptionalString os = getString(OS_Material_RoofVegetationFields::SoilLayerName, true);
      if (!os) {
        LOG_AND_THROW("Soil layer name is not yet set for " << briefDescription() << ".");
      }
      return *os;
    }

    std::string RoofVegetation_Impl::roughness() const {
      boost::optional<std::string> value = getString(OS_Material_RoofVegetationFields::Roughness, true);
      OS_ASSERT(value);
      return value.get();
    }

    double RoofVegetation_Impl::thickness() const {
      OptionalDouble od = getDouble(OS_Material_RoofVegetationFields::Thickness, true);
      if (!od) {
        LOG_AND_THROW("Thickness is not yet set for " << briefDescription() << ".");
      }
      return *od;
    }

    double RoofVegetation_Impl::thermalConductivity() const {
      OptionalDouble od = getDouble(OS_Material_RoofVegetationFields::ConductivityofDrySoil, true);
      if (!od) {
        LOG_AND_THROW("Conductivity (of dry soil) is not yet set for " << briefDescription() << ".");
      }
      return *od;
    }

    double RoofVegetation_Impl::thermalConductance() const {
      return thermalConductivity() / thickness();
    }

    double RoofVegetation_Impl::thermalResistivity() const {
      return 1.0 / thermalConductivity();
    }

    double RoofVegetation_Impl::thermalResistance() const {
      OS_ASSERT(thermalConductance());
      return 1.0 / thermalConductance();
    }

    double RoofVegetation_Impl::density() const {
      OptionalDouble od = getDouble(OS_Material_RoofVegetationFields::DensityofDrySoil, true);
      if (!od) {
        LOG_AND_THROW("Density (of dry soil) is not yet set for " << briefDescription() << ".");
      }
      return *od;
    }

    double RoofVegetation_Impl::specificHeat() const {
      OptionalDouble od = getDouble(OS_Material_RoofVegetationFields::SpecificHeatofDrySoil, true);
      if (!od) {
        LOG_AND_THROW("Specific heat (of dry soil) is not yet set for " << briefDescription() << ".");
      }
      return *od;
    }

    double RoofVegetation_Impl::thermalTransmittance() const {
      return 0.0;
    }

    double RoofVegetation_Impl::thermalAbsorptance() const {
      boost::optional<double> value = getDouble(OS_Material_RoofVegetationFields::ThermalAbsorptance, true);
      OS_ASSERT(value);
      return value.get();
    }

    OptionalDouble RoofVegetation_Impl::thermalReflectance() const {
      OptionalDouble result;
      result = 1.0 - thermalAbsorptance();
      return result;
    }

    double RoofVegetation_Impl::solarTransmittance() const {
      return 0.0;
    }

    OptionalDouble RoofVegetation_Impl::solarReflectance() const {
      OptionalDouble result;
      OptionalDouble od = solarAbsorptance();
      if (od) {
        result = (1.0 - *od);
      }
      return result;
    }

    double RoofVegetation_Impl::visibleTransmittance() const {
      return 0.0;
    }

    double RoofVegetation_Impl::visibleAbsorptance() const {
      boost::optional<double> value = getDouble(OS_Material_RoofVegetationFields::VisibleAbsorptance, true);
      OS_ASSERT(value);
      return value.get();
    }

    OptionalDouble RoofVegetation_Impl::visibleReflectance() const {
      OptionalDouble result;
      result = 1.0 - visibleAbsorptance();
      return result;
    }

    double RoofVegetation_Impl::saturationVolumetricMoistureContent() const {
      OptionalDouble od = getDouble(OS_Material_RoofVegetationFields::SaturationVolumetricMoistureContentoftheSoilLayer, true);
      if (!od) {
        LOG_AND_THROW("Volumetric moisture content of soil at saturation is not yet set for " << briefDescription() << ".");
      }
      return *od;
    }

    double RoofVegetation_Impl::residualVolumetricMoistureContent() const {
      OptionalDouble od = getDouble(OS_Material_RoofVegetationFields::ResidualVolumetricMoistureContentoftheSoilLayer, true);
      if (!od) {
        LOG_AND_THROW("Residual volumetric moisture content is not yet set for " << briefDescription() << ".");
      }
      return *od;
    }

    double RoofVegetation_Impl::initialVolumetricMoistureContent() const {
      OptionalDouble od = getDouble(OS_Material_RoofVegetationFields::InitialVolumetricMoistureContentoftheSoilLayer, true);
      if (!od) {
        LOG_AND_THROW("Initial volumetric moisture content is not yet set for " << briefDescription() << ".");
      }
      return *od;
    }

    std::string RoofVegetation_Impl::moistureDiffusionCalculationMethod() const {
      OptionalString os = getString(OS_Material_RoofVegetationFields::MoistureDiffusionCalculationMethod, true);
      if (!os) {
        LOG_AND_THROW("The moisture diffusion calculation method has not yet been set for " << briefDescription() << ".");
      }
      return *os;
    }

    const std::vector<std::string>& RoofVegetation_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Green Roof Soil Temperature",
                                                   "Green Roof Vegetation Temperature",
                                                   "Green Roof Soil Root Moisture Ratio",
                                                   "Green Roof Soil Near Surface Moisture Ratio",
                                                   "Green Roof Soil Sensible Heat Transfer Rate per Area",
                                                   "Green Roof Vegetation Sensible Heat Transfer Rate per Area",
                                                   "Green Roof Vegetation Moisture Transfer Rate",
                                                   "Green Roof Soil Moisture Transfer Rate",
                                                   "Green Roof Vegetation Latent Heat Transfer Rate per Area",
                                                   "Green Roof Soil Latent Heat Transfer Rate per Area",
                                                   "Green Roof Cumulative Precipitation Depth",
                                                   "Green Roof Cumulative Irrigation Depth",
                                                   "Green Roof Cumulative Runoff Depth",
                                                   "Green Roof Cumulative Evapotranspiration Depth",
                                                   "Green Roof Current Precipitation Depth",
                                                   "Green Roof Current Irrigation Depth",
                                                   "Green Roof Current Runoff Depth",
                                                   "Green Roof Current Evapotranspiration Depth"};
      return result;
    }

    IddObjectType RoofVegetation_Impl::iddObjectType() const {
      return RoofVegetation::iddObjectType();
    }

    double RoofVegetation_Impl::heightofPlants() const {
      boost::optional<double> value = getDouble(OS_Material_RoofVegetationFields::HeightofPlants, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool RoofVegetation_Impl::setLeafAreaIndex(double value) {
      return setDouble(OS_Material_RoofVegetationFields::LeafAreaIndex, value);
    }

    bool RoofVegetation_Impl::setLeafReflectivity(double value) {
      return setDouble(OS_Material_RoofVegetationFields::LeafReflectivity, value);
    }

    bool RoofVegetation_Impl::setLeafEmissivity(double value) {
      return setDouble(OS_Material_RoofVegetationFields::LeafEmissivity, value);
    }

    bool RoofVegetation_Impl::setMinimumStomatalResistance(double value) {
      return setDouble(OS_Material_RoofVegetationFields::MinimumStomatalResistance, value);
    }

    bool RoofVegetation_Impl::setSoilLayerName(const std::string& name) {
      return setString(OS_Material_RoofVegetationFields::SoilLayerName, name);
    }

    bool RoofVegetation_Impl::setRoughness(const std::string& value) {
      return setString(OS_Material_RoofVegetationFields::Roughness, value);
    }

    bool RoofVegetation_Impl::setThickness(double value) {
      return setDouble(OS_Material_RoofVegetationFields::Thickness, value);
    }

    bool RoofVegetation_Impl::setThermalConductivity(double value) {
      return setDouble(OS_Material_RoofVegetationFields::ConductivityofDrySoil, value);
    }

    bool RoofVegetation_Impl::setThermalConductance(double value) {
      // change thickness to achieve conductance
      return setThickness(thermalConductivity() / value);
    }

    bool RoofVegetation_Impl::setThermalResistivity(double value) {
      return setThermalConductivity(1.0 / value);
    }

    bool RoofVegetation_Impl::setThermalResistance(double value) {
      // change thickness to achieve resistance
      return setThickness(value / thermalResistivity());
    }

    bool RoofVegetation_Impl::setDensity(double value) {
      return setDouble(OS_Material_RoofVegetationFields::DensityofDrySoil, value);
    }

    bool RoofVegetation_Impl::setSpecificHeat(double value) {
      return setDouble(OS_Material_RoofVegetationFields::SpecificHeatofDrySoil, value);
    }

    bool RoofVegetation_Impl::setThermalTransmittance(double /*value*/) {
      return false;
    }

    bool RoofVegetation_Impl::setThermalAbsorptance(OptionalDouble value) {
      if (!value) {
        return setString(OS_Material_RoofVegetationFields::ThermalAbsorptance, "");
      }
      return setDouble(OS_Material_RoofVegetationFields::ThermalAbsorptance, *value);
    }

    bool RoofVegetation_Impl::setThermalReflectance(OptionalDouble value) {
      if (!value) {
        return setThermalAbsorptance(value);
      }
      OptionalDouble od = (1.0 - *value);
      return setThermalAbsorptance(od);
    }

    bool RoofVegetation_Impl::setSolarTransmittance(double /*value*/) {
      return false;
    }

    bool RoofVegetation_Impl::setSolarAbsorptance(OptionalDouble value) {
      if (!value) {
        return setString(OS_Material_RoofVegetationFields::SolarAbsorptance, "");
      }
      return setDouble(OS_Material_RoofVegetationFields::SolarAbsorptance, *value);
    }

    bool RoofVegetation_Impl::setSolarReflectance(OptionalDouble value) {
      if (!value) {
        return setSolarAbsorptance(value);
      }
      OptionalDouble od = (1.0 - *value);
      return setSolarAbsorptance(od);
    }

    bool RoofVegetation_Impl::setVisibleTransmittance(double /*value*/) {
      return false;
    }

    bool RoofVegetation_Impl::setVisibleAbsorptance(OptionalDouble value) {
      if (!value) {
        return setString(OS_Material_RoofVegetationFields::VisibleAbsorptance, "");
      }
      return setDouble(OS_Material_RoofVegetationFields::VisibleAbsorptance, *value);
    }

    bool RoofVegetation_Impl::setVisibleReflectance(OptionalDouble value) {
      if (!value) {
        return setVisibleAbsorptance(value);
      }
      OptionalDouble od = (1.0 - *value);
      return setVisibleAbsorptance(od);
    }

    bool RoofVegetation_Impl::setSaturationVolumetricMoistureContent(double value) {
      return setDouble(OS_Material_RoofVegetationFields::SaturationVolumetricMoistureContentoftheSoilLayer, value);
    }

    bool RoofVegetation_Impl::setResidualVolumetricMoistureContent(double value) {
      return setDouble(OS_Material_RoofVegetationFields::ResidualVolumetricMoistureContentoftheSoilLayer, value);
    }

    bool RoofVegetation_Impl::setInitialVolumetricMoistureContent(double value) {
      return setDouble(OS_Material_RoofVegetationFields::InitialVolumetricMoistureContentoftheSoilLayer, value);
    }

    bool RoofVegetation_Impl::setMoistureDiffusionCalculationMethod(const std::string& value) {
      return setString(OS_Material_RoofVegetationFields::MoistureDiffusionCalculationMethod, value);
    }

    boost::optional<double> RoofVegetation_Impl::heatCapacity() const {
      return specificHeat() * density() * thickness();
    }

    bool RoofVegetation_Impl::isHeightofPlantsDefaulted() const {
      return isEmpty(OS_Material_RoofVegetationFields::HeightofPlants);
    }

    bool RoofVegetation_Impl::isLeafAreaIndexDefaulted() const {
      return isEmpty(OS_Material_RoofVegetationFields::LeafAreaIndex);
    }

    bool RoofVegetation_Impl::isLeafReflectivityDefaulted() const {
      return isEmpty(OS_Material_RoofVegetationFields::LeafReflectivity);
    }

    bool RoofVegetation_Impl::isLeafEmissivityDefaulted() const {
      return isEmpty(OS_Material_RoofVegetationFields::LeafEmissivity);
    }

    bool RoofVegetation_Impl::isMinimumStomatalResistanceDefaulted() const {
      return isEmpty(OS_Material_RoofVegetationFields::MinimumStomatalResistance);
    }

    bool RoofVegetation_Impl::isSoilLayerNameDefaulted() const {
      return isEmpty(OS_Material_RoofVegetationFields::SoilLayerName);
    }

    bool RoofVegetation_Impl::isRoughnessDefaulted() const {
      return isEmpty(OS_Material_RoofVegetationFields::Roughness);
    }

    bool RoofVegetation_Impl::isThicknessDefaulted() const {
      return isEmpty(OS_Material_RoofVegetationFields::Thickness);
    }

    double RoofVegetation_Impl::conductivityofDrySoil() const {
      boost::optional<double> value = getDouble(OS_Material_RoofVegetationFields::ConductivityofDrySoil, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool RoofVegetation_Impl::isConductivityofDrySoilDefaulted() const {
      return isEmpty(OS_Material_RoofVegetationFields::ConductivityofDrySoil);
    }

    double RoofVegetation_Impl::densityofDrySoil() const {
      boost::optional<double> value = getDouble(OS_Material_RoofVegetationFields::DensityofDrySoil, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool RoofVegetation_Impl::isDensityofDrySoilDefaulted() const {
      return isEmpty(OS_Material_RoofVegetationFields::DensityofDrySoil);
    }

    double RoofVegetation_Impl::specificHeatofDrySoil() const {
      boost::optional<double> value = getDouble(OS_Material_RoofVegetationFields::SpecificHeatofDrySoil, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool RoofVegetation_Impl::isSpecificHeatofDrySoilDefaulted() const {
      return isEmpty(OS_Material_RoofVegetationFields::SpecificHeatofDrySoil);
    }

    bool RoofVegetation_Impl::isThermalAbsorptanceDefaulted() const {
      return isEmpty(OS_Material_RoofVegetationFields::ThermalAbsorptance);
    }

    double RoofVegetation_Impl::solarAbsorptance() const {
      boost::optional<double> value = getDouble(OS_Material_RoofVegetationFields::SolarAbsorptance, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool RoofVegetation_Impl::isSolarAbsorptanceDefaulted() const {
      return isEmpty(OS_Material_RoofVegetationFields::SolarAbsorptance);
    }

    bool RoofVegetation_Impl::isVisibleAbsorptanceDefaulted() const {
      return isEmpty(OS_Material_RoofVegetationFields::VisibleAbsorptance);
    }

    double RoofVegetation_Impl::saturationVolumetricMoistureContentoftheSoilLayer() const {
      boost::optional<double> value = getDouble(OS_Material_RoofVegetationFields::SaturationVolumetricMoistureContentoftheSoilLayer, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool RoofVegetation_Impl::isSaturationVolumetricMoistureContentoftheSoilLayerDefaulted() const {
      return isEmpty(OS_Material_RoofVegetationFields::SaturationVolumetricMoistureContentoftheSoilLayer);
    }

    double RoofVegetation_Impl::residualVolumetricMoistureContentoftheSoilLayer() const {
      boost::optional<double> value = getDouble(OS_Material_RoofVegetationFields::ResidualVolumetricMoistureContentoftheSoilLayer, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool RoofVegetation_Impl::isResidualVolumetricMoistureContentoftheSoilLayerDefaulted() const {
      return isEmpty(OS_Material_RoofVegetationFields::ResidualVolumetricMoistureContentoftheSoilLayer);
    }

    double RoofVegetation_Impl::initialVolumetricMoistureContentoftheSoilLayer() const {
      boost::optional<double> value = getDouble(OS_Material_RoofVegetationFields::InitialVolumetricMoistureContentoftheSoilLayer, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool RoofVegetation_Impl::isInitialVolumetricMoistureContentoftheSoilLayerDefaulted() const {
      return isEmpty(OS_Material_RoofVegetationFields::InitialVolumetricMoistureContentoftheSoilLayer);
    }

    bool RoofVegetation_Impl::isMoistureDiffusionCalculationMethodDefaulted() const {
      return isEmpty(OS_Material_RoofVegetationFields::MoistureDiffusionCalculationMethod);
    }

    bool RoofVegetation_Impl::setHeightofPlants(double heightofPlants) {
      bool result = setDouble(OS_Material_RoofVegetationFields::HeightofPlants, heightofPlants);
      return result;
    }

    void RoofVegetation_Impl::resetHeightofPlants() {
      bool result = setString(OS_Material_RoofVegetationFields::HeightofPlants, "");
      OS_ASSERT(result);
    }

    void RoofVegetation_Impl::resetLeafAreaIndex() {
      bool result = setString(OS_Material_RoofVegetationFields::LeafAreaIndex, "");
      OS_ASSERT(result);
    }

    void RoofVegetation_Impl::resetLeafReflectivity() {
      bool result = setString(OS_Material_RoofVegetationFields::LeafReflectivity, "");
      OS_ASSERT(result);
    }

    void RoofVegetation_Impl::resetLeafEmissivity() {
      bool result = setString(OS_Material_RoofVegetationFields::LeafEmissivity, "");
      OS_ASSERT(result);
    }

    void RoofVegetation_Impl::resetMinimumStomatalResistance() {
      bool result = setString(OS_Material_RoofVegetationFields::MinimumStomatalResistance, "");
      OS_ASSERT(result);
    }

    void RoofVegetation_Impl::resetSoilLayerName() {
      bool result = setString(OS_Material_RoofVegetationFields::SoilLayerName, "");
      OS_ASSERT(result);
    }

    void RoofVegetation_Impl::resetRoughness() {
      bool result = setString(OS_Material_RoofVegetationFields::Roughness, "");
      OS_ASSERT(result);
    }

    void RoofVegetation_Impl::resetThickness() {
      bool result = setString(OS_Material_RoofVegetationFields::Thickness, "");
      OS_ASSERT(result);
    }

    bool RoofVegetation_Impl::setConductivityofDrySoil(double conductivityofDrySoil) {
      bool result = setDouble(OS_Material_RoofVegetationFields::ConductivityofDrySoil, conductivityofDrySoil);
      return result;
    }

    void RoofVegetation_Impl::resetConductivityofDrySoil() {
      bool result = setString(OS_Material_RoofVegetationFields::ConductivityofDrySoil, "");
      OS_ASSERT(result);
    }

    bool RoofVegetation_Impl::setDensityofDrySoil(double densityofDrySoil) {
      bool result = setDouble(OS_Material_RoofVegetationFields::DensityofDrySoil, densityofDrySoil);
      return result;
    }

    void RoofVegetation_Impl::resetDensityofDrySoil() {
      bool result = setString(OS_Material_RoofVegetationFields::DensityofDrySoil, "");
      OS_ASSERT(result);
    }

    bool RoofVegetation_Impl::setSpecificHeatofDrySoil(double specificHeatofDrySoil) {
      bool result = setDouble(OS_Material_RoofVegetationFields::SpecificHeatofDrySoil, specificHeatofDrySoil);
      return result;
    }

    void RoofVegetation_Impl::resetSpecificHeatofDrySoil() {
      bool result = setString(OS_Material_RoofVegetationFields::SpecificHeatofDrySoil, "");
      OS_ASSERT(result);
    }

    bool RoofVegetation_Impl::setThermalAbsorptance(double thermalAbsorptance) {
      bool result = setDouble(OS_Material_RoofVegetationFields::ThermalAbsorptance, thermalAbsorptance);
      return result;
    }

    void RoofVegetation_Impl::resetThermalAbsorptance() {
      bool result = setString(OS_Material_RoofVegetationFields::ThermalAbsorptance, "");
      OS_ASSERT(result);
    }

    bool RoofVegetation_Impl::setSolarAbsorptance(double solarAbsorptance) {
      bool result = setDouble(OS_Material_RoofVegetationFields::SolarAbsorptance, solarAbsorptance);
      return result;
    }

    void RoofVegetation_Impl::resetSolarAbsorptance() {
      bool result = setString(OS_Material_RoofVegetationFields::SolarAbsorptance, "");
      OS_ASSERT(result);
    }

    bool RoofVegetation_Impl::setVisibleAbsorptance(double visibleAbsorptance) {
      bool result = setDouble(OS_Material_RoofVegetationFields::VisibleAbsorptance, visibleAbsorptance);
      return result;
    }

    void RoofVegetation_Impl::resetVisibleAbsorptance() {
      bool result = setString(OS_Material_RoofVegetationFields::VisibleAbsorptance, "");
      OS_ASSERT(result);
    }

    bool RoofVegetation_Impl::setSaturationVolumetricMoistureContentoftheSoilLayer(double saturationVolumetricMoistureContentoftheSoilLayer) {
      bool result = setDouble(OS_Material_RoofVegetationFields::SaturationVolumetricMoistureContentoftheSoilLayer,
                              saturationVolumetricMoistureContentoftheSoilLayer);
      return result;
    }

    void RoofVegetation_Impl::resetSaturationVolumetricMoistureContentoftheSoilLayer() {
      bool result = setString(OS_Material_RoofVegetationFields::SaturationVolumetricMoistureContentoftheSoilLayer, "");
      OS_ASSERT(result);
    }

    bool RoofVegetation_Impl::setResidualVolumetricMoistureContentoftheSoilLayer(double residualVolumetricMoistureContentoftheSoilLayer) {
      bool result =
        setDouble(OS_Material_RoofVegetationFields::ResidualVolumetricMoistureContentoftheSoilLayer, residualVolumetricMoistureContentoftheSoilLayer);
      return result;
    }

    void RoofVegetation_Impl::resetResidualVolumetricMoistureContentoftheSoilLayer() {
      bool result = setString(OS_Material_RoofVegetationFields::ResidualVolumetricMoistureContentoftheSoilLayer, "");
      OS_ASSERT(result);
    }

    bool RoofVegetation_Impl::setInitialVolumetricMoistureContentoftheSoilLayer(double initialVolumetricMoistureContentoftheSoilLayer) {
      bool result =
        setDouble(OS_Material_RoofVegetationFields::InitialVolumetricMoistureContentoftheSoilLayer, initialVolumetricMoistureContentoftheSoilLayer);
      return result;
    }

    void RoofVegetation_Impl::resetInitialVolumetricMoistureContentoftheSoilLayer() {
      bool result = setString(OS_Material_RoofVegetationFields::InitialVolumetricMoistureContentoftheSoilLayer, "");
      OS_ASSERT(result);
    }

    void RoofVegetation_Impl::resetMoistureDiffusionCalculationMethod() {
      bool result = setString(OS_Material_RoofVegetationFields::MoistureDiffusionCalculationMethod, "");
      OS_ASSERT(result);
    }

    std::vector<std::string> RoofVegetation_Impl::roughnessValues() const {
      return RoofVegetation::roughnessValues();
    }

    std::vector<std::string> RoofVegetation_Impl::moistureDiffusionCalculationMethodValues() const {
      return RoofVegetation::moistureDiffusionCalculationMethodValues();
    }

  }  // namespace detail

  RoofVegetation::RoofVegetation(const Model& model, const std::string& roughness) : OpaqueMaterial(RoofVegetation::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::RoofVegetation_Impl>());

    // TODO: Appropriately handle the following required object-list fields.
    bool ok = true;
    // ok = setHandle();
    OS_ASSERT(ok);
    ok = setRoughness(roughness);
    OS_ASSERT(ok);
  }

  IddObjectType RoofVegetation::iddObjectType() {
    return {IddObjectType::OS_Material_RoofVegetation};
  }

  std::vector<std::string> RoofVegetation::roughnessValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Material_RoofVegetationFields::Roughness);
  }

  std::vector<std::string> RoofVegetation::moistureDiffusionCalculationMethodValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_Material_RoofVegetationFields::MoistureDiffusionCalculationMethod);
  }

  double RoofVegetation::heightofPlants() const {
    return getImpl<detail::RoofVegetation_Impl>()->heightofPlants();
  }

  bool RoofVegetation::isHeightofPlantsDefaulted() const {
    return getImpl<detail::RoofVegetation_Impl>()->isHeightofPlantsDefaulted();
  }

  double RoofVegetation::leafAreaIndex() const {
    return getImpl<detail::RoofVegetation_Impl>()->leafAreaIndex();
  }

  bool RoofVegetation::isLeafAreaIndexDefaulted() const {
    return getImpl<detail::RoofVegetation_Impl>()->isLeafAreaIndexDefaulted();
  }

  double RoofVegetation::leafReflectivity() const {
    return getImpl<detail::RoofVegetation_Impl>()->leafReflectivity();
  }

  bool RoofVegetation::isLeafReflectivityDefaulted() const {
    return getImpl<detail::RoofVegetation_Impl>()->isLeafReflectivityDefaulted();
  }

  double RoofVegetation::leafEmissivity() const {
    return getImpl<detail::RoofVegetation_Impl>()->leafEmissivity();
  }

  bool RoofVegetation::isLeafEmissivityDefaulted() const {
    return getImpl<detail::RoofVegetation_Impl>()->isLeafEmissivityDefaulted();
  }

  double RoofVegetation::minimumStomatalResistance() const {
    return getImpl<detail::RoofVegetation_Impl>()->minimumStomatalResistance();
  }

  bool RoofVegetation::isMinimumStomatalResistanceDefaulted() const {
    return getImpl<detail::RoofVegetation_Impl>()->isMinimumStomatalResistanceDefaulted();
  }

  std::string RoofVegetation::soilLayerName() const {
    return getImpl<detail::RoofVegetation_Impl>()->soilLayerName();
  }

  bool RoofVegetation::isSoilLayerNameDefaulted() const {
    return getImpl<detail::RoofVegetation_Impl>()->isSoilLayerNameDefaulted();
  }

  std::string RoofVegetation::roughness() const {
    return getImpl<detail::RoofVegetation_Impl>()->roughness();
  }

  double RoofVegetation::density() const {
    return getImpl<detail::RoofVegetation_Impl>()->density();
  }

  double RoofVegetation::specificHeat() const {
    return getImpl<detail::RoofVegetation_Impl>()->specificHeat();
  }

  double RoofVegetation::saturationVolumetricMoistureContent() const {
    return getImpl<detail::RoofVegetation_Impl>()->saturationVolumetricMoistureContent();
  }

  double RoofVegetation::residualVolumetricMoistureContent() const {
    return getImpl<detail::RoofVegetation_Impl>()->residualVolumetricMoistureContent();
  }

  double RoofVegetation::initialVolumetricMoistureContent() const {
    return getImpl<detail::RoofVegetation_Impl>()->initialVolumetricMoistureContent();
  }

  bool RoofVegetation::isRoughnessDefaulted() const {
    return getImpl<detail::RoofVegetation_Impl>()->isRoughnessDefaulted();
  }

  // cppcheck-suppress [duplInheritedMember] for documentation purposes
  double RoofVegetation::thickness() const {
    return getImpl<detail::RoofVegetation_Impl>()->thickness();
  }

  bool RoofVegetation::isThicknessDefaulted() const {
    return getImpl<detail::RoofVegetation_Impl>()->isThicknessDefaulted();
  }

  double RoofVegetation::conductivityofDrySoil() const {
    return getImpl<detail::RoofVegetation_Impl>()->conductivityofDrySoil();
  }

  bool RoofVegetation::isConductivityofDrySoilDefaulted() const {
    return getImpl<detail::RoofVegetation_Impl>()->isConductivityofDrySoilDefaulted();
  }

  double RoofVegetation::densityofDrySoil() const {
    return getImpl<detail::RoofVegetation_Impl>()->densityofDrySoil();
  }

  bool RoofVegetation::isDensityofDrySoilDefaulted() const {
    return getImpl<detail::RoofVegetation_Impl>()->isDensityofDrySoilDefaulted();
  }

  double RoofVegetation::specificHeatofDrySoil() const {
    return getImpl<detail::RoofVegetation_Impl>()->specificHeatofDrySoil();
  }

  bool RoofVegetation::isSpecificHeatofDrySoilDefaulted() const {
    return getImpl<detail::RoofVegetation_Impl>()->isSpecificHeatofDrySoilDefaulted();
  }

  // cppcheck-suppress [duplInheritedMember] for documentation purposes
  OptionalDouble RoofVegetation::thermalAbsorptance() const {
    return getImpl<detail::RoofVegetation_Impl>()->thermalAbsorptance();
  }

  bool RoofVegetation::isThermalAbsorptanceDefaulted() const {
    return getImpl<detail::RoofVegetation_Impl>()->isThermalAbsorptanceDefaulted();
  }

  // cppcheck-suppress [duplInheritedMember] for documentation purposes
  OptionalDouble RoofVegetation::solarAbsorptance() const {
    return getImpl<detail::RoofVegetation_Impl>()->solarAbsorptance();
  }

  bool RoofVegetation::isSolarAbsorptanceDefaulted() const {
    return getImpl<detail::RoofVegetation_Impl>()->isSolarAbsorptanceDefaulted();
  }

  // cppcheck-suppress [duplInheritedMember] for documentation purposes
  OptionalDouble RoofVegetation::visibleAbsorptance() const {
    return getImpl<detail::RoofVegetation_Impl>()->visibleAbsorptance();
  }

  bool RoofVegetation::isVisibleAbsorptanceDefaulted() const {
    return getImpl<detail::RoofVegetation_Impl>()->isVisibleAbsorptanceDefaulted();
  }

  double RoofVegetation::saturationVolumetricMoistureContentoftheSoilLayer() const {
    return getImpl<detail::RoofVegetation_Impl>()->saturationVolumetricMoistureContentoftheSoilLayer();
  }

  bool RoofVegetation::isSaturationVolumetricMoistureContentoftheSoilLayerDefaulted() const {
    return getImpl<detail::RoofVegetation_Impl>()->isSaturationVolumetricMoistureContentoftheSoilLayerDefaulted();
  }

  double RoofVegetation::residualVolumetricMoistureContentoftheSoilLayer() const {
    return getImpl<detail::RoofVegetation_Impl>()->residualVolumetricMoistureContentoftheSoilLayer();
  }

  bool RoofVegetation::isResidualVolumetricMoistureContentoftheSoilLayerDefaulted() const {
    return getImpl<detail::RoofVegetation_Impl>()->isResidualVolumetricMoistureContentoftheSoilLayerDefaulted();
  }

  double RoofVegetation::initialVolumetricMoistureContentoftheSoilLayer() const {
    return getImpl<detail::RoofVegetation_Impl>()->initialVolumetricMoistureContentoftheSoilLayer();
  }

  bool RoofVegetation::isInitialVolumetricMoistureContentoftheSoilLayerDefaulted() const {
    return getImpl<detail::RoofVegetation_Impl>()->isInitialVolumetricMoistureContentoftheSoilLayerDefaulted();
  }

  std::string RoofVegetation::moistureDiffusionCalculationMethod() const {
    return getImpl<detail::RoofVegetation_Impl>()->moistureDiffusionCalculationMethod();
  }

  bool RoofVegetation::isMoistureDiffusionCalculationMethodDefaulted() const {
    return getImpl<detail::RoofVegetation_Impl>()->isMoistureDiffusionCalculationMethodDefaulted();
  }

  bool RoofVegetation::setHeightofPlants(double heightofPlants) {
    return getImpl<detail::RoofVegetation_Impl>()->setHeightofPlants(heightofPlants);
  }

  void RoofVegetation::resetHeightofPlants() {
    getImpl<detail::RoofVegetation_Impl>()->resetHeightofPlants();
  }

  bool RoofVegetation::setLeafAreaIndex(double leafAreaIndex) {
    return getImpl<detail::RoofVegetation_Impl>()->setLeafAreaIndex(leafAreaIndex);
  }

  void RoofVegetation::resetLeafAreaIndex() {
    getImpl<detail::RoofVegetation_Impl>()->resetLeafAreaIndex();
  }

  bool RoofVegetation::setLeafReflectivity(double leafReflectivity) {
    return getImpl<detail::RoofVegetation_Impl>()->setLeafReflectivity(leafReflectivity);
  }

  void RoofVegetation::resetLeafReflectivity() {
    getImpl<detail::RoofVegetation_Impl>()->resetLeafReflectivity();
  }

  bool RoofVegetation::setLeafEmissivity(double leafEmissivity) {
    return getImpl<detail::RoofVegetation_Impl>()->setLeafEmissivity(leafEmissivity);
  }

  void RoofVegetation::resetLeafEmissivity() {
    getImpl<detail::RoofVegetation_Impl>()->resetLeafEmissivity();
  }

  bool RoofVegetation::setMinimumStomatalResistance(double value) {
    return getImpl<detail::RoofVegetation_Impl>()->setMinimumStomatalResistance(value);
  }

  bool RoofVegetation::setSoilLayerName(const std::string& name) {
    return getImpl<detail::RoofVegetation_Impl>()->setSoilLayerName(name);
  }

  bool RoofVegetation::setRoughness(const std::string& value) {
    return getImpl<detail::RoofVegetation_Impl>()->setRoughness(value);
  }

  bool RoofVegetation::setDensity(double value) {
    return getImpl<detail::RoofVegetation_Impl>()->setDensity(value);
  }

  bool RoofVegetation::setSpecificHeat(double value) {
    return getImpl<detail::RoofVegetation_Impl>()->setSpecificHeat(value);
  }

  bool RoofVegetation::setSaturationVolumetricMoistureContent(double value) {
    return getImpl<detail::RoofVegetation_Impl>()->setSaturationVolumetricMoistureContent(value);
  }

  bool RoofVegetation::setResidualVolumetricMoistureContent(double value) {
    return getImpl<detail::RoofVegetation_Impl>()->setResidualVolumetricMoistureContent(value);
  }

  bool RoofVegetation::setInitialVolumetricMoistureConent(double value) {
    return getImpl<detail::RoofVegetation_Impl>()->setInitialVolumetricMoistureContent(value);
  }

  bool RoofVegetation::setMoistureDiffusionCalculationMethod(const std::string& value) {
    return getImpl<detail::RoofVegetation_Impl>()->setMoistureDiffusionCalculationMethod(value);
  }

  void RoofVegetation::resetMinimumStomatalResistance() {
    getImpl<detail::RoofVegetation_Impl>()->resetMinimumStomatalResistance();
  }

  void RoofVegetation::resetSoilLayerName() {
    getImpl<detail::RoofVegetation_Impl>()->resetSoilLayerName();
  }

  void RoofVegetation::resetRoughness() {
    getImpl<detail::RoofVegetation_Impl>()->resetRoughness();
  }

  // cppcheck-suppress [duplInheritedMember] for documentation purposes
  bool RoofVegetation::setThickness(double thickness) {
    return getImpl<detail::RoofVegetation_Impl>()->setThickness(thickness);
  }

  void RoofVegetation::resetThickness() {
    getImpl<detail::RoofVegetation_Impl>()->resetThickness();
  }

  bool RoofVegetation::setConductivityofDrySoil(double conductivityofDrySoil) {
    return getImpl<detail::RoofVegetation_Impl>()->setConductivityofDrySoil(conductivityofDrySoil);
  }

  void RoofVegetation::resetConductivityofDrySoil() {
    getImpl<detail::RoofVegetation_Impl>()->resetConductivityofDrySoil();
  }

  bool RoofVegetation::setDensityofDrySoil(double densityofDrySoil) {
    return getImpl<detail::RoofVegetation_Impl>()->setDensityofDrySoil(densityofDrySoil);
  }

  void RoofVegetation::resetDensityofDrySoil() {
    getImpl<detail::RoofVegetation_Impl>()->resetDensityofDrySoil();
  }

  bool RoofVegetation::setSpecificHeatofDrySoil(double specificHeatofDrySoil) {
    return getImpl<detail::RoofVegetation_Impl>()->setSpecificHeatofDrySoil(specificHeatofDrySoil);
  }

  void RoofVegetation::resetSpecificHeatofDrySoil() {
    getImpl<detail::RoofVegetation_Impl>()->resetSpecificHeatofDrySoil();
  }

  // cppcheck-suppress [duplInheritedMember] for documentation purposes
  bool RoofVegetation::setThermalAbsorptance(double thermalAbsorptance) {
    return getImpl<detail::RoofVegetation_Impl>()->setThermalAbsorptance(thermalAbsorptance);
  }

  void RoofVegetation::resetThermalAbsorptance() {
    getImpl<detail::RoofVegetation_Impl>()->resetThermalAbsorptance();
  }

  bool RoofVegetation::setSolarAbsorptance(double solarAbsorptance) {
    return getImpl<detail::RoofVegetation_Impl>()->setSolarAbsorptance(solarAbsorptance);
  }

  void RoofVegetation::resetSolarAbsorptance() {
    getImpl<detail::RoofVegetation_Impl>()->resetSolarAbsorptance();
  }

  bool RoofVegetation::setVisibleAbsorptance(double visibleAbsorptance) {
    return getImpl<detail::RoofVegetation_Impl>()->setVisibleAbsorptance(visibleAbsorptance);
  }

  void RoofVegetation::resetVisibleAbsorptance() {
    getImpl<detail::RoofVegetation_Impl>()->resetVisibleAbsorptance();
  }

  bool RoofVegetation::setSaturationVolumetricMoistureContentoftheSoilLayer(double saturationVolumetricMoistureContentoftheSoilLayer) {
    return getImpl<detail::RoofVegetation_Impl>()->setSaturationVolumetricMoistureContentoftheSoilLayer(
      saturationVolumetricMoistureContentoftheSoilLayer);
  }

  void RoofVegetation::resetSaturationVolumetricMoistureContentoftheSoilLayer() {
    getImpl<detail::RoofVegetation_Impl>()->resetSaturationVolumetricMoistureContentoftheSoilLayer();
  }

  bool RoofVegetation::setResidualVolumetricMoistureContentoftheSoilLayer(double residualVolumetricMoistureContentoftheSoilLayer) {
    return getImpl<detail::RoofVegetation_Impl>()->setResidualVolumetricMoistureContentoftheSoilLayer(
      residualVolumetricMoistureContentoftheSoilLayer);
  }

  void RoofVegetation::resetResidualVolumetricMoistureContentoftheSoilLayer() {
    getImpl<detail::RoofVegetation_Impl>()->resetResidualVolumetricMoistureContentoftheSoilLayer();
  }

  bool RoofVegetation::setInitialVolumetricMoistureContentoftheSoilLayer(double initialVolumetricMoistureContentoftheSoilLayer) {
    return getImpl<detail::RoofVegetation_Impl>()->setInitialVolumetricMoistureContentoftheSoilLayer(initialVolumetricMoistureContentoftheSoilLayer);
  }

  void RoofVegetation::resetInitialVolumetricMoistureContentoftheSoilLayer() {
    getImpl<detail::RoofVegetation_Impl>()->resetInitialVolumetricMoistureContentoftheSoilLayer();
  }

  void RoofVegetation::resetMoistureDiffusionCalculationMethod() {
    getImpl<detail::RoofVegetation_Impl>()->resetMoistureDiffusionCalculationMethod();
  }

  /// @cond
  RoofVegetation::RoofVegetation(std::shared_ptr<detail::RoofVegetation_Impl> impl) : OpaqueMaterial(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
