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

#include "CurveFunctionalPressureDrop.hpp"
#include "CurveFunctionalPressureDrop_Impl.hpp"

#include <utilities/idd/OS_Curve_Functional_PressureDrop_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

#include <cmath>

using namespace std;

namespace openstudio {
namespace model {

namespace detail {

  CurveFunctionalPressureDrop_Impl::CurveFunctionalPressureDrop_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : Curve_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CurveFunctionalPressureDrop::iddObjectType());
  }

  CurveFunctionalPressureDrop_Impl::CurveFunctionalPressureDrop_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                     Model_Impl* model,
                                                                     bool keepHandle)
    : Curve_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CurveFunctionalPressureDrop::iddObjectType());
  }

  CurveFunctionalPressureDrop_Impl::CurveFunctionalPressureDrop_Impl(const CurveFunctionalPressureDrop_Impl& other,
                                                                     Model_Impl* model,
                                                                     bool keepHandle)
    : Curve_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CurveFunctionalPressureDrop_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CurveFunctionalPressureDrop_Impl::iddObjectType() const {
    return CurveFunctionalPressureDrop::iddObjectType();
  }

  int CurveFunctionalPressureDrop_Impl::numVariables() const {
    LOG_AND_THROW("CurveFunctionalPressureDrop must be evaluated by EnergyPlus, in the context of "
        << "plant pressure simulations. Therefore, numVariables and evaluate are not applicable "
        << "for this curve type.");
  }

  double CurveFunctionalPressureDrop_Impl::evaluate(const std::vector<double>& x) const {
    LOG_AND_THROW("CurveFunctionalPressureDrop must be evaluated by EnergyPlus, in the context of "
        << "plant pressure simulations. Therefore, numVariables and evaluate are not applicable "
        << "for this curve type.");
  }

  double CurveFunctionalPressureDrop_Impl::diameter() const {
    boost::optional<double> value = getDouble(OS_Curve_Functional_PressureDropFields::Diameter,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> CurveFunctionalPressureDrop_Impl::minorLossCoefficient() const {
    return getDouble(OS_Curve_Functional_PressureDropFields::MinorLossCoefficient,true);
  }

  boost::optional<double> CurveFunctionalPressureDrop_Impl::length() const {
    return getDouble(OS_Curve_Functional_PressureDropFields::Length,true);
  }

  boost::optional<double> CurveFunctionalPressureDrop_Impl::roughness() const {
    return getDouble(OS_Curve_Functional_PressureDropFields::Roughness,true);
  }

  boost::optional<double> CurveFunctionalPressureDrop_Impl::fixedFrictionFactor() const {
    return getDouble(OS_Curve_Functional_PressureDropFields::FixedFrictionFactor,true);
  }

  void CurveFunctionalPressureDrop_Impl::setDiameter(double diameter) {
    bool result = setDouble(OS_Curve_Functional_PressureDropFields::Diameter, diameter);
    OS_ASSERT(result);
  }

  void CurveFunctionalPressureDrop_Impl::setMinorLossCoefficient(boost::optional<double> minorLossCoefficient) {
    bool result = false;
    if (minorLossCoefficient) {
      result = setDouble(OS_Curve_Functional_PressureDropFields::MinorLossCoefficient, minorLossCoefficient.get());
    } else {
      result = setString(OS_Curve_Functional_PressureDropFields::MinorLossCoefficient, "");
    }
    OS_ASSERT(result);
  }

  void CurveFunctionalPressureDrop_Impl::resetMinorLossCoefficient() {
    bool result = setString(OS_Curve_Functional_PressureDropFields::MinorLossCoefficient, "");
    OS_ASSERT(result);
  }

  void CurveFunctionalPressureDrop_Impl::setLength(boost::optional<double> length) {
    bool result = false;
    if (length) {
      result = setDouble(OS_Curve_Functional_PressureDropFields::Length, length.get());
    } else {
      result = setString(OS_Curve_Functional_PressureDropFields::Length, "");
    }
    OS_ASSERT(result);
  }

  void CurveFunctionalPressureDrop_Impl::resetLength() {
    bool result = setString(OS_Curve_Functional_PressureDropFields::Length, "");
    OS_ASSERT(result);
  }

  void CurveFunctionalPressureDrop_Impl::setRoughness(boost::optional<double> roughness) {
    bool result = false;
    if (roughness) {
      result = setDouble(OS_Curve_Functional_PressureDropFields::Roughness, roughness.get());
    } else {
      result = setString(OS_Curve_Functional_PressureDropFields::Roughness, "");
    }
    OS_ASSERT(result);
  }

  void CurveFunctionalPressureDrop_Impl::resetRoughness() {
    bool result = setString(OS_Curve_Functional_PressureDropFields::Roughness, "");
    OS_ASSERT(result);
  }

  void CurveFunctionalPressureDrop_Impl::setFixedFrictionFactor(boost::optional<double> fixedFrictionFactor) {
    bool result = false;
    if (fixedFrictionFactor) {
      result = setDouble(OS_Curve_Functional_PressureDropFields::FixedFrictionFactor, fixedFrictionFactor.get());
    } else {
      result = setString(OS_Curve_Functional_PressureDropFields::FixedFrictionFactor, "");
    }
    OS_ASSERT(result);
  }

  void CurveFunctionalPressureDrop_Impl::resetFixedFrictionFactor() {
    bool result = setString(OS_Curve_Functional_PressureDropFields::FixedFrictionFactor, "");
    OS_ASSERT(result);
  }

} // detail

CurveFunctionalPressureDrop::CurveFunctionalPressureDrop(const Model& model)
  : Curve(CurveFunctionalPressureDrop::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CurveFunctionalPressureDrop_Impl>());
  setDouble(OS_Curve_Functional_PressureDropFields::Diameter,0.05);
}

IddObjectType CurveFunctionalPressureDrop::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Curve_Functional_PressureDrop);
  return result;
}

double CurveFunctionalPressureDrop::diameter() const {
  return getImpl<detail::CurveFunctionalPressureDrop_Impl>()->diameter();
}

boost::optional<double> CurveFunctionalPressureDrop::minorLossCoefficient() const {
  return getImpl<detail::CurveFunctionalPressureDrop_Impl>()->minorLossCoefficient();
}

boost::optional<double> CurveFunctionalPressureDrop::length() const {
  return getImpl<detail::CurveFunctionalPressureDrop_Impl>()->length();
}

boost::optional<double> CurveFunctionalPressureDrop::roughness() const {
  return getImpl<detail::CurveFunctionalPressureDrop_Impl>()->roughness();
}

boost::optional<double> CurveFunctionalPressureDrop::fixedFrictionFactor() const {
  return getImpl<detail::CurveFunctionalPressureDrop_Impl>()->fixedFrictionFactor();
}

void CurveFunctionalPressureDrop::setDiameter(double diameter) {
  getImpl<detail::CurveFunctionalPressureDrop_Impl>()->setDiameter(diameter);
}

void CurveFunctionalPressureDrop::setMinorLossCoefficient(double minorLossCoefficient) {
  getImpl<detail::CurveFunctionalPressureDrop_Impl>()->setMinorLossCoefficient(minorLossCoefficient);
}

void CurveFunctionalPressureDrop::resetMinorLossCoefficient() {
  getImpl<detail::CurveFunctionalPressureDrop_Impl>()->resetMinorLossCoefficient();
}

void CurveFunctionalPressureDrop::setLength(double length) {
  getImpl<detail::CurveFunctionalPressureDrop_Impl>()->setLength(length);
}

void CurveFunctionalPressureDrop::resetLength() {
  getImpl<detail::CurveFunctionalPressureDrop_Impl>()->resetLength();
}

void CurveFunctionalPressureDrop::setRoughness(double roughness) {
  getImpl<detail::CurveFunctionalPressureDrop_Impl>()->setRoughness(roughness);
}

void CurveFunctionalPressureDrop::resetRoughness() {
  getImpl<detail::CurveFunctionalPressureDrop_Impl>()->resetRoughness();
}

void CurveFunctionalPressureDrop::setFixedFrictionFactor(double fixedFrictionFactor) {
  getImpl<detail::CurveFunctionalPressureDrop_Impl>()->setFixedFrictionFactor(fixedFrictionFactor);
}

void CurveFunctionalPressureDrop::resetFixedFrictionFactor() {
  getImpl<detail::CurveFunctionalPressureDrop_Impl>()->resetFixedFrictionFactor();
}

/// @cond
CurveFunctionalPressureDrop::CurveFunctionalPressureDrop(std::shared_ptr<detail::CurveFunctionalPressureDrop_Impl> impl)
  : Curve(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

