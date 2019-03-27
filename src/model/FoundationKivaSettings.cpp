/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include <vector>
#include <string>
#include "FoundationKivaSettings.hpp"
#include "FoundationKivaSettings_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Site.hpp"
#include "Site_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Foundation_Kiva_Settings_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  FoundationKivaSettings_Impl::FoundationKivaSettings_Impl(const IdfObject& idfObject,
                                                           Model_Impl* model,
                                                           bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == FoundationKivaSettings::iddObjectType());
  }

  FoundationKivaSettings_Impl::FoundationKivaSettings_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                           Model_Impl* model,
                                                           bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == FoundationKivaSettings::iddObjectType());
  }

  FoundationKivaSettings_Impl::FoundationKivaSettings_Impl(const FoundationKivaSettings_Impl& other,
                                                           Model_Impl* model,
                                                           bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  boost::optional<ParentObject> FoundationKivaSettings_Impl::parent() const {
    boost::optional<Site> result = this->model().getOptionalUniqueModelObject<Site>();
    return boost::optional<ParentObject>(result);
  }

  const std::vector<std::string>& FoundationKivaSettings_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    return result;
  }

  IddObjectType FoundationKivaSettings_Impl::iddObjectType() const {
    return FoundationKivaSettings::iddObjectType();
  }

  double FoundationKivaSettings_Impl::soilConductivity() const {
    boost::optional<double> value = getDouble(OS_Foundation_Kiva_SettingsFields::SoilConductivity, true);
    OS_ASSERT(value);
    return value.get();
  }

  bool FoundationKivaSettings_Impl::isSoilConductivityDefaulted() const {
    return isEmpty(OS_Foundation_Kiva_SettingsFields::SoilConductivity);
  }

  double FoundationKivaSettings_Impl::soilDensity() const {
    boost::optional<double> value = getDouble(OS_Foundation_Kiva_SettingsFields::SoilDensity, true);
    OS_ASSERT(value);
    return value.get();
  }

  bool FoundationKivaSettings_Impl::isSoilDensityDefaulted() const {
    return isEmpty(OS_Foundation_Kiva_SettingsFields::SoilDensity);
  }

  double FoundationKivaSettings_Impl::soilSpecificHeat() const {
    boost::optional<double> value = getDouble(OS_Foundation_Kiva_SettingsFields::SoilSpecificHeat, true);
    OS_ASSERT(value);
    return value.get();
  }

  bool FoundationKivaSettings_Impl::isSoilSpecificHeatDefaulted() const {
    return isEmpty(OS_Foundation_Kiva_SettingsFields::SoilSpecificHeat);
  }

  double FoundationKivaSettings_Impl::groundSolarAbsorptivity() const {
    boost::optional<double> value = getDouble(OS_Foundation_Kiva_SettingsFields::GroundSolarAbsorptivity, true);
    OS_ASSERT(value);
    return value.get();
  }

  bool FoundationKivaSettings_Impl::isGroundSolarAbsorptivityDefaulted() const {
    return isEmpty(OS_Foundation_Kiva_SettingsFields::GroundSolarAbsorptivity);
  }

  double FoundationKivaSettings_Impl::groundThermalAbsorptivity() const {
    boost::optional<double> value = getDouble(OS_Foundation_Kiva_SettingsFields::GroundThermalAbsorptivity, true);
    OS_ASSERT(value);
    return value.get();
  }

  bool FoundationKivaSettings_Impl::isGroundThermalAbsorptivityDefaulted() const {
    return isEmpty(OS_Foundation_Kiva_SettingsFields::GroundThermalAbsorptivity);
  }

  double FoundationKivaSettings_Impl::groundSurfaceRoughness() const {
    boost::optional<double> value = getDouble(OS_Foundation_Kiva_SettingsFields::GroundSurfaceRoughness, true);
    OS_ASSERT(value);
    return value.get();
  }

  bool FoundationKivaSettings_Impl::isGroundSurfaceRoughnessDefaulted() const {
    return isEmpty(OS_Foundation_Kiva_SettingsFields::GroundSurfaceRoughness);
  }

  double FoundationKivaSettings_Impl::farFieldWidth() const {
    boost::optional<double> value = getDouble(OS_Foundation_Kiva_SettingsFields::FarFieldWidth, true);
    OS_ASSERT(value);
    return value.get();
  }

  bool FoundationKivaSettings_Impl::isFarFieldWidthDefaulted() const {
    return isEmpty(OS_Foundation_Kiva_SettingsFields::FarFieldWidth);
  }

  std::string FoundationKivaSettings_Impl::deepGroundBoundaryCondition() const {
    boost::optional<std::string> value = getString(OS_Foundation_Kiva_SettingsFields::DeepGroundBoundaryCondition, true);
    OS_ASSERT(value);
    return value.get();
  }

  bool FoundationKivaSettings_Impl::isDeepGroundBoundaryConditionAutoselected() {
    boost::optional<std::string> value = getString(OS_Foundation_Kiva_SettingsFields::DeepGroundBoundaryCondition, true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Autoselect");
  }

  boost::optional<double> FoundationKivaSettings_Impl::deepGroundDepth() {
    return getDouble(OS_Foundation_Kiva_SettingsFields::DeepGroundDepth, true);
  }

  bool FoundationKivaSettings_Impl::isDeepGroundDepthAutocalculated() {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Foundation_Kiva_SettingsFields::DeepGroundDepth, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autocalculate");
    }
    return result;
  }

  double FoundationKivaSettings_Impl::minimumCellDimension() const {
    boost::optional<double> value = getDouble(OS_Foundation_Kiva_SettingsFields::MinimumCellDimension, true);
    OS_ASSERT(value);
    return value.get();
  }

  bool FoundationKivaSettings_Impl::isMinimumCellDimensionDefaulted() const {
    return isEmpty(OS_Foundation_Kiva_SettingsFields::MinimumCellDimension);
  }

  double FoundationKivaSettings_Impl::maximumCellGrowthCoefficient() const {
    boost::optional<double> value = getDouble(OS_Foundation_Kiva_SettingsFields::MaximumCellGrowthCoefficient, true);
    OS_ASSERT(value);
    return value.get();
  }

  bool FoundationKivaSettings_Impl::isMaximumCellGrowthCoefficientDefaulted() const {
    return isEmpty(OS_Foundation_Kiva_SettingsFields::MaximumCellGrowthCoefficient);
  }

  std::string FoundationKivaSettings_Impl::simulationTimestep() const {
    boost::optional<std::string> value = getString(OS_Foundation_Kiva_SettingsFields::SimulationTimestep, true);
    OS_ASSERT(value);
    return value.get();
  }

  bool FoundationKivaSettings_Impl::isSimulationTimestepDefaulted() const {
    boost::optional<std::string> value = getString(OS_Foundation_Kiva_SettingsFields::SimulationTimestep, true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Hourly");
  }

  bool FoundationKivaSettings_Impl::setSoilConductivity(double soilConductivity) {
    bool result = setDouble(OS_Foundation_Kiva_SettingsFields::SoilConductivity, soilConductivity);
    OS_ASSERT(result);
    return result;
  }

  void FoundationKivaSettings_Impl::resetSoilConductivity() {
    bool result = setString(OS_Foundation_Kiva_SettingsFields::SoilConductivity, "");
    OS_ASSERT(result);
  }

  bool FoundationKivaSettings_Impl::setSoilDensity(double soilDensity) {
    bool result = setDouble(OS_Foundation_Kiva_SettingsFields::SoilDensity, soilDensity);
    OS_ASSERT(result);
    return result;
  }

  void FoundationKivaSettings_Impl::resetSoilDensity() {
    bool result = setString(OS_Foundation_Kiva_SettingsFields::SoilDensity, "");
    OS_ASSERT(result);
  }

  bool FoundationKivaSettings_Impl::setSoilSpecificHeat(double soilSpecificHeat) {
    bool result = setDouble(OS_Foundation_Kiva_SettingsFields::SoilSpecificHeat, soilSpecificHeat);
    OS_ASSERT(result);
    return result;
  }

  void FoundationKivaSettings_Impl::resetSoilSpecificHeat() {
    bool result = setString(OS_Foundation_Kiva_SettingsFields::SoilSpecificHeat, "");
    OS_ASSERT(result);
  }

  bool FoundationKivaSettings_Impl::setGroundSolarAbsorptivity(double groundSolarAbsorptivity) {
    bool result = setDouble(OS_Foundation_Kiva_SettingsFields::GroundSolarAbsorptivity, groundSolarAbsorptivity);
    OS_ASSERT(result);
    return result;
  }

  void FoundationKivaSettings_Impl::resetGroundSolarAbsorptivity() {
    bool result = setString(OS_Foundation_Kiva_SettingsFields::GroundSolarAbsorptivity, "");
    OS_ASSERT(result);
  }

  bool FoundationKivaSettings_Impl::setGroundThermalAbsorptivity(double groundThermalAbsorptivity) {
    bool result = setDouble(OS_Foundation_Kiva_SettingsFields::GroundThermalAbsorptivity, groundThermalAbsorptivity);
    OS_ASSERT(result);
    return result;
  }

  void FoundationKivaSettings_Impl::resetGroundThermalAbsorptivity() {
    bool result = setString(OS_Foundation_Kiva_SettingsFields::GroundThermalAbsorptivity, "");
    OS_ASSERT(result);
  }

  bool FoundationKivaSettings_Impl::setGroundSurfaceRoughness(double groundSurfaceRoughness) {
    bool result = setDouble(OS_Foundation_Kiva_SettingsFields::GroundSurfaceRoughness, groundSurfaceRoughness);
    OS_ASSERT(result);
    return result;
  }

  void FoundationKivaSettings_Impl::resetGroundSurfaceRoughness() {
    bool result = setString(OS_Foundation_Kiva_SettingsFields::GroundSurfaceRoughness, "");
    OS_ASSERT(result);
  }

  bool FoundationKivaSettings_Impl::setFarFieldWidth(double farFieldWidth) {
    bool result = setDouble(OS_Foundation_Kiva_SettingsFields::FarFieldWidth, farFieldWidth);
    OS_ASSERT(result);
    return result;
  }

  void FoundationKivaSettings_Impl::resetFarFieldWidth() {
    bool result = setString(OS_Foundation_Kiva_SettingsFields::FarFieldWidth, "");
    OS_ASSERT(result);
  }

  bool FoundationKivaSettings_Impl::setDeepGroundBoundaryCondition(std::string deepGroundBoundaryCondition) {
    bool result = setString(OS_Foundation_Kiva_SettingsFields::DeepGroundBoundaryCondition, deepGroundBoundaryCondition);
    OS_ASSERT(result);
    return result;
  }

  void FoundationKivaSettings_Impl::resetDeepGroundBoundaryCondition() {
    bool result = setString(OS_Foundation_Kiva_SettingsFields::DeepGroundBoundaryCondition, "");
    OS_ASSERT(result);
  }

  bool FoundationKivaSettings_Impl::setDeepGroundDepth(double deepGroundDepth) {
    bool result = setDouble(OS_Foundation_Kiva_SettingsFields::DeepGroundDepth, deepGroundDepth);
    OS_ASSERT(result);
    return result;
  }

  void FoundationKivaSettings_Impl::autocalculateDeepGroundDepth() {
    bool result = setString(OS_Foundation_Kiva_SettingsFields::DeepGroundDepth, "");
    OS_ASSERT(result);
  }

  bool FoundationKivaSettings_Impl::setMinimumCellDimension(double minimumCellDimension) {
    bool result = setDouble(OS_Foundation_Kiva_SettingsFields::MinimumCellDimension, minimumCellDimension);
    OS_ASSERT(result);
    return result;
  }

  void FoundationKivaSettings_Impl::resetMinimumCellDimension() {
    bool result = setString(OS_Foundation_Kiva_SettingsFields::MinimumCellDimension, "");
    OS_ASSERT(result);
  }

  bool FoundationKivaSettings_Impl::setMaximumCellGrowthCoefficient(double maximumCellGrowthCoefficient) {
    bool result = setDouble(OS_Foundation_Kiva_SettingsFields::MaximumCellGrowthCoefficient, maximumCellGrowthCoefficient);
    OS_ASSERT(result);
    return result;
  }

  void FoundationKivaSettings_Impl::resetMaximumCellGrowthCoefficient() {
    bool result = setString(OS_Foundation_Kiva_SettingsFields::MaximumCellGrowthCoefficient, "");
    OS_ASSERT(result);
  }

  bool FoundationKivaSettings_Impl::setSimulationTimestep(std::string simulationTimestep) {
    bool result = setString(OS_Foundation_Kiva_SettingsFields::SimulationTimestep, simulationTimestep);
    OS_ASSERT(result);
    return result;
  }

  void FoundationKivaSettings_Impl::resetSimulationTimestep() {
    bool result = setString(OS_Foundation_Kiva_SettingsFields::SimulationTimestep, "");
    OS_ASSERT(result);
  }

} // detail

IddObjectType FoundationKivaSettings::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Foundation_Kiva_Settings);
}

double FoundationKivaSettings::soilConductivity() const {
  return getImpl<detail::FoundationKivaSettings_Impl>()->soilConductivity();
}

bool FoundationKivaSettings::isSoilConductivityDefaulted() const {
  return getImpl<detail::FoundationKivaSettings_Impl>()->isSoilConductivityDefaulted();
}

double FoundationKivaSettings::soilDensity() const {
  return getImpl<detail::FoundationKivaSettings_Impl>()->soilDensity();
}

bool FoundationKivaSettings::isSoilDensityDefaulted() const {
  return getImpl<detail::FoundationKivaSettings_Impl>()->isSoilDensityDefaulted();
}

double FoundationKivaSettings::soilSpecificHeat() const {
  return getImpl<detail::FoundationKivaSettings_Impl>()->soilSpecificHeat();
}

bool FoundationKivaSettings::isSoilSpecificHeatDefaulted() const {
  return getImpl<detail::FoundationKivaSettings_Impl>()->isSoilSpecificHeatDefaulted();
}

double FoundationKivaSettings::groundSolarAbsorptivity() const {
  return getImpl<detail::FoundationKivaSettings_Impl>()->groundSolarAbsorptivity();
}

bool FoundationKivaSettings::isGroundSolarAbsorptivityDefaulted() const {
  return getImpl<detail::FoundationKivaSettings_Impl>()->isGroundSolarAbsorptivityDefaulted();
}

double FoundationKivaSettings::groundThermalAbsorptivity() const {
  return getImpl<detail::FoundationKivaSettings_Impl>()->groundThermalAbsorptivity();
}

bool FoundationKivaSettings::isGroundThermalAbsorptivityDefaulted() const {
  return getImpl<detail::FoundationKivaSettings_Impl>()->isGroundThermalAbsorptivityDefaulted();
}

double FoundationKivaSettings::groundSurfaceRoughness() const {
  return getImpl<detail::FoundationKivaSettings_Impl>()->groundSurfaceRoughness();
}

bool FoundationKivaSettings::isGroundSurfaceRoughnessDefaulted() const {
  return getImpl<detail::FoundationKivaSettings_Impl>()->isGroundSurfaceRoughnessDefaulted();
}

double FoundationKivaSettings::farFieldWidth() const {
  return getImpl<detail::FoundationKivaSettings_Impl>()->farFieldWidth();
}

bool FoundationKivaSettings::isFarFieldWidthDefaulted() const {
  return getImpl<detail::FoundationKivaSettings_Impl>()->isFarFieldWidthDefaulted();
}

std::string FoundationKivaSettings::deepGroundBoundaryCondition() const {
  return getImpl<detail::FoundationKivaSettings_Impl>()->deepGroundBoundaryCondition();
}

bool FoundationKivaSettings::isDeepGroundBoundaryConditionAutoselected() {
  return getImpl<detail::FoundationKivaSettings_Impl>()->isDeepGroundBoundaryConditionAutoselected();
}

boost::optional<double> FoundationKivaSettings::deepGroundDepth() {
  return getImpl<detail::FoundationKivaSettings_Impl>()->deepGroundDepth();
}

bool FoundationKivaSettings::isDeepGroundDepthAutocalculated() {
  return getImpl<detail::FoundationKivaSettings_Impl>()->isDeepGroundDepthAutocalculated();
}

double FoundationKivaSettings::minimumCellDimension() const {
  return getImpl<detail::FoundationKivaSettings_Impl>()->minimumCellDimension();
}

bool FoundationKivaSettings::isMinimumCellDimensionDefaulted() const {
  return getImpl<detail::FoundationKivaSettings_Impl>()->isMinimumCellDimensionDefaulted();
}

double FoundationKivaSettings::maximumCellGrowthCoefficient() const {
  return getImpl<detail::FoundationKivaSettings_Impl>()->maximumCellGrowthCoefficient();
}

bool FoundationKivaSettings::isMaximumCellGrowthCoefficientDefaulted() const {
  return getImpl<detail::FoundationKivaSettings_Impl>()->isMaximumCellGrowthCoefficientDefaulted();
}

std::string FoundationKivaSettings::simulationTimestep() const {
  return getImpl<detail::FoundationKivaSettings_Impl>()->simulationTimestep();
}

bool FoundationKivaSettings::isSimulationTimestepDefaulted() const {
  return getImpl<detail::FoundationKivaSettings_Impl>()->isSimulationTimestepDefaulted();
}

bool FoundationKivaSettings::setSoilConductivity(double soilConductivity) {
  return getImpl<detail::FoundationKivaSettings_Impl>()->setSoilConductivity(soilConductivity);
}

void FoundationKivaSettings::resetSoilConductivity() {
  getImpl<detail::FoundationKivaSettings_Impl>()->resetSoilConductivity();
}

bool FoundationKivaSettings::setSoilDensity(double soilDensity) {
  return getImpl<detail::FoundationKivaSettings_Impl>()->setSoilDensity(soilDensity);
}

void FoundationKivaSettings::resetSoilDensity() {
  getImpl<detail::FoundationKivaSettings_Impl>()->resetSoilDensity();
}

bool FoundationKivaSettings::setSoilSpecificHeat(double soilSpecificHeat) {
  return getImpl<detail::FoundationKivaSettings_Impl>()->setSoilSpecificHeat(soilSpecificHeat);
}

void FoundationKivaSettings::resetSoilSpecificHeat() {
  getImpl<detail::FoundationKivaSettings_Impl>()->resetSoilSpecificHeat();
}

bool FoundationKivaSettings::setGroundSolarAbsorptivity(double groundSolarAbsorptivity) {
  return getImpl<detail::FoundationKivaSettings_Impl>()->setGroundSolarAbsorptivity(groundSolarAbsorptivity);
}

void FoundationKivaSettings::resetGroundSolarAbsorptivity() {
  getImpl<detail::FoundationKivaSettings_Impl>()->resetGroundSolarAbsorptivity();
}

bool FoundationKivaSettings::setGroundThermalAbsorptivity(double groundThermalAbsorptivity) {
  return getImpl<detail::FoundationKivaSettings_Impl>()->setGroundThermalAbsorptivity(groundThermalAbsorptivity);
}

void FoundationKivaSettings::resetGroundThermalAbsorptivity() {
  getImpl<detail::FoundationKivaSettings_Impl>()->resetGroundThermalAbsorptivity();
}

bool FoundationKivaSettings::setGroundSurfaceRoughness(double groundSurfaceRoughness) {
  return getImpl<detail::FoundationKivaSettings_Impl>()->setGroundSurfaceRoughness(groundSurfaceRoughness);
}

void FoundationKivaSettings::resetGroundSurfaceRoughness() {
  getImpl<detail::FoundationKivaSettings_Impl>()->resetGroundSurfaceRoughness();
}

bool FoundationKivaSettings::setFarFieldWidth(double farFieldWidth) {
  return getImpl<detail::FoundationKivaSettings_Impl>()->setFarFieldWidth(farFieldWidth);
}

void FoundationKivaSettings::resetFarFieldWidth() {
  getImpl<detail::FoundationKivaSettings_Impl>()->resetFarFieldWidth();
}

bool FoundationKivaSettings::setDeepGroundBoundaryCondition(std::string deepGroundBoundaryCondition) {
  return getImpl<detail::FoundationKivaSettings_Impl>()->setDeepGroundBoundaryCondition(deepGroundBoundaryCondition);
}

void FoundationKivaSettings::resetDeepGroundBoundaryCondition() {
  getImpl<detail::FoundationKivaSettings_Impl>()->resetDeepGroundBoundaryCondition();
}

bool FoundationKivaSettings::setDeepGroundDepth(double deepGroundDepth) {
  return getImpl<detail::FoundationKivaSettings_Impl>()->setDeepGroundDepth(deepGroundDepth);
}

void FoundationKivaSettings::autocalculateDeepGroundDepth() {
  getImpl<detail::FoundationKivaSettings_Impl>()->autocalculateDeepGroundDepth();
}

bool FoundationKivaSettings::setMinimumCellDimension(double minimumCellDimension) {
  return getImpl<detail::FoundationKivaSettings_Impl>()->setMinimumCellDimension(minimumCellDimension);
}

void FoundationKivaSettings::resetMinimumCellDimension() {
  getImpl<detail::FoundationKivaSettings_Impl>()->resetMinimumCellDimension();
}

bool FoundationKivaSettings::setMaximumCellGrowthCoefficient(double maximumCellGrowthCoefficient) {
  return getImpl<detail::FoundationKivaSettings_Impl>()->setMaximumCellGrowthCoefficient(maximumCellGrowthCoefficient);
}

void FoundationKivaSettings::resetMaximumCellGrowthCoefficient() {
  getImpl<detail::FoundationKivaSettings_Impl>()->resetMaximumCellGrowthCoefficient();
}

bool FoundationKivaSettings::setSimulationTimestep(std::string simulationTimestep) {
  return getImpl<detail::FoundationKivaSettings_Impl>()->setSimulationTimestep(simulationTimestep);
}

void FoundationKivaSettings::resetSimulationTimestep() {
  getImpl<detail::FoundationKivaSettings_Impl>()->resetSimulationTimestep();
}

/// @cond
FoundationKivaSettings::FoundationKivaSettings(std::shared_ptr<detail::FoundationKivaSettings_Impl> impl)
  : ModelObject(std::move(impl))
{}

FoundationKivaSettings::FoundationKivaSettings(Model& model)
  : ModelObject(FoundationKivaSettings::iddObjectType(), model)
{}

/// @endcond

} // model
} // openstudio