/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#include "CurveExponentialSkewNormal.hpp"
#include "CurveExponentialSkewNormal_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Curve_ExponentialSkewNormal_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

#include <cmath>
#include <boost/math/special_functions/erf.hpp>

using namespace std;

namespace openstudio {
namespace model {

namespace detail {

  CurveExponentialSkewNormal_Impl::CurveExponentialSkewNormal_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : Curve_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CurveExponentialSkewNormal::iddObjectType());
  }

  CurveExponentialSkewNormal_Impl::CurveExponentialSkewNormal_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                   Model_Impl* model,
                                                                   bool keepHandle)
    : Curve_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CurveExponentialSkewNormal::iddObjectType());
  }

  CurveExponentialSkewNormal_Impl::CurveExponentialSkewNormal_Impl(const CurveExponentialSkewNormal_Impl& other,
                                                                   Model_Impl* model,
                                                                   bool keepHandle)
    : Curve_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CurveExponentialSkewNormal_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CurveExponentialSkewNormal_Impl::iddObjectType() const {
    return CurveExponentialSkewNormal::iddObjectType();
  }

  int CurveExponentialSkewNormal_Impl::numVariables() const {
    return 1;
  }

  double CurveExponentialSkewNormal_Impl::evaluate(const std::vector<double>& x) const {
    OS_ASSERT(x.size() == 1u);
    double z1 = (x[0] - coefficient1C1()) / coefficient2C2();
    double z2 = (exp(coefficient3C3() * x[0]) * coefficient4C4() * x[0] - coefficient1C1()) / 
                coefficient2C2();
    double z3 = -coefficient1C1()/coefficient4C4();
    double numerator = 1.0 + (z2/abs(z2)) * boost::math::erf<double>(abs(z2)/sqrt(2.0));
    numerator *= exp(-0.5 * pow(z1,2));
    double denominator = 1.0 + (z3/abs(z3)) * boost::math::erf<double>(abs(z3)/sqrt(2.0));
    denominator *= exp(-0.5 * pow(z3,2));
    return numerator/denominator;
  }

  double CurveExponentialSkewNormal_Impl::coefficient1C1() const {
    boost::optional<double> value = getDouble(OS_Curve_ExponentialSkewNormalFields::Coefficient1C1,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveExponentialSkewNormal_Impl::coefficient2C2() const {
    boost::optional<double> value = getDouble(OS_Curve_ExponentialSkewNormalFields::Coefficient2C2,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveExponentialSkewNormal_Impl::coefficient3C3() const {
    boost::optional<double> value = getDouble(OS_Curve_ExponentialSkewNormalFields::Coefficient3C3,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveExponentialSkewNormal_Impl::coefficient4C4() const {
    boost::optional<double> value = getDouble(OS_Curve_ExponentialSkewNormalFields::Coefficient4C4,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveExponentialSkewNormal_Impl::minimumValueofx() const {
    boost::optional<double> value = getDouble(OS_Curve_ExponentialSkewNormalFields::MinimumValueofx,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveExponentialSkewNormal_Impl::maximumValueofx() const {
    boost::optional<double> value = getDouble(OS_Curve_ExponentialSkewNormalFields::MaximumValueofx,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> CurveExponentialSkewNormal_Impl::minimumCurveOutput() const {
    return getDouble(OS_Curve_ExponentialSkewNormalFields::MinimumCurveOutput,true);
  }

  boost::optional<double> CurveExponentialSkewNormal_Impl::maximumCurveOutput() const {
    return getDouble(OS_Curve_ExponentialSkewNormalFields::MaximumCurveOutput,true);
  }

  std::string CurveExponentialSkewNormal_Impl::inputUnitTypeforx() const {
    boost::optional<std::string> value = getString(OS_Curve_ExponentialSkewNormalFields::InputUnitTypeforx,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CurveExponentialSkewNormal_Impl::isInputUnitTypeforxDefaulted() const {
    return isEmpty(OS_Curve_ExponentialSkewNormalFields::InputUnitTypeforx);
  }

  std::string CurveExponentialSkewNormal_Impl::outputUnitType() const {
    boost::optional<std::string> value = getString(OS_Curve_ExponentialSkewNormalFields::OutputUnitType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CurveExponentialSkewNormal_Impl::isOutputUnitTypeDefaulted() const {
    return isEmpty(OS_Curve_ExponentialSkewNormalFields::OutputUnitType);
  }

  void CurveExponentialSkewNormal_Impl::setCoefficient1C1(double coefficient1C1) {
    bool result = setDouble(OS_Curve_ExponentialSkewNormalFields::Coefficient1C1, coefficient1C1);
    OS_ASSERT(result);
  }

  void CurveExponentialSkewNormal_Impl::setCoefficient2C2(double coefficient2C2) {
    bool result = setDouble(OS_Curve_ExponentialSkewNormalFields::Coefficient2C2, coefficient2C2);
    OS_ASSERT(result);
  }

  void CurveExponentialSkewNormal_Impl::setCoefficient3C3(double coefficient3C3) {
    bool result = setDouble(OS_Curve_ExponentialSkewNormalFields::Coefficient3C3, coefficient3C3);
    OS_ASSERT(result);
  }

  void CurveExponentialSkewNormal_Impl::setCoefficient4C4(double coefficient4C4) {
    bool result = setDouble(OS_Curve_ExponentialSkewNormalFields::Coefficient4C4, coefficient4C4);
    OS_ASSERT(result);
  }

  void CurveExponentialSkewNormal_Impl::setMinimumValueofx(double minimumValueofx) {
    bool result = setDouble(OS_Curve_ExponentialSkewNormalFields::MinimumValueofx, minimumValueofx);
    OS_ASSERT(result);
  }

  void CurveExponentialSkewNormal_Impl::setMaximumValueofx(double maximumValueofx) {
    bool result = setDouble(OS_Curve_ExponentialSkewNormalFields::MaximumValueofx, maximumValueofx);
    OS_ASSERT(result);
  }

  void CurveExponentialSkewNormal_Impl::setMinimumCurveOutput(boost::optional<double> minimumCurveOutput) {
    bool result = false;
    if (minimumCurveOutput) {
      result = setDouble(OS_Curve_ExponentialSkewNormalFields::MinimumCurveOutput, minimumCurveOutput.get());
    } else {
      result = setString(OS_Curve_ExponentialSkewNormalFields::MinimumCurveOutput, "");
    }
    OS_ASSERT(result);
  }

  void CurveExponentialSkewNormal_Impl::resetMinimumCurveOutput() {
    bool result = setString(OS_Curve_ExponentialSkewNormalFields::MinimumCurveOutput, "");
    OS_ASSERT(result);
  }

  void CurveExponentialSkewNormal_Impl::setMaximumCurveOutput(boost::optional<double> maximumCurveOutput) {
    bool result = false;
    if (maximumCurveOutput) {
      result = setDouble(OS_Curve_ExponentialSkewNormalFields::MaximumCurveOutput, maximumCurveOutput.get());
    } else {
      result = setString(OS_Curve_ExponentialSkewNormalFields::MaximumCurveOutput, "");
    }
    OS_ASSERT(result);
  }

  void CurveExponentialSkewNormal_Impl::resetMaximumCurveOutput() {
    bool result = setString(OS_Curve_ExponentialSkewNormalFields::MaximumCurveOutput, "");
    OS_ASSERT(result);
  }

  bool CurveExponentialSkewNormal_Impl::setInputUnitTypeforx(std::string inputUnitTypeforx) {
    bool result = setString(OS_Curve_ExponentialSkewNormalFields::InputUnitTypeforx, inputUnitTypeforx);
    return result;
  }

  void CurveExponentialSkewNormal_Impl::resetInputUnitTypeforx() {
    bool result = setString(OS_Curve_ExponentialSkewNormalFields::InputUnitTypeforx, "");
    OS_ASSERT(result);
  }

  bool CurveExponentialSkewNormal_Impl::setOutputUnitType(std::string outputUnitType) {
    bool result = setString(OS_Curve_ExponentialSkewNormalFields::OutputUnitType, outputUnitType);
    return result;
  }

  void CurveExponentialSkewNormal_Impl::resetOutputUnitType() {
    bool result = setString(OS_Curve_ExponentialSkewNormalFields::OutputUnitType, "");
    OS_ASSERT(result);
  }

} // detail

CurveExponentialSkewNormal::CurveExponentialSkewNormal(const Model& model)
  : Curve(CurveExponentialSkewNormal::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CurveExponentialSkewNormal_Impl>());
  setDouble(OS_Curve_ExponentialSkewNormalFields::Coefficient1C1,1.0);
  setDouble(OS_Curve_ExponentialSkewNormalFields::Coefficient2C2,1.0);
  setDouble(OS_Curve_ExponentialSkewNormalFields::Coefficient3C3,-1.0);
  setDouble(OS_Curve_ExponentialSkewNormalFields::Coefficient4C4,1.0);
  setDouble(OS_Curve_ExponentialSkewNormalFields::MinimumValueofx,-1.0);
  setDouble(OS_Curve_ExponentialSkewNormalFields::MaximumValueofx,1.0);
}

IddObjectType CurveExponentialSkewNormal::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Curve_ExponentialSkewNormal);
  return result;
}

std::vector<std::string> CurveExponentialSkewNormal::validInputUnitTypeforxValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Curve_ExponentialSkewNormalFields::InputUnitTypeforx);
}

std::vector<std::string> CurveExponentialSkewNormal::validOutputUnitTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Curve_ExponentialSkewNormalFields::OutputUnitType);
}

double CurveExponentialSkewNormal::coefficient1C1() const {
  return getImpl<detail::CurveExponentialSkewNormal_Impl>()->coefficient1C1();
}

double CurveExponentialSkewNormal::coefficient2C2() const {
  return getImpl<detail::CurveExponentialSkewNormal_Impl>()->coefficient2C2();
}

double CurveExponentialSkewNormal::coefficient3C3() const {
  return getImpl<detail::CurveExponentialSkewNormal_Impl>()->coefficient3C3();
}

double CurveExponentialSkewNormal::coefficient4C4() const {
  return getImpl<detail::CurveExponentialSkewNormal_Impl>()->coefficient4C4();
}

double CurveExponentialSkewNormal::minimumValueofx() const {
  return getImpl<detail::CurveExponentialSkewNormal_Impl>()->minimumValueofx();
}

double CurveExponentialSkewNormal::maximumValueofx() const {
  return getImpl<detail::CurveExponentialSkewNormal_Impl>()->maximumValueofx();
}

boost::optional<double> CurveExponentialSkewNormal::minimumCurveOutput() const {
  return getImpl<detail::CurveExponentialSkewNormal_Impl>()->minimumCurveOutput();
}

boost::optional<double> CurveExponentialSkewNormal::maximumCurveOutput() const {
  return getImpl<detail::CurveExponentialSkewNormal_Impl>()->maximumCurveOutput();
}

std::string CurveExponentialSkewNormal::inputUnitTypeforx() const {
  return getImpl<detail::CurveExponentialSkewNormal_Impl>()->inputUnitTypeforx();
}

bool CurveExponentialSkewNormal::isInputUnitTypeforxDefaulted() const {
  return getImpl<detail::CurveExponentialSkewNormal_Impl>()->isInputUnitTypeforxDefaulted();
}

std::string CurveExponentialSkewNormal::outputUnitType() const {
  return getImpl<detail::CurveExponentialSkewNormal_Impl>()->outputUnitType();
}

bool CurveExponentialSkewNormal::isOutputUnitTypeDefaulted() const {
  return getImpl<detail::CurveExponentialSkewNormal_Impl>()->isOutputUnitTypeDefaulted();
}

void CurveExponentialSkewNormal::setCoefficient1C1(double coefficient1C1) {
  getImpl<detail::CurveExponentialSkewNormal_Impl>()->setCoefficient1C1(coefficient1C1);
}

void CurveExponentialSkewNormal::setCoefficient2C2(double coefficient2C2) {
  getImpl<detail::CurveExponentialSkewNormal_Impl>()->setCoefficient2C2(coefficient2C2);
}

void CurveExponentialSkewNormal::setCoefficient3C3(double coefficient3C3) {
  getImpl<detail::CurveExponentialSkewNormal_Impl>()->setCoefficient3C3(coefficient3C3);
}

void CurveExponentialSkewNormal::setCoefficient4C4(double coefficient4C4) {
  getImpl<detail::CurveExponentialSkewNormal_Impl>()->setCoefficient4C4(coefficient4C4);
}

void CurveExponentialSkewNormal::setMinimumValueofx(double minimumValueofx) {
  getImpl<detail::CurveExponentialSkewNormal_Impl>()->setMinimumValueofx(minimumValueofx);
}

void CurveExponentialSkewNormal::setMaximumValueofx(double maximumValueofx) {
  getImpl<detail::CurveExponentialSkewNormal_Impl>()->setMaximumValueofx(maximumValueofx);
}

void CurveExponentialSkewNormal::setMinimumCurveOutput(double minimumCurveOutput) {
  getImpl<detail::CurveExponentialSkewNormal_Impl>()->setMinimumCurveOutput(minimumCurveOutput);
}

void CurveExponentialSkewNormal::resetMinimumCurveOutput() {
  getImpl<detail::CurveExponentialSkewNormal_Impl>()->resetMinimumCurveOutput();
}

void CurveExponentialSkewNormal::setMaximumCurveOutput(double maximumCurveOutput) {
  getImpl<detail::CurveExponentialSkewNormal_Impl>()->setMaximumCurveOutput(maximumCurveOutput);
}

void CurveExponentialSkewNormal::resetMaximumCurveOutput() {
  getImpl<detail::CurveExponentialSkewNormal_Impl>()->resetMaximumCurveOutput();
}

bool CurveExponentialSkewNormal::setInputUnitTypeforx(std::string inputUnitTypeforx) {
  return getImpl<detail::CurveExponentialSkewNormal_Impl>()->setInputUnitTypeforx(inputUnitTypeforx);
}

void CurveExponentialSkewNormal::resetInputUnitTypeforx() {
  getImpl<detail::CurveExponentialSkewNormal_Impl>()->resetInputUnitTypeforx();
}

bool CurveExponentialSkewNormal::setOutputUnitType(std::string outputUnitType) {
  return getImpl<detail::CurveExponentialSkewNormal_Impl>()->setOutputUnitType(outputUnitType);
}

void CurveExponentialSkewNormal::resetOutputUnitType() {
  getImpl<detail::CurveExponentialSkewNormal_Impl>()->resetOutputUnitType();
}

/// @cond
CurveExponentialSkewNormal::CurveExponentialSkewNormal(std::shared_ptr<detail::CurveExponentialSkewNormal_Impl> impl)
  : Curve(impl)
{}
/// @endcond

} // model
} // openstudio

