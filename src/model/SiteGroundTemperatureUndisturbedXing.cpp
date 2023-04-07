/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include <vector>
#include <string>
#include "SiteGroundTemperatureUndisturbedXing.hpp"
#include "SiteGroundTemperatureUndisturbedXing_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Site.hpp"
#include "Site_Impl.hpp"

#include <utilities/idd/OS_Site_GroundTemperature_Undisturbed_Xing_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"
#include "../utilities/time/Date.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    SiteGroundTemperatureUndisturbedXing_Impl::SiteGroundTemperatureUndisturbedXing_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                         bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == SiteGroundTemperatureUndisturbedXing::iddObjectType());
    }

    SiteGroundTemperatureUndisturbedXing_Impl::SiteGroundTemperatureUndisturbedXing_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                         Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == SiteGroundTemperatureUndisturbedXing::iddObjectType());
    }

    SiteGroundTemperatureUndisturbedXing_Impl::SiteGroundTemperatureUndisturbedXing_Impl(const SiteGroundTemperatureUndisturbedXing_Impl& other,
                                                                                         Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    boost::optional<ParentObject> SiteGroundTemperatureUndisturbedXing_Impl::parent() const {
      boost::optional<Site> result = this->model().getOptionalUniqueModelObject<Site>();
      return boost::optional<ParentObject>(result);
    }

    const std::vector<std::string>& SiteGroundTemperatureUndisturbedXing_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType SiteGroundTemperatureUndisturbedXing_Impl::iddObjectType() const {
      return SiteGroundTemperatureUndisturbedXing::iddObjectType();
    }

    double SiteGroundTemperatureUndisturbedXing_Impl::soilThermalConductivity() const {
      boost::optional<double> value = getDouble(OS_Site_GroundTemperature_Undisturbed_XingFields::SoilThermalConductivity, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SiteGroundTemperatureUndisturbedXing_Impl::soilDensity() const {
      boost::optional<double> value = getDouble(OS_Site_GroundTemperature_Undisturbed_XingFields::SoilDensity, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SiteGroundTemperatureUndisturbedXing_Impl::soilSpecificHeat() const {
      boost::optional<double> value = getDouble(OS_Site_GroundTemperature_Undisturbed_XingFields::SoilSpecificHeat, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SiteGroundTemperatureUndisturbedXing_Impl::averageSoilSurfaceTemperature() const {
      boost::optional<double> value = getDouble(OS_Site_GroundTemperature_Undisturbed_XingFields::AverageSoilSurfaceTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SiteGroundTemperatureUndisturbedXing_Impl::soilSurfaceTemperatureAmplitude1() const {
      boost::optional<double> value = getDouble(OS_Site_GroundTemperature_Undisturbed_XingFields::SoilSurfaceTemperatureAmplitude1, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SiteGroundTemperatureUndisturbedXing_Impl::soilSurfaceTemperatureAmplitude2() const {
      boost::optional<double> value = getDouble(OS_Site_GroundTemperature_Undisturbed_XingFields::SoilSurfaceTemperatureAmplitude2, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SiteGroundTemperatureUndisturbedXing_Impl::phaseShiftofTemperatureAmplitude1() const {
      boost::optional<double> value = getDouble(OS_Site_GroundTemperature_Undisturbed_XingFields::PhaseShiftofTemperatureAmplitude1, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SiteGroundTemperatureUndisturbedXing_Impl::phaseShiftofTemperatureAmplitude2() const {
      boost::optional<double> value = getDouble(OS_Site_GroundTemperature_Undisturbed_XingFields::PhaseShiftofTemperatureAmplitude2, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SiteGroundTemperatureUndisturbedXing_Impl::setSoilThermalConductivity(double soilThermalConductivity) {
      bool result = setDouble(OS_Site_GroundTemperature_Undisturbed_XingFields::SoilThermalConductivity, soilThermalConductivity);
      OS_ASSERT(result);
      return result;
    }

    bool SiteGroundTemperatureUndisturbedXing_Impl::setSoilDensity(double soilDensity) {
      bool result = setDouble(OS_Site_GroundTemperature_Undisturbed_XingFields::SoilDensity, soilDensity);
      OS_ASSERT(result);
      return result;
    }

    bool SiteGroundTemperatureUndisturbedXing_Impl::setSoilSpecificHeat(double soilSpecificHeat) {
      bool result = setDouble(OS_Site_GroundTemperature_Undisturbed_XingFields::SoilSpecificHeat, soilSpecificHeat);
      OS_ASSERT(result);
      return result;
    }

    bool SiteGroundTemperatureUndisturbedXing_Impl::setAverageSoilSurfaceTemperature(double averageSoilSurfaceTemperature) {
      bool result = setDouble(OS_Site_GroundTemperature_Undisturbed_XingFields::AverageSoilSurfaceTemperature, averageSoilSurfaceTemperature);
      OS_ASSERT(result);
      return result;
    }

    bool SiteGroundTemperatureUndisturbedXing_Impl::setSoilSurfaceTemperatureAmplitude1(double soilSurfaceTemperatureAmplitude1) {
      bool result = setDouble(OS_Site_GroundTemperature_Undisturbed_XingFields::SoilSurfaceTemperatureAmplitude1, soilSurfaceTemperatureAmplitude1);
      OS_ASSERT(result);
      return result;
    }

    bool SiteGroundTemperatureUndisturbedXing_Impl::setSoilSurfaceTemperatureAmplitude2(double soilSurfaceTemperatureAmplitude2) {
      bool result = setDouble(OS_Site_GroundTemperature_Undisturbed_XingFields::SoilSurfaceTemperatureAmplitude2, soilSurfaceTemperatureAmplitude2);
      OS_ASSERT(result);
      return result;
    }

    bool SiteGroundTemperatureUndisturbedXing_Impl::setPhaseShiftofTemperatureAmplitude1(double phaseShiftofTemperatureAmplitude1) {
      bool result = setDouble(OS_Site_GroundTemperature_Undisturbed_XingFields::PhaseShiftofTemperatureAmplitude1, phaseShiftofTemperatureAmplitude1);
      OS_ASSERT(result);
      return result;
    }

    bool SiteGroundTemperatureUndisturbedXing_Impl::setPhaseShiftofTemperatureAmplitude2(double phaseShiftofTemperatureAmplitude2) {
      bool result = setDouble(OS_Site_GroundTemperature_Undisturbed_XingFields::PhaseShiftofTemperatureAmplitude2, phaseShiftofTemperatureAmplitude2);
      OS_ASSERT(result);
      return result;
    }

  }  // namespace detail

  SiteGroundTemperatureUndisturbedXing::SiteGroundTemperatureUndisturbedXing(const Model& model)
    : ModelObject(SiteGroundTemperatureUndisturbedXing::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::SiteGroundTemperatureUndisturbedXing_Impl>());

    bool ok = setSoilThermalConductivity(1.08);  // E+ docs
    OS_ASSERT(ok);
    ok = setSoilDensity(962);  // E+ docs
    OS_ASSERT(ok);
    ok = setSoilSpecificHeat(2576);  // E+ docs
    OS_ASSERT(ok);
    ok = setAverageSoilSurfaceTemperature(11.1);  // E+ docs
    OS_ASSERT(ok);
    ok = setSoilSurfaceTemperatureAmplitude1(13.4);  // E+ docs
    OS_ASSERT(ok);
    ok = setSoilSurfaceTemperatureAmplitude2(0.7);  // E+ docs
    OS_ASSERT(ok);
    ok = setPhaseShiftofTemperatureAmplitude1(25);  // E+ docs
    OS_ASSERT(ok);
    ok = setPhaseShiftofTemperatureAmplitude2(30);  // E+ docs
    OS_ASSERT(ok);
  }

  IddObjectType SiteGroundTemperatureUndisturbedXing::iddObjectType() {
    IddObjectType result(IddObjectType::OS_Site_GroundTemperature_Undisturbed_Xing);
    return result;
  }

  double SiteGroundTemperatureUndisturbedXing::soilThermalConductivity() const {
    return getImpl<detail::SiteGroundTemperatureUndisturbedXing_Impl>()->soilThermalConductivity();
  }

  double SiteGroundTemperatureUndisturbedXing::soilDensity() const {
    return getImpl<detail::SiteGroundTemperatureUndisturbedXing_Impl>()->soilDensity();
  }

  double SiteGroundTemperatureUndisturbedXing::soilSpecificHeat() const {
    return getImpl<detail::SiteGroundTemperatureUndisturbedXing_Impl>()->soilSpecificHeat();
  }

  double SiteGroundTemperatureUndisturbedXing::averageSoilSurfaceTemperature() const {
    return getImpl<detail::SiteGroundTemperatureUndisturbedXing_Impl>()->averageSoilSurfaceTemperature();
  }

  double SiteGroundTemperatureUndisturbedXing::soilSurfaceTemperatureAmplitude1() const {
    return getImpl<detail::SiteGroundTemperatureUndisturbedXing_Impl>()->soilSurfaceTemperatureAmplitude1();
  }

  double SiteGroundTemperatureUndisturbedXing::soilSurfaceTemperatureAmplitude2() const {
    return getImpl<detail::SiteGroundTemperatureUndisturbedXing_Impl>()->soilSurfaceTemperatureAmplitude2();
  }

  double SiteGroundTemperatureUndisturbedXing::phaseShiftofTemperatureAmplitude1() const {
    return getImpl<detail::SiteGroundTemperatureUndisturbedXing_Impl>()->phaseShiftofTemperatureAmplitude1();
  }

  double SiteGroundTemperatureUndisturbedXing::phaseShiftofTemperatureAmplitude2() const {
    return getImpl<detail::SiteGroundTemperatureUndisturbedXing_Impl>()->phaseShiftofTemperatureAmplitude2();
  }

  bool SiteGroundTemperatureUndisturbedXing::setSoilThermalConductivity(double soilThermalConductivity) {
    return getImpl<detail::SiteGroundTemperatureUndisturbedXing_Impl>()->setSoilThermalConductivity(soilThermalConductivity);
  }

  bool SiteGroundTemperatureUndisturbedXing::setSoilDensity(double soilDensity) {
    return getImpl<detail::SiteGroundTemperatureUndisturbedXing_Impl>()->setSoilDensity(soilDensity);
  }

  bool SiteGroundTemperatureUndisturbedXing::setSoilSpecificHeat(double soilSpecificHeat) {
    return getImpl<detail::SiteGroundTemperatureUndisturbedXing_Impl>()->setSoilSpecificHeat(soilSpecificHeat);
  }

  bool SiteGroundTemperatureUndisturbedXing::setAverageSoilSurfaceTemperature(double averageSoilSurfaceTemperature) {
    return getImpl<detail::SiteGroundTemperatureUndisturbedXing_Impl>()->setAverageSoilSurfaceTemperature(averageSoilSurfaceTemperature);
  }

  bool SiteGroundTemperatureUndisturbedXing::setSoilSurfaceTemperatureAmplitude1(double soilSurfaceTemperatureAmplitude1) {
    return getImpl<detail::SiteGroundTemperatureUndisturbedXing_Impl>()->setSoilSurfaceTemperatureAmplitude1(soilSurfaceTemperatureAmplitude1);
  }

  bool SiteGroundTemperatureUndisturbedXing::setSoilSurfaceTemperatureAmplitude2(double soilSurfaceTemperatureAmplitude2) {
    return getImpl<detail::SiteGroundTemperatureUndisturbedXing_Impl>()->setSoilSurfaceTemperatureAmplitude2(soilSurfaceTemperatureAmplitude2);
  }

  bool SiteGroundTemperatureUndisturbedXing::setPhaseShiftofTemperatureAmplitude1(double phaseShiftofTemperatureAmplitude1) {
    return getImpl<detail::SiteGroundTemperatureUndisturbedXing_Impl>()->setPhaseShiftofTemperatureAmplitude1(phaseShiftofTemperatureAmplitude1);
  }

  bool SiteGroundTemperatureUndisturbedXing::setPhaseShiftofTemperatureAmplitude2(double phaseShiftofTemperatureAmplitude2) {
    return getImpl<detail::SiteGroundTemperatureUndisturbedXing_Impl>()->setPhaseShiftofTemperatureAmplitude2(phaseShiftofTemperatureAmplitude2);
  }

  /// @cond
  SiteGroundTemperatureUndisturbedXing::SiteGroundTemperatureUndisturbedXing(std::shared_ptr<detail::SiteGroundTemperatureUndisturbedXing_Impl> impl)
    : ModelObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
