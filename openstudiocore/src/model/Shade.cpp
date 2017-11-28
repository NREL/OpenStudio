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

#include "Shade.hpp"
#include "Shade_Impl.hpp"

#include <utilities/idd/OS_WindowMaterial_Shade_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  Shade_Impl::Shade_Impl(const IdfObject& idfObject,
                         Model_Impl* model,
                         bool keepHandle)
    : ShadingMaterial_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == Shade::iddObjectType());
  }

  Shade_Impl::Shade_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                         Model_Impl* model,
                         bool keepHandle)
    : ShadingMaterial_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == Shade::iddObjectType());
  }

  Shade_Impl::Shade_Impl(const Shade_Impl& other,
                         Model_Impl* model,
                         bool keepHandle)
    : ShadingMaterial_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& Shade_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType Shade_Impl::iddObjectType() const {
    return Shade::iddObjectType();
  }

  Quantity Shade_Impl::getSolarTransmittance(bool returnIP) const {
    OptionalDouble value = solarTransmittance();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_ShadeFields::SolarTransmittance, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  Quantity Shade_Impl::getSolarReflectance(bool returnIP) const {
    OptionalDouble value = solarReflectance();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_ShadeFields::SolarReflectance, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  double Shade_Impl::visibleTransmittance() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_ShadeFields::VisibleTransmittance,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> Shade_Impl::getVisibleTransmittance() const {
    OptionalDouble od = visibleTransmittance();
    return od;
  }

  Quantity Shade_Impl::getVisibleTransmittance(bool returnIP) const {
    OptionalDouble value = visibleTransmittance();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_ShadeFields::VisibleTransmittance, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  Quantity Shade_Impl::getVisibleReflectance(bool returnIP) const {
    OptionalDouble value = visibleReflectance();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_ShadeFields::VisibleReflectance, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  double Shade_Impl::thermalHemisphericalEmissivity() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_ShadeFields::ThermalHemisphericalEmissivity,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity Shade_Impl::getThermalHemisphericalEmissivity(bool returnIP) const {
    OptionalDouble value = thermalHemisphericalEmissivity();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_ShadeFields::ThermalHemisphericalEmissivity, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  Quantity Shade_Impl::getThermalTransmittance(bool returnIP) const {
    OptionalDouble value = thermalTransmittance();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_ShadeFields::ThermalTransmittance, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  double Shade_Impl::thickness() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_ShadeFields::Thickness,true);
    OS_ASSERT(value);
    return value.get();
  }

  double Shade_Impl::thermalConductivity() const {
    OptionalDouble od = getDouble(OS_WindowMaterial_ShadeFields::Conductivity,true);
    if (!od) {
      LOG_AND_THROW("Conductivity not yet set for " << briefDescription() << ".");
    }
    return *od;
  }

  double Shade_Impl::thermalConductance() const {
    return thermalConductivity()/thickness();
  }

  double Shade_Impl::thermalResistivity() const {
    return 1.0/thermalConductivity();
  }

  double Shade_Impl::thermalResistance() const {
    return 1.0/thermalConductance();
  }

  double Shade_Impl::thermalTransmittance() const {
    OptionalDouble od = getDouble(OS_WindowMaterial_ShadeFields::ThermalTransmittance,true);
    if (!od) {
      LOG_AND_THROW("Thermal transmittance not yet set for " << briefDescription() << ".");
    }
    return *od;
  }

  double Shade_Impl::solarTransmittance() const {
    OptionalDouble od = getDouble(OS_WindowMaterial_ShadeFields::SolarTransmittance,true);
    if (!od) {
      LOG_AND_THROW("Solar transmittance not yet set for " << briefDescription() << ".");
    }
    return *od;
  }

  double Shade_Impl::solarAbsorptance() const {
    return 1.0 - solarTransmittance() - solarReflectance();
  }

  double Shade_Impl::solarReflectance() const {
    OptionalDouble od = getDouble(OS_WindowMaterial_ShadeFields::SolarReflectance,true);
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
    OptionalDouble od = getDouble(OS_WindowMaterial_ShadeFields::VisibleReflectance,true);
    if (!od) {
      LOG_AND_THROW("Visible reflectance not yet set for " << briefDescription() << ".");
    }
    return *od;
  }

  bool Shade_Impl::setThermalConductivity(double value) {
    return setDouble(OS_WindowMaterial_ShadeFields::Conductivity,value);
  }

  bool Shade_Impl::setThermalConductance(double value) {
    return setThickness(thermalConductivity()/value);
  }

  bool Shade_Impl::setThermalResistivity(double value) {
    return setThermalConductivity(1.0/value);
  }

  bool Shade_Impl::setThermalResistance(double value) {
    return setThickness(value/thermalResistivity());
  }

  Quantity Shade_Impl::getThickness(bool returnIP) const {
    OptionalDouble value = thickness();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_ShadeFields::Thickness, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  double Shade_Impl::conductivity() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_ShadeFields::Conductivity,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity Shade_Impl::getConductivity(bool returnIP) const {
    OptionalDouble value = conductivity();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_ShadeFields::Conductivity, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  double Shade_Impl::shadetoGlassDistance() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_ShadeFields::ShadetoGlassDistance,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity Shade_Impl::getShadetoGlassDistance(bool returnIP) const {
    OptionalDouble value = shadetoGlassDistance();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_ShadeFields::ShadetoGlassDistance, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool Shade_Impl::isShadetoGlassDistanceDefaulted() const {
    return isEmpty(OS_WindowMaterial_ShadeFields::ShadetoGlassDistance);
  }

  double Shade_Impl::topOpeningMultiplier() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_ShadeFields::TopOpeningMultiplier,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity Shade_Impl::getTopOpeningMultiplier(bool returnIP) const {
    OptionalDouble value = topOpeningMultiplier();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_ShadeFields::TopOpeningMultiplier, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool Shade_Impl::isTopOpeningMultiplierDefaulted() const {
    return isEmpty(OS_WindowMaterial_ShadeFields::TopOpeningMultiplier);
  }

  double Shade_Impl::bottomOpeningMultiplier() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_ShadeFields::BottomOpeningMultiplier,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity Shade_Impl::getBottomOpeningMultiplier(bool returnIP) const {
    OptionalDouble value = bottomOpeningMultiplier();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_ShadeFields::BottomOpeningMultiplier, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool Shade_Impl::isBottomOpeningMultiplierDefaulted() const {
    return isEmpty(OS_WindowMaterial_ShadeFields::BottomOpeningMultiplier);
  }

  double Shade_Impl::leftSideOpeningMultiplier() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_ShadeFields::LeftSideOpeningMultiplier,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity Shade_Impl::getLeftSideOpeningMultiplier(bool returnIP) const {
    OptionalDouble value = leftSideOpeningMultiplier();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_ShadeFields::LeftSideOpeningMultiplier, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool Shade_Impl::isLeftSideOpeningMultiplierDefaulted() const {
    return isEmpty(OS_WindowMaterial_ShadeFields::LeftSideOpeningMultiplier);
  }

  double Shade_Impl::rightSideOpeningMultiplier() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_ShadeFields::RightSideOpeningMultiplier,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity Shade_Impl::getRightSideOpeningMultiplier(bool returnIP) const {
    OptionalDouble value = rightSideOpeningMultiplier();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_ShadeFields::RightSideOpeningMultiplier, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool Shade_Impl::isRightSideOpeningMultiplierDefaulted() const {
    return isEmpty(OS_WindowMaterial_ShadeFields::RightSideOpeningMultiplier);
  }

  double Shade_Impl::airflowPermeability() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_ShadeFields::AirflowPermeability,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity Shade_Impl::getAirflowPermeability(bool returnIP) const {
    OptionalDouble value = airflowPermeability();
    OSOptionalQuantity result = getQuantityFromDouble(OS_WindowMaterial_ShadeFields::AirflowPermeability, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool Shade_Impl::isAirflowPermeabilityDefaulted() const {
    return isEmpty(OS_WindowMaterial_ShadeFields::AirflowPermeability);
  }

  bool Shade_Impl::setSolarTransmittance(double solarTransmittance) {
    bool result = setDouble(OS_WindowMaterial_ShadeFields::SolarTransmittance, solarTransmittance);
    return result;
  }

  bool Shade_Impl::setSolarTransmittance(const Quantity& solarTransmittance) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_ShadeFields::SolarTransmittance,solarTransmittance);
    if (!value) {
      return false;
    }
    return setSolarTransmittance(value.get());
  }

  bool Shade_Impl::setSolarReflectance(double solarReflectance) {
    bool result = setDouble(OS_WindowMaterial_ShadeFields::SolarReflectance, solarReflectance);
    return result;
  }

  bool Shade_Impl::setSolarReflectance(const Quantity& solarReflectance) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_ShadeFields::SolarReflectance,solarReflectance);
    if (!value) {
      return false;
    }
    return setSolarReflectance(value.get());
  }

  bool Shade_Impl::setVisibleTransmittance(double visibleTransmittance) {
    bool result = setDouble(OS_WindowMaterial_ShadeFields::VisibleTransmittance, visibleTransmittance);
    return result;
  }

  bool Shade_Impl::setVisibleTransmittance(const Quantity& visibleTransmittance) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_ShadeFields::VisibleTransmittance,visibleTransmittance);
    if (!value) {
      return false;
    }
    return setVisibleTransmittance(value.get());
  }

  bool Shade_Impl::setVisibleReflectance(double visibleReflectance) {
    bool result = setDouble(OS_WindowMaterial_ShadeFields::VisibleReflectance, visibleReflectance);
    return result;
  }

  bool Shade_Impl::setVisibleReflectance(const Quantity& visibleReflectance) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_ShadeFields::VisibleReflectance,visibleReflectance);
    if (!value) {
      return false;
    }
    return setVisibleReflectance(value.get());
  }

  bool Shade_Impl::setThermalHemisphericalEmissivity(double thermalHemisphericalEmissivity) {
    bool result = setDouble(OS_WindowMaterial_ShadeFields::ThermalHemisphericalEmissivity, thermalHemisphericalEmissivity);
    return result;
  }

  bool Shade_Impl::setThermalHemisphericalEmissivity(const Quantity& thermalHemisphericalEmissivity) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_ShadeFields::ThermalHemisphericalEmissivity,thermalHemisphericalEmissivity);
    if (!value) {
      return false;
    }
    return setThermalHemisphericalEmissivity(value.get());
  }

  bool Shade_Impl::setThermalTransmittance(double thermalTransmittance) {
    bool result = setDouble(OS_WindowMaterial_ShadeFields::ThermalTransmittance, thermalTransmittance);
    return result;
  }

  bool Shade_Impl::setThermalTransmittance(const Quantity& thermalTransmittance) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_ShadeFields::ThermalTransmittance,thermalTransmittance);
    if (!value) {
      return false;
    }
    return setThermalTransmittance(value.get());
  }

  bool Shade_Impl::setThickness(double thickness) {
    bool result = setDouble(OS_WindowMaterial_ShadeFields::Thickness, thickness);
    return result;
  }

  bool Shade_Impl::setThickness(const Quantity& thickness) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_ShadeFields::Thickness,thickness);
    if (!value) {
      return false;
    }
    return setThickness(value.get());
  }

  bool Shade_Impl::setConductivity(double conductivity) {
    bool result = setDouble(OS_WindowMaterial_ShadeFields::Conductivity, conductivity);
    return result;
  }

  bool Shade_Impl::setConductivity(const Quantity& conductivity) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_ShadeFields::Conductivity,conductivity);
    if (!value) {
      return false;
    }
    return setConductivity(value.get());
  }

  bool Shade_Impl::setShadetoGlassDistance(double shadetoGlassDistance) {
    bool result = setDouble(OS_WindowMaterial_ShadeFields::ShadetoGlassDistance, shadetoGlassDistance);
    return result;
  }

  bool Shade_Impl::setShadetoGlassDistance(const Quantity& shadetoGlassDistance) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_ShadeFields::ShadetoGlassDistance,shadetoGlassDistance);
    if (!value) {
      return false;
    }
    return setShadetoGlassDistance(value.get());
  }

  void Shade_Impl::resetShadetoGlassDistance() {
    bool result = setString(OS_WindowMaterial_ShadeFields::ShadetoGlassDistance, "");
    OS_ASSERT(result);
  }

  bool Shade_Impl::setTopOpeningMultiplier(double topOpeningMultiplier) {
    bool result = setDouble(OS_WindowMaterial_ShadeFields::TopOpeningMultiplier, topOpeningMultiplier);
    return result;
  }

  bool Shade_Impl::setTopOpeningMultiplier(const Quantity& topOpeningMultiplier) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_ShadeFields::TopOpeningMultiplier,topOpeningMultiplier);
    if (!value) {
      return false;
    }
    return setTopOpeningMultiplier(value.get());
  }

  void Shade_Impl::resetTopOpeningMultiplier() {
    bool result = setString(OS_WindowMaterial_ShadeFields::TopOpeningMultiplier, "");
    OS_ASSERT(result);
  }

  bool Shade_Impl::setBottomOpeningMultiplier(double bottomOpeningMultiplier) {
    bool result = setDouble(OS_WindowMaterial_ShadeFields::BottomOpeningMultiplier, bottomOpeningMultiplier);
    return result;
  }

  bool Shade_Impl::setBottomOpeningMultiplier(const Quantity& bottomOpeningMultiplier) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_ShadeFields::BottomOpeningMultiplier,bottomOpeningMultiplier);
    if (!value) {
      return false;
    }
    return setBottomOpeningMultiplier(value.get());
  }

  void Shade_Impl::resetBottomOpeningMultiplier() {
    bool result = setString(OS_WindowMaterial_ShadeFields::BottomOpeningMultiplier, "");
    OS_ASSERT(result);
  }

  bool Shade_Impl::setLeftSideOpeningMultiplier(double leftSideOpeningMultiplier) {
    bool result = setDouble(OS_WindowMaterial_ShadeFields::LeftSideOpeningMultiplier, leftSideOpeningMultiplier);
    return result;
  }

  bool Shade_Impl::setLeftSideOpeningMultiplier(const Quantity& leftSideOpeningMultiplier) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_ShadeFields::LeftSideOpeningMultiplier,leftSideOpeningMultiplier);
    if (!value) {
      return false;
    }
    return setLeftSideOpeningMultiplier(value.get());
  }

  void Shade_Impl::resetLeftSideOpeningMultiplier() {
    bool result = setString(OS_WindowMaterial_ShadeFields::LeftSideOpeningMultiplier, "");
    OS_ASSERT(result);
  }

  bool Shade_Impl::setRightSideOpeningMultiplier(double rightSideOpeningMultiplier) {
    bool result = setDouble(OS_WindowMaterial_ShadeFields::RightSideOpeningMultiplier, rightSideOpeningMultiplier);
    return result;
  }

  bool Shade_Impl::setRightSideOpeningMultiplier(const Quantity& rightSideOpeningMultiplier) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_ShadeFields::RightSideOpeningMultiplier,rightSideOpeningMultiplier);
    if (!value) {
      return false;
    }
    return setRightSideOpeningMultiplier(value.get());
  }

  void Shade_Impl::resetRightSideOpeningMultiplier() {
    bool result = setString(OS_WindowMaterial_ShadeFields::RightSideOpeningMultiplier, "");
    OS_ASSERT(result);
  }

  bool Shade_Impl::setAirflowPermeability(double airflowPermeability) {
    bool result = setDouble(OS_WindowMaterial_ShadeFields::AirflowPermeability, airflowPermeability);
    return result;
  }

  bool Shade_Impl::setAirflowPermeability(const Quantity& airflowPermeability) {
    OptionalDouble value = getDoubleFromQuantity(OS_WindowMaterial_ShadeFields::AirflowPermeability,airflowPermeability);
    if (!value) {
      return false;
    }
    return setAirflowPermeability(value.get());
  }

  void Shade_Impl::resetAirflowPermeability() {
    bool result = setString(OS_WindowMaterial_ShadeFields::AirflowPermeability, "");
    OS_ASSERT(result);
  }

  openstudio::Quantity Shade_Impl::solarTransmittance_SI() const {
    return getSolarTransmittance(false);
  }

  openstudio::Quantity Shade_Impl::solarTransmittance_IP() const {
    return getSolarTransmittance(true);
  }

  openstudio::Quantity Shade_Impl::solarReflectance_SI() const {
    return getSolarReflectance(false);
  }

  openstudio::Quantity Shade_Impl::solarReflectance_IP() const {
    return getSolarReflectance(true);
  }

  openstudio::Quantity Shade_Impl::visibleTransmittance_SI() const {
    return getVisibleTransmittance(false);
  }

  openstudio::Quantity Shade_Impl::visibleTransmittance_IP() const {
    return getVisibleTransmittance(true);
  }

  openstudio::Quantity Shade_Impl::visibleReflectance_SI() const {
    return getVisibleReflectance(false);
  }

  openstudio::Quantity Shade_Impl::visibleReflectance_IP() const {
    return getVisibleReflectance(true);
  }

  openstudio::Quantity Shade_Impl::thermalHemisphericalEmissivity_SI() const {
    return getThermalHemisphericalEmissivity(false);
  }

  openstudio::Quantity Shade_Impl::thermalHemisphericalEmissivity_IP() const {
    return getThermalHemisphericalEmissivity(true);
  }

  openstudio::Quantity Shade_Impl::thermalTransmittance_SI() const {
    return getThermalTransmittance(false);
  }

  openstudio::Quantity Shade_Impl::thermalTransmittance_IP() const {
    return getThermalTransmittance(true);
  }

  openstudio::Quantity Shade_Impl::thickness_SI() const {
    return getThickness(false);
  }

  openstudio::Quantity Shade_Impl::thickness_IP() const {
    return getThickness(true);
  }

  openstudio::Quantity Shade_Impl::conductivity_SI() const {
    return getConductivity(false);
  }

  openstudio::Quantity Shade_Impl::conductivity_IP() const {
    return getConductivity(true);
  }

  openstudio::Quantity Shade_Impl::shadetoGlassDistance_SI() const {
    return getShadetoGlassDistance(false);
  }

  openstudio::Quantity Shade_Impl::shadetoGlassDistance_IP() const {
    return getShadetoGlassDistance(true);
  }

  openstudio::Quantity Shade_Impl::topOpeningMultiplier_SI() const {
    return getTopOpeningMultiplier(false);
  }

  openstudio::Quantity Shade_Impl::topOpeningMultiplier_IP() const {
    return getTopOpeningMultiplier(true);
  }

  openstudio::Quantity Shade_Impl::bottomOpeningMultiplier_SI() const {
    return getBottomOpeningMultiplier(false);
  }

  openstudio::Quantity Shade_Impl::bottomOpeningMultiplier_IP() const {
    return getBottomOpeningMultiplier(true);
  }

  openstudio::Quantity Shade_Impl::leftSideOpeningMultiplier_SI() const {
    return getLeftSideOpeningMultiplier(false);
  }

  openstudio::Quantity Shade_Impl::leftSideOpeningMultiplier_IP() const {
    return getLeftSideOpeningMultiplier(true);
  }

  openstudio::Quantity Shade_Impl::rightSideOpeningMultiplier_SI() const {
    return getRightSideOpeningMultiplier(false);
  }

  openstudio::Quantity Shade_Impl::rightSideOpeningMultiplier_IP() const {
    return getRightSideOpeningMultiplier(true);
  }

  openstudio::Quantity Shade_Impl::airflowPermeability_SI() const {
    return getAirflowPermeability(false);
  }

  openstudio::Quantity Shade_Impl::airflowPermeability_IP() const {
    return getAirflowPermeability(true);
  }

} // detail

Shade::Shade(const Model& model,
  double solarTransmittance,
  double solarReflectance,
  double visibleTransmittance,
  double visibleReflectance,
  double thermalHemisphericalEmissivity,
  double thermalTransmittance,
  double thickness,
  double conductivity)
  : ShadingMaterial(Shade::iddObjectType(),model)
{
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
  return IddObjectType(IddObjectType::OS_WindowMaterial_Shade);
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

Quantity Shade::getThermalTransmittance(bool returnIP) const {
  return getImpl<detail::Shade_Impl>()->getThermalTransmittance(returnIP);
}
Quantity Shade::getSolarTransmittance(bool returnIP) const {
  return getImpl<detail::Shade_Impl>()->getSolarTransmittance(returnIP);
}

Quantity Shade::getSolarReflectance(bool returnIP) const {
  return getImpl<detail::Shade_Impl>()->getSolarReflectance(returnIP);
}

double Shade::visibleTransmittance() const {
  return getImpl<detail::Shade_Impl>()->visibleTransmittance();
}

Quantity Shade::getVisibleTransmittance(bool returnIP) const {
  return getImpl<detail::Shade_Impl>()->getVisibleTransmittance(returnIP);
}

Quantity Shade::getVisibleReflectance(bool returnIP) const {
  return getImpl<detail::Shade_Impl>()->getVisibleReflectance(returnIP);
}

double Shade::thermalHemisphericalEmissivity() const {
  return getImpl<detail::Shade_Impl>()->thermalHemisphericalEmissivity();
}

Quantity Shade::getThermalHemisphericalEmissivity(bool returnIP) const {
  return getImpl<detail::Shade_Impl>()->getThermalHemisphericalEmissivity(returnIP);
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

bool Shade::setThermalTransmittance(double value) {
  return getImpl<detail::Shade_Impl>()->setThermalTransmittance(value);
}

double Shade::thickness() const {
  return getImpl<detail::Shade_Impl>()->thickness();
}

Quantity Shade::getThickness(bool returnIP) const {
  return getImpl<detail::Shade_Impl>()->getThickness(returnIP);
}

double Shade::conductivity() const {
  return getImpl<detail::Shade_Impl>()->conductivity();
}

Quantity Shade::getConductivity(bool returnIP) const {
  return getImpl<detail::Shade_Impl>()->getConductivity(returnIP);
}

double Shade::shadetoGlassDistance() const {
  return getImpl<detail::Shade_Impl>()->shadetoGlassDistance();
}

Quantity Shade::getShadetoGlassDistance(bool returnIP) const {
  return getImpl<detail::Shade_Impl>()->getShadetoGlassDistance(returnIP);
}

bool Shade::isShadetoGlassDistanceDefaulted() const {
  return getImpl<detail::Shade_Impl>()->isShadetoGlassDistanceDefaulted();
}

double Shade::topOpeningMultiplier() const {
  return getImpl<detail::Shade_Impl>()->topOpeningMultiplier();
}

Quantity Shade::getTopOpeningMultiplier(bool returnIP) const {
  return getImpl<detail::Shade_Impl>()->getTopOpeningMultiplier(returnIP);
}

bool Shade::isTopOpeningMultiplierDefaulted() const {
  return getImpl<detail::Shade_Impl>()->isTopOpeningMultiplierDefaulted();
}

double Shade::bottomOpeningMultiplier() const {
  return getImpl<detail::Shade_Impl>()->bottomOpeningMultiplier();
}

Quantity Shade::getBottomOpeningMultiplier(bool returnIP) const {
  return getImpl<detail::Shade_Impl>()->getBottomOpeningMultiplier(returnIP);
}

bool Shade::isBottomOpeningMultiplierDefaulted() const {
  return getImpl<detail::Shade_Impl>()->isBottomOpeningMultiplierDefaulted();
}

double Shade::leftSideOpeningMultiplier() const {
  return getImpl<detail::Shade_Impl>()->leftSideOpeningMultiplier();
}

Quantity Shade::getLeftSideOpeningMultiplier(bool returnIP) const {
  return getImpl<detail::Shade_Impl>()->getLeftSideOpeningMultiplier(returnIP);
}

bool Shade::isLeftSideOpeningMultiplierDefaulted() const {
  return getImpl<detail::Shade_Impl>()->isLeftSideOpeningMultiplierDefaulted();
}

double Shade::rightSideOpeningMultiplier() const {
  return getImpl<detail::Shade_Impl>()->rightSideOpeningMultiplier();
}

Quantity Shade::getRightSideOpeningMultiplier(bool returnIP) const {
  return getImpl<detail::Shade_Impl>()->getRightSideOpeningMultiplier(returnIP);
}

bool Shade::isRightSideOpeningMultiplierDefaulted() const {
  return getImpl<detail::Shade_Impl>()->isRightSideOpeningMultiplierDefaulted();
}

double Shade::airflowPermeability() const {
  return getImpl<detail::Shade_Impl>()->airflowPermeability();
}

Quantity Shade::getAirflowPermeability(bool returnIP) const {
  return getImpl<detail::Shade_Impl>()->getAirflowPermeability(returnIP);
}

bool Shade::isAirflowPermeabilityDefaulted() const {
  return getImpl<detail::Shade_Impl>()->isAirflowPermeabilityDefaulted();
}

bool Shade::setSolarTransmittance(double solarTransmittance) {
  return getImpl<detail::Shade_Impl>()->setSolarTransmittance(solarTransmittance);
}

bool Shade::setSolarTransmittance(const Quantity& solarTransmittance) {
  return getImpl<detail::Shade_Impl>()->setSolarTransmittance(solarTransmittance);
}

bool Shade::setSolarReflectance(double solarReflectance) {
  return getImpl<detail::Shade_Impl>()->setSolarReflectance(solarReflectance);
}

bool Shade::setSolarReflectance(const Quantity& solarReflectance) {
  return getImpl<detail::Shade_Impl>()->setSolarReflectance(solarReflectance);
}

bool Shade::setVisibleTransmittance(double visibleTransmittance) {
  return getImpl<detail::Shade_Impl>()->setVisibleTransmittance(visibleTransmittance);
}

bool Shade::setVisibleTransmittance(const Quantity& visibleTransmittance) {
  return getImpl<detail::Shade_Impl>()->setVisibleTransmittance(visibleTransmittance);
}

bool Shade::setVisibleReflectance(double visibleReflectance) {
  return getImpl<detail::Shade_Impl>()->setVisibleReflectance(visibleReflectance);
}

bool Shade::setVisibleReflectance(const Quantity& visibleReflectance) {
  return getImpl<detail::Shade_Impl>()->setVisibleReflectance(visibleReflectance);
}

bool Shade::setThermalHemisphericalEmissivity(double thermalHemisphericalEmissivity) {
  return getImpl<detail::Shade_Impl>()->setThermalHemisphericalEmissivity(thermalHemisphericalEmissivity);
}

bool Shade::setThermalHemisphericalEmissivity(const Quantity& thermalHemisphericalEmissivity) {
  return getImpl<detail::Shade_Impl>()->setThermalHemisphericalEmissivity(thermalHemisphericalEmissivity);
}

bool Shade::setThermalTransmittance(const Quantity& thermalTransmittance) {
  return getImpl<detail::Shade_Impl>()->setThermalTransmittance(thermalTransmittance);
}

bool Shade::setThickness(double thickness) {
  return getImpl<detail::Shade_Impl>()->setThickness(thickness);
}

bool Shade::setThickness(const Quantity& thickness) {
  return getImpl<detail::Shade_Impl>()->setThickness(thickness);
}

bool Shade::setConductivity(double conductivity) {
  return getImpl<detail::Shade_Impl>()->setConductivity(conductivity);
}

bool Shade::setConductivity(const Quantity& conductivity) {
  return getImpl<detail::Shade_Impl>()->setConductivity(conductivity);
}

bool Shade::setShadetoGlassDistance(double shadetoGlassDistance) {
  return getImpl<detail::Shade_Impl>()->setShadetoGlassDistance(shadetoGlassDistance);
}

bool Shade::setShadetoGlassDistance(const Quantity& shadetoGlassDistance) {
  return getImpl<detail::Shade_Impl>()->setShadetoGlassDistance(shadetoGlassDistance);
}

void Shade::resetShadetoGlassDistance() {
  getImpl<detail::Shade_Impl>()->resetShadetoGlassDistance();
}

bool Shade::setTopOpeningMultiplier(double topOpeningMultiplier) {
  return getImpl<detail::Shade_Impl>()->setTopOpeningMultiplier(topOpeningMultiplier);
}

bool Shade::setTopOpeningMultiplier(const Quantity& topOpeningMultiplier) {
  return getImpl<detail::Shade_Impl>()->setTopOpeningMultiplier(topOpeningMultiplier);
}

void Shade::resetTopOpeningMultiplier() {
  getImpl<detail::Shade_Impl>()->resetTopOpeningMultiplier();
}

bool Shade::setBottomOpeningMultiplier(double bottomOpeningMultiplier) {
  return getImpl<detail::Shade_Impl>()->setBottomOpeningMultiplier(bottomOpeningMultiplier);
}

bool Shade::setBottomOpeningMultiplier(const Quantity& bottomOpeningMultiplier) {
  return getImpl<detail::Shade_Impl>()->setBottomOpeningMultiplier(bottomOpeningMultiplier);
}

void Shade::resetBottomOpeningMultiplier() {
  getImpl<detail::Shade_Impl>()->resetBottomOpeningMultiplier();
}

bool Shade::setLeftSideOpeningMultiplier(double leftSideOpeningMultiplier) {
  return getImpl<detail::Shade_Impl>()->setLeftSideOpeningMultiplier(leftSideOpeningMultiplier);
}

bool Shade::setLeftSideOpeningMultiplier(const Quantity& leftSideOpeningMultiplier) {
  return getImpl<detail::Shade_Impl>()->setLeftSideOpeningMultiplier(leftSideOpeningMultiplier);
}

void Shade::resetLeftSideOpeningMultiplier() {
  getImpl<detail::Shade_Impl>()->resetLeftSideOpeningMultiplier();
}

bool Shade::setRightSideOpeningMultiplier(double rightSideOpeningMultiplier) {
  return getImpl<detail::Shade_Impl>()->setRightSideOpeningMultiplier(rightSideOpeningMultiplier);
}

bool Shade::setRightSideOpeningMultiplier(const Quantity& rightSideOpeningMultiplier) {
  return getImpl<detail::Shade_Impl>()->setRightSideOpeningMultiplier(rightSideOpeningMultiplier);
}

void Shade::resetRightSideOpeningMultiplier() {
  getImpl<detail::Shade_Impl>()->resetRightSideOpeningMultiplier();
}

bool Shade::setAirflowPermeability(double airflowPermeability) {
  return getImpl<detail::Shade_Impl>()->setAirflowPermeability(airflowPermeability);
}

bool Shade::setAirflowPermeability(const Quantity& airflowPermeability) {
  return getImpl<detail::Shade_Impl>()->setAirflowPermeability(airflowPermeability);
}

void Shade::resetAirflowPermeability() {
  getImpl<detail::Shade_Impl>()->resetAirflowPermeability();
}

/// @cond
Shade::Shade(std::shared_ptr<detail::Shade_Impl> impl)
  : ShadingMaterial(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

