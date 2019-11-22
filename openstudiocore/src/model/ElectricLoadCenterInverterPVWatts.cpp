/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "ElectricLoadCenterInverterPVWatts.hpp"
#include "ElectricLoadCenterInverterPVWatts_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ElectricLoadCenter_Inverter_PVWatts_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ElectricLoadCenterInverterPVWatts_Impl::ElectricLoadCenterInverterPVWatts_Impl(const IdfObject& idfObject,
                                                                                 Model_Impl* model,
                                                                                 bool keepHandle)
    : Inverter_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ElectricLoadCenterInverterPVWatts::iddObjectType());
  }

  ElectricLoadCenterInverterPVWatts_Impl::ElectricLoadCenterInverterPVWatts_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                 Model_Impl* model,
                                                                                 bool keepHandle)
    : Inverter_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ElectricLoadCenterInverterPVWatts::iddObjectType());
  }

  ElectricLoadCenterInverterPVWatts_Impl::ElectricLoadCenterInverterPVWatts_Impl(const ElectricLoadCenterInverterPVWatts_Impl& other,
                                                                                 Model_Impl* model,
                                                                                 bool keepHandle)
    : Inverter_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ElectricLoadCenterInverterPVWatts_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty())
    {
      result.push_back("Inverter DC to AC Efficiency");
      result.push_back("Inverter DC Input Electric Power");
      result.push_back("Inverter DC Input Electric Energy");
      result.push_back("Inverter AC Output Electric Power");
      result.push_back("Inverter AC Output Electric Energy");
      result.push_back("Inverter Conversion Loss Power");
      result.push_back("Inverter Conversion Loss Energy");
      result.push_back("Inverter Conversion Loss Decrement Energy");
      result.push_back("Inverter Thermal Loss Rate");
      result.push_back("Inverter Thermal Loss Energy");
      result.push_back("Inverter Ancillary AC Electric Power");
      result.push_back("Inverter Ancillary AC Electric Energy");
    }
    return result;
  }

  IddObjectType ElectricLoadCenterInverterPVWatts_Impl::iddObjectType() const {
    return ElectricLoadCenterInverterPVWatts::iddObjectType();
  }

  std::vector<ScheduleTypeKey> ElectricLoadCenterInverterPVWatts_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
    std::vector<ScheduleTypeKey> result;
    return result;
  }

  boost::optional<ThermalZone> ElectricLoadCenterInverterPVWatts_Impl::thermalZone() const {
    return boost::none;
  }

  bool ElectricLoadCenterInverterPVWatts_Impl::setThermalZone(const ThermalZone& thermalZone) {
    return boost::none;
  }

  void ElectricLoadCenterInverterPVWatts_Impl::resetThermalZone() {
    bool result = true;
    OS_ASSERT(result);
  }

  double ElectricLoadCenterInverterPVWatts_Impl::dcToACSizeRatio() const {
    boost::optional<double> value = getDouble(OS_ElectricLoadCenter_Inverter_PVWattsFields::DCtoACSizeRatio, true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ElectricLoadCenterInverterPVWatts_Impl::isDCToACSizeRatioDefaulted() const {
    return isEmpty(OS_ElectricLoadCenter_Inverter_PVWattsFields::DCtoACSizeRatio);
  }

  double ElectricLoadCenterInverterPVWatts_Impl::inverterEfficiency() const {
    boost::optional<double> value = getDouble(OS_ElectricLoadCenter_Inverter_PVWattsFields::InverterEfficiency, true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ElectricLoadCenterInverterPVWatts_Impl::isInverterEfficiencyDefaulted() const {
    return isEmpty(OS_ElectricLoadCenter_Inverter_PVWattsFields::InverterEfficiency);
  }

  bool ElectricLoadCenterInverterPVWatts_Impl::setDCToACSizeRatio(double dcToACSizeRatio) {
    bool result = setDouble(OS_ElectricLoadCenter_Inverter_PVWattsFields::DCtoACSizeRatio, dcToACSizeRatio);
    OS_ASSERT(result);
    return result;
  }

  void ElectricLoadCenterInverterPVWatts_Impl::resetDCToACSizeRatio() {
    bool result = setString(OS_ElectricLoadCenter_Inverter_PVWattsFields::DCtoACSizeRatio, "");
    OS_ASSERT(result);
  }

  bool ElectricLoadCenterInverterPVWatts_Impl::setInverterEfficiency(double inverterEfficiency) {
    bool result = setDouble(OS_ElectricLoadCenter_Inverter_PVWattsFields::InverterEfficiency, inverterEfficiency);
    OS_ASSERT(result);
    return result;
  }

  void ElectricLoadCenterInverterPVWatts_Impl::resetInverterEfficiency() {
    bool result = setString(OS_ElectricLoadCenter_Inverter_PVWattsFields::InverterEfficiency, "");
    OS_ASSERT(result);
  }

} // detail

ElectricLoadCenterInverterPVWatts::ElectricLoadCenterInverterPVWatts(const Model& model)
  : Inverter(ElectricLoadCenterInverterPVWatts::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ElectricLoadCenterInverterPVWatts_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
}

IddObjectType ElectricLoadCenterInverterPVWatts::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ElectricLoadCenter_Inverter_PVWatts);
}

double ElectricLoadCenterInverterPVWatts::dcToACSizeRatio() const {
  return getImpl<detail::ElectricLoadCenterInverterPVWatts_Impl>()->dcToACSizeRatio();
}

bool ElectricLoadCenterInverterPVWatts::isDCToACSizeRatioDefaulted() const {
  return getImpl<detail::ElectricLoadCenterInverterPVWatts_Impl>()->isDCToACSizeRatioDefaulted();
}

double ElectricLoadCenterInverterPVWatts::inverterEfficiency() const {
  return getImpl<detail::ElectricLoadCenterInverterPVWatts_Impl>()->inverterEfficiency();
}

bool ElectricLoadCenterInverterPVWatts::isInverterEfficiencyDefaulted() const {
  return getImpl<detail::ElectricLoadCenterInverterPVWatts_Impl>()->isInverterEfficiencyDefaulted();
}

bool ElectricLoadCenterInverterPVWatts::setDCToACSizeRatio(double dcToACSizeRatio) {
  return getImpl<detail::ElectricLoadCenterInverterPVWatts_Impl>()->setDCToACSizeRatio(dcToACSizeRatio);
}

void ElectricLoadCenterInverterPVWatts::resetDCToACSizeRatio() {
  getImpl<detail::ElectricLoadCenterInverterPVWatts_Impl>()->resetDCToACSizeRatio();
}

bool ElectricLoadCenterInverterPVWatts::setInverterEfficiency(double inverterEfficiency) {
  return getImpl<detail::ElectricLoadCenterInverterPVWatts_Impl>()->setInverterEfficiency(inverterEfficiency);
}

void ElectricLoadCenterInverterPVWatts::resetInverterEfficiency() {
  getImpl<detail::ElectricLoadCenterInverterPVWatts_Impl>()->resetInverterEfficiency();
}

/// @cond
ElectricLoadCenterInverterPVWatts::ElectricLoadCenterInverterPVWatts(std::shared_ptr<detail::ElectricLoadCenterInverterPVWatts_Impl> impl)
  : Inverter(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

