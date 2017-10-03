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

#include "RefractionExtinctionGlazing.hpp"
#include "RefractionExtinctionGlazing_Impl.hpp"

#include <utilities/idd/OS_WindowMaterial_Glazing_RefractionExtinctionMethod_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  RefractionExtinctionGlazing_Impl::RefractionExtinctionGlazing_Impl(const IdfObject& idfObject,
                                                                     Model_Impl* model,
                                                                     bool keepHandle)
    : Glazing_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == RefractionExtinctionGlazing::iddObjectType());
  }

  RefractionExtinctionGlazing_Impl::RefractionExtinctionGlazing_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                     Model_Impl* model,
                                                                     bool keepHandle)
    : Glazing_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == RefractionExtinctionGlazing::iddObjectType());
  }

  RefractionExtinctionGlazing_Impl::RefractionExtinctionGlazing_Impl(const RefractionExtinctionGlazing_Impl& other,
                                                                     Model_Impl* model,
                                                                     bool keepHandle)
    : Glazing_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& RefractionExtinctionGlazing_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType RefractionExtinctionGlazing_Impl::iddObjectType() const {
    return RefractionExtinctionGlazing::iddObjectType();
  }

  double RefractionExtinctionGlazing_Impl::thickness() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::Thickness,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity RefractionExtinctionGlazing_Impl::getThickness(bool returnIP) const {
    OptionalDouble value = thickness();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::Thickness, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  double RefractionExtinctionGlazing_Impl::solarIndexofRefraction() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::SolarIndexofRefraction,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity RefractionExtinctionGlazing_Impl::getSolarIndexofRefraction(bool returnIP) const {
    OptionalDouble value = solarIndexofRefraction();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::SolarIndexofRefraction, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  double RefractionExtinctionGlazing_Impl::solarExtinctionCoefficient() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::SolarExtinctionCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity RefractionExtinctionGlazing_Impl::getSolarExtinctionCoefficient(bool returnIP) const {
    OptionalDouble value = solarExtinctionCoefficient();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::SolarExtinctionCoefficient, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  double RefractionExtinctionGlazing_Impl::visibleIndexofRefraction() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::VisibleIndexofRefraction,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity RefractionExtinctionGlazing_Impl::getVisibleIndexofRefraction(bool returnIP) const {
    OptionalDouble value = visibleIndexofRefraction();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::VisibleIndexofRefraction, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  double RefractionExtinctionGlazing_Impl::visibleExtinctionCoefficient() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::VisibleExtinctionCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity RefractionExtinctionGlazing_Impl::getVisibleExtinctionCoefficient(bool returnIP) const {
    OptionalDouble value = visibleExtinctionCoefficient();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::VisibleExtinctionCoefficient, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  double RefractionExtinctionGlazing_Impl::infraredTransmittanceatNormalIncidence() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::InfraredTransmittanceatNormalIncidence,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity RefractionExtinctionGlazing_Impl::getInfraredTransmittanceatNormalIncidence(bool returnIP) const {
    OptionalDouble value = infraredTransmittanceatNormalIncidence();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::InfraredTransmittanceatNormalIncidence, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool RefractionExtinctionGlazing_Impl::isInfraredTransmittanceatNormalIncidenceDefaulted() const {
    return isEmpty(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::InfraredTransmittanceatNormalIncidence);
  }

  double RefractionExtinctionGlazing_Impl::infraredHemisphericalEmissivity() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::InfraredHemisphericalEmissivity,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity RefractionExtinctionGlazing_Impl::getInfraredHemisphericalEmissivity(bool returnIP) const {
    OptionalDouble value = infraredHemisphericalEmissivity();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::InfraredHemisphericalEmissivity, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool RefractionExtinctionGlazing_Impl::isInfraredHemisphericalEmissivityDefaulted() const {
    return isEmpty(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::InfraredHemisphericalEmissivity);
  }

  double RefractionExtinctionGlazing_Impl::conductivity() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::Conductivity,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity RefractionExtinctionGlazing_Impl::getConductivity(bool returnIP) const {
    OptionalDouble value = conductivity();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::Conductivity, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool RefractionExtinctionGlazing_Impl::isConductivityDefaulted() const {
    return isEmpty(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::Conductivity);
  }

  double RefractionExtinctionGlazing_Impl::dirtCorrectionFactorforSolarandVisibleTransmittance() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::DirtCorrectionFactorforSolarandVisibleTransmittance,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity RefractionExtinctionGlazing_Impl::getDirtCorrectionFactorforSolarandVisibleTransmittance(bool returnIP) const {
    OptionalDouble value = dirtCorrectionFactorforSolarandVisibleTransmittance();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::DirtCorrectionFactorforSolarandVisibleTransmittance, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool RefractionExtinctionGlazing_Impl::isDirtCorrectionFactorforSolarandVisibleTransmittanceDefaulted() const {
    return isEmpty(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::DirtCorrectionFactorforSolarandVisibleTransmittance);
  }

  bool RefractionExtinctionGlazing_Impl::solarDiffusing() const {
    boost::optional<std::string> value = getString(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::SolarDiffusing,true);
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

  bool RefractionExtinctionGlazing_Impl::setThickness(const Quantity& thickness) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::Thickness,thickness);
    if (!value) {
      return false;
    }
    return setThickness(value.get());
  }

  double RefractionExtinctionGlazing_Impl::thermalConductivity() const {
    OptionalDouble od = getDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::Conductivity,true);
    if (!od) {
      LOG_AND_THROW("Thermal conductivity not yet defined for " << briefDescription() << ".");
    }
    return *od;
  }

  double RefractionExtinctionGlazing_Impl::thermalConductance() const {
    return thermalConductivity()/thickness();
  }

  double RefractionExtinctionGlazing_Impl::thermalResistivity() const {
    return 1.0/thermalConductivity();
  }

  double RefractionExtinctionGlazing_Impl::thermalResistance() const {
    return 1.0/thermalConductance();
  }

  double RefractionExtinctionGlazing_Impl::thermalTransmittance() const {
    OptionalDouble od = getDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::InfraredTransmittanceatNormalIncidence,true);
    if (!od) {
      LOG_AND_THROW("Thermal transmittance not yet defined for " << briefDescription() << ".");
    }
    return *od;
  }

  OptionalDouble RefractionExtinctionGlazing_Impl::getVisibleTransmittance() const {
    LOG_AND_THROW("Not yet implemented. Need to convert index of refraction and exinction coefficient to transmittance and absorptance, or look it up in the SQL file.");
    return 0.0;
  }

  double RefractionExtinctionGlazing_Impl::visibleAbsorptance() const {
    LOG_AND_THROW("Not yet implemented. Need to convert index of refraction and exinction coefficient to transmittance and absorptance, or look it up in the SQL file.");
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

  bool RefractionExtinctionGlazing_Impl::setSolarIndexofRefraction(const Quantity& solarIndexofRefraction) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::SolarIndexofRefraction,solarIndexofRefraction);
    if (!value) {
      return false;
    }
    return setSolarIndexofRefraction(value.get());
  }

  bool RefractionExtinctionGlazing_Impl::setSolarExtinctionCoefficient(double solarExtinctionCoefficient) {
    bool result = setDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::SolarExtinctionCoefficient, solarExtinctionCoefficient);
    return result;
  }

  bool RefractionExtinctionGlazing_Impl::setSolarExtinctionCoefficient(const Quantity& solarExtinctionCoefficient) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::SolarExtinctionCoefficient,solarExtinctionCoefficient);
    if (!value) {
      return false;
    }
    return setSolarExtinctionCoefficient(value.get());
  }

  bool RefractionExtinctionGlazing_Impl::setVisibleIndexofRefraction(double visibleIndexofRefraction) {
    bool result = setDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::VisibleIndexofRefraction, visibleIndexofRefraction);
    return result;
  }

  bool RefractionExtinctionGlazing_Impl::setVisibleIndexofRefraction(const Quantity& visibleIndexofRefraction) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::VisibleIndexofRefraction,visibleIndexofRefraction);
    if (!value) {
      return false;
    }
    return setVisibleIndexofRefraction(value.get());
  }

  bool RefractionExtinctionGlazing_Impl::setVisibleExtinctionCoefficient(double visibleExtinctionCoefficient) {
    bool result = setDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::VisibleExtinctionCoefficient, visibleExtinctionCoefficient);
    return result;
  }

  bool RefractionExtinctionGlazing_Impl::setVisibleExtinctionCoefficient(const Quantity& visibleExtinctionCoefficient) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::VisibleExtinctionCoefficient,visibleExtinctionCoefficient);
    if (!value) {
      return false;
    }
    return setVisibleExtinctionCoefficient(value.get());
  }

  bool RefractionExtinctionGlazing_Impl::setInfraredTransmittanceatNormalIncidence(double infraredTransmittanceatNormalIncidence) {
    bool result = setDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::InfraredTransmittanceatNormalIncidence, infraredTransmittanceatNormalIncidence);
    return result;
  }

  bool RefractionExtinctionGlazing_Impl::setInfraredTransmittanceatNormalIncidence(const Quantity& infraredTransmittanceatNormalIncidence) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::InfraredTransmittanceatNormalIncidence,infraredTransmittanceatNormalIncidence);
    if (!value) {
      return false;
    }
    return setInfraredTransmittanceatNormalIncidence(value.get());
  }

  void RefractionExtinctionGlazing_Impl::resetInfraredTransmittanceatNormalIncidence() {
    bool result = setString(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::InfraredTransmittanceatNormalIncidence, "");
    OS_ASSERT(result);
  }

  bool RefractionExtinctionGlazing_Impl::setInfraredHemisphericalEmissivity(double infraredHemisphericalEmissivity) {
    bool result = setDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::InfraredHemisphericalEmissivity, infraredHemisphericalEmissivity);
    return result;
  }

  bool RefractionExtinctionGlazing_Impl::setInfraredHemisphericalEmissivity(const Quantity& infraredHemisphericalEmissivity) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::InfraredHemisphericalEmissivity,infraredHemisphericalEmissivity);
    if (!value) {
      return false;
    }
    return setInfraredHemisphericalEmissivity(value.get());
  }

  void RefractionExtinctionGlazing_Impl::resetInfraredHemisphericalEmissivity() {
    bool result = setString(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::InfraredHemisphericalEmissivity, "");
    OS_ASSERT(result);
  }

  bool RefractionExtinctionGlazing_Impl::setConductivity(double conductivity) {
    bool result = setDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::Conductivity, conductivity);
    return result;
  }

  bool RefractionExtinctionGlazing_Impl::setConductivity(const Quantity& conductivity) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::Conductivity,conductivity);
    if (!value) {
      return false;
    }
    return setConductivity(value.get());
  }

  void RefractionExtinctionGlazing_Impl::resetConductivity() {
    bool result = setString(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::Conductivity, "");
    OS_ASSERT(result);
  }

  bool RefractionExtinctionGlazing_Impl::setThermalConductivity(double value) {
    return setDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::Conductivity,value);
  }

  bool RefractionExtinctionGlazing_Impl::setThermalConductance(double value) {
    return setThickness(thermalConductivity()/value);
  }

  bool RefractionExtinctionGlazing_Impl::setThermalResistivity(double value) {
    return setThermalConductivity(1.0/value);
  }

  bool RefractionExtinctionGlazing_Impl::setThermalResistance(double value) {
    return setThickness(value/thermalResistivity());
  }

  bool RefractionExtinctionGlazing_Impl::setThermalTransmittance(double value) {
    return setDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::InfraredTransmittanceatNormalIncidence,value);
  }

  bool RefractionExtinctionGlazing_Impl::setDirtCorrectionFactorforSolarandVisibleTransmittance(double dirtCorrectionFactorforSolarandVisibleTransmittance) {
    bool result = setDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::DirtCorrectionFactorforSolarandVisibleTransmittance, dirtCorrectionFactorforSolarandVisibleTransmittance);
    return result;
  }

  bool RefractionExtinctionGlazing_Impl::setDirtCorrectionFactorforSolarandVisibleTransmittance(const Quantity& dirtCorrectionFactorforSolarandVisibleTransmittance) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::DirtCorrectionFactorforSolarandVisibleTransmittance,dirtCorrectionFactorforSolarandVisibleTransmittance);
    if (!value) {
      return false;
    }
    return setDirtCorrectionFactorforSolarandVisibleTransmittance(value.get());
  }

  void RefractionExtinctionGlazing_Impl::resetDirtCorrectionFactorforSolarandVisibleTransmittance() {
    bool result = setString(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::DirtCorrectionFactorforSolarandVisibleTransmittance, "");
    OS_ASSERT(result);
  }

  void RefractionExtinctionGlazing_Impl::setSolarDiffusing(bool solarDiffusing) {
    if (solarDiffusing) {
      setBooleanFieldValue(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::SolarDiffusing, true);
    } else {
      setBooleanFieldValue(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::SolarDiffusing, false);
    }
  }

  void RefractionExtinctionGlazing_Impl::resetSolarDiffusing() {
    bool result = setString(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::SolarDiffusing, "");
    OS_ASSERT(result);
  }

  openstudio::Quantity RefractionExtinctionGlazing_Impl::thickness_SI() const {
    return getThickness(false);
  }

  openstudio::Quantity RefractionExtinctionGlazing_Impl::thickness_IP() const {
    return getThickness(true);
  }

  openstudio::Quantity RefractionExtinctionGlazing_Impl::solarIndexofRefraction_SI() const {
    return getSolarIndexofRefraction(false);
  }

  openstudio::Quantity RefractionExtinctionGlazing_Impl::solarIndexofRefraction_IP() const {
    return getSolarIndexofRefraction(true);
  }

  openstudio::Quantity RefractionExtinctionGlazing_Impl::solarExtinctionCoefficient_SI() const {
    return getSolarExtinctionCoefficient(false);
  }

  openstudio::Quantity RefractionExtinctionGlazing_Impl::solarExtinctionCoefficient_IP() const {
    return getSolarExtinctionCoefficient(true);
  }

  openstudio::Quantity RefractionExtinctionGlazing_Impl::visibleIndexofRefraction_SI() const {
    return getVisibleIndexofRefraction(false);
  }

  openstudio::Quantity RefractionExtinctionGlazing_Impl::visibleIndexofRefraction_IP() const {
    return getVisibleIndexofRefraction(true);
  }

  openstudio::Quantity RefractionExtinctionGlazing_Impl::visibleExtinctionCoefficient_SI() const {
    return getVisibleExtinctionCoefficient(false);
  }

  openstudio::Quantity RefractionExtinctionGlazing_Impl::visibleExtinctionCoefficient_IP() const {
    return getVisibleExtinctionCoefficient(true);
  }

  openstudio::Quantity RefractionExtinctionGlazing_Impl::infraredTransmittanceatNormalIncidence_SI() const {
    return getInfraredTransmittanceatNormalIncidence(false);
  }

  openstudio::Quantity RefractionExtinctionGlazing_Impl::infraredTransmittanceatNormalIncidence_IP() const {
    return getInfraredTransmittanceatNormalIncidence(true);
  }

  openstudio::Quantity RefractionExtinctionGlazing_Impl::infraredHemisphericalEmissivity_SI() const {
    return getInfraredHemisphericalEmissivity(false);
  }

  openstudio::Quantity RefractionExtinctionGlazing_Impl::infraredHemisphericalEmissivity_IP() const {
    return getInfraredHemisphericalEmissivity(true);
  }

  openstudio::Quantity RefractionExtinctionGlazing_Impl::conductivity_SI() const {
    return getConductivity(false);
  }

  openstudio::Quantity RefractionExtinctionGlazing_Impl::conductivity_IP() const {
    return getConductivity(true);
  }

  openstudio::Quantity RefractionExtinctionGlazing_Impl::dirtCorrectionFactorforSolarandVisibleTransmittance_SI() const {
    return getDirtCorrectionFactorforSolarandVisibleTransmittance(false);
  }

  openstudio::Quantity RefractionExtinctionGlazing_Impl::dirtCorrectionFactorforSolarandVisibleTransmittance_IP() const {
    return getDirtCorrectionFactorforSolarandVisibleTransmittance(true);
  }

} // detail

RefractionExtinctionGlazing::RefractionExtinctionGlazing(const Model& model,
                                                         double thickness,
                                                         double solarIndexofRefraction,
                                                         double solarExtinctionCoefficient,
                                                         double visibleIndexofRefraction,
                                                         double visibleExtinctionCoefficient)
                                                         //double dirtCorrectionFactorforSolarandVisibleTransmittance)
  : Glazing(RefractionExtinctionGlazing::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::RefractionExtinctionGlazing_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  //bool ok = true;
  //ok = setHandle();
  //OS_ASSERT(ok);
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
  return IddObjectType(IddObjectType::OS_WindowMaterial_Glazing_RefractionExtinctionMethod);
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

double RefractionExtinctionGlazing::thickness() const {
  return getImpl<detail::RefractionExtinctionGlazing_Impl>()->thickness();
}

Quantity RefractionExtinctionGlazing::getThickness(bool returnIP) const {
  return getImpl<detail::RefractionExtinctionGlazing_Impl>()->getThickness(returnIP);
}

double RefractionExtinctionGlazing::solarIndexofRefraction() const {
  return getImpl<detail::RefractionExtinctionGlazing_Impl>()->solarIndexofRefraction();
}

Quantity RefractionExtinctionGlazing::getSolarIndexofRefraction(bool returnIP) const {
  return getImpl<detail::RefractionExtinctionGlazing_Impl>()->getSolarIndexofRefraction(returnIP);
}

double RefractionExtinctionGlazing::solarExtinctionCoefficient() const {
  return getImpl<detail::RefractionExtinctionGlazing_Impl>()->solarExtinctionCoefficient();
}

Quantity RefractionExtinctionGlazing::getSolarExtinctionCoefficient(bool returnIP) const {
  return getImpl<detail::RefractionExtinctionGlazing_Impl>()->getSolarExtinctionCoefficient(returnIP);
}

double RefractionExtinctionGlazing::visibleIndexofRefraction() const {
  return getImpl<detail::RefractionExtinctionGlazing_Impl>()->visibleIndexofRefraction();
}

Quantity RefractionExtinctionGlazing::getVisibleIndexofRefraction(bool returnIP) const {
  return getImpl<detail::RefractionExtinctionGlazing_Impl>()->getVisibleIndexofRefraction(returnIP);
}

double RefractionExtinctionGlazing::visibleExtinctionCoefficient() const {
  return getImpl<detail::RefractionExtinctionGlazing_Impl>()->visibleExtinctionCoefficient();
}

Quantity RefractionExtinctionGlazing::getVisibleExtinctionCoefficient(bool returnIP) const {
  return getImpl<detail::RefractionExtinctionGlazing_Impl>()->getVisibleExtinctionCoefficient(returnIP);
}

double RefractionExtinctionGlazing::infraredTransmittanceatNormalIncidence() const {
  return getImpl<detail::RefractionExtinctionGlazing_Impl>()->infraredTransmittanceatNormalIncidence();
}

Quantity RefractionExtinctionGlazing::getInfraredTransmittanceatNormalIncidence(bool returnIP) const {
  return getImpl<detail::RefractionExtinctionGlazing_Impl>()->getInfraredTransmittanceatNormalIncidence(returnIP);
}

bool RefractionExtinctionGlazing::isInfraredTransmittanceatNormalIncidenceDefaulted() const {
  return getImpl<detail::RefractionExtinctionGlazing_Impl>()->isInfraredTransmittanceatNormalIncidenceDefaulted();
}

double RefractionExtinctionGlazing::infraredHemisphericalEmissivity() const {
  return getImpl<detail::RefractionExtinctionGlazing_Impl>()->infraredHemisphericalEmissivity();
}

Quantity RefractionExtinctionGlazing::getInfraredHemisphericalEmissivity(bool returnIP) const {
  return getImpl<detail::RefractionExtinctionGlazing_Impl>()->getInfraredHemisphericalEmissivity(returnIP);
}

bool RefractionExtinctionGlazing::isInfraredHemisphericalEmissivityDefaulted() const {
  return getImpl<detail::RefractionExtinctionGlazing_Impl>()->isInfraredHemisphericalEmissivityDefaulted();
}

double RefractionExtinctionGlazing::conductivity() const {
  return getImpl<detail::RefractionExtinctionGlazing_Impl>()->conductivity();
}

Quantity RefractionExtinctionGlazing::getConductivity(bool returnIP) const {
  return getImpl<detail::RefractionExtinctionGlazing_Impl>()->getConductivity(returnIP);
}

bool RefractionExtinctionGlazing::isConductivityDefaulted() const {
  return getImpl<detail::RefractionExtinctionGlazing_Impl>()->isConductivityDefaulted();
}

double RefractionExtinctionGlazing::dirtCorrectionFactorforSolarandVisibleTransmittance() const {
  return getImpl<detail::RefractionExtinctionGlazing_Impl>()->dirtCorrectionFactorforSolarandVisibleTransmittance();
}

Quantity RefractionExtinctionGlazing::getDirtCorrectionFactorforSolarandVisibleTransmittance(bool returnIP) const {
  return getImpl<detail::RefractionExtinctionGlazing_Impl>()->getDirtCorrectionFactorforSolarandVisibleTransmittance(returnIP);
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

bool RefractionExtinctionGlazing::setThickness(double thickness) {
  return getImpl<detail::RefractionExtinctionGlazing_Impl>()->setThickness(thickness);
}

bool RefractionExtinctionGlazing::setThickness(const Quantity& thickness) {
  return getImpl<detail::RefractionExtinctionGlazing_Impl>()->setThickness(thickness);
}

bool RefractionExtinctionGlazing::setSolarIndexofRefraction(double solarIndexofRefraction) {
  return getImpl<detail::RefractionExtinctionGlazing_Impl>()->setSolarIndexofRefraction(solarIndexofRefraction);
}

bool RefractionExtinctionGlazing::setSolarIndexofRefraction(const Quantity& solarIndexofRefraction) {
  return getImpl<detail::RefractionExtinctionGlazing_Impl>()->setSolarIndexofRefraction(solarIndexofRefraction);
}

bool RefractionExtinctionGlazing::setSolarExtinctionCoefficient(double solarExtinctionCoefficient) {
  return getImpl<detail::RefractionExtinctionGlazing_Impl>()->setSolarExtinctionCoefficient(solarExtinctionCoefficient);
}

bool RefractionExtinctionGlazing::setSolarExtinctionCoefficient(const Quantity& solarExtinctionCoefficient) {
  return getImpl<detail::RefractionExtinctionGlazing_Impl>()->setSolarExtinctionCoefficient(solarExtinctionCoefficient);
}

bool RefractionExtinctionGlazing::setVisibleIndexofRefraction(double visibleIndexofRefraction) {
  return getImpl<detail::RefractionExtinctionGlazing_Impl>()->setVisibleIndexofRefraction(visibleIndexofRefraction);
}

bool RefractionExtinctionGlazing::setVisibleIndexofRefraction(const Quantity& visibleIndexofRefraction) {
  return getImpl<detail::RefractionExtinctionGlazing_Impl>()->setVisibleIndexofRefraction(visibleIndexofRefraction);
}

bool RefractionExtinctionGlazing::setVisibleExtinctionCoefficient(double visibleExtinctionCoefficient) {
  return getImpl<detail::RefractionExtinctionGlazing_Impl>()->setVisibleExtinctionCoefficient(visibleExtinctionCoefficient);
}

bool RefractionExtinctionGlazing::setVisibleExtinctionCoefficient(const Quantity& visibleExtinctionCoefficient) {
  return getImpl<detail::RefractionExtinctionGlazing_Impl>()->setVisibleExtinctionCoefficient(visibleExtinctionCoefficient);
}

bool RefractionExtinctionGlazing::setInfraredTransmittanceatNormalIncidence(double infraredTransmittanceatNormalIncidence) {
  return getImpl<detail::RefractionExtinctionGlazing_Impl>()->setInfraredTransmittanceatNormalIncidence(infraredTransmittanceatNormalIncidence);
}

bool RefractionExtinctionGlazing::setInfraredTransmittanceatNormalIncidence(const Quantity& infraredTransmittanceatNormalIncidence) {
  return getImpl<detail::RefractionExtinctionGlazing_Impl>()->setInfraredTransmittanceatNormalIncidence(infraredTransmittanceatNormalIncidence);
}

void RefractionExtinctionGlazing::resetInfraredTransmittanceatNormalIncidence() {
  getImpl<detail::RefractionExtinctionGlazing_Impl>()->resetInfraredTransmittanceatNormalIncidence();
}

bool RefractionExtinctionGlazing::setInfraredHemisphericalEmissivity(double infraredHemisphericalEmissivity) {
  return getImpl<detail::RefractionExtinctionGlazing_Impl>()->setInfraredHemisphericalEmissivity(infraredHemisphericalEmissivity);
}

bool RefractionExtinctionGlazing::setInfraredHemisphericalEmissivity(const Quantity& infraredHemisphericalEmissivity) {
  return getImpl<detail::RefractionExtinctionGlazing_Impl>()->setInfraredHemisphericalEmissivity(infraredHemisphericalEmissivity);
}

void RefractionExtinctionGlazing::resetInfraredHemisphericalEmissivity() {
  getImpl<detail::RefractionExtinctionGlazing_Impl>()->resetInfraredHemisphericalEmissivity();
}

bool RefractionExtinctionGlazing::setConductivity(double conductivity) {
  return getImpl<detail::RefractionExtinctionGlazing_Impl>()->setConductivity(conductivity);
}

bool RefractionExtinctionGlazing::setConductivity(const Quantity& conductivity) {
  return getImpl<detail::RefractionExtinctionGlazing_Impl>()->setConductivity(conductivity);
}

void RefractionExtinctionGlazing::resetConductivity() {
  getImpl<detail::RefractionExtinctionGlazing_Impl>()->resetConductivity();
}

bool RefractionExtinctionGlazing::setDirtCorrectionFactorforSolarandVisibleTransmittance(double dirtCorrectionFactorforSolarandVisibleTransmittance) {
  return getImpl<detail::RefractionExtinctionGlazing_Impl>()->setDirtCorrectionFactorforSolarandVisibleTransmittance(dirtCorrectionFactorforSolarandVisibleTransmittance);
}

bool RefractionExtinctionGlazing::setDirtCorrectionFactorforSolarandVisibleTransmittance(const Quantity& dirtCorrectionFactorforSolarandVisibleTransmittance) {
  return getImpl<detail::RefractionExtinctionGlazing_Impl>()->setDirtCorrectionFactorforSolarandVisibleTransmittance(dirtCorrectionFactorforSolarandVisibleTransmittance);
}

void RefractionExtinctionGlazing::resetDirtCorrectionFactorforSolarandVisibleTransmittance() {
  getImpl<detail::RefractionExtinctionGlazing_Impl>()->resetDirtCorrectionFactorforSolarandVisibleTransmittance();
}

void RefractionExtinctionGlazing::setSolarDiffusing(bool solarDiffusing) {
  getImpl<detail::RefractionExtinctionGlazing_Impl>()->setSolarDiffusing(solarDiffusing);
}

void RefractionExtinctionGlazing::resetSolarDiffusing() {
  getImpl<detail::RefractionExtinctionGlazing_Impl>()->resetSolarDiffusing();
}

/// @cond
RefractionExtinctionGlazing::RefractionExtinctionGlazing(std::shared_ptr<detail::RefractionExtinctionGlazing_Impl> impl)
  : Glazing(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

