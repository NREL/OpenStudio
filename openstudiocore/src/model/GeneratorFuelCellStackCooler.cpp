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

#include "GeneratorFuelCellStackCooler.hpp"
#include "GeneratorFuelCellStackCooler_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Generator_FuelCell_StackCooler_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  GeneratorFuelCellStackCooler_Impl::GeneratorFuelCellStackCooler_Impl(const IdfObject& idfObject,
                                                                       Model_Impl* model,
                                                                       bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == GeneratorFuelCellStackCooler::iddObjectType());
  }

  GeneratorFuelCellStackCooler_Impl::GeneratorFuelCellStackCooler_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                       Model_Impl* model,
                                                                       bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == GeneratorFuelCellStackCooler::iddObjectType());
  }

  GeneratorFuelCellStackCooler_Impl::GeneratorFuelCellStackCooler_Impl(const GeneratorFuelCellStackCooler_Impl& other,
                                                                       Model_Impl* model,
                                                                       bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& GeneratorFuelCellStackCooler_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType GeneratorFuelCellStackCooler_Impl::iddObjectType() const {
    return GeneratorFuelCellStackCooler::iddObjectType();
  }

  boost::optional<Connection> GeneratorFuelCellStackCooler_Impl::heatRecoveryWaterInletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_Generator_FuelCell_StackCoolerFields::HeatRecoveryWaterInletNodeName);
  }

  boost::optional<Connection> GeneratorFuelCellStackCooler_Impl::heatRecoveryWaterOutletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_Generator_FuelCell_StackCoolerFields::HeatRecoveryWaterOutletNodeName);
  }

  boost::optional<double> GeneratorFuelCellStackCooler_Impl::nominalStackTemperature() const {
    return getDouble(OS_Generator_FuelCell_StackCoolerFields::NominalStackTemperature,true);
  }

  boost::optional<double> GeneratorFuelCellStackCooler_Impl::actualStackTemperature() const {
    return getDouble(OS_Generator_FuelCell_StackCoolerFields::ActualStackTemperature,true);
  }

  boost::optional<double> GeneratorFuelCellStackCooler_Impl::coefficientr0() const {
    return getDouble(OS_Generator_FuelCell_StackCoolerFields::Coefficientr0,true);
  }

  boost::optional<double> GeneratorFuelCellStackCooler_Impl::coefficientr1() const {
    return getDouble(OS_Generator_FuelCell_StackCoolerFields::Coefficientr1,true);
  }

  boost::optional<double> GeneratorFuelCellStackCooler_Impl::coefficientr2() const {
    return getDouble(OS_Generator_FuelCell_StackCoolerFields::Coefficientr2,true);
  }

  boost::optional<double> GeneratorFuelCellStackCooler_Impl::coefficientr3() const {
    return getDouble(OS_Generator_FuelCell_StackCoolerFields::Coefficientr3,true);
  }

  boost::optional<double> GeneratorFuelCellStackCooler_Impl::stackCoolantFlowRate() const {
    return getDouble(OS_Generator_FuelCell_StackCoolerFields::StackCoolantFlowRate,true);
  }

  boost::optional<double> GeneratorFuelCellStackCooler_Impl::stackCoolerUFactorTimesAreaValue() const {
    return getDouble(OS_Generator_FuelCell_StackCoolerFields::StackCoolerUFactorTimesAreaValue,true);
  }

  boost::optional<double> GeneratorFuelCellStackCooler_Impl::fscogenAdjustmentFactor() const {
    return getDouble(OS_Generator_FuelCell_StackCoolerFields::FscogenAdjustmentFactor,true);
  }

  boost::optional<double> GeneratorFuelCellStackCooler_Impl::stackCogenerationExchangerArea() const {
    return getDouble(OS_Generator_FuelCell_StackCoolerFields::StackCogenerationExchangerArea,true);
  }

  boost::optional<double> GeneratorFuelCellStackCooler_Impl::stackCogenerationExchangerNominalFlowRate() const {
    return getDouble(OS_Generator_FuelCell_StackCoolerFields::StackCogenerationExchangerNominalFlowRate,true);
  }

  boost::optional<double> GeneratorFuelCellStackCooler_Impl::stackCogenerationExchangerNominalHeatTransferCoefficient() const {
    return getDouble(OS_Generator_FuelCell_StackCoolerFields::StackCogenerationExchangerNominalHeatTransferCoefficient,true);
  }

  boost::optional<double> GeneratorFuelCellStackCooler_Impl::stackCogenerationExchangerNominalHeatTransferCoefficientExponent() const {
    return getDouble(OS_Generator_FuelCell_StackCoolerFields::StackCogenerationExchangerNominalHeatTransferCoefficientExponent,true);
  }

  boost::optional<double> GeneratorFuelCellStackCooler_Impl::stackCoolerPumpPower() const {
    return getDouble(OS_Generator_FuelCell_StackCoolerFields::StackCoolerPumpPower,true);
  }

  boost::optional<double> GeneratorFuelCellStackCooler_Impl::stackCoolerPumpHeatLossFraction() const {
    return getDouble(OS_Generator_FuelCell_StackCoolerFields::StackCoolerPumpHeatLossFraction,true);
  }

  boost::optional<double> GeneratorFuelCellStackCooler_Impl::stackAirCoolerFanCoefficientf0() const {
    return getDouble(OS_Generator_FuelCell_StackCoolerFields::StackAirCoolerFanCoefficientf0,true);
  }

  boost::optional<double> GeneratorFuelCellStackCooler_Impl::stackAirCoolerFanCoefficientf1() const {
    return getDouble(OS_Generator_FuelCell_StackCoolerFields::StackAirCoolerFanCoefficientf1,true);
  }

  boost::optional<double> GeneratorFuelCellStackCooler_Impl::stackAirCoolerFanCoefficientf2() const {
    return getDouble(OS_Generator_FuelCell_StackCoolerFields::StackAirCoolerFanCoefficientf2,true);
  }

  bool GeneratorFuelCellStackCooler_Impl::setHeatRecoveryWaterInletNode(const Connection& connection) {
    bool result = setPointer(OS_Generator_FuelCell_StackCoolerFields::HeatRecoveryWaterInletNodeName, connection.handle());
    return result;
  }

  void GeneratorFuelCellStackCooler_Impl::resetHeatRecoveryWaterInletNode() {
    bool result = setString(OS_Generator_FuelCell_StackCoolerFields::HeatRecoveryWaterInletNodeName, "");
    OS_ASSERT(result);
  }

  bool GeneratorFuelCellStackCooler_Impl::setHeatRecoveryWaterOutletNode(const Connection& connection) {
    bool result = setPointer(OS_Generator_FuelCell_StackCoolerFields::HeatRecoveryWaterOutletNodeName, connection.handle());
    return result;
  }

  void GeneratorFuelCellStackCooler_Impl::resetHeatRecoveryWaterOutletNode() {
    bool result = setString(OS_Generator_FuelCell_StackCoolerFields::HeatRecoveryWaterOutletNodeName, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellStackCooler_Impl::setNominalStackTemperature(double nominalStackTemperature) {
    bool result = setDouble(OS_Generator_FuelCell_StackCoolerFields::NominalStackTemperature, nominalStackTemperature);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellStackCooler_Impl::resetNominalStackTemperature() {
    bool result = setString(OS_Generator_FuelCell_StackCoolerFields::NominalStackTemperature, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellStackCooler_Impl::setActualStackTemperature(double actualStackTemperature) {
    bool result = setDouble(OS_Generator_FuelCell_StackCoolerFields::ActualStackTemperature, actualStackTemperature);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellStackCooler_Impl::resetActualStackTemperature() {
    bool result = setString(OS_Generator_FuelCell_StackCoolerFields::ActualStackTemperature, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellStackCooler_Impl::setCoefficientr0(double coefficientr0) {
    bool result = setDouble(OS_Generator_FuelCell_StackCoolerFields::Coefficientr0, coefficientr0);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellStackCooler_Impl::resetCoefficientr0() {
    bool result = setString(OS_Generator_FuelCell_StackCoolerFields::Coefficientr0, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellStackCooler_Impl::setCoefficientr1(double coefficientr1) {
    bool result = setDouble(OS_Generator_FuelCell_StackCoolerFields::Coefficientr1, coefficientr1);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellStackCooler_Impl::resetCoefficientr1() {
    bool result = setString(OS_Generator_FuelCell_StackCoolerFields::Coefficientr1, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellStackCooler_Impl::setCoefficientr2(double coefficientr2) {
    bool result = setDouble(OS_Generator_FuelCell_StackCoolerFields::Coefficientr2, coefficientr2);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellStackCooler_Impl::resetCoefficientr2() {
    bool result = setString(OS_Generator_FuelCell_StackCoolerFields::Coefficientr2, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellStackCooler_Impl::setCoefficientr3(double coefficientr3) {
    bool result = setDouble(OS_Generator_FuelCell_StackCoolerFields::Coefficientr3, coefficientr3);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellStackCooler_Impl::resetCoefficientr3() {
    bool result = setString(OS_Generator_FuelCell_StackCoolerFields::Coefficientr3, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellStackCooler_Impl::setStackCoolantFlowRate(double stackCoolantFlowRate) {
    bool result = setDouble(OS_Generator_FuelCell_StackCoolerFields::StackCoolantFlowRate, stackCoolantFlowRate);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellStackCooler_Impl::resetStackCoolantFlowRate() {
    bool result = setString(OS_Generator_FuelCell_StackCoolerFields::StackCoolantFlowRate, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellStackCooler_Impl::setStackCoolerUFactorTimesAreaValue(double stackCoolerUFactorTimesAreaValue) {
    bool result = setDouble(OS_Generator_FuelCell_StackCoolerFields::StackCoolerUFactorTimesAreaValue, stackCoolerUFactorTimesAreaValue);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellStackCooler_Impl::resetStackCoolerUFactorTimesAreaValue() {
    bool result = setString(OS_Generator_FuelCell_StackCoolerFields::StackCoolerUFactorTimesAreaValue, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellStackCooler_Impl::setFscogenAdjustmentFactor(double fscogenAdjustmentFactor) {
    bool result = setDouble(OS_Generator_FuelCell_StackCoolerFields::FscogenAdjustmentFactor, fscogenAdjustmentFactor);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellStackCooler_Impl::resetFscogenAdjustmentFactor() {
    bool result = setString(OS_Generator_FuelCell_StackCoolerFields::FscogenAdjustmentFactor, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellStackCooler_Impl::setStackCogenerationExchangerArea(double stackCogenerationExchangerArea) {
    bool result = setDouble(OS_Generator_FuelCell_StackCoolerFields::StackCogenerationExchangerArea, stackCogenerationExchangerArea);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellStackCooler_Impl::resetStackCogenerationExchangerArea() {
    bool result = setString(OS_Generator_FuelCell_StackCoolerFields::StackCogenerationExchangerArea, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellStackCooler_Impl::setStackCogenerationExchangerNominalFlowRate(double stackCogenerationExchangerNominalFlowRate) {
    bool result = setDouble(OS_Generator_FuelCell_StackCoolerFields::StackCogenerationExchangerNominalFlowRate, stackCogenerationExchangerNominalFlowRate);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellStackCooler_Impl::resetStackCogenerationExchangerNominalFlowRate() {
    bool result = setString(OS_Generator_FuelCell_StackCoolerFields::StackCogenerationExchangerNominalFlowRate, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellStackCooler_Impl::setStackCogenerationExchangerNominalHeatTransferCoefficient(double stackCogenerationExchangerNominalHeatTransferCoefficient) {
    bool result = setDouble(OS_Generator_FuelCell_StackCoolerFields::StackCogenerationExchangerNominalHeatTransferCoefficient, stackCogenerationExchangerNominalHeatTransferCoefficient);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellStackCooler_Impl::resetStackCogenerationExchangerNominalHeatTransferCoefficient() {
    bool result = setString(OS_Generator_FuelCell_StackCoolerFields::StackCogenerationExchangerNominalHeatTransferCoefficient, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellStackCooler_Impl::setStackCogenerationExchangerNominalHeatTransferCoefficientExponent(double stackCogenerationExchangerNominalHeatTransferCoefficientExponent) {
    bool result = setDouble(OS_Generator_FuelCell_StackCoolerFields::StackCogenerationExchangerNominalHeatTransferCoefficientExponent, stackCogenerationExchangerNominalHeatTransferCoefficientExponent);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellStackCooler_Impl::resetStackCogenerationExchangerNominalHeatTransferCoefficientExponent() {
    bool result = setString(OS_Generator_FuelCell_StackCoolerFields::StackCogenerationExchangerNominalHeatTransferCoefficientExponent, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellStackCooler_Impl::setStackCoolerPumpPower(double stackCoolerPumpPower) {
    bool result = setDouble(OS_Generator_FuelCell_StackCoolerFields::StackCoolerPumpPower, stackCoolerPumpPower);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellStackCooler_Impl::resetStackCoolerPumpPower() {
    bool result = setString(OS_Generator_FuelCell_StackCoolerFields::StackCoolerPumpPower, "");
    OS_ASSERT(result);
  }

  bool GeneratorFuelCellStackCooler_Impl::setStackCoolerPumpHeatLossFraction(double stackCoolerPumpHeatLossFraction) {
    bool result = setDouble(OS_Generator_FuelCell_StackCoolerFields::StackCoolerPumpHeatLossFraction, stackCoolerPumpHeatLossFraction);
    return result;
  }

  void GeneratorFuelCellStackCooler_Impl::resetStackCoolerPumpHeatLossFraction() {
    bool result = setString(OS_Generator_FuelCell_StackCoolerFields::StackCoolerPumpHeatLossFraction, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellStackCooler_Impl::setStackAirCoolerFanCoefficientf0(double stackAirCoolerFanCoefficientf0) {
    bool result = setDouble(OS_Generator_FuelCell_StackCoolerFields::StackAirCoolerFanCoefficientf0, stackAirCoolerFanCoefficientf0);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellStackCooler_Impl::resetStackAirCoolerFanCoefficientf0() {
    bool result = setString(OS_Generator_FuelCell_StackCoolerFields::StackAirCoolerFanCoefficientf0, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellStackCooler_Impl::setStackAirCoolerFanCoefficientf1(double stackAirCoolerFanCoefficientf1) {
    bool result = setDouble(OS_Generator_FuelCell_StackCoolerFields::StackAirCoolerFanCoefficientf1, stackAirCoolerFanCoefficientf1);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellStackCooler_Impl::resetStackAirCoolerFanCoefficientf1() {
    bool result = setString(OS_Generator_FuelCell_StackCoolerFields::StackAirCoolerFanCoefficientf1, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellStackCooler_Impl::setStackAirCoolerFanCoefficientf2(double stackAirCoolerFanCoefficientf2) {
    bool result = setDouble(OS_Generator_FuelCell_StackCoolerFields::StackAirCoolerFanCoefficientf2, stackAirCoolerFanCoefficientf2);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellStackCooler_Impl::resetStackAirCoolerFanCoefficientf2() {
    bool result = setString(OS_Generator_FuelCell_StackCoolerFields::StackAirCoolerFanCoefficientf2, "");
    OS_ASSERT(result);
  }

} // detail

GeneratorFuelCellStackCooler::GeneratorFuelCellStackCooler(const Model& model)
  : ModelObject(GeneratorFuelCellStackCooler::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::GeneratorFuelCellStackCooler_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
}

IddObjectType GeneratorFuelCellStackCooler::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Generator_FuelCell_StackCooler);
}

boost::optional<Connection> GeneratorFuelCellStackCooler::heatRecoveryWaterInletNode() const {
  return getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->heatRecoveryWaterInletNode();
}

boost::optional<Connection> GeneratorFuelCellStackCooler::heatRecoveryWaterOutletNode() const {
  return getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->heatRecoveryWaterOutletNode();
}

boost::optional<double> GeneratorFuelCellStackCooler::nominalStackTemperature() const {
  return getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->nominalStackTemperature();
}

boost::optional<double> GeneratorFuelCellStackCooler::actualStackTemperature() const {
  return getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->actualStackTemperature();
}

boost::optional<double> GeneratorFuelCellStackCooler::coefficientr0() const {
  return getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->coefficientr0();
}

boost::optional<double> GeneratorFuelCellStackCooler::coefficientr1() const {
  return getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->coefficientr1();
}

boost::optional<double> GeneratorFuelCellStackCooler::coefficientr2() const {
  return getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->coefficientr2();
}

boost::optional<double> GeneratorFuelCellStackCooler::coefficientr3() const {
  return getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->coefficientr3();
}

boost::optional<double> GeneratorFuelCellStackCooler::stackCoolantFlowRate() const {
  return getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->stackCoolantFlowRate();
}

boost::optional<double> GeneratorFuelCellStackCooler::stackCoolerUFactorTimesAreaValue() const {
  return getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->stackCoolerUFactorTimesAreaValue();
}

boost::optional<double> GeneratorFuelCellStackCooler::fscogenAdjustmentFactor() const {
  return getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->fscogenAdjustmentFactor();
}

boost::optional<double> GeneratorFuelCellStackCooler::stackCogenerationExchangerArea() const {
  return getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->stackCogenerationExchangerArea();
}

boost::optional<double> GeneratorFuelCellStackCooler::stackCogenerationExchangerNominalFlowRate() const {
  return getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->stackCogenerationExchangerNominalFlowRate();
}

boost::optional<double> GeneratorFuelCellStackCooler::stackCogenerationExchangerNominalHeatTransferCoefficient() const {
  return getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->stackCogenerationExchangerNominalHeatTransferCoefficient();
}

boost::optional<double> GeneratorFuelCellStackCooler::stackCogenerationExchangerNominalHeatTransferCoefficientExponent() const {
  return getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->stackCogenerationExchangerNominalHeatTransferCoefficientExponent();
}

boost::optional<double> GeneratorFuelCellStackCooler::stackCoolerPumpPower() const {
  return getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->stackCoolerPumpPower();
}

boost::optional<double> GeneratorFuelCellStackCooler::stackCoolerPumpHeatLossFraction() const {
  return getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->stackCoolerPumpHeatLossFraction();
}

boost::optional<double> GeneratorFuelCellStackCooler::stackAirCoolerFanCoefficientf0() const {
  return getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->stackAirCoolerFanCoefficientf0();
}

boost::optional<double> GeneratorFuelCellStackCooler::stackAirCoolerFanCoefficientf1() const {
  return getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->stackAirCoolerFanCoefficientf1();
}

boost::optional<double> GeneratorFuelCellStackCooler::stackAirCoolerFanCoefficientf2() const {
  return getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->stackAirCoolerFanCoefficientf2();
}

bool GeneratorFuelCellStackCooler::setHeatRecoveryWaterInletNode(const Connection& connection) {
  return getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->setHeatRecoveryWaterInletNode(connection);
}

void GeneratorFuelCellStackCooler::resetHeatRecoveryWaterInletNode() {
  getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->resetHeatRecoveryWaterInletNode();
}

bool GeneratorFuelCellStackCooler::setHeatRecoveryWaterOutletNode(const Connection& connection) {
  return getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->setHeatRecoveryWaterOutletNode(connection);
}

void GeneratorFuelCellStackCooler::resetHeatRecoveryWaterOutletNode() {
  getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->resetHeatRecoveryWaterOutletNode();
}

void GeneratorFuelCellStackCooler::setNominalStackTemperature(double nominalStackTemperature) {
  getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->setNominalStackTemperature(nominalStackTemperature);
}

void GeneratorFuelCellStackCooler::resetNominalStackTemperature() {
  getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->resetNominalStackTemperature();
}

void GeneratorFuelCellStackCooler::setActualStackTemperature(double actualStackTemperature) {
  getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->setActualStackTemperature(actualStackTemperature);
}

void GeneratorFuelCellStackCooler::resetActualStackTemperature() {
  getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->resetActualStackTemperature();
}

void GeneratorFuelCellStackCooler::setCoefficientr0(double coefficientr0) {
  getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->setCoefficientr0(coefficientr0);
}

void GeneratorFuelCellStackCooler::resetCoefficientr0() {
  getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->resetCoefficientr0();
}

void GeneratorFuelCellStackCooler::setCoefficientr1(double coefficientr1) {
  getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->setCoefficientr1(coefficientr1);
}

void GeneratorFuelCellStackCooler::resetCoefficientr1() {
  getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->resetCoefficientr1();
}

void GeneratorFuelCellStackCooler::setCoefficientr2(double coefficientr2) {
  getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->setCoefficientr2(coefficientr2);
}

void GeneratorFuelCellStackCooler::resetCoefficientr2() {
  getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->resetCoefficientr2();
}

void GeneratorFuelCellStackCooler::setCoefficientr3(double coefficientr3) {
  getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->setCoefficientr3(coefficientr3);
}

void GeneratorFuelCellStackCooler::resetCoefficientr3() {
  getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->resetCoefficientr3();
}

void GeneratorFuelCellStackCooler::setStackCoolantFlowRate(double stackCoolantFlowRate) {
  getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->setStackCoolantFlowRate(stackCoolantFlowRate);
}

void GeneratorFuelCellStackCooler::resetStackCoolantFlowRate() {
  getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->resetStackCoolantFlowRate();
}

void GeneratorFuelCellStackCooler::setStackCoolerUFactorTimesAreaValue(double stackCoolerUFactorTimesAreaValue) {
  getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->setStackCoolerUFactorTimesAreaValue(stackCoolerUFactorTimesAreaValue);
}

void GeneratorFuelCellStackCooler::resetStackCoolerUFactorTimesAreaValue() {
  getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->resetStackCoolerUFactorTimesAreaValue();
}

void GeneratorFuelCellStackCooler::setFscogenAdjustmentFactor(double fscogenAdjustmentFactor) {
  getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->setFscogenAdjustmentFactor(fscogenAdjustmentFactor);
}

void GeneratorFuelCellStackCooler::resetFscogenAdjustmentFactor() {
  getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->resetFscogenAdjustmentFactor();
}

void GeneratorFuelCellStackCooler::setStackCogenerationExchangerArea(double stackCogenerationExchangerArea) {
  getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->setStackCogenerationExchangerArea(stackCogenerationExchangerArea);
}

void GeneratorFuelCellStackCooler::resetStackCogenerationExchangerArea() {
  getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->resetStackCogenerationExchangerArea();
}

void GeneratorFuelCellStackCooler::setStackCogenerationExchangerNominalFlowRate(double stackCogenerationExchangerNominalFlowRate) {
  getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->setStackCogenerationExchangerNominalFlowRate(stackCogenerationExchangerNominalFlowRate);
}

void GeneratorFuelCellStackCooler::resetStackCogenerationExchangerNominalFlowRate() {
  getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->resetStackCogenerationExchangerNominalFlowRate();
}

void GeneratorFuelCellStackCooler::setStackCogenerationExchangerNominalHeatTransferCoefficient(double stackCogenerationExchangerNominalHeatTransferCoefficient) {
  getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->setStackCogenerationExchangerNominalHeatTransferCoefficient(stackCogenerationExchangerNominalHeatTransferCoefficient);
}

void GeneratorFuelCellStackCooler::resetStackCogenerationExchangerNominalHeatTransferCoefficient() {
  getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->resetStackCogenerationExchangerNominalHeatTransferCoefficient();
}

void GeneratorFuelCellStackCooler::setStackCogenerationExchangerNominalHeatTransferCoefficientExponent(double stackCogenerationExchangerNominalHeatTransferCoefficientExponent) {
  getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->setStackCogenerationExchangerNominalHeatTransferCoefficientExponent(stackCogenerationExchangerNominalHeatTransferCoefficientExponent);
}

void GeneratorFuelCellStackCooler::resetStackCogenerationExchangerNominalHeatTransferCoefficientExponent() {
  getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->resetStackCogenerationExchangerNominalHeatTransferCoefficientExponent();
}

void GeneratorFuelCellStackCooler::setStackCoolerPumpPower(double stackCoolerPumpPower) {
  getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->setStackCoolerPumpPower(stackCoolerPumpPower);
}

void GeneratorFuelCellStackCooler::resetStackCoolerPumpPower() {
  getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->resetStackCoolerPumpPower();
}

bool GeneratorFuelCellStackCooler::setStackCoolerPumpHeatLossFraction(double stackCoolerPumpHeatLossFraction) {
  return getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->setStackCoolerPumpHeatLossFraction(stackCoolerPumpHeatLossFraction);
}

void GeneratorFuelCellStackCooler::resetStackCoolerPumpHeatLossFraction() {
  getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->resetStackCoolerPumpHeatLossFraction();
}

void GeneratorFuelCellStackCooler::setStackAirCoolerFanCoefficientf0(double stackAirCoolerFanCoefficientf0) {
  getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->setStackAirCoolerFanCoefficientf0(stackAirCoolerFanCoefficientf0);
}

void GeneratorFuelCellStackCooler::resetStackAirCoolerFanCoefficientf0() {
  getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->resetStackAirCoolerFanCoefficientf0();
}

void GeneratorFuelCellStackCooler::setStackAirCoolerFanCoefficientf1(double stackAirCoolerFanCoefficientf1) {
  getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->setStackAirCoolerFanCoefficientf1(stackAirCoolerFanCoefficientf1);
}

void GeneratorFuelCellStackCooler::resetStackAirCoolerFanCoefficientf1() {
  getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->resetStackAirCoolerFanCoefficientf1();
}

void GeneratorFuelCellStackCooler::setStackAirCoolerFanCoefficientf2(double stackAirCoolerFanCoefficientf2) {
  getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->setStackAirCoolerFanCoefficientf2(stackAirCoolerFanCoefficientf2);
}

void GeneratorFuelCellStackCooler::resetStackAirCoolerFanCoefficientf2() {
  getImpl<detail::GeneratorFuelCellStackCooler_Impl>()->resetStackAirCoolerFanCoefficientf2();
}

/// @cond
GeneratorFuelCellStackCooler::GeneratorFuelCellStackCooler(std::shared_ptr<detail::GeneratorFuelCellStackCooler_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

