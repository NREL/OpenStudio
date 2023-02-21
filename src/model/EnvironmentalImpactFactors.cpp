/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "EnvironmentalImpactFactors.hpp"
#include "EnvironmentalImpactFactors_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_EnvironmentalImpactFactors_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    EnvironmentalImpactFactors_Impl::EnvironmentalImpactFactors_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == EnvironmentalImpactFactors::iddObjectType());
    }

    EnvironmentalImpactFactors_Impl::EnvironmentalImpactFactors_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                     bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == EnvironmentalImpactFactors::iddObjectType());
    }

    EnvironmentalImpactFactors_Impl::EnvironmentalImpactFactors_Impl(const EnvironmentalImpactFactors_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& EnvironmentalImpactFactors_Impl::outputVariableNames() const {
      static std::vector<std::string> result;
      if (result.empty()) {
      }
      return result;
    }

    IddObjectType EnvironmentalImpactFactors_Impl::iddObjectType() const {
      return EnvironmentalImpactFactors::iddObjectType();
    }

    double EnvironmentalImpactFactors_Impl::districtHeatingEfficiency() const {
      boost::optional<double> value = getDouble(OS_EnvironmentalImpactFactorsFields::DistrictHeatingEfficiency, true);
      OS_ASSERT(value);
      return value.get();
    }

    double EnvironmentalImpactFactors_Impl::districtCoolingCOP() const {
      boost::optional<double> value = getDouble(OS_EnvironmentalImpactFactorsFields::DistrictCoolingCOP, true);
      OS_ASSERT(value);
      return value.get();
    }

    double EnvironmentalImpactFactors_Impl::steamConversionEfficiency() const {
      boost::optional<double> value = getDouble(OS_EnvironmentalImpactFactorsFields::SteamConversionEfficiency, true);
      OS_ASSERT(value);
      return value.get();
    }

    double EnvironmentalImpactFactors_Impl::totalCarbonEquivalentEmissionFactorFromN2O() const {
      boost::optional<double> value = getDouble(OS_EnvironmentalImpactFactorsFields::TotalCarbonEquivalentEmissionFactorFromN2O, true);
      OS_ASSERT(value);
      return value.get();
    }

    double EnvironmentalImpactFactors_Impl::totalCarbonEquivalentEmissionFactorFromCH4() const {
      boost::optional<double> value = getDouble(OS_EnvironmentalImpactFactorsFields::TotalCarbonEquivalentEmissionFactorFromCH4, true);
      OS_ASSERT(value);
      return value.get();
    }

    double EnvironmentalImpactFactors_Impl::totalCarbonEquivalentEmissionFactorFromCO2() const {
      boost::optional<double> value = getDouble(OS_EnvironmentalImpactFactorsFields::TotalCarbonEquivalentEmissionFactorFromCO2, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool EnvironmentalImpactFactors_Impl::setDistrictHeatingEfficiency(double districtHeatingEfficiency) {
      bool result = setDouble(OS_EnvironmentalImpactFactorsFields::DistrictHeatingEfficiency, districtHeatingEfficiency);
      return result;
    }

    bool EnvironmentalImpactFactors_Impl::setDistrictCoolingCOP(double districtCoolingCOP) {
      bool result = setDouble(OS_EnvironmentalImpactFactorsFields::DistrictCoolingCOP, districtCoolingCOP);
      return result;
    }

    bool EnvironmentalImpactFactors_Impl::setSteamConversionEfficiency(double steamConversionEfficiency) {
      bool result = setDouble(OS_EnvironmentalImpactFactorsFields::SteamConversionEfficiency, steamConversionEfficiency);
      return result;
    }

    bool EnvironmentalImpactFactors_Impl::setTotalCarbonEquivalentEmissionFactorFromN2O(double totalCarbonEquivalentEmissionFactorFromN2O) {
      bool result =
        setDouble(OS_EnvironmentalImpactFactorsFields::TotalCarbonEquivalentEmissionFactorFromN2O, totalCarbonEquivalentEmissionFactorFromN2O);
      OS_ASSERT(result);
      return result;
    }

    bool EnvironmentalImpactFactors_Impl::setTotalCarbonEquivalentEmissionFactorFromCH4(double totalCarbonEquivalentEmissionFactorFromCH4) {
      bool result =
        setDouble(OS_EnvironmentalImpactFactorsFields::TotalCarbonEquivalentEmissionFactorFromCH4, totalCarbonEquivalentEmissionFactorFromCH4);
      OS_ASSERT(result);
      return result;
    }

    bool EnvironmentalImpactFactors_Impl::setTotalCarbonEquivalentEmissionFactorFromCO2(double totalCarbonEquivalentEmissionFactorFromCO2) {
      bool result =
        setDouble(OS_EnvironmentalImpactFactorsFields::TotalCarbonEquivalentEmissionFactorFromCO2, totalCarbonEquivalentEmissionFactorFromCO2);
      OS_ASSERT(result);
      return result;
    }

  }  // namespace detail

  EnvironmentalImpactFactors::EnvironmentalImpactFactors(Model& model) : ModelObject(EnvironmentalImpactFactors::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::EnvironmentalImpactFactors_Impl>());

    // Defaults from E+ IDD, captured at v9.5.0
    setDistrictHeatingEfficiency(0.3);
    setDistrictCoolingCOP(3.0);
    setSteamConversionEfficiency(0.25);
    setTotalCarbonEquivalentEmissionFactorFromN2O(80.7272);
    setTotalCarbonEquivalentEmissionFactorFromCH4(6.2727);
    setTotalCarbonEquivalentEmissionFactorFromCO2(0.2727);
  }

  IddObjectType EnvironmentalImpactFactors::iddObjectType() {
    return {IddObjectType::OS_EnvironmentalImpactFactors};
  }

  double EnvironmentalImpactFactors::districtHeatingEfficiency() const {
    return getImpl<detail::EnvironmentalImpactFactors_Impl>()->districtHeatingEfficiency();
  }

  double EnvironmentalImpactFactors::districtCoolingCOP() const {
    return getImpl<detail::EnvironmentalImpactFactors_Impl>()->districtCoolingCOP();
  }

  double EnvironmentalImpactFactors::steamConversionEfficiency() const {
    return getImpl<detail::EnvironmentalImpactFactors_Impl>()->steamConversionEfficiency();
  }

  double EnvironmentalImpactFactors::totalCarbonEquivalentEmissionFactorFromN2O() const {
    return getImpl<detail::EnvironmentalImpactFactors_Impl>()->totalCarbonEquivalentEmissionFactorFromN2O();
  }

  double EnvironmentalImpactFactors::totalCarbonEquivalentEmissionFactorFromCH4() const {
    return getImpl<detail::EnvironmentalImpactFactors_Impl>()->totalCarbonEquivalentEmissionFactorFromCH4();
  }

  double EnvironmentalImpactFactors::totalCarbonEquivalentEmissionFactorFromCO2() const {
    return getImpl<detail::EnvironmentalImpactFactors_Impl>()->totalCarbonEquivalentEmissionFactorFromCO2();
  }

  bool EnvironmentalImpactFactors::setDistrictHeatingEfficiency(double districtHeatingEfficiency) {
    return getImpl<detail::EnvironmentalImpactFactors_Impl>()->setDistrictHeatingEfficiency(districtHeatingEfficiency);
  }

  bool EnvironmentalImpactFactors::setDistrictCoolingCOP(double districtCoolingCOP) {
    return getImpl<detail::EnvironmentalImpactFactors_Impl>()->setDistrictCoolingCOP(districtCoolingCOP);
  }

  bool EnvironmentalImpactFactors::setSteamConversionEfficiency(double steamConversionEfficiency) {
    return getImpl<detail::EnvironmentalImpactFactors_Impl>()->setSteamConversionEfficiency(steamConversionEfficiency);
  }

  bool EnvironmentalImpactFactors::setTotalCarbonEquivalentEmissionFactorFromN2O(double totalCarbonEquivalentEmissionFactorFromN2O) {
    return getImpl<detail::EnvironmentalImpactFactors_Impl>()->setTotalCarbonEquivalentEmissionFactorFromN2O(
      totalCarbonEquivalentEmissionFactorFromN2O);
  }

  bool EnvironmentalImpactFactors::setTotalCarbonEquivalentEmissionFactorFromCH4(double totalCarbonEquivalentEmissionFactorFromCH4) {
    return getImpl<detail::EnvironmentalImpactFactors_Impl>()->setTotalCarbonEquivalentEmissionFactorFromCH4(
      totalCarbonEquivalentEmissionFactorFromCH4);
  }

  bool EnvironmentalImpactFactors::setTotalCarbonEquivalentEmissionFactorFromCO2(double totalCarbonEquivalentEmissionFactorFromCO2) {
    return getImpl<detail::EnvironmentalImpactFactors_Impl>()->setTotalCarbonEquivalentEmissionFactorFromCO2(
      totalCarbonEquivalentEmissionFactorFromCO2);
  }

  /// @cond
  EnvironmentalImpactFactors::EnvironmentalImpactFactors(std::shared_ptr<detail::EnvironmentalImpactFactors_Impl> impl) : ModelObject(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
