/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "MasslessOpaqueMaterial.hpp"
#include "MasslessOpaqueMaterial_Impl.hpp"
#include "StandardsInformationMaterial.hpp"
#include "StandardsInformationMaterial_Impl.hpp"
#include "MaterialPropertyMoisturePenetrationDepthSettings.hpp"
#include "MaterialPropertyMoisturePenetrationDepthSettings_Impl.hpp"
#include "MaterialPropertyPhaseChange.hpp"
#include "MaterialPropertyPhaseChange_Impl.hpp"
#include "MaterialPropertyPhaseChangeHysteresis.hpp"
#include "MaterialPropertyPhaseChangeHysteresis_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Material_NoMass_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    MasslessOpaqueMaterial_Impl::MasslessOpaqueMaterial_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : OpaqueMaterial_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == MasslessOpaqueMaterial::iddObjectType());
    }

    MasslessOpaqueMaterial_Impl::MasslessOpaqueMaterial_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                             bool keepHandle)
      : OpaqueMaterial_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == MasslessOpaqueMaterial::iddObjectType());
    }

    MasslessOpaqueMaterial_Impl::MasslessOpaqueMaterial_Impl(const MasslessOpaqueMaterial_Impl& other, Model_Impl* model, bool keepHandle)
      : OpaqueMaterial_Impl(other, model, keepHandle) {}

    std::vector<ModelObject> MasslessOpaqueMaterial_Impl::children() const {
      std::vector<ModelObject> results(castVector<ModelObject>(getObject<Material>().getModelObjectSources<StandardsInformationMaterial>()));

      if (boost::optional<MaterialPropertyMoisturePenetrationDepthSettings> empd = this->materialPropertyMoisturePenetrationDepthSettings()) {
        results.push_back(empd.get());
      }

      if (boost::optional<MaterialPropertyPhaseChange> phaseChange = this->materialPropertyPhaseChange()) {
        results.push_back(phaseChange.get());
      }

      if (boost::optional<MaterialPropertyPhaseChangeHysteresis> phaseChangeHysteresis = this->materialPropertyPhaseChangeHysteresis()) {
        results.push_back(phaseChangeHysteresis.get());
      }

      return results;
    }

    std::string MasslessOpaqueMaterial_Impl::roughness() const {
      boost::optional<std::string> value = getString(OS_Material_NoMassFields::Roughness, true);
      OS_ASSERT(value);
      return value.get();
    }

    double MasslessOpaqueMaterial_Impl::thermalConductivity() const {
      LOG_AND_THROW("Unable to convert thermal resistance to thermal conductivity for " << "MasslessOpaqueMaterial " << briefDescription() << ".");
      return 0.0;
    }

    double MasslessOpaqueMaterial_Impl::thermalConductance() const {
      return 1.0 / thermalResistance();
    }

    double MasslessOpaqueMaterial_Impl::thermalResistivity() const {
      LOG_AND_THROW("Unable to convert thermal resistance to thermal resistivity for " << "MasslessOpaqueMaterial " << briefDescription() << ".");
      return 0.0;
    }

    double MasslessOpaqueMaterial_Impl::thermalAbsorptance() const {
      boost::optional<double> value = getDouble(OS_Material_NoMassFields::ThermalAbsorptance, true);
      OS_ASSERT(value);
      return value.get();
    }

    OptionalDouble MasslessOpaqueMaterial_Impl::thermalReflectance() const {
      OptionalDouble result;
      OptionalDouble od = thermalAbsorptance();
      if (od) {
        result = (1.0 - *od);
      }
      return result;
    }

    OptionalDouble MasslessOpaqueMaterial_Impl::solarReflectance() const {
      OptionalDouble result;
      OptionalDouble od = solarAbsorptance();
      if (od) {
        result = (1.0 - *od);
      }
      return result;
    }

    double MasslessOpaqueMaterial_Impl::visibleAbsorptance() const {
      boost::optional<double> value = getDouble(OS_Material_NoMassFields::VisibleAbsorptance, true);
      OS_ASSERT(value);
      return value.get();
    }

    OptionalDouble MasslessOpaqueMaterial_Impl::visibleReflectance() const {
      OptionalDouble result;
      OptionalDouble od = visibleAbsorptance();
      if (od) {
        result = (1.0 - *od);
      }
      return result;
    }

    const std::vector<std::string>& MasslessOpaqueMaterial_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType MasslessOpaqueMaterial_Impl::iddObjectType() const {
      return MasslessOpaqueMaterial::iddObjectType();
    }

    bool MasslessOpaqueMaterial_Impl::setThickness(double /*value*/) {
      return false;
    }

    bool MasslessOpaqueMaterial_Impl::setThermalConductivity(double /*value*/) {
      return false;
    }

    bool MasslessOpaqueMaterial_Impl::setThermalConductance(double value) {
      return setThermalResistance(1.0 / value);
    }

    bool MasslessOpaqueMaterial_Impl::setThermalResistivity(double /*value*/) {
      return false;
    }

    bool MasslessOpaqueMaterial_Impl::setThermalAbsorptance(OptionalDouble value) {
      if (!value) {
        return setString(OS_Material_NoMassFields::ThermalAbsorptance, "");
      }
      return setDouble(OS_Material_NoMassFields::ThermalAbsorptance, *value);
    }

    bool MasslessOpaqueMaterial_Impl::setThermalReflectance(OptionalDouble value) {
      if (!value) {
        return setThermalAbsorptance(value);
      }
      OptionalDouble od = (1.0 - *value);
      return setThermalAbsorptance(od);
    }

    bool MasslessOpaqueMaterial_Impl::setSolarAbsorptance(OptionalDouble value) {
      if (!value) {
        return setString(OS_Material_NoMassFields::SolarAbsorptance, "");
      }
      return setDouble(OS_Material_NoMassFields::SolarAbsorptance, *value);
    }

    bool MasslessOpaqueMaterial_Impl::setSolarReflectance(OptionalDouble value) {
      if (!value) {
        return setSolarAbsorptance(value);
      }
      OptionalDouble od = (1.0 - *value);
      return setSolarAbsorptance(od);
    }

    bool MasslessOpaqueMaterial_Impl::setVisibleAbsorptance(OptionalDouble value) {
      if (!value) {
        return setString(OS_Material_NoMassFields::VisibleAbsorptance, "");
      }
      return setDouble(OS_Material_NoMassFields::VisibleAbsorptance, *value);
    }

    bool MasslessOpaqueMaterial_Impl::setVisibleReflectance(OptionalDouble value) {
      if (!value) {
        return setVisibleAbsorptance(value);
      }
      OptionalDouble od = (1.0 - *value);
      return setVisibleAbsorptance(od);
    }

    double MasslessOpaqueMaterial_Impl::thermalResistance() const {
      boost::optional<double> value = getDouble(OS_Material_NoMassFields::ThermalResistance, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool MasslessOpaqueMaterial_Impl::isThermalAbsorptanceDefaulted() const {
      return isEmpty(OS_Material_NoMassFields::ThermalAbsorptance);
    }

    double MasslessOpaqueMaterial_Impl::solarAbsorptance() const {
      boost::optional<double> value = getDouble(OS_Material_NoMassFields::SolarAbsorptance, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool MasslessOpaqueMaterial_Impl::isSolarAbsorptanceDefaulted() const {
      return isEmpty(OS_Material_NoMassFields::SolarAbsorptance);
    }

    bool MasslessOpaqueMaterial_Impl::isVisibleAbsorptanceDefaulted() const {
      return isEmpty(OS_Material_NoMassFields::VisibleAbsorptance);
    }

    bool MasslessOpaqueMaterial_Impl::setRoughness(const std::string& roughness) {
      bool result = setString(OS_Material_NoMassFields::Roughness, roughness);
      return result;
    }

    bool MasslessOpaqueMaterial_Impl::setThermalResistance(double thermalResistance) {
      bool result = setDouble(OS_Material_NoMassFields::ThermalResistance, thermalResistance);
      return result;
    }

    bool MasslessOpaqueMaterial_Impl::setThermalAbsorptance(double thermalAbsorptance) {
      bool result = setDouble(OS_Material_NoMassFields::ThermalAbsorptance, thermalAbsorptance);
      return result;
    }

    void MasslessOpaqueMaterial_Impl::resetThermalAbsorptance() {
      bool result = setString(OS_Material_NoMassFields::ThermalAbsorptance, "");
      OS_ASSERT(result);
    }

    bool MasslessOpaqueMaterial_Impl::setSolarAbsorptance(double solarAbsorptance) {
      bool result = setDouble(OS_Material_NoMassFields::SolarAbsorptance, solarAbsorptance);
      return result;
    }

    void MasslessOpaqueMaterial_Impl::resetSolarAbsorptance() {
      bool result = setString(OS_Material_NoMassFields::SolarAbsorptance, "");
      OS_ASSERT(result);
    }

    bool MasslessOpaqueMaterial_Impl::setVisibleAbsorptance(double visibleAbsorptance) {
      bool result = setDouble(OS_Material_NoMassFields::VisibleAbsorptance, visibleAbsorptance);
      return result;
    }

    void MasslessOpaqueMaterial_Impl::resetVisibleAbsorptance() {
      bool result = setString(OS_Material_NoMassFields::VisibleAbsorptance, "");
      OS_ASSERT(result);
    }

    std::vector<std::string> MasslessOpaqueMaterial_Impl::roughnessValues() const {
      return MasslessOpaqueMaterial::roughnessValues();
    }

    boost::optional<MaterialPropertyMoisturePenetrationDepthSettings>
      MasslessOpaqueMaterial_Impl::createMaterialPropertyMoisturePenetrationDepthSettings(
        double waterVaporDiffusionResistanceFactor, double moistureEquationCoefficientA, double moistureEquationCoefficientB,
        double moistureEquationCoefficientC, double moistureEquationCoefficientD, double coatingLayerThickness,
        double coatingLayerWaterVaporDiffusionResistanceFactor) {
      auto thisMaterial = getObject<MasslessOpaqueMaterial>();
      std::vector<MaterialPropertyMoisturePenetrationDepthSettings> empds =
        thisMaterial.getModelObjectSources<MaterialPropertyMoisturePenetrationDepthSettings>(
          MaterialPropertyMoisturePenetrationDepthSettings::iddObjectType());
      if (!empds.empty()) {
        return boost::none;
      }

      MaterialPropertyMoisturePenetrationDepthSettings empd(thisMaterial, waterVaporDiffusionResistanceFactor, moistureEquationCoefficientA,
                                                            moistureEquationCoefficientB, moistureEquationCoefficientC, moistureEquationCoefficientD,
                                                            coatingLayerThickness, coatingLayerWaterVaporDiffusionResistanceFactor);
      return empd;
    }

    boost::optional<MaterialPropertyMoisturePenetrationDepthSettings>
      MasslessOpaqueMaterial_Impl::materialPropertyMoisturePenetrationDepthSettings() const {
      std::vector<MaterialPropertyMoisturePenetrationDepthSettings> empds =
        getObject<ModelObject>().getModelObjectSources<MaterialPropertyMoisturePenetrationDepthSettings>(
          MaterialPropertyMoisturePenetrationDepthSettings::iddObjectType());
      if (empds.empty()) {
        // no error
      } else if (empds.size() == 1) {
        return empds[0];
      } else {
        // error
      }
      return boost::none;
    }

    void MasslessOpaqueMaterial_Impl::resetMaterialPropertyMoisturePenetrationDepthSettings() {
      boost::optional<MaterialPropertyMoisturePenetrationDepthSettings> empd = this->materialPropertyMoisturePenetrationDepthSettings();
      if (empd) {
        empd->remove();
      }
    }

    boost::optional<MaterialPropertyPhaseChange> MasslessOpaqueMaterial_Impl::createMaterialPropertyPhaseChange() {
      auto thisMaterial = getObject<MasslessOpaqueMaterial>();
      std::vector<MaterialPropertyPhaseChange> phaseChanges =
        thisMaterial.getModelObjectSources<MaterialPropertyPhaseChange>(MaterialPropertyPhaseChange::iddObjectType());
      if (!phaseChanges.empty()) {
        return boost::none;
      }

      MaterialPropertyPhaseChange phaseChange(thisMaterial);
      return phaseChange;
    }

    boost::optional<MaterialPropertyPhaseChange>
      MasslessOpaqueMaterial_Impl::createMaterialPropertyPhaseChange(const std::vector<TemperatureEnthalpy>& temperatureEnthalpys) {
      auto thisMaterial = getObject<MasslessOpaqueMaterial>();
      std::vector<MaterialPropertyPhaseChange> phaseChanges =
        thisMaterial.getModelObjectSources<MaterialPropertyPhaseChange>(MaterialPropertyPhaseChange::iddObjectType());
      if (!phaseChanges.empty()) {
        return boost::none;
      }

      MaterialPropertyPhaseChange phaseChange(thisMaterial, temperatureEnthalpys);
      return phaseChange;
    }

    boost::optional<MaterialPropertyPhaseChange> MasslessOpaqueMaterial_Impl::materialPropertyPhaseChange() const {
      std::vector<MaterialPropertyPhaseChange> phaseChanges =
        getObject<ModelObject>().getModelObjectSources<MaterialPropertyPhaseChange>(MaterialPropertyPhaseChange::iddObjectType());
      if (phaseChanges.empty()) {
        // no error
      } else if (phaseChanges.size() == 1) {
        return phaseChanges[0];
      } else {
        // error
      }
      return boost::none;
    }

    void MasslessOpaqueMaterial_Impl::resetMaterialPropertyPhaseChange() {
      boost::optional<MaterialPropertyPhaseChange> phaseChange = this->materialPropertyPhaseChange();
      if (phaseChange) {
        phaseChange->remove();
      }
    }

    boost::optional<MaterialPropertyPhaseChangeHysteresis> MasslessOpaqueMaterial_Impl::createMaterialPropertyPhaseChangeHysteresis() {
      auto thisMaterial = getObject<MasslessOpaqueMaterial>();
      std::vector<MaterialPropertyPhaseChangeHysteresis> phaseChangeHysteresiss =
        thisMaterial.getModelObjectSources<MaterialPropertyPhaseChangeHysteresis>(MaterialPropertyPhaseChangeHysteresis::iddObjectType());
      if (!phaseChangeHysteresiss.empty()) {
        return boost::none;
      }

      MaterialPropertyPhaseChangeHysteresis phaseChangeHysteresis(thisMaterial);
      return phaseChangeHysteresis;
    }

    boost::optional<MaterialPropertyPhaseChangeHysteresis> MasslessOpaqueMaterial_Impl::materialPropertyPhaseChangeHysteresis() const {
      std::vector<MaterialPropertyPhaseChangeHysteresis> phaseChangeHysteresiss =
        getObject<ModelObject>().getModelObjectSources<MaterialPropertyPhaseChangeHysteresis>(MaterialPropertyPhaseChangeHysteresis::iddObjectType());
      if (phaseChangeHysteresiss.empty()) {
        // no error
      } else if (phaseChangeHysteresiss.size() == 1) {
        return phaseChangeHysteresiss[0];
      } else {
        // error
      }
      return boost::none;
    }

    void MasslessOpaqueMaterial_Impl::resetMaterialPropertyPhaseChangeHysteresis() {
      boost::optional<MaterialPropertyPhaseChangeHysteresis> phaseChangeHysteresis = this->materialPropertyPhaseChangeHysteresis();
      if (phaseChangeHysteresis) {
        phaseChangeHysteresis->remove();
      }
    }

  }  // namespace detail

  MasslessOpaqueMaterial::MasslessOpaqueMaterial(const Model& model, const std::string& roughness, double thermalResistance)
    : OpaqueMaterial(MasslessOpaqueMaterial::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::MasslessOpaqueMaterial_Impl>());

    // TODO: Appropriately handle the following required object-list fields.
    bool ok = true;
    //ok = setHandle();
    OS_ASSERT(ok);
    ok = setRoughness(roughness);
    OS_ASSERT(ok);
    ok = setThermalResistance(thermalResistance);
    OS_ASSERT(ok);
  }

  IddObjectType MasslessOpaqueMaterial::iddObjectType() {
    return {IddObjectType::OS_Material_NoMass};
  }

  std::vector<std::string> MasslessOpaqueMaterial::roughnessValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Material_NoMassFields::Roughness);
  }

  std::string MasslessOpaqueMaterial::roughness() const {
    return getImpl<detail::MasslessOpaqueMaterial_Impl>()->roughness();
  }

  // cppcheck-suppress [duplInheritedMember] for documentation purposes
  boost::optional<double> MasslessOpaqueMaterial::thermalAbsorptance() const {
    OptionalDouble result = getImpl<detail::MasslessOpaqueMaterial_Impl>()->thermalAbsorptance();
    return result;
  }

  // cppcheck-suppress [duplInheritedMember] for documentation purposes
  boost::optional<double> MasslessOpaqueMaterial::solarAbsorptance() const {
    OptionalDouble result = getImpl<detail::MasslessOpaqueMaterial_Impl>()->solarAbsorptance();
    return result;
  }

  // cppcheck-suppress [duplInheritedMember] for documentation purposes
  boost::optional<double> MasslessOpaqueMaterial::visibleAbsorptance() const {
    OptionalDouble result = getImpl<detail::MasslessOpaqueMaterial_Impl>()->visibleAbsorptance();
    return result;
  }

  // cppcheck-suppress [duplInheritedMember] for documentation purposes
  double MasslessOpaqueMaterial::thermalResistance() const {
    return getImpl<detail::MasslessOpaqueMaterial_Impl>()->thermalResistance();
  }

  bool MasslessOpaqueMaterial::isThermalAbsorptanceDefaulted() const {
    return getImpl<detail::MasslessOpaqueMaterial_Impl>()->isThermalAbsorptanceDefaulted();
  }

  bool MasslessOpaqueMaterial::isSolarAbsorptanceDefaulted() const {
    return getImpl<detail::MasslessOpaqueMaterial_Impl>()->isSolarAbsorptanceDefaulted();
  }

  bool MasslessOpaqueMaterial::isVisibleAbsorptanceDefaulted() const {
    return getImpl<detail::MasslessOpaqueMaterial_Impl>()->isVisibleAbsorptanceDefaulted();
  }

  bool MasslessOpaqueMaterial::setRoughness(const std::string& roughness) {
    return getImpl<detail::MasslessOpaqueMaterial_Impl>()->setRoughness(roughness);
  }

  // cppcheck-suppress [duplInheritedMember] for documentation purposes
  bool MasslessOpaqueMaterial::setThermalResistance(double thermalResistance) {
    return getImpl<detail::MasslessOpaqueMaterial_Impl>()->setThermalResistance(thermalResistance);
  }

  // cppcheck-suppress [duplInheritedMember] for documentation purposes
  bool MasslessOpaqueMaterial::setThermalAbsorptance(double thermalAbsorptance) {
    return getImpl<detail::MasslessOpaqueMaterial_Impl>()->setThermalAbsorptance(thermalAbsorptance);
  }

  void MasslessOpaqueMaterial::resetThermalAbsorptance() {
    getImpl<detail::MasslessOpaqueMaterial_Impl>()->resetThermalAbsorptance();
  }

  bool MasslessOpaqueMaterial::setSolarAbsorptance(double solarAbsorptance) {
    return getImpl<detail::MasslessOpaqueMaterial_Impl>()->setSolarAbsorptance(solarAbsorptance);
  }

  void MasslessOpaqueMaterial::resetSolarAbsorptance() {
    getImpl<detail::MasslessOpaqueMaterial_Impl>()->resetSolarAbsorptance();
  }

  bool MasslessOpaqueMaterial::setVisibleAbsorptance(double visibleAbsorptance) {
    return getImpl<detail::MasslessOpaqueMaterial_Impl>()->setVisibleAbsorptance(visibleAbsorptance);
  }

  void MasslessOpaqueMaterial::resetVisibleAbsorptance() {
    getImpl<detail::MasslessOpaqueMaterial_Impl>()->resetVisibleAbsorptance();
  }

  boost::optional<MaterialPropertyMoisturePenetrationDepthSettings> MasslessOpaqueMaterial::createMaterialPropertyMoisturePenetrationDepthSettings(
    double waterVaporDiffusionResistanceFactor, double moistureEquationCoefficientA, double moistureEquationCoefficientB,
    double moistureEquationCoefficientC, double moistureEquationCoefficientD, double coatingLayerThickness,
    double coatingLayerWaterVaporDiffusionResistanceFactor) {
    return getImpl<detail::MasslessOpaqueMaterial_Impl>()->createMaterialPropertyMoisturePenetrationDepthSettings(
      waterVaporDiffusionResistanceFactor, moistureEquationCoefficientA, moistureEquationCoefficientB, moistureEquationCoefficientC,
      moistureEquationCoefficientD, coatingLayerThickness, coatingLayerWaterVaporDiffusionResistanceFactor);
  }

  boost::optional<MaterialPropertyMoisturePenetrationDepthSettings> MasslessOpaqueMaterial::materialPropertyMoisturePenetrationDepthSettings() const {
    return getImpl<detail::MasslessOpaqueMaterial_Impl>()->materialPropertyMoisturePenetrationDepthSettings();
  }

  void MasslessOpaqueMaterial::resetMaterialPropertyMoisturePenetrationDepthSettings() {
    getImpl<detail::MasslessOpaqueMaterial_Impl>()->resetMaterialPropertyMoisturePenetrationDepthSettings();
  }

  boost::optional<MaterialPropertyPhaseChange> MasslessOpaqueMaterial::createMaterialPropertyPhaseChange() {
    return getImpl<detail::MasslessOpaqueMaterial_Impl>()->createMaterialPropertyPhaseChange();
  }

  boost::optional<MaterialPropertyPhaseChange>
    MasslessOpaqueMaterial::createMaterialPropertyPhaseChange(const std::vector<TemperatureEnthalpy>& temperatureEnthalpys) {
    return getImpl<detail::MasslessOpaqueMaterial_Impl>()->createMaterialPropertyPhaseChange(temperatureEnthalpys);
  }

  boost::optional<MaterialPropertyPhaseChange> MasslessOpaqueMaterial::materialPropertyPhaseChange() const {
    return getImpl<detail::MasslessOpaqueMaterial_Impl>()->materialPropertyPhaseChange();
  }

  void MasslessOpaqueMaterial::resetMaterialPropertyPhaseChange() {
    getImpl<detail::MasslessOpaqueMaterial_Impl>()->resetMaterialPropertyPhaseChange();
  }

  boost::optional<MaterialPropertyPhaseChangeHysteresis> MasslessOpaqueMaterial::createMaterialPropertyPhaseChangeHysteresis() {
    return getImpl<detail::MasslessOpaqueMaterial_Impl>()->createMaterialPropertyPhaseChangeHysteresis();
  }

  boost::optional<MaterialPropertyPhaseChangeHysteresis> MasslessOpaqueMaterial::materialPropertyPhaseChangeHysteresis() const {
    return getImpl<detail::MasslessOpaqueMaterial_Impl>()->materialPropertyPhaseChangeHysteresis();
  }

  void MasslessOpaqueMaterial::resetMaterialPropertyPhaseChangeHysteresis() {
    getImpl<detail::MasslessOpaqueMaterial_Impl>()->resetMaterialPropertyPhaseChangeHysteresis();
  }

  /// @cond
  MasslessOpaqueMaterial::MasslessOpaqueMaterial(std::shared_ptr<detail::MasslessOpaqueMaterial_Impl> impl) : OpaqueMaterial(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
