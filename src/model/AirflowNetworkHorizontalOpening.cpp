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

#include "AirflowNetworkHorizontalOpening.hpp"
#include "AirflowNetworkHorizontalOpening_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AirflowNetworkHorizontalOpening_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    AirflowNetworkHorizontalOpening_Impl::AirflowNetworkHorizontalOpening_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : AirflowNetworkComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AirflowNetworkHorizontalOpening::iddObjectType());
    }

    AirflowNetworkHorizontalOpening_Impl::AirflowNetworkHorizontalOpening_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                               Model_Impl* model, bool keepHandle)
      : AirflowNetworkComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AirflowNetworkHorizontalOpening::iddObjectType());
    }

    AirflowNetworkHorizontalOpening_Impl::AirflowNetworkHorizontalOpening_Impl(const AirflowNetworkHorizontalOpening_Impl& other, Model_Impl* model,
                                                                               bool keepHandle)
      : AirflowNetworkComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& AirflowNetworkHorizontalOpening_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType AirflowNetworkHorizontalOpening_Impl::iddObjectType() const {
      return AirflowNetworkHorizontalOpening::iddObjectType();
    }

    double AirflowNetworkHorizontalOpening_Impl::airMassFlowCoefficientWhenOpeningisClosed() const {
      boost::optional<double> value = getDouble(OS_AirflowNetworkHorizontalOpeningFields::AirMassFlowCoefficientWhenOpeningisClosed, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirflowNetworkHorizontalOpening_Impl::airMassFlowExponentWhenOpeningisClosed() const {
      boost::optional<double> value = getDouble(OS_AirflowNetworkHorizontalOpeningFields::AirMassFlowExponentWhenOpeningisClosed, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirflowNetworkHorizontalOpening_Impl::isAirMassFlowExponentWhenOpeningisClosedDefaulted() const {
      return isEmpty(OS_AirflowNetworkHorizontalOpeningFields::AirMassFlowExponentWhenOpeningisClosed);
    }

    double AirflowNetworkHorizontalOpening_Impl::slopingPlaneAngle() const {
      boost::optional<double> value = getDouble(OS_AirflowNetworkHorizontalOpeningFields::SlopingPlaneAngle, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirflowNetworkHorizontalOpening_Impl::isSlopingPlaneAngleDefaulted() const {
      return isEmpty(OS_AirflowNetworkHorizontalOpeningFields::SlopingPlaneAngle);
    }

    double AirflowNetworkHorizontalOpening_Impl::dischargeCoefficient() const {
      boost::optional<double> value = getDouble(OS_AirflowNetworkHorizontalOpeningFields::DischargeCoefficient, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirflowNetworkHorizontalOpening_Impl::setAirMassFlowCoefficientWhenOpeningisClosed(double airMassFlowCoefficientWhenOpeningisClosed) {
      bool result =
        setDouble(OS_AirflowNetworkHorizontalOpeningFields::AirMassFlowCoefficientWhenOpeningisClosed, airMassFlowCoefficientWhenOpeningisClosed);
      return result;
    }

    bool AirflowNetworkHorizontalOpening_Impl::setAirMassFlowExponentWhenOpeningisClosed(double airMassFlowExponentWhenOpeningisClosed) {
      bool result =
        setDouble(OS_AirflowNetworkHorizontalOpeningFields::AirMassFlowExponentWhenOpeningisClosed, airMassFlowExponentWhenOpeningisClosed);
      return result;
    }

    void AirflowNetworkHorizontalOpening_Impl::resetAirMassFlowExponentWhenOpeningisClosed() {
      bool result = setString(OS_AirflowNetworkHorizontalOpeningFields::AirMassFlowExponentWhenOpeningisClosed, "");
      OS_ASSERT(result);
    }

    bool AirflowNetworkHorizontalOpening_Impl::setSlopingPlaneAngle(double slopingPlaneAngle) {
      bool result = setDouble(OS_AirflowNetworkHorizontalOpeningFields::SlopingPlaneAngle, slopingPlaneAngle);
      return result;
    }

    void AirflowNetworkHorizontalOpening_Impl::resetSlopingPlaneAngle() {
      bool result = setString(OS_AirflowNetworkHorizontalOpeningFields::SlopingPlaneAngle, "");
      OS_ASSERT(result);
    }

    bool AirflowNetworkHorizontalOpening_Impl::setDischargeCoefficient(double dischargeCoefficient) {
      bool result = setDouble(OS_AirflowNetworkHorizontalOpeningFields::DischargeCoefficient, dischargeCoefficient);
      return result;
    }

  }  // namespace detail

  AirflowNetworkHorizontalOpening::AirflowNetworkHorizontalOpening(const Model& model, double massFlowCoefficientWhenOpeningisClosed,
                                                                   double dischargeCoefficient)
    : AirflowNetworkComponent(AirflowNetworkHorizontalOpening::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirflowNetworkHorizontalOpening_Impl>());

    // TODO: Appropriately handle the following required object-list fields.
    //OS_ASSERT(setHandle());
    OS_ASSERT(setAirMassFlowCoefficientWhenOpeningisClosed(massFlowCoefficientWhenOpeningisClosed));
    OS_ASSERT(setDischargeCoefficient(dischargeCoefficient));
  }

  AirflowNetworkHorizontalOpening::AirflowNetworkHorizontalOpening(const Model& model, double massFlowCoefficientWhenOpeningisClosed,
                                                                   double massFlowExponentWhenOpeningisClosed, double slopingPlaneAngle,
                                                                   double dischargeCoefficient)
    : AirflowNetworkComponent(AirflowNetworkHorizontalOpening::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirflowNetworkHorizontalOpening_Impl>());

    bool ok = setAirMassFlowCoefficientWhenOpeningisClosed(massFlowCoefficientWhenOpeningisClosed);
    OS_ASSERT(ok);
    ok = setAirMassFlowExponentWhenOpeningisClosed(massFlowExponentWhenOpeningisClosed);
    OS_ASSERT(ok);
    ok = setSlopingPlaneAngle(slopingPlaneAngle);
    OS_ASSERT(ok);
    ok = setDischargeCoefficient(dischargeCoefficient);
    OS_ASSERT(ok);
  }

  IddObjectType AirflowNetworkHorizontalOpening::iddObjectType() {
    return IddObjectType(IddObjectType::OS_AirflowNetworkHorizontalOpening);
  }

  double AirflowNetworkHorizontalOpening::airMassFlowCoefficientWhenOpeningisClosed() const {
    return getImpl<detail::AirflowNetworkHorizontalOpening_Impl>()->airMassFlowCoefficientWhenOpeningisClosed();
  }

  double AirflowNetworkHorizontalOpening::airMassFlowExponentWhenOpeningisClosed() const {
    return getImpl<detail::AirflowNetworkHorizontalOpening_Impl>()->airMassFlowExponentWhenOpeningisClosed();
  }

  bool AirflowNetworkHorizontalOpening::isAirMassFlowExponentWhenOpeningisClosedDefaulted() const {
    return getImpl<detail::AirflowNetworkHorizontalOpening_Impl>()->isAirMassFlowExponentWhenOpeningisClosedDefaulted();
  }

  double AirflowNetworkHorizontalOpening::slopingPlaneAngle() const {
    return getImpl<detail::AirflowNetworkHorizontalOpening_Impl>()->slopingPlaneAngle();
  }

  bool AirflowNetworkHorizontalOpening::isSlopingPlaneAngleDefaulted() const {
    return getImpl<detail::AirflowNetworkHorizontalOpening_Impl>()->isSlopingPlaneAngleDefaulted();
  }

  double AirflowNetworkHorizontalOpening::dischargeCoefficient() const {
    return getImpl<detail::AirflowNetworkHorizontalOpening_Impl>()->dischargeCoefficient();
  }

  bool AirflowNetworkHorizontalOpening::setAirMassFlowCoefficientWhenOpeningisClosed(double airMassFlowCoefficientWhenOpeningisClosed) {
    return getImpl<detail::AirflowNetworkHorizontalOpening_Impl>()->setAirMassFlowCoefficientWhenOpeningisClosed(
      airMassFlowCoefficientWhenOpeningisClosed);
  }

  bool AirflowNetworkHorizontalOpening::setAirMassFlowExponentWhenOpeningisClosed(double airMassFlowExponentWhenOpeningisClosed) {
    return getImpl<detail::AirflowNetworkHorizontalOpening_Impl>()->setAirMassFlowExponentWhenOpeningisClosed(airMassFlowExponentWhenOpeningisClosed);
  }

  void AirflowNetworkHorizontalOpening::resetAirMassFlowExponentWhenOpeningisClosed() {
    getImpl<detail::AirflowNetworkHorizontalOpening_Impl>()->resetAirMassFlowExponentWhenOpeningisClosed();
  }

  bool AirflowNetworkHorizontalOpening::setSlopingPlaneAngle(double slopingPlaneAngle) {
    return getImpl<detail::AirflowNetworkHorizontalOpening_Impl>()->setSlopingPlaneAngle(slopingPlaneAngle);
  }

  void AirflowNetworkHorizontalOpening::resetSlopingPlaneAngle() {
    getImpl<detail::AirflowNetworkHorizontalOpening_Impl>()->resetSlopingPlaneAngle();
  }

  bool AirflowNetworkHorizontalOpening::setDischargeCoefficient(double dischargeCoefficient) {
    return getImpl<detail::AirflowNetworkHorizontalOpening_Impl>()->setDischargeCoefficient(dischargeCoefficient);
  }

  /// @cond
  AirflowNetworkHorizontalOpening::AirflowNetworkHorizontalOpening(std::shared_ptr<detail::AirflowNetworkHorizontalOpening_Impl> impl)
    : AirflowNetworkComponent(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
