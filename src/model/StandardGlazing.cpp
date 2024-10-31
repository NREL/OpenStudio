/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "StandardGlazing.hpp"
#include "StandardGlazing_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include "MaterialPropertyGlazingSpectralData.hpp"
#include "MaterialPropertyGlazingSpectralData_Impl.hpp"

#include <utilities/idd/OS_WindowMaterial_Glazing_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    StandardGlazing_Impl::StandardGlazing_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : Glazing_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == StandardGlazing::iddObjectType());
    }

    StandardGlazing_Impl::StandardGlazing_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : Glazing_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == StandardGlazing::iddObjectType());
    }

    StandardGlazing_Impl::StandardGlazing_Impl(const StandardGlazing_Impl& other, Model_Impl* model, bool keepHandle)
      : Glazing_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& StandardGlazing_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType StandardGlazing_Impl::iddObjectType() const {
      return StandardGlazing::iddObjectType();
    }

    std::string StandardGlazing_Impl::opticalDataType() const {
      boost::optional<std::string> value = getString(OS_WindowMaterial_GlazingFields::OpticalDataType, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<std::string> StandardGlazing_Impl::windowGlassSpectralDataSetName() const {
      LOG(Warn, "StandardGlazing::windowGlassSpectralDataSetName is deprecated, use StandardGlazing::windowGlassSpectralDataSet instead");
      boost::optional<MaterialPropertyGlazingSpectralData> spectralData = windowGlassSpectralDataSet();
      if (spectralData) {
        return spectralData->name();
      }
      return boost::none;
    }

    boost::optional<MaterialPropertyGlazingSpectralData> StandardGlazing_Impl::windowGlassSpectralDataSet() const {
      return getObject<ModelObject>().getModelObjectTarget<MaterialPropertyGlazingSpectralData>(
        OS_WindowMaterial_GlazingFields::WindowGlassSpectralDataSetName);
    }

    double StandardGlazing_Impl::thickness() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_GlazingFields::Thickness, true);
      OS_ASSERT(value);
      return value.get();
    }

    double StandardGlazing_Impl::solarTransmittance() const {
      OptionalDouble od = getDouble(OS_WindowMaterial_GlazingFields::SolarTransmittanceatNormalIncidence, true);
      if (!od) {
        LOG_AND_THROW("Solar transmittance is not yet set for " << briefDescription() << ".");
      }
      return *od;
    }

    boost::optional<double> StandardGlazing_Impl::solarTransmittanceatNormalIncidence() const {
      return getDouble(OS_WindowMaterial_GlazingFields::SolarTransmittanceatNormalIncidence, true);
    }

    boost::optional<double> StandardGlazing_Impl::frontSideSolarReflectanceatNormalIncidence() const {
      return getDouble(OS_WindowMaterial_GlazingFields::FrontSideSolarReflectanceatNormalIncidence, true);
    }

    boost::optional<double> StandardGlazing_Impl::backSideSolarReflectanceatNormalIncidence() const {
      return getDouble(OS_WindowMaterial_GlazingFields::BackSideSolarReflectanceatNormalIncidence, true);
    }

    OptionalDouble StandardGlazing_Impl::getVisibleTransmittance() const {
      OptionalDouble od = getDouble(OS_WindowMaterial_GlazingFields::VisibleTransmittanceatNormalIncidence, true);
      return *od;
    }

    boost::optional<double> StandardGlazing_Impl::visibleTransmittanceatNormalIncidence() const {
      return getDouble(OS_WindowMaterial_GlazingFields::VisibleTransmittanceatNormalIncidence, true);
    }

    boost::optional<double> StandardGlazing_Impl::frontSideVisibleReflectanceatNormalIncidence() const {
      return getDouble(OS_WindowMaterial_GlazingFields::FrontSideVisibleReflectanceatNormalIncidence, true);
    }

    boost::optional<double> StandardGlazing_Impl::backSideVisibleReflectanceatNormalIncidence() const {
      return getDouble(OS_WindowMaterial_GlazingFields::BackSideVisibleReflectanceatNormalIncidence, true);
    }

    double StandardGlazing_Impl::infraredTransmittance() const {
      OptionalDouble od = getDouble(OS_WindowMaterial_GlazingFields::InfraredTransmittanceatNormalIncidence, true);
      if (!od) {
        LOG_AND_THROW("Thermal conductivity is not yet set for " << briefDescription() << ".");
      }
      return *od;
    }

    double StandardGlazing_Impl::infraredTransmittanceatNormalIncidence() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_GlazingFields::InfraredTransmittanceatNormalIncidence, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool StandardGlazing_Impl::isInfraredTransmittanceatNormalIncidenceDefaulted() const {
      return isEmpty(OS_WindowMaterial_GlazingFields::InfraredTransmittanceatNormalIncidence);
    }

    double StandardGlazing_Impl::frontSideInfraredHemisphericalEmissivity() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_GlazingFields::FrontSideInfraredHemisphericalEmissivity, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool StandardGlazing_Impl::isFrontSideInfraredHemisphericalEmissivityDefaulted() const {
      return isEmpty(OS_WindowMaterial_GlazingFields::FrontSideInfraredHemisphericalEmissivity);
    }

    double StandardGlazing_Impl::backSideInfraredHemisphericalEmissivity() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_GlazingFields::BackSideInfraredHemisphericalEmissivity, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool StandardGlazing_Impl::isBackSideInfraredHemisphericalEmissivityDefaulted() const {
      return isEmpty(OS_WindowMaterial_GlazingFields::BackSideInfraredHemisphericalEmissivity);
    }

    double StandardGlazing_Impl::conductivity() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_GlazingFields::Conductivity, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool StandardGlazing_Impl::isConductivityDefaulted() const {
      return isEmpty(OS_WindowMaterial_GlazingFields::Conductivity);
    }

    double StandardGlazing_Impl::dirtCorrectionFactorforSolarandVisibleTransmittance() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_GlazingFields::DirtCorrectionFactorforSolarandVisibleTransmittance, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool StandardGlazing_Impl::isDirtCorrectionFactorforSolarandVisibleTransmittanceDefaulted() const {
      return isEmpty(OS_WindowMaterial_GlazingFields::DirtCorrectionFactorforSolarandVisibleTransmittance);
    }

    bool StandardGlazing_Impl::solarDiffusing() const {
      boost::optional<std::string> value = getString(OS_WindowMaterial_GlazingFields::SolarDiffusing, true);
      OS_ASSERT(value);
      return openstudio::istringEqual(value.get(), "Yes");
    }

    bool StandardGlazing_Impl::isSolarDiffusingDefaulted() const {
      return isEmpty(OS_WindowMaterial_GlazingFields::SolarDiffusing);
    }

    bool StandardGlazing_Impl::setOpticalDataType(const std::string& opticalDataType) {
      bool result = setString(OS_WindowMaterial_GlazingFields::OpticalDataType, opticalDataType);
      return result;
    }

    bool StandardGlazing_Impl::setWindowGlassSpectralDataSetName(boost::optional<std::string> windowGlassSpectralDataSetName) {
      LOG(Warn, "StandardGlazing::setWindowGlassSpectralDataSetName is deprecated, use StandardGlazing::setWindowGlassSpectralDataSet");
      bool result(false);
      if (windowGlassSpectralDataSetName) {
        result = setString(OS_WindowMaterial_GlazingFields::WindowGlassSpectralDataSetName, windowGlassSpectralDataSetName.get());
        if (result) {
          result = setOpticalDataType("Spectral");
          OS_ASSERT(result);
        }
      } else {
        resetWindowGlassSpectralDataSetName();
        result = setOpticalDataType("SpectralAverage");
        OS_ASSERT(result);
      }
      return result;
    }

    void StandardGlazing_Impl::resetWindowGlassSpectralDataSetName() {
      LOG(Warn, "StandardGlazing::resetWindowGlassSpectralDataSetName is deprecated, use StandardGlazing::resetWindowGlassSpectralDataSet");
      bool result = setString(OS_WindowMaterial_GlazingFields::WindowGlassSpectralDataSetName, "");
      OS_ASSERT(result);
      result = setOpticalDataType("SpectralAverage");
      OS_ASSERT(result);
    }

    bool StandardGlazing_Impl::setWindowGlassSpectralDataSet(const MaterialPropertyGlazingSpectralData& spectralData) {
      bool result = setPointer(OS_WindowMaterial_GlazingFields::WindowGlassSpectralDataSetName, spectralData.handle());
      if (result) {
        result = setOpticalDataType("Spectral");
        OS_ASSERT(result);
      }
      return result;
    }

    void StandardGlazing_Impl::resetWindowGlassSpectralDataSet() {
      bool result = setString(OS_WindowMaterial_GlazingFields::WindowGlassSpectralDataSetName, "");
      OS_ASSERT(result);
      result = setOpticalDataType("SpectralAverage");
      OS_ASSERT(result);
    }

    bool StandardGlazing_Impl::setThickness(double thickness) {
      bool result = setDouble(OS_WindowMaterial_GlazingFields::Thickness, thickness);
      return result;
    }

    bool StandardGlazing_Impl::setSolarTransmittance(double value) {
      return setDouble(OS_WindowMaterial_GlazingFields::SolarTransmittanceatNormalIncidence, value);
    }

    bool StandardGlazing_Impl::setSolarTransmittanceatNormalIncidence(boost::optional<double> solarTransmittanceatNormalIncidence) {
      bool result(false);
      if (solarTransmittanceatNormalIncidence) {
        result = setDouble(OS_WindowMaterial_GlazingFields::SolarTransmittanceatNormalIncidence, solarTransmittanceatNormalIncidence.get());
      } else {
        resetSolarTransmittanceatNormalIncidence();
        result = true;
      }
      return result;
    }

    void StandardGlazing_Impl::resetSolarTransmittanceatNormalIncidence() {
      bool result = setString(OS_WindowMaterial_GlazingFields::SolarTransmittanceatNormalIncidence, "");
      OS_ASSERT(result);
    }

    bool StandardGlazing_Impl::setFrontSideSolarReflectanceatNormalIncidence(boost::optional<double> frontSideSolarReflectanceatNormalIncidence) {
      bool result(false);
      if (frontSideSolarReflectanceatNormalIncidence) {
        result =
          setDouble(OS_WindowMaterial_GlazingFields::FrontSideSolarReflectanceatNormalIncidence, frontSideSolarReflectanceatNormalIncidence.get());
      } else {
        resetFrontSideSolarReflectanceatNormalIncidence();
        result = true;
      }
      return result;
    }

    void StandardGlazing_Impl::resetFrontSideSolarReflectanceatNormalIncidence() {
      bool result = setString(OS_WindowMaterial_GlazingFields::FrontSideSolarReflectanceatNormalIncidence, "");
      OS_ASSERT(result);
    }

    bool StandardGlazing_Impl::setBackSideSolarReflectanceatNormalIncidence(boost::optional<double> backSideSolarReflectanceatNormalIncidence) {
      bool result(false);
      if (backSideSolarReflectanceatNormalIncidence) {
        result =
          setDouble(OS_WindowMaterial_GlazingFields::BackSideSolarReflectanceatNormalIncidence, backSideSolarReflectanceatNormalIncidence.get());
      } else {
        resetBackSideSolarReflectanceatNormalIncidence();
        result = true;
      }
      return result;
    }

    void StandardGlazing_Impl::resetBackSideSolarReflectanceatNormalIncidence() {
      bool result = setString(OS_WindowMaterial_GlazingFields::BackSideSolarReflectanceatNormalIncidence, "");
      OS_ASSERT(result);
    }

    bool StandardGlazing_Impl::setVisibleTransmittance(double value) {
      return setDouble(OS_WindowMaterial_GlazingFields::VisibleTransmittanceatNormalIncidence, value);
    }

    bool StandardGlazing_Impl::setVisibleTransmittanceatNormalIncidence(boost::optional<double> visibleTransmittanceatNormalIncidence) {
      bool result(false);
      if (visibleTransmittanceatNormalIncidence) {
        result = setDouble(OS_WindowMaterial_GlazingFields::VisibleTransmittanceatNormalIncidence, visibleTransmittanceatNormalIncidence.get());
      } else {
        resetVisibleTransmittanceatNormalIncidence();
        result = true;
      }
      return result;
    }

    void StandardGlazing_Impl::resetVisibleTransmittanceatNormalIncidence() {
      bool result = setString(OS_WindowMaterial_GlazingFields::VisibleTransmittanceatNormalIncidence, "");
      OS_ASSERT(result);
    }

    bool StandardGlazing_Impl::setFrontSideVisibleReflectanceatNormalIncidence(boost::optional<double> frontSideVisibleReflectanceatNormalIncidence) {
      bool result(false);
      if (frontSideVisibleReflectanceatNormalIncidence) {
        result = setDouble(OS_WindowMaterial_GlazingFields::FrontSideVisibleReflectanceatNormalIncidence,
                           frontSideVisibleReflectanceatNormalIncidence.get());
      } else {
        resetFrontSideVisibleReflectanceatNormalIncidence();
        result = true;
      }
      return result;
    }

    void StandardGlazing_Impl::resetFrontSideVisibleReflectanceatNormalIncidence() {
      bool result = setString(OS_WindowMaterial_GlazingFields::FrontSideVisibleReflectanceatNormalIncidence, "");
      OS_ASSERT(result);
    }

    bool StandardGlazing_Impl::setBackSideVisibleReflectanceatNormalIncidence(boost::optional<double> backSideVisibleReflectanceatNormalIncidence) {
      bool result(false);
      if (backSideVisibleReflectanceatNormalIncidence) {
        result =
          setDouble(OS_WindowMaterial_GlazingFields::BackSideVisibleReflectanceatNormalIncidence, backSideVisibleReflectanceatNormalIncidence.get());
      } else {
        resetBackSideVisibleReflectanceatNormalIncidence();
        result = true;
      }
      return result;
    }

    void StandardGlazing_Impl::resetBackSideVisibleReflectanceatNormalIncidence() {
      bool result = setString(OS_WindowMaterial_GlazingFields::BackSideVisibleReflectanceatNormalIncidence, "");
      OS_ASSERT(result);
    }

    bool StandardGlazing_Impl::setInfraredTransmittance(double value) {
      return setDouble(OS_WindowMaterial_GlazingFields::InfraredTransmittanceatNormalIncidence, value);
    }

    bool StandardGlazing_Impl::setInfraredTransmittanceatNormalIncidence(double infraredTransmittanceatNormalIncidence) {
      bool result = setDouble(OS_WindowMaterial_GlazingFields::InfraredTransmittanceatNormalIncidence, infraredTransmittanceatNormalIncidence);
      return result;
    }

    void StandardGlazing_Impl::resetInfraredTransmittanceatNormalIncidence() {
      bool result = setString(OS_WindowMaterial_GlazingFields::InfraredTransmittanceatNormalIncidence, "");
      OS_ASSERT(result);
    }

    bool StandardGlazing_Impl::setFrontSideInfraredHemisphericalEmissivity(double frontSideInfraredHemisphericalEmissivity) {
      bool result = setDouble(OS_WindowMaterial_GlazingFields::FrontSideInfraredHemisphericalEmissivity, frontSideInfraredHemisphericalEmissivity);
      return result;
    }

    void StandardGlazing_Impl::resetFrontSideInfraredHemisphericalEmissivity() {
      bool result = setString(OS_WindowMaterial_GlazingFields::FrontSideInfraredHemisphericalEmissivity, "");
      OS_ASSERT(result);
    }

    bool StandardGlazing_Impl::setBackSideInfraredHemisphericalEmissivity(double backSideInfraredHemisphericalEmissivity) {
      bool result = setDouble(OS_WindowMaterial_GlazingFields::BackSideInfraredHemisphericalEmissivity, backSideInfraredHemisphericalEmissivity);
      return result;
    }

    void StandardGlazing_Impl::resetBackSideInfraredHemisphericalEmissivity() {
      bool result = setString(OS_WindowMaterial_GlazingFields::BackSideInfraredHemisphericalEmissivity, "");
      OS_ASSERT(result);
    }

    bool StandardGlazing_Impl::setConductivity(double conductivity) {
      bool result = setDouble(OS_WindowMaterial_GlazingFields::Conductivity, conductivity);
      return result;
    }

    void StandardGlazing_Impl::resetConductivity() {
      bool result = setString(OS_WindowMaterial_GlazingFields::Conductivity, "");
      OS_ASSERT(result);
    }

    bool StandardGlazing_Impl::setDirtCorrectionFactorforSolarandVisibleTransmittance(double dirtCorrectionFactorforSolarandVisibleTransmittance) {
      bool result = setDouble(OS_WindowMaterial_GlazingFields::DirtCorrectionFactorforSolarandVisibleTransmittance,
                              dirtCorrectionFactorforSolarandVisibleTransmittance);
      return result;
    }

    void StandardGlazing_Impl::resetDirtCorrectionFactorforSolarandVisibleTransmittance() {
      bool result = setString(OS_WindowMaterial_GlazingFields::DirtCorrectionFactorforSolarandVisibleTransmittance, "");
      OS_ASSERT(result);
    }

    bool StandardGlazing_Impl::setSolarDiffusing(bool solarDiffusing) {
      return setBooleanFieldValue(OS_WindowMaterial_GlazingFields::SolarDiffusing, solarDiffusing);
      ;
    }

    void StandardGlazing_Impl::resetSolarDiffusing() {
      bool result = setString(OS_WindowMaterial_GlazingFields::SolarDiffusing, "");
      OS_ASSERT(result);
    }

    double StandardGlazing_Impl::thermalConductivity() const {
      OptionalDouble od = getDouble(OS_WindowMaterial_GlazingFields::Conductivity, true);
      if (!od) {
        LOG_AND_THROW("Thermal conductivity is not yet set for " << briefDescription() << ".");
      }
      return *od;
    }

    double StandardGlazing_Impl::thermalConductance() const {
      return thermalConductivity() / thickness();
    }

    double StandardGlazing_Impl::thermalResistivity() const {
      return 1.0 / thermalConductivity();
    }

    double StandardGlazing_Impl::thermalResistance() const {
      return 1.0 / thermalConductance();
    }

    double StandardGlazing_Impl::exteriorVisibleReflectance() const {
      OptionalDouble od = getDouble(OS_WindowMaterial_GlazingFields::FrontSideVisibleReflectanceatNormalIncidence, true);
      if (!od) {
        LOG_AND_THROW("Front-side visible reflectance is not yet set for " << briefDescription() << ".");
      }
      return *od;
    }

    double StandardGlazing_Impl::interiorVisibleReflectance() const {
      OptionalDouble od = getDouble(OS_WindowMaterial_GlazingFields::BackSideVisibleReflectanceatNormalIncidence, true);
      if (!od) {
        LOG_AND_THROW("Back-side visible reflectance is not yet set for " << briefDescription() << ".");
      }
      return *od;
    }

    boost::optional<double> StandardGlazing_Impl::interiorVisibleAbsorptance() const {
      return 1.0 - (*(getVisibleTransmittance()) + interiorVisibleReflectance());
    }

    boost::optional<double> StandardGlazing_Impl::exteriorVisibleAbsorptance() const {
      return 1.0 - (*(getVisibleTransmittance()) + exteriorVisibleReflectance());
    }

    bool StandardGlazing_Impl::setThermalConductivity(double value) {
      return setDouble(OS_WindowMaterial_GlazingFields::Conductivity, value);
    }

    bool StandardGlazing_Impl::setThermalConductance(double value) {
      return setThickness(thermalConductivity() / value);
    }

    bool StandardGlazing_Impl::setThermalResistivity(double value) {
      return setThermalConductivity(1.0 / value);
    }

    bool StandardGlazing_Impl::setThermalResistance(double value) {
      return setThickness(value / thermalResistivity());
    }

    std::vector<std::string> StandardGlazing_Impl::opticalDataTypeValues() const {
      return StandardGlazing::opticalDataTypeValues();
    }

  }  // namespace detail

  StandardGlazing::StandardGlazing(const Model& model, const std::string& opticalDataType, double thickness)
    : Glazing(StandardGlazing::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::StandardGlazing_Impl>());

    // TODO: Appropriately handle the following required object-list fields.
    bool ok = true;
    // ok = setHandle();
    OS_ASSERT(ok);
    ok = setOpticalDataType(opticalDataType);
    OS_ASSERT(ok);
    ok = setThickness(thickness);
    OS_ASSERT(ok);
  }

  IddObjectType StandardGlazing::iddObjectType() {
    return {IddObjectType::OS_WindowMaterial_Glazing};
  }

  std::vector<std::string> StandardGlazing::opticalDataTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_WindowMaterial_GlazingFields::OpticalDataType);
  }

  std::string StandardGlazing::opticalDataType() const {
    return getImpl<detail::StandardGlazing_Impl>()->opticalDataType();
  }

  boost::optional<std::string> StandardGlazing::windowGlassSpectralDataSetName() const {
    return getImpl<detail::StandardGlazing_Impl>()->windowGlassSpectralDataSetName();
  }

  boost::optional<MaterialPropertyGlazingSpectralData> StandardGlazing::windowGlassSpectralDataSet() const {
    return getImpl<detail::StandardGlazing_Impl>()->windowGlassSpectralDataSet();
  }

  double StandardGlazing::solarTransmittance() const {
    return getImpl<detail::StandardGlazing_Impl>()->solarTransmittance();
  }

  // cppcheck-suppress [duplInheritedMember] for documentation purposes
  double StandardGlazing::thickness() const {
    return getImpl<detail::StandardGlazing_Impl>()->thickness();
  }

  boost::optional<double> StandardGlazing::solarTransmittanceatNormalIncidence() const {
    return getImpl<detail::StandardGlazing_Impl>()->solarTransmittanceatNormalIncidence();
  }

  boost::optional<double> StandardGlazing::frontSideSolarReflectanceatNormalIncidence() const {
    return getImpl<detail::StandardGlazing_Impl>()->frontSideSolarReflectanceatNormalIncidence();
  }

  boost::optional<double> StandardGlazing::backSideSolarReflectanceatNormalIncidence() const {
    return getImpl<detail::StandardGlazing_Impl>()->backSideSolarReflectanceatNormalIncidence();
  }

  boost::optional<double> StandardGlazing::visibleTransmittanceatNormalIncidence() const {
    return getImpl<detail::StandardGlazing_Impl>()->visibleTransmittanceatNormalIncidence();
  }

  boost::optional<double> StandardGlazing::frontSideVisibleReflectanceatNormalIncidence() const {
    return getImpl<detail::StandardGlazing_Impl>()->frontSideVisibleReflectanceatNormalIncidence();
  }

  boost::optional<double> StandardGlazing::backSideVisibleReflectanceatNormalIncidence() const {
    return getImpl<detail::StandardGlazing_Impl>()->backSideVisibleReflectanceatNormalIncidence();
  }

  double StandardGlazing::infraredTransmittance() const {
    return getImpl<detail::StandardGlazing_Impl>()->infraredTransmittance();
  }

  double StandardGlazing::infraredTransmittanceatNormalIncidence() const {
    return getImpl<detail::StandardGlazing_Impl>()->infraredTransmittanceatNormalIncidence();
  }

  bool StandardGlazing::isInfraredTransmittanceatNormalIncidenceDefaulted() const {
    return getImpl<detail::StandardGlazing_Impl>()->isInfraredTransmittanceatNormalIncidenceDefaulted();
  }

  double StandardGlazing::frontSideInfraredHemisphericalEmissivity() const {
    return getImpl<detail::StandardGlazing_Impl>()->frontSideInfraredHemisphericalEmissivity();
  }

  bool StandardGlazing::isFrontSideInfraredHemisphericalEmissivityDefaulted() const {
    return getImpl<detail::StandardGlazing_Impl>()->isFrontSideInfraredHemisphericalEmissivityDefaulted();
  }

  double StandardGlazing::backSideInfraredHemisphericalEmissivity() const {
    return getImpl<detail::StandardGlazing_Impl>()->backSideInfraredHemisphericalEmissivity();
  }

  bool StandardGlazing::isBackSideInfraredHemisphericalEmissivityDefaulted() const {
    return getImpl<detail::StandardGlazing_Impl>()->isBackSideInfraredHemisphericalEmissivityDefaulted();
  }

  double StandardGlazing::conductivity() const {
    return getImpl<detail::StandardGlazing_Impl>()->conductivity();
  }

  bool StandardGlazing::isConductivityDefaulted() const {
    return getImpl<detail::StandardGlazing_Impl>()->isConductivityDefaulted();
  }

  double StandardGlazing::dirtCorrectionFactorforSolarandVisibleTransmittance() const {
    return getImpl<detail::StandardGlazing_Impl>()->dirtCorrectionFactorforSolarandVisibleTransmittance();
  }

  bool StandardGlazing::isDirtCorrectionFactorforSolarandVisibleTransmittanceDefaulted() const {
    return getImpl<detail::StandardGlazing_Impl>()->isDirtCorrectionFactorforSolarandVisibleTransmittanceDefaulted();
  }

  bool StandardGlazing::solarDiffusing() const {
    return getImpl<detail::StandardGlazing_Impl>()->solarDiffusing();
  }

  bool StandardGlazing::isSolarDiffusingDefaulted() const {
    return getImpl<detail::StandardGlazing_Impl>()->isSolarDiffusingDefaulted();
  }

  bool StandardGlazing::setOpticalDataType(const std::string& opticalDataType) {
    return getImpl<detail::StandardGlazing_Impl>()->setOpticalDataType(opticalDataType);
  }

  bool StandardGlazing::setWindowGlassSpectralDataSetName(const std::string& windowGlassSpectralDataSetName) {
    return getImpl<detail::StandardGlazing_Impl>()->setWindowGlassSpectralDataSetName(windowGlassSpectralDataSetName);
  }

  void StandardGlazing::resetWindowGlassSpectralDataSetName() {
    getImpl<detail::StandardGlazing_Impl>()->resetWindowGlassSpectralDataSetName();
  }

  bool StandardGlazing::setWindowGlassSpectralDataSet(const MaterialPropertyGlazingSpectralData& spectralData) {
    return getImpl<detail::StandardGlazing_Impl>()->setWindowGlassSpectralDataSet(spectralData);
  }

  void StandardGlazing::resetWindowGlassSpectralDataSet() {
    getImpl<detail::StandardGlazing_Impl>()->resetWindowGlassSpectralDataSet();
  }

  bool StandardGlazing::setSolarTransmittance(double value) {
    return getImpl<detail::StandardGlazing_Impl>()->setSolarTransmittance(value);
  }

  // cppcheck-suppress [duplInheritedMember] for documentation purposes
  bool StandardGlazing::setThickness(double thickness) {
    return getImpl<detail::StandardGlazing_Impl>()->setThickness(thickness);
  }

  bool StandardGlazing::setSolarTransmittanceatNormalIncidence(double solarTransmittanceatNormalIncidence) {
    return getImpl<detail::StandardGlazing_Impl>()->setSolarTransmittanceatNormalIncidence(solarTransmittanceatNormalIncidence);
  }

  void StandardGlazing::resetSolarTransmittanceatNormalIncidence() {
    getImpl<detail::StandardGlazing_Impl>()->resetSolarTransmittanceatNormalIncidence();
  }

  bool StandardGlazing::setFrontSideSolarReflectanceatNormalIncidence(double frontSideSolarReflectanceatNormalIncidence) {
    return getImpl<detail::StandardGlazing_Impl>()->setFrontSideSolarReflectanceatNormalIncidence(frontSideSolarReflectanceatNormalIncidence);
  }

  void StandardGlazing::resetFrontSideSolarReflectanceatNormalIncidence() {
    getImpl<detail::StandardGlazing_Impl>()->resetFrontSideSolarReflectanceatNormalIncidence();
  }

  bool StandardGlazing::setBackSideSolarReflectanceatNormalIncidence(double backSideSolarReflectanceatNormalIncidence) {
    return getImpl<detail::StandardGlazing_Impl>()->setBackSideSolarReflectanceatNormalIncidence(backSideSolarReflectanceatNormalIncidence);
  }

  void StandardGlazing::resetBackSideSolarReflectanceatNormalIncidence() {
    getImpl<detail::StandardGlazing_Impl>()->resetBackSideSolarReflectanceatNormalIncidence();
  }

  bool StandardGlazing::setVisibleTransmittance(double value) {
    return getImpl<detail::StandardGlazing_Impl>()->setVisibleTransmittance(value);
  }

  bool StandardGlazing::setVisibleTransmittanceatNormalIncidence(double visibleTransmittanceatNormalIncidence) {
    return getImpl<detail::StandardGlazing_Impl>()->setVisibleTransmittanceatNormalIncidence(visibleTransmittanceatNormalIncidence);
  }

  void StandardGlazing::resetVisibleTransmittanceatNormalIncidence() {
    getImpl<detail::StandardGlazing_Impl>()->resetVisibleTransmittanceatNormalIncidence();
  }

  bool StandardGlazing::setFrontSideVisibleReflectanceatNormalIncidence(double frontSideVisibleReflectanceatNormalIncidence) {
    return getImpl<detail::StandardGlazing_Impl>()->setFrontSideVisibleReflectanceatNormalIncidence(frontSideVisibleReflectanceatNormalIncidence);
  }

  void StandardGlazing::resetFrontSideVisibleReflectanceatNormalIncidence() {
    getImpl<detail::StandardGlazing_Impl>()->resetFrontSideVisibleReflectanceatNormalIncidence();
  }

  bool StandardGlazing::setBackSideVisibleReflectanceatNormalIncidence(double backSideVisibleReflectanceatNormalIncidence) {
    return getImpl<detail::StandardGlazing_Impl>()->setBackSideVisibleReflectanceatNormalIncidence(backSideVisibleReflectanceatNormalIncidence);
  }

  void StandardGlazing::resetBackSideVisibleReflectanceatNormalIncidence() {
    getImpl<detail::StandardGlazing_Impl>()->resetBackSideVisibleReflectanceatNormalIncidence();
  }

  bool StandardGlazing::setInfraredTransmittance(double value) {
    return getImpl<detail::StandardGlazing_Impl>()->setInfraredTransmittance(value);
  }

  bool StandardGlazing::setInfraredTransmittanceatNormalIncidence(double infraredTransmittanceatNormalIncidence) {
    return getImpl<detail::StandardGlazing_Impl>()->setInfraredTransmittanceatNormalIncidence(infraredTransmittanceatNormalIncidence);
  }

  void StandardGlazing::resetInfraredTransmittanceatNormalIncidence() {
    getImpl<detail::StandardGlazing_Impl>()->resetInfraredTransmittanceatNormalIncidence();
  }

  bool StandardGlazing::setFrontSideInfraredHemisphericalEmissivity(double frontSideInfraredHemisphericalEmissivity) {
    return getImpl<detail::StandardGlazing_Impl>()->setFrontSideInfraredHemisphericalEmissivity(frontSideInfraredHemisphericalEmissivity);
  }

  void StandardGlazing::resetFrontSideInfraredHemisphericalEmissivity() {
    getImpl<detail::StandardGlazing_Impl>()->resetFrontSideInfraredHemisphericalEmissivity();
  }

  bool StandardGlazing::setBackSideInfraredHemisphericalEmissivity(double backSideInfraredHemisphericalEmissivity) {
    return getImpl<detail::StandardGlazing_Impl>()->setBackSideInfraredHemisphericalEmissivity(backSideInfraredHemisphericalEmissivity);
  }

  void StandardGlazing::resetBackSideInfraredHemisphericalEmissivity() {
    getImpl<detail::StandardGlazing_Impl>()->resetBackSideInfraredHemisphericalEmissivity();
  }

  bool StandardGlazing::setConductivity(double conductivity) {
    return getImpl<detail::StandardGlazing_Impl>()->setConductivity(conductivity);
  }

  void StandardGlazing::resetConductivity() {
    getImpl<detail::StandardGlazing_Impl>()->resetConductivity();
  }

  bool StandardGlazing::setDirtCorrectionFactorforSolarandVisibleTransmittance(double dirtCorrectionFactorforSolarandVisibleTransmittance) {
    return getImpl<detail::StandardGlazing_Impl>()->setDirtCorrectionFactorforSolarandVisibleTransmittance(
      dirtCorrectionFactorforSolarandVisibleTransmittance);
  }

  void StandardGlazing::resetDirtCorrectionFactorforSolarandVisibleTransmittance() {
    getImpl<detail::StandardGlazing_Impl>()->resetDirtCorrectionFactorforSolarandVisibleTransmittance();
  }

  bool StandardGlazing::setSolarDiffusing(bool solarDiffusing) {
    return getImpl<detail::StandardGlazing_Impl>()->setSolarDiffusing(solarDiffusing);
  }

  void StandardGlazing::setSolarDiffusingNoFail(bool solarDiffusing) {
    bool result = getImpl<detail::StandardGlazing_Impl>()->setSolarDiffusing(solarDiffusing);
    OS_ASSERT(result);
  }

  void StandardGlazing::resetSolarDiffusing() {
    getImpl<detail::StandardGlazing_Impl>()->resetSolarDiffusing();
  }

  double StandardGlazing::thermalConductivity() const {
    return getImpl<detail::StandardGlazing_Impl>()->thermalConductivity();
  }

  double StandardGlazing::thermalConductance() const {
    return getImpl<detail::StandardGlazing_Impl>()->thermalConductance();
  }

  double StandardGlazing::thermalResistivity() const {
    return getImpl<detail::StandardGlazing_Impl>()->thermalResistivity();
  }

  double StandardGlazing::thermalResistance() const {
    return getImpl<detail::StandardGlazing_Impl>()->thermalResistance();
  }

  double StandardGlazing::interiorVisibleReflectance() const {
    return getImpl<detail::StandardGlazing_Impl>()->interiorVisibleReflectance();
  }

  double StandardGlazing::exteriorVisibleReflectance() const {
    return getImpl<detail::StandardGlazing_Impl>()->exteriorVisibleReflectance();
  }

  bool StandardGlazing::setThermalConductivity(double value) {
    return getImpl<detail::StandardGlazing_Impl>()->setThermalConductivity(value);
  }

  bool StandardGlazing::setThermalConductance(double value) {
    return getImpl<detail::StandardGlazing_Impl>()->setThermalConductance(value);
  }

  bool StandardGlazing::setThermalResistivity(double value) {
    return getImpl<detail::StandardGlazing_Impl>()->setThermalResistivity(value);
  }

  bool StandardGlazing::setThermalResistance(double value) {
    return getImpl<detail::StandardGlazing_Impl>()->setThermalResistance(value);
  }

  /// @cond
  StandardGlazing::StandardGlazing(std::shared_ptr<detail::StandardGlazing_Impl> impl) : Glazing(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
