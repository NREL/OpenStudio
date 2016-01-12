/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#include "AirGap.hpp"
#include "AirGap_Impl.hpp"

#include <utilities/idd/OS_Material_AirGap_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>


#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  AirGap_Impl::AirGap_Impl(const IdfObject& idfObject,
                           Model_Impl* model,
                           bool keepHandle)
    : OpaqueMaterial_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AirGap::iddObjectType());
  }

  AirGap_Impl::AirGap_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                           Model_Impl* model,
                           bool keepHandle)
    : OpaqueMaterial_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AirGap::iddObjectType());
  }

  AirGap_Impl::AirGap_Impl(const AirGap_Impl& other,
                           Model_Impl* model,
                           bool keepHandle)
    : OpaqueMaterial_Impl(other,model,keepHandle)
  {}

  double AirGap_Impl::thickness() const {
    return 0.0;
  }

  double AirGap_Impl::thermalConductivity() const {
    LOG_AND_THROW("Unable to convert thermal resistance to thermal conductivity for AirGap "
      << briefDescription() << ".");
    return 0.0;
  }

  double AirGap_Impl::thermalConductance() const {
    OS_ASSERT(thermalResistance());
    return 1.0/thermalResistance();
  }

  double AirGap_Impl::thermalResistivity() const {
    LOG_AND_THROW("Unable to convert thermal resistance to thermal resistivity for AirGap "
      << briefDescription() << ".");
    return 0.0;
  }

  double AirGap_Impl::thermalResistance() const {
    OptionalDouble od = getDouble(OS_Material_AirGapFields::ThermalResistance,true);
    if (!od) {
      LOG_AND_THROW("Thermal resistance is not set for AirGap " << briefDescription() << ".");
    }
    return *od;
  }

  double AirGap_Impl::thermalAbsorptance() const {
    OptionalDouble od(0.0);
    return *od;
  }

  OptionalDouble AirGap_Impl::thermalReflectance() const {
    OptionalDouble od(0.0);
    return od;
  }

  double AirGap_Impl::solarAbsorptance() const {
    OptionalDouble od(0.0);
    return *od;
  }

  OptionalDouble AirGap_Impl::solarReflectance() const {
    OptionalDouble od(0.0);
    return od;
  }

  double AirGap_Impl::visibleTransmittance() const {
    return 1.0;
  }

  double AirGap_Impl::visibleAbsorptance() const {
    OptionalDouble od(0.0);
    return *od;
  }

  OptionalDouble AirGap_Impl::visibleReflectance() const {
    OptionalDouble od(0.0);
    return od;
  }

  const std::vector<std::string>& AirGap_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType AirGap_Impl::iddObjectType() const {
    return AirGap::iddObjectType();
  }

  bool AirGap_Impl::setThickness(double value) {
    return false;
  }

  bool AirGap_Impl::setThermalConductivity(double value) {
    return false;
  }

  bool AirGap_Impl::setThermalConductance(double value) {
    return setThermalResistance(1.0/value);
  }

  bool AirGap_Impl::setThermalResistivity(double value) {
    return false;
  }

  bool AirGap_Impl::setThermalResistance(double value) {
    return setDouble(OS_Material_AirGapFields::ThermalResistance,value);
  }

  bool AirGap_Impl::setThermalAbsorptance(double value) {
    return false;
  }

  bool AirGap_Impl::setThermalReflectance(OptionalDouble value) {
    return false;
  }

  bool AirGap_Impl::setSolarAbsorptance(OptionalDouble value) {
    return false;
  }

  bool AirGap_Impl::setSolarReflectance(OptionalDouble value) {
    return false;
  }

  bool AirGap_Impl::setVisibleAbsorptance(OptionalDouble value) {
    return false;
  }

  bool AirGap_Impl::setVisibleReflectance(OptionalDouble value) {
    return false;
  }

  OSOptionalQuantity AirGap_Impl::getThermalResistance(bool returnIP) const {
    double value = thermalResistance();
    return getQuantityFromDouble(OS_Material_AirGapFields::ThermalResistance, value, returnIP);
  }

  bool AirGap_Impl::setThermalResistance(boost::optional<double> thermalResistance) {
    bool result(false);
    if (thermalResistance) {
      result = setDouble(OS_Material_AirGapFields::ThermalResistance, thermalResistance.get());
    }
    else {
      resetThermalResistance();
      result = true;
    }
    return result;
  }

  bool AirGap_Impl::setThermalResistance(const OSOptionalQuantity& thermalResistance) {
    bool result(false);
    OptionalDouble value;
    if (thermalResistance.isSet()) {
      value = getDoubleFromQuantity(OS_Material_AirGapFields::ThermalResistance,thermalResistance.get());
      if (value) {
        result = setThermalResistance(value);
      }
    }
    else {
      result = setThermalResistance(value);
    }
    return result;
  }

  void AirGap_Impl::resetThermalResistance() {
    bool result = setString(OS_Material_AirGapFields::ThermalResistance, "");
    OS_ASSERT(result);
  }

  openstudio::OSOptionalQuantity AirGap_Impl::thermalResistance_SI() const {
    return getThermalResistance(false);
  }

  openstudio::OSOptionalQuantity AirGap_Impl::thermalResistance_IP() const {
    return getThermalResistance(true);
  }

} // detail

AirGap::AirGap(const Model& model,
               double thermalResistance)
  : OpaqueMaterial(AirGap::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AirGap_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
  ok = setThermalResistance(thermalResistance);
  OS_ASSERT(ok);
}

IddObjectType AirGap::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Material_AirGap);
}

double AirGap::thermalResistance() const {
  return getImpl<detail::AirGap_Impl>()->thermalResistance();
}

OSOptionalQuantity AirGap::getThermalResistance(bool returnIP) const {
  return getImpl<detail::AirGap_Impl>()->getThermalResistance(returnIP);
}

bool AirGap::setThermalResistance(double thermalResistance) {
  return getImpl<detail::AirGap_Impl>()->setThermalResistance(thermalResistance);
}

bool AirGap::setThermalResistance(const Quantity& thermalResistance) {
  return getImpl<detail::AirGap_Impl>()->setThermalResistance(thermalResistance);
}

void AirGap::resetThermalResistance() {
  getImpl<detail::AirGap_Impl>()->resetThermalResistance();
}

/// @cond
AirGap::AirGap(std::shared_ptr<detail::AirGap_Impl> impl)
  : OpaqueMaterial(impl)
{}
/// @endcond

} // model
} // openstudio

