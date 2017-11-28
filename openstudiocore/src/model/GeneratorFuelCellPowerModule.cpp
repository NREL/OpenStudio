/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
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
#include "Model.hpp"
#include "Model_Impl.hpp"

#include "GeneratorFuelCell.hpp"
#include "GeneratorFuelCell_Impl.hpp"
#include "GeneratorFuelCellPowerModule.hpp"
#include "GeneratorFuelCellPowerModule_Impl.hpp"

#include "CurveQuadratic.hpp"
#include "CurveQuadratic_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Generator_FuelCell_PowerModule_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  GeneratorFuelCellPowerModule_Impl::GeneratorFuelCellPowerModule_Impl(const IdfObject& idfObject,
                                                                       Model_Impl* model,
                                                                       bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == GeneratorFuelCellPowerModule::iddObjectType());
  }

  GeneratorFuelCellPowerModule_Impl::GeneratorFuelCellPowerModule_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                       Model_Impl* model,
                                                                       bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == GeneratorFuelCellPowerModule::iddObjectType());
  }

  GeneratorFuelCellPowerModule_Impl::GeneratorFuelCellPowerModule_Impl(const GeneratorFuelCellPowerModule_Impl& other,
                                                                       Model_Impl* model,
                                                                       bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& GeneratorFuelCellPowerModule_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType GeneratorFuelCellPowerModule_Impl::iddObjectType() const {
    return GeneratorFuelCellPowerModule::iddObjectType();
  }

  // This will clone both the GeneratorFuelCellExhaustGasToWaterHeatExchanger and its linked GeneratorFuelCell
  // and will return a reference to the GeneratorMicroTurbineHeatRecovery
  ModelObject GeneratorFuelCellPowerModule_Impl::clone(Model model) const {

    // We call the parent generator's Clone method which will clone both the fuelCell and fuelCellHX
    GeneratorFuelCell fs = fuelCell();
    GeneratorFuelCell fsClone = fs.clone(model).cast<GeneratorFuelCell>();

    // We get the clone of the parent generator's MTHR so we can return that
    GeneratorFuelCellPowerModule hxClone = fsClone.powerModule();


    return hxClone;
  }

  std::vector<IddObjectType> GeneratorFuelCellPowerModule_Impl::allowableChildTypes() const {
    std::vector<IddObjectType> result;
    result.push_back(IddObjectType::OS_Curve_Quadratic);
    return result;
  }

  // Returns the children object
  std::vector<ModelObject> GeneratorFuelCellPowerModule_Impl::children() const {
    std::vector<ModelObject> result;
    boost::optional<CurveQuadratic> curveQ;

    if (curveQ = efficiencyCurve()) {
      result.push_back(curveQ.get());
    }
    if (curveQ = skinLossQuadraticCurve()) {
      result.push_back(curveQ.get());
    }

    return result;
  }

  // Get the parent GeneratorFuelCell
  GeneratorFuelCell GeneratorFuelCellPowerModule_Impl::fuelCell() const {

    boost::optional<GeneratorFuelCell> value;
    for (const GeneratorFuelCell& fc : this->model().getConcreteModelObjects<GeneratorFuelCell>()) {
      if (boost::optional<GeneratorFuelCellPowerModule> fcHX = fc.powerModule()) {
        if (fcHX->handle() == this->handle()) {
          value = fc;
        }
      }
    }
    OS_ASSERT(value);
    return value.get();

  }

  std::string GeneratorFuelCellPowerModule_Impl::efficiencyCurveMode() const {
    boost::optional<std::string> value = getString(OS_Generator_FuelCell_PowerModuleFields::EfficiencyCurveMode,true);
    if (!value) {
      LOG_AND_THROW(" does not have an Efficiency Curve Mode defined.");
    }
    return value.get();
  }

  CurveQuadratic GeneratorFuelCellPowerModule_Impl::efficiencyCurve() const {
    boost::optional<CurveQuadratic> value = optionalEfficiencyCurve();
    if (!value) {
      LOG_AND_THROW(" does not have an Efficiency Curve attached.");
    }
    return value.get();
  }

  double GeneratorFuelCellPowerModule_Impl::nominalEfficiency() const {
    boost::optional<double> value = getDouble(OS_Generator_FuelCell_PowerModuleFields::NominalEfficiency,true);
    if (!value) {
      LOG_AND_THROW(" does not have an Nominal Efficiency.");
    }
    return value.get();
  }

  double GeneratorFuelCellPowerModule_Impl::nominalElectricalPower() const {
    boost::optional<double> value = getDouble(OS_Generator_FuelCell_PowerModuleFields::NominalElectricalPower, true);
    if (!value) {
      LOG_AND_THROW(" does not have an Nominal Electrical Power.");
    }
    return value.get();
  }

  double GeneratorFuelCellPowerModule_Impl::numberofStopsatStartofSimulation() const {
    boost::optional<double> value = getDouble(OS_Generator_FuelCell_PowerModuleFields::NumberofStopsatStartofSimulation, true);
    if (!value) {
      LOG_AND_THROW(" does not have numberofStopsatStartofSimulation.");
    }
    return value.get();
  }

  double GeneratorFuelCellPowerModule_Impl::cyclingPerformanceDegradationCoefficient() const {
    boost::optional<double> value = getDouble(OS_Generator_FuelCell_PowerModuleFields::CyclingPerformanceDegradationCoefficient, true);
    if (!value) {
      LOG_AND_THROW(" does not have cyclingPerformanceDegradationCoefficient.");
    }
    return value.get();
  }

  double GeneratorFuelCellPowerModule_Impl::numberofRunHoursatBeginningofSimulation() const {
    boost::optional<double> value = getDouble(OS_Generator_FuelCell_PowerModuleFields::NumberofRunHoursatBeginningofSimulation, true);
    if (!value) {
      LOG_AND_THROW(" does not have numberofRunHoursatBeginningofSimulation.");
    }
    return value.get();
  }

  double GeneratorFuelCellPowerModule_Impl::accumulatedRunTimeDegradationCoefficient() const {
    boost::optional<double> value = getDouble(OS_Generator_FuelCell_PowerModuleFields::AccumulatedRunTimeDegradationCoefficient, true);
    if (!value) {
      LOG_AND_THROW(" does not have accumulatedRunTimeDegradationCoefficient.");
    }
    return value.get();
  }

  double GeneratorFuelCellPowerModule_Impl::runTimeDegradationInitiationTimeThreshold() const {
    boost::optional<double> value = getDouble(OS_Generator_FuelCell_PowerModuleFields::RunTimeDegradationInitiationTimeThreshold, true);
    if (!value) {
      LOG_AND_THROW(" does not have runTimeDegradationInitiationTimeThreshold.");
    }
    return value.get();
  }

  double GeneratorFuelCellPowerModule_Impl::powerUpTransientLimit() const {
    boost::optional<double> value = getDouble(OS_Generator_FuelCell_PowerModuleFields::PowerUpTransientLimit, true);
    if (!value) {
      LOG_AND_THROW(" does not have powerUpTransientLimit.");
    }
    return value.get();
  }

  double GeneratorFuelCellPowerModule_Impl::powerDownTransientLimit() const {
    boost::optional<double> value = getDouble(OS_Generator_FuelCell_PowerModuleFields::PowerDownTransientLimit, true);
    if (!value) {
      LOG_AND_THROW(" does not have powerDownTransientLimit.");
    }
    return value.get();
  }

  double GeneratorFuelCellPowerModule_Impl::startUpTime() const {
    boost::optional<double> value = getDouble(OS_Generator_FuelCell_PowerModuleFields::StartUpTime, true);
    if (!value) {
      LOG_AND_THROW(" does not have startUpTime.");
    }
    return value.get();
  }

  double GeneratorFuelCellPowerModule_Impl::startUpFuel() const {
    boost::optional<double> value = getDouble(OS_Generator_FuelCell_PowerModuleFields::StartUpFuel, true);
    if (!value) {
      LOG_AND_THROW(" does not have startUpFuel.");
    }
    return value.get();
  }

  double GeneratorFuelCellPowerModule_Impl::startUpElectricityConsumption() const {
    boost::optional<double> value = getDouble(OS_Generator_FuelCell_PowerModuleFields::StartUpElectricityConsumption, true);
    if (!value) {
      LOG_AND_THROW(" does not have startUpElectricityConsumption.");
    }
    return value.get();
  }

  double GeneratorFuelCellPowerModule_Impl::startUpElectricityProduced() const {
    boost::optional<double> value = getDouble(OS_Generator_FuelCell_PowerModuleFields::StartUpElectricityProduced, true);
    if (!value) {
      LOG_AND_THROW(" does not have startUpElectricityProduced.");
    }
    return value.get();
  }

  double GeneratorFuelCellPowerModule_Impl::shutDownTime() const {
    boost::optional<double> value = getDouble(OS_Generator_FuelCell_PowerModuleFields::ShutDownTime, true);
    if (!value) {
      LOG_AND_THROW(" does not have shutDownTime.");
    }
    return value.get();
  }

  double GeneratorFuelCellPowerModule_Impl::shutDownFuel() const {
    boost::optional<double> value = getDouble(OS_Generator_FuelCell_PowerModuleFields::ShutDownFuel, true);
    if (!value) {
      LOG_AND_THROW(" does not have shutDownFuel.");
    }
    return value.get();
  }

  double GeneratorFuelCellPowerModule_Impl::shutDownElectricityConsumption() const {
    boost::optional<double> value = getDouble(OS_Generator_FuelCell_PowerModuleFields::ShutDownElectricityConsumption, true);
    if (!value) {
      LOG_AND_THROW(" does not have shutDownElectricityConsumption.");
    }
    return value.get();
  }

  double GeneratorFuelCellPowerModule_Impl::ancillaryElectricityConstantTerm() const {
    boost::optional<double> value = getDouble(OS_Generator_FuelCell_PowerModuleFields::AncillaryElectricityConstantTerm, true);
    if (!value) {
      LOG_AND_THROW(" does not have ancillaryElectricityConstantTerm.");
    }
    return value.get();
  }

  double GeneratorFuelCellPowerModule_Impl::ancillaryElectricityLinearTerm() const {
    boost::optional<double> value = getDouble(OS_Generator_FuelCell_PowerModuleFields::AncillaryElectricityLinearTerm, true);
    if (!value) {
      LOG_AND_THROW(" does not have ancillaryElectricityLinearTerm.");
    }
    return value.get();
  }

  std::string GeneratorFuelCellPowerModule_Impl::skinLossCalculationMode() const {
    boost::optional<std::string> value = getString(OS_Generator_FuelCell_PowerModuleFields::SkinLossCalculationMode,true);
    if (!value) {
      LOG_AND_THROW(" does not have skinLossCalculationMode.");
    }
    return value.get();
  }

  boost::optional<ThermalZone> GeneratorFuelCellPowerModule_Impl::zone() const {
    return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_Generator_FuelCell_PowerModuleFields::ZoneName);
  }

  double GeneratorFuelCellPowerModule_Impl::skinLossRadiativeFraction() const {
    boost::optional<double> value = getDouble(OS_Generator_FuelCell_PowerModuleFields::SkinLossRadiativeFraction, true);
    if (!value) {
      LOG_AND_THROW(" does not have skinLossRadiativeFraction.");
    }
    return value.get();
  }

  double GeneratorFuelCellPowerModule_Impl::constantSkinLossRate() const {
    boost::optional<double> value = getDouble(OS_Generator_FuelCell_PowerModuleFields::ConstantSkinLossRate, true);
    if (!value) {
      LOG_AND_THROW(" does not have constantSkinLossRate.");
    }
    return value.get();
  }

  double GeneratorFuelCellPowerModule_Impl::skinLossUFactorTimesAreaTerm() const {
    boost::optional<double> value = getDouble(OS_Generator_FuelCell_PowerModuleFields::SkinLossUFactorTimesAreaTerm, true);
    if (!value) {
      LOG_AND_THROW(" does not have skinLossUFactorTimesAreaTerm.");
    }
    return value.get();
  }

  boost::optional<CurveQuadratic> GeneratorFuelCellPowerModule_Impl::skinLossQuadraticCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveQuadratic>(OS_Generator_FuelCell_PowerModuleFields::SkinLossQuadraticCurveName);
  }

  double GeneratorFuelCellPowerModule_Impl::dilutionAirFlowRate() const {
    boost::optional<double> value = getDouble(OS_Generator_FuelCell_PowerModuleFields::DilutionAirFlowRate, true);
    if (!value) {
      LOG_AND_THROW(" does not have dilutionAirFlowRate.");
    }
    return value.get();
  }

  double GeneratorFuelCellPowerModule_Impl::stackHeatlosstoDilutionAir() const {
    boost::optional<double> value = getDouble(OS_Generator_FuelCell_PowerModuleFields::StackHeatlosstoDilutionAir, true);
    if (!value) {
      LOG_AND_THROW(" does not have stackHeatlosstoDilutionAir.");
    }
    return value.get();
  }

  boost::optional<Node> GeneratorFuelCellPowerModule_Impl::dilutionInletAirNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_Generator_FuelCell_PowerModuleFields::DilutionInletAirNodeName);
  }

  boost::optional<Node> GeneratorFuelCellPowerModule_Impl::dilutionOutletAirNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_Generator_FuelCell_PowerModuleFields::DilutionOutletAirNodeName);
  }

  double GeneratorFuelCellPowerModule_Impl::minimumOperatingPoint() const {
    boost::optional<double> value = getDouble(OS_Generator_FuelCell_PowerModuleFields::MinimumOperatingPoint, true);
    if (!value) {
      LOG_AND_THROW(" does not have minimumOperatingPoint.");
    }
    return value.get();
  }

  double GeneratorFuelCellPowerModule_Impl::maximumOperatingPoint() const {
    boost::optional<double> value = getDouble(OS_Generator_FuelCell_PowerModuleFields::MaximumOperatingPoint, true);
    if (!value) {
      LOG_AND_THROW(" does not have maximumOperatingPoint.");
    }
    return value.get();
  }

  bool GeneratorFuelCellPowerModule_Impl::setEfficiencyCurveMode(const std::string& efficiencyCurveMode) {
    bool result = setString(OS_Generator_FuelCell_PowerModuleFields::EfficiencyCurveMode, efficiencyCurveMode);
    return result;
  }

  void GeneratorFuelCellPowerModule_Impl::resetEfficiencyCurveMode() {
    bool result = setString(OS_Generator_FuelCell_PowerModuleFields::EfficiencyCurveMode, "Annex42");
    OS_ASSERT(result);
  }

  bool GeneratorFuelCellPowerModule_Impl::setEfficiencyCurve(const CurveQuadratic& quadraticCurve) {
    bool result = setPointer(OS_Generator_FuelCell_PowerModuleFields::EfficiencyCurveName, quadraticCurve.handle());
    return result;
  }

  void GeneratorFuelCellPowerModule_Impl::setNominalEfficiency(double nominalEfficiency) {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::NominalEfficiency, nominalEfficiency);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::resetNominalEfficiency() {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::NominalEfficiency, 1);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::setNominalElectricalPower(double nominalElectricalPower) {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::NominalElectricalPower, nominalElectricalPower);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::resetNominalElectricalPower() {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::NominalElectricalPower, 3400);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::setNumberofStopsatStartofSimulation(double numberofStopsatStartofSimulation) {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::NumberofStopsatStartofSimulation, numberofStopsatStartofSimulation);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::resetNumberofStopsatStartofSimulation() {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::NumberofStopsatStartofSimulation, 0);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::setCyclingPerformanceDegradationCoefficient(double cyclingPerformanceDegradationCoefficient) {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::CyclingPerformanceDegradationCoefficient, cyclingPerformanceDegradationCoefficient);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::resetCyclingPerformanceDegradationCoefficient() {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::CyclingPerformanceDegradationCoefficient, 0);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::setNumberofRunHoursatBeginningofSimulation(double numberofRunHoursatBeginningofSimulation) {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::NumberofRunHoursatBeginningofSimulation, numberofRunHoursatBeginningofSimulation);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::resetNumberofRunHoursatBeginningofSimulation() {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::NumberofRunHoursatBeginningofSimulation, 0);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::setAccumulatedRunTimeDegradationCoefficient(double accumulatedRunTimeDegradationCoefficient) {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::AccumulatedRunTimeDegradationCoefficient, accumulatedRunTimeDegradationCoefficient);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::resetAccumulatedRunTimeDegradationCoefficient() {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::AccumulatedRunTimeDegradationCoefficient, 0);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::setRunTimeDegradationInitiationTimeThreshold(double runTimeDegradationInitiationTimeThreshold) {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::RunTimeDegradationInitiationTimeThreshold, runTimeDegradationInitiationTimeThreshold);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::resetRunTimeDegradationInitiationTimeThreshold() {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::RunTimeDegradationInitiationTimeThreshold, 10000);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::setPowerUpTransientLimit(double powerUpTransientLimit) {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::PowerUpTransientLimit, powerUpTransientLimit);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::resetPowerUpTransientLimit() {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::PowerUpTransientLimit, 1.4);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::setPowerDownTransientLimit(double powerDownTransientLimit) {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::PowerDownTransientLimit, powerDownTransientLimit);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::resetPowerDownTransientLimit() {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::PowerDownTransientLimit, 0.2);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::setStartUpTime(double startUpTime) {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::StartUpTime, startUpTime);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::resetStartUpTime() {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::StartUpTime, 0);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::setStartUpFuel(double startUpFuel) {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::StartUpFuel, startUpFuel);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::resetStartUpFuel() {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::StartUpFuel, 0);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::setStartUpElectricityConsumption(double startUpElectricityConsumption) {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::StartUpElectricityConsumption, startUpElectricityConsumption);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::resetStartUpElectricityConsumption() {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::StartUpElectricityConsumption, 0);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::setStartUpElectricityProduced(double startUpElectricityProduced) {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::StartUpElectricityProduced, startUpElectricityProduced);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::resetStartUpElectricityProduced() {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::StartUpElectricityProduced, 0);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::setShutDownTime(double shutDownTime) {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::ShutDownTime, shutDownTime);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::resetShutDownTime() {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::ShutDownTime, 0);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::setShutDownFuel(double shutDownFuel) {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::ShutDownFuel, shutDownFuel);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::resetShutDownFuel() {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::ShutDownFuel, 0);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::setShutDownElectricityConsumption(double shutDownElectricityConsumption) {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::ShutDownElectricityConsumption, shutDownElectricityConsumption);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::resetShutDownElectricityConsumption() {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::ShutDownElectricityConsumption, 0);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::setAncillaryElectricityConstantTerm(double ancillaryElectricityConstantTerm) {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::AncillaryElectricityConstantTerm, ancillaryElectricityConstantTerm);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::resetAncillaryElectricityConstantTerm() {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::AncillaryElectricityConstantTerm, 0);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::setAncillaryElectricityLinearTerm(double ancillaryElectricityLinearTerm) {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::AncillaryElectricityLinearTerm, ancillaryElectricityLinearTerm);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::resetAncillaryElectricityLinearTerm() {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::AncillaryElectricityLinearTerm, 0);
    OS_ASSERT(result);
  }

  bool GeneratorFuelCellPowerModule_Impl::setSkinLossCalculationMode(const std::string& skinLossCalculationMode) {
    bool result = setString(OS_Generator_FuelCell_PowerModuleFields::SkinLossCalculationMode, skinLossCalculationMode);
    return result;
  }

  void GeneratorFuelCellPowerModule_Impl::resetSkinLossCalculationMode() {
    bool result = setString(OS_Generator_FuelCell_PowerModuleFields::SkinLossCalculationMode, "ConstantRate");
    OS_ASSERT(result);
  }

  bool GeneratorFuelCellPowerModule_Impl::setZone(const ThermalZone& zone) {
    bool result = setPointer(OS_Generator_FuelCell_PowerModuleFields::ZoneName, zone.handle());
    return result;
  }

  void GeneratorFuelCellPowerModule_Impl::resetZone() {
    bool result = setString(OS_Generator_FuelCell_PowerModuleFields::ZoneName, "");
    OS_ASSERT(result);
  }

  bool GeneratorFuelCellPowerModule_Impl::setSkinLossRadiativeFraction(double skinLossRadiativeFraction) {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::SkinLossRadiativeFraction, skinLossRadiativeFraction);
    return result;
  }

  void GeneratorFuelCellPowerModule_Impl::resetSkinLossRadiativeFraction() {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::SkinLossRadiativeFraction, 1);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::setConstantSkinLossRate(double constantSkinLossRate) {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::ConstantSkinLossRate, constantSkinLossRate);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::resetConstantSkinLossRate() {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::ConstantSkinLossRate, 0);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::setSkinLossUFactorTimesAreaTerm(double skinLossUFactorTimesAreaTerm) {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::SkinLossUFactorTimesAreaTerm, skinLossUFactorTimesAreaTerm);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::resetSkinLossUFactorTimesAreaTerm() {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::SkinLossUFactorTimesAreaTerm, 1);
    OS_ASSERT(result);
  }

  bool GeneratorFuelCellPowerModule_Impl::setSkinLossQuadraticCurve(const CurveQuadratic& quadraticCurves) {
    bool result = setPointer(OS_Generator_FuelCell_PowerModuleFields::SkinLossQuadraticCurveName, quadraticCurves.handle());
    return result;
  }

  void GeneratorFuelCellPowerModule_Impl::resetSkinLossQuadraticCurve() {
    bool result = setString(OS_Generator_FuelCell_PowerModuleFields::SkinLossQuadraticCurveName, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::setDilutionAirFlowRate(double dilutionAirFlowRate) {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::DilutionAirFlowRate, dilutionAirFlowRate);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::resetDilutionAirFlowRate() {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::DilutionAirFlowRate, 0);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::setStackHeatlosstoDilutionAir(double stackHeatlosstoDilutionAir) {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::StackHeatlosstoDilutionAir, stackHeatlosstoDilutionAir);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::resetStackHeatlosstoDilutionAir() {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::StackHeatlosstoDilutionAir, 0);
    OS_ASSERT(result);
  }

  bool GeneratorFuelCellPowerModule_Impl::setDilutionInletAirNode(const Node& node) {
    bool result = setPointer(OS_Generator_FuelCell_PowerModuleFields::DilutionInletAirNodeName, node.handle());
    return result;
  }

  void GeneratorFuelCellPowerModule_Impl::resetDilutionInletAirNode() {
    bool result = setString(OS_Generator_FuelCell_PowerModuleFields::DilutionInletAirNodeName, "");
    OS_ASSERT(result);
  }

  bool GeneratorFuelCellPowerModule_Impl::setDilutionOutletAirNode(const Node& node) {
    bool result = setPointer(OS_Generator_FuelCell_PowerModuleFields::DilutionOutletAirNodeName, node.handle());
    return result;
  }

  void GeneratorFuelCellPowerModule_Impl::resetDilutionOutletAirNode() {
    bool result = setString(OS_Generator_FuelCell_PowerModuleFields::DilutionOutletAirNodeName, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::setMinimumOperatingPoint(double minimumOperatingPoint) {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::MinimumOperatingPoint, minimumOperatingPoint);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::resetMinimumOperatingPoint() {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::MinimumOperatingPoint, 0);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::setMaximumOperatingPoint(double maximumOperatingPoint) {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::MaximumOperatingPoint, maximumOperatingPoint);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::resetMaximumOperatingPoint() {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::MaximumOperatingPoint, 0);
    OS_ASSERT(result);
  }

  boost::optional<CurveQuadratic> GeneratorFuelCellPowerModule_Impl::optionalEfficiencyCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveQuadratic>(OS_Generator_FuelCell_PowerModuleFields::EfficiencyCurveName);
  }

} // detail

GeneratorFuelCellPowerModule::GeneratorFuelCellPowerModule(const Model& model,
                                                           const ThermalZone& heatlossZone,
                                                           const Node& dilutionInletAirNode,
                                                           const Node& dilutionOutletAirNode)
  : ModelObject(GeneratorFuelCellPowerModule::iddObjectType(), model) {
  OS_ASSERT(getImpl<detail::GeneratorFuelCellPowerModule_Impl>());
  setEfficiencyCurveMode("Annex42");
  //create default curve
  CurveQuadratic curveQuadratic(model);
  curveQuadratic.setCoefficient1Constant(1);
  curveQuadratic.setCoefficient2x(0);
  curveQuadratic.setCoefficient3xPOW2(0);
  curveQuadratic.setMinimumValueofx(0);
  curveQuadratic.setMaximumValueofx(1);

  bool ok = setEfficiencyCurve(curveQuadratic);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s efficiencyCurve to "
      << curveQuadratic.briefDescription() << ".");
  }
  setNominalEfficiency(1.0);
  setNominalElectricalPower(3400);
  setNumberofStopsatStartofSimulation(0);
  setCyclingPerformanceDegradationCoefficient(0.0);
  setNumberofRunHoursatBeginningofSimulation(0);
  setAccumulatedRunTimeDegradationCoefficient(0.0);
  setRunTimeDegradationInitiationTimeThreshold(10000);
  setPowerUpTransientLimit(1.4);
  setPowerDownTransientLimit(0.2);
  setStartUpTime(0.0);
  setStartUpFuel(0.2);
  setStartUpElectricityConsumption(0);
  setStartUpElectricityProduced(0.0);
  setShutDownTime(0.0);
  setShutDownFuel(0.2);
  setShutDownElectricityConsumption(0);
  setAncillaryElectricityConstantTerm(0.0);
  setAncillaryElectricityLinearTerm(0.0);
  ok = setZone(heatlossZone);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s heat loss zone to "
      << heatlossZone.briefDescription() << ".");
  }
  setSkinLossCalculationMode("ConstantRate");
  setSkinLossRadiativeFraction(0.6392);
  setConstantSkinLossRate(729);
  setSkinLossUFactorTimesAreaTerm(0.0);

  ok = setSkinLossQuadraticCurve(curveQuadratic);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s skin Loss Curve to "
      << curveQuadratic.briefDescription() << ".");
  }

  setDilutionAirFlowRate(0.006156);
  setStackHeatlosstoDilutionAir(2307);

  ok = setDilutionInletAirNode(dilutionInletAirNode);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s dilution inlet air node to "
      << dilutionInletAirNode.briefDescription() << ".");
  }
  ok = setDilutionOutletAirNode(dilutionOutletAirNode);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s dilution outlet air node to "
      << dilutionOutletAirNode.briefDescription() << ".");
  }
  setMinimumOperatingPoint(3010);
  setMaximumOperatingPoint(3728);
}


GeneratorFuelCellPowerModule::GeneratorFuelCellPowerModule(const Model& model,
                                                           const CurveQuadratic& efficiencyCurve,
                                                           const ThermalZone& heatlossZone,
                                                           const Node& dilutionInletAirNode,
                                                           const Node& dilutionOutletAirNode,
                                                           const CurveQuadratic& skinlossCurve)
  : ModelObject(GeneratorFuelCellPowerModule::iddObjectType(), model) {
  OS_ASSERT(getImpl<detail::GeneratorFuelCellPowerModule_Impl>());
  setEfficiencyCurveMode("Annex42");
  bool ok = setEfficiencyCurve(efficiencyCurve);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s efficiencyCurve to "
      << efficiencyCurve.briefDescription() << ".");
  }
  setNominalEfficiency(1.0);
  setNominalElectricalPower(3400);
  setNumberofStopsatStartofSimulation(0);
  setCyclingPerformanceDegradationCoefficient(0.0);
  setNumberofRunHoursatBeginningofSimulation(0);
  setAccumulatedRunTimeDegradationCoefficient(0.0);
  setRunTimeDegradationInitiationTimeThreshold(10000);
  setPowerUpTransientLimit(1.4);
  setPowerDownTransientLimit(0.2);
  setStartUpTime(0.0);
  setStartUpFuel(0.2);
  setStartUpElectricityConsumption(0);
  setStartUpElectricityProduced(0.0);
  setShutDownTime(0.0);
  setShutDownFuel(0.2);
  setShutDownElectricityConsumption(0);
  setAncillaryElectricityConstantTerm(0.0);
  setAncillaryElectricityLinearTerm(0.0);
  ok = setZone(heatlossZone);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s heatloss zone to "
      << heatlossZone.briefDescription() << ".");
  }
  setSkinLossCalculationMode("ConstantRate");
  setSkinLossRadiativeFraction(0.6392);
  setConstantSkinLossRate(729);
  setSkinLossUFactorTimesAreaTerm(0.0);

  ok = setSkinLossQuadraticCurve(skinlossCurve);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s skin loss curve to "
      << skinlossCurve.briefDescription() << ".");
  }

  setDilutionAirFlowRate(0.006156);
  setStackHeatlosstoDilutionAir(2307);

  ok = setDilutionInletAirNode(dilutionInletAirNode);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s dilution inlet air node to "
      << dilutionInletAirNode.briefDescription() << ".");
  }
  ok = setDilutionOutletAirNode(dilutionOutletAirNode);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s dilution outlet air node to "
      << dilutionOutletAirNode.briefDescription() << ".");
  }
  setMinimumOperatingPoint(3010);
  setMaximumOperatingPoint(3728);
}

GeneratorFuelCellPowerModule::GeneratorFuelCellPowerModule(const Model& model,
                                                           const CurveQuadratic& efficiencyCurve,
                                                           const ThermalZone& heatlossZone,
                                                           const Node& dilutionInletAirNode,
                                                           const Node& dilutionOutletAirNode)
  : ModelObject(GeneratorFuelCellPowerModule::iddObjectType(), model) {
  OS_ASSERT(getImpl<detail::GeneratorFuelCellPowerModule_Impl>());
  setEfficiencyCurveMode("Annex42");
  bool ok = setEfficiencyCurve(efficiencyCurve);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s efficiencyCurve to "
      << efficiencyCurve.briefDescription() << ".");
  }
  setNominalEfficiency(1.0);
  setNominalElectricalPower(3400);
  setNumberofStopsatStartofSimulation(0);
  setCyclingPerformanceDegradationCoefficient(0.0);
  setNumberofRunHoursatBeginningofSimulation(0);
  setAccumulatedRunTimeDegradationCoefficient(0.0);
  setRunTimeDegradationInitiationTimeThreshold(10000);
  setPowerUpTransientLimit(1.4);
  setPowerDownTransientLimit(0.2);
  setStartUpTime(0.0);
  setStartUpFuel(0.2);
  setStartUpElectricityConsumption(0);
  setStartUpElectricityProduced(0.0);
  setShutDownTime(0.0);
  setShutDownFuel(0.2);
  setShutDownElectricityConsumption(0);
  setAncillaryElectricityConstantTerm(0.0);
  setAncillaryElectricityLinearTerm(0.0);
  ok = setZone(heatlossZone);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s heat loss zone to "
      << heatlossZone.briefDescription() << ".");
  }
  setSkinLossCalculationMode("ConstantRate");
  setSkinLossRadiativeFraction(0.6392);
  setConstantSkinLossRate(729);
  setSkinLossUFactorTimesAreaTerm(0.0);

  setDilutionAirFlowRate(0.006156);
  setStackHeatlosstoDilutionAir(2307);

  ok = setDilutionInletAirNode(dilutionInletAirNode);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s dilution inlet air node to "
      << dilutionInletAirNode.briefDescription() << ".");
  }
  ok = setDilutionOutletAirNode(dilutionOutletAirNode);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s dilution outlet air node to "
      << dilutionOutletAirNode.briefDescription() << ".");
  }
  setMinimumOperatingPoint(3010);
  setMaximumOperatingPoint(3728);
}

GeneratorFuelCellPowerModule::GeneratorFuelCellPowerModule(const Model& model, const CurveQuadratic& efficiencyCurve)
  : ModelObject(GeneratorFuelCellPowerModule::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::GeneratorFuelCellPowerModule_Impl>());
  setEfficiencyCurveMode("Annex42");
  bool ok = setEfficiencyCurve(efficiencyCurve);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s efficiencyCurve to "
      << efficiencyCurve.briefDescription() << ".");
  }
  setNominalEfficiency(1.0);
  setNominalElectricalPower(3400);
  setNumberofStopsatStartofSimulation(0);
  setCyclingPerformanceDegradationCoefficient(0.0);
  setNumberofRunHoursatBeginningofSimulation(0);
  setAccumulatedRunTimeDegradationCoefficient(0.0);
  setRunTimeDegradationInitiationTimeThreshold(10000);
  setPowerUpTransientLimit(1.4);
  setPowerDownTransientLimit(0.2);
  setStartUpTime(0.0);
  setStartUpFuel(0.2);
  setStartUpElectricityConsumption(0);
  setStartUpElectricityProduced(0.0);
  setShutDownTime(0.0);
  setShutDownFuel(0.2);
  setShutDownElectricityConsumption(0);
  setAncillaryElectricityConstantTerm(0.0);
  setAncillaryElectricityLinearTerm(0.0);
  //TODO
  //setZone();
  setSkinLossCalculationMode("ConstantRate");
  setSkinLossRadiativeFraction(0.6392);
  setConstantSkinLossRate(729);
  setSkinLossUFactorTimesAreaTerm(0.0);

  setDilutionAirFlowRate(0.006156);
  setStackHeatlosstoDilutionAir(2307);
  //TODO
  //setDilutionInletAirNode();
  //setDilutionOutletAirNode();
  setMinimumOperatingPoint(3010);
  setMaximumOperatingPoint(3728);
}

GeneratorFuelCellPowerModule::GeneratorFuelCellPowerModule(const Model& model)
  : ModelObject(GeneratorFuelCellPowerModule::iddObjectType(), model) {
  OS_ASSERT(getImpl<detail::GeneratorFuelCellPowerModule_Impl>());
  setEfficiencyCurveMode("Annex42");
  //create default curve
  CurveQuadratic curveQuadratic(model);
  curveQuadratic.setCoefficient1Constant(0.642388);
  curveQuadratic.setCoefficient2x(-0.0001619);
  curveQuadratic.setCoefficient3xPOW2(0.0000000226);
  curveQuadratic.setMinimumValueofx(0);
  curveQuadratic.setMaximumValueofx(10000);
  curveQuadratic.setName("Power Module Efficiency Curve");
  bool ok = setEfficiencyCurve(curveQuadratic);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s efficiencyCurve to "
      << curveQuadratic.briefDescription() << ".");
  }
  setNominalEfficiency(1.0);
  setNominalElectricalPower(3400);
  setNumberofStopsatStartofSimulation(0);
  setCyclingPerformanceDegradationCoefficient(0.0);
  setNumberofRunHoursatBeginningofSimulation(0);
  setAccumulatedRunTimeDegradationCoefficient(0.0);
  setRunTimeDegradationInitiationTimeThreshold(10000);
  setPowerUpTransientLimit(1.4);
  setPowerDownTransientLimit(0.2);
  setStartUpTime(0.0);
  setStartUpFuel(0.2);
  setStartUpElectricityConsumption(0);
  setStartUpElectricityProduced(0.0);
  setShutDownTime(0.0);
  setShutDownFuel(0.2);
  setShutDownElectricityConsumption(0);
  setAncillaryElectricityConstantTerm(0.0);
  setAncillaryElectricityLinearTerm(0.0);
  //TODO
  //setZone();
  setSkinLossCalculationMode("ConstantRate");
  setSkinLossRadiativeFraction(0.6392);
  setConstantSkinLossRate(729);
  setSkinLossUFactorTimesAreaTerm(0.0);

  setDilutionAirFlowRate(0.006156);
  setStackHeatlosstoDilutionAir(2307);
  //TODO
  //setDilutionInletAirNode();
  //setDilutionOutletAirNode();
  setMinimumOperatingPoint(3010);
  setMaximumOperatingPoint(3728);

  CurveQuadratic curveQ2(model);
  curveQ2.setCoefficient1Constant(0);
  curveQ2.setCoefficient2x(0);
  curveQ2.setCoefficient3xPOW2(0);
  curveQ2.setMinimumValueofx(-1.0e10);
  curveQ2.setMaximumValueofx(1.0e10);
  curveQ2.setName("Skin Loss Curve");
  setSkinLossQuadraticCurve(curveQ2);
}

IddObjectType GeneratorFuelCellPowerModule::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Generator_FuelCell_PowerModule);
}

std::vector<std::string> GeneratorFuelCellPowerModule::efficiencyCurveModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Generator_FuelCell_PowerModuleFields::EfficiencyCurveMode);
}

std::vector<std::string> GeneratorFuelCellPowerModule::skinLossCalculationModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Generator_FuelCell_PowerModuleFields::SkinLossCalculationMode);
}

std::string GeneratorFuelCellPowerModule::efficiencyCurveMode() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->efficiencyCurveMode();
}

CurveQuadratic GeneratorFuelCellPowerModule::efficiencyCurve() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->efficiencyCurve();
}

double GeneratorFuelCellPowerModule::nominalEfficiency() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->nominalEfficiency();
}

double GeneratorFuelCellPowerModule::nominalElectricalPower() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->nominalElectricalPower();
}

double GeneratorFuelCellPowerModule::numberofStopsatStartofSimulation() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->numberofStopsatStartofSimulation();
}

double GeneratorFuelCellPowerModule::cyclingPerformanceDegradationCoefficient() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->cyclingPerformanceDegradationCoefficient();
}

double GeneratorFuelCellPowerModule::numberofRunHoursatBeginningofSimulation() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->numberofRunHoursatBeginningofSimulation();
}

double GeneratorFuelCellPowerModule::accumulatedRunTimeDegradationCoefficient() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->accumulatedRunTimeDegradationCoefficient();
}

double GeneratorFuelCellPowerModule::runTimeDegradationInitiationTimeThreshold() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->runTimeDegradationInitiationTimeThreshold();
}

double GeneratorFuelCellPowerModule::powerUpTransientLimit() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->powerUpTransientLimit();
}

double GeneratorFuelCellPowerModule::powerDownTransientLimit() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->powerDownTransientLimit();
}

double GeneratorFuelCellPowerModule::startUpTime() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->startUpTime();
}

double GeneratorFuelCellPowerModule::startUpFuel() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->startUpFuel();
}

double GeneratorFuelCellPowerModule::startUpElectricityConsumption() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->startUpElectricityConsumption();
}

double GeneratorFuelCellPowerModule::startUpElectricityProduced() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->startUpElectricityProduced();
}

double GeneratorFuelCellPowerModule::shutDownTime() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->shutDownTime();
}

double GeneratorFuelCellPowerModule::shutDownFuel() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->shutDownFuel();
}

double GeneratorFuelCellPowerModule::shutDownElectricityConsumption() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->shutDownElectricityConsumption();
}

double GeneratorFuelCellPowerModule::ancillaryElectricityConstantTerm() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->ancillaryElectricityConstantTerm();
}

double GeneratorFuelCellPowerModule::ancillaryElectricityLinearTerm() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->ancillaryElectricityLinearTerm();
}

std::string GeneratorFuelCellPowerModule::skinLossCalculationMode() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->skinLossCalculationMode();
}

boost::optional<ThermalZone> GeneratorFuelCellPowerModule::zone() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->zone();
}

double GeneratorFuelCellPowerModule::skinLossRadiativeFraction() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->skinLossRadiativeFraction();
}

double GeneratorFuelCellPowerModule::constantSkinLossRate() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->constantSkinLossRate();
}

double GeneratorFuelCellPowerModule::skinLossUFactorTimesAreaTerm() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->skinLossUFactorTimesAreaTerm();
}

boost::optional<CurveQuadratic> GeneratorFuelCellPowerModule::skinLossQuadraticCurve() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->skinLossQuadraticCurve();
}

double GeneratorFuelCellPowerModule::dilutionAirFlowRate() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->dilutionAirFlowRate();
}

double GeneratorFuelCellPowerModule::stackHeatlosstoDilutionAir() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->stackHeatlosstoDilutionAir();
}

boost::optional<Node> GeneratorFuelCellPowerModule::dilutionInletAirNode() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->dilutionInletAirNode();
}

boost::optional<Node> GeneratorFuelCellPowerModule::dilutionOutletAirNode() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->dilutionOutletAirNode();
}

double GeneratorFuelCellPowerModule::minimumOperatingPoint() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->minimumOperatingPoint();
}

double GeneratorFuelCellPowerModule::maximumOperatingPoint() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->maximumOperatingPoint();
}

bool GeneratorFuelCellPowerModule::setEfficiencyCurveMode(const std::string& efficiencyCurveMode) {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->setEfficiencyCurveMode(efficiencyCurveMode);
}

void GeneratorFuelCellPowerModule::resetEfficiencyCurveMode() {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->resetEfficiencyCurveMode();
}

bool GeneratorFuelCellPowerModule::setEfficiencyCurve(const CurveQuadratic& quadraticCurve) {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->setEfficiencyCurve(quadraticCurve);
}

void GeneratorFuelCellPowerModule::setNominalEfficiency(double nominalEfficiency) {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->setNominalEfficiency(nominalEfficiency);
}

void GeneratorFuelCellPowerModule::resetNominalEfficiency() {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->resetNominalEfficiency();
}

void GeneratorFuelCellPowerModule::setNominalElectricalPower(double nominalElectricalPower) {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->setNominalElectricalPower(nominalElectricalPower);
}

void GeneratorFuelCellPowerModule::resetNominalElectricalPower() {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->resetNominalElectricalPower();
}

void GeneratorFuelCellPowerModule::setNumberofStopsatStartofSimulation(double numberofStopsatStartofSimulation) {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->setNumberofStopsatStartofSimulation(numberofStopsatStartofSimulation);
}

void GeneratorFuelCellPowerModule::resetNumberofStopsatStartofSimulation() {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->resetNumberofStopsatStartofSimulation();
}

void GeneratorFuelCellPowerModule::setCyclingPerformanceDegradationCoefficient(double cyclingPerformanceDegradationCoefficient) {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->setCyclingPerformanceDegradationCoefficient(cyclingPerformanceDegradationCoefficient);
}

void GeneratorFuelCellPowerModule::resetCyclingPerformanceDegradationCoefficient() {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->resetCyclingPerformanceDegradationCoefficient();
}

void GeneratorFuelCellPowerModule::setNumberofRunHoursatBeginningofSimulation(double numberofRunHoursatBeginningofSimulation) {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->setNumberofRunHoursatBeginningofSimulation(numberofRunHoursatBeginningofSimulation);
}

void GeneratorFuelCellPowerModule::resetNumberofRunHoursatBeginningofSimulation() {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->resetNumberofRunHoursatBeginningofSimulation();
}

void GeneratorFuelCellPowerModule::setAccumulatedRunTimeDegradationCoefficient(double accumulatedRunTimeDegradationCoefficient) {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->setAccumulatedRunTimeDegradationCoefficient(accumulatedRunTimeDegradationCoefficient);
}

void GeneratorFuelCellPowerModule::resetAccumulatedRunTimeDegradationCoefficient() {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->resetAccumulatedRunTimeDegradationCoefficient();
}

void GeneratorFuelCellPowerModule::setRunTimeDegradationInitiationTimeThreshold(double runTimeDegradationInitiationTimeThreshold) {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->setRunTimeDegradationInitiationTimeThreshold(runTimeDegradationInitiationTimeThreshold);
}

void GeneratorFuelCellPowerModule::resetRunTimeDegradationInitiationTimeThreshold() {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->resetRunTimeDegradationInitiationTimeThreshold();
}

void GeneratorFuelCellPowerModule::setPowerUpTransientLimit(double powerUpTransientLimit) {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->setPowerUpTransientLimit(powerUpTransientLimit);
}

void GeneratorFuelCellPowerModule::resetPowerUpTransientLimit() {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->resetPowerUpTransientLimit();
}

void GeneratorFuelCellPowerModule::setPowerDownTransientLimit(double powerDownTransientLimit) {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->setPowerDownTransientLimit(powerDownTransientLimit);
}

void GeneratorFuelCellPowerModule::resetPowerDownTransientLimit() {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->resetPowerDownTransientLimit();
}

void GeneratorFuelCellPowerModule::setStartUpTime(double startUpTime) {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->setStartUpTime(startUpTime);
}

void GeneratorFuelCellPowerModule::resetStartUpTime() {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->resetStartUpTime();
}

void GeneratorFuelCellPowerModule::setStartUpFuel(double startUpFuel) {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->setStartUpFuel(startUpFuel);
}

void GeneratorFuelCellPowerModule::resetStartUpFuel() {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->resetStartUpFuel();
}

void GeneratorFuelCellPowerModule::setStartUpElectricityConsumption(double startUpElectricityConsumption) {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->setStartUpElectricityConsumption(startUpElectricityConsumption);
}

void GeneratorFuelCellPowerModule::resetStartUpElectricityConsumption() {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->resetStartUpElectricityConsumption();
}

void GeneratorFuelCellPowerModule::setStartUpElectricityProduced(double startUpElectricityProduced) {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->setStartUpElectricityProduced(startUpElectricityProduced);
}

void GeneratorFuelCellPowerModule::resetStartUpElectricityProduced() {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->resetStartUpElectricityProduced();
}

void GeneratorFuelCellPowerModule::setShutDownTime(double shutDownTime) {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->setShutDownTime(shutDownTime);
}

void GeneratorFuelCellPowerModule::resetShutDownTime() {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->resetShutDownTime();
}

void GeneratorFuelCellPowerModule::setShutDownFuel(double shutDownFuel) {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->setShutDownFuel(shutDownFuel);
}

void GeneratorFuelCellPowerModule::resetShutDownFuel() {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->resetShutDownFuel();
}

void GeneratorFuelCellPowerModule::setShutDownElectricityConsumption(double shutDownElectricityConsumption) {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->setShutDownElectricityConsumption(shutDownElectricityConsumption);
}

void GeneratorFuelCellPowerModule::resetShutDownElectricityConsumption() {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->resetShutDownElectricityConsumption();
}

void GeneratorFuelCellPowerModule::setAncillaryElectricityConstantTerm(double ancillaryElectricityConstantTerm) {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->setAncillaryElectricityConstantTerm(ancillaryElectricityConstantTerm);
}

void GeneratorFuelCellPowerModule::resetAncillaryElectricityConstantTerm() {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->resetAncillaryElectricityConstantTerm();
}

void GeneratorFuelCellPowerModule::setAncillaryElectricityLinearTerm(double ancillaryElectricityLinearTerm) {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->setAncillaryElectricityLinearTerm(ancillaryElectricityLinearTerm);
}

void GeneratorFuelCellPowerModule::resetAncillaryElectricityLinearTerm() {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->resetAncillaryElectricityLinearTerm();
}

bool GeneratorFuelCellPowerModule::setSkinLossCalculationMode(const std::string& skinLossCalculationMode) {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->setSkinLossCalculationMode(skinLossCalculationMode);
}

void GeneratorFuelCellPowerModule::resetSkinLossCalculationMode() {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->resetSkinLossCalculationMode();
}

bool GeneratorFuelCellPowerModule::setZone(const ThermalZone& zone) {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->setZone(zone);
}

void GeneratorFuelCellPowerModule::resetZone() {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->resetZone();
}

bool GeneratorFuelCellPowerModule::setSkinLossRadiativeFraction(double skinLossRadiativeFraction) {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->setSkinLossRadiativeFraction(skinLossRadiativeFraction);
}

void GeneratorFuelCellPowerModule::resetSkinLossRadiativeFraction() {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->resetSkinLossRadiativeFraction();
}

void GeneratorFuelCellPowerModule::setConstantSkinLossRate(double constantSkinLossRate) {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->setConstantSkinLossRate(constantSkinLossRate);
}

void GeneratorFuelCellPowerModule::resetConstantSkinLossRate() {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->resetConstantSkinLossRate();
}

void GeneratorFuelCellPowerModule::setSkinLossUFactorTimesAreaTerm(double skinLossUFactorTimesAreaTerm) {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->setSkinLossUFactorTimesAreaTerm(skinLossUFactorTimesAreaTerm);
}

void GeneratorFuelCellPowerModule::resetSkinLossUFactorTimesAreaTerm() {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->resetSkinLossUFactorTimesAreaTerm();
}

bool GeneratorFuelCellPowerModule::setSkinLossQuadraticCurve(const CurveQuadratic& quadraticCurves) {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->setSkinLossQuadraticCurve(quadraticCurves);
}

void GeneratorFuelCellPowerModule::resetSkinLossQuadraticCurve() {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->resetSkinLossQuadraticCurve();
}

void GeneratorFuelCellPowerModule::setDilutionAirFlowRate(double dilutionAirFlowRate) {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->setDilutionAirFlowRate(dilutionAirFlowRate);
}

void GeneratorFuelCellPowerModule::resetDilutionAirFlowRate() {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->resetDilutionAirFlowRate();
}

void GeneratorFuelCellPowerModule::setStackHeatlosstoDilutionAir(double stackHeatlosstoDilutionAir) {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->setStackHeatlosstoDilutionAir(stackHeatlosstoDilutionAir);
}

void GeneratorFuelCellPowerModule::resetStackHeatlosstoDilutionAir() {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->resetStackHeatlosstoDilutionAir();
}

bool GeneratorFuelCellPowerModule::setDilutionInletAirNode(const Node& node) {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->setDilutionInletAirNode(node);
}

void GeneratorFuelCellPowerModule::resetDilutionInletAirNode() {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->resetDilutionInletAirNode();
}

bool GeneratorFuelCellPowerModule::setDilutionOutletAirNode(const Node& node) {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->setDilutionOutletAirNode(node);
}

void GeneratorFuelCellPowerModule::resetDilutionOutletAirNode() {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->resetDilutionOutletAirNode();
}

void GeneratorFuelCellPowerModule::setMinimumOperatingPoint(double minimumOperatingPoint) {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->setMinimumOperatingPoint(minimumOperatingPoint);
}

void GeneratorFuelCellPowerModule::resetMinimumOperatingPoint() {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->resetMinimumOperatingPoint();
}

void GeneratorFuelCellPowerModule::setMaximumOperatingPoint(double maximumOperatingPoint) {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->setMaximumOperatingPoint(maximumOperatingPoint);
}

void GeneratorFuelCellPowerModule::resetMaximumOperatingPoint() {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->resetMaximumOperatingPoint();
}

GeneratorFuelCell GeneratorFuelCellPowerModule::fuelCell() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->fuelCell();
}

/// @cond
GeneratorFuelCellPowerModule::GeneratorFuelCellPowerModule(std::shared_ptr<detail::GeneratorFuelCellPowerModule_Impl> impl)
  : ModelObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

