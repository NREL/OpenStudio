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

#include "CurveBicubic.hpp"
#include "CurveBicubic_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Curve_Bicubic_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

#include <cmath>

using namespace std;

namespace openstudio {
namespace model {

namespace detail {

  CurveBicubic_Impl::CurveBicubic_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : Curve_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CurveBicubic::iddObjectType());
  }

  CurveBicubic_Impl::CurveBicubic_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                       Model_Impl* model,
                                       bool keepHandle)
    : Curve_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CurveBicubic::iddObjectType());
  }

  CurveBicubic_Impl::CurveBicubic_Impl(const CurveBicubic_Impl& other,
                                       Model_Impl* model,
                                       bool keepHandle)
    : Curve_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CurveBicubic_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CurveBicubic_Impl::iddObjectType() const {
    return CurveBicubic::iddObjectType();
  }

  int CurveBicubic_Impl::numVariables() const {
    return 2;
  }

  double CurveBicubic_Impl::evaluate(const std::vector<double>& x) const {
    OS_ASSERT(x.size() == 2u);

    double result = coefficient1Constant();
    double x2 = pow(x[0],2);
    double y2 = pow(x[1],2);
    result += coefficient2x() * x[0];
    result += coefficient3xPOW2() * x2;
    result += coefficient4y() * x[1];
    result += coefficient5yPOW2() * y2;
    result += coefficient6xTIMESY() * x[0] * x[1];
    result += coefficient7xPOW3() * pow(x[0],3);
    result += coefficient8yPOW3() * pow(x[1],3);
    result += coefficient9xPOW2TIMESY() * x2 * x[1];
    result += coefficient10xTIMESYPOW2() * x[0] * y2;
    return result;
  }

  double CurveBicubic_Impl::coefficient1Constant() const {
    boost::optional<double> value = getDouble(OS_Curve_BicubicFields::Coefficient1Constant,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveBicubic_Impl::coefficient2x() const {
    boost::optional<double> value = getDouble(OS_Curve_BicubicFields::Coefficient2x,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveBicubic_Impl::coefficient3xPOW2() const {
    boost::optional<double> value = getDouble(OS_Curve_BicubicFields::Coefficient3x_POW_2,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveBicubic_Impl::coefficient4y() const {
    boost::optional<double> value = getDouble(OS_Curve_BicubicFields::Coefficient4y,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveBicubic_Impl::coefficient5yPOW2() const {
    boost::optional<double> value = getDouble(OS_Curve_BicubicFields::Coefficient5y_POW_2,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveBicubic_Impl::coefficient6xTIMESY() const {
    boost::optional<double> value = getDouble(OS_Curve_BicubicFields::Coefficient6x_TIMES_y,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveBicubic_Impl::coefficient7xPOW3() const {
    boost::optional<double> value = getDouble(OS_Curve_BicubicFields::Coefficient7x_POW_3,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveBicubic_Impl::coefficient8yPOW3() const {
    boost::optional<double> value = getDouble(OS_Curve_BicubicFields::Coefficient8y_POW_3,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveBicubic_Impl::coefficient9xPOW2TIMESY() const {
    boost::optional<double> value = getDouble(OS_Curve_BicubicFields::Coefficient9x_POW_2_TIMES_y,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveBicubic_Impl::coefficient10xTIMESYPOW2() const {
    boost::optional<double> value = getDouble(OS_Curve_BicubicFields::Coefficient10x_TIMES_y_POW_2,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveBicubic_Impl::minimumValueofx() const {
    boost::optional<double> value = getDouble(OS_Curve_BicubicFields::MinimumValueofx,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveBicubic_Impl::maximumValueofx() const {
    boost::optional<double> value = getDouble(OS_Curve_BicubicFields::MaximumValueofx,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveBicubic_Impl::minimumValueofy() const {
    boost::optional<double> value = getDouble(OS_Curve_BicubicFields::MinimumValueofy,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveBicubic_Impl::maximumValueofy() const {
    boost::optional<double> value = getDouble(OS_Curve_BicubicFields::MaximumValueofy,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> CurveBicubic_Impl::minimumCurveOutput() const {
    return getDouble(OS_Curve_BicubicFields::MinimumCurveOutput,true);
  }

  boost::optional<double> CurveBicubic_Impl::maximumCurveOutput() const {
    return getDouble(OS_Curve_BicubicFields::MaximumCurveOutput,true);
  }

  std::string CurveBicubic_Impl::inputUnitTypeforX() const {
    boost::optional<std::string> value = getString(OS_Curve_BicubicFields::InputUnitTypeforX,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CurveBicubic_Impl::isInputUnitTypeforXDefaulted() const {
    return isEmpty(OS_Curve_BicubicFields::InputUnitTypeforX);
  }

  std::string CurveBicubic_Impl::inputUnitTypeforY() const {
    boost::optional<std::string> value = getString(OS_Curve_BicubicFields::InputUnitTypeforY,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CurveBicubic_Impl::isInputUnitTypeforYDefaulted() const {
    return isEmpty(OS_Curve_BicubicFields::InputUnitTypeforY);
  }

  std::string CurveBicubic_Impl::outputUnitType() const {
    boost::optional<std::string> value = getString(OS_Curve_BicubicFields::OutputUnitType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CurveBicubic_Impl::isOutputUnitTypeDefaulted() const {
    return isEmpty(OS_Curve_BicubicFields::OutputUnitType);
  }

  void CurveBicubic_Impl::setCoefficient1Constant(double coefficient1Constant) {
    bool result = setDouble(OS_Curve_BicubicFields::Coefficient1Constant, coefficient1Constant);
    OS_ASSERT(result);
  }

  void CurveBicubic_Impl::setCoefficient2x(double coefficient2x) {
    bool result = setDouble(OS_Curve_BicubicFields::Coefficient2x, coefficient2x);
    OS_ASSERT(result);
  }

  void CurveBicubic_Impl::setCoefficient3xPOW2(double coefficient3xPOW2) {
    bool result = setDouble(OS_Curve_BicubicFields::Coefficient3x_POW_2, coefficient3xPOW2);
    OS_ASSERT(result);
  }

  void CurveBicubic_Impl::setCoefficient4y(double coefficient4y) {
    bool result = setDouble(OS_Curve_BicubicFields::Coefficient4y, coefficient4y);
    OS_ASSERT(result);
  }

  void CurveBicubic_Impl::setCoefficient5yPOW2(double coefficient5yPOW2) {
    bool result = setDouble(OS_Curve_BicubicFields::Coefficient5y_POW_2, coefficient5yPOW2);
    OS_ASSERT(result);
  }

  void CurveBicubic_Impl::setCoefficient6xTIMESY(double coefficient6xTIMESY) {
    bool result = setDouble(OS_Curve_BicubicFields::Coefficient6x_TIMES_y, coefficient6xTIMESY);
    OS_ASSERT(result);
  }

  void CurveBicubic_Impl::setCoefficient7xPOW3(double coefficient7xPOW3) {
    bool result = setDouble(OS_Curve_BicubicFields::Coefficient7x_POW_3, coefficient7xPOW3);
    OS_ASSERT(result);
  }

  void CurveBicubic_Impl::setCoefficient8yPOW3(double coefficient8yPOW3) {
    bool result = setDouble(OS_Curve_BicubicFields::Coefficient8y_POW_3, coefficient8yPOW3);
    OS_ASSERT(result);
  }

  void CurveBicubic_Impl::setCoefficient9xPOW2TIMESY(double coefficient9xPOW2TIMESY) {
    bool result = setDouble(OS_Curve_BicubicFields::Coefficient9x_POW_2_TIMES_y, coefficient9xPOW2TIMESY);
    OS_ASSERT(result);
  }

  void CurveBicubic_Impl::setCoefficient10xTIMESYPOW2(double coefficient10xTIMESYPOW2) {
    bool result = setDouble(OS_Curve_BicubicFields::Coefficient10x_TIMES_y_POW_2, coefficient10xTIMESYPOW2);
    OS_ASSERT(result);
  }

  void CurveBicubic_Impl::setMinimumValueofx(double minimumValueofx) {
    bool result = setDouble(OS_Curve_BicubicFields::MinimumValueofx, minimumValueofx);
    OS_ASSERT(result);
  }

  void CurveBicubic_Impl::setMaximumValueofx(double maximumValueofx) {
    bool result = setDouble(OS_Curve_BicubicFields::MaximumValueofx, maximumValueofx);
    OS_ASSERT(result);
  }

  void CurveBicubic_Impl::setMinimumValueofy(double minimumValueofy) {
    bool result = setDouble(OS_Curve_BicubicFields::MinimumValueofy, minimumValueofy);
    OS_ASSERT(result);
  }

  void CurveBicubic_Impl::setMaximumValueofy(double maximumValueofy) {
    bool result = setDouble(OS_Curve_BicubicFields::MaximumValueofy, maximumValueofy);
    OS_ASSERT(result);
  }

  void CurveBicubic_Impl::setMinimumCurveOutput(boost::optional<double> minimumCurveOutput) {
    bool result = false;
    if (minimumCurveOutput) {
      result = setDouble(OS_Curve_BicubicFields::MinimumCurveOutput, minimumCurveOutput.get());
    } else {
      result = setString(OS_Curve_BicubicFields::MinimumCurveOutput, "");
    }
    OS_ASSERT(result);
  }

  void CurveBicubic_Impl::resetMinimumCurveOutput() {
    bool result = setString(OS_Curve_BicubicFields::MinimumCurveOutput, "");
    OS_ASSERT(result);
  }

  void CurveBicubic_Impl::setMaximumCurveOutput(boost::optional<double> maximumCurveOutput) {
    bool result = false;
    if (maximumCurveOutput) {
      result = setDouble(OS_Curve_BicubicFields::MaximumCurveOutput, maximumCurveOutput.get());
    } else {
      result = setString(OS_Curve_BicubicFields::MaximumCurveOutput, "");
    }
    OS_ASSERT(result);
  }

  void CurveBicubic_Impl::resetMaximumCurveOutput() {
    bool result = setString(OS_Curve_BicubicFields::MaximumCurveOutput, "");
    OS_ASSERT(result);
  }

  bool CurveBicubic_Impl::setInputUnitTypeforX(std::string inputUnitTypeforX) {
    bool result = setString(OS_Curve_BicubicFields::InputUnitTypeforX, inputUnitTypeforX);
    return result;
  }

  void CurveBicubic_Impl::resetInputUnitTypeforX() {
    bool result = setString(OS_Curve_BicubicFields::InputUnitTypeforX, "");
    OS_ASSERT(result);
  }

  bool CurveBicubic_Impl::setInputUnitTypeforY(std::string inputUnitTypeforY) {
    bool result = setString(OS_Curve_BicubicFields::InputUnitTypeforY, inputUnitTypeforY);
    return result;
  }

  void CurveBicubic_Impl::resetInputUnitTypeforY() {
    bool result = setString(OS_Curve_BicubicFields::InputUnitTypeforY, "");
    OS_ASSERT(result);
  }

  bool CurveBicubic_Impl::setOutputUnitType(std::string outputUnitType) {
    bool result = setString(OS_Curve_BicubicFields::OutputUnitType, outputUnitType);
    return result;
  }

  void CurveBicubic_Impl::resetOutputUnitType() {
    bool result = setString(OS_Curve_BicubicFields::OutputUnitType, "");
    OS_ASSERT(result);
  }

} // detail

CurveBicubic::CurveBicubic(const Model& model)
  : Curve(CurveBicubic::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CurveBicubic_Impl>());
  setDouble(OS_Curve_BicubicFields::Coefficient1Constant,0.0);
  setDouble(OS_Curve_BicubicFields::Coefficient2x,0.0);
  setDouble(OS_Curve_BicubicFields::Coefficient3x_POW_2,0.0);
  setDouble(OS_Curve_BicubicFields::Coefficient4y,0.0);
  setDouble(OS_Curve_BicubicFields::Coefficient5y_POW_2,0.0);
  setDouble(OS_Curve_BicubicFields::Coefficient6x_TIMES_y,0.0);
  setDouble(OS_Curve_BicubicFields::Coefficient7x_POW_3,0.0);
  setDouble(OS_Curve_BicubicFields::Coefficient8y_POW_3,0.0);
  setDouble(OS_Curve_BicubicFields::Coefficient9x_POW_2_TIMES_y,0.0);
  setDouble(OS_Curve_BicubicFields::Coefficient10x_TIMES_y_POW_2,0.0);
  setDouble(OS_Curve_BicubicFields::MinimumValueofx,0.0);
  setDouble(OS_Curve_BicubicFields::MaximumValueofx,1.0);
  setDouble(OS_Curve_BicubicFields::MinimumValueofy,0.0);
  setDouble(OS_Curve_BicubicFields::MaximumValueofy,1.0);
}

IddObjectType CurveBicubic::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Curve_Bicubic);
  return result;
}

std::vector<std::string> CurveBicubic::validInputUnitTypeforXValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Curve_BicubicFields::InputUnitTypeforX);
}

std::vector<std::string> CurveBicubic::validInputUnitTypeforYValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Curve_BicubicFields::InputUnitTypeforY);
}

std::vector<std::string> CurveBicubic::validOutputUnitTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Curve_BicubicFields::OutputUnitType);
}

double CurveBicubic::coefficient1Constant() const {
  return getImpl<detail::CurveBicubic_Impl>()->coefficient1Constant();
}

double CurveBicubic::coefficient2x() const {
  return getImpl<detail::CurveBicubic_Impl>()->coefficient2x();
}

double CurveBicubic::coefficient3xPOW2() const {
  return getImpl<detail::CurveBicubic_Impl>()->coefficient3xPOW2();
}

double CurveBicubic::coefficient4y() const {
  return getImpl<detail::CurveBicubic_Impl>()->coefficient4y();
}

double CurveBicubic::coefficient5yPOW2() const {
  return getImpl<detail::CurveBicubic_Impl>()->coefficient5yPOW2();
}

double CurveBicubic::coefficient6xTIMESY() const {
  return getImpl<detail::CurveBicubic_Impl>()->coefficient6xTIMESY();
}

double CurveBicubic::coefficient7xPOW3() const {
  return getImpl<detail::CurveBicubic_Impl>()->coefficient7xPOW3();
}

double CurveBicubic::coefficient8yPOW3() const {
  return getImpl<detail::CurveBicubic_Impl>()->coefficient8yPOW3();
}

double CurveBicubic::coefficient9xPOW2TIMESY() const {
  return getImpl<detail::CurveBicubic_Impl>()->coefficient9xPOW2TIMESY();
}

double CurveBicubic::coefficient10xTIMESYPOW2() const {
  return getImpl<detail::CurveBicubic_Impl>()->coefficient10xTIMESYPOW2();
}

double CurveBicubic::minimumValueofx() const {
  return getImpl<detail::CurveBicubic_Impl>()->minimumValueofx();
}

double CurveBicubic::maximumValueofx() const {
  return getImpl<detail::CurveBicubic_Impl>()->maximumValueofx();
}

double CurveBicubic::minimumValueofy() const {
  return getImpl<detail::CurveBicubic_Impl>()->minimumValueofy();
}

double CurveBicubic::maximumValueofy() const {
  return getImpl<detail::CurveBicubic_Impl>()->maximumValueofy();
}

boost::optional<double> CurveBicubic::minimumCurveOutput() const {
  return getImpl<detail::CurveBicubic_Impl>()->minimumCurveOutput();
}

boost::optional<double> CurveBicubic::maximumCurveOutput() const {
  return getImpl<detail::CurveBicubic_Impl>()->maximumCurveOutput();
}

std::string CurveBicubic::inputUnitTypeforX() const {
  return getImpl<detail::CurveBicubic_Impl>()->inputUnitTypeforX();
}

bool CurveBicubic::isInputUnitTypeforXDefaulted() const {
  return getImpl<detail::CurveBicubic_Impl>()->isInputUnitTypeforXDefaulted();
}

std::string CurveBicubic::inputUnitTypeforY() const {
  return getImpl<detail::CurveBicubic_Impl>()->inputUnitTypeforY();
}

bool CurveBicubic::isInputUnitTypeforYDefaulted() const {
  return getImpl<detail::CurveBicubic_Impl>()->isInputUnitTypeforYDefaulted();
}

std::string CurveBicubic::outputUnitType() const {
  return getImpl<detail::CurveBicubic_Impl>()->outputUnitType();
}

bool CurveBicubic::isOutputUnitTypeDefaulted() const {
  return getImpl<detail::CurveBicubic_Impl>()->isOutputUnitTypeDefaulted();
}

void CurveBicubic::setCoefficient1Constant(double coefficient1Constant) {
  getImpl<detail::CurveBicubic_Impl>()->setCoefficient1Constant(coefficient1Constant);
}

void CurveBicubic::setCoefficient2x(double coefficient2x) {
  getImpl<detail::CurveBicubic_Impl>()->setCoefficient2x(coefficient2x);
}

void CurveBicubic::setCoefficient3xPOW2(double coefficient3xPOW2) {
  getImpl<detail::CurveBicubic_Impl>()->setCoefficient3xPOW2(coefficient3xPOW2);
}

void CurveBicubic::setCoefficient4y(double coefficient4y) {
  getImpl<detail::CurveBicubic_Impl>()->setCoefficient4y(coefficient4y);
}

void CurveBicubic::setCoefficient5yPOW2(double coefficient5yPOW2) {
  getImpl<detail::CurveBicubic_Impl>()->setCoefficient5yPOW2(coefficient5yPOW2);
}

void CurveBicubic::setCoefficient6xTIMESY(double coefficient6xTIMESY) {
  getImpl<detail::CurveBicubic_Impl>()->setCoefficient6xTIMESY(coefficient6xTIMESY);
}

void CurveBicubic::setCoefficient7xPOW3(double coefficient7xPOW3) {
  getImpl<detail::CurveBicubic_Impl>()->setCoefficient7xPOW3(coefficient7xPOW3);
}

void CurveBicubic::setCoefficient8yPOW3(double coefficient8yPOW3) {
  getImpl<detail::CurveBicubic_Impl>()->setCoefficient8yPOW3(coefficient8yPOW3);
}

void CurveBicubic::setCoefficient9xPOW2TIMESY(double coefficient9xPOW2TIMESY) {
  getImpl<detail::CurveBicubic_Impl>()->setCoefficient9xPOW2TIMESY(coefficient9xPOW2TIMESY);
}

void CurveBicubic::setCoefficient10xTIMESYPOW2(double coefficient10xTIMESYPOW2) {
  getImpl<detail::CurveBicubic_Impl>()->setCoefficient10xTIMESYPOW2(coefficient10xTIMESYPOW2);
}

void CurveBicubic::setMinimumValueofx(double minimumValueofx) {
  getImpl<detail::CurveBicubic_Impl>()->setMinimumValueofx(minimumValueofx);
}

void CurveBicubic::setMaximumValueofx(double maximumValueofx) {
  getImpl<detail::CurveBicubic_Impl>()->setMaximumValueofx(maximumValueofx);
}

void CurveBicubic::setMinimumValueofy(double minimumValueofy) {
  getImpl<detail::CurveBicubic_Impl>()->setMinimumValueofy(minimumValueofy);
}

void CurveBicubic::setMaximumValueofy(double maximumValueofy) {
  getImpl<detail::CurveBicubic_Impl>()->setMaximumValueofy(maximumValueofy);
}

void CurveBicubic::setMinimumCurveOutput(double minimumCurveOutput) {
  getImpl<detail::CurveBicubic_Impl>()->setMinimumCurveOutput(minimumCurveOutput);
}

void CurveBicubic::resetMinimumCurveOutput() {
  getImpl<detail::CurveBicubic_Impl>()->resetMinimumCurveOutput();
}

void CurveBicubic::setMaximumCurveOutput(double maximumCurveOutput) {
  getImpl<detail::CurveBicubic_Impl>()->setMaximumCurveOutput(maximumCurveOutput);
}

void CurveBicubic::resetMaximumCurveOutput() {
  getImpl<detail::CurveBicubic_Impl>()->resetMaximumCurveOutput();
}

bool CurveBicubic::setInputUnitTypeforX(std::string inputUnitTypeforX) {
  return getImpl<detail::CurveBicubic_Impl>()->setInputUnitTypeforX(inputUnitTypeforX);
}

void CurveBicubic::resetInputUnitTypeforX() {
  getImpl<detail::CurveBicubic_Impl>()->resetInputUnitTypeforX();
}

bool CurveBicubic::setInputUnitTypeforY(std::string inputUnitTypeforY) {
  return getImpl<detail::CurveBicubic_Impl>()->setInputUnitTypeforY(inputUnitTypeforY);
}

void CurveBicubic::resetInputUnitTypeforY() {
  getImpl<detail::CurveBicubic_Impl>()->resetInputUnitTypeforY();
}

bool CurveBicubic::setOutputUnitType(std::string outputUnitType) {
  return getImpl<detail::CurveBicubic_Impl>()->setOutputUnitType(outputUnitType);
}

void CurveBicubic::resetOutputUnitType() {
  getImpl<detail::CurveBicubic_Impl>()->resetOutputUnitType();
}

/// @cond
CurveBicubic::CurveBicubic(std::shared_ptr<detail::CurveBicubic_Impl> impl)
  : Curve(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

