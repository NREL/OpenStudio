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

#include "Gas.hpp"
#include "Gas_Impl.hpp"

#include "../utilities/idf/ValidityReport.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_WindowMaterial_Gas_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  Gas_Impl::Gas_Impl(const IdfObject& idfObject,
                     Model_Impl* model,
                     bool keepHandle)
    : GasLayer_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == Gas::iddObjectType());
  }

  Gas_Impl::Gas_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                     Model_Impl* model,
                     bool keepHandle)
    : GasLayer_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == Gas::iddObjectType());
  }

  Gas_Impl::Gas_Impl(const Gas_Impl& other,
                     Model_Impl* model,
                     bool keepHandle)
    : GasLayer_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& Gas_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType Gas_Impl::iddObjectType() const {
    return Gas::iddObjectType();
  }

  std::string Gas_Impl::gasType() const {
    boost::optional<std::string> value = getString(OS_WindowMaterial_GasFields::GasType,true);
    OS_ASSERT(value);
    return value.get();
  }

  double Gas_Impl::thickness() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_GasFields::Thickness,true);
    OS_ASSERT(value);
    return value.get();
  }

  double Gas_Impl::getThermalConductivity(double temperature) const {
    std::string gasType = this->gasType();
    std::vector<double> coeffs;
    if (openstudio::istringEqual(gasType,"Air")) {
      coeffs = FenestrationMaterial::airThermalConductivityCoefficients();
    }
    else if (openstudio::istringEqual(gasType,"Argon")) {
      coeffs = FenestrationMaterial::argonThermalConductivityCoefficients();
    }
    else if (openstudio::istringEqual(gasType,"Krypton")) {
      coeffs = FenestrationMaterial::kryptonThermalConductivityCoefficients();
    }
    else if (openstudio::istringEqual(gasType,"Xenon")) {
      coeffs = FenestrationMaterial::xenonThermalConductivityCoefficients();
    }
    else if (openstudio::istringEqual(gasType,"Custom")) {
      OptionalDouble A = customConductivityCoefficientA();
      OptionalDouble B = customConductivityCoefficientB();
      OptionalDouble C = customConductivityCoefficientC();
      if (!(A && B && C)) {
        LOG_AND_THROW("Model" << briefDescription() << " has gasType == 'Custom', but no "
          << "conductivity coefficients set. Cannot calculate thermal conductivity.");
      }
      coeffs[0] = *A;
      coeffs[1] = *B;
      coeffs[2] = *C;
    }
    else {
      LOG_AND_THROW("Unknown gasType listed in " << briefDescription() << ".");
    }

    return coeffs[0] + (coeffs[1] * temperature) + (coeffs[2] * ::pow(temperature,2));
  }

  double Gas_Impl::getThermalConductance(double temperature) const {
    return getThermalConductivity(temperature)/thickness();
  }

  double Gas_Impl::getThermalResistivity(double temperature) const {
    return 1.0/getThermalConductivity(temperature);
  }

  double Gas_Impl::getThermalResistance(double temperature) const {
    return 1.0/getThermalConductance(temperature);
  }

  double Gas_Impl::getViscosity(double temperature) const {
    std::string gasType = this->gasType();
    std::vector<double> coeffs(3);
    if (openstudio::istringEqual(gasType,"Air")) {
      coeffs = FenestrationMaterial::airViscosityCoefficients();
    }
    else if (openstudio::istringEqual(gasType,"Argon")) {
      coeffs = FenestrationMaterial::argonViscosityCoefficients();
    }
    else if (openstudio::istringEqual(gasType,"Krypton")) {
      coeffs = FenestrationMaterial::kryptonViscosityCoefficients();
    }
    else if (openstudio::istringEqual(gasType,"Xenon")) {
      coeffs = FenestrationMaterial::xenonViscosityCoefficients();
    }
    else if (openstudio::istringEqual(gasType,"Custom")) {
      OptionalDouble A = customViscosityCoefficientA();
      OptionalDouble B = customViscosityCoefficientB();
      OptionalDouble C = customViscosityCoefficientC();
      if (!(A && B && C)) {
        LOG_AND_THROW("Gas " << briefDescription() << " has gasType == 'Custom', but no "
          << "viscosity coefficients set. Cannot calculate viscosity.");
      }
      coeffs[0] = *A;
      coeffs[1] = *B;
      coeffs[2] = *C;
    }
    else {
      LOG_AND_THROW("Unknown gasType listed in " << briefDescription() << ".");
    }

    return coeffs[0] + (coeffs[1]* temperature) + (coeffs[2] * ::pow(temperature,2));
  }

  double Gas_Impl::getSpecificHeat(double temperature) const {
    std::string gasType = this->gasType();
    std::vector<double> coeffs;
    if (openstudio::istringEqual(gasType,"Air")) {
      coeffs = FenestrationMaterial::airSpecificHeatCoefficients();
    }
    else if (openstudio::istringEqual(gasType,"Argon")) {
      coeffs = FenestrationMaterial::argonSpecificHeatCoefficients();
    }
    else if (openstudio::istringEqual(gasType,"Krypton")) {
      coeffs = FenestrationMaterial::kryptonSpecificHeatCoefficients();
    }
    else if (openstudio::istringEqual(gasType,"Xenon")) {
      coeffs = FenestrationMaterial::xenonSpecificHeatCoefficients();
    }
    else if (openstudio::istringEqual(gasType,"Custom")) {
      OptionalDouble A = customSpecificHeatCoefficientA();
      OptionalDouble B = customSpecificHeatCoefficientB();
      OptionalDouble C = customSpecificHeatCoefficientC();
      if (!(A && B && C)) {
        LOG_AND_THROW("Model" << briefDescription() << " has gasType == 'Custom', but no "
          << "specific heat coefficients set. Cannot calculate specific heat.");
      }
      coeffs[0] = *A;
      coeffs[1] = *B;
      coeffs[2] = *C;
    }
    else {
      LOG_AND_THROW("Unknown gasType listed in " << briefDescription() << ".");
    }

    return coeffs[0] + (coeffs[1] * temperature) + (coeffs[2] * ::pow(temperature,2));

  }

  boost::optional<double> Gas_Impl::customConductivityCoefficientA() const {
    return getDouble(OS_WindowMaterial_GasFields::ConductivityCoefficientA,true);
  }

  boost::optional<double> Gas_Impl::customConductivityCoefficientB() const {
    return getDouble(OS_WindowMaterial_GasFields::ConductivityCoefficientB,true);
  }

  boost::optional<double> Gas_Impl::customConductivityCoefficientC() const {
    return getDouble(OS_WindowMaterial_GasFields::ConductivityCoefficientC,true);
  }

  boost::optional<double> Gas_Impl::customViscosityCoefficientA() const {
    return getDouble(OS_WindowMaterial_GasFields::ViscosityCoefficientA,true);
  }

  boost::optional<double> Gas_Impl::customViscosityCoefficientB() const {
    return getDouble(OS_WindowMaterial_GasFields::ViscosityCoefficientB,true);
  }

  boost::optional<double> Gas_Impl::customViscosityCoefficientC() const {
    return getDouble(OS_WindowMaterial_GasFields::ViscosityCoefficientC,true);
  }

  boost::optional<double> Gas_Impl::customSpecificHeatCoefficientA() const {
    return getDouble(OS_WindowMaterial_GasFields::SpecificHeatCoefficientA,true);
  }

  boost::optional<double> Gas_Impl::customSpecificHeatCoefficientB() const {
    return getDouble(OS_WindowMaterial_GasFields::SpecificHeatCoefficientB,true);
  }

  boost::optional<double> Gas_Impl::customSpecificHeatCoefficientC() const {
    return getDouble(OS_WindowMaterial_GasFields::SpecificHeatCoefficientC,true);
  }

  boost::optional<double> Gas_Impl::customMolecularWeight() const {
    return getDouble(OS_WindowMaterial_GasFields::MolecularWeight,true);
  }

  bool Gas_Impl::setCustomConductivity(double A, double B, double C) {

    OptionalDouble oldAValue = customConductivityCoefficientA();
    bool result = setDouble(OS_WindowMaterial_GasFields::ConductivityCoefficientA,A);
    if (!result) { return false; }

    result = setDouble(OS_WindowMaterial_GasFields::ConductivityCoefficientB,B);
    if (!result) {
      // reset A to previous value
      if (oldAValue) { setDouble(OS_WindowMaterial_GasFields::ConductivityCoefficientA,*oldAValue); }
      else { setString(OS_WindowMaterial_GasFields::ConductivityCoefficientA,""); }
    }

    setDouble(OS_WindowMaterial_GasFields::ConductivityCoefficientC,C);

    return result;
  }

  bool Gas_Impl::setCustomViscosity(double A, double B, double C) {
    OptionalDouble oldAValue = customViscosityCoefficientA();
    bool result = setDouble(OS_WindowMaterial_GasFields::ViscosityCoefficientA,A);
    if (!result) { return false; }

    result = setDouble(OS_WindowMaterial_GasFields::ViscosityCoefficientB,B);
    if (!result) {
      // reset A to previous value
      if (oldAValue) { setDouble(OS_WindowMaterial_GasFields::ViscosityCoefficientA,*oldAValue); }
      else { setString(OS_WindowMaterial_GasFields::ViscosityCoefficientA,""); }
    }

    setDouble(OS_WindowMaterial_GasFields::ViscosityCoefficientC,C);

    return result;
  }

  bool Gas_Impl::setCustomSpecificHeat(double A, double B, double C) {
    OptionalDouble oldAValue = customSpecificHeatCoefficientA();
    bool result = setDouble(OS_WindowMaterial_GasFields::SpecificHeatCoefficientA,A);
    if (!result) { return false; }

    result = setDouble(OS_WindowMaterial_GasFields::SpecificHeatCoefficientB,B);
    if (!result) {
      // reset A to previous value
      if (oldAValue) { setDouble(OS_WindowMaterial_GasFields::SpecificHeatCoefficientA,*oldAValue); }
      else { setString(OS_WindowMaterial_GasFields::SpecificHeatCoefficientA,""); }
    }

    setDouble(OS_WindowMaterial_GasFields::SpecificHeatCoefficientC,C);

    return result;
  }

  bool Gas_Impl::setCustomMolecularWeight(double value) {
    return setDouble(OS_WindowMaterial_GasFields::MolecularWeight,value);
  }

  Quantity Gas_Impl::getThickness(bool returnIP) const {
    OptionalDouble value = thickness();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_GasFields::Thickness, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  boost::optional<double> Gas_Impl::conductivityCoefficientA() const {
    return getDouble(OS_WindowMaterial_GasFields::ConductivityCoefficientA,true);
  }

  OSOptionalQuantity Gas_Impl::getConductivityCoefficientA(bool returnIP) const {
    OptionalDouble value = conductivityCoefficientA();
    return getQuantityFromDouble(OS_WindowMaterial_GasFields::ConductivityCoefficientA, value, returnIP);
  }

  boost::optional<double> Gas_Impl::conductivityCoefficientB() const {
    return getDouble(OS_WindowMaterial_GasFields::ConductivityCoefficientB,true);
  }

  OSOptionalQuantity Gas_Impl::getConductivityCoefficientB(bool returnIP) const {
    OptionalDouble value = conductivityCoefficientB();
    return getQuantityFromDouble(OS_WindowMaterial_GasFields::ConductivityCoefficientB, value, returnIP);
  }

  boost::optional<double> Gas_Impl::conductivityCoefficientC() const {
    return getDouble(OS_WindowMaterial_GasFields::ConductivityCoefficientC,true);
  }

  OSOptionalQuantity Gas_Impl::getConductivityCoefficientC(bool returnIP) const {
    OptionalDouble value = conductivityCoefficientC();
    return getQuantityFromDouble(OS_WindowMaterial_GasFields::ConductivityCoefficientC, value, returnIP);
  }

  boost::optional<double> Gas_Impl::viscosityCoefficientA() const {
    return getDouble(OS_WindowMaterial_GasFields::ViscosityCoefficientA,true);
  }

  OSOptionalQuantity Gas_Impl::getViscosityCoefficientA(bool returnIP) const {
    OptionalDouble value = viscosityCoefficientA();
    return getQuantityFromDouble(OS_WindowMaterial_GasFields::ViscosityCoefficientA, value, returnIP);
  }

  boost::optional<double> Gas_Impl::viscosityCoefficientB() const {
    return getDouble(OS_WindowMaterial_GasFields::ViscosityCoefficientB,true);
  }

  OSOptionalQuantity Gas_Impl::getViscosityCoefficientB(bool returnIP) const {
    OptionalDouble value = viscosityCoefficientB();
    return getQuantityFromDouble(OS_WindowMaterial_GasFields::ViscosityCoefficientB, value, returnIP);
  }

  boost::optional<double> Gas_Impl::viscosityCoefficientC() const {
    return getDouble(OS_WindowMaterial_GasFields::ViscosityCoefficientC,true);
  }

  OSOptionalQuantity Gas_Impl::getViscosityCoefficientC(bool returnIP) const {
    OptionalDouble value = viscosityCoefficientC();
    return getQuantityFromDouble(OS_WindowMaterial_GasFields::ViscosityCoefficientC, value, returnIP);
  }

  boost::optional<double> Gas_Impl::specificHeatCoefficientA() const {
    return getDouble(OS_WindowMaterial_GasFields::SpecificHeatCoefficientA,true);
  }

  OSOptionalQuantity Gas_Impl::getSpecificHeatCoefficientA(bool returnIP) const {
    OptionalDouble value = specificHeatCoefficientA();
    return getQuantityFromDouble(OS_WindowMaterial_GasFields::SpecificHeatCoefficientA, value, returnIP);
  }

  boost::optional<double> Gas_Impl::specificHeatCoefficientB() const {
    return getDouble(OS_WindowMaterial_GasFields::SpecificHeatCoefficientB,true);
  }

  OSOptionalQuantity Gas_Impl::getSpecificHeatCoefficientB(bool returnIP) const {
    OptionalDouble value = specificHeatCoefficientB();
    return getQuantityFromDouble(OS_WindowMaterial_GasFields::SpecificHeatCoefficientB, value, returnIP);
  }

  boost::optional<double> Gas_Impl::specificHeatCoefficientC() const {
    return getDouble(OS_WindowMaterial_GasFields::SpecificHeatCoefficientC,true);
  }

  OSOptionalQuantity Gas_Impl::getSpecificHeatCoefficientC(bool returnIP) const {
    OptionalDouble value = specificHeatCoefficientC();
    return getQuantityFromDouble(OS_WindowMaterial_GasFields::SpecificHeatCoefficientC, value, returnIP);
  }

  boost::optional<double> Gas_Impl::molecularWeight() const {
    return getDouble(OS_WindowMaterial_GasFields::MolecularWeight,true);
  }

  OSOptionalQuantity Gas_Impl::getMolecularWeight(bool returnIP) const {
    OptionalDouble value = molecularWeight();
    return getQuantityFromDouble(OS_WindowMaterial_GasFields::MolecularWeight, value, returnIP);
  }

  boost::optional<double> Gas_Impl::specificHeatRatio() const {
    return getDouble(OS_WindowMaterial_GasFields::SpecificHeatRatio,true);
  }

  OSOptionalQuantity Gas_Impl::getSpecificHeatRatio(bool returnIP) const {
    OptionalDouble value = specificHeatRatio();
    return getQuantityFromDouble(OS_WindowMaterial_GasFields::SpecificHeatRatio, value, returnIP);
  }

  bool Gas_Impl::setGasType(std::string gasType) {
    bool result = setString(OS_WindowMaterial_GasFields::GasType, gasType);
    return result;
  }

  bool Gas_Impl::setThickness(double thickness) {
    bool result = setDouble(OS_WindowMaterial_GasFields::Thickness, thickness);
    return result;
  }

  bool Gas_Impl::setThickness(const Quantity& thickness) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_GasFields::Thickness,thickness);
    if (!value) {
      return false;
    }
    return setThickness(value.get());
  }

  void Gas_Impl::setConductivityCoefficientA(boost::optional<double> conductivityCoefficientA) {
    bool result(false);
    if (conductivityCoefficientA) {
      result = setDouble(OS_WindowMaterial_GasFields::ConductivityCoefficientA, conductivityCoefficientA.get());
    }
    else {
      resetConductivityCoefficientA();
      result = true;
    }
    OS_ASSERT(result);
  }

  bool Gas_Impl::setConductivityCoefficientA(const OSOptionalQuantity& conductivityCoefficientA) {
    bool result(false);
    OptionalDouble value;
    if (conductivityCoefficientA.isSet()) {
      value = getDoubleFromQuantity(OS_WindowMaterial_GasFields::ConductivityCoefficientA,conductivityCoefficientA.get());
      if (value) {
        setConductivityCoefficientA(value);
        result = true;
      }
    }
    else {
      setConductivityCoefficientA(value);
      result = true;
    }
    return result;
  }

  void Gas_Impl::resetConductivityCoefficientA() {
    bool result = setString(OS_WindowMaterial_GasFields::ConductivityCoefficientA, "");
    OS_ASSERT(result);
  }

  void Gas_Impl::setConductivityCoefficientB(boost::optional<double> conductivityCoefficientB) {
    bool result(false);
    if (conductivityCoefficientB) {
      result = setDouble(OS_WindowMaterial_GasFields::ConductivityCoefficientB, conductivityCoefficientB.get());
    }
    else {
      resetConductivityCoefficientB();
      result = true;
    }
    OS_ASSERT(result);
  }

  bool Gas_Impl::setConductivityCoefficientB(const OSOptionalQuantity& conductivityCoefficientB) {
    bool result(false);
    OptionalDouble value;
    if (conductivityCoefficientB.isSet()) {
      value = getDoubleFromQuantity(OS_WindowMaterial_GasFields::ConductivityCoefficientB,conductivityCoefficientB.get());
      if (value) {
        setConductivityCoefficientB(value);
        result = true;
      }
    }
    else {
      setConductivityCoefficientB(value);
      result = true;
    }
    return result;
  }

  void Gas_Impl::resetConductivityCoefficientB() {
    bool result = setString(OS_WindowMaterial_GasFields::ConductivityCoefficientB, "");
    OS_ASSERT(result);
  }

  void Gas_Impl::setConductivityCoefficientC(boost::optional<double> conductivityCoefficientC) {
    bool result(false);
    if (conductivityCoefficientC) {
      result = setDouble(OS_WindowMaterial_GasFields::ConductivityCoefficientC, conductivityCoefficientC.get());
    }
    else {
      resetConductivityCoefficientC();
      result = true;
    }
    OS_ASSERT(result);
  }

  bool Gas_Impl::setConductivityCoefficientC(const OSOptionalQuantity& conductivityCoefficientC) {
    bool result(false);
    OptionalDouble value;
    if (conductivityCoefficientC.isSet()) {
      value = getDoubleFromQuantity(OS_WindowMaterial_GasFields::ConductivityCoefficientC,conductivityCoefficientC.get());
      if (value) {
        setConductivityCoefficientC(value);
        result = true;
      }
    }
    else {
      setConductivityCoefficientC(value);
      result = true;
    }
    return result;
  }

  void Gas_Impl::resetConductivityCoefficientC() {
    bool result = setString(OS_WindowMaterial_GasFields::ConductivityCoefficientC, "");
    OS_ASSERT(result);
  }

  bool Gas_Impl::setViscosityCoefficientA(boost::optional<double> viscosityCoefficientA) {
    bool result(false);
    if (viscosityCoefficientA) {
      result = setDouble(OS_WindowMaterial_GasFields::ViscosityCoefficientA, viscosityCoefficientA.get());
    }
    else {
      resetViscosityCoefficientA();
      result = true;
    }
    return result;
  }

  bool Gas_Impl::setViscosityCoefficientA(const OSOptionalQuantity& viscosityCoefficientA) {
    bool result(false);
    OptionalDouble value;
    if (viscosityCoefficientA.isSet()) {
      value = getDoubleFromQuantity(OS_WindowMaterial_GasFields::ViscosityCoefficientA,viscosityCoefficientA.get());
      if (value) {
        result = setViscosityCoefficientA(value);
      }
    }
    else {
      result = setViscosityCoefficientA(value);
    }
    return result;
  }

  void Gas_Impl::resetViscosityCoefficientA() {
    bool result = setString(OS_WindowMaterial_GasFields::ViscosityCoefficientA, "");
    OS_ASSERT(result);
  }

  void Gas_Impl::setViscosityCoefficientB(boost::optional<double> viscosityCoefficientB) {
    bool result(false);
    if (viscosityCoefficientB) {
      result = setDouble(OS_WindowMaterial_GasFields::ViscosityCoefficientB, viscosityCoefficientB.get());
    }
    else {
      resetViscosityCoefficientB();
      result = true;
    }
    OS_ASSERT(result);
  }

  bool Gas_Impl::setViscosityCoefficientB(const OSOptionalQuantity& viscosityCoefficientB) {
    bool result(false);
    OptionalDouble value;
    if (viscosityCoefficientB.isSet()) {
      value = getDoubleFromQuantity(OS_WindowMaterial_GasFields::ViscosityCoefficientB,viscosityCoefficientB.get());
      if (value) {
        setViscosityCoefficientB(value);
        result = true;
      }
    }
    else {
      setViscosityCoefficientB(value);
      result = true;
    }
    return result;
  }

  void Gas_Impl::resetViscosityCoefficientB() {
    bool result = setString(OS_WindowMaterial_GasFields::ViscosityCoefficientB, "");
    OS_ASSERT(result);
  }

  void Gas_Impl::setViscosityCoefficientC(boost::optional<double> viscosityCoefficientC) {
    bool result(false);
    if (viscosityCoefficientC) {
      result = setDouble(OS_WindowMaterial_GasFields::ViscosityCoefficientC, viscosityCoefficientC.get());
    }
    else {
      resetViscosityCoefficientC();
      result = true;
    }
    OS_ASSERT(result);
  }

  bool Gas_Impl::setViscosityCoefficientC(const OSOptionalQuantity& viscosityCoefficientC) {
    bool result(false);
    OptionalDouble value;
    if (viscosityCoefficientC.isSet()) {
      value = getDoubleFromQuantity(OS_WindowMaterial_GasFields::ViscosityCoefficientC,viscosityCoefficientC.get());
      if (value) {
        setViscosityCoefficientC(value);
        result = true;
      }
    }
    else {
      setViscosityCoefficientC(value);
      result = true;
    }
    return result;
  }

  void Gas_Impl::resetViscosityCoefficientC() {
    bool result = setString(OS_WindowMaterial_GasFields::ViscosityCoefficientC, "");
    OS_ASSERT(result);
  }

  bool Gas_Impl::setSpecificHeatCoefficientA(boost::optional<double> specificHeatCoefficientA) {
    bool result(false);
    if (specificHeatCoefficientA) {
      result = setDouble(OS_WindowMaterial_GasFields::SpecificHeatCoefficientA, specificHeatCoefficientA.get());
    }
    else {
      resetSpecificHeatCoefficientA();
      result = true;
    }
    return result;
  }

  bool Gas_Impl::setSpecificHeatCoefficientA(const OSOptionalQuantity& specificHeatCoefficientA) {
    bool result(false);
    OptionalDouble value;
    if (specificHeatCoefficientA.isSet()) {
      value = getDoubleFromQuantity(OS_WindowMaterial_GasFields::SpecificHeatCoefficientA,specificHeatCoefficientA.get());
      if (value) {
        result = setSpecificHeatCoefficientA(value);
      }
    }
    else {
      result = setSpecificHeatCoefficientA(value);
    }
    return result;
  }

  void Gas_Impl::resetSpecificHeatCoefficientA() {
    bool result = setString(OS_WindowMaterial_GasFields::SpecificHeatCoefficientA, "");
    OS_ASSERT(result);
  }

  void Gas_Impl::setSpecificHeatCoefficientB(boost::optional<double> specificHeatCoefficientB) {
    bool result(false);
    if (specificHeatCoefficientB) {
      result = setDouble(OS_WindowMaterial_GasFields::SpecificHeatCoefficientB, specificHeatCoefficientB.get());
    }
    else {
      resetSpecificHeatCoefficientB();
      result = true;
    }
    OS_ASSERT(result);
  }

  bool Gas_Impl::setSpecificHeatCoefficientB(const OSOptionalQuantity& specificHeatCoefficientB) {
    bool result(false);
    OptionalDouble value;
    if (specificHeatCoefficientB.isSet()) {
      value = getDoubleFromQuantity(OS_WindowMaterial_GasFields::SpecificHeatCoefficientB,specificHeatCoefficientB.get());
      if (value) {
        setSpecificHeatCoefficientB(value);
        result = true;
      }
    }
    else {
      setSpecificHeatCoefficientB(value);
      result = true;
    }
    return result;
  }

  void Gas_Impl::resetSpecificHeatCoefficientB() {
    bool result = setString(OS_WindowMaterial_GasFields::SpecificHeatCoefficientB, "");
    OS_ASSERT(result);
  }

  void Gas_Impl::setSpecificHeatCoefficientC(boost::optional<double> specificHeatCoefficientC) {
    bool result(false);
    if (specificHeatCoefficientC) {
      result = setDouble(OS_WindowMaterial_GasFields::SpecificHeatCoefficientC, specificHeatCoefficientC.get());
    }
    else {
      resetSpecificHeatCoefficientC();
      result = true;
    }
    OS_ASSERT(result);
  }

  bool Gas_Impl::setSpecificHeatCoefficientC(const OSOptionalQuantity& specificHeatCoefficientC) {
    bool result(false);
    OptionalDouble value;
    if (specificHeatCoefficientC.isSet()) {
      value = getDoubleFromQuantity(OS_WindowMaterial_GasFields::SpecificHeatCoefficientC,specificHeatCoefficientC.get());
      if (value) {
        setSpecificHeatCoefficientC(value);
        result = true;
      }
    }
    else {
      setSpecificHeatCoefficientC(value);
      result = true;
    }
    return result;
  }

  void Gas_Impl::resetSpecificHeatCoefficientC() {
    bool result = setString(OS_WindowMaterial_GasFields::SpecificHeatCoefficientC, "");
    OS_ASSERT(result);
  }

  bool Gas_Impl::setMolecularWeight(boost::optional<double> molecularWeight) {
    bool result(false);
    if (molecularWeight) {
      result = setDouble(OS_WindowMaterial_GasFields::MolecularWeight, molecularWeight.get());
    }
    else {
      resetMolecularWeight();
      result = true;
    }
    return result;
  }

  bool Gas_Impl::setMolecularWeight(const OSOptionalQuantity& molecularWeight) {
    bool result(false);
    OptionalDouble value;
    if (molecularWeight.isSet()) {
      value = getDoubleFromQuantity(OS_WindowMaterial_GasFields::MolecularWeight,molecularWeight.get());
      if (value) {
        result = setMolecularWeight(value);
      }
    }
    else {
      result = setMolecularWeight(value);
    }
    return result;
  }

  void Gas_Impl::resetMolecularWeight() {
    bool result = setString(OS_WindowMaterial_GasFields::MolecularWeight, "");
    OS_ASSERT(result);
  }

  bool Gas_Impl::setSpecificHeatRatio(boost::optional<double> specificHeatRatio) {
    bool result(false);
    if (specificHeatRatio) {
      result = setDouble(OS_WindowMaterial_GasFields::SpecificHeatRatio, specificHeatRatio.get());
    }
    else {
      resetSpecificHeatRatio();
      result = true;
    }
    return result;
  }

  bool Gas_Impl::setSpecificHeatRatio(const OSOptionalQuantity& specificHeatRatio) {
    bool result(false);
    OptionalDouble value;
    if (specificHeatRatio.isSet()) {
      value = getDoubleFromQuantity(OS_WindowMaterial_GasFields::SpecificHeatRatio,specificHeatRatio.get());
      if (value) {
        result = setSpecificHeatRatio(value);
      }
    }
    else {
      result = setSpecificHeatRatio(value);
    }
    return result;
  }

  void Gas_Impl::resetSpecificHeatRatio() {
    bool result = setString(OS_WindowMaterial_GasFields::SpecificHeatRatio, "");
    OS_ASSERT(result);
  }

  std::vector<std::string> Gas_Impl::gasTypeValues() const {
    return Gas::gasTypeValues();
  }

  openstudio::Quantity Gas_Impl::thickness_SI() const {
    return getThickness(false);
  }

  openstudio::Quantity Gas_Impl::thickness_IP() const {
    return getThickness(true);
  }

  openstudio::OSOptionalQuantity Gas_Impl::conductivityCoefficientA_SI() const {
    return getConductivityCoefficientA(false);
  }

  openstudio::OSOptionalQuantity Gas_Impl::conductivityCoefficientA_IP() const {
    return getConductivityCoefficientA(true);
  }

  openstudio::OSOptionalQuantity Gas_Impl::conductivityCoefficientB_SI() const {
    return getConductivityCoefficientB(false);
  }

  openstudio::OSOptionalQuantity Gas_Impl::conductivityCoefficientB_IP() const {
    return getConductivityCoefficientB(true);
  }

  openstudio::OSOptionalQuantity Gas_Impl::conductivityCoefficientC_SI() const {
    return getConductivityCoefficientC(false);
  }

  openstudio::OSOptionalQuantity Gas_Impl::conductivityCoefficientC_IP() const {
    return getConductivityCoefficientC(true);
  }

  openstudio::OSOptionalQuantity Gas_Impl::viscosityCoefficientA_SI() const {
    return getViscosityCoefficientA(false);
  }

  openstudio::OSOptionalQuantity Gas_Impl::viscosityCoefficientA_IP() const {
    return getViscosityCoefficientA(true);
  }

  openstudio::OSOptionalQuantity Gas_Impl::viscosityCoefficientB_SI() const {
    return getViscosityCoefficientB(false);
  }

  openstudio::OSOptionalQuantity Gas_Impl::viscosityCoefficientB_IP() const {
    return getViscosityCoefficientB(true);
  }

  openstudio::OSOptionalQuantity Gas_Impl::viscosityCoefficientC_SI() const {
    return getViscosityCoefficientC(false);
  }

  openstudio::OSOptionalQuantity Gas_Impl::viscosityCoefficientC_IP() const {
    return getViscosityCoefficientC(true);
  }

  openstudio::OSOptionalQuantity Gas_Impl::specificHeatCoefficientA_SI() const {
    return getSpecificHeatCoefficientA(false);
  }

  openstudio::OSOptionalQuantity Gas_Impl::specificHeatCoefficientA_IP() const {
    return getSpecificHeatCoefficientA(true);
  }

  openstudio::OSOptionalQuantity Gas_Impl::specificHeatCoefficientB_SI() const {
    return getSpecificHeatCoefficientB(false);
  }

  openstudio::OSOptionalQuantity Gas_Impl::specificHeatCoefficientB_IP() const {
    return getSpecificHeatCoefficientB(true);
  }

  openstudio::OSOptionalQuantity Gas_Impl::specificHeatCoefficientC_SI() const {
    return getSpecificHeatCoefficientC(false);
  }

  openstudio::OSOptionalQuantity Gas_Impl::specificHeatCoefficientC_IP() const {
    return getSpecificHeatCoefficientC(true);
  }

  openstudio::OSOptionalQuantity Gas_Impl::molecularWeight_SI() const {
    return getMolecularWeight(false);
  }

  openstudio::OSOptionalQuantity Gas_Impl::molecularWeight_IP() const {
    return getMolecularWeight(true);
  }

  openstudio::OSOptionalQuantity Gas_Impl::specificHeatRatio_SI() const {
    return getSpecificHeatRatio(false);
  }

  openstudio::OSOptionalQuantity Gas_Impl::specificHeatRatio_IP() const {
    return getSpecificHeatRatio(true);
  }

} // detail

Gas::Gas(const Model& model,
         std::string gasType,
         double thickness)
  : GasLayer(Gas::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::Gas_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
  ok = setGasType(gasType);
  OS_ASSERT(ok);
   ok = setThickness(thickness);
  OS_ASSERT(ok);
}

std::vector<std::string> Gas::validGasTypes() {
  static StringVector result;
  if (result.empty()) {
    result = getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                            OS_WindowMaterial_GasFields::GasType);
    OS_ASSERT(!result.empty());
  }
  return result;
}

IddObjectType Gas::iddObjectType() {
  return IddObjectType(IddObjectType::OS_WindowMaterial_Gas);
}

std::vector<std::string> Gas::gasTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_WindowMaterial_GasFields::GasType);
}

std::string Gas::gasType() const {
  return getImpl<detail::Gas_Impl>()->gasType();
}

double Gas::getThermalConductivity(double temperature) const {
  return getImpl<detail::Gas_Impl>()->getThermalConductivity(temperature);
}

double Gas::getThermalConductance(double temperature) const {
  return getImpl<detail::Gas_Impl>()->getThermalConductance(temperature);
}

double Gas::getThermalResistivity(double temperature) const {
  return getImpl<detail::Gas_Impl>()->getThermalResistivity(temperature);
}

double Gas::getThermalResistance(double temperature) const {
  return getImpl<detail::Gas_Impl>()->getThermalResistance(temperature);
}

double Gas::getViscosity(double temperature) const {
  return getImpl<detail::Gas_Impl>()->getViscosity(temperature);
}

double Gas::getSpecificHeat(double temperature) const {
  return getImpl<detail::Gas_Impl>()->getSpecificHeat(temperature);
}

boost::optional<double> Gas::customConductivityCoefficientA() const {
  return getImpl<detail::Gas_Impl>()->customConductivityCoefficientA();
}

boost::optional<double> Gas::customConductivityCoefficientB() const {
  return getImpl<detail::Gas_Impl>()->customConductivityCoefficientB();
}

boost::optional<double> Gas::customConductivityCoefficientC() const {
  return getImpl<detail::Gas_Impl>()->customConductivityCoefficientC();
}

boost::optional<double> Gas::customViscosityCoefficientA() const {
  return getImpl<detail::Gas_Impl>()->customViscosityCoefficientA();
}

boost::optional<double> Gas::customViscosityCoefficientB() const {
  return getImpl<detail::Gas_Impl>()->customViscosityCoefficientB();
}

boost::optional<double> Gas::customViscosityCoefficientC() const {
  return getImpl<detail::Gas_Impl>()->customViscosityCoefficientC();
}

boost::optional<double> Gas::customSpecificHeatCoefficientA() const {
  return getImpl<detail::Gas_Impl>()->customSpecificHeatCoefficientA();
}

boost::optional<double> Gas::customSpecificHeatCoefficientB() const {
  return getImpl<detail::Gas_Impl>()->customSpecificHeatCoefficientB();
}

boost::optional<double> Gas::customSpecificHeatCoefficientC() const {
  return getImpl<detail::Gas_Impl>()->customSpecificHeatCoefficientC();
}

boost::optional<double> Gas::customMolecularWeight() const {
  return getImpl<detail::Gas_Impl>()->customMolecularWeight();
}

double Gas::thickness() const {
  return getImpl<detail::Gas_Impl>()->thickness();
}

Quantity Gas::getThickness(bool returnIP) const {
  return getImpl<detail::Gas_Impl>()->getThickness(returnIP);
}

boost::optional<double> Gas::conductivityCoefficientA() const {
  return getImpl<detail::Gas_Impl>()->conductivityCoefficientA();
}

OSOptionalQuantity Gas::getConductivityCoefficientA(bool returnIP) const {
  return getImpl<detail::Gas_Impl>()->getConductivityCoefficientA(returnIP);
}

boost::optional<double> Gas::conductivityCoefficientB() const {
  return getImpl<detail::Gas_Impl>()->conductivityCoefficientB();
}

OSOptionalQuantity Gas::getConductivityCoefficientB(bool returnIP) const {
  return getImpl<detail::Gas_Impl>()->getConductivityCoefficientB(returnIP);
}

boost::optional<double> Gas::conductivityCoefficientC() const {
  return getImpl<detail::Gas_Impl>()->conductivityCoefficientC();
}

OSOptionalQuantity Gas::getConductivityCoefficientC(bool returnIP) const {
  return getImpl<detail::Gas_Impl>()->getConductivityCoefficientC(returnIP);
}

boost::optional<double> Gas::viscosityCoefficientA() const {
  return getImpl<detail::Gas_Impl>()->viscosityCoefficientA();
}

OSOptionalQuantity Gas::getViscosityCoefficientA(bool returnIP) const {
  return getImpl<detail::Gas_Impl>()->getViscosityCoefficientA(returnIP);
}

boost::optional<double> Gas::viscosityCoefficientB() const {
  return getImpl<detail::Gas_Impl>()->viscosityCoefficientB();
}

OSOptionalQuantity Gas::getViscosityCoefficientB(bool returnIP) const {
  return getImpl<detail::Gas_Impl>()->getViscosityCoefficientB(returnIP);
}

boost::optional<double> Gas::viscosityCoefficientC() const {
  return getImpl<detail::Gas_Impl>()->viscosityCoefficientC();
}

OSOptionalQuantity Gas::getViscosityCoefficientC(bool returnIP) const {
  return getImpl<detail::Gas_Impl>()->getViscosityCoefficientC(returnIP);
}

boost::optional<double> Gas::specificHeatCoefficientA() const {
  return getImpl<detail::Gas_Impl>()->specificHeatCoefficientA();
}

OSOptionalQuantity Gas::getSpecificHeatCoefficientA(bool returnIP) const {
  return getImpl<detail::Gas_Impl>()->getSpecificHeatCoefficientA(returnIP);
}

boost::optional<double> Gas::specificHeatCoefficientB() const {
  return getImpl<detail::Gas_Impl>()->specificHeatCoefficientB();
}

OSOptionalQuantity Gas::getSpecificHeatCoefficientB(bool returnIP) const {
  return getImpl<detail::Gas_Impl>()->getSpecificHeatCoefficientB(returnIP);
}

boost::optional<double> Gas::specificHeatCoefficientC() const {
  return getImpl<detail::Gas_Impl>()->specificHeatCoefficientC();
}

OSOptionalQuantity Gas::getSpecificHeatCoefficientC(bool returnIP) const {
  return getImpl<detail::Gas_Impl>()->getSpecificHeatCoefficientC(returnIP);
}

boost::optional<double> Gas::molecularWeight() const {
  return getImpl<detail::Gas_Impl>()->molecularWeight();
}

OSOptionalQuantity Gas::getMolecularWeight(bool returnIP) const {
  return getImpl<detail::Gas_Impl>()->getMolecularWeight(returnIP);
}

boost::optional<double> Gas::specificHeatRatio() const {
  return getImpl<detail::Gas_Impl>()->specificHeatRatio();
}

OSOptionalQuantity Gas::getSpecificHeatRatio(bool returnIP) const {
  return getImpl<detail::Gas_Impl>()->getSpecificHeatRatio(returnIP);
}

bool Gas::setGasType(std::string gasType) {
  return getImpl<detail::Gas_Impl>()->setGasType(gasType);
}

bool Gas::setCustomConductivity(double A, double B, double C) {
  return getImpl<detail::Gas_Impl>()->setCustomConductivity(A,B,C);
}

bool Gas::setCustomViscosity(double A, double B, double C) {
  return getImpl<detail::Gas_Impl>()->setCustomViscosity(A,B,C);
}

bool Gas::setCustomSpecificHeat(double A, double B, double C) {
  return getImpl<detail::Gas_Impl>()->setCustomSpecificHeat(A,B,C);
}

bool Gas::setCustomMolecularWeight(double value) {
  return getImpl<detail::Gas_Impl>()->setCustomMolecularWeight(value);
}

bool Gas::setThickness(double thickness) {
  return getImpl<detail::Gas_Impl>()->setThickness(thickness);
}

bool Gas::setThickness(const Quantity& thickness) {
  return getImpl<detail::Gas_Impl>()->setThickness(thickness);
}

void Gas::setConductivityCoefficientA(double conductivityCoefficientA) {
  getImpl<detail::Gas_Impl>()->setConductivityCoefficientA(conductivityCoefficientA);
}

bool Gas::setConductivityCoefficientA(const Quantity& conductivityCoefficientA) {
  return getImpl<detail::Gas_Impl>()->setConductivityCoefficientA(conductivityCoefficientA);
}

void Gas::resetConductivityCoefficientA() {
  getImpl<detail::Gas_Impl>()->resetConductivityCoefficientA();
}

void Gas::setConductivityCoefficientB(double conductivityCoefficientB) {
  getImpl<detail::Gas_Impl>()->setConductivityCoefficientB(conductivityCoefficientB);
}

bool Gas::setConductivityCoefficientB(const Quantity& conductivityCoefficientB) {
  return getImpl<detail::Gas_Impl>()->setConductivityCoefficientB(conductivityCoefficientB);
}

void Gas::resetConductivityCoefficientB() {
  getImpl<detail::Gas_Impl>()->resetConductivityCoefficientB();
}

void Gas::setConductivityCoefficientC(double conductivityCoefficientC) {
  getImpl<detail::Gas_Impl>()->setConductivityCoefficientC(conductivityCoefficientC);
}

bool Gas::setConductivityCoefficientC(const Quantity& conductivityCoefficientC) {
  return getImpl<detail::Gas_Impl>()->setConductivityCoefficientC(conductivityCoefficientC);
}

void Gas::resetConductivityCoefficientC() {
  getImpl<detail::Gas_Impl>()->resetConductivityCoefficientC();
}

bool Gas::setViscosityCoefficientA(double viscosityCoefficientA) {
  return getImpl<detail::Gas_Impl>()->setViscosityCoefficientA(viscosityCoefficientA);
}

bool Gas::setViscosityCoefficientA(const Quantity& viscosityCoefficientA) {
  return getImpl<detail::Gas_Impl>()->setViscosityCoefficientA(viscosityCoefficientA);
}

void Gas::resetViscosityCoefficientA() {
  getImpl<detail::Gas_Impl>()->resetViscosityCoefficientA();
}

void Gas::setViscosityCoefficientB(double viscosityCoefficientB) {
  getImpl<detail::Gas_Impl>()->setViscosityCoefficientB(viscosityCoefficientB);
}

bool Gas::setViscosityCoefficientB(const Quantity& viscosityCoefficientB) {
  return getImpl<detail::Gas_Impl>()->setViscosityCoefficientB(viscosityCoefficientB);
}

void Gas::resetViscosityCoefficientB() {
  getImpl<detail::Gas_Impl>()->resetViscosityCoefficientB();
}

void Gas::setViscosityCoefficientC(double viscosityCoefficientC) {
  getImpl<detail::Gas_Impl>()->setViscosityCoefficientC(viscosityCoefficientC);
}

bool Gas::setViscosityCoefficientC(const Quantity& viscosityCoefficientC) {
  return getImpl<detail::Gas_Impl>()->setViscosityCoefficientC(viscosityCoefficientC);
}

void Gas::resetViscosityCoefficientC() {
  getImpl<detail::Gas_Impl>()->resetViscosityCoefficientC();
}

bool Gas::setSpecificHeatCoefficientA(double specificHeatCoefficientA) {
  return getImpl<detail::Gas_Impl>()->setSpecificHeatCoefficientA(specificHeatCoefficientA);
}

bool Gas::setSpecificHeatCoefficientA(const Quantity& specificHeatCoefficientA) {
  return getImpl<detail::Gas_Impl>()->setSpecificHeatCoefficientA(specificHeatCoefficientA);
}

void Gas::resetSpecificHeatCoefficientA() {
  getImpl<detail::Gas_Impl>()->resetSpecificHeatCoefficientA();
}

void Gas::setSpecificHeatCoefficientB(double specificHeatCoefficientB) {
  getImpl<detail::Gas_Impl>()->setSpecificHeatCoefficientB(specificHeatCoefficientB);
}

bool Gas::setSpecificHeatCoefficientB(const Quantity& specificHeatCoefficientB) {
  return getImpl<detail::Gas_Impl>()->setSpecificHeatCoefficientB(specificHeatCoefficientB);
}

void Gas::resetSpecificHeatCoefficientB() {
  getImpl<detail::Gas_Impl>()->resetSpecificHeatCoefficientB();
}

void Gas::setSpecificHeatCoefficientC(double specificHeatCoefficientC) {
  getImpl<detail::Gas_Impl>()->setSpecificHeatCoefficientC(specificHeatCoefficientC);
}

bool Gas::setSpecificHeatCoefficientC(const Quantity& specificHeatCoefficientC) {
  return getImpl<detail::Gas_Impl>()->setSpecificHeatCoefficientC(specificHeatCoefficientC);
}

void Gas::resetSpecificHeatCoefficientC() {
  getImpl<detail::Gas_Impl>()->resetSpecificHeatCoefficientC();
}

bool Gas::setMolecularWeight(double molecularWeight) {
  return getImpl<detail::Gas_Impl>()->setMolecularWeight(molecularWeight);
}

bool Gas::setMolecularWeight(const Quantity& molecularWeight) {
  return getImpl<detail::Gas_Impl>()->setMolecularWeight(molecularWeight);
}

void Gas::resetMolecularWeight() {
  getImpl<detail::Gas_Impl>()->resetMolecularWeight();
}

bool Gas::setSpecificHeatRatio(double specificHeatRatio) {
  return getImpl<detail::Gas_Impl>()->setSpecificHeatRatio(specificHeatRatio);
}

bool Gas::setSpecificHeatRatio(const Quantity& specificHeatRatio) {
  return getImpl<detail::Gas_Impl>()->setSpecificHeatRatio(specificHeatRatio);
}

void Gas::resetSpecificHeatRatio() {
  getImpl<detail::Gas_Impl>()->resetSpecificHeatRatio();
}

/// @cond
Gas::Gas(std::shared_ptr<detail::Gas_Impl> impl)
  : GasLayer(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

