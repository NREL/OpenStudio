/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "CurveQuadLinear.hpp"
#include "CurveQuadLinear_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Curve_QuadLinear_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    CurveQuadLinear_Impl::CurveQuadLinear_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : Curve_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == CurveQuadLinear::iddObjectType());
    }

    CurveQuadLinear_Impl::CurveQuadLinear_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : Curve_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == CurveQuadLinear::iddObjectType());
    }

    CurveQuadLinear_Impl::CurveQuadLinear_Impl(const CurveQuadLinear_Impl& other, Model_Impl* model, bool keepHandle)
      : Curve_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& CurveQuadLinear_Impl::outputVariableNames() const {
      static std::vector<std::string> result;
      if (result.empty()) {
      }
      return result;
    }

    IddObjectType CurveQuadLinear_Impl::iddObjectType() const {
      return CurveQuadLinear::iddObjectType();
    }

    int CurveQuadLinear_Impl::numVariables() const {
      return 4;
    }

    double CurveQuadLinear_Impl::evaluate(const std::vector<double>& independentVariables) const {
      OS_ASSERT(independentVariables.size() == 4u);

      double w = independentVariables[0];
      if (w < minimumValueofw()) {
        LOG(Warn, "Supplied w is below the minimumValueofw, resetting it.");
        w = minimumValueofw();
      }
      if (w > maximumValueofw()) {
        LOG(Warn, "Supplied w is above the maximumValueofw, resetting it.");
        w = maximumValueofw();
      }

      double x = independentVariables[1];
      if (x < minimumValueofx()) {
        LOG(Warn, "Supplied x is below the minimumValueofx, resetting it.");
        x = minimumValueofx();
      }
      if (x > maximumValueofx()) {
        LOG(Warn, "Supplied x is above the maximumValueofx, resetting it.");
        x = maximumValueofx();
      }

      double y = independentVariables[2];
      if (y < minimumValueofy()) {
        LOG(Warn, "Supplied y is below the minimumValueofy, resetting it.");
        y = minimumValueofy();
      }
      if (y > maximumValueofy()) {
        LOG(Warn, "Supplied y is above the maximumValueofy, resetting it.");
        y = maximumValueofy();
      }

      double z = independentVariables[3];
      if (z < minimumValueofz()) {
        LOG(Warn, "Supplied z is below the minimumValueofz, resetting it.");
        z = minimumValueofz();
      }
      if (z > maximumValueofz()) {
        LOG(Warn, "Supplied z is above the maximumValueofz, resetting it.");
        z = maximumValueofz();
      }

      // C1 + C2*w + C3*x + C4*y + C5*z
      double result = coefficient1Constant();
      result += coefficient2w() * w;
      result += coefficient3x() * x;
      result += coefficient4y() * y;
      result += coefficient5z() * z;

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

    double CurveQuadLinear_Impl::coefficient1Constant() const {
      boost::optional<double> value = getDouble(OS_Curve_QuadLinearFields::Coefficient1Constant, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CurveQuadLinear_Impl::coefficient2w() const {
      boost::optional<double> value = getDouble(OS_Curve_QuadLinearFields::Coefficient2w, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CurveQuadLinear_Impl::coefficient3x() const {
      boost::optional<double> value = getDouble(OS_Curve_QuadLinearFields::Coefficient3x, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CurveQuadLinear_Impl::coefficient4y() const {
      boost::optional<double> value = getDouble(OS_Curve_QuadLinearFields::Coefficient4y, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CurveQuadLinear_Impl::coefficient5z() const {
      boost::optional<double> value = getDouble(OS_Curve_QuadLinearFields::Coefficient5z, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CurveQuadLinear_Impl::minimumValueofw() const {
      boost::optional<double> value = getDouble(OS_Curve_QuadLinearFields::MinimumValueofw, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CurveQuadLinear_Impl::maximumValueofw() const {
      boost::optional<double> value = getDouble(OS_Curve_QuadLinearFields::MaximumValueofw, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CurveQuadLinear_Impl::minimumValueofx() const {
      boost::optional<double> value = getDouble(OS_Curve_QuadLinearFields::MinimumValueofx, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CurveQuadLinear_Impl::maximumValueofx() const {
      boost::optional<double> value = getDouble(OS_Curve_QuadLinearFields::MaximumValueofx, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CurveQuadLinear_Impl::minimumValueofy() const {
      boost::optional<double> value = getDouble(OS_Curve_QuadLinearFields::MinimumValueofy, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CurveQuadLinear_Impl::maximumValueofy() const {
      boost::optional<double> value = getDouble(OS_Curve_QuadLinearFields::MaximumValueofy, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CurveQuadLinear_Impl::minimumValueofz() const {
      boost::optional<double> value = getDouble(OS_Curve_QuadLinearFields::MinimumValueofz, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CurveQuadLinear_Impl::maximumValueofz() const {
      boost::optional<double> value = getDouble(OS_Curve_QuadLinearFields::MaximumValueofz, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> CurveQuadLinear_Impl::minimumCurveOutput() const {
      return getDouble(OS_Curve_QuadLinearFields::MinimumCurveOutput, true);
    }

    boost::optional<double> CurveQuadLinear_Impl::maximumCurveOutput() const {
      return getDouble(OS_Curve_QuadLinearFields::MaximumCurveOutput, true);
    }

    std::string CurveQuadLinear_Impl::inputUnitTypeforw() const {
      boost::optional<std::string> value = getString(OS_Curve_QuadLinearFields::InputUnitTypeforw, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CurveQuadLinear_Impl::isInputUnitTypeforwDefaulted() const {
      return isEmpty(OS_Curve_QuadLinearFields::InputUnitTypeforw);
    }

    std::string CurveQuadLinear_Impl::inputUnitTypeforx() const {
      boost::optional<std::string> value = getString(OS_Curve_QuadLinearFields::InputUnitTypeforx, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CurveQuadLinear_Impl::isInputUnitTypeforxDefaulted() const {
      return isEmpty(OS_Curve_QuadLinearFields::InputUnitTypeforx);
    }

    std::string CurveQuadLinear_Impl::inputUnitTypefory() const {
      boost::optional<std::string> value = getString(OS_Curve_QuadLinearFields::InputUnitTypefory, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CurveQuadLinear_Impl::isInputUnitTypeforyDefaulted() const {
      return isEmpty(OS_Curve_QuadLinearFields::InputUnitTypefory);
    }

    std::string CurveQuadLinear_Impl::inputUnitTypeforz() const {
      boost::optional<std::string> value = getString(OS_Curve_QuadLinearFields::InputUnitTypeforz, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CurveQuadLinear_Impl::isInputUnitTypeforzDefaulted() const {
      return isEmpty(OS_Curve_QuadLinearFields::InputUnitTypeforz);
    }

    bool CurveQuadLinear_Impl::setCoefficient1Constant(double coefficient1Constant) {
      bool result = setDouble(OS_Curve_QuadLinearFields::Coefficient1Constant, coefficient1Constant);
      OS_ASSERT(result);
      return result;
    }

    bool CurveQuadLinear_Impl::setCoefficient2w(double coefficient2w) {
      bool result = setDouble(OS_Curve_QuadLinearFields::Coefficient2w, coefficient2w);
      OS_ASSERT(result);
      return result;
    }

    bool CurveQuadLinear_Impl::setCoefficient3x(double coefficient3x) {
      bool result = setDouble(OS_Curve_QuadLinearFields::Coefficient3x, coefficient3x);
      OS_ASSERT(result);
      return result;
    }

    bool CurveQuadLinear_Impl::setCoefficient4y(double coefficient4y) {
      bool result = setDouble(OS_Curve_QuadLinearFields::Coefficient4y, coefficient4y);
      OS_ASSERT(result);
      return result;
    }

    bool CurveQuadLinear_Impl::setCoefficient5z(double coefficient5z) {
      bool result = setDouble(OS_Curve_QuadLinearFields::Coefficient5z, coefficient5z);
      OS_ASSERT(result);
      return result;
    }

    bool CurveQuadLinear_Impl::setMinimumValueofw(double minimumValueofw) {
      bool result = setDouble(OS_Curve_QuadLinearFields::MinimumValueofw, minimumValueofw);
      OS_ASSERT(result);
      return result;
    }

    bool CurveQuadLinear_Impl::setMaximumValueofw(double maximumValueofw) {
      bool result = setDouble(OS_Curve_QuadLinearFields::MaximumValueofw, maximumValueofw);
      OS_ASSERT(result);
      return result;
    }

    bool CurveQuadLinear_Impl::setMinimumValueofx(double minimumValueofx) {
      bool result = setDouble(OS_Curve_QuadLinearFields::MinimumValueofx, minimumValueofx);
      OS_ASSERT(result);
      return result;
    }

    bool CurveQuadLinear_Impl::setMaximumValueofx(double maximumValueofx) {
      bool result = setDouble(OS_Curve_QuadLinearFields::MaximumValueofx, maximumValueofx);
      OS_ASSERT(result);
      return result;
    }

    bool CurveQuadLinear_Impl::setMinimumValueofy(double minimumValueofy) {
      bool result = setDouble(OS_Curve_QuadLinearFields::MinimumValueofy, minimumValueofy);
      OS_ASSERT(result);
      return result;
    }

    bool CurveQuadLinear_Impl::setMaximumValueofy(double maximumValueofy) {
      bool result = setDouble(OS_Curve_QuadLinearFields::MaximumValueofy, maximumValueofy);
      OS_ASSERT(result);
      return result;
    }

    bool CurveQuadLinear_Impl::setMinimumValueofz(double minimumValueofz) {
      bool result = setDouble(OS_Curve_QuadLinearFields::MinimumValueofz, minimumValueofz);
      OS_ASSERT(result);
      return result;
    }

    bool CurveQuadLinear_Impl::setMaximumValueofz(double maximumValueofz) {
      bool result = setDouble(OS_Curve_QuadLinearFields::MaximumValueofz, maximumValueofz);
      OS_ASSERT(result);
      return result;
    }

    bool CurveQuadLinear_Impl::setMinimumCurveOutput(double minimumCurveOutput) {
      bool result = setDouble(OS_Curve_QuadLinearFields::MinimumCurveOutput, minimumCurveOutput);
      OS_ASSERT(result);
      return result;
    }

    void CurveQuadLinear_Impl::resetMinimumCurveOutput() {
      bool result = setString(OS_Curve_QuadLinearFields::MinimumCurveOutput, "");
      OS_ASSERT(result);
    }

    bool CurveQuadLinear_Impl::setMaximumCurveOutput(double maximumCurveOutput) {
      bool result = setDouble(OS_Curve_QuadLinearFields::MaximumCurveOutput, maximumCurveOutput);
      OS_ASSERT(result);
      return result;
    }

    void CurveQuadLinear_Impl::resetMaximumCurveOutput() {
      bool result = setString(OS_Curve_QuadLinearFields::MaximumCurveOutput, "");
      OS_ASSERT(result);
    }

    bool CurveQuadLinear_Impl::setInputUnitTypeforw(const std::string& inputUnitTypeforw) {
      bool result = setString(OS_Curve_QuadLinearFields::InputUnitTypeforw, inputUnitTypeforw);
      return result;
    }

    void CurveQuadLinear_Impl::resetInputUnitTypeforw() {
      bool result = setString(OS_Curve_QuadLinearFields::InputUnitTypeforw, "");
      OS_ASSERT(result);
    }

    bool CurveQuadLinear_Impl::setInputUnitTypeforx(const std::string& inputUnitTypeforx) {
      bool result = setString(OS_Curve_QuadLinearFields::InputUnitTypeforx, inputUnitTypeforx);
      return result;
    }

    void CurveQuadLinear_Impl::resetInputUnitTypeforx() {
      bool result = setString(OS_Curve_QuadLinearFields::InputUnitTypeforx, "");
      OS_ASSERT(result);
    }

    bool CurveQuadLinear_Impl::setInputUnitTypefory(const std::string& inputUnitTypefory) {
      bool result = setString(OS_Curve_QuadLinearFields::InputUnitTypefory, inputUnitTypefory);
      return result;
    }

    void CurveQuadLinear_Impl::resetInputUnitTypefory() {
      bool result = setString(OS_Curve_QuadLinearFields::InputUnitTypefory, "");
      OS_ASSERT(result);
    }

    bool CurveQuadLinear_Impl::setInputUnitTypeforz(const std::string& inputUnitTypeforz) {
      bool result = setString(OS_Curve_QuadLinearFields::InputUnitTypeforz, inputUnitTypeforz);
      return result;
    }

    void CurveQuadLinear_Impl::resetInputUnitTypeforz() {
      bool result = setString(OS_Curve_QuadLinearFields::InputUnitTypeforz, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  CurveQuadLinear::CurveQuadLinear(const Model& model) : Curve(CurveQuadLinear::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CurveQuadLinear_Impl>());

    setCoefficient1Constant(0.0);
    setCoefficient2w(1.0);
    setCoefficient3x(1.0);
    setCoefficient4y(1.0);
    setCoefficient5z(1.0);
    setMinimumValueofw(-100.0);
    setMaximumValueofw(100.0);
    setMinimumValueofx(-100.0);
    setMaximumValueofx(100.0);
    setMinimumValueofy(0.0);
    setMaximumValueofy(100.0);
    setMinimumValueofz(0.0);
    setMaximumValueofz(100.0);
  }

  IddObjectType CurveQuadLinear::iddObjectType() {
    return {IddObjectType::OS_Curve_QuadLinear};
  }

  std::vector<std::string> CurveQuadLinear::inputUnitTypeforwValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Curve_QuadLinearFields::InputUnitTypeforw);
  }

  std::vector<std::string> CurveQuadLinear::inputUnitTypeforxValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Curve_QuadLinearFields::InputUnitTypeforx);
  }

  std::vector<std::string> CurveQuadLinear::inputUnitTypeforyValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Curve_QuadLinearFields::InputUnitTypefory);
  }

  std::vector<std::string> CurveQuadLinear::inputUnitTypeforzValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Curve_QuadLinearFields::InputUnitTypeforz);
  }

  double CurveQuadLinear::coefficient1Constant() const {
    return getImpl<detail::CurveQuadLinear_Impl>()->coefficient1Constant();
  }

  double CurveQuadLinear::coefficient2w() const {
    return getImpl<detail::CurveQuadLinear_Impl>()->coefficient2w();
  }

  double CurveQuadLinear::coefficient3x() const {
    return getImpl<detail::CurveQuadLinear_Impl>()->coefficient3x();
  }

  double CurveQuadLinear::coefficient4y() const {
    return getImpl<detail::CurveQuadLinear_Impl>()->coefficient4y();
  }

  double CurveQuadLinear::coefficient5z() const {
    return getImpl<detail::CurveQuadLinear_Impl>()->coefficient5z();
  }

  double CurveQuadLinear::minimumValueofw() const {
    return getImpl<detail::CurveQuadLinear_Impl>()->minimumValueofw();
  }

  double CurveQuadLinear::maximumValueofw() const {
    return getImpl<detail::CurveQuadLinear_Impl>()->maximumValueofw();
  }

  double CurveQuadLinear::minimumValueofx() const {
    return getImpl<detail::CurveQuadLinear_Impl>()->minimumValueofx();
  }

  double CurveQuadLinear::maximumValueofx() const {
    return getImpl<detail::CurveQuadLinear_Impl>()->maximumValueofx();
  }

  double CurveQuadLinear::minimumValueofy() const {
    return getImpl<detail::CurveQuadLinear_Impl>()->minimumValueofy();
  }

  double CurveQuadLinear::maximumValueofy() const {
    return getImpl<detail::CurveQuadLinear_Impl>()->maximumValueofy();
  }

  double CurveQuadLinear::minimumValueofz() const {
    return getImpl<detail::CurveQuadLinear_Impl>()->minimumValueofz();
  }

  double CurveQuadLinear::maximumValueofz() const {
    return getImpl<detail::CurveQuadLinear_Impl>()->maximumValueofz();
  }

  boost::optional<double> CurveQuadLinear::minimumCurveOutput() const {
    return getImpl<detail::CurveQuadLinear_Impl>()->minimumCurveOutput();
  }

  boost::optional<double> CurveQuadLinear::maximumCurveOutput() const {
    return getImpl<detail::CurveQuadLinear_Impl>()->maximumCurveOutput();
  }

  std::string CurveQuadLinear::inputUnitTypeforw() const {
    return getImpl<detail::CurveQuadLinear_Impl>()->inputUnitTypeforw();
  }

  bool CurveQuadLinear::isInputUnitTypeforwDefaulted() const {
    return getImpl<detail::CurveQuadLinear_Impl>()->isInputUnitTypeforwDefaulted();
  }

  std::string CurveQuadLinear::inputUnitTypeforx() const {
    return getImpl<detail::CurveQuadLinear_Impl>()->inputUnitTypeforx();
  }

  bool CurveQuadLinear::isInputUnitTypeforxDefaulted() const {
    return getImpl<detail::CurveQuadLinear_Impl>()->isInputUnitTypeforxDefaulted();
  }

  std::string CurveQuadLinear::inputUnitTypefory() const {
    return getImpl<detail::CurveQuadLinear_Impl>()->inputUnitTypefory();
  }

  bool CurveQuadLinear::isInputUnitTypeforyDefaulted() const {
    return getImpl<detail::CurveQuadLinear_Impl>()->isInputUnitTypeforyDefaulted();
  }

  std::string CurveQuadLinear::inputUnitTypeforz() const {
    return getImpl<detail::CurveQuadLinear_Impl>()->inputUnitTypeforz();
  }

  bool CurveQuadLinear::isInputUnitTypeforzDefaulted() const {
    return getImpl<detail::CurveQuadLinear_Impl>()->isInputUnitTypeforzDefaulted();
  }

  bool CurveQuadLinear::setCoefficient1Constant(double coefficient1Constant) {
    return getImpl<detail::CurveQuadLinear_Impl>()->setCoefficient1Constant(coefficient1Constant);
  }

  bool CurveQuadLinear::setCoefficient2w(double coefficient2w) {
    return getImpl<detail::CurveQuadLinear_Impl>()->setCoefficient2w(coefficient2w);
  }

  bool CurveQuadLinear::setCoefficient3x(double coefficient3x) {
    return getImpl<detail::CurveQuadLinear_Impl>()->setCoefficient3x(coefficient3x);
  }

  bool CurveQuadLinear::setCoefficient4y(double coefficient4y) {
    return getImpl<detail::CurveQuadLinear_Impl>()->setCoefficient4y(coefficient4y);
  }

  bool CurveQuadLinear::setCoefficient5z(double coefficient5z) {
    return getImpl<detail::CurveQuadLinear_Impl>()->setCoefficient5z(coefficient5z);
  }

  bool CurveQuadLinear::setMinimumValueofw(double minimumValueofw) {
    return getImpl<detail::CurveQuadLinear_Impl>()->setMinimumValueofw(minimumValueofw);
  }

  bool CurveQuadLinear::setMaximumValueofw(double maximumValueofw) {
    return getImpl<detail::CurveQuadLinear_Impl>()->setMaximumValueofw(maximumValueofw);
  }

  bool CurveQuadLinear::setMinimumValueofx(double minimumValueofx) {
    return getImpl<detail::CurveQuadLinear_Impl>()->setMinimumValueofx(minimumValueofx);
  }

  bool CurveQuadLinear::setMaximumValueofx(double maximumValueofx) {
    return getImpl<detail::CurveQuadLinear_Impl>()->setMaximumValueofx(maximumValueofx);
  }

  bool CurveQuadLinear::setMinimumValueofy(double minimumValueofy) {
    return getImpl<detail::CurveQuadLinear_Impl>()->setMinimumValueofy(minimumValueofy);
  }

  bool CurveQuadLinear::setMaximumValueofy(double maximumValueofy) {
    return getImpl<detail::CurveQuadLinear_Impl>()->setMaximumValueofy(maximumValueofy);
  }

  bool CurveQuadLinear::setMinimumValueofz(double minimumValueofz) {
    return getImpl<detail::CurveQuadLinear_Impl>()->setMinimumValueofz(minimumValueofz);
  }

  bool CurveQuadLinear::setMaximumValueofz(double maximumValueofz) {
    return getImpl<detail::CurveQuadLinear_Impl>()->setMaximumValueofz(maximumValueofz);
  }

  bool CurveQuadLinear::setMinimumCurveOutput(double minimumCurveOutput) {
    return getImpl<detail::CurveQuadLinear_Impl>()->setMinimumCurveOutput(minimumCurveOutput);
  }

  void CurveQuadLinear::resetMinimumCurveOutput() {
    return getImpl<detail::CurveQuadLinear_Impl>()->resetMinimumCurveOutput();
  }

  bool CurveQuadLinear::setMaximumCurveOutput(double maximumCurveOutput) {
    return getImpl<detail::CurveQuadLinear_Impl>()->setMaximumCurveOutput(maximumCurveOutput);
  }

  void CurveQuadLinear::resetMaximumCurveOutput() {
    getImpl<detail::CurveQuadLinear_Impl>()->resetMaximumCurveOutput();
  }

  bool CurveQuadLinear::setInputUnitTypeforw(const std::string& inputUnitTypeforw) {
    return getImpl<detail::CurveQuadLinear_Impl>()->setInputUnitTypeforw(inputUnitTypeforw);
  }

  void CurveQuadLinear::resetInputUnitTypeforw() {
    getImpl<detail::CurveQuadLinear_Impl>()->resetInputUnitTypeforw();
  }

  bool CurveQuadLinear::setInputUnitTypeforx(const std::string& inputUnitTypeforx) {
    return getImpl<detail::CurveQuadLinear_Impl>()->setInputUnitTypeforx(inputUnitTypeforx);
  }

  void CurveQuadLinear::resetInputUnitTypeforx() {
    getImpl<detail::CurveQuadLinear_Impl>()->resetInputUnitTypeforx();
  }

  bool CurveQuadLinear::setInputUnitTypefory(const std::string& inputUnitTypefory) {
    return getImpl<detail::CurveQuadLinear_Impl>()->setInputUnitTypefory(inputUnitTypefory);
  }

  void CurveQuadLinear::resetInputUnitTypefory() {
    getImpl<detail::CurveQuadLinear_Impl>()->resetInputUnitTypefory();
  }

  bool CurveQuadLinear::setInputUnitTypeforz(const std::string& inputUnitTypeforz) {
    return getImpl<detail::CurveQuadLinear_Impl>()->setInputUnitTypeforz(inputUnitTypeforz);
  }

  void CurveQuadLinear::resetInputUnitTypeforz() {
    getImpl<detail::CurveQuadLinear_Impl>()->resetInputUnitTypeforz();
  }

  /// @cond
  CurveQuadLinear::CurveQuadLinear(std::shared_ptr<detail::CurveQuadLinear_Impl> impl) : Curve(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
