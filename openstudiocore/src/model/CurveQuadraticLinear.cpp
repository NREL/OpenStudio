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

#include "CurveQuadraticLinear.hpp"
#include "CurveQuadraticLinear_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Curve_QuadraticLinear_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

#include <cmath>

using namespace std;

namespace openstudio {
namespace model {

namespace detail {

  CurveQuadraticLinear_Impl::CurveQuadraticLinear_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : Curve_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CurveQuadraticLinear::iddObjectType());
  }

  CurveQuadraticLinear_Impl::CurveQuadraticLinear_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                       Model_Impl* model,
                                                       bool keepHandle)
    : Curve_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CurveQuadraticLinear::iddObjectType());
  }

  CurveQuadraticLinear_Impl::CurveQuadraticLinear_Impl(const CurveQuadraticLinear_Impl& other,
                                                       Model_Impl* model,
                                                       bool keepHandle)
    : Curve_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CurveQuadraticLinear_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CurveQuadraticLinear_Impl::iddObjectType() const {
    return CurveQuadraticLinear::iddObjectType();
  }

  int CurveQuadraticLinear_Impl::numVariables() const {
    return 2;
  }

  double CurveQuadraticLinear_Impl::evaluate(const std::vector<double>& x) const {
    OS_ASSERT(x.size() == 2u);
    double x2 = pow(x[0],2);
    double result = coefficient1Constant();
    result += coefficient2x() * x[0];
    result += coefficient3xPOW2() * x2;
    double temp = coefficient4y();
    temp += coefficient5xTIMESY() * x[0];
    temp += coefficient6xPOW2TIMESY() * x2;
    result += temp * x[1];
    return result;
  }

  double CurveQuadraticLinear_Impl::coefficient1Constant() const {
    boost::optional<double> value = getDouble(OS_Curve_QuadraticLinearFields::Coefficient1Constant,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveQuadraticLinear_Impl::coefficient2x() const {
    boost::optional<double> value = getDouble(OS_Curve_QuadraticLinearFields::Coefficient2x,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveQuadraticLinear_Impl::coefficient3xPOW2() const {
    boost::optional<double> value = getDouble(OS_Curve_QuadraticLinearFields::Coefficient3x_POW_2,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveQuadraticLinear_Impl::coefficient4y() const {
    boost::optional<double> value = getDouble(OS_Curve_QuadraticLinearFields::Coefficient4y,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveQuadraticLinear_Impl::coefficient5xTIMESY() const {
    boost::optional<double> value = getDouble(OS_Curve_QuadraticLinearFields::Coefficient5x_TIMES_y,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveQuadraticLinear_Impl::coefficient6xPOW2TIMESY() const {
    boost::optional<double> value = getDouble(OS_Curve_QuadraticLinearFields::Coefficient6x_POW_2_TIMES_y,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveQuadraticLinear_Impl::minimumValueofx() const {
    boost::optional<double> value = getDouble(OS_Curve_QuadraticLinearFields::MinimumValueofx,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveQuadraticLinear_Impl::maximumValueofx() const {
    boost::optional<double> value = getDouble(OS_Curve_QuadraticLinearFields::MaximumValueofx,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveQuadraticLinear_Impl::minimumValueofy() const {
    boost::optional<double> value = getDouble(OS_Curve_QuadraticLinearFields::MinimumValueofy,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveQuadraticLinear_Impl::maximumValueofy() const {
    boost::optional<double> value = getDouble(OS_Curve_QuadraticLinearFields::MaximumValueofy,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> CurveQuadraticLinear_Impl::minimumCurveOutput() const {
    return getDouble(OS_Curve_QuadraticLinearFields::MinimumCurveOutput,true);
  }

  boost::optional<double> CurveQuadraticLinear_Impl::maximumCurveOutput() const {
    return getDouble(OS_Curve_QuadraticLinearFields::MaximumCurveOutput,true);
  }

  std::string CurveQuadraticLinear_Impl::inputUnitTypeforX() const {
    boost::optional<std::string> value = getString(OS_Curve_QuadraticLinearFields::InputUnitTypeforX,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CurveQuadraticLinear_Impl::isInputUnitTypeforXDefaulted() const {
    return isEmpty(OS_Curve_QuadraticLinearFields::InputUnitTypeforX);
  }

  std::string CurveQuadraticLinear_Impl::inputUnitTypeforY() const {
    boost::optional<std::string> value = getString(OS_Curve_QuadraticLinearFields::InputUnitTypeforY,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CurveQuadraticLinear_Impl::isInputUnitTypeforYDefaulted() const {
    return isEmpty(OS_Curve_QuadraticLinearFields::InputUnitTypeforY);
  }

  std::string CurveQuadraticLinear_Impl::outputUnitType() const {
    boost::optional<std::string> value = getString(OS_Curve_QuadraticLinearFields::OutputUnitType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CurveQuadraticLinear_Impl::isOutputUnitTypeDefaulted() const {
    return isEmpty(OS_Curve_QuadraticLinearFields::OutputUnitType);
  }

  void CurveQuadraticLinear_Impl::setCoefficient1Constant(double coefficient1Constant) {
    bool result = setDouble(OS_Curve_QuadraticLinearFields::Coefficient1Constant, coefficient1Constant);
    OS_ASSERT(result);
  }

  void CurveQuadraticLinear_Impl::setCoefficient2x(double coefficient2x) {
    bool result = setDouble(OS_Curve_QuadraticLinearFields::Coefficient2x, coefficient2x);
    OS_ASSERT(result);
  }

  void CurveQuadraticLinear_Impl::setCoefficient3xPOW2(double coefficient3xPOW2) {
    bool result = setDouble(OS_Curve_QuadraticLinearFields::Coefficient3x_POW_2, coefficient3xPOW2);
    OS_ASSERT(result);
  }

  void CurveQuadraticLinear_Impl::setCoefficient4y(double coefficient4y) {
    bool result = setDouble(OS_Curve_QuadraticLinearFields::Coefficient4y, coefficient4y);
    OS_ASSERT(result);
  }

  void CurveQuadraticLinear_Impl::setCoefficient5xTIMESY(double coefficient5xTIMESY) {
    bool result = setDouble(OS_Curve_QuadraticLinearFields::Coefficient5x_TIMES_y, coefficient5xTIMESY);
    OS_ASSERT(result);
  }

  void CurveQuadraticLinear_Impl::setCoefficient6xPOW2TIMESY(double coefficient6xPOW2TIMESY) {
    bool result = setDouble(OS_Curve_QuadraticLinearFields::Coefficient6x_POW_2_TIMES_y, coefficient6xPOW2TIMESY);
    OS_ASSERT(result);
  }

  void CurveQuadraticLinear_Impl::setMinimumValueofx(double minimumValueofx) {
    bool result = setDouble(OS_Curve_QuadraticLinearFields::MinimumValueofx, minimumValueofx);
    OS_ASSERT(result);
  }

  void CurveQuadraticLinear_Impl::setMaximumValueofx(double maximumValueofx) {
    bool result = setDouble(OS_Curve_QuadraticLinearFields::MaximumValueofx, maximumValueofx);
    OS_ASSERT(result);
  }

  void CurveQuadraticLinear_Impl::setMinimumValueofy(double minimumValueofy) {
    bool result = setDouble(OS_Curve_QuadraticLinearFields::MinimumValueofy, minimumValueofy);
    OS_ASSERT(result);
  }

  void CurveQuadraticLinear_Impl::setMaximumValueofy(double maximumValueofy) {
    bool result = setDouble(OS_Curve_QuadraticLinearFields::MaximumValueofy, maximumValueofy);
    OS_ASSERT(result);
  }

  void CurveQuadraticLinear_Impl::setMinimumCurveOutput(boost::optional<double> minimumCurveOutput) {
    bool result = false;
    if (minimumCurveOutput) {
      result = setDouble(OS_Curve_QuadraticLinearFields::MinimumCurveOutput, minimumCurveOutput.get());
    } else {
      result = setString(OS_Curve_QuadraticLinearFields::MinimumCurveOutput, "");
    }
    OS_ASSERT(result);
  }

  void CurveQuadraticLinear_Impl::resetMinimumCurveOutput() {
    bool result = setString(OS_Curve_QuadraticLinearFields::MinimumCurveOutput, "");
    OS_ASSERT(result);
  }

  void CurveQuadraticLinear_Impl::setMaximumCurveOutput(boost::optional<double> maximumCurveOutput) {
    bool result = false;
    if (maximumCurveOutput) {
      result = setDouble(OS_Curve_QuadraticLinearFields::MaximumCurveOutput, maximumCurveOutput.get());
    } else {
      result = setString(OS_Curve_QuadraticLinearFields::MaximumCurveOutput, "");
    }
    OS_ASSERT(result);
  }

  void CurveQuadraticLinear_Impl::resetMaximumCurveOutput() {
    bool result = setString(OS_Curve_QuadraticLinearFields::MaximumCurveOutput, "");
    OS_ASSERT(result);
  }

  bool CurveQuadraticLinear_Impl::setInputUnitTypeforX(std::string inputUnitTypeforX) {
    bool result = setString(OS_Curve_QuadraticLinearFields::InputUnitTypeforX, inputUnitTypeforX);
    return result;
  }

  void CurveQuadraticLinear_Impl::resetInputUnitTypeforX() {
    bool result = setString(OS_Curve_QuadraticLinearFields::InputUnitTypeforX, "");
    OS_ASSERT(result);
  }

  bool CurveQuadraticLinear_Impl::setInputUnitTypeforY(std::string inputUnitTypeforY) {
    bool result = setString(OS_Curve_QuadraticLinearFields::InputUnitTypeforY, inputUnitTypeforY);
    return result;
  }

  void CurveQuadraticLinear_Impl::resetInputUnitTypeforY() {
    bool result = setString(OS_Curve_QuadraticLinearFields::InputUnitTypeforY, "");
    OS_ASSERT(result);
  }

  bool CurveQuadraticLinear_Impl::setOutputUnitType(std::string outputUnitType) {
    bool result = setString(OS_Curve_QuadraticLinearFields::OutputUnitType, outputUnitType);
    return result;
  }

  void CurveQuadraticLinear_Impl::resetOutputUnitType() {
    bool result = setString(OS_Curve_QuadraticLinearFields::OutputUnitType, "");
    OS_ASSERT(result);
  }

} // detail

CurveQuadraticLinear::CurveQuadraticLinear(const Model& model)
  : Curve(CurveQuadraticLinear::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CurveQuadraticLinear_Impl>());
  setDouble(OS_Curve_QuadraticLinearFields::Coefficient1Constant,0.0);
  setDouble(OS_Curve_QuadraticLinearFields::Coefficient2x,0.0);
  setDouble(OS_Curve_QuadraticLinearFields::Coefficient3x_POW_2,0.0);
  setDouble(OS_Curve_QuadraticLinearFields::Coefficient4y,0.0);
  setDouble(OS_Curve_QuadraticLinearFields::Coefficient5x_TIMES_y,0.0);
  setDouble(OS_Curve_QuadraticLinearFields::Coefficient6x_POW_2_TIMES_y,1.0);
  setDouble(OS_Curve_QuadraticLinearFields::MinimumValueofx,0.0);
  setDouble(OS_Curve_QuadraticLinearFields::MaximumValueofx,1.0);
  setDouble(OS_Curve_QuadraticLinearFields::MinimumValueofy,0.0);
  setDouble(OS_Curve_QuadraticLinearFields::MaximumValueofy,1.0);
}

IddObjectType CurveQuadraticLinear::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Curve_QuadraticLinear);
  return result;
}

std::vector<std::string> CurveQuadraticLinear::validInputUnitTypeforXValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Curve_QuadraticLinearFields::InputUnitTypeforX);
}

std::vector<std::string> CurveQuadraticLinear::validInputUnitTypeforYValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Curve_QuadraticLinearFields::InputUnitTypeforY);
}

std::vector<std::string> CurveQuadraticLinear::validOutputUnitTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Curve_QuadraticLinearFields::OutputUnitType);
}

double CurveQuadraticLinear::coefficient1Constant() const {
  return getImpl<detail::CurveQuadraticLinear_Impl>()->coefficient1Constant();
}

double CurveQuadraticLinear::coefficient2x() const {
  return getImpl<detail::CurveQuadraticLinear_Impl>()->coefficient2x();
}

double CurveQuadraticLinear::coefficient3xPOW2() const {
  return getImpl<detail::CurveQuadraticLinear_Impl>()->coefficient3xPOW2();
}

double CurveQuadraticLinear::coefficient4y() const {
  return getImpl<detail::CurveQuadraticLinear_Impl>()->coefficient4y();
}

double CurveQuadraticLinear::coefficient5xTIMESY() const {
  return getImpl<detail::CurveQuadraticLinear_Impl>()->coefficient5xTIMESY();
}

double CurveQuadraticLinear::coefficient6xPOW2TIMESY() const {
  return getImpl<detail::CurveQuadraticLinear_Impl>()->coefficient6xPOW2TIMESY();
}

double CurveQuadraticLinear::minimumValueofx() const {
  return getImpl<detail::CurveQuadraticLinear_Impl>()->minimumValueofx();
}

double CurveQuadraticLinear::maximumValueofx() const {
  return getImpl<detail::CurveQuadraticLinear_Impl>()->maximumValueofx();
}

double CurveQuadraticLinear::minimumValueofy() const {
  return getImpl<detail::CurveQuadraticLinear_Impl>()->minimumValueofy();
}

double CurveQuadraticLinear::maximumValueofy() const {
  return getImpl<detail::CurveQuadraticLinear_Impl>()->maximumValueofy();
}

boost::optional<double> CurveQuadraticLinear::minimumCurveOutput() const {
  return getImpl<detail::CurveQuadraticLinear_Impl>()->minimumCurveOutput();
}

boost::optional<double> CurveQuadraticLinear::maximumCurveOutput() const {
  return getImpl<detail::CurveQuadraticLinear_Impl>()->maximumCurveOutput();
}

std::string CurveQuadraticLinear::inputUnitTypeforX() const {
  return getImpl<detail::CurveQuadraticLinear_Impl>()->inputUnitTypeforX();
}

bool CurveQuadraticLinear::isInputUnitTypeforXDefaulted() const {
  return getImpl<detail::CurveQuadraticLinear_Impl>()->isInputUnitTypeforXDefaulted();
}

std::string CurveQuadraticLinear::inputUnitTypeforY() const {
  return getImpl<detail::CurveQuadraticLinear_Impl>()->inputUnitTypeforY();
}

bool CurveQuadraticLinear::isInputUnitTypeforYDefaulted() const {
  return getImpl<detail::CurveQuadraticLinear_Impl>()->isInputUnitTypeforYDefaulted();
}

std::string CurveQuadraticLinear::outputUnitType() const {
  return getImpl<detail::CurveQuadraticLinear_Impl>()->outputUnitType();
}

bool CurveQuadraticLinear::isOutputUnitTypeDefaulted() const {
  return getImpl<detail::CurveQuadraticLinear_Impl>()->isOutputUnitTypeDefaulted();
}

void CurveQuadraticLinear::setCoefficient1Constant(double coefficient1Constant) {
  getImpl<detail::CurveQuadraticLinear_Impl>()->setCoefficient1Constant(coefficient1Constant);
}

void CurveQuadraticLinear::setCoefficient2x(double coefficient2x) {
  getImpl<detail::CurveQuadraticLinear_Impl>()->setCoefficient2x(coefficient2x);
}

void CurveQuadraticLinear::setCoefficient3xPOW2(double coefficient3xPOW2) {
  getImpl<detail::CurveQuadraticLinear_Impl>()->setCoefficient3xPOW2(coefficient3xPOW2);
}

void CurveQuadraticLinear::setCoefficient4y(double coefficient4y) {
  getImpl<detail::CurveQuadraticLinear_Impl>()->setCoefficient4y(coefficient4y);
}

void CurveQuadraticLinear::setCoefficient5xTIMESY(double coefficient5xTIMESY) {
  getImpl<detail::CurveQuadraticLinear_Impl>()->setCoefficient5xTIMESY(coefficient5xTIMESY);
}

void CurveQuadraticLinear::setCoefficient6xPOW2TIMESY(double coefficient6xPOW2TIMESY) {
  getImpl<detail::CurveQuadraticLinear_Impl>()->setCoefficient6xPOW2TIMESY(coefficient6xPOW2TIMESY);
}

void CurveQuadraticLinear::setMinimumValueofx(double minimumValueofx) {
  getImpl<detail::CurveQuadraticLinear_Impl>()->setMinimumValueofx(minimumValueofx);
}

void CurveQuadraticLinear::setMaximumValueofx(double maximumValueofx) {
  getImpl<detail::CurveQuadraticLinear_Impl>()->setMaximumValueofx(maximumValueofx);
}

void CurveQuadraticLinear::setMinimumValueofy(double minimumValueofy) {
  getImpl<detail::CurveQuadraticLinear_Impl>()->setMinimumValueofy(minimumValueofy);
}

void CurveQuadraticLinear::setMaximumValueofy(double maximumValueofy) {
  getImpl<detail::CurveQuadraticLinear_Impl>()->setMaximumValueofy(maximumValueofy);
}

void CurveQuadraticLinear::setMinimumCurveOutput(double minimumCurveOutput) {
  getImpl<detail::CurveQuadraticLinear_Impl>()->setMinimumCurveOutput(minimumCurveOutput);
}

void CurveQuadraticLinear::resetMinimumCurveOutput() {
  getImpl<detail::CurveQuadraticLinear_Impl>()->resetMinimumCurveOutput();
}

void CurveQuadraticLinear::setMaximumCurveOutput(double maximumCurveOutput) {
  getImpl<detail::CurveQuadraticLinear_Impl>()->setMaximumCurveOutput(maximumCurveOutput);
}

void CurveQuadraticLinear::resetMaximumCurveOutput() {
  getImpl<detail::CurveQuadraticLinear_Impl>()->resetMaximumCurveOutput();
}

bool CurveQuadraticLinear::setInputUnitTypeforX(std::string inputUnitTypeforX) {
  return getImpl<detail::CurveQuadraticLinear_Impl>()->setInputUnitTypeforX(inputUnitTypeforX);
}

void CurveQuadraticLinear::resetInputUnitTypeforX() {
  getImpl<detail::CurveQuadraticLinear_Impl>()->resetInputUnitTypeforX();
}

bool CurveQuadraticLinear::setInputUnitTypeforY(std::string inputUnitTypeforY) {
  return getImpl<detail::CurveQuadraticLinear_Impl>()->setInputUnitTypeforY(inputUnitTypeforY);
}

void CurveQuadraticLinear::resetInputUnitTypeforY() {
  getImpl<detail::CurveQuadraticLinear_Impl>()->resetInputUnitTypeforY();
}

bool CurveQuadraticLinear::setOutputUnitType(std::string outputUnitType) {
  return getImpl<detail::CurveQuadraticLinear_Impl>()->setOutputUnitType(outputUnitType);
}

void CurveQuadraticLinear::resetOutputUnitType() {
  getImpl<detail::CurveQuadraticLinear_Impl>()->resetOutputUnitType();
}

/// @cond
CurveQuadraticLinear::CurveQuadraticLinear(std::shared_ptr<detail::CurveQuadraticLinear_Impl> impl)
  : Curve(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

