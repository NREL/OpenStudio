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

#include "Blind.hpp"
#include "Blind_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_WindowMaterial_Blind_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  Blind_Impl::Blind_Impl(const IdfObject& idfObject,
                         Model_Impl* model,
                         bool keepHandle)
    : ShadingMaterial_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == Blind::iddObjectType());
  }

  Blind_Impl::Blind_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                         Model_Impl* model,
                         bool keepHandle)
    : ShadingMaterial_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == Blind::iddObjectType());
  }

  Blind_Impl::Blind_Impl(const Blind_Impl& other,
                         Model_Impl* model,
                         bool keepHandle)
    : ShadingMaterial_Impl(other,model,keepHandle)
  {}

  double Blind_Impl::thickness() const {
    OptionalDouble od = getDouble(OS_WindowMaterial_BlindFields::SlatThickness,true);
    if (!od) {
      LOG_AND_THROW("Slat thickness not yet set for " << briefDescription() << ".");
    }
    return *od;
  }

  OptionalDouble Blind_Impl::getVisibleTransmittance() const {
    LOG_AND_THROW("Visible transmittance not supported for blinds.");
    OptionalDouble od(false);
    return od;
  }

  const std::vector<std::string>& Blind_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType Blind_Impl::iddObjectType() const {
    return Blind::iddObjectType();
  }

  bool Blind_Impl::setThickness(double value) {
    return setDouble(OS_WindowMaterial_BlindFields::SlatThickness,value);
  }

  std::string Blind_Impl::slatOrientation() const {
    boost::optional<std::string> value = getString(OS_WindowMaterial_BlindFields::SlatOrientation,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool Blind_Impl::isSlatOrientationDefaulted() const {
    return isEmpty(OS_WindowMaterial_BlindFields::SlatOrientation);
  }

  double Blind_Impl::slatWidth() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_BlindFields::SlatWidth,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity Blind_Impl::getSlatWidth(bool returnIP) const {
    OptionalDouble value = slatWidth();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_BlindFields::SlatWidth, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  double Blind_Impl::slatSeparation() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_BlindFields::SlatSeparation,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity Blind_Impl::getSlatSeparation(bool returnIP) const {
    OptionalDouble value = slatSeparation();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_BlindFields::SlatSeparation, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  double Blind_Impl::slatThickness() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_BlindFields::SlatThickness,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity Blind_Impl::getSlatThickness(bool returnIP) const {
    OptionalDouble value = slatThickness();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_BlindFields::SlatThickness, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool Blind_Impl::isSlatThicknessDefaulted() const {
    return isEmpty(OS_WindowMaterial_BlindFields::SlatThickness);
  }

  double Blind_Impl::slatAngle() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_BlindFields::SlatAngle,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity Blind_Impl::getSlatAngle(bool returnIP) const {
    OptionalDouble value = slatAngle();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_BlindFields::SlatAngle, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool Blind_Impl::isSlatAngleDefaulted() const {
    return isEmpty(OS_WindowMaterial_BlindFields::SlatAngle);
  }

  double Blind_Impl::slatConductivity() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_BlindFields::SlatConductivity,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity Blind_Impl::getSlatConductivity(bool returnIP) const {
    OptionalDouble value = slatConductivity();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_BlindFields::SlatConductivity, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool Blind_Impl::isSlatConductivityDefaulted() const {
    return isEmpty(OS_WindowMaterial_BlindFields::SlatConductivity);
  }

  double Blind_Impl::slatBeamSolarTransmittance() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_BlindFields::SlatBeamSolarTransmittance,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity Blind_Impl::getSlatBeamSolarTransmittance(bool returnIP) const {
    OptionalDouble value = slatBeamSolarTransmittance();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_BlindFields::SlatBeamSolarTransmittance, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool Blind_Impl::isSlatBeamSolarTransmittanceDefaulted() const {
    return isEmpty(OS_WindowMaterial_BlindFields::SlatBeamSolarTransmittance);
  }

  double Blind_Impl::frontSideSlatBeamSolarReflectance() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_BlindFields::FrontSideSlatBeamSolarReflectance,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity Blind_Impl::getFrontSideSlatBeamSolarReflectance(bool returnIP) const {
    OptionalDouble value = frontSideSlatBeamSolarReflectance();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_BlindFields::FrontSideSlatBeamSolarReflectance, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  double Blind_Impl::backSideSlatBeamSolarReflectance() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_BlindFields::BackSideSlatBeamSolarReflectance,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity Blind_Impl::getBackSideSlatBeamSolarReflectance(bool returnIP) const {
    OptionalDouble value = backSideSlatBeamSolarReflectance();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_BlindFields::BackSideSlatBeamSolarReflectance, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  double Blind_Impl::slatDiffuseSolarTransmittance() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_BlindFields::SlatDiffuseSolarTransmittance,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity Blind_Impl::getSlatDiffuseSolarTransmittance(bool returnIP) const {
    OptionalDouble value = slatDiffuseSolarTransmittance();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_BlindFields::SlatDiffuseSolarTransmittance, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool Blind_Impl::isSlatDiffuseSolarTransmittanceDefaulted() const {
    return isEmpty(OS_WindowMaterial_BlindFields::SlatDiffuseSolarTransmittance);
  }

  double Blind_Impl::frontSideSlatDiffuseSolarReflectance() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_BlindFields::FrontSideSlatDiffuseSolarReflectance,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity Blind_Impl::getFrontSideSlatDiffuseSolarReflectance(bool returnIP) const {
    OptionalDouble value = frontSideSlatDiffuseSolarReflectance();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_BlindFields::FrontSideSlatDiffuseSolarReflectance, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  double Blind_Impl::backSideSlatDiffuseSolarReflectance() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_BlindFields::BackSideSlatDiffuseSolarReflectance,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity Blind_Impl::getBackSideSlatDiffuseSolarReflectance(bool returnIP) const {
    OptionalDouble value = backSideSlatDiffuseSolarReflectance();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_BlindFields::BackSideSlatDiffuseSolarReflectance, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  double Blind_Impl::slatBeamVisibleTransmittance() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_BlindFields::SlatBeamVisibleTransmittance,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity Blind_Impl::getSlatBeamVisibleTransmittance(bool returnIP) const {
    OptionalDouble value = slatBeamVisibleTransmittance();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_BlindFields::SlatBeamVisibleTransmittance, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  boost::optional<double> Blind_Impl::frontSideSlatBeamVisibleReflectance() const {
    return getDouble(OS_WindowMaterial_BlindFields::FrontSideSlatBeamVisibleReflectance,true);
  }

  OSOptionalQuantity Blind_Impl::getFrontSideSlatBeamVisibleReflectance(bool returnIP) const {
    OptionalDouble value = frontSideSlatBeamVisibleReflectance();
    return getQuantityFromDouble(OS_WindowMaterial_BlindFields::FrontSideSlatBeamVisibleReflectance, value, returnIP);
  }

  boost::optional<double> Blind_Impl::backSideSlatBeamVisibleReflectance() const {
    return getDouble(OS_WindowMaterial_BlindFields::BackSideSlatBeamVisibleReflectance,true);
  }

  OSOptionalQuantity Blind_Impl::getBackSideSlatBeamVisibleReflectance(bool returnIP) const {
    OptionalDouble value = backSideSlatBeamVisibleReflectance();
    return getQuantityFromDouble(OS_WindowMaterial_BlindFields::BackSideSlatBeamVisibleReflectance, value, returnIP);
  }

  double Blind_Impl::slatDiffuseVisibleTransmittance() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_BlindFields::SlatDiffuseVisibleTransmittance,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity Blind_Impl::getSlatDiffuseVisibleTransmittance(bool returnIP) const {
    OptionalDouble value = slatDiffuseVisibleTransmittance();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_BlindFields::SlatDiffuseVisibleTransmittance, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool Blind_Impl::isSlatDiffuseVisibleTransmittanceDefaulted() const {
    return isEmpty(OS_WindowMaterial_BlindFields::SlatDiffuseVisibleTransmittance);
  }

  boost::optional<double> Blind_Impl::frontSideSlatDiffuseVisibleReflectance() const {
    return getDouble(OS_WindowMaterial_BlindFields::FrontSideSlatDiffuseVisibleReflectance,true);
  }

  OSOptionalQuantity Blind_Impl::getFrontSideSlatDiffuseVisibleReflectance(bool returnIP) const {
    OptionalDouble value = frontSideSlatDiffuseVisibleReflectance();
    return getQuantityFromDouble(OS_WindowMaterial_BlindFields::FrontSideSlatDiffuseVisibleReflectance, value, returnIP);
  }

  boost::optional<double> Blind_Impl::backSideSlatDiffuseVisibleReflectance() const {
    return getDouble(OS_WindowMaterial_BlindFields::BackSideSlatDiffuseVisibleReflectance,true);
  }

  OSOptionalQuantity Blind_Impl::getBackSideSlatDiffuseVisibleReflectance(bool returnIP) const {
    OptionalDouble value = backSideSlatDiffuseVisibleReflectance();
    return getQuantityFromDouble(OS_WindowMaterial_BlindFields::BackSideSlatDiffuseVisibleReflectance, value, returnIP);
  }

  double Blind_Impl::slatInfraredHemisphericalTransmittance() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_BlindFields::SlatInfraredHemisphericalTransmittance,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity Blind_Impl::getSlatInfraredHemisphericalTransmittance(bool returnIP) const {
    OptionalDouble value = slatInfraredHemisphericalTransmittance();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_BlindFields::SlatInfraredHemisphericalTransmittance, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool Blind_Impl::isSlatInfraredHemisphericalTransmittanceDefaulted() const {
    return isEmpty(OS_WindowMaterial_BlindFields::SlatInfraredHemisphericalTransmittance);
  }

  double Blind_Impl::frontSideSlatInfraredHemisphericalEmissivity() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_BlindFields::FrontSideSlatInfraredHemisphericalEmissivity,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity Blind_Impl::getFrontSideSlatInfraredHemisphericalEmissivity(bool returnIP) const {
    OptionalDouble value = frontSideSlatInfraredHemisphericalEmissivity();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_BlindFields::FrontSideSlatInfraredHemisphericalEmissivity, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool Blind_Impl::isFrontSideSlatInfraredHemisphericalEmissivityDefaulted() const {
    return isEmpty(OS_WindowMaterial_BlindFields::FrontSideSlatInfraredHemisphericalEmissivity);
  }

  double Blind_Impl::backSideSlatInfraredHemisphericalEmissivity() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_BlindFields::BackSideSlatInfraredHemisphericalEmissivity,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity Blind_Impl::getBackSideSlatInfraredHemisphericalEmissivity(bool returnIP) const {
    OptionalDouble value = backSideSlatInfraredHemisphericalEmissivity();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_BlindFields::BackSideSlatInfraredHemisphericalEmissivity, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool Blind_Impl::isBackSideSlatInfraredHemisphericalEmissivityDefaulted() const {
    return isEmpty(OS_WindowMaterial_BlindFields::BackSideSlatInfraredHemisphericalEmissivity);
  }

  double Blind_Impl::blindtoGlassDistance() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_BlindFields::BlindtoGlassDistance,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity Blind_Impl::getBlindtoGlassDistance(bool returnIP) const {
    OptionalDouble value = blindtoGlassDistance();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_BlindFields::BlindtoGlassDistance, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool Blind_Impl::isBlindtoGlassDistanceDefaulted() const {
    return isEmpty(OS_WindowMaterial_BlindFields::BlindtoGlassDistance);
  }

  double Blind_Impl::blindTopOpeningMultiplier() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_BlindFields::BlindTopOpeningMultiplier,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity Blind_Impl::getBlindTopOpeningMultiplier(bool returnIP) const {
    OptionalDouble value = blindTopOpeningMultiplier();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_BlindFields::BlindTopOpeningMultiplier, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool Blind_Impl::isBlindTopOpeningMultiplierDefaulted() const {
    return isEmpty(OS_WindowMaterial_BlindFields::BlindTopOpeningMultiplier);
  }

  double Blind_Impl::blindBottomOpeningMultiplier() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_BlindFields::BlindBottomOpeningMultiplier,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity Blind_Impl::getBlindBottomOpeningMultiplier(bool returnIP) const {
    OptionalDouble value = blindBottomOpeningMultiplier();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_BlindFields::BlindBottomOpeningMultiplier, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool Blind_Impl::isBlindBottomOpeningMultiplierDefaulted() const {
    return isEmpty(OS_WindowMaterial_BlindFields::BlindBottomOpeningMultiplier);
  }

  double Blind_Impl::blindLeftSideOpeningMultiplier() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_BlindFields::BlindLeftSideOpeningMultiplier,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity Blind_Impl::getBlindLeftSideOpeningMultiplier(bool returnIP) const {
    OptionalDouble value = blindLeftSideOpeningMultiplier();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_BlindFields::BlindLeftSideOpeningMultiplier, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool Blind_Impl::isBlindLeftSideOpeningMultiplierDefaulted() const {
    return isEmpty(OS_WindowMaterial_BlindFields::BlindLeftSideOpeningMultiplier);
  }

  double Blind_Impl::blindRightSideOpeningMultiplier() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_BlindFields::BlindRightSideOpeningMultiplier,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity Blind_Impl::getBlindRightSideOpeningMultiplier(bool returnIP) const {
    OptionalDouble value = blindRightSideOpeningMultiplier();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_BlindFields::BlindRightSideOpeningMultiplier, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool Blind_Impl::isBlindRightSideOpeningMultiplierDefaulted() const {
    return isEmpty(OS_WindowMaterial_BlindFields::BlindRightSideOpeningMultiplier);
  }

  double Blind_Impl::minimumSlatAngle() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_BlindFields::MinimumSlatAngle,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity Blind_Impl::getMinimumSlatAngle(bool returnIP) const {
    OptionalDouble value = minimumSlatAngle();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_BlindFields::MinimumSlatAngle, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool Blind_Impl::isMinimumSlatAngleDefaulted() const {
    return isEmpty(OS_WindowMaterial_BlindFields::MinimumSlatAngle);
  }

  double Blind_Impl::maximumSlatAngle() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_BlindFields::MaximumSlatAngle,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity Blind_Impl::getMaximumSlatAngle(bool returnIP) const {
    OptionalDouble value = maximumSlatAngle();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_BlindFields::MaximumSlatAngle, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool Blind_Impl::isMaximumSlatAngleDefaulted() const {
    return isEmpty(OS_WindowMaterial_BlindFields::MaximumSlatAngle);
  }

  bool Blind_Impl::setSlatOrientation(std::string slatOrientation) {
    bool result = setString(OS_WindowMaterial_BlindFields::SlatOrientation, slatOrientation);
    return result;
  }

  void Blind_Impl::resetSlatOrientation() {
    bool result = setString(OS_WindowMaterial_BlindFields::SlatOrientation, "");
    OS_ASSERT(result);
  }

  bool Blind_Impl::setSlatWidth(double slatWidth) {
    bool result = setDouble(OS_WindowMaterial_BlindFields::SlatWidth, slatWidth);
    return result;
  }

  bool Blind_Impl::setSlatWidth(const Quantity& slatWidth) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_BlindFields::SlatWidth,slatWidth);
    if (!value) {
      return false;
    }
    return setSlatWidth(value.get());
  }

  bool Blind_Impl::setSlatSeparation(double slatSeparation) {
    bool result = setDouble(OS_WindowMaterial_BlindFields::SlatSeparation, slatSeparation);
    return result;
  }

  bool Blind_Impl::setSlatSeparation(const Quantity& slatSeparation) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_BlindFields::SlatSeparation,slatSeparation);
    if (!value) {
      return false;
    }
    return setSlatSeparation(value.get());
  }

  bool Blind_Impl::setSlatThickness(double slatThickness) {
    bool result = setDouble(OS_WindowMaterial_BlindFields::SlatThickness, slatThickness);
    return result;
  }

  bool Blind_Impl::setSlatThickness(const Quantity& slatThickness) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_BlindFields::SlatThickness,slatThickness);
    if (!value) {
      return false;
    }
    return setSlatThickness(value.get());
  }

  void Blind_Impl::resetSlatThickness() {
    bool result = setString(OS_WindowMaterial_BlindFields::SlatThickness, "");
    OS_ASSERT(result);
  }

  bool Blind_Impl::setSlatAngle(double slatAngle) {
    bool result = setDouble(OS_WindowMaterial_BlindFields::SlatAngle, slatAngle);
    return result;
  }

  bool Blind_Impl::setSlatAngle(const Quantity& slatAngle) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_BlindFields::SlatAngle,slatAngle);
    if (!value) {
      return false;
    }
    return setSlatAngle(value.get());
  }

  void Blind_Impl::resetSlatAngle() {
    bool result = setString(OS_WindowMaterial_BlindFields::SlatAngle, "");
    OS_ASSERT(result);
  }

  bool Blind_Impl::setSlatConductivity(double slatConductivity) {
    bool result = setDouble(OS_WindowMaterial_BlindFields::SlatConductivity, slatConductivity);
    return result;
  }

  bool Blind_Impl::setSlatConductivity(const Quantity& slatConductivity) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_BlindFields::SlatConductivity,slatConductivity);
    if (!value) {
      return false;
    }
    return setSlatConductivity(value.get());
  }

  void Blind_Impl::resetSlatConductivity() {
    bool result = setString(OS_WindowMaterial_BlindFields::SlatConductivity, "");
    OS_ASSERT(result);
  }

  bool Blind_Impl::setSlatBeamSolarTransmittance(double slatBeamSolarTransmittance) {
    bool result = setDouble(OS_WindowMaterial_BlindFields::SlatBeamSolarTransmittance, slatBeamSolarTransmittance);
    return result;
  }

  bool Blind_Impl::setSlatBeamSolarTransmittance(const Quantity& slatBeamSolarTransmittance) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_BlindFields::SlatBeamSolarTransmittance,slatBeamSolarTransmittance);
    if (!value) {
      return false;
    }
    return setSlatBeamSolarTransmittance(value.get());
  }

  void Blind_Impl::resetSlatBeamSolarTransmittance() {
    bool result = setString(OS_WindowMaterial_BlindFields::SlatBeamSolarTransmittance, "");
    OS_ASSERT(result);
  }

  bool Blind_Impl::setFrontSideSlatBeamSolarReflectance(double frontSideSlatBeamSolarReflectance) {
    bool result = setDouble(OS_WindowMaterial_BlindFields::FrontSideSlatBeamSolarReflectance, frontSideSlatBeamSolarReflectance);
    return result;
  }

  bool Blind_Impl::setFrontSideSlatBeamSolarReflectance(const Quantity& frontSideSlatBeamSolarReflectance) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_BlindFields::FrontSideSlatBeamSolarReflectance,frontSideSlatBeamSolarReflectance);
    if (!value) {
      return false;
    }
    return setFrontSideSlatBeamSolarReflectance(value.get());
  }

  bool Blind_Impl::setBackSideSlatBeamSolarReflectance(double backSideSlatBeamSolarReflectance) {
    bool result = setDouble(OS_WindowMaterial_BlindFields::BackSideSlatBeamSolarReflectance, backSideSlatBeamSolarReflectance);
    return result;
  }

  bool Blind_Impl::setBackSideSlatBeamSolarReflectance(const Quantity& backSideSlatBeamSolarReflectance) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_BlindFields::BackSideSlatBeamSolarReflectance,backSideSlatBeamSolarReflectance);
    if (!value) {
      return false;
    }
    return setBackSideSlatBeamSolarReflectance(value.get());
  }

  bool Blind_Impl::setSlatDiffuseSolarTransmittance(double slatDiffuseSolarTransmittance) {
    bool result = setDouble(OS_WindowMaterial_BlindFields::SlatDiffuseSolarTransmittance, slatDiffuseSolarTransmittance);
    return result;
  }

  bool Blind_Impl::setSlatDiffuseSolarTransmittance(const Quantity& slatDiffuseSolarTransmittance) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_BlindFields::SlatDiffuseSolarTransmittance,slatDiffuseSolarTransmittance);
    if (!value) {
      return false;
    }
    return setSlatDiffuseSolarTransmittance(value.get());
  }

  void Blind_Impl::resetSlatDiffuseSolarTransmittance() {
    bool result = setString(OS_WindowMaterial_BlindFields::SlatDiffuseSolarTransmittance, "");
    OS_ASSERT(result);
  }

  bool Blind_Impl::setFrontSideSlatDiffuseSolarReflectance(double frontSideSlatDiffuseSolarReflectance) {
    bool result = setDouble(OS_WindowMaterial_BlindFields::FrontSideSlatDiffuseSolarReflectance, frontSideSlatDiffuseSolarReflectance);
    return result;
  }

  bool Blind_Impl::setFrontSideSlatDiffuseSolarReflectance(const Quantity& frontSideSlatDiffuseSolarReflectance) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_BlindFields::FrontSideSlatDiffuseSolarReflectance,frontSideSlatDiffuseSolarReflectance);
    if (!value) {
      return false;
    }
    return setFrontSideSlatDiffuseSolarReflectance(value.get());
  }

  bool Blind_Impl::setBackSideSlatDiffuseSolarReflectance(double backSideSlatDiffuseSolarReflectance) {
    bool result = setDouble(OS_WindowMaterial_BlindFields::BackSideSlatDiffuseSolarReflectance, backSideSlatDiffuseSolarReflectance);
    return result;
  }

  bool Blind_Impl::setBackSideSlatDiffuseSolarReflectance(const Quantity& backSideSlatDiffuseSolarReflectance) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_BlindFields::BackSideSlatDiffuseSolarReflectance,backSideSlatDiffuseSolarReflectance);
    if (!value) {
      return false;
    }
    return setBackSideSlatDiffuseSolarReflectance(value.get());
  }

  bool Blind_Impl::setSlatBeamVisibleTransmittance(double slatBeamVisibleTransmittance) {
    bool result = setDouble(OS_WindowMaterial_BlindFields::SlatBeamVisibleTransmittance, slatBeamVisibleTransmittance);
    return result;
  }

  bool Blind_Impl::setSlatBeamVisibleTransmittance(const Quantity& slatBeamVisibleTransmittance) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_BlindFields::SlatBeamVisibleTransmittance,slatBeamVisibleTransmittance);
    if (!value) {
      return false;
    }
    return setSlatBeamVisibleTransmittance(value.get());
  }

  bool Blind_Impl::setFrontSideSlatBeamVisibleReflectance(boost::optional<double> frontSideSlatBeamVisibleReflectance) {
    bool result(false);
    if (frontSideSlatBeamVisibleReflectance) {
      result = setDouble(OS_WindowMaterial_BlindFields::FrontSideSlatBeamVisibleReflectance, frontSideSlatBeamVisibleReflectance.get());
    }
    else {
      resetFrontSideSlatBeamVisibleReflectance();
      result = true;
    }
    return result;
  }

  bool Blind_Impl::setFrontSideSlatBeamVisibleReflectance(const OSOptionalQuantity& frontSideSlatBeamVisibleReflectance) {
    bool result(false);
    OptionalDouble value;
    if (frontSideSlatBeamVisibleReflectance.isSet()) {
      value = getDoubleFromQuantity(OS_WindowMaterial_BlindFields::FrontSideSlatBeamVisibleReflectance,frontSideSlatBeamVisibleReflectance.get());
      if (value) {
        result = setFrontSideSlatBeamVisibleReflectance(value);
      }
    }
    else {
      result = setFrontSideSlatBeamVisibleReflectance(value);
    }
    return result;
  }

  void Blind_Impl::resetFrontSideSlatBeamVisibleReflectance() {
    bool result = setString(OS_WindowMaterial_BlindFields::FrontSideSlatBeamVisibleReflectance, "");
    OS_ASSERT(result);
  }

  bool Blind_Impl::setBackSideSlatBeamVisibleReflectance(boost::optional<double> backSideSlatBeamVisibleReflectance) {
    bool result(false);
    if (backSideSlatBeamVisibleReflectance) {
      result = setDouble(OS_WindowMaterial_BlindFields::BackSideSlatBeamVisibleReflectance, backSideSlatBeamVisibleReflectance.get());
    }
    else {
      resetBackSideSlatBeamVisibleReflectance();
      result = true;
    }
    return result;
  }

  bool Blind_Impl::setBackSideSlatBeamVisibleReflectance(const OSOptionalQuantity& backSideSlatBeamVisibleReflectance) {
    bool result(false);
    OptionalDouble value;
    if (backSideSlatBeamVisibleReflectance.isSet()) {
      value = getDoubleFromQuantity(OS_WindowMaterial_BlindFields::BackSideSlatBeamVisibleReflectance,backSideSlatBeamVisibleReflectance.get());
      if (value) {
        result = setBackSideSlatBeamVisibleReflectance(value);
      }
    }
    else {
      result = setBackSideSlatBeamVisibleReflectance(value);
    }
    return result;
  }

  void Blind_Impl::resetBackSideSlatBeamVisibleReflectance() {
    bool result = setString(OS_WindowMaterial_BlindFields::BackSideSlatBeamVisibleReflectance, "");
    OS_ASSERT(result);
  }

  bool Blind_Impl::setSlatDiffuseVisibleTransmittance(double slatDiffuseVisibleTransmittance) {
    bool result = setDouble(OS_WindowMaterial_BlindFields::SlatDiffuseVisibleTransmittance, slatDiffuseVisibleTransmittance);
    return result;
  }

  bool Blind_Impl::setSlatDiffuseVisibleTransmittance(const Quantity& slatDiffuseVisibleTransmittance) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_BlindFields::SlatDiffuseVisibleTransmittance,slatDiffuseVisibleTransmittance);
    if (!value) {
      return false;
    }
    return setSlatDiffuseVisibleTransmittance(value.get());
  }

  void Blind_Impl::resetSlatDiffuseVisibleTransmittance() {
    bool result = setString(OS_WindowMaterial_BlindFields::SlatDiffuseVisibleTransmittance, "");
    OS_ASSERT(result);
  }

  bool Blind_Impl::setFrontSideSlatDiffuseVisibleReflectance(boost::optional<double> frontSideSlatDiffuseVisibleReflectance) {
    bool result(false);
    if (frontSideSlatDiffuseVisibleReflectance) {
      result = setDouble(OS_WindowMaterial_BlindFields::FrontSideSlatDiffuseVisibleReflectance, frontSideSlatDiffuseVisibleReflectance.get());
    }
    else {
      resetFrontSideSlatDiffuseVisibleReflectance();
      result = true;
    }
    return result;
  }

  bool Blind_Impl::setFrontSideSlatDiffuseVisibleReflectance(const OSOptionalQuantity& frontSideSlatDiffuseVisibleReflectance) {
    bool result(false);
    OptionalDouble value;
    if (frontSideSlatDiffuseVisibleReflectance.isSet()) {
      value = getDoubleFromQuantity(OS_WindowMaterial_BlindFields::FrontSideSlatDiffuseVisibleReflectance,frontSideSlatDiffuseVisibleReflectance.get());
      if (value) {
        result = setFrontSideSlatDiffuseVisibleReflectance(value);
      }
    }
    else {
      result = setFrontSideSlatDiffuseVisibleReflectance(value);
    }
    return result;
  }

  void Blind_Impl::resetFrontSideSlatDiffuseVisibleReflectance() {
    bool result = setString(OS_WindowMaterial_BlindFields::FrontSideSlatDiffuseVisibleReflectance, "");
    OS_ASSERT(result);
  }

  bool Blind_Impl::setBackSideSlatDiffuseVisibleReflectance(boost::optional<double> backSideSlatDiffuseVisibleReflectance) {
    bool result(false);
    if (backSideSlatDiffuseVisibleReflectance) {
      result = setDouble(OS_WindowMaterial_BlindFields::BackSideSlatDiffuseVisibleReflectance, backSideSlatDiffuseVisibleReflectance.get());
    }
    else {
      resetBackSideSlatDiffuseVisibleReflectance();
      result = true;
    }
    return result;
  }

  bool Blind_Impl::setBackSideSlatDiffuseVisibleReflectance(const OSOptionalQuantity& backSideSlatDiffuseVisibleReflectance) {
    bool result(false);
    OptionalDouble value;
    if (backSideSlatDiffuseVisibleReflectance.isSet()) {
      value = getDoubleFromQuantity(OS_WindowMaterial_BlindFields::BackSideSlatDiffuseVisibleReflectance,backSideSlatDiffuseVisibleReflectance.get());
      if (value) {
        result = setBackSideSlatDiffuseVisibleReflectance(value);
      }
    }
    else {
      result = setBackSideSlatDiffuseVisibleReflectance(value);
    }
    return result;
  }

  void Blind_Impl::resetBackSideSlatDiffuseVisibleReflectance() {
    bool result = setString(OS_WindowMaterial_BlindFields::BackSideSlatDiffuseVisibleReflectance, "");
    OS_ASSERT(result);
  }

  bool Blind_Impl::setSlatInfraredHemisphericalTransmittance(double slatInfraredHemisphericalTransmittance) {
    bool result = setDouble(OS_WindowMaterial_BlindFields::SlatInfraredHemisphericalTransmittance, slatInfraredHemisphericalTransmittance);
    return result;
  }

  bool Blind_Impl::setSlatInfraredHemisphericalTransmittance(const Quantity& slatInfraredHemisphericalTransmittance) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_BlindFields::SlatInfraredHemisphericalTransmittance,slatInfraredHemisphericalTransmittance);
    if (!value) {
      return false;
    }
    return setSlatInfraredHemisphericalTransmittance(value.get());
  }

  void Blind_Impl::resetSlatInfraredHemisphericalTransmittance() {
    bool result = setString(OS_WindowMaterial_BlindFields::SlatInfraredHemisphericalTransmittance, "");
    OS_ASSERT(result);
  }

  bool Blind_Impl::setFrontSideSlatInfraredHemisphericalEmissivity(double frontSideSlatInfraredHemisphericalEmissivity) {
    bool result = setDouble(OS_WindowMaterial_BlindFields::FrontSideSlatInfraredHemisphericalEmissivity, frontSideSlatInfraredHemisphericalEmissivity);
    return result;
  }

  bool Blind_Impl::setFrontSideSlatInfraredHemisphericalEmissivity(const Quantity& frontSideSlatInfraredHemisphericalEmissivity) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_BlindFields::FrontSideSlatInfraredHemisphericalEmissivity,frontSideSlatInfraredHemisphericalEmissivity);
    if (!value) {
      return false;
    }
    return setFrontSideSlatInfraredHemisphericalEmissivity(value.get());
  }

  void Blind_Impl::resetFrontSideSlatInfraredHemisphericalEmissivity() {
    bool result = setString(OS_WindowMaterial_BlindFields::FrontSideSlatInfraredHemisphericalEmissivity, "");
    OS_ASSERT(result);
  }

  bool Blind_Impl::setBackSideSlatInfraredHemisphericalEmissivity(double backSideSlatInfraredHemisphericalEmissivity) {
    bool result = setDouble(OS_WindowMaterial_BlindFields::BackSideSlatInfraredHemisphericalEmissivity, backSideSlatInfraredHemisphericalEmissivity);
    return result;
  }

  bool Blind_Impl::setBackSideSlatInfraredHemisphericalEmissivity(const Quantity& backSideSlatInfraredHemisphericalEmissivity) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_BlindFields::BackSideSlatInfraredHemisphericalEmissivity,backSideSlatInfraredHemisphericalEmissivity);
    if (!value) {
      return false;
    }
    return setBackSideSlatInfraredHemisphericalEmissivity(value.get());
  }

  void Blind_Impl::resetBackSideSlatInfraredHemisphericalEmissivity() {
    bool result = setString(OS_WindowMaterial_BlindFields::BackSideSlatInfraredHemisphericalEmissivity, "");
    OS_ASSERT(result);
  }

  bool Blind_Impl::setBlindtoGlassDistance(double blindtoGlassDistance) {
    bool result = setDouble(OS_WindowMaterial_BlindFields::BlindtoGlassDistance, blindtoGlassDistance);
    return result;
  }

  bool Blind_Impl::setBlindtoGlassDistance(const Quantity& blindtoGlassDistance) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_BlindFields::BlindtoGlassDistance,blindtoGlassDistance);
    if (!value) {
      return false;
    }
    return setBlindtoGlassDistance(value.get());
  }

  void Blind_Impl::resetBlindtoGlassDistance() {
    bool result = setString(OS_WindowMaterial_BlindFields::BlindtoGlassDistance, "");
    OS_ASSERT(result);
  }

  bool Blind_Impl::setBlindTopOpeningMultiplier(double blindTopOpeningMultiplier) {
    bool result = setDouble(OS_WindowMaterial_BlindFields::BlindTopOpeningMultiplier, blindTopOpeningMultiplier);
    return result;
  }

  bool Blind_Impl::setBlindTopOpeningMultiplier(const Quantity& blindTopOpeningMultiplier) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_BlindFields::BlindTopOpeningMultiplier,blindTopOpeningMultiplier);
    if (!value) {
      return false;
    }
    return setBlindTopOpeningMultiplier(value.get());
  }

  void Blind_Impl::resetBlindTopOpeningMultiplier() {
    bool result = setString(OS_WindowMaterial_BlindFields::BlindTopOpeningMultiplier, "");
    OS_ASSERT(result);
  }

  bool Blind_Impl::setBlindBottomOpeningMultiplier(double blindBottomOpeningMultiplier) {
    bool result = setDouble(OS_WindowMaterial_BlindFields::BlindBottomOpeningMultiplier, blindBottomOpeningMultiplier);
    return result;
  }

  bool Blind_Impl::setBlindBottomOpeningMultiplier(const Quantity& blindBottomOpeningMultiplier) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_BlindFields::BlindBottomOpeningMultiplier,blindBottomOpeningMultiplier);
    if (!value) {
      return false;
    }
    return setBlindBottomOpeningMultiplier(value.get());
  }

  void Blind_Impl::resetBlindBottomOpeningMultiplier() {
    bool result = setString(OS_WindowMaterial_BlindFields::BlindBottomOpeningMultiplier, "");
    OS_ASSERT(result);
  }

  bool Blind_Impl::setBlindLeftSideOpeningMultiplier(double blindLeftSideOpeningMultiplier) {
    bool result = setDouble(OS_WindowMaterial_BlindFields::BlindLeftSideOpeningMultiplier, blindLeftSideOpeningMultiplier);
    return result;
  }

  bool Blind_Impl::setBlindLeftSideOpeningMultiplier(const Quantity& blindLeftSideOpeningMultiplier) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_BlindFields::BlindLeftSideOpeningMultiplier,blindLeftSideOpeningMultiplier);
    if (!value) {
      return false;
    }
    return setBlindLeftSideOpeningMultiplier(value.get());
  }

  void Blind_Impl::resetBlindLeftSideOpeningMultiplier() {
    bool result = setString(OS_WindowMaterial_BlindFields::BlindLeftSideOpeningMultiplier, "");
    OS_ASSERT(result);
  }

  bool Blind_Impl::setBlindRightSideOpeningMultiplier(double blindRightSideOpeningMultiplier) {
    bool result = setDouble(OS_WindowMaterial_BlindFields::BlindRightSideOpeningMultiplier, blindRightSideOpeningMultiplier);
    return result;
  }

  bool Blind_Impl::setBlindRightSideOpeningMultiplier(const Quantity& blindRightSideOpeningMultiplier) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_BlindFields::BlindRightSideOpeningMultiplier,blindRightSideOpeningMultiplier);
    if (!value) {
      return false;
    }
    return setBlindRightSideOpeningMultiplier(value.get());
  }

  void Blind_Impl::resetBlindRightSideOpeningMultiplier() {
    bool result = setString(OS_WindowMaterial_BlindFields::BlindRightSideOpeningMultiplier, "");
    OS_ASSERT(result);
  }

  bool Blind_Impl::setMinimumSlatAngle(double minimumSlatAngle) {
    bool result = setDouble(OS_WindowMaterial_BlindFields::MinimumSlatAngle, minimumSlatAngle);
    return result;
  }

  bool Blind_Impl::setMinimumSlatAngle(const Quantity& minimumSlatAngle) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_BlindFields::MinimumSlatAngle,minimumSlatAngle);
    if (!value) {
      return false;
    }
    return setMinimumSlatAngle(value.get());
  }

  void Blind_Impl::resetMinimumSlatAngle() {
    bool result = setString(OS_WindowMaterial_BlindFields::MinimumSlatAngle, "");
    OS_ASSERT(result);
  }

  bool Blind_Impl::setMaximumSlatAngle(double maximumSlatAngle) {
    bool result = setDouble(OS_WindowMaterial_BlindFields::MaximumSlatAngle, maximumSlatAngle);
    return result;
  }

  bool Blind_Impl::setMaximumSlatAngle(const Quantity& maximumSlatAngle) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_BlindFields::MaximumSlatAngle,maximumSlatAngle);
    if (!value) {
      return false;
    }
    return setMaximumSlatAngle(value.get());
  }

  void Blind_Impl::resetMaximumSlatAngle() {
    bool result = setString(OS_WindowMaterial_BlindFields::MaximumSlatAngle, "");
    OS_ASSERT(result);
  }

  std::vector<std::string> Blind_Impl::slatOrientationValues() const {
    return Blind::slatOrientationValues();
  }

  openstudio::Quantity Blind_Impl::slatWidth_SI() const {
    return getSlatWidth(false);
  }

  openstudio::Quantity Blind_Impl::slatWidth_IP() const {
    return getSlatWidth(true);
  }

  openstudio::Quantity Blind_Impl::slatSeparation_SI() const {
    return getSlatSeparation(false);
  }

  openstudio::Quantity Blind_Impl::slatSeparation_IP() const {
    return getSlatSeparation(true);
  }

  openstudio::Quantity Blind_Impl::slatThickness_SI() const {
    return getSlatThickness(false);
  }

  openstudio::Quantity Blind_Impl::slatThickness_IP() const {
    return getSlatThickness(true);
  }

  openstudio::Quantity Blind_Impl::slatAngle_SI() const {
    return getSlatAngle(false);
  }

  openstudio::Quantity Blind_Impl::slatAngle_IP() const {
    return getSlatAngle(true);
  }

  openstudio::Quantity Blind_Impl::slatConductivity_SI() const {
    return getSlatConductivity(false);
  }

  openstudio::Quantity Blind_Impl::slatConductivity_IP() const {
    return getSlatConductivity(true);
  }

  openstudio::Quantity Blind_Impl::slatBeamSolarTransmittance_SI() const {
    return getSlatBeamSolarTransmittance(false);
  }

  openstudio::Quantity Blind_Impl::slatBeamSolarTransmittance_IP() const {
    return getSlatBeamSolarTransmittance(true);
  }

  openstudio::Quantity Blind_Impl::frontSideSlatBeamSolarReflectance_SI() const {
    return getFrontSideSlatBeamSolarReflectance(false);
  }

  openstudio::Quantity Blind_Impl::frontSideSlatBeamSolarReflectance_IP() const {
    return getFrontSideSlatBeamSolarReflectance(true);
  }

  openstudio::Quantity Blind_Impl::backSideSlatBeamSolarReflectance_SI() const {
    return getBackSideSlatBeamSolarReflectance(false);
  }

  openstudio::Quantity Blind_Impl::backSideSlatBeamSolarReflectance_IP() const {
    return getBackSideSlatBeamSolarReflectance(true);
  }

  openstudio::Quantity Blind_Impl::slatDiffuseSolarTransmittance_SI() const {
    return getSlatDiffuseSolarTransmittance(false);
  }

  openstudio::Quantity Blind_Impl::slatDiffuseSolarTransmittance_IP() const {
    return getSlatDiffuseSolarTransmittance(true);
  }

  openstudio::Quantity Blind_Impl::frontSideSlatDiffuseSolarReflectance_SI() const {
    return getFrontSideSlatDiffuseSolarReflectance(false);
  }

  openstudio::Quantity Blind_Impl::frontSideSlatDiffuseSolarReflectance_IP() const {
    return getFrontSideSlatDiffuseSolarReflectance(true);
  }

  openstudio::Quantity Blind_Impl::backSideSlatDiffuseSolarReflectance_SI() const {
    return getBackSideSlatDiffuseSolarReflectance(false);
  }

  openstudio::Quantity Blind_Impl::backSideSlatDiffuseSolarReflectance_IP() const {
    return getBackSideSlatDiffuseSolarReflectance(true);
  }

  openstudio::Quantity Blind_Impl::slatBeamVisibleTransmittance_SI() const {
    return getSlatBeamVisibleTransmittance(false);
  }

  openstudio::Quantity Blind_Impl::slatBeamVisibleTransmittance_IP() const {
    return getSlatBeamVisibleTransmittance(true);
  }

  openstudio::OSOptionalQuantity Blind_Impl::frontSideSlatBeamVisibleReflectance_SI() const {
    return getFrontSideSlatBeamVisibleReflectance(false);
  }

  openstudio::OSOptionalQuantity Blind_Impl::frontSideSlatBeamVisibleReflectance_IP() const {
    return getFrontSideSlatBeamVisibleReflectance(true);
  }

  openstudio::OSOptionalQuantity Blind_Impl::backSideSlatBeamVisibleReflectance_SI() const {
    return getBackSideSlatBeamVisibleReflectance(false);
  }

  openstudio::OSOptionalQuantity Blind_Impl::backSideSlatBeamVisibleReflectance_IP() const {
    return getBackSideSlatBeamVisibleReflectance(true);
  }

  openstudio::Quantity Blind_Impl::slatDiffuseVisibleTransmittance_SI() const {
    return getSlatDiffuseVisibleTransmittance(false);
  }

  openstudio::Quantity Blind_Impl::slatDiffuseVisibleTransmittance_IP() const {
    return getSlatDiffuseVisibleTransmittance(true);
  }

  openstudio::OSOptionalQuantity Blind_Impl::frontSideSlatDiffuseVisibleReflectance_SI() const {
    return getFrontSideSlatDiffuseVisibleReflectance(false);
  }

  openstudio::OSOptionalQuantity Blind_Impl::frontSideSlatDiffuseVisibleReflectance_IP() const {
    return getFrontSideSlatDiffuseVisibleReflectance(true);
  }

  openstudio::OSOptionalQuantity Blind_Impl::backSideSlatDiffuseVisibleReflectance_SI() const {
    return getBackSideSlatDiffuseVisibleReflectance(false);
  }

  openstudio::OSOptionalQuantity Blind_Impl::backSideSlatDiffuseVisibleReflectance_IP() const {
    return getBackSideSlatDiffuseVisibleReflectance(true);
  }

  openstudio::Quantity Blind_Impl::slatInfraredHemisphericalTransmittance_SI() const {
    return getSlatInfraredHemisphericalTransmittance(false);
  }

  openstudio::Quantity Blind_Impl::slatInfraredHemisphericalTransmittance_IP() const {
    return getSlatInfraredHemisphericalTransmittance(true);
  }

  openstudio::Quantity Blind_Impl::frontSideSlatInfraredHemisphericalEmissivity_SI() const {
    return getFrontSideSlatInfraredHemisphericalEmissivity(false);
  }

  openstudio::Quantity Blind_Impl::frontSideSlatInfraredHemisphericalEmissivity_IP() const {
    return getFrontSideSlatInfraredHemisphericalEmissivity(true);
  }

  openstudio::Quantity Blind_Impl::backSideSlatInfraredHemisphericalEmissivity_SI() const {
    return getBackSideSlatInfraredHemisphericalEmissivity(false);
  }

  openstudio::Quantity Blind_Impl::backSideSlatInfraredHemisphericalEmissivity_IP() const {
    return getBackSideSlatInfraredHemisphericalEmissivity(true);
  }

  openstudio::Quantity Blind_Impl::blindtoGlassDistance_SI() const {
    return getBlindtoGlassDistance(false);
  }

  openstudio::Quantity Blind_Impl::blindtoGlassDistance_IP() const {
    return getBlindtoGlassDistance(true);
  }

  openstudio::Quantity Blind_Impl::blindTopOpeningMultiplier_SI() const {
    return getBlindTopOpeningMultiplier(false);
  }

  openstudio::Quantity Blind_Impl::blindTopOpeningMultiplier_IP() const {
    return getBlindTopOpeningMultiplier(true);
  }

  openstudio::Quantity Blind_Impl::blindBottomOpeningMultiplier_SI() const {
    return getBlindBottomOpeningMultiplier(false);
  }

  openstudio::Quantity Blind_Impl::blindBottomOpeningMultiplier_IP() const {
    return getBlindBottomOpeningMultiplier(true);
  }

  openstudio::Quantity Blind_Impl::blindLeftSideOpeningMultiplier_SI() const {
    return getBlindLeftSideOpeningMultiplier(false);
  }

  openstudio::Quantity Blind_Impl::blindLeftSideOpeningMultiplier_IP() const {
    return getBlindLeftSideOpeningMultiplier(true);
  }

  openstudio::Quantity Blind_Impl::blindRightSideOpeningMultiplier_SI() const {
    return getBlindRightSideOpeningMultiplier(false);
  }

  openstudio::Quantity Blind_Impl::blindRightSideOpeningMultiplier_IP() const {
    return getBlindRightSideOpeningMultiplier(true);
  }

  openstudio::Quantity Blind_Impl::minimumSlatAngle_SI() const {
    return getMinimumSlatAngle(false);
  }

  openstudio::Quantity Blind_Impl::minimumSlatAngle_IP() const {
    return getMinimumSlatAngle(true);
  }

  openstudio::Quantity Blind_Impl::maximumSlatAngle_SI() const {
    return getMaximumSlatAngle(false);
  }

  openstudio::Quantity Blind_Impl::maximumSlatAngle_IP() const {
    return getMaximumSlatAngle(true);
  }

} // detail

Blind::Blind(const Model& model,
             double slatWidth,
             double slatSeparation,
             double frontSideSlatBeamSolarReflectance,
             double backSideSlatBeamSolarReflectance,
             double frontSideSlatDiffuseSolarReflectance,
             double backSideSlatDiffuseSolarReflectance,
             double slatBeamVisibleTransmittance)
  : ShadingMaterial(Blind::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::Blind_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
  ok = setSlatWidth(slatWidth);
  OS_ASSERT(ok);
  ok = setSlatSeparation(slatSeparation);
  OS_ASSERT(ok);
  ok = setFrontSideSlatBeamSolarReflectance(frontSideSlatBeamSolarReflectance);
  OS_ASSERT(ok);
  ok = setBackSideSlatBeamSolarReflectance(backSideSlatBeamSolarReflectance);
  OS_ASSERT(ok);
  ok = setFrontSideSlatDiffuseSolarReflectance(frontSideSlatDiffuseSolarReflectance);
  OS_ASSERT(ok);
  ok = setBackSideSlatDiffuseSolarReflectance(backSideSlatDiffuseSolarReflectance);
  OS_ASSERT(ok);
  ok = setSlatBeamVisibleTransmittance(slatBeamVisibleTransmittance);
  OS_ASSERT(ok);
}

IddObjectType Blind::iddObjectType() {
  return IddObjectType(IddObjectType::OS_WindowMaterial_Blind);
}

std::vector<std::string> Blind::slatOrientationValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_WindowMaterial_BlindFields::SlatOrientation);
}

std::string Blind::slatOrientation() const {
  return getImpl<detail::Blind_Impl>()->slatOrientation();
}

bool Blind::isSlatOrientationDefaulted() const {
  return getImpl<detail::Blind_Impl>()->isSlatOrientationDefaulted();
}

double Blind::slatWidth() const {
  return getImpl<detail::Blind_Impl>()->slatWidth();
}

Quantity Blind::getSlatWidth(bool returnIP) const {
  return getImpl<detail::Blind_Impl>()->getSlatWidth(returnIP);
}

double Blind::slatSeparation() const {
  return getImpl<detail::Blind_Impl>()->slatSeparation();
}

Quantity Blind::getSlatSeparation(bool returnIP) const {
  return getImpl<detail::Blind_Impl>()->getSlatSeparation(returnIP);
}

double Blind::slatThickness() const {
  return getImpl<detail::Blind_Impl>()->slatThickness();
}

Quantity Blind::getSlatThickness(bool returnIP) const {
  return getImpl<detail::Blind_Impl>()->getSlatThickness(returnIP);
}

bool Blind::isSlatThicknessDefaulted() const {
  return getImpl<detail::Blind_Impl>()->isSlatThicknessDefaulted();
}

double Blind::slatAngle() const {
  return getImpl<detail::Blind_Impl>()->slatAngle();
}

Quantity Blind::getSlatAngle(bool returnIP) const {
  return getImpl<detail::Blind_Impl>()->getSlatAngle(returnIP);
}

bool Blind::isSlatAngleDefaulted() const {
  return getImpl<detail::Blind_Impl>()->isSlatAngleDefaulted();
}

double Blind::slatConductivity() const {
  return getImpl<detail::Blind_Impl>()->slatConductivity();
}

Quantity Blind::getSlatConductivity(bool returnIP) const {
  return getImpl<detail::Blind_Impl>()->getSlatConductivity(returnIP);
}

bool Blind::isSlatConductivityDefaulted() const {
  return getImpl<detail::Blind_Impl>()->isSlatConductivityDefaulted();
}

double Blind::slatBeamSolarTransmittance() const {
  return getImpl<detail::Blind_Impl>()->slatBeamSolarTransmittance();
}

Quantity Blind::getSlatBeamSolarTransmittance(bool returnIP) const {
  return getImpl<detail::Blind_Impl>()->getSlatBeamSolarTransmittance(returnIP);
}

bool Blind::isSlatBeamSolarTransmittanceDefaulted() const {
  return getImpl<detail::Blind_Impl>()->isSlatBeamSolarTransmittanceDefaulted();
}

double Blind::frontSideSlatBeamSolarReflectance() const {
  return getImpl<detail::Blind_Impl>()->frontSideSlatBeamSolarReflectance();
}

Quantity Blind::getFrontSideSlatBeamSolarReflectance(bool returnIP) const {
  return getImpl<detail::Blind_Impl>()->getFrontSideSlatBeamSolarReflectance(returnIP);
}

double Blind::backSideSlatBeamSolarReflectance() const {
  return getImpl<detail::Blind_Impl>()->backSideSlatBeamSolarReflectance();
}

Quantity Blind::getBackSideSlatBeamSolarReflectance(bool returnIP) const {
  return getImpl<detail::Blind_Impl>()->getBackSideSlatBeamSolarReflectance(returnIP);
}

double Blind::slatDiffuseSolarTransmittance() const {
  return getImpl<detail::Blind_Impl>()->slatDiffuseSolarTransmittance();
}

Quantity Blind::getSlatDiffuseSolarTransmittance(bool returnIP) const {
  return getImpl<detail::Blind_Impl>()->getSlatDiffuseSolarTransmittance(returnIP);
}

bool Blind::isSlatDiffuseSolarTransmittanceDefaulted() const {
  return getImpl<detail::Blind_Impl>()->isSlatDiffuseSolarTransmittanceDefaulted();
}

double Blind::frontSideSlatDiffuseSolarReflectance() const {
  return getImpl<detail::Blind_Impl>()->frontSideSlatDiffuseSolarReflectance();
}

Quantity Blind::getFrontSideSlatDiffuseSolarReflectance(bool returnIP) const {
  return getImpl<detail::Blind_Impl>()->getFrontSideSlatDiffuseSolarReflectance(returnIP);
}

double Blind::backSideSlatDiffuseSolarReflectance() const {
  return getImpl<detail::Blind_Impl>()->backSideSlatDiffuseSolarReflectance();
}

Quantity Blind::getBackSideSlatDiffuseSolarReflectance(bool returnIP) const {
  return getImpl<detail::Blind_Impl>()->getBackSideSlatDiffuseSolarReflectance(returnIP);
}

double Blind::slatBeamVisibleTransmittance() const {
  return getImpl<detail::Blind_Impl>()->slatBeamVisibleTransmittance();
}

Quantity Blind::getSlatBeamVisibleTransmittance(bool returnIP) const {
  return getImpl<detail::Blind_Impl>()->getSlatBeamVisibleTransmittance(returnIP);
}

boost::optional<double> Blind::frontSideSlatBeamVisibleReflectance() const {
  return getImpl<detail::Blind_Impl>()->frontSideSlatBeamVisibleReflectance();
}

OSOptionalQuantity Blind::getFrontSideSlatBeamVisibleReflectance(bool returnIP) const {
  return getImpl<detail::Blind_Impl>()->getFrontSideSlatBeamVisibleReflectance(returnIP);
}

boost::optional<double> Blind::backSideSlatBeamVisibleReflectance() const {
  return getImpl<detail::Blind_Impl>()->backSideSlatBeamVisibleReflectance();
}

OSOptionalQuantity Blind::getBackSideSlatBeamVisibleReflectance(bool returnIP) const {
  return getImpl<detail::Blind_Impl>()->getBackSideSlatBeamVisibleReflectance(returnIP);
}

double Blind::slatDiffuseVisibleTransmittance() const {
  return getImpl<detail::Blind_Impl>()->slatDiffuseVisibleTransmittance();
}

Quantity Blind::getSlatDiffuseVisibleTransmittance(bool returnIP) const {
  return getImpl<detail::Blind_Impl>()->getSlatDiffuseVisibleTransmittance(returnIP);
}

bool Blind::isSlatDiffuseVisibleTransmittanceDefaulted() const {
  return getImpl<detail::Blind_Impl>()->isSlatDiffuseVisibleTransmittanceDefaulted();
}

boost::optional<double> Blind::frontSideSlatDiffuseVisibleReflectance() const {
  return getImpl<detail::Blind_Impl>()->frontSideSlatDiffuseVisibleReflectance();
}

OSOptionalQuantity Blind::getFrontSideSlatDiffuseVisibleReflectance(bool returnIP) const {
  return getImpl<detail::Blind_Impl>()->getFrontSideSlatDiffuseVisibleReflectance(returnIP);
}

boost::optional<double> Blind::backSideSlatDiffuseVisibleReflectance() const {
  return getImpl<detail::Blind_Impl>()->backSideSlatDiffuseVisibleReflectance();
}

OSOptionalQuantity Blind::getBackSideSlatDiffuseVisibleReflectance(bool returnIP) const {
  return getImpl<detail::Blind_Impl>()->getBackSideSlatDiffuseVisibleReflectance(returnIP);
}

double Blind::slatInfraredHemisphericalTransmittance() const {
  return getImpl<detail::Blind_Impl>()->slatInfraredHemisphericalTransmittance();
}

Quantity Blind::getSlatInfraredHemisphericalTransmittance(bool returnIP) const {
  return getImpl<detail::Blind_Impl>()->getSlatInfraredHemisphericalTransmittance(returnIP);
}

bool Blind::isSlatInfraredHemisphericalTransmittanceDefaulted() const {
  return getImpl<detail::Blind_Impl>()->isSlatInfraredHemisphericalTransmittanceDefaulted();
}

double Blind::frontSideSlatInfraredHemisphericalEmissivity() const {
  return getImpl<detail::Blind_Impl>()->frontSideSlatInfraredHemisphericalEmissivity();
}

Quantity Blind::getFrontSideSlatInfraredHemisphericalEmissivity(bool returnIP) const {
  return getImpl<detail::Blind_Impl>()->getFrontSideSlatInfraredHemisphericalEmissivity(returnIP);
}

bool Blind::isFrontSideSlatInfraredHemisphericalEmissivityDefaulted() const {
  return getImpl<detail::Blind_Impl>()->isFrontSideSlatInfraredHemisphericalEmissivityDefaulted();
}

double Blind::backSideSlatInfraredHemisphericalEmissivity() const {
  return getImpl<detail::Blind_Impl>()->backSideSlatInfraredHemisphericalEmissivity();
}

Quantity Blind::getBackSideSlatInfraredHemisphericalEmissivity(bool returnIP) const {
  return getImpl<detail::Blind_Impl>()->getBackSideSlatInfraredHemisphericalEmissivity(returnIP);
}

bool Blind::isBackSideSlatInfraredHemisphericalEmissivityDefaulted() const {
  return getImpl<detail::Blind_Impl>()->isBackSideSlatInfraredHemisphericalEmissivityDefaulted();
}

double Blind::blindtoGlassDistance() const {
  return getImpl<detail::Blind_Impl>()->blindtoGlassDistance();
}

Quantity Blind::getBlindtoGlassDistance(bool returnIP) const {
  return getImpl<detail::Blind_Impl>()->getBlindtoGlassDistance(returnIP);
}

bool Blind::isBlindtoGlassDistanceDefaulted() const {
  return getImpl<detail::Blind_Impl>()->isBlindtoGlassDistanceDefaulted();
}

double Blind::blindTopOpeningMultiplier() const {
  return getImpl<detail::Blind_Impl>()->blindTopOpeningMultiplier();
}

Quantity Blind::getBlindTopOpeningMultiplier(bool returnIP) const {
  return getImpl<detail::Blind_Impl>()->getBlindTopOpeningMultiplier(returnIP);
}

bool Blind::isBlindTopOpeningMultiplierDefaulted() const {
  return getImpl<detail::Blind_Impl>()->isBlindTopOpeningMultiplierDefaulted();
}

double Blind::blindBottomOpeningMultiplier() const {
  return getImpl<detail::Blind_Impl>()->blindBottomOpeningMultiplier();
}

Quantity Blind::getBlindBottomOpeningMultiplier(bool returnIP) const {
  return getImpl<detail::Blind_Impl>()->getBlindBottomOpeningMultiplier(returnIP);
}

bool Blind::isBlindBottomOpeningMultiplierDefaulted() const {
  return getImpl<detail::Blind_Impl>()->isBlindBottomOpeningMultiplierDefaulted();
}

double Blind::blindLeftSideOpeningMultiplier() const {
  return getImpl<detail::Blind_Impl>()->blindLeftSideOpeningMultiplier();
}

Quantity Blind::getBlindLeftSideOpeningMultiplier(bool returnIP) const {
  return getImpl<detail::Blind_Impl>()->getBlindLeftSideOpeningMultiplier(returnIP);
}

bool Blind::isBlindLeftSideOpeningMultiplierDefaulted() const {
  return getImpl<detail::Blind_Impl>()->isBlindLeftSideOpeningMultiplierDefaulted();
}

double Blind::blindRightSideOpeningMultiplier() const {
  return getImpl<detail::Blind_Impl>()->blindRightSideOpeningMultiplier();
}

Quantity Blind::getBlindRightSideOpeningMultiplier(bool returnIP) const {
  return getImpl<detail::Blind_Impl>()->getBlindRightSideOpeningMultiplier(returnIP);
}

bool Blind::isBlindRightSideOpeningMultiplierDefaulted() const {
  return getImpl<detail::Blind_Impl>()->isBlindRightSideOpeningMultiplierDefaulted();
}

double Blind::minimumSlatAngle() const {
  return getImpl<detail::Blind_Impl>()->minimumSlatAngle();
}

Quantity Blind::getMinimumSlatAngle(bool returnIP) const {
  return getImpl<detail::Blind_Impl>()->getMinimumSlatAngle(returnIP);
}

bool Blind::isMinimumSlatAngleDefaulted() const {
  return getImpl<detail::Blind_Impl>()->isMinimumSlatAngleDefaulted();
}

double Blind::maximumSlatAngle() const {
  return getImpl<detail::Blind_Impl>()->maximumSlatAngle();
}

Quantity Blind::getMaximumSlatAngle(bool returnIP) const {
  return getImpl<detail::Blind_Impl>()->getMaximumSlatAngle(returnIP);
}

bool Blind::isMaximumSlatAngleDefaulted() const {
  return getImpl<detail::Blind_Impl>()->isMaximumSlatAngleDefaulted();
}

bool Blind::setSlatOrientation(std::string slatOrientation) {
  return getImpl<detail::Blind_Impl>()->setSlatOrientation(slatOrientation);
}

void Blind::resetSlatOrientation() {
  getImpl<detail::Blind_Impl>()->resetSlatOrientation();
}

bool Blind::setSlatWidth(double slatWidth) {
  return getImpl<detail::Blind_Impl>()->setSlatWidth(slatWidth);
}

bool Blind::setSlatWidth(const Quantity& slatWidth) {
  return getImpl<detail::Blind_Impl>()->setSlatWidth(slatWidth);
}

bool Blind::setSlatSeparation(double slatSeparation) {
  return getImpl<detail::Blind_Impl>()->setSlatSeparation(slatSeparation);
}

bool Blind::setSlatSeparation(const Quantity& slatSeparation) {
  return getImpl<detail::Blind_Impl>()->setSlatSeparation(slatSeparation);
}

bool Blind::setSlatThickness(double slatThickness) {
  return getImpl<detail::Blind_Impl>()->setSlatThickness(slatThickness);
}

bool Blind::setSlatThickness(const Quantity& slatThickness) {
  return getImpl<detail::Blind_Impl>()->setSlatThickness(slatThickness);
}

void Blind::resetSlatThickness() {
  getImpl<detail::Blind_Impl>()->resetSlatThickness();
}

bool Blind::setSlatAngle(double slatAngle) {
  return getImpl<detail::Blind_Impl>()->setSlatAngle(slatAngle);
}

bool Blind::setSlatAngle(const Quantity& slatAngle) {
  return getImpl<detail::Blind_Impl>()->setSlatAngle(slatAngle);
}

void Blind::resetSlatAngle() {
  getImpl<detail::Blind_Impl>()->resetSlatAngle();
}

bool Blind::setSlatConductivity(double slatConductivity) {
  return getImpl<detail::Blind_Impl>()->setSlatConductivity(slatConductivity);
}

bool Blind::setSlatConductivity(const Quantity& slatConductivity) {
  return getImpl<detail::Blind_Impl>()->setSlatConductivity(slatConductivity);
}

void Blind::resetSlatConductivity() {
  getImpl<detail::Blind_Impl>()->resetSlatConductivity();
}

bool Blind::setSlatBeamSolarTransmittance(double slatBeamSolarTransmittance) {
  return getImpl<detail::Blind_Impl>()->setSlatBeamSolarTransmittance(slatBeamSolarTransmittance);
}

bool Blind::setSlatBeamSolarTransmittance(const Quantity& slatBeamSolarTransmittance) {
  return getImpl<detail::Blind_Impl>()->setSlatBeamSolarTransmittance(slatBeamSolarTransmittance);
}

void Blind::resetSlatBeamSolarTransmittance() {
  getImpl<detail::Blind_Impl>()->resetSlatBeamSolarTransmittance();
}

bool Blind::setFrontSideSlatBeamSolarReflectance(double frontSideSlatBeamSolarReflectance) {
  return getImpl<detail::Blind_Impl>()->setFrontSideSlatBeamSolarReflectance(frontSideSlatBeamSolarReflectance);
}

bool Blind::setFrontSideSlatBeamSolarReflectance(const Quantity& frontSideSlatBeamSolarReflectance) {
  return getImpl<detail::Blind_Impl>()->setFrontSideSlatBeamSolarReflectance(frontSideSlatBeamSolarReflectance);
}

bool Blind::setBackSideSlatBeamSolarReflectance(double backSideSlatBeamSolarReflectance) {
  return getImpl<detail::Blind_Impl>()->setBackSideSlatBeamSolarReflectance(backSideSlatBeamSolarReflectance);
}

bool Blind::setBackSideSlatBeamSolarReflectance(const Quantity& backSideSlatBeamSolarReflectance) {
  return getImpl<detail::Blind_Impl>()->setBackSideSlatBeamSolarReflectance(backSideSlatBeamSolarReflectance);
}

bool Blind::setSlatDiffuseSolarTransmittance(double slatDiffuseSolarTransmittance) {
  return getImpl<detail::Blind_Impl>()->setSlatDiffuseSolarTransmittance(slatDiffuseSolarTransmittance);
}

bool Blind::setSlatDiffuseSolarTransmittance(const Quantity& slatDiffuseSolarTransmittance) {
  return getImpl<detail::Blind_Impl>()->setSlatDiffuseSolarTransmittance(slatDiffuseSolarTransmittance);
}

void Blind::resetSlatDiffuseSolarTransmittance() {
  getImpl<detail::Blind_Impl>()->resetSlatDiffuseSolarTransmittance();
}

bool Blind::setFrontSideSlatDiffuseSolarReflectance(double frontSideSlatDiffuseSolarReflectance) {
  return getImpl<detail::Blind_Impl>()->setFrontSideSlatDiffuseSolarReflectance(frontSideSlatDiffuseSolarReflectance);
}

bool Blind::setFrontSideSlatDiffuseSolarReflectance(const Quantity& frontSideSlatDiffuseSolarReflectance) {
  return getImpl<detail::Blind_Impl>()->setFrontSideSlatDiffuseSolarReflectance(frontSideSlatDiffuseSolarReflectance);
}

bool Blind::setBackSideSlatDiffuseSolarReflectance(double backSideSlatDiffuseSolarReflectance) {
  return getImpl<detail::Blind_Impl>()->setBackSideSlatDiffuseSolarReflectance(backSideSlatDiffuseSolarReflectance);
}

bool Blind::setBackSideSlatDiffuseSolarReflectance(const Quantity& backSideSlatDiffuseSolarReflectance) {
  return getImpl<detail::Blind_Impl>()->setBackSideSlatDiffuseSolarReflectance(backSideSlatDiffuseSolarReflectance);
}

bool Blind::setSlatBeamVisibleTransmittance(double slatBeamVisibleTransmittance) {
  return getImpl<detail::Blind_Impl>()->setSlatBeamVisibleTransmittance(slatBeamVisibleTransmittance);
}

bool Blind::setSlatBeamVisibleTransmittance(const Quantity& slatBeamVisibleTransmittance) {
  return getImpl<detail::Blind_Impl>()->setSlatBeamVisibleTransmittance(slatBeamVisibleTransmittance);
}

bool Blind::setFrontSideSlatBeamVisibleReflectance(double frontSideSlatBeamVisibleReflectance) {
  return getImpl<detail::Blind_Impl>()->setFrontSideSlatBeamVisibleReflectance(frontSideSlatBeamVisibleReflectance);
}

bool Blind::setFrontSideSlatBeamVisibleReflectance(const Quantity& frontSideSlatBeamVisibleReflectance) {
  return getImpl<detail::Blind_Impl>()->setFrontSideSlatBeamVisibleReflectance(frontSideSlatBeamVisibleReflectance);
}

void Blind::resetFrontSideSlatBeamVisibleReflectance() {
  getImpl<detail::Blind_Impl>()->resetFrontSideSlatBeamVisibleReflectance();
}

bool Blind::setBackSideSlatBeamVisibleReflectance(double backSideSlatBeamVisibleReflectance) {
  return getImpl<detail::Blind_Impl>()->setBackSideSlatBeamVisibleReflectance(backSideSlatBeamVisibleReflectance);
}

bool Blind::setBackSideSlatBeamVisibleReflectance(const Quantity& backSideSlatBeamVisibleReflectance) {
  return getImpl<detail::Blind_Impl>()->setBackSideSlatBeamVisibleReflectance(backSideSlatBeamVisibleReflectance);
}

void Blind::resetBackSideSlatBeamVisibleReflectance() {
  getImpl<detail::Blind_Impl>()->resetBackSideSlatBeamVisibleReflectance();
}

bool Blind::setSlatDiffuseVisibleTransmittance(double slatDiffuseVisibleTransmittance) {
  return getImpl<detail::Blind_Impl>()->setSlatDiffuseVisibleTransmittance(slatDiffuseVisibleTransmittance);
}

bool Blind::setSlatDiffuseVisibleTransmittance(const Quantity& slatDiffuseVisibleTransmittance) {
  return getImpl<detail::Blind_Impl>()->setSlatDiffuseVisibleTransmittance(slatDiffuseVisibleTransmittance);
}

void Blind::resetSlatDiffuseVisibleTransmittance() {
  getImpl<detail::Blind_Impl>()->resetSlatDiffuseVisibleTransmittance();
}

bool Blind::setFrontSideSlatDiffuseVisibleReflectance(double frontSideSlatDiffuseVisibleReflectance) {
  return getImpl<detail::Blind_Impl>()->setFrontSideSlatDiffuseVisibleReflectance(frontSideSlatDiffuseVisibleReflectance);
}

bool Blind::setFrontSideSlatDiffuseVisibleReflectance(const Quantity& frontSideSlatDiffuseVisibleReflectance) {
  return getImpl<detail::Blind_Impl>()->setFrontSideSlatDiffuseVisibleReflectance(frontSideSlatDiffuseVisibleReflectance);
}

void Blind::resetFrontSideSlatDiffuseVisibleReflectance() {
  getImpl<detail::Blind_Impl>()->resetFrontSideSlatDiffuseVisibleReflectance();
}

bool Blind::setBackSideSlatDiffuseVisibleReflectance(double backSideSlatDiffuseVisibleReflectance) {
  return getImpl<detail::Blind_Impl>()->setBackSideSlatDiffuseVisibleReflectance(backSideSlatDiffuseVisibleReflectance);
}

bool Blind::setBackSideSlatDiffuseVisibleReflectance(const Quantity& backSideSlatDiffuseVisibleReflectance) {
  return getImpl<detail::Blind_Impl>()->setBackSideSlatDiffuseVisibleReflectance(backSideSlatDiffuseVisibleReflectance);
}

void Blind::resetBackSideSlatDiffuseVisibleReflectance() {
  getImpl<detail::Blind_Impl>()->resetBackSideSlatDiffuseVisibleReflectance();
}

bool Blind::setSlatInfraredHemisphericalTransmittance(double slatInfraredHemisphericalTransmittance) {
  return getImpl<detail::Blind_Impl>()->setSlatInfraredHemisphericalTransmittance(slatInfraredHemisphericalTransmittance);
}

bool Blind::setSlatInfraredHemisphericalTransmittance(const Quantity& slatInfraredHemisphericalTransmittance) {
  return getImpl<detail::Blind_Impl>()->setSlatInfraredHemisphericalTransmittance(slatInfraredHemisphericalTransmittance);
}

void Blind::resetSlatInfraredHemisphericalTransmittance() {
  getImpl<detail::Blind_Impl>()->resetSlatInfraredHemisphericalTransmittance();
}

bool Blind::setFrontSideSlatInfraredHemisphericalEmissivity(double frontSideSlatInfraredHemisphericalEmissivity) {
  return getImpl<detail::Blind_Impl>()->setFrontSideSlatInfraredHemisphericalEmissivity(frontSideSlatInfraredHemisphericalEmissivity);
}

bool Blind::setFrontSideSlatInfraredHemisphericalEmissivity(const Quantity& frontSideSlatInfraredHemisphericalEmissivity) {
  return getImpl<detail::Blind_Impl>()->setFrontSideSlatInfraredHemisphericalEmissivity(frontSideSlatInfraredHemisphericalEmissivity);
}

void Blind::resetFrontSideSlatInfraredHemisphericalEmissivity() {
  getImpl<detail::Blind_Impl>()->resetFrontSideSlatInfraredHemisphericalEmissivity();
}

bool Blind::setBackSideSlatInfraredHemisphericalEmissivity(double backSideSlatInfraredHemisphericalEmissivity) {
  return getImpl<detail::Blind_Impl>()->setBackSideSlatInfraredHemisphericalEmissivity(backSideSlatInfraredHemisphericalEmissivity);
}

bool Blind::setBackSideSlatInfraredHemisphericalEmissivity(const Quantity& backSideSlatInfraredHemisphericalEmissivity) {
  return getImpl<detail::Blind_Impl>()->setBackSideSlatInfraredHemisphericalEmissivity(backSideSlatInfraredHemisphericalEmissivity);
}

void Blind::resetBackSideSlatInfraredHemisphericalEmissivity() {
  getImpl<detail::Blind_Impl>()->resetBackSideSlatInfraredHemisphericalEmissivity();
}

bool Blind::setBlindtoGlassDistance(double blindtoGlassDistance) {
  return getImpl<detail::Blind_Impl>()->setBlindtoGlassDistance(blindtoGlassDistance);
}

bool Blind::setBlindtoGlassDistance(const Quantity& blindtoGlassDistance) {
  return getImpl<detail::Blind_Impl>()->setBlindtoGlassDistance(blindtoGlassDistance);
}

void Blind::resetBlindtoGlassDistance() {
  getImpl<detail::Blind_Impl>()->resetBlindtoGlassDistance();
}

bool Blind::setBlindTopOpeningMultiplier(double blindTopOpeningMultiplier) {
  return getImpl<detail::Blind_Impl>()->setBlindTopOpeningMultiplier(blindTopOpeningMultiplier);
}

bool Blind::setBlindTopOpeningMultiplier(const Quantity& blindTopOpeningMultiplier) {
  return getImpl<detail::Blind_Impl>()->setBlindTopOpeningMultiplier(blindTopOpeningMultiplier);
}

void Blind::resetBlindTopOpeningMultiplier() {
  getImpl<detail::Blind_Impl>()->resetBlindTopOpeningMultiplier();
}

bool Blind::setBlindBottomOpeningMultiplier(double blindBottomOpeningMultiplier) {
  return getImpl<detail::Blind_Impl>()->setBlindBottomOpeningMultiplier(blindBottomOpeningMultiplier);
}

bool Blind::setBlindBottomOpeningMultiplier(const Quantity& blindBottomOpeningMultiplier) {
  return getImpl<detail::Blind_Impl>()->setBlindBottomOpeningMultiplier(blindBottomOpeningMultiplier);
}

void Blind::resetBlindBottomOpeningMultiplier() {
  getImpl<detail::Blind_Impl>()->resetBlindBottomOpeningMultiplier();
}

bool Blind::setBlindLeftSideOpeningMultiplier(double blindLeftSideOpeningMultiplier) {
  return getImpl<detail::Blind_Impl>()->setBlindLeftSideOpeningMultiplier(blindLeftSideOpeningMultiplier);
}

bool Blind::setBlindLeftSideOpeningMultiplier(const Quantity& blindLeftSideOpeningMultiplier) {
  return getImpl<detail::Blind_Impl>()->setBlindLeftSideOpeningMultiplier(blindLeftSideOpeningMultiplier);
}

void Blind::resetBlindLeftSideOpeningMultiplier() {
  getImpl<detail::Blind_Impl>()->resetBlindLeftSideOpeningMultiplier();
}

bool Blind::setBlindRightSideOpeningMultiplier(double blindRightSideOpeningMultiplier) {
  return getImpl<detail::Blind_Impl>()->setBlindRightSideOpeningMultiplier(blindRightSideOpeningMultiplier);
}

bool Blind::setBlindRightSideOpeningMultiplier(const Quantity& blindRightSideOpeningMultiplier) {
  return getImpl<detail::Blind_Impl>()->setBlindRightSideOpeningMultiplier(blindRightSideOpeningMultiplier);
}

void Blind::resetBlindRightSideOpeningMultiplier() {
  getImpl<detail::Blind_Impl>()->resetBlindRightSideOpeningMultiplier();
}

bool Blind::setMinimumSlatAngle(double minimumSlatAngle) {
  return getImpl<detail::Blind_Impl>()->setMinimumSlatAngle(minimumSlatAngle);
}

bool Blind::setMinimumSlatAngle(const Quantity& minimumSlatAngle) {
  return getImpl<detail::Blind_Impl>()->setMinimumSlatAngle(minimumSlatAngle);
}

void Blind::resetMinimumSlatAngle() {
  getImpl<detail::Blind_Impl>()->resetMinimumSlatAngle();
}

bool Blind::setMaximumSlatAngle(double maximumSlatAngle) {
  return getImpl<detail::Blind_Impl>()->setMaximumSlatAngle(maximumSlatAngle);
}

bool Blind::setMaximumSlatAngle(const Quantity& maximumSlatAngle) {
  return getImpl<detail::Blind_Impl>()->setMaximumSlatAngle(maximumSlatAngle);
}

void Blind::resetMaximumSlatAngle() {
  getImpl<detail::Blind_Impl>()->resetMaximumSlatAngle();
}

/// @cond
Blind::Blind(std::shared_ptr<detail::Blind_Impl> impl)
  : ShadingMaterial(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

