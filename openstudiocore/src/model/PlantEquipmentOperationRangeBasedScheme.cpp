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

#include "PlantEquipmentOperationRangeBasedScheme.hpp"
#include "PlantEquipmentOperationRangeBasedScheme_Impl.hpp"
#include "PlantEquipmentOperationScheme.hpp"
#include "PlantEquipmentOperationScheme_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "HVACComponent.hpp"
#include "HVACComponent_Impl.hpp"
#include "ModelObjectList.hpp"
#include "ModelObjectList_Impl.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/idf/IdfExtensibleGroup.hpp"
#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"
#include "../utilities/math/FloatCompare.hpp"

namespace openstudio {

namespace model {

// These are the same for heating and cooling load range schemes
enum LoadExtensibleFields { LOADRANGEFIELDS_LOWERLIMIT, LOADRANGEFIELDS_UPPERLIMIT, LOADRANGEFIELDS_RANGEEQUIPMENTLISTNAME };

namespace detail {

PlantEquipmentOperationRangeBasedScheme_Impl::PlantEquipmentOperationRangeBasedScheme_Impl(IddObjectType type, Model_Impl* model)
  : PlantEquipmentOperationScheme_Impl(type,model)
{
}

PlantEquipmentOperationRangeBasedScheme_Impl::PlantEquipmentOperationRangeBasedScheme_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
  : PlantEquipmentOperationScheme_Impl(idfObject, model, keepHandle)
{ 
}

PlantEquipmentOperationRangeBasedScheme_Impl::PlantEquipmentOperationRangeBasedScheme_Impl(
    const openstudio::detail::WorkspaceObject_Impl& other, 
    Model_Impl* model, 
    bool keepHandle)
  : PlantEquipmentOperationScheme_Impl(other,model,keepHandle)
{
}

bool PlantEquipmentOperationRangeBasedScheme_Impl::addLoadRange(double upperLimit, const std::vector<HVACComponent> & equipment)
{
  if( upperLimit < minimumLowerLimit() ) {
    return false;
  }

  unsigned i = 0;
  for( auto & eg : extensibleGroups() ) {
    const auto & t_upperLimit = eg.getDouble(LOADRANGEFIELDS_UPPERLIMIT);
    OS_ASSERT(t_upperLimit);
    const auto & t_lowerLimit = eg.getDouble(LOADRANGEFIELDS_LOWERLIMIT);
    OS_ASSERT(t_lowerLimit);
    if( upperLimit < t_upperLimit ) {
      eg.setDouble(LOADRANGEFIELDS_LOWERLIMIT,upperLimit);
      // Create new eg and insert
      auto newEg = getObject<IdfObject>().insertExtensibleGroup(i).cast<WorkspaceExtensibleGroup>();
      auto m = model();
      ModelObjectList modelObjectList(m);

      // Remove duplicates
      //auto equipmentCopy = equipment;
      //std::sort( equipmentCopy.begin(), equipmentCopy.end() );
      //equipmentCopy.erase( unique( equipmentCopy.begin(), equipmentCopy.end() ), equipmentCopy.end() );
      
      // Remove duplicates primitively so that the order is preserved
      std::vector<HVACComponent> equipmentCopy;
      for( const auto & mo : equipment ) {
        if( std::find(equipmentCopy.begin(),equipmentCopy.end(),mo) == equipmentCopy.end() ) {
          equipmentCopy.push_back(mo);
        }
      }
      for( const auto & mo : equipmentCopy ) {
        modelObjectList.addModelObject(mo);
      }

      newEg.setPointer(LOADRANGEFIELDS_RANGEEQUIPMENTLISTNAME,modelObjectList.handle());
      newEg.setDouble(LOADRANGEFIELDS_LOWERLIMIT,t_lowerLimit.get());
      newEg.setDouble(LOADRANGEFIELDS_UPPERLIMIT,upperLimit);
      return true;
    }
    ++i;
  }

  return false;
}

std::vector<HVACComponent> PlantEquipmentOperationRangeBasedScheme_Impl::removeLoadRange(double upperLimit)
{
  auto t_extensibleGroups = extensibleGroups();
  if( t_extensibleGroups.size() == 1u ) { return std::vector<HVACComponent>(); }
  if( upperLimit >= maximumUpperLimit() ) { return std::vector<HVACComponent>(); }

  for( auto eg = t_extensibleGroups.begin(); eg != t_extensibleGroups.end(); ++eg ) {
    const auto & t_upperLimit = eg->getDouble(LOADRANGEFIELDS_UPPERLIMIT);
    OS_ASSERT(t_upperLimit);
    if( equal(upperLimit,t_upperLimit.get()) ) {
      const auto & t_lowerLimit = eg->getDouble(LOADRANGEFIELDS_LOWERLIMIT);
      OS_ASSERT(t_lowerLimit);
      auto nextEg = (eg + 1);
      OS_ASSERT(nextEg != t_extensibleGroups.end()); 
      nextEg->setDouble(LOADRANGEFIELDS_LOWERLIMIT,t_lowerLimit.get());

      const auto & wo = eg->cast<WorkspaceExtensibleGroup>().getTarget(LOADRANGEFIELDS_RANGEEQUIPMENTLISTNAME);
      OS_ASSERT(wo);
      const auto & modelObjectList = wo->optionalCast<ModelObjectList>();
      OS_ASSERT(modelObjectList);
      auto result = subsetCastVector<HVACComponent>(modelObjectList->modelObjects());

      eraseExtensibleGroup(eg->groupIndex());

      return result;
    }
  }

  return std::vector<HVACComponent>();
}

std::vector<double> PlantEquipmentOperationRangeBasedScheme_Impl::loadRangeUpperLimits() const
{
  std::vector<double> result;
  
  for( const auto & eg : extensibleGroups() ) {
    const auto & value = eg.getDouble(LOADRANGEFIELDS_UPPERLIMIT);
    OS_ASSERT(value);
    result.push_back(value.get());
  }

  return result;
}

std::vector<HVACComponent> PlantEquipmentOperationRangeBasedScheme_Impl::equipment(double upperLimit) const
{
  for( const auto & eg : extensibleGroups() ) {
    const auto & value = eg.getDouble(LOADRANGEFIELDS_UPPERLIMIT);
    OS_ASSERT(value);
    if( equal(value.get(),upperLimit) ) {
      const auto & wo = eg.cast<WorkspaceExtensibleGroup>().getTarget(LOADRANGEFIELDS_RANGEEQUIPMENTLISTNAME);
      OS_ASSERT(wo);
      const auto & modelObjectList = wo->optionalCast<ModelObjectList>();
      OS_ASSERT(modelObjectList);
      return subsetCastVector<HVACComponent>(modelObjectList->modelObjects());
    }
  }

  return std::vector<HVACComponent>();
}

bool PlantEquipmentOperationRangeBasedScheme_Impl::addEquipment(double upperLimit, const HVACComponent & equipment)
{
  for( const auto & eg : extensibleGroups() ) {
    const auto & value = eg.getDouble(LOADRANGEFIELDS_UPPERLIMIT);
    OS_ASSERT(value);
    if( equal(value.get(),upperLimit) ) {
      const auto & wo = eg.cast<WorkspaceExtensibleGroup>().getTarget(2);
      OS_ASSERT(wo);
      auto modelObjectList = wo->optionalCast<ModelObjectList>();
      OS_ASSERT(modelObjectList);
      auto currentEquipment = modelObjectList->modelObjects();
      if( std::find(currentEquipment.begin(),currentEquipment.end(),equipment) == currentEquipment.end() ) {
        return modelObjectList->addModelObject(equipment);
      }
    }
  }

  return false;
}

bool PlantEquipmentOperationRangeBasedScheme_Impl::addEquipment(const HVACComponent & equipment)
{
  const auto & egs = extensibleGroups();

  OS_ASSERT(! egs.empty());

  const auto & eg = egs.back();
  const auto & wo = eg.cast<WorkspaceExtensibleGroup>().getTarget(2);
  OS_ASSERT(wo);
  auto modelObjectList = wo->optionalCast<ModelObjectList>();
  OS_ASSERT(modelObjectList);
  auto currentEquipment = modelObjectList->modelObjects();
  if( std::find(currentEquipment.begin(),currentEquipment.end(),equipment) == currentEquipment.end() ) {
    return modelObjectList->addModelObject(equipment);
  }

  return false;
}

bool PlantEquipmentOperationRangeBasedScheme_Impl::removeEquipment(double upperLimit, const HVACComponent & equipment)
{
  for( const auto & eg : extensibleGroups() ) {
    const auto & value = eg.getDouble(LOADRANGEFIELDS_UPPERLIMIT);
    OS_ASSERT(value);
    if( equal(value.get(),upperLimit) ) {
      const auto & wo = eg.cast<WorkspaceExtensibleGroup>().getTarget(LOADRANGEFIELDS_RANGEEQUIPMENTLISTNAME);
      OS_ASSERT(wo);
      auto modelObjectList = wo->optionalCast<ModelObjectList>();
      OS_ASSERT(modelObjectList);
      const auto & modelObjects = modelObjectList->modelObjects();
      if( std::find(modelObjects.begin(),modelObjects.end(),equipment.cast<ModelObject>()) != modelObjects.end() ) {
        modelObjectList->removeModelObject(equipment);
        return true;
      } else {
        return false;
      }
    }
  }

  return false;
}

void PlantEquipmentOperationRangeBasedScheme_Impl::clearLoadRanges()
{
}

PlantEquipmentOperationRangeBasedScheme_Impl::PlantEquipmentOperationRangeBasedScheme_Impl(const PlantEquipmentOperationRangeBasedScheme_Impl& other, 
                                               Model_Impl* model, 
                                               bool keepHandles)
  : PlantEquipmentOperationScheme_Impl(other,model,keepHandles)
{
}

} // detail

PlantEquipmentOperationRangeBasedScheme::PlantEquipmentOperationRangeBasedScheme(IddObjectType type,const Model& model)
  : PlantEquipmentOperationScheme(type,model)
{
  OS_ASSERT(getImpl<detail::PlantEquipmentOperationRangeBasedScheme_Impl>());

  auto eg = pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();
  ModelObjectList modelObjectList(model);
  eg.setPointer(LOADRANGEFIELDS_RANGEEQUIPMENTLISTNAME,modelObjectList.handle());
  eg.setDouble(LOADRANGEFIELDS_LOWERLIMIT,getImpl<detail::PlantEquipmentOperationRangeBasedScheme_Impl>()->minimumLowerLimit());
  eg.setDouble(LOADRANGEFIELDS_UPPERLIMIT,getImpl<detail::PlantEquipmentOperationRangeBasedScheme_Impl>()->maximumUpperLimit());
}     

PlantEquipmentOperationRangeBasedScheme::PlantEquipmentOperationRangeBasedScheme(std::shared_ptr<detail::PlantEquipmentOperationRangeBasedScheme_Impl> p)
  : PlantEquipmentOperationScheme(p)
{}

bool PlantEquipmentOperationRangeBasedScheme::addLoadRange(double upperLimit, const std::vector<HVACComponent> & equipment)
{
  return getImpl<detail::PlantEquipmentOperationRangeBasedScheme_Impl>()->addLoadRange(upperLimit,equipment);
}

std::vector<HVACComponent> PlantEquipmentOperationRangeBasedScheme::removeLoadRange(double upperLimit)
{
  return getImpl<detail::PlantEquipmentOperationRangeBasedScheme_Impl>()->removeLoadRange(upperLimit);
}

std::vector<double> PlantEquipmentOperationRangeBasedScheme::loadRangeUpperLimits() const
{
  return getImpl<detail::PlantEquipmentOperationRangeBasedScheme_Impl>()->loadRangeUpperLimits();
}

std::vector<HVACComponent> PlantEquipmentOperationRangeBasedScheme::equipment(double upperLimit) const
{
  return getImpl<detail::PlantEquipmentOperationRangeBasedScheme_Impl>()->equipment(upperLimit);
}

bool PlantEquipmentOperationRangeBasedScheme::addEquipment(double upperLimit, const HVACComponent & equipment)
{
  return getImpl<detail::PlantEquipmentOperationRangeBasedScheme_Impl>()->addEquipment(upperLimit,equipment);
}

bool PlantEquipmentOperationRangeBasedScheme::addEquipment(const HVACComponent & equipment)
{
  return getImpl<detail::PlantEquipmentOperationRangeBasedScheme_Impl>()->addEquipment(equipment);
}

bool PlantEquipmentOperationRangeBasedScheme::removeEquipment(double upperLimit, const HVACComponent & equipment)
{
  return getImpl<detail::PlantEquipmentOperationRangeBasedScheme_Impl>()->removeEquipment(upperLimit,equipment);
}

void PlantEquipmentOperationRangeBasedScheme::clearLoadRanges()
{
  return getImpl<detail::PlantEquipmentOperationRangeBasedScheme_Impl>()->clearLoadRanges();
}

double PlantEquipmentOperationRangeBasedScheme::maximumUpperLimit() const
{
  return getImpl<detail::PlantEquipmentOperationRangeBasedScheme_Impl>()->maximumUpperLimit();
}

double PlantEquipmentOperationRangeBasedScheme::minimumLowerLimit() const
{
  return getImpl<detail::PlantEquipmentOperationRangeBasedScheme_Impl>()->minimumLowerLimit();
}

} // model

} // openstudio

