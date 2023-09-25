/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <vector>
#include <string>
#include "SiteGroundTemperatureUndisturbedKusudaAchenbach.hpp"
#include "SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Site.hpp"
#include "Site_Impl.hpp"

#include <utilities/idd/OS_Site_GroundTemperature_Undisturbed_KusudaAchenbach_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"
#include "../utilities/time/Date.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl::SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl(const IdfObject& idfObject,
                                                                                                               Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == SiteGroundTemperatureUndisturbedKusudaAchenbach::iddObjectType());
    }

    SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl::SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == SiteGroundTemperatureUndisturbedKusudaAchenbach::iddObjectType());
    }

    SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl::SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl(
      const SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    boost::optional<ParentObject> SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl::parent() const {
      boost::optional<Site> result = this->model().getOptionalUniqueModelObject<Site>();
      return boost::optional<ParentObject>(result);
    }

    const std::vector<std::string>& SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl::iddObjectType() const {
      return SiteGroundTemperatureUndisturbedKusudaAchenbach::iddObjectType();
    }

    double SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl::soilThermalConductivity() const {
      boost::optional<double> value = getDouble(OS_Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::SoilThermalConductivity, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl::soilDensity() const {
      boost::optional<double> value = getDouble(OS_Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::SoilDensity, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl::soilSpecificHeat() const {
      boost::optional<double> value = getDouble(OS_Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::SoilSpecificHeat, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl::averageSoilSurfaceTemperature() const {
      return getDouble(OS_Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::AverageSoilSurfaceTemperature, true);
    }

    boost::optional<double> SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl::averageAmplitudeofSurfaceTemperature() const {
      return getDouble(OS_Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::AverageAmplitudeofSurfaceTemperature, true);
    }

    boost::optional<double> SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl::phaseShiftofMinimumSurfaceTemperature() const {
      return getDouble(OS_Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::PhaseShiftofMinimumSurfaceTemperature, true);
    }

    bool SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl::setSoilThermalConductivity(double soilThermalConductivity) {
      bool result = setDouble(OS_Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::SoilThermalConductivity, soilThermalConductivity);
      OS_ASSERT(result);
      return result;
    }

    bool SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl::setSoilDensity(double soilDensity) {
      bool result = setDouble(OS_Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::SoilDensity, soilDensity);
      OS_ASSERT(result);
      return result;
    }

    bool SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl::setSoilSpecificHeat(double soilSpecificHeat) {
      bool result = setDouble(OS_Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::SoilSpecificHeat, soilSpecificHeat);
      OS_ASSERT(result);
      return result;
    }

    bool SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl::setAverageSoilSurfaceTemperature(double averageSoilSurfaceTemperature) {
      bool result =
        setDouble(OS_Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::AverageSoilSurfaceTemperature, averageSoilSurfaceTemperature);
      OS_ASSERT(result);
      return result;
    }

    void SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl::resetAverageSoilSurfaceTemperature() {
      bool result = setString(OS_Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::AverageSoilSurfaceTemperature, "");
      OS_ASSERT(result);
    }

    bool SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl::setAverageAmplitudeofSurfaceTemperature(double averageAmplitudeofSurfaceTemperature) {
      bool result = setDouble(OS_Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::AverageAmplitudeofSurfaceTemperature,
                              averageAmplitudeofSurfaceTemperature);
      OS_ASSERT(result);
      return result;
    }

    void SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl::resetAverageAmplitudeofSurfaceTemperature() {
      bool result = setString(OS_Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::AverageAmplitudeofSurfaceTemperature, "");
      OS_ASSERT(result);
    }

    bool
      SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl::setPhaseShiftofMinimumSurfaceTemperature(double phaseShiftofMinimumSurfaceTemperature) {
      bool result = setDouble(OS_Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::PhaseShiftofMinimumSurfaceTemperature,
                              phaseShiftofMinimumSurfaceTemperature);
      OS_ASSERT(result);
      return result;
    }

    void SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl::resetPhaseShiftofMinimumSurfaceTemperature() {
      bool result = setString(OS_Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::PhaseShiftofMinimumSurfaceTemperature, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  SiteGroundTemperatureUndisturbedKusudaAchenbach::SiteGroundTemperatureUndisturbedKusudaAchenbach(const Model& model)
    : ModelObject(SiteGroundTemperatureUndisturbedKusudaAchenbach::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl>());

    bool ok = setSoilThermalConductivity(1.08);  // E+ docs
    OS_ASSERT(ok);
    ok = setSoilDensity(962);  // E+ docs
    OS_ASSERT(ok);
    ok = setSoilSpecificHeat(2576);  // E+ docs
    OS_ASSERT(ok);
    ok = setAverageSoilSurfaceTemperature(15.5);
    OS_ASSERT(ok);
    ok = setAverageAmplitudeofSurfaceTemperature(12.8);
    OS_ASSERT(ok);
    ok = setPhaseShiftofMinimumSurfaceTemperature(17.3);
    OS_ASSERT(ok);
  }

  IddObjectType SiteGroundTemperatureUndisturbedKusudaAchenbach::iddObjectType() {
    IddObjectType result(IddObjectType::OS_Site_GroundTemperature_Undisturbed_KusudaAchenbach);
    return result;
  }

  double SiteGroundTemperatureUndisturbedKusudaAchenbach::soilThermalConductivity() const {
    return getImpl<detail::SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl>()->soilThermalConductivity();
  }

  double SiteGroundTemperatureUndisturbedKusudaAchenbach::soilDensity() const {
    return getImpl<detail::SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl>()->soilDensity();
  }

  double SiteGroundTemperatureUndisturbedKusudaAchenbach::soilSpecificHeat() const {
    return getImpl<detail::SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl>()->soilSpecificHeat();
  }

  boost::optional<double> SiteGroundTemperatureUndisturbedKusudaAchenbach::averageSoilSurfaceTemperature() const {
    return getImpl<detail::SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl>()->averageSoilSurfaceTemperature();
  }

  boost::optional<double> SiteGroundTemperatureUndisturbedKusudaAchenbach::averageAmplitudeofSurfaceTemperature() const {
    return getImpl<detail::SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl>()->averageAmplitudeofSurfaceTemperature();
  }

  boost::optional<double> SiteGroundTemperatureUndisturbedKusudaAchenbach::phaseShiftofMinimumSurfaceTemperature() const {
    return getImpl<detail::SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl>()->phaseShiftofMinimumSurfaceTemperature();
  }

  bool SiteGroundTemperatureUndisturbedKusudaAchenbach::setSoilThermalConductivity(double soilThermalConductivity) {
    return getImpl<detail::SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl>()->setSoilThermalConductivity(soilThermalConductivity);
  }

  bool SiteGroundTemperatureUndisturbedKusudaAchenbach::setSoilDensity(double soilDensity) {
    return getImpl<detail::SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl>()->setSoilDensity(soilDensity);
  }

  bool SiteGroundTemperatureUndisturbedKusudaAchenbach::setSoilSpecificHeat(double soilSpecificHeat) {
    return getImpl<detail::SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl>()->setSoilSpecificHeat(soilSpecificHeat);
  }

  bool SiteGroundTemperatureUndisturbedKusudaAchenbach::setAverageSoilSurfaceTemperature(double averageSoilSurfaceTemperature) {
    return getImpl<detail::SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl>()->setAverageSoilSurfaceTemperature(averageSoilSurfaceTemperature);
  }

  void SiteGroundTemperatureUndisturbedKusudaAchenbach::resetAverageSoilSurfaceTemperature() {
    getImpl<detail::SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl>()->resetAverageSoilSurfaceTemperature();
  }

  bool SiteGroundTemperatureUndisturbedKusudaAchenbach::setAverageAmplitudeofSurfaceTemperature(double averageAmplitudeofSurfaceTemperature) {
    return getImpl<detail::SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl>()->setAverageAmplitudeofSurfaceTemperature(
      averageAmplitudeofSurfaceTemperature);
  }

  void SiteGroundTemperatureUndisturbedKusudaAchenbach::resetAverageAmplitudeofSurfaceTemperature() {
    getImpl<detail::SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl>()->resetAverageAmplitudeofSurfaceTemperature();
  }

  bool SiteGroundTemperatureUndisturbedKusudaAchenbach::setPhaseShiftofMinimumSurfaceTemperature(double phaseShiftofMinimumSurfaceTemperature) {
    return getImpl<detail::SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl>()->setPhaseShiftofMinimumSurfaceTemperature(
      phaseShiftofMinimumSurfaceTemperature);
  }

  void SiteGroundTemperatureUndisturbedKusudaAchenbach::resetPhaseShiftofMinimumSurfaceTemperature() {
    getImpl<detail::SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl>()->resetPhaseShiftofMinimumSurfaceTemperature();
  }

  /// @cond
  SiteGroundTemperatureUndisturbedKusudaAchenbach::SiteGroundTemperatureUndisturbedKusudaAchenbach(
    std::shared_ptr<detail::SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl> impl)
    : ModelObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
