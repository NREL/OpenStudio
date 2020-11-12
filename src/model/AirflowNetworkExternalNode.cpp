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

#include "AirflowNetworkExternalNode.hpp"
#include "AirflowNetworkExternalNode_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "CurveLinear.hpp"
#include "CurveLinear_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AirflowNetworkExternalNode_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    AirflowNetworkExternalNode_Impl::AirflowNetworkExternalNode_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : AirflowNetworkNode_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AirflowNetworkExternalNode::iddObjectType());
    }

    AirflowNetworkExternalNode_Impl::AirflowNetworkExternalNode_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                     bool keepHandle)
      : AirflowNetworkNode_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AirflowNetworkExternalNode::iddObjectType());
    }

    AirflowNetworkExternalNode_Impl::AirflowNetworkExternalNode_Impl(const AirflowNetworkExternalNode_Impl& other, Model_Impl* model, bool keepHandle)
      : AirflowNetworkNode_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& AirflowNetworkExternalNode_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType AirflowNetworkExternalNode_Impl::iddObjectType() const {
      return AirflowNetworkExternalNode::iddObjectType();
    }

    double AirflowNetworkExternalNode_Impl::externalNodeHeight() const {
      boost::optional<double> value = getDouble(OS_AirflowNetworkExternalNodeFields::ExternalNodeHeight, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirflowNetworkExternalNode_Impl::isExternalNodeHeightDefaulted() const {
      return isEmpty(OS_AirflowNetworkExternalNodeFields::ExternalNodeHeight);
    }

    Curve AirflowNetworkExternalNode_Impl::windPressureCoefficientCurve() const {
      boost::optional<Curve> value = optionalWindPressureCoefficientCurve();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Wind Pressure Coefficient Curve attached.");
      }
      return value.get();
    }

    bool AirflowNetworkExternalNode_Impl::symmetricWindPressureCoefficientCurve() const {
      boost::optional<std::string> value = getString(OS_AirflowNetworkExternalNodeFields::SymmetricWindPressureCoefficientCurve, true);
      OS_ASSERT(value);
      return openstudio::istringEqual(value.get(), "Yes");
    }

    bool AirflowNetworkExternalNode_Impl::isSymmetricWindPressureCoefficientCurveDefaulted() const {
      return isEmpty(OS_AirflowNetworkExternalNodeFields::SymmetricWindPressureCoefficientCurve);
    }

    std::string AirflowNetworkExternalNode_Impl::windAngleType() const {
      boost::optional<std::string> value = getString(OS_AirflowNetworkExternalNodeFields::WindAngleType, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirflowNetworkExternalNode_Impl::isWindAngleTypeDefaulted() const {
      return isEmpty(OS_AirflowNetworkExternalNodeFields::WindAngleType);
    }

    void AirflowNetworkExternalNode_Impl::setExternalNodeHeight(double externalNodeHeight) {
      bool result = setDouble(OS_AirflowNetworkExternalNodeFields::ExternalNodeHeight, externalNodeHeight);
      OS_ASSERT(result);
    }

    void AirflowNetworkExternalNode_Impl::resetExternalNodeHeight() {
      bool result = setString(OS_AirflowNetworkExternalNodeFields::ExternalNodeHeight, "");
      OS_ASSERT(result);
    }

    bool AirflowNetworkExternalNode_Impl::setWindPressureCoefficientCurve(const Curve& wPCValue) {
      bool result = setPointer(OS_AirflowNetworkExternalNodeFields::WindPressureCoefficientCurveName, wPCValue.handle());
      return result;
    }

    void AirflowNetworkExternalNode_Impl::setSymmetricWindPressureCoefficientCurve(bool symmetricWindPressureCoefficientCurve) {
      bool result = false;
      if (symmetricWindPressureCoefficientCurve) {
        result = setString(OS_AirflowNetworkExternalNodeFields::SymmetricWindPressureCoefficientCurve, "Yes");
      } else {
        result = setString(OS_AirflowNetworkExternalNodeFields::SymmetricWindPressureCoefficientCurve, "No");
      }
      OS_ASSERT(result);
    }

    void AirflowNetworkExternalNode_Impl::resetSymmetricWindPressureCoefficientCurve() {
      bool result = setString(OS_AirflowNetworkExternalNodeFields::SymmetricWindPressureCoefficientCurve, "");
      OS_ASSERT(result);
    }

    bool AirflowNetworkExternalNode_Impl::setWindAngleType(const std::string& windAngleType) {
      bool result = setString(OS_AirflowNetworkExternalNodeFields::WindAngleType, windAngleType);
      return result;
    }

    void AirflowNetworkExternalNode_Impl::resetWindAngleType() {
      bool result = setString(OS_AirflowNetworkExternalNodeFields::WindAngleType, "");
      OS_ASSERT(result);
    }

    boost::optional<Curve> AirflowNetworkExternalNode_Impl::optionalWindPressureCoefficientCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_AirflowNetworkExternalNodeFields::WindPressureCoefficientCurveName);
    }

  }  // namespace detail

  AirflowNetworkExternalNode::AirflowNetworkExternalNode(const Model& model)
    : AirflowNetworkNode(AirflowNetworkExternalNode::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirflowNetworkExternalNode_Impl>());
    // Set up a default unity-valued curve
    CurveLinear curve(model);
    curve.setCoefficient1Constant(1.0);
    curve.setCoefficient2x(0.0);
    curve.setMinimumValueofx(0.0);
    curve.setMaximumValueofx(360.0);
    bool ok = setWindPressureCoefficientCurve(curve);
    OS_ASSERT(ok);
  }

  AirflowNetworkExternalNode::AirflowNetworkExternalNode(const Model& model, const Curve& curve)
    : AirflowNetworkNode(AirflowNetworkExternalNode::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirflowNetworkExternalNode_Impl>());
    bool ok = setWindPressureCoefficientCurve(curve);
    OS_ASSERT(ok);
  }

  IddObjectType AirflowNetworkExternalNode::iddObjectType() {
    return IddObjectType(IddObjectType::OS_AirflowNetworkExternalNode);
  }

  std::vector<std::string> AirflowNetworkExternalNode::windAngleTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_AirflowNetworkExternalNodeFields::WindAngleType);
  }

  double AirflowNetworkExternalNode::externalNodeHeight() const {
    return getImpl<detail::AirflowNetworkExternalNode_Impl>()->externalNodeHeight();
  }

  bool AirflowNetworkExternalNode::isExternalNodeHeightDefaulted() const {
    return getImpl<detail::AirflowNetworkExternalNode_Impl>()->isExternalNodeHeightDefaulted();
  }

  Curve AirflowNetworkExternalNode::windPressureCoefficientCurve() const {
    return getImpl<detail::AirflowNetworkExternalNode_Impl>()->windPressureCoefficientCurve();
  }

  bool AirflowNetworkExternalNode::symmetricWindPressureCoefficientCurve() const {
    return getImpl<detail::AirflowNetworkExternalNode_Impl>()->symmetricWindPressureCoefficientCurve();
  }

  bool AirflowNetworkExternalNode::isSymmetricWindPressureCoefficientCurveDefaulted() const {
    return getImpl<detail::AirflowNetworkExternalNode_Impl>()->isSymmetricWindPressureCoefficientCurveDefaulted();
  }

  std::string AirflowNetworkExternalNode::windAngleType() const {
    return getImpl<detail::AirflowNetworkExternalNode_Impl>()->windAngleType();
  }

  bool AirflowNetworkExternalNode::isWindAngleTypeDefaulted() const {
    return getImpl<detail::AirflowNetworkExternalNode_Impl>()->isWindAngleTypeDefaulted();
  }

  void AirflowNetworkExternalNode::setExternalNodeHeight(double externalNodeHeight) {
    getImpl<detail::AirflowNetworkExternalNode_Impl>()->setExternalNodeHeight(externalNodeHeight);
  }

  void AirflowNetworkExternalNode::resetExternalNodeHeight() {
    getImpl<detail::AirflowNetworkExternalNode_Impl>()->resetExternalNodeHeight();
  }

  bool AirflowNetworkExternalNode::setWindPressureCoefficientCurve(const Curve& wPCValue) {
    return getImpl<detail::AirflowNetworkExternalNode_Impl>()->setWindPressureCoefficientCurve(wPCValue);
  }

  void AirflowNetworkExternalNode::setSymmetricWindPressureCoefficientCurve(bool symmetricWindPressureCoefficientCurve) {
    getImpl<detail::AirflowNetworkExternalNode_Impl>()->setSymmetricWindPressureCoefficientCurve(symmetricWindPressureCoefficientCurve);
  }

  void AirflowNetworkExternalNode::resetSymmetricWindPressureCoefficientCurve() {
    getImpl<detail::AirflowNetworkExternalNode_Impl>()->resetSymmetricWindPressureCoefficientCurve();
  }

  bool AirflowNetworkExternalNode::setWindAngleType(const std::string& windAngleType) {
    return getImpl<detail::AirflowNetworkExternalNode_Impl>()->setWindAngleType(windAngleType);
  }

  void AirflowNetworkExternalNode::resetWindAngleType() {
    getImpl<detail::AirflowNetworkExternalNode_Impl>()->resetWindAngleType();
  }

  /// @cond
  AirflowNetworkExternalNode::AirflowNetworkExternalNode(std::shared_ptr<detail::AirflowNetworkExternalNode_Impl> impl) : AirflowNetworkNode(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
