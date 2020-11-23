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

#include "AirflowNetworkReferenceCrackConditions.hpp"
#include "AirflowNetworkReferenceCrackConditions_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AirflowNetworkReferenceCrackConditions_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    AirflowNetworkReferenceCrackConditions_Impl::AirflowNetworkReferenceCrackConditions_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                             bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AirflowNetworkReferenceCrackConditions::iddObjectType());
    }

    AirflowNetworkReferenceCrackConditions_Impl::AirflowNetworkReferenceCrackConditions_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                             Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AirflowNetworkReferenceCrackConditions::iddObjectType());
    }

    AirflowNetworkReferenceCrackConditions_Impl::AirflowNetworkReferenceCrackConditions_Impl(const AirflowNetworkReferenceCrackConditions_Impl& other,
                                                                                             Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& AirflowNetworkReferenceCrackConditions_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType AirflowNetworkReferenceCrackConditions_Impl::iddObjectType() const {
      return AirflowNetworkReferenceCrackConditions::iddObjectType();
    }

    double AirflowNetworkReferenceCrackConditions_Impl::referenceTemperature() const {
      boost::optional<double> value = getDouble(OS_AirflowNetworkReferenceCrackConditionsFields::ReferenceTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirflowNetworkReferenceCrackConditions_Impl::isReferenceTemperatureDefaulted() const {
      return isEmpty(OS_AirflowNetworkReferenceCrackConditionsFields::ReferenceTemperature);
    }

    double AirflowNetworkReferenceCrackConditions_Impl::referenceBarometricPressure() const {
      boost::optional<double> value = getDouble(OS_AirflowNetworkReferenceCrackConditionsFields::ReferenceBarometricPressure, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirflowNetworkReferenceCrackConditions_Impl::isReferenceBarometricPressureDefaulted() const {
      return isEmpty(OS_AirflowNetworkReferenceCrackConditionsFields::ReferenceBarometricPressure);
    }

    double AirflowNetworkReferenceCrackConditions_Impl::referenceHumidityRatio() const {
      boost::optional<double> value = getDouble(OS_AirflowNetworkReferenceCrackConditionsFields::ReferenceHumidityRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirflowNetworkReferenceCrackConditions_Impl::isReferenceHumidityRatioDefaulted() const {
      return isEmpty(OS_AirflowNetworkReferenceCrackConditionsFields::ReferenceHumidityRatio);
    }

    void AirflowNetworkReferenceCrackConditions_Impl::setReferenceTemperature(double referenceTemperature) {
      bool result = setDouble(OS_AirflowNetworkReferenceCrackConditionsFields::ReferenceTemperature, referenceTemperature);
      OS_ASSERT(result);
    }

    void AirflowNetworkReferenceCrackConditions_Impl::resetReferenceTemperature() {
      bool result = setString(OS_AirflowNetworkReferenceCrackConditionsFields::ReferenceTemperature, "");
      OS_ASSERT(result);
    }

    bool AirflowNetworkReferenceCrackConditions_Impl::setReferenceBarometricPressure(double referenceBarometricPressure) {
      bool result = setDouble(OS_AirflowNetworkReferenceCrackConditionsFields::ReferenceBarometricPressure, referenceBarometricPressure);
      return result;
    }

    void AirflowNetworkReferenceCrackConditions_Impl::resetReferenceBarometricPressure() {
      bool result = setString(OS_AirflowNetworkReferenceCrackConditionsFields::ReferenceBarometricPressure, "");
      OS_ASSERT(result);
    }

    void AirflowNetworkReferenceCrackConditions_Impl::setReferenceHumidityRatio(double referenceHumidityRatio) {
      bool result = setDouble(OS_AirflowNetworkReferenceCrackConditionsFields::ReferenceHumidityRatio, referenceHumidityRatio);
      OS_ASSERT(result);
    }

    void AirflowNetworkReferenceCrackConditions_Impl::resetReferenceHumidityRatio() {
      bool result = setString(OS_AirflowNetworkReferenceCrackConditionsFields::ReferenceHumidityRatio, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  AirflowNetworkReferenceCrackConditions::AirflowNetworkReferenceCrackConditions(const Model& model)
    : ModelObject(AirflowNetworkReferenceCrackConditions::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirflowNetworkReferenceCrackConditions_Impl>());

    // TODO: Appropriately handle the following required object-list fields.
  }

  AirflowNetworkReferenceCrackConditions::AirflowNetworkReferenceCrackConditions(const Model& model, double temperature, double barometricPressure,
                                                                                 double humidityRatio)
    : ModelObject(AirflowNetworkReferenceCrackConditions::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirflowNetworkReferenceCrackConditions_Impl>());

    setTemperature(temperature);
    setBarometricPressure(barometricPressure);
    setHumidityRatio(humidityRatio);
  }

  IddObjectType AirflowNetworkReferenceCrackConditions::iddObjectType() {
    return IddObjectType(IddObjectType::OS_AirflowNetworkReferenceCrackConditions);
  }

  double AirflowNetworkReferenceCrackConditions::temperature() const {
    return getImpl<detail::AirflowNetworkReferenceCrackConditions_Impl>()->referenceTemperature();
  }

  bool AirflowNetworkReferenceCrackConditions::isTemperatureDefaulted() const {
    return getImpl<detail::AirflowNetworkReferenceCrackConditions_Impl>()->isReferenceTemperatureDefaulted();
  }

  double AirflowNetworkReferenceCrackConditions::barometricPressure() const {
    return getImpl<detail::AirflowNetworkReferenceCrackConditions_Impl>()->referenceBarometricPressure();
  }

  bool AirflowNetworkReferenceCrackConditions::isBarometricPressureDefaulted() const {
    return getImpl<detail::AirflowNetworkReferenceCrackConditions_Impl>()->isReferenceBarometricPressureDefaulted();
  }

  double AirflowNetworkReferenceCrackConditions::humidityRatio() const {
    return getImpl<detail::AirflowNetworkReferenceCrackConditions_Impl>()->referenceHumidityRatio();
  }

  bool AirflowNetworkReferenceCrackConditions::isHumidityRatioDefaulted() const {
    return getImpl<detail::AirflowNetworkReferenceCrackConditions_Impl>()->isReferenceHumidityRatioDefaulted();
  }

  void AirflowNetworkReferenceCrackConditions::setTemperature(double referenceTemperature) {
    getImpl<detail::AirflowNetworkReferenceCrackConditions_Impl>()->setReferenceTemperature(referenceTemperature);
  }

  void AirflowNetworkReferenceCrackConditions::resetTemperature() {
    getImpl<detail::AirflowNetworkReferenceCrackConditions_Impl>()->resetReferenceTemperature();
  }

  bool AirflowNetworkReferenceCrackConditions::setBarometricPressure(double referenceBarometricPressure) {
    return getImpl<detail::AirflowNetworkReferenceCrackConditions_Impl>()->setReferenceBarometricPressure(referenceBarometricPressure);
  }

  void AirflowNetworkReferenceCrackConditions::resetBarometricPressure() {
    getImpl<detail::AirflowNetworkReferenceCrackConditions_Impl>()->resetReferenceBarometricPressure();
  }

  void AirflowNetworkReferenceCrackConditions::setHumidityRatio(double referenceHumidityRatio) {
    getImpl<detail::AirflowNetworkReferenceCrackConditions_Impl>()->setReferenceHumidityRatio(referenceHumidityRatio);
  }

  void AirflowNetworkReferenceCrackConditions::resetHumidityRatio() {
    getImpl<detail::AirflowNetworkReferenceCrackConditions_Impl>()->resetReferenceHumidityRatio();
  }

  /// @cond
  AirflowNetworkReferenceCrackConditions::AirflowNetworkReferenceCrackConditions(
    std::shared_ptr<detail::AirflowNetworkReferenceCrackConditions_Impl> impl)
    : ModelObject(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
