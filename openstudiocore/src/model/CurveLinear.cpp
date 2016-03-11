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

#include "CurveLinear.hpp"
#include "CurveLinear_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Curve_Linear_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

#include <cmath>

using namespace std;

namespace openstudio {
namespace model {

namespace detail {

  CurveLinear_Impl::CurveLinear_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : Curve_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CurveLinear::iddObjectType());
  }

  CurveLinear_Impl::CurveLinear_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle)
    : Curve_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CurveLinear::iddObjectType());
  }

  CurveLinear_Impl::CurveLinear_Impl(const CurveLinear_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle)
    : Curve_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CurveLinear_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CurveLinear_Impl::iddObjectType() const {
    return CurveLinear::iddObjectType();
  }

  int CurveLinear_Impl::numVariables() const {
    return 1;
  }

  double CurveLinear_Impl::evaluate(const std::vector<double>& x) const {
    OS_ASSERT(x.size() == 1u);
    double result = coefficient1Constant();
    result += coefficient2x() * x[0];
    return result;
  }

  double CurveLinear_Impl::coefficient1Constant() const {
    boost::optional<double> value = getDouble(OS_Curve_LinearFields::Coefficient1Constant,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveLinear_Impl::coefficient2x() const {
    boost::optional<double> value = getDouble(OS_Curve_LinearFields::Coefficient2x,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveLinear_Impl::minimumValueofx() const {
    boost::optional<double> value = getDouble(OS_Curve_LinearFields::MinimumValueofx,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveLinear_Impl::maximumValueofx() const {
    boost::optional<double> value = getDouble(OS_Curve_LinearFields::MaximumValueofx,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> CurveLinear_Impl::minimumCurveOutput() const {
    return getDouble(OS_Curve_LinearFields::MinimumCurveOutput,true);
  }

  boost::optional<double> CurveLinear_Impl::maximumCurveOutput() const {
    return getDouble(OS_Curve_LinearFields::MaximumCurveOutput,true);
  }

  std::string CurveLinear_Impl::inputUnitTypeforX() const {
    boost::optional<std::string> value = getString(OS_Curve_LinearFields::InputUnitTypeforX,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CurveLinear_Impl::isInputUnitTypeforXDefaulted() const {
    return isEmpty(OS_Curve_LinearFields::InputUnitTypeforX);
  }

  std::string CurveLinear_Impl::outputUnitType() const {
    boost::optional<std::string> value = getString(OS_Curve_LinearFields::OutputUnitType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CurveLinear_Impl::isOutputUnitTypeDefaulted() const {
    return isEmpty(OS_Curve_LinearFields::OutputUnitType);
  }

  void CurveLinear_Impl::setCoefficient1Constant(double coefficient1Constant) {
    bool result = setDouble(OS_Curve_LinearFields::Coefficient1Constant, coefficient1Constant);
    OS_ASSERT(result);
  }

  void CurveLinear_Impl::setCoefficient2x(double coefficient2x) {
    bool result = setDouble(OS_Curve_LinearFields::Coefficient2x, coefficient2x);
    OS_ASSERT(result);
  }

  void CurveLinear_Impl::setMinimumValueofx(double minimumValueofx) {
    bool result = setDouble(OS_Curve_LinearFields::MinimumValueofx, minimumValueofx);
    OS_ASSERT(result);
  }

  void CurveLinear_Impl::setMaximumValueofx(double maximumValueofx) {
    bool result = setDouble(OS_Curve_LinearFields::MaximumValueofx, maximumValueofx);
    OS_ASSERT(result);
  }

  void CurveLinear_Impl::setMinimumCurveOutput(boost::optional<double> minimumCurveOutput) {
    bool result = false;
    if (minimumCurveOutput) {
      result = setDouble(OS_Curve_LinearFields::MinimumCurveOutput, minimumCurveOutput.get());
    } else {
      result = setString(OS_Curve_LinearFields::MinimumCurveOutput, "");
    }
    OS_ASSERT(result);
  }

  void CurveLinear_Impl::resetMinimumCurveOutput() {
    bool result = setString(OS_Curve_LinearFields::MinimumCurveOutput, "");
    OS_ASSERT(result);
  }

  void CurveLinear_Impl::setMaximumCurveOutput(boost::optional<double> maximumCurveOutput) {
    bool result = false;
    if (maximumCurveOutput) {
      result = setDouble(OS_Curve_LinearFields::MaximumCurveOutput, maximumCurveOutput.get());
    } else {
      result = setString(OS_Curve_LinearFields::MaximumCurveOutput, "");
    }
    OS_ASSERT(result);
  }

  void CurveLinear_Impl::resetMaximumCurveOutput() {
    bool result = setString(OS_Curve_LinearFields::MaximumCurveOutput, "");
    OS_ASSERT(result);
  }

  bool CurveLinear_Impl::setInputUnitTypeforX(std::string inputUnitTypeforX) {
    bool result = setString(OS_Curve_LinearFields::InputUnitTypeforX, inputUnitTypeforX);
    return result;
  }

  void CurveLinear_Impl::resetInputUnitTypeforX() {
    bool result = setString(OS_Curve_LinearFields::InputUnitTypeforX, "");
    OS_ASSERT(result);
  }

  bool CurveLinear_Impl::setOutputUnitType(std::string outputUnitType) {
    bool result = setString(OS_Curve_LinearFields::OutputUnitType, outputUnitType);
    return result;
  }

  void CurveLinear_Impl::resetOutputUnitType() {
    bool result = setString(OS_Curve_LinearFields::OutputUnitType, "");
    OS_ASSERT(result);
  }

} // detail

CurveLinear::CurveLinear(const Model& model)
  : Curve(CurveLinear::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CurveLinear_Impl>());
  setDouble(OS_Curve_LinearFields::Coefficient1Constant,0.0);
  setDouble(OS_Curve_LinearFields::Coefficient2x,1.0);
  setDouble(OS_Curve_LinearFields::MinimumValueofx,0.0);
  setDouble(OS_Curve_LinearFields::MaximumValueofx,1.0);
}

IddObjectType CurveLinear::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Curve_Linear);
  return result;
}

std::vector<std::string> CurveLinear::validInputUnitTypeforXValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Curve_LinearFields::InputUnitTypeforX);
}

std::vector<std::string> CurveLinear::validOutputUnitTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Curve_LinearFields::OutputUnitType);
}

double CurveLinear::coefficient1Constant() const {
  return getImpl<detail::CurveLinear_Impl>()->coefficient1Constant();
}

double CurveLinear::coefficient2x() const {
  return getImpl<detail::CurveLinear_Impl>()->coefficient2x();
}

double CurveLinear::minimumValueofx() const {
  return getImpl<detail::CurveLinear_Impl>()->minimumValueofx();
}

double CurveLinear::maximumValueofx() const {
  return getImpl<detail::CurveLinear_Impl>()->maximumValueofx();
}

boost::optional<double> CurveLinear::minimumCurveOutput() const {
  return getImpl<detail::CurveLinear_Impl>()->minimumCurveOutput();
}

boost::optional<double> CurveLinear::maximumCurveOutput() const {
  return getImpl<detail::CurveLinear_Impl>()->maximumCurveOutput();
}

std::string CurveLinear::inputUnitTypeforX() const {
  return getImpl<detail::CurveLinear_Impl>()->inputUnitTypeforX();
}

bool CurveLinear::isInputUnitTypeforXDefaulted() const {
  return getImpl<detail::CurveLinear_Impl>()->isInputUnitTypeforXDefaulted();
}

std::string CurveLinear::outputUnitType() const {
  return getImpl<detail::CurveLinear_Impl>()->outputUnitType();
}

bool CurveLinear::isOutputUnitTypeDefaulted() const {
  return getImpl<detail::CurveLinear_Impl>()->isOutputUnitTypeDefaulted();
}

void CurveLinear::setCoefficient1Constant(double coefficient1Constant) {
  getImpl<detail::CurveLinear_Impl>()->setCoefficient1Constant(coefficient1Constant);
}

void CurveLinear::setCoefficient2x(double coefficient2x) {
  getImpl<detail::CurveLinear_Impl>()->setCoefficient2x(coefficient2x);
}

void CurveLinear::setMinimumValueofx(double minimumValueofx) {
  getImpl<detail::CurveLinear_Impl>()->setMinimumValueofx(minimumValueofx);
}

void CurveLinear::setMaximumValueofx(double maximumValueofx) {
  getImpl<detail::CurveLinear_Impl>()->setMaximumValueofx(maximumValueofx);
}

void CurveLinear::setMinimumCurveOutput(double minimumCurveOutput) {
  getImpl<detail::CurveLinear_Impl>()->setMinimumCurveOutput(minimumCurveOutput);
}

void CurveLinear::resetMinimumCurveOutput() {
  getImpl<detail::CurveLinear_Impl>()->resetMinimumCurveOutput();
}

void CurveLinear::setMaximumCurveOutput(double maximumCurveOutput) {
  getImpl<detail::CurveLinear_Impl>()->setMaximumCurveOutput(maximumCurveOutput);
}

void CurveLinear::resetMaximumCurveOutput() {
  getImpl<detail::CurveLinear_Impl>()->resetMaximumCurveOutput();
}

bool CurveLinear::setInputUnitTypeforX(std::string inputUnitTypeforX) {
  return getImpl<detail::CurveLinear_Impl>()->setInputUnitTypeforX(inputUnitTypeforX);
}

void CurveLinear::resetInputUnitTypeforX() {
  getImpl<detail::CurveLinear_Impl>()->resetInputUnitTypeforX();
}

bool CurveLinear::setOutputUnitType(std::string outputUnitType) {
  return getImpl<detail::CurveLinear_Impl>()->setOutputUnitType(outputUnitType);
}

void CurveLinear::resetOutputUnitType() {
  getImpl<detail::CurveLinear_Impl>()->resetOutputUnitType();
}

/// @cond
CurveLinear::CurveLinear(std::shared_ptr<detail::CurveLinear_Impl> impl)
  : Curve(impl)
{}
/// @endcond

} // model
} // openstudio

