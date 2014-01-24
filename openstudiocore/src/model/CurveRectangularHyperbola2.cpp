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

#include <model/CurveRectangularHyperbola2.hpp>
#include <model/CurveRectangularHyperbola2_Impl.hpp>

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_Curve_RectangularHyperbola2_FieldEnums.hxx>

#include <utilities/core/Assert.hpp>

#include <cmath>

using namespace std;

namespace openstudio {
namespace model {

namespace detail {

  CurveRectangularHyperbola2_Impl::CurveRectangularHyperbola2_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : Curve_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CurveRectangularHyperbola2::iddObjectType());
  }

  CurveRectangularHyperbola2_Impl::CurveRectangularHyperbola2_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                   Model_Impl* model,
                                                                   bool keepHandle)
    : Curve_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CurveRectangularHyperbola2::iddObjectType());
  }

  CurveRectangularHyperbola2_Impl::CurveRectangularHyperbola2_Impl(const CurveRectangularHyperbola2_Impl& other,
                                                                   Model_Impl* model,
                                                                   bool keepHandle)
    : Curve_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CurveRectangularHyperbola2_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CurveRectangularHyperbola2_Impl::iddObjectType() const {
    return CurveRectangularHyperbola2::iddObjectType();
  }

  int CurveRectangularHyperbola2_Impl::numVariables() const {
    return 1;
  }

  double CurveRectangularHyperbola2_Impl::evaluate(const std::vector<double>& x) const {
    OS_ASSERT(x.size() == 1u);
    double result = coefficient1C1() * x[0];
    double temp = coefficient2C2() + x[0];
    result /= temp;
    result += coefficient3C3() * x[0];
    return result;
  }

  double CurveRectangularHyperbola2_Impl::coefficient1C1() const {
    boost::optional<double> value = getDouble(OS_Curve_RectangularHyperbola2Fields::Coefficient1C1,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveRectangularHyperbola2_Impl::coefficient2C2() const {
    boost::optional<double> value = getDouble(OS_Curve_RectangularHyperbola2Fields::Coefficient2C2,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveRectangularHyperbola2_Impl::coefficient3C3() const {
    boost::optional<double> value = getDouble(OS_Curve_RectangularHyperbola2Fields::Coefficient3C3,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveRectangularHyperbola2_Impl::minimumValueofx() const {
    boost::optional<double> value = getDouble(OS_Curve_RectangularHyperbola2Fields::MinimumValueofx,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CurveRectangularHyperbola2_Impl::maximumValueofx() const {
    boost::optional<double> value = getDouble(OS_Curve_RectangularHyperbola2Fields::MaximumValueofx,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> CurveRectangularHyperbola2_Impl::minimumCurveOutput() const {
    return getDouble(OS_Curve_RectangularHyperbola2Fields::MinimumCurveOutput,true);
  }

  boost::optional<double> CurveRectangularHyperbola2_Impl::maximumCurveOutput() const {
    return getDouble(OS_Curve_RectangularHyperbola2Fields::MaximumCurveOutput,true);
  }

  std::string CurveRectangularHyperbola2_Impl::inputUnitTypeforx() const {
    boost::optional<std::string> value = getString(OS_Curve_RectangularHyperbola2Fields::InputUnitTypeforx,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CurveRectangularHyperbola2_Impl::isInputUnitTypeforxDefaulted() const {
    return isEmpty(OS_Curve_RectangularHyperbola2Fields::InputUnitTypeforx);
  }

  std::string CurveRectangularHyperbola2_Impl::outputUnitType() const {
    boost::optional<std::string> value = getString(OS_Curve_RectangularHyperbola2Fields::OutputUnitType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CurveRectangularHyperbola2_Impl::isOutputUnitTypeDefaulted() const {
    return isEmpty(OS_Curve_RectangularHyperbola2Fields::OutputUnitType);
  }

  void CurveRectangularHyperbola2_Impl::setCoefficient1C1(double coefficient1C1) {
    bool result = setDouble(OS_Curve_RectangularHyperbola2Fields::Coefficient1C1, coefficient1C1);
    OS_ASSERT(result);
  }

  void CurveRectangularHyperbola2_Impl::setCoefficient2C2(double coefficient2C2) {
    bool result = setDouble(OS_Curve_RectangularHyperbola2Fields::Coefficient2C2, coefficient2C2);
    OS_ASSERT(result);
  }

  void CurveRectangularHyperbola2_Impl::setCoefficient3C3(double coefficient3C3) {
    bool result = setDouble(OS_Curve_RectangularHyperbola2Fields::Coefficient3C3, coefficient3C3);
    OS_ASSERT(result);
  }

  void CurveRectangularHyperbola2_Impl::setMinimumValueofx(double minimumValueofx) {
    bool result = setDouble(OS_Curve_RectangularHyperbola2Fields::MinimumValueofx, minimumValueofx);
    OS_ASSERT(result);
  }

  void CurveRectangularHyperbola2_Impl::setMaximumValueofx(double maximumValueofx) {
    bool result = setDouble(OS_Curve_RectangularHyperbola2Fields::MaximumValueofx, maximumValueofx);
    OS_ASSERT(result);
  }

  void CurveRectangularHyperbola2_Impl::setMinimumCurveOutput(boost::optional<double> minimumCurveOutput) {
    bool result = false;
    if (minimumCurveOutput) {
      result = setDouble(OS_Curve_RectangularHyperbola2Fields::MinimumCurveOutput, minimumCurveOutput.get());
    } else {
      result = setString(OS_Curve_RectangularHyperbola2Fields::MinimumCurveOutput, "");
    }
    OS_ASSERT(result);
  }

  void CurveRectangularHyperbola2_Impl::resetMinimumCurveOutput() {
    bool result = setString(OS_Curve_RectangularHyperbola2Fields::MinimumCurveOutput, "");
    OS_ASSERT(result);
  }

  void CurveRectangularHyperbola2_Impl::setMaximumCurveOutput(boost::optional<double> maximumCurveOutput) {
    bool result = false;
    if (maximumCurveOutput) {
      result = setDouble(OS_Curve_RectangularHyperbola2Fields::MaximumCurveOutput, maximumCurveOutput.get());
    } else {
      result = setString(OS_Curve_RectangularHyperbola2Fields::MaximumCurveOutput, "");
    }
    OS_ASSERT(result);
  }

  void CurveRectangularHyperbola2_Impl::resetMaximumCurveOutput() {
    bool result = setString(OS_Curve_RectangularHyperbola2Fields::MaximumCurveOutput, "");
    OS_ASSERT(result);
  }

  bool CurveRectangularHyperbola2_Impl::setInputUnitTypeforx(std::string inputUnitTypeforx) {
    bool result = setString(OS_Curve_RectangularHyperbola2Fields::InputUnitTypeforx, inputUnitTypeforx);
    return result;
  }

  void CurveRectangularHyperbola2_Impl::resetInputUnitTypeforx() {
    bool result = setString(OS_Curve_RectangularHyperbola2Fields::InputUnitTypeforx, "");
    OS_ASSERT(result);
  }

  bool CurveRectangularHyperbola2_Impl::setOutputUnitType(std::string outputUnitType) {
    bool result = setString(OS_Curve_RectangularHyperbola2Fields::OutputUnitType, outputUnitType);
    return result;
  }

  void CurveRectangularHyperbola2_Impl::resetOutputUnitType() {
    bool result = setString(OS_Curve_RectangularHyperbola2Fields::OutputUnitType, "");
    OS_ASSERT(result);
  }

} // detail

CurveRectangularHyperbola2::CurveRectangularHyperbola2(const Model& model)
  : Curve(CurveRectangularHyperbola2::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CurveRectangularHyperbola2_Impl>());
  setDouble(OS_Curve_RectangularHyperbola2Fields::Coefficient1C1,1.0);
  setDouble(OS_Curve_RectangularHyperbola2Fields::Coefficient2C2,1.0);
  setDouble(OS_Curve_RectangularHyperbola2Fields::Coefficient3C3,1.0);
  setDouble(OS_Curve_RectangularHyperbola2Fields::MinimumValueofx,0.0);
  setDouble(OS_Curve_RectangularHyperbola2Fields::MaximumValueofx,1.0);
}

IddObjectType CurveRectangularHyperbola2::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Curve_RectangularHyperbola2);
  return result;
}

std::vector<std::string> CurveRectangularHyperbola2::validInputUnitTypeforxValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Curve_RectangularHyperbola2Fields::InputUnitTypeforx);
}

std::vector<std::string> CurveRectangularHyperbola2::validOutputUnitTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Curve_RectangularHyperbola2Fields::OutputUnitType);
}

double CurveRectangularHyperbola2::coefficient1C1() const {
  return getImpl<detail::CurveRectangularHyperbola2_Impl>()->coefficient1C1();
}

double CurveRectangularHyperbola2::coefficient2C2() const {
  return getImpl<detail::CurveRectangularHyperbola2_Impl>()->coefficient2C2();
}

double CurveRectangularHyperbola2::coefficient3C3() const {
  return getImpl<detail::CurveRectangularHyperbola2_Impl>()->coefficient3C3();
}

double CurveRectangularHyperbola2::minimumValueofx() const {
  return getImpl<detail::CurveRectangularHyperbola2_Impl>()->minimumValueofx();
}

double CurveRectangularHyperbola2::maximumValueofx() const {
  return getImpl<detail::CurveRectangularHyperbola2_Impl>()->maximumValueofx();
}

boost::optional<double> CurveRectangularHyperbola2::minimumCurveOutput() const {
  return getImpl<detail::CurveRectangularHyperbola2_Impl>()->minimumCurveOutput();
}

boost::optional<double> CurveRectangularHyperbola2::maximumCurveOutput() const {
  return getImpl<detail::CurveRectangularHyperbola2_Impl>()->maximumCurveOutput();
}

std::string CurveRectangularHyperbola2::inputUnitTypeforx() const {
  return getImpl<detail::CurveRectangularHyperbola2_Impl>()->inputUnitTypeforx();
}

bool CurveRectangularHyperbola2::isInputUnitTypeforxDefaulted() const {
  return getImpl<detail::CurveRectangularHyperbola2_Impl>()->isInputUnitTypeforxDefaulted();
}

std::string CurveRectangularHyperbola2::outputUnitType() const {
  return getImpl<detail::CurveRectangularHyperbola2_Impl>()->outputUnitType();
}

bool CurveRectangularHyperbola2::isOutputUnitTypeDefaulted() const {
  return getImpl<detail::CurveRectangularHyperbola2_Impl>()->isOutputUnitTypeDefaulted();
}

void CurveRectangularHyperbola2::setCoefficient1C1(double coefficient1C1) {
  getImpl<detail::CurveRectangularHyperbola2_Impl>()->setCoefficient1C1(coefficient1C1);
}

void CurveRectangularHyperbola2::setCoefficient2C2(double coefficient2C2) {
  getImpl<detail::CurveRectangularHyperbola2_Impl>()->setCoefficient2C2(coefficient2C2);
}

void CurveRectangularHyperbola2::setCoefficient3C3(double coefficient3C3) {
  getImpl<detail::CurveRectangularHyperbola2_Impl>()->setCoefficient3C3(coefficient3C3);
}

void CurveRectangularHyperbola2::setMinimumValueofx(double minimumValueofx) {
  getImpl<detail::CurveRectangularHyperbola2_Impl>()->setMinimumValueofx(minimumValueofx);
}

void CurveRectangularHyperbola2::setMaximumValueofx(double maximumValueofx) {
  getImpl<detail::CurveRectangularHyperbola2_Impl>()->setMaximumValueofx(maximumValueofx);
}

void CurveRectangularHyperbola2::setMinimumCurveOutput(double minimumCurveOutput) {
  getImpl<detail::CurveRectangularHyperbola2_Impl>()->setMinimumCurveOutput(minimumCurveOutput);
}

void CurveRectangularHyperbola2::resetMinimumCurveOutput() {
  getImpl<detail::CurveRectangularHyperbola2_Impl>()->resetMinimumCurveOutput();
}

void CurveRectangularHyperbola2::setMaximumCurveOutput(double maximumCurveOutput) {
  getImpl<detail::CurveRectangularHyperbola2_Impl>()->setMaximumCurveOutput(maximumCurveOutput);
}

void CurveRectangularHyperbola2::resetMaximumCurveOutput() {
  getImpl<detail::CurveRectangularHyperbola2_Impl>()->resetMaximumCurveOutput();
}

bool CurveRectangularHyperbola2::setInputUnitTypeforx(std::string inputUnitTypeforx) {
  return getImpl<detail::CurveRectangularHyperbola2_Impl>()->setInputUnitTypeforx(inputUnitTypeforx);
}

void CurveRectangularHyperbola2::resetInputUnitTypeforx() {
  getImpl<detail::CurveRectangularHyperbola2_Impl>()->resetInputUnitTypeforx();
}

bool CurveRectangularHyperbola2::setOutputUnitType(std::string outputUnitType) {
  return getImpl<detail::CurveRectangularHyperbola2_Impl>()->setOutputUnitType(outputUnitType);
}

void CurveRectangularHyperbola2::resetOutputUnitType() {
  getImpl<detail::CurveRectangularHyperbola2_Impl>()->resetOutputUnitType();
}

/// @cond
CurveRectangularHyperbola2::CurveRectangularHyperbola2(boost::shared_ptr<detail::CurveRectangularHyperbola2_Impl> impl)
  : Curve(impl)
{}
/// @endcond

} // model
} // openstudio

