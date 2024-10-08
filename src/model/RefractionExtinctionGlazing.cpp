/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "RefractionExtinctionGlazing.hpp"
#include "RefractionExtinctionGlazing_Impl.hpp"

#include <utilities/idd/OS_WindowMaterial_Glazing_RefractionExtinctionMethod_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    RefractionExtinctionGlazing_Impl::RefractionExtinctionGlazing_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : Glazing_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == RefractionExtinctionGlazing::iddObjectType());
    }

    RefractionExtinctionGlazing_Impl::RefractionExtinctionGlazing_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                       bool keepHandle)
      : Glazing_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == RefractionExtinctionGlazing::iddObjectType());
    }

    RefractionExtinctionGlazing_Impl::RefractionExtinctionGlazing_Impl(const RefractionExtinctionGlazing_Impl& other, Model_Impl* model,
                                                                       bool keepHandle)
      : Glazing_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& RefractionExtinctionGlazing_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType RefractionExtinctionGlazing_Impl::iddObjectType() const {
      return RefractionExtinctionGlazing::iddObjectType();
    }

    double RefractionExtinctionGlazing_Impl::thickness() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::Thickness, true);
      OS_ASSERT(value);
      return value.get();
    }

    double RefractionExtinctionGlazing_Impl::solarIndexofRefraction() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::SolarIndexofRefraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    double RefractionExtinctionGlazing_Impl::solarExtinctionCoefficient() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::SolarExtinctionCoefficient, true);
      OS_ASSERT(value);
      return value.get();
    }

    double RefractionExtinctionGlazing_Impl::visibleIndexofRefraction() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::VisibleIndexofRefraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    double RefractionExtinctionGlazing_Impl::visibleExtinctionCoefficient() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::VisibleExtinctionCoefficient, true);
      OS_ASSERT(value);
      return value.get();
    }

    double RefractionExtinctionGlazing_Impl::infraredTransmittanceatNormalIncidence() const {
      boost::optional<double> value =
        getDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::InfraredTransmittanceatNormalIncidence, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool RefractionExtinctionGlazing_Impl::isInfraredTransmittanceatNormalIncidenceDefaulted() const {
      return isEmpty(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::InfraredTransmittanceatNormalIncidence);
    }

    double RefractionExtinctionGlazing_Impl::infraredHemisphericalEmissivity() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::InfraredHemisphericalEmissivity, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool RefractionExtinctionGlazing_Impl::isInfraredHemisphericalEmissivityDefaulted() const {
      return isEmpty(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::InfraredHemisphericalEmissivity);
    }

    double RefractionExtinctionGlazing_Impl::conductivity() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::Conductivity, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool RefractionExtinctionGlazing_Impl::isConductivityDefaulted() const {
      return isEmpty(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::Conductivity);
    }

    double RefractionExtinctionGlazing_Impl::dirtCorrectionFactorforSolarandVisibleTransmittance() const {
      boost::optional<double> value =
        getDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::DirtCorrectionFactorforSolarandVisibleTransmittance, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool RefractionExtinctionGlazing_Impl::isDirtCorrectionFactorforSolarandVisibleTransmittanceDefaulted() const {
      return isEmpty(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::DirtCorrectionFactorforSolarandVisibleTransmittance);
    }

    bool RefractionExtinctionGlazing_Impl::solarDiffusing() const {
      boost::optional<std::string> value = getString(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::SolarDiffusing, true);
      OS_ASSERT(value);
      return openstudio::istringEqual(value.get(), "Yes");
    }

    bool RefractionExtinctionGlazing_Impl::isSolarDiffusingDefaulted() const {
      return isEmpty(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::SolarDiffusing);
    }

    bool RefractionExtinctionGlazing_Impl::setThickness(double thickness) {
      bool result = setDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::Thickness, thickness);
      return result;
    }

    double RefractionExtinctionGlazing_Impl::thermalConductivity() const {
      OptionalDouble od = getDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::Conductivity, true);
      if (!od) {
        LOG_AND_THROW("Thermal conductivity not yet defined for " << briefDescription() << ".");
      }
      return *od;
    }

    double RefractionExtinctionGlazing_Impl::thermalConductance() const {
      return thermalConductivity() / thickness();
    }

    double RefractionExtinctionGlazing_Impl::thermalResistivity() const {
      return 1.0 / thermalConductivity();
    }

    double RefractionExtinctionGlazing_Impl::thermalResistance() const {
      return 1.0 / thermalConductance();
    }

    double RefractionExtinctionGlazing_Impl::thermalTransmittance() const {
      OptionalDouble od = getDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::InfraredTransmittanceatNormalIncidence, true);
      if (!od) {
        LOG_AND_THROW("Thermal transmittance not yet defined for " << briefDescription() << ".");
      }
      return *od;
    }

    OptionalDouble RefractionExtinctionGlazing_Impl::getVisibleTransmittance() const {
      LOG_AND_THROW("Not yet implemented. Need to convert index of refraction and exinction coefficient to transmittance and absorptance, or look it "
                    "up in the SQL file.");
      return 0.0;
    }

    double RefractionExtinctionGlazing_Impl::visibleAbsorptance() const {
      LOG_AND_THROW("Not yet implemented. Need to convert index of refraction and exinction coefficient to transmittance and absorptance, or look it "
                    "up in the SQL file.");
      return 0.0;
    }

    boost::optional<double> RefractionExtinctionGlazing_Impl::interiorVisibleAbsorptance() const {
      return visibleAbsorptance();
    }

    boost::optional<double> RefractionExtinctionGlazing_Impl::exteriorVisibleAbsorptance() const {
      return visibleAbsorptance();
    }

    bool RefractionExtinctionGlazing_Impl::setSolarIndexofRefraction(double solarIndexofRefraction) {
      bool result = setDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::SolarIndexofRefraction, solarIndexofRefraction);
      return result;
    }

    bool RefractionExtinctionGlazing_Impl::setSolarExtinctionCoefficient(double solarExtinctionCoefficient) {
      bool result = setDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::SolarExtinctionCoefficient, solarExtinctionCoefficient);
      return result;
    }

    bool RefractionExtinctionGlazing_Impl::setVisibleIndexofRefraction(double visibleIndexofRefraction) {
      bool result = setDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::VisibleIndexofRefraction, visibleIndexofRefraction);
      return result;
    }

    bool RefractionExtinctionGlazing_Impl::setVisibleExtinctionCoefficient(double visibleExtinctionCoefficient) {
      bool result = setDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::VisibleExtinctionCoefficient, visibleExtinctionCoefficient);
      return result;
    }

    bool RefractionExtinctionGlazing_Impl::setInfraredTransmittanceatNormalIncidence(double infraredTransmittanceatNormalIncidence) {
      bool result = setDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::InfraredTransmittanceatNormalIncidence,
                              infraredTransmittanceatNormalIncidence);
      return result;
    }

    void RefractionExtinctionGlazing_Impl::resetInfraredTransmittanceatNormalIncidence() {
      bool result = setString(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::InfraredTransmittanceatNormalIncidence, "");
      OS_ASSERT(result);
    }

    bool RefractionExtinctionGlazing_Impl::setInfraredHemisphericalEmissivity(double infraredHemisphericalEmissivity) {
      bool result =
        setDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::InfraredHemisphericalEmissivity, infraredHemisphericalEmissivity);
      return result;
    }

    void RefractionExtinctionGlazing_Impl::resetInfraredHemisphericalEmissivity() {
      bool result = setString(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::InfraredHemisphericalEmissivity, "");
      OS_ASSERT(result);
    }

    bool RefractionExtinctionGlazing_Impl::setConductivity(double conductivity) {
      bool result = setDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::Conductivity, conductivity);
      return result;
    }

    void RefractionExtinctionGlazing_Impl::resetConductivity() {
      bool result = setString(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::Conductivity, "");
      OS_ASSERT(result);
    }

    bool RefractionExtinctionGlazing_Impl::setThermalConductivity(double value) {
      return setDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::Conductivity, value);
    }

    bool RefractionExtinctionGlazing_Impl::setThermalConductance(double value) {
      return setThickness(thermalConductivity() / value);
    }

    bool RefractionExtinctionGlazing_Impl::setThermalResistivity(double value) {
      return setThermalConductivity(1.0 / value);
    }

    bool RefractionExtinctionGlazing_Impl::setThermalResistance(double value) {
      return setThickness(value / thermalResistivity());
    }

    bool RefractionExtinctionGlazing_Impl::setThermalTransmittance(double value) {
      return setDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::InfraredTransmittanceatNormalIncidence, value);
    }

    bool RefractionExtinctionGlazing_Impl::setDirtCorrectionFactorforSolarandVisibleTransmittance(
      double dirtCorrectionFactorforSolarandVisibleTransmittance) {
      bool result = setDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::DirtCorrectionFactorforSolarandVisibleTransmittance,
                              dirtCorrectionFactorforSolarandVisibleTransmittance);
      return result;
    }

    void RefractionExtinctionGlazing_Impl::resetDirtCorrectionFactorforSolarandVisibleTransmittance() {
      bool result = setString(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::DirtCorrectionFactorforSolarandVisibleTransmittance, "");
      OS_ASSERT(result);
    }

    bool RefractionExtinctionGlazing_Impl::setSolarDiffusing(bool solarDiffusing) {
      if (solarDiffusing) {
        return setBooleanFieldValue(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::SolarDiffusing, true);
      } else {
        return setBooleanFieldValue(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::SolarDiffusing, false);
        ;
      }
    }

    void RefractionExtinctionGlazing_Impl::resetSolarDiffusing() {
      bool result = setString(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::SolarDiffusing, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  RefractionExtinctionGlazing::RefractionExtinctionGlazing(const Model& model, double thickness, double solarIndexofRefraction,
                                                           double solarExtinctionCoefficient, double visibleIndexofRefraction,
                                                           double visibleExtinctionCoefficient)
    //double dirtCorrectionFactorforSolarandVisibleTransmittance)
    : Glazing(RefractionExtinctionGlazing::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::RefractionExtinctionGlazing_Impl>());

    bool ok = this->setThickness(thickness);
    OS_ASSERT(ok);
    ok = this->setSolarIndexofRefraction(solarIndexofRefraction);
    OS_ASSERT(ok);
    ok = this->setSolarExtinctionCoefficient(solarExtinctionCoefficient);
    OS_ASSERT(ok);
    ok = this->setVisibleIndexofRefraction(visibleIndexofRefraction);
    OS_ASSERT(ok);
    ok = this->setVisibleExtinctionCoefficient(visibleExtinctionCoefficient);
    OS_ASSERT(ok);
  }

  IddObjectType RefractionExtinctionGlazing::iddObjectType() {
    return {IddObjectType::OS_WindowMaterial_Glazing_RefractionExtinctionMethod};
  }

  double RefractionExtinctionGlazing::thermalConductivity() const {
    return getImpl<detail::RefractionExtinctionGlazing_Impl>()->thermalConductivity();
  }

  double RefractionExtinctionGlazing::thermalConductance() const {
    return getImpl<detail::RefractionExtinctionGlazing_Impl>()->thermalConductance();
  }

  double RefractionExtinctionGlazing::thermalResistivity() const {
    return getImpl<detail::RefractionExtinctionGlazing_Impl>()->thermalResistivity();
  }

  double RefractionExtinctionGlazing::thermalResistance() const {
    return getImpl<detail::RefractionExtinctionGlazing_Impl>()->thermalResistance();
  }

  double RefractionExtinctionGlazing::thermalTransmittance() const {
    return getImpl<detail::RefractionExtinctionGlazing_Impl>()->thermalTransmittance();
  }

  bool RefractionExtinctionGlazing::setThermalConductivity(double value) {
    return getImpl<detail::RefractionExtinctionGlazing_Impl>()->setThermalConductivity(value);
  }

  bool RefractionExtinctionGlazing::setThermalConductance(double value) {
    return getImpl<detail::RefractionExtinctionGlazing_Impl>()->setThermalConductance(value);
  }

  bool RefractionExtinctionGlazing::setThermalResistivity(double value) {
    return getImpl<detail::RefractionExtinctionGlazing_Impl>()->setThermalResistivity(value);
  }

  bool RefractionExtinctionGlazing::setThermalResistance(double value) {
    return getImpl<detail::RefractionExtinctionGlazing_Impl>()->setThermalResistance(value);
  }

  bool RefractionExtinctionGlazing::setThermalTransmittance(double value) {
    return getImpl<detail::RefractionExtinctionGlazing_Impl>()->setThermalTransmittance(value);
  }

  // cppcheck-suppress [duplInheritedMember] for documentation purposes
  double RefractionExtinctionGlazing::thickness() const {
    return getImpl<detail::RefractionExtinctionGlazing_Impl>()->thickness();
  }

  double RefractionExtinctionGlazing::solarIndexofRefraction() const {
    return getImpl<detail::RefractionExtinctionGlazing_Impl>()->solarIndexofRefraction();
  }

  double RefractionExtinctionGlazing::solarExtinctionCoefficient() const {
    return getImpl<detail::RefractionExtinctionGlazing_Impl>()->solarExtinctionCoefficient();
  }

  double RefractionExtinctionGlazing::visibleIndexofRefraction() const {
    return getImpl<detail::RefractionExtinctionGlazing_Impl>()->visibleIndexofRefraction();
  }

  double RefractionExtinctionGlazing::visibleExtinctionCoefficient() const {
    return getImpl<detail::RefractionExtinctionGlazing_Impl>()->visibleExtinctionCoefficient();
  }

  double RefractionExtinctionGlazing::infraredTransmittanceatNormalIncidence() const {
    return getImpl<detail::RefractionExtinctionGlazing_Impl>()->infraredTransmittanceatNormalIncidence();
  }

  bool RefractionExtinctionGlazing::isInfraredTransmittanceatNormalIncidenceDefaulted() const {
    return getImpl<detail::RefractionExtinctionGlazing_Impl>()->isInfraredTransmittanceatNormalIncidenceDefaulted();
  }

  double RefractionExtinctionGlazing::infraredHemisphericalEmissivity() const {
    return getImpl<detail::RefractionExtinctionGlazing_Impl>()->infraredHemisphericalEmissivity();
  }

  bool RefractionExtinctionGlazing::isInfraredHemisphericalEmissivityDefaulted() const {
    return getImpl<detail::RefractionExtinctionGlazing_Impl>()->isInfraredHemisphericalEmissivityDefaulted();
  }

  double RefractionExtinctionGlazing::conductivity() const {
    return getImpl<detail::RefractionExtinctionGlazing_Impl>()->conductivity();
  }

  bool RefractionExtinctionGlazing::isConductivityDefaulted() const {
    return getImpl<detail::RefractionExtinctionGlazing_Impl>()->isConductivityDefaulted();
  }

  double RefractionExtinctionGlazing::dirtCorrectionFactorforSolarandVisibleTransmittance() const {
    return getImpl<detail::RefractionExtinctionGlazing_Impl>()->dirtCorrectionFactorforSolarandVisibleTransmittance();
  }

  bool RefractionExtinctionGlazing::isDirtCorrectionFactorforSolarandVisibleTransmittanceDefaulted() const {
    return getImpl<detail::RefractionExtinctionGlazing_Impl>()->isDirtCorrectionFactorforSolarandVisibleTransmittanceDefaulted();
  }

  bool RefractionExtinctionGlazing::solarDiffusing() const {
    return getImpl<detail::RefractionExtinctionGlazing_Impl>()->solarDiffusing();
  }

  bool RefractionExtinctionGlazing::isSolarDiffusingDefaulted() const {
    return getImpl<detail::RefractionExtinctionGlazing_Impl>()->isSolarDiffusingDefaulted();
  }

  // cppcheck-suppress [duplInheritedMember] for documentation purposes
  bool RefractionExtinctionGlazing::setThickness(double thickness) {
    return getImpl<detail::RefractionExtinctionGlazing_Impl>()->setThickness(thickness);
  }

  bool RefractionExtinctionGlazing::setSolarIndexofRefraction(double solarIndexofRefraction) {
    return getImpl<detail::RefractionExtinctionGlazing_Impl>()->setSolarIndexofRefraction(solarIndexofRefraction);
  }

  bool RefractionExtinctionGlazing::setSolarExtinctionCoefficient(double solarExtinctionCoefficient) {
    return getImpl<detail::RefractionExtinctionGlazing_Impl>()->setSolarExtinctionCoefficient(solarExtinctionCoefficient);
  }

  bool RefractionExtinctionGlazing::setVisibleIndexofRefraction(double visibleIndexofRefraction) {
    return getImpl<detail::RefractionExtinctionGlazing_Impl>()->setVisibleIndexofRefraction(visibleIndexofRefraction);
  }

  bool RefractionExtinctionGlazing::setVisibleExtinctionCoefficient(double visibleExtinctionCoefficient) {
    return getImpl<detail::RefractionExtinctionGlazing_Impl>()->setVisibleExtinctionCoefficient(visibleExtinctionCoefficient);
  }

  bool RefractionExtinctionGlazing::setInfraredTransmittanceatNormalIncidence(double infraredTransmittanceatNormalIncidence) {
    return getImpl<detail::RefractionExtinctionGlazing_Impl>()->setInfraredTransmittanceatNormalIncidence(infraredTransmittanceatNormalIncidence);
  }

  void RefractionExtinctionGlazing::resetInfraredTransmittanceatNormalIncidence() {
    getImpl<detail::RefractionExtinctionGlazing_Impl>()->resetInfraredTransmittanceatNormalIncidence();
  }

  bool RefractionExtinctionGlazing::setInfraredHemisphericalEmissivity(double infraredHemisphericalEmissivity) {
    return getImpl<detail::RefractionExtinctionGlazing_Impl>()->setInfraredHemisphericalEmissivity(infraredHemisphericalEmissivity);
  }

  void RefractionExtinctionGlazing::resetInfraredHemisphericalEmissivity() {
    getImpl<detail::RefractionExtinctionGlazing_Impl>()->resetInfraredHemisphericalEmissivity();
  }

  bool RefractionExtinctionGlazing::setConductivity(double conductivity) {
    return getImpl<detail::RefractionExtinctionGlazing_Impl>()->setConductivity(conductivity);
  }

  void RefractionExtinctionGlazing::resetConductivity() {
    getImpl<detail::RefractionExtinctionGlazing_Impl>()->resetConductivity();
  }

  bool
    RefractionExtinctionGlazing::setDirtCorrectionFactorforSolarandVisibleTransmittance(double dirtCorrectionFactorforSolarandVisibleTransmittance) {
    return getImpl<detail::RefractionExtinctionGlazing_Impl>()->setDirtCorrectionFactorforSolarandVisibleTransmittance(
      dirtCorrectionFactorforSolarandVisibleTransmittance);
  }

  void RefractionExtinctionGlazing::resetDirtCorrectionFactorforSolarandVisibleTransmittance() {
    getImpl<detail::RefractionExtinctionGlazing_Impl>()->resetDirtCorrectionFactorforSolarandVisibleTransmittance();
  }

  bool RefractionExtinctionGlazing::setSolarDiffusing(bool solarDiffusing) {
    return getImpl<detail::RefractionExtinctionGlazing_Impl>()->setSolarDiffusing(solarDiffusing);
  }

  void RefractionExtinctionGlazing::setSolarDiffusingNoFail(bool solarDiffusing) {
    bool result = getImpl<detail::RefractionExtinctionGlazing_Impl>()->setSolarDiffusing(solarDiffusing);
    OS_ASSERT(result);
  }

  void RefractionExtinctionGlazing::resetSolarDiffusing() {
    getImpl<detail::RefractionExtinctionGlazing_Impl>()->resetSolarDiffusing();
  }

  /// @cond
  RefractionExtinctionGlazing::RefractionExtinctionGlazing(std::shared_ptr<detail::RefractionExtinctionGlazing_Impl> impl)
    : Glazing(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
