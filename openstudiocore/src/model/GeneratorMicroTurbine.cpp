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

#include "StraightComponent.hpp"
#include "StraightComponent_Impl.hpp"
#include "GeneratorMicroTurbineHeatRecovery.hpp"
#include "GeneratorMicroTurbineHeatRecovery_Impl.hpp"

// TODO: Add DataTables if added later to OS?
//#include "DataTables.hpp" // UniVariateTables and BiVariateTables
//#include "DataTables.hpp"

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

  // If defaulted, return referenceElectricalPowerOutput
  double GeneratorMicroTurbine_Impl::maximumFullLoadElectricalPowerOutput() const {
	// TODO: Check if that's the typical way of doing this... Might has well write code that looks the same as usual. @danmacumber?
	  boost::optional<double> maximumFullLoadElectricalPowerOutput = getDouble(OS_Generator_MicroTurbineFields::MaximumFullLoadElectricalPowerOutput,true);
		// If there is a Heat Recovery Object
		if (maximumFullLoadElectricalPowerOutput) {
			// Get it and return
			return maximumFullLoadElectricalPowerOutput.get();
		}
		else { 
			boost::optional<double> referenceElectricalPowerOutput = getDouble(OS_Generator_MicroTurbineFields::ReferenceElectricalPowerOutput,true);
			OS_ASSERT(referenceElectricalPowerOutput);
			return referenceElectricalPowerOutput.get();
		}
  }
	
	bool GeneratorMicroTurbine_Impl::isMaximumFullLoadElectricalPowerOutputDefaulted() const {
    return isEmpty(OS_Generator_MicroTurbineFields::MaximumFullLoadElectricalPowerOutput);
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
	  return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Generator_MicroTurbineFields::ElectricalPowerFunctionofTemperatureandElevationCurveName);
  }

  Curve GeneratorMicroTurbine_Impl::electricalEfficiencyFunctionofTemperatureCurve() const {
	  return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Generator_MicroTurbineFields::ElectricalEfficiencyFunctionofTemperatureCurveName);
  }

  Curve GeneratorMicroTurbine_Impl::electricalEfficiencyFunctionofPartLoadRatioCurve() const {
	  return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Generator_MicroTurbineFields::ElectricalEfficiencyFunctionofPartLoadRatioCurveName);
  }
/*  Curve GeneratorMicroTurbine_Impl::electricalEfficiencyFunctionofPartLoadRatioCurve() const {
    boost::optional<Curve> value = optionalElectricalEfficiencyFunctionofPartLoadRatioCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Electrical Efficiency Function of Part Load Ratio Curve attached.");
    }
    return value.get();
  }*/
  

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

  // Optional Generator:MicroTurbine:HeatRecovery
  boost::optional<GeneratorMicroTurbineHeatRecovery> GeneratorMicroTurbine_Impl::generatorMicroTurbineHeatRecovery() const {
    return getObject<ModelObject>().getModelObjectTarget<GeneratorMicroTurbineHeatRecovery>(OS_Generator_MicroTurbineFields::GeneratorMicroTurbineHeatRecoveryName);
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
      LOG(briefDescription() << " does not belong to the same model as the curve you want to set.");
      return false;
    }
    bool result = setPointer(OS_Generator_MicroTurbineFields::ElectricalPowerFunctionofTemperatureandElevationCurveName, curve.handle());
    return result;
  }

  bool GeneratorMicroTurbine_Impl::setElectricalEfficiencyFunctionofTemperatureCurve(const Curve& curve) {
    if(model() != curve.model())
    {
      LOG(briefDescription() << " does not belong to the same model as the curve you want to set.");
      return false;
    }
    bool result = setPointer(OS_Generator_MicroTurbineFields::ElectricalEfficiencyFunctionofTemperatureCurveName, curve.handle());
    return result;
  }

  bool GeneratorMicroTurbine_Impl::setElectricalEfficiencyFunctionofPartLoadRatioCurve(const Curve& curve) {
    if(model() != curve.model())
    {
      LOG(briefDescription() << " does not belong to the same model as the curve you want to set.");
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
        LOG(briefDescription() << " does not belong to the same model as the curve you want to set.");
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
  
  
  // Optional Generator:MicroTurbine:HeatRecovery
  bool GeneratorMicroTurbine_Impl::setGeneratorMicroTurbineHeatRecovery(const boost::optional<GeneratorMicroTurbineHeatRecovery>& generatorMicroTurbineHeatRecovery) {
    bool result(false);
    if (generatorMicroTurbineHeatRecovery) {
      if(model() != generatorMicroTurbineHeatRecovery.get().model())
      {
        LOG(briefDescription() << " does not belong to the same model as the Generator:MicroTurbine:HeatRecovery you want to set.");
        return false;
      }
      result = setPointer(OS_Generator_MicroTurbineFields::GeneratorMicroTurbineHeatRecoveryName, generatorMicroTurbineHeatRecovery.get().handle());
    }
    else {
      resetGeneratorMicroTurbineHeatRecovery();
      result = true;
    }
    return result;
  }

  void GeneratorMicroTurbine_Impl::resetGeneratorMicroTurbineHeatRecovery() {
    bool result = setString(OS_Generator_MicroTurbineFields::GeneratorMicroTurbineHeatRecoveryName, "");
    OS_ASSERT(result);
  }
  
  boost::optional<GeneratorMicroTurbineHeatRecovery> GeneratorMicroTurbine_Impl::generatorMicroTurbineHeatRecovery() const {
    return getObject<ModelObject>().getModelObjectTarget<GeneratorMicroTurbineHeatRecovery>(OS_Generator_MicroTurbineFields::GeneratorMicroTurbineHeatRecoveryName);
  }
  
  // Optional Generator:MicroTurbine:HeatRecovery
    bool setGeneratorMicroTurbineHeatRecovery(const GeneratorMicroTurbineHeatRecovery& generatorMicroTurbineHeatRecovery);
    void resetGeneratorMicroTurbineHeatRecovery();
    
    
    

  /* void GeneratorMicroTurbine_Impl::resetCombustionAirInletNode() {
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
  } */

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
        LOG(briefDescription() << " does not belong to the same model as the curve you want to set.");
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
        LOG(briefDescription() << " does not belong to the same model as the curve you want to set.");
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
        LOG(briefDescription() << " does not belong to the same model as the curve you want to set.");
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
        LOG(briefDescription() << " does not belong to the same model as the curve you want to set.");
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
  /*
  boost::optional<Curve> GeneratorMicroTurbine_Impl::optionalElectricalPowerFunctionofTemperatureandElevationCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Generator_MicroTurbineFields::ElectricalPowerFunctionofTemperatureandElevationCurveName);
  }

  boost::optional<Curve> GeneratorMicroTurbine_Impl::optionalElectricalEfficiencyFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Generator_MicroTurbineFields::ElectricalEfficiencyFunctionofTemperatureCurveName);
  }

  boost::optional<Curve> GeneratorMicroTurbine_Impl::optionalElectricalEfficiencyFunctionofPartLoadRatioCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Generator_MicroTurbineFields::ElectricalEfficiencyFunctionofPartLoadRatioCurveName);
  }
  */

} // detail

GeneratorMicroTurbine::GeneratorMicroTurbine(const Model& model)
  : Generator(GeneratorMicroTurbine::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::GeneratorMicroTurbine_Impl>());

  // TODO set a value to all required fields with no default...
  // Source: Generators.idf, Capstone C65
  //setName("Generator MicroTurbine Capstone C65");

  // Reference Electrical Power Output
  setReferenceElectricalPowerOutput(65000);

  // Reference Electrical Efficiency Using Lower Heating Value
  setReferenceElectricalEfficiencyUsingLowerHeatingValue(0.29);

  //Electrical Power Function of Temperature and Elevation Curve Name
  CurveCubic electricalPowerFunctionofTemperatureandElevationCurve(model);
	  electricalPowerFunctionofTemperatureandElevationCurve.setName(name().get() + " Capstone C65 Power_vs_Temp_Elev")
	  electricalPowerFunctionofTemperatureandElevationCurve.setCoefficient1Constant(1.2027697);
	  electricalPowerFunctionofTemperatureandElevationCurve.setCoefficient2x(-9.671305E-03);
	  electricalPowerFunctionofTemperatureandElevationCurve.setCoefficient3xPOW2(-4.860793E-06);
	  electricalPowerFunctionofTemperatureandElevationCurve.setCoefficient4y(-1.542394E-04);
	  electricalPowerFunctionofTemperatureandElevationCurve.setCoefficient5yPOW(9.111418E-09);
	  electricalPowerFunctionofTemperatureandElevationCurve.setCoefficient6xTIMESY(8.797885E-07);
	  electricalPowerFunctionofTemperatureandElevationCurve.setMinimumValueofx(-17.8);
	  electricalPowerFunctionofTemperatureandElevationCurve.setMaximumValueofx(50);
	  electricalPowerFunctionofTemperatureandElevationCurve.setMinimumValueofy(0);
      electricalPowerFunctionofTemperatureandElevationCurve.setMaximumValueofy(3050.);
	  //	Temperature, !- Input Unit Type for X
	  // Distance, !- Input Unit Type for Y
	  // Dimensionless;           !- Output Unit Type
	setElectricalPowerFunctionofTemperatureandElevationCurve(electricalPowerFunctionofTemperatureandElevationCurve);

	// ElectricalEfficiencyFunctionofTemperatureCurveName
	// \object - list QuadraticCubicCurves
	/*! Electrical Efficiency Modifier Curve (function of temperature)
! x = Dry-Bulb Temperature of Combustion Inlet Air (C)

  Curve:Cubic,
    Capstone C65 Efficiency_vs_Temp,  !- Name
    1.0402217,               !- Coefficient1 Constant
    -0.0017314,              !- Coefficient2 x
    -6.497040E-05,           !- Coefficient3 x**2
    5.133175E-07,            !- Coefficient4 x**3
    -20.0,                   !- Minimum Value of x
    50.0,                    !- Maximum Value of x
    ,                        !- Minimum Curve Output
    ,                        !- Maximum Curve Output
    Temperature,             !- Input Unit Type for X
    Dimensionless;           !- Output Unit Type*/
  CurveCubic electricalEfficiencyFunctionofTemperatureCurve(model);
	electricalEfficiencyFunctionofTemperatureCurve.setName(name().get() + " Capstone C65 Efficiency_vs_Temp")
	electricalEfficiencyFunctionofTemperatureCurve.setCoefficient1Constant(1.0402217);
	electricalEfficiencyFunctionofTemperatureCurve.setCoefficient2x(-0.0017314);
	electricalEfficiencyFunctionofTemperatureCurve.setCoefficient3xPOW2(-6.497040E-05);
	electricalEfficiencyFunctionofTemperatureCurve.setCoefficient4xPOW3(5.133175E-07);
	electricalEfficiencyFunctionofTemperatureCurve.setMinimumValueofx(-20);
	electricalEfficiencyFunctionofTemperatureCurve.setMaximumValueofx(50);
	setElectricalEfficiencyFunctionofTemperatureCurve(electricalEfficiencyFunctionofTemperatureCurve);

  // ElectricalEfficiencyFunctionofPartLoadRatioCurveName
  // QuadraticCubicCurves
	/*  Curve:Cubic,
    Capstone C65 Efficiency_vs_PLR,  !- Name
    ,                !- Coefficient1 Constant
    ,                !- Coefficient2 x
    ,                !- Coefficient3 x**2
    ,                !- Coefficient4 x**3
    0.03,                    !- Minimum Value of x
    1.0;                     !- Maximum Value of x*/
  CurveCubic electricalEfficiencyFunctionofPartLoadRatioCurve(model);
	electricalEfficiencyFunctionofPartLoadRatioCurve.setName(name().get() + " Capstone C65 Efficiency_vs_PLR")
	electricalEfficiencyFunctionofPartLoadRatioCurve.setCoefficient1Constant(0.215290);
	electricalEfficiencyFunctionofPartLoadRatioCurve.setCoefficient2x(2.561463);
	electricalEfficiencyFunctionofPartLoadRatioCurve.setCoefficient3xPOW2(-3.24613);
	electricalEfficiencyFunctionofPartLoadRatioCurve.setCoefficient4xPOW3(1.497306);
	electricalEfficiencyFunctionofPartLoadRatioCurve.setMinimumValueofx(0.03);
	electricalEfficiencyFunctionofPartLoadRatioCurve.setMaximumValueofx(1.0);
	setElectricalEfficiencyFunctionofTemperatureCurve(electricalEfficiencyFunctionofPartLoadRatioCurve);
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

double GeneratorMicroTurbine::maximumFullLoadElectricalPowerOutput() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->maximumFullLoadElectricalPowerOutput();
}

bool GeneratorMicroTurbine::isMaximumFullLoadElectricalPowerOutputDefaulted() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->isMaximumFullLoadElectricalPowerOutputDefaulted();
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

boost::optional<Curve> GeneratorMicroTurbine::generatorMicroTurbineHeatRecovery() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->generatorMicroTurbineHeatRecovery();
}

/*
boost::optional<Connection> GeneratorMicroTurbine::combustionAirInletNode() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->combustionAirInletNode();
}

boost::optional<Connection> GeneratorMicroTurbine::combustionAirOutletNode() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->combustionAirOutletNode();
} */

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

void GeneratorMicroTurbine::setGeneratorMicroTurbineHeatRecovery() {
  getImpl<detail::GeneratorMicroTurbine_Impl>()->setGeneratorMicroTurbineHeatRecovery();
}

void GeneratorMicroTurbine::resetGeneratorMicroTurbineHeatRecovery() {
  getImpl<detail::GeneratorMicroTurbine_Impl>()->resetGeneratorMicroTurbineHeatRecovery();
}

/*
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
} */

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

  /*
  ModelObject GeneratorMicroTurbine_Impl::clone(Model model) const
  {
    GeneratorMicroTurbine newMCHP = Generator_Impl::clone(model).cast<GeneratorMicroTurbine>();

    return newMCHP;
  }
  */




/// @cond
GeneratorMicroTurbine::GeneratorMicroTurbine(std::shared_ptr<detail::GeneratorMicroTurbine_Impl> impl)
  : Generator(impl)
{}
/// @endcond

} // model
} // openstudio

