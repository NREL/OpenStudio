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

#include "Model.hpp"
#include "Model_Impl.hpp"

#include "GeneratorMicroTurbine.hpp"
#include "GeneratorMicroTurbine_Impl.hpp"
#include "GeneratorMicroTurbineHeatRecovery.hpp"
#include "GeneratorMicroTurbineHeatRecovery_Impl.hpp"

#include "Node.hpp"
#include "Node_Impl.hpp"
#include "PlantLoop.hpp"
#include "PlantLoop_Impl.hpp"

#include "Curve.hpp"
#include "Curve_Impl.hpp"
// TODO: add the tables class if they get added to OS later?
//#include "DataTables.hpp"


#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Generator_MicroTurbine_HeatRecovery_FieldEnums.hxx>


// TODO not sure if need all of these
#include "../utilities/core/Compare.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"


namespace openstudio {
namespace model {

  namespace detail {

  GeneratorMicroTurbineHeatRecovery_Impl::GeneratorMicroTurbineHeatRecovery_Impl(const IdfObject& idfObject,
    Model_Impl* model,
    bool keepHandle)
    : StraightComponent_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == GeneratorMicroTurbineHeatRecovery::iddObjectType());
  }

  GeneratorMicroTurbineHeatRecovery_Impl::GeneratorMicroTurbineHeatRecovery_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
    Model_Impl* model,
    bool keepHandle)
    : StraightComponent_Impl(other, model, keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == GeneratorMicroTurbineHeatRecovery::iddObjectType());
  }

  GeneratorMicroTurbineHeatRecovery_Impl::GeneratorMicroTurbineHeatRecovery_Impl(const GeneratorMicroTurbineHeatRecovery_Impl& other,
    Model_Impl* model,
    bool keepHandle)
    : StraightComponent_Impl(other, model, keepHandle)
  {}

  // Lists the output variables that are specific to the heat recovery portion of a Generator:MicroTurbine
  const std::vector<std::string>& GeneratorMicroTurbineHeatRecovery_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
      result.push_back("Generator Produced Thermal Rate");
      result.push_back("Generator Produced Thermal Energy");
      result.push_back("Generator Thermal Efficiency LHV Basis");
      result.push_back("Generator Heat Recovery Inlet Temperature");
      result.push_back("Generator Heat Recovery Outlet Temperature");
      result.push_back("Generator Heat Recovery Water Mass Flow Rate");
    }
    return result;
  }

  IddObjectType GeneratorMicroTurbineHeatRecovery_Impl::iddObjectType() const {
    return GeneratorMicroTurbineHeatRecovery::iddObjectType();
  }

  // This will clone both the GeneratorMicroTurbineHeatRecovery and its linked GeneratorMicroTurbineHeatRecovery
  // and will return a reference to the GeneratorMicroTurbineHeatRecovery
  ModelObject GeneratorMicroTurbineHeatRecovery_Impl::clone(Model model) const
  {

    // We call the parent generator's Clone method which will clone both the mchp and mchpHR
    GeneratorMicroTurbine mchp = generatorMicroTurbine();
    GeneratorMicroTurbine mchpClone = mchp.clone(model).cast<GeneratorMicroTurbine>();

    // We get the clone of the parent generator's MTHR so we can return that
    GeneratorMicroTurbineHeatRecovery mchpHRClone = mchpClone.generatorMicroTurbineHeatRecovery().get();


    return mchpHRClone;
  }

  std::vector<IddObjectType> GeneratorMicroTurbineHeatRecovery_Impl::allowableChildTypes() const
  {
    std::vector<IddObjectType> result;
    result.push_back(IddObjectType::OS_Curve_Bicubic);
    result.push_back(IddObjectType::OS_Curve_Biquadratic);
    result.push_back(IddObjectType::OS_Curve_Cubic);
    result.push_back(IddObjectType::OS_Curve_Quadratic);
    return result;
  }

  // Returns the children objects (max of 4 curves)
  std::vector<ModelObject> GeneratorMicroTurbineHeatRecovery_Impl::children() const
  {
    std::vector<ModelObject> result;
    boost::optional<Curve> oCurve;

    // Curves should be included
    if (oCurve = thermalEfficiencyFunctionofTemperatureandElevationCurve()) {
      result.push_back(oCurve.get());
    }
    if (oCurve = heatRecoveryRateFunctionofPartLoadRatioCurve()) {
      result.push_back(oCurve.get());
    }
    if (oCurve = heatRecoveryRateFunctionofInletWaterTemperatureCurve()) {
      result.push_back(oCurve.get());
    }

    if (oCurve = heatRecoveryRateFunctionofWaterFlowRateCurve()) {
      result.push_back(oCurve.get());
    }

    return result;
  }

  unsigned GeneratorMicroTurbineHeatRecovery_Impl::inletPort()
  {
    return OS_Generator_MicroTurbine_HeatRecoveryFields::HeatRecoveryWaterInletNodeName;
  }

  unsigned GeneratorMicroTurbineHeatRecovery_Impl::outletPort()
  {
    return OS_Generator_MicroTurbine_HeatRecoveryFields::HeatRecoveryWaterOutletNodeName;
  }

  // Add to plantLoop Node: accepts to be placed on both the supply and the demand
  // You should make sure that this is compatible with your Generator Operation Scheme
  // For example, if FollowThermal, it must be placed on the supply side of the plant loop
  bool  GeneratorMicroTurbineHeatRecovery_Impl::addToNode(Node & node)
  {
    // This can be placed on the supply or the demand side
    if( boost::optional<PlantLoop> plant = node.plantLoop() )
    {
      return StraightComponent_Impl::addToNode(node);
    }

    return false;
  }


  //boost::optional<Connection> GeneratorMicroTurbineHeatRecovery_Impl::heatRecoveryWaterInletNode() const {
  //  return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_Generator_MicroTurbine_HeatRecoveryFields::HeatRecoveryWaterInletNodeName);
  //}

  //boost::optional<Connection> GeneratorMicroTurbineHeatRecovery_Impl::heatRecoveryWaterOutletNode() const {
  //  return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_Generator_MicroTurbineHeatRecoveryFields::HeatRecoveryWaterOutletNodeName);
  //}

  double GeneratorMicroTurbineHeatRecovery_Impl::referenceThermalEfficiencyUsingLowerHeatValue() const {
    boost::optional<double> value = getDouble(OS_Generator_MicroTurbine_HeatRecoveryFields::ReferenceThermalEfficiencyUsingLowerHeatValue,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool GeneratorMicroTurbineHeatRecovery_Impl::isReferenceThermalEfficiencyUsingLowerHeatValueDefaulted() const {
    return isEmpty(OS_Generator_MicroTurbine_HeatRecoveryFields::ReferenceThermalEfficiencyUsingLowerHeatValue);
  }

  double GeneratorMicroTurbineHeatRecovery_Impl::referenceInletWaterTemperature() const {
    boost::optional<double> value = getDouble(OS_Generator_MicroTurbine_HeatRecoveryFields::ReferenceInletWaterTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string GeneratorMicroTurbineHeatRecovery_Impl::heatRecoveryWaterFlowOperatingMode() const {
    boost::optional<std::string> value = getString(OS_Generator_MicroTurbine_HeatRecoveryFields::HeatRecoveryWaterFlowOperatingMode,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool GeneratorMicroTurbineHeatRecovery_Impl::isHeatRecoveryWaterFlowOperatingModeDefaulted() const {
    return isEmpty(OS_Generator_MicroTurbine_HeatRecoveryFields::HeatRecoveryWaterFlowOperatingMode);
  }

  double GeneratorMicroTurbineHeatRecovery_Impl::referenceHeatRecoveryWaterFlowRate() const {
    boost::optional<double> value = getDouble(OS_Generator_MicroTurbine_HeatRecoveryFields::ReferenceHeatRecoveryWaterFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Curve> GeneratorMicroTurbineHeatRecovery_Impl::heatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Generator_MicroTurbine_HeatRecoveryFields::HeatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurveName);
  }

  boost::optional<Curve> GeneratorMicroTurbineHeatRecovery_Impl::thermalEfficiencyFunctionofTemperatureandElevationCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Generator_MicroTurbine_HeatRecoveryFields::ThermalEfficiencyFunctionofTemperatureandElevationCurveName);
  }

  boost::optional<Curve> GeneratorMicroTurbineHeatRecovery_Impl::heatRecoveryRateFunctionofPartLoadRatioCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Generator_MicroTurbine_HeatRecoveryFields::HeatRecoveryRateFunctionofPartLoadRatioCurveName);
  }

  boost::optional<Curve> GeneratorMicroTurbineHeatRecovery_Impl::heatRecoveryRateFunctionofInletWaterTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Generator_MicroTurbine_HeatRecoveryFields::HeatRecoveryRateFunctionofInletWaterTemperatureCurveName);
  }

  boost::optional<Curve> GeneratorMicroTurbineHeatRecovery_Impl::heatRecoveryRateFunctionofWaterFlowRateCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Generator_MicroTurbine_HeatRecoveryFields::HeatRecoveryRateFunctionofWaterFlowRateCurveName);
  }

  double GeneratorMicroTurbineHeatRecovery_Impl::minimumHeatRecoveryWaterFlowRate() const {
    boost::optional<double> value = getDouble(OS_Generator_MicroTurbine_HeatRecoveryFields::MinimumHeatRecoveryWaterFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool GeneratorMicroTurbineHeatRecovery_Impl::isMinimumHeatRecoveryWaterFlowRateDefaulted() const {
    return isEmpty(OS_Generator_MicroTurbine_HeatRecoveryFields::MinimumHeatRecoveryWaterFlowRate);
  }

  double GeneratorMicroTurbineHeatRecovery_Impl::maximumHeatRecoveryWaterFlowRate() const {
    boost::optional<double> value = getDouble(OS_Generator_MicroTurbine_HeatRecoveryFields::MaximumHeatRecoveryWaterFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool GeneratorMicroTurbineHeatRecovery_Impl::isMaximumHeatRecoveryWaterFlowRateDefaulted() const {
    return isEmpty(OS_Generator_MicroTurbine_HeatRecoveryFields::MaximumHeatRecoveryWaterFlowRate);
  }

  boost::optional<double> GeneratorMicroTurbineHeatRecovery_Impl::maximumHeatRecoveryWaterTemperature() const {
    return getDouble(OS_Generator_MicroTurbine_HeatRecoveryFields::MaximumHeatRecoveryWaterTemperature,true);
  }

  // Get the parent generatorMicroTurbine
  GeneratorMicroTurbine GeneratorMicroTurbineHeatRecovery_Impl::generatorMicroTurbine() const {

    boost::optional<GeneratorMicroTurbine> value;
    for ( const GeneratorMicroTurbine& mchp : this->model().getConcreteModelObjects<GeneratorMicroTurbine>() )
    {
      if ( boost::optional<GeneratorMicroTurbineHeatRecovery> mchpHR = mchp.generatorMicroTurbineHeatRecovery() )
      {
        if (mchpHR->handle() == this->handle())
        {
          value = mchp;
        }
      }
    }
    OS_ASSERT(value);
    return value.get();

  }





  // If defaulted, return generatorMicroTurbineHeatRecovery 'Reference Thermal Efficiency Using Lower Heat Value' divided by its generatorMicroTurbine 'Reference Electrical Efficiency Using Lower Heating Value'
  double GeneratorMicroTurbineHeatRecovery_Impl::ratedThermaltoElectricalPowerRatio() const {
    boost::optional<double> optRatedThermaltoElectricalPowerRatio = getDouble(OS_Generator_MicroTurbine_HeatRecoveryFields::RatedThermaltoElectricalPowerRatio, true);
    // If there it's set
    if (optRatedThermaltoElectricalPowerRatio) {
      // Get it and return
      return optRatedThermaltoElectricalPowerRatio.get();
    }
    else {
      // We try to default it
      // First, check if there's a linked generator
      // TODO: This should return an actual mchp once I make it part of the constructor.
      //boost::optional<GeneratorMicroTurbine> mchp = this->generatorMicroTurbine();
      //if ( mchp ) {
      GeneratorMicroTurbine mchp = this->generatorMicroTurbine();
      double refElecEffUsingLowerHeatingVal = mchp.referenceElectricalEfficiencyUsingLowerHeatingValue();
      double refThermalEffUsingLowerHeatVal = this->referenceThermalEfficiencyUsingLowerHeatValue();
      double ratedThermaltoElectricalPowerRatio = refThermalEffUsingLowerHeatVal / refElecEffUsingLowerHeatingVal;
      return ratedThermaltoElectricalPowerRatio;
      //}
    }
  }

  bool GeneratorMicroTurbineHeatRecovery_Impl::isRatedThermaltoElectricalPowerRatioDefaulted() const {
    return isEmpty(OS_Generator_MicroTurbine_HeatRecoveryFields::RatedThermaltoElectricalPowerRatio);
  }


  /*bool GeneratorMicroTurbineHeatRecovery_Impl::setHeatRecoveryWaterInletNode(const boost::optional<Connection>& connection) {
    bool result(false);
    if (connection) {
      result = setPointer(OS_Generator_MicroTurbineHeatRecoveryFields::HeatRecoveryWaterInletNodeName, connection.get().handle());
    }
    else {
      resetHeatRecoveryWaterInletNode();
      result = true;
    }
    return result;
  }

  void GeneratorMicroTurbineHeatRecovery_Impl::resetHeatRecoveryWaterInletNode() {
    bool result = setString(OS_Generator_MicroTurbineHeatRecoveryFields::HeatRecoveryWaterInletNodeName, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbineHeatRecovery_Impl::setHeatRecoveryWaterOutletNode(const boost::optional<Connection>& connection) {
    bool result(false);
    if (connection) {
      result = setPointer(OS_Generator_MicroTurbineHeatRecoveryFields::HeatRecoveryWaterOutletNodeName, connection.get().handle());
    }
    else {
      resetHeatRecoveryWaterOutletNode();
      result = true;
    }
    return result;
  }

  void GeneratorMicroTurbineHeatRecovery_Impl::resetHeatRecoveryWaterOutletNode() {
    bool result = setString(OS_Generator_MicroTurbineHeatRecoveryFields::HeatRecoveryWaterOutletNodeName, "");
    OS_ASSERT(result);
  } */

  bool GeneratorMicroTurbineHeatRecovery_Impl::setReferenceThermalEfficiencyUsingLowerHeatValue(double value) {
    bool result = setDouble(OS_Generator_MicroTurbine_HeatRecoveryFields::ReferenceThermalEfficiencyUsingLowerHeatValue, value);
    return result;
  }

  void GeneratorMicroTurbineHeatRecovery_Impl::resetReferenceThermalEfficiencyUsingLowerHeatValue() {
    bool result = setString(OS_Generator_MicroTurbine_HeatRecoveryFields::ReferenceThermalEfficiencyUsingLowerHeatValue, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbineHeatRecovery_Impl::setReferenceInletWaterTemperature(double value) {
    bool result = setDouble(OS_Generator_MicroTurbine_HeatRecoveryFields::ReferenceInletWaterTemperature, value);
    return result;
  }

  bool GeneratorMicroTurbineHeatRecovery_Impl::setHeatRecoveryWaterFlowOperatingMode(std::string heatRecoveryWaterFlowOperatingMode) {
    bool result = setString(OS_Generator_MicroTurbine_HeatRecoveryFields::HeatRecoveryWaterFlowOperatingMode, heatRecoveryWaterFlowOperatingMode);
    return result;
  }

  void GeneratorMicroTurbineHeatRecovery_Impl::resetHeatRecoveryWaterFlowOperatingMode() {
    bool result = setString(OS_Generator_MicroTurbine_HeatRecoveryFields::HeatRecoveryWaterFlowOperatingMode, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbineHeatRecovery_Impl::setReferenceHeatRecoveryWaterFlowRate(double value) {
    bool result = setDouble(OS_Generator_MicroTurbine_HeatRecoveryFields::ReferenceHeatRecoveryWaterFlowRate, value);
    return result;
  }

  bool GeneratorMicroTurbineHeatRecovery_Impl::setHeatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve(const Curve& curve) {
    bool result = setPointer(OS_Generator_MicroTurbine_HeatRecoveryFields::HeatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurveName, curve.handle());
    return result;
  }

  void GeneratorMicroTurbineHeatRecovery_Impl::resetHeatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve() {
    bool result = setString(OS_Generator_MicroTurbine_HeatRecoveryFields::HeatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurveName, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbineHeatRecovery_Impl::setThermalEfficiencyFunctionofTemperatureandElevationCurve(const Curve& curve) {
    bool result = setPointer(OS_Generator_MicroTurbine_HeatRecoveryFields::ThermalEfficiencyFunctionofTemperatureandElevationCurveName, curve.handle());
    return result;
  }

  void GeneratorMicroTurbineHeatRecovery_Impl::resetThermalEfficiencyFunctionofTemperatureandElevationCurve() {
    bool result = setString(OS_Generator_MicroTurbine_HeatRecoveryFields::ThermalEfficiencyFunctionofTemperatureandElevationCurveName, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbineHeatRecovery_Impl::setHeatRecoveryRateFunctionofPartLoadRatioCurve(const Curve& curve) {
    bool result = setPointer(OS_Generator_MicroTurbine_HeatRecoveryFields::HeatRecoveryRateFunctionofPartLoadRatioCurveName, curve.handle());
    return result;
  }

  void GeneratorMicroTurbineHeatRecovery_Impl::resetHeatRecoveryRateFunctionofPartLoadRatioCurve() {
    bool result = setString(OS_Generator_MicroTurbine_HeatRecoveryFields::HeatRecoveryRateFunctionofPartLoadRatioCurveName, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbineHeatRecovery_Impl::setHeatRecoveryRateFunctionofInletWaterTemperatureCurve(const Curve& curve) {
    bool result = setPointer(OS_Generator_MicroTurbine_HeatRecoveryFields::HeatRecoveryRateFunctionofInletWaterTemperatureCurveName, curve.handle());
    return result;
  }

  void GeneratorMicroTurbineHeatRecovery_Impl::resetHeatRecoveryRateFunctionofInletWaterTemperatureCurve() {
    bool result = setString(OS_Generator_MicroTurbine_HeatRecoveryFields::HeatRecoveryRateFunctionofInletWaterTemperatureCurveName, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbineHeatRecovery_Impl::setHeatRecoveryRateFunctionofWaterFlowRateCurve(const Curve& curve) {
    bool result = setPointer(OS_Generator_MicroTurbine_HeatRecoveryFields::HeatRecoveryRateFunctionofWaterFlowRateCurveName, curve.handle());
    return result;
  }

  void GeneratorMicroTurbineHeatRecovery_Impl::resetHeatRecoveryRateFunctionofWaterFlowRateCurve() {
    bool result = setString(OS_Generator_MicroTurbine_HeatRecoveryFields::HeatRecoveryRateFunctionofWaterFlowRateCurveName, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbineHeatRecovery_Impl::setMinimumHeatRecoveryWaterFlowRate(double minimumHeatRecoveryWaterFlowRate) {
    bool result = setDouble(OS_Generator_MicroTurbine_HeatRecoveryFields::MinimumHeatRecoveryWaterFlowRate, minimumHeatRecoveryWaterFlowRate);
    return result;
  }

  void GeneratorMicroTurbineHeatRecovery_Impl::resetMinimumHeatRecoveryWaterFlowRate() {
    bool result = setString(OS_Generator_MicroTurbine_HeatRecoveryFields::MinimumHeatRecoveryWaterFlowRate, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbineHeatRecovery_Impl::setMaximumHeatRecoveryWaterFlowRate(double maximumHeatRecoveryWaterFlowRate) {
    bool result = setDouble(OS_Generator_MicroTurbine_HeatRecoveryFields::MaximumHeatRecoveryWaterFlowRate, maximumHeatRecoveryWaterFlowRate);
    return result;
  }

  void GeneratorMicroTurbineHeatRecovery_Impl::resetMaximumHeatRecoveryWaterFlowRate() {
    bool result = setString(OS_Generator_MicroTurbine_HeatRecoveryFields::MaximumHeatRecoveryWaterFlowRate, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbineHeatRecovery_Impl::setMaximumHeatRecoveryWaterTemperature(double maximumHeatRecoveryWaterTemperature) {
    bool result = setDouble(OS_Generator_MicroTurbine_HeatRecoveryFields::MaximumHeatRecoveryWaterTemperature, maximumHeatRecoveryWaterTemperature);
    return result;
  }

  void GeneratorMicroTurbineHeatRecovery_Impl::resetMaximumHeatRecoveryWaterTemperature() {
    bool result = setString(OS_Generator_MicroTurbine_HeatRecoveryFields::MaximumHeatRecoveryWaterTemperature, "");
    OS_ASSERT(result);
  }

  bool GeneratorMicroTurbineHeatRecovery_Impl::setRatedThermaltoElectricalPowerRatio(double ratedThermaltoElectricalPowerRatio) {
    bool result = setDouble(OS_Generator_MicroTurbine_HeatRecoveryFields::RatedThermaltoElectricalPowerRatio, ratedThermaltoElectricalPowerRatio);
    return result;
  }

  void GeneratorMicroTurbineHeatRecovery_Impl::resetRatedThermaltoElectricalPowerRatio() {
    bool result = setString(OS_Generator_MicroTurbine_HeatRecoveryFields::RatedThermaltoElectricalPowerRatio, "");
    OS_ASSERT(result);
  }


} // detail

// The constructor needs model, and a GeneratorMicroTurbine
GeneratorMicroTurbineHeatRecovery::GeneratorMicroTurbineHeatRecovery( const Model& model,
                                                                      GeneratorMicroTurbine & mchp )
  : StraightComponent(GeneratorMicroTurbineHeatRecovery::iddObjectType(),model)
{

  // Set object in parent
  mchp.getImpl<detail::GeneratorMicroTurbine_Impl>()->setGeneratorMicroTurbineHeatRecovery((*this));
  //mchp.setGeneratorMicroTurbineHeatRecovery( (*this) );

  // Set the Name to mchp.name + " Heat Recovery"
  if (OptionalString parentName = mchp.name()) {
    setName( (*parentName) + " Heat Recovery");
  }

  // Reference Thermal Efficiency Using Lower Heat Value has a default of 0 in E+ idd which isn't smart in this case
  // TODO Should I set it here, or change the .idd??
  // 0.4975 would be better
  // Going with Kyle and Mark's way
  setReferenceThermalEfficiencyUsingLowerHeatValue(0.4975);

  // Assign all values that are required but have no default

  // Reference Inlet Water Temperature
  setReferenceInletWaterTemperature(60);

  // Reference Heat Recovery Water Flow Rate
  setReferenceHeatRecoveryWaterFlowRate(0.00252362);

  // Maximum Heat Recovery Water Flow Rate: has a default value of 0 in E+.idd which isn't right in this case
  // TODO: Should I set it here or change default in .idd?
  // 0.003785432
  // TODO: Or make it an optional and set it to the reference Heat recovery water flow rate times 1.5?



}

IddObjectType GeneratorMicroTurbineHeatRecovery::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Generator_MicroTurbine_HeatRecovery);
}

std::vector<std::string> GeneratorMicroTurbineHeatRecovery::validHeatRecoveryWaterFlowOperatingModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Generator_MicroTurbine_HeatRecoveryFields::HeatRecoveryWaterFlowOperatingMode);
}


/*
boost::optional<Connection> GeneratorMicroTurbineHeatRecovery::heatRecoveryWaterInletNode() const {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->heatRecoveryWaterInletNode();
}

boost::optional<Connection> GeneratorMicroTurbineHeatRecovery::heatRecoveryWaterOutletNode() const {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->heatRecoveryWaterOutletNode();
}
*/

double GeneratorMicroTurbineHeatRecovery::referenceThermalEfficiencyUsingLowerHeatValue() const {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->referenceThermalEfficiencyUsingLowerHeatValue();
}

bool GeneratorMicroTurbineHeatRecovery::isReferenceThermalEfficiencyUsingLowerHeatValueDefaulted() const {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->isReferenceThermalEfficiencyUsingLowerHeatValueDefaulted();
}

double GeneratorMicroTurbineHeatRecovery::referenceInletWaterTemperature() const {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->referenceInletWaterTemperature();
}

std::string GeneratorMicroTurbineHeatRecovery::heatRecoveryWaterFlowOperatingMode() const {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->heatRecoveryWaterFlowOperatingMode();
}

bool GeneratorMicroTurbineHeatRecovery::isHeatRecoveryWaterFlowOperatingModeDefaulted() const {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->isHeatRecoveryWaterFlowOperatingModeDefaulted();
}

double GeneratorMicroTurbineHeatRecovery::referenceHeatRecoveryWaterFlowRate() const {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->referenceHeatRecoveryWaterFlowRate();
}

boost::optional<Curve> GeneratorMicroTurbineHeatRecovery::heatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve() const {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->heatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve();
}

boost::optional<Curve> GeneratorMicroTurbineHeatRecovery::thermalEfficiencyFunctionofTemperatureandElevationCurve() const {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->thermalEfficiencyFunctionofTemperatureandElevationCurve();
}

boost::optional<Curve> GeneratorMicroTurbineHeatRecovery::heatRecoveryRateFunctionofPartLoadRatioCurve() const {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->heatRecoveryRateFunctionofPartLoadRatioCurve();
}

boost::optional<Curve> GeneratorMicroTurbineHeatRecovery::heatRecoveryRateFunctionofInletWaterTemperatureCurve() const {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->heatRecoveryRateFunctionofInletWaterTemperatureCurve();
}

boost::optional<Curve> GeneratorMicroTurbineHeatRecovery::heatRecoveryRateFunctionofWaterFlowRateCurve() const {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->heatRecoveryRateFunctionofWaterFlowRateCurve();
}

double GeneratorMicroTurbineHeatRecovery::minimumHeatRecoveryWaterFlowRate() const {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->minimumHeatRecoveryWaterFlowRate();
}

bool GeneratorMicroTurbineHeatRecovery::isMinimumHeatRecoveryWaterFlowRateDefaulted() const {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->isMinimumHeatRecoveryWaterFlowRateDefaulted();
}

double GeneratorMicroTurbineHeatRecovery::maximumHeatRecoveryWaterFlowRate() const {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->maximumHeatRecoveryWaterFlowRate();
}

bool GeneratorMicroTurbineHeatRecovery::isMaximumHeatRecoveryWaterFlowRateDefaulted() const {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->isMaximumHeatRecoveryWaterFlowRateDefaulted();
}

boost::optional<double> GeneratorMicroTurbineHeatRecovery::maximumHeatRecoveryWaterTemperature() const {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->maximumHeatRecoveryWaterTemperature();
}

double GeneratorMicroTurbineHeatRecovery::ratedThermaltoElectricalPowerRatio() const {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->ratedThermaltoElectricalPowerRatio();
}

bool GeneratorMicroTurbineHeatRecovery::isRatedThermaltoElectricalPowerRatioDefaulted() const {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->isRatedThermaltoElectricalPowerRatioDefaulted();
}

GeneratorMicroTurbine GeneratorMicroTurbineHeatRecovery::generatorMicroTurbine() const {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->generatorMicroTurbine();
}

/*
bool GeneratorMicroTurbineHeatRecovery::setHeatRecoveryWaterInletNode(const Connection& connection) {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->setHeatRecoveryWaterInletNode(connection);
}

void GeneratorMicroTurbineHeatRecovery::resetHeatRecoveryWaterInletNode() {
  getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->resetHeatRecoveryWaterInletNode();
}

bool GeneratorMicroTurbineHeatRecovery::setHeatRecoveryWaterOutletNode(const Connection& connection) {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->setHeatRecoveryWaterOutletNode(connection);
}

void GeneratorMicroTurbineHeatRecovery::resetHeatRecoveryWaterOutletNode() {
  getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->resetHeatRecoveryWaterOutletNode();
} */

bool GeneratorMicroTurbineHeatRecovery::setReferenceThermalEfficiencyUsingLowerHeatValue(double referenceThermalEfficiencyUsingLowerHeatValue) {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->setReferenceThermalEfficiencyUsingLowerHeatValue(referenceThermalEfficiencyUsingLowerHeatValue);
}

void GeneratorMicroTurbineHeatRecovery::resetReferenceThermalEfficiencyUsingLowerHeatValue() {
  getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->resetReferenceThermalEfficiencyUsingLowerHeatValue();
}

bool GeneratorMicroTurbineHeatRecovery::setReferenceInletWaterTemperature(double value) {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->setReferenceInletWaterTemperature(value);
}

bool GeneratorMicroTurbineHeatRecovery::setHeatRecoveryWaterFlowOperatingMode(std::string heatRecoveryWaterFlowOperatingMode) {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->setHeatRecoveryWaterFlowOperatingMode(heatRecoveryWaterFlowOperatingMode);
}

void GeneratorMicroTurbineHeatRecovery::resetHeatRecoveryWaterFlowOperatingMode() {
  getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->resetHeatRecoveryWaterFlowOperatingMode();
}

bool GeneratorMicroTurbineHeatRecovery::setReferenceHeatRecoveryWaterFlowRate(double value) {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->setReferenceHeatRecoveryWaterFlowRate(value);
}

bool GeneratorMicroTurbineHeatRecovery::setHeatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve(const Curve& curve) {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->setHeatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve(curve);
}

void GeneratorMicroTurbineHeatRecovery::resetHeatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve() {
  getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->resetHeatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve();
}

bool GeneratorMicroTurbineHeatRecovery::setThermalEfficiencyFunctionofTemperatureandElevationCurve(const Curve& curve) {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->setThermalEfficiencyFunctionofTemperatureandElevationCurve(curve);
}

void GeneratorMicroTurbineHeatRecovery::resetThermalEfficiencyFunctionofTemperatureandElevationCurve() {
  getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->resetThermalEfficiencyFunctionofTemperatureandElevationCurve();
}

bool GeneratorMicroTurbineHeatRecovery::setHeatRecoveryRateFunctionofPartLoadRatioCurve(const Curve& curve) {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->setHeatRecoveryRateFunctionofPartLoadRatioCurve(curve);
}

void GeneratorMicroTurbineHeatRecovery::resetHeatRecoveryRateFunctionofPartLoadRatioCurve() {
  getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->resetHeatRecoveryRateFunctionofPartLoadRatioCurve();
}

bool GeneratorMicroTurbineHeatRecovery::setHeatRecoveryRateFunctionofInletWaterTemperatureCurve(const Curve& curve) {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->setHeatRecoveryRateFunctionofInletWaterTemperatureCurve(curve);
}

void GeneratorMicroTurbineHeatRecovery::resetHeatRecoveryRateFunctionofInletWaterTemperatureCurve() {
  getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->resetHeatRecoveryRateFunctionofInletWaterTemperatureCurve();
}

bool GeneratorMicroTurbineHeatRecovery::setHeatRecoveryRateFunctionofWaterFlowRateCurve(const Curve& curve) {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->setHeatRecoveryRateFunctionofWaterFlowRateCurve(curve);
}

void GeneratorMicroTurbineHeatRecovery::resetHeatRecoveryRateFunctionofWaterFlowRateCurve() {
  getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->resetHeatRecoveryRateFunctionofWaterFlowRateCurve();
}

bool GeneratorMicroTurbineHeatRecovery::setMinimumHeatRecoveryWaterFlowRate(double minimumHeatRecoveryWaterFlowRate) {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->setMinimumHeatRecoveryWaterFlowRate(minimumHeatRecoveryWaterFlowRate);
}

void GeneratorMicroTurbineHeatRecovery::resetMinimumHeatRecoveryWaterFlowRate() {
  getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->resetMinimumHeatRecoveryWaterFlowRate();
}

bool GeneratorMicroTurbineHeatRecovery::setMaximumHeatRecoveryWaterFlowRate(double maximumHeatRecoveryWaterFlowRate) {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->setMaximumHeatRecoveryWaterFlowRate(maximumHeatRecoveryWaterFlowRate);
}

void GeneratorMicroTurbineHeatRecovery::resetMaximumHeatRecoveryWaterFlowRate() {
  getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->resetMaximumHeatRecoveryWaterFlowRate();
}

bool GeneratorMicroTurbineHeatRecovery::setMaximumHeatRecoveryWaterTemperature(double maximumHeatRecoveryWaterTemperature) {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->setMaximumHeatRecoveryWaterTemperature(maximumHeatRecoveryWaterTemperature);
}

void GeneratorMicroTurbineHeatRecovery::resetMaximumHeatRecoveryWaterTemperature() {
  getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->resetMaximumHeatRecoveryWaterTemperature();
}

bool GeneratorMicroTurbineHeatRecovery::setRatedThermaltoElectricalPowerRatio(double ratedThermaltoElectricalPowerRatio) {
  return getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->setRatedThermaltoElectricalPowerRatio(ratedThermaltoElectricalPowerRatio);
}

void GeneratorMicroTurbineHeatRecovery::resetRatedThermaltoElectricalPowerRatio() {
  getImpl<detail::GeneratorMicroTurbineHeatRecovery_Impl>()->resetRatedThermaltoElectricalPowerRatio();
}

/// @cond
GeneratorMicroTurbineHeatRecovery::GeneratorMicroTurbineHeatRecovery(std::shared_ptr<detail::GeneratorMicroTurbineHeatRecovery_Impl> impl)
  : StraightComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

