/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "CurveQuintLinear.hpp"
#include "CurveQuintLinear_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Curve_QuintLinear_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    CurveQuintLinear_Impl::CurveQuintLinear_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : Curve_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == CurveQuintLinear::iddObjectType());
    }

    CurveQuintLinear_Impl::CurveQuintLinear_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : Curve_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == CurveQuintLinear::iddObjectType());
    }

    CurveQuintLinear_Impl::CurveQuintLinear_Impl(const CurveQuintLinear_Impl& other, Model_Impl* model, bool keepHandle)
      : Curve_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& CurveQuintLinear_Impl::outputVariableNames() const {
      static std::vector<std::string> result;
      if (result.empty()) {
      }
      return result;
    }

    IddObjectType CurveQuintLinear_Impl::iddObjectType() const {
      return CurveQuintLinear::iddObjectType();
    }

    int CurveQuintLinear_Impl::numVariables() const {
      return 5;
    }

    double CurveQuintLinear_Impl::evaluate(const std::vector<double>& independentVariables) const {
      OS_ASSERT(independentVariables.size() == 5u);

      double v = independentVariables[0];
      if (v < minimumValueofv()) {
        LOG(Warn, "Supplied x is belov the minimumValueofv, resetting it.");
        v = minimumValueofv();
      }
      if (v > maximumValueofv()) {
        LOG(Warn, "Supplied v is above the maximumValueofv, resetting it.");
        v = maximumValueofv();
      }

      double w = independentVariables[1];
      if (w < minimumValueofw()) {
        LOG(Warn, "Supplied x is below the minimumValueofw, resetting it.");
        w = minimumValueofw();
      }
      if (w > maximumValueofw()) {
        LOG(Warn, "Supplied w is above the maximumValueofw, resetting it.");
        w = maximumValueofw();
      }

      double x = independentVariables[2];
      if (x < minimumValueofx()) {
        LOG(Warn, "Supplied x is below the minimumValueofx, resetting it.");
        x = minimumValueofx();
      }
      if (x > maximumValueofx()) {
        LOG(Warn, "Supplied x is above the maximumValueofx, resetting it.");
        x = maximumValueofx();
      }

      double y = independentVariables[3];
      if (y < minimumValueofy()) {
        LOG(Warn, "Supplied y is below the minimumValueofy, resetting it.");
        y = minimumValueofy();
      }
      if (y > maximumValueofy()) {
        LOG(Warn, "Supplied y is above the maximumValueofy, resetting it.");
        y = maximumValueofy();
      }

      double z = independentVariables[4];
      if (z < minimumValueofz()) {
        LOG(Warn, "Supplied z is below the minimumValueofz, resetting it.");
        z = minimumValueofz();
      }
      if (z > maximumValueofz()) {
        LOG(Warn, "Supplied z is above the maximumValueofz, resetting it.");
        z = maximumValueofz();
      }

      // C1 + C2*v + C3*w + C4*x + C5*y + C6*z
      double result = coefficient1Constant();
      result += coefficient2v() * v;
      result += coefficient3w() * w;
      result += coefficient4x() * x;
      result += coefficient5y() * y;
      result += coefficient6z() * z;

      if (boost::optional<double> _minVal = minimumCurveOutput()) {
        double minVal = _minVal.get();
        if (result < minVal) {
          LOG(Warn, "Calculated curve output is below minimumCurveOutput, resetting it.");
          result = minVal;
        }
      }

      if (boost::optional<double> _maxVal = maximumCurveOutput()) {
        double maxVal = _maxVal.get();
        if (result > maxVal) {
          LOG(Warn, "Calculated curve output is above maximumCurveOutput, resetting it.");
          result = maxVal;
        }
      }

      return result;
    }

    double CurveQuintLinear_Impl::coefficient1Constant() const {
      boost::optional<double> value = getDouble(OS_Curve_QuintLinearFields::Coefficient1Constant, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CurveQuintLinear_Impl::coefficient2v() const {
      boost::optional<double> value = getDouble(OS_Curve_QuintLinearFields::Coefficient2v, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CurveQuintLinear_Impl::coefficient3w() const {
      boost::optional<double> value = getDouble(OS_Curve_QuintLinearFields::Coefficient3w, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CurveQuintLinear_Impl::coefficient4x() const {
      boost::optional<double> value = getDouble(OS_Curve_QuintLinearFields::Coefficient4x, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CurveQuintLinear_Impl::coefficient5y() const {
      boost::optional<double> value = getDouble(OS_Curve_QuintLinearFields::Coefficient5y, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CurveQuintLinear_Impl::coefficient6z() const {
      boost::optional<double> value = getDouble(OS_Curve_QuintLinearFields::Coefficient6z, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CurveQuintLinear_Impl::minimumValueofv() const {
      boost::optional<double> value = getDouble(OS_Curve_QuintLinearFields::MinimumValueofv, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CurveQuintLinear_Impl::maximumValueofv() const {
      boost::optional<double> value = getDouble(OS_Curve_QuintLinearFields::MaximumValueofv, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CurveQuintLinear_Impl::minimumValueofw() const {
      boost::optional<double> value = getDouble(OS_Curve_QuintLinearFields::MinimumValueofw, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CurveQuintLinear_Impl::maximumValueofw() const {
      boost::optional<double> value = getDouble(OS_Curve_QuintLinearFields::MaximumValueofw, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CurveQuintLinear_Impl::minimumValueofx() const {
      boost::optional<double> value = getDouble(OS_Curve_QuintLinearFields::MinimumValueofx, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CurveQuintLinear_Impl::maximumValueofx() const {
      boost::optional<double> value = getDouble(OS_Curve_QuintLinearFields::MaximumValueofx, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CurveQuintLinear_Impl::minimumValueofy() const {
      boost::optional<double> value = getDouble(OS_Curve_QuintLinearFields::MinimumValueofy, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CurveQuintLinear_Impl::maximumValueofy() const {
      boost::optional<double> value = getDouble(OS_Curve_QuintLinearFields::MaximumValueofy, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CurveQuintLinear_Impl::minimumValueofz() const {
      boost::optional<double> value = getDouble(OS_Curve_QuintLinearFields::MinimumValueofz, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CurveQuintLinear_Impl::maximumValueofz() const {
      boost::optional<double> value = getDouble(OS_Curve_QuintLinearFields::MaximumValueofz, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> CurveQuintLinear_Impl::minimumCurveOutput() const {
      return getDouble(OS_Curve_QuintLinearFields::MinimumCurveOutput, true);
    }

    boost::optional<double> CurveQuintLinear_Impl::maximumCurveOutput() const {
      return getDouble(OS_Curve_QuintLinearFields::MaximumCurveOutput, true);
    }

    std::string CurveQuintLinear_Impl::inputUnitTypeforv() const {
      boost::optional<std::string> value = getString(OS_Curve_QuintLinearFields::InputUnitTypeforv, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CurveQuintLinear_Impl::isInputUnitTypeforvDefaulted() const {
      return isEmpty(OS_Curve_QuintLinearFields::InputUnitTypeforv);
    }

    std::string CurveQuintLinear_Impl::inputUnitTypeforw() const {
      boost::optional<std::string> value = getString(OS_Curve_QuintLinearFields::InputUnitTypeforw, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CurveQuintLinear_Impl::isInputUnitTypeforwDefaulted() const {
      return isEmpty(OS_Curve_QuintLinearFields::InputUnitTypeforw);
    }

    std::string CurveQuintLinear_Impl::inputUnitTypeforx() const {
      boost::optional<std::string> value = getString(OS_Curve_QuintLinearFields::InputUnitTypeforx, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CurveQuintLinear_Impl::isInputUnitTypeforxDefaulted() const {
      return isEmpty(OS_Curve_QuintLinearFields::InputUnitTypeforx);
    }

    std::string CurveQuintLinear_Impl::inputUnitTypefory() const {
      boost::optional<std::string> value = getString(OS_Curve_QuintLinearFields::InputUnitTypefory, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CurveQuintLinear_Impl::isInputUnitTypeforyDefaulted() const {
      return isEmpty(OS_Curve_QuintLinearFields::InputUnitTypefory);
    }

    std::string CurveQuintLinear_Impl::inputUnitTypeforz() const {
      boost::optional<std::string> value = getString(OS_Curve_QuintLinearFields::InputUnitTypeforz, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CurveQuintLinear_Impl::isInputUnitTypeforzDefaulted() const {
      return isEmpty(OS_Curve_QuintLinearFields::InputUnitTypeforz);
    }

    bool CurveQuintLinear_Impl::setCoefficient1Constant(double coefficient1Constant) {
      bool result = setDouble(OS_Curve_QuintLinearFields::Coefficient1Constant, coefficient1Constant);
      OS_ASSERT(result);
      return result;
    }

    bool CurveQuintLinear_Impl::setCoefficient2v(double coefficient2v) {
      bool result = setDouble(OS_Curve_QuintLinearFields::Coefficient2v, coefficient2v);
      OS_ASSERT(result);
      return result;
    }

    bool CurveQuintLinear_Impl::setCoefficient3w(double coefficient3w) {
      bool result = setDouble(OS_Curve_QuintLinearFields::Coefficient3w, coefficient3w);
      OS_ASSERT(result);
      return result;
    }

    bool CurveQuintLinear_Impl::setCoefficient4x(double coefficient4x) {
      bool result = setDouble(OS_Curve_QuintLinearFields::Coefficient4x, coefficient4x);
      OS_ASSERT(result);
      return result;
    }

    bool CurveQuintLinear_Impl::setCoefficient5y(double coefficient5y) {
      bool result = setDouble(OS_Curve_QuintLinearFields::Coefficient5y, coefficient5y);
      OS_ASSERT(result);
      return result;
    }

    bool CurveQuintLinear_Impl::setCoefficient6z(double coefficient6z) {
      bool result = setDouble(OS_Curve_QuintLinearFields::Coefficient6z, coefficient6z);
      OS_ASSERT(result);
      return result;
    }

    bool CurveQuintLinear_Impl::setMinimumValueofv(double minimumValueofv) {
      bool result = setDouble(OS_Curve_QuintLinearFields::MinimumValueofv, minimumValueofv);
      OS_ASSERT(result);
      return result;
    }

    bool CurveQuintLinear_Impl::setMaximumValueofv(double maximumValueofv) {
      bool result = setDouble(OS_Curve_QuintLinearFields::MaximumValueofv, maximumValueofv);
      OS_ASSERT(result);
      return result;
    }

    bool CurveQuintLinear_Impl::setMinimumValueofw(double minimumValueofw) {
      bool result = setDouble(OS_Curve_QuintLinearFields::MinimumValueofw, minimumValueofw);
      OS_ASSERT(result);
      return result;
    }

    bool CurveQuintLinear_Impl::setMaximumValueofw(double maximumValueofw) {
      bool result = setDouble(OS_Curve_QuintLinearFields::MaximumValueofw, maximumValueofw);
      OS_ASSERT(result);
      return result;
    }

    bool CurveQuintLinear_Impl::setMinimumValueofx(double minimumValueofx) {
      bool result = setDouble(OS_Curve_QuintLinearFields::MinimumValueofx, minimumValueofx);
      OS_ASSERT(result);
      return result;
    }

    bool CurveQuintLinear_Impl::setMaximumValueofx(double maximumValueofx) {
      bool result = setDouble(OS_Curve_QuintLinearFields::MaximumValueofx, maximumValueofx);
      OS_ASSERT(result);
      return result;
    }

    bool CurveQuintLinear_Impl::setMinimumValueofy(double minimumValueofy) {
      bool result = setDouble(OS_Curve_QuintLinearFields::MinimumValueofy, minimumValueofy);
      OS_ASSERT(result);
      return result;
    }

    bool CurveQuintLinear_Impl::setMaximumValueofy(double maximumValueofy) {
      bool result = setDouble(OS_Curve_QuintLinearFields::MaximumValueofy, maximumValueofy);
      OS_ASSERT(result);
      return result;
    }

    bool CurveQuintLinear_Impl::setMinimumValueofz(double minimumValueofz) {
      bool result = setDouble(OS_Curve_QuintLinearFields::MinimumValueofz, minimumValueofz);
      OS_ASSERT(result);
      return result;
    }

    bool CurveQuintLinear_Impl::setMaximumValueofz(double maximumValueofz) {
      bool result = setDouble(OS_Curve_QuintLinearFields::MaximumValueofz, maximumValueofz);
      OS_ASSERT(result);
      return result;
    }

    bool CurveQuintLinear_Impl::setMinimumCurveOutput(double minimumCurveOutput) {
      bool result = setDouble(OS_Curve_QuintLinearFields::MinimumCurveOutput, minimumCurveOutput);
      OS_ASSERT(result);
      return result;
    }

    void CurveQuintLinear_Impl::resetMinimumCurveOutput() {
      bool result = setString(OS_Curve_QuintLinearFields::MinimumCurveOutput, "");
      OS_ASSERT(result);
    }

    bool CurveQuintLinear_Impl::setMaximumCurveOutput(double maximumCurveOutput) {
      bool result = setDouble(OS_Curve_QuintLinearFields::MaximumCurveOutput, maximumCurveOutput);
      OS_ASSERT(result);
      return result;
    }

    void CurveQuintLinear_Impl::resetMaximumCurveOutput() {
      bool result = setString(OS_Curve_QuintLinearFields::MaximumCurveOutput, "");
      OS_ASSERT(result);
    }

    bool CurveQuintLinear_Impl::setInputUnitTypeforv(const std::string& inputUnitTypeforv) {
      bool result = setString(OS_Curve_QuintLinearFields::InputUnitTypeforv, inputUnitTypeforv);
      return result;
    }

    void CurveQuintLinear_Impl::resetInputUnitTypeforv() {
      bool result = setString(OS_Curve_QuintLinearFields::InputUnitTypeforv, "");
      OS_ASSERT(result);
    }

    bool CurveQuintLinear_Impl::setInputUnitTypeforw(const std::string& inputUnitTypeforw) {
      bool result = setString(OS_Curve_QuintLinearFields::InputUnitTypeforw, inputUnitTypeforw);
      return result;
    }

    void CurveQuintLinear_Impl::resetInputUnitTypeforw() {
      bool result = setString(OS_Curve_QuintLinearFields::InputUnitTypeforw, "");
      OS_ASSERT(result);
    }

    bool CurveQuintLinear_Impl::setInputUnitTypeforx(const std::string& inputUnitTypeforx) {
      bool result = setString(OS_Curve_QuintLinearFields::InputUnitTypeforx, inputUnitTypeforx);
      return result;
    }

    void CurveQuintLinear_Impl::resetInputUnitTypeforx() {
      bool result = setString(OS_Curve_QuintLinearFields::InputUnitTypeforx, "");
      OS_ASSERT(result);
    }

    bool CurveQuintLinear_Impl::setInputUnitTypefory(const std::string& inputUnitTypefory) {
      bool result = setString(OS_Curve_QuintLinearFields::InputUnitTypefory, inputUnitTypefory);
      return result;
    }

    void CurveQuintLinear_Impl::resetInputUnitTypefory() {
      bool result = setString(OS_Curve_QuintLinearFields::InputUnitTypefory, "");
      OS_ASSERT(result);
    }

    bool CurveQuintLinear_Impl::setInputUnitTypeforz(const std::string& inputUnitTypeforz) {
      bool result = setString(OS_Curve_QuintLinearFields::InputUnitTypeforz, inputUnitTypeforz);
      return result;
    }

    void CurveQuintLinear_Impl::resetInputUnitTypeforz() {
      bool result = setString(OS_Curve_QuintLinearFields::InputUnitTypeforz, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  CurveQuintLinear::CurveQuintLinear(const Model& model) : Curve(CurveQuintLinear::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CurveQuintLinear_Impl>());

    setCoefficient1Constant(0.0);
    setCoefficient2v(1.0);
    setCoefficient3w(1.0);
    setCoefficient4x(1.0);
    setCoefficient5y(1.0);
    setCoefficient6z(1.0);
    setMinimumValueofv(-100.0);
    setMaximumValueofv(100.0);
    setMinimumValueofw(-100.0);
    setMaximumValueofw(100.0);
    setMinimumValueofx(-100.0);
    setMaximumValueofx(100.0);
    setMinimumValueofy(0.0);
    setMaximumValueofy(100.0);
    setMinimumValueofz(0.0);
    setMaximumValueofz(100.0);
  }

  IddObjectType CurveQuintLinear::iddObjectType() {
    return {IddObjectType::OS_Curve_QuintLinear};
  }

  std::vector<std::string> CurveQuintLinear::inputUnitTypeforvValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Curve_QuintLinearFields::InputUnitTypeforv);
  }

  std::vector<std::string> CurveQuintLinear::inputUnitTypeforwValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Curve_QuintLinearFields::InputUnitTypeforw);
  }

  std::vector<std::string> CurveQuintLinear::inputUnitTypeforxValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Curve_QuintLinearFields::InputUnitTypeforx);
  }

  std::vector<std::string> CurveQuintLinear::inputUnitTypeforyValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Curve_QuintLinearFields::InputUnitTypefory);
  }

  std::vector<std::string> CurveQuintLinear::inputUnitTypeforzValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Curve_QuintLinearFields::InputUnitTypeforz);
  }

  double CurveQuintLinear::coefficient1Constant() const {
    return getImpl<detail::CurveQuintLinear_Impl>()->coefficient1Constant();
  }

  double CurveQuintLinear::coefficient2v() const {
    return getImpl<detail::CurveQuintLinear_Impl>()->coefficient2v();
  }

  double CurveQuintLinear::coefficient3w() const {
    return getImpl<detail::CurveQuintLinear_Impl>()->coefficient3w();
  }

  double CurveQuintLinear::coefficient4x() const {
    return getImpl<detail::CurveQuintLinear_Impl>()->coefficient4x();
  }

  double CurveQuintLinear::coefficient5y() const {
    return getImpl<detail::CurveQuintLinear_Impl>()->coefficient5y();
  }

  double CurveQuintLinear::coefficient6z() const {
    return getImpl<detail::CurveQuintLinear_Impl>()->coefficient6z();
  }

  double CurveQuintLinear::minimumValueofv() const {
    return getImpl<detail::CurveQuintLinear_Impl>()->minimumValueofv();
  }

  double CurveQuintLinear::maximumValueofv() const {
    return getImpl<detail::CurveQuintLinear_Impl>()->maximumValueofv();
  }

  double CurveQuintLinear::minimumValueofw() const {
    return getImpl<detail::CurveQuintLinear_Impl>()->minimumValueofw();
  }

  double CurveQuintLinear::maximumValueofw() const {
    return getImpl<detail::CurveQuintLinear_Impl>()->maximumValueofw();
  }

  double CurveQuintLinear::minimumValueofx() const {
    return getImpl<detail::CurveQuintLinear_Impl>()->minimumValueofx();
  }

  double CurveQuintLinear::maximumValueofx() const {
    return getImpl<detail::CurveQuintLinear_Impl>()->maximumValueofx();
  }

  double CurveQuintLinear::minimumValueofy() const {
    return getImpl<detail::CurveQuintLinear_Impl>()->minimumValueofy();
  }

  double CurveQuintLinear::maximumValueofy() const {
    return getImpl<detail::CurveQuintLinear_Impl>()->maximumValueofy();
  }

  double CurveQuintLinear::minimumValueofz() const {
    return getImpl<detail::CurveQuintLinear_Impl>()->minimumValueofz();
  }

  double CurveQuintLinear::maximumValueofz() const {
    return getImpl<detail::CurveQuintLinear_Impl>()->maximumValueofz();
  }

  boost::optional<double> CurveQuintLinear::minimumCurveOutput() const {
    return getImpl<detail::CurveQuintLinear_Impl>()->minimumCurveOutput();
  }

  boost::optional<double> CurveQuintLinear::maximumCurveOutput() const {
    return getImpl<detail::CurveQuintLinear_Impl>()->maximumCurveOutput();
  }

  std::string CurveQuintLinear::inputUnitTypeforv() const {
    return getImpl<detail::CurveQuintLinear_Impl>()->inputUnitTypeforv();
  }

  bool CurveQuintLinear::isInputUnitTypeforvDefaulted() const {
    return getImpl<detail::CurveQuintLinear_Impl>()->isInputUnitTypeforvDefaulted();
  }

  std::string CurveQuintLinear::inputUnitTypeforw() const {
    return getImpl<detail::CurveQuintLinear_Impl>()->inputUnitTypeforw();
  }

  bool CurveQuintLinear::isInputUnitTypeforwDefaulted() const {
    return getImpl<detail::CurveQuintLinear_Impl>()->isInputUnitTypeforwDefaulted();
  }

  std::string CurveQuintLinear::inputUnitTypeforx() const {
    return getImpl<detail::CurveQuintLinear_Impl>()->inputUnitTypeforx();
  }

  bool CurveQuintLinear::isInputUnitTypeforxDefaulted() const {
    return getImpl<detail::CurveQuintLinear_Impl>()->isInputUnitTypeforxDefaulted();
  }

  std::string CurveQuintLinear::inputUnitTypefory() const {
    return getImpl<detail::CurveQuintLinear_Impl>()->inputUnitTypefory();
  }

  bool CurveQuintLinear::isInputUnitTypeforyDefaulted() const {
    return getImpl<detail::CurveQuintLinear_Impl>()->isInputUnitTypeforyDefaulted();
  }

  std::string CurveQuintLinear::inputUnitTypeforz() const {
    return getImpl<detail::CurveQuintLinear_Impl>()->inputUnitTypeforz();
  }

  bool CurveQuintLinear::isInputUnitTypeforzDefaulted() const {
    return getImpl<detail::CurveQuintLinear_Impl>()->isInputUnitTypeforzDefaulted();
  }

  bool CurveQuintLinear::setCoefficient1Constant(double coefficient1Constant) {
    return getImpl<detail::CurveQuintLinear_Impl>()->setCoefficient1Constant(coefficient1Constant);
  }

  bool CurveQuintLinear::setCoefficient2v(double coefficient2v) {
    return getImpl<detail::CurveQuintLinear_Impl>()->setCoefficient2v(coefficient2v);
  }

  bool CurveQuintLinear::setCoefficient3w(double coefficient3w) {
    return getImpl<detail::CurveQuintLinear_Impl>()->setCoefficient3w(coefficient3w);
  }

  bool CurveQuintLinear::setCoefficient4x(double coefficient4x) {
    return getImpl<detail::CurveQuintLinear_Impl>()->setCoefficient4x(coefficient4x);
  }

  bool CurveQuintLinear::setCoefficient5y(double coefficient5y) {
    return getImpl<detail::CurveQuintLinear_Impl>()->setCoefficient5y(coefficient5y);
  }

  bool CurveQuintLinear::setCoefficient6z(double coefficient6z) {
    return getImpl<detail::CurveQuintLinear_Impl>()->setCoefficient6z(coefficient6z);
  }

  bool CurveQuintLinear::setMinimumValueofv(double minimumValueofv) {
    return getImpl<detail::CurveQuintLinear_Impl>()->setMinimumValueofv(minimumValueofv);
  }

  bool CurveQuintLinear::setMaximumValueofv(double maximumValueofv) {
    return getImpl<detail::CurveQuintLinear_Impl>()->setMaximumValueofv(maximumValueofv);
  }

  bool CurveQuintLinear::setMinimumValueofw(double minimumValueofw) {
    return getImpl<detail::CurveQuintLinear_Impl>()->setMinimumValueofw(minimumValueofw);
  }

  bool CurveQuintLinear::setMaximumValueofw(double maximumValueofw) {
    return getImpl<detail::CurveQuintLinear_Impl>()->setMaximumValueofw(maximumValueofw);
  }

  bool CurveQuintLinear::setMinimumValueofx(double minimumValueofx) {
    return getImpl<detail::CurveQuintLinear_Impl>()->setMinimumValueofx(minimumValueofx);
  }

  bool CurveQuintLinear::setMaximumValueofx(double maximumValueofx) {
    return getImpl<detail::CurveQuintLinear_Impl>()->setMaximumValueofx(maximumValueofx);
  }

  bool CurveQuintLinear::setMinimumValueofy(double minimumValueofy) {
    return getImpl<detail::CurveQuintLinear_Impl>()->setMinimumValueofy(minimumValueofy);
  }

  bool CurveQuintLinear::setMaximumValueofy(double maximumValueofy) {
    return getImpl<detail::CurveQuintLinear_Impl>()->setMaximumValueofy(maximumValueofy);
  }

  bool CurveQuintLinear::setMinimumValueofz(double minimumValueofz) {
    return getImpl<detail::CurveQuintLinear_Impl>()->setMinimumValueofz(minimumValueofz);
  }

  bool CurveQuintLinear::setMaximumValueofz(double maximumValueofz) {
    return getImpl<detail::CurveQuintLinear_Impl>()->setMaximumValueofz(maximumValueofz);
  }

  bool CurveQuintLinear::setMinimumCurveOutput(double minimumCurveOutput) {
    return getImpl<detail::CurveQuintLinear_Impl>()->setMinimumCurveOutput(minimumCurveOutput);
  }

  void CurveQuintLinear::resetMinimumCurveOutput() {
    getImpl<detail::CurveQuintLinear_Impl>()->resetMinimumCurveOutput();
  }

  bool CurveQuintLinear::setMaximumCurveOutput(double maximumCurveOutput) {
    return getImpl<detail::CurveQuintLinear_Impl>()->setMaximumCurveOutput(maximumCurveOutput);
  }

  void CurveQuintLinear::resetMaximumCurveOutput() {
    getImpl<detail::CurveQuintLinear_Impl>()->resetMaximumCurveOutput();
  }

  bool CurveQuintLinear::setInputUnitTypeforv(const std::string& inputUnitTypeforv) {
    return getImpl<detail::CurveQuintLinear_Impl>()->setInputUnitTypeforv(inputUnitTypeforv);
  }

  void CurveQuintLinear::resetInputUnitTypeforv() {
    getImpl<detail::CurveQuintLinear_Impl>()->resetInputUnitTypeforv();
  }

  bool CurveQuintLinear::setInputUnitTypeforw(const std::string& inputUnitTypeforw) {
    return getImpl<detail::CurveQuintLinear_Impl>()->setInputUnitTypeforw(inputUnitTypeforw);
  }

  void CurveQuintLinear::resetInputUnitTypeforw() {
    getImpl<detail::CurveQuintLinear_Impl>()->resetInputUnitTypeforw();
  }

  bool CurveQuintLinear::setInputUnitTypeforx(const std::string& inputUnitTypeforx) {
    return getImpl<detail::CurveQuintLinear_Impl>()->setInputUnitTypeforx(inputUnitTypeforx);
  }

  void CurveQuintLinear::resetInputUnitTypeforx() {
    getImpl<detail::CurveQuintLinear_Impl>()->resetInputUnitTypeforx();
  }

  bool CurveQuintLinear::setInputUnitTypefory(const std::string& inputUnitTypefory) {
    return getImpl<detail::CurveQuintLinear_Impl>()->setInputUnitTypefory(inputUnitTypefory);
  }

  void CurveQuintLinear::resetInputUnitTypefory() {
    getImpl<detail::CurveQuintLinear_Impl>()->resetInputUnitTypefory();
  }

  bool CurveQuintLinear::setInputUnitTypeforz(const std::string& inputUnitTypeforz) {
    return getImpl<detail::CurveQuintLinear_Impl>()->setInputUnitTypeforz(inputUnitTypeforz);
  }

  void CurveQuintLinear::resetInputUnitTypeforz() {
    getImpl<detail::CurveQuintLinear_Impl>()->resetInputUnitTypeforz();
  }

  /// @cond
  CurveQuintLinear::CurveQuintLinear(std::shared_ptr<detail::CurveQuintLinear_Impl> impl) : Curve(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
