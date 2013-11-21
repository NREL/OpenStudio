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

#include <model/ZoneHVACRefrigerationChillerSet.hpp>
#include <model/ZoneHVACRefrigerationChillerSet_Impl.hpp>

// TODO: Check the following class names against object getters and setters.
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
// #include <model/ThermalZone.hpp>
// #include <model/ThermalZone_Impl.hpp>
#include <model/ScheduleTypeLimits.hpp>
#include <model/ScheduleTypeRegistry.hpp>
// #include <model/RefrigerationAirChiller.hpp>
// #include <model/RefrigerationAirChiller_Impl.hpp>

#include <utilities/idd/OS_ZoneHVAC_RefrigerationChillerSet_FieldEnums.hxx>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model {

namespace detail {

  ZoneHVACRefrigerationChillerSet_Impl::ZoneHVACRefrigerationChillerSet_Impl(const IdfObject& idfObject,
                                                                             Model_Impl* model,
                                                                             bool keepHandle)
    : ZoneHVACComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ZoneHVACRefrigerationChillerSet::iddObjectType());
  }

  ZoneHVACRefrigerationChillerSet_Impl::ZoneHVACRefrigerationChillerSet_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                             Model_Impl* model,
                                                                             bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ZoneHVACRefrigerationChillerSet::iddObjectType());
  }

  ZoneHVACRefrigerationChillerSet_Impl::ZoneHVACRefrigerationChillerSet_Impl(const ZoneHVACRefrigerationChillerSet_Impl& other,
                                                                             Model_Impl* model,
                                                                             bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ZoneHVACRefrigerationChillerSet_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ZoneHVACRefrigerationChillerSet_Impl::iddObjectType() const {
    return ZoneHVACRefrigerationChillerSet::iddObjectType();
  }

  std::vector<ScheduleTypeKey> ZoneHVACRefrigerationChillerSet_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    // TODO: Check schedule display names.
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_ZoneHVAC_RefrigerationChillerSetFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneHVACRefrigerationChillerSet","Availability"));
    }
    return result;
  }

  unsigned ZoneHVACRefrigerationChillerSet_Impl::inletPort()
  {
    return OS_ZoneHVAC_RefrigerationChillerSetFields::AirInletNodeName;
  }

  unsigned ZoneHVACRefrigerationChillerSet_Impl::outletPort()
  {
    return OS_ZoneHVAC_RefrigerationChillerSetFields::AirOutletNodeName;
  }

  boost::optional<Schedule> ZoneHVACRefrigerationChillerSet_Impl::availabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneHVAC_RefrigerationChillerSetFields::AvailabilityScheduleName);
  }

  // std::vector<RefrigerationAirChiller> airChillers() const {
  //   std::vector<RefrigerationAirChiller> result;

  //   std::vector<IdfExtensibleGroup> groups = extensibleGroups();

  //   for( std::vector<IdfExtensibleGroup>::iterator it = groups.begin();
  //        it != groups.end();
  //        it++ )
  //   {
  //     boost::optional<RefrigerationAirChiller> refrigerationAirChiller = it->cast<WorkspaceExtensibleGroup>().getObject<ModelObject>().getModelObjectTarget<RefrigerationAirChiller>(OS_ZoneHVAC_RefrigerationChillerSetExtensibleFields::AirChillerName);

  //     if(refrigerationAirChiller) {
  //       result.push_back( refrigerationAirChiller.get() );
  //     }
  //   }

  //   return result;
  // }

  // boost::optional<ThermalZone> ZoneHVACRefrigerationChillerSet_Impl::zone() const {
  //   return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_ZoneHVAC_RefrigerationChillerSetFields::ZoneName);
  // }

  // boost::optional<std::string> ZoneHVACRefrigerationChillerSet_Impl::airInletNodeName() const {
  //   return getString(OS_ZoneHVAC_RefrigerationChillerSetFields::AirInletNodeName,true);
  // }

  // boost::optional<std::string> ZoneHVACRefrigerationChillerSet_Impl::airOutletNodeName() const {
  //   return getString(OS_ZoneHVAC_RefrigerationChillerSetFields::AirOutletNodeName,true);
  // }

  bool ZoneHVACRefrigerationChillerSet_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ZoneHVAC_RefrigerationChillerSetFields::AvailabilityScheduleName,
                              "ZoneHVACRefrigerationChillerSet",
                              "Availability",
                              schedule);
    return result;
  }

  void ZoneHVACRefrigerationChillerSet_Impl::resetAvailabilitySchedule() {
    bool result = setString(OS_ZoneHVAC_RefrigerationChillerSetFields::AvailabilityScheduleName, "");
    OS_ASSERT(result);
  }

  // bool ZoneHVACRefrigerationChillerSet_Impl::addAirChiller(const RefrigerationAirChiller& refrigerationAirChiller)
  // {
  //   WorkspaceExtensibleGroup eg = getObject<ModelObject>().pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();

  //   bool temp = eg.setPointer(OS_ZoneHVAC_RefrigerationChillerSetExtensibleFields::AirChillerName, refrigerationAirChiller.handle());

  //   if( !temp ) {
  //     getObject<ModelObject>().eraseExtensibleGroup(eg.groupIndex());
  //     return temp;
  //   }
  //   return temp;
  // }

  // bool ZoneHVACRefrigerationChillerSet_Impl::addAirChiller(const RefrigerationAirChiller& refrigerationAirChiller, unsigned index)
  // {
  //   bool temp = false;
  //   if(index > -1 && index < numExtensibleGroups()) {
  //     WorkspaceExtensibleGroup eg = getObject<ModelObject>().insertExtensibleGroup(index).cast<WorkspaceExtensibleGroup>();

  //     temp = eg.setPointer(OS_ZoneHVAC_RefrigerationChillerSetExtensibleFields::AirChillerName, refrigerationAirChiller.handle());

  //     if( !temp ) {
  //       getObject<ModelObject>().eraseExtensibleGroup(eg.groupIndex());
  //       return temp;
  //     }
  //   } 
  //   else if( index == numExtensibleGroups() ) {
  //     return addAirChiller( refrigerationAirChiller );
  //   }

  //   return temp;
  // }

  // void ZoneHVACRefrigerationChillerSet_Impl::removeAirChiller(unsigned index)
  // {
  //   if(index > -1 && index < numExtensibleGroups()) {
  //     getObject<ModelObject>().eraseExtensibleGroup(index);
  //   }
  // }

  // void ZoneHVACRefrigerationChillerSet_Impl::removeAirChiller(const RefrigerationAirChiller& refrigerationAirChiller)
  // {
  //   std::vector<IdfExtensibleGroup> groups = extensibleGroups();

  //   for( std::vector<IdfExtensibleGroup>::iterator it = groups.begin();
  //        it != groups.end();
  //        it++ )
  //   {
  //     boost::optional<WorkspaceObject> wo = it->cast<WorkspaceExtensibleGroup>().getTarget(OS_ZoneHVAC_RefrigerationChillerSetExtensibleFields::AirChillerName);
  //     OS_ASSERT(wo);

  //     if( wo->handle() == refrigerationAirChiller.handle() )
  //     {
  //       unsigned index = it->cast<WorkspaceExtensibleGroup>().groupIndex();
  //       getObject<ModelObject>().eraseExtensibleGroup(index);
  //       break;
  //     }
  //   }
  // }

  // void ZoneHVACRefrigerationChillerSet_Impl::removeAllAirChillers() 
  // {
  //   getObject<ModelObject>().clearExtensibleGroups();
  // }

  // bool ZoneHVACRefrigerationChillerSet_Impl::setZone(const boost::optional<ThermalZone>& thermalZone) {
  //   bool result(false);
  //   if (thermalZone) {
  //     result = setPointer(OS_ZoneHVAC_RefrigerationChillerSetFields::ZoneName, thermalZone.get().handle());
  //   }
  //   else {
  //     resetZone();
  //     result = true;
  //   }
  //   return result;
  // }

  // void ZoneHVACRefrigerationChillerSet_Impl::resetZone() {
  //   bool result = setString(OS_ZoneHVAC_RefrigerationChillerSetFields::ZoneName, "");
  //   OS_ASSERT(result);
  // }

  // void ZoneHVACRefrigerationChillerSet_Impl::setAirInletNodeName(boost::optional<std::string> airInletNodeName) {
  //   bool result(false);
  //   if (airInletNodeName) {
  //     result = setString(OS_ZoneHVAC_RefrigerationChillerSetFields::AirInletNodeName, airInletNodeName.get());
  //   }
  //   else {
  //     resetAirInletNodeName();
  //     result = true;
  //   }
  //   OS_ASSERT(result);
  // }

  // void ZoneHVACRefrigerationChillerSet_Impl::resetAirInletNodeName() {
  //   bool result = setString(OS_ZoneHVAC_RefrigerationChillerSetFields::AirInletNodeName, "");
  //   OS_ASSERT(result);
  // }

  // void ZoneHVACRefrigerationChillerSet_Impl::setAirOutletNodeName(boost::optional<std::string> airOutletNodeName) {
  //   bool result(false);
  //   if (airOutletNodeName) {
  //     result = setString(OS_ZoneHVAC_RefrigerationChillerSetFields::AirOutletNodeName, airOutletNodeName.get());
  //   }
  //   else {
  //     resetAirOutletNodeName();
  //     result = true;
  //   }
  //   OS_ASSERT(result);
  // }

  // void ZoneHVACRefrigerationChillerSet_Impl::resetAirOutletNodeName() {
  //   bool result = setString(OS_ZoneHVAC_RefrigerationChillerSetFields::AirOutletNodeName, "");
  //   OS_ASSERT(result);
  // }

} // detail

ZoneHVACRefrigerationChillerSet::ZoneHVACRefrigerationChillerSet(const Model& model)
  : ZoneHVACComponent(ZoneHVACRefrigerationChillerSet::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ZoneHVACRefrigerationChillerSet_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
}

IddObjectType ZoneHVACRefrigerationChillerSet::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ZoneHVAC_RefrigerationChillerSet);
}

boost::optional<Schedule> ZoneHVACRefrigerationChillerSet::availabilitySchedule() const {
  return getImpl<detail::ZoneHVACRefrigerationChillerSet_Impl>()->availabilitySchedule();
}

// boost::optional<ThermalZone> ZoneHVACRefrigerationChillerSet::zone() const {
//   return getImpl<detail::ZoneHVACRefrigerationChillerSet_Impl>()->zone();
// }

// boost::optional<std::string> ZoneHVACRefrigerationChillerSet::airInletNodeName() const {
//   return getImpl<detail::ZoneHVACRefrigerationChillerSet_Impl>()->airInletNodeName();
// }

// boost::optional<std::string> ZoneHVACRefrigerationChillerSet::airOutletNodeName() const {
//   return getImpl<detail::ZoneHVACRefrigerationChillerSet_Impl>()->airOutletNodeName();
// }

bool ZoneHVACRefrigerationChillerSet::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::ZoneHVACRefrigerationChillerSet_Impl>()->setAvailabilitySchedule(schedule);
}

void ZoneHVACRefrigerationChillerSet::resetAvailabilitySchedule() {
  getImpl<detail::ZoneHVACRefrigerationChillerSet_Impl>()->resetAvailabilitySchedule();
}

// bool ZoneHVACRefrigerationChillerSet::setZone(const ThermalZone& thermalZone) {
//   return getImpl<detail::ZoneHVACRefrigerationChillerSet_Impl>()->setZone(thermalZone);
// }

// void ZoneHVACRefrigerationChillerSet::resetZone() {
//   getImpl<detail::ZoneHVACRefrigerationChillerSet_Impl>()->resetZone();
// }

// void ZoneHVACRefrigerationChillerSet::setAirInletNodeName(std::string airInletNodeName) {
//   getImpl<detail::ZoneHVACRefrigerationChillerSet_Impl>()->setAirInletNodeName(airInletNodeName);
// }

// void ZoneHVACRefrigerationChillerSet::resetAirInletNodeName() {
//   getImpl<detail::ZoneHVACRefrigerationChillerSet_Impl>()->resetAirInletNodeName();
// }

// void ZoneHVACRefrigerationChillerSet::setAirOutletNodeName(std::string airOutletNodeName) {
//   getImpl<detail::ZoneHVACRefrigerationChillerSet_Impl>()->setAirOutletNodeName(airOutletNodeName);
// }

// void ZoneHVACRefrigerationChillerSet::resetAirOutletNodeName() {
//   getImpl<detail::ZoneHVACRefrigerationChillerSet_Impl>()->resetAirOutletNodeName();
// }

/// @cond
ZoneHVACRefrigerationChillerSet::ZoneHVACRefrigerationChillerSet(boost::shared_ptr<detail::ZoneHVACRefrigerationChillerSet_Impl> impl)
  : ZoneHVACComponent(impl)
{}
/// @endcond

} // model
} // openstudio

