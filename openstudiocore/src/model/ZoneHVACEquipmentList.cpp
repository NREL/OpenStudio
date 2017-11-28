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

#include "ZoneHVACEquipmentList.hpp"
#include "ZoneHVACEquipmentList_Impl.hpp"
#include "ZoneHVACComponent.hpp"
#include "ZoneHVACComponent_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Model.hpp"
#include <utilities/idd/OS_ZoneHVAC_EquipmentList_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"
#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"

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

  for( const auto & equipment : equipmentVector )
  {
    WorkspaceExtensibleGroup eg = getGroupForModelObject(equipment);

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

  for( const auto & equipment : equipmentVector )
  {
    WorkspaceExtensibleGroup eg = getGroupForModelObject(equipment);

    eg.setUnsigned(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentHeatingorNoLoadSequence,newPriority);

    newPriority++;
  }
}

WorkspaceExtensibleGroup ZoneHVACEquipmentList_Impl::getGroupForModelObject(const ModelObject & modelObject)
{
  boost::optional<WorkspaceExtensibleGroup> result;

  std::vector<IdfExtensibleGroup> groups = extensibleGroups();

  for( const auto & group : groups )
  {
    boost::optional<WorkspaceObject> wo = group.cast<WorkspaceExtensibleGroup>().getTarget(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipment);

    OS_ASSERT(wo);

    if( wo->handle() == modelObject.handle() )
    {
      result = group.cast<WorkspaceExtensibleGroup>();

      break;
    }
  }

  OS_ASSERT(result);

  return result.get();
}

std::vector<ModelObject> ZoneHVACEquipmentList_Impl::equipment() const
{
  std::vector<ModelObject> result;

  std::vector<IdfExtensibleGroup> groups = extensibleGroups();

  for( const auto & group : groups )
  {
    boost::optional<WorkspaceObject> wo = group.cast<WorkspaceExtensibleGroup>().getTarget(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipment);

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

  for( const auto & group : groups )
  {
    unsigned heatingPriority = group.getUnsigned(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentHeatingorNoLoadSequence).get();

    boost::optional<WorkspaceObject> wo = group.cast<WorkspaceExtensibleGroup>().getTarget(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipment);

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

  for( const auto & group : groups )
  {
    unsigned coolingPriority = group.getUnsigned(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentCoolingSequence).get();

    boost::optional<WorkspaceObject> wo = group.cast<WorkspaceExtensibleGroup>().getTarget(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipment);

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

  for( const auto & group : groups )
  {
    boost::optional<WorkspaceObject> wo = group.cast<WorkspaceExtensibleGroup>().getTarget(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipment);

    OS_ASSERT(wo);

    if( wo->handle() == equipment.handle() )
    {
      getObject<ModelObject>().eraseExtensibleGroup(group.groupIndex());

      break;
    }
  }

  coolingVector.erase(std::find(coolingVector.begin(),coolingVector.end(),equipment));
  heatingVector.erase(std::find(heatingVector.begin(),heatingVector.end(),equipment));

  unsigned priority = 1;

  for( const auto & elem : coolingVector )
  {
    WorkspaceExtensibleGroup eg = getGroupForModelObject(elem);

    eg.setUnsigned(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentCoolingSequence,priority);

    priority++;
  }

  priority = 1;

  for( const auto & elem : heatingVector )
  {
    WorkspaceExtensibleGroup eg = getGroupForModelObject(elem);

    eg.setUnsigned(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentHeatingorNoLoadSequence,priority);

    priority++;
  }

}

unsigned ZoneHVACEquipmentList_Impl::heatingPriority(const ModelObject & equipment)
{
  boost::optional<unsigned> result;

  std::vector<IdfExtensibleGroup> groups = extensibleGroups();

  for( const auto & group : groups )
  {
    boost::optional<WorkspaceObject> wo = group.cast<WorkspaceExtensibleGroup>().getTarget(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipment);

    OS_ASSERT(wo);

    if( wo->handle() == equipment.handle() )
    {
      result = group.getUnsigned(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentHeatingorNoLoadSequence);

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

  for( const auto & group : groups )
  {
    boost::optional<WorkspaceObject> wo = group.cast<WorkspaceExtensibleGroup>().getTarget(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipment);

    OS_ASSERT(wo);

    if( wo->handle() == equipment.handle() )
    {
      result = group.getUnsigned(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentCoolingSequence);

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

std::vector<ModelObject> ZoneHVACEquipmentList::equipment() const
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
ZoneHVACEquipmentList::ZoneHVACEquipmentList(std::shared_ptr<detail::ZoneHVACEquipmentList_Impl> impl)
  : ModelObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

