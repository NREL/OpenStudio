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

#include "GeneratorMicroTurbine.hpp"
#include "GeneratorMicroTurbine_Impl.hpp"
#include "GeneratorMicroTurbineHeatRecovery.hpp"
#include "GeneratorMicroTurbineHeatRecovery_Impl.hpp"

// Need model to check if curve is part of model when setting
#include "Model.hpp"
#include "Model_Impl.hpp"

#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "CurveBiquadratic.hpp"
#include "CurveBiquadratic_Impl.hpp"
#include "CurveCubic.hpp"
#include "CurveCubic_Impl.hpp"
#include "CurveQuadratic.hpp"
#include "CurveQuadratic_Impl.hpp"

#include "StraightComponent.hpp"
#include "StraightComponent_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "../../model/ScheduleTypeLimits.hpp"
#include "../../model/ScheduleTypeRegistry.hpp"

// TODO: Add DataTables if added later to OS?
//#include "DataTables.hpp" // UniVariateTables and BiVariateTables
//#include "DataTables.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Generator_MicroTurbine_FieldEnums.hxx>
#include <utilities/idd/OS_Generator_MicroTurbine_HeatRecovery_FieldEnums.hxx>

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

  // Lists the output variables of a Generator:MicroTurbine, excepts the Heat Recovery specific ones
  // (see GeneratorMicroTurbineHeatRecovery::outputVariableNames for these)
  const std::vector<std::string>& GeneratorMicroTurbine_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
      result.push_back("Generator Produced Electric Power");
      result.push_back("Generator Produced Electric Energy");
      result.push_back("Generator LHV Basis Electric Efficiency");
      // <FuelType>
      result.push_back("Generator Gas HHV Basis Rate");
      result.push_back("Generator Gas HHV Basis Energy");
      result.push_back("Generator Gas Mass Flow Rate");

      result.push_back("Generator Propane HHV Basis Rate");
      result.push_back("Generator Propane HHV Basis Energy");
      result.push_back("Generator Propane Mass Flow Rate");
      // </FuelType>
      result.push_back("Generator Fuel HHV Basis Rate");
      result.push_back("Generator Fuel HHV Basis Energy");

      // These are part of Generator:MicroTurbine:HeatRecovery
      // result.push_back("Generator Produced Thermal Rate");
      // result.push_back("Generator Produced Thermal Energy");
      // result.push_back("Generator Thermal Efficiency LHV Basis");
      // result.push_back("Generator Heat Recovery Inlet Temperature");
      // result.push_back("Generator Heat Recovery Outlet Temperature");
      // result.push_back("Generator Heat Recovery Water Mass Flow Rate");
      result.push_back("Generator Standby Electric Power");
      result.push_back("Generator Standby Electric Energy");
      result.push_back("Generator Ancillary Electric Power");
      result.push_back("Generator Ancillary Electric Energy");
      result.push_back("Generator Exhaust Air Mass Flow Rate");
      result.push_back("Generator Exhaust Air Temperature");
    }
    return result;
  }

  IddObjectType GeneratorMicroTurbine_Impl::iddObjectType() const {
    return GeneratorMicroTurbine::iddObjectType();
  }

  std::string GeneratorMicroTurbine_Impl::generatorObjectType() const
  {
    // translated to ElectricLoadCenter:Generators 'Generator Object Type'
    return "Generator:MicroTurbine";
  }

  // This will clone the GeneratorMicroTurbine as well as the GeneratorMicroTurbineHeatRecovery if there is one
  // and will return a reference to the GeneratorMicroTurbine
  ModelObject GeneratorMicroTurbine_Impl::clone(Model model) const
  {
    GeneratorMicroTurbine newCHP = ModelObject_Impl::clone(model).cast<GeneratorMicroTurbine>();



    // If there's a GeneratorMicroTurbineHeatRecovery, clone it as well
    if (boost::optional<GeneratorMicroTurbineHeatRecovery> mchpHR = generatorMicroTurbineHeatRecovery())
    {
      // Call the BaseClass (ModelObject) clone method to avoid circular references
      GeneratorMicroTurbineHeatRecovery mchpHRClone = mchpHR->getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->ModelObject_Impl::clone(model).cast<GeneratorMicroTurbineHeatRecovery>();
      newCHP.getImpl<detail::GeneratorMicroTurbine_Impl>()->setGeneratorMicroTurbineHeatRecovery(mchpHRClone);

    }

    return newCHP;
  }

  // Return allowable child types: curves and Generator:MicroTurbine
  std::vector<IddObjectType> GeneratorMicroTurbine_Impl::allowableChildTypes() const
  {
    std::vector<IddObjectType> result;
    result.push_back(IddObjectType::OS_Generator_MicroTurbine_HeatRecovery);
    result.push_back(IddObjectType::OS_Curve_Biquadratic);
    result.push_back(IddObjectType::OS_Curve_Cubic);
    result.push_back(IddObjectType::OS_Curve_Quadratic);
    return result;
  }

  // Returns the children object: max of 7 curves and a the GeneratorMicroTurbineHeatRecovery if it exists
  std::vector<ModelObject> GeneratorMicroTurbine_Impl::children() const
  {
    std::vector<ModelObject> result;
    boost::optional<Curve> oCurve;

    if(boost::optional<GeneratorMicroTurbineHeatRecovery> mo = generatorMicroTurbineHeatRecovery())
    {
      result.push_back(mo.get());
    }

    // Should I include curves in there? Even now that then can be shared (resources)
    Curve curve = electricalPowerFunctionofTemperatureandElevationCurve();
    result.push_back(curve);

    curve = electricalEfficiencyFunctionofTemperatureCurve();
    result.push_back(curve);

    curve = electricalEfficiencyFunctionofPartLoadRatioCurve();
    result.push_back(curve);

    if (oCurve = exhaustAirFlowRateFunctionofTemperatureCurve()) {
      result.push_back(oCurve.get());
    }
    if (oCurve = exhaustAirFlowRateFunctionofPartLoadRatioCurve()) {
      result.push_back(oCurve.get());
    }

    if (oCurve = exhaustAirTemperatureFunctionofTemperatureCurve()) {
      result.push_back(oCurve.get());
    }
    if (oCurve = exhaustAirTemperatureFunctionofPartLoadRatioCurve()) {
      result.push_back(oCurve.get());
    }



    return result;
  }

  std::vector<ScheduleTypeKey> GeneratorMicroTurbine_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    // TODO: Check schedule display names.
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Generator_MicroTurbineFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("GeneratorMicroTurbine","Availability"));
    }
    return result;
  }

  // translated to ElectricLoadCenter:Generators 'Generator Rated Electric Power Output'
  boost::optional<double> GeneratorMicroTurbine_Impl::ratedElectricPowerOutput() const
  {
    return referenceElectricalPowerOutput();
  }

  boost::optional<Schedule> GeneratorMicroTurbine_Impl::availabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Generator_MicroTurbineFields::AvailabilityScheduleName);
  }

  // Convenience method to go fetch the connected GeneratorMicroTurbineHeatRecovery's 'Rated Thermal to Electrical Power Ratio'
  boost::optional<double> GeneratorMicroTurbine_Impl::ratedThermaltoElectricalPowerRatio() const
  {
    // translated to ElectricLoadCenter:Generators 'Generator Rated Thermal to Electrical Power Ratio'
    //DLM: need to look into meaning of this field, is this heat recovery size divided by electrical power?
    // TODO: Use the GeneratorMicroTurbineHeatRecovery method
    boost::optional<GeneratorMicroTurbineHeatRecovery> mchpHR = this->generatorMicroTurbineHeatRecovery();
    boost::optional<double> value;

    if (mchpHR) {
        value = mchpHR->ratedThermaltoElectricalPowerRatio();
    }
    return value;
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
    boost::optional<Curve> curve = getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Generator_MicroTurbineFields::ElectricalPowerFunctionofTemperatureandElevationCurveName);
    OS_ASSERT(curve);
    return curve.get();
  }

  Curve GeneratorMicroTurbine_Impl::electricalEfficiencyFunctionofTemperatureCurve() const {
    boost::optional<Curve> curve = getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Generator_MicroTurbineFields::ElectricalEfficiencyFunctionofTemperatureCurveName);
    OS_ASSERT(curve);
    return curve.get();
  }

  Curve GeneratorMicroTurbine_Impl::electricalEfficiencyFunctionofPartLoadRatioCurve() const {
    boost::optional<Curve> curve =  getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Generator_MicroTurbineFields::ElectricalEfficiencyFunctionofPartLoadRatioCurveName);
    OS_ASSERT(curve);
    return curve.get();
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


  bool GeneratorMicroTurbine_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Generator_MicroTurbineFields::AvailabilityScheduleName,
                              "GeneratorMicroTurbine",
                              "Availability",
                              schedule);
    return result;
  }

  void GeneratorMicroTurbine_Impl::resetAvailabilitySchedule() {
    bool result = setString(OS_Generator_MicroTurbineFields::AvailabilityScheduleName, "");
    OS_ASSERT(result);
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

  bool GeneratorMicroTurbine_Impl::setMaximumFullLoadElectricalPowerOutput(double maximumFullLoadElectricalPowerOutput) {
    bool result = setDouble(OS_Generator_MicroTurbineFields::MaximumFullLoadElectricalPowerOutput, maximumFullLoadElectricalPowerOutput);
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
      LOG(Warn,briefDescription() << " does not belong to the same model as the curve you want to set.");
      return false;
    }
  // Todo: do I need to explicitly check if the curve is of the right type? or does defining \object-list BiquadraticCurves in .idd suffice?
    bool result = setPointer(OS_Generator_MicroTurbineFields::ElectricalPowerFunctionofTemperatureandElevationCurveName, curve.handle());
    return result;
  }

  bool GeneratorMicroTurbine_Impl::setElectricalEfficiencyFunctionofTemperatureCurve(const Curve& curve) {
    if(model() != curve.model())
    {
      LOG(Warn,briefDescription() << " does not belong to the same model as the curve you want to set.");
      return false;
    }
    bool result = setPointer(OS_Generator_MicroTurbineFields::ElectricalEfficiencyFunctionofTemperatureCurveName, curve.handle());
    return result;
  }

  bool GeneratorMicroTurbine_Impl::setElectricalEfficiencyFunctionofPartLoadRatioCurve(const Curve& curve) {
    if(model() != curve.model())
    {
      LOG(Warn,briefDescription() << " does not belong to the same model as the curve you want to set.");
      return false;
    }
    bool result = setPointer(OS_Generator_MicroTurbineFields::ElectricalEfficiencyFunctionofPartLoadRatioCurveName, curve.handle());
    return result;
  }

  bool GeneratorMicroTurbine_Impl::setFuelType(const std::string& fuelType) {
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

  bool GeneratorMicroTurbine_Impl::setAncillaryPowerFunctionofFuelInputCurve(const Curve& curve) {
    bool result = setPointer(OS_Generator_MicroTurbineFields::AncillaryPowerFunctionofFuelInputCurveName, curve.handle());
    return result;
  }

  void GeneratorMicroTurbine_Impl::resetAncillaryPowerFunctionofFuelInputCurve() {
    bool result = setString(OS_Generator_MicroTurbineFields::AncillaryPowerFunctionofFuelInputCurveName, "");
    OS_ASSERT(result);
  }


  // Private: Generator:MicroTurbine:HeatRecovery
  bool GeneratorMicroTurbine_Impl::setGeneratorMicroTurbineHeatRecovery(const GeneratorMicroTurbineHeatRecovery& generatorMicroTurbineHeatRecovery) {
    bool result = setPointer(OS_Generator_MicroTurbineFields::GeneratorMicroTurbineHeatRecoveryName, generatorMicroTurbineHeatRecovery.handle());
    return result;
  }

  /*
  void GeneratorMicroTurbine_Impl::resetGeneratorMicroTurbineHeatRecovery() {
    bool result = setString(OS_Generator_MicroTurbineFields::GeneratorMicroTurbineHeatRecoveryName, "");
    OS_ASSERT(result);
  }*/

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

  bool GeneratorMicroTurbine_Impl::setReferenceExhaustAirMassFlowRate(double referenceExhaustAirMassFlowRate) {
    bool result = setDouble(OS_Generator_MicroTurbineFields::ReferenceExhaustAirMassFlowRate, referenceExhaustAirMassFlowRate);
    return result;
  }

  void GeneratorMicroTurbine_Impl::resetReferenceExhaustAirMassFlowRate() {
    bool result = setString(OS_Generator_MicroTurbineFields::ReferenceExhaustAirMassFlowRate, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbine_Impl::setExhaustAirFlowRateFunctionofTemperatureCurve(const Curve& curve) {
    bool result = setPointer(OS_Generator_MicroTurbineFields::ExhaustAirFlowRateFunctionofTemperatureCurveName, curve.handle());
    return result;
  }

  void GeneratorMicroTurbine_Impl::resetExhaustAirFlowRateFunctionofTemperatureCurve() {
    bool result = setString(OS_Generator_MicroTurbineFields::ExhaustAirFlowRateFunctionofTemperatureCurveName, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbine_Impl::setExhaustAirFlowRateFunctionofPartLoadRatioCurve(const Curve& curve) {
    bool result = setPointer(OS_Generator_MicroTurbineFields::ExhaustAirFlowRateFunctionofPartLoadRatioCurveName, curve.handle());
    return result;
  }

  void GeneratorMicroTurbine_Impl::resetExhaustAirFlowRateFunctionofPartLoadRatioCurve() {
    bool result = setString(OS_Generator_MicroTurbineFields::ExhaustAirFlowRateFunctionofPartLoadRatioCurveName, "");
    OS_ASSERT(result);
  }

  void GeneratorMicroTurbine_Impl::setNominalExhaustAirOutletTemperature(double nominalExhaustAirOutletTemperature) {
    bool result = setDouble(OS_Generator_MicroTurbineFields::NominalExhaustAirOutletTemperature, nominalExhaustAirOutletTemperature);
    OS_ASSERT(result);
  }

  void GeneratorMicroTurbine_Impl::resetNominalExhaustAirOutletTemperature() {
    bool result = setString(OS_Generator_MicroTurbineFields::NominalExhaustAirOutletTemperature, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbine_Impl::setExhaustAirTemperatureFunctionofTemperatureCurve(const Curve& curve) {
    bool result = setPointer(OS_Generator_MicroTurbineFields::ExhaustAirTemperatureFunctionofTemperatureCurveName, curve.handle());
    return result;
  }

  void GeneratorMicroTurbine_Impl::resetExhaustAirTemperatureFunctionofTemperatureCurve() {
    bool result = setString(OS_Generator_MicroTurbineFields::ExhaustAirTemperatureFunctionofTemperatureCurveName, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbine_Impl::setExhaustAirTemperatureFunctionofPartLoadRatioCurve(const Curve& curve) {
    bool result = setPointer(OS_Generator_MicroTurbineFields::ExhaustAirTemperatureFunctionofPartLoadRatioCurveName, curve.handle());
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
  // CurveBiquadratic
  CurveBiquadratic elecPowerFTempElevation(model);
  elecPowerFTempElevation.setName(name().get() + " Capstone C65 Power_vs_Temp_Elev");
  elecPowerFTempElevation.setCoefficient1Constant(1.2027697);
  elecPowerFTempElevation.setCoefficient2x(-9.671305E-03);
  elecPowerFTempElevation.setCoefficient3xPOW2(-4.860793E-06);
  elecPowerFTempElevation.setCoefficient4y(-1.542394E-04);
  elecPowerFTempElevation.setCoefficient5yPOW2(9.111418E-09);
  elecPowerFTempElevation.setCoefficient6xTIMESY(8.797885E-07);
  elecPowerFTempElevation.setMinimumValueofx(-17.8);
  elecPowerFTempElevation.setMaximumValueofx(50);
  elecPowerFTempElevation.setMinimumValueofy(0);
  elecPowerFTempElevation.setMaximumValueofy(3050.);
    //	Temperature, !- Input Unit Type for X
    // Distance, !- Input Unit Type for Y
    // Dimensionless;           !- Output Unit Type
  setElectricalPowerFunctionofTemperatureandElevationCurve(elecPowerFTempElevation);

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
  CurveCubic elecEffFT(model);
  elecEffFT.setName(name().get() + " Capstone C65 Efficiency_vs_Temp");
  elecEffFT.setCoefficient1Constant(1.0402217);
  elecEffFT.setCoefficient2x(-0.0017314);
  elecEffFT.setCoefficient3xPOW2(-6.497040E-05);
  elecEffFT.setCoefficient4xPOW3(5.133175E-07);
  elecEffFT.setMinimumValueofx(-20);
  elecEffFT.setMaximumValueofx(50);
  setElectricalEfficiencyFunctionofTemperatureCurve(elecEffFT);

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
  CurveCubic elecEffFPLR(model);
  elecEffFPLR.setName(name().get() + " Capstone C65 Efficiency_vs_PLR");
  elecEffFPLR.setCoefficient1Constant(0.215290);
  elecEffFPLR.setCoefficient2x(2.561463);
  elecEffFPLR.setCoefficient3xPOW2(-3.24613);
  elecEffFPLR.setCoefficient4xPOW3(1.497306);
  elecEffFPLR.setMinimumValueofx(0.03);
  elecEffFPLR.setMaximumValueofx(1.0);
  setElectricalEfficiencyFunctionofPartLoadRatioCurve(elecEffFPLR);
}

IddObjectType GeneratorMicroTurbine::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Generator_MicroTurbine);
}

std::vector<std::string> GeneratorMicroTurbine::validFuelTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Generator_MicroTurbineFields::FuelType);
}

double GeneratorMicroTurbine::referenceElectricalPowerOutput() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->referenceElectricalPowerOutput();
}

boost::optional<double> GeneratorMicroTurbine::ratedThermaltoElectricalPowerRatio() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->ratedThermaltoElectricalPowerRatio();
}

boost::optional<Schedule> GeneratorMicroTurbine::availabilitySchedule() const {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->availabilitySchedule();
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

boost::optional<GeneratorMicroTurbineHeatRecovery> GeneratorMicroTurbine::generatorMicroTurbineHeatRecovery() const {
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

bool GeneratorMicroTurbine::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->setAvailabilitySchedule(schedule);
}

void GeneratorMicroTurbine::resetAvailabilitySchedule() {
  getImpl<detail::GeneratorMicroTurbine_Impl>()->resetAvailabilitySchedule();
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

bool GeneratorMicroTurbine::setFuelType(const std::string& fuelType) {
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

// These isn't made "public" to bindings
/*bool GeneratorMicroTurbine::setGeneratorMicroTurbineHeatRecovery(const GeneratorMicroTurbineHeatRecovery& heatRecovery) {
  return getImpl<detail::GeneratorMicroTurbine_Impl>()->setGeneratorMicroTurbineHeatRecovery(heatRecovery);
}*/

/* This isn't implemented
void GeneratorMicroTurbine::resetGeneratorMicroTurbineHeatRecovery() {
  getImpl<detail::GeneratorMicroTurbine_Impl>()->resetGeneratorMicroTurbineHeatRecovery();
}*/

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


/// @cond
GeneratorMicroTurbine::GeneratorMicroTurbine(std::shared_ptr<detail::GeneratorMicroTurbine_Impl> impl)
  : Generator(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

