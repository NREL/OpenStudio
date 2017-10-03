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

#include "CurveDoubleExponentialDecay.hpp"
#include "CurveDoubleExponentialDecay_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Curve_DoubleExponentialDecay_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

#include <cmath>

using namespace std;

namespace openstudio {
namespace model {

namespace detail {

  CurveDoubleExponentialDecay_Impl::CurveDoubleExponentialDecay_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : Curve_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CurveDoubleExponentialDecay::iddObjectType());
  }

  CurveDoubleExponentialDecay_Impl::CurveDoubleExponentialDecay_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                     Model_Impl* model,
                                                                     bool keepHandle)
    : Curve_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CurveDoubleExponentialDecay::iddObjectType());
  }

  CurveDoubleExponentialDecay_Impl::CurveDoubleExponentialDecay_Impl(const CurveDoubleExponentialDecay_Impl& other,
                                                                     Model_Impl* model,
                                                                     bool keepHandle)
    : Curve_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CurveDoubleExponentialDecay_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CurveDoubleExponentialDecay_Impl::iddObjectType() const {
    return CurveDoubleExponentialDecay::iddObjectType();
  }

  int CurveDoubleExponentialDecay_Impl::numVariables() const {
    return 1;
  }

  double CurveDoubleExponentialDecay_Impl::evaluate(const std::vector<double>& x) const {
    OS_ASSERT(x.size() == 1u);
    LOG_AND_THROW("CurveDoubleExponentialDecay created during transition to EnergyPlus 7.0, when "
        << "documentation for this new curve type was unavailable.");
  }

  double CurveDoubleExponentialDecay_Impl::coefficient1C1() const {
    boost::optional<double> value = getDouble(OS_Curve_DoubleExponentialDecayFields::Coefficient1C1,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveDoubleExponentialDecay_Impl::coefficient2C2() const {
    boost::optional<double> value = getDouble(OS_Curve_DoubleExponentialDecayFields::Coefficient2C2,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveDoubleExponentialDecay_Impl::coefficient3C3() const {
    boost::optional<double> value = getDouble(OS_Curve_DoubleExponentialDecayFields::Coefficient3C3,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveDoubleExponentialDecay_Impl::coefficient3C4() const {
    boost::optional<double> value = getDouble(OS_Curve_DoubleExponentialDecayFields::Coefficient3C4,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveDoubleExponentialDecay_Impl::coefficient3C5() const {
    boost::optional<double> value = getDouble(OS_Curve_DoubleExponentialDecayFields::Coefficient3C5,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveDoubleExponentialDecay_Impl::minimumValueofx() const {
    boost::optional<double> value = getDouble(OS_Curve_DoubleExponentialDecayFields::MinimumValueofx,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveDoubleExponentialDecay_Impl::maximumValueofx() const {
    boost::optional<double> value = getDouble(OS_Curve_DoubleExponentialDecayFields::MaximumValueofx,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> CurveDoubleExponentialDecay_Impl::minimumCurveOutput() const {
    return getDouble(OS_Curve_DoubleExponentialDecayFields::MinimumCurveOutput,true);
  }

  boost::optional<double> CurveDoubleExponentialDecay_Impl::maximumCurveOutput() const {
    return getDouble(OS_Curve_DoubleExponentialDecayFields::MaximumCurveOutput,true);
  }

  std::string CurveDoubleExponentialDecay_Impl::inputUnitTypeforx() const {
    boost::optional<std::string> value = getString(OS_Curve_DoubleExponentialDecayFields::InputUnitTypeforx,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CurveDoubleExponentialDecay_Impl::isInputUnitTypeforxDefaulted() const {
    return isEmpty(OS_Curve_DoubleExponentialDecayFields::InputUnitTypeforx);
  }

  std::string CurveDoubleExponentialDecay_Impl::outputUnitType() const {
    boost::optional<std::string> value = getString(OS_Curve_DoubleExponentialDecayFields::OutputUnitType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CurveDoubleExponentialDecay_Impl::isOutputUnitTypeDefaulted() const {
    return isEmpty(OS_Curve_DoubleExponentialDecayFields::OutputUnitType);
  }

  void CurveDoubleExponentialDecay_Impl::setCoefficient1C1(double coefficient1C1) {
    bool result = setDouble(OS_Curve_DoubleExponentialDecayFields::Coefficient1C1, coefficient1C1);
    OS_ASSERT(result);
  }

  void CurveDoubleExponentialDecay_Impl::setCoefficient2C2(double coefficient2C2) {
    bool result = setDouble(OS_Curve_DoubleExponentialDecayFields::Coefficient2C2, coefficient2C2);
    OS_ASSERT(result);
  }

  void CurveDoubleExponentialDecay_Impl::setCoefficient3C3(double coefficient3C3) {
    bool result = setDouble(OS_Curve_DoubleExponentialDecayFields::Coefficient3C3, coefficient3C3);
    OS_ASSERT(result);
  }

  void CurveDoubleExponentialDecay_Impl::setCoefficient3C4(double coefficient3C4) {
    bool result = setDouble(OS_Curve_DoubleExponentialDecayFields::Coefficient3C4, coefficient3C4);
    OS_ASSERT(result);
  }

  void CurveDoubleExponentialDecay_Impl::setCoefficient3C5(double coefficient3C5) {
    bool result = setDouble(OS_Curve_DoubleExponentialDecayFields::Coefficient3C5, coefficient3C5);
    OS_ASSERT(result);
  }

  void CurveDoubleExponentialDecay_Impl::setMinimumValueofx(double minimumValueofx) {
    bool result = setDouble(OS_Curve_DoubleExponentialDecayFields::MinimumValueofx, minimumValueofx);
    OS_ASSERT(result);
  }

  void CurveDoubleExponentialDecay_Impl::setMaximumValueofx(double maximumValueofx) {
    bool result = setDouble(OS_Curve_DoubleExponentialDecayFields::MaximumValueofx, maximumValueofx);
    OS_ASSERT(result);
  }

  void CurveDoubleExponentialDecay_Impl::setMinimumCurveOutput(boost::optional<double> minimumCurveOutput) {
    bool result = false;
    if (minimumCurveOutput) {
      result = setDouble(OS_Curve_DoubleExponentialDecayFields::MinimumCurveOutput, minimumCurveOutput.get());
    } else {
      result = setString(OS_Curve_DoubleExponentialDecayFields::MinimumCurveOutput, "");
    }
    OS_ASSERT(result);
  }

  void CurveDoubleExponentialDecay_Impl::resetMinimumCurveOutput() {
    bool result = setString(OS_Curve_DoubleExponentialDecayFields::MinimumCurveOutput, "");
    OS_ASSERT(result);
  }

  void CurveDoubleExponentialDecay_Impl::setMaximumCurveOutput(boost::optional<double> maximumCurveOutput) {
    bool result = false;
    if (maximumCurveOutput) {
      result = setDouble(OS_Curve_DoubleExponentialDecayFields::MaximumCurveOutput, maximumCurveOutput.get());
    } else {
      result = setString(OS_Curve_DoubleExponentialDecayFields::MaximumCurveOutput, "");
    }
    OS_ASSERT(result);
  }

  void CurveDoubleExponentialDecay_Impl::resetMaximumCurveOutput() {
    bool result = setString(OS_Curve_DoubleExponentialDecayFields::MaximumCurveOutput, "");
    OS_ASSERT(result);
  }

  bool CurveDoubleExponentialDecay_Impl::setInputUnitTypeforx(std::string inputUnitTypeforx) {
    bool result = setString(OS_Curve_DoubleExponentialDecayFields::InputUnitTypeforx, inputUnitTypeforx);
    return result;
  }

  void CurveDoubleExponentialDecay_Impl::resetInputUnitTypeforx() {
    bool result = setString(OS_Curve_DoubleExponentialDecayFields::InputUnitTypeforx, "");
    OS_ASSERT(result);
  }

  bool CurveDoubleExponentialDecay_Impl::setOutputUnitType(std::string outputUnitType) {
    bool result = setString(OS_Curve_DoubleExponentialDecayFields::OutputUnitType, outputUnitType);
    return result;
  }

  void CurveDoubleExponentialDecay_Impl::resetOutputUnitType() {
    bool result = setString(OS_Curve_DoubleExponentialDecayFields::OutputUnitType, "");
    OS_ASSERT(result);
  }

} // detail

CurveDoubleExponentialDecay::CurveDoubleExponentialDecay(const Model& model)
  : Curve(CurveDoubleExponentialDecay::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CurveDoubleExponentialDecay_Impl>());
  setDouble(OS_Curve_DoubleExponentialDecayFields::Coefficient1C1,0.0);
  setDouble(OS_Curve_DoubleExponentialDecayFields::Coefficient2C2,0.0);
  setDouble(OS_Curve_DoubleExponentialDecayFields::Coefficient3C3,0.0);
  setDouble(OS_Curve_DoubleExponentialDecayFields::Coefficient3C4,0.0);
  setDouble(OS_Curve_DoubleExponentialDecayFields::Coefficient3C5,0.0);
  setDouble(OS_Curve_DoubleExponentialDecayFields::MinimumValueofx,0.0);
  setDouble(OS_Curve_DoubleExponentialDecayFields::MaximumValueofx,1.0);
}

IddObjectType CurveDoubleExponentialDecay::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Curve_DoubleExponentialDecay);
  return result;
}

std::vector<std::string> CurveDoubleExponentialDecay::validInputUnitTypeforxValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Curve_DoubleExponentialDecayFields::InputUnitTypeforx);
}

std::vector<std::string> CurveDoubleExponentialDecay::validOutputUnitTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Curve_DoubleExponentialDecayFields::OutputUnitType);
}

double CurveDoubleExponentialDecay::coefficient1C1() const {
  return getImpl<detail::CurveDoubleExponentialDecay_Impl>()->coefficient1C1();
}

double CurveDoubleExponentialDecay::coefficient2C2() const {
  return getImpl<detail::CurveDoubleExponentialDecay_Impl>()->coefficient2C2();
}

double CurveDoubleExponentialDecay::coefficient3C3() const {
  return getImpl<detail::CurveDoubleExponentialDecay_Impl>()->coefficient3C3();
}

double CurveDoubleExponentialDecay::coefficient3C4() const {
  return getImpl<detail::CurveDoubleExponentialDecay_Impl>()->coefficient3C4();
}

double CurveDoubleExponentialDecay::coefficient3C5() const {
  return getImpl<detail::CurveDoubleExponentialDecay_Impl>()->coefficient3C5();
}

double CurveDoubleExponentialDecay::minimumValueofx() const {
  return getImpl<detail::CurveDoubleExponentialDecay_Impl>()->minimumValueofx();
}

double CurveDoubleExponentialDecay::maximumValueofx() const {
  return getImpl<detail::CurveDoubleExponentialDecay_Impl>()->maximumValueofx();
}

boost::optional<double> CurveDoubleExponentialDecay::minimumCurveOutput() const {
  return getImpl<detail::CurveDoubleExponentialDecay_Impl>()->minimumCurveOutput();
}

boost::optional<double> CurveDoubleExponentialDecay::maximumCurveOutput() const {
  return getImpl<detail::CurveDoubleExponentialDecay_Impl>()->maximumCurveOutput();
}

std::string CurveDoubleExponentialDecay::inputUnitTypeforx() const {
  return getImpl<detail::CurveDoubleExponentialDecay_Impl>()->inputUnitTypeforx();
}

bool CurveDoubleExponentialDecay::isInputUnitTypeforxDefaulted() const {
  return getImpl<detail::CurveDoubleExponentialDecay_Impl>()->isInputUnitTypeforxDefaulted();
}

std::string CurveDoubleExponentialDecay::outputUnitType() const {
  return getImpl<detail::CurveDoubleExponentialDecay_Impl>()->outputUnitType();
}

bool CurveDoubleExponentialDecay::isOutputUnitTypeDefaulted() const {
  return getImpl<detail::CurveDoubleExponentialDecay_Impl>()->isOutputUnitTypeDefaulted();
}

void CurveDoubleExponentialDecay::setCoefficient1C1(double coefficient1C1) {
  getImpl<detail::CurveDoubleExponentialDecay_Impl>()->setCoefficient1C1(coefficient1C1);
}

void CurveDoubleExponentialDecay::setCoefficient2C2(double coefficient2C2) {
  getImpl<detail::CurveDoubleExponentialDecay_Impl>()->setCoefficient2C2(coefficient2C2);
}

void CurveDoubleExponentialDecay::setCoefficient3C3(double coefficient3C3) {
  getImpl<detail::CurveDoubleExponentialDecay_Impl>()->setCoefficient3C3(coefficient3C3);
}

void CurveDoubleExponentialDecay::setCoefficient3C4(double coefficient3C4) {
  getImpl<detail::CurveDoubleExponentialDecay_Impl>()->setCoefficient3C4(coefficient3C4);
}

void CurveDoubleExponentialDecay::setCoefficient3C5(double coefficient3C5) {
  getImpl<detail::CurveDoubleExponentialDecay_Impl>()->setCoefficient3C5(coefficient3C5);
}

void CurveDoubleExponentialDecay::setMinimumValueofx(double minimumValueofx) {
  getImpl<detail::CurveDoubleExponentialDecay_Impl>()->setMinimumValueofx(minimumValueofx);
}

void CurveDoubleExponentialDecay::setMaximumValueofx(double maximumValueofx) {
  getImpl<detail::CurveDoubleExponentialDecay_Impl>()->setMaximumValueofx(maximumValueofx);
}

void CurveDoubleExponentialDecay::setMinimumCurveOutput(double minimumCurveOutput) {
  getImpl<detail::CurveDoubleExponentialDecay_Impl>()->setMinimumCurveOutput(minimumCurveOutput);
}

void CurveDoubleExponentialDecay::resetMinimumCurveOutput() {
  getImpl<detail::CurveDoubleExponentialDecay_Impl>()->resetMinimumCurveOutput();
}

void CurveDoubleExponentialDecay::setMaximumCurveOutput(double maximumCurveOutput) {
  getImpl<detail::CurveDoubleExponentialDecay_Impl>()->setMaximumCurveOutput(maximumCurveOutput);
}

void CurveDoubleExponentialDecay::resetMaximumCurveOutput() {
  getImpl<detail::CurveDoubleExponentialDecay_Impl>()->resetMaximumCurveOutput();
}

bool CurveDoubleExponentialDecay::setInputUnitTypeforx(std::string inputUnitTypeforx) {
  return getImpl<detail::CurveDoubleExponentialDecay_Impl>()->setInputUnitTypeforx(inputUnitTypeforx);
}

void CurveDoubleExponentialDecay::resetInputUnitTypeforx() {
  getImpl<detail::CurveDoubleExponentialDecay_Impl>()->resetInputUnitTypeforx();
}

bool CurveDoubleExponentialDecay::setOutputUnitType(std::string outputUnitType) {
  return getImpl<detail::CurveDoubleExponentialDecay_Impl>()->setOutputUnitType(outputUnitType);
}

void CurveDoubleExponentialDecay::resetOutputUnitType() {
  getImpl<detail::CurveDoubleExponentialDecay_Impl>()->resetOutputUnitType();
}

/// @cond
CurveDoubleExponentialDecay::CurveDoubleExponentialDecay(std::shared_ptr<detail::CurveDoubleExponentialDecay_Impl> impl)
  : Curve(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

