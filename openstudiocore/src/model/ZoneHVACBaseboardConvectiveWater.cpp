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

#include "ZoneHVACBaseboardConvectiveWater.hpp"
#include "ZoneHVACBaseboardConvectiveWater_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "StraightComponent.hpp"
#include "StraightComponent_Impl.hpp"
#include "CoilHeatingWaterBaseboard.hpp"
#include "CoilHeatingWaterBaseboard_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"


#include <utilities/idd/OS_ZoneHVAC_Baseboard_Convective_Water_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ZoneHVACBaseboardConvectiveWater_Impl::ZoneHVACBaseboardConvectiveWater_Impl(const IdfObject& idfObject,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : ZoneHVACComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ZoneHVACBaseboardConvectiveWater::iddObjectType());
  }

  ZoneHVACBaseboardConvectiveWater_Impl::ZoneHVACBaseboardConvectiveWater_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ZoneHVACBaseboardConvectiveWater::iddObjectType());
  }

  ZoneHVACBaseboardConvectiveWater_Impl::ZoneHVACBaseboardConvectiveWater_Impl(const ZoneHVACBaseboardConvectiveWater_Impl& other,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
  {}

ModelObject ZoneHVACBaseboardConvectiveWater_Impl::clone(Model model) const
  {
    ZoneHVACBaseboardConvectiveWater baseboardConvWaterClone = ZoneHVACComponent_Impl::clone(model).cast<ZoneHVACBaseboardConvectiveWater>();

    auto t_heatingCoil = heatingCoil();
    StraightComponent heatingCoilClone = t_heatingCoil.clone(model).cast<StraightComponent>();

    baseboardConvWaterClone.setHeatingCoil(heatingCoilClone);

    if( model == this->model() ) {
      if( auto plant = t_heatingCoil.plantLoop() ) {
        plant->addDemandBranchForComponent(heatingCoilClone);
      }
    }

    return baseboardConvWaterClone;
  }

std::vector<IdfObject> ZoneHVACBaseboardConvectiveWater_Impl::remove()
  {
    if( boost::optional<CoilHeatingWaterBaseboard> waterHeatingCoil =
          heatingCoil().optionalCast<CoilHeatingWaterBaseboard>() )
    {
      if( boost::optional<PlantLoop> plantLoop = waterHeatingCoil->plantLoop() )
      {
        plantLoop->removeDemandBranchWithComponent( waterHeatingCoil.get() );
      }
    }
    return ZoneHVACComponent_Impl::remove();
  }

  const std::vector<std::string>& ZoneHVACBaseboardConvectiveWater_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ZoneHVACBaseboardConvectiveWater_Impl::iddObjectType() const {
    return ZoneHVACBaseboardConvectiveWater::iddObjectType();
  }

  std::vector<ScheduleTypeKey> ZoneHVACBaseboardConvectiveWater_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_ZoneHVAC_Baseboard_Convective_WaterFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneHVACBaseboardConvectiveWater","Availability"));
    }
    return result;
  }
  unsigned ZoneHVACBaseboardConvectiveWater_Impl::inletPort() const
  {
    return 0; // this object has no inlet or outlet node
  }

   std::vector<ModelObject> ZoneHVACBaseboardConvectiveWater_Impl::children() const
  {
    std::vector<ModelObject> result;
    if (OptionalStraightComponent intermediate = optionalHeatingCoil()) {
      result.push_back(*intermediate);
    }
    return result;
  }

  unsigned ZoneHVACBaseboardConvectiveWater_Impl::outletPort() const
  {
    return 0; // this object has no inlet or outlet node
  }
  Schedule ZoneHVACBaseboardConvectiveWater_Impl::availabilitySchedule() const {
    boost::optional<Schedule> value = optionalAvailabilitySchedule();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Availability Schedule attached.");
    }
    return value.get();
  }

  StraightComponent ZoneHVACBaseboardConvectiveWater_Impl::heatingCoil() const {
    boost::optional<StraightComponent> value = optionalHeatingCoil();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heating Coil attached.");
    }
    return value.get();
  }

  bool ZoneHVACBaseboardConvectiveWater_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ZoneHVAC_Baseboard_Convective_WaterFields::AvailabilityScheduleName,
                              "ZoneHVACBaseboardConvectiveWater",
                              "Availability",
                              schedule);
    return result;
  }

  bool ZoneHVACBaseboardConvectiveWater_Impl::setHeatingCoil(const StraightComponent& baseboardHeatingCoil) {
    bool result = setPointer(OS_ZoneHVAC_Baseboard_Convective_WaterFields::HeatingCoilName, baseboardHeatingCoil.handle());
    return result;
  }

  boost::optional<Schedule> ZoneHVACBaseboardConvectiveWater_Impl::optionalAvailabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneHVAC_Baseboard_Convective_WaterFields::AvailabilityScheduleName);
  }

  boost::optional<StraightComponent> ZoneHVACBaseboardConvectiveWater_Impl::optionalHeatingCoil() const {
    return getObject<ModelObject>().getModelObjectTarget<StraightComponent>(OS_ZoneHVAC_Baseboard_Convective_WaterFields::HeatingCoilName);
  }

  boost::optional<ModelObject> ZoneHVACBaseboardConvectiveWater_Impl::availabilityScheduleAsModelObject() const {
    OptionalModelObject result = availabilitySchedule();
    return result;
  }

  boost::optional<ModelObject> ZoneHVACBaseboardConvectiveWater_Impl::heatingCoilAsModelObject() const {
    OptionalModelObject result = heatingCoil();
    return result;
  }

  bool ZoneHVACBaseboardConvectiveWater_Impl::setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setAvailabilitySchedule(schedule);
      }
    }
    return false;
  }

  bool ZoneHVACBaseboardConvectiveWater_Impl::setHeatingCoilAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalStraightComponent intermediate = modelObject->optionalCast<StraightComponent>();
      if (intermediate) {
        StraightComponent baseboardHeatingCoil(*intermediate);
        return setHeatingCoil(baseboardHeatingCoil);
      }
    }
    return false;
  }

  boost::optional<ThermalZone> ZoneHVACBaseboardConvectiveWater_Impl::thermalZone()
  {
    ModelObject thisObject = this->getObject<ModelObject>();
    std::vector<ThermalZone> thermalZones = this->model().getConcreteModelObjects<ThermalZone>();
    for( const auto & thermalZone : thermalZones )
    {
      std::vector<ModelObject> equipment = thermalZone.equipment();

      if( std::find(equipment.begin(),equipment.end(),thisObject) != equipment.end() )
      {
        return thermalZone;
      }
    }
    return boost::none;
  }

  //reimplemented to override the base-class method in ZoneHVACComponent
  //because this component doesn't get attached to the zone inlet and zone outlet nodes
  bool ZoneHVACBaseboardConvectiveWater_Impl::addToThermalZone(ThermalZone & thermalZone)
  {
    Model m = this->model();

    if( thermalZone.model() != m )
    {
      return false;
    }

    if( thermalZone.isPlenum() )
    {
      return false;
    }

    removeFromThermalZone();

    thermalZone.setUseIdealAirLoads(false);

    thermalZone.addEquipment(this->getObject<ZoneHVACComponent>());

    return true;
  }

  //reimplemented to override the base-class method in ZoneHVACComponent
  //because this component doesn't get attached to the zone inlet and zone outlet nodes
  //and therefore doesn't need to be removed from them when removed from the zone
  void ZoneHVACBaseboardConvectiveWater_Impl::removeFromThermalZone()
  {
    if ( boost::optional<ThermalZone> thermalZone = this->thermalZone() ) {
      thermalZone->removeEquipment(this->getObject<ZoneHVACComponent>());
    }
  }
} // detail

ZoneHVACBaseboardConvectiveWater::ZoneHVACBaseboardConvectiveWater(const Model& model, Schedule& availabilitySchedule,StraightComponent& heatingCoilBaseboard)
  : ZoneHVACComponent(ZoneHVACBaseboardConvectiveWater::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ZoneHVACBaseboardConvectiveWater_Impl>());

  //     OS_ZoneHVAC_Baseboard_Convective_WaterFields::AvailabilityScheduleName
  //     OS_ZoneHVAC_Baseboard_Convective_WaterFields::HeatingCoilName
  bool ok = setAvailabilitySchedule(availabilitySchedule);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s availability schedule to "
                  << availabilitySchedule.briefDescription() << ".");
  }
  ok = setHeatingCoil(heatingCoilBaseboard);
  OS_ASSERT(ok);
}

IddObjectType ZoneHVACBaseboardConvectiveWater::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ZoneHVAC_Baseboard_Convective_Water);
}

Schedule ZoneHVACBaseboardConvectiveWater::availabilitySchedule() const {
  return getImpl<detail::ZoneHVACBaseboardConvectiveWater_Impl>()->availabilitySchedule();
}

StraightComponent ZoneHVACBaseboardConvectiveWater::heatingCoil() const {
  return getImpl<detail::ZoneHVACBaseboardConvectiveWater_Impl>()->heatingCoil();
}

bool ZoneHVACBaseboardConvectiveWater::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::ZoneHVACBaseboardConvectiveWater_Impl>()->setAvailabilitySchedule(schedule);
}

bool ZoneHVACBaseboardConvectiveWater::setHeatingCoil(const StraightComponent& baseboardHeatingCoil) {
  return getImpl<detail::ZoneHVACBaseboardConvectiveWater_Impl>()->setHeatingCoil(baseboardHeatingCoil);
}
boost::optional<ThermalZone> ZoneHVACBaseboardConvectiveWater::thermalZone()
{
  return getImpl<detail::ZoneHVACBaseboardConvectiveWater_Impl>()->thermalZone();
}

bool ZoneHVACBaseboardConvectiveWater::addToThermalZone(ThermalZone & thermalZone)
{
  return getImpl<detail::ZoneHVACBaseboardConvectiveWater_Impl>()->addToThermalZone(thermalZone);
}

void ZoneHVACBaseboardConvectiveWater::removeFromThermalZone()
{
  return getImpl<detail::ZoneHVACBaseboardConvectiveWater_Impl>()->removeFromThermalZone();
}
/// @cond
ZoneHVACBaseboardConvectiveWater::ZoneHVACBaseboardConvectiveWater(std::shared_ptr<detail::ZoneHVACBaseboardConvectiveWater_Impl> impl)
  : ZoneHVACComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

