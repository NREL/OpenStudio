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

#include "CurveFanPressureRise.hpp"
#include "CurveFanPressureRise_Impl.hpp"

#include <utilities/idd/OS_Curve_FanPressureRise_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

#include <cmath>

using namespace std;

namespace openstudio {
namespace model {

namespace detail {

  CurveFanPressureRise_Impl::CurveFanPressureRise_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : Curve_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CurveFanPressureRise::iddObjectType());
  }

  CurveFanPressureRise_Impl::CurveFanPressureRise_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                       Model_Impl* model,
                                                       bool keepHandle)
    : Curve_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CurveFanPressureRise::iddObjectType());
  }

  CurveFanPressureRise_Impl::CurveFanPressureRise_Impl(const CurveFanPressureRise_Impl& other,
                                                       Model_Impl* model,
                                                       bool keepHandle)
    : Curve_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CurveFanPressureRise_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CurveFanPressureRise_Impl::iddObjectType() const {
    return CurveFanPressureRise::iddObjectType();
  }

  int CurveFanPressureRise_Impl::numVariables() const {
    return 2;
  }

  double CurveFanPressureRise_Impl::evaluate(const std::vector<double>& x) const {
    OS_ASSERT(x.size() == 2u);
    double Qfan = x[0];
    double Psm = x[1];
    double result = coefficient1C1() * pow(Qfan,2);
    result += coefficient2C2() * Qfan;
    result += coefficient3C3() * Qfan * sqrt(Psm);
    result += coefficient4C4() * Psm;
    return result;
  }

  double CurveFanPressureRise_Impl::coefficient1C1() const {
    boost::optional<double> value = getDouble(OS_Curve_FanPressureRiseFields::Coefficient1C1,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveFanPressureRise_Impl::coefficient2C2() const {
    boost::optional<double> value = getDouble(OS_Curve_FanPressureRiseFields::Coefficient2C2,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveFanPressureRise_Impl::coefficient3C3() const {
    boost::optional<double> value = getDouble(OS_Curve_FanPressureRiseFields::Coefficient3C3,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveFanPressureRise_Impl::coefficient4C4() const {
    boost::optional<double> value = getDouble(OS_Curve_FanPressureRiseFields::Coefficient4C4,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveFanPressureRise_Impl::minimumValueofQfan() const {
    boost::optional<double> value = getDouble(OS_Curve_FanPressureRiseFields::MinimumValueofQfan,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveFanPressureRise_Impl::maximumValueofQfan() const {
    boost::optional<double> value = getDouble(OS_Curve_FanPressureRiseFields::MaximumValueofQfan,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveFanPressureRise_Impl::minimumValueofPsm() const {
    boost::optional<double> value = getDouble(OS_Curve_FanPressureRiseFields::MinimumValueofPsm,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveFanPressureRise_Impl::maximumValueofPsm() const {
    boost::optional<double> value = getDouble(OS_Curve_FanPressureRiseFields::MaximumValueofPsm,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> CurveFanPressureRise_Impl::minimumCurveOutput() const {
    return getDouble(OS_Curve_FanPressureRiseFields::MinimumCurveOutput,true);
  }

  boost::optional<double> CurveFanPressureRise_Impl::maximumCurveOutput() const {
    return getDouble(OS_Curve_FanPressureRiseFields::MaximumCurveOutput,true);
  }

  void CurveFanPressureRise_Impl::setCoefficient1C1(double coefficient1C1) {
    bool result = setDouble(OS_Curve_FanPressureRiseFields::Coefficient1C1, coefficient1C1);
    OS_ASSERT(result);
  }

  void CurveFanPressureRise_Impl::setCoefficient2C2(double coefficient2C2) {
    bool result = setDouble(OS_Curve_FanPressureRiseFields::Coefficient2C2, coefficient2C2);
    OS_ASSERT(result);
  }

  void CurveFanPressureRise_Impl::setCoefficient3C3(double coefficient3C3) {
    bool result = setDouble(OS_Curve_FanPressureRiseFields::Coefficient3C3, coefficient3C3);
    OS_ASSERT(result);
  }

  void CurveFanPressureRise_Impl::setCoefficient4C4(double coefficient4C4) {
    bool result = setDouble(OS_Curve_FanPressureRiseFields::Coefficient4C4, coefficient4C4);
    OS_ASSERT(result);
  }

  void CurveFanPressureRise_Impl::setMinimumValueofQfan(double minimumValueofQfan) {
    bool result = setDouble(OS_Curve_FanPressureRiseFields::MinimumValueofQfan, minimumValueofQfan);
    OS_ASSERT(result);
  }

  void CurveFanPressureRise_Impl::setMaximumValueofQfan(double maximumValueofQfan) {
    bool result = setDouble(OS_Curve_FanPressureRiseFields::MaximumValueofQfan, maximumValueofQfan);
    OS_ASSERT(result);
  }

  void CurveFanPressureRise_Impl::setMinimumValueofPsm(double minimumValueofPsm) {
    bool result = setDouble(OS_Curve_FanPressureRiseFields::MinimumValueofPsm, minimumValueofPsm);
    OS_ASSERT(result);
  }

  void CurveFanPressureRise_Impl::setMaximumValueofPsm(double maximumValueofPsm) {
    bool result = setDouble(OS_Curve_FanPressureRiseFields::MaximumValueofPsm, maximumValueofPsm);
    OS_ASSERT(result);
  }

  void CurveFanPressureRise_Impl::setMinimumCurveOutput(boost::optional<double> minimumCurveOutput) {
    bool result = false;
    if (minimumCurveOutput) {
      result = setDouble(OS_Curve_FanPressureRiseFields::MinimumCurveOutput, minimumCurveOutput.get());
    } else {
      result = setString(OS_Curve_FanPressureRiseFields::MinimumCurveOutput, "");
    }
    OS_ASSERT(result);
  }

  void CurveFanPressureRise_Impl::resetMinimumCurveOutput() {
    bool result = setString(OS_Curve_FanPressureRiseFields::MinimumCurveOutput, "");
    OS_ASSERT(result);
  }

  void CurveFanPressureRise_Impl::setMaximumCurveOutput(boost::optional<double> maximumCurveOutput) {
    bool result = false;
    if (maximumCurveOutput) {
      result = setDouble(OS_Curve_FanPressureRiseFields::MaximumCurveOutput, maximumCurveOutput.get());
    } else {
      result = setString(OS_Curve_FanPressureRiseFields::MaximumCurveOutput, "");
    }
    OS_ASSERT(result);
  }

  void CurveFanPressureRise_Impl::resetMaximumCurveOutput() {
    bool result = setString(OS_Curve_FanPressureRiseFields::MaximumCurveOutput, "");
    OS_ASSERT(result);
  }

} // detail

CurveFanPressureRise::CurveFanPressureRise(const Model& model)
  : Curve(CurveFanPressureRise::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CurveFanPressureRise_Impl>());
  setDouble(OS_Curve_FanPressureRiseFields::Coefficient1C1,1.0);
  setDouble(OS_Curve_FanPressureRiseFields::Coefficient2C2,1.0);
  setDouble(OS_Curve_FanPressureRiseFields::Coefficient3C3,1.0);
  setDouble(OS_Curve_FanPressureRiseFields::Coefficient4C4,1.0);
  setDouble(OS_Curve_FanPressureRiseFields::MinimumValueofQfan,0.0);
  setDouble(OS_Curve_FanPressureRiseFields::MaximumValueofQfan,10.0);
  setDouble(OS_Curve_FanPressureRiseFields::MinimumValueofPsm,0.0);
  setDouble(OS_Curve_FanPressureRiseFields::MaximumValueofPsm,500.0);
}

IddObjectType CurveFanPressureRise::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Curve_FanPressureRise);
  return result;
}

double CurveFanPressureRise::coefficient1C1() const {
  return getImpl<detail::CurveFanPressureRise_Impl>()->coefficient1C1();
}

double CurveFanPressureRise::coefficient2C2() const {
  return getImpl<detail::CurveFanPressureRise_Impl>()->coefficient2C2();
}

double CurveFanPressureRise::coefficient3C3() const {
  return getImpl<detail::CurveFanPressureRise_Impl>()->coefficient3C3();
}

double CurveFanPressureRise::coefficient4C4() const {
  return getImpl<detail::CurveFanPressureRise_Impl>()->coefficient4C4();
}

double CurveFanPressureRise::minimumValueofQfan() const {
  return getImpl<detail::CurveFanPressureRise_Impl>()->minimumValueofQfan();
}

double CurveFanPressureRise::maximumValueofQfan() const {
  return getImpl<detail::CurveFanPressureRise_Impl>()->maximumValueofQfan();
}

double CurveFanPressureRise::minimumValueofPsm() const {
  return getImpl<detail::CurveFanPressureRise_Impl>()->minimumValueofPsm();
}

double CurveFanPressureRise::maximumValueofPsm() const {
  return getImpl<detail::CurveFanPressureRise_Impl>()->maximumValueofPsm();
}

boost::optional<double> CurveFanPressureRise::minimumCurveOutput() const {
  return getImpl<detail::CurveFanPressureRise_Impl>()->minimumCurveOutput();
}

boost::optional<double> CurveFanPressureRise::maximumCurveOutput() const {
  return getImpl<detail::CurveFanPressureRise_Impl>()->maximumCurveOutput();
}

void CurveFanPressureRise::setCoefficient1C1(double coefficient1C1) {
  getImpl<detail::CurveFanPressureRise_Impl>()->setCoefficient1C1(coefficient1C1);
}

void CurveFanPressureRise::setCoefficient2C2(double coefficient2C2) {
  getImpl<detail::CurveFanPressureRise_Impl>()->setCoefficient2C2(coefficient2C2);
}

void CurveFanPressureRise::setCoefficient3C3(double coefficient3C3) {
  getImpl<detail::CurveFanPressureRise_Impl>()->setCoefficient3C3(coefficient3C3);
}

void CurveFanPressureRise::setCoefficient4C4(double coefficient4C4) {
  getImpl<detail::CurveFanPressureRise_Impl>()->setCoefficient4C4(coefficient4C4);
}

void CurveFanPressureRise::setMinimumValueofQfan(double minimumValueofQfan) {
  getImpl<detail::CurveFanPressureRise_Impl>()->setMinimumValueofQfan(minimumValueofQfan);
}

void CurveFanPressureRise::setMaximumValueofQfan(double maximumValueofQfan) {
  getImpl<detail::CurveFanPressureRise_Impl>()->setMaximumValueofQfan(maximumValueofQfan);
}

void CurveFanPressureRise::setMinimumValueofPsm(double minimumValueofPsm) {
  getImpl<detail::CurveFanPressureRise_Impl>()->setMinimumValueofPsm(minimumValueofPsm);
}

void CurveFanPressureRise::setMaximumValueofPsm(double maximumValueofPsm) {
  getImpl<detail::CurveFanPressureRise_Impl>()->setMaximumValueofPsm(maximumValueofPsm);
}

void CurveFanPressureRise::setMinimumCurveOutput(double minimumCurveOutput) {
  getImpl<detail::CurveFanPressureRise_Impl>()->setMinimumCurveOutput(minimumCurveOutput);
}

void CurveFanPressureRise::resetMinimumCurveOutput() {
  getImpl<detail::CurveFanPressureRise_Impl>()->resetMinimumCurveOutput();
}

void CurveFanPressureRise::setMaximumCurveOutput(double maximumCurveOutput) {
  getImpl<detail::CurveFanPressureRise_Impl>()->setMaximumCurveOutput(maximumCurveOutput);
}

void CurveFanPressureRise::resetMaximumCurveOutput() {
  getImpl<detail::CurveFanPressureRise_Impl>()->resetMaximumCurveOutput();
}

/// @cond
CurveFanPressureRise::CurveFanPressureRise(std::shared_ptr<detail::CurveFanPressureRise_Impl> impl)
  : Curve(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

