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

#include "GeneratorFuelCellPowerModule.hpp"
#include "GeneratorFuelCellPowerModule_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "CurveCubic.hpp"
#include "CurveCubic_Impl.hpp"
#include "CurveQuadratic.hpp"
#include "CurveQuadratic_Impl.hpp"
//#include "Zone.hpp"
//#include "Zone_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"

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

  boost::optional<std::string> GeneratorFuelCellPowerModule_Impl::efficiencyCurveMode() const {
    return getString(OS_Generator_FuelCell_PowerModuleFields::EfficiencyCurveMode,true);
  }

  QuadraticCurves GeneratorFuelCellPowerModule_Impl::efficiencyCurve() const {
    boost::optional<QuadraticCurves> value = optionalEfficiencyCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Efficiency Curve attached.");
    }
    return value.get();
  }

  boost::optional<double> GeneratorFuelCellPowerModule_Impl::nominalEfficiency() const {
    return getDouble(OS_Generator_FuelCell_PowerModuleFields::NominalEfficiency,true);
  }

  boost::optional<double> GeneratorFuelCellPowerModule_Impl::nominalElectricalPower() const {
    return getDouble(OS_Generator_FuelCell_PowerModuleFields::NominalElectricalPower,true);
  }

  boost::optional<double> GeneratorFuelCellPowerModule_Impl::numberofStopsatStartofSimulation() const {
    return getDouble(OS_Generator_FuelCell_PowerModuleFields::NumberofStopsatStartofSimulation,true);
  }

  boost::optional<double> GeneratorFuelCellPowerModule_Impl::cyclingPerformanceDegradationCoefficient() const {
    return getDouble(OS_Generator_FuelCell_PowerModuleFields::CyclingPerformanceDegradationCoefficient,true);
  }

  boost::optional<double> GeneratorFuelCellPowerModule_Impl::numberofRunHoursatBeginningofSimulation() const {
    return getDouble(OS_Generator_FuelCell_PowerModuleFields::NumberofRunHoursatBeginningofSimulation,true);
  }

  boost::optional<double> GeneratorFuelCellPowerModule_Impl::accumulatedRunTimeDegradationCoefficient() const {
    return getDouble(OS_Generator_FuelCell_PowerModuleFields::AccumulatedRunTimeDegradationCoefficient,true);
  }

  boost::optional<double> GeneratorFuelCellPowerModule_Impl::runTimeDegradationInitiationTimeThreshold() const {
    return getDouble(OS_Generator_FuelCell_PowerModuleFields::RunTimeDegradationInitiationTimeThreshold,true);
  }

  boost::optional<double> GeneratorFuelCellPowerModule_Impl::powerUpTransientLimit() const {
    return getDouble(OS_Generator_FuelCell_PowerModuleFields::PowerUpTransientLimit,true);
  }

  boost::optional<double> GeneratorFuelCellPowerModule_Impl::powerDownTransientLimit() const {
    return getDouble(OS_Generator_FuelCell_PowerModuleFields::PowerDownTransientLimit,true);
  }

  boost::optional<double> GeneratorFuelCellPowerModule_Impl::startUpTime() const {
    return getDouble(OS_Generator_FuelCell_PowerModuleFields::StartUpTime,true);
  }

  boost::optional<double> GeneratorFuelCellPowerModule_Impl::startUpFuel() const {
    return getDouble(OS_Generator_FuelCell_PowerModuleFields::StartUpFuel,true);
  }

  boost::optional<double> GeneratorFuelCellPowerModule_Impl::startUpElectricityConsumption() const {
    return getDouble(OS_Generator_FuelCell_PowerModuleFields::StartUpElectricityConsumption,true);
  }

  boost::optional<double> GeneratorFuelCellPowerModule_Impl::startUpElectricityProduced() const {
    return getDouble(OS_Generator_FuelCell_PowerModuleFields::StartUpElectricityProduced,true);
  }

  boost::optional<double> GeneratorFuelCellPowerModule_Impl::shutDownTime() const {
    return getDouble(OS_Generator_FuelCell_PowerModuleFields::ShutDownTime,true);
  }

  boost::optional<double> GeneratorFuelCellPowerModule_Impl::shutDownFuel() const {
    return getDouble(OS_Generator_FuelCell_PowerModuleFields::ShutDownFuel,true);
  }

  boost::optional<double> GeneratorFuelCellPowerModule_Impl::shutDownElectricityConsumption() const {
    return getDouble(OS_Generator_FuelCell_PowerModuleFields::ShutDownElectricityConsumption,true);
  }

  boost::optional<double> GeneratorFuelCellPowerModule_Impl::ancillaryElectricityConstantTerm() const {
    return getDouble(OS_Generator_FuelCell_PowerModuleFields::AncillaryElectricityConstantTerm,true);
  }

  boost::optional<double> GeneratorFuelCellPowerModule_Impl::ancillaryElectricityLinearTerm() const {
    return getDouble(OS_Generator_FuelCell_PowerModuleFields::AncillaryElectricityLinearTerm,true);
  }

  boost::optional<std::string> GeneratorFuelCellPowerModule_Impl::skinLossCalculationMode() const {
    return getString(OS_Generator_FuelCell_PowerModuleFields::SkinLossCalculationMode,true);
  }

  boost::optional<Zone> GeneratorFuelCellPowerModule_Impl::zone() const {
    return getObject<ModelObject>().getModelObjectTarget<Zone>(OS_Generator_FuelCell_PowerModuleFields::ZoneName);
  }

  boost::optional<double> GeneratorFuelCellPowerModule_Impl::skinLossRadiativeFraction() const {
    return getDouble(OS_Generator_FuelCell_PowerModuleFields::SkinLossRadiativeFraction,true);
  }

  boost::optional<double> GeneratorFuelCellPowerModule_Impl::constantSkinLossRate() const {
    return getDouble(OS_Generator_FuelCell_PowerModuleFields::ConstantSkinLossRate,true);
  }

  boost::optional<double> GeneratorFuelCellPowerModule_Impl::skinLossUFactorTimesAreaTerm() const {
    return getDouble(OS_Generator_FuelCell_PowerModuleFields::SkinLossUFactorTimesAreaTerm,true);
  }

  boost::optional<QuadraticCurves> GeneratorFuelCellPowerModule_Impl::skinLossQuadraticCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<QuadraticCurves>(OS_Generator_FuelCell_PowerModuleFields::SkinLossQuadraticCurveName);
  }

  boost::optional<double> GeneratorFuelCellPowerModule_Impl::dilutionAirFlowRate() const {
    return getDouble(OS_Generator_FuelCell_PowerModuleFields::DilutionAirFlowRate,true);
  }

  boost::optional<double> GeneratorFuelCellPowerModule_Impl::stackHeatlosstoDilutionAir() const {
    return getDouble(OS_Generator_FuelCell_PowerModuleFields::StackHeatlosstoDilutionAir,true);
  }

  boost::optional<Connection> GeneratorFuelCellPowerModule_Impl::dilutionInletAirNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_Generator_FuelCell_PowerModuleFields::DilutionInletAirNodeName);
  }

  boost::optional<Connection> GeneratorFuelCellPowerModule_Impl::dilutionOutletAirNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_Generator_FuelCell_PowerModuleFields::DilutionOutletAirNodeName);
  }

  boost::optional<double> GeneratorFuelCellPowerModule_Impl::minimumOperatingPoint() const {
    return getDouble(OS_Generator_FuelCell_PowerModuleFields::MinimumOperatingPoint,true);
  }

  boost::optional<double> GeneratorFuelCellPowerModule_Impl::maximumOperatingPoint() const {
    return getDouble(OS_Generator_FuelCell_PowerModuleFields::MaximumOperatingPoint,true);
  }

  bool GeneratorFuelCellPowerModule_Impl::setEfficiencyCurveMode(const std::string& efficiencyCurveMode) {
    bool result = setString(OS_Generator_FuelCell_PowerModuleFields::EfficiencyCurveMode, efficiencyCurveMode);
    return result;
  }

  void GeneratorFuelCellPowerModule_Impl::resetEfficiencyCurveMode() {
    bool result = setString(OS_Generator_FuelCell_PowerModuleFields::EfficiencyCurveMode, "");
    OS_ASSERT(result);
  }

  bool GeneratorFuelCellPowerModule_Impl::setEfficiencyCurve(const QuadraticCurves& quadraticCurves) {
    bool result = setPointer(OS_Generator_FuelCell_PowerModuleFields::EfficiencyCurveName, quadraticCurves.handle());
    return result;
  }

  void GeneratorFuelCellPowerModule_Impl::setNominalEfficiency(double nominalEfficiency) {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::NominalEfficiency, nominalEfficiency);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::resetNominalEfficiency() {
    bool result = setString(OS_Generator_FuelCell_PowerModuleFields::NominalEfficiency, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::setNominalElectricalPower(double nominalElectricalPower) {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::NominalElectricalPower, nominalElectricalPower);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::resetNominalElectricalPower() {
    bool result = setString(OS_Generator_FuelCell_PowerModuleFields::NominalElectricalPower, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::setNumberofStopsatStartofSimulation(double numberofStopsatStartofSimulation) {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::NumberofStopsatStartofSimulation, numberofStopsatStartofSimulation);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::resetNumberofStopsatStartofSimulation() {
    bool result = setString(OS_Generator_FuelCell_PowerModuleFields::NumberofStopsatStartofSimulation, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::setCyclingPerformanceDegradationCoefficient(double cyclingPerformanceDegradationCoefficient) {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::CyclingPerformanceDegradationCoefficient, cyclingPerformanceDegradationCoefficient);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::resetCyclingPerformanceDegradationCoefficient() {
    bool result = setString(OS_Generator_FuelCell_PowerModuleFields::CyclingPerformanceDegradationCoefficient, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::setNumberofRunHoursatBeginningofSimulation(double numberofRunHoursatBeginningofSimulation) {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::NumberofRunHoursatBeginningofSimulation, numberofRunHoursatBeginningofSimulation);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::resetNumberofRunHoursatBeginningofSimulation() {
    bool result = setString(OS_Generator_FuelCell_PowerModuleFields::NumberofRunHoursatBeginningofSimulation, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::setAccumulatedRunTimeDegradationCoefficient(double accumulatedRunTimeDegradationCoefficient) {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::AccumulatedRunTimeDegradationCoefficient, accumulatedRunTimeDegradationCoefficient);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::resetAccumulatedRunTimeDegradationCoefficient() {
    bool result = setString(OS_Generator_FuelCell_PowerModuleFields::AccumulatedRunTimeDegradationCoefficient, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::setRunTimeDegradationInitiationTimeThreshold(double runTimeDegradationInitiationTimeThreshold) {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::RunTimeDegradationInitiationTimeThreshold, runTimeDegradationInitiationTimeThreshold);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::resetRunTimeDegradationInitiationTimeThreshold() {
    bool result = setString(OS_Generator_FuelCell_PowerModuleFields::RunTimeDegradationInitiationTimeThreshold, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::setPowerUpTransientLimit(double powerUpTransientLimit) {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::PowerUpTransientLimit, powerUpTransientLimit);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::resetPowerUpTransientLimit() {
    bool result = setString(OS_Generator_FuelCell_PowerModuleFields::PowerUpTransientLimit, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::setPowerDownTransientLimit(double powerDownTransientLimit) {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::PowerDownTransientLimit, powerDownTransientLimit);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::resetPowerDownTransientLimit() {
    bool result = setString(OS_Generator_FuelCell_PowerModuleFields::PowerDownTransientLimit, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::setStartUpTime(double startUpTime) {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::StartUpTime, startUpTime);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::resetStartUpTime() {
    bool result = setString(OS_Generator_FuelCell_PowerModuleFields::StartUpTime, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::setStartUpFuel(double startUpFuel) {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::StartUpFuel, startUpFuel);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::resetStartUpFuel() {
    bool result = setString(OS_Generator_FuelCell_PowerModuleFields::StartUpFuel, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::setStartUpElectricityConsumption(double startUpElectricityConsumption) {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::StartUpElectricityConsumption, startUpElectricityConsumption);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::resetStartUpElectricityConsumption() {
    bool result = setString(OS_Generator_FuelCell_PowerModuleFields::StartUpElectricityConsumption, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::setStartUpElectricityProduced(double startUpElectricityProduced) {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::StartUpElectricityProduced, startUpElectricityProduced);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::resetStartUpElectricityProduced() {
    bool result = setString(OS_Generator_FuelCell_PowerModuleFields::StartUpElectricityProduced, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::setShutDownTime(double shutDownTime) {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::ShutDownTime, shutDownTime);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::resetShutDownTime() {
    bool result = setString(OS_Generator_FuelCell_PowerModuleFields::ShutDownTime, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::setShutDownFuel(double shutDownFuel) {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::ShutDownFuel, shutDownFuel);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::resetShutDownFuel() {
    bool result = setString(OS_Generator_FuelCell_PowerModuleFields::ShutDownFuel, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::setShutDownElectricityConsumption(double shutDownElectricityConsumption) {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::ShutDownElectricityConsumption, shutDownElectricityConsumption);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::resetShutDownElectricityConsumption() {
    bool result = setString(OS_Generator_FuelCell_PowerModuleFields::ShutDownElectricityConsumption, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::setAncillaryElectricityConstantTerm(double ancillaryElectricityConstantTerm) {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::AncillaryElectricityConstantTerm, ancillaryElectricityConstantTerm);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::resetAncillaryElectricityConstantTerm() {
    bool result = setString(OS_Generator_FuelCell_PowerModuleFields::AncillaryElectricityConstantTerm, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::setAncillaryElectricityLinearTerm(double ancillaryElectricityLinearTerm) {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::AncillaryElectricityLinearTerm, ancillaryElectricityLinearTerm);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::resetAncillaryElectricityLinearTerm() {
    bool result = setString(OS_Generator_FuelCell_PowerModuleFields::AncillaryElectricityLinearTerm, "");
    OS_ASSERT(result);
  }

  bool GeneratorFuelCellPowerModule_Impl::setSkinLossCalculationMode(const std::string& skinLossCalculationMode) {
    bool result = setString(OS_Generator_FuelCell_PowerModuleFields::SkinLossCalculationMode, skinLossCalculationMode);
    return result;
  }

  void GeneratorFuelCellPowerModule_Impl::resetSkinLossCalculationMode() {
    bool result = setString(OS_Generator_FuelCell_PowerModuleFields::SkinLossCalculationMode, "");
    OS_ASSERT(result);
  }

  bool GeneratorFuelCellPowerModule_Impl::setZone(const Zone& zone) {
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
    bool result = setString(OS_Generator_FuelCell_PowerModuleFields::SkinLossRadiativeFraction, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::setConstantSkinLossRate(double constantSkinLossRate) {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::ConstantSkinLossRate, constantSkinLossRate);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::resetConstantSkinLossRate() {
    bool result = setString(OS_Generator_FuelCell_PowerModuleFields::ConstantSkinLossRate, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::setSkinLossUFactorTimesAreaTerm(double skinLossUFactorTimesAreaTerm) {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::SkinLossUFactorTimesAreaTerm, skinLossUFactorTimesAreaTerm);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::resetSkinLossUFactorTimesAreaTerm() {
    bool result = setString(OS_Generator_FuelCell_PowerModuleFields::SkinLossUFactorTimesAreaTerm, "");
    OS_ASSERT(result);
  }

  bool GeneratorFuelCellPowerModule_Impl::setSkinLossQuadraticCurve(const QuadraticCurves& quadraticCurves) {
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
    bool result = setString(OS_Generator_FuelCell_PowerModuleFields::DilutionAirFlowRate, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::setStackHeatlosstoDilutionAir(double stackHeatlosstoDilutionAir) {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::StackHeatlosstoDilutionAir, stackHeatlosstoDilutionAir);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::resetStackHeatlosstoDilutionAir() {
    bool result = setString(OS_Generator_FuelCell_PowerModuleFields::StackHeatlosstoDilutionAir, "");
    OS_ASSERT(result);
  }

  bool GeneratorFuelCellPowerModule_Impl::setDilutionInletAirNode(const Connection& connection) {
    bool result = setPointer(OS_Generator_FuelCell_PowerModuleFields::DilutionInletAirNodeName, connection.handle());
    return result;
  }

  void GeneratorFuelCellPowerModule_Impl::resetDilutionInletAirNode() {
    bool result = setString(OS_Generator_FuelCell_PowerModuleFields::DilutionInletAirNodeName, "");
    OS_ASSERT(result);
  }

  bool GeneratorFuelCellPowerModule_Impl::setDilutionOutletAirNode(const Connection& connection) {
    bool result = setPointer(OS_Generator_FuelCell_PowerModuleFields::DilutionOutletAirNodeName, connection.handle());
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
    bool result = setString(OS_Generator_FuelCell_PowerModuleFields::MinimumOperatingPoint, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::setMaximumOperatingPoint(double maximumOperatingPoint) {
    bool result = setDouble(OS_Generator_FuelCell_PowerModuleFields::MaximumOperatingPoint, maximumOperatingPoint);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellPowerModule_Impl::resetMaximumOperatingPoint() {
    bool result = setString(OS_Generator_FuelCell_PowerModuleFields::MaximumOperatingPoint, "");
    OS_ASSERT(result);
  }

  boost::optional<QuadraticCurves> GeneratorFuelCellPowerModule_Impl::optionalEfficiencyCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<QuadraticCurves>(OS_Generator_FuelCell_PowerModuleFields::EfficiencyCurveName);
  }

} // detail

GeneratorFuelCellPowerModule::GeneratorFuelCellPowerModule(const Model& model)
  : ModelObject(GeneratorFuelCellPowerModule::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::GeneratorFuelCellPowerModule_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  //     OS_Generator_FuelCell_PowerModuleFields::EfficiencyCurveName
  bool ok = true;
  // ok = setEfficiencyCurve();
  OS_ASSERT(ok);
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

boost::optional<std::string> GeneratorFuelCellPowerModule::efficiencyCurveMode() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->efficiencyCurveMode();
}

QuadraticCurves GeneratorFuelCellPowerModule::efficiencyCurve() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->efficiencyCurve();
}

boost::optional<double> GeneratorFuelCellPowerModule::nominalEfficiency() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->nominalEfficiency();
}

boost::optional<double> GeneratorFuelCellPowerModule::nominalElectricalPower() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->nominalElectricalPower();
}

boost::optional<double> GeneratorFuelCellPowerModule::numberofStopsatStartofSimulation() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->numberofStopsatStartofSimulation();
}

boost::optional<double> GeneratorFuelCellPowerModule::cyclingPerformanceDegradationCoefficient() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->cyclingPerformanceDegradationCoefficient();
}

boost::optional<double> GeneratorFuelCellPowerModule::numberofRunHoursatBeginningofSimulation() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->numberofRunHoursatBeginningofSimulation();
}

boost::optional<double> GeneratorFuelCellPowerModule::accumulatedRunTimeDegradationCoefficient() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->accumulatedRunTimeDegradationCoefficient();
}

boost::optional<double> GeneratorFuelCellPowerModule::runTimeDegradationInitiationTimeThreshold() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->runTimeDegradationInitiationTimeThreshold();
}

boost::optional<double> GeneratorFuelCellPowerModule::powerUpTransientLimit() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->powerUpTransientLimit();
}

boost::optional<double> GeneratorFuelCellPowerModule::powerDownTransientLimit() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->powerDownTransientLimit();
}

boost::optional<double> GeneratorFuelCellPowerModule::startUpTime() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->startUpTime();
}

boost::optional<double> GeneratorFuelCellPowerModule::startUpFuel() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->startUpFuel();
}

boost::optional<double> GeneratorFuelCellPowerModule::startUpElectricityConsumption() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->startUpElectricityConsumption();
}

boost::optional<double> GeneratorFuelCellPowerModule::startUpElectricityProduced() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->startUpElectricityProduced();
}

boost::optional<double> GeneratorFuelCellPowerModule::shutDownTime() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->shutDownTime();
}

boost::optional<double> GeneratorFuelCellPowerModule::shutDownFuel() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->shutDownFuel();
}

boost::optional<double> GeneratorFuelCellPowerModule::shutDownElectricityConsumption() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->shutDownElectricityConsumption();
}

boost::optional<double> GeneratorFuelCellPowerModule::ancillaryElectricityConstantTerm() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->ancillaryElectricityConstantTerm();
}

boost::optional<double> GeneratorFuelCellPowerModule::ancillaryElectricityLinearTerm() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->ancillaryElectricityLinearTerm();
}

boost::optional<std::string> GeneratorFuelCellPowerModule::skinLossCalculationMode() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->skinLossCalculationMode();
}

boost::optional<Zone> GeneratorFuelCellPowerModule::zone() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->zone();
}

boost::optional<double> GeneratorFuelCellPowerModule::skinLossRadiativeFraction() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->skinLossRadiativeFraction();
}

boost::optional<double> GeneratorFuelCellPowerModule::constantSkinLossRate() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->constantSkinLossRate();
}

boost::optional<double> GeneratorFuelCellPowerModule::skinLossUFactorTimesAreaTerm() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->skinLossUFactorTimesAreaTerm();
}

boost::optional<QuadraticCurves> GeneratorFuelCellPowerModule::skinLossQuadraticCurve() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->skinLossQuadraticCurve();
}

boost::optional<double> GeneratorFuelCellPowerModule::dilutionAirFlowRate() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->dilutionAirFlowRate();
}

boost::optional<double> GeneratorFuelCellPowerModule::stackHeatlosstoDilutionAir() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->stackHeatlosstoDilutionAir();
}

boost::optional<Connection> GeneratorFuelCellPowerModule::dilutionInletAirNode() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->dilutionInletAirNode();
}

boost::optional<Connection> GeneratorFuelCellPowerModule::dilutionOutletAirNode() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->dilutionOutletAirNode();
}

boost::optional<double> GeneratorFuelCellPowerModule::minimumOperatingPoint() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->minimumOperatingPoint();
}

boost::optional<double> GeneratorFuelCellPowerModule::maximumOperatingPoint() const {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->maximumOperatingPoint();
}

bool GeneratorFuelCellPowerModule::setEfficiencyCurveMode(const std::string& efficiencyCurveMode) {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->setEfficiencyCurveMode(efficiencyCurveMode);
}

void GeneratorFuelCellPowerModule::resetEfficiencyCurveMode() {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->resetEfficiencyCurveMode();
}

bool GeneratorFuelCellPowerModule::setEfficiencyCurve(const QuadraticCurves& quadraticCurves) {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->setEfficiencyCurve(quadraticCurves);
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

bool GeneratorFuelCellPowerModule::setZone(const Zone& zone) {
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

bool GeneratorFuelCellPowerModule::setSkinLossQuadraticCurve(const QuadraticCurves& quadraticCurves) {
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

bool GeneratorFuelCellPowerModule::setDilutionInletAirNode(const Connection& connection) {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->setDilutionInletAirNode(connection);
}

void GeneratorFuelCellPowerModule::resetDilutionInletAirNode() {
  getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->resetDilutionInletAirNode();
}

bool GeneratorFuelCellPowerModule::setDilutionOutletAirNode(const Connection& connection) {
  return getImpl<detail::GeneratorFuelCellPowerModule_Impl>()->setDilutionOutletAirNode(connection);
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

/// @cond
GeneratorFuelCellPowerModule::GeneratorFuelCellPowerModule(std::shared_ptr<detail::GeneratorFuelCellPowerModule_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

