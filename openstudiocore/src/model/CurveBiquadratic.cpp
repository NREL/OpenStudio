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

#include "CurveBiquadratic.hpp"
#include "CurveBiquadratic_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Curve_Biquadratic_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

#include <cmath>

using namespace std;

namespace openstudio {
namespace model {

namespace detail {

  CurveBiquadratic_Impl::CurveBiquadratic_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : Curve_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CurveBiquadratic::iddObjectType());
  }

  CurveBiquadratic_Impl::CurveBiquadratic_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                               Model_Impl* model,
                                               bool keepHandle)
    : Curve_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CurveBiquadratic::iddObjectType());
  }

  CurveBiquadratic_Impl::CurveBiquadratic_Impl(const CurveBiquadratic_Impl& other,
                                               Model_Impl* model,
                                               bool keepHandle)
    : Curve_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CurveBiquadratic_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CurveBiquadratic_Impl::iddObjectType() const {
    return CurveBiquadratic::iddObjectType();
  }

  int CurveBiquadratic_Impl::numVariables() const {
    return 2u;
  }

  double CurveBiquadratic_Impl::evaluate(const std::vector<double>& x) const {
    OS_ASSERT(x.size() == 2u);
    double result = coefficient1Constant();
    result += coefficient2x() * x[0];
    result += coefficient3xPOW2() * pow(x[0],2);
    result += coefficient4y() * x[1];
    result += coefficient5yPOW2() * pow(x[1],2);
    result += coefficient6xTIMESY() * x[0] * x[1];
    return result;
  }

  double CurveBiquadratic_Impl::coefficient1Constant() const {
    boost::optional<double> value = getDouble(OS_Curve_BiquadraticFields::Coefficient1Constant,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveBiquadratic_Impl::coefficient2x() const {
    boost::optional<double> value = getDouble(OS_Curve_BiquadraticFields::Coefficient2x,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveBiquadratic_Impl::coefficient3xPOW2() const {
    boost::optional<double> value = getDouble(OS_Curve_BiquadraticFields::Coefficient3x_POW_2,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveBiquadratic_Impl::coefficient4y() const {
    boost::optional<double> value = getDouble(OS_Curve_BiquadraticFields::Coefficient4y,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveBiquadratic_Impl::coefficient5yPOW2() const {
    boost::optional<double> value = getDouble(OS_Curve_BiquadraticFields::Coefficient5y_POW_2,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveBiquadratic_Impl::coefficient6xTIMESY() const {
    boost::optional<double> value = getDouble(OS_Curve_BiquadraticFields::Coefficient6x_TIMES_y,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveBiquadratic_Impl::minimumValueofx() const {
    boost::optional<double> value = getDouble(OS_Curve_BiquadraticFields::MinimumValueofx,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveBiquadratic_Impl::maximumValueofx() const {
    boost::optional<double> value = getDouble(OS_Curve_BiquadraticFields::MaximumValueofx,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveBiquadratic_Impl::minimumValueofy() const {
    boost::optional<double> value = getDouble(OS_Curve_BiquadraticFields::MinimumValueofy,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveBiquadratic_Impl::maximumValueofy() const {
    boost::optional<double> value = getDouble(OS_Curve_BiquadraticFields::MaximumValueofy,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> CurveBiquadratic_Impl::minimumCurveOutput() const {
    return getDouble(OS_Curve_BiquadraticFields::MinimumCurveOutput,true);
  }

  boost::optional<double> CurveBiquadratic_Impl::maximumCurveOutput() const {
    return getDouble(OS_Curve_BiquadraticFields::MaximumCurveOutput,true);
  }

  std::string CurveBiquadratic_Impl::inputUnitTypeforX() const {
    boost::optional<std::string> value = getString(OS_Curve_BiquadraticFields::InputUnitTypeforX,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CurveBiquadratic_Impl::isInputUnitTypeforXDefaulted() const {
    return isEmpty(OS_Curve_BiquadraticFields::InputUnitTypeforX);
  }

  std::string CurveBiquadratic_Impl::inputUnitTypeforY() const {
    boost::optional<std::string> value = getString(OS_Curve_BiquadraticFields::InputUnitTypeforY,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CurveBiquadratic_Impl::isInputUnitTypeforYDefaulted() const {
    return isEmpty(OS_Curve_BiquadraticFields::InputUnitTypeforY);
  }

  std::string CurveBiquadratic_Impl::outputUnitType() const {
    boost::optional<std::string> value = getString(OS_Curve_BiquadraticFields::OutputUnitType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CurveBiquadratic_Impl::isOutputUnitTypeDefaulted() const {
    return isEmpty(OS_Curve_BiquadraticFields::OutputUnitType);
  }

  void CurveBiquadratic_Impl::setCoefficient1Constant(double coefficient1Constant) {
    bool result = setDouble(OS_Curve_BiquadraticFields::Coefficient1Constant, coefficient1Constant);
    OS_ASSERT(result);
  }

  void CurveBiquadratic_Impl::setCoefficient2x(double coefficient2x) {
    bool result = setDouble(OS_Curve_BiquadraticFields::Coefficient2x, coefficient2x);
    OS_ASSERT(result);
  }

  void CurveBiquadratic_Impl::setCoefficient3xPOW2(double coefficient3xPOW2) {
    bool result = setDouble(OS_Curve_BiquadraticFields::Coefficient3x_POW_2, coefficient3xPOW2);
    OS_ASSERT(result);
  }

  void CurveBiquadratic_Impl::setCoefficient4y(double coefficient4y) {
    bool result = setDouble(OS_Curve_BiquadraticFields::Coefficient4y, coefficient4y);
    OS_ASSERT(result);
  }

  void CurveBiquadratic_Impl::setCoefficient5yPOW2(double coefficient5yPOW2) {
    bool result = setDouble(OS_Curve_BiquadraticFields::Coefficient5y_POW_2, coefficient5yPOW2);
    OS_ASSERT(result);
  }

  void CurveBiquadratic_Impl::setCoefficient6xTIMESY(double coefficient6xTIMESY) {
    bool result = setDouble(OS_Curve_BiquadraticFields::Coefficient6x_TIMES_y, coefficient6xTIMESY);
    OS_ASSERT(result);
  }

  void CurveBiquadratic_Impl::setMinimumValueofx(double minimumValueofx) {
    bool result = setDouble(OS_Curve_BiquadraticFields::MinimumValueofx, minimumValueofx);
    OS_ASSERT(result);
  }

  void CurveBiquadratic_Impl::setMaximumValueofx(double maximumValueofx) {
    bool result = setDouble(OS_Curve_BiquadraticFields::MaximumValueofx, maximumValueofx);
    OS_ASSERT(result);
  }

  void CurveBiquadratic_Impl::setMinimumValueofy(double minimumValueofy) {
    bool result = setDouble(OS_Curve_BiquadraticFields::MinimumValueofy, minimumValueofy);
    OS_ASSERT(result);
  }

  void CurveBiquadratic_Impl::setMaximumValueofy(double maximumValueofy) {
    bool result = setDouble(OS_Curve_BiquadraticFields::MaximumValueofy, maximumValueofy);
    OS_ASSERT(result);
  }

  void CurveBiquadratic_Impl::setMinimumCurveOutput(boost::optional<double> minimumCurveOutput) {
    bool result = false;
    if (minimumCurveOutput) {
      result = setDouble(OS_Curve_BiquadraticFields::MinimumCurveOutput, minimumCurveOutput.get());
    } else {
      result = setString(OS_Curve_BiquadraticFields::MinimumCurveOutput, "");
    }
    OS_ASSERT(result);
  }

  void CurveBiquadratic_Impl::resetMinimumCurveOutput() {
    bool result = setString(OS_Curve_BiquadraticFields::MinimumCurveOutput, "");
    OS_ASSERT(result);
  }

  void CurveBiquadratic_Impl::setMaximumCurveOutput(boost::optional<double> maximumCurveOutput) {
    bool result = false;
    if (maximumCurveOutput) {
      result = setDouble(OS_Curve_BiquadraticFields::MaximumCurveOutput, maximumCurveOutput.get());
    } else {
      result = setString(OS_Curve_BiquadraticFields::MaximumCurveOutput, "");
    }
    OS_ASSERT(result);
  }

  void CurveBiquadratic_Impl::resetMaximumCurveOutput() {
    bool result = setString(OS_Curve_BiquadraticFields::MaximumCurveOutput, "");
    OS_ASSERT(result);
  }

  bool CurveBiquadratic_Impl::setInputUnitTypeforX(std::string inputUnitTypeforX) {
    bool result = setString(OS_Curve_BiquadraticFields::InputUnitTypeforX, inputUnitTypeforX);
    return result;
  }

  void CurveBiquadratic_Impl::resetInputUnitTypeforX() {
    bool result = setString(OS_Curve_BiquadraticFields::InputUnitTypeforX, "");
    OS_ASSERT(result);
  }

  bool CurveBiquadratic_Impl::setInputUnitTypeforY(std::string inputUnitTypeforY) {
    bool result = setString(OS_Curve_BiquadraticFields::InputUnitTypeforY, inputUnitTypeforY);
    return result;
  }

  void CurveBiquadratic_Impl::resetInputUnitTypeforY() {
    bool result = setString(OS_Curve_BiquadraticFields::InputUnitTypeforY, "");
    OS_ASSERT(result);
  }

  bool CurveBiquadratic_Impl::setOutputUnitType(std::string outputUnitType) {
    bool result = setString(OS_Curve_BiquadraticFields::OutputUnitType, outputUnitType);
    return result;
  }

  void CurveBiquadratic_Impl::resetOutputUnitType() {
    bool result = setString(OS_Curve_BiquadraticFields::OutputUnitType, "");
    OS_ASSERT(result);
  }

} // detail

CurveBiquadratic::CurveBiquadratic(const Model& model)
  : Curve(CurveBiquadratic::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CurveBiquadratic_Impl>());
  setDouble(OS_Curve_BiquadraticFields::Coefficient1Constant,0.0);
  setDouble(OS_Curve_BiquadraticFields::Coefficient2x,0.0);
  setDouble(OS_Curve_BiquadraticFields::Coefficient3x_POW_2,0.0);
  setDouble(OS_Curve_BiquadraticFields::Coefficient4y,0.0);
  setDouble(OS_Curve_BiquadraticFields::Coefficient5y_POW_2,0.0);
  setDouble(OS_Curve_BiquadraticFields::Coefficient6x_TIMES_y,0.0);
  setDouble(OS_Curve_BiquadraticFields::MinimumValueofx,0.0);
  setDouble(OS_Curve_BiquadraticFields::MaximumValueofx,1.0);
  setDouble(OS_Curve_BiquadraticFields::MinimumValueofy,0.0);
  setDouble(OS_Curve_BiquadraticFields::MaximumValueofy,1.0);
}

IddObjectType CurveBiquadratic::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Curve_Biquadratic);
  return result;
}

std::vector<std::string> CurveBiquadratic::validInputUnitTypeforXValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Curve_BiquadraticFields::InputUnitTypeforX);
}

std::vector<std::string> CurveBiquadratic::validInputUnitTypeforYValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Curve_BiquadraticFields::InputUnitTypeforY);
}

std::vector<std::string> CurveBiquadratic::validOutputUnitTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Curve_BiquadraticFields::OutputUnitType);
}

double CurveBiquadratic::coefficient1Constant() const {
  return getImpl<detail::CurveBiquadratic_Impl>()->coefficient1Constant();
}

double CurveBiquadratic::coefficient2x() const {
  return getImpl<detail::CurveBiquadratic_Impl>()->coefficient2x();
}

double CurveBiquadratic::coefficient3xPOW2() const {
  return getImpl<detail::CurveBiquadratic_Impl>()->coefficient3xPOW2();
}

double CurveBiquadratic::coefficient4y() const {
  return getImpl<detail::CurveBiquadratic_Impl>()->coefficient4y();
}

double CurveBiquadratic::coefficient5yPOW2() const {
  return getImpl<detail::CurveBiquadratic_Impl>()->coefficient5yPOW2();
}

double CurveBiquadratic::coefficient6xTIMESY() const {
  return getImpl<detail::CurveBiquadratic_Impl>()->coefficient6xTIMESY();
}

double CurveBiquadratic::minimumValueofx() const {
  return getImpl<detail::CurveBiquadratic_Impl>()->minimumValueofx();
}

double CurveBiquadratic::maximumValueofx() const {
  return getImpl<detail::CurveBiquadratic_Impl>()->maximumValueofx();
}

double CurveBiquadratic::minimumValueofy() const {
  return getImpl<detail::CurveBiquadratic_Impl>()->minimumValueofy();
}

double CurveBiquadratic::maximumValueofy() const {
  return getImpl<detail::CurveBiquadratic_Impl>()->maximumValueofy();
}

boost::optional<double> CurveBiquadratic::minimumCurveOutput() const {
  return getImpl<detail::CurveBiquadratic_Impl>()->minimumCurveOutput();
}

boost::optional<double> CurveBiquadratic::maximumCurveOutput() const {
  return getImpl<detail::CurveBiquadratic_Impl>()->maximumCurveOutput();
}

std::string CurveBiquadratic::inputUnitTypeforX() const {
  return getImpl<detail::CurveBiquadratic_Impl>()->inputUnitTypeforX();
}

bool CurveBiquadratic::isInputUnitTypeforXDefaulted() const {
  return getImpl<detail::CurveBiquadratic_Impl>()->isInputUnitTypeforXDefaulted();
}

std::string CurveBiquadratic::inputUnitTypeforY() const {
  return getImpl<detail::CurveBiquadratic_Impl>()->inputUnitTypeforY();
}

bool CurveBiquadratic::isInputUnitTypeforYDefaulted() const {
  return getImpl<detail::CurveBiquadratic_Impl>()->isInputUnitTypeforYDefaulted();
}

std::string CurveBiquadratic::outputUnitType() const {
  return getImpl<detail::CurveBiquadratic_Impl>()->outputUnitType();
}

bool CurveBiquadratic::isOutputUnitTypeDefaulted() const {
  return getImpl<detail::CurveBiquadratic_Impl>()->isOutputUnitTypeDefaulted();
}

void CurveBiquadratic::setCoefficient1Constant(double coefficient1Constant) {
  getImpl<detail::CurveBiquadratic_Impl>()->setCoefficient1Constant(coefficient1Constant);
}

void CurveBiquadratic::setCoefficient2x(double coefficient2x) {
  getImpl<detail::CurveBiquadratic_Impl>()->setCoefficient2x(coefficient2x);
}

void CurveBiquadratic::setCoefficient3xPOW2(double coefficient3xPOW2) {
  getImpl<detail::CurveBiquadratic_Impl>()->setCoefficient3xPOW2(coefficient3xPOW2);
}

void CurveBiquadratic::setCoefficient4y(double coefficient4y) {
  getImpl<detail::CurveBiquadratic_Impl>()->setCoefficient4y(coefficient4y);
}

void CurveBiquadratic::setCoefficient5yPOW2(double coefficient5yPOW2) {
  getImpl<detail::CurveBiquadratic_Impl>()->setCoefficient5yPOW2(coefficient5yPOW2);
}

void CurveBiquadratic::setCoefficient6xTIMESY(double coefficient6xTIMESY) {
  getImpl<detail::CurveBiquadratic_Impl>()->setCoefficient6xTIMESY(coefficient6xTIMESY);
}

void CurveBiquadratic::setMinimumValueofx(double minimumValueofx) {
  getImpl<detail::CurveBiquadratic_Impl>()->setMinimumValueofx(minimumValueofx);
}

void CurveBiquadratic::setMaximumValueofx(double maximumValueofx) {
  getImpl<detail::CurveBiquadratic_Impl>()->setMaximumValueofx(maximumValueofx);
}

void CurveBiquadratic::setMinimumValueofy(double minimumValueofy) {
  getImpl<detail::CurveBiquadratic_Impl>()->setMinimumValueofy(minimumValueofy);
}

void CurveBiquadratic::setMaximumValueofy(double maximumValueofy) {
  getImpl<detail::CurveBiquadratic_Impl>()->setMaximumValueofy(maximumValueofy);
}

void CurveBiquadratic::setMinimumCurveOutput(double minimumCurveOutput) {
  getImpl<detail::CurveBiquadratic_Impl>()->setMinimumCurveOutput(minimumCurveOutput);
}

void CurveBiquadratic::resetMinimumCurveOutput() {
  getImpl<detail::CurveBiquadratic_Impl>()->resetMinimumCurveOutput();
}

void CurveBiquadratic::setMaximumCurveOutput(double maximumCurveOutput) {
  getImpl<detail::CurveBiquadratic_Impl>()->setMaximumCurveOutput(maximumCurveOutput);
}

void CurveBiquadratic::resetMaximumCurveOutput() {
  getImpl<detail::CurveBiquadratic_Impl>()->resetMaximumCurveOutput();
}

bool CurveBiquadratic::setInputUnitTypeforX(std::string inputUnitTypeforX) {
  return getImpl<detail::CurveBiquadratic_Impl>()->setInputUnitTypeforX(inputUnitTypeforX);
}

void CurveBiquadratic::resetInputUnitTypeforX() {
  getImpl<detail::CurveBiquadratic_Impl>()->resetInputUnitTypeforX();
}

bool CurveBiquadratic::setInputUnitTypeforY(std::string inputUnitTypeforY) {
  return getImpl<detail::CurveBiquadratic_Impl>()->setInputUnitTypeforY(inputUnitTypeforY);
}

void CurveBiquadratic::resetInputUnitTypeforY() {
  getImpl<detail::CurveBiquadratic_Impl>()->resetInputUnitTypeforY();
}

bool CurveBiquadratic::setOutputUnitType(std::string outputUnitType) {
  return getImpl<detail::CurveBiquadratic_Impl>()->setOutputUnitType(outputUnitType);
}

void CurveBiquadratic::resetOutputUnitType() {
  getImpl<detail::CurveBiquadratic_Impl>()->resetOutputUnitType();
}

/// @cond
CurveBiquadratic::CurveBiquadratic(std::shared_ptr<detail::CurveBiquadratic_Impl> impl)
  : Curve(std::move(impl))
{}
/// @endcond

} // model
} // openstudio
