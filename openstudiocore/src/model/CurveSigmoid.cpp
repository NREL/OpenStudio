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

#include "CurveSigmoid.hpp"
#include "CurveSigmoid_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Curve_Sigmoid_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

#include <cmath>

using namespace std;

namespace openstudio {
namespace model {

namespace detail {

  CurveSigmoid_Impl::CurveSigmoid_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : Curve_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CurveSigmoid::iddObjectType());
  }

  CurveSigmoid_Impl::CurveSigmoid_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                       Model_Impl* model,
                                       bool keepHandle)
    : Curve_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CurveSigmoid::iddObjectType());
  }

  CurveSigmoid_Impl::CurveSigmoid_Impl(const CurveSigmoid_Impl& other,
                                       Model_Impl* model,
                                       bool keepHandle)
    : Curve_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CurveSigmoid_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CurveSigmoid_Impl::iddObjectType() const {
    return CurveSigmoid::iddObjectType();
  }

  int CurveSigmoid_Impl::numVariables() const {
    return 1;
  }

  double CurveSigmoid_Impl::evaluate(const std::vector<double>& x) const {
    OS_ASSERT(x.size() == 1u);
    double result = coefficient2C2();
    double temp = coefficient3C3() - x[0];
    temp /= coefficient4C4();
    temp = exp(temp);
    temp = pow(temp,coefficient5C5());
    result /= (1.0 + temp);
    result += coefficient1C1();
    return result;
  }

  double CurveSigmoid_Impl::coefficient1C1() const {
    boost::optional<double> value = getDouble(OS_Curve_SigmoidFields::Coefficient1C1,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveSigmoid_Impl::coefficient2C2() const {
    boost::optional<double> value = getDouble(OS_Curve_SigmoidFields::Coefficient2C2,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveSigmoid_Impl::coefficient3C3() const {
    boost::optional<double> value = getDouble(OS_Curve_SigmoidFields::Coefficient3C3,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveSigmoid_Impl::coefficient4C4() const {
    boost::optional<double> value = getDouble(OS_Curve_SigmoidFields::Coefficient4C4,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveSigmoid_Impl::coefficient5C5() const {
    boost::optional<double> value = getDouble(OS_Curve_SigmoidFields::Coefficient5C5,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveSigmoid_Impl::minimumValueofx() const {
    boost::optional<double> value = getDouble(OS_Curve_SigmoidFields::MinimumValueofx,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveSigmoid_Impl::maximumValueofx() const {
    boost::optional<double> value = getDouble(OS_Curve_SigmoidFields::MaximumValueofx,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> CurveSigmoid_Impl::minimumCurveOutput() const {
    return getDouble(OS_Curve_SigmoidFields::MinimumCurveOutput,true);
  }

  boost::optional<double> CurveSigmoid_Impl::maximumCurveOutput() const {
    return getDouble(OS_Curve_SigmoidFields::MaximumCurveOutput,true);
  }

  std::string CurveSigmoid_Impl::inputUnitTypeforx() const {
    boost::optional<std::string> value = getString(OS_Curve_SigmoidFields::InputUnitTypeforx,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CurveSigmoid_Impl::isInputUnitTypeforxDefaulted() const {
    return isEmpty(OS_Curve_SigmoidFields::InputUnitTypeforx);
  }

  std::string CurveSigmoid_Impl::outputUnitType() const {
    boost::optional<std::string> value = getString(OS_Curve_SigmoidFields::OutputUnitType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CurveSigmoid_Impl::isOutputUnitTypeDefaulted() const {
    return isEmpty(OS_Curve_SigmoidFields::OutputUnitType);
  }

  void CurveSigmoid_Impl::setCoefficient1C1(double coefficient1C1) {
    bool result = setDouble(OS_Curve_SigmoidFields::Coefficient1C1, coefficient1C1);
    OS_ASSERT(result);
  }

  void CurveSigmoid_Impl::setCoefficient2C2(double coefficient2C2) {
    bool result = setDouble(OS_Curve_SigmoidFields::Coefficient2C2, coefficient2C2);
    OS_ASSERT(result);
  }

  void CurveSigmoid_Impl::setCoefficient3C3(double coefficient3C3) {
    bool result = setDouble(OS_Curve_SigmoidFields::Coefficient3C3, coefficient3C3);
    OS_ASSERT(result);
  }

  void CurveSigmoid_Impl::setCoefficient4C4(double coefficient4C4) {
    bool result = setDouble(OS_Curve_SigmoidFields::Coefficient4C4, coefficient4C4);
    OS_ASSERT(result);
  }

  void CurveSigmoid_Impl::setCoefficient5C5(double coefficient5C5) {
    bool result = setDouble(OS_Curve_SigmoidFields::Coefficient5C5, coefficient5C5);
    OS_ASSERT(result);
  }

  void CurveSigmoid_Impl::setMinimumValueofx(double minimumValueofx) {
    bool result = setDouble(OS_Curve_SigmoidFields::MinimumValueofx, minimumValueofx);
    OS_ASSERT(result);
  }

  void CurveSigmoid_Impl::setMaximumValueofx(double maximumValueofx) {
    bool result = setDouble(OS_Curve_SigmoidFields::MaximumValueofx, maximumValueofx);
    OS_ASSERT(result);
  }

  void CurveSigmoid_Impl::setMinimumCurveOutput(boost::optional<double> minimumCurveOutput) {
    bool result = false;
    if (minimumCurveOutput) {
      result = setDouble(OS_Curve_SigmoidFields::MinimumCurveOutput, minimumCurveOutput.get());
    } else {
      result = setString(OS_Curve_SigmoidFields::MinimumCurveOutput, "");
    }
    OS_ASSERT(result);
  }

  void CurveSigmoid_Impl::resetMinimumCurveOutput() {
    bool result = setString(OS_Curve_SigmoidFields::MinimumCurveOutput, "");
    OS_ASSERT(result);
  }

  void CurveSigmoid_Impl::setMaximumCurveOutput(boost::optional<double> maximumCurveOutput) {
    bool result = false;
    if (maximumCurveOutput) {
      result = setDouble(OS_Curve_SigmoidFields::MaximumCurveOutput, maximumCurveOutput.get());
    } else {
      result = setString(OS_Curve_SigmoidFields::MaximumCurveOutput, "");
    }
    OS_ASSERT(result);
  }

  void CurveSigmoid_Impl::resetMaximumCurveOutput() {
    bool result = setString(OS_Curve_SigmoidFields::MaximumCurveOutput, "");
    OS_ASSERT(result);
  }

  bool CurveSigmoid_Impl::setInputUnitTypeforx(std::string inputUnitTypeforx) {
    bool result = setString(OS_Curve_SigmoidFields::InputUnitTypeforx, inputUnitTypeforx);
    return result;
  }

  void CurveSigmoid_Impl::resetInputUnitTypeforx() {

    bool result = setString(OS_Curve_SigmoidFields::InputUnitTypeforx, "");
    OS_ASSERT(result);
  }

  bool CurveSigmoid_Impl::setOutputUnitType(std::string outputUnitType) {
    bool result = setString(OS_Curve_SigmoidFields::OutputUnitType, outputUnitType);
    return result;
  }

  void CurveSigmoid_Impl::resetOutputUnitType() {
    bool result = setString(OS_Curve_SigmoidFields::OutputUnitType, "");
    OS_ASSERT(result);
  }

} // detail

CurveSigmoid::CurveSigmoid(const Model& model)
  : Curve(CurveSigmoid::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CurveSigmoid_Impl>());
  setDouble(OS_Curve_SigmoidFields::Coefficient1C1,0.0);
  setDouble(OS_Curve_SigmoidFields::Coefficient2C2,1.0);
  setDouble(OS_Curve_SigmoidFields::Coefficient3C3,1.0);
  setDouble(OS_Curve_SigmoidFields::Coefficient4C4,1.0);
  setDouble(OS_Curve_SigmoidFields::Coefficient5C5,1.0);
  setDouble(OS_Curve_SigmoidFields::MinimumValueofx,0.0);
  setDouble(OS_Curve_SigmoidFields::MaximumValueofx,1.0);
}

IddObjectType CurveSigmoid::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Curve_Sigmoid);
  return result;
}

std::vector<std::string> CurveSigmoid::validInputUnitTypeforxValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Curve_SigmoidFields::InputUnitTypeforx);
}

std::vector<std::string> CurveSigmoid::validOutputUnitTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Curve_SigmoidFields::OutputUnitType);
}

double CurveSigmoid::coefficient1C1() const {
  return getImpl<detail::CurveSigmoid_Impl>()->coefficient1C1();
}

double CurveSigmoid::coefficient2C2() const {
  return getImpl<detail::CurveSigmoid_Impl>()->coefficient2C2();
}

double CurveSigmoid::coefficient3C3() const {
  return getImpl<detail::CurveSigmoid_Impl>()->coefficient3C3();
}

double CurveSigmoid::coefficient4C4() const {
  return getImpl<detail::CurveSigmoid_Impl>()->coefficient4C4();
}

double CurveSigmoid::coefficient5C5() const {
  return getImpl<detail::CurveSigmoid_Impl>()->coefficient5C5();
}

double CurveSigmoid::minimumValueofx() const {
  return getImpl<detail::CurveSigmoid_Impl>()->minimumValueofx();
}

double CurveSigmoid::maximumValueofx() const {
  return getImpl<detail::CurveSigmoid_Impl>()->maximumValueofx();
}

boost::optional<double> CurveSigmoid::minimumCurveOutput() const {
  return getImpl<detail::CurveSigmoid_Impl>()->minimumCurveOutput();
}

boost::optional<double> CurveSigmoid::maximumCurveOutput() const {
  return getImpl<detail::CurveSigmoid_Impl>()->maximumCurveOutput();
}

std::string CurveSigmoid::inputUnitTypeforx() const {
  return getImpl<detail::CurveSigmoid_Impl>()->inputUnitTypeforx();
}

bool CurveSigmoid::isInputUnitTypeforxDefaulted() const {
  return getImpl<detail::CurveSigmoid_Impl>()->isInputUnitTypeforxDefaulted();
}

std::string CurveSigmoid::outputUnitType() const {
  return getImpl<detail::CurveSigmoid_Impl>()->outputUnitType();
}

bool CurveSigmoid::isOutputUnitTypeDefaulted() const {
  return getImpl<detail::CurveSigmoid_Impl>()->isOutputUnitTypeDefaulted();
}

void CurveSigmoid::setCoefficient1C1(double coefficient1C1) {
  getImpl<detail::CurveSigmoid_Impl>()->setCoefficient1C1(coefficient1C1);
}

void CurveSigmoid::setCoefficient2C2(double coefficient2C2) {
  getImpl<detail::CurveSigmoid_Impl>()->setCoefficient2C2(coefficient2C2);
}

void CurveSigmoid::setCoefficient3C3(double coefficient3C3) {
  getImpl<detail::CurveSigmoid_Impl>()->setCoefficient3C3(coefficient3C3);
}

void CurveSigmoid::setCoefficient4C4(double coefficient4C4) {
  getImpl<detail::CurveSigmoid_Impl>()->setCoefficient4C4(coefficient4C4);
}

void CurveSigmoid::setCoefficient5C5(double coefficient5C5) {
  getImpl<detail::CurveSigmoid_Impl>()->setCoefficient5C5(coefficient5C5);
}

void CurveSigmoid::setMinimumValueofx(double minimumValueofx) {
  getImpl<detail::CurveSigmoid_Impl>()->setMinimumValueofx(minimumValueofx);
}

void CurveSigmoid::setMaximumValueofx(double maximumValueofx) {
  getImpl<detail::CurveSigmoid_Impl>()->setMaximumValueofx(maximumValueofx);
}

void CurveSigmoid::setMinimumCurveOutput(double minimumCurveOutput) {
  getImpl<detail::CurveSigmoid_Impl>()->setMinimumCurveOutput(minimumCurveOutput);
}

void CurveSigmoid::resetMinimumCurveOutput() {
  getImpl<detail::CurveSigmoid_Impl>()->resetMinimumCurveOutput();
}

void CurveSigmoid::setMaximumCurveOutput(double maximumCurveOutput) {
  getImpl<detail::CurveSigmoid_Impl>()->setMaximumCurveOutput(maximumCurveOutput);
}

void CurveSigmoid::resetMaximumCurveOutput() {
  getImpl<detail::CurveSigmoid_Impl>()->resetMaximumCurveOutput();
}

bool CurveSigmoid::setInputUnitTypeforx(std::string inputUnitTypeforx) {
  return getImpl<detail::CurveSigmoid_Impl>()->setInputUnitTypeforx(inputUnitTypeforx);
}

void CurveSigmoid::resetInputUnitTypeforx() {
  getImpl<detail::CurveSigmoid_Impl>()->resetInputUnitTypeforx();
}

bool CurveSigmoid::setOutputUnitType(std::string outputUnitType) {
  return getImpl<detail::CurveSigmoid_Impl>()->setOutputUnitType(outputUnitType);
}

void CurveSigmoid::resetOutputUnitType() {
  getImpl<detail::CurveSigmoid_Impl>()->resetOutputUnitType();
}

/// @cond
CurveSigmoid::CurveSigmoid(std::shared_ptr<detail::CurveSigmoid_Impl> impl)
  : Curve(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

