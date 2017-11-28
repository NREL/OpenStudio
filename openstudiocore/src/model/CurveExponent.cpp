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

#include "CurveExponent.hpp"
#include "CurveExponent_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Curve_Exponent_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

#include <cmath>

using namespace std;

namespace openstudio {
namespace model {

namespace detail {

  CurveExponent_Impl::CurveExponent_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : Curve_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CurveExponent::iddObjectType());
  }

  CurveExponent_Impl::CurveExponent_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                         Model_Impl* model,
                                         bool keepHandle)
    : Curve_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CurveExponent::iddObjectType());
  }

  CurveExponent_Impl::CurveExponent_Impl(const CurveExponent_Impl& other,
                                         Model_Impl* model,
                                         bool keepHandle)
    : Curve_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CurveExponent_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CurveExponent_Impl::iddObjectType() const {
    return CurveExponent::iddObjectType();
  }

  int CurveExponent_Impl::numVariables() const {
    return 1;
  }

  double CurveExponent_Impl::evaluate(const std::vector<double>& x) const {
    OS_ASSERT(x.size() == 1u);
    double result = coefficient1Constant();
    result += coefficient2Constant() * pow(x[0],coefficient3Constant());
    return result;
  }

  double CurveExponent_Impl::coefficient1Constant() const {
    boost::optional<double> value = getDouble(OS_Curve_ExponentFields::Coefficient1Constant,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveExponent_Impl::coefficient2Constant() const {
    boost::optional<double> value = getDouble(OS_Curve_ExponentFields::Coefficient2Constant,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveExponent_Impl::coefficient3Constant() const {
    boost::optional<double> value = getDouble(OS_Curve_ExponentFields::Coefficient3Constant,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveExponent_Impl::minimumValueofx() const {
    boost::optional<double> value = getDouble(OS_Curve_ExponentFields::MinimumValueofx,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveExponent_Impl::maximumValueofx() const {
    boost::optional<double> value = getDouble(OS_Curve_ExponentFields::MaximumValueofx,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> CurveExponent_Impl::minimumCurveOutput() const {
    return getDouble(OS_Curve_ExponentFields::MinimumCurveOutput,true);
  }

  boost::optional<double> CurveExponent_Impl::maximumCurveOutput() const {
    return getDouble(OS_Curve_ExponentFields::MaximumCurveOutput,true);
  }

  std::string CurveExponent_Impl::inputUnitTypeforX() const {
    boost::optional<std::string> value = getString(OS_Curve_ExponentFields::InputUnitTypeforX,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CurveExponent_Impl::isInputUnitTypeforXDefaulted() const {
    return isEmpty(OS_Curve_ExponentFields::InputUnitTypeforX);
  }

  std::string CurveExponent_Impl::outputUnitType() const {
    boost::optional<std::string> value = getString(OS_Curve_ExponentFields::OutputUnitType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CurveExponent_Impl::isOutputUnitTypeDefaulted() const {
    return isEmpty(OS_Curve_ExponentFields::OutputUnitType);
  }

  void CurveExponent_Impl::setCoefficient1Constant(double coefficient1Constant) {
    bool result = setDouble(OS_Curve_ExponentFields::Coefficient1Constant, coefficient1Constant);
    OS_ASSERT(result);
  }

  void CurveExponent_Impl::setCoefficient2Constant(double coefficient2Constant) {
    bool result = setDouble(OS_Curve_ExponentFields::Coefficient2Constant, coefficient2Constant);
    OS_ASSERT(result);
  }

  void CurveExponent_Impl::setCoefficient3Constant(double coefficient3Constant) {
    bool result = setDouble(OS_Curve_ExponentFields::Coefficient3Constant, coefficient3Constant);
    OS_ASSERT(result);
  }

  void CurveExponent_Impl::setMinimumValueofx(double minimumValueofx) {
    bool result = setDouble(OS_Curve_ExponentFields::MinimumValueofx, minimumValueofx);
    OS_ASSERT(result);
  }

  void CurveExponent_Impl::setMaximumValueofx(double maximumValueofx) {
    bool result = setDouble(OS_Curve_ExponentFields::MaximumValueofx, maximumValueofx);
    OS_ASSERT(result);
  }

  void CurveExponent_Impl::setMinimumCurveOutput(boost::optional<double> minimumCurveOutput) {
    bool result = false;
    if (minimumCurveOutput) {
      result = setDouble(OS_Curve_ExponentFields::MinimumCurveOutput, minimumCurveOutput.get());
    } else {
      result = setString(OS_Curve_ExponentFields::MinimumCurveOutput, "");
    }
    OS_ASSERT(result);
  }

  void CurveExponent_Impl::resetMinimumCurveOutput() {
    bool result = setString(OS_Curve_ExponentFields::MinimumCurveOutput, "");
    OS_ASSERT(result);
  }

  void CurveExponent_Impl::setMaximumCurveOutput(boost::optional<double> maximumCurveOutput) {
    bool result = false;
    if (maximumCurveOutput) {
      result = setDouble(OS_Curve_ExponentFields::MaximumCurveOutput, maximumCurveOutput.get());
    } else {
      result = setString(OS_Curve_ExponentFields::MaximumCurveOutput, "");
    }
    OS_ASSERT(result);
  }

  void CurveExponent_Impl::resetMaximumCurveOutput() {
    bool result = setString(OS_Curve_ExponentFields::MaximumCurveOutput, "");
    OS_ASSERT(result);
  }

  bool CurveExponent_Impl::setInputUnitTypeforX(std::string inputUnitTypeforX) {
    bool result = setString(OS_Curve_ExponentFields::InputUnitTypeforX, inputUnitTypeforX);
    return result;
  }

  void CurveExponent_Impl::resetInputUnitTypeforX() {
    bool result = setString(OS_Curve_ExponentFields::InputUnitTypeforX, "");
    OS_ASSERT(result);
  }

  bool CurveExponent_Impl::setOutputUnitType(std::string outputUnitType) {
    bool result = setString(OS_Curve_ExponentFields::OutputUnitType, outputUnitType);
    return result;
  }

  void CurveExponent_Impl::resetOutputUnitType() {
    bool result = setString(OS_Curve_ExponentFields::OutputUnitType, "");
    OS_ASSERT(result);
  }

} // detail

CurveExponent::CurveExponent(const Model& model)
  : Curve(CurveExponent::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CurveExponent_Impl>());
  setDouble(OS_Curve_ExponentFields::Coefficient1Constant,0.0);
  setDouble(OS_Curve_ExponentFields::Coefficient2Constant,1.0);
  setDouble(OS_Curve_ExponentFields::Coefficient3Constant,1.0);
  setDouble(OS_Curve_ExponentFields::MinimumValueofx,0.0);
  setDouble(OS_Curve_ExponentFields::MaximumValueofx,1.0);
  setString(OS_Curve_ExponentFields::MinimumCurveOutput,"");
  setString(OS_Curve_ExponentFields::MaximumCurveOutput,"");
  setString(OS_Curve_ExponentFields::InputUnitTypeforX,"");
  setString(OS_Curve_ExponentFields::OutputUnitType,"");
}

IddObjectType CurveExponent::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Curve_Exponent);
  return result;
}

std::vector<std::string> CurveExponent::validInputUnitTypeforXValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Curve_ExponentFields::InputUnitTypeforX);
}

std::vector<std::string> CurveExponent::validOutputUnitTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Curve_ExponentFields::OutputUnitType);
}

double CurveExponent::coefficient1Constant() const {
  return getImpl<detail::CurveExponent_Impl>()->coefficient1Constant();
}

double CurveExponent::coefficient2Constant() const {
  return getImpl<detail::CurveExponent_Impl>()->coefficient2Constant();
}

double CurveExponent::coefficient3Constant() const {
  return getImpl<detail::CurveExponent_Impl>()->coefficient3Constant();
}

double CurveExponent::minimumValueofx() const {
  return getImpl<detail::CurveExponent_Impl>()->minimumValueofx();
}

double CurveExponent::maximumValueofx() const {
  return getImpl<detail::CurveExponent_Impl>()->maximumValueofx();
}

boost::optional<double> CurveExponent::minimumCurveOutput() const {
  return getImpl<detail::CurveExponent_Impl>()->minimumCurveOutput();
}

boost::optional<double> CurveExponent::maximumCurveOutput() const {
  return getImpl<detail::CurveExponent_Impl>()->maximumCurveOutput();
}

std::string CurveExponent::inputUnitTypeforX() const {
  return getImpl<detail::CurveExponent_Impl>()->inputUnitTypeforX();
}

bool CurveExponent::isInputUnitTypeforXDefaulted() const {
  return getImpl<detail::CurveExponent_Impl>()->isInputUnitTypeforXDefaulted();
}

std::string CurveExponent::outputUnitType() const {
  return getImpl<detail::CurveExponent_Impl>()->outputUnitType();
}

bool CurveExponent::isOutputUnitTypeDefaulted() const {
  return getImpl<detail::CurveExponent_Impl>()->isOutputUnitTypeDefaulted();
}

void CurveExponent::setCoefficient1Constant(double coefficient1Constant) {
  getImpl<detail::CurveExponent_Impl>()->setCoefficient1Constant(coefficient1Constant);
}

void CurveExponent::setCoefficient2Constant(double coefficient2Constant) {
  getImpl<detail::CurveExponent_Impl>()->setCoefficient2Constant(coefficient2Constant);
}

void CurveExponent::setCoefficient3Constant(double coefficient3Constant) {
  getImpl<detail::CurveExponent_Impl>()->setCoefficient3Constant(coefficient3Constant);
}

void CurveExponent::setMinimumValueofx(double minimumValueofx) {
  getImpl<detail::CurveExponent_Impl>()->setMinimumValueofx(minimumValueofx);
}

void CurveExponent::setMaximumValueofx(double maximumValueofx) {
  getImpl<detail::CurveExponent_Impl>()->setMaximumValueofx(maximumValueofx);
}

void CurveExponent::setMinimumCurveOutput(double minimumCurveOutput) {
  getImpl<detail::CurveExponent_Impl>()->setMinimumCurveOutput(minimumCurveOutput);
}

void CurveExponent::resetMinimumCurveOutput() {
  getImpl<detail::CurveExponent_Impl>()->resetMinimumCurveOutput();
}

void CurveExponent::setMaximumCurveOutput(double maximumCurveOutput) {
  getImpl<detail::CurveExponent_Impl>()->setMaximumCurveOutput(maximumCurveOutput);
}

void CurveExponent::resetMaximumCurveOutput() {
  getImpl<detail::CurveExponent_Impl>()->resetMaximumCurveOutput();
}

bool CurveExponent::setInputUnitTypeforX(std::string inputUnitTypeforX) {
  return getImpl<detail::CurveExponent_Impl>()->setInputUnitTypeforX(inputUnitTypeforX);
}

void CurveExponent::resetInputUnitTypeforX() {
  getImpl<detail::CurveExponent_Impl>()->resetInputUnitTypeforX();
}

bool CurveExponent::setOutputUnitType(std::string outputUnitType) {
  return getImpl<detail::CurveExponent_Impl>()->setOutputUnitType(outputUnitType);
}

void CurveExponent::resetOutputUnitType() {
  getImpl<detail::CurveExponent_Impl>()->resetOutputUnitType();
}

/// @cond
CurveExponent::CurveExponent(std::shared_ptr<detail::CurveExponent_Impl> impl)
  : Curve(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

