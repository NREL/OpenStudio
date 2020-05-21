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

#include "CoilCoolingDX.hpp"
#include "CoilCoolingDX_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Coil_Cooling_DX_FieldEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoilCoolingDX_Impl::CoilCoolingDX_Impl(const IdfObject& idfObject,
                                         Model_Impl* model,
                                         bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilCoolingDX::iddObjectType());
  }

  CoilCoolingDX_Impl::CoilCoolingDX_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                         Model_Impl* model,
                                         bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilCoolingDX::iddObjectType());
  }

  CoilCoolingDX_Impl::CoilCoolingDX_Impl(const CoilCoolingDX_Impl& other,
                                         Model_Impl* model,
                                         bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CoilCoolingDX_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CoilCoolingDX_Impl::iddObjectType() const {
    return CoilCoolingDX::iddObjectType();
  }

  std::vector<ScheduleTypeKey> CoilCoolingDX_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    // TODO: Check schedule display names.
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Coil_Cooling_DXFields::AvailabilitySchedule) != e)
    {
      result.push_back(ScheduleTypeKey("CoilCoolingDX","Availability Schedule"));
    }
    return result;
  }

  boost::optional<Schedule> CoilCoolingDX_Impl::availabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Cooling_DXFields::AvailabilitySchedule);
  }

  boost::optional<ThermalZone> CoilCoolingDX_Impl::condenserZone() const {
    return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_Coil_Cooling_DXFields::CondenserZone);
  }

  boost::optional<std::string> CoilCoolingDX_Impl::condenserInletNodeName() const {
    return getString(OS_Coil_Cooling_DXFields::CondenserInletNodeName,true);
  }

  boost::optional<std::string> CoilCoolingDX_Impl::condenserOutletNodeName() const {
    return getString(OS_Coil_Cooling_DXFields::CondenserOutletNodeName,true);
  }

  boost::optional<std::string> CoilCoolingDX_Impl::condensateCollectionWaterStorageTankName() const {
    return getString(OS_Coil_Cooling_DXFields::CondensateCollectionWaterStorageTankName,true);
  }

  boost::optional<std::string> CoilCoolingDX_Impl::evaporativeCondenserSupplyWaterStorageTankName() const {
    return getString(OS_Coil_Cooling_DXFields::EvaporativeCondenserSupplyWaterStorageTankName,true);
  }

  bool CoilCoolingDX_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Coil_Cooling_DXFields::AvailabilitySchedule,
                              "CoilCoolingDX",
                              "Availability Schedule",
                              schedule);
    return result;
  }

  void CoilCoolingDX_Impl::resetAvailabilitySchedule() {
    bool result = setString(OS_Coil_Cooling_DXFields::AvailabilitySchedule, "");
    OS_ASSERT(result);
  }

  bool CoilCoolingDX_Impl::setCondenserZone(const ThermalZone& thermalZone) {
    bool result = setPointer(OS_Coil_Cooling_DXFields::CondenserZone, thermalZone.handle());
    return result;
  }

  void CoilCoolingDX_Impl::resetCondenserZone() {
    bool result = setString(OS_Coil_Cooling_DXFields::CondenserZone, "");
    OS_ASSERT(result);
  }

  bool CoilCoolingDX_Impl::setCondenserInletNodeName(const std::string& condenserInletNodeName) {
    bool result = setString(OS_Coil_Cooling_DXFields::CondenserInletNodeName, condenserInletNodeName);
    OS_ASSERT(result);
  }

  void CoilCoolingDX_Impl::resetCondenserInletNodeName() {
    bool result = setString(OS_Coil_Cooling_DXFields::CondenserInletNodeName, "");
    OS_ASSERT(result);
  }

  bool CoilCoolingDX_Impl::setCondenserOutletNodeName(const std::string& condenserOutletNodeName) {
    bool result = setString(OS_Coil_Cooling_DXFields::CondenserOutletNodeName, condenserOutletNodeName);
    OS_ASSERT(result);
  }

  void CoilCoolingDX_Impl::resetCondenserOutletNodeName() {
    bool result = setString(OS_Coil_Cooling_DXFields::CondenserOutletNodeName, "");
    OS_ASSERT(result);
  }

  bool CoilCoolingDX_Impl::setCondensateCollectionWaterStorageTankName(const std::string& condensateCollectionWaterStorageTankName) {
    bool result = setString(OS_Coil_Cooling_DXFields::CondensateCollectionWaterStorageTankName, condensateCollectionWaterStorageTankName);
    OS_ASSERT(result);
  }

  void CoilCoolingDX_Impl::resetCondensateCollectionWaterStorageTankName() {
    bool result = setString(OS_Coil_Cooling_DXFields::CondensateCollectionWaterStorageTankName, "");
    OS_ASSERT(result);
  }

  bool CoilCoolingDX_Impl::setEvaporativeCondenserSupplyWaterStorageTankName(const std::string& evaporativeCondenserSupplyWaterStorageTankName) {
    bool result = setString(OS_Coil_Cooling_DXFields::EvaporativeCondenserSupplyWaterStorageTankName, evaporativeCondenserSupplyWaterStorageTankName);
    OS_ASSERT(result);
  }

  void CoilCoolingDX_Impl::resetEvaporativeCondenserSupplyWaterStorageTankName() {
    bool result = setString(OS_Coil_Cooling_DXFields::EvaporativeCondenserSupplyWaterStorageTankName, "");
    OS_ASSERT(result);
  }

  unsigned CoilCoolingDX_Impl::inletPort() const {
    return OS_Coil_Cooling_DXFields::EvaporatorInletNode;
  }

  unsigned CoilCoolingDX_Impl::outletPort() const {
    return OS_Coil_Cooling_DXFields::EvaporatorOutletNode;
  }

  ModelObject CoilCoolingDX_Impl::clone(Model model) const {
    auto t_clone = StraightComponent_Impl::clone(model).cast<CoilCoolingDX>();

    return t_clone;
  }

  std::vector<ModelObject> CoilCoolingDX_Impl::children() const {

    return result;
  }

} // detail

CoilCoolingDX::CoilCoolingDX(const Model& model)
  : StraightComponent(CoilCoolingDX::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilCoolingDX_Impl>());

}

IddObjectType CoilCoolingDX::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Coil_Cooling_DX);
}

boost::optional<Schedule> CoilCoolingDX::availabilitySchedule() const {
  return getImpl<detail::CoilCoolingDX_Impl>()->availabilitySchedule();
}

boost::optional<ThermalZone> CoilCoolingDX::condenserZone() const {
  return getImpl<detail::CoilCoolingDX_Impl>()->condenserZone();
}

boost::optional<std::string> CoilCoolingDX::condenserInletNodeName() const {
  return getImpl<detail::CoilCoolingDX_Impl>()->condenserInletNodeName();
}

boost::optional<std::string> CoilCoolingDX::condenserOutletNodeName() const {
  return getImpl<detail::CoilCoolingDX_Impl>()->condenserOutletNodeName();
}

boost::optional<std::string> CoilCoolingDX::condensateCollectionWaterStorageTankName() const {
  return getImpl<detail::CoilCoolingDX_Impl>()->condensateCollectionWaterStorageTankName();
}

boost::optional<std::string> CoilCoolingDX::evaporativeCondenserSupplyWaterStorageTankName() const {
  return getImpl<detail::CoilCoolingDX_Impl>()->evaporativeCondenserSupplyWaterStorageTankName();
}

bool CoilCoolingDX::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::CoilCoolingDX_Impl>()->setAvailabilitySchedule(schedule);
}

void CoilCoolingDX::resetAvailabilitySchedule() {
  getImpl<detail::CoilCoolingDX_Impl>()->resetAvailabilitySchedule();
}

bool CoilCoolingDX::setCondenserZone(const ThermalZone& thermalZone) {
  return getImpl<detail::CoilCoolingDX_Impl>()->setCondenserZone(thermalZone);
}

void CoilCoolingDX::resetCondenserZone() {
  getImpl<detail::CoilCoolingDX_Impl>()->resetCondenserZone();
}

void CoilCoolingDX::setCondenserInletNodeName(const std::string& condenserInletNodeName) {
  getImpl<detail::CoilCoolingDX_Impl>()->setCondenserInletNodeName(condenserInletNodeName);
}

void CoilCoolingDX::resetCondenserInletNodeName() {
  getImpl<detail::CoilCoolingDX_Impl>()->resetCondenserInletNodeName();
}

void CoilCoolingDX::setCondenserOutletNodeName(const std::string& condenserOutletNodeName) {
  getImpl<detail::CoilCoolingDX_Impl>()->setCondenserOutletNodeName(condenserOutletNodeName);
}

void CoilCoolingDX::resetCondenserOutletNodeName() {
  getImpl<detail::CoilCoolingDX_Impl>()->resetCondenserOutletNodeName();
}

void CoilCoolingDX::setCondensateCollectionWaterStorageTankName(const std::string& condensateCollectionWaterStorageTankName) {
  getImpl<detail::CoilCoolingDX_Impl>()->setCondensateCollectionWaterStorageTankName(condensateCollectionWaterStorageTankName);
}

void CoilCoolingDX::resetCondensateCollectionWaterStorageTankName() {
  getImpl<detail::CoilCoolingDX_Impl>()->resetCondensateCollectionWaterStorageTankName();
}

void CoilCoolingDX::setEvaporativeCondenserSupplyWaterStorageTankName(const std::string& evaporativeCondenserSupplyWaterStorageTankName) {
  getImpl<detail::CoilCoolingDX_Impl>()->setEvaporativeCondenserSupplyWaterStorageTankName(evaporativeCondenserSupplyWaterStorageTankName);
}

void CoilCoolingDX::resetEvaporativeCondenserSupplyWaterStorageTankName() {
  getImpl<detail::CoilCoolingDX_Impl>()->resetEvaporativeCondenserSupplyWaterStorageTankName();
}

/// @cond
CoilCoolingDX::CoilCoolingDX(std::shared_ptr<detail::CoilCoolingDX_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

