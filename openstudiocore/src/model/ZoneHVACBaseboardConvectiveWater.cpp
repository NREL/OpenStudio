/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <model/ZoneHVACBaseboardConvectiveWater.hpp>
#include <model/ZoneHVACBaseboardConvectiveWater_Impl.hpp>

// TODO: Check the following class names against object getters and setters.
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/StraightComponent.hpp>
#include <model/StraightComponent_Impl.hpp>
#include <model/CoilHeatingWaterBaseboard.hpp>
#include <model/CoilHeatingWaterBaseboard_Impl.hpp>
#include <model/ScheduleTypeLimits.hpp>
#include <model/ScheduleTypeRegistry.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/Model.hpp>
#include <model/Model_Impl.hpp>


#include <utilities/idd/OS_ZoneHVAC_Baseboard_Convective_Water_FieldEnums.hxx>

#include <utilities/core/Assert.hpp>

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

    StraightComponent heatingCoilClone = this->heatingCoil().clone(model).cast<StraightComponent>();

    baseboardConvWaterClone.setHeatingCoil(heatingCoilClone);

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
  unsigned ZoneHVACBaseboardConvectiveWater_Impl::inletPort()
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

  unsigned ZoneHVACBaseboardConvectiveWater_Impl::outletPort()
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
    boost::optional<ThermalZone> result;
    Model m = this->model();
    std::vector<ThermalZone> thermalZones = m.getModelObjects<ThermalZone>();
    BOOST_FOREACH(ThermalZone& thermalZone, thermalZones){
      std::vector<ModelObject> equipments = thermalZone.equipment(); 
      BOOST_FOREACH(ModelObject& equipment, equipments){
        if (equipment.handle() == this->handle()){
          result = thermalZone;
        }
      }
    }

    return result;
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
    boost::optional<ThermalZone> thermalZone = this->thermalZone();
    Model m = this->model();
    ModelObject thisObject = this->getObject<ModelObject>();
    std::vector<ThermalZone> thermalZones = m.getModelObjects<ThermalZone>();
    for( std::vector<ThermalZone>::iterator it = thermalZones.begin();
         it != thermalZones.end();
         it++ )
    {
      std::vector<ModelObject> equipment = it->equipment();

      if( std::find(equipment.begin(),equipment.end(),thisObject) != equipment.end() )
      {
        it->removeEquipment(thisObject);

        break;
      }
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
ZoneHVACBaseboardConvectiveWater::ZoneHVACBaseboardConvectiveWater(boost::shared_ptr<detail::ZoneHVACBaseboardConvectiveWater_Impl> impl)
  : ZoneHVACComponent(impl)
{}
/// @endcond

} // model
} // openstudio

