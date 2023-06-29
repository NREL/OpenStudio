/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "CurveDoubleExponentialDecay.hpp"
#include "CurveDoubleExponentialDecay_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Curve_DoubleExponentialDecay_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

#include "../utilities/core/DeprecatedHelpers.hpp"

#include <cmath>

using namespace std;

namespace openstudio {
namespace model {

  namespace detail {

    CurveDoubleExponentialDecay_Impl::CurveDoubleExponentialDecay_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : Curve_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == CurveDoubleExponentialDecay::iddObjectType());
    }

    CurveDoubleExponentialDecay_Impl::CurveDoubleExponentialDecay_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                       bool keepHandle)
      : Curve_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == CurveDoubleExponentialDecay::iddObjectType());
    }

    CurveDoubleExponentialDecay_Impl::CurveDoubleExponentialDecay_Impl(const CurveDoubleExponentialDecay_Impl& other, Model_Impl* model,
                                                                       bool keepHandle)
      : Curve_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& CurveDoubleExponentialDecay_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Performance Curve Output Value", "Performance Curve Input Variable 1 Value"};
      return result;
    }

    IddObjectType CurveDoubleExponentialDecay_Impl::iddObjectType() const {
      return CurveDoubleExponentialDecay::iddObjectType();
    }

    int CurveDoubleExponentialDecay_Impl::numVariables() const {
      return 1;
    }

    double CurveDoubleExponentialDecay_Impl::evaluate(const std::vector<double>& independentVariables) const {
      OS_ASSERT(independentVariables.size() == 1u);

      double x = independentVariables[0];
      if (x < minimumValueofx()) {
        LOG(Warn, "Supplied x is below the minimumValueofx, resetting it.");
        x = minimumValueofx();
      }
      if (x > maximumValueofx()) {
        LOG(Warn, "Supplied x is above the maximumValueofx, resetting it.");
        x = maximumValueofx();
      }

      double result = coefficient1C1();
      result += coefficient2C2() * exp(coefficient3C3() * x);
      result += coefficient4C4() * exp(coefficient5C5() * x);

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

    double CurveDoubleExponentialDecay_Impl::coefficient1C1() const {
      boost::optional<double> value = getDouble(OS_Curve_DoubleExponentialDecayFields::Coefficient1C1, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CurveDoubleExponentialDecay_Impl::coefficient2C2() const {
      boost::optional<double> value = getDouble(OS_Curve_DoubleExponentialDecayFields::Coefficient2C2, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CurveDoubleExponentialDecay_Impl::coefficient3C3() const {
      boost::optional<double> value = getDouble(OS_Curve_DoubleExponentialDecayFields::Coefficient3C3, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CurveDoubleExponentialDecay_Impl::coefficient4C4() const {
      boost::optional<double> value = getDouble(OS_Curve_DoubleExponentialDecayFields::Coefficient4C4, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CurveDoubleExponentialDecay_Impl::coefficient5C5() const {
      boost::optional<double> value = getDouble(OS_Curve_DoubleExponentialDecayFields::Coefficient5C5, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CurveDoubleExponentialDecay_Impl::minimumValueofx() const {
      boost::optional<double> value = getDouble(OS_Curve_DoubleExponentialDecayFields::MinimumValueofx, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CurveDoubleExponentialDecay_Impl::maximumValueofx() const {
      boost::optional<double> value = getDouble(OS_Curve_DoubleExponentialDecayFields::MaximumValueofx, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> CurveDoubleExponentialDecay_Impl::minimumCurveOutput() const {
      return getDouble(OS_Curve_DoubleExponentialDecayFields::MinimumCurveOutput, true);
    }

    boost::optional<double> CurveDoubleExponentialDecay_Impl::maximumCurveOutput() const {
      return getDouble(OS_Curve_DoubleExponentialDecayFields::MaximumCurveOutput, true);
    }

    std::string CurveDoubleExponentialDecay_Impl::inputUnitTypeforx() const {
      boost::optional<std::string> value = getString(OS_Curve_DoubleExponentialDecayFields::InputUnitTypeforx, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CurveDoubleExponentialDecay_Impl::isInputUnitTypeforxDefaulted() const {
      return isEmpty(OS_Curve_DoubleExponentialDecayFields::InputUnitTypeforx);
    }

    std::string CurveDoubleExponentialDecay_Impl::outputUnitType() const {
      boost::optional<std::string> value = getString(OS_Curve_DoubleExponentialDecayFields::OutputUnitType, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CurveDoubleExponentialDecay_Impl::isOutputUnitTypeDefaulted() const {
      return isEmpty(OS_Curve_DoubleExponentialDecayFields::OutputUnitType);
    }

    bool CurveDoubleExponentialDecay_Impl::setCoefficient1C1(double coefficient1C1) {
      bool result = setDouble(OS_Curve_DoubleExponentialDecayFields::Coefficient1C1, coefficient1C1);
      OS_ASSERT(result);
      return result;
    }

    bool CurveDoubleExponentialDecay_Impl::setCoefficient2C2(double coefficient2C2) {
      bool result = setDouble(OS_Curve_DoubleExponentialDecayFields::Coefficient2C2, coefficient2C2);
      OS_ASSERT(result);
      return result;
    }

    bool CurveDoubleExponentialDecay_Impl::setCoefficient3C3(double coefficient3C3) {
      bool result = setDouble(OS_Curve_DoubleExponentialDecayFields::Coefficient3C3, coefficient3C3);
      OS_ASSERT(result);
      return result;
    }

    bool CurveDoubleExponentialDecay_Impl::setCoefficient4C4(double coefficient4C4) {
      bool result = setDouble(OS_Curve_DoubleExponentialDecayFields::Coefficient4C4, coefficient4C4);
      OS_ASSERT(result);
      return result;
    }

    bool CurveDoubleExponentialDecay_Impl::setCoefficient5C5(double coefficient5C5) {
      bool result = setDouble(OS_Curve_DoubleExponentialDecayFields::Coefficient5C5, coefficient5C5);
      OS_ASSERT(result);
      return result;
    }

    bool CurveDoubleExponentialDecay_Impl::setMinimumValueofx(double minimumValueofx) {
      bool result = setDouble(OS_Curve_DoubleExponentialDecayFields::MinimumValueofx, minimumValueofx);
      OS_ASSERT(result);
      return result;
    }

    bool CurveDoubleExponentialDecay_Impl::setMaximumValueofx(double maximumValueofx) {
      bool result = setDouble(OS_Curve_DoubleExponentialDecayFields::MaximumValueofx, maximumValueofx);
      OS_ASSERT(result);
      return result;
    }

    bool CurveDoubleExponentialDecay_Impl::setMinimumCurveOutput(boost::optional<double> minimumCurveOutput) {
      bool result = false;
      if (minimumCurveOutput) {
        result = setDouble(OS_Curve_DoubleExponentialDecayFields::MinimumCurveOutput, minimumCurveOutput.get());
      } else {
        result = setString(OS_Curve_DoubleExponentialDecayFields::MinimumCurveOutput, "");
      }
      OS_ASSERT(result);
      return result;
    }

    void CurveDoubleExponentialDecay_Impl::resetMinimumCurveOutput() {
      bool result = setString(OS_Curve_DoubleExponentialDecayFields::MinimumCurveOutput, "");
      OS_ASSERT(result);
    }

    bool CurveDoubleExponentialDecay_Impl::setMaximumCurveOutput(boost::optional<double> maximumCurveOutput) {
      bool result = false;
      if (maximumCurveOutput) {
        result = setDouble(OS_Curve_DoubleExponentialDecayFields::MaximumCurveOutput, maximumCurveOutput.get());
      } else {
        result = setString(OS_Curve_DoubleExponentialDecayFields::MaximumCurveOutput, "");
      }
      OS_ASSERT(result);
      return result;
    }

    void CurveDoubleExponentialDecay_Impl::resetMaximumCurveOutput() {
      bool result = setString(OS_Curve_DoubleExponentialDecayFields::MaximumCurveOutput, "");
      OS_ASSERT(result);
    }

    bool CurveDoubleExponentialDecay_Impl::setInputUnitTypeforx(const std::string& inputUnitTypeforx) {
      bool result = setString(OS_Curve_DoubleExponentialDecayFields::InputUnitTypeforx, inputUnitTypeforx);
      return result;
    }

    void CurveDoubleExponentialDecay_Impl::resetInputUnitTypeforx() {
      bool result = setString(OS_Curve_DoubleExponentialDecayFields::InputUnitTypeforx, "");
      OS_ASSERT(result);
    }

    bool CurveDoubleExponentialDecay_Impl::setOutputUnitType(const std::string& outputUnitType) {
      bool result = setString(OS_Curve_DoubleExponentialDecayFields::OutputUnitType, outputUnitType);
      return result;
    }

    void CurveDoubleExponentialDecay_Impl::resetOutputUnitType() {
      bool result = setString(OS_Curve_DoubleExponentialDecayFields::OutputUnitType, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  CurveDoubleExponentialDecay::CurveDoubleExponentialDecay(const Model& model) : Curve(CurveDoubleExponentialDecay::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CurveDoubleExponentialDecay_Impl>());
    setDouble(OS_Curve_DoubleExponentialDecayFields::Coefficient1C1, 0.0);
    setDouble(OS_Curve_DoubleExponentialDecayFields::Coefficient2C2, 0.0);
    setDouble(OS_Curve_DoubleExponentialDecayFields::Coefficient3C3, 0.0);
    setDouble(OS_Curve_DoubleExponentialDecayFields::Coefficient4C4, 0.0);
    setDouble(OS_Curve_DoubleExponentialDecayFields::Coefficient5C5, 0.0);
    setDouble(OS_Curve_DoubleExponentialDecayFields::MinimumValueofx, 0.0);
    setDouble(OS_Curve_DoubleExponentialDecayFields::MaximumValueofx, 1.0);
  }

  IddObjectType CurveDoubleExponentialDecay::iddObjectType() {
    IddObjectType result(IddObjectType::OS_Curve_DoubleExponentialDecay);
    return result;
  }

  std::vector<std::string> CurveDoubleExponentialDecay::validInputUnitTypeforxValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Curve_DoubleExponentialDecayFields::InputUnitTypeforx);
  }

  std::vector<std::string> CurveDoubleExponentialDecay::validOutputUnitTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Curve_DoubleExponentialDecayFields::OutputUnitType);
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

  double CurveDoubleExponentialDecay::coefficient4C4() const {
    return getImpl<detail::CurveDoubleExponentialDecay_Impl>()->coefficient4C4();
  }

  double CurveDoubleExponentialDecay::coefficient3C4() const {
    DEPRECATED_AT_MSG(3, 1, 0, "Use coefficient4C4 instead.");
    return getImpl<detail::CurveDoubleExponentialDecay_Impl>()->coefficient4C4();
  }

  double CurveDoubleExponentialDecay::coefficient5C5() const {
    return getImpl<detail::CurveDoubleExponentialDecay_Impl>()->coefficient5C5();
  }

  double CurveDoubleExponentialDecay::coefficient3C5() const {
    DEPRECATED_AT_MSG(3, 1, 0, "Use coefficient4C4 instead.");
    return getImpl<detail::CurveDoubleExponentialDecay_Impl>()->coefficient5C5();
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

  bool CurveDoubleExponentialDecay::setCoefficient1C1(double coefficient1C1) {
    return getImpl<detail::CurveDoubleExponentialDecay_Impl>()->setCoefficient1C1(coefficient1C1);
  }

  bool CurveDoubleExponentialDecay::setCoefficient2C2(double coefficient2C2) {
    return getImpl<detail::CurveDoubleExponentialDecay_Impl>()->setCoefficient2C2(coefficient2C2);
  }

  bool CurveDoubleExponentialDecay::setCoefficient3C3(double coefficient3C3) {
    return getImpl<detail::CurveDoubleExponentialDecay_Impl>()->setCoefficient3C3(coefficient3C3);
  }

  bool CurveDoubleExponentialDecay::setCoefficient4C4(double coefficient4C4) {
    return getImpl<detail::CurveDoubleExponentialDecay_Impl>()->setCoefficient4C4(coefficient4C4);
  }

  bool CurveDoubleExponentialDecay::setCoefficient3C4(double coefficient4C4) {
    DEPRECATED_AT_MSG(3, 1, 0, "Use setCoefficient4C4 instead.");
    return getImpl<detail::CurveDoubleExponentialDecay_Impl>()->setCoefficient4C4(coefficient4C4);
  }

  bool CurveDoubleExponentialDecay::setCoefficient5C5(double coefficient5C5) {
    return getImpl<detail::CurveDoubleExponentialDecay_Impl>()->setCoefficient5C5(coefficient5C5);
  }

  bool CurveDoubleExponentialDecay::setCoefficient3C5(double coefficient5C5) {
    DEPRECATED_AT_MSG(3, 1, 0, "Use setCoefficient5C5 instead.");
    return getImpl<detail::CurveDoubleExponentialDecay_Impl>()->setCoefficient5C5(coefficient5C5);
  }

  bool CurveDoubleExponentialDecay::setMinimumValueofx(double minimumValueofx) {
    return getImpl<detail::CurveDoubleExponentialDecay_Impl>()->setMinimumValueofx(minimumValueofx);
  }

  bool CurveDoubleExponentialDecay::setMaximumValueofx(double maximumValueofx) {
    return getImpl<detail::CurveDoubleExponentialDecay_Impl>()->setMaximumValueofx(maximumValueofx);
  }

  bool CurveDoubleExponentialDecay::setMinimumCurveOutput(double minimumCurveOutput) {
    return getImpl<detail::CurveDoubleExponentialDecay_Impl>()->setMinimumCurveOutput(minimumCurveOutput);
  }

  void CurveDoubleExponentialDecay::resetMinimumCurveOutput() {
    getImpl<detail::CurveDoubleExponentialDecay_Impl>()->resetMinimumCurveOutput();
  }

  bool CurveDoubleExponentialDecay::setMaximumCurveOutput(double maximumCurveOutput) {
    return getImpl<detail::CurveDoubleExponentialDecay_Impl>()->setMaximumCurveOutput(maximumCurveOutput);
  }

  void CurveDoubleExponentialDecay::resetMaximumCurveOutput() {
    getImpl<detail::CurveDoubleExponentialDecay_Impl>()->resetMaximumCurveOutput();
  }

  bool CurveDoubleExponentialDecay::setInputUnitTypeforx(const std::string& inputUnitTypeforx) {
    return getImpl<detail::CurveDoubleExponentialDecay_Impl>()->setInputUnitTypeforx(inputUnitTypeforx);
  }

  void CurveDoubleExponentialDecay::resetInputUnitTypeforx() {
    getImpl<detail::CurveDoubleExponentialDecay_Impl>()->resetInputUnitTypeforx();
  }

  bool CurveDoubleExponentialDecay::setOutputUnitType(const std::string& outputUnitType) {
    return getImpl<detail::CurveDoubleExponentialDecay_Impl>()->setOutputUnitType(outputUnitType);
  }

  void CurveDoubleExponentialDecay::resetOutputUnitType() {
    getImpl<detail::CurveDoubleExponentialDecay_Impl>()->resetOutputUnitType();
  }

  /// @cond
  CurveDoubleExponentialDecay::CurveDoubleExponentialDecay(std::shared_ptr<detail::CurveDoubleExponentialDecay_Impl> impl) : Curve(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
