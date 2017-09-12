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

#include "CurveQuartic.hpp"
#include "CurveQuartic_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Curve_Quartic_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

#include <cmath>

using namespace std;

namespace openstudio {
namespace model {

namespace detail {

  CurveQuartic_Impl::CurveQuartic_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : Curve_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CurveQuartic::iddObjectType());
  }

  CurveQuartic_Impl::CurveQuartic_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                       Model_Impl* model,
                                       bool keepHandle)
    : Curve_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CurveQuartic::iddObjectType());
  }

  CurveQuartic_Impl::CurveQuartic_Impl(const CurveQuartic_Impl& other,
                                       Model_Impl* model,
                                       bool keepHandle)
    : Curve_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CurveQuartic_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CurveQuartic_Impl::iddObjectType() const {
    return CurveQuartic::iddObjectType();
  }

  int CurveQuartic_Impl::numVariables() const {
    return 1;
  }

  double CurveQuartic_Impl::evaluate(const std::vector<double>& x) const {
    OS_ASSERT(x.size() == 1u);
    double result = coefficient1Constant();
    result += coefficient2x() * x[0];
    result += coefficient3xPOW2() * pow(x[0],2);
    result += coefficient4xPOW3() * pow(x[0],3);
    result += coefficient5xPOW4() * pow(x[0],4);
    return result;
  }

  double CurveQuartic_Impl::coefficient1Constant() const {
    boost::optional<double> value = getDouble(OS_Curve_QuarticFields::Coefficient1Constant,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveQuartic_Impl::coefficient2x() const {
    boost::optional<double> value = getDouble(OS_Curve_QuarticFields::Coefficient2x,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveQuartic_Impl::coefficient3xPOW2() const {
    boost::optional<double> value = getDouble(OS_Curve_QuarticFields::Coefficient3x_POW_2,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveQuartic_Impl::coefficient4xPOW3() const {
    boost::optional<double> value = getDouble(OS_Curve_QuarticFields::Coefficient4x_POW_3,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveQuartic_Impl::coefficient5xPOW4() const {
    boost::optional<double> value = getDouble(OS_Curve_QuarticFields::Coefficient5x_POW_4,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveQuartic_Impl::minimumValueofx() const {
    boost::optional<double> value = getDouble(OS_Curve_QuarticFields::MinimumValueofx,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveQuartic_Impl::maximumValueofx() const {
    boost::optional<double> value = getDouble(OS_Curve_QuarticFields::MaximumValueofx,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> CurveQuartic_Impl::minimumCurveOutput() const {
    return getDouble(OS_Curve_QuarticFields::MinimumCurveOutput,true);
  }

  boost::optional<double> CurveQuartic_Impl::maximumCurveOutput() const {
    return getDouble(OS_Curve_QuarticFields::MaximumCurveOutput,true);
  }

  std::string CurveQuartic_Impl::inputUnitTypeforX() const {
    boost::optional<std::string> value = getString(OS_Curve_QuarticFields::InputUnitTypeforX,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CurveQuartic_Impl::isInputUnitTypeforXDefaulted() const {
    return isEmpty(OS_Curve_QuarticFields::InputUnitTypeforX);
  }

  std::string CurveQuartic_Impl::outputUnitType() const {
    boost::optional<std::string> value = getString(OS_Curve_QuarticFields::OutputUnitType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CurveQuartic_Impl::isOutputUnitTypeDefaulted() const {
    return isEmpty(OS_Curve_QuarticFields::OutputUnitType);
  }

  void CurveQuartic_Impl::setCoefficient1Constant(double coefficient1Constant) {
    bool result = setDouble(OS_Curve_QuarticFields::Coefficient1Constant, coefficient1Constant);
    OS_ASSERT(result);
  }

  void CurveQuartic_Impl::setCoefficient2x(double coefficient2x) {
    bool result = setDouble(OS_Curve_QuarticFields::Coefficient2x, coefficient2x);
    OS_ASSERT(result);
  }

  void CurveQuartic_Impl::setCoefficient3xPOW2(double coefficient3xPOW2) {
    bool result = setDouble(OS_Curve_QuarticFields::Coefficient3x_POW_2, coefficient3xPOW2);
    OS_ASSERT(result);
  }

  void CurveQuartic_Impl::setCoefficient4xPOW3(double coefficient4xPOW3) {
    bool result = setDouble(OS_Curve_QuarticFields::Coefficient4x_POW_3, coefficient4xPOW3);
    OS_ASSERT(result);
  }

  void CurveQuartic_Impl::setCoefficient5xPOW4(double coefficient5xPOW4) {
    bool result = setDouble(OS_Curve_QuarticFields::Coefficient5x_POW_4, coefficient5xPOW4);
    OS_ASSERT(result);
  }

  void CurveQuartic_Impl::setMinimumValueofx(double minimumValueofx) {
    bool result = setDouble(OS_Curve_QuarticFields::MinimumValueofx, minimumValueofx);
    OS_ASSERT(result);
  }

  void CurveQuartic_Impl::setMaximumValueofx(double maximumValueofx) {
    bool result = setDouble(OS_Curve_QuarticFields::MaximumValueofx, maximumValueofx);
    OS_ASSERT(result);
  }

  void CurveQuartic_Impl::setMinimumCurveOutput(boost::optional<double> minimumCurveOutput) {
    bool result = false;
    if (minimumCurveOutput) {
      result = setDouble(OS_Curve_QuarticFields::MinimumCurveOutput, minimumCurveOutput.get());
    } else {
      result = setString(OS_Curve_QuarticFields::MinimumCurveOutput, "");
    }
    OS_ASSERT(result);
  }

  void CurveQuartic_Impl::resetMinimumCurveOutput() {
    bool result = setString(OS_Curve_QuarticFields::MinimumCurveOutput, "");
    OS_ASSERT(result);
  }

  void CurveQuartic_Impl::setMaximumCurveOutput(boost::optional<double> maximumCurveOutput) {
    bool result = false;
    if (maximumCurveOutput) {
      result = setDouble(OS_Curve_QuarticFields::MaximumCurveOutput, maximumCurveOutput.get());
    } else {
      result = setString(OS_Curve_QuarticFields::MaximumCurveOutput, "");
    }
    OS_ASSERT(result);
  }

  void CurveQuartic_Impl::resetMaximumCurveOutput() {
    bool result = setString(OS_Curve_QuarticFields::MaximumCurveOutput, "");
    OS_ASSERT(result);
  }

  bool CurveQuartic_Impl::setInputUnitTypeforX(std::string inputUnitTypeforX) {
    bool result = setString(OS_Curve_QuarticFields::InputUnitTypeforX, inputUnitTypeforX);
    return result;
  }

  void CurveQuartic_Impl::resetInputUnitTypeforX() {
    bool result = setString(OS_Curve_QuarticFields::InputUnitTypeforX, "");
    OS_ASSERT(result);
  }

  bool CurveQuartic_Impl::setOutputUnitType(std::string outputUnitType) {
    bool result = setString(OS_Curve_QuarticFields::OutputUnitType, outputUnitType);
    return result;
  }

  void CurveQuartic_Impl::resetOutputUnitType() {
    bool result = setString(OS_Curve_QuarticFields::OutputUnitType, "");
    OS_ASSERT(result);
  }

} // detail

CurveQuartic::CurveQuartic(const Model& model)
  : Curve(CurveQuartic::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CurveQuartic_Impl>());
  setDouble(OS_Curve_QuarticFields::Coefficient1Constant,0.0);
  setDouble(OS_Curve_QuarticFields::Coefficient2x,0.0);
  setDouble(OS_Curve_QuarticFields::Coefficient3x_POW_2,0.0);
  setDouble(OS_Curve_QuarticFields::Coefficient4x_POW_3,0.0);
  setDouble(OS_Curve_QuarticFields::Coefficient5x_POW_4,1.0);
  setDouble(OS_Curve_QuarticFields::MinimumValueofx,0.0);
  setDouble(OS_Curve_QuarticFields::MaximumValueofx,1.0);
}

IddObjectType CurveQuartic::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Curve_Quartic);
  return result;
}

std::vector<std::string> CurveQuartic::validInputUnitTypeforXValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Curve_QuarticFields::InputUnitTypeforX);
}

std::vector<std::string> CurveQuartic::validOutputUnitTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Curve_QuarticFields::OutputUnitType);
}

double CurveQuartic::coefficient1Constant() const {
  return getImpl<detail::CurveQuartic_Impl>()->coefficient1Constant();
}

double CurveQuartic::coefficient2x() const {
  return getImpl<detail::CurveQuartic_Impl>()->coefficient2x();
}

double CurveQuartic::coefficient3xPOW2() const {
  return getImpl<detail::CurveQuartic_Impl>()->coefficient3xPOW2();
}

double CurveQuartic::coefficient4xPOW3() const {
  return getImpl<detail::CurveQuartic_Impl>()->coefficient4xPOW3();
}

double CurveQuartic::coefficient5xPOW4() const {
  return getImpl<detail::CurveQuartic_Impl>()->coefficient5xPOW4();
}

double CurveQuartic::minimumValueofx() const {
  return getImpl<detail::CurveQuartic_Impl>()->minimumValueofx();
}

double CurveQuartic::maximumValueofx() const {
  return getImpl<detail::CurveQuartic_Impl>()->maximumValueofx();
}

boost::optional<double> CurveQuartic::minimumCurveOutput() const {
  return getImpl<detail::CurveQuartic_Impl>()->minimumCurveOutput();
}

boost::optional<double> CurveQuartic::maximumCurveOutput() const {
  return getImpl<detail::CurveQuartic_Impl>()->maximumCurveOutput();
}

std::string CurveQuartic::inputUnitTypeforX() const {
  return getImpl<detail::CurveQuartic_Impl>()->inputUnitTypeforX();
}

bool CurveQuartic::isInputUnitTypeforXDefaulted() const {
  return getImpl<detail::CurveQuartic_Impl>()->isInputUnitTypeforXDefaulted();
}

std::string CurveQuartic::outputUnitType() const {
  return getImpl<detail::CurveQuartic_Impl>()->outputUnitType();
}

bool CurveQuartic::isOutputUnitTypeDefaulted() const {
  return getImpl<detail::CurveQuartic_Impl>()->isOutputUnitTypeDefaulted();
}

void CurveQuartic::setCoefficient1Constant(double coefficient1Constant) {
  getImpl<detail::CurveQuartic_Impl>()->setCoefficient1Constant(coefficient1Constant);
}

void CurveQuartic::setCoefficient2x(double coefficient2x) {
  getImpl<detail::CurveQuartic_Impl>()->setCoefficient2x(coefficient2x);
}

void CurveQuartic::setCoefficient3xPOW2(double coefficient3xPOW2) {
  getImpl<detail::CurveQuartic_Impl>()->setCoefficient3xPOW2(coefficient3xPOW2);
}

void CurveQuartic::setCoefficient4xPOW3(double coefficient4xPOW3) {
  getImpl<detail::CurveQuartic_Impl>()->setCoefficient4xPOW3(coefficient4xPOW3);
}

void CurveQuartic::setCoefficient5xPOW4(double coefficient5xPOW4) {
  getImpl<detail::CurveQuartic_Impl>()->setCoefficient5xPOW4(coefficient5xPOW4);
}

void CurveQuartic::setMinimumValueofx(double minimumValueofx) {
  getImpl<detail::CurveQuartic_Impl>()->setMinimumValueofx(minimumValueofx);
}

void CurveQuartic::setMaximumValueofx(double maximumValueofx) {
  getImpl<detail::CurveQuartic_Impl>()->setMaximumValueofx(maximumValueofx);
}

void CurveQuartic::setMinimumCurveOutput(double minimumCurveOutput) {
  getImpl<detail::CurveQuartic_Impl>()->setMinimumCurveOutput(minimumCurveOutput);
}

void CurveQuartic::resetMinimumCurveOutput() {
  getImpl<detail::CurveQuartic_Impl>()->resetMinimumCurveOutput();
}

void CurveQuartic::setMaximumCurveOutput(double maximumCurveOutput) {
  getImpl<detail::CurveQuartic_Impl>()->setMaximumCurveOutput(maximumCurveOutput);
}

void CurveQuartic::resetMaximumCurveOutput() {
  getImpl<detail::CurveQuartic_Impl>()->resetMaximumCurveOutput();
}

bool CurveQuartic::setInputUnitTypeforX(std::string inputUnitTypeforX) {
  return getImpl<detail::CurveQuartic_Impl>()->setInputUnitTypeforX(inputUnitTypeforX);
}

void CurveQuartic::resetInputUnitTypeforX() {
  getImpl<detail::CurveQuartic_Impl>()->resetInputUnitTypeforX();
}

bool CurveQuartic::setOutputUnitType(std::string outputUnitType) {
  return getImpl<detail::CurveQuartic_Impl>()->setOutputUnitType(outputUnitType);
}

void CurveQuartic::resetOutputUnitType() {
  getImpl<detail::CurveQuartic_Impl>()->resetOutputUnitType();
}

/// @cond
CurveQuartic::CurveQuartic(std::shared_ptr<detail::CurveQuartic_Impl> impl)
  : Curve(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

