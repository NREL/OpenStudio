/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

  RoofVegetation_Impl::RoofVegetation_Impl(const IdfObject& idfObject,
                                           Model_Impl* model,
                                           bool keepHandle)
    : OpaqueMaterial_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == RoofVegetation::iddObjectType());
  }

  RoofVegetation_Impl::RoofVegetation_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
    : OpaqueMaterial_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == RoofVegetation::iddObjectType());
  }

  RoofVegetation_Impl::RoofVegetation_Impl(const RoofVegetation_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
    : OpaqueMaterial_Impl(other,model,keepHandle)
  {}

  double RoofVegetation_Impl::leafAreaIndex() const {
    OptionalDouble od = getDouble(OS_Material_RoofVegetationFields::LeafAreaIndex,true);
    if (!od) {
      LOG_AND_THROW("Leaf area index is not yet set for " << briefDescription() << ".");
    }
    return *od;
  }

  double RoofVegetation_Impl::leafReflectivity() const {
    OptionalDouble od = getDouble(OS_Material_RoofVegetationFields::LeafReflectivity,true);
    if (!od) {
      LOG_AND_THROW("Leaf reflectivity is not yet set for " << briefDescription() << ".");
    }
    return *od;
  }

  double RoofVegetation_Impl::leafEmissivity() const {
    OptionalDouble od = getDouble(OS_Material_RoofVegetationFields::LeafEmissivity,true);
    if (!od) {
      LOG_AND_THROW("Leaf emissivity is not yet set for " << briefDescription() << ".");
    }
    return *od;
  }

  double RoofVegetation_Impl::minimumStomatalResistance() const {
    OptionalDouble od = getDouble(OS_Material_RoofVegetationFields::MinimumStomatalResistance,true);
    if (!od) {
      LOG_AND_THROW("Minimum stomatal resistance is not yet set for " << briefDescription() << ".");
    }
    return *od;
  }

  std::string RoofVegetation_Impl::soilLayerName() const {
    OptionalString os = getString(OS_Material_RoofVegetationFields::SoilLayerName,true);
    if (!os) {
      LOG_AND_THROW("Soil layer name is not yet set for " << briefDescription() << ".");
    }
    return *os;
  }

  std::string RoofVegetation_Impl::roughness() const {
    boost::optional<std::string> value = getString(OS_Material_RoofVegetationFields::Roughness,true);
    OS_ASSERT(value);
    return value.get();
  }

  double RoofVegetation_Impl::thickness() const {
    OptionalDouble od = getDouble(OS_Material_RoofVegetationFields::Thickness,true);
    if (!od) { LOG_AND_THROW("Thickness is not yet set for " << briefDescription() << "."); }
    return *od;
  }

  double RoofVegetation_Impl::thermalConductivity() const {
    OptionalDouble od = getDouble(OS_Material_RoofVegetationFields::ConductivityofDrySoil,true);
    if (!od) {
      LOG_AND_THROW("Conductivity (of dry soil) is not yet set for " << briefDescription() << ".");
    }
    return *od;
  }

  double RoofVegetation_Impl::thermalConductance() const {
    return thermalConductivity()/thickness();
  }

  double RoofVegetation_Impl::thermalResistivity() const {
    return 1.0/thermalConductivity();
  }

  double RoofVegetation_Impl::thermalResistance() const {
    OS_ASSERT(thermalConductance());
    return 1.0/thermalConductance();
  }

  double RoofVegetation_Impl::density() const {
    OptionalDouble od = getDouble(OS_Material_RoofVegetationFields::DensityofDrySoil,true);
    if (!od) {
      LOG_AND_THROW("Density (of dry soil) is not yet set for " << briefDescription() << ".");
    }
    return *od;
  }

  double RoofVegetation_Impl::specificHeat() const {
    OptionalDouble od = getDouble(OS_Material_RoofVegetationFields::SpecificHeatofDrySoil,true);
    if (!od) {
      LOG_AND_THROW("Specific heat (of dry soil) is not yet set for " << briefDescription() << ".");
    }
    return *od;
  }

  double RoofVegetation_Impl::thermalTransmittance() const {
    return 0.0;
  }

  double RoofVegetation_Impl::thermalAbsorptance() const {
    boost::optional<double> value = getDouble(OS_Material_RoofVegetationFields::ThermalAbsorptance,true);
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
    if( od ) {
      result = (1.0 - *od);
    }
    return result;
  }

  double RoofVegetation_Impl::visibleTransmittance() const {
    return 0.0;
  }

  double RoofVegetation_Impl::visibleAbsorptance() const {
    boost::optional<double> value = getDouble(OS_Material_RoofVegetationFields::VisibleAbsorptance,true);
    OS_ASSERT(value);
    return value.get();
  }

  OptionalDouble RoofVegetation_Impl::visibleReflectance() const {
    OptionalDouble result;
    result = 1.0 - visibleAbsorptance();
    return result;
  }

  double RoofVegetation_Impl::saturationVolumetricMoistureContent() const {
    OptionalDouble od = getDouble(OS_Material_RoofVegetationFields::SaturationVolumetricMoistureContentoftheSoilLayer,true);
    if (!od) {
      LOG_AND_THROW("Volumetric moisture content of soil at saturation is not yet set for " << briefDescription() << ".");
    }
    return *od;
  }

  double RoofVegetation_Impl::residualVolumetricMoistureContent() const {
    OptionalDouble od = getDouble(OS_Material_RoofVegetationFields::ResidualVolumetricMoistureContentoftheSoilLayer,true);
    if (!od) {
      LOG_AND_THROW("Residual volumetric moisture content is not yet set for " << briefDescription() << ".");
    }
    return *od;
  }

  double RoofVegetation_Impl::initialVolumetricMoistureContent() const {
    OptionalDouble od = getDouble(OS_Material_RoofVegetationFields::InitialVolumetricMoistureContentoftheSoilLayer,true);
    if (!od) {
      LOG_AND_THROW("Initial volumetric moisture content is not yet set for " << briefDescription() << ".");
    }
    return *od;
  }

  std::string RoofVegetation_Impl::moistureDiffusionCalculationMethod() const {
    OptionalString os = getString(OS_Material_RoofVegetationFields::MoistureDiffusionCalculationMethod,true);
    if (!os) {
      LOG_AND_THROW("The moisture diffusion calculation method has not yet been set for " << briefDescription() << ".");
    }
    return *os;
  }

  const std::vector<std::string>& RoofVegetation_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType RoofVegetation_Impl::iddObjectType() const {
    return RoofVegetation::iddObjectType();
  }

  double RoofVegetation_Impl::heightofPlants() const {
    boost::optional<double> value = getDouble(OS_Material_RoofVegetationFields::HeightofPlants,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RoofVegetation_Impl::setLeafAreaIndex(double value) {
    return setDouble(OS_Material_RoofVegetationFields::LeafAreaIndex,value);
  }

  bool RoofVegetation_Impl::setLeafReflectivity(double value) {
    return setDouble(OS_Material_RoofVegetationFields::LeafReflectivity,value);
  }

  bool RoofVegetation_Impl::setLeafEmissivity(double value) {
    return setDouble(OS_Material_RoofVegetationFields::LeafEmissivity,value);
  }

  bool RoofVegetation_Impl::setMinimumStomatalResistance(double value) {
    return setDouble(OS_Material_RoofVegetationFields::MinimumStomatalResistance,value);
  }

  bool RoofVegetation_Impl::setSoilLayerName(const std::string& name) {
    return setString(OS_Material_RoofVegetationFields::SoilLayerName,name);
  }

  bool RoofVegetation_Impl::setRoughness(const std::string& value) {
    return setString(OS_Material_RoofVegetationFields::Roughness,value);
  }

  bool RoofVegetation_Impl::setThickness(double value) {
    return setDouble(OS_Material_RoofVegetationFields::Thickness,value);
  }

  bool RoofVegetation_Impl::setThermalConductivity(double value) {
    return setDouble(OS_Material_RoofVegetationFields::ConductivityofDrySoil,value);
  }

  bool RoofVegetation_Impl::setThermalConductance(double value) {
    // change thickness to achieve conductance
    return setThickness(thermalConductivity()/value);
  }

  bool RoofVegetation_Impl::setThermalResistivity(double value) {
    return setThermalConductivity(1.0/value);
  }

  bool RoofVegetation_Impl::setThermalResistance(double value) {
    // change thickness to achieve resistance
    return setThickness(value/thermalResistivity());
  }

  bool RoofVegetation_Impl::setDensity(double value) {
    return setDouble(OS_Material_RoofVegetationFields::DensityofDrySoil,value);
  }

  bool RoofVegetation_Impl::setSpecificHeat(double value) {
    return setDouble(OS_Material_RoofVegetationFields::SpecificHeatofDrySoil,value);
  }

  bool RoofVegetation_Impl::setThermalTransmittance(double value) {
    return false;
  }

  bool RoofVegetation_Impl::setThermalAbsorptance(OptionalDouble value) {
    if( !value ) {
      return setString(OS_Material_RoofVegetationFields::ThermalAbsorptance, "");
    }
    return setDouble(OS_Material_RoofVegetationFields::ThermalAbsorptance, *value);
  }

  bool RoofVegetation_Impl::setThermalReflectance(OptionalDouble value) {
    if( !value ) {
      return setThermalAbsorptance(value);
    }
    OptionalDouble od = (1.0 - *value);
    return setThermalAbsorptance(od);
  }

  bool RoofVegetation_Impl::setSolarTransmittance(double value) {
    return false;
  }

  bool RoofVegetation_Impl::setSolarAbsorptance(OptionalDouble value) {
    if( !value ) {
      return setString(OS_Material_RoofVegetationFields::SolarAbsorptance, "");
    }
    return setDouble(OS_Material_RoofVegetationFields::SolarAbsorptance, *value);
  }

  bool RoofVegetation_Impl::setSolarReflectance(OptionalDouble value) {
    if( !value ) {
      return setSolarAbsorptance(value);
    }
    OptionalDouble od = (1.0 - *value);
    return setSolarAbsorptance(od);
  }

  bool RoofVegetation_Impl::setVisibleTransmittance(double value) {
    return false;
  }

  bool RoofVegetation_Impl::setVisibleAbsorptance(OptionalDouble value) {
    if( !value ) {
      return setString(OS_Material_RoofVegetationFields::VisibleAbsorptance, "");
    }
    return setDouble(OS_Material_RoofVegetationFields::VisibleAbsorptance, *value);
  }

  bool RoofVegetation_Impl::setVisibleReflectance(OptionalDouble value) {
    if( !value ) {
      return setVisibleAbsorptance(value);
    }
    OptionalDouble od = (1.0 - *value);
    return setVisibleAbsorptance(od);
  }

  bool RoofVegetation_Impl::setSaturationVolumetricMoistureContent(double value) {
    return setDouble(OS_Material_RoofVegetationFields::SaturationVolumetricMoistureContentoftheSoilLayer,value);
  }

  bool RoofVegetation_Impl::setResidualVolumetricMoistureContent(double value) {
    return setDouble(OS_Material_RoofVegetationFields::ResidualVolumetricMoistureContentoftheSoilLayer,value);
  }

  bool RoofVegetation_Impl::setInitialVolumetricMoistureContent(double value) {
    return setDouble(OS_Material_RoofVegetationFields::InitialVolumetricMoistureContentoftheSoilLayer,value);
  }

  bool RoofVegetation_Impl::setMoistureDiffusionCalculationMethod(const std::string& value) {
    return setString(OS_Material_RoofVegetationFields::MoistureDiffusionCalculationMethod,value);
  }

  boost::optional<double> RoofVegetation_Impl::heatCapacity() const {
    return specificHeat() * density() * thickness();
  }

  Quantity RoofVegetation_Impl::getHeightofPlants(bool returnIP) const {
    OptionalDouble value = heightofPlants();
    OSOptionalQuantity result = getQuantityFromDouble(OS_Material_RoofVegetationFields::HeightofPlants, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool RoofVegetation_Impl::isHeightofPlantsDefaulted() const {
    return isEmpty(OS_Material_RoofVegetationFields::HeightofPlants);
  }

  Quantity RoofVegetation_Impl::getLeafAreaIndex(bool returnIP) const {
    OptionalDouble value = leafAreaIndex();
    OSOptionalQuantity result = getQuantityFromDouble(OS_Material_RoofVegetationFields::LeafAreaIndex, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool RoofVegetation_Impl::isLeafAreaIndexDefaulted() const {
    return isEmpty(OS_Material_RoofVegetationFields::LeafAreaIndex);
  }

  Quantity RoofVegetation_Impl::getLeafReflectivity(bool returnIP) const {
    OptionalDouble value = leafReflectivity();
    OSOptionalQuantity result = getQuantityFromDouble(OS_Material_RoofVegetationFields::LeafReflectivity, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool RoofVegetation_Impl::isLeafReflectivityDefaulted() const {
    return isEmpty(OS_Material_RoofVegetationFields::LeafReflectivity);
  }

  Quantity RoofVegetation_Impl::getLeafEmissivity(bool returnIP) const {
    OptionalDouble value = leafEmissivity();
    OSOptionalQuantity result = getQuantityFromDouble(OS_Material_RoofVegetationFields::LeafEmissivity, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool RoofVegetation_Impl::isLeafEmissivityDefaulted() const {
    return isEmpty(OS_Material_RoofVegetationFields::LeafEmissivity);
  }

  Quantity RoofVegetation_Impl::getMinimumStomatalResistance(bool returnIP) const {
    OptionalDouble value = minimumStomatalResistance();
    OSOptionalQuantity result = getQuantityFromDouble(OS_Material_RoofVegetationFields::MinimumStomatalResistance, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
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

  Quantity RoofVegetation_Impl::getThickness(bool returnIP) const {
    OptionalDouble value = thickness();
    OSOptionalQuantity result = getQuantityFromDouble(OS_Material_RoofVegetationFields::Thickness, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool RoofVegetation_Impl::isThicknessDefaulted() const {
    return isEmpty(OS_Material_RoofVegetationFields::Thickness);
  }

  double RoofVegetation_Impl::conductivityofDrySoil() const {
    boost::optional<double> value = getDouble(OS_Material_RoofVegetationFields::ConductivityofDrySoil,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity RoofVegetation_Impl::getConductivityofDrySoil(bool returnIP) const {
    OptionalDouble value = conductivityofDrySoil();
    OSOptionalQuantity result = getQuantityFromDouble(OS_Material_RoofVegetationFields::ConductivityofDrySoil, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool RoofVegetation_Impl::isConductivityofDrySoilDefaulted() const {
    return isEmpty(OS_Material_RoofVegetationFields::ConductivityofDrySoil);
  }

  double RoofVegetation_Impl::densityofDrySoil() const {
    boost::optional<double> value = getDouble(OS_Material_RoofVegetationFields::DensityofDrySoil,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity RoofVegetation_Impl::getDensityofDrySoil(bool returnIP) const {
    OptionalDouble value = densityofDrySoil();
    OSOptionalQuantity result = getQuantityFromDouble(OS_Material_RoofVegetationFields::DensityofDrySoil, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool RoofVegetation_Impl::isDensityofDrySoilDefaulted() const {
    return isEmpty(OS_Material_RoofVegetationFields::DensityofDrySoil);
  }

  double RoofVegetation_Impl::specificHeatofDrySoil() const {
    boost::optional<double> value = getDouble(OS_Material_RoofVegetationFields::SpecificHeatofDrySoil,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity RoofVegetation_Impl::getSpecificHeatofDrySoil(bool returnIP) const {
    OptionalDouble value = specificHeatofDrySoil();
    OSOptionalQuantity result = getQuantityFromDouble(OS_Material_RoofVegetationFields::SpecificHeatofDrySoil, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool RoofVegetation_Impl::isSpecificHeatofDrySoilDefaulted() const {
    return isEmpty(OS_Material_RoofVegetationFields::SpecificHeatofDrySoil);
  }

  Quantity RoofVegetation_Impl::getThermalAbsorptance(bool returnIP) const {
    OptionalDouble value(thermalAbsorptance());
    OSOptionalQuantity result = getQuantityFromDouble(OS_Material_RoofVegetationFields::ThermalAbsorptance, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool RoofVegetation_Impl::isThermalAbsorptanceDefaulted() const {
    return isEmpty(OS_Material_RoofVegetationFields::ThermalAbsorptance);
  }

  double RoofVegetation_Impl::solarAbsorptance() const {
    boost::optional<double> value = getDouble(OS_Material_RoofVegetationFields::SolarAbsorptance,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity RoofVegetation_Impl::getSolarAbsorptance(bool returnIP) const {
    OptionalDouble value = solarAbsorptance();
    OSOptionalQuantity result = getQuantityFromDouble(OS_Material_RoofVegetationFields::SolarAbsorptance, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool RoofVegetation_Impl::isSolarAbsorptanceDefaulted() const {
    return isEmpty(OS_Material_RoofVegetationFields::SolarAbsorptance);
  }

  Quantity RoofVegetation_Impl::getVisibleAbsorptance(bool returnIP) const {
    OptionalDouble value = visibleAbsorptance();
    OSOptionalQuantity result = getQuantityFromDouble(OS_Material_RoofVegetationFields::VisibleAbsorptance, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool RoofVegetation_Impl::isVisibleAbsorptanceDefaulted() const {
    return isEmpty(OS_Material_RoofVegetationFields::VisibleAbsorptance);
  }

  double RoofVegetation_Impl::saturationVolumetricMoistureContentoftheSoilLayer() const {
    boost::optional<double> value = getDouble(OS_Material_RoofVegetationFields::SaturationVolumetricMoistureContentoftheSoilLayer,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity RoofVegetation_Impl::getSaturationVolumetricMoistureContentoftheSoilLayer(bool returnIP) const {
    OptionalDouble value = saturationVolumetricMoistureContentoftheSoilLayer();
    OSOptionalQuantity result = getQuantityFromDouble(OS_Material_RoofVegetationFields::SaturationVolumetricMoistureContentoftheSoilLayer, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool RoofVegetation_Impl::isSaturationVolumetricMoistureContentoftheSoilLayerDefaulted() const {
    return isEmpty(OS_Material_RoofVegetationFields::SaturationVolumetricMoistureContentoftheSoilLayer);
  }

  double RoofVegetation_Impl::residualVolumetricMoistureContentoftheSoilLayer() const {
    boost::optional<double> value = getDouble(OS_Material_RoofVegetationFields::ResidualVolumetricMoistureContentoftheSoilLayer,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity RoofVegetation_Impl::getResidualVolumetricMoistureContentoftheSoilLayer(bool returnIP) const {
    OptionalDouble value = residualVolumetricMoistureContentoftheSoilLayer();
    OSOptionalQuantity result = getQuantityFromDouble(OS_Material_RoofVegetationFields::ResidualVolumetricMoistureContentoftheSoilLayer, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool RoofVegetation_Impl::isResidualVolumetricMoistureContentoftheSoilLayerDefaulted() const {
    return isEmpty(OS_Material_RoofVegetationFields::ResidualVolumetricMoistureContentoftheSoilLayer);
  }

  double RoofVegetation_Impl::initialVolumetricMoistureContentoftheSoilLayer() const {
    boost::optional<double> value = getDouble(OS_Material_RoofVegetationFields::InitialVolumetricMoistureContentoftheSoilLayer,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity RoofVegetation_Impl::getInitialVolumetricMoistureContentoftheSoilLayer(bool returnIP) const {
    OptionalDouble value = initialVolumetricMoistureContentoftheSoilLayer();
    OSOptionalQuantity result = getQuantityFromDouble(OS_Material_RoofVegetationFields::InitialVolumetricMoistureContentoftheSoilLayer, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
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

  bool RoofVegetation_Impl::setHeightofPlants(const Quantity& heightofPlants) {
    OptionalDouble value = getDoubleFromQuantity(OS_Material_RoofVegetationFields::HeightofPlants,heightofPlants);
    if (!value) {
      return false;
    }
    return setHeightofPlants(value.get());
  }

  void RoofVegetation_Impl::resetHeightofPlants() {
    bool result = setString(OS_Material_RoofVegetationFields::HeightofPlants, "");
    OS_ASSERT(result);
  }

  bool RoofVegetation_Impl::setLeafAreaIndex(const Quantity& leafAreaIndex) {
    OptionalDouble value = getDoubleFromQuantity(OS_Material_RoofVegetationFields::LeafAreaIndex,leafAreaIndex);
    if (!value) {
      return false;
    }
    return setLeafAreaIndex(value.get());
  }

  void RoofVegetation_Impl::resetLeafAreaIndex() {
    bool result = setString(OS_Material_RoofVegetationFields::LeafAreaIndex, "");
    OS_ASSERT(result);
  }

  bool RoofVegetation_Impl::setLeafReflectivity(const Quantity& leafReflectivity) {
    OptionalDouble value = getDoubleFromQuantity(OS_Material_RoofVegetationFields::LeafReflectivity,leafReflectivity);
    if (!value) {
      return false;
    }
    return setLeafReflectivity(value.get());
  }

  void RoofVegetation_Impl::resetLeafReflectivity() {
    bool result = setString(OS_Material_RoofVegetationFields::LeafReflectivity, "");
    OS_ASSERT(result);
  }

  bool RoofVegetation_Impl::setLeafEmissivity(const Quantity& leafEmissivity) {
    OptionalDouble value = getDoubleFromQuantity(OS_Material_RoofVegetationFields::LeafEmissivity,leafEmissivity);
    if (!value) {
      return false;
    }
    return setLeafEmissivity(value.get());
  }

  void RoofVegetation_Impl::resetLeafEmissivity() {
    bool result = setString(OS_Material_RoofVegetationFields::LeafEmissivity, "");
    OS_ASSERT(result);
  }

  bool RoofVegetation_Impl::setMinimumStomatalResistance(const Quantity& minimumStomatalResistance) {
    OptionalDouble value = getDoubleFromQuantity(OS_Material_RoofVegetationFields::MinimumStomatalResistance,minimumStomatalResistance);
    if (!value) {
      return false;
    }
    return setMinimumStomatalResistance(value.get());
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

  bool RoofVegetation_Impl::setThickness(const Quantity& thickness) {
    OptionalDouble value = getDoubleFromQuantity(OS_Material_RoofVegetationFields::Thickness,thickness);
    if (!value) {
      return false;
    }
    return setThickness(value.get());
  }

  void RoofVegetation_Impl::resetThickness() {
    bool result = setString(OS_Material_RoofVegetationFields::Thickness, "");
    OS_ASSERT(result);
  }

  bool RoofVegetation_Impl::setConductivityofDrySoil(double conductivityofDrySoil) {
    bool result = setDouble(OS_Material_RoofVegetationFields::ConductivityofDrySoil, conductivityofDrySoil);
    return result;
  }

  bool RoofVegetation_Impl::setConductivityofDrySoil(const Quantity& conductivityofDrySoil) {
    OptionalDouble value = getDoubleFromQuantity(OS_Material_RoofVegetationFields::ConductivityofDrySoil,conductivityofDrySoil);
    if (!value) {
      return false;
    }
    return setConductivityofDrySoil(value.get());
  }

  void RoofVegetation_Impl::resetConductivityofDrySoil() {
    bool result = setString(OS_Material_RoofVegetationFields::ConductivityofDrySoil, "");
    OS_ASSERT(result);
  }

  bool RoofVegetation_Impl::setDensityofDrySoil(double densityofDrySoil) {
    bool result = setDouble(OS_Material_RoofVegetationFields::DensityofDrySoil, densityofDrySoil);
    return result;
  }

  bool RoofVegetation_Impl::setDensityofDrySoil(const Quantity& densityofDrySoil) {
    OptionalDouble value = getDoubleFromQuantity(OS_Material_RoofVegetationFields::DensityofDrySoil,densityofDrySoil);
    if (!value) {
      return false;
    }
    return setDensityofDrySoil(value.get());
  }

  void RoofVegetation_Impl::resetDensityofDrySoil() {
    bool result = setString(OS_Material_RoofVegetationFields::DensityofDrySoil, "");
    OS_ASSERT(result);
  }

  bool RoofVegetation_Impl::setSpecificHeatofDrySoil(double specificHeatofDrySoil) {
    bool result = setDouble(OS_Material_RoofVegetationFields::SpecificHeatofDrySoil, specificHeatofDrySoil);
    return result;
  }

  bool RoofVegetation_Impl::setSpecificHeatofDrySoil(const Quantity& specificHeatofDrySoil) {
    OptionalDouble value = getDoubleFromQuantity(OS_Material_RoofVegetationFields::SpecificHeatofDrySoil,specificHeatofDrySoil);
    if (!value) {
      return false;
    }
    return setSpecificHeatofDrySoil(value.get());
  }

  void RoofVegetation_Impl::resetSpecificHeatofDrySoil() {
    bool result = setString(OS_Material_RoofVegetationFields::SpecificHeatofDrySoil, "");
    OS_ASSERT(result);
  }

  bool RoofVegetation_Impl::setThermalAbsorptance(double thermalAbsorptance) {
    bool result = setDouble(OS_Material_RoofVegetationFields::ThermalAbsorptance, thermalAbsorptance);
    return result;
  }

  bool RoofVegetation_Impl::setThermalAbsorptance(const Quantity& thermalAbsorptance) {
    OptionalDouble value = getDoubleFromQuantity(OS_Material_RoofVegetationFields::ThermalAbsorptance,thermalAbsorptance);
    if (!value) {
      return false;
    }
    return setThermalAbsorptance(value.get());
  }

  void RoofVegetation_Impl::resetThermalAbsorptance() {
    bool result = setString(OS_Material_RoofVegetationFields::ThermalAbsorptance, "");
    OS_ASSERT(result);
  }

  bool RoofVegetation_Impl::setSolarAbsorptance(double solarAbsorptance) {
    bool result = setDouble(OS_Material_RoofVegetationFields::SolarAbsorptance, solarAbsorptance);
    return result;
  }

  bool RoofVegetation_Impl::setSolarAbsorptance(const Quantity& solarAbsorptance) {
    OptionalDouble value = getDoubleFromQuantity(OS_Material_RoofVegetationFields::SolarAbsorptance,solarAbsorptance);
    if (!value) {
      return false;
    }
    return setSolarAbsorptance(value.get());
  }

  void RoofVegetation_Impl::resetSolarAbsorptance() {
    bool result = setString(OS_Material_RoofVegetationFields::SolarAbsorptance, "");
    OS_ASSERT(result);
  }

  bool RoofVegetation_Impl::setVisibleAbsorptance(double visibleAbsorptance) {
    bool result = setDouble(OS_Material_RoofVegetationFields::VisibleAbsorptance, visibleAbsorptance);
    return result;
  }

  bool RoofVegetation_Impl::setVisibleAbsorptance(const Quantity& visibleAbsorptance) {
    OptionalDouble value = getDoubleFromQuantity(OS_Material_RoofVegetationFields::VisibleAbsorptance,visibleAbsorptance);
    if (!value) {
      return false;
    }
    return setVisibleAbsorptance(value.get());
  }

  void RoofVegetation_Impl::resetVisibleAbsorptance() {
    bool result = setString(OS_Material_RoofVegetationFields::VisibleAbsorptance, "");
    OS_ASSERT(result);
  }

  bool RoofVegetation_Impl::setSaturationVolumetricMoistureContentoftheSoilLayer(double saturationVolumetricMoistureContentoftheSoilLayer) {
    bool result = setDouble(OS_Material_RoofVegetationFields::SaturationVolumetricMoistureContentoftheSoilLayer, saturationVolumetricMoistureContentoftheSoilLayer);
    return result;
  }

  bool RoofVegetation_Impl::setSaturationVolumetricMoistureContentoftheSoilLayer(const Quantity& saturationVolumetricMoistureContentoftheSoilLayer) {
    OptionalDouble value = getDoubleFromQuantity(OS_Material_RoofVegetationFields::SaturationVolumetricMoistureContentoftheSoilLayer,saturationVolumetricMoistureContentoftheSoilLayer);
    if (!value) {
      return false;
    }
    return setSaturationVolumetricMoistureContentoftheSoilLayer(value.get());
  }

  void RoofVegetation_Impl::resetSaturationVolumetricMoistureContentoftheSoilLayer() {
    bool result = setString(OS_Material_RoofVegetationFields::SaturationVolumetricMoistureContentoftheSoilLayer, "");
    OS_ASSERT(result);
  }

  bool RoofVegetation_Impl::setResidualVolumetricMoistureContentoftheSoilLayer(double residualVolumetricMoistureContentoftheSoilLayer) {
    bool result = setDouble(OS_Material_RoofVegetationFields::ResidualVolumetricMoistureContentoftheSoilLayer, residualVolumetricMoistureContentoftheSoilLayer);
    return result;
  }

  bool RoofVegetation_Impl::setResidualVolumetricMoistureContentoftheSoilLayer(const Quantity& residualVolumetricMoistureContentoftheSoilLayer) {
    OptionalDouble value = getDoubleFromQuantity(OS_Material_RoofVegetationFields::ResidualVolumetricMoistureContentoftheSoilLayer,residualVolumetricMoistureContentoftheSoilLayer);
    if (!value) {
      return false;
    }
    return setResidualVolumetricMoistureContentoftheSoilLayer(value.get());
  }

  void RoofVegetation_Impl::resetResidualVolumetricMoistureContentoftheSoilLayer() {
    bool result = setString(OS_Material_RoofVegetationFields::ResidualVolumetricMoistureContentoftheSoilLayer, "");
    OS_ASSERT(result);
  }

  bool RoofVegetation_Impl::setInitialVolumetricMoistureContentoftheSoilLayer(double initialVolumetricMoistureContentoftheSoilLayer) {
    bool result = setDouble(OS_Material_RoofVegetationFields::InitialVolumetricMoistureContentoftheSoilLayer, initialVolumetricMoistureContentoftheSoilLayer);
    return result;
  }

  bool RoofVegetation_Impl::setInitialVolumetricMoistureContentoftheSoilLayer(const Quantity& initialVolumetricMoistureContentoftheSoilLayer) {
    OptionalDouble value = getDoubleFromQuantity(OS_Material_RoofVegetationFields::InitialVolumetricMoistureContentoftheSoilLayer,initialVolumetricMoistureContentoftheSoilLayer);
    if (!value) {
      return false;
    }
    return setInitialVolumetricMoistureContentoftheSoilLayer(value.get());
  }

  void RoofVegetation_Impl::resetInitialVolumetricMoistureContentoftheSoilLayer() {
    bool result = setString(OS_Material_RoofVegetationFields::InitialVolumetricMoistureContentoftheSoilLayer, "");
    OS_ASSERT(result);
  }

  void RoofVegetation_Impl::resetMoistureDiffusionCalculationMethod() {
    bool result = setString(OS_Material_RoofVegetationFields::MoistureDiffusionCalculationMethod, "");
    OS_ASSERT(result);
  }

  openstudio::Quantity RoofVegetation_Impl::heightofPlants_SI() const {
    return getHeightofPlants(false);
  }

  openstudio::Quantity RoofVegetation_Impl::heightofPlants_IP() const {
    return getHeightofPlants(true);
  }

  openstudio::Quantity RoofVegetation_Impl::leafAreaIndex_SI() const {
    return getLeafAreaIndex(false);
  }

  openstudio::Quantity RoofVegetation_Impl::leafAreaIndex_IP() const {
    return getLeafAreaIndex(true);
  }

  openstudio::Quantity RoofVegetation_Impl::leafReflectivity_SI() const {
    return getLeafReflectivity(false);
  }

  openstudio::Quantity RoofVegetation_Impl::leafReflectivity_IP() const {
    return getLeafReflectivity(true);
  }

  openstudio::Quantity RoofVegetation_Impl::leafEmissivity_SI() const {
    return getLeafEmissivity(false);
  }

  openstudio::Quantity RoofVegetation_Impl::leafEmissivity_IP() const {
    return getLeafEmissivity(true);
  }

  openstudio::Quantity RoofVegetation_Impl::minimumStomatalResistance_SI() const {
    return getMinimumStomatalResistance(false);
  }

  openstudio::Quantity RoofVegetation_Impl::minimumStomatalResistance_IP() const {
    return getMinimumStomatalResistance(true);
  }

  std::vector<std::string> RoofVegetation_Impl::roughnessValues() const {
    return RoofVegetation::roughnessValues();
  }

  openstudio::Quantity RoofVegetation_Impl::thickness_SI() const {
    return getThickness(false);
  }

  openstudio::Quantity RoofVegetation_Impl::thickness_IP() const {
    return getThickness(true);
  }

  openstudio::Quantity RoofVegetation_Impl::conductivityofDrySoil_SI() const {
    return getConductivityofDrySoil(false);
  }

  openstudio::Quantity RoofVegetation_Impl::conductivityofDrySoil_IP() const {
    return getConductivityofDrySoil(true);
  }

  openstudio::Quantity RoofVegetation_Impl::densityofDrySoil_SI() const {
    return getDensityofDrySoil(false);
  }

  openstudio::Quantity RoofVegetation_Impl::densityofDrySoil_IP() const {
    return getDensityofDrySoil(true);
  }

  openstudio::Quantity RoofVegetation_Impl::specificHeatofDrySoil_SI() const {
    return getSpecificHeatofDrySoil(false);
  }

  openstudio::Quantity RoofVegetation_Impl::specificHeatofDrySoil_IP() const {
    return getSpecificHeatofDrySoil(true);
  }

  openstudio::Quantity RoofVegetation_Impl::thermalAbsorptance_SI() const {
    return getThermalAbsorptance(false);
  }

  openstudio::Quantity RoofVegetation_Impl::thermalAbsorptance_IP() const {
    return getThermalAbsorptance(true);
  }

  openstudio::Quantity RoofVegetation_Impl::solarAbsorptance_SI() const {
    return getSolarAbsorptance(false);
  }

  openstudio::Quantity RoofVegetation_Impl::solarAbsorptance_IP() const {
    return getSolarAbsorptance(true);
  }

  openstudio::Quantity RoofVegetation_Impl::visibleAbsorptance_SI() const {
    return getVisibleAbsorptance(false);
  }

  openstudio::Quantity RoofVegetation_Impl::visibleAbsorptance_IP() const {
    return getVisibleAbsorptance(true);
  }

  openstudio::Quantity RoofVegetation_Impl::saturationVolumetricMoistureContentoftheSoilLayer_SI() const {
    return getSaturationVolumetricMoistureContentoftheSoilLayer(false);
  }

  openstudio::Quantity RoofVegetation_Impl::saturationVolumetricMoistureContentoftheSoilLayer_IP() const {
    return getSaturationVolumetricMoistureContentoftheSoilLayer(true);
  }

  openstudio::Quantity RoofVegetation_Impl::residualVolumetricMoistureContentoftheSoilLayer_SI() const {
    return getResidualVolumetricMoistureContentoftheSoilLayer(false);
  }

  openstudio::Quantity RoofVegetation_Impl::residualVolumetricMoistureContentoftheSoilLayer_IP() const {
    return getResidualVolumetricMoistureContentoftheSoilLayer(true);
  }

  openstudio::Quantity RoofVegetation_Impl::initialVolumetricMoistureContentoftheSoilLayer_SI() const {
    return getInitialVolumetricMoistureContentoftheSoilLayer(false);
  }

  openstudio::Quantity RoofVegetation_Impl::initialVolumetricMoistureContentoftheSoilLayer_IP() const {
    return getInitialVolumetricMoistureContentoftheSoilLayer(true);
  }

  std::vector<std::string> RoofVegetation_Impl::moistureDiffusionCalculationMethodValues() const {
    return RoofVegetation::moistureDiffusionCalculationMethodValues();
  }

} // detail

RoofVegetation::RoofVegetation(const Model& model,
                               std::string roughness)
  : OpaqueMaterial(RoofVegetation::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::RoofVegetation_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
  ok = setRoughness(roughness);
  OS_ASSERT(ok);
}

IddObjectType RoofVegetation::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Material_RoofVegetation);
}

std::vector<std::string> RoofVegetation::roughnessValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Material_RoofVegetationFields::Roughness);
}

std::vector<std::string> RoofVegetation::moistureDiffusionCalculationMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Material_RoofVegetationFields::MoistureDiffusionCalculationMethod);
}

double RoofVegetation::heightofPlants() const {
  return getImpl<detail::RoofVegetation_Impl>()->heightofPlants();
}

Quantity RoofVegetation::getHeightofPlants(bool returnIP) const {
  return getImpl<detail::RoofVegetation_Impl>()->getHeightofPlants(returnIP);
}

bool RoofVegetation::isHeightofPlantsDefaulted() const {
  return getImpl<detail::RoofVegetation_Impl>()->isHeightofPlantsDefaulted();
}

double RoofVegetation::leafAreaIndex() const {
  return getImpl<detail::RoofVegetation_Impl>()->leafAreaIndex();
}

Quantity RoofVegetation::getLeafAreaIndex(bool returnIP) const {
  return getImpl<detail::RoofVegetation_Impl>()->getLeafAreaIndex(returnIP);
}

bool RoofVegetation::isLeafAreaIndexDefaulted() const {
  return getImpl<detail::RoofVegetation_Impl>()->isLeafAreaIndexDefaulted();
}

double RoofVegetation::leafReflectivity() const {
  return getImpl<detail::RoofVegetation_Impl>()->leafReflectivity();
}

Quantity RoofVegetation::getLeafReflectivity(bool returnIP) const {
  return getImpl<detail::RoofVegetation_Impl>()->getLeafReflectivity(returnIP);
}

bool RoofVegetation::isLeafReflectivityDefaulted() const {
  return getImpl<detail::RoofVegetation_Impl>()->isLeafReflectivityDefaulted();
}

double RoofVegetation::leafEmissivity() const {
  return getImpl<detail::RoofVegetation_Impl>()->leafEmissivity();
}

Quantity RoofVegetation::getLeafEmissivity(bool returnIP) const {
  return getImpl<detail::RoofVegetation_Impl>()->getLeafEmissivity(returnIP);
}

bool RoofVegetation::isLeafEmissivityDefaulted() const {
  return getImpl<detail::RoofVegetation_Impl>()->isLeafEmissivityDefaulted();
}

double RoofVegetation::minimumStomatalResistance() const {
  return getImpl<detail::RoofVegetation_Impl>()->minimumStomatalResistance();
}

Quantity RoofVegetation::getMinimumStomatalResistance(bool returnIP) const {
  return getImpl<detail::RoofVegetation_Impl>()->getMinimumStomatalResistance(returnIP);
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

double RoofVegetation::thickness() const {
  return getImpl<detail::RoofVegetation_Impl>()->thickness();
}

Quantity RoofVegetation::getThickness(bool returnIP) const {
  return getImpl<detail::RoofVegetation_Impl>()->getThickness(returnIP);
}

bool RoofVegetation::isThicknessDefaulted() const {
  return getImpl<detail::RoofVegetation_Impl>()->isThicknessDefaulted();
}

double RoofVegetation::conductivityofDrySoil() const {
  return getImpl<detail::RoofVegetation_Impl>()->conductivityofDrySoil();
}

Quantity RoofVegetation::getConductivityofDrySoil(bool returnIP) const {
  return getImpl<detail::RoofVegetation_Impl>()->getConductivityofDrySoil(returnIP);
}

bool RoofVegetation::isConductivityofDrySoilDefaulted() const {
  return getImpl<detail::RoofVegetation_Impl>()->isConductivityofDrySoilDefaulted();
}

double RoofVegetation::densityofDrySoil() const {
  return getImpl<detail::RoofVegetation_Impl>()->densityofDrySoil();
}

Quantity RoofVegetation::getDensityofDrySoil(bool returnIP) const {
  return getImpl<detail::RoofVegetation_Impl>()->getDensityofDrySoil(returnIP);
}

bool RoofVegetation::isDensityofDrySoilDefaulted() const {
  return getImpl<detail::RoofVegetation_Impl>()->isDensityofDrySoilDefaulted();
}

double RoofVegetation::specificHeatofDrySoil() const {
  return getImpl<detail::RoofVegetation_Impl>()->specificHeatofDrySoil();
}

Quantity RoofVegetation::getSpecificHeatofDrySoil(bool returnIP) const {
  return getImpl<detail::RoofVegetation_Impl>()->getSpecificHeatofDrySoil(returnIP);
}

bool RoofVegetation::isSpecificHeatofDrySoilDefaulted() const {
  return getImpl<detail::RoofVegetation_Impl>()->isSpecificHeatofDrySoilDefaulted();
}

OptionalDouble RoofVegetation::thermalAbsorptance() const {
  return getImpl<detail::RoofVegetation_Impl>()->thermalAbsorptance();
}

Quantity RoofVegetation::getThermalAbsorptance(bool returnIP) const {
  return getImpl<detail::RoofVegetation_Impl>()->getThermalAbsorptance(returnIP);
}

bool RoofVegetation::isThermalAbsorptanceDefaulted() const {
  return getImpl<detail::RoofVegetation_Impl>()->isThermalAbsorptanceDefaulted();
}

OptionalDouble RoofVegetation::solarAbsorptance() const {
  return getImpl<detail::RoofVegetation_Impl>()->solarAbsorptance();
}

Quantity RoofVegetation::getSolarAbsorptance(bool returnIP) const {
  return getImpl<detail::RoofVegetation_Impl>()->getSolarAbsorptance(returnIP);
}

bool RoofVegetation::isSolarAbsorptanceDefaulted() const {
  return getImpl<detail::RoofVegetation_Impl>()->isSolarAbsorptanceDefaulted();
}

OptionalDouble RoofVegetation::visibleAbsorptance() const {
  return getImpl<detail::RoofVegetation_Impl>()->visibleAbsorptance();
}

Quantity RoofVegetation::getVisibleAbsorptance(bool returnIP) const {
  return getImpl<detail::RoofVegetation_Impl>()->getVisibleAbsorptance(returnIP);
}

bool RoofVegetation::isVisibleAbsorptanceDefaulted() const {
  return getImpl<detail::RoofVegetation_Impl>()->isVisibleAbsorptanceDefaulted();
}

double RoofVegetation::saturationVolumetricMoistureContentoftheSoilLayer() const {
  return getImpl<detail::RoofVegetation_Impl>()->saturationVolumetricMoistureContentoftheSoilLayer();
}

Quantity RoofVegetation::getSaturationVolumetricMoistureContentoftheSoilLayer(bool returnIP) const {
  return getImpl<detail::RoofVegetation_Impl>()->getSaturationVolumetricMoistureContentoftheSoilLayer(returnIP);
}

bool RoofVegetation::isSaturationVolumetricMoistureContentoftheSoilLayerDefaulted() const {
  return getImpl<detail::RoofVegetation_Impl>()->isSaturationVolumetricMoistureContentoftheSoilLayerDefaulted();
}

double RoofVegetation::residualVolumetricMoistureContentoftheSoilLayer() const {
  return getImpl<detail::RoofVegetation_Impl>()->residualVolumetricMoistureContentoftheSoilLayer();
}

Quantity RoofVegetation::getResidualVolumetricMoistureContentoftheSoilLayer(bool returnIP) const {
  return getImpl<detail::RoofVegetation_Impl>()->getResidualVolumetricMoistureContentoftheSoilLayer(returnIP);
}

bool RoofVegetation::isResidualVolumetricMoistureContentoftheSoilLayerDefaulted() const {
  return getImpl<detail::RoofVegetation_Impl>()->isResidualVolumetricMoistureContentoftheSoilLayerDefaulted();
}

double RoofVegetation::initialVolumetricMoistureContentoftheSoilLayer() const {
  return getImpl<detail::RoofVegetation_Impl>()->initialVolumetricMoistureContentoftheSoilLayer();
}

Quantity RoofVegetation::getInitialVolumetricMoistureContentoftheSoilLayer(bool returnIP) const {
  return getImpl<detail::RoofVegetation_Impl>()->getInitialVolumetricMoistureContentoftheSoilLayer(returnIP);
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

bool RoofVegetation::setHeightofPlants(const Quantity& heightofPlants) {
  return getImpl<detail::RoofVegetation_Impl>()->setHeightofPlants(heightofPlants);
}

void RoofVegetation::resetHeightofPlants() {
  getImpl<detail::RoofVegetation_Impl>()->resetHeightofPlants();
}

bool RoofVegetation::setLeafAreaIndex(double leafAreaIndex) {
  return getImpl<detail::RoofVegetation_Impl>()->setLeafAreaIndex(leafAreaIndex);
}

bool RoofVegetation::setLeafAreaIndex(const Quantity& leafAreaIndex) {
  return getImpl<detail::RoofVegetation_Impl>()->setLeafAreaIndex(leafAreaIndex);
}

void RoofVegetation::resetLeafAreaIndex() {
  getImpl<detail::RoofVegetation_Impl>()->resetLeafAreaIndex();
}

bool RoofVegetation::setLeafReflectivity(double leafReflectivity) {
  return getImpl<detail::RoofVegetation_Impl>()->setLeafReflectivity(leafReflectivity);
}

bool RoofVegetation::setLeafReflectivity(const Quantity& leafReflectivity) {
  return getImpl<detail::RoofVegetation_Impl>()->setLeafReflectivity(leafReflectivity);
}

void RoofVegetation::resetLeafReflectivity() {
  getImpl<detail::RoofVegetation_Impl>()->resetLeafReflectivity();
}

bool RoofVegetation::setLeafEmissivity(double leafEmissivity) {
  return getImpl<detail::RoofVegetation_Impl>()->setLeafEmissivity(leafEmissivity);
}

bool RoofVegetation::setLeafEmissivity(const Quantity& leafEmissivity) {
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

bool RoofVegetation::setMinimumStomatalResistance(const Quantity& minimumStomatalResistance) {
  return getImpl<detail::RoofVegetation_Impl>()->setMinimumStomatalResistance(minimumStomatalResistance);
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

bool RoofVegetation::setThickness(double thickness) {
  return getImpl<detail::RoofVegetation_Impl>()->setThickness(thickness);
}

bool RoofVegetation::setThickness(const Quantity& thickness) {
  return getImpl<detail::RoofVegetation_Impl>()->setThickness(thickness);
}

void RoofVegetation::resetThickness() {
  getImpl<detail::RoofVegetation_Impl>()->resetThickness();
}

bool RoofVegetation::setConductivityofDrySoil(double conductivityofDrySoil) {
  return getImpl<detail::RoofVegetation_Impl>()->setConductivityofDrySoil(conductivityofDrySoil);
}

bool RoofVegetation::setConductivityofDrySoil(const Quantity& conductivityofDrySoil) {
  return getImpl<detail::RoofVegetation_Impl>()->setConductivityofDrySoil(conductivityofDrySoil);
}

void RoofVegetation::resetConductivityofDrySoil() {
  getImpl<detail::RoofVegetation_Impl>()->resetConductivityofDrySoil();
}

bool RoofVegetation::setDensityofDrySoil(double densityofDrySoil) {
  return getImpl<detail::RoofVegetation_Impl>()->setDensityofDrySoil(densityofDrySoil);
}

bool RoofVegetation::setDensityofDrySoil(const Quantity& densityofDrySoil) {
  return getImpl<detail::RoofVegetation_Impl>()->setDensityofDrySoil(densityofDrySoil);
}

void RoofVegetation::resetDensityofDrySoil() {
  getImpl<detail::RoofVegetation_Impl>()->resetDensityofDrySoil();
}

bool RoofVegetation::setSpecificHeatofDrySoil(double specificHeatofDrySoil) {
  return getImpl<detail::RoofVegetation_Impl>()->setSpecificHeatofDrySoil(specificHeatofDrySoil);
}

bool RoofVegetation::setSpecificHeatofDrySoil(const Quantity& specificHeatofDrySoil) {
  return getImpl<detail::RoofVegetation_Impl>()->setSpecificHeatofDrySoil(specificHeatofDrySoil);
}

void RoofVegetation::resetSpecificHeatofDrySoil() {
  getImpl<detail::RoofVegetation_Impl>()->resetSpecificHeatofDrySoil();
}

bool RoofVegetation::setThermalAbsorptance(double thermalAbsorptance) {
  return getImpl<detail::RoofVegetation_Impl>()->setThermalAbsorptance(thermalAbsorptance);
}

bool RoofVegetation::setThermalAbsorptance(const Quantity& thermalAbsorptance) {
  return getImpl<detail::RoofVegetation_Impl>()->setThermalAbsorptance(thermalAbsorptance);
}

void RoofVegetation::resetThermalAbsorptance() {
  getImpl<detail::RoofVegetation_Impl>()->resetThermalAbsorptance();
}

bool RoofVegetation::setSolarAbsorptance(double solarAbsorptance) {
  return getImpl<detail::RoofVegetation_Impl>()->setSolarAbsorptance(solarAbsorptance);
}

bool RoofVegetation::setSolarAbsorptance(const Quantity& solarAbsorptance) {
  return getImpl<detail::RoofVegetation_Impl>()->setSolarAbsorptance(solarAbsorptance);
}

void RoofVegetation::resetSolarAbsorptance() {
  getImpl<detail::RoofVegetation_Impl>()->resetSolarAbsorptance();
}

bool RoofVegetation::setVisibleAbsorptance(double visibleAbsorptance) {
  return getImpl<detail::RoofVegetation_Impl>()->setVisibleAbsorptance(visibleAbsorptance);
}

bool RoofVegetation::setVisibleAbsorptance(const Quantity& visibleAbsorptance) {
  return getImpl<detail::RoofVegetation_Impl>()->setVisibleAbsorptance(visibleAbsorptance);
}

void RoofVegetation::resetVisibleAbsorptance() {
  getImpl<detail::RoofVegetation_Impl>()->resetVisibleAbsorptance();
}

bool RoofVegetation::setSaturationVolumetricMoistureContentoftheSoilLayer(double saturationVolumetricMoistureContentoftheSoilLayer) {
  return getImpl<detail::RoofVegetation_Impl>()->setSaturationVolumetricMoistureContentoftheSoilLayer(saturationVolumetricMoistureContentoftheSoilLayer);
}

bool RoofVegetation::setSaturationVolumetricMoistureContentoftheSoilLayer(const Quantity& saturationVolumetricMoistureContentoftheSoilLayer) {
  return getImpl<detail::RoofVegetation_Impl>()->setSaturationVolumetricMoistureContentoftheSoilLayer(saturationVolumetricMoistureContentoftheSoilLayer);
}

void RoofVegetation::resetSaturationVolumetricMoistureContentoftheSoilLayer() {
  getImpl<detail::RoofVegetation_Impl>()->resetSaturationVolumetricMoistureContentoftheSoilLayer();
}

bool RoofVegetation::setResidualVolumetricMoistureContentoftheSoilLayer(double residualVolumetricMoistureContentoftheSoilLayer) {
  return getImpl<detail::RoofVegetation_Impl>()->setResidualVolumetricMoistureContentoftheSoilLayer(residualVolumetricMoistureContentoftheSoilLayer);
}

bool RoofVegetation::setResidualVolumetricMoistureContentoftheSoilLayer(const Quantity& residualVolumetricMoistureContentoftheSoilLayer) {
  return getImpl<detail::RoofVegetation_Impl>()->setResidualVolumetricMoistureContentoftheSoilLayer(residualVolumetricMoistureContentoftheSoilLayer);
}

void RoofVegetation::resetResidualVolumetricMoistureContentoftheSoilLayer() {
  getImpl<detail::RoofVegetation_Impl>()->resetResidualVolumetricMoistureContentoftheSoilLayer();
}

bool RoofVegetation::setInitialVolumetricMoistureContentoftheSoilLayer(double initialVolumetricMoistureContentoftheSoilLayer) {
  return getImpl<detail::RoofVegetation_Impl>()->setInitialVolumetricMoistureContentoftheSoilLayer(initialVolumetricMoistureContentoftheSoilLayer);
}

bool RoofVegetation::setInitialVolumetricMoistureContentoftheSoilLayer(const Quantity& initialVolumetricMoistureContentoftheSoilLayer) {
  return getImpl<detail::RoofVegetation_Impl>()->setInitialVolumetricMoistureContentoftheSoilLayer(initialVolumetricMoistureContentoftheSoilLayer);
}

void RoofVegetation::resetInitialVolumetricMoistureContentoftheSoilLayer() {
  getImpl<detail::RoofVegetation_Impl>()->resetInitialVolumetricMoistureContentoftheSoilLayer();
}

void RoofVegetation::resetMoistureDiffusionCalculationMethod() {
  getImpl<detail::RoofVegetation_Impl>()->resetMoistureDiffusionCalculationMethod();
}

/// @cond
RoofVegetation::RoofVegetation(std::shared_ptr<detail::RoofVegetation_Impl> impl)
  : OpaqueMaterial(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

