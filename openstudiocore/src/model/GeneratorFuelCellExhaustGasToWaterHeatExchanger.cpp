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
#include "GeneratorFuelCellAirSupply.hpp"
#include "GeneratorFuelCellAirSupply_Impl.hpp"
#include "GeneratorFuelCellWaterSupply.hpp"
#include "GeneratorFuelCellWaterSupply_Impl.hpp"
#include "GeneratorFuelCellAuxiliaryHeater.hpp"
#include "GeneratorFuelCellAuxiliaryHeater_Impl.hpp"
#include "GeneratorFuelCellExhaustGasToWaterHeatExchanger.hpp"
#include "GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl.hpp"
#include "GeneratorFuelCellElectricalStorage.hpp"
#include "GeneratorFuelCellElectricalStorage_Impl.hpp"
#include "GeneratorFuelCellInverter.hpp"
#include "GeneratorFuelCellInverter_Impl.hpp"
#include "GeneratorFuelCellStackCooler.hpp"
#include "GeneratorFuelCellStackCooler_Impl.hpp"
#include "GeneratorFuelSupply.hpp"
#include "GeneratorFuelSupply_Impl.hpp"

#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Generator_FuelCell_ExhaustGasToWaterHeatExchanger_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl(const IdfObject& idfObject,
                                                                                                             Model_Impl* model,
                                                                                                             bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == GeneratorFuelCellExhaustGasToWaterHeatExchanger::iddObjectType());
  }

  GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                             Model_Impl* model,
                                                                                                             bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == GeneratorFuelCellExhaustGasToWaterHeatExchanger::iddObjectType());
  }

  GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl(const GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl& other,
                                                                                                             Model_Impl* model,
                                                                                                             bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::iddObjectType() const {
    return GeneratorFuelCellExhaustGasToWaterHeatExchanger::iddObjectType();
  }

  // This will clone both the GeneratorFuelCellExhaustGasToWaterHeatExchanger and its linked GeneratorFuelCell
  // and will return a reference to the GeneratorMicroTurbineHeatRecovery
  ModelObject GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::clone(Model model) const {

    // We call the parent generator's Clone method which will clone both the fuelCell and fuelCellHX
    GeneratorFuelCell fs = fuelCell();
    GeneratorFuelCell fsClone = fs.clone(model).cast<GeneratorFuelCell>();

    // We get the clone of the parent generator's MTHR so we can return that
    GeneratorFuelCellExhaustGasToWaterHeatExchanger hxClone = fsClone.heatExchanger();


    return hxClone;
  }

  std::vector<IddObjectType> GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::allowableChildTypes() const {
    std::vector<IddObjectType> result;
    result.push_back(IddObjectType::OS_Generator_FuelCell);
    return result;
  }

  // Returns the children object
  std::vector<ModelObject> GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::children() const {
    std::vector<ModelObject> result;

    if (boost::optional<GeneratorFuelCell> fc = fuelCell()) {
      result.push_back(fc.get());
    }

    return result;
  }

  // Get the parent GeneratorFuelCell
  GeneratorFuelCell GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::fuelCell() const {

    boost::optional<GeneratorFuelCell> value;
    for (const GeneratorFuelCell& fc : this->model().getConcreteModelObjects<GeneratorFuelCell>()) {
      if (boost::optional<GeneratorFuelCellExhaustGasToWaterHeatExchanger> fcHX = fc.heatExchanger()) {
        if (fcHX->handle() == this->handle()) {
          value = fc;
        }
      }
    }
    OS_ASSERT(value);
    return value.get();

  }

  double GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::heatRecoveryWaterMaximumFlowRate() const {
    boost::optional<double> value = getDouble(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::HeatRecoveryWaterMaximumFlowRate,true);
    if (!value) {
      LOG_AND_THROW(" does not have a heatRecoveryWaterMaximumFlowRate.");
    }
    return value.get();
  }

  boost::optional<Node> GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::exhaustOutletAirNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::ExhaustOutletAirNodeName);
  }

  std::string GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::heatExchangerCalculationMethod() const {
    boost::optional<std::string> value = getString(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::HeatExchangerCalculationMethod,true);
    if (!value) {
      LOG_AND_THROW(" does not have a heatExchangerCalculationMethod.");
    }
    return value.get();
  }

  boost::optional<double> GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::method1HeatExchangerEffectiveness() const {
    return getDouble(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method1HeatExchangerEffectiveness,true);
  }

  boost::optional<double> GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::method2Parameterhxs0() const {
    return getDouble(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method2Parameterhxs0,true);
  }

  boost::optional<double> GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::method2Parameterhxs1() const {
    return getDouble(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method2Parameterhxs1,true);
  }

  boost::optional<double> GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::method2Parameterhxs2() const {
    return getDouble(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method2Parameterhxs2,true);
  }

  boost::optional<double> GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::method2Parameterhxs3() const {
    return getDouble(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method2Parameterhxs3,true);
  }

  boost::optional<double> GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::method2Parameterhxs4() const {
    return getDouble(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method2Parameterhxs4,true);
  }

  boost::optional<double> GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::method3h0GasCoefficient() const {
    return getDouble(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method3h0GasCoefficient,true);
  }

  boost::optional<double> GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::method3NdotGasRefCoefficient() const {
    return getDouble(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method3NdotGasRefCoefficient,true);
  }

  boost::optional<double> GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::method3nCoefficient() const {
    return getDouble(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method3nCoefficient,true);
  }

  boost::optional<double> GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::method3GasArea() const {
    return getDouble(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method3GasArea,true);
  }

  boost::optional<double> GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::method3h0WaterCoefficient() const {
    return getDouble(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method3h0WaterCoefficient,true);
  }

  boost::optional<double> GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::method3NdotWaterrefCoefficient() const {
    return getDouble(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method3NdotWaterrefCoefficient,true);
  }

  boost::optional<double> GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::method3mCoefficient() const {
    return getDouble(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method3mCoefficient,true);
  }

  boost::optional<double> GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::method3WaterArea() const {
    return getDouble(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method3WaterArea,true);
  }

  boost::optional<double> GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::method3FAdjustmentFactor() const {
    return getDouble(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method3FAdjustmentFactor,true);
  }

  boost::optional<double> GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::method4hxl1Coefficient() const {
    return getDouble(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method4hxl1Coefficient,true);
  }

  boost::optional<double> GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::method4hxl2Coefficient() const {
    return getDouble(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method4hxl2Coefficient,true);
  }

  boost::optional<double> GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::method4CondensationThreshold() const {
    return getDouble(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method4CondensationThreshold,true);
  }

  void GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::setHeatRecoveryWaterMaximumFlowRate(double heatRecoveryWaterMaximumFlowRate) {
    bool result = setDouble(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::HeatRecoveryWaterMaximumFlowRate, heatRecoveryWaterMaximumFlowRate);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::resetHeatRecoveryWaterMaximumFlowRate() {
    bool result = setDouble(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::HeatRecoveryWaterMaximumFlowRate, 0);
    OS_ASSERT(result);
  }

  bool GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::setExhaustOutletAirNode(const Node& node) {
    bool result = setPointer(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::ExhaustOutletAirNodeName, node.handle());
    return result;
  }

  void GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::resetExhaustOutletAirNode() {
    bool result = setString(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::ExhaustOutletAirNodeName, "");
    OS_ASSERT(result);
  }

  bool GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::setHeatExchangerCalculationMethod(const std::string& heatExchangerCalculationMethod) {
    bool result = setString(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::HeatExchangerCalculationMethod, heatExchangerCalculationMethod);
    if (heatExchangerCalculationMethod == "FixedEffectiveness") {
      this->setMethod1HeatExchangerEffectiveness(1.0);
    }
    else if (heatExchangerCalculationMethod == "EmpiricalUAeff") {
      this->setMethod2Parameterhxs0(1.0);
      this->setMethod2Parameterhxs1(0.0);
      this->setMethod2Parameterhxs2(0.0);
      this->setMethod2Parameterhxs3(0.0);
      this->setMethod2Parameterhxs4(0.0);
    }
    else if (heatExchangerCalculationMethod == "FundementalUAeff") {
      this->setMethod3FAdjustmentFactor(0.0);
      this->setMethod3GasArea(1.0);
      this->setMethod3h0GasCoefficient(1.0);
      this->setMethod3h0WaterCoefficient(1.0);
      this->setMethod3mCoefficient(1.0);
      this->setMethod3nCoefficient(1.0);
      this->setMethod3NdotGasRefCoefficient(1.0);
      this->setMethod3NdotWaterrefCoefficient(1.0);
      this->setMethod3WaterArea(1.0);
    }
    else if (heatExchangerCalculationMethod == "Condensing") {
      this->setMethod4CondensationThreshold(35.0);
      this->setMethod4hxl1Coefficient(1.0);
      this->setMethod4hxl2Coefficient(1.0);
    }
    return result;
  }

  void GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::resetHeatExchangerCalculationMethod() {
    bool result = this->setHeatExchangerCalculationMethod("FixedEffectiveness");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::setMethod1HeatExchangerEffectiveness(double method1HeatExchangerEffectiveness) {
    bool result = setDouble(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method1HeatExchangerEffectiveness, method1HeatExchangerEffectiveness);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::resetMethod1HeatExchangerEffectiveness() {
    bool result = setString(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method1HeatExchangerEffectiveness, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::setMethod2Parameterhxs0(double method2Parameterhxs0) {
    bool result = setDouble(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method2Parameterhxs0, method2Parameterhxs0);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::resetMethod2Parameterhxs0() {
    bool result = setString(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method2Parameterhxs0, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::setMethod2Parameterhxs1(double method2Parameterhxs1) {
    bool result = setDouble(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method2Parameterhxs1, method2Parameterhxs1);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::resetMethod2Parameterhxs1() {
    bool result = setString(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method2Parameterhxs1, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::setMethod2Parameterhxs2(double method2Parameterhxs2) {
    bool result = setDouble(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method2Parameterhxs2, method2Parameterhxs2);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::resetMethod2Parameterhxs2() {
    bool result = setString(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method2Parameterhxs2, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::setMethod2Parameterhxs3(double method2Parameterhxs3) {
    bool result = setDouble(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method2Parameterhxs3, method2Parameterhxs3);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::resetMethod2Parameterhxs3() {
    bool result = setString(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method2Parameterhxs3, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::setMethod2Parameterhxs4(double method2Parameterhxs4) {
    bool result = setDouble(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method2Parameterhxs4, method2Parameterhxs4);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::resetMethod2Parameterhxs4() {
    bool result = setString(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method2Parameterhxs4, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::setMethod3h0GasCoefficient(double method3h0GasCoefficient) {
    bool result = setDouble(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method3h0GasCoefficient, method3h0GasCoefficient);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::resetMethod3h0GasCoefficient() {
    bool result = setString(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method3h0GasCoefficient, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::setMethod3NdotGasRefCoefficient(double method3NdotGasRefCoefficient) {
    bool result = setDouble(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method3NdotGasRefCoefficient, method3NdotGasRefCoefficient);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::resetMethod3NdotGasRefCoefficient() {
    bool result = setString(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method3NdotGasRefCoefficient, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::setMethod3nCoefficient(double method3nCoefficient) {
    bool result = setDouble(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method3nCoefficient, method3nCoefficient);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::resetMethod3nCoefficient() {
    bool result = setString(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method3nCoefficient, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::setMethod3GasArea(double method3GasArea) {
    bool result = setDouble(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method3GasArea, method3GasArea);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::resetMethod3GasArea() {
    bool result = setString(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method3GasArea, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::setMethod3h0WaterCoefficient(double method3h0WaterCoefficient) {
    bool result = setDouble(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method3h0WaterCoefficient, method3h0WaterCoefficient);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::resetMethod3h0WaterCoefficient() {
    bool result = setString(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method3h0WaterCoefficient, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::setMethod3NdotWaterrefCoefficient(double method3NdotWaterrefCoefficient) {
    bool result = setDouble(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method3NdotWaterrefCoefficient, method3NdotWaterrefCoefficient);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::resetMethod3NdotWaterrefCoefficient() {
    bool result = setString(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method3NdotWaterrefCoefficient, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::setMethod3mCoefficient(double method3mCoefficient) {
    bool result = setDouble(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method3mCoefficient, method3mCoefficient);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::resetMethod3mCoefficient() {
    bool result = setString(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method3mCoefficient, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::setMethod3WaterArea(double method3WaterArea) {
    bool result = setDouble(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method3WaterArea, method3WaterArea);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::resetMethod3WaterArea() {
    bool result = setString(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method3WaterArea, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::setMethod3FAdjustmentFactor(double method3FAdjustmentFactor) {
    bool result = setDouble(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method3FAdjustmentFactor, method3FAdjustmentFactor);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::resetMethod3FAdjustmentFactor() {
    bool result = setString(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method3FAdjustmentFactor, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::setMethod4hxl1Coefficient(double method4hxl1Coefficient) {
    bool result = setDouble(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method4hxl1Coefficient, method4hxl1Coefficient);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::resetMethod4hxl1Coefficient() {
    bool result = setString(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method4hxl1Coefficient, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::setMethod4hxl2Coefficient(double method4hxl2Coefficient) {
    bool result = setDouble(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method4hxl2Coefficient, method4hxl2Coefficient);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::resetMethod4hxl2Coefficient() {
    bool result = setString(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method4hxl2Coefficient, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::setMethod4CondensationThreshold(double method4CondensationThreshold) {
    bool result = setDouble(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method4CondensationThreshold, method4CondensationThreshold);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::resetMethod4CondensationThreshold() {
    bool result = setString(OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method4CondensationThreshold, "");
    OS_ASSERT(result);
  }

  unsigned GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::inletPort() {
    return OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::HeatRecoveryWaterInletNodeName;
  }

  unsigned GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::outletPort() {
    return OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::HeatRecoveryWaterOutletNodeName;
  }

  bool GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl::addToNode(Node & node) {
    if (boost::optional<PlantLoop> plant = node.plantLoop()) {
      return StraightComponent_Impl::addToNode(node);
    }

    return false;
  }

} // detail

GeneratorFuelCellExhaustGasToWaterHeatExchanger::GeneratorFuelCellExhaustGasToWaterHeatExchanger(const Model& model,
                                                                                                 const Node& exhaustOutletAirNode)
  : StraightComponent(GeneratorFuelCellExhaustGasToWaterHeatExchanger::iddObjectType(), model) {
  OS_ASSERT(getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>());

  //note: This field contains the name of the node that connects the heat exchanger s inlet to the plant loop. This water is used for heat recovery.
  //setHeatRecoveryWaterInletNode(waterInletNode);
  //note: This field contains the name of the node that connects the heat exchanger s outlet to the plant loop.
  //setHeatRecoveryWaterOutletNode(waterOutletNode);
  //note: This field is used to determine which node will receive the exhaust air stream leaving the FC. This node will usually be outside and not be referenced elsewhere.
  //      However, if the exhaust stream is used in a heat recovery ventilator (as described in section 11 of the Annex 42 specification) then the node would be reference in the heat recovery ventilator object.
  setExhaustOutletAirNode(exhaustOutletAirNode);
  setHeatRecoveryWaterMaximumFlowRate(0.0004);
  setHeatExchangerCalculationMethod("FixedEffectiveness");
}

GeneratorFuelCellExhaustGasToWaterHeatExchanger::GeneratorFuelCellExhaustGasToWaterHeatExchanger(const Model& model)
  : StraightComponent(GeneratorFuelCellExhaustGasToWaterHeatExchanger::iddObjectType(), model)
{
  OS_ASSERT(getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>());

  setHeatRecoveryWaterMaximumFlowRate(0.0004);
  setHeatExchangerCalculationMethod("Condensing");
  setMethod2Parameterhxs0(83.1);
  setMethod2Parameterhxs1(4798);
  setMethod2Parameterhxs2(-138e3);
  setMethod2Parameterhxs3(-353.8e3);
  setMethod2Parameterhxs4(5.15e8);
  setMethod4hxl1Coefficient(-0.000196);
  setMethod4hxl1Coefficient(0.0031);
  setMethod4CondensationThreshold(35.0);
}

IddObjectType GeneratorFuelCellExhaustGasToWaterHeatExchanger::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Generator_FuelCell_ExhaustGasToWaterHeatExchanger);
}

std::vector<std::string> GeneratorFuelCellExhaustGasToWaterHeatExchanger::heatExchangerCalculationMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::HeatExchangerCalculationMethod);
}

double GeneratorFuelCellExhaustGasToWaterHeatExchanger::heatRecoveryWaterMaximumFlowRate() const {
  return getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->heatRecoveryWaterMaximumFlowRate();
}

boost::optional<Node> GeneratorFuelCellExhaustGasToWaterHeatExchanger::exhaustOutletAirNode() const {
  return getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->exhaustOutletAirNode();
}

std::string GeneratorFuelCellExhaustGasToWaterHeatExchanger::heatExchangerCalculationMethod() const {
  return getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->heatExchangerCalculationMethod();
}

boost::optional<double> GeneratorFuelCellExhaustGasToWaterHeatExchanger::method1HeatExchangerEffectiveness() const {
  return getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->method1HeatExchangerEffectiveness();
}

boost::optional<double> GeneratorFuelCellExhaustGasToWaterHeatExchanger::method2Parameterhxs0() const {
  return getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->method2Parameterhxs0();
}

boost::optional<double> GeneratorFuelCellExhaustGasToWaterHeatExchanger::method2Parameterhxs1() const {
  return getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->method2Parameterhxs1();
}

boost::optional<double> GeneratorFuelCellExhaustGasToWaterHeatExchanger::method2Parameterhxs2() const {
  return getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->method2Parameterhxs2();
}

boost::optional<double> GeneratorFuelCellExhaustGasToWaterHeatExchanger::method2Parameterhxs3() const {
  return getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->method2Parameterhxs3();
}

boost::optional<double> GeneratorFuelCellExhaustGasToWaterHeatExchanger::method2Parameterhxs4() const {
  return getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->method2Parameterhxs4();
}

boost::optional<double> GeneratorFuelCellExhaustGasToWaterHeatExchanger::method3h0GasCoefficient() const {
  return getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->method3h0GasCoefficient();
}

boost::optional<double> GeneratorFuelCellExhaustGasToWaterHeatExchanger::method3NdotGasRefCoefficient() const {
  return getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->method3NdotGasRefCoefficient();
}

boost::optional<double> GeneratorFuelCellExhaustGasToWaterHeatExchanger::method3nCoefficient() const {
  return getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->method3nCoefficient();
}

boost::optional<double> GeneratorFuelCellExhaustGasToWaterHeatExchanger::method3GasArea() const {
  return getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->method3GasArea();
}

boost::optional<double> GeneratorFuelCellExhaustGasToWaterHeatExchanger::method3h0WaterCoefficient() const {
  return getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->method3h0WaterCoefficient();
}

boost::optional<double> GeneratorFuelCellExhaustGasToWaterHeatExchanger::method3NdotWaterrefCoefficient() const {
  return getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->method3NdotWaterrefCoefficient();
}

boost::optional<double> GeneratorFuelCellExhaustGasToWaterHeatExchanger::method3mCoefficient() const {
  return getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->method3mCoefficient();
}

boost::optional<double> GeneratorFuelCellExhaustGasToWaterHeatExchanger::method3WaterArea() const {
  return getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->method3WaterArea();
}

boost::optional<double> GeneratorFuelCellExhaustGasToWaterHeatExchanger::method3FAdjustmentFactor() const {
  return getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->method3FAdjustmentFactor();
}

boost::optional<double> GeneratorFuelCellExhaustGasToWaterHeatExchanger::method4hxl1Coefficient() const {
  return getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->method4hxl1Coefficient();
}

boost::optional<double> GeneratorFuelCellExhaustGasToWaterHeatExchanger::method4hxl2Coefficient() const {
  return getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->method4hxl2Coefficient();
}

boost::optional<double> GeneratorFuelCellExhaustGasToWaterHeatExchanger::method4CondensationThreshold() const {
  return getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->method4CondensationThreshold();
}

void GeneratorFuelCellExhaustGasToWaterHeatExchanger::setHeatRecoveryWaterMaximumFlowRate(double heatRecoveryWaterMaximumFlowRate) {
  getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->setHeatRecoveryWaterMaximumFlowRate(heatRecoveryWaterMaximumFlowRate);
}

void GeneratorFuelCellExhaustGasToWaterHeatExchanger::resetHeatRecoveryWaterMaximumFlowRate() {
  getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->resetHeatRecoveryWaterMaximumFlowRate();
}

bool GeneratorFuelCellExhaustGasToWaterHeatExchanger::setExhaustOutletAirNode(const Node& node) {
  return getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->setExhaustOutletAirNode(node);
}

void GeneratorFuelCellExhaustGasToWaterHeatExchanger::resetExhaustOutletAirNode() {
  getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->resetExhaustOutletAirNode();
}

bool GeneratorFuelCellExhaustGasToWaterHeatExchanger::setHeatExchangerCalculationMethod(const std::string& heatExchangerCalculationMethod) {
  return getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->setHeatExchangerCalculationMethod(heatExchangerCalculationMethod);
}

void GeneratorFuelCellExhaustGasToWaterHeatExchanger::resetHeatExchangerCalculationMethod() {
  getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->resetHeatExchangerCalculationMethod();
}

void GeneratorFuelCellExhaustGasToWaterHeatExchanger::setMethod1HeatExchangerEffectiveness(double method1HeatExchangerEffectiveness) {
  getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->setMethod1HeatExchangerEffectiveness(method1HeatExchangerEffectiveness);
}

void GeneratorFuelCellExhaustGasToWaterHeatExchanger::resetMethod1HeatExchangerEffectiveness() {
  getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->resetMethod1HeatExchangerEffectiveness();
}

void GeneratorFuelCellExhaustGasToWaterHeatExchanger::setMethod2Parameterhxs0(double method2Parameterhxs0) {
  getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->setMethod2Parameterhxs0(method2Parameterhxs0);
}

void GeneratorFuelCellExhaustGasToWaterHeatExchanger::resetMethod2Parameterhxs0() {
  getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->resetMethod2Parameterhxs0();
}

void GeneratorFuelCellExhaustGasToWaterHeatExchanger::setMethod2Parameterhxs1(double method2Parameterhxs1) {
  getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->setMethod2Parameterhxs1(method2Parameterhxs1);
}

void GeneratorFuelCellExhaustGasToWaterHeatExchanger::resetMethod2Parameterhxs1() {
  getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->resetMethod2Parameterhxs1();
}

void GeneratorFuelCellExhaustGasToWaterHeatExchanger::setMethod2Parameterhxs2(double method2Parameterhxs2) {
  getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->setMethod2Parameterhxs2(method2Parameterhxs2);
}

void GeneratorFuelCellExhaustGasToWaterHeatExchanger::resetMethod2Parameterhxs2() {
  getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->resetMethod2Parameterhxs2();
}

void GeneratorFuelCellExhaustGasToWaterHeatExchanger::setMethod2Parameterhxs3(double method2Parameterhxs3) {
  getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->setMethod2Parameterhxs3(method2Parameterhxs3);
}

void GeneratorFuelCellExhaustGasToWaterHeatExchanger::resetMethod2Parameterhxs3() {
  getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->resetMethod2Parameterhxs3();
}

void GeneratorFuelCellExhaustGasToWaterHeatExchanger::setMethod2Parameterhxs4(double method2Parameterhxs4) {
  getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->setMethod2Parameterhxs4(method2Parameterhxs4);
}

void GeneratorFuelCellExhaustGasToWaterHeatExchanger::resetMethod2Parameterhxs4() {
  getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->resetMethod2Parameterhxs4();
}

void GeneratorFuelCellExhaustGasToWaterHeatExchanger::setMethod3h0GasCoefficient(double method3h0GasCoefficient) {
  getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->setMethod3h0GasCoefficient(method3h0GasCoefficient);
}

void GeneratorFuelCellExhaustGasToWaterHeatExchanger::resetMethod3h0GasCoefficient() {
  getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->resetMethod3h0GasCoefficient();
}

void GeneratorFuelCellExhaustGasToWaterHeatExchanger::setMethod3NdotGasRefCoefficient(double method3NdotGasRefCoefficient) {
  getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->setMethod3NdotGasRefCoefficient(method3NdotGasRefCoefficient);
}

void GeneratorFuelCellExhaustGasToWaterHeatExchanger::resetMethod3NdotGasRefCoefficient() {
  getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->resetMethod3NdotGasRefCoefficient();
}

void GeneratorFuelCellExhaustGasToWaterHeatExchanger::setMethod3nCoefficient(double method3nCoefficient) {
  getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->setMethod3nCoefficient(method3nCoefficient);
}

void GeneratorFuelCellExhaustGasToWaterHeatExchanger::resetMethod3nCoefficient() {
  getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->resetMethod3nCoefficient();
}

void GeneratorFuelCellExhaustGasToWaterHeatExchanger::setMethod3GasArea(double method3GasArea) {
  getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->setMethod3GasArea(method3GasArea);
}

void GeneratorFuelCellExhaustGasToWaterHeatExchanger::resetMethod3GasArea() {
  getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->resetMethod3GasArea();
}

void GeneratorFuelCellExhaustGasToWaterHeatExchanger::setMethod3h0WaterCoefficient(double method3h0WaterCoefficient) {
  getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->setMethod3h0WaterCoefficient(method3h0WaterCoefficient);
}

void GeneratorFuelCellExhaustGasToWaterHeatExchanger::resetMethod3h0WaterCoefficient() {
  getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->resetMethod3h0WaterCoefficient();
}

void GeneratorFuelCellExhaustGasToWaterHeatExchanger::setMethod3NdotWaterrefCoefficient(double method3NdotWaterrefCoefficient) {
  getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->setMethod3NdotWaterrefCoefficient(method3NdotWaterrefCoefficient);
}

void GeneratorFuelCellExhaustGasToWaterHeatExchanger::resetMethod3NdotWaterrefCoefficient() {
  getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->resetMethod3NdotWaterrefCoefficient();
}

void GeneratorFuelCellExhaustGasToWaterHeatExchanger::setMethod3mCoefficient(double method3mCoefficient) {
  getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->setMethod3mCoefficient(method3mCoefficient);
}

void GeneratorFuelCellExhaustGasToWaterHeatExchanger::resetMethod3mCoefficient() {
  getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->resetMethod3mCoefficient();
}

void GeneratorFuelCellExhaustGasToWaterHeatExchanger::setMethod3WaterArea(double method3WaterArea) {
  getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->setMethod3WaterArea(method3WaterArea);
}

void GeneratorFuelCellExhaustGasToWaterHeatExchanger::resetMethod3WaterArea() {
  getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->resetMethod3WaterArea();
}

void GeneratorFuelCellExhaustGasToWaterHeatExchanger::setMethod3FAdjustmentFactor(double method3FAdjustmentFactor) {
  getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->setMethod3FAdjustmentFactor(method3FAdjustmentFactor);
}

void GeneratorFuelCellExhaustGasToWaterHeatExchanger::resetMethod3FAdjustmentFactor() {
  getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->resetMethod3FAdjustmentFactor();
}

void GeneratorFuelCellExhaustGasToWaterHeatExchanger::setMethod4hxl1Coefficient(double method4hxl1Coefficient) {
  getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->setMethod4hxl1Coefficient(method4hxl1Coefficient);
}

void GeneratorFuelCellExhaustGasToWaterHeatExchanger::resetMethod4hxl1Coefficient() {
  getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->resetMethod4hxl1Coefficient();
}

void GeneratorFuelCellExhaustGasToWaterHeatExchanger::setMethod4hxl2Coefficient(double method4hxl2Coefficient) {
  getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->setMethod4hxl2Coefficient(method4hxl2Coefficient);
}

void GeneratorFuelCellExhaustGasToWaterHeatExchanger::resetMethod4hxl2Coefficient() {
  getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->resetMethod4hxl2Coefficient();
}

void GeneratorFuelCellExhaustGasToWaterHeatExchanger::setMethod4CondensationThreshold(double method4CondensationThreshold) {
  getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->setMethod4CondensationThreshold(method4CondensationThreshold);
}

void GeneratorFuelCellExhaustGasToWaterHeatExchanger::resetMethod4CondensationThreshold() {
  getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->resetMethod4CondensationThreshold();
}

GeneratorFuelCell GeneratorFuelCellExhaustGasToWaterHeatExchanger::fuelCell() const {
  return getImpl<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl>()->fuelCell();
}

/// @cond
GeneratorFuelCellExhaustGasToWaterHeatExchanger::GeneratorFuelCellExhaustGasToWaterHeatExchanger(std::shared_ptr<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl> impl)
  : StraightComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

