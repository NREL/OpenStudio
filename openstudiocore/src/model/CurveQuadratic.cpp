/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#include <model/CurveQuadratic.hpp>
#include <model/CurveQuadratic_Impl.hpp>

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_Curve_Quadratic_FieldEnums.hxx>

#include <utilities/core/Assert.hpp>

#include <cmath>

using namespace std;

namespace openstudio {
namespace model {

namespace detail {

  CurveQuadratic_Impl::CurveQuadratic_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : Curve_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CurveQuadratic::iddObjectType());
  }

  CurveQuadratic_Impl::CurveQuadratic_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
    : Curve_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CurveQuadratic::iddObjectType());
  }

  CurveQuadratic_Impl::CurveQuadratic_Impl(const CurveQuadratic_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
    : Curve_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CurveQuadratic_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CurveQuadratic_Impl::iddObjectType() const {
    return CurveQuadratic::iddObjectType();
  }

  int CurveQuadratic_Impl::numVariables() const {
    return 1;
  }

  double CurveQuadratic_Impl::evaluate(const std::vector<double>& x) const {
    OS_ASSERT(x.size() == 1u);
    double result = coefficient1Constant();
    result += coefficient2x() * x[0];
    result += coefficient3xPOW2() * pow(x[0],2);
    return result;
  }

  double CurveQuadratic_Impl::coefficient1Constant() const {
    boost::optional<double> value = getDouble(OS_Curve_QuadraticFields::Coefficient1Constant,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveQuadratic_Impl::coefficient2x() const {
    boost::optional<double> value = getDouble(OS_Curve_QuadraticFields::Coefficient2x,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveQuadratic_Impl::coefficient3xPOW2() const {
    boost::optional<double> value = getDouble(OS_Curve_QuadraticFields::Coefficient3x_POW_2,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveQuadratic_Impl::minimumValueofx() const {
    boost::optional<double> value = getDouble(OS_Curve_QuadraticFields::MinimumValueofx,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveQuadratic_Impl::maximumValueofx() const {
    boost::optional<double> value = getDouble(OS_Curve_QuadraticFields::MaximumValueofx,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> CurveQuadratic_Impl::minimumCurveOutput() const {
    return getDouble(OS_Curve_QuadraticFields::MinimumCurveOutput,true);
  }

  boost::optional<double> CurveQuadratic_Impl::maximumCurveOutput() const {
    return getDouble(OS_Curve_QuadraticFields::MaximumCurveOutput,true);
  }

  std::string CurveQuadratic_Impl::inputUnitTypeforX() const {
    boost::optional<std::string> value = getString(OS_Curve_QuadraticFields::InputUnitTypeforX,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CurveQuadratic_Impl::isInputUnitTypeforXDefaulted() const {
    return isEmpty(OS_Curve_QuadraticFields::InputUnitTypeforX);
  }

  std::string CurveQuadratic_Impl::outputUnitType() const {
    boost::optional<std::string> value = getString(OS_Curve_QuadraticFields::OutputUnitType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CurveQuadratic_Impl::isOutputUnitTypeDefaulted() const {
    return isEmpty(OS_Curve_QuadraticFields::OutputUnitType);
  }

  void CurveQuadratic_Impl::setCoefficient1Constant(double coefficient1Constant) {
    bool result = setDouble(OS_Curve_QuadraticFields::Coefficient1Constant, coefficient1Constant);
    OS_ASSERT(result);
  }

  void CurveQuadratic_Impl::setCoefficient2x(double coefficient2x) {
    bool result = setDouble(OS_Curve_QuadraticFields::Coefficient2x, coefficient2x);
    OS_ASSERT(result);
  }

  void CurveQuadratic_Impl::setCoefficient3xPOW2(double coefficient3xPOW2) {
    bool result = setDouble(OS_Curve_QuadraticFields::Coefficient3x_POW_2, coefficient3xPOW2);
    OS_ASSERT(result);
  }

  void CurveQuadratic_Impl::setMinimumValueofx(double minimumValueofx) {
    bool result = setDouble(OS_Curve_QuadraticFields::MinimumValueofx, minimumValueofx);
    OS_ASSERT(result);
  }

  void CurveQuadratic_Impl::setMaximumValueofx(double maximumValueofx) {
    bool result = setDouble(OS_Curve_QuadraticFields::MaximumValueofx, maximumValueofx);
    OS_ASSERT(result);
  }

  void CurveQuadratic_Impl::setMinimumCurveOutput(boost::optional<double> minimumCurveOutput) {
    bool result = false;
    if (minimumCurveOutput) {
      result = setDouble(OS_Curve_QuadraticFields::MinimumCurveOutput, minimumCurveOutput.get());
    } else {
      result = setString(OS_Curve_QuadraticFields::MinimumCurveOutput, "");
    }
    OS_ASSERT(result);
  }

  void CurveQuadratic_Impl::resetMinimumCurveOutput() {
    bool result = setString(OS_Curve_QuadraticFields::MinimumCurveOutput, "");
    OS_ASSERT(result);
  }

  void CurveQuadratic_Impl::setMaximumCurveOutput(boost::optional<double> maximumCurveOutput) {
    bool result = false;
    if (maximumCurveOutput) {
      result = setDouble(OS_Curve_QuadraticFields::MaximumCurveOutput, maximumCurveOutput.get());
    } else {
      result = setString(OS_Curve_QuadraticFields::MaximumCurveOutput, "");
    }
    OS_ASSERT(result);
  }

  void CurveQuadratic_Impl::resetMaximumCurveOutput() {
    bool result = setString(OS_Curve_QuadraticFields::MaximumCurveOutput, "");
    OS_ASSERT(result);
  }

  bool CurveQuadratic_Impl::setInputUnitTypeforX(std::string inputUnitTypeforX) {
    bool result = setString(OS_Curve_QuadraticFields::InputUnitTypeforX, inputUnitTypeforX);
    return result;
  }

  void CurveQuadratic_Impl::resetInputUnitTypeforX() {
    bool result = setString(OS_Curve_QuadraticFields::InputUnitTypeforX, "");
    OS_ASSERT(result);
  }

  bool CurveQuadratic_Impl::setOutputUnitType(std::string outputUnitType) {
    bool result = setString(OS_Curve_QuadraticFields::OutputUnitType, outputUnitType);
    return result;
  }

  void CurveQuadratic_Impl::resetOutputUnitType() {
    bool result = setString(OS_Curve_QuadraticFields::OutputUnitType, "");
    OS_ASSERT(result);
  }

} // detail

CurveQuadratic::CurveQuadratic(const Model& model)
  : Curve(CurveQuadratic::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CurveQuadratic_Impl>());
  setDouble(OS_Curve_QuadraticFields::Coefficient1Constant,0.0);
  setDouble(OS_Curve_QuadraticFields::Coefficient2x,0.0);
  setDouble(OS_Curve_QuadraticFields::Coefficient3x_POW_2,1.0);
  setDouble(OS_Curve_QuadraticFields::MinimumValueofx,0.0);
  setDouble(OS_Curve_QuadraticFields::MaximumValueofx,1.0);
}

IddObjectType CurveQuadratic::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Curve_Quadratic);
  return result;
}

std::vector<std::string> CurveQuadratic::validInputUnitTypeforXValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Curve_QuadraticFields::InputUnitTypeforX);
}

std::vector<std::string> CurveQuadratic::validOutputUnitTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Curve_QuadraticFields::OutputUnitType);
}

double CurveQuadratic::coefficient1Constant() const {
  return getImpl<detail::CurveQuadratic_Impl>()->coefficient1Constant();
}

double CurveQuadratic::coefficient2x() const {
  return getImpl<detail::CurveQuadratic_Impl>()->coefficient2x();
}

double CurveQuadratic::coefficient3xPOW2() const {
  return getImpl<detail::CurveQuadratic_Impl>()->coefficient3xPOW2();
}

double CurveQuadratic::minimumValueofx() const {
  return getImpl<detail::CurveQuadratic_Impl>()->minimumValueofx();
}

double CurveQuadratic::maximumValueofx() const {
  return getImpl<detail::CurveQuadratic_Impl>()->maximumValueofx();
}

boost::optional<double> CurveQuadratic::minimumCurveOutput() const {
  return getImpl<detail::CurveQuadratic_Impl>()->minimumCurveOutput();
}

boost::optional<double> CurveQuadratic::maximumCurveOutput() const {
  return getImpl<detail::CurveQuadratic_Impl>()->maximumCurveOutput();
}

std::string CurveQuadratic::inputUnitTypeforX() const {
  return getImpl<detail::CurveQuadratic_Impl>()->inputUnitTypeforX();
}

bool CurveQuadratic::isInputUnitTypeforXDefaulted() const {
  return getImpl<detail::CurveQuadratic_Impl>()->isInputUnitTypeforXDefaulted();
}

std::string CurveQuadratic::outputUnitType() const {
  return getImpl<detail::CurveQuadratic_Impl>()->outputUnitType();
}

bool CurveQuadratic::isOutputUnitTypeDefaulted() const {
  return getImpl<detail::CurveQuadratic_Impl>()->isOutputUnitTypeDefaulted();
}

void CurveQuadratic::setCoefficient1Constant(double coefficient1Constant) {
  getImpl<detail::CurveQuadratic_Impl>()->setCoefficient1Constant(coefficient1Constant);
}

void CurveQuadratic::setCoefficient2x(double coefficient2x) {
  getImpl<detail::CurveQuadratic_Impl>()->setCoefficient2x(coefficient2x);
}

void CurveQuadratic::setCoefficient3xPOW2(double coefficient3xPOW2) {
  getImpl<detail::CurveQuadratic_Impl>()->setCoefficient3xPOW2(coefficient3xPOW2);
}

void CurveQuadratic::setMinimumValueofx(double minimumValueofx) {
  getImpl<detail::CurveQuadratic_Impl>()->setMinimumValueofx(minimumValueofx);
}

void CurveQuadratic::setMaximumValueofx(double maximumValueofx) {
  getImpl<detail::CurveQuadratic_Impl>()->setMaximumValueofx(maximumValueofx);
}

void CurveQuadratic::setMinimumCurveOutput(double minimumCurveOutput) {
  getImpl<detail::CurveQuadratic_Impl>()->setMinimumCurveOutput(minimumCurveOutput);
}

void CurveQuadratic::resetMinimumCurveOutput() {
  getImpl<detail::CurveQuadratic_Impl>()->resetMinimumCurveOutput();
}

void CurveQuadratic::setMaximumCurveOutput(double maximumCurveOutput) {
  getImpl<detail::CurveQuadratic_Impl>()->setMaximumCurveOutput(maximumCurveOutput);
}

void CurveQuadratic::resetMaximumCurveOutput() {
  getImpl<detail::CurveQuadratic_Impl>()->resetMaximumCurveOutput();
}

bool CurveQuadratic::setInputUnitTypeforX(std::string inputUnitTypeforX) {
  return getImpl<detail::CurveQuadratic_Impl>()->setInputUnitTypeforX(inputUnitTypeforX);
}

void CurveQuadratic::resetInputUnitTypeforX() {
  getImpl<detail::CurveQuadratic_Impl>()->resetInputUnitTypeforX();
}

bool CurveQuadratic::setOutputUnitType(std::string outputUnitType) {
  return getImpl<detail::CurveQuadratic_Impl>()->setOutputUnitType(outputUnitType);
}

void CurveQuadratic::resetOutputUnitType() {
  getImpl<detail::CurveQuadratic_Impl>()->resetOutputUnitType();
}

/// @cond
CurveQuadratic::CurveQuadratic(boost::shared_ptr<detail::CurveQuadratic_Impl> impl)
  : Curve(impl)
{}
/// @endcond

} // model
} // openstudio
