/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "CurveExponentialDecay.hpp"
#include "CurveExponentialDecay_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Curve_ExponentialDecay_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

#include <cmath>

using namespace std;

namespace openstudio {
namespace model {

  namespace detail {

    CurveExponentialDecay_Impl::CurveExponentialDecay_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : Curve_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == CurveExponentialDecay::iddObjectType());
    }

    CurveExponentialDecay_Impl::CurveExponentialDecay_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : Curve_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == CurveExponentialDecay::iddObjectType());
    }

    CurveExponentialDecay_Impl::CurveExponentialDecay_Impl(const CurveExponentialDecay_Impl& other, Model_Impl* model, bool keepHandle)
      : Curve_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& CurveExponentialDecay_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Performance Curve Output Value", "Performance Curve Input Variable 1 Value"};
      return result;
    }

    IddObjectType CurveExponentialDecay_Impl::iddObjectType() const {
      return CurveExponentialDecay::iddObjectType();
    }

    int CurveExponentialDecay_Impl::numVariables() const {
      return 1;
    }

    double CurveExponentialDecay_Impl::evaluate(const std::vector<double>& independentVariables) const {
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

    double CurveExponentialDecay_Impl::coefficient1C1() const {
      boost::optional<double> value = getDouble(OS_Curve_ExponentialDecayFields::Coefficient1C1, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CurveExponentialDecay_Impl::coefficient2C2() const {
      boost::optional<double> value = getDouble(OS_Curve_ExponentialDecayFields::Coefficient2C2, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CurveExponentialDecay_Impl::coefficient3C3() const {
      boost::optional<double> value = getDouble(OS_Curve_ExponentialDecayFields::Coefficient3C3, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CurveExponentialDecay_Impl::minimumValueofx() const {
      boost::optional<double> value = getDouble(OS_Curve_ExponentialDecayFields::MinimumValueofx, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CurveExponentialDecay_Impl::maximumValueofx() const {
      boost::optional<double> value = getDouble(OS_Curve_ExponentialDecayFields::MaximumValueofx, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> CurveExponentialDecay_Impl::minimumCurveOutput() const {
      return getDouble(OS_Curve_ExponentialDecayFields::MinimumCurveOutput, true);
    }

    boost::optional<double> CurveExponentialDecay_Impl::maximumCurveOutput() const {
      return getDouble(OS_Curve_ExponentialDecayFields::MaximumCurveOutput, true);
    }

    std::string CurveExponentialDecay_Impl::inputUnitTypeforx() const {
      boost::optional<std::string> value = getString(OS_Curve_ExponentialDecayFields::InputUnitTypeforx, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CurveExponentialDecay_Impl::isInputUnitTypeforxDefaulted() const {
      return isEmpty(OS_Curve_ExponentialDecayFields::InputUnitTypeforx);
    }

    std::string CurveExponentialDecay_Impl::outputUnitType() const {
      boost::optional<std::string> value = getString(OS_Curve_ExponentialDecayFields::OutputUnitType, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CurveExponentialDecay_Impl::isOutputUnitTypeDefaulted() const {
      return isEmpty(OS_Curve_ExponentialDecayFields::OutputUnitType);
    }

    bool CurveExponentialDecay_Impl::setCoefficient1C1(double coefficient1C1) {
      bool result = setDouble(OS_Curve_ExponentialDecayFields::Coefficient1C1, coefficient1C1);
      OS_ASSERT(result);
      return result;
    }

    bool CurveExponentialDecay_Impl::setCoefficient2C2(double coefficient2C2) {
      bool result = setDouble(OS_Curve_ExponentialDecayFields::Coefficient2C2, coefficient2C2);
      OS_ASSERT(result);
      return result;
    }

    bool CurveExponentialDecay_Impl::setCoefficient3C3(double coefficient3C3) {
      bool result = setDouble(OS_Curve_ExponentialDecayFields::Coefficient3C3, coefficient3C3);
      OS_ASSERT(result);
      return result;
    }

    bool CurveExponentialDecay_Impl::setMinimumValueofx(double minimumValueofx) {
      bool result = setDouble(OS_Curve_ExponentialDecayFields::MinimumValueofx, minimumValueofx);
      OS_ASSERT(result);
      return result;
    }

    bool CurveExponentialDecay_Impl::setMaximumValueofx(double maximumValueofx) {
      bool result = setDouble(OS_Curve_ExponentialDecayFields::MaximumValueofx, maximumValueofx);
      OS_ASSERT(result);
      return result;
    }

    bool CurveExponentialDecay_Impl::setMinimumCurveOutput(boost::optional<double> minimumCurveOutput) {
      bool result = false;
      if (minimumCurveOutput) {
        result = setDouble(OS_Curve_ExponentialDecayFields::MinimumCurveOutput, minimumCurveOutput.get());
      } else {
        result = setString(OS_Curve_ExponentialDecayFields::MinimumCurveOutput, "");
      }
      OS_ASSERT(result);
      return result;
    }

    void CurveExponentialDecay_Impl::resetMinimumCurveOutput() {
      bool result = setString(OS_Curve_ExponentialDecayFields::MinimumCurveOutput, "");
      OS_ASSERT(result);
    }

    bool CurveExponentialDecay_Impl::setMaximumCurveOutput(boost::optional<double> maximumCurveOutput) {
      bool result = false;
      if (maximumCurveOutput) {
        result = setDouble(OS_Curve_ExponentialDecayFields::MaximumCurveOutput, maximumCurveOutput.get());
      } else {
        result = setString(OS_Curve_ExponentialDecayFields::MaximumCurveOutput, "");
      }
      OS_ASSERT(result);
      return result;
    }

    void CurveExponentialDecay_Impl::resetMaximumCurveOutput() {
      bool result = setString(OS_Curve_ExponentialDecayFields::MaximumCurveOutput, "");
      OS_ASSERT(result);
    }

    bool CurveExponentialDecay_Impl::setInputUnitTypeforx(const std::string& inputUnitTypeforx) {
      bool result = setString(OS_Curve_ExponentialDecayFields::InputUnitTypeforx, inputUnitTypeforx);
      return result;
    }

    void CurveExponentialDecay_Impl::resetInputUnitTypeforx() {
      bool result = setString(OS_Curve_ExponentialDecayFields::InputUnitTypeforx, "");
      OS_ASSERT(result);
    }

    bool CurveExponentialDecay_Impl::setOutputUnitType(const std::string& outputUnitType) {
      bool result = setString(OS_Curve_ExponentialDecayFields::OutputUnitType, outputUnitType);
      return result;
    }

    void CurveExponentialDecay_Impl::resetOutputUnitType() {
      bool result = setString(OS_Curve_ExponentialDecayFields::OutputUnitType, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  CurveExponentialDecay::CurveExponentialDecay(const Model& model) : Curve(CurveExponentialDecay::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CurveExponentialDecay_Impl>());
    setDouble(OS_Curve_ExponentialDecayFields::Coefficient1C1, 0.0);
    setDouble(OS_Curve_ExponentialDecayFields::Coefficient2C2, 1.0);
    setDouble(OS_Curve_ExponentialDecayFields::Coefficient3C3, -1.0);
    setDouble(OS_Curve_ExponentialDecayFields::MinimumValueofx, 0.0);
    setDouble(OS_Curve_ExponentialDecayFields::MaximumValueofx, 1.0);
  }

  IddObjectType CurveExponentialDecay::iddObjectType() {
    IddObjectType result(IddObjectType::OS_Curve_ExponentialDecay);
    return result;
  }

  std::vector<std::string> CurveExponentialDecay::validInputUnitTypeforxValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Curve_ExponentialDecayFields::InputUnitTypeforx);
  }

  std::vector<std::string> CurveExponentialDecay::validOutputUnitTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Curve_ExponentialDecayFields::OutputUnitType);
  }

  double CurveExponentialDecay::coefficient1C1() const {
    return getImpl<detail::CurveExponentialDecay_Impl>()->coefficient1C1();
  }

  double CurveExponentialDecay::coefficient2C2() const {
    return getImpl<detail::CurveExponentialDecay_Impl>()->coefficient2C2();
  }

  double CurveExponentialDecay::coefficient3C3() const {
    return getImpl<detail::CurveExponentialDecay_Impl>()->coefficient3C3();
  }

  double CurveExponentialDecay::minimumValueofx() const {
    return getImpl<detail::CurveExponentialDecay_Impl>()->minimumValueofx();
  }

  double CurveExponentialDecay::maximumValueofx() const {
    return getImpl<detail::CurveExponentialDecay_Impl>()->maximumValueofx();
  }

  boost::optional<double> CurveExponentialDecay::minimumCurveOutput() const {
    return getImpl<detail::CurveExponentialDecay_Impl>()->minimumCurveOutput();
  }

  boost::optional<double> CurveExponentialDecay::maximumCurveOutput() const {
    return getImpl<detail::CurveExponentialDecay_Impl>()->maximumCurveOutput();
  }

  std::string CurveExponentialDecay::inputUnitTypeforx() const {
    return getImpl<detail::CurveExponentialDecay_Impl>()->inputUnitTypeforx();
  }

  bool CurveExponentialDecay::isInputUnitTypeforxDefaulted() const {
    return getImpl<detail::CurveExponentialDecay_Impl>()->isInputUnitTypeforxDefaulted();
  }

  std::string CurveExponentialDecay::outputUnitType() const {
    return getImpl<detail::CurveExponentialDecay_Impl>()->outputUnitType();
  }

  bool CurveExponentialDecay::isOutputUnitTypeDefaulted() const {
    return getImpl<detail::CurveExponentialDecay_Impl>()->isOutputUnitTypeDefaulted();
  }

  bool CurveExponentialDecay::setCoefficient1C1(double coefficient1C1) {
    return getImpl<detail::CurveExponentialDecay_Impl>()->setCoefficient1C1(coefficient1C1);
  }

  bool CurveExponentialDecay::setCoefficient2C2(double coefficient2C2) {
    return getImpl<detail::CurveExponentialDecay_Impl>()->setCoefficient2C2(coefficient2C2);
  }

  bool CurveExponentialDecay::setCoefficient3C3(double coefficient3C3) {
    return getImpl<detail::CurveExponentialDecay_Impl>()->setCoefficient3C3(coefficient3C3);
  }

  bool CurveExponentialDecay::setMinimumValueofx(double minimumValueofx) {
    return getImpl<detail::CurveExponentialDecay_Impl>()->setMinimumValueofx(minimumValueofx);
  }

  bool CurveExponentialDecay::setMaximumValueofx(double maximumValueofx) {
    return getImpl<detail::CurveExponentialDecay_Impl>()->setMaximumValueofx(maximumValueofx);
  }

  bool CurveExponentialDecay::setMinimumCurveOutput(double minimumCurveOutput) {
    return getImpl<detail::CurveExponentialDecay_Impl>()->setMinimumCurveOutput(minimumCurveOutput);
  }

  void CurveExponentialDecay::resetMinimumCurveOutput() {
    getImpl<detail::CurveExponentialDecay_Impl>()->resetMinimumCurveOutput();
  }

  bool CurveExponentialDecay::setMaximumCurveOutput(double maximumCurveOutput) {
    return getImpl<detail::CurveExponentialDecay_Impl>()->setMaximumCurveOutput(maximumCurveOutput);
  }

  void CurveExponentialDecay::resetMaximumCurveOutput() {
    getImpl<detail::CurveExponentialDecay_Impl>()->resetMaximumCurveOutput();
  }

  bool CurveExponentialDecay::setInputUnitTypeforx(const std::string& inputUnitTypeforx) {
    return getImpl<detail::CurveExponentialDecay_Impl>()->setInputUnitTypeforx(inputUnitTypeforx);
  }

  void CurveExponentialDecay::resetInputUnitTypeforx() {
    getImpl<detail::CurveExponentialDecay_Impl>()->resetInputUnitTypeforx();
  }

  bool CurveExponentialDecay::setOutputUnitType(const std::string& outputUnitType) {
    return getImpl<detail::CurveExponentialDecay_Impl>()->setOutputUnitType(outputUnitType);
  }

  void CurveExponentialDecay::resetOutputUnitType() {
    getImpl<detail::CurveExponentialDecay_Impl>()->resetOutputUnitType();
  }

  /// @cond
  CurveExponentialDecay::CurveExponentialDecay(std::shared_ptr<detail::CurveExponentialDecay_Impl> impl) : Curve(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
