/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "FenestrationMaterial.hpp"
#include "FenestrationMaterial_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    FenestrationMaterial_Impl::FenestrationMaterial_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : Material_Impl(idfObject, model, keepHandle) {}

    FenestrationMaterial_Impl::FenestrationMaterial_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : Material_Impl(other, model, keepHandle) {}

    FenestrationMaterial_Impl::FenestrationMaterial_Impl(const FenestrationMaterial_Impl& other, Model_Impl* model, bool keepHandle)
      : Material_Impl(other, model, keepHandle) {}

  }  // namespace detail

  double FenestrationMaterial::ufactorInsideAirTemperature() {
    static const double result = 294.261111111;
    return result;
  }

  double FenestrationMaterial::ufactorOutsideAirTemperature() {
    static const double result = 255.372222222;
    return result;
  }

  const std::vector<double> FenestrationMaterial::airThermalConductivityCoefficients() {
    std::vector<double> result(3);
    result[0] = 2.873E-3;
    result[1] = 7.760E-5;
    result[2] = 0.0;
    return result;
  }

  const std::vector<double> FenestrationMaterial::airViscosityCoefficients() {
    std::vector<double> result(3);
    result[0] = 3.723E-6;
    result[1] = 4.940E-8;
    result[2] = 0.0;
    return result;
  }

  const std::vector<double> FenestrationMaterial::airSpecificHeatCoefficients() {
    std::vector<double> result(3);
    result[0] = 1002.737;
    result[1] = 1.2324E-2;
    result[2] = 0.0;
    return result;
  }

  double FenestrationMaterial::airMolecularWeight() {
    static const double result(28.97);
    return result;
  }

  const std::vector<double> FenestrationMaterial::argonThermalConductivityCoefficients() {
    std::vector<double> result(3);
    result[0] = 2.285E-3;
    result[1] = 5.149E-5;
    result[2] = 0.0;
    return result;
  }

  const std::vector<double> FenestrationMaterial::argonViscosityCoefficients() {
    std::vector<double> result(3);
    result[0] = 3.379E-6;
    result[1] = 6.451E-8;
    result[2] = 0.0;
    return result;
  }

  const std::vector<double> FenestrationMaterial::argonSpecificHeatCoefficients() {
    std::vector<double> result(3);
    result[0] = 521.929;
    result[1] = 0.0;
    result[2] = 0.0;
    return result;
  }

  double FenestrationMaterial::argonMolecularWeight() {
    static const double result(39.94);
    return result;
  }

  const std::vector<double> FenestrationMaterial::kryptonThermalConductivityCoefficients() {
    std::vector<double> result(3);
    result[0] = 9.443E-4;
    result[1] = 2.826E-5;
    result[2] = 0.0;
    return result;
  }

  const std::vector<double> FenestrationMaterial::kryptonViscosityCoefficients() {
    std::vector<double> result(3);
    result[0] = 2.213E-6;
    result[1] = 7.777E-8;
    result[2] = 0.0;
    return result;
  }

  const std::vector<double> FenestrationMaterial::kryptonSpecificHeatCoefficients() {
    std::vector<double> result(3);
    result[0] = 248.091;
    result[1] = 0.0;
    result[2] = 0.0;
    return result;
  }

  double FenestrationMaterial::kryptonMolecularWeight() {
    static const double result(83.8);
    return result;
  }

  const std::vector<double> FenestrationMaterial::xenonThermalConductivityCoefficients() {
    std::vector<double> result(3);
    result[0] = 4.538E-4;
    result[1] = 1.723E-5;
    result[2] = 0.0;
    return result;
  }

  const std::vector<double> FenestrationMaterial::xenonViscosityCoefficients() {
    std::vector<double> result(3);
    result[0] = 1.069E-6;
    result[1] = 7.414E-8;
    result[2] = 0.0;
    return result;
  }

  const std::vector<double> FenestrationMaterial::xenonSpecificHeatCoefficients() {
    std::vector<double> result(3);
    result[0] = 158.34;
    result[1] = 0.0;
    result[2] = 0.0;
    return result;
  }

  double FenestrationMaterial::xenonMolecularWeight() {
    static const double result(131.29);
    return result;
  }

  /// @cond
  FenestrationMaterial::FenestrationMaterial(IddObjectType type, const Model& model) : Material(type, model) {
    OS_ASSERT(getImpl<detail::FenestrationMaterial_Impl>());
  }

  FenestrationMaterial::FenestrationMaterial(std::shared_ptr<detail::FenestrationMaterial_Impl> impl) : Material(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
