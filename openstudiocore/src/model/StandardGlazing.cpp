/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "StandardGlazing.hpp"
#include "StandardGlazing_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include "MaterialPropertyGlazingSpectralData.hpp"
#include "MaterialPropertyGlazingSpectralData_Impl.hpp"

#include <utilities/idd/OS_WindowMaterial_Glazing_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  StandardGlazing_Impl::StandardGlazing_Impl(const IdfObject& idfObject,
                                             Model_Impl* model,
                                             bool keepHandle)
    : Glazing_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == StandardGlazing::iddObjectType());
  }

  StandardGlazing_Impl::StandardGlazing_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle)
    : Glazing_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == StandardGlazing::iddObjectType());
  }

  StandardGlazing_Impl::StandardGlazing_Impl(const StandardGlazing_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle)
    : Glazing_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& StandardGlazing_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType StandardGlazing_Impl::iddObjectType() const {
    return StandardGlazing::iddObjectType();
  }

  std::string StandardGlazing_Impl::opticalDataType() const {
    boost::optional<std::string> value = getString(OS_WindowMaterial_GlazingFields::OpticalDataType,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<std::string> StandardGlazing_Impl::windowGlassSpectralDataSetName() const {
    LOG(Warn, "StandardGlazing::windowGlassSpectralDataSetName is deprecated, use StandardGlazing::windowGlassSpectralDataSet instead");
    boost::optional<MaterialPropertyGlazingSpectralData> spectralData = windowGlassSpectralDataSet();
    if (spectralData){
      return spectralData->name();
    }
    return boost::none;
  }

  boost::optional<MaterialPropertyGlazingSpectralData> StandardGlazing_Impl::windowGlassSpectralDataSet() const
  {
    return getObject<ModelObject>().getModelObjectTarget<MaterialPropertyGlazingSpectralData>(OS_WindowMaterial_GlazingFields::WindowGlassSpectralDataSetName);
  }

  double StandardGlazing_Impl::thickness() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_GlazingFields::Thickness,true);
    OS_ASSERT(value);
    return value.get();
  }

  double StandardGlazing_Impl::solarTransmittance() const {
    OptionalDouble od = getDouble(OS_WindowMaterial_GlazingFields::SolarTransmittanceatNormalIncidence,true);
    if (!od) {
      LOG_AND_THROW("Solar transmittance is not yet set for " << briefDescription() << ".");
    }
    return *od;
  }

  Quantity StandardGlazing_Impl::getThickness(bool returnIP) const {
    OptionalDouble value = thickness();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_GlazingFields::Thickness, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  boost::optional<double> StandardGlazing_Impl::solarTransmittanceatNormalIncidence() const {
    return getDouble(OS_WindowMaterial_GlazingFields::SolarTransmittanceatNormalIncidence,true);
  }

  OSOptionalQuantity StandardGlazing_Impl::getSolarTransmittanceatNormalIncidence(bool returnIP) const {
    OptionalDouble value = solarTransmittanceatNormalIncidence();
    return getQuantityFromDouble(OS_WindowMaterial_GlazingFields::SolarTransmittanceatNormalIncidence, value, returnIP);
  }

  boost::optional<double> StandardGlazing_Impl::frontSideSolarReflectanceatNormalIncidence() const {
    return getDouble(OS_WindowMaterial_GlazingFields::FrontSideSolarReflectanceatNormalIncidence,true);
  }

  OSOptionalQuantity StandardGlazing_Impl::getFrontSideSolarReflectanceatNormalIncidence(bool returnIP) const {
    OptionalDouble value = frontSideSolarReflectanceatNormalIncidence();
    return getQuantityFromDouble(OS_WindowMaterial_GlazingFields::FrontSideSolarReflectanceatNormalIncidence, value, returnIP);
  }

  boost::optional<double> StandardGlazing_Impl::backSideSolarReflectanceatNormalIncidence() const {
    return getDouble(OS_WindowMaterial_GlazingFields::BackSideSolarReflectanceatNormalIncidence,true);
  }

  OptionalDouble StandardGlazing_Impl::getVisibleTransmittance() const {
    OptionalDouble od = getDouble(OS_WindowMaterial_GlazingFields::VisibleTransmittanceatNormalIncidence,true);
    return *od;
  }

  OSOptionalQuantity StandardGlazing_Impl::getBackSideSolarReflectanceatNormalIncidence(bool returnIP) const {
    OptionalDouble value = backSideSolarReflectanceatNormalIncidence();
    return getQuantityFromDouble(OS_WindowMaterial_GlazingFields::BackSideSolarReflectanceatNormalIncidence, value, returnIP);
  }

  boost::optional<double> StandardGlazing_Impl::visibleTransmittanceatNormalIncidence() const {
    return getDouble(OS_WindowMaterial_GlazingFields::VisibleTransmittanceatNormalIncidence,true);
  }

  OSOptionalQuantity StandardGlazing_Impl::getVisibleTransmittanceatNormalIncidence(bool returnIP) const {
    OptionalDouble value = visibleTransmittanceatNormalIncidence();
    return getQuantityFromDouble(OS_WindowMaterial_GlazingFields::VisibleTransmittanceatNormalIncidence, value, returnIP);
  }

  boost::optional<double> StandardGlazing_Impl::frontSideVisibleReflectanceatNormalIncidence() const {
    return getDouble(OS_WindowMaterial_GlazingFields::FrontSideVisibleReflectanceatNormalIncidence,true);
  }

  OSOptionalQuantity StandardGlazing_Impl::getFrontSideVisibleReflectanceatNormalIncidence(bool returnIP) const {
    OptionalDouble value = frontSideVisibleReflectanceatNormalIncidence();
    return getQuantityFromDouble(OS_WindowMaterial_GlazingFields::FrontSideVisibleReflectanceatNormalIncidence, value, returnIP);
  }

  boost::optional<double> StandardGlazing_Impl::backSideVisibleReflectanceatNormalIncidence() const {
    return getDouble(OS_WindowMaterial_GlazingFields::BackSideVisibleReflectanceatNormalIncidence,true);
  }

  double StandardGlazing_Impl::infraredTransmittance() const {
    OptionalDouble od = getDouble(OS_WindowMaterial_GlazingFields::InfraredTransmittanceatNormalIncidence,true);
    if (!od) {
      LOG_AND_THROW("Thermal conductivity is not yet set for " << briefDescription() << ".");
    }
    return *od;
  }

  OSOptionalQuantity StandardGlazing_Impl::getBackSideVisibleReflectanceatNormalIncidence(bool returnIP) const {
    OptionalDouble value = backSideVisibleReflectanceatNormalIncidence();
    return getQuantityFromDouble(OS_WindowMaterial_GlazingFields::BackSideVisibleReflectanceatNormalIncidence, value, returnIP);
  }

  double StandardGlazing_Impl::infraredTransmittanceatNormalIncidence() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_GlazingFields::InfraredTransmittanceatNormalIncidence,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity StandardGlazing_Impl::getInfraredTransmittanceatNormalIncidence(bool returnIP) const {
    OptionalDouble value = infraredTransmittanceatNormalIncidence();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_GlazingFields::InfraredTransmittanceatNormalIncidence, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool StandardGlazing_Impl::isInfraredTransmittanceatNormalIncidenceDefaulted() const {
    return isEmpty(OS_WindowMaterial_GlazingFields::InfraredTransmittanceatNormalIncidence);
  }

  double StandardGlazing_Impl::frontSideInfraredHemisphericalEmissivity() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_GlazingFields::FrontSideInfraredHemisphericalEmissivity,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity StandardGlazing_Impl::getFrontSideInfraredHemisphericalEmissivity(bool returnIP) const {
    OptionalDouble value = frontSideInfraredHemisphericalEmissivity();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_GlazingFields::FrontSideInfraredHemisphericalEmissivity, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool StandardGlazing_Impl::isFrontSideInfraredHemisphericalEmissivityDefaulted() const {
    return isEmpty(OS_WindowMaterial_GlazingFields::FrontSideInfraredHemisphericalEmissivity);
  }

  double StandardGlazing_Impl::backSideInfraredHemisphericalEmissivity() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_GlazingFields::BackSideInfraredHemisphericalEmissivity,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity StandardGlazing_Impl::getBackSideInfraredHemisphericalEmissivity(bool returnIP) const {
    OptionalDouble value = backSideInfraredHemisphericalEmissivity();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_GlazingFields::BackSideInfraredHemisphericalEmissivity, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool StandardGlazing_Impl::isBackSideInfraredHemisphericalEmissivityDefaulted() const {
    return isEmpty(OS_WindowMaterial_GlazingFields::BackSideInfraredHemisphericalEmissivity);
  }

  double StandardGlazing_Impl::thermalConductivity() const {
    OptionalDouble od = getDouble(OS_WindowMaterial_GlazingFields::Conductivity,true);
    if (!od) {
      LOG_AND_THROW("Thermal conductivity is not yet set for " << briefDescription() << ".");
    }
    return *od;
  }

  double StandardGlazing_Impl::conductivity() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_GlazingFields::Conductivity,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity StandardGlazing_Impl::getConductivity(bool returnIP) const {
    OptionalDouble value = conductivity();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_GlazingFields::Conductivity, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool StandardGlazing_Impl::isConductivityDefaulted() const {
    return isEmpty(OS_WindowMaterial_GlazingFields::Conductivity);
  }

  double StandardGlazing_Impl::dirtCorrectionFactorforSolarandVisibleTransmittance() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_GlazingFields::DirtCorrectionFactorforSolarandVisibleTransmittance,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity StandardGlazing_Impl::getDirtCorrectionFactorforSolarandVisibleTransmittance(bool returnIP) const {
    OptionalDouble value = dirtCorrectionFactorforSolarandVisibleTransmittance();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_GlazingFields::DirtCorrectionFactorforSolarandVisibleTransmittance, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool StandardGlazing_Impl::isDirtCorrectionFactorforSolarandVisibleTransmittanceDefaulted() const {
    return isEmpty(OS_WindowMaterial_GlazingFields::DirtCorrectionFactorforSolarandVisibleTransmittance);
  }

  bool StandardGlazing_Impl::solarDiffusing() const {
    boost::optional<std::string> value = getString(OS_WindowMaterial_GlazingFields::SolarDiffusing,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }

  bool StandardGlazing_Impl::isSolarDiffusingDefaulted() const {
    return isEmpty(OS_WindowMaterial_GlazingFields::SolarDiffusing);
  }

  bool StandardGlazing_Impl::setOpticalDataType(std::string opticalDataType) {
    bool result = setString(OS_WindowMaterial_GlazingFields::OpticalDataType, opticalDataType);
    return result;
  }

  bool StandardGlazing_Impl::setWindowGlassSpectralDataSetName(boost::optional<std::string> windowGlassSpectralDataSetName) {
    LOG(Warn, "StandardGlazing::setWindowGlassSpectralDataSetName is deprecated, use StandardGlazing::setWindowGlassSpectralDataSet");
    bool result(false);
    if (windowGlassSpectralDataSetName) {
      result = setString(OS_WindowMaterial_GlazingFields::WindowGlassSpectralDataSetName, windowGlassSpectralDataSetName.get());
      if (result){
        result = setOpticalDataType("Spectral");
        OS_ASSERT(result);
      }
    }
    else {
      resetWindowGlassSpectralDataSetName();
      result = true;
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
    if (result){
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
    return setDouble(OS_WindowMaterial_GlazingFields::SolarTransmittanceatNormalIncidence,value);
  }

  bool StandardGlazing_Impl::setThickness(const Quantity& thickness) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_GlazingFields::Thickness,thickness);
    if (!value) {
      return false;
    }
    return setThickness(value.get());
  }

  bool StandardGlazing_Impl::setSolarTransmittanceatNormalIncidence(boost::optional<double> solarTransmittanceatNormalIncidence) {
    bool result(false);
    if (solarTransmittanceatNormalIncidence) {
      result = setDouble(OS_WindowMaterial_GlazingFields::SolarTransmittanceatNormalIncidence, solarTransmittanceatNormalIncidence.get());
    }
    else {
      resetSolarTransmittanceatNormalIncidence();
      result = true;
    }
    return result;
  }

  bool StandardGlazing_Impl::setSolarTransmittanceatNormalIncidence(const OSOptionalQuantity& solarTransmittanceatNormalIncidence) {
    bool result(false);
    OptionalDouble value;
    if (solarTransmittanceatNormalIncidence.isSet()) {
      value = getDoubleFromQuantity(OS_WindowMaterial_GlazingFields::SolarTransmittanceatNormalIncidence,solarTransmittanceatNormalIncidence.get());
      if (value) {
        result = setSolarTransmittanceatNormalIncidence(value);
      }
    }
    else {
      result = setSolarTransmittanceatNormalIncidence(value);
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
      result = setDouble(OS_WindowMaterial_GlazingFields::FrontSideSolarReflectanceatNormalIncidence, frontSideSolarReflectanceatNormalIncidence.get());
    }
    else {
      resetFrontSideSolarReflectanceatNormalIncidence();
      result = true;
    }
    return result;
  }

  bool StandardGlazing_Impl::setFrontSideSolarReflectanceatNormalIncidence(const OSOptionalQuantity& frontSideSolarReflectanceatNormalIncidence) {
    bool result(false);
    OptionalDouble value;
    if (frontSideSolarReflectanceatNormalIncidence.isSet()) {
      value = getDoubleFromQuantity(OS_WindowMaterial_GlazingFields::FrontSideSolarReflectanceatNormalIncidence,frontSideSolarReflectanceatNormalIncidence.get());
      if (value) {
        result = setFrontSideSolarReflectanceatNormalIncidence(value);
      }
    }
    else {
      result = setFrontSideSolarReflectanceatNormalIncidence(value);
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
      result = setDouble(OS_WindowMaterial_GlazingFields::BackSideSolarReflectanceatNormalIncidence, backSideSolarReflectanceatNormalIncidence.get());
    }
    else {
      resetBackSideSolarReflectanceatNormalIncidence();
      result = true;
    }
    return result;
  }

  bool StandardGlazing_Impl::setBackSideSolarReflectanceatNormalIncidence(const OSOptionalQuantity& backSideSolarReflectanceatNormalIncidence) {
    bool result(false);
    OptionalDouble value;
    if (backSideSolarReflectanceatNormalIncidence.isSet()) {
      value = getDoubleFromQuantity(OS_WindowMaterial_GlazingFields::BackSideSolarReflectanceatNormalIncidence,backSideSolarReflectanceatNormalIncidence.get());
      if (value) {
        result = setBackSideSolarReflectanceatNormalIncidence(value);
      }
    }
    else {
      result = setBackSideSolarReflectanceatNormalIncidence(value);
    }
    return result;
  }

  void StandardGlazing_Impl::resetBackSideSolarReflectanceatNormalIncidence() {
    bool result = setString(OS_WindowMaterial_GlazingFields::BackSideSolarReflectanceatNormalIncidence, "");
    OS_ASSERT(result);
  }

  bool StandardGlazing_Impl::setVisibleTransmittance(double value) {
    return setDouble(OS_WindowMaterial_GlazingFields::VisibleTransmittanceatNormalIncidence,value);
  }

  bool StandardGlazing_Impl::setVisibleTransmittanceatNormalIncidence(boost::optional<double> visibleTransmittanceatNormalIncidence) {
    bool result(false);
    if (visibleTransmittanceatNormalIncidence) {
      result = setDouble(OS_WindowMaterial_GlazingFields::VisibleTransmittanceatNormalIncidence, visibleTransmittanceatNormalIncidence.get());
    }
    else {
      resetVisibleTransmittanceatNormalIncidence();
      result = true;
    }
    return result;
  }

  bool StandardGlazing_Impl::setVisibleTransmittanceatNormalIncidence(const OSOptionalQuantity& visibleTransmittanceatNormalIncidence) {
    bool result(false);
    OptionalDouble value;
    if (visibleTransmittanceatNormalIncidence.isSet()) {
      value = getDoubleFromQuantity(OS_WindowMaterial_GlazingFields::VisibleTransmittanceatNormalIncidence,visibleTransmittanceatNormalIncidence.get());
      if (value) {
        result = setVisibleTransmittanceatNormalIncidence(value);
      }
    }
    else {
      result = setVisibleTransmittanceatNormalIncidence(value);
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
      result = setDouble(OS_WindowMaterial_GlazingFields::FrontSideVisibleReflectanceatNormalIncidence, frontSideVisibleReflectanceatNormalIncidence.get());
    }
    else {
      resetFrontSideVisibleReflectanceatNormalIncidence();
      result = true;
    }
    return result;
  }

  bool StandardGlazing_Impl::setFrontSideVisibleReflectanceatNormalIncidence(const OSOptionalQuantity& frontSideVisibleReflectanceatNormalIncidence) {
    bool result(false);
    OptionalDouble value;
    if (frontSideVisibleReflectanceatNormalIncidence.isSet()) {
      value = getDoubleFromQuantity(OS_WindowMaterial_GlazingFields::FrontSideVisibleReflectanceatNormalIncidence,frontSideVisibleReflectanceatNormalIncidence.get());
      if (value) {
        result = setFrontSideVisibleReflectanceatNormalIncidence(value);
      }
    }
    else {
      result = setFrontSideVisibleReflectanceatNormalIncidence(value);
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
      result = setDouble(OS_WindowMaterial_GlazingFields::BackSideVisibleReflectanceatNormalIncidence, backSideVisibleReflectanceatNormalIncidence.get());
    }
    else {
      resetBackSideVisibleReflectanceatNormalIncidence();
      result = true;
    }
    return result;
  }

  bool StandardGlazing_Impl::setBackSideVisibleReflectanceatNormalIncidence(const OSOptionalQuantity& backSideVisibleReflectanceatNormalIncidence) {
    bool result(false);
    OptionalDouble value;
    if (backSideVisibleReflectanceatNormalIncidence.isSet()) {
      value = getDoubleFromQuantity(OS_WindowMaterial_GlazingFields::BackSideVisibleReflectanceatNormalIncidence,backSideVisibleReflectanceatNormalIncidence.get());
      if (value) {
        result = setBackSideVisibleReflectanceatNormalIncidence(value);
      }
    }
    else {
      result = setBackSideVisibleReflectanceatNormalIncidence(value);
    }
    return result;
  }

  void StandardGlazing_Impl::resetBackSideVisibleReflectanceatNormalIncidence() {
    bool result = setString(OS_WindowMaterial_GlazingFields::BackSideVisibleReflectanceatNormalIncidence, "");
    OS_ASSERT(result);
  }

  bool StandardGlazing_Impl::setInfraredTransmittance(double value) {
    return setDouble(OS_WindowMaterial_GlazingFields::InfraredTransmittanceatNormalIncidence,value);
  }

  bool StandardGlazing_Impl::setInfraredTransmittanceatNormalIncidence(double infraredTransmittanceatNormalIncidence) {
    bool result = setDouble(OS_WindowMaterial_GlazingFields::InfraredTransmittanceatNormalIncidence, infraredTransmittanceatNormalIncidence);
    return result;
  }

  bool StandardGlazing_Impl::setInfraredTransmittanceatNormalIncidence(const Quantity& infraredTransmittanceatNormalIncidence) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_GlazingFields::InfraredTransmittanceatNormalIncidence,infraredTransmittanceatNormalIncidence);
    if (!value) {
      return false;
    }
    return setInfraredTransmittanceatNormalIncidence(value.get());
  }

  void StandardGlazing_Impl::resetInfraredTransmittanceatNormalIncidence() {
    bool result = setString(OS_WindowMaterial_GlazingFields::InfraredTransmittanceatNormalIncidence, "");
    OS_ASSERT(result);
  }

  bool StandardGlazing_Impl::setFrontSideInfraredHemisphericalEmissivity(double frontSideInfraredHemisphericalEmissivity) {
    bool result = setDouble(OS_WindowMaterial_GlazingFields::FrontSideInfraredHemisphericalEmissivity, frontSideInfraredHemisphericalEmissivity);
    return result;
  }

  bool StandardGlazing_Impl::setFrontSideInfraredHemisphericalEmissivity(const Quantity& frontSideInfraredHemisphericalEmissivity) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_GlazingFields::FrontSideInfraredHemisphericalEmissivity,frontSideInfraredHemisphericalEmissivity);
    if (!value) {
      return false;
    }
    return setFrontSideInfraredHemisphericalEmissivity(value.get());
  }

  void StandardGlazing_Impl::resetFrontSideInfraredHemisphericalEmissivity() {
    bool result = setString(OS_WindowMaterial_GlazingFields::FrontSideInfraredHemisphericalEmissivity, "");
    OS_ASSERT(result);
  }

  bool StandardGlazing_Impl::setBackSideInfraredHemisphericalEmissivity(double backSideInfraredHemisphericalEmissivity) {
    bool result = setDouble(OS_WindowMaterial_GlazingFields::BackSideInfraredHemisphericalEmissivity, backSideInfraredHemisphericalEmissivity);
    return result;
  }

  bool StandardGlazing_Impl::setBackSideInfraredHemisphericalEmissivity(const Quantity& backSideInfraredHemisphericalEmissivity) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_GlazingFields::BackSideInfraredHemisphericalEmissivity,backSideInfraredHemisphericalEmissivity);
    if (!value) {
      return false;
    }
    return setBackSideInfraredHemisphericalEmissivity(value.get());
  }

  void StandardGlazing_Impl::resetBackSideInfraredHemisphericalEmissivity() {
    bool result = setString(OS_WindowMaterial_GlazingFields::BackSideInfraredHemisphericalEmissivity, "");
    OS_ASSERT(result);
  }

  bool StandardGlazing_Impl::setThermalConductivity(double value) {
    return setDouble(OS_WindowMaterial_GlazingFields::Conductivity,value);
  }

  bool StandardGlazing_Impl::setConductivity(double conductivity) {
    bool result = setDouble(OS_WindowMaterial_GlazingFields::Conductivity, conductivity);
    return result;
  }

  bool StandardGlazing_Impl::setConductivity(const Quantity& conductivity) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_GlazingFields::Conductivity,conductivity);
    if (!value) {
      return false;
    }
    return setConductivity(value.get());
  }

  void StandardGlazing_Impl::resetConductivity() {
    bool result = setString(OS_WindowMaterial_GlazingFields::Conductivity, "");
    OS_ASSERT(result);
  }

  bool StandardGlazing_Impl::setDirtCorrectionFactorforSolarandVisibleTransmittance(double dirtCorrectionFactorforSolarandVisibleTransmittance) {
    bool result = setDouble(OS_WindowMaterial_GlazingFields::DirtCorrectionFactorforSolarandVisibleTransmittance, dirtCorrectionFactorforSolarandVisibleTransmittance);
    return result;
  }

  bool StandardGlazing_Impl::setDirtCorrectionFactorforSolarandVisibleTransmittance(const Quantity& dirtCorrectionFactorforSolarandVisibleTransmittance) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_GlazingFields::DirtCorrectionFactorforSolarandVisibleTransmittance,dirtCorrectionFactorforSolarandVisibleTransmittance);
    if (!value) {
      return false;
    }
    return setDirtCorrectionFactorforSolarandVisibleTransmittance(value.get());
  }

  void StandardGlazing_Impl::resetDirtCorrectionFactorforSolarandVisibleTransmittance() {
    bool result = setString(OS_WindowMaterial_GlazingFields::DirtCorrectionFactorforSolarandVisibleTransmittance, "");
    OS_ASSERT(result);
  }

  void StandardGlazing_Impl::setSolarDiffusing(bool solarDiffusing) {
    setBooleanFieldValue(OS_WindowMaterial_GlazingFields::SolarDiffusing, solarDiffusing);
  }

  void StandardGlazing_Impl::resetSolarDiffusing() {
    bool result = setString(OS_WindowMaterial_GlazingFields::SolarDiffusing, "");
    OS_ASSERT(result);
  }

  double StandardGlazing_Impl::thermalConductance() const {
    return thermalConductivity()/thickness();
  }

  double StandardGlazing_Impl::thermalResistivity() const {
    return 1.0/thermalConductivity();
  }

  double StandardGlazing_Impl::thermalResistance() const {
    return 1.0/thermalConductance();
  }

  double StandardGlazing_Impl::exteriorVisibleReflectance() const {
    OptionalDouble od = getDouble(OS_WindowMaterial_GlazingFields::FrontSideVisibleReflectanceatNormalIncidence,true);
    if (!od) {
      LOG_AND_THROW("Front-side visible reflectance is not yet set for " << briefDescription() << ".");
    }
    return *od;
  }

  double StandardGlazing_Impl::interiorVisibleReflectance() const {
    OptionalDouble od = getDouble(OS_WindowMaterial_GlazingFields::BackSideVisibleReflectanceatNormalIncidence,true);
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

  bool StandardGlazing_Impl::setThermalConductance(double value) {
    return setThickness(thermalConductivity()/value);
  }

  bool StandardGlazing_Impl::setThermalResistivity(double value) {
    return setThermalConductivity(1.0/value);
  }

  bool StandardGlazing_Impl::setThermalResistance(double value) {
    return setThickness(value/thermalResistivity());
  }

  std::vector<std::string> StandardGlazing_Impl::opticalDataTypeValues() const {
    return StandardGlazing::opticalDataTypeValues();
  }

  openstudio::Quantity StandardGlazing_Impl::thickness_SI() const {
    return getThickness(false);
  }

  openstudio::Quantity StandardGlazing_Impl::thickness_IP() const {
    return getThickness(true);
  }

  openstudio::OSOptionalQuantity StandardGlazing_Impl::solarTransmittanceatNormalIncidence_SI() const {
    return getSolarTransmittanceatNormalIncidence(false);
  }

  openstudio::OSOptionalQuantity StandardGlazing_Impl::solarTransmittanceatNormalIncidence_IP() const {
    return getSolarTransmittanceatNormalIncidence(true);
  }

  openstudio::OSOptionalQuantity StandardGlazing_Impl::frontSideSolarReflectanceatNormalIncidence_SI() const {
    return getFrontSideSolarReflectanceatNormalIncidence(false);
  }

  openstudio::OSOptionalQuantity StandardGlazing_Impl::frontSideSolarReflectanceatNormalIncidence_IP() const {
    return getFrontSideSolarReflectanceatNormalIncidence(true);
  }

  openstudio::OSOptionalQuantity StandardGlazing_Impl::backSideSolarReflectanceatNormalIncidence_SI() const {
    return getBackSideSolarReflectanceatNormalIncidence(false);
  }

  openstudio::OSOptionalQuantity StandardGlazing_Impl::backSideSolarReflectanceatNormalIncidence_IP() const {
    return getBackSideSolarReflectanceatNormalIncidence(true);
  }

  openstudio::OSOptionalQuantity StandardGlazing_Impl::visibleTransmittanceatNormalIncidence_SI() const {
    return getVisibleTransmittanceatNormalIncidence(false);
  }

  openstudio::OSOptionalQuantity StandardGlazing_Impl::visibleTransmittanceatNormalIncidence_IP() const {
    return getVisibleTransmittanceatNormalIncidence(true);
  }

  openstudio::OSOptionalQuantity StandardGlazing_Impl::frontSideVisibleReflectanceatNormalIncidence_SI() const {
    return getFrontSideVisibleReflectanceatNormalIncidence(false);
  }

  openstudio::OSOptionalQuantity StandardGlazing_Impl::frontSideVisibleReflectanceatNormalIncidence_IP() const {
    return getFrontSideVisibleReflectanceatNormalIncidence(true);
  }

  openstudio::OSOptionalQuantity StandardGlazing_Impl::backSideVisibleReflectanceatNormalIncidence_SI() const {
    return getBackSideVisibleReflectanceatNormalIncidence(false);
  }

  openstudio::OSOptionalQuantity StandardGlazing_Impl::backSideVisibleReflectanceatNormalIncidence_IP() const {
    return getBackSideVisibleReflectanceatNormalIncidence(true);
  }

  openstudio::Quantity StandardGlazing_Impl::infraredTransmittanceatNormalIncidence_SI() const {
    return getInfraredTransmittanceatNormalIncidence(false);
  }

  openstudio::Quantity StandardGlazing_Impl::infraredTransmittanceatNormalIncidence_IP() const {
    return getInfraredTransmittanceatNormalIncidence(true);
  }

  openstudio::Quantity StandardGlazing_Impl::frontSideInfraredHemisphericalEmissivity_SI() const {
    return getFrontSideInfraredHemisphericalEmissivity(false);
  }

  openstudio::Quantity StandardGlazing_Impl::frontSideInfraredHemisphericalEmissivity_IP() const {
    return getFrontSideInfraredHemisphericalEmissivity(true);
  }

  openstudio::Quantity StandardGlazing_Impl::backSideInfraredHemisphericalEmissivity_SI() const {
    return getBackSideInfraredHemisphericalEmissivity(false);
  }

  openstudio::Quantity StandardGlazing_Impl::backSideInfraredHemisphericalEmissivity_IP() const {
    return getBackSideInfraredHemisphericalEmissivity(true);
  }

  openstudio::Quantity StandardGlazing_Impl::conductivity_SI() const {
    return getConductivity(false);
  }

  openstudio::Quantity StandardGlazing_Impl::conductivity_IP() const {
    return getConductivity(true);
  }

  openstudio::Quantity StandardGlazing_Impl::dirtCorrectionFactorforSolarandVisibleTransmittance_SI() const {
    return getDirtCorrectionFactorforSolarandVisibleTransmittance(false);
  }

  openstudio::Quantity StandardGlazing_Impl::dirtCorrectionFactorforSolarandVisibleTransmittance_IP() const {
    return getDirtCorrectionFactorforSolarandVisibleTransmittance(true);
  }

} // detail

StandardGlazing::StandardGlazing(const Model& model,
                 std::string opticalDataType,
                 double thickness)
  : Glazing(StandardGlazing::iddObjectType(),model)
{
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
  return IddObjectType(IddObjectType::OS_WindowMaterial_Glazing);
}

std::vector<std::string> StandardGlazing::opticalDataTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_WindowMaterial_GlazingFields::OpticalDataType);
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

double StandardGlazing::thickness() const {
  return getImpl<detail::StandardGlazing_Impl>()->thickness();
}

Quantity StandardGlazing::getThickness(bool returnIP) const {
  return getImpl<detail::StandardGlazing_Impl>()->getThickness(returnIP);
}

boost::optional<double> StandardGlazing::solarTransmittanceatNormalIncidence() const {
  return getImpl<detail::StandardGlazing_Impl>()->solarTransmittanceatNormalIncidence();
}

OSOptionalQuantity StandardGlazing::getSolarTransmittanceatNormalIncidence(bool returnIP) const {
  return getImpl<detail::StandardGlazing_Impl>()->getSolarTransmittanceatNormalIncidence(returnIP);
}

boost::optional<double> StandardGlazing::frontSideSolarReflectanceatNormalIncidence() const {
  return getImpl<detail::StandardGlazing_Impl>()->frontSideSolarReflectanceatNormalIncidence();
}

OSOptionalQuantity StandardGlazing::getFrontSideSolarReflectanceatNormalIncidence(bool returnIP) const {
  return getImpl<detail::StandardGlazing_Impl>()->getFrontSideSolarReflectanceatNormalIncidence(returnIP);
}

boost::optional<double> StandardGlazing::backSideSolarReflectanceatNormalIncidence() const {
  return getImpl<detail::StandardGlazing_Impl>()->backSideSolarReflectanceatNormalIncidence();
}

OSOptionalQuantity StandardGlazing::getBackSideSolarReflectanceatNormalIncidence(bool returnIP) const {
  return getImpl<detail::StandardGlazing_Impl>()->getBackSideSolarReflectanceatNormalIncidence(returnIP);
}

boost::optional<double> StandardGlazing::visibleTransmittanceatNormalIncidence() const {
  return getImpl<detail::StandardGlazing_Impl>()->visibleTransmittanceatNormalIncidence();
}

OSOptionalQuantity StandardGlazing::getVisibleTransmittanceatNormalIncidence(bool returnIP) const {
  return getImpl<detail::StandardGlazing_Impl>()->getVisibleTransmittanceatNormalIncidence(returnIP);
}

boost::optional<double> StandardGlazing::frontSideVisibleReflectanceatNormalIncidence() const {
  return getImpl<detail::StandardGlazing_Impl>()->frontSideVisibleReflectanceatNormalIncidence();
}

OSOptionalQuantity StandardGlazing::getFrontSideVisibleReflectanceatNormalIncidence(bool returnIP) const {
  return getImpl<detail::StandardGlazing_Impl>()->getFrontSideVisibleReflectanceatNormalIncidence(returnIP);
}

boost::optional<double> StandardGlazing::backSideVisibleReflectanceatNormalIncidence() const {
  return getImpl<detail::StandardGlazing_Impl>()->backSideVisibleReflectanceatNormalIncidence();
}

double StandardGlazing::infraredTransmittance() const {
  return getImpl<detail::StandardGlazing_Impl>()->infraredTransmittance();
}

OSOptionalQuantity StandardGlazing::getBackSideVisibleReflectanceatNormalIncidence(bool returnIP) const {
  return getImpl<detail::StandardGlazing_Impl>()->getBackSideVisibleReflectanceatNormalIncidence(returnIP);
}

double StandardGlazing::infraredTransmittanceatNormalIncidence() const {
  return getImpl<detail::StandardGlazing_Impl>()->infraredTransmittanceatNormalIncidence();
}

Quantity StandardGlazing::getInfraredTransmittanceatNormalIncidence(bool returnIP) const {
  return getImpl<detail::StandardGlazing_Impl>()->getInfraredTransmittanceatNormalIncidence(returnIP);
}

bool StandardGlazing::isInfraredTransmittanceatNormalIncidenceDefaulted() const {
  return getImpl<detail::StandardGlazing_Impl>()->isInfraredTransmittanceatNormalIncidenceDefaulted();
}

double StandardGlazing::frontSideInfraredHemisphericalEmissivity() const {
  return getImpl<detail::StandardGlazing_Impl>()->frontSideInfraredHemisphericalEmissivity();
}

Quantity StandardGlazing::getFrontSideInfraredHemisphericalEmissivity(bool returnIP) const {
  return getImpl<detail::StandardGlazing_Impl>()->getFrontSideInfraredHemisphericalEmissivity(returnIP);
}

bool StandardGlazing::isFrontSideInfraredHemisphericalEmissivityDefaulted() const {
  return getImpl<detail::StandardGlazing_Impl>()->isFrontSideInfraredHemisphericalEmissivityDefaulted();
}

double StandardGlazing::backSideInfraredHemisphericalEmissivity() const {
  return getImpl<detail::StandardGlazing_Impl>()->backSideInfraredHemisphericalEmissivity();
}

Quantity StandardGlazing::getBackSideInfraredHemisphericalEmissivity(bool returnIP) const {
  return getImpl<detail::StandardGlazing_Impl>()->getBackSideInfraredHemisphericalEmissivity(returnIP);
}

bool StandardGlazing::isBackSideInfraredHemisphericalEmissivityDefaulted() const {
  return getImpl<detail::StandardGlazing_Impl>()->isBackSideInfraredHemisphericalEmissivityDefaulted();
}

double StandardGlazing::thermalConductivity() const {
  return getImpl<detail::StandardGlazing_Impl>()->thermalConductivity();
}

double StandardGlazing::conductivity() const {
  return getImpl<detail::StandardGlazing_Impl>()->conductivity();
}

Quantity StandardGlazing::getConductivity(bool returnIP) const {
  return getImpl<detail::StandardGlazing_Impl>()->getConductivity(returnIP);
}

bool StandardGlazing::isConductivityDefaulted() const {
  return getImpl<detail::StandardGlazing_Impl>()->isConductivityDefaulted();
}

double StandardGlazing::dirtCorrectionFactorforSolarandVisibleTransmittance() const {
  return getImpl<detail::StandardGlazing_Impl>()->dirtCorrectionFactorforSolarandVisibleTransmittance();
}

Quantity StandardGlazing::getDirtCorrectionFactorforSolarandVisibleTransmittance(bool returnIP) const {
  return getImpl<detail::StandardGlazing_Impl>()->getDirtCorrectionFactorforSolarandVisibleTransmittance(returnIP);
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

bool StandardGlazing::setOpticalDataType(std::string opticalDataType) {
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

bool StandardGlazing::setThickness(double thickness) {
  return getImpl<detail::StandardGlazing_Impl>()->setThickness(thickness);
}

bool StandardGlazing::setThickness(const Quantity& thickness) {
  return getImpl<detail::StandardGlazing_Impl>()->setThickness(thickness);
}

bool StandardGlazing::setSolarTransmittanceatNormalIncidence(double solarTransmittanceatNormalIncidence) {
  return getImpl<detail::StandardGlazing_Impl>()->setSolarTransmittanceatNormalIncidence(solarTransmittanceatNormalIncidence);
}

bool StandardGlazing::setSolarTransmittanceatNormalIncidence(const Quantity& solarTransmittanceatNormalIncidence) {
  return getImpl<detail::StandardGlazing_Impl>()->setSolarTransmittanceatNormalIncidence(solarTransmittanceatNormalIncidence);
}

void StandardGlazing::resetSolarTransmittanceatNormalIncidence() {
  getImpl<detail::StandardGlazing_Impl>()->resetSolarTransmittanceatNormalIncidence();
}

bool StandardGlazing::setFrontSideSolarReflectanceatNormalIncidence(double frontSideSolarReflectanceatNormalIncidence) {
  return getImpl<detail::StandardGlazing_Impl>()->setFrontSideSolarReflectanceatNormalIncidence(frontSideSolarReflectanceatNormalIncidence);
}

bool StandardGlazing::setFrontSideSolarReflectanceatNormalIncidence(const Quantity& frontSideSolarReflectanceatNormalIncidence) {
  return getImpl<detail::StandardGlazing_Impl>()->setFrontSideSolarReflectanceatNormalIncidence(frontSideSolarReflectanceatNormalIncidence);
}

void StandardGlazing::resetFrontSideSolarReflectanceatNormalIncidence() {
  getImpl<detail::StandardGlazing_Impl>()->resetFrontSideSolarReflectanceatNormalIncidence();
}

bool StandardGlazing::setBackSideSolarReflectanceatNormalIncidence(double backSideSolarReflectanceatNormalIncidence) {
  return getImpl<detail::StandardGlazing_Impl>()->setBackSideSolarReflectanceatNormalIncidence(backSideSolarReflectanceatNormalIncidence);
}

bool StandardGlazing::setBackSideSolarReflectanceatNormalIncidence(const Quantity& backSideSolarReflectanceatNormalIncidence) {
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

bool StandardGlazing::setVisibleTransmittanceatNormalIncidence(const Quantity& visibleTransmittanceatNormalIncidence) {
  return getImpl<detail::StandardGlazing_Impl>()->setVisibleTransmittanceatNormalIncidence(visibleTransmittanceatNormalIncidence);
}

void StandardGlazing::resetVisibleTransmittanceatNormalIncidence() {
  getImpl<detail::StandardGlazing_Impl>()->resetVisibleTransmittanceatNormalIncidence();
}

bool StandardGlazing::setFrontSideVisibleReflectanceatNormalIncidence(double frontSideVisibleReflectanceatNormalIncidence) {
  return getImpl<detail::StandardGlazing_Impl>()->setFrontSideVisibleReflectanceatNormalIncidence(frontSideVisibleReflectanceatNormalIncidence);
}

bool StandardGlazing::setFrontSideVisibleReflectanceatNormalIncidence(const Quantity& frontSideVisibleReflectanceatNormalIncidence) {
  return getImpl<detail::StandardGlazing_Impl>()->setFrontSideVisibleReflectanceatNormalIncidence(frontSideVisibleReflectanceatNormalIncidence);
}

void StandardGlazing::resetFrontSideVisibleReflectanceatNormalIncidence() {
  getImpl<detail::StandardGlazing_Impl>()->resetFrontSideVisibleReflectanceatNormalIncidence();
}

bool StandardGlazing::setBackSideVisibleReflectanceatNormalIncidence(double backSideVisibleReflectanceatNormalIncidence) {
  return getImpl<detail::StandardGlazing_Impl>()->setBackSideVisibleReflectanceatNormalIncidence(backSideVisibleReflectanceatNormalIncidence);
}

bool StandardGlazing::setBackSideVisibleReflectanceatNormalIncidence(const Quantity& backSideVisibleReflectanceatNormalIncidence) {
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

bool StandardGlazing::setInfraredTransmittanceatNormalIncidence(const Quantity& infraredTransmittanceatNormalIncidence) {
  return getImpl<detail::StandardGlazing_Impl>()->setInfraredTransmittanceatNormalIncidence(infraredTransmittanceatNormalIncidence);
}

void StandardGlazing::resetInfraredTransmittanceatNormalIncidence() {
  getImpl<detail::StandardGlazing_Impl>()->resetInfraredTransmittanceatNormalIncidence();
}

bool StandardGlazing::setFrontSideInfraredHemisphericalEmissivity(double frontSideInfraredHemisphericalEmissivity) {
  return getImpl<detail::StandardGlazing_Impl>()->setFrontSideInfraredHemisphericalEmissivity(frontSideInfraredHemisphericalEmissivity);
}

bool StandardGlazing::setFrontSideInfraredHemisphericalEmissivity(const Quantity& frontSideInfraredHemisphericalEmissivity) {
  return getImpl<detail::StandardGlazing_Impl>()->setFrontSideInfraredHemisphericalEmissivity(frontSideInfraredHemisphericalEmissivity);
}

void StandardGlazing::resetFrontSideInfraredHemisphericalEmissivity() {
  getImpl<detail::StandardGlazing_Impl>()->resetFrontSideInfraredHemisphericalEmissivity();
}

bool StandardGlazing::setBackSideInfraredHemisphericalEmissivity(double backSideInfraredHemisphericalEmissivity) {
  return getImpl<detail::StandardGlazing_Impl>()->setBackSideInfraredHemisphericalEmissivity(backSideInfraredHemisphericalEmissivity);
}

bool StandardGlazing::setBackSideInfraredHemisphericalEmissivity(const Quantity& backSideInfraredHemisphericalEmissivity) {
  return getImpl<detail::StandardGlazing_Impl>()->setBackSideInfraredHemisphericalEmissivity(backSideInfraredHemisphericalEmissivity);
}

void StandardGlazing::resetBackSideInfraredHemisphericalEmissivity() {
  getImpl<detail::StandardGlazing_Impl>()->resetBackSideInfraredHemisphericalEmissivity();
}

bool StandardGlazing::setThermalConductivity(double value) {
  return getImpl<detail::StandardGlazing_Impl>()->setThermalConductivity(value);
}

bool StandardGlazing::setConductivity(double conductivity) {
  return getImpl<detail::StandardGlazing_Impl>()->setConductivity(conductivity);
}

bool StandardGlazing::setConductivity(const Quantity& conductivity) {
  return getImpl<detail::StandardGlazing_Impl>()->setConductivity(conductivity);
}

void StandardGlazing::resetConductivity() {
  getImpl<detail::StandardGlazing_Impl>()->resetConductivity();
}

bool StandardGlazing::setDirtCorrectionFactorforSolarandVisibleTransmittance(double dirtCorrectionFactorforSolarandVisibleTransmittance) {
  return getImpl<detail::StandardGlazing_Impl>()->setDirtCorrectionFactorforSolarandVisibleTransmittance(dirtCorrectionFactorforSolarandVisibleTransmittance);
}

bool StandardGlazing::setDirtCorrectionFactorforSolarandVisibleTransmittance(const Quantity& dirtCorrectionFactorforSolarandVisibleTransmittance) {
  return getImpl<detail::StandardGlazing_Impl>()->setDirtCorrectionFactorforSolarandVisibleTransmittance(dirtCorrectionFactorforSolarandVisibleTransmittance);
}

void StandardGlazing::resetDirtCorrectionFactorforSolarandVisibleTransmittance() {
  getImpl<detail::StandardGlazing_Impl>()->resetDirtCorrectionFactorforSolarandVisibleTransmittance();
}

void StandardGlazing::setSolarDiffusing(bool solarDiffusing) {
  getImpl<detail::StandardGlazing_Impl>()->setSolarDiffusing(solarDiffusing);
}

void StandardGlazing::resetSolarDiffusing() {
  getImpl<detail::StandardGlazing_Impl>()->resetSolarDiffusing();
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
StandardGlazing::StandardGlazing(std::shared_ptr<detail::StandardGlazing_Impl> impl)
  : Glazing(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

