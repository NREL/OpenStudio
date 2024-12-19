/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "GasMixture.hpp"
#include "GasMixture_Impl.hpp"

#include "../utilities/idf/ValidityReport.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_WindowMaterial_GasMixture_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/math/FloatCompare.hpp"

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"
#include <math.h>

namespace openstudio {
namespace model {

  namespace detail {

    GasMixture_Impl::GasMixture_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle) : GasLayer_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == GasMixture::iddObjectType());
    }

    GasMixture_Impl::GasMixture_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : GasLayer_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == GasMixture::iddObjectType());
    }

    GasMixture_Impl::GasMixture_Impl(const GasMixture_Impl& other, Model_Impl* model, bool keepHandle) : GasLayer_Impl(other, model, keepHandle) {}

    IddObjectType GasMixture_Impl::iddObjectType() const {
      return GasMixture::iddObjectType();
    }

    std::string GasMixture_Impl::getGasType(unsigned index) const {
      if (index >= numGases()) {
        LOG_AND_THROW("This GasMixture " << briefDescription() << " only has " << numGases()
                                         << " gases in it, and so the type of gas cannot be returned for index " << index << ".");
      }
      OptionalString os = getString(mf_getGasTypeFieldIndex(index), true);
      if (!os) {
        LOG_AND_THROW("gasType is not yet set for the gas at index " << index << " of " << briefDescription() << ".");
      }
      return *os;
    }

    double GasMixture_Impl::getGasFraction(unsigned index) const {
      if (index >= numGases()) {
        LOG_AND_THROW("This GasMixture " << briefDescription() << " only has " << numGases()
                                         << " gases in it, and so the gas fraction cannot be returned for index " << index << ".");
      }
      OptionalDouble od = getDouble(mf_getGasFractionFieldIndex(index), true);
      if (!od) {
        LOG_AND_THROW("gasFraction is not yet set for the gas at index " << index << " of " << briefDescription() << ".");
      }
      return *od;
    }

    double GasMixture_Impl::getThermalConductivity(double temperature) const {
      double molecularWeight(0.0);  // cumulative sum of fraction * molecular weight
      double result(0.0);           // cumulative sum of conductivity * fraction * molecular weight
      for (unsigned i = 0, n = numGases(); i < n; ++i) {
        std::string type = getGasType(i);
        double fraction = getGasFraction(i);
        double weight(0.0);
        std::vector<double> coeffs;
        if (openstudio::istringEqual(type, "Air")) {
          weight = FenestrationMaterial::airMolecularWeight();
          coeffs = FenestrationMaterial::airThermalConductivityCoefficients();
        } else if (openstudio::istringEqual(type, "Argon")) {
          weight = FenestrationMaterial::argonMolecularWeight();
          coeffs = FenestrationMaterial::argonThermalConductivityCoefficients();
        } else if (openstudio::istringEqual(type, "Krypton")) {
          weight = FenestrationMaterial::kryptonMolecularWeight();
          coeffs = FenestrationMaterial::kryptonThermalConductivityCoefficients();
        } else if (openstudio::istringEqual(type, "Xenon")) {
          weight = FenestrationMaterial::xenonMolecularWeight();
          coeffs = FenestrationMaterial::xenonThermalConductivityCoefficients();
        } else {
          LOG_AND_THROW("Unknown gasType listed in " << briefDescription() << ".");
        }
        molecularWeight += weight * fraction;
        result += (coeffs[0] + (coeffs[1] * temperature) + (coeffs[2] * ::pow(temperature, 2.0))) * fraction * weight;
      }
      result /= molecularWeight;
      return result;
    }

    double GasMixture_Impl::getThermalConductance(double temperature) const {
      return getThermalConductivity(temperature) / thickness();
    }

    double GasMixture_Impl::getThermalResistivity(double temperature) const {
      return 1.0 / getThermalConductivity(temperature);
    }

    double GasMixture_Impl::getThermalResistance(double temperature) const {
      return 1.0 / getThermalConductance(temperature);
    }

    double GasMixture_Impl::getViscosity(double temperature) const {
      double molecularWeight(0.0);  // cumulative sum of fraction * molecular weight
      double result(0.0);           // cumulative sum of viscosity * fraction * molecular weight
      for (unsigned i = 0, n = numGases(); i < n; ++i) {
        std::string type = getGasType(i);
        double fraction = getGasFraction(i);
        double weight(0.0);
        std::vector<double> coeffs;
        if (openstudio::istringEqual(type, "Air")) {
          weight = FenestrationMaterial::airMolecularWeight();
          coeffs = FenestrationMaterial::airViscosityCoefficients();
        } else if (openstudio::istringEqual(type, "Argon")) {
          weight = FenestrationMaterial::argonMolecularWeight();
          coeffs = FenestrationMaterial::argonViscosityCoefficients();
        } else if (openstudio::istringEqual(type, "Krypton")) {
          weight = FenestrationMaterial::kryptonMolecularWeight();
          coeffs = FenestrationMaterial::kryptonViscosityCoefficients();
        } else if (openstudio::istringEqual(type, "Xenon")) {
          weight = FenestrationMaterial::xenonMolecularWeight();
          coeffs = FenestrationMaterial::xenonViscosityCoefficients();
        } else {
          LOG_AND_THROW("Unknown gasType listed in " << briefDescription() << ".");
        }
        molecularWeight += weight * fraction;
        result += (coeffs[0] + (coeffs[1] * temperature) + (coeffs[2] * ::pow(temperature, 2))) * fraction * weight;
      }
      result /= molecularWeight;
      return result;
    }

    double GasMixture_Impl::getSpecificHeat(double temperature) const {
      double molecularWeight(0.0);  // cumulative sum of fraction * molecular weight
      double result(0.0);           // cumulative sum of specific heat * fraction * molecular weight
      for (unsigned i = 0, n = numGases(); i < n; ++i) {
        std::string type = getGasType(i);
        double fraction = getGasFraction(i);
        double weight(0.0);
        std::vector<double> coeffs;
        if (openstudio::istringEqual(type, "Air")) {
          weight = FenestrationMaterial::airMolecularWeight();
          coeffs = FenestrationMaterial::airSpecificHeatCoefficients();
        } else if (openstudio::istringEqual(type, "Argon")) {
          weight = FenestrationMaterial::argonMolecularWeight();
          coeffs = FenestrationMaterial::argonSpecificHeatCoefficients();
        } else if (openstudio::istringEqual(type, "Krypton")) {
          weight = FenestrationMaterial::kryptonMolecularWeight();
          coeffs = FenestrationMaterial::kryptonSpecificHeatCoefficients();
        } else if (openstudio::istringEqual(type, "Xenon")) {
          weight = FenestrationMaterial::xenonMolecularWeight();
          coeffs = FenestrationMaterial::xenonSpecificHeatCoefficients();
        } else {
          LOG_AND_THROW("Unknown gasType listed in " << briefDescription() << ".");
        }
        molecularWeight += weight * fraction;
        result += (coeffs[0] + (coeffs[1] * temperature) + (coeffs[2] * ::pow(temperature, 2))) * fraction * weight;
      }
      result /= molecularWeight;
      return result;
    }

    const std::vector<std::string>& GasMixture_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    bool GasMixture_Impl::setGas(unsigned index, const std::string& type, double fraction) {
      if ((index > numGases()) || (index >= 4)) {
        LOG(Info, "Cannot set the gas at index " << index << " in GasMixture " << briefDescription() << ". The current number of gases is "
                                                 << numGases() << ", and the maximum is 4.");
        return false;
      }
      if (index == numGases() && index < 4) {
        return addGas(type, fraction);
      }

      unsigned typeIndex = mf_getGasTypeFieldIndex(index);
      unsigned fractionIndex = mf_getGasFractionFieldIndex(index);

      OptionalString oldType = getString(typeIndex, true);
      bool result = setString(typeIndex, type);
      if (!result) {
        return false;
      }
      result = setDouble(fractionIndex, fraction);
      if (!result) {
        if (oldType) {
          setString(typeIndex, *oldType);
        } else {
          setString(typeIndex, "");
        }
      }

      return result;
    }

    bool GasMixture_Impl::setGasType(unsigned index, const std::string& type) {
      if (index >= numGases()) {
        LOG(Info, "Cannot set the gas type at index " << index << " in GasMixture " << briefDescription() << ". The current number of gases is "
                                                      << numGases() << ".");
        return false;
      }
      return setString(mf_getGasTypeFieldIndex(index), type);
    }

    bool GasMixture_Impl::setGasFraction(unsigned index, double fraction) {
      if (index >= numGases()) {
        LOG(Info, "Cannot set the gas fraction at index " << index << " in GasMixture " << briefDescription() << ". The current number of gases is "
                                                          << numGases() << ".");
        return false;
      }
      return setDouble(mf_getGasFractionFieldIndex(index), fraction);
    }

    bool GasMixture_Impl::addGas(const std::string& type, double fraction) {
      if (numGases() == 4) {
        LOG(Info, "Cannot add any more gases to GasMixture " << briefDescription() << ".");
        return false;
      }

      unsigned index = numGases();
      bool ok = setUnsigned(OS_WindowMaterial_GasMixtureFields::NumberofGasesinMixture, index + 1);
      OS_ASSERT(ok);

      bool result = setGas(index, type, fraction);

      if (!result) {
        ok = setUnsigned(OS_WindowMaterial_GasMixtureFields::NumberofGasesinMixture, index);
        OS_ASSERT(ok);
      }

      return result;
    }

    bool GasMixture_Impl::setFractions(const std::vector<double>& fractions) {
      if (fractions.size() != numGases()) {
        LOG(Info, "Cannot set GasMixture " << briefDescription() << " fractions using vector " << "of length " << fractions.size()
                                           << ", because the object currently lists " << numGases() << " gases.");
        return false;
      }
      bool result = true;
      double fullSum(0.0);
      std::vector<std::string> rollbackValues;
      for (unsigned i = 0, n = numGases(); i < n; ++i) {
        // be able to rollback previously set values if necessary
        unsigned index = mf_getGasFractionFieldIndex(i);
        rollbackValues.push_back(getString(index, true).get());

        // determine where we are with respect to adding up to 1.0
        double value = fractions[i];
        double minusOneSum = fullSum;
        fullSum += value;
        if (openstudio::greaterThanOrEqual(minusOneSum, 1.0)) {
          LOG(Warn, "Unable to set gas fractions for GasMixture " << briefDescription() << " because the partial sum has met or exceeded 1.0");
          result = false;
          break;
        }
        if ((i == n - 1) && (!openstudio::equal(fullSum, 1.0))) {
          LOG(Warn, "The sum of the fractions entered for GasMixture " << briefDescription() << " equal " << fullSum
                                                                       << ", rather than 1.0, so the last entry will be adjusted " << "accordingly.");
          value = 1.0 - minusOneSum;
        }
        result = setDouble(index, value);
        if (!result) {
          break;
        }
        index += 2;
      }
      if (!result) {
        for (unsigned j = 0, nr = rollbackValues.size(); j < nr; ++j) {
          setString(mf_getGasFractionFieldIndex(j), rollbackValues[j]);
        }
      }
      return result;
    }

    bool GasMixture_Impl::balanceFractions(unsigned index) {
      if (index >= numGases()) {
        return false;
      }
      double partialSum(0.0);
      for (unsigned i = 0, n = numGases(); i < n; ++i) {
        if (i != index) {
          partialSum += getGasFraction(i);
        }
      }
      if (openstudio::greaterThanOrEqual(partialSum, 1.0)) {
        LOG(Warn, "Cannot balance gas fractions for GasMixture " << briefDescription() << " because the partial sum has met or exceeded 1.0");
        return false;
      }
      return setGasFraction(index, 1.0 - partialSum);
    }

    unsigned GasMixture_Impl::numGases() const {
      OptionalUnsigned ou = getUnsigned(OS_WindowMaterial_GasMixtureFields::NumberofGasesinMixture, true);
      OS_ASSERT(ou);
      return *ou;
    }

    unsigned GasMixture_Impl::mf_getGasTypeFieldIndex(unsigned gasIndex) const {
      unsigned result(0);
      switch (gasIndex) {
        case 0:
          result = OS_WindowMaterial_GasMixtureFields::Gas1Type;
          break;
        case 1:
          result = OS_WindowMaterial_GasMixtureFields::Gas2Type;
          break;
        case 2:
          result = OS_WindowMaterial_GasMixtureFields::Gas3Type;
          break;
        case 3:
          result = OS_WindowMaterial_GasMixtureFields::Gas4Type;
          break;
        default:
          LOG_AND_THROW("GasMixture only supports four gases, and is indexed starting with 0. Cannot " << "return the gas type at index " << gasIndex
                                                                                                       << ".");
      };
      OS_ASSERT(result > 0);
      return result;
    }

    unsigned GasMixture_Impl::mf_getGasFractionFieldIndex(unsigned gasIndex) const {
      unsigned result(0);
      switch (gasIndex) {
        case 0:
          result = OS_WindowMaterial_GasMixtureFields::Gas1Fraction;
          break;
        case 1:
          result = OS_WindowMaterial_GasMixtureFields::Gas2Fraction;
          break;
        case 2:
          result = OS_WindowMaterial_GasMixtureFields::Gas3Fraction;
          break;
        case 3:
          result = OS_WindowMaterial_GasMixtureFields::Gas4Fraction;
          break;
        default:
          LOG_AND_THROW("GasMixture only supports four gases, and is indexed starting with 0. Cannot " << "return the gas fraction at index "
                                                                                                       << gasIndex << ".");
      };
      OS_ASSERT(result > 0);
      return result;
    }

    double GasMixture_Impl::thickness() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_GasMixtureFields::Thickness, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool GasMixture_Impl::setThickness(double thickness) {
      bool result = setDouble(OS_WindowMaterial_GasMixtureFields::Thickness, thickness);
      return result;
    }

    int GasMixture_Impl::numberofGasesinMixture() const {
      boost::optional<int> value = getInt(OS_WindowMaterial_GasMixtureFields::NumberofGasesinMixture, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool GasMixture_Impl::setNumberofGasesinMixture(int numberofGasesinMixture) {
      bool result = setInt(OS_WindowMaterial_GasMixtureFields::NumberofGasesinMixture, numberofGasesinMixture);
      return result;
    }

    std::string GasMixture_Impl::gas1Type() const {
      boost::optional<std::string> value = getString(OS_WindowMaterial_GasMixtureFields::Gas1Type, true);
      OS_ASSERT(value);
      return value.get();
    }

    double GasMixture_Impl::gas1Fraction() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_GasMixtureFields::Gas1Fraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string GasMixture_Impl::gas2Type() const {
      boost::optional<std::string> value = getString(OS_WindowMaterial_GasMixtureFields::Gas2Type, true);
      OS_ASSERT(value);
      return value.get();
    }

    double GasMixture_Impl::gas2Fraction() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_GasMixtureFields::Gas2Fraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string GasMixture_Impl::gas3Type() const {
      boost::optional<std::string> value = getString(OS_WindowMaterial_GasMixtureFields::Gas3Type, true);
      if (value) {
        return *value;
      } else {
        return {};
      }
    }

    boost::optional<double> GasMixture_Impl::gas3Fraction() const {
      return getDouble(OS_WindowMaterial_GasMixtureFields::Gas3Fraction, true);
    }

    std::string GasMixture_Impl::gas4Type() const {
      boost::optional<std::string> value = getString(OS_WindowMaterial_GasMixtureFields::Gas4Type, true);
      if (value) {
        return *value;
      } else {
        return {};
      }
    }

    boost::optional<double> GasMixture_Impl::gas4Fraction() const {
      return getDouble(OS_WindowMaterial_GasMixtureFields::Gas4Fraction, true);
    }

    bool GasMixture_Impl::setGas1Type(const std::string& gas1Type) {
      bool result = setString(OS_WindowMaterial_GasMixtureFields::Gas1Type, gas1Type);
      return result;
    }

    bool GasMixture_Impl::setGas1Fraction(double gas1Fraction) {
      bool result = setDouble(OS_WindowMaterial_GasMixtureFields::Gas1Fraction, gas1Fraction);
      return result;
    }

    bool GasMixture_Impl::setGas2Type(const std::string& gas2Type) {
      bool result = setString(OS_WindowMaterial_GasMixtureFields::Gas2Type, gas2Type);
      return result;
    }

    bool GasMixture_Impl::setGas2Fraction(double gas2Fraction) {
      bool result = setDouble(OS_WindowMaterial_GasMixtureFields::Gas2Fraction, gas2Fraction);
      return result;
    }

    bool GasMixture_Impl::setGas3Type(boost::optional<std::string> gas3Type) {
      bool result(false);
      if (gas3Type) {
        result = setString(OS_WindowMaterial_GasMixtureFields::Gas3Type, gas3Type.get());
      } else {
        resetGas3Type();
        result = true;
      }
      return result;
    }

    void GasMixture_Impl::resetGas1Type() {
      bool result = setString(OS_WindowMaterial_GasMixtureFields::Gas1Type, "");
      OS_ASSERT(result);
    }

    void GasMixture_Impl::resetGas2Type() {
      bool result = setString(OS_WindowMaterial_GasMixtureFields::Gas2Type, "");
      OS_ASSERT(result);
    }

    void GasMixture_Impl::resetGas3Type() {
      bool result = setString(OS_WindowMaterial_GasMixtureFields::Gas3Type, "");
      OS_ASSERT(result);
    }

    bool GasMixture_Impl::setGas3Fraction(boost::optional<double> gas3Fraction) {
      bool result(false);
      if (gas3Fraction) {
        result = setDouble(OS_WindowMaterial_GasMixtureFields::Gas3Fraction, gas3Fraction.get());
      } else {
        resetGas3Fraction();
        result = true;
      }
      return result;
    }

    void GasMixture_Impl::resetGas1Fraction() {
      bool result = setString(OS_WindowMaterial_GasMixtureFields::Gas1Fraction, "");
      OS_ASSERT(result);
    }

    void GasMixture_Impl::resetGas2Fraction() {
      bool result = setString(OS_WindowMaterial_GasMixtureFields::Gas2Fraction, "");
      OS_ASSERT(result);
    }

    void GasMixture_Impl::resetGas3Fraction() {
      bool result = setString(OS_WindowMaterial_GasMixtureFields::Gas3Fraction, "");
      OS_ASSERT(result);
    }

    bool GasMixture_Impl::setGas4Type(boost::optional<std::string> gas4Type) {
      bool result(false);
      if (gas4Type) {
        result = setString(OS_WindowMaterial_GasMixtureFields::Gas4Type, gas4Type.get());
      } else {
        resetGas4Type();
        result = true;
      }
      return result;
    }

    void GasMixture_Impl::resetGas4Type() {
      bool result = setString(OS_WindowMaterial_GasMixtureFields::Gas4Type, "");
      OS_ASSERT(result);
    }

    bool GasMixture_Impl::setGas4Fraction(boost::optional<double> gas4Fraction) {
      bool result(false);
      if (gas4Fraction) {
        result = setDouble(OS_WindowMaterial_GasMixtureFields::Gas4Fraction, gas4Fraction.get());
      } else {
        resetGas4Fraction();
        result = true;
      }
      return result;
    }

    void GasMixture_Impl::resetGas4Fraction() {
      bool result = setString(OS_WindowMaterial_GasMixtureFields::Gas4Fraction, "");
      OS_ASSERT(result);
    }

    std::vector<std::string> GasMixture_Impl::gas1TypeValues() const {
      return GasMixture::gas1TypeValues();
    }

    std::vector<std::string> GasMixture_Impl::gas2TypeValues() const {
      return GasMixture::gas2TypeValues();
    }

    std::vector<std::string> GasMixture_Impl::gas3TypeValues() const {
      return GasMixture::gas3TypeValues();
    }

    std::vector<std::string> GasMixture_Impl::gas4TypeValues() const {
      return GasMixture::gas4TypeValues();
    }

  }  // namespace detail

  GasMixture::GasMixture(const Model& model, int numberofGasesinMixture, double thickness, std::string gas1Type, double gas1Fraction,
                         std::string gas2Type, double gas2Fraction, std::string gas3Type, double gas3Fraction, std::string gas4Type,
                         double gas4Fraction)
    : GasLayer(GasMixture::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::GasMixture_Impl>());

    // TODO: Appropriately handle the following required object-list fields.
    bool ok = true;
    // ok = setHandle();
    OS_ASSERT(ok);
    ok = setThickness(thickness);
    OS_ASSERT(ok);
    ok = setNumberofGasesinMixture(numberofGasesinMixture);
    OS_ASSERT(ok);
    ok = setGas1Type(gas1Type);
    OS_ASSERT(ok);
    ok = setGas1Fraction(gas1Fraction);
    OS_ASSERT(ok);
    ok = setGas2Type(gas2Type);
    OS_ASSERT(ok);
    ok = setGas2Fraction(gas2Fraction);
    OS_ASSERT(ok);
    ok = setGas3Type(gas3Type);
    OS_ASSERT(ok);
    ok = setGas3Fraction(gas3Fraction);
    OS_ASSERT(ok);
    ok = setGas4Type(gas4Type);
    OS_ASSERT(ok);
    ok = setGas4Fraction(gas4Fraction);
    OS_ASSERT(ok);
  }

  IddObjectType GasMixture::iddObjectType() {
    return {IddObjectType::OS_WindowMaterial_GasMixture};
  }

  std::vector<std::string> GasMixture::validGasTypes() {
    static const StringVector result =
      getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_WindowMaterial_GasMixtureFields::Gas1Type);
    OS_ASSERT(!result.empty());
    return result;
  }

  std::string GasMixture::getGasType(unsigned index) const {
    return getImpl<detail::GasMixture_Impl>()->getGasType(index);
  }

  double GasMixture::getGasFraction(unsigned index) const {
    return getImpl<detail::GasMixture_Impl>()->getGasFraction(index);
  }

  double GasMixture::getThermalConductivity(double temperature) const {
    return getImpl<detail::GasMixture_Impl>()->getThermalConductivity(temperature);
  }

  double GasMixture::getThermalConductance(double temperature) const {
    return getImpl<detail::GasMixture_Impl>()->getThermalConductance(temperature);
  }

  double GasMixture::getThermalResistivity(double temperature) const {
    return getImpl<detail::GasMixture_Impl>()->getThermalResistivity(temperature);
  }

  double GasMixture::getThermalResistance(double temperature) const {
    return getImpl<detail::GasMixture_Impl>()->getThermalResistance(temperature);
  }

  double GasMixture::getViscosity(double temperature) const {
    return getImpl<detail::GasMixture_Impl>()->getViscosity(temperature);
  }

  double GasMixture::getSpecificHeat(double temperature) const {
    return getImpl<detail::GasMixture_Impl>()->getSpecificHeat(temperature);
  }

  bool GasMixture::setGas(unsigned index, const std::string& type, double fraction) {
    return getImpl<detail::GasMixture_Impl>()->setGas(index, type, fraction);
  }

  bool GasMixture::setGasType(unsigned index, const std::string& type) {
    return getImpl<detail::GasMixture_Impl>()->setGasType(index, type);
  }

  bool GasMixture::setGasFraction(unsigned index, double fraction) {
    return getImpl<detail::GasMixture_Impl>()->setGasFraction(index, fraction);
  }

  bool GasMixture::addGas(const std::string& type, double fraction) {
    return getImpl<detail::GasMixture_Impl>()->addGas(type, fraction);
  }

  bool GasMixture::setFractions(const std::vector<double>& fractions) {
    return getImpl<detail::GasMixture_Impl>()->setFractions(fractions);
  }

  bool GasMixture::balanceFractions(unsigned index) {
    return getImpl<detail::GasMixture_Impl>()->balanceFractions(index);
  }

  unsigned GasMixture::numGases() const {
    return getImpl<detail::GasMixture_Impl>()->numGases();
  }

  std::vector<std::string> GasMixture::gas1TypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_WindowMaterial_GasMixtureFields::Gas1Type);
  }

  std::vector<std::string> GasMixture::gas2TypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_WindowMaterial_GasMixtureFields::Gas2Type);
  }

  std::vector<std::string> GasMixture::gas3TypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_WindowMaterial_GasMixtureFields::Gas3Type);
  }

  std::vector<std::string> GasMixture::gas4TypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_WindowMaterial_GasMixtureFields::Gas4Type);
  }

  double GasMixture::thickness() const {
    return getImpl<detail::GasMixture_Impl>()->thickness();
  }

  bool GasMixture::setThickness(double thickness) {
    return getImpl<detail::GasMixture_Impl>()->setThickness(thickness);
  }

  int GasMixture::numberofGasesinMixture() const {
    return getImpl<detail::GasMixture_Impl>()->numberofGasesinMixture();
  }

  bool GasMixture::setNumberofGasesinMixture(int numberofGasesinMixture) {
    return getImpl<detail::GasMixture_Impl>()->setNumberofGasesinMixture(numberofGasesinMixture);
  }

  std::string GasMixture::gas1Type() const {
    return getImpl<detail::GasMixture_Impl>()->gas1Type();
  }

  double GasMixture::gas1Fraction() const {
    return getImpl<detail::GasMixture_Impl>()->gas1Fraction();
  }

  std::string GasMixture::gas2Type() const {
    return getImpl<detail::GasMixture_Impl>()->gas2Type();
  }

  double GasMixture::gas2Fraction() const {
    return getImpl<detail::GasMixture_Impl>()->gas2Fraction();
  }

  std::string GasMixture::gas3Type() const {
    return getImpl<detail::GasMixture_Impl>()->gas3Type();
  }

  boost::optional<double> GasMixture::gas3Fraction() const {
    return getImpl<detail::GasMixture_Impl>()->gas3Fraction();
  }

  std::string GasMixture::gas4Type() const {
    return getImpl<detail::GasMixture_Impl>()->gas4Type();
  }

  boost::optional<double> GasMixture::gas4Fraction() const {
    return getImpl<detail::GasMixture_Impl>()->gas4Fraction();
  }

  bool GasMixture::setGas1Type(const std::string& gas1Type) {
    return getImpl<detail::GasMixture_Impl>()->setGas1Type(gas1Type);
  }

  void GasMixture::resetGas1Type() {
    getImpl<detail::GasMixture_Impl>()->resetGas1Type();
  }

  bool GasMixture::setGas1Fraction(double gas1Fraction) {
    return getImpl<detail::GasMixture_Impl>()->setGas1Fraction(gas1Fraction);
  }

  void GasMixture::resetGas1Fraction() {
    getImpl<detail::GasMixture_Impl>()->resetGas1Fraction();
  }

  bool GasMixture::setGas2Type(const std::string& gas2Type) {
    return getImpl<detail::GasMixture_Impl>()->setGas2Type(gas2Type);
  }

  void GasMixture::resetGas2Type() {
    getImpl<detail::GasMixture_Impl>()->resetGas2Type();
  }

  bool GasMixture::setGas2Fraction(double gas2Fraction) {
    return getImpl<detail::GasMixture_Impl>()->setGas2Fraction(gas2Fraction);
  }

  void GasMixture::resetGas2Fraction() {
    getImpl<detail::GasMixture_Impl>()->resetGas2Fraction();
  }

  bool GasMixture::setGas3Type(const std::string& gas3Type) {
    return getImpl<detail::GasMixture_Impl>()->setGas3Type(gas3Type);
  }

  void GasMixture::resetGas3Type() {
    getImpl<detail::GasMixture_Impl>()->resetGas3Type();
  }

  bool GasMixture::setGas3Fraction(double gas3Fraction) {
    return getImpl<detail::GasMixture_Impl>()->setGas3Fraction(gas3Fraction);
  }

  void GasMixture::resetGas3Fraction() {
    getImpl<detail::GasMixture_Impl>()->resetGas3Fraction();
  }

  bool GasMixture::setGas4Type(const std::string& gas4Type) {
    return getImpl<detail::GasMixture_Impl>()->setGas4Type(gas4Type);
  }

  void GasMixture::resetGas4Type() {
    getImpl<detail::GasMixture_Impl>()->resetGas4Type();
  }

  bool GasMixture::setGas4Fraction(double gas4Fraction) {
    return getImpl<detail::GasMixture_Impl>()->setGas4Fraction(gas4Fraction);
  }

  void GasMixture::resetGas4Fraction() {
    getImpl<detail::GasMixture_Impl>()->resetGas4Fraction();
  }

  /// @cond
  GasMixture::GasMixture(std::shared_ptr<detail::GasMixture_Impl> impl) : GasLayer(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
