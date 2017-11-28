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

#include "Screen.hpp"
#include "Screen_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_WindowMaterial_Screen_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  Screen_Impl::Screen_Impl(const IdfObject& idfObject,
                           Model_Impl* model,
                           bool keepHandle)
    : ShadingMaterial_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == Screen::iddObjectType());
  }

  Screen_Impl::Screen_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                           Model_Impl* model,
                           bool keepHandle)
    : ShadingMaterial_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == Screen::iddObjectType());
  }

  Screen_Impl::Screen_Impl(const Screen_Impl& other,
                           Model_Impl* model,
                           bool keepHandle)
    : ShadingMaterial_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& Screen_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType Screen_Impl::iddObjectType() const {
    return Screen::iddObjectType();
  }

  double Screen_Impl::thickness() const {
    OptionalDouble od = getDouble(OS_WindowMaterial_ScreenFields::ScreenMaterialDiameter,true);
    if (!od) {
      LOG_AND_THROW("Screen material diameter not yet set for " << briefDescription() << ".");
    }
    return *od;
  }

  double Screen_Impl::thermalConductivity() const {
    OptionalDouble od = getDouble(OS_WindowMaterial_ScreenFields::Conductivity,true);
    if (!od) {
      LOG_AND_THROW("Thermal conductivity not yet set for " << briefDescription() << ".");
    }
    return *od;
  }

  double Screen_Impl::thermalConductance() const {
    return thermalConductivity()/thickness();
  }

  double Screen_Impl::thermalResistivity() const {
    return 1.0/thermalConductivity();
  }

  double Screen_Impl::thermalResistance() const {
    return 1.0/thermalConductance();
  }

  OptionalDouble Screen_Impl::getVisibleTransmittance() const {
    LOG_AND_THROW("Visible transmittance not yet supported for Screens.");
    return 0.0;
  }

  bool Screen_Impl::setThickness(double value) {
    return setDouble(OS_WindowMaterial_ScreenFields::ScreenMaterialDiameter,value);
  }

  bool Screen_Impl::setThermalConductivity(double value) {
    return setDouble(OS_WindowMaterial_ScreenFields::Conductivity,value);
  }

  bool Screen_Impl::setThermalResistivity(double value) {
    return setThermalConductivity(1.0/value);
  }

  std::string Screen_Impl::reflectedBeamTransmittanceAccountingMethod() const {
    boost::optional<std::string> value = getString(OS_WindowMaterial_ScreenFields::ReflectedBeamTransmittanceAccountingMethod,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool Screen_Impl::isReflectedBeamTransmittanceAccountingMethodDefaulted() const {
    return isEmpty(OS_WindowMaterial_ScreenFields::ReflectedBeamTransmittanceAccountingMethod);
  }

  double Screen_Impl::diffuseSolarReflectance() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_ScreenFields::DiffuseSolarReflectance,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity Screen_Impl::getDiffuseSolarReflectance(bool returnIP) const {
    OptionalDouble value = diffuseSolarReflectance();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_ScreenFields::DiffuseSolarReflectance, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  double Screen_Impl::diffuseVisibleReflectance() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_ScreenFields::DiffuseVisibleReflectance,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity Screen_Impl::getDiffuseVisibleReflectance(bool returnIP) const {
    OptionalDouble value = diffuseVisibleReflectance();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_ScreenFields::DiffuseVisibleReflectance, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  double Screen_Impl::thermalHemisphericalEmissivity() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_ScreenFields::ThermalHemisphericalEmissivity,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity Screen_Impl::getThermalHemisphericalEmissivity(bool returnIP) const {
    OptionalDouble value = thermalHemisphericalEmissivity();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_ScreenFields::ThermalHemisphericalEmissivity, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool Screen_Impl::isThermalHemisphericalEmissivityDefaulted() const {
    return isEmpty(OS_WindowMaterial_ScreenFields::ThermalHemisphericalEmissivity);
  }

  double Screen_Impl::conductivity() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_ScreenFields::Conductivity,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity Screen_Impl::getConductivity(bool returnIP) const {
    OptionalDouble value = conductivity();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_ScreenFields::Conductivity, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool Screen_Impl::isConductivityDefaulted() const {
    return isEmpty(OS_WindowMaterial_ScreenFields::Conductivity);
  }

  double Screen_Impl::screenMaterialSpacing() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_ScreenFields::ScreenMaterialSpacing,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity Screen_Impl::getScreenMaterialSpacing(bool returnIP) const {
    OptionalDouble value = screenMaterialSpacing();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_ScreenFields::ScreenMaterialSpacing, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  double Screen_Impl::screenMaterialDiameter() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_ScreenFields::ScreenMaterialDiameter,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity Screen_Impl::getScreenMaterialDiameter(bool returnIP) const {
    OptionalDouble value = screenMaterialDiameter();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_ScreenFields::ScreenMaterialDiameter, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  double Screen_Impl::screentoGlassDistance() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_ScreenFields::ScreentoGlassDistance,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity Screen_Impl::getScreentoGlassDistance(bool returnIP) const {
    OptionalDouble value = screentoGlassDistance();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_ScreenFields::ScreentoGlassDistance, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool Screen_Impl::isScreentoGlassDistanceDefaulted() const {
    return isEmpty(OS_WindowMaterial_ScreenFields::ScreentoGlassDistance);
  }

  double Screen_Impl::topOpeningMultiplier() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_ScreenFields::TopOpeningMultiplier,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity Screen_Impl::getTopOpeningMultiplier(bool returnIP) const {
    OptionalDouble value = topOpeningMultiplier();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_ScreenFields::TopOpeningMultiplier, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool Screen_Impl::isTopOpeningMultiplierDefaulted() const {
    return isEmpty(OS_WindowMaterial_ScreenFields::TopOpeningMultiplier);
  }

  double Screen_Impl::bottomOpeningMultiplier() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_ScreenFields::BottomOpeningMultiplier,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity Screen_Impl::getBottomOpeningMultiplier(bool returnIP) const {
    OptionalDouble value = bottomOpeningMultiplier();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_ScreenFields::BottomOpeningMultiplier, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool Screen_Impl::isBottomOpeningMultiplierDefaulted() const {
    return isEmpty(OS_WindowMaterial_ScreenFields::BottomOpeningMultiplier);
  }

  double Screen_Impl::leftSideOpeningMultiplier() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_ScreenFields::LeftSideOpeningMultiplier,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity Screen_Impl::getLeftSideOpeningMultiplier(bool returnIP) const {
    OptionalDouble value = leftSideOpeningMultiplier();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_ScreenFields::LeftSideOpeningMultiplier, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool Screen_Impl::isLeftSideOpeningMultiplierDefaulted() const {
    return isEmpty(OS_WindowMaterial_ScreenFields::LeftSideOpeningMultiplier);
  }

  double Screen_Impl::rightSideOpeningMultiplier() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_ScreenFields::RightSideOpeningMultiplier,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity Screen_Impl::getRightSideOpeningMultiplier(bool returnIP) const {
    OptionalDouble value = rightSideOpeningMultiplier();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_ScreenFields::RightSideOpeningMultiplier, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool Screen_Impl::isRightSideOpeningMultiplierDefaulted() const {
    return isEmpty(OS_WindowMaterial_ScreenFields::RightSideOpeningMultiplier);
  }

  std::string Screen_Impl::angleofResolutionforScreenTransmittanceOutputMap() const {
    boost::optional<std::string> value = getString(OS_WindowMaterial_ScreenFields::AngleofResolutionforScreenTransmittanceOutputMap,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool Screen_Impl::isAngleofResolutionforScreenTransmittanceOutputMapDefaulted() const {
    return isEmpty(OS_WindowMaterial_ScreenFields::AngleofResolutionforScreenTransmittanceOutputMap);
  }

  bool Screen_Impl::setReflectedBeamTransmittanceAccountingMethod(std::string reflectedBeamTransmittanceAccountingMethod) {
    bool result = setString(OS_WindowMaterial_ScreenFields::ReflectedBeamTransmittanceAccountingMethod, reflectedBeamTransmittanceAccountingMethod);
    return result;
  }

  void Screen_Impl::resetReflectedBeamTransmittanceAccountingMethod() {
    bool result = setString(OS_WindowMaterial_ScreenFields::ReflectedBeamTransmittanceAccountingMethod, "");
    OS_ASSERT(result);
  }

  bool Screen_Impl::setDiffuseSolarReflectance(double diffuseSolarReflectance) {
    bool result = setDouble(OS_WindowMaterial_ScreenFields::DiffuseSolarReflectance, diffuseSolarReflectance);
    return result;
  }

  bool Screen_Impl::setDiffuseSolarReflectance(const Quantity& diffuseSolarReflectance) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_ScreenFields::DiffuseSolarReflectance,diffuseSolarReflectance);
    if (!value) {
      return false;
    }
    return setDiffuseSolarReflectance(value.get());
  }

  bool Screen_Impl::setDiffuseVisibleReflectance(double diffuseVisibleReflectance) {
    bool result = setDouble(OS_WindowMaterial_ScreenFields::DiffuseVisibleReflectance, diffuseVisibleReflectance);
    return result;
  }

  bool Screen_Impl::setDiffuseVisibleReflectance(const Quantity& diffuseVisibleReflectance) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_ScreenFields::DiffuseVisibleReflectance,diffuseVisibleReflectance);
    if (!value) {
      return false;
    }
    return setDiffuseVisibleReflectance(value.get());
  }

  bool Screen_Impl::setThermalHemisphericalEmissivity(double thermalHemisphericalEmissivity) {
    bool result = setDouble(OS_WindowMaterial_ScreenFields::ThermalHemisphericalEmissivity, thermalHemisphericalEmissivity);
    return result;
  }

  bool Screen_Impl::setThermalHemisphericalEmissivity(const Quantity& thermalHemisphericalEmissivity) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_ScreenFields::ThermalHemisphericalEmissivity,thermalHemisphericalEmissivity);
    if (!value) {
      return false;
    }
    return setThermalHemisphericalEmissivity(value.get());
  }

  void Screen_Impl::resetThermalHemisphericalEmissivity() {
    bool result = setString(OS_WindowMaterial_ScreenFields::ThermalHemisphericalEmissivity, "");
    OS_ASSERT(result);
  }

  bool Screen_Impl::setConductivity(double conductivity) {
    bool result = setDouble(OS_WindowMaterial_ScreenFields::Conductivity, conductivity);
    return result;
  }

  bool Screen_Impl::setConductivity(const Quantity& conductivity) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_ScreenFields::Conductivity,conductivity);
    if (!value) {
      return false;
    }
    return setConductivity(value.get());
  }

  void Screen_Impl::resetConductivity() {
    bool result = setString(OS_WindowMaterial_ScreenFields::Conductivity, "");
    OS_ASSERT(result);
  }

  bool Screen_Impl::setScreenMaterialSpacing(double screenMaterialSpacing) {
    bool result = setDouble(OS_WindowMaterial_ScreenFields::ScreenMaterialSpacing, screenMaterialSpacing);
    return result;
  }

  bool Screen_Impl::setScreenMaterialSpacing(const Quantity& screenMaterialSpacing) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_ScreenFields::ScreenMaterialSpacing,screenMaterialSpacing);
    if (!value) {
      return false;
    }
    return setScreenMaterialSpacing(value.get());
  }

  bool Screen_Impl::setScreenMaterialDiameter(double screenMaterialDiameter) {
    bool result = setDouble(OS_WindowMaterial_ScreenFields::ScreenMaterialDiameter, screenMaterialDiameter);
    return result;
  }

  bool Screen_Impl::setScreenMaterialDiameter(const Quantity& screenMaterialDiameter) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_ScreenFields::ScreenMaterialDiameter,screenMaterialDiameter);
    if (!value) {
      return false;
    }
    return setScreenMaterialDiameter(value.get());
  }

  bool Screen_Impl::setScreentoGlassDistance(double screentoGlassDistance) {
    bool result = setDouble(OS_WindowMaterial_ScreenFields::ScreentoGlassDistance, screentoGlassDistance);
    return result;
  }

  bool Screen_Impl::setScreentoGlassDistance(const Quantity& screentoGlassDistance) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_ScreenFields::ScreentoGlassDistance,screentoGlassDistance);
    if (!value) {
      return false;
    }
    return setScreentoGlassDistance(value.get());
  }

  void Screen_Impl::resetScreentoGlassDistance() {
    bool result = setString(OS_WindowMaterial_ScreenFields::ScreentoGlassDistance, "");
    OS_ASSERT(result);
  }

  bool Screen_Impl::setTopOpeningMultiplier(double topOpeningMultiplier) {
    bool result = setDouble(OS_WindowMaterial_ScreenFields::TopOpeningMultiplier, topOpeningMultiplier);
    return result;
  }

  bool Screen_Impl::setTopOpeningMultiplier(const Quantity& topOpeningMultiplier) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_ScreenFields::TopOpeningMultiplier,topOpeningMultiplier);
    if (!value) {
      return false;
    }
    return setTopOpeningMultiplier(value.get());
  }

  void Screen_Impl::resetTopOpeningMultiplier() {
    bool result = setString(OS_WindowMaterial_ScreenFields::TopOpeningMultiplier, "");
    OS_ASSERT(result);
  }

  bool Screen_Impl::setBottomOpeningMultiplier(double bottomOpeningMultiplier) {
    bool result = setDouble(OS_WindowMaterial_ScreenFields::BottomOpeningMultiplier, bottomOpeningMultiplier);
    return result;
  }

  bool Screen_Impl::setBottomOpeningMultiplier(const Quantity& bottomOpeningMultiplier) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_ScreenFields::BottomOpeningMultiplier,bottomOpeningMultiplier);
    if (!value) {
      return false;
    }
    return setBottomOpeningMultiplier(value.get());
  }

  void Screen_Impl::resetBottomOpeningMultiplier() {
    bool result = setString(OS_WindowMaterial_ScreenFields::BottomOpeningMultiplier, "");
    OS_ASSERT(result);
  }

  bool Screen_Impl::setLeftSideOpeningMultiplier(double leftSideOpeningMultiplier) {
    bool result = setDouble(OS_WindowMaterial_ScreenFields::LeftSideOpeningMultiplier, leftSideOpeningMultiplier);
    return result;
  }

  bool Screen_Impl::setLeftSideOpeningMultiplier(const Quantity& leftSideOpeningMultiplier) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_ScreenFields::LeftSideOpeningMultiplier,leftSideOpeningMultiplier);
    if (!value) {
      return false;
    }
    return setLeftSideOpeningMultiplier(value.get());
  }

  void Screen_Impl::resetLeftSideOpeningMultiplier() {
    bool result = setString(OS_WindowMaterial_ScreenFields::LeftSideOpeningMultiplier, "");
    OS_ASSERT(result);
  }

  bool Screen_Impl::setRightSideOpeningMultiplier(double rightSideOpeningMultiplier) {
    bool result = setDouble(OS_WindowMaterial_ScreenFields::RightSideOpeningMultiplier, rightSideOpeningMultiplier);
    return result;
  }

  bool Screen_Impl::setRightSideOpeningMultiplier(const Quantity& rightSideOpeningMultiplier) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_ScreenFields::RightSideOpeningMultiplier,rightSideOpeningMultiplier);
    if (!value) {
      return false;
    }
    return setRightSideOpeningMultiplier(value.get());
  }

  void Screen_Impl::resetRightSideOpeningMultiplier() {
    bool result = setString(OS_WindowMaterial_ScreenFields::RightSideOpeningMultiplier, "");
    OS_ASSERT(result);
  }

  bool Screen_Impl::setAngleofResolutionforScreenTransmittanceOutputMap(std::string angleofResolutionforScreenTransmittanceOutputMap) {
    bool result = setString(OS_WindowMaterial_ScreenFields::AngleofResolutionforScreenTransmittanceOutputMap, angleofResolutionforScreenTransmittanceOutputMap);
    return result;
  }

  void Screen_Impl::resetAngleofResolutionforScreenTransmittanceOutputMap() {
    bool result = setString(OS_WindowMaterial_ScreenFields::AngleofResolutionforScreenTransmittanceOutputMap, "");
    OS_ASSERT(result);
  }

  std::vector<std::string> Screen_Impl::reflectedBeamTransmittanceAccountingMethodValues() const {
    return Screen::reflectedBeamTransmittanceAccountingMethodValues();
  }

  openstudio::Quantity Screen_Impl::diffuseSolarReflectance_SI() const {
    return getDiffuseSolarReflectance(false);
  }

  openstudio::Quantity Screen_Impl::diffuseSolarReflectance_IP() const {
    return getDiffuseSolarReflectance(true);
  }

  openstudio::Quantity Screen_Impl::diffuseVisibleReflectance_SI() const {
    return getDiffuseVisibleReflectance(false);
  }

  openstudio::Quantity Screen_Impl::diffuseVisibleReflectance_IP() const {
    return getDiffuseVisibleReflectance(true);
  }

  openstudio::Quantity Screen_Impl::thermalHemisphericalEmissivity_SI() const {
    return getThermalHemisphericalEmissivity(false);
  }

  openstudio::Quantity Screen_Impl::thermalHemisphericalEmissivity_IP() const {
    return getThermalHemisphericalEmissivity(true);
  }

  openstudio::Quantity Screen_Impl::conductivity_SI() const {
    return getConductivity(false);
  }

  openstudio::Quantity Screen_Impl::conductivity_IP() const {
    return getConductivity(true);
  }

  openstudio::Quantity Screen_Impl::screenMaterialSpacing_SI() const {
    return getScreenMaterialSpacing(false);
  }

  openstudio::Quantity Screen_Impl::screenMaterialSpacing_IP() const {
    return getScreenMaterialSpacing(true);
  }

  openstudio::Quantity Screen_Impl::screenMaterialDiameter_SI() const {
    return getScreenMaterialDiameter(false);
  }

  openstudio::Quantity Screen_Impl::screenMaterialDiameter_IP() const {
    return getScreenMaterialDiameter(true);
  }

  openstudio::Quantity Screen_Impl::screentoGlassDistance_SI() const {
    return getScreentoGlassDistance(false);
  }

  openstudio::Quantity Screen_Impl::screentoGlassDistance_IP() const {
    return getScreentoGlassDistance(true);
  }

  openstudio::Quantity Screen_Impl::topOpeningMultiplier_SI() const {
    return getTopOpeningMultiplier(false);
  }

  openstudio::Quantity Screen_Impl::topOpeningMultiplier_IP() const {
    return getTopOpeningMultiplier(true);
  }

  openstudio::Quantity Screen_Impl::bottomOpeningMultiplier_SI() const {
    return getBottomOpeningMultiplier(false);
  }

  openstudio::Quantity Screen_Impl::bottomOpeningMultiplier_IP() const {
    return getBottomOpeningMultiplier(true);
  }

  openstudio::Quantity Screen_Impl::leftSideOpeningMultiplier_SI() const {
    return getLeftSideOpeningMultiplier(false);
  }

  openstudio::Quantity Screen_Impl::leftSideOpeningMultiplier_IP() const {
    return getLeftSideOpeningMultiplier(true);
  }

  openstudio::Quantity Screen_Impl::rightSideOpeningMultiplier_SI() const {
    return getRightSideOpeningMultiplier(false);
  }

  openstudio::Quantity Screen_Impl::rightSideOpeningMultiplier_IP() const {
    return getRightSideOpeningMultiplier(true);
  }

  std::vector<std::string> Screen_Impl::angleofResolutionforScreenTransmittanceOutputMapValues() const {
    return Screen::angleofResolutionforScreenTransmittanceOutputMapValues();
  }

} // detail

Screen::Screen(const Model& model,
               double diffuseSolarReflectance,
               double diffuseVisibleReflectance,
               double screenMaterialSpacing,
               double screenMaterialDiameter)
  : ShadingMaterial(Screen::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::Screen_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
   ok = setDiffuseSolarReflectance(diffuseSolarReflectance);
  OS_ASSERT(ok);
   ok = setDiffuseVisibleReflectance(diffuseVisibleReflectance);
  OS_ASSERT(ok);
   ok = setScreenMaterialSpacing(screenMaterialSpacing);
  OS_ASSERT(ok);
   ok = setScreenMaterialDiameter(screenMaterialDiameter);
  OS_ASSERT(ok);
}

IddObjectType Screen::iddObjectType() {
  return IddObjectType(IddObjectType::OS_WindowMaterial_Screen);
}

double Screen::thermalConductivity() const {
  return getImpl<detail::Screen_Impl>()->thermalConductivity();
}

double Screen::thermalConductance() const {
  return getImpl<detail::Screen_Impl>()->thermalConductance();
}

double Screen::thermalResistivity() const {
  return getImpl<detail::Screen_Impl>()->thermalResistivity();
}

double Screen::thermalResistance() const {
  return getImpl<detail::Screen_Impl>()->thermalResistance();
}

bool Screen::setThermalConductivity(double value) {
  return getImpl<detail::Screen_Impl>()->setThermalConductivity(value);
}

bool Screen::setThermalResistivity(double value) {
  return getImpl<detail::Screen_Impl>()->setThermalResistivity(value);
}

std::vector<std::string> Screen::reflectedBeamTransmittanceAccountingMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_WindowMaterial_ScreenFields::ReflectedBeamTransmittanceAccountingMethod);
}

std::vector<std::string> Screen::angleofResolutionforScreenTransmittanceOutputMapValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_WindowMaterial_ScreenFields::AngleofResolutionforScreenTransmittanceOutputMap);
}

std::string Screen::reflectedBeamTransmittanceAccountingMethod() const {
  return getImpl<detail::Screen_Impl>()->reflectedBeamTransmittanceAccountingMethod();
}

bool Screen::isReflectedBeamTransmittanceAccountingMethodDefaulted() const {
  return getImpl<detail::Screen_Impl>()->isReflectedBeamTransmittanceAccountingMethodDefaulted();
}

double Screen::diffuseSolarReflectance() const {
  return getImpl<detail::Screen_Impl>()->diffuseSolarReflectance();
}

Quantity Screen::getDiffuseSolarReflectance(bool returnIP) const {
  return getImpl<detail::Screen_Impl>()->getDiffuseSolarReflectance(returnIP);
}

double Screen::diffuseVisibleReflectance() const {
  return getImpl<detail::Screen_Impl>()->diffuseVisibleReflectance();
}

Quantity Screen::getDiffuseVisibleReflectance(bool returnIP) const {
  return getImpl<detail::Screen_Impl>()->getDiffuseVisibleReflectance(returnIP);
}

double Screen::thermalHemisphericalEmissivity() const {
  return getImpl<detail::Screen_Impl>()->thermalHemisphericalEmissivity();
}

Quantity Screen::getThermalHemisphericalEmissivity(bool returnIP) const {
  return getImpl<detail::Screen_Impl>()->getThermalHemisphericalEmissivity(returnIP);
}

bool Screen::isThermalHemisphericalEmissivityDefaulted() const {
  return getImpl<detail::Screen_Impl>()->isThermalHemisphericalEmissivityDefaulted();
}

double Screen::conductivity() const {
  return getImpl<detail::Screen_Impl>()->conductivity();
}

Quantity Screen::getConductivity(bool returnIP) const {
  return getImpl<detail::Screen_Impl>()->getConductivity(returnIP);
}

bool Screen::isConductivityDefaulted() const {
  return getImpl<detail::Screen_Impl>()->isConductivityDefaulted();
}

double Screen::screenMaterialSpacing() const {
  return getImpl<detail::Screen_Impl>()->screenMaterialSpacing();
}

Quantity Screen::getScreenMaterialSpacing(bool returnIP) const {
  return getImpl<detail::Screen_Impl>()->getScreenMaterialSpacing(returnIP);
}

double Screen::screenMaterialDiameter() const {
  return getImpl<detail::Screen_Impl>()->screenMaterialDiameter();
}

Quantity Screen::getScreenMaterialDiameter(bool returnIP) const {
  return getImpl<detail::Screen_Impl>()->getScreenMaterialDiameter(returnIP);
}

double Screen::screentoGlassDistance() const {
  return getImpl<detail::Screen_Impl>()->screentoGlassDistance();
}

Quantity Screen::getScreentoGlassDistance(bool returnIP) const {
  return getImpl<detail::Screen_Impl>()->getScreentoGlassDistance(returnIP);
}

bool Screen::isScreentoGlassDistanceDefaulted() const {
  return getImpl<detail::Screen_Impl>()->isScreentoGlassDistanceDefaulted();
}

double Screen::topOpeningMultiplier() const {
  return getImpl<detail::Screen_Impl>()->topOpeningMultiplier();
}

Quantity Screen::getTopOpeningMultiplier(bool returnIP) const {
  return getImpl<detail::Screen_Impl>()->getTopOpeningMultiplier(returnIP);
}

bool Screen::isTopOpeningMultiplierDefaulted() const {
  return getImpl<detail::Screen_Impl>()->isTopOpeningMultiplierDefaulted();
}

double Screen::bottomOpeningMultiplier() const {
  return getImpl<detail::Screen_Impl>()->bottomOpeningMultiplier();
}

Quantity Screen::getBottomOpeningMultiplier(bool returnIP) const {
  return getImpl<detail::Screen_Impl>()->getBottomOpeningMultiplier(returnIP);
}

bool Screen::isBottomOpeningMultiplierDefaulted() const {
  return getImpl<detail::Screen_Impl>()->isBottomOpeningMultiplierDefaulted();
}

double Screen::leftSideOpeningMultiplier() const {
  return getImpl<detail::Screen_Impl>()->leftSideOpeningMultiplier();
}

Quantity Screen::getLeftSideOpeningMultiplier(bool returnIP) const {
  return getImpl<detail::Screen_Impl>()->getLeftSideOpeningMultiplier(returnIP);
}

bool Screen::isLeftSideOpeningMultiplierDefaulted() const {
  return getImpl<detail::Screen_Impl>()->isLeftSideOpeningMultiplierDefaulted();
}

double Screen::rightSideOpeningMultiplier() const {
  return getImpl<detail::Screen_Impl>()->rightSideOpeningMultiplier();
}

Quantity Screen::getRightSideOpeningMultiplier(bool returnIP) const {
  return getImpl<detail::Screen_Impl>()->getRightSideOpeningMultiplier(returnIP);
}

bool Screen::isRightSideOpeningMultiplierDefaulted() const {
  return getImpl<detail::Screen_Impl>()->isRightSideOpeningMultiplierDefaulted();
}

std::string Screen::angleofResolutionforScreenTransmittanceOutputMap() const {
  return getImpl<detail::Screen_Impl>()->angleofResolutionforScreenTransmittanceOutputMap();
}

bool Screen::isAngleofResolutionforScreenTransmittanceOutputMapDefaulted() const {
  return getImpl<detail::Screen_Impl>()->isAngleofResolutionforScreenTransmittanceOutputMapDefaulted();
}

bool Screen::setReflectedBeamTransmittanceAccountingMethod(std::string reflectedBeamTransmittanceAccountingMethod) {
  return getImpl<detail::Screen_Impl>()->setReflectedBeamTransmittanceAccountingMethod(reflectedBeamTransmittanceAccountingMethod);
}

void Screen::resetReflectedBeamTransmittanceAccountingMethod() {
  getImpl<detail::Screen_Impl>()->resetReflectedBeamTransmittanceAccountingMethod();
}

bool Screen::setDiffuseSolarReflectance(double diffuseSolarReflectance) {
  return getImpl<detail::Screen_Impl>()->setDiffuseSolarReflectance(diffuseSolarReflectance);
}

bool Screen::setDiffuseSolarReflectance(const Quantity& diffuseSolarReflectance) {
  return getImpl<detail::Screen_Impl>()->setDiffuseSolarReflectance(diffuseSolarReflectance);
}

bool Screen::setDiffuseVisibleReflectance(double diffuseVisibleReflectance) {
  return getImpl<detail::Screen_Impl>()->setDiffuseVisibleReflectance(diffuseVisibleReflectance);
}

bool Screen::setDiffuseVisibleReflectance(const Quantity& diffuseVisibleReflectance) {
  return getImpl<detail::Screen_Impl>()->setDiffuseVisibleReflectance(diffuseVisibleReflectance);
}

bool Screen::setThermalHemisphericalEmissivity(double thermalHemisphericalEmissivity) {
  return getImpl<detail::Screen_Impl>()->setThermalHemisphericalEmissivity(thermalHemisphericalEmissivity);
}

bool Screen::setThermalHemisphericalEmissivity(const Quantity& thermalHemisphericalEmissivity) {
  return getImpl<detail::Screen_Impl>()->setThermalHemisphericalEmissivity(thermalHemisphericalEmissivity);
}

void Screen::resetThermalHemisphericalEmissivity() {
  getImpl<detail::Screen_Impl>()->resetThermalHemisphericalEmissivity();
}

bool Screen::setConductivity(double conductivity) {
  return getImpl<detail::Screen_Impl>()->setConductivity(conductivity);
}

bool Screen::setConductivity(const Quantity& conductivity) {
  return getImpl<detail::Screen_Impl>()->setConductivity(conductivity);
}

void Screen::resetConductivity() {
  getImpl<detail::Screen_Impl>()->resetConductivity();
}

bool Screen::setScreenMaterialSpacing(double screenMaterialSpacing) {
  return getImpl<detail::Screen_Impl>()->setScreenMaterialSpacing(screenMaterialSpacing);
}

bool Screen::setScreenMaterialSpacing(const Quantity& screenMaterialSpacing) {
  return getImpl<detail::Screen_Impl>()->setScreenMaterialSpacing(screenMaterialSpacing);
}

bool Screen::setScreenMaterialDiameter(double screenMaterialDiameter) {
  return getImpl<detail::Screen_Impl>()->setScreenMaterialDiameter(screenMaterialDiameter);
}

bool Screen::setScreenMaterialDiameter(const Quantity& screenMaterialDiameter) {
  return getImpl<detail::Screen_Impl>()->setScreenMaterialDiameter(screenMaterialDiameter);
}

bool Screen::setScreentoGlassDistance(double screentoGlassDistance) {
  return getImpl<detail::Screen_Impl>()->setScreentoGlassDistance(screentoGlassDistance);
}

bool Screen::setScreentoGlassDistance(const Quantity& screentoGlassDistance) {
  return getImpl<detail::Screen_Impl>()->setScreentoGlassDistance(screentoGlassDistance);
}

void Screen::resetScreentoGlassDistance() {
  getImpl<detail::Screen_Impl>()->resetScreentoGlassDistance();
}

bool Screen::setTopOpeningMultiplier(double topOpeningMultiplier) {
  return getImpl<detail::Screen_Impl>()->setTopOpeningMultiplier(topOpeningMultiplier);
}

bool Screen::setTopOpeningMultiplier(const Quantity& topOpeningMultiplier) {
  return getImpl<detail::Screen_Impl>()->setTopOpeningMultiplier(topOpeningMultiplier);
}

void Screen::resetTopOpeningMultiplier() {
  getImpl<detail::Screen_Impl>()->resetTopOpeningMultiplier();
}

bool Screen::setBottomOpeningMultiplier(double bottomOpeningMultiplier) {
  return getImpl<detail::Screen_Impl>()->setBottomOpeningMultiplier(bottomOpeningMultiplier);
}

bool Screen::setBottomOpeningMultiplier(const Quantity& bottomOpeningMultiplier) {
  return getImpl<detail::Screen_Impl>()->setBottomOpeningMultiplier(bottomOpeningMultiplier);
}

void Screen::resetBottomOpeningMultiplier() {
  getImpl<detail::Screen_Impl>()->resetBottomOpeningMultiplier();
}

bool Screen::setLeftSideOpeningMultiplier(double leftSideOpeningMultiplier) {
  return getImpl<detail::Screen_Impl>()->setLeftSideOpeningMultiplier(leftSideOpeningMultiplier);
}

bool Screen::setLeftSideOpeningMultiplier(const Quantity& leftSideOpeningMultiplier) {
  return getImpl<detail::Screen_Impl>()->setLeftSideOpeningMultiplier(leftSideOpeningMultiplier);
}

void Screen::resetLeftSideOpeningMultiplier() {
  getImpl<detail::Screen_Impl>()->resetLeftSideOpeningMultiplier();
}

bool Screen::setRightSideOpeningMultiplier(double rightSideOpeningMultiplier) {
  return getImpl<detail::Screen_Impl>()->setRightSideOpeningMultiplier(rightSideOpeningMultiplier);
}

bool Screen::setRightSideOpeningMultiplier(const Quantity& rightSideOpeningMultiplier) {
  return getImpl<detail::Screen_Impl>()->setRightSideOpeningMultiplier(rightSideOpeningMultiplier);
}

void Screen::resetRightSideOpeningMultiplier() {
  getImpl<detail::Screen_Impl>()->resetRightSideOpeningMultiplier();
}

bool Screen::setAngleofResolutionforScreenTransmittanceOutputMap(std::string angleofResolutionforScreenTransmittanceOutputMap) {
  return getImpl<detail::Screen_Impl>()->setAngleofResolutionforScreenTransmittanceOutputMap(angleofResolutionforScreenTransmittanceOutputMap);
}

void Screen::resetAngleofResolutionforScreenTransmittanceOutputMap() {
  getImpl<detail::Screen_Impl>()->resetAngleofResolutionforScreenTransmittanceOutputMap();
}

/// @cond
Screen::Screen(std::shared_ptr<detail::Screen_Impl> impl)
  : ShadingMaterial(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

