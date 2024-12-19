/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "CurveFunctionalPressureDrop.hpp"
#include "CurveFunctionalPressureDrop_Impl.hpp"

#include <utilities/idd/OS_Curve_Functional_PressureDrop_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

#include <cmath>

using namespace std;

namespace openstudio {
namespace model {

  namespace detail {

    CurveFunctionalPressureDrop_Impl::CurveFunctionalPressureDrop_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : Curve_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == CurveFunctionalPressureDrop::iddObjectType());
    }

    CurveFunctionalPressureDrop_Impl::CurveFunctionalPressureDrop_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                       bool keepHandle)
      : Curve_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == CurveFunctionalPressureDrop::iddObjectType());
    }

    CurveFunctionalPressureDrop_Impl::CurveFunctionalPressureDrop_Impl(const CurveFunctionalPressureDrop_Impl& other, Model_Impl* model,
                                                                       bool keepHandle)
      : Curve_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& CurveFunctionalPressureDrop_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{// Curve Input 1: MassFlow
                                                   "Performance Curve Input Variable 1 Value",

                                                   // Curve Input 2: Density
                                                   "Performance Curve Input Variable 2 Value",

                                                   // Curve Input 3: Velocity
                                                   "Performance Curve Input Variable 3 Value",

                                                   // Curve Output: the resultant value
                                                   "Performance Curve Output Value"};
      return result;
    }

    IddObjectType CurveFunctionalPressureDrop_Impl::iddObjectType() const {
      return CurveFunctionalPressureDrop::iddObjectType();
    }

    int CurveFunctionalPressureDrop_Impl::numVariables() const {
      LOG_AND_THROW("CurveFunctionalPressureDrop must be evaluated by EnergyPlus, in the context of "
                    << "plant pressure simulations. Therefore, numVariables and evaluate are not applicable " << "for this curve type.");
    }

    double CurveFunctionalPressureDrop_Impl::evaluate(const std::vector<double>& /*independentVariables*/) const {
      LOG_AND_THROW("CurveFunctionalPressureDrop must be evaluated by EnergyPlus, in the context of "
                    << "plant pressure simulations. Therefore, numVariables and evaluate are not applicable " << "for this curve type.");
    }

    double CurveFunctionalPressureDrop_Impl::diameter() const {
      boost::optional<double> value = getDouble(OS_Curve_Functional_PressureDropFields::Diameter, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> CurveFunctionalPressureDrop_Impl::minorLossCoefficient() const {
      return getDouble(OS_Curve_Functional_PressureDropFields::MinorLossCoefficient, true);
    }

    boost::optional<double> CurveFunctionalPressureDrop_Impl::length() const {
      return getDouble(OS_Curve_Functional_PressureDropFields::Length, true);
    }

    boost::optional<double> CurveFunctionalPressureDrop_Impl::roughness() const {
      return getDouble(OS_Curve_Functional_PressureDropFields::Roughness, true);
    }

    boost::optional<double> CurveFunctionalPressureDrop_Impl::fixedFrictionFactor() const {
      return getDouble(OS_Curve_Functional_PressureDropFields::FixedFrictionFactor, true);
    }

    bool CurveFunctionalPressureDrop_Impl::setDiameter(double diameter) {
      bool result = setDouble(OS_Curve_Functional_PressureDropFields::Diameter, diameter);
      return result;
    }

    bool CurveFunctionalPressureDrop_Impl::setMinorLossCoefficient(boost::optional<double> minorLossCoefficient) {
      bool result = false;
      if (minorLossCoefficient) {
        result = setDouble(OS_Curve_Functional_PressureDropFields::MinorLossCoefficient, minorLossCoefficient.get());
      } else {
        result = setString(OS_Curve_Functional_PressureDropFields::MinorLossCoefficient, "");
      }
      return result;
    }

    void CurveFunctionalPressureDrop_Impl::resetMinorLossCoefficient() {
      bool result = setString(OS_Curve_Functional_PressureDropFields::MinorLossCoefficient, "");
      OS_ASSERT(result);
    }

    bool CurveFunctionalPressureDrop_Impl::setLength(boost::optional<double> length) {
      bool result = false;
      if (length) {
        result = setDouble(OS_Curve_Functional_PressureDropFields::Length, length.get());
      } else {
        result = setString(OS_Curve_Functional_PressureDropFields::Length, "");
      }
      return result;
    }

    void CurveFunctionalPressureDrop_Impl::resetLength() {
      bool result = setString(OS_Curve_Functional_PressureDropFields::Length, "");
      OS_ASSERT(result);
    }

    bool CurveFunctionalPressureDrop_Impl::setRoughness(boost::optional<double> roughness) {
      bool result = false;
      if (roughness) {
        result = setDouble(OS_Curve_Functional_PressureDropFields::Roughness, roughness.get());
      } else {
        result = setString(OS_Curve_Functional_PressureDropFields::Roughness, "");
      }
      return result;
    }

    void CurveFunctionalPressureDrop_Impl::resetRoughness() {
      bool result = setString(OS_Curve_Functional_PressureDropFields::Roughness, "");
      OS_ASSERT(result);
    }

    bool CurveFunctionalPressureDrop_Impl::setFixedFrictionFactor(boost::optional<double> fixedFrictionFactor) {
      bool result = false;
      if (fixedFrictionFactor) {
        result = setDouble(OS_Curve_Functional_PressureDropFields::FixedFrictionFactor, fixedFrictionFactor.get());
      } else {
        result = setString(OS_Curve_Functional_PressureDropFields::FixedFrictionFactor, "");
      }
      return result;
    }

    void CurveFunctionalPressureDrop_Impl::resetFixedFrictionFactor() {
      bool result = setString(OS_Curve_Functional_PressureDropFields::FixedFrictionFactor, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  CurveFunctionalPressureDrop::CurveFunctionalPressureDrop(const Model& model) : Curve(CurveFunctionalPressureDrop::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CurveFunctionalPressureDrop_Impl>());
    setDouble(OS_Curve_Functional_PressureDropFields::Diameter, 0.05);
  }

  IddObjectType CurveFunctionalPressureDrop::iddObjectType() {
    IddObjectType result(IddObjectType::OS_Curve_Functional_PressureDrop);
    return result;
  }

  double CurveFunctionalPressureDrop::diameter() const {
    return getImpl<detail::CurveFunctionalPressureDrop_Impl>()->diameter();
  }

  boost::optional<double> CurveFunctionalPressureDrop::minorLossCoefficient() const {
    return getImpl<detail::CurveFunctionalPressureDrop_Impl>()->minorLossCoefficient();
  }

  boost::optional<double> CurveFunctionalPressureDrop::length() const {
    return getImpl<detail::CurveFunctionalPressureDrop_Impl>()->length();
  }

  boost::optional<double> CurveFunctionalPressureDrop::roughness() const {
    return getImpl<detail::CurveFunctionalPressureDrop_Impl>()->roughness();
  }

  boost::optional<double> CurveFunctionalPressureDrop::fixedFrictionFactor() const {
    return getImpl<detail::CurveFunctionalPressureDrop_Impl>()->fixedFrictionFactor();
  }

  bool CurveFunctionalPressureDrop::setDiameter(double diameter) {
    return getImpl<detail::CurveFunctionalPressureDrop_Impl>()->setDiameter(diameter);
  }

  bool CurveFunctionalPressureDrop::setMinorLossCoefficient(double minorLossCoefficient) {
    return getImpl<detail::CurveFunctionalPressureDrop_Impl>()->setMinorLossCoefficient(minorLossCoefficient);
  }

  void CurveFunctionalPressureDrop::resetMinorLossCoefficient() {
    getImpl<detail::CurveFunctionalPressureDrop_Impl>()->resetMinorLossCoefficient();
  }

  bool CurveFunctionalPressureDrop::setLength(double length) {
    return getImpl<detail::CurveFunctionalPressureDrop_Impl>()->setLength(length);
  }

  void CurveFunctionalPressureDrop::resetLength() {
    getImpl<detail::CurveFunctionalPressureDrop_Impl>()->resetLength();
  }

  bool CurveFunctionalPressureDrop::setRoughness(double roughness) {
    return getImpl<detail::CurveFunctionalPressureDrop_Impl>()->setRoughness(roughness);
  }

  void CurveFunctionalPressureDrop::resetRoughness() {
    getImpl<detail::CurveFunctionalPressureDrop_Impl>()->resetRoughness();
  }

  bool CurveFunctionalPressureDrop::setFixedFrictionFactor(double fixedFrictionFactor) {
    return getImpl<detail::CurveFunctionalPressureDrop_Impl>()->setFixedFrictionFactor(fixedFrictionFactor);
  }

  void CurveFunctionalPressureDrop::resetFixedFrictionFactor() {
    getImpl<detail::CurveFunctionalPressureDrop_Impl>()->resetFixedFrictionFactor();
  }

  /// @cond
  CurveFunctionalPressureDrop::CurveFunctionalPressureDrop(std::shared_ptr<detail::CurveFunctionalPressureDrop_Impl> impl) : Curve(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
