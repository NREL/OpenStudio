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

#include "CurveRectangularHyperbola1.hpp"
#include "CurveRectangularHyperbola1_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Curve_RectangularHyperbola1_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

#include <cmath>

using namespace std;

namespace openstudio {
namespace model {

namespace detail {

  CurveRectangularHyperbola1_Impl::CurveRectangularHyperbola1_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : Curve_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CurveRectangularHyperbola1::iddObjectType());
  }

  CurveRectangularHyperbola1_Impl::CurveRectangularHyperbola1_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                   Model_Impl* model,
                                                                   bool keepHandle)
    : Curve_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CurveRectangularHyperbola1::iddObjectType());
  }

  CurveRectangularHyperbola1_Impl::CurveRectangularHyperbola1_Impl(const CurveRectangularHyperbola1_Impl& other,
                                                                   Model_Impl* model,
                                                                   bool keepHandle)
    : Curve_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CurveRectangularHyperbola1_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CurveRectangularHyperbola1_Impl::iddObjectType() const {
    return CurveRectangularHyperbola1::iddObjectType();
  }

  int CurveRectangularHyperbola1_Impl::numVariables() const {
    return 1;
  }

  double CurveRectangularHyperbola1_Impl::evaluate(const std::vector<double>& x) const {
    OS_ASSERT(x.size() == 1u);
    double result = coefficient1C1() * x[0];
    double temp = coefficient2C2() + x[0];
    result /= temp;
    result += coefficient3C3();
    return result;
  }

  double CurveRectangularHyperbola1_Impl::coefficient1C1() const {
    boost::optional<double> value = getDouble(OS_Curve_RectangularHyperbola1Fields::Coefficient1C1,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveRectangularHyperbola1_Impl::coefficient2C2() const {
    boost::optional<double> value = getDouble(OS_Curve_RectangularHyperbola1Fields::Coefficient2C2,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveRectangularHyperbola1_Impl::coefficient3C3() const {
    boost::optional<double> value = getDouble(OS_Curve_RectangularHyperbola1Fields::Coefficient3C3,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveRectangularHyperbola1_Impl::minimumValueofx() const {
    boost::optional<double> value = getDouble(OS_Curve_RectangularHyperbola1Fields::MinimumValueofx,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveRectangularHyperbola1_Impl::maximumValueofx() const {
    boost::optional<double> value = getDouble(OS_Curve_RectangularHyperbola1Fields::MaximumValueofx,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> CurveRectangularHyperbola1_Impl::minimumCurveOutput() const {
    return getDouble(OS_Curve_RectangularHyperbola1Fields::MinimumCurveOutput,true);
  }

  boost::optional<double> CurveRectangularHyperbola1_Impl::maximumCurveOutput() const {
    return getDouble(OS_Curve_RectangularHyperbola1Fields::MaximumCurveOutput,true);
  }

  std::string CurveRectangularHyperbola1_Impl::inputUnitTypeforx() const {
    boost::optional<std::string> value = getString(OS_Curve_RectangularHyperbola1Fields::InputUnitTypeforx,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CurveRectangularHyperbola1_Impl::isInputUnitTypeforxDefaulted() const {
    return isEmpty(OS_Curve_RectangularHyperbola1Fields::InputUnitTypeforx);
  }

  std::string CurveRectangularHyperbola1_Impl::outputUnitType() const {
    boost::optional<std::string> value = getString(OS_Curve_RectangularHyperbola1Fields::OutputUnitType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CurveRectangularHyperbola1_Impl::isOutputUnitTypeDefaulted() const {
    return isEmpty(OS_Curve_RectangularHyperbola1Fields::OutputUnitType);
  }

  void CurveRectangularHyperbola1_Impl::setCoefficient1C1(double coefficient1C1) {
    bool result = setDouble(OS_Curve_RectangularHyperbola1Fields::Coefficient1C1, coefficient1C1);
    OS_ASSERT(result);
  }

  void CurveRectangularHyperbola1_Impl::setCoefficient2C2(double coefficient2C2) {
    bool result = setDouble(OS_Curve_RectangularHyperbola1Fields::Coefficient2C2, coefficient2C2);
    OS_ASSERT(result);
  }

  void CurveRectangularHyperbola1_Impl::setCoefficient3C3(double coefficient3C3) {
    bool result = setDouble(OS_Curve_RectangularHyperbola1Fields::Coefficient3C3, coefficient3C3);
    OS_ASSERT(result);
  }

  void CurveRectangularHyperbola1_Impl::setMinimumValueofx(double minimumValueofx) {
    bool result = setDouble(OS_Curve_RectangularHyperbola1Fields::MinimumValueofx, minimumValueofx);
    OS_ASSERT(result);
  }

  void CurveRectangularHyperbola1_Impl::setMaximumValueofx(double maximumValueofx) {
    bool result = setDouble(OS_Curve_RectangularHyperbola1Fields::MaximumValueofx, maximumValueofx);
    OS_ASSERT(result);
  }

  void CurveRectangularHyperbola1_Impl::setMinimumCurveOutput(boost::optional<double> minimumCurveOutput) {
    bool result = false;
    if (minimumCurveOutput) {
      result = setDouble(OS_Curve_RectangularHyperbola1Fields::MinimumCurveOutput, minimumCurveOutput.get());
    } else {
      result = setString(OS_Curve_RectangularHyperbola1Fields::MinimumCurveOutput, "");
    }
    OS_ASSERT(result);
  }

  void CurveRectangularHyperbola1_Impl::resetMinimumCurveOutput() {
    bool result = setString(OS_Curve_RectangularHyperbola1Fields::MinimumCurveOutput, "");
    OS_ASSERT(result);
  }

  void CurveRectangularHyperbola1_Impl::setMaximumCurveOutput(boost::optional<double> maximumCurveOutput) {
    bool result = false;
    if (maximumCurveOutput) {
      result = setDouble(OS_Curve_RectangularHyperbola1Fields::MaximumCurveOutput, maximumCurveOutput.get());
    } else {
      result = setString(OS_Curve_RectangularHyperbola1Fields::MaximumCurveOutput, "");
    }
    OS_ASSERT(result);
  }

  void CurveRectangularHyperbola1_Impl::resetMaximumCurveOutput() {
    bool result = setString(OS_Curve_RectangularHyperbola1Fields::MaximumCurveOutput, "");
    OS_ASSERT(result);
  }

  bool CurveRectangularHyperbola1_Impl::setInputUnitTypeforx(std::string inputUnitTypeforx) {
    bool result = setString(OS_Curve_RectangularHyperbola1Fields::InputUnitTypeforx, inputUnitTypeforx);
    return result;
  }

  void CurveRectangularHyperbola1_Impl::resetInputUnitTypeforx() {
    bool result = setString(OS_Curve_RectangularHyperbola1Fields::InputUnitTypeforx, "");
    OS_ASSERT(result);
  }

  bool CurveRectangularHyperbola1_Impl::setOutputUnitType(std::string outputUnitType) {
    bool result = setString(OS_Curve_RectangularHyperbola1Fields::OutputUnitType, outputUnitType);
    return result;
  }

  void CurveRectangularHyperbola1_Impl::resetOutputUnitType() {
    bool result = setString(OS_Curve_RectangularHyperbola1Fields::OutputUnitType, "");
    OS_ASSERT(result);
  }

} // detail

CurveRectangularHyperbola1::CurveRectangularHyperbola1(const Model& model)
  : Curve(CurveRectangularHyperbola1::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CurveRectangularHyperbola1_Impl>());
  setDouble(OS_Curve_RectangularHyperbola1Fields::Coefficient1C1,1.0);
  setDouble(OS_Curve_RectangularHyperbola1Fields::Coefficient2C2,1.0);
  setDouble(OS_Curve_RectangularHyperbola1Fields::Coefficient3C3,0.0);
  setDouble(OS_Curve_RectangularHyperbola1Fields::MinimumValueofx,0.0);
  setDouble(OS_Curve_RectangularHyperbola1Fields::MaximumValueofx,1.0);
}

IddObjectType CurveRectangularHyperbola1::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Curve_RectangularHyperbola1);
  return result;
}

std::vector<std::string> CurveRectangularHyperbola1::validInputUnitTypeforxValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Curve_RectangularHyperbola1Fields::InputUnitTypeforx);
}

std::vector<std::string> CurveRectangularHyperbola1::validOutputUnitTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Curve_RectangularHyperbola1Fields::OutputUnitType);
}

double CurveRectangularHyperbola1::coefficient1C1() const {
  return getImpl<detail::CurveRectangularHyperbola1_Impl>()->coefficient1C1();
}

double CurveRectangularHyperbola1::coefficient2C2() const {
  return getImpl<detail::CurveRectangularHyperbola1_Impl>()->coefficient2C2();
}

double CurveRectangularHyperbola1::coefficient3C3() const {
  return getImpl<detail::CurveRectangularHyperbola1_Impl>()->coefficient3C3();
}

double CurveRectangularHyperbola1::minimumValueofx() const {
  return getImpl<detail::CurveRectangularHyperbola1_Impl>()->minimumValueofx();
}

double CurveRectangularHyperbola1::maximumValueofx() const {
  return getImpl<detail::CurveRectangularHyperbola1_Impl>()->maximumValueofx();
}

boost::optional<double> CurveRectangularHyperbola1::minimumCurveOutput() const {
  return getImpl<detail::CurveRectangularHyperbola1_Impl>()->minimumCurveOutput();
}

boost::optional<double> CurveRectangularHyperbola1::maximumCurveOutput() const {
  return getImpl<detail::CurveRectangularHyperbola1_Impl>()->maximumCurveOutput();
}

std::string CurveRectangularHyperbola1::inputUnitTypeforx() const {
  return getImpl<detail::CurveRectangularHyperbola1_Impl>()->inputUnitTypeforx();
}

bool CurveRectangularHyperbola1::isInputUnitTypeforxDefaulted() const {
  return getImpl<detail::CurveRectangularHyperbola1_Impl>()->isInputUnitTypeforxDefaulted();
}

std::string CurveRectangularHyperbola1::outputUnitType() const {
  return getImpl<detail::CurveRectangularHyperbola1_Impl>()->outputUnitType();
}

bool CurveRectangularHyperbola1::isOutputUnitTypeDefaulted() const {
  return getImpl<detail::CurveRectangularHyperbola1_Impl>()->isOutputUnitTypeDefaulted();
}

void CurveRectangularHyperbola1::setCoefficient1C1(double coefficient1C1) {
  getImpl<detail::CurveRectangularHyperbola1_Impl>()->setCoefficient1C1(coefficient1C1);
}

void CurveRectangularHyperbola1::setCoefficient2C2(double coefficient2C2) {
  getImpl<detail::CurveRectangularHyperbola1_Impl>()->setCoefficient2C2(coefficient2C2);
}

void CurveRectangularHyperbola1::setCoefficient3C3(double coefficient3C3) {
  getImpl<detail::CurveRectangularHyperbola1_Impl>()->setCoefficient3C3(coefficient3C3);
}

void CurveRectangularHyperbola1::setMinimumValueofx(double minimumValueofx) {
  getImpl<detail::CurveRectangularHyperbola1_Impl>()->setMinimumValueofx(minimumValueofx);
}

void CurveRectangularHyperbola1::setMaximumValueofx(double maximumValueofx) {
  getImpl<detail::CurveRectangularHyperbola1_Impl>()->setMaximumValueofx(maximumValueofx);
}

void CurveRectangularHyperbola1::setMinimumCurveOutput(double minimumCurveOutput) {
  getImpl<detail::CurveRectangularHyperbola1_Impl>()->setMinimumCurveOutput(minimumCurveOutput);
}

void CurveRectangularHyperbola1::resetMinimumCurveOutput() {
  getImpl<detail::CurveRectangularHyperbola1_Impl>()->resetMinimumCurveOutput();
}

void CurveRectangularHyperbola1::setMaximumCurveOutput(double maximumCurveOutput) {
  getImpl<detail::CurveRectangularHyperbola1_Impl>()->setMaximumCurveOutput(maximumCurveOutput);
}

void CurveRectangularHyperbola1::resetMaximumCurveOutput() {
  getImpl<detail::CurveRectangularHyperbola1_Impl>()->resetMaximumCurveOutput();
}

bool CurveRectangularHyperbola1::setInputUnitTypeforx(std::string inputUnitTypeforx) {
  return getImpl<detail::CurveRectangularHyperbola1_Impl>()->setInputUnitTypeforx(inputUnitTypeforx);
}

void CurveRectangularHyperbola1::resetInputUnitTypeforx() {
  getImpl<detail::CurveRectangularHyperbola1_Impl>()->resetInputUnitTypeforx();
}

bool CurveRectangularHyperbola1::setOutputUnitType(std::string outputUnitType) {
  return getImpl<detail::CurveRectangularHyperbola1_Impl>()->setOutputUnitType(outputUnitType);
}

void CurveRectangularHyperbola1::resetOutputUnitType() {
  getImpl<detail::CurveRectangularHyperbola1_Impl>()->resetOutputUnitType();
}

/// @cond
CurveRectangularHyperbola1::CurveRectangularHyperbola1(std::shared_ptr<detail::CurveRectangularHyperbola1_Impl> impl)
  : Curve(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

