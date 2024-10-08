/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "Shade.hpp"
#include "Shade_Impl.hpp"

#include <utilities/idd/OS_WindowMaterial_Shade_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    Shade_Impl::Shade_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle) : ShadingMaterial_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == Shade::iddObjectType());
    }

    Shade_Impl::Shade_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ShadingMaterial_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == Shade::iddObjectType());
    }

    Shade_Impl::Shade_Impl(const Shade_Impl& other, Model_Impl* model, bool keepHandle) : ShadingMaterial_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& Shade_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType Shade_Impl::iddObjectType() const {
      return Shade::iddObjectType();
    }

    double Shade_Impl::visibleTransmittance() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_ShadeFields::VisibleTransmittance, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> Shade_Impl::getVisibleTransmittance() const {
      OptionalDouble od = visibleTransmittance();
      return od;
    }

    double Shade_Impl::thermalHemisphericalEmissivity() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_ShadeFields::ThermalHemisphericalEmissivity, true);
      OS_ASSERT(value);
      return value.get();
    }

    double Shade_Impl::thickness() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_ShadeFields::Thickness, true);
      OS_ASSERT(value);
      return value.get();
    }

    double Shade_Impl::thermalConductivity() const {
      OptionalDouble od = getDouble(OS_WindowMaterial_ShadeFields::Conductivity, true);
      if (!od) {
        LOG_AND_THROW("Conductivity not yet set for " << briefDescription() << ".");
      }
      return *od;
    }

    double Shade_Impl::thermalConductance() const {
      return thermalConductivity() / thickness();
    }

    double Shade_Impl::thermalResistivity() const {
      return 1.0 / thermalConductivity();
    }

    double Shade_Impl::thermalResistance() const {
      return 1.0 / thermalConductance();
    }

    double Shade_Impl::thermalTransmittance() const {
      OptionalDouble od = getDouble(OS_WindowMaterial_ShadeFields::ThermalTransmittance, true);
      if (!od) {
        LOG_AND_THROW("Thermal transmittance not yet set for " << briefDescription() << ".");
      }
      return *od;
    }

    double Shade_Impl::solarTransmittance() const {
      OptionalDouble od = getDouble(OS_WindowMaterial_ShadeFields::SolarTransmittance, true);
      if (!od) {
        LOG_AND_THROW("Solar transmittance not yet set for " << briefDescription() << ".");
      }
      return *od;
    }

    double Shade_Impl::solarAbsorptance() const {
      return 1.0 - solarTransmittance() - solarReflectance();
    }

    double Shade_Impl::solarReflectance() const {
      OptionalDouble od = getDouble(OS_WindowMaterial_ShadeFields::SolarReflectance, true);
      if (!od) {
        LOG_AND_THROW("Solar reflectance not yet set for " << briefDescription() << ".");
      }
      return *od;
    }

    double Shade_Impl::visibleAbsorptance() const {
      return 1.0 - visibleTransmittance() - visibleReflectance();
    }

    boost::optional<double> Shade_Impl::interiorVisibleAbsorptance() const {
      return visibleAbsorptance();
    }

    boost::optional<double> Shade_Impl::exteriorVisibleAbsorptance() const {
      return visibleAbsorptance();
    }

    double Shade_Impl::visibleReflectance() const {
      OptionalDouble od = getDouble(OS_WindowMaterial_ShadeFields::VisibleReflectance, true);
      if (!od) {
        LOG_AND_THROW("Visible reflectance not yet set for " << briefDescription() << ".");
      }
      return *od;
    }

    bool Shade_Impl::setThermalConductivity(double value) {
      return setDouble(OS_WindowMaterial_ShadeFields::Conductivity, value);
    }

    bool Shade_Impl::setThermalConductance(double value) {
      return setThickness(thermalConductivity() / value);
    }

    bool Shade_Impl::setThermalResistivity(double value) {
      return setThermalConductivity(1.0 / value);
    }

    bool Shade_Impl::setThermalResistance(double value) {
      return setThickness(value / thermalResistivity());
    }

    double Shade_Impl::conductivity() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_ShadeFields::Conductivity, true);
      OS_ASSERT(value);
      return value.get();
    }

    double Shade_Impl::shadetoGlassDistance() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_ShadeFields::ShadetoGlassDistance, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool Shade_Impl::isShadetoGlassDistanceDefaulted() const {
      return isEmpty(OS_WindowMaterial_ShadeFields::ShadetoGlassDistance);
    }

    double Shade_Impl::topOpeningMultiplier() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_ShadeFields::TopOpeningMultiplier, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool Shade_Impl::isTopOpeningMultiplierDefaulted() const {
      return isEmpty(OS_WindowMaterial_ShadeFields::TopOpeningMultiplier);
    }

    double Shade_Impl::bottomOpeningMultiplier() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_ShadeFields::BottomOpeningMultiplier, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool Shade_Impl::isBottomOpeningMultiplierDefaulted() const {
      return isEmpty(OS_WindowMaterial_ShadeFields::BottomOpeningMultiplier);
    }

    double Shade_Impl::leftSideOpeningMultiplier() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_ShadeFields::LeftSideOpeningMultiplier, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool Shade_Impl::isLeftSideOpeningMultiplierDefaulted() const {
      return isEmpty(OS_WindowMaterial_ShadeFields::LeftSideOpeningMultiplier);
    }

    double Shade_Impl::rightSideOpeningMultiplier() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_ShadeFields::RightSideOpeningMultiplier, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool Shade_Impl::isRightSideOpeningMultiplierDefaulted() const {
      return isEmpty(OS_WindowMaterial_ShadeFields::RightSideOpeningMultiplier);
    }

    double Shade_Impl::airflowPermeability() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_ShadeFields::AirflowPermeability, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool Shade_Impl::isAirflowPermeabilityDefaulted() const {
      return isEmpty(OS_WindowMaterial_ShadeFields::AirflowPermeability);
    }

    bool Shade_Impl::setSolarTransmittance(double solarTransmittance) {
      bool result = setDouble(OS_WindowMaterial_ShadeFields::SolarTransmittance, solarTransmittance);
      return result;
    }

    bool Shade_Impl::setSolarReflectance(double solarReflectance) {
      bool result = setDouble(OS_WindowMaterial_ShadeFields::SolarReflectance, solarReflectance);
      return result;
    }

    bool Shade_Impl::setVisibleTransmittance(double visibleTransmittance) {
      bool result = setDouble(OS_WindowMaterial_ShadeFields::VisibleTransmittance, visibleTransmittance);
      return result;
    }

    bool Shade_Impl::setVisibleReflectance(double visibleReflectance) {
      bool result = setDouble(OS_WindowMaterial_ShadeFields::VisibleReflectance, visibleReflectance);
      return result;
    }

    bool Shade_Impl::setThermalHemisphericalEmissivity(double thermalHemisphericalEmissivity) {
      bool result = setDouble(OS_WindowMaterial_ShadeFields::ThermalHemisphericalEmissivity, thermalHemisphericalEmissivity);
      return result;
    }

    bool Shade_Impl::setThermalTransmittance(double thermalTransmittance) {
      bool result = setDouble(OS_WindowMaterial_ShadeFields::ThermalTransmittance, thermalTransmittance);
      return result;
    }

    bool Shade_Impl::setThickness(double thickness) {
      bool result = setDouble(OS_WindowMaterial_ShadeFields::Thickness, thickness);
      return result;
    }

    bool Shade_Impl::setConductivity(double conductivity) {
      bool result = setDouble(OS_WindowMaterial_ShadeFields::Conductivity, conductivity);
      return result;
    }

    bool Shade_Impl::setShadetoGlassDistance(double shadetoGlassDistance) {
      bool result = setDouble(OS_WindowMaterial_ShadeFields::ShadetoGlassDistance, shadetoGlassDistance);
      return result;
    }

    void Shade_Impl::resetShadetoGlassDistance() {
      bool result = setString(OS_WindowMaterial_ShadeFields::ShadetoGlassDistance, "");
      OS_ASSERT(result);
    }

    bool Shade_Impl::setTopOpeningMultiplier(double topOpeningMultiplier) {
      bool result = setDouble(OS_WindowMaterial_ShadeFields::TopOpeningMultiplier, topOpeningMultiplier);
      return result;
    }

    void Shade_Impl::resetTopOpeningMultiplier() {
      bool result = setString(OS_WindowMaterial_ShadeFields::TopOpeningMultiplier, "");
      OS_ASSERT(result);
    }

    bool Shade_Impl::setBottomOpeningMultiplier(double bottomOpeningMultiplier) {
      bool result = setDouble(OS_WindowMaterial_ShadeFields::BottomOpeningMultiplier, bottomOpeningMultiplier);
      return result;
    }

    void Shade_Impl::resetBottomOpeningMultiplier() {
      bool result = setString(OS_WindowMaterial_ShadeFields::BottomOpeningMultiplier, "");
      OS_ASSERT(result);
    }

    bool Shade_Impl::setLeftSideOpeningMultiplier(double leftSideOpeningMultiplier) {
      bool result = setDouble(OS_WindowMaterial_ShadeFields::LeftSideOpeningMultiplier, leftSideOpeningMultiplier);
      return result;
    }

    void Shade_Impl::resetLeftSideOpeningMultiplier() {
      bool result = setString(OS_WindowMaterial_ShadeFields::LeftSideOpeningMultiplier, "");
      OS_ASSERT(result);
    }

    bool Shade_Impl::setRightSideOpeningMultiplier(double rightSideOpeningMultiplier) {
      bool result = setDouble(OS_WindowMaterial_ShadeFields::RightSideOpeningMultiplier, rightSideOpeningMultiplier);
      return result;
    }

    void Shade_Impl::resetRightSideOpeningMultiplier() {
      bool result = setString(OS_WindowMaterial_ShadeFields::RightSideOpeningMultiplier, "");
      OS_ASSERT(result);
    }

    bool Shade_Impl::setAirflowPermeability(double airflowPermeability) {
      bool result = setDouble(OS_WindowMaterial_ShadeFields::AirflowPermeability, airflowPermeability);
      return result;
    }

    void Shade_Impl::resetAirflowPermeability() {
      bool result = setString(OS_WindowMaterial_ShadeFields::AirflowPermeability, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  Shade::Shade(const Model& model, double solarTransmittance, double solarReflectance, double visibleTransmittance, double visibleReflectance,
               double thermalHemisphericalEmissivity, double thermalTransmittance, double thickness, double conductivity)
    : ShadingMaterial(Shade::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::Shade_Impl>());

    // TODO: Appropriately handle the following required object-list fields.
    bool ok = true;
    // ok = setHandle();
    OS_ASSERT(ok);
    ok = setSolarTransmittance(solarTransmittance);
    OS_ASSERT(ok);
    ok = setSolarReflectance(solarReflectance);
    OS_ASSERT(ok);
    ok = setVisibleTransmittance(visibleTransmittance);
    OS_ASSERT(ok);
    ok = setVisibleReflectance(visibleReflectance);
    OS_ASSERT(ok);
    ok = setThermalHemisphericalEmissivity(thermalHemisphericalEmissivity);
    OS_ASSERT(ok);
    ok = setThermalTransmittance(thermalTransmittance);
    OS_ASSERT(ok);
    ok = setThickness(thickness);
    OS_ASSERT(ok);
    ok = setConductivity(conductivity);
    OS_ASSERT(ok);
  }

  IddObjectType Shade::iddObjectType() {
    return {IddObjectType::OS_WindowMaterial_Shade};
  }

  double Shade::thermalConductivity() const {
    return getImpl<detail::Shade_Impl>()->thermalConductivity();
  }

  double Shade::thermalConductance() const {
    return getImpl<detail::Shade_Impl>()->thermalConductance();
  }

  double Shade::thermalResistivity() const {
    return getImpl<detail::Shade_Impl>()->thermalResistivity();
  }

  double Shade::thermalResistance() const {
    return getImpl<detail::Shade_Impl>()->thermalResistance();
  }

  double Shade::visibleTransmittance() const {
    return getImpl<detail::Shade_Impl>()->visibleTransmittance();
  }

  double Shade::thermalHemisphericalEmissivity() const {
    return getImpl<detail::Shade_Impl>()->thermalHemisphericalEmissivity();
  }

  double Shade::thermalTransmittance() const {
    return getImpl<detail::Shade_Impl>()->thermalTransmittance();
  }

  double Shade::solarTransmittance() const {
    return getImpl<detail::Shade_Impl>()->solarTransmittance();
  }

  double Shade::solarAbsorptance() const {
    return getImpl<detail::Shade_Impl>()->solarAbsorptance();
  }

  double Shade::solarReflectance() const {
    return getImpl<detail::Shade_Impl>()->solarReflectance();
  }

  double Shade::visibleAbsorptance() const {
    return getImpl<detail::Shade_Impl>()->visibleAbsorptance();
  }

  double Shade::visibleReflectance() const {
    return getImpl<detail::Shade_Impl>()->visibleReflectance();
  }

  bool Shade::setThermalConductivity(double value) {
    return getImpl<detail::Shade_Impl>()->setThermalConductivity(value);
  }

  bool Shade::setThermalConductance(double value) {
    return getImpl<detail::Shade_Impl>()->setThermalConductance(value);
  }

  bool Shade::setThermalResistivity(double value) {
    return getImpl<detail::Shade_Impl>()->setThermalResistivity(value);
  }

  bool Shade::setThermalResistance(double value) {
    return getImpl<detail::Shade_Impl>()->setThermalResistance(value);
  }

  bool Shade::setThermalTransmittance(double thermalTransmittance) {
    return getImpl<detail::Shade_Impl>()->setThermalTransmittance(thermalTransmittance);
  }

  // cppcheck-suppress [duplInheritedMember] for documentation purposes
  double Shade::thickness() const {
    return getImpl<detail::Shade_Impl>()->thickness();
  }

  double Shade::conductivity() const {
    return getImpl<detail::Shade_Impl>()->conductivity();
  }

  double Shade::shadetoGlassDistance() const {
    return getImpl<detail::Shade_Impl>()->shadetoGlassDistance();
  }

  bool Shade::isShadetoGlassDistanceDefaulted() const {
    return getImpl<detail::Shade_Impl>()->isShadetoGlassDistanceDefaulted();
  }

  double Shade::topOpeningMultiplier() const {
    return getImpl<detail::Shade_Impl>()->topOpeningMultiplier();
  }

  bool Shade::isTopOpeningMultiplierDefaulted() const {
    return getImpl<detail::Shade_Impl>()->isTopOpeningMultiplierDefaulted();
  }

  double Shade::bottomOpeningMultiplier() const {
    return getImpl<detail::Shade_Impl>()->bottomOpeningMultiplier();
  }

  bool Shade::isBottomOpeningMultiplierDefaulted() const {
    return getImpl<detail::Shade_Impl>()->isBottomOpeningMultiplierDefaulted();
  }

  double Shade::leftSideOpeningMultiplier() const {
    return getImpl<detail::Shade_Impl>()->leftSideOpeningMultiplier();
  }

  bool Shade::isLeftSideOpeningMultiplierDefaulted() const {
    return getImpl<detail::Shade_Impl>()->isLeftSideOpeningMultiplierDefaulted();
  }

  double Shade::rightSideOpeningMultiplier() const {
    return getImpl<detail::Shade_Impl>()->rightSideOpeningMultiplier();
  }

  bool Shade::isRightSideOpeningMultiplierDefaulted() const {
    return getImpl<detail::Shade_Impl>()->isRightSideOpeningMultiplierDefaulted();
  }

  double Shade::airflowPermeability() const {
    return getImpl<detail::Shade_Impl>()->airflowPermeability();
  }

  bool Shade::isAirflowPermeabilityDefaulted() const {
    return getImpl<detail::Shade_Impl>()->isAirflowPermeabilityDefaulted();
  }

  bool Shade::setSolarTransmittance(double solarTransmittance) {
    return getImpl<detail::Shade_Impl>()->setSolarTransmittance(solarTransmittance);
  }

  bool Shade::setSolarReflectance(double solarReflectance) {
    return getImpl<detail::Shade_Impl>()->setSolarReflectance(solarReflectance);
  }

  bool Shade::setVisibleTransmittance(double visibleTransmittance) {
    return getImpl<detail::Shade_Impl>()->setVisibleTransmittance(visibleTransmittance);
  }

  bool Shade::setVisibleReflectance(double visibleReflectance) {
    return getImpl<detail::Shade_Impl>()->setVisibleReflectance(visibleReflectance);
  }

  bool Shade::setThermalHemisphericalEmissivity(double thermalHemisphericalEmissivity) {
    return getImpl<detail::Shade_Impl>()->setThermalHemisphericalEmissivity(thermalHemisphericalEmissivity);
  }

  // cppcheck-suppress [duplInheritedMember] for documentation purposes
  bool Shade::setThickness(double thickness) {
    return getImpl<detail::Shade_Impl>()->setThickness(thickness);
  }

  bool Shade::setConductivity(double conductivity) {
    return getImpl<detail::Shade_Impl>()->setConductivity(conductivity);
  }

  bool Shade::setShadetoGlassDistance(double shadetoGlassDistance) {
    return getImpl<detail::Shade_Impl>()->setShadetoGlassDistance(shadetoGlassDistance);
  }

  void Shade::resetShadetoGlassDistance() {
    getImpl<detail::Shade_Impl>()->resetShadetoGlassDistance();
  }

  bool Shade::setTopOpeningMultiplier(double topOpeningMultiplier) {
    return getImpl<detail::Shade_Impl>()->setTopOpeningMultiplier(topOpeningMultiplier);
  }

  void Shade::resetTopOpeningMultiplier() {
    getImpl<detail::Shade_Impl>()->resetTopOpeningMultiplier();
  }

  bool Shade::setBottomOpeningMultiplier(double bottomOpeningMultiplier) {
    return getImpl<detail::Shade_Impl>()->setBottomOpeningMultiplier(bottomOpeningMultiplier);
  }

  void Shade::resetBottomOpeningMultiplier() {
    getImpl<detail::Shade_Impl>()->resetBottomOpeningMultiplier();
  }

  bool Shade::setLeftSideOpeningMultiplier(double leftSideOpeningMultiplier) {
    return getImpl<detail::Shade_Impl>()->setLeftSideOpeningMultiplier(leftSideOpeningMultiplier);
  }

  void Shade::resetLeftSideOpeningMultiplier() {
    getImpl<detail::Shade_Impl>()->resetLeftSideOpeningMultiplier();
  }

  bool Shade::setRightSideOpeningMultiplier(double rightSideOpeningMultiplier) {
    return getImpl<detail::Shade_Impl>()->setRightSideOpeningMultiplier(rightSideOpeningMultiplier);
  }

  void Shade::resetRightSideOpeningMultiplier() {
    getImpl<detail::Shade_Impl>()->resetRightSideOpeningMultiplier();
  }

  bool Shade::setAirflowPermeability(double airflowPermeability) {
    return getImpl<detail::Shade_Impl>()->setAirflowPermeability(airflowPermeability);
  }

  void Shade::resetAirflowPermeability() {
    getImpl<detail::Shade_Impl>()->resetAirflowPermeability();
  }

  /// @cond
  Shade::Shade(std::shared_ptr<detail::Shade_Impl> impl) : ShadingMaterial(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
