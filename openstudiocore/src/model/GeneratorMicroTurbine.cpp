/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#include "GeneratorMicroTurbine.hpp"
#include "GeneratorMicroTurbine_Impl.hpp"

#include "Curve.hpp"
#include "Curve_Impl.hpp"

// TODO: Here I think I need the child classes of Curve too? To make sure the user doesn't set to a curve that isn't appropriate?
//#include "CurveBicubic.hpp"
//#include "CurveBicubic_Impl.hpp"
//#include "CurveBiquadratic.hpp"
//#include "CurveBiquadratic_Impl.hpp"
//#include "CurveCubic.hpp"
//#include "CurveCubic_Impl.hpp"
//#include "CurveQuadratic.hpp"
//#include "CurveQuadratic_Impl.hpp"

// TODO: add the tables class if they get added to OS later?
//#include "VariateTables.hpp" // Or whatever the name will be. DataTables maybe
//#include "VariateTables_Impl.hpp"
//#include "UniVariateTables.hpp"
//#include "UniVariateTables_Impl.hpp"
//#include "BiVariateTables.hpp"
//#include "BiVariateTables_Impl.hpp"

//class Connection;

#include "Node.hpp"
#include "Node_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Generator_MicroTurbine_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  GeneratorMicroTurbine_Impl::GeneratorMicroTurbine_Impl(const IdfObject& idfObject,
                                                         Model_Impl* model,
                                                         bool keepHandle)
    : Generator_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == GeneratorMicroTurbine::iddObjectType());
  }

  GeneratorMicroTurbine_Impl::GeneratorMicroTurbine_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                         Model_Impl* model,
                                                         bool keepHandle)
    : Generator_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == GeneratorMicroTurbine::iddObjectType());
  }

  GeneratorMicroTurbine_Impl::GeneratorMicroTurbine_Impl(const GeneratorMicroTurbine_Impl& other,
                                                         Model_Impl* model,
                                                         bool keepHandle)
    : Generator_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& GeneratorMicroTurbine_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType GeneratorMicroTurbine_Impl::iddObjectType() const {
    return GeneratorMicroTurbine::iddObjectType();
  }
  
  // Add an implementation of StraightComponent_Impl?
  
  // TODO: this is actually an optional... should I handle that explicitly?
  unsigned GeneratorMicroTurbine_Impl::inletPort()
  {
    return OS_Generator_MicroTurbineFields::HeatRecoveryWaterInletNodeName;
  }

  unsigned GeneratorMicroTurbine_Impl::outletPort()
  {
    return OS_Generator_MicroTurbineFields::HeatRecoveryWaterOutletNodeName;
  }

  double GeneratorMicroTurbine_Impl::referenceElectricalPowerOutput() const {
    boost::optional<double> value = getDouble(OS_Generator_MicroTurbineFields::ReferenceElectricalPowerOutput,true);
    OS_ASSERT(value);
    return value.get();
  }

  double GeneratorMicroTurbine_Impl::minimumFullLoadElectricalPowerOutput() const {
    boost::optional<double> value = getDouble(OS_Generator_MicroTurbineFields::MinimumFullLoadElectricalPowerOutput,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool GeneratorMicroTurbine_Impl::isMinimumFullLoadElectricalPowerOutputDefaulted() const {
    return isEmpty(OS_Generator_MicroTurbineFields::MinimumFullLoadElectricalPowerOutput);
  }

  boost::optional<double> GeneratorMicroTurbine_Impl::maximumFullLoadElectricalPowerOutput() const {
    return getDouble(OS_Generator_MicroTurbineFields::MaximumFullLoadElectricalPowerOutput,true);
  }

  double GeneratorMicroTurbine_Impl::referenceElectricalEfficiencyUsingLowerHeatingValue() const {
    boost::optional<double> value = getDouble(OS_Generator_MicroTurbineFields::ReferenceElectricalEfficiencyUsingLowerHeatingValue,true);
    OS_ASSERT(value);
    return value.get();
  }

  double GeneratorMicroTurbine_Impl::referenceCombustionAirInletTemperature() const {
    boost::optional<double> value = getDouble(OS_Generator_MicroTurbineFields::ReferenceCombustionAirInletTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool GeneratorMicroTurbine_Impl::isReferenceCombustionAirInletTemperatureDefaulted() const {
    return isEmpty(OS_Generator_MicroTurbineFields::ReferenceCombustionAirInletTemperature);
  }

  double GeneratorMicroTurbine_Impl::referenceCombustionAirInletHumidityRatio() const {
    boost::optional<double> value = getDouble(OS_Generator_MicroTurbineFields::ReferenceCombustionAirInletHumidityRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool GeneratorMicroTurbine_Impl::isReferenceCombustionAirInletHumidityRatioDefaulted() const {
    return isEmpty(OS_Generator_MicroTurbineFields::ReferenceCombustionAirInletHumidityRatio);
  }

  double GeneratorMicroTurbine_Impl::referenceElevation() const {
    boost::optional<double> value = getDouble(OS_Generator_MicroTurbineFields::ReferenceElevation,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool GeneratorMicroTurbine_Impl::isReferenceElevationDefaulted() const {
    return isEmpty(OS_Generator_MicroTurbineFields::ReferenceElevation);
  }

  Curve GeneratorMicroTurbine_Impl::electricalPowerFunctionofTemperatureandElevationCurve() const {
    boost::optional<Curve> value = optionalElectricalPowerFunctionofTemperatureandElevationCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Electrical Power Function of Temperature and Elevation Curve attached.");
    }
    return value.get();
  }

  Curve GeneratorMicroTurbine_Impl::electricalEfficiencyFunctionofTemperatureCurve() const {
    boost::optional<Curve> value = optionalElectricalEfficiencyFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Electrical Efficiency Function of Temperature Curve attached.");
    }
    return value.get();
  }

  Curve GeneratorMicroTurbine_Impl::electricalEfficiencyFunctionofPartLoadRatioCurve() const {
    boost::optional<Curve> value = optionalElectricalEfficiencyFunctionofPartLoadRatioCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Electrical Efficiency Function of Part Load Ratio Curve attached.");
    }
    return value.get();
  }
  

  std::string GeneratorMicroTurbine_Impl::fuelType() const {
    boost::optional<std::string> value = getString(OS_Generator_MicroTurbineFields::FuelType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool GeneratorMicroTurbine_Impl::isFuelTypeDefaulted() const {
    return isEmpty(OS_Generator_MicroTurbineFields::FuelType);
  }

  double GeneratorMicroTurbine_Impl::fuelHigherHeatingValue() const {
    boost::optional<double> value = getDouble(OS_Generator_MicroTurbineFields::FuelHigherHeatingValue,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool GeneratorMicroTurbine_Impl::isFuelHigherHeatingValueDefaulted() const {
    return isEmpty(OS_Generator_MicroTurbineFields::FuelHigherHeatingValue);
  }

  double GeneratorMicroTurbine_Impl::fuelLowerHeatingValue() const {
    boost::optional<double> value = getDouble(OS_Generator_MicroTurbineFields::FuelLowerHeatingValue,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool GeneratorMicroTurbine_Impl::isFuelLowerHeatingValueDefaulted() const {
    return isEmpty(OS_Generator_MicroTurbineFields::FuelLowerHeatingValue);
  }

  double GeneratorMicroTurbine_Impl::standbyPower() const {
    boost::optional<double> value = getDouble(OS_Generator_MicroTurbineFields::StandbyPower,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool GeneratorMicroTurbine_Impl::isStandbyPowerDefaulted() const {
    return isEmpty(OS_Generator_MicroTurbineFields::StandbyPower);
  }

  double GeneratorMicroTurbine_Impl::ancillaryPower() const {
    boost::optional<double> value = getDouble(OS_Generator_MicroTurbineFields::AncillaryPower,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool GeneratorMicroTurbine_Impl::isAncillaryPowerDefaulted() const {
    return isEmpty(OS_Generator_MicroTurbineFields::AncillaryPower);
  }

  boost::optional<Curve> GeneratorMicroTurbine_Impl::ancillaryPowerFunctionofFuelInputCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Generator_MicroTurbineFields::AncillaryPowerFunctionofFuelInputCurveName);
  }

  //boost::optional<Connection> GeneratorMicroTurbine_Impl::heatRecoveryWaterInletNode() const {
  //  return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_Generator_MicroTurbineFields::HeatRecoveryWaterInletNodeName);
  //}

  //boost::optional<Connection> GeneratorMicroTurbine_Impl::heatRecoveryWaterOutletNode() const {
  //  return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_Generator_MicroTurbineFields::HeatRecoveryWaterOutletNodeName);
  //}

  double GeneratorMicroTurbine_Impl::referenceThermalEfficiencyUsingLowerHeatValue() const {
    boost::optional<double> value = getDouble(OS_Generator_MicroTurbineFields::ReferenceThermalEfficiencyUsingLowerHeatValue,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool GeneratorMicroTurbine_Impl::isReferenceThermalEfficiencyUsingLowerHeatValueDefaulted() const {
    return isEmpty(OS_Generator_MicroTurbineFields::ReferenceThermalEfficiencyUsingLowerHeatValue);
  }

  boost::optional<double> GeneratorMicroTurbine_Impl::referenceInletWaterTemperature() const {
    return getDouble(OS_Generator_MicroTurbineFields::ReferenceInletWaterTemperature,true);
  }

  std::string GeneratorMicroTurbine_Impl::heatRecoveryWaterFlowOperatingMode() const {
    boost::optional<std::string> value = getString(OS_Generator_MicroTurbineFields::HeatRecoveryWaterFlowOperatingMode,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool GeneratorMicroTurbine_Impl::isHeatRecoveryWaterFlowOperatingModeDefaulted() const {
    return isEmpty(OS_Generator_MicroTurbineFields::HeatRecoveryWaterFlowOperatingMode);
  }

  boost::optional<double> GeneratorMicroTurbine_Impl::referenceHeatRecoveryWaterFlowRate() const {
    return getDouble(OS_Generator_MicroTurbineFields::ReferenceHeatRecoveryWaterFlowRate,true);
  }

  boost::optional<Curve> GeneratorMicroTurbine_Impl::heatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Generator_MicroTurbineFields::HeatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurveName);
  }

  boost::optional<Curve> GeneratorMicroTurbine_Impl::thermalEfficiencyFunctionofTemperatureandElevationCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Generator_MicroTurbineFields::ThermalEfficiencyFunctionofTemperatureandElevationCurveName);
  }

  boost::optional<Curve> GeneratorMicroTurbine_Impl::heatRecoveryRateFunctionofPartLoadRatioCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Generator_MicroTurbineFields::HeatRecoveryRateFunctionofPartLoadRatioCurveName);
  }

  boost::optional<Curve> GeneratorMicroTurbine_Impl::heatRecoveryRateFunctionofInletWaterTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Generator_MicroTurbineFields::HeatRecoveryRateFunctionofInletWaterTemperatureCurveName);
  }

  boost::optional<Curve> GeneratorMicroTurbine_Impl::heatRecoveryRateFunctionofWaterFlowRateCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Generator_MicroTurbineFields::HeatRecoveryRateFunctionofWaterFlowRateCurveName);
  }

  double GeneratorMicroTurbine_Impl::minimumHeatRecoveryWaterFlowRate() const {
    boost::optional<double> value = getDouble(OS_Generator_MicroTurbineFields::MinimumHeatRecoveryWaterFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool GeneratorMicroTurbine_Impl::isMinimumHeatRecoveryWaterFlowRateDefaulted() const {
    return isEmpty(OS_Generator_MicroTurbineFields::MinimumHeatRecoveryWaterFlowRate);
  }

  double GeneratorMicroTurbine_Impl::maximumHeatRecoveryWaterFlowRate() const {
    boost::optional<double> value = getDouble(OS_Generator_MicroTurbineFields::MaximumHeatRecoveryWaterFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool GeneratorMicroTurbine_Impl::isMaximumHeatRecoveryWaterFlowRateDefaulted() const {
    return isEmpty(OS_Generator_MicroTurbineFields::MaximumHeatRecoveryWaterFlowRate);
  }

  boost::optional<double> GeneratorMicroTurbine_Impl::maximumHeatRecoveryWaterTemperature() const {
    return getDouble(OS_Generator_MicroTurbineFields::MaximumHeatRecoveryWaterTemperature,true);
  }

  //boost::optional<Connection> GeneratorMicroTurbine_Impl::combustionAirInletNode() const {
  //  return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_Generator_MicroTurbineFields::CombustionAirInletNodeName);
  //}

  //boost::optional<Connection> GeneratorMicroTurbine_Impl::combustionAirOutletNode() const {
  //  return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_Generator_MicroTurbineFields::CombustionAirOutletNodeName);
  //}

  boost::optional<double> GeneratorMicroTurbine_Impl::referenceExhaustAirMassFlowRate() const {
    return getDouble(OS_Generator_MicroTurbineFields::ReferenceExhaustAirMassFlowRate,true);
  }

  boost::optional<Curve> GeneratorMicroTurbine_Impl::exhaustAirFlowRateFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Generator_MicroTurbineFields::ExhaustAirFlowRateFunctionofTemperatureCurveName);
  }

  boost::optional<Curve> GeneratorMicroTurbine_Impl::exhaustAirFlowRateFunctionofPartLoadRatioCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Generator_MicroTurbineFields::ExhaustAirFlowRateFunctionofPartLoadRatioCurveName);
  }

  boost::optional<double> GeneratorMicroTurbine_Impl::nominalExhaustAirOutletTemperature() const {
    return getDouble(OS_Generator_MicroTurbineFields::NominalExhaustAirOutletTemperature,true);
  }

  boost::optional<Curve> GeneratorMicroTurbine_Impl::exhaustAirTemperatureFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Generator_MicroTurbineFields::ExhaustAirTemperatureFunctionofTemperatureCurveName);
  }

  boost::optional<Curve> GeneratorMicroTurbine_Impl::exhaustAirTemperatureFunctionofPartLoadRatioCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Generator_MicroTurbineFields::ExhaustAirTemperatureFunctionofPartLoadRatioCurveName);
  }

  bool GeneratorMicroTurbine_Impl::setReferenceElectricalPowerOutput(double referenceElectricalPowerOutput) {
    bool result = setDouble(OS_Generator_MicroTurbineFields::ReferenceElectricalPowerOutput, referenceElectricalPowerOutput);
    return result;
  }

  bool GeneratorMicroTurbine_Impl::setMinimumFullLoadElectricalPowerOutput(double minimumFullLoadElectricalPowerOutput) {
    bool result = setDouble(OS_Generator_MicroTurbineFields::MinimumFullLoadElectricalPowerOutput, minimumFullLoadElectricalPowerOutput);
    return result;
  }

  void GeneratorMicroTurbine_Impl::resetMinimumFullLoadElectricalPowerOutput() {
    bool result = setString(OS_Generator_MicroTurbineFields::MinimumFullLoadElectricalPowerOutput, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbine_Impl::setMaximumFullLoadElectricalPowerOutput(boost::optional<double> maximumFullLoadElectricalPowerOutput) {
    bool result(false);
    if (maximumFullLoadElectricalPowerOutput) {
      result = setDouble(OS_Generator_MicroTurbineFields::MaximumFullLoadElectricalPowerOutput, maximumFullLoadElectricalPowerOutput.get());
    }
    else {
      resetMaximumFullLoadElectricalPowerOutput();
      result = true;
    }
    return result;
  }

  void GeneratorMicroTurbine_Impl::resetMaximumFullLoadElectricalPowerOutput() {
    bool result = setString(OS_Generator_MicroTurbineFields::MaximumFullLoadElectricalPowerOutput, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbine_Impl::setReferenceElectricalEfficiencyUsingLowerHeatingValue(double referenceElectricalEfficiencyUsingLowerHeatingValue) {
    bool result = setDouble(OS_Generator_MicroTurbineFields::ReferenceElectricalEfficiencyUsingLowerHeatingValue, referenceElectricalEfficiencyUsingLowerHeatingValue);
    return result;
  }

  void GeneratorMicroTurbine_Impl::setReferenceCombustionAirInletTemperature(double referenceCombustionAirInletTemperature) {
    bool result = setDouble(OS_Generator_MicroTurbineFields::ReferenceCombustionAirInletTemperature, referenceCombustionAirInletTemperature);
    OS_ASSERT(result);
  }

  void GeneratorMicroTurbine_Impl::resetReferenceCombustionAirInletTemperature() {
    bool result = setString(OS_Generator_MicroTurbineFields::ReferenceCombustionAirInletTemperature, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbine_Impl::setReferenceCombustionAirInletHumidityRatio(double referenceCombustionAirInletHumidityRatio) {
    bool result = setDouble(OS_Generator_MicroTurbineFields::ReferenceCombustionAirInletHumidityRatio, referenceCombustionAirInletHumidityRatio);
    return result;
  }

  void GeneratorMicroTurbine_Impl::resetReferenceCombustionAirInletHumidityRatio() {
    bool result = setString(OS_Generator_MicroTurbineFields::ReferenceCombustionAirInletHumidityRatio, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbine_Impl::setReferenceElevation(double referenceElevation) {
    bool result = setDouble(OS_Generator_MicroTurbineFields::ReferenceElevation, referenceElevation);
    return result;
  }

  void GeneratorMicroTurbine_Impl::resetReferenceElevation() {
    bool result = setString(OS_Generator_MicroTurbineFields::ReferenceElevation, "");
    OS_ASSERT(result);
  }
  
  bool GeneratorMicroTurbine_Impl::setElectricalPowerFunctionofTemperatureandElevationCurve(const Curve& curve)
  {
    if(model() != curve.model())
    {
      LOG(briefDescription() << " does not below to the same model as the curve you want to set.");
      return false;
    }
    bool result = setPointer(OS_Chiller_Electric_EIRFields::CoolingCapacityFunctionofTemperatureCurveName, curve.handle());
    return result;
  }

  bool GeneratorMicroTurbine_Impl::setElectricalEfficiencyFunctionofTemperatureCurve(const Curve& curve) {
    if(model() != curve.model())
    {
      LOG(briefDescription() << " does not below to the same model as the curve you want to set.");
      return false;
    }
    bool result = setPointer(OS_Generator_MicroTurbineFields::ElectricalEfficiencyFunctionofTemperatureCurveName, curve.handle());
    return result;
  }

  bool GeneratorMicroTurbine_Impl::setElectricalEfficiencyFunctionofPartLoadRatioCurve(const Curve& curve) {
    if(model() != curve.model())
    {
      LOG(briefDescription() << " does not below to the same model as the curve you want to set.");
      return false;
    }
    bool result = setPointer(OS_Generator_MicroTurbineFields::ElectricalEfficiencyFunctionofPartLoadRatioCurveName, curve.handle());
    return result;
  }

  bool GeneratorMicroTurbine_Impl::setFuelType(std::string fuelType) {
    bool result = setString(OS_Generator_MicroTurbineFields::FuelType, fuelType);
    return result;
  }

  void GeneratorMicroTurbine_Impl::resetFuelType() {
    bool result = setString(OS_Generator_MicroTurbineFields::FuelType, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbine_Impl::setFuelHigherHeatingValue(double fuelHigherHeatingValue) {
    bool result = setDouble(OS_Generator_MicroTurbineFields::FuelHigherHeatingValue, fuelHigherHeatingValue);
    return result;
  }

  void GeneratorMicroTurbine_Impl::resetFuelHigherHeatingValue() {
    bool result = setString(OS_Generator_MicroTurbineFields::FuelHigherHeatingValue, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbine_Impl::setFuelLowerHeatingValue(double fuelLowerHeatingValue) {
    bool result = setDouble(OS_Generator_MicroTurbineFields::FuelLowerHeatingValue, fuelLowerHeatingValue);
    return result;
  }

  void GeneratorMicroTurbine_Impl::resetFuelLowerHeatingValue() {
    bool result = setString(OS_Generator_MicroTurbineFields::FuelLowerHeatingValue, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbine_Impl::setStandbyPower(double standbyPower) {
    bool result = setDouble(OS_Generator_MicroTurbineFields::StandbyPower, standbyPower);
    return result;
  }

  void GeneratorMicroTurbine_Impl::resetStandbyPower() {
    bool result = setString(OS_Generator_MicroTurbineFields::StandbyPower, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbine_Impl::setAncillaryPower(double ancillaryPower) {
    bool result = setDouble(OS_Generator_MicroTurbineFields::AncillaryPower, ancillaryPower);
    return result;
  }

  void GeneratorMicroTurbine_Impl::resetAncillaryPower() {
    bool result = setString(OS_Generator_MicroTurbineFields::AncillaryPower, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbine_Impl::setAncillaryPowerFunctionofFuelInputCurve(const boost::optional<Curve>& curve) {
    bool result(false);
    if (curve) {
      if(model() != curve.get().model())
      {
        LOG(briefDescription() << " does not below to the same model as the curve you want to set.");
        return false;
      }
      result = setPointer(OS_Generator_MicroTurbineFields::AncillaryPowerFunctionofFuelInputCurveName, curve.get().handle());
    }
    else {
      resetAncillaryPowerFunctionofFuelInputCurve();
      result = true;
    }
    return result;
  }

  void GeneratorMicroTurbine_Impl::resetAncillaryPowerFunctionofFuelInputCurve() {
    bool result = setString(OS_Generator_MicroTurbineFields::AncillaryPowerFunctionofFuelInputCurveName, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbine_Impl::setHeatRecoveryWaterInletNode(const boost::optional<Connection>& connection) {
    bool result(false);
    if (connection) {
      result = setPointer(OS_Generator_MicroTurbineFields::HeatRecoveryWaterInletNodeName, connection.get().handle());
    }
    else {
      resetHeatRecoveryWaterInletNode();
      result = true;
    }
    return result;
  }

  void GeneratorMicroTurbine_Impl::resetHeatRecoveryWaterInletNode() {
    bool result = setString(OS_Generator_MicroTurbineFields::HeatRecoveryWaterInletNodeName, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbine_Impl::setHeatRecoveryWaterOutletNode(const boost::optional<Connection>& connection) {
    bool result(false);
    if (connection) {
      result = setPointer(OS_Generator_MicroTurbineFields::HeatRecoveryWaterOutletNodeName, connection.get().handle());
    }
    else {
      resetHeatRecoveryWaterOutletNode();
      result = true;
    }
    return result;
  }

  void GeneratorMicroTurbine_Impl::resetHeatRecoveryWaterOutletNode() {
    bool result = setString(OS_Generator_MicroTurbineFields::HeatRecoveryWaterOutletNodeName, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbine_Impl::setReferenceThermalEfficiencyUsingLowerHeatValue(double referenceThermalEfficiencyUsingLowerHeatValue) {
    bool result = setDouble(OS_Generator_MicroTurbineFields::ReferenceThermalEfficiencyUsingLowerHeatValue, referenceThermalEfficiencyUsingLowerHeatValue);
    return result;
  }

  void GeneratorMicroTurbine_Impl::resetReferenceThermalEfficiencyUsingLowerHeatValue() {
    bool result = setString(OS_Generator_MicroTurbineFields::ReferenceThermalEfficiencyUsingLowerHeatValue, "");
    OS_ASSERT(result);
  }

  void GeneratorMicroTurbine_Impl::setReferenceInletWaterTemperature(boost::optional<double> referenceInletWaterTemperature) {
    bool result(false);
    if (referenceInletWaterTemperature) {
      result = setDouble(OS_Generator_MicroTurbineFields::ReferenceInletWaterTemperature, referenceInletWaterTemperature.get());
    }
    else {
      resetReferenceInletWaterTemperature();
      result = true;
    }
    OS_ASSERT(result);
  }

  void GeneratorMicroTurbine_Impl::resetReferenceInletWaterTemperature() {
    bool result = setString(OS_Generator_MicroTurbineFields::ReferenceInletWaterTemperature, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbine_Impl::setHeatRecoveryWaterFlowOperatingMode(std::string heatRecoveryWaterFlowOperatingMode) {
    bool result = setString(OS_Generator_MicroTurbineFields::HeatRecoveryWaterFlowOperatingMode, heatRecoveryWaterFlowOperatingMode);
    return result;
  }

  void GeneratorMicroTurbine_Impl::resetHeatRecoveryWaterFlowOperatingMode() {
    bool result = setString(OS_Generator_MicroTurbineFields::HeatRecoveryWaterFlowOperatingMode, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbine_Impl::setReferenceHeatRecoveryWaterFlowRate(boost::optional<double> referenceHeatRecoveryWaterFlowRate) {
    bool result(false);
    if (referenceHeatRecoveryWaterFlowRate) {
      result = setDouble(OS_Generator_MicroTurbineFields::ReferenceHeatRecoveryWaterFlowRate, referenceHeatRecoveryWaterFlowRate.get());
    }
    else {
      resetReferenceHeatRecoveryWaterFlowRate();
      result = true;
    }
    return result;
  }

  void GeneratorMicroTurbine_Impl::resetReferenceHeatRecoveryWaterFlowRate() {
    bool result = setString(OS_Generator_MicroTurbineFields::ReferenceHeatRecoveryWaterFlowRate, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbine_Impl::setHeatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve(const boost::optional<Curve>& curve) {
    bool result(false);
    if (curve) {
      if(model() != curve.get().model())
      {
        LOG(briefDescription() << " does not below to the same model as the curve you want to set.");
        return false;
      }
      result = setPointer(OS_Generator_MicroTurbineFields::HeatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurveName, curve.get().handle());
    }
    else {
      resetHeatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve();
      result = true;
    }
    return result;
  }

  void GeneratorMicroTurbine_Impl::resetHeatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve() {
    bool result = setString(OS_Generator_MicroTurbineFields::HeatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurveName, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbine_Impl::setThermalEfficiencyFunctionofTemperatureandElevationCurve(const boost::optional<Curve>& curve) {
    bool result(false);
    if (curve) {
      if(model() != curve.get().model())
      {
        LOG(briefDescription() << " does not below to the same model as the curve you want to set.");
        return false;
      }
      result = setPointer(OS_Generator_MicroTurbineFields::ThermalEfficiencyFunctionofTemperatureandElevationCurveName, curve.get().handle());
    }
    else {
      resetThermalEfficiencyFunctionofTemperatureandElevationCurve();
      result = true;
    }
    return result;
  }

  void GeneratorMicroTurbine_Impl::resetThermalEfficiencyFunctionofTemperatureandElevationCurve() {
    bool result = setString(OS_Generator_MicroTurbineFields::ThermalEfficiencyFunctionofTemperatureandElevationCurveName, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbine_Impl::setHeatRecoveryRateFunctionofPartLoadRatioCurve(const boost::optional<Curve>& curve) {
    bool result(false);
    if (curve) {
      if(model() != curve.get().model())
      {
        LOG(briefDescription() << " does not below to the same model as the curve you want to set.");
        return false;
      }
      result = setPointer(OS_Generator_MicroTurbineFields::HeatRecoveryRateFunctionofPartLoadRatioCurveName, curve.get().handle());
    }
    else {
      resetHeatRecoveryRateFunctionofPartLoadRatioCurve();
      result = true;
    }
    return result;
  }

  void GeneratorMicroTurbine_Impl::resetHeatRecoveryRateFunctionofPartLoadRatioCurve() {
    bool result = setString(OS_Generator_MicroTurbineFields::HeatRecoveryRateFunctionofPartLoadRatioCurveName, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbine_Impl::setHeatRecoveryRateFunctionofInletWaterTemperatureCurve(const boost::optional<Curve>& curve) {
    bool result(false);
    if (curve) {
      if(model() != curve.get().model())
      {
        LOG(briefDescription() << " does not below to the same model as the curve you want to set.");
        return false;
      }
      result = setPointer(OS_Generator_MicroTurbineFields::HeatRecoveryRateFunctionofInletWaterTemperatureCurveName, curve.get().handle());
    }
    else {
      resetHeatRecoveryRateFunctionofInletWaterTemperatureCurve();
      result = true;
    }
    return result;
  }

  void GeneratorMicroTurbine_Impl::resetHeatRecoveryRateFunctionofInletWaterTemperatureCurve() {
    bool result = setString(OS_Generator_MicroTurbineFields::HeatRecoveryRateFunctionofInletWaterTemperatureCurveName, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbine_Impl::setHeatRecoveryRateFunctionofWaterFlowRateCurve(const boost::optional<Curve>& curve) {
    bool result(false);
    if (curve) {
      if(model() != curve.get().model())
      {
        LOG(briefDescription() << " does not below to the same model as the curve you want to set.");
        return false;
      }
      result = setPointer(OS_Generator_MicroTurbineFields::HeatRecoveryRateFunctionofWaterFlowRateCurveName, curve.get().handle());
    }
    else {
      resetHeatRecoveryRateFunctionofWaterFlowRateCurve();
      result = true;
    }
    return result;
  }

  void GeneratorMicroTurbine_Impl::resetHeatRecoveryRateFunctionofWaterFlowRateCurve() {
    bool result = setString(OS_Generator_MicroTurbineFields::HeatRecoveryRateFunctionofWaterFlowRateCurveName, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbine_Impl::setMinimumHeatRecoveryWaterFlowRate(double minimumHeatRecoveryWaterFlowRate) {
    bool result = setDouble(OS_Generator_MicroTurbineFields::MinimumHeatRecoveryWaterFlowRate, minimumHeatRecoveryWaterFlowRate);
    return result;
  }

  void GeneratorMicroTurbine_Impl::resetMinimumHeatRecoveryWaterFlowRate() {
    bool result = setString(OS_Generator_MicroTurbineFields::MinimumHeatRecoveryWaterFlowRate, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbine_Impl::setMaximumHeatRecoveryWaterFlowRate(double maximumHeatRecoveryWaterFlowRate) {
    bool result = setDouble(OS_Generator_MicroTurbineFields::MaximumHeatRecoveryWaterFlowRate, maximumHeatRecoveryWaterFlowRate);
    return result;
  }

  void GeneratorMicroTurbine_Impl::resetMaximumHeatRecoveryWaterFlowRate() {
    bool result = setString(OS_Generator_MicroTurbineFields::MaximumHeatRecoveryWaterFlowRate, "");
    OS_ASSERT(result);
  }

  void GeneratorMicroTurbine_Impl::setMaximumHeatRecoveryWaterTemperature(boost::optional<double> maximumHeatRecoveryWaterTemperature) {
    bool result(false);
    if (maximumHeatRecoveryWaterTemperature) {
      result = setDouble(OS_Generator_MicroTurbineFields::MaximumHeatRecoveryWaterTemperature, maximumHeatRecoveryWaterTemperature.get());
    }
    else {
      resetMaximumHeatRecoveryWaterTemperature();
      result = true;
    }
    OS_ASSERT(result);
  }

  void GeneratorMicroTurbine_Impl::resetMaximumHeatRecoveryWaterTemperature() {
    bool result = setString(OS_Generator_MicroTurbineFields::MaximumHeatRecoveryWaterTemperature, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbine_Impl::setCombustionAirInletNode(const boost::optional<Connection>& connection) {
    bool result(false);
    if (connection) {
      result = setPointer(OS_Generator_MicroTurbineFields::CombustionAirInletNodeName, connection.get().handle());
    }
    else {
      resetCombustionAirInletNode();
      result = true;
    }
    return result;
  }

  void GeneratorMicroTurbine_Impl::resetCombustionAirInletNode() {
    bool result = setString(OS_Generator_MicroTurbineFields::CombustionAirInletNodeName, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbine_Impl::setCombustionAirOutletNode(const boost::optional<Connection>& connection) {
    bool result(false);
    if (connection) {
      result = setPointer(OS_Generator_MicroTurbineFields::CombustionAirOutletNodeName, connection.get().handle());
    }
    else {
      resetCombustionAirOutletNode();
      result = true;
    }
    return result;
  }

  void GeneratorMicroTurbine_Impl::resetCombustionAirOutletNode() {
    bool result = setString(OS_Generator_MicroTurbineFields::CombustionAirOutletNodeName, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbine_Impl::setReferenceExhaustAirMassFlowRate(boost::optional<double> referenceExhaustAirMassFlowRate) {
    bool result(false);
    if (referenceExhaustAirMassFlowRate) {
      result = setDouble(OS_Generator_MicroTurbineFields::ReferenceExhaustAirMassFlowRate, referenceExhaustAirMassFlowRate.get());
    }
    else {
      resetReferenceExhaustAirMassFlowRate();
      result = true;
    }
    return result;
  }

  void GeneratorMicroTurbine_Impl::resetReferenceExhaustAirMassFlowRate() {
    bool result = setString(OS_Generator_MicroTurbineFields::ReferenceExhaustAirMassFlowRate, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbine_Impl::setExhaustAirFlowRateFunctionofTemperatureCurve(const boost::optional<Curve>& curve) {
    bool result(false);
    if (curve) {
      if(model() != curve.get().model())
      {
        LOG(briefDescription() << " does not below to the same model as the curve you want to set.");
        return false;
      }
      result = setPointer(OS_Generator_MicroTurbineFields::ExhaustAirFlowRateFunctionofTemperatureCurveName, curve.get().handle());
    }
    else {
      resetExhaustAirFlowRateFunctionofTemperatureCurve();
      result = true;
    }
    return result;
  }

  void GeneratorMicroTurbine_Impl::resetExhaustAirFlowRateFunctionofTemperatureCurve() {
    bool result = setString(OS_Generator_MicroTurbineFields::ExhaustAirFlowRateFunctionofTemperatureCurveName, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbine_Impl::setExhaustAirFlowRateFunctionofPartLoadRatioCurve(const boost::optional<Curve>& curve) {
    bool result(false);
    if (curve) {
      if(model() != curve.get().model())
      {
        LOG(briefDescription() << " does not below to the same model as the curve you want to set.");
        return false;
      }  
      result = setPointer(OS_Generator_MicroTurbineFields::ExhaustAirFlowRateFunctionofPartLoadRatioCurveName, curve.get().handle());
    }
    else {
      resetExhaustAirFlowRateFunctionofPartLoadRatioCurve();
      result = true;
    }
    return result;
  }

  void GeneratorMicroTurbine_Impl::resetExhaustAirFlowRateFunctionofPartLoadRatioCurve() {
    bool result = setString(OS_Generator_MicroTurbineFields::ExhaustAirFlowRateFunctionofPartLoadRatioCurveName, "");
    OS_ASSERT(result);
  }

  void GeneratorMicroTurbine_Impl::setNominalExhaustAirOutletTemperature(boost::optional<double> nominalExhaustAirOutletTemperature) {
    bool result(false);
    if (nominalExhaustAirOutletTemperature) {
      result = setDouble(OS_Generator_MicroTurbineFields::NominalExhaustAirOutletTemperature, nominalExhaustAirOutletTemperature.get());
    }
    else {
      resetNominalExhaustAirOutletTemperature();
      result = true;
    }
    OS_ASSERT(result);
  }

  void GeneratorMicroTurbine_Impl::resetNominalExhaustAirOutletTemperature() {
    bool result = setString(OS_Generator_MicroTurbineFields::NominalExhaustAirOutletTemperature, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbine_Impl::setExhaustAirTemperatureFunctionofTemperatureCurve(const boost::optional<Curve>& curve) {
    bool result(false);
    if (curve) {
      if(model() != curve.get().model())
      {
        LOG(briefDescription() << " does not below to the same model as the curve you want to set.");
        return false;
      }
      result = setPointer(OS_Generator_MicroTurbineFields::ExhaustAirTemperatureFunctionofTemperatureCurveName, curve.get().handle());
    }
    else {
      resetExhaustAirTemperatureFunctionofTemperatureCurve();
      result = true;
    }
    return result;
  }

  void GeneratorMicroTurbine_Impl::resetExhaustAirTemperatureFunctionofTemperatureCurve() {
    bool result = setString(OS_Generator_MicroTurbineFields::ExhaustAirTemperatureFunctionofTemperatureCurveName, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbine_Impl::setExhaustAirTemperatureFunctionofPartLoadRatioCurve(const boost::optional<Curve>& curve) {
    bool result(false);
    if (curve) {
      if(model() != curve.get().model())
      {
        LOG(briefDescription() << " does not below to the same model as the curve you want to set.");
        return false;
      }
      result = setPointer(OS_Generator_MicroTurbineFields::ExhaustAirTemperatureFunctionofPartLoadRatioCurveName, curve.get().handle());
    }
    else {
      resetExhaustAirTemperatureFunctionofPartLoadRatioCurve();
      result = true;
    }
    return result;
  }

  void GeneratorMicroTurbine_Impl::resetExhaustAirTemperatureFunctionofPartLoadRatioCurve() {
    bool result = setString(OS_Generator_MicroTurbineFields::ExhaustAirTemperatureFunctionofPartLoadRatioCurveName, "");
    OS_ASSERT(result);
  }

  boost::optional<Curve> GeneratorMicroTurbine_Impl::optionalElectricalPowerFunctionofTemperatureandElevationCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Generator_MicroTurbineFields::ElectricalPowerFunctionofTemperatureandElevationCurveName);
  }

  boost::optional<Curve> GeneratorMicroTurbine_Impl::optionalElectricalEfficiencyFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Generator_MicroTurbineFields::ElectricalEfficiencyFunctionofTemperatureCurveName);
  }

  boost::optional<Curve> GeneratorMicroTurbine_Impl::optionalElectricalEfficiencyFunctionofPartLoadRatioCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Generator_MicroTurbineFields::ElectricalEfficiencyFunctionofPartLoadRatioCurveName);
  }

} // detail

GeneratorMicroTurbine::GeneratorMicroTurbine(const Model& model)
  : Generator(GeneratorMicroTurbine::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::GeneratorMicroTurbine_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  //     OS_Generator_MicroTurbineFields::ElectricalPowerFunctionofTemperatureandElevationCurveName
  //     OS_Generator_MicroTurbineFields::ElectricalEfficiencyFunctionofTemperatureCurveName
  //     OS_Generator_MicroTurbineFields::ElectricalEfficiencyFunctionofPartLoadRatioCurveName
  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
  // ok = setReferenceElectricalPowerOutput();
  OS_ASSERT(ok);
  // ok = setReferenceElectricalEfficiencyUsingLowerHeatingValue();
  OS_ASSERT(ok);
  // ok = setElectricalPowerFunctionofTemperatureandElevationCurve();
  OS_ASSERT(ok);
  // ok = setElectricalEfficiencyFunctionofTemperatureCurve();
  OS_ASSERT(ok);
  // ok = setElectricalEfficiencyFunctionofPartLoadRatioCurve();
  OS_ASSERT(ok);
}

IddObjectType GeneratorMicroTurbine::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Generator_MicroTurbine);
}

std::vector<std::string> GeneratorMicroTurbine::fuelTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Generator_MicroTurbineFields::FuelType);
}

std::vector<std::string> GeneratorMicroTurbine::heatRecoveryWaterFlowOperatingModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Generator_MicroTurbineFields::HeatRecoveryWaterFlowOperatingMode);
}

double GeneratorMicroTurbine::referenceElectricalPowerOutput() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->referenceElectricalPowerOutput();
}

double GeneratorMicroTurbine::minimumFullLoadElectricalPowerOutput() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->minimumFullLoadElectricalPowerOutput();
}

bool GeneratorMicroTurbine::isMinimumFullLoadElectricalPowerOutputDefaulted() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->isMinimumFullLoadElectricalPowerOutputDefaulted();
}

boost::optional<double> GeneratorMicroTurbine::maximumFullLoadElectricalPowerOutput() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->maximumFullLoadElectricalPowerOutput();
}

double GeneratorMicroTurbine::referenceElectricalEfficiencyUsingLowerHeatingValue() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->referenceElectricalEfficiencyUsingLowerHeatingValue();
}

double GeneratorMicroTurbine::referenceCombustionAirInletTemperature() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->referenceCombustionAirInletTemperature();
}

bool GeneratorMicroTurbine::isReferenceCombustionAirInletTemperatureDefaulted() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->isReferenceCombustionAirInletTemperatureDefaulted();
}

double GeneratorMicroTurbine::referenceCombustionAirInletHumidityRatio() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->referenceCombustionAirInletHumidityRatio();
}

bool GeneratorMicroTurbine::isReferenceCombustionAirInletHumidityRatioDefaulted() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->isReferenceCombustionAirInletHumidityRatioDefaulted();
}

double GeneratorMicroTurbine::referenceElevation() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->referenceElevation();
}

bool GeneratorMicroTurbine::isReferenceElevationDefaulted() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->isReferenceElevationDefaulted();
}

Curve GeneratorMicroTurbine::electricalPowerFunctionofTemperatureandElevationCurve() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->electricalPowerFunctionofTemperatureandElevationCurve();
}

Curve GeneratorMicroTurbine::electricalEfficiencyFunctionofTemperatureCurve() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->electricalEfficiencyFunctionofTemperatureCurve();
}

Curve GeneratorMicroTurbine::electricalEfficiencyFunctionofPartLoadRatioCurve() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->electricalEfficiencyFunctionofPartLoadRatioCurve();
}

std::string GeneratorMicroTurbine::fuelType() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->fuelType();
}

bool GeneratorMicroTurbine::isFuelTypeDefaulted() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->isFuelTypeDefaulted();
}

double GeneratorMicroTurbine::fuelHigherHeatingValue() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->fuelHigherHeatingValue();
}

bool GeneratorMicroTurbine::isFuelHigherHeatingValueDefaulted() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->isFuelHigherHeatingValueDefaulted();
}

double GeneratorMicroTurbine::fuelLowerHeatingValue() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->fuelLowerHeatingValue();
}

bool GeneratorMicroTurbine::isFuelLowerHeatingValueDefaulted() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->isFuelLowerHeatingValueDefaulted();
}

double GeneratorMicroTurbine::standbyPower() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->standbyPower();
}

bool GeneratorMicroTurbine::isStandbyPowerDefaulted() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->isStandbyPowerDefaulted();
}

double GeneratorMicroTurbine::ancillaryPower() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->ancillaryPower();
}

bool GeneratorMicroTurbine::isAncillaryPowerDefaulted() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->isAncillaryPowerDefaulted();
}

boost::optional<Curve> GeneratorMicroTurbine::ancillaryPowerFunctionofFuelInputCurve() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->ancillaryPowerFunctionofFuelInputCurve();
}

boost::optional<Connection> GeneratorMicroTurbine::heatRecoveryWaterInletNode() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->heatRecoveryWaterInletNode();
}

boost::optional<Connection> GeneratorMicroTurbine::heatRecoveryWaterOutletNode() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->heatRecoveryWaterOutletNode();
}

double GeneratorMicroTurbine::referenceThermalEfficiencyUsingLowerHeatValue() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->referenceThermalEfficiencyUsingLowerHeatValue();
}

bool GeneratorMicroTurbine::isReferenceThermalEfficiencyUsingLowerHeatValueDefaulted() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->isReferenceThermalEfficiencyUsingLowerHeatValueDefaulted();
}

boost::optional<double> GeneratorMicroTurbine::referenceInletWaterTemperature() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->referenceInletWaterTemperature();
}

std::string GeneratorMicroTurbine::heatRecoveryWaterFlowOperatingMode() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->heatRecoveryWaterFlowOperatingMode();
}

bool GeneratorMicroTurbine::isHeatRecoveryWaterFlowOperatingModeDefaulted() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->isHeatRecoveryWaterFlowOperatingModeDefaulted();
}

boost::optional<double> GeneratorMicroTurbine::referenceHeatRecoveryWaterFlowRate() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->referenceHeatRecoveryWaterFlowRate();
}

boost::optional<Curve> GeneratorMicroTurbine::heatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->heatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve();
}

boost::optional<Curve> GeneratorMicroTurbine::thermalEfficiencyFunctionofTemperatureandElevationCurve() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->thermalEfficiencyFunctionofTemperatureandElevationCurve();
}

boost::optional<Curve> GeneratorMicroTurbine::heatRecoveryRateFunctionofPartLoadRatioCurve() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->heatRecoveryRateFunctionofPartLoadRatioCurve();
}

boost::optional<Curve> GeneratorMicroTurbine::heatRecoveryRateFunctionofInletWaterTemperatureCurve() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->heatRecoveryRateFunctionofInletWaterTemperatureCurve();
}

boost::optional<Curve> GeneratorMicroTurbine::heatRecoveryRateFunctionofWaterFlowRateCurve() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->heatRecoveryRateFunctionofWaterFlowRateCurve();
}

double GeneratorMicroTurbine::minimumHeatRecoveryWaterFlowRate() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->minimumHeatRecoveryWaterFlowRate();
}

bool GeneratorMicroTurbine::isMinimumHeatRecoveryWaterFlowRateDefaulted() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->isMinimumHeatRecoveryWaterFlowRateDefaulted();
}

double GeneratorMicroTurbine::maximumHeatRecoveryWaterFlowRate() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->maximumHeatRecoveryWaterFlowRate();
}

bool GeneratorMicroTurbine::isMaximumHeatRecoveryWaterFlowRateDefaulted() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->isMaximumHeatRecoveryWaterFlowRateDefaulted();
}

boost::optional<double> GeneratorMicroTurbine::maximumHeatRecoveryWaterTemperature() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->maximumHeatRecoveryWaterTemperature();
}

boost::optional<Connection> GeneratorMicroTurbine::combustionAirInletNode() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->combustionAirInletNode();
}

boost::optional<Connection> GeneratorMicroTurbine::combustionAirOutletNode() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->combustionAirOutletNode();
}

boost::optional<double> GeneratorMicroTurbine::referenceExhaustAirMassFlowRate() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->referenceExhaustAirMassFlowRate();
}

boost::optional<Curve> GeneratorMicroTurbine::exhaustAirFlowRateFunctionofTemperatureCurve() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->exhaustAirFlowRateFunctionofTemperatureCurve();
}

boost::optional<Curve> GeneratorMicroTurbine::exhaustAirFlowRateFunctionofPartLoadRatioCurve() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->exhaustAirFlowRateFunctionofPartLoadRatioCurve();
}

boost::optional<double> GeneratorMicroTurbine::nominalExhaustAirOutletTemperature() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->nominalExhaustAirOutletTemperature();
}

boost::optional<Curve> GeneratorMicroTurbine::exhaustAirTemperatureFunctionofTemperatureCurve() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->exhaustAirTemperatureFunctionofTemperatureCurve();
}

boost::optional<Curve> GeneratorMicroTurbine::exhaustAirTemperatureFunctionofPartLoadRatioCurve() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->exhaustAirTemperatureFunctionofPartLoadRatioCurve();
}

bool GeneratorMicroTurbine::setReferenceElectricalPowerOutput(double referenceElectricalPowerOutput) {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->setReferenceElectricalPowerOutput(referenceElectricalPowerOutput);
}

bool GeneratorMicroTurbine::setMinimumFullLoadElectricalPowerOutput(double minimumFullLoadElectricalPowerOutput) {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->setMinimumFullLoadElectricalPowerOutput(minimumFullLoadElectricalPowerOutput);
}

void GeneratorMicroTurbine::resetMinimumFullLoadElectricalPowerOutput() {
  getImpl<detail::GeneratorMicroTurbine_Impl>()->resetMinimumFullLoadElectricalPowerOutput();
}

bool GeneratorMicroTurbine::setMaximumFullLoadElectricalPowerOutput(double maximumFullLoadElectricalPowerOutput) {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->setMaximumFullLoadElectricalPowerOutput(maximumFullLoadElectricalPowerOutput);
}

void GeneratorMicroTurbine::resetMaximumFullLoadElectricalPowerOutput() {
  getImpl<detail::GeneratorMicroTurbine_Impl>()->resetMaximumFullLoadElectricalPowerOutput();
}

bool GeneratorMicroTurbine::setReferenceElectricalEfficiencyUsingLowerHeatingValue(double referenceElectricalEfficiencyUsingLowerHeatingValue) {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->setReferenceElectricalEfficiencyUsingLowerHeatingValue(referenceElectricalEfficiencyUsingLowerHeatingValue);
}

void GeneratorMicroTurbine::setReferenceCombustionAirInletTemperature(double referenceCombustionAirInletTemperature) {
  getImpl<detail::GeneratorMicroTurbine_Impl>()->setReferenceCombustionAirInletTemperature(referenceCombustionAirInletTemperature);
}

void GeneratorMicroTurbine::resetReferenceCombustionAirInletTemperature() {
  getImpl<detail::GeneratorMicroTurbine_Impl>()->resetReferenceCombustionAirInletTemperature();
}

bool GeneratorMicroTurbine::setReferenceCombustionAirInletHumidityRatio(double referenceCombustionAirInletHumidityRatio) {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->setReferenceCombustionAirInletHumidityRatio(referenceCombustionAirInletHumidityRatio);
}

void GeneratorMicroTurbine::resetReferenceCombustionAirInletHumidityRatio() {
  getImpl<detail::GeneratorMicroTurbine_Impl>()->resetReferenceCombustionAirInletHumidityRatio();
}

bool GeneratorMicroTurbine::setReferenceElevation(double referenceElevation) {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->setReferenceElevation(referenceElevation);
}

void GeneratorMicroTurbine::resetReferenceElevation() {
  getImpl<detail::GeneratorMicroTurbine_Impl>()->resetReferenceElevation();
}

bool GeneratorMicroTurbine::setElectricalPowerFunctionofTemperatureandElevationCurve(const Curve& curve) {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->setElectricalPowerFunctionofTemperatureandElevationCurve(curve);
}

bool GeneratorMicroTurbine::setElectricalEfficiencyFunctionofTemperatureCurve(const Curve& curve) {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->setElectricalEfficiencyFunctionofTemperatureCurve(curve);
}

bool GeneratorMicroTurbine::setElectricalEfficiencyFunctionofPartLoadRatioCurve(const Curve& curve) {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->setElectricalEfficiencyFunctionofPartLoadRatioCurve(curve);
}

bool GeneratorMicroTurbine::setFuelType(std::string fuelType) {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->setFuelType(fuelType);
}

void GeneratorMicroTurbine::resetFuelType() {
  getImpl<detail::GeneratorMicroTurbine_Impl>()->resetFuelType();
}

bool GeneratorMicroTurbine::setFuelHigherHeatingValue(double fuelHigherHeatingValue) {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->setFuelHigherHeatingValue(fuelHigherHeatingValue);
}

void GeneratorMicroTurbine::resetFuelHigherHeatingValue() {
  getImpl<detail::GeneratorMicroTurbine_Impl>()->resetFuelHigherHeatingValue();
}

bool GeneratorMicroTurbine::setFuelLowerHeatingValue(double fuelLowerHeatingValue) {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->setFuelLowerHeatingValue(fuelLowerHeatingValue);
}

void GeneratorMicroTurbine::resetFuelLowerHeatingValue() {
  getImpl<detail::GeneratorMicroTurbine_Impl>()->resetFuelLowerHeatingValue();
}

bool GeneratorMicroTurbine::setStandbyPower(double standbyPower) {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->setStandbyPower(standbyPower);
}

void GeneratorMicroTurbine::resetStandbyPower() {
  getImpl<detail::GeneratorMicroTurbine_Impl>()->resetStandbyPower();
}

bool GeneratorMicroTurbine::setAncillaryPower(double ancillaryPower) {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->setAncillaryPower(ancillaryPower);
}

void GeneratorMicroTurbine::resetAncillaryPower() {
  getImpl<detail::GeneratorMicroTurbine_Impl>()->resetAncillaryPower();
}

bool GeneratorMicroTurbine::setAncillaryPowerFunctionofFuelInputCurve(const Curve& curve) {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->setAncillaryPowerFunctionofFuelInputCurve(curve);
}

void GeneratorMicroTurbine::resetAncillaryPowerFunctionofFuelInputCurve() {
  getImpl<detail::GeneratorMicroTurbine_Impl>()->resetAncillaryPowerFunctionofFuelInputCurve();
}

bool GeneratorMicroTurbine::setHeatRecoveryWaterInletNode(const Connection& connection) {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->setHeatRecoveryWaterInletNode(connection);
}

void GeneratorMicroTurbine::resetHeatRecoveryWaterInletNode() {
  getImpl<detail::GeneratorMicroTurbine_Impl>()->resetHeatRecoveryWaterInletNode();
}

bool GeneratorMicroTurbine::setHeatRecoveryWaterOutletNode(const Connection& connection) {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->setHeatRecoveryWaterOutletNode(connection);
}

void GeneratorMicroTurbine::resetHeatRecoveryWaterOutletNode() {
  getImpl<detail::GeneratorMicroTurbine_Impl>()->resetHeatRecoveryWaterOutletNode();
}

bool GeneratorMicroTurbine::setReferenceThermalEfficiencyUsingLowerHeatValue(double referenceThermalEfficiencyUsingLowerHeatValue) {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->setReferenceThermalEfficiencyUsingLowerHeatValue(referenceThermalEfficiencyUsingLowerHeatValue);
}

void GeneratorMicroTurbine::resetReferenceThermalEfficiencyUsingLowerHeatValue() {
  getImpl<detail::GeneratorMicroTurbine_Impl>()->resetReferenceThermalEfficiencyUsingLowerHeatValue();
}

void GeneratorMicroTurbine::setReferenceInletWaterTemperature(double referenceInletWaterTemperature) {
  getImpl<detail::GeneratorMicroTurbine_Impl>()->setReferenceInletWaterTemperature(referenceInletWaterTemperature);
}

void GeneratorMicroTurbine::resetReferenceInletWaterTemperature() {
  getImpl<detail::GeneratorMicroTurbine_Impl>()->resetReferenceInletWaterTemperature();
}

bool GeneratorMicroTurbine::setHeatRecoveryWaterFlowOperatingMode(std::string heatRecoveryWaterFlowOperatingMode) {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->setHeatRecoveryWaterFlowOperatingMode(heatRecoveryWaterFlowOperatingMode);
}

void GeneratorMicroTurbine::resetHeatRecoveryWaterFlowOperatingMode() {
  getImpl<detail::GeneratorMicroTurbine_Impl>()->resetHeatRecoveryWaterFlowOperatingMode();
}

bool GeneratorMicroTurbine::setReferenceHeatRecoveryWaterFlowRate(double referenceHeatRecoveryWaterFlowRate) {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->setReferenceHeatRecoveryWaterFlowRate(referenceHeatRecoveryWaterFlowRate);
}

void GeneratorMicroTurbine::resetReferenceHeatRecoveryWaterFlowRate() {
  getImpl<detail::GeneratorMicroTurbine_Impl>()->resetReferenceHeatRecoveryWaterFlowRate();
}

bool GeneratorMicroTurbine::setHeatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve(const Curve& curve) {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->setHeatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve(curve);
}

void GeneratorMicroTurbine::resetHeatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve() {
  getImpl<detail::GeneratorMicroTurbine_Impl>()->resetHeatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve();
}

bool GeneratorMicroTurbine::setThermalEfficiencyFunctionofTemperatureandElevationCurve(const Curve& curve) {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->setThermalEfficiencyFunctionofTemperatureandElevationCurve(curve);
}

void GeneratorMicroTurbine::resetThermalEfficiencyFunctionofTemperatureandElevationCurve() {
  getImpl<detail::GeneratorMicroTurbine_Impl>()->resetThermalEfficiencyFunctionofTemperatureandElevationCurve();
}

bool GeneratorMicroTurbine::setHeatRecoveryRateFunctionofPartLoadRatioCurve(const Curve& curve) {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->setHeatRecoveryRateFunctionofPartLoadRatioCurve(curve);
}

void GeneratorMicroTurbine::resetHeatRecoveryRateFunctionofPartLoadRatioCurve() {
  getImpl<detail::GeneratorMicroTurbine_Impl>()->resetHeatRecoveryRateFunctionofPartLoadRatioCurve();
}

bool GeneratorMicroTurbine::setHeatRecoveryRateFunctionofInletWaterTemperatureCurve(const Curve& curve) {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->setHeatRecoveryRateFunctionofInletWaterTemperatureCurve(curve);
}

void GeneratorMicroTurbine::resetHeatRecoveryRateFunctionofInletWaterTemperatureCurve() {
  getImpl<detail::GeneratorMicroTurbine_Impl>()->resetHeatRecoveryRateFunctionofInletWaterTemperatureCurve();
}

bool GeneratorMicroTurbine::setHeatRecoveryRateFunctionofWaterFlowRateCurve(const Curve& curve) {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->setHeatRecoveryRateFunctionofWaterFlowRateCurve(curve);
}

void GeneratorMicroTurbine::resetHeatRecoveryRateFunctionofWaterFlowRateCurve() {
  getImpl<detail::GeneratorMicroTurbine_Impl>()->resetHeatRecoveryRateFunctionofWaterFlowRateCurve();
}

bool GeneratorMicroTurbine::setMinimumHeatRecoveryWaterFlowRate(double minimumHeatRecoveryWaterFlowRate) {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->setMinimumHeatRecoveryWaterFlowRate(minimumHeatRecoveryWaterFlowRate);
}

void GeneratorMicroTurbine::resetMinimumHeatRecoveryWaterFlowRate() {
  getImpl<detail::GeneratorMicroTurbine_Impl>()->resetMinimumHeatRecoveryWaterFlowRate();
}

bool GeneratorMicroTurbine::setMaximumHeatRecoveryWaterFlowRate(double maximumHeatRecoveryWaterFlowRate) {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->setMaximumHeatRecoveryWaterFlowRate(maximumHeatRecoveryWaterFlowRate);
}

void GeneratorMicroTurbine::resetMaximumHeatRecoveryWaterFlowRate() {
  getImpl<detail::GeneratorMicroTurbine_Impl>()->resetMaximumHeatRecoveryWaterFlowRate();
}

void GeneratorMicroTurbine::setMaximumHeatRecoveryWaterTemperature(double maximumHeatRecoveryWaterTemperature) {
  getImpl<detail::GeneratorMicroTurbine_Impl>()->setMaximumHeatRecoveryWaterTemperature(maximumHeatRecoveryWaterTemperature);
}

void GeneratorMicroTurbine::resetMaximumHeatRecoveryWaterTemperature() {
  getImpl<detail::GeneratorMicroTurbine_Impl>()->resetMaximumHeatRecoveryWaterTemperature();
}

bool GeneratorMicroTurbine::setCombustionAirInletNode(const Connection& connection) {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->setCombustionAirInletNode(connection);
}

void GeneratorMicroTurbine::resetCombustionAirInletNode() {
  getImpl<detail::GeneratorMicroTurbine_Impl>()->resetCombustionAirInletNode();
}

bool GeneratorMicroTurbine::setCombustionAirOutletNode(const Connection& connection) {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->setCombustionAirOutletNode(connection);
}

void GeneratorMicroTurbine::resetCombustionAirOutletNode() {
  getImpl<detail::GeneratorMicroTurbine_Impl>()->resetCombustionAirOutletNode();
}

bool GeneratorMicroTurbine::setReferenceExhaustAirMassFlowRate(double referenceExhaustAirMassFlowRate) {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->setReferenceExhaustAirMassFlowRate(referenceExhaustAirMassFlowRate);
}

void GeneratorMicroTurbine::resetReferenceExhaustAirMassFlowRate() {
  getImpl<detail::GeneratorMicroTurbine_Impl>()->resetReferenceExhaustAirMassFlowRate();
}

bool GeneratorMicroTurbine::setExhaustAirFlowRateFunctionofTemperatureCurve(const Curve& curve) {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->setExhaustAirFlowRateFunctionofTemperatureCurve(curve);
}

void GeneratorMicroTurbine::resetExhaustAirFlowRateFunctionofTemperatureCurve() {
  getImpl<detail::GeneratorMicroTurbine_Impl>()->resetExhaustAirFlowRateFunctionofTemperatureCurve();
}

bool GeneratorMicroTurbine::setExhaustAirFlowRateFunctionofPartLoadRatioCurve(const Curve& curve) {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->setExhaustAirFlowRateFunctionofPartLoadRatioCurve(curve);
}

void GeneratorMicroTurbine::resetExhaustAirFlowRateFunctionofPartLoadRatioCurve() {
  getImpl<detail::GeneratorMicroTurbine_Impl>()->resetExhaustAirFlowRateFunctionofPartLoadRatioCurve();
}

void GeneratorMicroTurbine::setNominalExhaustAirOutletTemperature(double nominalExhaustAirOutletTemperature) {
  getImpl<detail::GeneratorMicroTurbine_Impl>()->setNominalExhaustAirOutletTemperature(nominalExhaustAirOutletTemperature);
}

void GeneratorMicroTurbine::resetNominalExhaustAirOutletTemperature() {
  getImpl<detail::GeneratorMicroTurbine_Impl>()->resetNominalExhaustAirOutletTemperature();
}

bool GeneratorMicroTurbine::setExhaustAirTemperatureFunctionofTemperatureCurve(const Curve& curve) {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->setExhaustAirTemperatureFunctionofTemperatureCurve(curve);
}

void GeneratorMicroTurbine::resetExhaustAirTemperatureFunctionofTemperatureCurve() {
  getImpl<detail::GeneratorMicroTurbine_Impl>()->resetExhaustAirTemperatureFunctionofTemperatureCurve();
}

bool GeneratorMicroTurbine::setExhaustAirTemperatureFunctionofPartLoadRatioCurve(const Curve& curve) {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->setExhaustAirTemperatureFunctionofPartLoadRatioCurve(curve);
}

void GeneratorMicroTurbine::resetExhaustAirTemperatureFunctionofPartLoadRatioCurve() {
  getImpl<detail::GeneratorMicroTurbine_Impl>()->resetExhaustAirTemperatureFunctionofPartLoadRatioCurve();
}


/// New Methods
  // Problably useless...
  //boost::optional<std::string> GeneratorMicroTurbine_Impl::heatRecoveryWaterInletNodeName() const {
  //  return getString(OS_Generator_MicroTurbineFields::HeatRecoveryWaterInletNodeName,true);
  //}

  ModelObject GeneratorMicroTurbine_Impl::clone(Model model) const
  {
    GeneratorMicroTurbine newMCHP = StraightComponent_Impl::clone(model).cast<GeneratorMicroTurbine>();

    return newMCHP;
  }
  
  bool GeneratorMicroTurbine_Impl::addToNode(Node & node)
  {
    if( boost::optional<PlantLoop> plant = node.plantLoop() )
    {
      if( plant->supplyComponent(node.handle()) )
      {
        if( StraightComponent_Impl::addToNode(node) )
        {
          return true;
        }
      }
    }

    return false;
  }




/// @cond
GeneratorMicroTurbine::GeneratorMicroTurbine(std::shared_ptr<detail::GeneratorMicroTurbine_Impl> impl)
  : Generator(impl)
{}
/// @endcond

} // model
} // openstudio

