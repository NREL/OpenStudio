/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#include <model/ZoneHVACEquipmentList.hpp>
#include <model/ZoneHVACEquipmentList_Impl.hpp>
#include <model/ZoneHVACComponent.hpp>
#include <model/ZoneHVACComponent_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/Model.hpp>
#include <utilities/idd/OS_ZoneHVAC_EquipmentList_FieldEnums.hxx>
#include <utilities/core/Assert.hpp>
#include <utilities/idf/WorkspaceExtensibleGroup.hpp>

namespace openstudio {
namespace model {

namespace detail {

ZoneHVACEquipmentList_Impl::ZoneHVACEquipmentList_Impl(const IdfObject& idfObject,
                                                       Model_Impl* model,
                                                       bool keepHandle)
  : ModelObject_Impl(idfObject,model,keepHandle)
{
  OS_ASSERT(idfObject.iddObject().type() == ZoneHVACEquipmentList::iddObjectType());
}

ZoneHVACEquipmentList_Impl::ZoneHVACEquipmentList_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                       Model_Impl* model,
                                                       bool keepHandle)
  : ModelObject_Impl(other,model,keepHandle)
{
  OS_ASSERT(other.iddObject().type() == ZoneHVACEquipmentList::iddObjectType());
}

ZoneHVACEquipmentList_Impl::ZoneHVACEquipmentList_Impl(const ZoneHVACEquipmentList_Impl& other,
                                                       Model_Impl* model,
                                                       bool keepHandle)
  : ModelObject_Impl(other,model,keepHandle)
{}

const std::vector<std::string>& ZoneHVACEquipmentList_Impl::outputVariableNames() const
{
  static std::vector<std::string> result;
  if (result.empty()){
  }
  return result;
}

IddObjectType ZoneHVACEquipmentList_Impl::iddObjectType() const {
  return ZoneHVACEquipmentList::iddObjectType();
}

void ZoneHVACEquipmentList_Impl::addEquipment(const ModelObject & equipment)
{
  unsigned count = this->equipment().size();

  WorkspaceExtensibleGroup eg = getObject<ModelObject>().pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();

  bool ok = eg.setPointer(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipment,equipment.handle());

  if( ok )
  {
    eg.setUnsigned(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentCoolingSequence,count + 1);
    eg.setUnsigned(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentHeatingorNoLoadSequence,count + 1);
  }
  else
  {
    getObject<ModelObject>().eraseExtensibleGroup(eg.groupIndex());
  }
}

void ZoneHVACEquipmentList_Impl::setCoolingPriority(const ModelObject & equipment, unsigned priority)
{
  std::vector<ModelObject> equipmentVector = equipmentInCoolingOrder();

  OS_ASSERT( std::find(equipmentVector.begin(),equipmentVector.end(),equipment) != equipmentVector.end() );

  if( priority > equipmentVector.size() ) priority = equipmentVector.size();
  if( priority < 1 ) priority = 1;

  equipmentVector.erase(std::find(equipmentVector.begin(),equipmentVector.end(),equipment));

  equipmentVector.insert(equipmentVector.begin() + (priority - 1),equipment);

  unsigned newPriority = 1;

  for( std::vector<ModelObject>::iterator it = equipmentVector.begin();
       it != equipmentVector.end();
       ++it )
  {
    WorkspaceExtensibleGroup eg = getGroupForModelObject(*it);

    eg.setUnsigned(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentCoolingSequence,newPriority);

    newPriority++;
  }
}

void ZoneHVACEquipmentList_Impl::setHeatingPriority(const ModelObject & equipment, unsigned priority)
{
  std::vector<ModelObject> equipmentVector = equipmentInHeatingOrder();

  OS_ASSERT( std::find(equipmentVector.begin(),equipmentVector.end(),equipment) != equipmentVector.end() );

  if( priority > equipmentVector.size() ) priority = equipmentVector.size();
  if( priority < 1 ) priority = 1;

  equipmentVector.erase(std::find(equipmentVector.begin(),equipmentVector.end(),equipment));

  equipmentVector.insert(equipmentVector.begin() + (priority - 1),equipment);

  unsigned newPriority = 1;

  for( std::vector<ModelObject>::iterator it = equipmentVector.begin();
       it != equipmentVector.end();
       ++it )
  {
    WorkspaceExtensibleGroup eg = getGroupForModelObject(*it);

    eg.setUnsigned(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentHeatingorNoLoadSequence,newPriority);

    newPriority++;
  }
}

WorkspaceExtensibleGroup ZoneHVACEquipmentList_Impl::getGroupForModelObject(const ModelObject & modelObject)
{
  boost::optional<WorkspaceExtensibleGroup> result;

  std::vector<IdfExtensibleGroup> groups = extensibleGroups();

  for( std::vector<IdfExtensibleGroup>::iterator it = groups.begin();
       it != groups.end();
       ++it )
  {
    boost::optional<WorkspaceObject> wo = it->cast<WorkspaceExtensibleGroup>().getTarget(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipment);

    OS_ASSERT(wo);

    if( wo->handle() == modelObject.handle() )
    {
      result = it->cast<WorkspaceExtensibleGroup>();

      break;
    }
  }

  OS_ASSERT(result);

  return result.get();
}

std::vector<ModelObject> ZoneHVACEquipmentList_Impl::equipment()
{
  std::vector<ModelObject> result;

  std::vector<IdfExtensibleGroup> groups = extensibleGroups();

  for( std::vector<IdfExtensibleGroup>::iterator it = groups.begin();
       it != groups.end();
       ++it )
  {
    boost::optional<WorkspaceObject> wo = it->cast<WorkspaceExtensibleGroup>().getTarget(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipment);

    if( wo )
    {
      result.push_back(wo->cast<ModelObject>());
    }
  }

  return result;
}

std::vector<ModelObject> ZoneHVACEquipmentList_Impl::equipmentInHeatingOrder()
{
  std::map<unsigned,ModelObject> equipmentMap;

  std::vector<IdfExtensibleGroup> groups = extensibleGroups();

  for( std::vector<IdfExtensibleGroup>::iterator it = groups.begin();
       it != groups.end();
       ++it )
  {
    unsigned heatingPriority = it->getUnsigned(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentHeatingorNoLoadSequence).get();

    boost::optional<WorkspaceObject> wo = it->cast<WorkspaceExtensibleGroup>().getTarget(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipment);

    OS_ASSERT(wo);

    ModelObject mo = wo->cast<ModelObject>();

    equipmentMap.insert(std::make_pair(heatingPriority,mo));
  }

  std::vector<ModelObject> equipmentVector;

  for( unsigned i = 1; i <= equipmentMap.size(); i++ )
  {
    ModelObject mo = equipmentMap.find(i)->second;

    equipmentVector.push_back(mo);
  }

  return equipmentVector;
}

std::vector<ModelObject> ZoneHVACEquipmentList_Impl::equipmentInCoolingOrder()
{
  std::map<unsigned,ModelObject> equipmentMap;

  std::vector<IdfExtensibleGroup> groups = extensibleGroups();

  for( std::vector<IdfExtensibleGroup>::iterator it = groups.begin();
       it != groups.end();
       ++it )
  {
    unsigned coolingPriority = it->getUnsigned(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentCoolingSequence).get();

    boost::optional<WorkspaceObject> wo = it->cast<WorkspaceExtensibleGroup>().getTarget(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipment);

    OS_ASSERT(wo);

    ModelObject mo = wo->cast<ModelObject>();

    equipmentMap.insert(std::make_pair(coolingPriority,mo));
  }

  std::vector<ModelObject> equipmentVector;

  for( unsigned i = 1; i <= equipmentMap.size(); i++ )
  {
    ModelObject mo = equipmentMap.find(i)->second;

    equipmentVector.push_back(mo);
  }

  return equipmentVector;
}

ThermalZone ZoneHVACEquipmentList_Impl::thermalZone() const
{
  boost::optional<WorkspaceObject> wo = getTarget(OS_ZoneHVAC_EquipmentListFields::ThermalZone);

  OS_ASSERT(wo);

  return wo->cast<ThermalZone>();
}

void ZoneHVACEquipmentList_Impl::removeEquipment(const ModelObject & equipment)
{
  std::vector<ModelObject> coolingVector = equipmentInCoolingOrder();
  std::vector<ModelObject> heatingVector = equipmentInHeatingOrder();

  std::vector<IdfExtensibleGroup> groups = extensibleGroups();

  for( std::vector<IdfExtensibleGroup>::iterator it = groups.begin();
       it != groups.end();
       ++it )
  {
    boost::optional<WorkspaceObject> wo = it->cast<WorkspaceExtensibleGroup>().getTarget(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipment);

    OS_ASSERT(wo);

    if( wo->handle() == equipment.handle() )
    {
      getObject<ModelObject>().eraseExtensibleGroup(it->groupIndex());

      break;
    }
  }

  coolingVector.erase(std::find(coolingVector.begin(),coolingVector.end(),equipment));
  heatingVector.erase(std::find(heatingVector.begin(),heatingVector.end(),equipment));

  unsigned priority = 1;

  for( std::vector<ModelObject>::iterator it = coolingVector.begin();
       it != coolingVector.end();
       ++it )
  {
    WorkspaceExtensibleGroup eg = getGroupForModelObject(*it);

    eg.setUnsigned(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentCoolingSequence,priority);

    priority++;
  }

  priority = 1;

  for( std::vector<ModelObject>::iterator it = heatingVector.begin();
       it != heatingVector.end();
       ++it )
  {
    WorkspaceExtensibleGroup eg = getGroupForModelObject(*it);

    eg.setUnsigned(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentHeatingorNoLoadSequence,priority);

    priority++;
  }

}

unsigned ZoneHVACEquipmentList_Impl::heatingPriority(const ModelObject & equipment)
{
  boost::optional<unsigned> result;

  std::vector<IdfExtensibleGroup> groups = extensibleGroups();

  for( std::vector<IdfExtensibleGroup>::iterator it = groups.begin();
       it != groups.end();
       ++it )
  {
    boost::optional<WorkspaceObject> wo = it->cast<WorkspaceExtensibleGroup>().getTarget(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipment);

    OS_ASSERT(wo);

    if( wo->handle() == equipment.handle() )
    {
      result = it->getUnsigned(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentHeatingorNoLoadSequence);

      break;
    }
  }

  OS_ASSERT(result);

  return result.get();
}

unsigned ZoneHVACEquipmentList_Impl::coolingPriority(const ModelObject & equipment)
{
  boost::optional<unsigned> result;

  std::vector<IdfExtensibleGroup> groups = extensibleGroups();

  for( std::vector<IdfExtensibleGroup>::iterator it = groups.begin();
       it != groups.end();
       ++it )
  {
    boost::optional<WorkspaceObject> wo = it->cast<WorkspaceExtensibleGroup>().getTarget(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipment);

    OS_ASSERT(wo);

    if( wo->handle() == equipment.handle() )
    {
      result = it->getUnsigned(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentCoolingSequence);

      break;
    }
  }

  OS_ASSERT(result);

  return result.get();
}

} // detail

ZoneHVACEquipmentList::ZoneHVACEquipmentList(const ThermalZone & thermalZone)
  : ModelObject(ZoneHVACEquipmentList::iddObjectType(),thermalZone.model())
{
  OS_ASSERT(getImpl<detail::ZoneHVACEquipmentList_Impl>());

  setPointer(OS_ZoneHVAC_EquipmentListFields::ThermalZone,thermalZone.handle());
}

IddObjectType ZoneHVACEquipmentList::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ZoneHVAC_EquipmentList);
}

void ZoneHVACEquipmentList::addEquipment(const ModelObject & equipment)
{
  getImpl<detail::ZoneHVACEquipmentList_Impl>()->addEquipment(equipment);
}

void ZoneHVACEquipmentList::setCoolingPriority(const ModelObject & equipment, unsigned priority)
{
  getImpl<detail::ZoneHVACEquipmentList_Impl>()->setCoolingPriority(equipment,priority);
}

void ZoneHVACEquipmentList::setHeatingPriority(const ModelObject & equipment, unsigned priority)
{
  getImpl<detail::ZoneHVACEquipmentList_Impl>()->setHeatingPriority(equipment,priority);
}

std::vector<ModelObject> ZoneHVACEquipmentList::equipment()
{
  return getImpl<detail::ZoneHVACEquipmentList_Impl>()->equipment();
}

std::vector<ModelObject> ZoneHVACEquipmentList::equipmentInHeatingOrder()
{
  return getImpl<detail::ZoneHVACEquipmentList_Impl>()->equipmentInHeatingOrder();
}

std::vector<ModelObject> ZoneHVACEquipmentList::equipmentInCoolingOrder()
{
  return getImpl<detail::ZoneHVACEquipmentList_Impl>()->equipmentInCoolingOrder();
}

ThermalZone ZoneHVACEquipmentList::thermalZone() const
{
  return getImpl<detail::ZoneHVACEquipmentList_Impl>()->thermalZone();
}

void ZoneHVACEquipmentList::removeEquipment(const ModelObject & equipment)
{
  getImpl<detail::ZoneHVACEquipmentList_Impl>()->removeEquipment(equipment);
}

unsigned ZoneHVACEquipmentList::heatingPriority(const ModelObject & equipment)
{
  return getImpl<detail::ZoneHVACEquipmentList_Impl>()->heatingPriority(equipment);
}

unsigned ZoneHVACEquipmentList::coolingPriority(const ModelObject & equipment)
{
  return getImpl<detail::ZoneHVACEquipmentList_Impl>()->coolingPriority(equipment);
}

/// @cond
ZoneHVACEquipmentList::ZoneHVACEquipmentList(boost::shared_ptr<detail::ZoneHVACEquipmentList_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

