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

#include "CurveCubic.hpp"
#include "CurveCubic_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Curve_Cubic_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

#include <cmath>

using namespace std;

namespace openstudio {
namespace model {

namespace detail {

  CurveCubic_Impl::CurveCubic_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : Curve_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CurveCubic::iddObjectType());
  }

  CurveCubic_Impl::CurveCubic_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                   Model_Impl* model,
                                   bool keepHandle)
    : Curve_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CurveCubic::iddObjectType());
  }

  CurveCubic_Impl::CurveCubic_Impl(const CurveCubic_Impl& other,
                                   Model_Impl* model,
                                   bool keepHandle)
    : Curve_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CurveCubic_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CurveCubic_Impl::iddObjectType() const {
    return CurveCubic::iddObjectType();
  }

  int CurveCubic_Impl::numVariables() const {
    return 1;
  }

  double CurveCubic_Impl::evaluate(const std::vector<double>& x) const {
    OS_ASSERT(x.size() == 1u);
    double result = coefficient1Constant();
    result += coefficient2x() * x[0];
    result += coefficient3xPOW2() * pow(x[0],2);
    result += coefficient4xPOW3() * pow(x[0],3);
    return result;
  }

  double CurveCubic_Impl::coefficient1Constant() const {
    boost::optional<double> value = getDouble(OS_Curve_CubicFields::Coefficient1Constant,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveCubic_Impl::coefficient2x() const {
    boost::optional<double> value = getDouble(OS_Curve_CubicFields::Coefficient2x,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveCubic_Impl::coefficient3xPOW2() const {
    boost::optional<double> value = getDouble(OS_Curve_CubicFields::Coefficient3x_POW_2,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveCubic_Impl::coefficient4xPOW3() const {
    boost::optional<double> value = getDouble(OS_Curve_CubicFields::Coefficient4x_POW_3,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveCubic_Impl::minimumValueofx() const {
    boost::optional<double> value = getDouble(OS_Curve_CubicFields::MinimumValueofx,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveCubic_Impl::maximumValueofx() const {
    boost::optional<double> value = getDouble(OS_Curve_CubicFields::MaximumValueofx,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> CurveCubic_Impl::minimumCurveOutput() const {
    return getDouble(OS_Curve_CubicFields::MinimumCurveOutput,true);
  }

  boost::optional<double> CurveCubic_Impl::maximumCurveOutput() const {
    return getDouble(OS_Curve_CubicFields::MaximumCurveOutput,true);
  }

  std::string CurveCubic_Impl::inputUnitTypeforX() const {
    boost::optional<std::string> value = getString(OS_Curve_CubicFields::InputUnitTypeforX,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CurveCubic_Impl::isInputUnitTypeforXDefaulted() const {
    return isEmpty(OS_Curve_CubicFields::InputUnitTypeforX);
  }

  std::string CurveCubic_Impl::outputUnitType() const {
    boost::optional<std::string> value = getString(OS_Curve_CubicFields::OutputUnitType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CurveCubic_Impl::isOutputUnitTypeDefaulted() const {
    return isEmpty(OS_Curve_CubicFields::OutputUnitType);
  }

  void CurveCubic_Impl::setCoefficient1Constant(double coefficient1Constant) {
    bool result = setDouble(OS_Curve_CubicFields::Coefficient1Constant, coefficient1Constant);
    OS_ASSERT(result);
  }

  void CurveCubic_Impl::setCoefficient2x(double coefficient2x) {
    bool result = setDouble(OS_Curve_CubicFields::Coefficient2x, coefficient2x);
    OS_ASSERT(result);
  }

  void CurveCubic_Impl::setCoefficient3xPOW2(double coefficient3xPOW2) {
    bool result = setDouble(OS_Curve_CubicFields::Coefficient3x_POW_2, coefficient3xPOW2);
    OS_ASSERT(result);
  }

  void CurveCubic_Impl::setCoefficient4xPOW3(double coefficient4xPOW3) {
    bool result = setDouble(OS_Curve_CubicFields::Coefficient4x_POW_3, coefficient4xPOW3);
    OS_ASSERT(result);
  }

  void CurveCubic_Impl::setMinimumValueofx(double minimumValueofx) {
    bool result = setDouble(OS_Curve_CubicFields::MinimumValueofx, minimumValueofx);
    OS_ASSERT(result);
  }

  void CurveCubic_Impl::setMaximumValueofx(double maximumValueofx) {
    bool result = setDouble(OS_Curve_CubicFields::MaximumValueofx, maximumValueofx);
    OS_ASSERT(result);
  }

  void CurveCubic_Impl::setMinimumCurveOutput(boost::optional<double> minimumCurveOutput) {
    bool result = false;
    if (minimumCurveOutput) {
      result = setDouble(OS_Curve_CubicFields::MinimumCurveOutput, minimumCurveOutput.get());
    } else {
      result = setString(OS_Curve_CubicFields::MinimumCurveOutput, "");
    }
    OS_ASSERT(result);
  }

  void CurveCubic_Impl::resetMinimumCurveOutput() {
    bool result = setString(OS_Curve_CubicFields::MinimumCurveOutput, "");
    OS_ASSERT(result);
  }

  void CurveCubic_Impl::setMaximumCurveOutput(boost::optional<double> maximumCurveOutput) {
    bool result = false;
    if (maximumCurveOutput) {
      result = setDouble(OS_Curve_CubicFields::MaximumCurveOutput, maximumCurveOutput.get());
    } else {
      result = setString(OS_Curve_CubicFields::MaximumCurveOutput, "");
    }
    OS_ASSERT(result);
  }

  void CurveCubic_Impl::resetMaximumCurveOutput() {
    bool result = setString(OS_Curve_CubicFields::MaximumCurveOutput, "");
    OS_ASSERT(result);
  }

  bool CurveCubic_Impl::setInputUnitTypeforX(std::string inputUnitTypeforX) {
    bool result = setString(OS_Curve_CubicFields::InputUnitTypeforX, inputUnitTypeforX);
    return result;
  }

  void CurveCubic_Impl::resetInputUnitTypeforX() {
    bool result = setString(OS_Curve_CubicFields::InputUnitTypeforX, "");
    OS_ASSERT(result);
  }

  bool CurveCubic_Impl::setOutputUnitType(std::string outputUnitType) {
    bool result = setString(OS_Curve_CubicFields::OutputUnitType, outputUnitType);
    return result;
  }

  void CurveCubic_Impl::resetOutputUnitType() {
    bool result = setString(OS_Curve_CubicFields::OutputUnitType, "");
    OS_ASSERT(result);
  }

} // detail

CurveCubic::CurveCubic(const Model& model)
  : Curve(CurveCubic::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CurveCubic_Impl>());
  setDouble(OS_Curve_CubicFields::Coefficient1Constant,0.0);
  setDouble(OS_Curve_CubicFields::Coefficient2x,0.0);
  setDouble(OS_Curve_CubicFields::Coefficient3x_POW_2,0.0);
  setDouble(OS_Curve_CubicFields::Coefficient4x_POW_3,1.0);
  setDouble(OS_Curve_CubicFields::MinimumValueofx,0.0);
  setDouble(OS_Curve_CubicFields::MaximumValueofx,1.0);
}

IddObjectType CurveCubic::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Curve_Cubic);
  return result;
}

std::vector<std::string> CurveCubic::validInputUnitTypeforXValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Curve_CubicFields::InputUnitTypeforX);
}

std::vector<std::string> CurveCubic::validOutputUnitTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Curve_CubicFields::OutputUnitType);
}

double CurveCubic::coefficient1Constant() const {
  return getImpl<detail::CurveCubic_Impl>()->coefficient1Constant();
}

double CurveCubic::coefficient2x() const {
  return getImpl<detail::CurveCubic_Impl>()->coefficient2x();
}

double CurveCubic::coefficient3xPOW2() const {
  return getImpl<detail::CurveCubic_Impl>()->coefficient3xPOW2();
}

double CurveCubic::coefficient4xPOW3() const {
  return getImpl<detail::CurveCubic_Impl>()->coefficient4xPOW3();
}

double CurveCubic::minimumValueofx() const {
  return getImpl<detail::CurveCubic_Impl>()->minimumValueofx();
}

double CurveCubic::maximumValueofx() const {
  return getImpl<detail::CurveCubic_Impl>()->maximumValueofx();
}

boost::optional<double> CurveCubic::minimumCurveOutput() const {
  return getImpl<detail::CurveCubic_Impl>()->minimumCurveOutput();
}

boost::optional<double> CurveCubic::maximumCurveOutput() const {
  return getImpl<detail::CurveCubic_Impl>()->maximumCurveOutput();
}

std::string CurveCubic::inputUnitTypeforX() const {
  return getImpl<detail::CurveCubic_Impl>()->inputUnitTypeforX();
}

bool CurveCubic::isInputUnitTypeforXDefaulted() const {
  return getImpl<detail::CurveCubic_Impl>()->isInputUnitTypeforXDefaulted();
}

std::string CurveCubic::outputUnitType() const {
  return getImpl<detail::CurveCubic_Impl>()->outputUnitType();
}

bool CurveCubic::isOutputUnitTypeDefaulted() const {
  return getImpl<detail::CurveCubic_Impl>()->isOutputUnitTypeDefaulted();
}

void CurveCubic::setCoefficient1Constant(double coefficient1Constant) {
  getImpl<detail::CurveCubic_Impl>()->setCoefficient1Constant(coefficient1Constant);
}

void CurveCubic::setCoefficient2x(double coefficient2x) {
  getImpl<detail::CurveCubic_Impl>()->setCoefficient2x(coefficient2x);
}

void CurveCubic::setCoefficient3xPOW2(double coefficient3xPOW2) {
  getImpl<detail::CurveCubic_Impl>()->setCoefficient3xPOW2(coefficient3xPOW2);
}

void CurveCubic::setCoefficient4xPOW3(double coefficient4xPOW3) {
  getImpl<detail::CurveCubic_Impl>()->setCoefficient4xPOW3(coefficient4xPOW3);
}

void CurveCubic::setMinimumValueofx(double minimumValueofx) {
  getImpl<detail::CurveCubic_Impl>()->setMinimumValueofx(minimumValueofx);
}

void CurveCubic::setMaximumValueofx(double maximumValueofx) {
  getImpl<detail::CurveCubic_Impl>()->setMaximumValueofx(maximumValueofx);
}

void CurveCubic::setMinimumCurveOutput(double minimumCurveOutput) {
  getImpl<detail::CurveCubic_Impl>()->setMinimumCurveOutput(minimumCurveOutput);
}

void CurveCubic::resetMinimumCurveOutput() {
  getImpl<detail::CurveCubic_Impl>()->resetMinimumCurveOutput();
}

void CurveCubic::setMaximumCurveOutput(double maximumCurveOutput) {
  getImpl<detail::CurveCubic_Impl>()->setMaximumCurveOutput(maximumCurveOutput);
}

void CurveCubic::resetMaximumCurveOutput() {
  getImpl<detail::CurveCubic_Impl>()->resetMaximumCurveOutput();
}

bool CurveCubic::setInputUnitTypeforX(std::string inputUnitTypeforX) {
  return getImpl<detail::CurveCubic_Impl>()->setInputUnitTypeforX(inputUnitTypeforX);
}

void CurveCubic::resetInputUnitTypeforX() {
  getImpl<detail::CurveCubic_Impl>()->resetInputUnitTypeforX();
}

bool CurveCubic::setOutputUnitType(std::string outputUnitType) {
  return getImpl<detail::CurveCubic_Impl>()->setOutputUnitType(outputUnitType);
}

void CurveCubic::resetOutputUnitType() {
  getImpl<detail::CurveCubic_Impl>()->resetOutputUnitType();
}

/// @cond
CurveCubic::CurveCubic(std::shared_ptr<detail::CurveCubic_Impl> impl)
  : Curve(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

