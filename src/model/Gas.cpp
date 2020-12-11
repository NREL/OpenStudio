/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

    Gas_Impl::Gas_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle) : GasLayer_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == Gas::iddObjectType());
    }

    Gas_Impl::Gas_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : GasLayer_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == Gas::iddObjectType());
    }

    Gas_Impl::Gas_Impl(const Gas_Impl& other, Model_Impl* model, bool keepHandle) : GasLayer_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& Gas_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType Gas_Impl::iddObjectType() const {
      return Gas::iddObjectType();
    }

    std::string Gas_Impl::gasType() const {
      boost::optional<std::string> value = getString(OS_WindowMaterial_GasFields::GasType, true);
      OS_ASSERT(value);
      return value.get();
    }

    double Gas_Impl::thickness() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_GasFields::Thickness, true);
      OS_ASSERT(value);
      return value.get();
    }

    double Gas_Impl::getThermalConductivity(double temperature) const {
      std::string gasType = this->gasType();
      std::vector<double> coeffs;
      if (openstudio::istringEqual(gasType, "Air")) {
        coeffs = FenestrationMaterial::airThermalConductivityCoefficients();
      } else if (openstudio::istringEqual(gasType, "Argon")) {
        coeffs = FenestrationMaterial::argonThermalConductivityCoefficients();
      } else if (openstudio::istringEqual(gasType, "Krypton")) {
        coeffs = FenestrationMaterial::kryptonThermalConductivityCoefficients();
      } else if (openstudio::istringEqual(gasType, "Xenon")) {
        coeffs = FenestrationMaterial::xenonThermalConductivityCoefficients();
      } else if (openstudio::istringEqual(gasType, "Custom")) {
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
      } else {
        LOG_AND_THROW("Unknown gasType listed in " << briefDescription() << ".");
      }

      return coeffs[0] + (coeffs[1] * temperature) + (coeffs[2] * ::pow(temperature, 2));
    }

    double Gas_Impl::getThermalConductance(double temperature) const {
      return getThermalConductivity(temperature) / thickness();
    }

    double Gas_Impl::getThermalResistivity(double temperature) const {
      return 1.0 / getThermalConductivity(temperature);
    }

    double Gas_Impl::getThermalResistance(double temperature) const {
      return 1.0 / getThermalConductance(temperature);
    }

    double Gas_Impl::getViscosity(double temperature) const {
      std::string gasType = this->gasType();
      std::vector<double> coeffs(3);
      if (openstudio::istringEqual(gasType, "Air")) {
        coeffs = FenestrationMaterial::airViscosityCoefficients();
      } else if (openstudio::istringEqual(gasType, "Argon")) {
        coeffs = FenestrationMaterial::argonViscosityCoefficients();
      } else if (openstudio::istringEqual(gasType, "Krypton")) {
        coeffs = FenestrationMaterial::kryptonViscosityCoefficients();
      } else if (openstudio::istringEqual(gasType, "Xenon")) {
        coeffs = FenestrationMaterial::xenonViscosityCoefficients();
      } else if (openstudio::istringEqual(gasType, "Custom")) {
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
      } else {
        LOG_AND_THROW("Unknown gasType listed in " << briefDescription() << ".");
      }

      return coeffs[0] + (coeffs[1] * temperature) + (coeffs[2] * ::pow(temperature, 2));
    }

    double Gas_Impl::getSpecificHeat(double temperature) const {
      std::string gasType = this->gasType();
      std::vector<double> coeffs;
      if (openstudio::istringEqual(gasType, "Air")) {
        coeffs = FenestrationMaterial::airSpecificHeatCoefficients();
      } else if (openstudio::istringEqual(gasType, "Argon")) {
        coeffs = FenestrationMaterial::argonSpecificHeatCoefficients();
      } else if (openstudio::istringEqual(gasType, "Krypton")) {
        coeffs = FenestrationMaterial::kryptonSpecificHeatCoefficients();
      } else if (openstudio::istringEqual(gasType, "Xenon")) {
        coeffs = FenestrationMaterial::xenonSpecificHeatCoefficients();
      } else if (openstudio::istringEqual(gasType, "Custom")) {
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
      } else {
        LOG_AND_THROW("Unknown gasType listed in " << briefDescription() << ".");
      }

      return coeffs[0] + (coeffs[1] * temperature) + (coeffs[2] * ::pow(temperature, 2));
    }

    boost::optional<double> Gas_Impl::customConductivityCoefficientA() const {
      return getDouble(OS_WindowMaterial_GasFields::ConductivityCoefficientA, true);
    }

    boost::optional<double> Gas_Impl::customConductivityCoefficientB() const {
      return getDouble(OS_WindowMaterial_GasFields::ConductivityCoefficientB, true);
    }

    boost::optional<double> Gas_Impl::customConductivityCoefficientC() const {
      return getDouble(OS_WindowMaterial_GasFields::ConductivityCoefficientC, true);
    }

    boost::optional<double> Gas_Impl::customViscosityCoefficientA() const {
      return getDouble(OS_WindowMaterial_GasFields::ViscosityCoefficientA, true);
    }

    boost::optional<double> Gas_Impl::customViscosityCoefficientB() const {
      return getDouble(OS_WindowMaterial_GasFields::ViscosityCoefficientB, true);
    }

    boost::optional<double> Gas_Impl::customViscosityCoefficientC() const {
      return getDouble(OS_WindowMaterial_GasFields::ViscosityCoefficientC, true);
    }

    boost::optional<double> Gas_Impl::customSpecificHeatCoefficientA() const {
      return getDouble(OS_WindowMaterial_GasFields::SpecificHeatCoefficientA, true);
    }

    boost::optional<double> Gas_Impl::customSpecificHeatCoefficientB() const {
      return getDouble(OS_WindowMaterial_GasFields::SpecificHeatCoefficientB, true);
    }

    boost::optional<double> Gas_Impl::customSpecificHeatCoefficientC() const {
      return getDouble(OS_WindowMaterial_GasFields::SpecificHeatCoefficientC, true);
    }

    boost::optional<double> Gas_Impl::customMolecularWeight() const {
      return getDouble(OS_WindowMaterial_GasFields::MolecularWeight, true);
    }

    bool Gas_Impl::setCustomConductivity(double A, double B, double C) {

      OptionalDouble oldAValue = customConductivityCoefficientA();
      bool result = setDouble(OS_WindowMaterial_GasFields::ConductivityCoefficientA, A);
      if (!result) {
        return false;
      }

      result = setDouble(OS_WindowMaterial_GasFields::ConductivityCoefficientB, B);
      if (!result) {
        // reset A to previous value
        if (oldAValue) {
          setDouble(OS_WindowMaterial_GasFields::ConductivityCoefficientA, *oldAValue);
        } else {
          setString(OS_WindowMaterial_GasFields::ConductivityCoefficientA, "");
        }
      }

      setDouble(OS_WindowMaterial_GasFields::ConductivityCoefficientC, C);

      return result;
    }

    bool Gas_Impl::setCustomViscosity(double A, double B, double C) {
      OptionalDouble oldAValue = customViscosityCoefficientA();
      bool result = setDouble(OS_WindowMaterial_GasFields::ViscosityCoefficientA, A);
      if (!result) {
        return false;
      }

      result = setDouble(OS_WindowMaterial_GasFields::ViscosityCoefficientB, B);
      if (!result) {
        // reset A to previous value
        if (oldAValue) {
          setDouble(OS_WindowMaterial_GasFields::ViscosityCoefficientA, *oldAValue);
        } else {
          setString(OS_WindowMaterial_GasFields::ViscosityCoefficientA, "");
        }
      }

      setDouble(OS_WindowMaterial_GasFields::ViscosityCoefficientC, C);

      return result;
    }

    bool Gas_Impl::setCustomSpecificHeat(double A, double B, double C) {
      OptionalDouble oldAValue = customSpecificHeatCoefficientA();
      bool result = setDouble(OS_WindowMaterial_GasFields::SpecificHeatCoefficientA, A);
      if (!result) {
        return false;
      }

      result = setDouble(OS_WindowMaterial_GasFields::SpecificHeatCoefficientB, B);
      if (!result) {
        // reset A to previous value
        if (oldAValue) {
          setDouble(OS_WindowMaterial_GasFields::SpecificHeatCoefficientA, *oldAValue);
        } else {
          setString(OS_WindowMaterial_GasFields::SpecificHeatCoefficientA, "");
        }
      }

      setDouble(OS_WindowMaterial_GasFields::SpecificHeatCoefficientC, C);

      return result;
    }

    bool Gas_Impl::setCustomMolecularWeight(double value) {
      return setDouble(OS_WindowMaterial_GasFields::MolecularWeight, value);
    }

    boost::optional<double> Gas_Impl::conductivityCoefficientA() const {
      return getDouble(OS_WindowMaterial_GasFields::ConductivityCoefficientA, true);
    }

    boost::optional<double> Gas_Impl::conductivityCoefficientB() const {
      return getDouble(OS_WindowMaterial_GasFields::ConductivityCoefficientB, true);
    }

    boost::optional<double> Gas_Impl::conductivityCoefficientC() const {
      return getDouble(OS_WindowMaterial_GasFields::ConductivityCoefficientC, true);
    }

    boost::optional<double> Gas_Impl::viscosityCoefficientA() const {
      return getDouble(OS_WindowMaterial_GasFields::ViscosityCoefficientA, true);
    }

    boost::optional<double> Gas_Impl::viscosityCoefficientB() const {
      return getDouble(OS_WindowMaterial_GasFields::ViscosityCoefficientB, true);
    }

    boost::optional<double> Gas_Impl::viscosityCoefficientC() const {
      return getDouble(OS_WindowMaterial_GasFields::ViscosityCoefficientC, true);
    }

    boost::optional<double> Gas_Impl::specificHeatCoefficientA() const {
      return getDouble(OS_WindowMaterial_GasFields::SpecificHeatCoefficientA, true);
    }

    boost::optional<double> Gas_Impl::specificHeatCoefficientB() const {
      return getDouble(OS_WindowMaterial_GasFields::SpecificHeatCoefficientB, true);
    }

    boost::optional<double> Gas_Impl::specificHeatCoefficientC() const {
      return getDouble(OS_WindowMaterial_GasFields::SpecificHeatCoefficientC, true);
    }

    boost::optional<double> Gas_Impl::molecularWeight() const {
      return getDouble(OS_WindowMaterial_GasFields::MolecularWeight, true);
    }

    boost::optional<double> Gas_Impl::specificHeatRatio() const {
      return getDouble(OS_WindowMaterial_GasFields::SpecificHeatRatio, true);
    }

    bool Gas_Impl::setGasType(std::string gasType) {
      bool result = setString(OS_WindowMaterial_GasFields::GasType, gasType);
      return result;
    }

    bool Gas_Impl::setThickness(double thickness) {
      bool result = setDouble(OS_WindowMaterial_GasFields::Thickness, thickness);
      return result;
    }

    bool Gas_Impl::setConductivityCoefficientA(boost::optional<double> conductivityCoefficientA) {
      bool result(false);
      if (conductivityCoefficientA) {
        result = setDouble(OS_WindowMaterial_GasFields::ConductivityCoefficientA, conductivityCoefficientA.get());
      } else {
        resetConductivityCoefficientA();
        result = true;
      }
      OS_ASSERT(result);
      return result;
    }

    void Gas_Impl::resetConductivityCoefficientA() {
      bool result = setString(OS_WindowMaterial_GasFields::ConductivityCoefficientA, "");
      OS_ASSERT(result);
    }

    bool Gas_Impl::setConductivityCoefficientB(boost::optional<double> conductivityCoefficientB) {
      bool result(false);
      if (conductivityCoefficientB) {
        result = setDouble(OS_WindowMaterial_GasFields::ConductivityCoefficientB, conductivityCoefficientB.get());
      } else {
        resetConductivityCoefficientB();
        result = true;
      }
      OS_ASSERT(result);
      return result;
    }

    void Gas_Impl::resetConductivityCoefficientB() {
      bool result = setString(OS_WindowMaterial_GasFields::ConductivityCoefficientB, "");
      OS_ASSERT(result);
    }

    bool Gas_Impl::setConductivityCoefficientC(boost::optional<double> conductivityCoefficientC) {
      bool result(false);
      if (conductivityCoefficientC) {
        result = setDouble(OS_WindowMaterial_GasFields::ConductivityCoefficientC, conductivityCoefficientC.get());
      } else {
        resetConductivityCoefficientC();
        result = true;
      }
      OS_ASSERT(result);
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
      } else {
        resetViscosityCoefficientA();
        result = true;
      }
      return result;
    }

    void Gas_Impl::resetViscosityCoefficientA() {
      bool result = setString(OS_WindowMaterial_GasFields::ViscosityCoefficientA, "");
      OS_ASSERT(result);
    }

    bool Gas_Impl::setViscosityCoefficientB(boost::optional<double> viscosityCoefficientB) {
      bool result(false);
      if (viscosityCoefficientB) {
        result = setDouble(OS_WindowMaterial_GasFields::ViscosityCoefficientB, viscosityCoefficientB.get());
      } else {
        resetViscosityCoefficientB();
        result = true;
      }
      OS_ASSERT(result);
      return result;
    }

    void Gas_Impl::resetViscosityCoefficientB() {
      bool result = setString(OS_WindowMaterial_GasFields::ViscosityCoefficientB, "");
      OS_ASSERT(result);
    }

    bool Gas_Impl::setViscosityCoefficientC(boost::optional<double> viscosityCoefficientC) {
      bool result(false);
      if (viscosityCoefficientC) {
        result = setDouble(OS_WindowMaterial_GasFields::ViscosityCoefficientC, viscosityCoefficientC.get());
      } else {
        resetViscosityCoefficientC();
        result = true;
      }
      OS_ASSERT(result);
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
      } else {
        resetSpecificHeatCoefficientA();
        result = true;
      }
      return result;
    }

    void Gas_Impl::resetSpecificHeatCoefficientA() {
      bool result = setString(OS_WindowMaterial_GasFields::SpecificHeatCoefficientA, "");
      OS_ASSERT(result);
    }

    bool Gas_Impl::setSpecificHeatCoefficientB(boost::optional<double> specificHeatCoefficientB) {
      bool result(false);
      if (specificHeatCoefficientB) {
        result = setDouble(OS_WindowMaterial_GasFields::SpecificHeatCoefficientB, specificHeatCoefficientB.get());
      } else {
        resetSpecificHeatCoefficientB();
        result = true;
      }
      OS_ASSERT(result);
      return result;
    }

    void Gas_Impl::resetSpecificHeatCoefficientB() {
      bool result = setString(OS_WindowMaterial_GasFields::SpecificHeatCoefficientB, "");
      OS_ASSERT(result);
    }

    bool Gas_Impl::setSpecificHeatCoefficientC(boost::optional<double> specificHeatCoefficientC) {
      bool result(false);
      if (specificHeatCoefficientC) {
        result = setDouble(OS_WindowMaterial_GasFields::SpecificHeatCoefficientC, specificHeatCoefficientC.get());
      } else {
        resetSpecificHeatCoefficientC();
        result = true;
      }
      OS_ASSERT(result);
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
      } else {
        resetMolecularWeight();
        result = true;
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
      } else {
        resetSpecificHeatRatio();
        result = true;
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

  }  // namespace detail

  Gas::Gas(const Model& model, std::string gasType, double thickness) : GasLayer(Gas::iddObjectType(), model) {
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
    static const StringVector result = getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_WindowMaterial_GasFields::GasType);
    OS_ASSERT(!result.empty());
    return result;
  }

  IddObjectType Gas::iddObjectType() {
    return IddObjectType(IddObjectType::OS_WindowMaterial_Gas);
  }

  std::vector<std::string> Gas::gasTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_WindowMaterial_GasFields::GasType);
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

  boost::optional<double> Gas::conductivityCoefficientA() const {
    return getImpl<detail::Gas_Impl>()->conductivityCoefficientA();
  }

  boost::optional<double> Gas::conductivityCoefficientB() const {
    return getImpl<detail::Gas_Impl>()->conductivityCoefficientB();
  }

  boost::optional<double> Gas::conductivityCoefficientC() const {
    return getImpl<detail::Gas_Impl>()->conductivityCoefficientC();
  }

  boost::optional<double> Gas::viscosityCoefficientA() const {
    return getImpl<detail::Gas_Impl>()->viscosityCoefficientA();
  }

  boost::optional<double> Gas::viscosityCoefficientB() const {
    return getImpl<detail::Gas_Impl>()->viscosityCoefficientB();
  }

  boost::optional<double> Gas::viscosityCoefficientC() const {
    return getImpl<detail::Gas_Impl>()->viscosityCoefficientC();
  }

  boost::optional<double> Gas::specificHeatCoefficientA() const {
    return getImpl<detail::Gas_Impl>()->specificHeatCoefficientA();
  }

  boost::optional<double> Gas::specificHeatCoefficientB() const {
    return getImpl<detail::Gas_Impl>()->specificHeatCoefficientB();
  }

  boost::optional<double> Gas::specificHeatCoefficientC() const {
    return getImpl<detail::Gas_Impl>()->specificHeatCoefficientC();
  }

  boost::optional<double> Gas::molecularWeight() const {
    return getImpl<detail::Gas_Impl>()->molecularWeight();
  }

  boost::optional<double> Gas::specificHeatRatio() const {
    return getImpl<detail::Gas_Impl>()->specificHeatRatio();
  }

  bool Gas::setGasType(std::string gasType) {
    return getImpl<detail::Gas_Impl>()->setGasType(gasType);
  }

  bool Gas::setCustomConductivity(double A, double B, double C) {
    return getImpl<detail::Gas_Impl>()->setCustomConductivity(A, B, C);
  }

  bool Gas::setCustomViscosity(double A, double B, double C) {
    return getImpl<detail::Gas_Impl>()->setCustomViscosity(A, B, C);
  }

  bool Gas::setCustomSpecificHeat(double A, double B, double C) {
    return getImpl<detail::Gas_Impl>()->setCustomSpecificHeat(A, B, C);
  }

  bool Gas::setCustomMolecularWeight(double value) {
    return getImpl<detail::Gas_Impl>()->setCustomMolecularWeight(value);
  }

  bool Gas::setThickness(double thickness) {
    return getImpl<detail::Gas_Impl>()->setThickness(thickness);
  }

  bool Gas::setConductivityCoefficientA(double conductivityCoefficientA) {
    return getImpl<detail::Gas_Impl>()->setConductivityCoefficientA(conductivityCoefficientA);
  }

  void Gas::resetConductivityCoefficientA() {
    getImpl<detail::Gas_Impl>()->resetConductivityCoefficientA();
  }

  bool Gas::setConductivityCoefficientB(double conductivityCoefficientB) {
    return getImpl<detail::Gas_Impl>()->setConductivityCoefficientB(conductivityCoefficientB);
  }

  void Gas::resetConductivityCoefficientB() {
    getImpl<detail::Gas_Impl>()->resetConductivityCoefficientB();
  }

  bool Gas::setConductivityCoefficientC(double conductivityCoefficientC) {
    return getImpl<detail::Gas_Impl>()->setConductivityCoefficientC(conductivityCoefficientC);
  }

  void Gas::resetConductivityCoefficientC() {
    getImpl<detail::Gas_Impl>()->resetConductivityCoefficientC();
  }

  bool Gas::setViscosityCoefficientA(double viscosityCoefficientA) {
    return getImpl<detail::Gas_Impl>()->setViscosityCoefficientA(viscosityCoefficientA);
  }

  void Gas::resetViscosityCoefficientA() {
    getImpl<detail::Gas_Impl>()->resetViscosityCoefficientA();
  }

  bool Gas::setViscosityCoefficientB(double viscosityCoefficientB) {
    return getImpl<detail::Gas_Impl>()->setViscosityCoefficientB(viscosityCoefficientB);
  }

  void Gas::resetViscosityCoefficientB() {
    getImpl<detail::Gas_Impl>()->resetViscosityCoefficientB();
  }

  bool Gas::setViscosityCoefficientC(double viscosityCoefficientC) {
    return getImpl<detail::Gas_Impl>()->setViscosityCoefficientC(viscosityCoefficientC);
  }

  void Gas::resetViscosityCoefficientC() {
    getImpl<detail::Gas_Impl>()->resetViscosityCoefficientC();
  }

  bool Gas::setSpecificHeatCoefficientA(double specificHeatCoefficientA) {
    return getImpl<detail::Gas_Impl>()->setSpecificHeatCoefficientA(specificHeatCoefficientA);
  }

  void Gas::resetSpecificHeatCoefficientA() {
    getImpl<detail::Gas_Impl>()->resetSpecificHeatCoefficientA();
  }

  bool Gas::setSpecificHeatCoefficientB(double specificHeatCoefficientB) {
    return getImpl<detail::Gas_Impl>()->setSpecificHeatCoefficientB(specificHeatCoefficientB);
  }

  void Gas::resetSpecificHeatCoefficientB() {
    getImpl<detail::Gas_Impl>()->resetSpecificHeatCoefficientB();
  }

  bool Gas::setSpecificHeatCoefficientC(double specificHeatCoefficientC) {
    return getImpl<detail::Gas_Impl>()->setSpecificHeatCoefficientC(specificHeatCoefficientC);
  }

  void Gas::resetSpecificHeatCoefficientC() {
    getImpl<detail::Gas_Impl>()->resetSpecificHeatCoefficientC();
  }

  bool Gas::setMolecularWeight(double molecularWeight) {
    return getImpl<detail::Gas_Impl>()->setMolecularWeight(molecularWeight);
  }

  void Gas::resetMolecularWeight() {
    getImpl<detail::Gas_Impl>()->resetMolecularWeight();
  }

  bool Gas::setSpecificHeatRatio(double specificHeatRatio) {
    return getImpl<detail::Gas_Impl>()->setSpecificHeatRatio(specificHeatRatio);
  }

  void Gas::resetSpecificHeatRatio() {
    getImpl<detail::Gas_Impl>()->resetSpecificHeatRatio();
  }

  /// @cond
  Gas::Gas(std::shared_ptr<detail::Gas_Impl> impl) : GasLayer(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio