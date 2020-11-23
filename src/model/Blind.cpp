/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

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

    Blind_Impl::Blind_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle) : ShadingMaterial_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == Blind::iddObjectType());
    }

    Blind_Impl::Blind_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ShadingMaterial_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == Blind::iddObjectType());
    }

    Blind_Impl::Blind_Impl(const Blind_Impl& other, Model_Impl* model, bool keepHandle) : ShadingMaterial_Impl(other, model, keepHandle) {}

    double Blind_Impl::thickness() const {
      OptionalDouble od = getDouble(OS_WindowMaterial_BlindFields::SlatThickness, true);
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

    const std::vector<std::string>& Blind_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType Blind_Impl::iddObjectType() const {
      return Blind::iddObjectType();
    }

    bool Blind_Impl::setThickness(double value) {
      return setDouble(OS_WindowMaterial_BlindFields::SlatThickness, value);
    }

    std::string Blind_Impl::slatOrientation() const {
      boost::optional<std::string> value = getString(OS_WindowMaterial_BlindFields::SlatOrientation, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool Blind_Impl::isSlatOrientationDefaulted() const {
      return isEmpty(OS_WindowMaterial_BlindFields::SlatOrientation);
    }

    double Blind_Impl::slatWidth() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_BlindFields::SlatWidth, true);
      OS_ASSERT(value);
      return value.get();
    }

    double Blind_Impl::slatSeparation() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_BlindFields::SlatSeparation, true);
      OS_ASSERT(value);
      return value.get();
    }

    double Blind_Impl::slatThickness() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_BlindFields::SlatThickness, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool Blind_Impl::isSlatThicknessDefaulted() const {
      return isEmpty(OS_WindowMaterial_BlindFields::SlatThickness);
    }

    double Blind_Impl::slatAngle() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_BlindFields::SlatAngle, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool Blind_Impl::isSlatAngleDefaulted() const {
      return isEmpty(OS_WindowMaterial_BlindFields::SlatAngle);
    }

    double Blind_Impl::slatConductivity() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_BlindFields::SlatConductivity, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool Blind_Impl::isSlatConductivityDefaulted() const {
      return isEmpty(OS_WindowMaterial_BlindFields::SlatConductivity);
    }

    double Blind_Impl::slatBeamSolarTransmittance() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_BlindFields::SlatBeamSolarTransmittance, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool Blind_Impl::isSlatBeamSolarTransmittanceDefaulted() const {
      return isEmpty(OS_WindowMaterial_BlindFields::SlatBeamSolarTransmittance);
    }

    double Blind_Impl::frontSideSlatBeamSolarReflectance() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_BlindFields::FrontSideSlatBeamSolarReflectance, true);
      OS_ASSERT(value);
      return value.get();
    }

    double Blind_Impl::backSideSlatBeamSolarReflectance() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_BlindFields::BackSideSlatBeamSolarReflectance, true);
      OS_ASSERT(value);
      return value.get();
    }

    double Blind_Impl::slatDiffuseSolarTransmittance() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_BlindFields::SlatDiffuseSolarTransmittance, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool Blind_Impl::isSlatDiffuseSolarTransmittanceDefaulted() const {
      return isEmpty(OS_WindowMaterial_BlindFields::SlatDiffuseSolarTransmittance);
    }

    double Blind_Impl::frontSideSlatDiffuseSolarReflectance() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_BlindFields::FrontSideSlatDiffuseSolarReflectance, true);
      OS_ASSERT(value);
      return value.get();
    }

    double Blind_Impl::backSideSlatDiffuseSolarReflectance() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_BlindFields::BackSideSlatDiffuseSolarReflectance, true);
      OS_ASSERT(value);
      return value.get();
    }

    double Blind_Impl::slatBeamVisibleTransmittance() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_BlindFields::SlatBeamVisibleTransmittance, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> Blind_Impl::frontSideSlatBeamVisibleReflectance() const {
      return getDouble(OS_WindowMaterial_BlindFields::FrontSideSlatBeamVisibleReflectance, true);
    }

    boost::optional<double> Blind_Impl::backSideSlatBeamVisibleReflectance() const {
      return getDouble(OS_WindowMaterial_BlindFields::BackSideSlatBeamVisibleReflectance, true);
    }

    double Blind_Impl::slatDiffuseVisibleTransmittance() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_BlindFields::SlatDiffuseVisibleTransmittance, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool Blind_Impl::isSlatDiffuseVisibleTransmittanceDefaulted() const {
      return isEmpty(OS_WindowMaterial_BlindFields::SlatDiffuseVisibleTransmittance);
    }

    boost::optional<double> Blind_Impl::frontSideSlatDiffuseVisibleReflectance() const {
      return getDouble(OS_WindowMaterial_BlindFields::FrontSideSlatDiffuseVisibleReflectance, true);
    }

    boost::optional<double> Blind_Impl::backSideSlatDiffuseVisibleReflectance() const {
      return getDouble(OS_WindowMaterial_BlindFields::BackSideSlatDiffuseVisibleReflectance, true);
    }

    double Blind_Impl::slatInfraredHemisphericalTransmittance() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_BlindFields::SlatInfraredHemisphericalTransmittance, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool Blind_Impl::isSlatInfraredHemisphericalTransmittanceDefaulted() const {
      return isEmpty(OS_WindowMaterial_BlindFields::SlatInfraredHemisphericalTransmittance);
    }

    double Blind_Impl::frontSideSlatInfraredHemisphericalEmissivity() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_BlindFields::FrontSideSlatInfraredHemisphericalEmissivity, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool Blind_Impl::isFrontSideSlatInfraredHemisphericalEmissivityDefaulted() const {
      return isEmpty(OS_WindowMaterial_BlindFields::FrontSideSlatInfraredHemisphericalEmissivity);
    }

    double Blind_Impl::backSideSlatInfraredHemisphericalEmissivity() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_BlindFields::BackSideSlatInfraredHemisphericalEmissivity, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool Blind_Impl::isBackSideSlatInfraredHemisphericalEmissivityDefaulted() const {
      return isEmpty(OS_WindowMaterial_BlindFields::BackSideSlatInfraredHemisphericalEmissivity);
    }

    double Blind_Impl::blindtoGlassDistance() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_BlindFields::BlindtoGlassDistance, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool Blind_Impl::isBlindtoGlassDistanceDefaulted() const {
      return isEmpty(OS_WindowMaterial_BlindFields::BlindtoGlassDistance);
    }

    double Blind_Impl::blindTopOpeningMultiplier() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_BlindFields::BlindTopOpeningMultiplier, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool Blind_Impl::isBlindTopOpeningMultiplierDefaulted() const {
      return isEmpty(OS_WindowMaterial_BlindFields::BlindTopOpeningMultiplier);
    }

    double Blind_Impl::blindBottomOpeningMultiplier() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_BlindFields::BlindBottomOpeningMultiplier, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool Blind_Impl::isBlindBottomOpeningMultiplierDefaulted() const {
      return isEmpty(OS_WindowMaterial_BlindFields::BlindBottomOpeningMultiplier);
    }

    double Blind_Impl::blindLeftSideOpeningMultiplier() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_BlindFields::BlindLeftSideOpeningMultiplier, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool Blind_Impl::isBlindLeftSideOpeningMultiplierDefaulted() const {
      return isEmpty(OS_WindowMaterial_BlindFields::BlindLeftSideOpeningMultiplier);
    }

    double Blind_Impl::blindRightSideOpeningMultiplier() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_BlindFields::BlindRightSideOpeningMultiplier, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool Blind_Impl::isBlindRightSideOpeningMultiplierDefaulted() const {
      return isEmpty(OS_WindowMaterial_BlindFields::BlindRightSideOpeningMultiplier);
    }

    double Blind_Impl::minimumSlatAngle() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_BlindFields::MinimumSlatAngle, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool Blind_Impl::isMinimumSlatAngleDefaulted() const {
      return isEmpty(OS_WindowMaterial_BlindFields::MinimumSlatAngle);
    }

    double Blind_Impl::maximumSlatAngle() const {
      boost::optional<double> value = getDouble(OS_WindowMaterial_BlindFields::MaximumSlatAngle, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool Blind_Impl::isMaximumSlatAngleDefaulted() const {
      return isEmpty(OS_WindowMaterial_BlindFields::MaximumSlatAngle);
    }

    bool Blind_Impl::setSlatOrientation(const std::string& slatOrientation) {
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

    bool Blind_Impl::setSlatSeparation(double slatSeparation) {
      bool result = setDouble(OS_WindowMaterial_BlindFields::SlatSeparation, slatSeparation);
      return result;
    }

    bool Blind_Impl::setSlatThickness(double slatThickness) {
      bool result = setDouble(OS_WindowMaterial_BlindFields::SlatThickness, slatThickness);
      return result;
    }

    void Blind_Impl::resetSlatThickness() {
      bool result = setString(OS_WindowMaterial_BlindFields::SlatThickness, "");
      OS_ASSERT(result);
    }

    bool Blind_Impl::setSlatAngle(double slatAngle) {
      bool result = setDouble(OS_WindowMaterial_BlindFields::SlatAngle, slatAngle);
      return result;
    }

    void Blind_Impl::resetSlatAngle() {
      bool result = setString(OS_WindowMaterial_BlindFields::SlatAngle, "");
      OS_ASSERT(result);
    }

    bool Blind_Impl::setSlatConductivity(double slatConductivity) {
      bool result = setDouble(OS_WindowMaterial_BlindFields::SlatConductivity, slatConductivity);
      return result;
    }

    void Blind_Impl::resetSlatConductivity() {
      bool result = setString(OS_WindowMaterial_BlindFields::SlatConductivity, "");
      OS_ASSERT(result);
    }

    bool Blind_Impl::setSlatBeamSolarTransmittance(double slatBeamSolarTransmittance) {
      bool result = setDouble(OS_WindowMaterial_BlindFields::SlatBeamSolarTransmittance, slatBeamSolarTransmittance);
      return result;
    }

    void Blind_Impl::resetSlatBeamSolarTransmittance() {
      bool result = setString(OS_WindowMaterial_BlindFields::SlatBeamSolarTransmittance, "");
      OS_ASSERT(result);
    }

    bool Blind_Impl::setFrontSideSlatBeamSolarReflectance(double frontSideSlatBeamSolarReflectance) {
      bool result = setDouble(OS_WindowMaterial_BlindFields::FrontSideSlatBeamSolarReflectance, frontSideSlatBeamSolarReflectance);
      return result;
    }

    bool Blind_Impl::setBackSideSlatBeamSolarReflectance(double backSideSlatBeamSolarReflectance) {
      bool result = setDouble(OS_WindowMaterial_BlindFields::BackSideSlatBeamSolarReflectance, backSideSlatBeamSolarReflectance);
      return result;
    }

    bool Blind_Impl::setSlatDiffuseSolarTransmittance(double slatDiffuseSolarTransmittance) {
      bool result = setDouble(OS_WindowMaterial_BlindFields::SlatDiffuseSolarTransmittance, slatDiffuseSolarTransmittance);
      return result;
    }

    void Blind_Impl::resetSlatDiffuseSolarTransmittance() {
      bool result = setString(OS_WindowMaterial_BlindFields::SlatDiffuseSolarTransmittance, "");
      OS_ASSERT(result);
    }

    bool Blind_Impl::setFrontSideSlatDiffuseSolarReflectance(double frontSideSlatDiffuseSolarReflectance) {
      bool result = setDouble(OS_WindowMaterial_BlindFields::FrontSideSlatDiffuseSolarReflectance, frontSideSlatDiffuseSolarReflectance);
      return result;
    }

    bool Blind_Impl::setBackSideSlatDiffuseSolarReflectance(double backSideSlatDiffuseSolarReflectance) {
      bool result = setDouble(OS_WindowMaterial_BlindFields::BackSideSlatDiffuseSolarReflectance, backSideSlatDiffuseSolarReflectance);
      return result;
    }

    bool Blind_Impl::setSlatBeamVisibleTransmittance(double slatBeamVisibleTransmittance) {
      bool result = setDouble(OS_WindowMaterial_BlindFields::SlatBeamVisibleTransmittance, slatBeamVisibleTransmittance);
      return result;
    }

    bool Blind_Impl::setFrontSideSlatBeamVisibleReflectance(boost::optional<double> frontSideSlatBeamVisibleReflectance) {
      bool result(false);
      if (frontSideSlatBeamVisibleReflectance) {
        result = setDouble(OS_WindowMaterial_BlindFields::FrontSideSlatBeamVisibleReflectance, frontSideSlatBeamVisibleReflectance.get());
      } else {
        resetFrontSideSlatBeamVisibleReflectance();
        result = true;
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
      } else {
        resetBackSideSlatBeamVisibleReflectance();
        result = true;
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

    void Blind_Impl::resetSlatDiffuseVisibleTransmittance() {
      bool result = setString(OS_WindowMaterial_BlindFields::SlatDiffuseVisibleTransmittance, "");
      OS_ASSERT(result);
    }

    bool Blind_Impl::setFrontSideSlatDiffuseVisibleReflectance(boost::optional<double> frontSideSlatDiffuseVisibleReflectance) {
      bool result(false);
      if (frontSideSlatDiffuseVisibleReflectance) {
        result = setDouble(OS_WindowMaterial_BlindFields::FrontSideSlatDiffuseVisibleReflectance, frontSideSlatDiffuseVisibleReflectance.get());
      } else {
        resetFrontSideSlatDiffuseVisibleReflectance();
        result = true;
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
      } else {
        resetBackSideSlatDiffuseVisibleReflectance();
        result = true;
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

    void Blind_Impl::resetSlatInfraredHemisphericalTransmittance() {
      bool result = setString(OS_WindowMaterial_BlindFields::SlatInfraredHemisphericalTransmittance, "");
      OS_ASSERT(result);
    }

    bool Blind_Impl::setFrontSideSlatInfraredHemisphericalEmissivity(double frontSideSlatInfraredHemisphericalEmissivity) {
      bool result =
        setDouble(OS_WindowMaterial_BlindFields::FrontSideSlatInfraredHemisphericalEmissivity, frontSideSlatInfraredHemisphericalEmissivity);
      return result;
    }

    void Blind_Impl::resetFrontSideSlatInfraredHemisphericalEmissivity() {
      bool result = setString(OS_WindowMaterial_BlindFields::FrontSideSlatInfraredHemisphericalEmissivity, "");
      OS_ASSERT(result);
    }

    bool Blind_Impl::setBackSideSlatInfraredHemisphericalEmissivity(double backSideSlatInfraredHemisphericalEmissivity) {
      bool result =
        setDouble(OS_WindowMaterial_BlindFields::BackSideSlatInfraredHemisphericalEmissivity, backSideSlatInfraredHemisphericalEmissivity);
      return result;
    }

    void Blind_Impl::resetBackSideSlatInfraredHemisphericalEmissivity() {
      bool result = setString(OS_WindowMaterial_BlindFields::BackSideSlatInfraredHemisphericalEmissivity, "");
      OS_ASSERT(result);
    }

    bool Blind_Impl::setBlindtoGlassDistance(double blindtoGlassDistance) {
      bool result = setDouble(OS_WindowMaterial_BlindFields::BlindtoGlassDistance, blindtoGlassDistance);
      return result;
    }

    void Blind_Impl::resetBlindtoGlassDistance() {
      bool result = setString(OS_WindowMaterial_BlindFields::BlindtoGlassDistance, "");
      OS_ASSERT(result);
    }

    bool Blind_Impl::setBlindTopOpeningMultiplier(double blindTopOpeningMultiplier) {
      bool result = setDouble(OS_WindowMaterial_BlindFields::BlindTopOpeningMultiplier, blindTopOpeningMultiplier);
      return result;
    }

    void Blind_Impl::resetBlindTopOpeningMultiplier() {
      bool result = setString(OS_WindowMaterial_BlindFields::BlindTopOpeningMultiplier, "");
      OS_ASSERT(result);
    }

    bool Blind_Impl::setBlindBottomOpeningMultiplier(double blindBottomOpeningMultiplier) {
      bool result = setDouble(OS_WindowMaterial_BlindFields::BlindBottomOpeningMultiplier, blindBottomOpeningMultiplier);
      return result;
    }

    void Blind_Impl::resetBlindBottomOpeningMultiplier() {
      bool result = setString(OS_WindowMaterial_BlindFields::BlindBottomOpeningMultiplier, "");
      OS_ASSERT(result);
    }

    bool Blind_Impl::setBlindLeftSideOpeningMultiplier(double blindLeftSideOpeningMultiplier) {
      bool result = setDouble(OS_WindowMaterial_BlindFields::BlindLeftSideOpeningMultiplier, blindLeftSideOpeningMultiplier);
      return result;
    }

    void Blind_Impl::resetBlindLeftSideOpeningMultiplier() {
      bool result = setString(OS_WindowMaterial_BlindFields::BlindLeftSideOpeningMultiplier, "");
      OS_ASSERT(result);
    }

    bool Blind_Impl::setBlindRightSideOpeningMultiplier(double blindRightSideOpeningMultiplier) {
      bool result = setDouble(OS_WindowMaterial_BlindFields::BlindRightSideOpeningMultiplier, blindRightSideOpeningMultiplier);
      return result;
    }

    void Blind_Impl::resetBlindRightSideOpeningMultiplier() {
      bool result = setString(OS_WindowMaterial_BlindFields::BlindRightSideOpeningMultiplier, "");
      OS_ASSERT(result);
    }

    bool Blind_Impl::setMinimumSlatAngle(double minimumSlatAngle) {
      bool result = setDouble(OS_WindowMaterial_BlindFields::MinimumSlatAngle, minimumSlatAngle);
      return result;
    }

    void Blind_Impl::resetMinimumSlatAngle() {
      bool result = setString(OS_WindowMaterial_BlindFields::MinimumSlatAngle, "");
      OS_ASSERT(result);
    }

    bool Blind_Impl::setMaximumSlatAngle(double maximumSlatAngle) {
      bool result = setDouble(OS_WindowMaterial_BlindFields::MaximumSlatAngle, maximumSlatAngle);
      return result;
    }

    void Blind_Impl::resetMaximumSlatAngle() {
      bool result = setString(OS_WindowMaterial_BlindFields::MaximumSlatAngle, "");
      OS_ASSERT(result);
    }

    std::vector<std::string> Blind_Impl::slatOrientationValues() const {
      return Blind::slatOrientationValues();
    }

  }  // namespace detail

  Blind::Blind(const Model& model, double slatWidth, double slatSeparation, double frontSideSlatBeamSolarReflectance,
               double backSideSlatBeamSolarReflectance, double frontSideSlatDiffuseSolarReflectance, double backSideSlatDiffuseSolarReflectance,
               double slatBeamVisibleTransmittance)
    : ShadingMaterial(Blind::iddObjectType(), model) {
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
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_WindowMaterial_BlindFields::SlatOrientation);
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

  double Blind::slatSeparation() const {
    return getImpl<detail::Blind_Impl>()->slatSeparation();
  }

  double Blind::slatThickness() const {
    return getImpl<detail::Blind_Impl>()->slatThickness();
  }

  bool Blind::isSlatThicknessDefaulted() const {
    return getImpl<detail::Blind_Impl>()->isSlatThicknessDefaulted();
  }

  double Blind::slatAngle() const {
    return getImpl<detail::Blind_Impl>()->slatAngle();
  }

  bool Blind::isSlatAngleDefaulted() const {
    return getImpl<detail::Blind_Impl>()->isSlatAngleDefaulted();
  }

  double Blind::slatConductivity() const {
    return getImpl<detail::Blind_Impl>()->slatConductivity();
  }

  bool Blind::isSlatConductivityDefaulted() const {
    return getImpl<detail::Blind_Impl>()->isSlatConductivityDefaulted();
  }

  double Blind::slatBeamSolarTransmittance() const {
    return getImpl<detail::Blind_Impl>()->slatBeamSolarTransmittance();
  }

  bool Blind::isSlatBeamSolarTransmittanceDefaulted() const {
    return getImpl<detail::Blind_Impl>()->isSlatBeamSolarTransmittanceDefaulted();
  }

  double Blind::frontSideSlatBeamSolarReflectance() const {
    return getImpl<detail::Blind_Impl>()->frontSideSlatBeamSolarReflectance();
  }

  double Blind::backSideSlatBeamSolarReflectance() const {
    return getImpl<detail::Blind_Impl>()->backSideSlatBeamSolarReflectance();
  }

  double Blind::slatDiffuseSolarTransmittance() const {
    return getImpl<detail::Blind_Impl>()->slatDiffuseSolarTransmittance();
  }

  bool Blind::isSlatDiffuseSolarTransmittanceDefaulted() const {
    return getImpl<detail::Blind_Impl>()->isSlatDiffuseSolarTransmittanceDefaulted();
  }

  double Blind::frontSideSlatDiffuseSolarReflectance() const {
    return getImpl<detail::Blind_Impl>()->frontSideSlatDiffuseSolarReflectance();
  }

  double Blind::backSideSlatDiffuseSolarReflectance() const {
    return getImpl<detail::Blind_Impl>()->backSideSlatDiffuseSolarReflectance();
  }

  double Blind::slatBeamVisibleTransmittance() const {
    return getImpl<detail::Blind_Impl>()->slatBeamVisibleTransmittance();
  }

  boost::optional<double> Blind::frontSideSlatBeamVisibleReflectance() const {
    return getImpl<detail::Blind_Impl>()->frontSideSlatBeamVisibleReflectance();
  }

  boost::optional<double> Blind::backSideSlatBeamVisibleReflectance() const {
    return getImpl<detail::Blind_Impl>()->backSideSlatBeamVisibleReflectance();
  }

  double Blind::slatDiffuseVisibleTransmittance() const {
    return getImpl<detail::Blind_Impl>()->slatDiffuseVisibleTransmittance();
  }

  bool Blind::isSlatDiffuseVisibleTransmittanceDefaulted() const {
    return getImpl<detail::Blind_Impl>()->isSlatDiffuseVisibleTransmittanceDefaulted();
  }

  boost::optional<double> Blind::frontSideSlatDiffuseVisibleReflectance() const {
    return getImpl<detail::Blind_Impl>()->frontSideSlatDiffuseVisibleReflectance();
  }

  boost::optional<double> Blind::backSideSlatDiffuseVisibleReflectance() const {
    return getImpl<detail::Blind_Impl>()->backSideSlatDiffuseVisibleReflectance();
  }

  double Blind::slatInfraredHemisphericalTransmittance() const {
    return getImpl<detail::Blind_Impl>()->slatInfraredHemisphericalTransmittance();
  }

  bool Blind::isSlatInfraredHemisphericalTransmittanceDefaulted() const {
    return getImpl<detail::Blind_Impl>()->isSlatInfraredHemisphericalTransmittanceDefaulted();
  }

  double Blind::frontSideSlatInfraredHemisphericalEmissivity() const {
    return getImpl<detail::Blind_Impl>()->frontSideSlatInfraredHemisphericalEmissivity();
  }

  bool Blind::isFrontSideSlatInfraredHemisphericalEmissivityDefaulted() const {
    return getImpl<detail::Blind_Impl>()->isFrontSideSlatInfraredHemisphericalEmissivityDefaulted();
  }

  double Blind::backSideSlatInfraredHemisphericalEmissivity() const {
    return getImpl<detail::Blind_Impl>()->backSideSlatInfraredHemisphericalEmissivity();
  }

  bool Blind::isBackSideSlatInfraredHemisphericalEmissivityDefaulted() const {
    return getImpl<detail::Blind_Impl>()->isBackSideSlatInfraredHemisphericalEmissivityDefaulted();
  }

  double Blind::blindtoGlassDistance() const {
    return getImpl<detail::Blind_Impl>()->blindtoGlassDistance();
  }

  bool Blind::isBlindtoGlassDistanceDefaulted() const {
    return getImpl<detail::Blind_Impl>()->isBlindtoGlassDistanceDefaulted();
  }

  double Blind::blindTopOpeningMultiplier() const {
    return getImpl<detail::Blind_Impl>()->blindTopOpeningMultiplier();
  }

  bool Blind::isBlindTopOpeningMultiplierDefaulted() const {
    return getImpl<detail::Blind_Impl>()->isBlindTopOpeningMultiplierDefaulted();
  }

  double Blind::blindBottomOpeningMultiplier() const {
    return getImpl<detail::Blind_Impl>()->blindBottomOpeningMultiplier();
  }

  bool Blind::isBlindBottomOpeningMultiplierDefaulted() const {
    return getImpl<detail::Blind_Impl>()->isBlindBottomOpeningMultiplierDefaulted();
  }

  double Blind::blindLeftSideOpeningMultiplier() const {
    return getImpl<detail::Blind_Impl>()->blindLeftSideOpeningMultiplier();
  }

  bool Blind::isBlindLeftSideOpeningMultiplierDefaulted() const {
    return getImpl<detail::Blind_Impl>()->isBlindLeftSideOpeningMultiplierDefaulted();
  }

  double Blind::blindRightSideOpeningMultiplier() const {
    return getImpl<detail::Blind_Impl>()->blindRightSideOpeningMultiplier();
  }

  bool Blind::isBlindRightSideOpeningMultiplierDefaulted() const {
    return getImpl<detail::Blind_Impl>()->isBlindRightSideOpeningMultiplierDefaulted();
  }

  double Blind::minimumSlatAngle() const {
    return getImpl<detail::Blind_Impl>()->minimumSlatAngle();
  }

  bool Blind::isMinimumSlatAngleDefaulted() const {
    return getImpl<detail::Blind_Impl>()->isMinimumSlatAngleDefaulted();
  }

  double Blind::maximumSlatAngle() const {
    return getImpl<detail::Blind_Impl>()->maximumSlatAngle();
  }

  bool Blind::isMaximumSlatAngleDefaulted() const {
    return getImpl<detail::Blind_Impl>()->isMaximumSlatAngleDefaulted();
  }

  bool Blind::setSlatOrientation(const std::string& slatOrientation) {
    return getImpl<detail::Blind_Impl>()->setSlatOrientation(slatOrientation);
  }

  void Blind::resetSlatOrientation() {
    getImpl<detail::Blind_Impl>()->resetSlatOrientation();
  }

  bool Blind::setSlatWidth(double slatWidth) {
    return getImpl<detail::Blind_Impl>()->setSlatWidth(slatWidth);
  }

  bool Blind::setSlatSeparation(double slatSeparation) {
    return getImpl<detail::Blind_Impl>()->setSlatSeparation(slatSeparation);
  }

  bool Blind::setSlatThickness(double slatThickness) {
    return getImpl<detail::Blind_Impl>()->setSlatThickness(slatThickness);
  }

  void Blind::resetSlatThickness() {
    getImpl<detail::Blind_Impl>()->resetSlatThickness();
  }

  bool Blind::setSlatAngle(double slatAngle) {
    return getImpl<detail::Blind_Impl>()->setSlatAngle(slatAngle);
  }

  void Blind::resetSlatAngle() {
    getImpl<detail::Blind_Impl>()->resetSlatAngle();
  }

  bool Blind::setSlatConductivity(double slatConductivity) {
    return getImpl<detail::Blind_Impl>()->setSlatConductivity(slatConductivity);
  }

  void Blind::resetSlatConductivity() {
    getImpl<detail::Blind_Impl>()->resetSlatConductivity();
  }

  bool Blind::setSlatBeamSolarTransmittance(double slatBeamSolarTransmittance) {
    return getImpl<detail::Blind_Impl>()->setSlatBeamSolarTransmittance(slatBeamSolarTransmittance);
  }

  void Blind::resetSlatBeamSolarTransmittance() {
    getImpl<detail::Blind_Impl>()->resetSlatBeamSolarTransmittance();
  }

  bool Blind::setFrontSideSlatBeamSolarReflectance(double frontSideSlatBeamSolarReflectance) {
    return getImpl<detail::Blind_Impl>()->setFrontSideSlatBeamSolarReflectance(frontSideSlatBeamSolarReflectance);
  }

  bool Blind::setBackSideSlatBeamSolarReflectance(double backSideSlatBeamSolarReflectance) {
    return getImpl<detail::Blind_Impl>()->setBackSideSlatBeamSolarReflectance(backSideSlatBeamSolarReflectance);
  }

  bool Blind::setSlatDiffuseSolarTransmittance(double slatDiffuseSolarTransmittance) {
    return getImpl<detail::Blind_Impl>()->setSlatDiffuseSolarTransmittance(slatDiffuseSolarTransmittance);
  }

  void Blind::resetSlatDiffuseSolarTransmittance() {
    getImpl<detail::Blind_Impl>()->resetSlatDiffuseSolarTransmittance();
  }

  bool Blind::setFrontSideSlatDiffuseSolarReflectance(double frontSideSlatDiffuseSolarReflectance) {
    return getImpl<detail::Blind_Impl>()->setFrontSideSlatDiffuseSolarReflectance(frontSideSlatDiffuseSolarReflectance);
  }

  bool Blind::setBackSideSlatDiffuseSolarReflectance(double backSideSlatDiffuseSolarReflectance) {
    return getImpl<detail::Blind_Impl>()->setBackSideSlatDiffuseSolarReflectance(backSideSlatDiffuseSolarReflectance);
  }

  bool Blind::setSlatBeamVisibleTransmittance(double slatBeamVisibleTransmittance) {
    return getImpl<detail::Blind_Impl>()->setSlatBeamVisibleTransmittance(slatBeamVisibleTransmittance);
  }

  bool Blind::setFrontSideSlatBeamVisibleReflectance(double frontSideSlatBeamVisibleReflectance) {
    return getImpl<detail::Blind_Impl>()->setFrontSideSlatBeamVisibleReflectance(frontSideSlatBeamVisibleReflectance);
  }

  void Blind::resetFrontSideSlatBeamVisibleReflectance() {
    getImpl<detail::Blind_Impl>()->resetFrontSideSlatBeamVisibleReflectance();
  }

  bool Blind::setBackSideSlatBeamVisibleReflectance(double backSideSlatBeamVisibleReflectance) {
    return getImpl<detail::Blind_Impl>()->setBackSideSlatBeamVisibleReflectance(backSideSlatBeamVisibleReflectance);
  }

  void Blind::resetBackSideSlatBeamVisibleReflectance() {
    getImpl<detail::Blind_Impl>()->resetBackSideSlatBeamVisibleReflectance();
  }

  bool Blind::setSlatDiffuseVisibleTransmittance(double slatDiffuseVisibleTransmittance) {
    return getImpl<detail::Blind_Impl>()->setSlatDiffuseVisibleTransmittance(slatDiffuseVisibleTransmittance);
  }

  void Blind::resetSlatDiffuseVisibleTransmittance() {
    getImpl<detail::Blind_Impl>()->resetSlatDiffuseVisibleTransmittance();
  }

  bool Blind::setFrontSideSlatDiffuseVisibleReflectance(double frontSideSlatDiffuseVisibleReflectance) {
    return getImpl<detail::Blind_Impl>()->setFrontSideSlatDiffuseVisibleReflectance(frontSideSlatDiffuseVisibleReflectance);
  }

  void Blind::resetFrontSideSlatDiffuseVisibleReflectance() {
    getImpl<detail::Blind_Impl>()->resetFrontSideSlatDiffuseVisibleReflectance();
  }

  bool Blind::setBackSideSlatDiffuseVisibleReflectance(double backSideSlatDiffuseVisibleReflectance) {
    return getImpl<detail::Blind_Impl>()->setBackSideSlatDiffuseVisibleReflectance(backSideSlatDiffuseVisibleReflectance);
  }

  void Blind::resetBackSideSlatDiffuseVisibleReflectance() {
    getImpl<detail::Blind_Impl>()->resetBackSideSlatDiffuseVisibleReflectance();
  }

  bool Blind::setSlatInfraredHemisphericalTransmittance(double slatInfraredHemisphericalTransmittance) {
    return getImpl<detail::Blind_Impl>()->setSlatInfraredHemisphericalTransmittance(slatInfraredHemisphericalTransmittance);
  }

  void Blind::resetSlatInfraredHemisphericalTransmittance() {
    getImpl<detail::Blind_Impl>()->resetSlatInfraredHemisphericalTransmittance();
  }

  bool Blind::setFrontSideSlatInfraredHemisphericalEmissivity(double frontSideSlatInfraredHemisphericalEmissivity) {
    return getImpl<detail::Blind_Impl>()->setFrontSideSlatInfraredHemisphericalEmissivity(frontSideSlatInfraredHemisphericalEmissivity);
  }

  void Blind::resetFrontSideSlatInfraredHemisphericalEmissivity() {
    getImpl<detail::Blind_Impl>()->resetFrontSideSlatInfraredHemisphericalEmissivity();
  }

  bool Blind::setBackSideSlatInfraredHemisphericalEmissivity(double backSideSlatInfraredHemisphericalEmissivity) {
    return getImpl<detail::Blind_Impl>()->setBackSideSlatInfraredHemisphericalEmissivity(backSideSlatInfraredHemisphericalEmissivity);
  }

  void Blind::resetBackSideSlatInfraredHemisphericalEmissivity() {
    getImpl<detail::Blind_Impl>()->resetBackSideSlatInfraredHemisphericalEmissivity();
  }

  bool Blind::setBlindtoGlassDistance(double blindtoGlassDistance) {
    return getImpl<detail::Blind_Impl>()->setBlindtoGlassDistance(blindtoGlassDistance);
  }

  void Blind::resetBlindtoGlassDistance() {
    getImpl<detail::Blind_Impl>()->resetBlindtoGlassDistance();
  }

  bool Blind::setBlindTopOpeningMultiplier(double blindTopOpeningMultiplier) {
    return getImpl<detail::Blind_Impl>()->setBlindTopOpeningMultiplier(blindTopOpeningMultiplier);
  }

  void Blind::resetBlindTopOpeningMultiplier() {
    getImpl<detail::Blind_Impl>()->resetBlindTopOpeningMultiplier();
  }

  bool Blind::setBlindBottomOpeningMultiplier(double blindBottomOpeningMultiplier) {
    return getImpl<detail::Blind_Impl>()->setBlindBottomOpeningMultiplier(blindBottomOpeningMultiplier);
  }

  void Blind::resetBlindBottomOpeningMultiplier() {
    getImpl<detail::Blind_Impl>()->resetBlindBottomOpeningMultiplier();
  }

  bool Blind::setBlindLeftSideOpeningMultiplier(double blindLeftSideOpeningMultiplier) {
    return getImpl<detail::Blind_Impl>()->setBlindLeftSideOpeningMultiplier(blindLeftSideOpeningMultiplier);
  }

  void Blind::resetBlindLeftSideOpeningMultiplier() {
    getImpl<detail::Blind_Impl>()->resetBlindLeftSideOpeningMultiplier();
  }

  bool Blind::setBlindRightSideOpeningMultiplier(double blindRightSideOpeningMultiplier) {
    return getImpl<detail::Blind_Impl>()->setBlindRightSideOpeningMultiplier(blindRightSideOpeningMultiplier);
  }

  void Blind::resetBlindRightSideOpeningMultiplier() {
    getImpl<detail::Blind_Impl>()->resetBlindRightSideOpeningMultiplier();
  }

  bool Blind::setMinimumSlatAngle(double minimumSlatAngle) {
    return getImpl<detail::Blind_Impl>()->setMinimumSlatAngle(minimumSlatAngle);
  }

  void Blind::resetMinimumSlatAngle() {
    getImpl<detail::Blind_Impl>()->resetMinimumSlatAngle();
  }

  bool Blind::setMaximumSlatAngle(double maximumSlatAngle) {
    return getImpl<detail::Blind_Impl>()->setMaximumSlatAngle(maximumSlatAngle);
  }

  void Blind::resetMaximumSlatAngle() {
    getImpl<detail::Blind_Impl>()->resetMaximumSlatAngle();
  }

  /// @cond
  Blind::Blind(std::shared_ptr<detail::Blind_Impl> impl) : ShadingMaterial(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
