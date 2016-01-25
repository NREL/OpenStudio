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

#include "ZoneCapacitanceMultiplierResearchSpecial.hpp"
#include "ZoneCapacitanceMultiplierResearchSpecial_Impl.hpp"
#include "SimulationControl.hpp"
#include "SimulationControl_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/OS_ZoneCapacitanceMultiplier_ResearchSpecial_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ZoneCapacitanceMultiplierResearchSpecial_Impl::ZoneCapacitanceMultiplierResearchSpecial_Impl(const IdfObject& idfObject,
                                                                                               Model_Impl* model,
                                                                                               bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ZoneCapacitanceMultiplierResearchSpecial::iddObjectType());
  }

  ZoneCapacitanceMultiplierResearchSpecial_Impl::ZoneCapacitanceMultiplierResearchSpecial_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                               Model_Impl* model,
                                                                                               bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ZoneCapacitanceMultiplierResearchSpecial::iddObjectType());
  }

  ZoneCapacitanceMultiplierResearchSpecial_Impl::ZoneCapacitanceMultiplierResearchSpecial_Impl(const ZoneCapacitanceMultiplierResearchSpecial_Impl& other,
                                                                                               Model_Impl* model,
                                                                                               bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ZoneCapacitanceMultiplierResearchSpecial_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ZoneCapacitanceMultiplierResearchSpecial_Impl::iddObjectType() const {
    return ZoneCapacitanceMultiplierResearchSpecial::iddObjectType();
  }

  // return the parent object in the hierarchy
  boost::optional<ParentObject> ZoneCapacitanceMultiplierResearchSpecial_Impl::parent() const
  {
    boost::optional<ParentObject> result(model().getUniqueModelObject<SimulationControl>());
    return result;
  }

  // set the parent, child may have to call methods on the parent
  bool ZoneCapacitanceMultiplierResearchSpecial_Impl::setParent(ParentObject& newParent)
  {
    if (newParent.optionalCast<SimulationControl>() && (newParent.model() == model())) {
      return true;
    }
    return false;
  }

  double ZoneCapacitanceMultiplierResearchSpecial_Impl::temperatureCapacityMultiplier() const {
    boost::optional<double> value = getDouble(OS_ZoneCapacitanceMultiplier_ResearchSpecialFields::TemperatureCapacityMultiplier,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity ZoneCapacitanceMultiplierResearchSpecial_Impl::getTemperatureCapacityMultiplier(bool returnIP) const {
    OptionalDouble value = temperatureCapacityMultiplier();
    OSOptionalQuantity result = getQuantityFromDouble(OS_ZoneCapacitanceMultiplier_ResearchSpecialFields::TemperatureCapacityMultiplier, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool ZoneCapacitanceMultiplierResearchSpecial_Impl::isTemperatureCapacityMultiplierDefaulted() const {
    return isEmpty(OS_ZoneCapacitanceMultiplier_ResearchSpecialFields::TemperatureCapacityMultiplier);
  }

  double ZoneCapacitanceMultiplierResearchSpecial_Impl::humidityCapacityMultiplier() const {
    boost::optional<double> value = getDouble(OS_ZoneCapacitanceMultiplier_ResearchSpecialFields::HumidityCapacityMultiplier,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity ZoneCapacitanceMultiplierResearchSpecial_Impl::getHumidityCapacityMultiplier(bool returnIP) const {
    OptionalDouble value = humidityCapacityMultiplier();
    OSOptionalQuantity result = getQuantityFromDouble(OS_ZoneCapacitanceMultiplier_ResearchSpecialFields::HumidityCapacityMultiplier, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool ZoneCapacitanceMultiplierResearchSpecial_Impl::isHumidityCapacityMultiplierDefaulted() const {
    return isEmpty(OS_ZoneCapacitanceMultiplier_ResearchSpecialFields::HumidityCapacityMultiplier);
  }

  double ZoneCapacitanceMultiplierResearchSpecial_Impl::carbonDioxideCapacityMultiplier() const {
    boost::optional<double> value = getDouble(OS_ZoneCapacitanceMultiplier_ResearchSpecialFields::CarbonDioxideCapacityMultiplier,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity ZoneCapacitanceMultiplierResearchSpecial_Impl::getCarbonDioxideCapacityMultiplier(bool returnIP) const {
    OptionalDouble value = carbonDioxideCapacityMultiplier();
    OSOptionalQuantity result = getQuantityFromDouble(OS_ZoneCapacitanceMultiplier_ResearchSpecialFields::CarbonDioxideCapacityMultiplier, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool ZoneCapacitanceMultiplierResearchSpecial_Impl::isCarbonDioxideCapacityMultiplierDefaulted() const {
    return isEmpty(OS_ZoneCapacitanceMultiplier_ResearchSpecialFields::CarbonDioxideCapacityMultiplier);
  }

  bool ZoneCapacitanceMultiplierResearchSpecial_Impl::setTemperatureCapacityMultiplier(double temperatureCapacityMultiplier) {
    bool result = setDouble(OS_ZoneCapacitanceMultiplier_ResearchSpecialFields::TemperatureCapacityMultiplier, temperatureCapacityMultiplier);
    return result;
  }

  bool ZoneCapacitanceMultiplierResearchSpecial_Impl::setTemperatureCapacityMultiplier(const Quantity& temperatureCapacityMultiplier) {
    OptionalDouble value = getDoubleFromQuantity(OS_ZoneCapacitanceMultiplier_ResearchSpecialFields::TemperatureCapacityMultiplier,temperatureCapacityMultiplier);
    if (!value) {
      return false;
    }
    return setTemperatureCapacityMultiplier(value.get());
  }

  void ZoneCapacitanceMultiplierResearchSpecial_Impl::resetTemperatureCapacityMultiplier() {
    bool result = setString(OS_ZoneCapacitanceMultiplier_ResearchSpecialFields::TemperatureCapacityMultiplier, "");
    OS_ASSERT(result);
  }

  bool ZoneCapacitanceMultiplierResearchSpecial_Impl::setHumidityCapacityMultiplier(double humidityCapacityMultiplier) {
    bool result = setDouble(OS_ZoneCapacitanceMultiplier_ResearchSpecialFields::HumidityCapacityMultiplier, humidityCapacityMultiplier);
    return result;
  }

  bool ZoneCapacitanceMultiplierResearchSpecial_Impl::setHumidityCapacityMultiplier(const Quantity& humidityCapacityMultiplier) {
    OptionalDouble value = getDoubleFromQuantity(OS_ZoneCapacitanceMultiplier_ResearchSpecialFields::HumidityCapacityMultiplier,humidityCapacityMultiplier);
    if (!value) {
      return false;
    }
    return setHumidityCapacityMultiplier(value.get());
  }

  void ZoneCapacitanceMultiplierResearchSpecial_Impl::resetHumidityCapacityMultiplier() {
    bool result = setString(OS_ZoneCapacitanceMultiplier_ResearchSpecialFields::HumidityCapacityMultiplier, "");
    OS_ASSERT(result);
  }

  bool ZoneCapacitanceMultiplierResearchSpecial_Impl::setCarbonDioxideCapacityMultiplier(double carbonDioxideCapacityMultiplier) {
    bool result = setDouble(OS_ZoneCapacitanceMultiplier_ResearchSpecialFields::CarbonDioxideCapacityMultiplier, carbonDioxideCapacityMultiplier);
    return result;
  }

  bool ZoneCapacitanceMultiplierResearchSpecial_Impl::setCarbonDioxideCapacityMultiplier(const Quantity& carbonDioxideCapacityMultiplier) {
    OptionalDouble value = getDoubleFromQuantity(OS_ZoneCapacitanceMultiplier_ResearchSpecialFields::CarbonDioxideCapacityMultiplier,carbonDioxideCapacityMultiplier);
    if (!value) {
      return false;
    }
    return setCarbonDioxideCapacityMultiplier(value.get());
  }

  void ZoneCapacitanceMultiplierResearchSpecial_Impl::resetCarbonDioxideCapacityMultiplier() {
    bool result = setString(OS_ZoneCapacitanceMultiplier_ResearchSpecialFields::CarbonDioxideCapacityMultiplier, "");
    OS_ASSERT(result);
  }

  openstudio::Quantity ZoneCapacitanceMultiplierResearchSpecial_Impl::temperatureCapacityMultiplier_SI() const {
    return getTemperatureCapacityMultiplier(false);
  }

  openstudio::Quantity ZoneCapacitanceMultiplierResearchSpecial_Impl::temperatureCapacityMultiplier_IP() const {
    return getTemperatureCapacityMultiplier(true);
  }

  openstudio::Quantity ZoneCapacitanceMultiplierResearchSpecial_Impl::humidityCapacityMultiplier_SI() const {
    return getHumidityCapacityMultiplier(false);
  }

  openstudio::Quantity ZoneCapacitanceMultiplierResearchSpecial_Impl::humidityCapacityMultiplier_IP() const {
    return getHumidityCapacityMultiplier(true);
  }

  openstudio::Quantity ZoneCapacitanceMultiplierResearchSpecial_Impl::carbonDioxideCapacityMultiplier_SI() const {
    return getCarbonDioxideCapacityMultiplier(false);
  }

  openstudio::Quantity ZoneCapacitanceMultiplierResearchSpecial_Impl::carbonDioxideCapacityMultiplier_IP() const {
    return getCarbonDioxideCapacityMultiplier(true);
  }

} // detail

IddObjectType ZoneCapacitanceMultiplierResearchSpecial::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ZoneCapacitanceMultiplier_ResearchSpecial);
}

double ZoneCapacitanceMultiplierResearchSpecial::temperatureCapacityMultiplier() const {
  return getImpl<detail::ZoneCapacitanceMultiplierResearchSpecial_Impl>()->temperatureCapacityMultiplier();
}

Quantity ZoneCapacitanceMultiplierResearchSpecial::getTemperatureCapacityMultiplier(bool returnIP) const {
  return getImpl<detail::ZoneCapacitanceMultiplierResearchSpecial_Impl>()->getTemperatureCapacityMultiplier(returnIP);
}

bool ZoneCapacitanceMultiplierResearchSpecial::isTemperatureCapacityMultiplierDefaulted() const {
  return getImpl<detail::ZoneCapacitanceMultiplierResearchSpecial_Impl>()->isTemperatureCapacityMultiplierDefaulted();
}

double ZoneCapacitanceMultiplierResearchSpecial::humidityCapacityMultiplier() const {
  return getImpl<detail::ZoneCapacitanceMultiplierResearchSpecial_Impl>()->humidityCapacityMultiplier();
}

Quantity ZoneCapacitanceMultiplierResearchSpecial::getHumidityCapacityMultiplier(bool returnIP) const {
  return getImpl<detail::ZoneCapacitanceMultiplierResearchSpecial_Impl>()->getHumidityCapacityMultiplier(returnIP);
}

bool ZoneCapacitanceMultiplierResearchSpecial::isHumidityCapacityMultiplierDefaulted() const {
  return getImpl<detail::ZoneCapacitanceMultiplierResearchSpecial_Impl>()->isHumidityCapacityMultiplierDefaulted();
}

double ZoneCapacitanceMultiplierResearchSpecial::carbonDioxideCapacityMultiplier() const {
  return getImpl<detail::ZoneCapacitanceMultiplierResearchSpecial_Impl>()->carbonDioxideCapacityMultiplier();
}

Quantity ZoneCapacitanceMultiplierResearchSpecial::getCarbonDioxideCapacityMultiplier(bool returnIP) const {
  return getImpl<detail::ZoneCapacitanceMultiplierResearchSpecial_Impl>()->getCarbonDioxideCapacityMultiplier(returnIP);
}

bool ZoneCapacitanceMultiplierResearchSpecial::isCarbonDioxideCapacityMultiplierDefaulted() const {
  return getImpl<detail::ZoneCapacitanceMultiplierResearchSpecial_Impl>()->isCarbonDioxideCapacityMultiplierDefaulted();
}

bool ZoneCapacitanceMultiplierResearchSpecial::setTemperatureCapacityMultiplier(double temperatureCapacityMultiplier) {
  return getImpl<detail::ZoneCapacitanceMultiplierResearchSpecial_Impl>()->setTemperatureCapacityMultiplier(temperatureCapacityMultiplier);
}

bool ZoneCapacitanceMultiplierResearchSpecial::setTemperatureCapacityMultiplier(const Quantity& temperatureCapacityMultiplier) {
  return getImpl<detail::ZoneCapacitanceMultiplierResearchSpecial_Impl>()->setTemperatureCapacityMultiplier(temperatureCapacityMultiplier);
}

void ZoneCapacitanceMultiplierResearchSpecial::resetTemperatureCapacityMultiplier() {
  getImpl<detail::ZoneCapacitanceMultiplierResearchSpecial_Impl>()->resetTemperatureCapacityMultiplier();
}

bool ZoneCapacitanceMultiplierResearchSpecial::setHumidityCapacityMultiplier(double humidityCapacityMultiplier) {
  return getImpl<detail::ZoneCapacitanceMultiplierResearchSpecial_Impl>()->setHumidityCapacityMultiplier(humidityCapacityMultiplier);
}

bool ZoneCapacitanceMultiplierResearchSpecial::setHumidityCapacityMultiplier(const Quantity& humidityCapacityMultiplier) {
  return getImpl<detail::ZoneCapacitanceMultiplierResearchSpecial_Impl>()->setHumidityCapacityMultiplier(humidityCapacityMultiplier);
}

void ZoneCapacitanceMultiplierResearchSpecial::resetHumidityCapacityMultiplier() {
  getImpl<detail::ZoneCapacitanceMultiplierResearchSpecial_Impl>()->resetHumidityCapacityMultiplier();
}

bool ZoneCapacitanceMultiplierResearchSpecial::setCarbonDioxideCapacityMultiplier(double carbonDioxideCapacityMultiplier) {
  return getImpl<detail::ZoneCapacitanceMultiplierResearchSpecial_Impl>()->setCarbonDioxideCapacityMultiplier(carbonDioxideCapacityMultiplier);
}

bool ZoneCapacitanceMultiplierResearchSpecial::setCarbonDioxideCapacityMultiplier(const Quantity& carbonDioxideCapacityMultiplier) {
  return getImpl<detail::ZoneCapacitanceMultiplierResearchSpecial_Impl>()->setCarbonDioxideCapacityMultiplier(carbonDioxideCapacityMultiplier);
}

void ZoneCapacitanceMultiplierResearchSpecial::resetCarbonDioxideCapacityMultiplier() {
  getImpl<detail::ZoneCapacitanceMultiplierResearchSpecial_Impl>()->resetCarbonDioxideCapacityMultiplier();
}

/// @cond
ZoneCapacitanceMultiplierResearchSpecial::ZoneCapacitanceMultiplierResearchSpecial(std::shared_ptr<detail::ZoneCapacitanceMultiplierResearchSpecial_Impl> impl)
  : ModelObject(impl)
{}
ZoneCapacitanceMultiplierResearchSpecial::ZoneCapacitanceMultiplierResearchSpecial(Model& model)
  : ModelObject(ZoneCapacitanceMultiplierResearchSpecial::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ZoneCapacitanceMultiplierResearchSpecial_Impl>());
}

/// @endcond

} // model
} // openstudio

